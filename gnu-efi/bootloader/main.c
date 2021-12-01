#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

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
} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;
Framebuffer framebuffer;

int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++){
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

Framebuffer* InitializeGOP()
{
	EFI_GUID GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
	EFI_STATUS Status = uefi_call_wrapper(BS->LocateProtocol, 3, &GOP_GUID, NULL, (void**)&GOP);

	Print(L"[..] Initializing GOP\r");
	if (EFI_ERROR(Status))
	{
		Print(L"[ER] GOP Failed   		\n\r");
		return NULL;
	}
	else
	{
		Print(L"[OK] GOP initialized   \n\r");
	}

	framebuffer.Base = (unsigned int*)GOP->Mode->FrameBufferBase;
	framebuffer.Size = GOP->Mode->FrameBufferSize;
	framebuffer.Width = GOP->Mode->Info->HorizontalResolution;
	framebuffer.Height = GOP->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanline = GOP->Mode->Info->PixelsPerScanLine;

	Print(L"BUFFER INFO\n\r");
	Print(L"Base: 0x%x\n\r", framebuffer.Base);
	Print(L"Size: 0x%x\n\r", framebuffer.Size);
	Print(L"Width: %d\n\r", framebuffer.Width);
	Print(L"Height: %d\n\r", framebuffer.Height);
	Print(L"PixelsPerScanline: %d\n\r", framebuffer.PixelsPerScanline);
	Print(L"END");

	return &framebuffer;
}

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

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path)
{
	Print(L"[..] Loading font\r");

	EFI_FILE* Font = LoadFile(Directory, Path);

	if (Font == NULL)
	{
		Print(L"[ER] Font failed to load\n\r");
		return NULL;
	}

	PSF1_HEADER* FontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&FontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	Font->Read(Font, &size, FontHeader);

	if (FontHeader->magic[0] != PSF1_MAGIC0 || FontHeader->magic[1] != PSF1_MAGIC1)
	{
		Print(L"[ER] Invalid font loaded\n\r");
		return NULL;
	}

	UINTN GlyphBufferSize = FontHeader->charsize * 256;
	if (FontHeader->mode == 1)
	{
		GlyphBufferSize = FontHeader->charsize * 512;
	}

	void* GlyphBuffer;
	{
		Font->SetPosition(Font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, GlyphBufferSize, (void**)&GlyphBuffer);
		Font->Read(Font, &GlyphBufferSize, GlyphBuffer);
	}

	PSF1_FONT* FinishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&FinishedFont);

	FinishedFont->psf1_Header = FontHeader;
	FinishedFont->glyphBuffer = (char*)GlyphBuffer;

	Print(L"[OK] Font loaded   		\n\r");
	Print(L"FONT INFO\n\r");
	Print(L"Char size: %d\n\r", FinishedFont->psf1_Header->charsize);
	Print(L"END");

	return FinishedFont;
}

typedef struct
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	UINTN mMapSize;
	UINTN mMapDescSize;
} BootInfo;

EFI_STATUS efi_main(EFI_HANDLE In_ImageHandle, EFI_SYSTEM_TABLE* In_SystemTable)
{
	ImageHandle = In_ImageHandle;
	SystemTable = In_SystemTable;

	InitializeLib(ImageHandle, SystemTable);

	//Load kernel
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

	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"zap-light16.psf");
	Framebuffer* newBuffer = InitializeGOP();

	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{

		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);

	}

	void (*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*) ) Header.e_entry);

	BootInfo bootInfo;
	bootInfo.framebuffer = newBuffer;
	bootInfo.psf1_Font = newFont;
	bootInfo.mMap = Map;
	bootInfo.mMapSize = MapSize;
	bootInfo.mMapDescSize = DescriptorSize;

	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	KernelStart(&bootInfo);

	return EFI_SUCCESS; // Exit the UEFI application
}
