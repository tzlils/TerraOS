#include "../include/ext2.h"
#include "../include/device.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/vfs.h"

static inode_t *inode = 0;
static uint8_t *root_buf = 0;
static uint8_t *block_buf = 0;
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
    uint32_t bg = (inode - 1) / data->sb.blockgroup_inodes;
    uint32_t i = 0;

    if(!block_buf) block_buf = (uint8_t *)kmalloc(data->blocksize);

    read_block(block_buf, data->first_bgd, dev, data);
    block_group_desc_t *bgd = (block_group_desc_t *)block_buf;
    for (i = 0; i < bg; i++)
        bgd++;

    uint32_t index = (inode - 1) % data->sb.blockgroup_inodes;
	uint32_t block = (index * sizeof(inode_t))/ data->blocksize;
	read_block(block_buf, bgd->block_of_inode_table + block, dev, data);
	inode_t* _inode = (inode_t *)block_buf;
	index = index % data->inodes_per_block;
	for(i = 0; i < index; i++)
		_inode++;
	/* We have found the inode! */
	memcpy(buf, _inode, sizeof(inode_t));
}

void write_inode(inode_t *inode_buf, uint32_t ii, device_t *dev, ext2_metadata *data) {
	uint32_t bg = (ii - 1) / data->sb.blockgroup_inodes;
	uint32_t i = 0;
	/* Now we have which BG the inode is in, load that desc */
	if(!block_buf) block_buf = (uint8_t *)kmalloc(data->blocksize);
	read_block(block_buf, data->first_bgd, dev, data);
	block_group_desc_t *bgd = (block_group_desc_t*)block_buf;
	/* Seek to the BG's desc */
	for(i = 0; i < bg; i++)
		bgd++;
	/* Find the index and seek to the inode */
	uint32_t index = (ii - 1) % data->sb.blockgroup_inodes;
	uint32_t block = (index * sizeof(inode_t))/ data->blocksize;
	uint32_t final = bgd->block_of_inode_table + block;
	read_block(block_buf, final, dev, data);
	inode_t* _inode = (inode_t *)block_buf;
	index = index % data->inodes_per_block;
	for(i = 0; i < index; i++)
		_inode++;
	/* We have found the inode! */
	memcpy(_inode, inode_buf, sizeof(inode_t));
	write_block(block_buf, final, dev, data);
}

uint32_t read_directory(char *filename, ext2_dir *dir, device_t *dev, ext2_metadata *data) {
	while(dir->inode != 0) {
		char *name = (char *)kmalloc(dir->namelength + 1);
		memcpy(name, &dir->reserved+1, dir->namelength);
		name[dir->namelength] = 0;
		//kprintf("DIR: %s (%d)\n", name, dir->size);
		if(filename && strcmp(filename, name) == 0)
		{
			/* If we are looking for a file, we had found it */
			read_inode(inode, dir->inode, dev, data);
			kfree(name);
			return dir->inode;
		}
		if(!filename && (uint32_t)filename != 1) {
			//mprint("Found dir entry: %s to inode %d \n", name, dir->inode);
		}
		dir = (ext2_dir *)((uint32_t)dir + dir->size);
		kfree(name);
	}
	return 0;
}

uint8_t read_root_directory(char *filename, device_t *dev, ext2_metadata *priv) {
	if(!inode) inode = (inode_t *)kmalloc(sizeof(inode_t));
	if(!root_buf) root_buf = (uint8_t *)kmalloc(priv->blocksize);
	read_inode(inode, 2, dev, priv);
	if((inode->type & 0xF000) != INODE_TYPE_DIRECTORY) return 0;
	/* We have found the directory!
	 * Now, load the starting block
	 */
	for(int i = 0;i < 12; i++)
	{
		uint32_t b = inode->dbp[i];
		if(b == 0) break;
		read_block(root_buf, b, dev, priv);
		/* Now loop through the entries of the directory */
		if(read_directory(filename, (ext2_dir*)root_buf, dev, priv)) return 1;
	}
	if(filename && (uint32_t)filename != 1) return 0;
	return 1;
}

