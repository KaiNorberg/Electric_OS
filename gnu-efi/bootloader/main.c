#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define PSF_MAGIC0 0x36
#define PSF_MAGIC1 0x04

typedef unsigned long long size_t;

typedef struct
{
	unsigned int* Base;
	size_t Size;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
} Framebuffer;

typedef struct
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF_HEADER;

typedef struct
{
	PSF_HEADER* PSF_Header;
	void* glyphBuffer;
} PSF_FONT;

typedef struct
{
	EFI_MEMORY_DESCRIPTOR* Base;
	uint64_t Size;
	uint64_t DescSize;
	uint64_t Key;
} EFI_MEMORY_MAP;

typedef struct
{
	Framebuffer* framebuffer;
	PSF_FONT* PSF_Font;
	EFI_MEMORY_MAP* MemoryMap;
	void* RSDP;
} BootInfo;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;

int strcmp(const char* Str1, const char* Str2)
{
	int i = 0;
	while (Str1[i] != 0 && Str2[i] != 0)
	{
		if (Str1[i] != Str2[i])
		{
			return 0;
		}
		i++;
	}

	return (i != 0);
}

int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++){
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path)
{
	Print(L"Loading File (%s)...\n\r", Path);

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

Framebuffer GetFramebuffer()
{
	EFI_GUID GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
	EFI_STATUS Status = uefi_call_wrapper(BS->LocateProtocol, 3, &GOP_GUID, NULL, (void**)&GOP);

	Print(L"Initializing GOP..\n\r");
	if (EFI_ERROR(Status))
	{
		Print(L"ERROR: GOP Failed!\n\r");
		
		while (1)
		{
			__asm__("HLT");
		}
	}

	Framebuffer NewBuffer;

	NewBuffer.Base = (unsigned int*)GOP->Mode->FrameBufferBase;
	NewBuffer.Size = GOP->Mode->FrameBufferSize;
	NewBuffer.Width = GOP->Mode->Info->HorizontalResolution;
	NewBuffer.Height = GOP->Mode->Info->VerticalResolution;
	NewBuffer.PixelsPerScanline = GOP->Mode->Info->PixelsPerScanLine;

	Print(L"GOP BUFFER INFO\n\r");
	Print(L"Base: 0x%x\n\r", NewBuffer.Base);
	Print(L"Size: 0x%x\n\r", NewBuffer.Size);
	Print(L"Width: %d\n\r", NewBuffer.Width);
	Print(L"Height: %d\n\r", NewBuffer.Height);
	Print(L"PixelsPerScanline: %d\n\r", NewBuffer.PixelsPerScanline);
	Print(L"GOP BUFFER INFO END\n\r");

	return NewBuffer;
}

PSF_FONT LoadPSFFont(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* Font = LoadFile(Directory, Path);

	if (Font == NULL)
	{
		Print(L"ERROR: Failed to load font!\n\r");
		
		while (1)
		{
			__asm__("HLT");
		}
	}

	PSF_HEADER* FontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF_HEADER), (void**)&FontHeader);
	UINTN size = sizeof(PSF_HEADER);
	Font->Read(Font, &size, FontHeader);

	if (FontHeader->magic[0] != PSF_MAGIC0 || FontHeader->magic[1] != PSF_MAGIC1)
	{
		Print(L"ERROR: Invalid font loaded!\n\r");
		
		while (1)
		{
			__asm__("HLT");
		}
	}

	UINTN GlyphBufferSize = FontHeader->charsize * 256;
	if (FontHeader->mode == 1)
	{
		GlyphBufferSize = FontHeader->charsize * 512;
	}

	void* GlyphBuffer;
	{
		Font->SetPosition(Font, sizeof(PSF_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, GlyphBufferSize, (void**)&GlyphBuffer);
		Font->Read(Font, &GlyphBufferSize, GlyphBuffer);
	}

	PSF_FONT newFont;

	newFont.PSF_Header = FontHeader;
	newFont.glyphBuffer = (char*)GlyphBuffer;

	Print(L"FONT INFO\n\r");
	Print(L"Char Size: %d\n\r", newFont.PSF_Header->charsize);
	Print(L"Mode: 0x%x\n\r", newFont.PSF_Header->mode);
	Print(L"FONT INFO END\n\r");

	return newFont;
}

