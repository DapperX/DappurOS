#include "base.h"

#define EXT2_MAX_LEN_NAME 255

struct ext2_superBlock{
	le32 cnt_inode;
	le32 cnt_block;
	le32 cnt_block_reserve;
	le32 cnt_block_free;
	le32 cnt_inode_free;
	le32 index_data_start;
	le32 size_block;
	le32 size_fragment;
	le32 cnt_block_pre_group;
	le32 cnt_fragment_pre_group;
	le32 time_mount;
	le32 time_write;
	le16 cnt_mount;
	le16 max_mount;
	le16 magic;
	le16 state;
	le16 handle_error;
	le16 reversion_minor;
	le32 last_check;
	le32 max_interval_check;
	le32 os_creator;
	le32 reversion;
	le16 UID_reserve;
	le16 GID_reserve;
	
	le32 inode_first;
	le16 size_inode;
	le16 group_now;
	le32 feature_compatible;
	le32 feature_incompatible;
	u8 uuid[16];
	char name_volume[16];
	char dir_mount_last[64];
	le32 algo_usage_bitmap;
	
	u8 cnt_block_prealloc;
	u8 cnt_dir_prealloc;
	u16 padding;
	
	u32 reserve[190];
}


struct ext2_group_descriptor{
	le32 block_bitmap;
	le32 inode_bitmap;
	le32 inode_table;
	le16 cnt_blocks_free;
	le16 cnt_inode_free;
	le16 cnt_dir_used;
	le16 pad;
	le32 reserve[3];
}


struct ext2_inode{
	le16 mode;
	le16 uid;
	le32 size; //file length, byte
	le32 time_access;
	le32 time_create;
	le32 time_modify;
	le32 time_delete;
	le16 gid;
	le16 cnt_link;
	le32 cnt_block;//file length, block
	le32 flag;
}


struct ext2_dir{
	le32 inode;
	le16 size_record;
	u8 len_name;
	u8 type;
	char name[EXT2_MAX_LEN_NAME];
}