OUTPUTNAME = Kernel
OSNAME = Electric_OS

GNUEFI = gnu-efi
OVMFDIR = OVMFbin
LDS = linker/linker.ld
CC = g++
ASMC = nasm
LD = ld

CFLAGS = -fno-rtti -ffreestanding -nostdlib -fno-exceptions -Isrc/ -Os -std=c++20
ASMFLAGS =
LDFLAGS = -T $(LDS) -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := build
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_ASM.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

os: $(OBJS) link

$(OBJDIR)/kernel/Interrupts/Handlers.o: $(SRCDIR)/kernel/Interrupts/Handlers.cpp
	@echo !==== COMPILING $^
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -mno-red-zone -mgeneral-regs-only -c $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo !==== COMPILING $^
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_ASM.o: $(SRCDIR)/%.asm
	@echo !==== COMPILING $^
	@mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@echo !==== LINKING
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/$(OUTPUTNAME).elf $(OBJS)

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	sudo mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/KERNEL
	cp $(BOOTEFI)/main.efi  $(BOOTEFI)/bootx64.efi 
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI)/bootx64.efi ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/$(OUTPUTNAME).elf ::/KERNEL
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-vga16.psf ::/KERNEL

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 4G -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
