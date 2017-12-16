#include "stdio.h"
#include "string.h"
#include "base.h"
#include "ext2.h"
#define SIZE_BUFFER (1024*50)

static byte buffer[SIZE_BUFFER];

void repeat_char(char c,u32 times)
{
	while(times--) putchar(c);
}

void output_string(char *s,u32 len)
{
	for(u32 i=0;i<len;++i) putchar(s[i]);
}

struct ext2_superBlock* output_ext2_superBlock(byte *buffer)
{
	// start from 1KB because of boot sector
	struct ext2_superBlock *superBlock=(struct ext2_superBlock*)&buffer[1024];
	printf("cnt_inode: %d\n",superBlock->cnt_inode);
	printf("cnt_block: %d\n",superBlock->cnt_block);
	printf("cnt_block_reserve: %d\n",superBlock->cnt_block_reserve);
	printf("cnt_block_free: %d\n",superBlock->cnt_block_free);
	printf("cnt_inode_free: %d\n",superBlock->cnt_inode_free);
	printf("index_data_start: %d\n",superBlock->index_data_start);
	printf("size_block: %d\n",superBlock->size_block);
	printf("size_fragment: %d\n",superBlock->size_fragment);
	printf("cnt_block_pre_group: %d\n",superBlock->cnt_block_pre_group);
	printf("cnt_fragment_pre_group: %d\n",superBlock->cnt_fragment_pre_group);
	printf("cnt_inode_pre_group: %d\n",superBlock->cnt_inode_pre_group);
	printf("time_mount: %d\n",superBlock->time_mount);
	printf("time_write: %d\n",superBlock->time_write);
	printf("cnt_mount: %d\n",superBlock->cnt_mount);
	printf("max_mount: %d\n",superBlock->max_mount);
	printf("magic: %X\n",superBlock->magic);
	printf("UUID: ");
	for(int i=0;i<16;++i) printf("%x",superBlock->uuid[i]);
	putchar('\n');
	printf("inode_first: %d\n",superBlock->inode_first);

	printf("size of superBlock: %d\n",sizeof(*superBlock));
	return superBlock;
}

struct ext2_inode* get_ext2_inode(byte *buffer,struct ext2_superBlock* superBlock)
{
	u32 size_block=(1u<<superBlock->size_block)*1024;
	printf("size_block: %d\n",size_block);
	struct ext2_group_descriptor *grpDesc=(struct ext2_group_descriptor*)&buffer[size_block*(superBlock->index_data_start+1)];
	u32 cnt_grpDesc=(superBlock->cnt_inode+superBlock->cnt_inode_pre_group-1)/superBlock->cnt_inode_pre_group;
	if(cnt_grpDesc!=(superBlock->cnt_block+superBlock->cnt_block_pre_group-1)/superBlock->cnt_block_pre_group)
	{
		puts("*WARNING*: cnt_grpDesc_not_equal");
	}
	printf("id_inode_table %d\n",grpDesc->inode_table);
	struct ext2_inode *inode=(struct ext2_inode*)&buffer[grpDesc->inode_table*size_block];
	return inode;
}

void output_ext2_dir(const byte *buffer,const struct ext2_superBlock* superBlock,struct ext2_inode *inode,struct ext2_inode *inode_now,u32 depth)
{
	register u32 size_block=(1u<<superBlock->size_block)*1024;
	// In this simple test, I just think that the directory consists of only one block
	struct ext2_dir *dir=(struct ext2_dir*)&buffer[inode_now->block[0]*size_block];
	byte *end_dir=(byte*)dir+inode_now->size;
	for(;(byte*)dir<end_dir;dir=(struct ext2_dir*)((byte*)dir+dir->size_record))
	{
		if(dir->type==EXT2_FT_UNKNOWN) continue;

		repeat_char(' ',depth*4);
		output_string(dir->name,dir->len_name);
		// [indent]filename{inode}[filesize]
		printf(" {%d}[%d]",dir->inode,inode[dir->inode-1].size);
		putchar('\n');

		if(dir->type==EXT2_FT_DIR)
		{
			if(!strcmp(dir->name,".")) continue;
			if(!strcmp(dir->name,"..")) continue;
			output_ext2_dir(buffer,superBlock,inode,&inode[dir->inode-1],depth+1);
		}
	}
}

int main()
{
	FILE *img=fopen("ext2.img","rb+");
	if(!img)
	{
		fputs("Failed to open img",stderr);
		goto exit_main;
	}
	fread(buffer,1,SIZE_BUFFER,img);
	fclose(img);

	struct ext2_superBlock* superBlock=output_ext2_superBlock(buffer);
	struct ext2_inode *inode=get_ext2_inode(buffer,superBlock);
	output_ext2_dir(buffer,superBlock,inode,&inode[EXT2_ROOT_INO-1],0);

exit_main:
	return 0;
}