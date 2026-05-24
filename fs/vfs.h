#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stdbool.h>
#include "../fs/fat32.h"
#include "../drivers/ide_disk.h"

#define VFS_MAX_OPEN_FILES 32
#define VFS_PATH_MAX 256

// File handle
typedef struct {
    FAT32File file;
    char path[VFS_PATH_MAX];
    uint8_t flags;  // Read/write flags
} VFSFile;

// Virtual filesystem
typedef struct {
    FAT32FileSystem fat32_fs;
    VFSFile open_files[VFS_MAX_OPEN_FILES];
    uint32_t open_count;
    bool initialized;
} VirtualFileSystem;

// VFS functions
bool vfs_init(void);
bool vfs_mount_fat32(uint8_t drive);
bool vfs_open(const char* path, VFSFile* file);
bool vfs_close(VFSFile* file);
bool vfs_read(VFSFile* file, uint8_t* buffer, uint32_t size);
bool vfs_write(VFSFile* file, const uint8_t* buffer, uint32_t size);
bool vfs_seek(VFSFile* file, uint32_t offset);
bool vfs_list_directory(const char* path);
bool vfs_get_file_info(const char* path, FAT32DirEntry* info);
VirtualFileSystem* vfs_get_instance(void);

#endif
