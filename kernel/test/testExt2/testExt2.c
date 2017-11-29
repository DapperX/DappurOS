#include "stdio.h"
#include "base.h"
#include "ext2.h"
#define SIZE_BUFFER 2048

static byte buffer[SIZE_BUFFER];

void output_ext2_info(byte *buffer)
{
	struct ext2_superBlock *superBlock=(struct ext2_superBlock*)&buffer[0];
	printf("cnt_inode: %d\n",superBlock->cnt_inode);
	printf("cnt_block: %d\n",superBlock->cnt_block);
	printf("cnt_block_reserve: %d\n",superBlock->cnt_block_reserve);
	printf("cnt_block_free: %d\n",superBlock->cnt_block_free);
	printf("cnt_inode_free: %d\n",superBlock->cnt_inode_free);
	printf("index_data_start: %d\n",superBlock->index_data_start);
}

int main()
{
	FILE *img=fopen("ext2.img","rb+");
	if(!img)
	{
		fputs("Failed to open img",stderr);
		goto exit_main;
	}
	//start at 1K to skip the boot section
	fseek(img,1024,SEEK_SET);
	fread(buffer,1,SIZE_BUFFER,img);
	output_ext2_info(buffer);
	fclose(img);
exit_main:
	return 0;
}