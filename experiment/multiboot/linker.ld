ENTRY(start)

SECTIONS {
    . = 1M;
    .text ALIGN(0x1000) :
    {
        *(.multiboot_header)
        *(.text)
    }
}