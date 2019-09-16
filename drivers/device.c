#include "../include/device.h"
#include "../include/kheap.h"
#include "../include/stdio.h"
#include "../include/string.h"

device_t *devices = 0;
uint8_t lastid = 0;

void init_dev() {
    devices = (device_t*)kmalloc(64*sizeof(device_t));
	memset(devices, 0, 64*sizeof(device_t));
}

void dev_stats() {
	for(int i = 0; i < lastid; i++)
	{
		//if(!devices[lastid]) return;
		printf("id: %d, unique: %d, %s, %s\n", i, devices[i].unique_id,
				devices[i].dev_type == DEVICE_CHAR ?"CHAR":"BLOCK", devices[i].name);
	}
}

int device_add(device_t* dev)
{
	devices[lastid] = *dev;
	printf("Registered Device %s (%d) as Device#%d\n", dev->name, dev->unique_id, lastid);
	lastid++;
	return lastid-1;
}

device_t *device_get_by_id(uint32_t id)
{
	for(int i = 0;i < 64; i++)
	{
		if(devices[i].unique_id == id) return &devices[i];
	}
	return 0;
}