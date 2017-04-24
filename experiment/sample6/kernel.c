void printf(const char*,...);
void putchar(int c);
void cls();

void os_start(int eax,int ebx)
{
	int pos=0;
	//cls();
	printf("OS starts running...\n");
	printf("eax: %d\n",eax);
	printf("ebx: %d\n",ebx);
}