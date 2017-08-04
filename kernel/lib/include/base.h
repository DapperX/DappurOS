#define CNT_COLUMN (80)
#define CNT_ROW (24)

#define COLOR_WB (0x07)

typedef unsigned int uint;


void write_video(int pos,char c,char color);
void kcls();
void kputchar(int c);
void kputs(char *s);
void kprintf(const char *format,...);
void kprint_int(const int x);