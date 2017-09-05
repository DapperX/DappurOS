#include "init_mem.h"
#include "multiboot.h"
#include "print.h"
#include "memory.h"
#include "elf.h"
#include "algorithm.h"
#include "string.h"

#define ADDR_MBI 0x100000
#define ADDR_ELF 0x101000

#define CNT_MODULE 5
uint cnt_module=0;
struct multiboot_tag_module module[CNT_MODULE];

uint size_reserveMemory;

static inline uint align8(register uint p)
{
	return (p+7u)&(~7u);
}

static inline uint align12(register uint p)
{
	return (p+4095u)&(~4095u);
}

// upper align `val` to multiply of 2**`bit`
static inline uint align(uint val,uint bit)
{
	register uint mask=(1u<<bit)-1;
	return (val+mask)&(~mask);
}

// store pointer of tag to `module`
// the max number of modules to handle is CNT_MODULE, exceeding will NOT cause error
void handle_tag_module(struct multiboot_tag_module *tag)
{
	kprintf("Modules: %s\n",tag->cmdline);
	kprintf("%d %d\n",tag->mod_start,tag->mod_end);
	if(cnt_module<CNT_MODULE)
		module[cnt_module++]=*tag;
}

// initialize the reversePageTable according to the mmap
void handle_tag_mmap(struct multiboot_tag_mmap *tag)
{
	if(tag->entry_version!=0) kputs("Warning: mmap version is incompatible");

	byte *end=(byte*)tag+tag->size;
	for(multiboot_memory_map_t *entry=tag->entries;
		(byte*)entry<end;
		entry=(multiboot_memory_map_t*)((byte*)entry+tag->entry_size))
	{
		if(entry->type==MULTIBOOT_MEMORY_ACPI_RECLAIMABLE)
			ACPI_addr=(uint)entry->addr,ACPI_len=(uint)entry->len; // {unsafe}

		if(entry->addr>=MIN_MEMORY) continue;

		uint begin=(uint)(entry->addr)>>12; // {unsafe}
		uint end=(uint)(entry->addr+entry->len)>>12; // {unsafe}
		uint attr_used=entry->type==MULTIBOOT_MEMORY_AVAILABLE?0:RPE_U;

		// pad holes between memory blocks
		for(register uint i=len_reversePageTable;i<begin;++i)
			reversePageTable[i]=(uint*)RPE_U;

		for(register uint i=begin;i<end;++i)
			reversePageTable[i]=(uint*)((i<<12)|attr_used);

		if(end>len_reversePageTable) len_reversePageTable=end;
	}
	kprintf("len_reversePageTable:%d\n",len_reversePageTable);
}


inline uint* get_addr_PDE(register void *addr_physical)
{
	return &((uint*)(pageDirectory[(uint)addr_physical>>22]&~((1u<<12)-1)))[((uint)addr_physical>>12)&((1u<<10)-1)];
}

void* get_page_free(const uint cnt,const uint is_writable)
{
	kprintf("get_page: %p %d\n",size_reserveMemory,cnt);
	const uint base=size_reserveMemory;
	for(uint i=0;i<cnt;++i)
	{
		uint *addr_PDE=&pageTable_temp[size_reserveMemory>>12];
		pageDirectory[size_reserveMemory>>22]=(uint)&pageTable_temp[(size_reserveMemory>>22)*1024]|PDE_P|PDE_R;
		(*addr_PDE)=size_reserveMemory|PDE_P|is_writable;
		reversePageTable[size_reserveMemory>>12]=(uint*)(((uint)addr_PDE-OFFSET_KERNEL_LOW_MEMORY+ADDR_HIGH_MEMORY)|RPE_U);
		size_reserveMemory+=(1<<12);
	}
	return (void*)base;
}

void elf_init_LMA(byte *const buffer)
{
	// following define may be used in the future for supportnig more segment attributes
	#define SEGMENT_RO 0
	#define SEGMENT_RW 1

	#define TO_TYPE_SEGMENT(type_section) ((type_section)&SHF_WRITE)

	#define CNT_TYPE 2
	uint offset[CNT_TYPE]={0};
	uint base[CNT_TYPE];

	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(!(section[i].sh_flags&SHF_ALLOC)) continue;
		// for now there are only two types of segment: ReadOnly and Read/Write
		// NoeXecute attribute of page is unsupported yet
		// the code is designed for more types of segment
		uint type=TO_TYPE_SEGMENT(section[i].sh_flags);
		section[i].sh_addr=align(offset[type],section[i].sh_addralign);
		offset[type]=section[i].sh_addr+section[i].sh_size;
	}
	for(uint i=0;i<CNT_TYPE;++i)
	{
		uint cnt=align12(offset[i])>>12;
		base[i]=(uint)get_page_free(cnt,i<<1);
	}
	for(uint i=0;i<header->e_shnum;++i)
		if(section[i].sh_flags&SHF_ALLOC)
		{
			section[i].sh_addr+=base[TO_TYPE_SEGMENT(section[i].sh_flags)];
			kprintf("*section[%d].addr:%p\n",i,section[i].sh_addr);
		}
}


