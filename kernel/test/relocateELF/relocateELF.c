#include "stdio.h"
#include "stdlib.h"
#include "init_mem.h"
#define SIZE_BUFFER (1024*40)

static byte buffer[SIZE_BUFFER];

int main()
{
	FILE *elf=fopen("test.mod","rb");
	if(!elf)
	{
		fputs("Failed to open elf",stderr);
		goto exit_main;
	}
	fread(buffer,1,SIZE_BUFFER,elf);
	fclose(elf);

//	elf_init_LMA(buffer);
	elf_relocate(buffer);

	puts("done");

exit_main:
	return 0;
}