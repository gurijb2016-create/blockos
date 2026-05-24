#pragma once
#include <stdint.h>

typedef enum {
    FS_OK = 0,
    FS_ERR = -1,
} fs_status_t;

typedef struct {
    uint32_t inodes_count;
    uint32_t blocks_count;
    uint32_t log_block_size;
    uint16_t magic;
} ext2_info_t;

void fs_init(void);
fs_status_t vfs_mount_root(void);
fs_status_t ext2_probe(void);
fs_status_t ext2_parse_superblock(const uint8_t* sb, ext2_info_t* out);
fs_status_t ramfs_mount(void);
