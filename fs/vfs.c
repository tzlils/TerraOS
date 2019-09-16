#include "../include/vfs.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/stdio.h"

#define MAX_MOUNTS 16

mount_info_t **mount_points = 0;
int last_mount_id = 0;

device_t *check_mount(char *loc) {
	for(int i = 0; i < last_mount_id; i++)
	{
		if(strcmp(loc, mount_points[i]->loc) == 0)
		{
			return mount_points[i]->dev;
		}
	}
	return 0;
}

uint8_t list_mount() {
	for(int i = 0;i < MAX_MOUNTS;i ++) {
		if(!mount_points[i])break;
		printf("%s on %s type: %s\n", mount_points[i]->dev->name,
		 mount_points[i]->loc, mount_points[i]->dev->fs->name);
	}
	return 1;
}

// void vfs_mount(device_t *dev, char *loc) {
//     if(!dev || !(dev->unique_id)) return 0;
//     if(check_mount(loc)) return 0;
// }

void vfs_mount_device(device_t *dev, char *loc) {
    if(check_mount(loc)) return 0;
    mount_info_t *m = (mount_info_t *)kmalloc(sizeof(mount_info_t));
    m->loc = loc;
    m->dev = dev;
    last_mount_id++;
    mount_points[last_mount_id-1] = m;
    return 1;
}

void vfs_init() {
    mount_points = (mount_info_t **)kmalloc(sizeof(uint32_t) * MAX_MOUNTS);
}