#include "init_mem.h"
#include "multiboot.h"
#include "print.h"
#include "memory.h"
#include "elf.h"
#include "algorithm.h"
#include "string.h"
#include "debug.h"

u32 *const pageDirectory=(u32*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY);
u32 *const pageTable_kernel=(u32*)(OFFSET_LOW_MEMORY+ADDR_PAGE_DIRECTORY+4*1024);
u32 *const pageTable_temp=(u32*)PAGE_TABLE_TEMP;
// reversePageTable[i]: high 30 bits show the virtual page address(4bit align) used by i-th physical page
// low 2 bits show the attribute of this page
u32 len_reversePageTable=0;
u32 **const reversePageTable=(u32**)(OFFSET_LOW_MEMORY+ADDR_REVERSE_PAGE_TABLE);

kernelCall *const kernelCallTable=(kernelCall*)(OFFSET_LOW_MEMORY+ADDR_KERNEL_CALL_TABLE);

u32 ACPI_addr=0,ACPI_len=0;

#define ADDR_MBI 0x100000
#define ADDR_ELF 0x101000

#define CNT_MODULE 5
u32 cnt_module=0;
struct multiboot_tag_module module[CNT_MODULE];

u32 size_reserveMemory;

static inline u32 align8(register u32 p)
{
	return (p+7u)&(~7u);
}

static inline u32 align12(register u32 p)
{
	return (p+4095u)&(~4095u);
}

// upper align `val` to multiply of 2**`bit`
static inline u32 align(u32 val,u32 bit)
{
	register u32 mask=(1u<<bit)-1;
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
			ACPI_addr=(u32)entry->addr,ACPI_len=(u32)entry->len; // {unsafe}

		if(entry->addr>=MIN_MEMORY) continue;

		u32 begin=(u32)(entry->addr)>>12; // {unsafe}
		u32 end=(u32)(entry->addr+entry->len)>>12; // {unsafe}
		u32 attr_used=entry->type==MULTIBOOT_MEMORY_AVAILABLE?0:RPE_U;

		// pad holes between memory blocks
		for(register u32 i=len_reversePageTable;i<begin;++i)
			reversePageTable[i]=(u32*)RPE_U;

		for(register u32 i=begin;i<end;++i)
			reversePageTable[i]=(u32*)((i<<12)|attr_used);

		if(end>len_reversePageTable) len_reversePageTable=end;
	}
	kprintf("len_reversePageTable:%d\n",len_reversePageTable);
}


inline u32* get_addr_PDE(register void *addr_physical)
{
	return &((u32*)(pageDirectory[(u32)addr_physical>>22]&~((1u<<12)-1)))[((u32)addr_physical>>12)&((1u<<10)-1)];
}

