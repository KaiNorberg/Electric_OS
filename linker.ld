OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    _KernelStart = .;

<<<<<<< HEAD:linker.ld
    .syscallargs BLOCK(0x1000) : ALIGN(0x1000)
    {
        _SYS_CALL_ADDR = .;
        . = . + 0x1000;
    }

=======
>>>>>>> parent of 0d6c9a1 (Started work on system calls.):kernel/kernel.ld
    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _IDT = .;
        . = . + 0x1000;
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
