#ifndef VFS_H
#define VFS_H

#include "../include/device.h"
struct __device_t;

typedef struct __fs_t {
    char *name; 
    uint8_t (*probe)(struct __device_t *);
	uint8_t (*read)(char *, char *, struct __device_t *, void *);
	uint8_t (*read_dir)(char *, char *, struct __device_t *, void *);
	uint8_t (*touch)(char *fn, struct __device_t *, void *);
	uint8_t (*writefile)(char *fn, char *buf, uint32_t len, struct __device_t *, void *);
	uint8_t (*exist)(char *filename, struct __device_t *, void *);
	uint8_t (*mount)(struct __device_t *, void *);
	uint8_t *metadata;
} filesystem_t;

typedef struct __mount_info_t {
	char *loc;
	struct __device_t *dev;
} mount_info_t;

void vfs_init();
uint8_t vfs_mount_device(struct __device_t *dev, char *loc);
uint8_t list_mount();
uint32_t vfs_ls(char *dir, char* buffer);
uint8_t vfs_touch(char *filename);
#endif