void elf_relocate(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	kprintf("e_shentsize: %d\n",header->e_shentsize);
	kprintf("e_shnum: %d\n",header->e_shnum);

	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	char *shstrtab=(char*)&buffer[section[header->e_shstrndx].sh_offset];

	// relocatation
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_REL) continue;
		kprintf("#%s symtab:%d dist:%d\n",&shstrtab[section[i].sh_name],section[i].sh_link,section[i].sh_info);

		uint cnt_reltab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Rel *reltab=(Elf32_Rel*)&buffer[section[i].sh_offset];
		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[section[i].sh_link].sh_offset];
		char *strtab=(char*)&buffer[section[section[section[i].sh_link].sh_link].sh_offset];
		byte *section_dist=(byte*)&buffer[section[section[i].sh_info].sh_offset];
		byte *addr_section_dist=(byte*)section[section[i].sh_info].sh_addr;

		for(uint j=0;j<cnt_reltab;++j)
		{
			Elf32_Addr offset=reltab[j].r_offset;
			Elf32_Sym *symbol=&symtab[reltab[j].r_info>>8];

			kprintf("@symtab[%d]: %s\n",reltab[j].r_info>>8,&strtab[symbol->st_name]);

			byte *addr_section_sym=NULL;
			switch(symbol->st_shndx)
			{
				case SHN_ABS:
				case SHN_COMMON:
				case SHN_UNDEF:
					break;
				default:
					addr_section_sym=(byte*)section[symbol->st_shndx].sh_addr;
			}
			if(!addr_section_sym)
			{
				kputs("unsupported symbol section");
				continue;
			}

			if((reltab[j].r_info&0xff)==R_386_32) // absolute location
				*(int*)(section_dist+offset)+=(uint)(addr_section_sym+symbol->st_value)-OFFSET_KERNEL_LOW_MEMORY+ADDR_HIGH_MEMORY;
			else // relative location(R_386_PC32)
				*(int*)(section_dist+offset)+=(int)((uint)(addr_section_sym+symbol->st_value)-(uint)(addr_section_dist+offset));
		}
	}
	kprintf("buffer: %p\n",buffer);
}


void load_elf(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(!section[i].sh_addr) continue;
		kmemcpy((byte*)section[i].sh_addr,&buffer[section[i].sh_offset],section[i].sh_size);
	}
}


void set_kernelCall(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(uint i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_SYMTAB) continue;
		uint cnt_symtab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[i].sh_offset];
		char *strtab=(char*)&buffer[section[section[i].sh_link].sh_offset];

		uint kernelCall_index=0;
		kernelCall kernelCall_entry=NULL;		
		for(uint j=0;j<cnt_symtab;++j)
		{
			uint symVal=*(uint*)(section[symtab[j].st_shndx].sh_addr+symtab[j].st_value);
			if(!kstrcmp(&strtab[symtab[j].st_name],"module_kernelCall_index"))
			{
				kernelCall_index=symVal;
				kprintf("kernelCall_index: %u\n",kernelCall_index);
			}
			if(!kstrcmp(&strtab[symtab[j].st_name],"module_kernelCall_entry"))
			{
				kernelCall_entry=(kernelCall)symVal;
				kprintf("kernelCall_entry: %p\n",kernelCall_entry);
			}
		}
		if(kernelCall_entry)
			kernelCallTable[kernelCall_index]=kernelCall_entry;
	}
	kprintf("KCT[0]: %p\n",kernelCallTable[0]);
}


void load_module()
{
	ksort(module,module+CNT_MODULE,sizeof(*module),cmp_default_pointer);
	byte *addr_elf=(byte*)ADDR_ELF;
	for(uint i=0;i<cnt_module;++i)
	{
		uint size=module[i].mod_end-module[i].mod_start;
		kmemcpy(addr_elf,(byte*)module[i].mod_start,size);
		module[i].mod_start=(uint)addr_elf;
		addr_elf+=size;
	}
	for(uint i=0;i<cnt_module;++i)
	{
		byte *const addr_module=(byte*)module[i].mod_start;
		elf_init_LMA(addr_module);
		elf_relocate(addr_module);
		load_elf(addr_module);
		set_kernelCall(addr_module);
	}
}


