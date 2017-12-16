#include "base.h"

#define EXT2_MAX_LEN_NAME 255
#define EXT2_CNT_BLOCK_PER_INODE 15

//Definition of Reserved Inodes
#define EXT2_BAD_INO 1//bad blocks inode
#define EXT2_ROOT_INO 2//root directory inode
#define EXT2_ACL_IDX_INO 3//ACL index inode (deprecated?)
#define EXT2_ACL_DATA_INO 4//ACL data inode (deprecated?)
#define EXT2_BOOT_LOADER_INO 5//boot loader inode
#define EXT2_UNDEL_DIR_INO 6//undelete directory inode

//Definition of Inode File Type Values

#define EXT2_FT_UNKNOWN 0//Unknown File Type
#define EXT2_FT_REG_FILE 1//Regular File
#define EXT2_FT_DIR 2//Directory File
#define EXT2_FT_CHRDEV 3//Character Device
#define EXT2_FT_BLKDEV 4//Block Device
#define EXT2_FT_FIFO 5//Buffer File
#define EXT2_FT_SOCK 6//Socket File
#define EXT2_FT_SYMLINK 7//Symbolic Link

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
	le32 cnt_inode_pre_group;
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
	le32 feature_compatible_readonly;
	u8 uuid[16];
	char name_volume[16];
	char dir_mount_last[64];
	le32 algo_usage_bitmap;
	
	u8 cnt_block_prealloc;
	u8 cnt_dir_prealloc;
	u16 padding;
	
//	u32 reserve[190];
};


struct ext2_group_descriptor{
	le32 block_bitmap;
	le32 inode_bitmap;
	le32 inode_table;
	le16 cnt_blocks_free;
	le16 cnt_inode_free;
	le16 cnt_dir_used;
	le16 pad;
	le32 reserve[3];
};


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
	byte osd1[4];
	le32 block[EXT2_CNT_BLOCK_PER_INODE];
	le32 generation;//file version, used for NAS
	le32 ACL_file;
	le32 ACL_dir;
	le32 addr_fragment;
	byte osd2[12];
};


struct ext2_dir{
	le32 inode;
	le16 size_record;
	u8 len_name;
	u8 type;
	char name[EXT2_MAX_LEN_NAME];
};