void* get_page_free(const u32 cnt,const u32 is_writable)
{
	kprintf("get_page: %p %d\n",size_reserveMemory,cnt);
	const u32 base=size_reserveMemory;
	for(u32 i=0;i<cnt;++i)
	{
		u32 *addr_PDE=&pageTable_temp[size_reserveMemory>>12];
		pageDirectory[size_reserveMemory>>22]=(u32)&pageTable_temp[(size_reserveMemory>>22)*1024]|PDE_P|PDE_R;
		(*addr_PDE)=size_reserveMemory|PDE_P|is_writable;
		reversePageTable[size_reserveMemory>>12]=(u32*)(((u32)addr_PDE-OFFSET_LOW_MEMORY+OFFSET_HIGH_MEMORY)|RPE_U);
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
	u32 offset[CNT_TYPE]={0};
	u32 base[CNT_TYPE];

	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	kprintf("e_shentsize: %d\n",header->e_shentsize);
	kprintf("e_shnum: %d\n",header->e_shnum);
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(u32 i=0;i<header->e_shnum;++i)
	{
		if(!(section[i].sh_flags&SHF_ALLOC)) continue;
		// for now there are only two types of segment: ReadOnly and Read/Write
		// NoeXecute attribute of page is unsupported yet
		// the code is designed for more types of segment
		u32 type=TO_TYPE_SEGMENT(section[i].sh_flags);
		section[i].sh_addr=align(offset[type],section[i].sh_addralign);
		offset[type]=section[i].sh_addr+section[i].sh_size;
	}
	for(u32 i=0;i<CNT_TYPE;++i)
	{
		u32 cnt=align12(offset[i])>>12;
		base[i]=(u32)get_page_free(cnt,i<<1);
	}
	for(u32 i=0;i<header->e_shnum;++i)
		if(section[i].sh_flags&SHF_ALLOC)
		{
			section[i].sh_addr+=base[TO_TYPE_SEGMENT(section[i].sh_flags)];
			kprintf("*section[%d].addr:%p\n",i,section[i].sh_addr);
		}
}


void elf_relocate(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	char *shstrtab=(char*)&buffer[section[header->e_shstrndx].sh_offset];

	// relocatation
	for(u32 i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_REL) continue;

		u32 cnt_reltab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Rel *reltab=(Elf32_Rel*)&buffer[section[i].sh_offset];
		Elf32_Word id_symtab=section[i].sh_link;
		Elf32_Word dist=section[i].sh_info;
		kprintf("#%s symtab:%d dist:%d\n",&shstrtab[section[i].sh_name],id_symtab,dist);

		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[id_symtab].sh_offset];
		char *strtab=(char*)&buffer[section[section[id_symtab].sh_link].sh_offset];
		byte *section_dist=(byte*)&buffer[section[dist].sh_offset];
		byte *addr_section_dist=(byte*)section[dist].sh_addr;

		for(u32 j=0;j<cnt_reltab;++j)
		{
			Elf32_Addr offset=reltab[j].r_offset;
			Elf32_Sym *symbol=&symtab[reltab[j].r_info>>8];

			kprintf("@symtab[%d]: ",reltab[j].r_info>>8);
			if((symbol->st_info&0xf)==STT_SECTION)
				kputs(&shstrtab[section[symbol->st_shndx].sh_name]);
			else
				kputs(&strtab[symbol->st_name]);
			kprintf("offset: %x\n",offset);

			byte *addr_section_sym=NULL;
			switch(symbol->st_shndx)
			{
				case SHN_ABS:
				case SHN_COMMON:
				case SHN_UNDEF:
					DEBUG_BREAKPOINT; // ignore these pseudo sections, for now
					break;
				default:
					addr_section_sym=(byte*)section[symbol->st_shndx].sh_addr;
			}
			if(!addr_section_sym)
			{
				kprintf("unsupported symbol section %d\n",symbol->st_shndx);
				continue;
			}

			if((reltab[j].r_info&0xff)==R_386_32) // absolute location
				*(int*)(section_dist+offset)+=(int)((u32)(addr_section_sym+symbol->st_value)-OFFSET_LOW_MEMORY+OFFSET_HIGH_MEMORY);
			else // relative location(R_386_PC32)
				*(int*)(section_dist+offset)+=(int)((u32)(addr_section_sym+symbol->st_value)-(u32)(addr_section_dist+offset));
		}
	}
	kprintf("buffer: %p\n",buffer);
}


void load_elf(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(u32 i=0;i<header->e_shnum;++i)
	{
		if(!section[i].sh_addr) continue;
		kmemcpy((byte*)section[i].sh_addr,&buffer[section[i].sh_offset],section[i].sh_size);
	}
}