uint8_t find_file_inode(char *ff, inode_t *inode_buf, device_t *dev, ext2_metadata *priv) {
	char *filename = kmalloc(strlen(ff) + 1);
	memcpy(filename, ff, strlen(ff) +1);
	size_t n = strsplit(filename, '/');
	filename ++; // skip the first crap
	uint32_t retnode = 0;
	if(n > 1)
	{ 
		/* Read inode#2 (Root dir) into inode */
		read_inode(inode, 2, dev, priv);
		/* Now, loop through the DPB's and see if it contains this filename */
		n--;
		while(n--)
		{
			for(int i = 0; i < 12; i++)
			{
				uint32_t b = inode->dbp[i];
				if(!b) break;
				read_block(root_buf, b, dev, priv);
				uint32_t rc = read_directory(filename, (ext2_dir *)root_buf, dev, priv);
				if(!rc)
				{
					if(strcmp(filename, "") == 0)
					{
						kfree(filename);
						return strcmp(ff, "/")?retnode:1;
					}
					kfree(filename);
					return 0;
				} else {
					/* inode now contains that inode
					 * get out of the for loop and continue traversing
					 */
					 retnode = rc;
					 goto fix;
				}
			}
			fix:;
			uint32_t s = strlen(filename);
			filename += s + 1;
		}
		memcpy(inode_buf, inode, sizeof(inode_t));
	} else {
		/* This means the file is in the root directory */
		read_root_directory(filename, dev, priv);
		memcpy(inode_buf, inode, sizeof(inode_t));
	}
	kfree(filename);
	return retnode;
}

void list_directory(char *dd, char *buffer, device_t *dev, ext2_metadata *data) {
	char *dir = dd;
	int rc = find_file_inode(dir, (inode_t *)buffer, dev, data);
	if(!rc) return;
	for(int i = 0;i < 12; i++)
	{
		uint32_t b = inode->dbp[i];
		if(!b) break;
		read_block(root_buf, b, dev, data);
		read_directory(0, (ext2_dir *)root_buf, dev, data);
	}
}

uint32_t get_inode_block(uint32_t inode, uint32_t *b, uint32_t *ioff, device_t *dev, ext2_metadata *priv) {
	uint32_t bg = (inode - 1) / priv->sb.blockgroup_inodes;
	uint32_t i = 0;
	/* Now we have which BG the inode is in, load that desc */
	if(!block_buf) block_buf = (uint8_t *)kmalloc(priv->blocksize);
	read_block(block_buf, priv->first_bgd, dev, priv);
	block_group_desc_t *bgd = (block_group_desc_t*)block_buf;
	/* Seek to the BG's desc */
	for(i = 0; i < bg; i++)
		bgd++;
	/* Find the index and seek to the inode */
	uint32_t index = (inode - 1) % priv->sb.blockgroup_inodes;
	uint32_t block = (index * sizeof(inode_t))/ priv->blocksize;
	index = index % priv->inodes_per_block;
	*b = block + bgd->block_of_inode_table;
	*ioff = index;
	return 1;
}

uint8_t read_singly_linked(uint32_t blockid, uint8_t *buf, device_t *dev, ext2_metadata *priv)
{
	uint32_t blockadded = 0;
	uint32_t maxblocks = ((priv->blocksize) / (sizeof(uint32_t)));
	/* A singly linked block is essentially an array of
	 * uint32_t's storing the block's id which points to data
	 */
	 /* Read the block into root_buf */
	 read_block(root_buf, blockid, dev, priv);
	 /* Loop through the block id's reading them into the appropriate buffer */
	 uint32_t *block = (uint32_t *)root_buf;
	 for(int i =0;i < maxblocks; i++)
	 {
	 	/* If it is zero, we have finished loading. */
	 	if(block[i] == 0) break;
	 	/* Else, read the block into the buffer */
	 	read_block(buf + i * priv->blocksize, block[i], dev, priv);
	 }
	 return 1;
}

uint8_t read_doubly_linked(uint32_t blockid, uint8_t *buf, device_t *dev, ext2_metadata *priv) {
	uint32_t blockadded = 0;
	uint32_t maxblocks = ((priv->blocksize) / (sizeof(uint32_t)));
	/* A singly linked block is essentially an array of
	 * uint32_t's storing the block's id which points to data
	 */
	 /* Read the block into root_buf */
	 read_block(block_buf, blockid, dev, priv);
	 /* Loop through the block id's reading them into the appropriate buffer */
	 uint32_t *block = (uint32_t *)block_buf;
	 uint32_t s = (priv->blocksize * priv->blocksize / 4);
	 for(int i =0;i < maxblocks; i++)
	 {
	 	/* If it is zero, we have finished loading. */
	 	if(block[i] == 0) break;
	 	/* Else, read the block into the buffer */
	 	read_singly_linked(block[i], buf + i * s , dev, priv);
	 }
	 return 1;
}

static inode_t *minode = 0;
uint8_t read_file(char *fn, char *buffer, device_t *dev, ext2_metadata *data) {
	if(!minode) minode = (inode_t *)kmalloc(sizeof(inode_t));
	char *filename = fn;
	if(!find_file_inode(filename, minode, dev, data)) return 0;
	for(int i = 0; i < 12; i++)
	{
		uint32_t b = minode->dbp[i];
		if(b > data->sb.blocks) PANIC("%s: block %d outside range (max: %d)!\n", __func__,
				b, data->sb.blocks);

		read_block(root_buf, b, dev, data);
		memcpy(buffer + i*(data->blocksize), root_buf, data->blocksize);
	}
	if(minode->singly_block) {
		read_singly_linked(minode->singly_block, buffer + 12*(data->blocksize), dev, data);
	}
	if(minode->doubly_block) {
		uint32_t s = (data->blocksize * data->blocksize / 4) + 12*data->blocksize;
		read_doubly_linked(minode->doubly_block, buffer + s, dev, data);
	}
	return 1;
}

