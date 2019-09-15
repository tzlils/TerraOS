#ifndef VFS_H
#define VFS_H

#include "../include/device.h"
struct __device_t;
typedef struct {
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
	filesystem_t *fs;
} mount_info_t;

#endif