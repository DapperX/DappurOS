#include "ext2.h"

void output_ext2_info(byte *buffer)
{
	ext2_superBlock *superBlock=&buffer[2048];
	printf("cnt_inode%d\n",superBlock->cnt_inode);
	printf("cnt_block%d\n",superBlock->cnt_block);
	printf("cnt_block_reserve%d\n",superBlock->cnt_block_reserve);
	printf("cnt_block_free%d\n",superBlock->cnt_block_free);
	printf("cnt_inode_free%d\n",superBlock->cnt_inode_free);
	printf("index_data_start%d\n",superBlock->index_data_start);
}