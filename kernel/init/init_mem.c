#include "init_mem.h"
#include "multiboot.h"
#include "print.h"
#include "memory.h"
#include "elf.h"
#include "algorithm.h"
#include "string.h"
#include "assert.h"
#include "debug.h"

#define ADDR_MBI (ADDR_LOW_MEMORY + OFFSET_IDT)
#define ADDR_SEGMENT (ADDR_LOW_MEMORY + 12*1024)

u32 *const pageDirectory=(u32*)(ADDR_LOW_MEMORY + OFFSET_PAGE_DIRECTORY);
u32 *const pageTable=(u32*)(ADDR_LOW_MEMORY + OFFSET_PAGE_DIRECTORY + 4096);
kernelCall *const kernelCallTable=(kernelCall*)(ADDR_LOW_MEMORY + OFFSET_KERNEL_CALL_TABLE);

#define CNT_MODULE 10
u32 cnt_module = 0;
struct multiboot_tag_module *module[CNT_MODULE];

u32 ACPI_addr=0,ACPI_len=0;
u32 size_reserveMemory = 12*1024;

static inline u32 align12(register u32 p)
{
	return (p+4095u)&(~4095u);
}

// upper align `val` to multiple of 2**`bit`
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
		module[cnt_module++] = tag;
}

// initialize the reversePageTable according to the mmap
// this part will soon be transfered to `mm` module
void handle_tag_mmap(struct multiboot_tag_mmap *tag)
{
	/*
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
	*/
}


inline u32* get_addr_PDE(register void *addr_physical)
{
	return &((u32*)(pageDirectory[(u32)addr_physical>>22]&~((1u<<12)-1)))[((u32)addr_physical>>12)&((1u<<10)-1)];
}

void* get_page_free(const u32 cnt,const u32 is_writable)
{
	kprintf("get_page: %p %d\n",size_reserveMemory,cnt);
	const u32 base = size_reserveMemory;
	if(size_reserveMemory>1048576*2)
	{
		kputs("Error: Init memory size is greater than 2MB");
		HALT;
	}
	u32 mask_writable = (!!is_writable)<<1;
	for(u32 i=0;i<cnt;++i)
	{
		pageTable[size_reserveMemory>>12] = (ADDR_LOW_MEMORY+size_reserveMemory)|PTE_P|mask_writable;
		size_reserveMemory += (1<<12);
	}
	return (void*)(ADDR_LOW_MEMORY + base);
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
	//	kprintf("#%s symtab:%d dist:%d\n",&shstrtab[section[i].sh_name],id_symtab,dist);

		Elf32_Sym *symtab=(Elf32_Sym*)&buffer[section[id_symtab].sh_offset];
		char *strtab=(char*)&buffer[section[section[id_symtab].sh_link].sh_offset];
		byte *section_dist=(byte*)&buffer[section[dist].sh_offset];
		byte *addr_section_dest=(byte*)section[dist].sh_addr;

		for(u32 j=0;j<cnt_reltab;++j)
		{
			Elf32_Addr offset=reltab[j].r_offset;
			Elf32_Sym *symbol=&symtab[reltab[j].r_info>>8];
		/*
			kprintf("@symtab[%d]: ",reltab[j].r_info>>8);
			if((symbol->st_info&0xf)==STT_SECTION)
				kputs(&shstrtab[section[symbol->st_shndx].sh_name]);
			else
				kputs(&strtab[symbol->st_name]);
			kprintf("offset: %x\n",offset);
		*/
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
				*(int*)(section_dist+offset)+=(int)((u32)(addr_section_sym+symbol->st_value)-ADDR_LOW_MEMORY+ADDR_HIGH_MEMORY);
			else // relative location(R_386_PC32)
				*(int*)(section_dist+offset)+=(int)((u32)(addr_section_sym+symbol->st_value)-(u32)(addr_section_dest+offset));
		}
	}
	kprintf("buffer: %p\n",buffer);
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
		kernelCall kernelCall_entry = NULL;		
		for(u32 j=0;j<cnt_symtab;++j)
		{
			u32 symVal=*(u32*)&buffer[section[symtab[j].st_shndx].sh_offset+symtab[j].st_value];
			if(!kstrcmp(&strtab[symtab[j].st_name],"module_kernelCall_index"))
			{
				kernelCall_index = symVal;
				kprintf("kernelCall_index: %u\n",kernelCall_index);
			}
			if(!kstrcmp(&strtab[symtab[j].st_name],"module_kernelCall_entry"))
			{
				kernelCall_entry = (kernelCall)symVal;
				kprintf("kernelCall_entry: %p\n",kernelCall_entry);
			}
		}
		kprintf("KCL idx: %d\n", kernelCall_index);
		kprintf("KCL entry: %p\n", kernelCall_entry);
		if(kernelCall_entry)
			kernelCallTable[kernelCall_index]=kernelCall_entry;
	}
	kprintf("KCT[0]: %p\n",kernelCallTable[0]);
}


