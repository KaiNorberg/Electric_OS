OUTPUT_FORMAT(elf64-x86-64)
ENTRY(KernelMain)

SECTIONS
{
    . = 0x100000;

    _KernelStart = .;
    
    .text ALIGN(4K) :
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) :
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) :
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) :
    {
        KEEP(*(COMMON))
        KEEP(*(.bss*))
    }
  
    _KernelEnd = .;
}
