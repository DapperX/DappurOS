#define SYS_WRITE (4)
#define STDOUT (0)
#define LEN_S (8)

char *s="Success\n";
int g_uninit;

void print()
{
	__asm__ __volatile__(
		"int $0x80\n\t"
	:
	:
		"a"(SYS_WRITE),"b"(g_uninit),"c"(s),"d"(LEN_S)
	:
	);
}

void exit()
{
	__asm__ __volatile__(
		"movl $0, %ebx\n\t"
		"movl $1, %eax\n\t"
		"int $0x80\n\t"
	);
}

void main()
{
	print();
	exit();
}