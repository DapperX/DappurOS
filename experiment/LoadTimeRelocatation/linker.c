#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "elf.h"

typedef unsigned char byte;
typedef unsigned int uint;

byte buffer[100000];

// upper align val to multiply of 2**bit
inline uint align(uint val,uint bit)
{
	register uint mask=(1<<bit)-1;
	return (val+mask)&(~mask);
}

int main()
{
	FILE *elf=fopen("t.o","r");
	if(!elf) return puts("ELF not found"),-1;

	fseek(elf,0,SEEK_END);
	int size=ftell(elf);
	printf("file size: %d\n",size);

	rewind(elf);
	int size_rtn=fread(buffer,1,size,elf);
	fclose(elf);

	if(size!=size_rtn)
		return puts("Read ELF failed"),0;

	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	printf("e_shentsize: %d\n",header->e_shentsize);
	printf("e_shnum: %d\n",header->e_shnum);
	printf("e_entry: %p\n",(byte*)header->e_entry);

	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	char *shstrtab=(char*)&buffer[section[header->e_shstrndx].sh_offset];
	for(int i=0;i<header->e_shnum;++i)
		printf("%d %s\n",i,&shstrtab[section[i].sh_name]);

	// relocatation
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_REL) continue;
		printf("#%s symtab:%d dist:%d\n",&shstrtab[section[i].sh_name],section[i].sh_link,section[i].sh_info);

		uint cnt_reltab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Rel *reltab=(Elf32_Rel*)&buffer[section[i].sh_offset];
		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[section[i].sh_link].sh_offset];
		char *strtab=(char*)&buffer[section[section[section[i].sh_link].sh_link].sh_offset];
		byte *section_dist=(byte*)&buffer[section[section[i].sh_info].sh_offset];

		for(uint j=0;j<cnt_reltab;++j)
		{
			Elf32_Addr offset=reltab[j].r_offset;
			Elf32_Sym *symbol=&symtab[reltab[j].r_info>>8];

			printf("@Index of symtab: %d(%s)\n",reltab[j].r_info>>8,&strtab[symbol->st_name]);

			if((symbol->st_info&0xf)==STT_OBJECT) puts("OBJECT");
			else if((symbol->st_info&0xf)==STT_FUNC) puts("FUNCTION");
			else if((symbol->st_info&0xf)==STT_SECTION) puts("SECTION");
			else puts("OTHER");

			byte *section_sym=NULL;
			if(symbol->st_shndx==SHN_ABS) puts("ABS");
			else if(symbol->st_shndx==SHN_COMMON) puts("COMMON");
			else if(symbol->st_shndx==SHN_UNDEF) puts("UNDEF");
			else section_sym=&buffer[section[symbol->st_shndx].sh_offset];

			if(!section_sym) continue;

			printf("st_shndx: %d\n",symbol->st_shndx);
			printf("st_value: %x\n",symbol->st_value);

			printf("initVal: %d\n",*(int*)(section_dist+offset));
			printf("symVal: %p\n",section_sym+symbol->st_value);

			if((reltab[j].r_info&0xff)==R_386_32) // absolute location
			{
				puts("R_386_32");
				*(int*)(section_dist+offset)+=(int)(section_sym+symbol->st_value);
			}
			else // relative location(R_386_PC32)
			{
				puts("R_386_PC32");
				*(int*)(section_dist+offset)+=(int)(section_sym+symbol->st_value)-(int)(section_dist+offset);
			}
		}
	}
	printf("buffer: %p\n",buffer);
	printf("[%s]\n",(char*)0x804a0dc);

	// get symtab
	Elf32_Sym *symtab=NULL;
	for(int i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type==SHT_SYMTAB)
		{
			symtab=(Elf32_Sym*)&buffer[section[i].sh_offset];
			break;
		}
	}

	typedef void (*func_t)(void);
	Elf32_Sym *symbol_main=&symtab[11];
	func_t addr_main=(func_t)&buffer[section[symbol_main->st_shndx].sh_offset+symbol_main->st_value];
	printf("addr_main: %p\n",addr_main);
//	addr_main();

	return 0;
}