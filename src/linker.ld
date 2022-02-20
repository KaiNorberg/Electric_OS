OUTPUT_FORMAT(elf64-x86-64)
ENTRY(KernelMain)

SECTIONS
{
	. = 1M;
    
    _KernelStart = .;
	
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{		
		*(.data)
		*(.bss)
		*(COMMON)
    }
  
    _KernelEnd = .;
}
