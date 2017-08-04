void printf(const char*,...);
void print_int(const char*,int);
void putchar(int c);
void cls();

void os_start(int eax,int ebx)
{
	int pos=0;
	//cls();
	printf("OS starts running...\n");
	printf("eax: %d\n",eax);
	printf("ebx: %d\n%s\n",ebx,"test_text");
	print_int("addr: %d\n",(int)&eax);
}