void init_pageDirectory()
{
	const uint cnt_pageDirectory=(1<<10);
	for(register uint i=0;i<cnt_pageDirectory;++i)
		pageDirectory[i]=0;
	// important! it needs to align to 2**22(4MB)
	for(register uint i=0;i<align(size_reserveMemory,22)>>22;++i)
		pageDirectory[i]=(uint)&pageTable_temp[i*1024]|PDE_P|PDE_R;
	for(register uint i=(ADDR_HIGH_MEMORY>>22);i<cnt_pageDirectory;++i)
		pageDirectory[i]=(uint)&pageTable_kernel[(i-(ADDR_HIGH_MEMORY>>22))*1024]|PDE_P|PDE_R;
}


void init_pageTable_temp()
{
	// i is the high 20bit of reserve memory
	for(register uint i=0;i<(size_reserveMemory>>12);++i)
	{
		// 0x0:size_reserveMemory -> 0x0:size_reserveMemory
		pageTable_temp[i]=(i<<12)|PTE_P|PTE_R;
	}
}

void init_pageTable_kernel()
{
	// i is the high 20bit of kernel address space
	for(register uint i=(ADDR_HIGH_MEMORY>>12);i<(1<<20);++i)
		pageTable_kernel[i-(ADDR_HIGH_MEMORY>>12)]=0;
	// i is the high 20bit of reserve memory
	for(register uint i=(OFFSET_KERNEL_LOW_MEMORY>>12);i<(size_reserveMemory>>12);++i)
	{
		// ADDR_HIGH_MEMORY:ADDR_HIGH_MEMORY+size_reserveMemory-1MB -> 1MB:size_reserveMemory
		pageTable_kernel[i-(OFFSET_KERNEL_LOW_MEMORY>>12)]=pageTable_temp[i];
	}
}


typedef void (*handle_tag_t)(struct multiboot_tag*); 
handle_tag_t handle_tag[7]={
	[3]=(handle_tag_t)handle_tag_module,
	[6]=(handle_tag_t)handle_tag_mmap,
};

static inline void enable_page()
{
	__asm__ __volatile__(
		"movl %0, %%cr3\n\t"
		"movl %%cr0, %%eax\n\t"
		"orl $0x80010000, %%eax\n\t"
		"movl %%eax, %%cr0\n\t"
	:
	:
		"a"(pageDirectory)
	);
}

void debug_output()
{
	kputs("##pageDirectory");
	for(uint i=0;i<32;++i)
	{
		kprintf("%x ",pageDirectory[i]);
		if(!(~i&3)) kputchar('\n');
	}
	kputs("##pageTable_kernel");
	for(uint i=0;i<32;++i)
	{
		kprintf("%x ",pageTable_kernel[i]);
		if(!(~i&3)) kputchar('\n');
	}
	kputs("##pageTable_temp");
	for(uint i=0;i<32;++i)
	{
		kprintf("%x ",pageTable_temp[i]);
		if(!(~i&3)) kputchar('\n');
	}
}

void init_memory_(uint magic,uint mbi)
{
	kputs("DappurOS initializing...");
	kprintf("magic: %x\n",magic);
	if(magic!=MULTIBOOT2_BOOTLOADER_MAGIC) return;
	kprintf("mbi: %p\n",mbi);
	if(mbi&7) return;

	kmemcpy((byte*)ADDR_MBI,(byte*)mbi,*(uint*)mbi);

	struct multiboot_tag *tag=(struct multiboot_tag*)(ADDR_MBI+8);
	while(tag->type!=MULTIBOOT_TAG_TYPE_END)
	{
		if(tag->type<7 && handle_tag[tag->type])
			handle_tag[tag->type](tag);
		tag=(struct multiboot_tag*)align8((uint)tag+tag->size);
	}

	kmemset(kernelCallTable,0,256);
	size_reserveMemory=align12((uint)&reversePageTable[len_reversePageTable]);

	init_pageDirectory();
	// since the modules will temporarily occupy the `pageTable_kernel` memory area
	// while to load modules, it needs to modify `pageTable_temp`
	// it has to initialize these two pageTables respectively
	init_pageTable_temp();
	load_module();
	init_pageTable_kernel();

	enable_page();
	//__asm__ __volatile__("hlt");
	kernelCallTable[0](0); // let's get kernel started
}


void init_memory(uint magic,uint mbi)
{
	init_memory_(magic,mbi);
	// `init_memory_` should not return, except it failed
	kputs("Startup has failed");
}