ENTRY(module_start)

SECTIONS {
	multiboot_header :
	{
		*(multiboot_header)
	}
	start 0x7c00 :
	{
		*(start)
	}
	.bss :
	{
		*(.bss)
	}
	.rodata :
	{
		*(.rodata)
	}
	.data :
	{
		*(.data)
	}
	.text :
	{
		*(.text)
	}
	LMA_GDT = 0x102800;
	LMA_LDT = 0x102880;
	reserve 0x100000 :
	{
		*(reserve)
	}
	/DISCARD/ :
	{
		*(.eh_frame .comment)
	}
}