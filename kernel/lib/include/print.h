#ifndef _PRINT_H
#define _PRINT_H


#include "base.h"

#define CNT_COLUMN (80)
#define CNT_ROW (24)

#define COLOR_WB (0x07)


void write_video(i32 pos,char c,char color);
void kcls();
void kputchar(i32 c);
void kputs(char *s);
void kprintf(const char *format,...);
void kprint_int(const i32 x);

#endif //_PRINT_H