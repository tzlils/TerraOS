#include "../include/ext2.h"
#include "../include/device.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/vfs.h"

void read_block(uint8_t *buf, uint32_t block, device_t *dev, ext2_metadata *data) {
    uint32_t sectors_per_block = data->sectors_per_block;
    if(!sectors_per_block) sectors_per_block = 1;
    dev->read(buf, block*sectors_per_block, sectors_per_block, dev);
}

void write_block(uint8_t *buf, uint32_t block, device_t *dev, ext2_metadata *data) {
    uint32_t sectors_per_block = data->sectors_per_block;
    if(!sectors_per_block) sectors_per_block = 1;
    dev->write(buf, block*sectors_per_block, sectors_per_block, dev);
}

void read_inode(uint8_t *buf, uint32_t inode, device_t *dev, ext2_metadata *data) {
    uint32_t bg = (inode - 1) / data->sb.blockground_inodes;
    uint32_t i = 0;

    uint8_t *block_buf = (uint8_t *)kmalloc(data->blocksize);

    read_block(block_buf, data->first_bgd, dev, data);
    block_group_desc_t *bgd = (block_group_desc_t *)block_buf;
    for (i = 0; i < bg; i++)
        bgd++;
    
}

uint8_t construct_ext2_filesystem(device_t *dev) {
    if(!dev->read) return 0;
    uint8_t *buf = (uint8_t *)kmalloc(1024);
    dev->read(buf, 2, 2, dev);
    superblock_t *sb = (superblock_t *)buf;
    if(sb->ext2_sig != EXT2_SIGNATURE) return 0;
    filesystem_t *fs = (filesystem_t *)kmalloc(sizeof(filesystem_t));
    ext2_metadata *metadata = (ext2_metadata *)kmalloc(sizeof(ext2_metadata));
    memcpy(&metadata->sb, sb, sizeof(superblock_t));
    
    uint32_t blocksize = 1024 << sb->blocksize_shift;
    metadata->blocksize = blocksize;
    metadata->inodes_per_block = blocksize / sizeof(inode_t);
    metadata->sectors_per_block = blocksize / 512;

    uint32_t number_of_blockgroups = sb->blocks / sb->blockgroup_blocks;
    if(!number_of_blockgroups) number_of_blockgroups = 1;
    metadata->number_of_bgs = number_of_blockgroups;

    uint32_t block_bgdt = sb->parent_block + (sizeof(superblock_t) / blocksize);
    metadata->first_bgd = block_bgdt;
    fs->name = dev->name;
	// fs->probe = (uint8_t(*)(device_t*)) ext2_probe;
	// fs->mount = (uint8_t(*)(device_t*, void *)) ext2_mount;
	// fs->read = (uint8_t(*)(char *, char *, device_t *, void *)) ext2_read_file;
	// fs->exist = (uint8_t(*)(char *, device_t*, void *)) ext2_exist;
	// fs->read_dir = (uint8_t(*)(char * , char *, device_t *, void *)) ext2_list_directory;
	// fs->touch = (uint8_t(*)(char *, device_t *, void *)) ext2_touch;
	// fs->writefile = (uint8_t(*)(char *, char *m, uint32_t, device_t *, void *)) ext2_writefile;
	fs->metadata = (void *)metadata;
    dev->fs = fs;
    kfree(buf);
    return 1;
}