#include "fs.h"

static int g_root_ready;

void fs_init(void) {
    g_root_ready = 0;
}

fs_status_t vfs_mount_root(void) {
    g_root_ready = 1;
    return FS_OK;
}

int vfs_root_ready(void) {
    return g_root_ready;
}
