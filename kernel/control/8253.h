#ifndef _8253_H
#define _8253_H ARCH_X86

#define PORT_PIT_CNT0 0x40
#define PORT_PIT_CNT1 0x41
#define PORT_PIT_CNT2 0x42
#define PORT_PIT_CTRL 0x43

#define PIT_CNT0 0x00
#define PIT_CNT1 0x40
#define PIT_CNT2 0x80

#define PIT_LATCH 0x00
#define PIT_READ_LOW 0x10
#define PIT_READ_HIGH 0x20
#define PIT_READ_LH 0x30

#define PIT_MODE_0 0x00
#define PIT_MODE_1 0x02
#define PIT_MODE_2 0x04
#define PIT_MODE_3 0x06
#define PIT_MODE_4 0x08
#define PIT_MODE_5 0x0a

#define PIT_BIN 0x00
#define PIT_BCD 0x01

#endif // _8253_H