void find_new_inode_id(uint32_t *id, device_t *dev, ext2_metadata *priv)
{
	read_block(root_buf, priv->first_bgd, dev, priv);
	block_group_desc_t *bg = (block_group_desc_t *)root_buf;
	for(int i = 0; i < priv->number_of_bgs; i++)
	{
		if(bg->num_of_unalloc_block)
		{
			 *id = ((i + 1) * priv->sb.blockgroup_inodes) - bg->num_of_unalloc_inode + 1;
			 bg->num_of_unalloc_inode --;
			 write_block(root_buf, priv->first_bgd + i, dev, priv);
			 /* Now, update the superblock as well */
			 read_block(root_buf, priv->sb.parent_block, dev, priv);
			 superblock_t *sb = (superblock_t *)root_buf;
			 sb->unallocated_inodes --;
			 write_block(root_buf, priv->sb.parent_block, dev, priv);
			 return;
		}
		bg++;
	}
}


void alloc_block(uint32_t *out, device_t *dev, ext2_metadata *priv) {
	 read_block(root_buf, priv->first_bgd, dev, priv);
	 block_group_desc_t *bg = (block_group_desc_t *)root_buf;
	 for(int i = 0; i < priv->number_of_bgs; i++)
	 {
	 	if(bg->num_of_unalloc_block)
	 	{
	 		*out = priv->sb.blocks - bg->num_of_unalloc_block + 1;
	 		bg->num_of_unalloc_block --;
	 		write_block(root_buf, priv->first_bgd + i, dev, priv);


	 		read_block(root_buf, priv->sb.parent_block, dev, priv);
			superblock_t *sb = (superblock_t *)root_buf;
			sb->unallocated_inodes --;
			write_block(root_buf, priv->sb.parent_block, dev, priv);
			return;
	 	}
	 	bg++;
	 }
}

uint8_t touch(char *file, device_t *dev, ext2_metadata *priv) {
	if(!dev->write)
		return 0;
	/* file = "/levex.txt"; */
	/* First create the inode */
	char *fil = (char *)kmalloc(strlen(file) + 1);
	memcpy(fil, file, strlen(file) + 1);
	inode_t *fi = (inode_t *)kmalloc(sizeof(inode_t));
	fi->hardlinks = 1;
	fi->size = 0;
	fi->type = INODE_TYPE_FILE;
	fi->disk_sectors = 2;
	/* Create the directory entry */
	size_t n = strsplit(fil, '/');
	n--;
	while(n)
	{
		fil += strlen(fil) + 1;
		n--;
	}
	//kprintf("filename: %s\n", fil);
		
	ext2_dir *entry = (ext2_dir *)kmalloc(sizeof(ext2_dir) + strlen(fil) + 1);
	entry->size = sizeof(ext2_dir) + strlen(fil) + 1;
	entry->namelength = strlen(fil) + 1;
	entry->reserved = 0;
	memcpy(&entry->reserved + 1, fil, strlen(fil) + 1);
	uint32_t id = 0;
	find_new_inode_id(&id, dev, priv);
	entry->inode = id;
	uint32_t block = 0; /* The block where this inode should be written */
	uint32_t ioff = 0; /* Offset into the block function to sizeof(inode_t) */
	get_inode_block(id, &block, &ioff, dev, priv);
	read_block(root_buf, block, dev, priv);
	inode_t *winode = (inode_t *)root_buf;
	for(int i = 0;i < ioff; i++)
		winode++;
	memcpy(winode, fi, sizeof(inode_t));
	write_block(root_buf, block, dev, priv);
	char *f = (char *)kmalloc(strlen(file) + 1);
	memcpy(f, file, strlen(file) + 1);
	str_backspace(f, '/');


	//kprintf("LF: %s\n", f);
	if(!inode) inode = (inode_t *)kmalloc(sizeof(inode_t));
	if(!block_buf) block_buf = (uint8_t *)kmalloc(priv->blocksize);
	uint32_t t = find_file_inode(f, inode, dev, priv);
	t++;
	uint8_t found = 0;
	for(int i = 0; i < 12; i++)
	{
		if(inode->dbp[i] == 0)
		{
			uint32_t theblock = 0;
			alloc_block(&theblock, dev, priv);
			inode->dbp[i] = theblock;
			write_inode(inode, t, dev, priv);
 		}
		read_block(block_buf, inode->dbp[i], dev, priv);
		ext2_dir *d = (ext2_dir *)block_buf;
		uint32_t passed = 0;
		while(d->inode != 0) {
			if(d->size == 0) break;
			uint32_t truesize = d->namelength + 8;
			truesize += 4 - truesize % 4;
			uint32_t origsize = d->size;
			if(truesize != d->size)
			{
				d->size = truesize;
				passed += d->size;
				d = (ext2_dir *)((uint32_t)d + d->size);
				entry->size = priv->blocksize - passed;
				break;
			}
			passed += d->size;
			d = (ext2_dir *)((uint32_t)d + d->size);
		}

		if(passed >= priv->blocksize)
		{
			continue;
		}
	dir_write:
		memcpy(d, entry, entry->size);
		write_block(block_buf, inode->dbp[i], dev, priv);
		return 1;
	}
	return 0;
}

