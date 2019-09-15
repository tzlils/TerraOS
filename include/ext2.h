#ifndef EXT2_H
#define EXT2_H

#include "../include/stdint.h"
#include "../include/device.h"

#define EXT2_SIGNATURE 0xEF53
#define INODE_TYPE_FIFO 0x1000
#define INODE_TYPE_CHAR_DEV 0x2000
#define INODE_TYPE_DIRECTORY 0x4000
#define INODE_TYPE_BLOCK_DEV 0x6000
#define INODE_TYPE_FILE 0x8000
#define INODE_TYPE_SYMLINK 0xA000
#define INODE_TYPE_SOCKET 0xC000
typedef struct {
    uint32_t inodes;
    uint32_t blocks;
    uint32_t root_blocks;
    uint32_t unallocated_blocks;
    uint32_t unallocated_inodes;
    uint32_t parent_block;
    uint32_t blocksize_shift;
    uint32_t fragmentsize_shift;
    uint32_t blockgroup_blocks;
    uint32_t blockground_fragments;
    uint32_t blockground_inodes;
    uint32_t last_mount;
    uint32_t last_write;
    uint16_t mounts_since_check;
    uint16_t max_mounts_since_check;
    uint16_t ext2_sig;
    uint16_t state;
    uint16_t op_on_err;
    uint16_t minor_version;
    uint32_t last_check;
    uint32_t check_interval;
    uint32_t os_id;
    uint32_t major_version;
    uint16_t uuid;
    uint16_t gid;
} __attribute__((packed)) superblock_t;

typedef struct {
	uint32_t block_of_block_usage_bitmap;
	uint32_t block_of_inode_usage_bitmap;
	uint32_t block_of_inode_table;
	uint16_t num_of_unalloc_block;
	uint16_t num_of_unalloc_inode;
	uint16_t num_of_dirs;
	uint8_t unused[14];
} __attribute__((packed)) block_group_desc_t;

typedef struct {
	uint16_t type;
	uint16_t uid;
	uint32_t size;
	uint32_t last_access;
	uint32_t create_time;
	uint32_t last_modif;
	uint32_t delete_time;
	uint16_t gid;
	uint16_t hardlinks;
	uint32_t disk_sectors;
	uint32_t flags;
	uint32_t ossv1;
	uint32_t dbp[12];
	uint32_t singly_block;
	uint32_t doubly_block;
	uint32_t triply_block;
	uint32_t gen_no;
	uint32_t reserved1;
	uint32_t reserved2;
	uint32_t fragment_block;
	uint8_t ossv2[12];
} __attribute__((packed)) inode_t;

typedef struct {
	superblock_t sb;
	uint32_t first_bgd;
	uint32_t number_of_bgs;
	uint32_t blocksize;
	uint32_t sectors_per_block;
	uint32_t inodes_per_block;
} __attribute__((packed)) ext2_metadata;

typedef struct {
	uint32_t inode;
	uint16_t size;
	uint8_t namelength;
	uint8_t reserved;
	/* name here */
} __attribute__((packed)) ext2_dir;


uint8_t construct_ext2_filesystem(device_t *dev);
#endif