#include "vfs.h"
#include <string.h>

static VirtualFileSystem vfs_instance = {0};

// Initialize VFS
bool vfs_init(void) {
    memset(&vfs_instance, 0, sizeof(VirtualFileSystem));
    vfs_instance.initialized = true;
    return true;
}

// Mount FAT32 filesystem
bool vfs_mount_fat32(uint8_t drive) {
    if (!vfs_instance.initialized) return false;
    
    // Initialize IDE disk driver
    if (!ide_init()) return false;
    
    // Initialize FAT32
    if (!fat32_init(&vfs_instance.fat32_fs)) return false;
    
    // Mount FAT32 filesystem
    if (!fat32_mount(&vfs_instance.fat32_fs)) return false;
    
    return true;
}

// Open file
bool vfs_open(const char* path, VFSFile* file) {
    if (!vfs_instance.initialized || !path || !file) return false;
    if (vfs_instance.open_count >= VFS_MAX_OPEN_FILES) return false;
    
    // Open FAT32 file
    if (!fat32_open(&vfs_instance.fat32_fs, path, &file->file)) return false;
    
    // Store path and initialize file handle
    strncpy(file->path, path, VFS_PATH_MAX - 1);
    file->path[VFS_PATH_MAX - 1] = '\0';
    file->flags = 0;
    
    vfs_instance.open_count++;
    return true;
}

// Close file
bool vfs_close(VFSFile* file) {
    if (!vfs_instance.initialized || !file) return false;
    
    fat32_close(&file->file);
    file->path[0] = '\0';
    
    if (vfs_instance.open_count > 0) vfs_instance.open_count--;
    return true;
}

// Read from file
bool vfs_read(VFSFile* file, uint8_t* buffer, uint32_t size) {
    if (!vfs_instance.initialized || !file || !buffer) return false;
    return fat32_read(&vfs_instance.fat32_fs, &file->file, buffer, size);
}

// Write to file
bool vfs_write(VFSFile* file, const uint8_t* buffer, uint32_t size) {
    if (!vfs_instance.initialized || !file || !buffer) return false;
    return fat32_write(&vfs_instance.fat32_fs, &file->file, buffer, size);
}

// Seek in file
bool vfs_seek(VFSFile* file, uint32_t offset) {
    if (!vfs_instance.initialized || !file) return false;
    return fat32_seek(&file->file, offset);
}

// List directory
bool vfs_list_directory(const char* path) {
    if (!vfs_instance.initialized || !path) return false;
    
    FAT32DirEntry entries[64];
    uint32_t count = 64;
    
    if (!fat32_list_directory(&vfs_instance.fat32_fs, vfs_instance.fat32_fs.root_cluster, entries, &count)) {
        return false;
    }
    
    return true;
}

// Get file info
bool vfs_get_file_info(const char* path, FAT32DirEntry* info) {
    if (!vfs_instance.initialized || !path || !info) return false;
    
    uint32_t cluster;
    return fat32_find_entry(&vfs_instance.fat32_fs, path, info, &cluster);
}

// Get VFS instance
VirtualFileSystem* vfs_get_instance(void) {
    return &vfs_instance;
}
