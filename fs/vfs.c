#include "../include/vfs.h"
#include "../include/memory.h"
#include "../include/string.h"

#define MAX_MOUNTS 16

mount_info_t **mount_points = 0;
int last_mount_id = 0;

device_t *check_mount(char *loc) {
	for(int i = 0; i < last_mount_id; i++)
	{
		if(strcmp(loc, mount_points[i]->loc) == 0)
		{
			return mount_points[i]->fs;
		}
	}
	return 0;
}

// void vfs_mount(device_t *dev, char *loc) {
//     if(!dev || !(dev->unique_id)) return 0;
//     if(check_mount(loc)) return 0;
// }

void vfs_mount_filesystem(filesystem_t *fs, char *loc) {
    if(check_mount(loc)) return 0;
    mount_info_t *m = (mount_info_t *)malloc(sizeof(mount_info_t));
    m->loc = loc;
    m->fs = fs;
    last_mount_id++;
    mount_points[last_mount_id-1] = m;
    return 1;
}

void vfs_init() {
    mount_points = (mount_info_t **)malloc(sizeof(uint32_t) * MAX_MOUNTS);
}