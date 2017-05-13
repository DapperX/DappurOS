ENTRY(_start)

SECTIONS {
	.multiboot_header 0x1000 :
	{
		*(multiboot_header)
	}
	.text 0x2000 :
	{
		*(.text)
	}
}