uint8_t writefile(char *fn, char *buf, uint32_t len, device_t *dev, ext2_metadata *priv) {
	/* Locate and load the inode */
	uint32_t inode_id = find_file_inode(fn, inode, dev, priv);
	inode_id ++;
	if(inode_id == 1) return 0;
	if(!inode) inode = (inode_t *)kmalloc(sizeof(inode_t));
	read_block(inode, inode_id, dev, priv);
	/* Check if it is of type INODE_TYPE_FILE */
	/*if(! (inode->type & INODE_TYPE_FILE))
	{
		/* -- If no, bail out. 
		kprintf("Not a file.\n");
		return 0;
	}*/
	/* If inode->size == 0 */
	if(inode->size == 0)
	{
		/* Allocate len / priv->blocksize amount of blocks. */
		uint32_t blocks_to_alloc = len / priv->blocksize;
		blocks_to_alloc ++; /* Allocate atleast one! */
		if(blocks_to_alloc > 12)
		{
			/* @todo */
			return 0;
		}
		for(int i = 0; i < blocks_to_alloc; i++)
		{
			uint32_t bid = 0;
			alloc_block(&bid, dev, priv);
			inode->dbp[i] = bid;
			//kprintf("Set dbp[%d] to %d\n", i, inode->dbp[i]);
		}
		inode->size += len; // UPDATE the size
		/* Commit the inode to the disk */
		write_inode(inode, inode_id - 1, dev, priv);
		/* Write the buf to the blocks. */
		for(int i = 0; i < blocks_to_alloc; i++)
		{
			/* We loop through the blocks and write. */
			read_block(root_buf, inode->dbp[i], dev, priv);
			if(i + 1 < blocks_to_alloc) { // If not last block
				memcpy(root_buf, buf + i*priv->blocksize, priv->blocksize);
			} else {// If last block
				memcpy(root_buf, buf + i*priv->blocksize, len);
			}
			write_block(root_buf, inode->dbp[i], dev, priv);
		}
		return 1;
	}
	/* Else, check which block has the last byte, by
	 *   dividing inode->size by priv->blocksize.
	 */
	uint32_t last_data_block = inode->size / priv->blocksize;
	uint32_t last_data_offset = (inode->size) % priv->blocksize;
	/* Load that block. */
	read_block(root_buf, last_data_block, dev, priv);
	/* If len < priv->blocksize - (inode->size)%priv->blocksize
	 */
	if(len < priv->blocksize - last_data_offset)
	{
		/*    which means that the buf can fill the block. */
		/* Write and return noerror.*/
		memcpy(root_buf + last_data_offset, buf, len);
		write_block(root_buf, last_data_block, dev, priv);
		return 1;
	}
	/*Else,
	 * --- Write the maximum possible bytes to the block.
	 * --- The next block doesn't exist. Allocate a new one.
	 * --- Write the rest to that block and repeat.
	 */
	/*uint32_t data_wrote = 0;
	memcpy(root_buf + last_data_offset, buf, priv->blocksize - last_data_offset);
	data_wrote += priv->blocksize - last_data_offset;*/

 	return 0;
}

uint8_t exist(char *file, device_t *dev, ext2_metadata *priv) {
	return read_file(file, 0, dev, priv);
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
	fs->read = (uint8_t(*)(char *, char *, device_t *, void *)) read_file;
	fs->exist = (uint8_t(*)(char *, device_t*, void *)) exist;
	fs->read_dir = (uint8_t(*)(char * , char *, device_t *, void *)) list_directory;
	fs->touch = (uint8_t(*)(char *, device_t *, void *)) touch;
	fs->writefile = (uint8_t(*)(char *, char *m, uint32_t, device_t *, void *)) writefile;
	fs->metadata = (void *)metadata;
    dev->fs = fs;
    kfree(buf);
    return 1;
}