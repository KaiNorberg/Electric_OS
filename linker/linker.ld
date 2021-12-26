OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    _KernelStart = .;

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _IDT = .;
        . = . + 0x1000;
    }

    .backbuffer BLOCK(0x21C0000) : ALIGN(0x1000)
    {
        _BACKBUFFER = .;
        . = . + 0x21C0000;
    }
    
    .PageStatusMap BLOCK(0x800000) : ALIGN(0x1000)
    {
        _PageStatusMap = .;
        . = . + 0x800000;
    }

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
    
    _KernelEnd = .;
}
