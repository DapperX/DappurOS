#include "multiboot.h"
#include "base.h"

void handle_tag_cmdline(struct multiboot_tag_string *tag)
{
	kprintf("Boot command line: %s\n",tag->string);
	kputchar('\n');
}

void handle_tag_bootLoaderName(struct multiboot_tag_string *tag)
{
	kprintf("Boot loader name: %s\n",tag->string);
	kputchar('\n');
}

void handle_tag_module(struct multiboot_tag_module *tag)
{
	kprintf("Modules: %s\n",tag->cmdline);
	kprintf("%d %d\n",tag->mod_start,tag->mod_end);
	kputchar('\n');
}

void handle_tag_basicMemInfo(struct multiboot_tag_basic_meminfo *tag)
{
	kputs("Basic memory information");
	kprintf("mem_lower: %d\n",tag->mem_lower);
	kprintf("mem_upper: %d\n",tag->mem_upper);
	kputchar('\n');
}

void handle_tag_bootdev(struct multiboot_tag_bootdev *tag)
{
	kprintf("BIOS Boot device booted by: %x\n",tag->biosdev);
	kprintf("partition: %x\n",tag->partition);
	kprintf("sub_partition: %x\n",tag->sub_partition);
	kputchar('\n');
}

void handle_tag_mmap(struct multiboot_tag_mmap *tag)
{
	kprintf("Memory map with %d items\n",(tag->size-sizeof(struct multiboot_tag_mmap)+sizeof(tag->entries))/tag->entry_size);
	if(tag->entry_version!=0) kputs("Warning: mmap version may be incompatible");
	char *end=(char*)tag+tag->size;
	for(multiboot_memory_map_t *entry=tag->entries;(char*)entry<end;entry=(multiboot_memory_map_t*)((char*)entry+tag->entry_size))
		kprintf("%p %d %d\n",(uint)entry->addr,(uint)entry->len,entry->type);
	kputchar('\n');
}

void handle_tag_VBE(struct multiboot_tag_vbe *tag)
{
	kputs("VBE info\n");
}

void handle_tag_framebuffer(struct multiboot_tag_framebuffer *tag)
{
	kputs("Framebuffer info\n");
}

void handle_tag_elf_sections(struct multiboot_tag_elf_sections *tag)
{
	kputs("ELF-Symbols\n");
}

void handle_tag_APM(struct multiboot_tag_apm *tag)
{
	kputs("APM table\n");
}

typedef void (*handle_tag_t)(struct multiboot_tag*); 
handle_tag_t handle_tag[12]={
	[1]=(handle_tag_t)handle_tag_cmdline,
	[2]=(handle_tag_t)handle_tag_bootLoaderName,
	[3]=(handle_tag_t)handle_tag_module,
	[4]=(handle_tag_t)handle_tag_basicMemInfo,
	[5]=(handle_tag_t)handle_tag_bootdev,
	[6]=(handle_tag_t)handle_tag_mmap,
	[7]=(handle_tag_t)handle_tag_VBE,
	[8]=(handle_tag_t)handle_tag_framebuffer,
	[9]=(handle_tag_t)handle_tag_elf_sections,
	[10]=(handle_tag_t)handle_tag_APM,
};

uint align8(uint p)
{
	return (p+7u)&(~7u);
}

void init_c_start(uint eax,uint ebx)
{
	kputs("DappurOS initializing...");
	kprintf("eax: %x\n",eax);
	if(eax!=MULTIBOOT2_BOOTLOADER_MAGIC) return;
	kprintf("ebx: %p\n",ebx);
	if(ebx&7) return;
	kprintf("MBI size: %dbytes\n",*(multiboot_uint32_t*)ebx);

	struct multiboot_tag *tag=(struct multiboot_tag*)(ebx+8);
	while(tag->type!=MULTIBOOT_TAG_TYPE_END)
	{
		if(tag->type>10||tag->type<1)
		{
			kprintf("##Unknown type %d\n",tag->type);
			return;
		}
		if(handle_tag[tag->type]) handle_tag[tag->type](tag);
		tag=(struct multiboot_tag*)align8((uint)tag+tag->size);
	}
}