static inline bool cmp_section_LMA(void *x,void *y)
{
	return (*(Elf32_Shdr **)x)->sh_addr < (*(Elf32_Shdr **)y)->sh_addr;
}

byte* collect_section(byte *buffer, byte *addr_segment, u32 *const info_copy)
{
	KASSERT(addr_segment<buffer);

	#define CNT_SECTION_COLLECT 8
	Elf32_Shdr *section_collect[CNT_SECTION_COLLECT];
	u32 cnt_section_collect = 0;

	Elf32_Ehdr *header=(Elf32_Ehdr*)buffer;
	Elf32_Shdr *section=(Elf32_Shdr*)&buffer[header->e_shoff];
	for(u32 i=0;i<header->e_shnum;++i)
	{
		if(!section[i].sh_addr) continue;
		if(cnt_section_collect>=CNT_SECTION_COLLECT)
		{
			kputs("Error: too many sections to load");
			HALT;
		}
		section_collect[cnt_section_collect++] = &section[i];
	}

	ksort(section_collect,section_collect+cnt_section_collect,sizeof(*section_collect),cmp_section_LMA);

	// safe copy sections to `addr_segment`
	for(u32 i=0;i<cnt_section_collect;++i)
	{
		#define SIZE_EXCHANGE 32
		byte exchange[SIZE_EXCHANGE];

		u32 offset = section_collect[i]->sh_offset;
		u32 size_section = section_collect[i]->sh_size;
		KASSERT((i32)offset>0);

		u32 cnt_info_copy = info_copy[0]++;
		info_copy[cnt_info_copy*3+1] = section_collect[i]->sh_addr;
		info_copy[cnt_info_copy*3+2] = (u32)addr_segment;
		info_copy[cnt_info_copy*3+3] = size_section;

		for(u32 i=0;i<size_section/SIZE_EXCHANGE;++i)
		{
			kmemcpy(exchange,buffer+offset,SIZE_EXCHANGE);
			kmemmove(buffer+SIZE_EXCHANGE,buffer,offset);
			kmemcpy(addr_segment,exchange,SIZE_EXCHANGE);
			buffer+=SIZE_EXCHANGE, addr_segment+=SIZE_EXCHANGE;
		}
		u32 t = size_section%SIZE_EXCHANGE;
		kmemcpy(exchange,buffer+offset,t);
		kmemmove(buffer+(t/8*8),buffer,offset); // prevent misalign
		kmemcpy(addr_segment,exchange,t);
		buffer+=t/8*8, addr_segment+=t;

		for(u32 j=i+1;j<cnt_section_collect;++j)
		{
			if((byte*)section_collect[j]<buffer+offset)
				section_collect[j] = (Elf32_Shdr*)((byte*)section_collect[j]+size_section/8*8);
			if(section_collect[j]->sh_offset>offset)
				section_collect[j]->sh_offset-=size_section/8*8;
		}
	}
	return addr_segment;
}

void fix_copy_section(u32 *const info_copy)
{
	u32 cnt_info_copy = info_copy[0];
	for(u32 i=cnt_info_copy;i>0;--i)
		kmemmove((byte*)info_copy[(i-1)*3+1],(byte*)info_copy[(i-1)*3+2],info_copy[(i-1)*3+3]);
}

static inline bool cmp_module_start(void *x,void *y)
{
	return (*(struct multiboot_tag_module**)x)->mod_start<(*(struct multiboot_tag_module**)y)->mod_start;
}

void load_module()
{
	byte *addr_segment = (byte*)ADDR_SEGMENT;
	static u32 *const info_copy = (u32*)(ADDR_LOW_MEMORY + 11*1024);
	info_copy[0] = 0;

	ksort(module,module+cnt_module,sizeof(*module),cmp_module_start);
	for(u32 i=0;i<cnt_module;++i)
	{
		byte *const addr_module=(byte*)module[i]->mod_start;
		elf_init_LMA(addr_module);
		elf_relocate(addr_module);
		set_kernelCall(addr_module);
		addr_segment = collect_section(addr_module,addr_segment,info_copy);
	}
	fix_copy_section(info_copy);
}


