#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if (s != EFI_SUCCESS)
	{
		return NULL;
	}

	return LoadedFile;
}

int memcmp(const void* Ptr1, const void* Ptr2, size_t Size)
{
	const unsigned char* P1 = Ptr1, *P2 = Ptr2;

	for (size_t i = 0; i < Size; i++)
	{
		if (P1[i] < P2[i])
		{
			return -1;
		}
		else if (P1[i] > P2[i])
		{
			return 1;
		}
	}

	return 0;
}

EFI_STATUS efi_main(EFI_HANDLE In_ImageHandle, EFI_SYSTEM_TABLE* In_SystemTable)
{
	ImageHandle = In_ImageHandle;
	SystemTable = In_SystemTable;

	InitializeLib(ImageHandle, SystemTable);

	Print(L"[..] Kernel loading\r");
	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf");
	if (Kernel == NULL)
	{
		Print(L"[ER] Kernel load   \n\r");
	}
	else
	{
		Print(L"[OK] Kernel load   \n\r");
	}

	Elf64_Ehdr Header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN Size = sizeof(Header);
		Kernel->Read(Kernel, &Size, &Header);
	}

	Print(L"[..] Checking kernel format\r");
	if (memcmp(&Header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		Header.e_ident[EI_CLASS] != ELFCLASS64 ||
		Header.e_ident[EI_DATA] != ELFDATA2LSB ||
		Header.e_machine != EM_X86_64 ||
		Header.e_version != EV_CURRENT)
	{
		Print(L"[ER] Kernal format         \n\r");
	}
	else
	{
		Print(L"[OK] Kernal format         \n\r");
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, Header.e_phoff);

		UINTN size = Header.e_phnum * Header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (Elf64_Phdr* phdr = phdrs; (char*)phdr < (char*)phdrs + Header.e_phnum * Header.e_phentsize; phdr = (Elf64_Phdr*)((char*)phdr + Header.e_phentsize))
	{
		switch (phdr->p_type)
		{
		case PT_LOAD:
		{
			int pages = (phdr->p_memsz * 0x1000 - 1) / 0x1000;
			Elf64_Addr segment = phdr->p_paddr;
			SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

			Kernel->SetPosition(Kernel, phdr->p_offset);
			UINTN size = phdr->p_filesz;
			Kernel->Read(Kernel, &size, (void*)segment);
		}
		break;
		}
	}

	Print(L"Entering kernel...\n\r");

	int (*KernelStart)() = ((__attribute__((sysv_abi)) int (*)()) Header.e_entry);

	Print(L"Kernel return: %d\n\r", KernelStart());

	return EFI_SUCCESS; // Exit the UEFI application
}
