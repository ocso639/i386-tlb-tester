ENTRY(_start)

SECTIONS
{	
	. = 0x9000;
	.preload :
	{
		*(.preload)
	}

	.text :
	{
		*(.text)
	}
	
	.rodata :
	{
		*(.rodata)		
	}			


	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}			   