void init_pageDirectory()
{
	kmemset(pageDirectory,0,4096);
	// Under the limit that init memory size <= 2MB,
	// Only one page directory needs setting in high memory area
	pageDirectory[ADDR_HIGH_MEMORY>>22] = (u32)pageTable|PDE_P|PDE_R;
}

void init_pageTable()
{
	kmemset(pageTable, 0, 1024);
	const u32 cnt_PTE = size_reserveMemory>>12;
	// ADDR_HIGH_MEMORY:ADDR_HIGH_MEMORY+size_reserveMemory -> ADDR_LOW_MEMORY:ADDR_LOW_MEMORY+size_reserveMemory
	// i is the high 20bit of offset
	for(register u32 i=0;i<cnt_PTE;++i)
		pageTable[i] = (ADDR_LOW_MEMORY+(i<<12))|PTE_P|PTE_R;
}

void init_page_temp()
{
	// Since this code locates in low memory area,
	// to keep running after enabling pages,
	// we need to establish a temporary page mapping
	// 0:ADDR_LOW_MEMORY+size_reserveMemory -> 0:ADDR_LOW_MEMORY+size_reserveMemory
	u32 *const pageTable_temp = (u32*)(ADDR_LOW_MEMORY+size_reserveMemory);

	const u32 cnt_PDE = align(ADDR_LOW_MEMORY+size_reserveMemory,22)>>22;
	for(register u32 i=0;i<cnt_PDE;++i)
		pageDirectory[i] = (u32)&pageTable_temp[i*1024]|PDE_P|PDE_R;

	const u32 cnt_PTE = (ADDR_LOW_MEMORY+size_reserveMemory)>>12;
	for(register u32 i=0;i<cnt_PTE;++i)
		pageTable_temp[i] = (i<<12)|PTE_P|PTE_R;
}

static inline void enable_page()
{
	asm volatile(
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
	kputs("##pageTable");
	for(u32 i=0;i<32;++i)
	{
		kprintf("%x ",pageTable[i]);
		if(!(~i&3)) kputchar('\n');
	}
	kputs("##pageTable_temp");
	for(u32 i=0;i<32;++i)
	{
		kprintf("%x ",pageTable[i+1024]);
		if(!(~i&3)) kputchar('\n');
	}
	DEBUG_BREAKPOINT;
}


typedef void (*handle_tag_t)(struct multiboot_tag*); 
handle_tag_t handle_tag[7]={
	[MULTIBOOT_TAG_TYPE_MODULE]=(handle_tag_t)handle_tag_module,
	[MULTIBOOT_TAG_TYPE_MMAP]=(handle_tag_t)handle_tag_mmap,
};

void init_memory_(u32 magic,u32 mbi)
{
	kputs("DappurOS initializing...");
	kprintf("magic: %x\n",magic);
	if(magic!=MULTIBOOT2_BOOTLOADER_MAGIC) return;
	kprintf("mbi: %p\n",mbi);
	if(mbi&7) return;

	// handle multiboot information
	kmemcpy((byte*)ADDR_MBI,(byte*)mbi,*(u32*)mbi);
	struct multiboot_tag *tag=(struct multiboot_tag*)(ADDR_MBI+8);
	while(tag->type!=MULTIBOOT_TAG_TYPE_END)
	{
		kprintf("tag: %d\n",tag->type);
		if(tag->type<7 && handle_tag[tag->type])
		{
			kputs("handled");
			handle_tag[tag->type](tag);
		}
		tag=(struct multiboot_tag*)align((u32)tag+tag->size,__builtin_ctz(MULTIBOOT_INFO_ALIGN));
	}

	// initialize all page settings (the allocation will happens later)
	init_pageDirectory();
	init_pageTable();
	kmemset(kernelCallTable,0,256);

	load_module();

	// preparing the information passing to the `control` module
	kmemset(bootInfo, 0, 1024);
	// bootInfo[0..3]: size_reserveMemory
	*(u32*)&bootInfo[0] = size_reserveMemory;

	// enable pages
	init_page_temp();
	enable_page();
	kernelCallTable[MODULE_TYPE_CONTROL](KERNEL_CALL_INIT); // let's get kernel started
}


void init_memory(u32 magic,u32 mbi)
{
	init_memory_(magic,mbi);
	// `init_memory_` should not return, except it failed
	kputs("Startup has failed");
}