void set_kernelCall(byte *const buffer)
{
	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(u32 i=0;i<header->e_shnum;++i)
	{
		if(section[i].sh_type!=SHT_SYMTAB) continue;
		u32 cnt_symtab=section[i].sh_size/section[i].sh_entsize;
		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[i].sh_offset];
		char *strtab=(char*)&buffer[section[section[i].sh_link].sh_offset];

		u32 kernelCall_index=0;
		kernelCall kernelCall_entry=NULL;		
		for(u32 j=0;j<cnt_symtab;++j)
		{
			u32 symVal=*(u32*)(section[symtab[j].st_shndx].sh_addr+symtab[j].st_value);
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


static inline bool cmp_module_start(void *x,void *y)
{
	//return cmp_default_pointer(x,y);
	return ((struct multiboot_tag_module*)x)->mod_start<((struct multiboot_tag_module*)y)->mod_start;
}

void load_module()
{
	ksort(module,module+CNT_MODULE,sizeof(*module),cmp_module_start);
	byte *addr_elf=(byte*)ADDR_ELF;
	for(u32 i=0;i<cnt_module;++i)
	{
		u32 size=module[i].mod_end-module[i].mod_start;
		kmemcpy(addr_elf,(byte*)module[i].mod_start,size);
		module[i].mod_start=(u32)addr_elf;
		addr_elf+=size;
	}
	for(u32 i=0;i<cnt_module;++i)
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
	const u32 cnt_pageDirectory=(1<<10);
	for(register u32 i=0;i<cnt_pageDirectory;++i)
		pageDirectory[i]=0;
	// important! it needs to align to 2**22(4MB)
	for(register u32 i=0;i<align(size_reserveMemory,22)>>22;++i)
		pageDirectory[i]=(u32)&pageTable_temp[i*1024]|PDE_P|PDE_R;
	for(register u32 i=(OFFSET_HIGH_MEMORY>>22);i<cnt_pageDirectory;++i)
		pageDirectory[i]=(u32)&pageTable_kernel[(i-(OFFSET_HIGH_MEMORY>>22))*1024]|PDE_P|PDE_R;
}


void init_pageTable_temp()
{
	// i is the high 20bit of reserve memory
	for(register u32 i=0;i<(size_reserveMemory>>12);++i)
	{
		// 0x0:size_reserveMemory -> 0x0:size_reserveMemory
		pageTable_temp[i]=(i<<12)|PTE_P|PTE_R;
	}
}


void init_pageTable_kernel()
{
	// i is the high 20bit of kernel address space
	for(register u32 i=(OFFSET_HIGH_MEMORY>>12);i<(1<<20);++i)
		pageTable_kernel[i-(OFFSET_HIGH_MEMORY>>12)]=0;
	// i is the high 20bit of reserve memory
	for(register u32 i=(OFFSET_LOW_MEMORY>>12);i<(size_reserveMemory>>12);++i)
	{
		// OFFSET_HIGH_MEMORY:OFFSET_HIGH_MEMORY+size_reserveMemory-1MB -> 1MB:size_reserveMemory
		pageTable_kernel[i-(OFFSET_LOW_MEMORY>>12)]=pageTable_temp[i];
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
	DEBUG_BREAKPOINT;
	kcls();
	kputs("##pageDirectory");
	for(u32 i=0;i<32;++i)
	{
		kprintf("%x ",pageDirectory[i]);
		if(!(~i&3)) kputchar('\n');
	}
	kputs("##pageTable_kernel");
	for(u32 i=0;i<32;++i)
	{
		kprintf("%x ",pageTable_kernel[i]);
		if(!(~i&3)) kputchar('\n');
	}
	kputs("##pageTable_temp");
	for(u32 i=0;i<32;++i)
	{
		kprintf("%x ",pageTable_temp[i]);
		if(!(~i&3)) kputchar('\n');
	}
	DEBUG_BREAKPOINT;
}


void init_memory_(u32 magic,u32 mbi)
{
	kputs("DappurOS initializing...");
	kprintf("magic: %x\n",magic);
	if(magic!=MULTIBOOT2_BOOTLOADER_MAGIC) return;
	kprintf("mbi: %p\n",mbi);
	if(mbi&7) return;

	kmemcpy((byte*)ADDR_MBI,(byte*)mbi,*(u32*)mbi);

	struct multiboot_tag *tag=(struct multiboot_tag*)(ADDR_MBI+8);
	while(tag->type!=MULTIBOOT_TAG_TYPE_END)
	{
		if(tag->type<7 && handle_tag[tag->type])
			handle_tag[tag->type](tag);
		tag=(struct multiboot_tag*)align8((u32)tag+tag->size);
	}

	kmemset(kernelCallTable,0,256);
	size_reserveMemory=align12((u32)&reversePageTable[len_reversePageTable]);

	init_pageDirectory();
	// since the modules will temporarily occupy the `pageTable_kernel` memory area
	// while to load modules, it needs to modify `pageTable_temp`
	// it has to initialize these two pageTables respectively
	init_pageTable_temp();
	load_module();
	init_pageTable_kernel();

	enable_page();
	kernelCallTable[MODULE_TYPE_CONTROL](KERNEL_CALL_INIT); // let's get kernel started
}


void init_memory(u32 magic,u32 mbi)
{
	init_memory_(magic,mbi);
	// `init_memory_` should not return, except it failed
	kputs("Startup has failed");
}