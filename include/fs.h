#pragma once
#include <stdint.h>

typedef enum {
    FS_OK = 0,
    FS_ERR = -1,
} fs_status_t;

void fs_init(void);
fs_status_t vfs_mount_root(void);
fs_status_t ext2_probe(void);
fs_status_t ramfs_mount(void);
