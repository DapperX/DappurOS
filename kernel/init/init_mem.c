#include "multiboot.h"
#include "print.h"
#include "memory.h"
#include "elf.h"

uint size_mem=0;
uint ACPI_addr=0,ACPI_len=0;
byte *addr_mod=NULL;

uint len_reversePageTable=0;
byte *const reversePageTable=(byte*)0x201900;

void handle_tag_module(struct multiboot_tag_module *tag)
{
	kprintf("Modules: %s\n",tag->cmdline);
	kprintf("%d %d\n",tag->mod_start,tag->mod_end);
	addr_mod=(byte*)tag->mod_start;
}

void handle_tag_mmap(struct multiboot_tag_mmap *tag)
{
	if(tag->entry_version!=0) kputs("Warning: mmap version may be incompatible");

	byte *end=(byte*)tag+tag->size;
	for(multiboot_memory_map_t *entry=tag->entries;
		(byte*)entry<end;
		entry=(multiboot_memory_map_t*)((byte*)entry+tag->entry_size))
	{
		if(entry->type==MULTIBOOT_MEMORY_ACPI_RECLAIMABLE)
			ACPI_addr=(uint)entry->addr,ACPI_len=(uint)entry->len; // {unsafe}

		byte fillchar=entry->type==MULTIBOOT_MEMORY_AVAILABLE?0:1;

		uint end=(uint)(entry->addr+entry->len)>>12; // {unsafe}
		if(end>len_reversePageTable) len_reversePageTable=end;

		for(uint i=(uint)(entry->addr)>>12;i<end;++i)
			reversePageTable[i]=fillchar;
	}
	kprintf("len_reversePageTable:%d\n",len_reversePageTable);
}

uint align8(uint p)
{
	return (p+7u)&(~7u);
}

typedef void (*handle_tag_t)(struct multiboot_tag*); 
handle_tag_t handle_tag[7]={
	[3]=(handle_tag_t)handle_tag_module,
	[6]=(handle_tag_t)handle_tag_mmap,
};

void parser_elf(byte *buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	kprintf("e_shentsize: %d\n",header->e_shentsize);
	kprintf("e_shnum: %d\n",header->e_shnum);

	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	char *strtab=(char*)&buffer[section[header->e_shstrndx].sh_offset];
	for(int i=0;i<header->e_shnum;++i)
		kprintf("%d %s\n",i,&strtab[section[i].sh_name]);

	// relocatation
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_REL) continue;
		kprintf("#%s symtab:%d dist:%d\n",&strtab[section[i].sh_name],section[i].sh_link,section[i].sh_info);

		uint cnt_reltab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Rel *reltab=(Elf32_Rel*)&buffer[section[i].sh_offset];
		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[section[i].sh_link].sh_offset];
		byte *section_dist=(byte*)&buffer[section[section[i].sh_info].sh_offset];

		for(uint j=0;j<cnt_reltab;++j)
		{
			Elf32_Addr offset=reltab[j].r_offset;
			Elf32_Sym *symbol=&symtab[reltab[j].r_info>>8];

			kprintf("@Index of symtab: %d\n",reltab[j].r_info>>8);

			if((symbol->st_info&0xf)==STT_OBJECT) kputs("OBJECT");
			else if((symbol->st_info&0xf)==STT_FUNC) kputs("FUNCTION");
			else if((symbol->st_info&0xf)==STT_SECTION) kputs("SECTION");
			else kputs("OTHER");

			byte *section_sym=NULL;
			if(symbol->st_shndx==SHN_ABS) kputs("ABS");
			else if(symbol->st_shndx==SHN_COMMON) kputs("COMMON");
			else if(symbol->st_shndx==SHN_UNDEF) kputs("UNDEF");
			else section_sym=&buffer[section[symbol->st_shndx].sh_offset];

			if(!section_sym) continue;

			kprintf("st_shndx: %d\n",symbol->st_shndx);
			kprintf("st_value: %x\n",symbol->st_value);

			kprintf("initVal: %d\n",*(int*)(section_dist+offset));
			kprintf("symVal: %p\n",section_sym+symbol->st_value);

			if((reltab[j].r_info&0xff)==R_386_32) // absolute location
			{
				kputs("R_386_32");
				*(int*)(section_dist+offset)+=(int)(section_sym+symbol->st_value);
			}
			else // relative location(R_386_PC32)
			{
				kputs("R_386_PC32");
				*(int*)(section_dist+offset)+=(int)(section_sym+symbol->st_value)-(int)(section_dist+offset);
			}
		}
	}
	kprintf("buffer: %p\n",buffer);

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
	Elf32_Sym *symbol_main=&symtab[7];
	func_t addr_main=(func_t)&buffer[section[symbol_main->st_shndx].sh_offset+symbol_main->st_value];
	kprintf("addr_main: %p\n",addr_main);
	addr_main();
}


void init_c_start(uint magic,uint mbi)
{
	kputs("DappurOS initializing...");
	kprintf("magic: %x\n",magic);
	if(magic!=MULTIBOOT2_BOOTLOADER_MAGIC) return;
	kprintf("mbi: %p\n",mbi);
	if(mbi&7) return;

	struct multiboot_tag *tag=(struct multiboot_tag*)(mbi+8);
	while(tag->type!=MULTIBOOT_TAG_TYPE_END)
	{
		if(tag->type<7 && handle_tag[tag->type])
			handle_tag[tag->type](tag);
		tag=(struct multiboot_tag*)align8((uint)tag+tag->size);
	}

	kprintf("addr_mod: %p\n",addr_mod);
	parser_elf(addr_mod);
}