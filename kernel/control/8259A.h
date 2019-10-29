#ifndef _8259A_H
#define _8259A_H ARCH_X86

#define PORT_PIC0_CTRL 0x20
#define PORT_PIC0_DATA 0x21
#define PORT_PIC1_CTRL 0xa0
#define PORT_PIC1_DATA 0xa1

// The following macros refer to (OSDev.org)[https://wiki.osdev.org/8259_PIC]
#define ICW1_ICW4 0x01 // ICW4 (not) needed
#define ICW1_SINGLE 0x02 // Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL 0x08 // Level triggered (edge) mode
#define ICW1_ID 0x10 // Initialization - required!
 
#define ICW4_8086 0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x02 // Auto (normal) EOI
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_BUF_SLAVE 0x08 // Buffered mode/slave
#define ICW4_SFNM 0x10 // Special fully nested (not)

#endif // _8259A_H