EFI_MEMORY_MAP GetMemoryMap()
{
	Print(L"Retrieving EFI Memory Map...\n\r");

	EFI_MEMORY_DESCRIPTOR* Base = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{

		SystemTable->BootServices->GetMemoryMap(&MapSize, Base, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Base);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Base, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

	EFI_MEMORY_MAP NewMap;
	NewMap.Base = Base;
	NewMap.Size = MapSize;
	NewMap.DescSize = DescriptorSize;
	NewMap.Key = MapKey;

	return NewMap;
}

Elf64_Ehdr LoadELFFile(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* File = LoadFile(Directory, Path);
	if (File == NULL)
	{
		Print(L"ERROR: Failed to load %s\n\r", Path);
				
		while (1)
		{
			__asm__("HLT");
		}
	}

	Print(L"Reading ELF File...\n\r");

	Elf64_Ehdr Header;
	UINTN FileInfoSize;
	EFI_FILE_INFO* FileInfo;
	File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, NULL);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
	File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);
	UINTN Size = sizeof(Header);
	File->Read(File, &Size, &Header);

	if (memcmp(&Header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		Header.e_ident[EI_CLASS] != ELFCLASS64 ||
		Header.e_ident[EI_DATA] != ELFDATA2LSB ||
		Header.e_machine != EM_X86_64 ||
		Header.e_version != EV_CURRENT)
	{
		Print(L"ERROR: %s is corrupted!", Path);
	}

	Elf64_Phdr* phdrs;
	{
		File->SetPosition(File, Header.e_phoff);

		UINTN size = Header.e_phnum * Header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		File->Read(File, &size, phdrs);
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

			File->SetPosition(File, phdr->p_offset);
			UINTN size = phdr->p_filesz;
			File->Read(File, &size, (void*)segment);
		}
		break;
		}
	}

	return Header;
}

void* GetRSDP()
{	
	Print(L"Getting RSDP...\n\r");

	EFI_CONFIGURATION_TABLE* ConfigTable = SystemTable->ConfigurationTable;
	void* RSDP = 0;
	EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;

	for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++)
	{
		if (CompareGuid(&ConfigTable[i].VendorGuid, &Acpi2TableGuid) && strcmp("RSD PTR ", ConfigTable->VendorTable))
		{
			RSDP = ConfigTable->VendorTable;
		}
		ConfigTable++;
	}

	return RSDP;
}

EFI_STATUS efi_main(EFI_HANDLE In_ImageHandle, EFI_SYSTEM_TABLE* In_SystemTable)
{
	ImageHandle = In_ImageHandle;
	SystemTable = In_SystemTable;

	InitializeLib(ImageHandle, SystemTable);
	Print(L"Bootloader loaded!\n\r");


	EFI_FILE* KernelDir = LoadFile(NULL, L"KERNEL");
	Elf64_Ehdr Header = LoadELFFile(KernelDir, L"Kernel.elf");
	PSF_FONT newFont = LoadPSFFont(KernelDir, L"zap-vga16.psf");
	Framebuffer newBuffer = GetFramebuffer();
	EFI_MEMORY_MAP newMap = GetMemoryMap();
	void* RSDP = GetRSDP();

	void (*KernelMain)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*)) Header.e_entry);

	BootInfo bootInfo;
	bootInfo.framebuffer = &newBuffer;
	bootInfo.PSF_Font = &newFont;
	bootInfo.MemoryMap = &newMap;
	bootInfo.RSDP = RSDP;

	Print(L"Exiting boot services...\n\r");
	SystemTable->BootServices->ExitBootServices(ImageHandle, newMap.Key);

	Print(L"Entering Kernel...\n\r");
	KernelMain(&bootInfo);

	return EFI_SUCCESS; // Exit the UEFI application
}
