#include "stdio.h"
#include "stdlib.h"
#include "multiboot.h"
FILE *output=NULL;

void halt(char *s)
{
	fputs(stderr,s);
	exit(0);
}

void write_header(multiboot_uint32_t size_header)
{
	multiboot_header_tag tag={
		.magic = MULTIBOOT2_HEADER_MAGIC,
		.architecture = MULTIBOOT_ARCHITECTURE_I386,
		.header_length = size_header,
		.checksum = 0u-(MULTIBOOT2_HEADER_MAGIC+MULTIBOOT_ARCHITECTURE_I386-size_header)
	};
}

int main()
{
	output=fopen(output,"wb");
	if(!output) halt("failed create header");

	write_header(sizeof(multiboot_header_tag)
				+sizeof(multiboot_header_tag_information_request)
				+sizeof(multiboot_hea));


	fclose(output);
	return 0;
}