#include "../include/vfs.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/utils.h"

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
		if(!mount_points[i]) break;
		printf("%s mounted on %s\n", mount_points[i]->dev->name, mount_points[i]->loc);
		// printf("%s on %s type: %s\n", mount_points[i]->dev->name,
		//  mount_points[i]->loc, mount_points[i]->dev->fs->name);
	}
	return 1;
}

// void vfs_mount(device_t *dev, char *loc) {
//     if(!dev || !(dev->unique_id)) return 0;
//     if(check_mount(loc)) return 0;
// }

uint8_t vfs_mount_device(device_t *dev, char *loc) {
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

inline uint8_t __find_mount(char *filename, int *adjust)
{
	 char *orig = (char *)kmalloc(strlen(filename) + 1);
	 memset(orig, 0, strlen(filename) + 1);
	 memcpy(orig, filename, strlen(filename) + 1);
	 if(orig[strlen(orig)] == '/') str_backspace(orig, '/');
	 while(1)
	 {
	 	for(int i = 0;i<MAX_MOUNTS; i++)
	 	{
	 		if(!mount_points[i]) break;
	 		if(strcmp(mount_points[i]->loc, orig) == 0)
	 		{
	 			*adjust = (strlen(orig) - 1);
	 			kfree(orig);
				return i;
	 		}
	 	}
	 	if(strcmp(orig, "/") == 0)
			break;
	 	str_backspace(orig, '/');
	 }
	 return 0;
}

uint8_t vfs_read(char *filename, char *buffer)
{
	/* Correct algorithm to resolve mounts:
	 * In a loop remove until '/' and then look for match
	 * if no match, continue until last '/' and then we know
	 * it is on the root_device
	 */
	 int adjust = 0;
	 int i = __find_mount(filename, &adjust);
	 filename += adjust;
	 //kprintf("Passing with adjust %d: %s\n", adjust, filename);
	 int rc = mount_points[i]->dev->fs->read(filename, buffer,
				mount_points[i]->dev, mount_points[i]->dev->fs->metadata);
	 return rc;
}

uint8_t vfs_write(char *filename, char *buffer, uint8_t len)
{
	/* Correct algorithm to resolve mounts:
	 * In a loop remove until '/' and then look for match
	 * if no match, continue until last '/' and then we know
	 * it is on the root_device
	 */
	 int adjust = 0;
	 int i = __find_mount(filename, &adjust);
	 filename += adjust;
	 //kprintf("Passing with adjust %d: %s\n", adjust, filename);
	 mount_points[i]->dev->fs->writefile(filename, buffer, len,
				mount_points[i]->dev, mount_points[i]->dev->fs->metadata);
	 return 0;
}

uint8_t vfs_touch(char *filename)
{
	/* Correct algorithm to resolve mounts:
	 * In a loop remove until '/' and then look for match
	 * if no match, continue until last '/' and then we know
	 * it is on the root_device
	 */
	 int adjust = 0;
	 int i = __find_mount(filename, &adjust);
	 filename += adjust;
	 //kprintf("Passing with adjust %d: %s\n", adjust, filename);
	 mount_points[i]->dev->fs->touch(filename,
				mount_points[i]->dev, mount_points[i]->dev->fs->metadata);
	 return 0;
}

uint32_t vfs_ls(char *dir, char* buffer) {
	char *orig = (char *)kmalloc(strlen(dir) + 1);
	memset(orig, 0, strlen(dir) + 1);
	memcpy(orig, dir, strlen(dir) + 1);
	while(1)
	{
		for(int i = 0; i < MAX_MOUNTS; i++)
		{
			if(!mount_points[i]) break;
			if(strcmp(mount_points[i]->loc, orig) == 0)
			{
				mount_points[i]->dev->fs->read_dir(dir + strlen(mount_points[i]->loc) - 1,
					buffer, mount_points[i]->dev, mount_points[i]->dev->fs->metadata);
				for(int k = 0; k < MAX_MOUNTS; k++)
				{
					if(!mount_points[k]) break;
					char *mount = (char *)kmalloc(strlen(mount_points[k]->loc) + 1);
					memcpy(mount, mount_points[k]->loc, strlen(mount_points[k]->loc) + 1);
					str_backspace(mount, '/');
					if(strcmp(mount, dir) == 0)
					{
						char *p = mount_points[k]->loc + strlen(dir);
						if(strlen(p) == 0 || strlen(p) == 1) continue;
					}
					kfree(mount);
				}
				break;
			}
		}
		if(strcmp(orig, "/") == 0) break;
		str_backspace(orig, '/');
	}
	kfree(orig);
	return 1;
}