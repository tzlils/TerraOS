#ifndef DEVICE_H_
#define DEVICE_H_

#include "../include/stdint.h"
#include "../include/vfs.h"
typedef enum __device_type {
	DEVICE_UNKNOWN = 0,
	DEVICE_CHAR = 1,
	DEVICE_BLOCK = 2,
} device_type;

struct __fs_t;
typedef struct __device_t {
	char *name;
	uint32_t unique_id;
	device_type dev_type;
	struct __fs_t *fs;
	uint8_t (*read)(uint8_t* buffer, uint32_t offset , uint32_t len, void* dev);
	uint8_t (*write)(uint8_t *buffer, uint32_t offset, uint32_t len, void* dev);
	void *priv;
} device_t;

int device_add(device_t* dev);
device_t *device_get_by_id(uint32_t id);
void init_dev();
void dev_stats();

#endif