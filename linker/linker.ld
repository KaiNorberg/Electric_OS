OUTPUT_FORMAT(elf64-x86-64)
ENTRY(KernelMain)

SECTIONS
{
	. = 1M;
    
    _KernelStart = .;
    
	/*.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
    }*/
  
    _KernelEnd = .;
}
