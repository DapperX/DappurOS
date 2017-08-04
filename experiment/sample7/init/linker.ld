ENTRY(_start)

SECTIONS {
	.data 0x500 :
	{
		*(.data)
	}
	.start 0x7c00 :
	{
		*(start)
	}
	.text :
	{
		*(.text)
	}
	.multiboot_header 0x1000 :
	{
		*(multiboot_header)
	}
	PDE_kernel 0x100000 :
	{
		*(PDE_kernel)
	}
	PTE_kernel 0x101000 :
	{
		*(PTE_kernel)
	}
	IDT 0x201000 :
	{
		*(IDT)
	}
	GDT 0x201800 :
	{
		*(GDT)
	}
	LDT 0x2018A0 :
	{
		*(LDT)
	}
}