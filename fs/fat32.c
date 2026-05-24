#include "fat32.h"
#include <string.h>

// Disk I/O functions (to be implemented based on storage device)
extern bool disk_read(uint32_t lba, uint8_t* buffer, uint32_t count);
extern bool disk_write(uint32_t lba, const uint8_t* buffer, uint32_t count);

static FAT32FileSystem* g_fs = NULL;

// Helper function to read sectors from disk
static bool fat32_read_sectors(uint32_t sector, uint8_t* buffer, uint32_t count) {
    return disk_read(sector, buffer, count);
}

// Helper function to write sectors to disk
static bool fat32_write_sectors(uint32_t sector, const uint8_t* buffer, uint32_t count) {
    return disk_write(sector, buffer, count);
}

// Get FAT entry value for a cluster
static uint32_t fat32_get_fat_entry(FAT32FileSystem* fs, uint32_t cluster) {
    uint32_t fat_offset = fs->fat_offset;
    uint32_t entry_offset = cluster * 4;  // Each FAT32 entry is 4 bytes
    uint32_t sector = fat_offset + (entry_offset / fs->bytes_per_sector);
    uint32_t offset = entry_offset % fs->bytes_per_sector;
    
    static uint8_t sector_buffer[FAT32_SECTOR_SIZE];
    if (!fat32_read_sectors(sector, sector_buffer, 1)) return FAT32_BAD_CLUSTER;
    
    uint32_t* entry = (uint32_t*)(sector_buffer + offset);
    return *entry & 0x0FFFFFFF;  // Mask off top 4 bits
}

// Set FAT entry value for a cluster
static bool fat32_set_fat_entry(FAT32FileSystem* fs, uint32_t cluster, uint32_t value) {
    uint32_t fat_offset = fs->fat_offset;
    uint32_t entry_offset = cluster * 4;
    uint32_t sector = fat_offset + (entry_offset / fs->bytes_per_sector);
    uint32_t offset = entry_offset % fs->bytes_per_sector;
    
    static uint8_t sector_buffer[FAT32_SECTOR_SIZE];
    if (!fat32_read_sectors(sector, sector_buffer, 1)) return false;
    
    uint32_t* entry = (uint32_t*)(sector_buffer + offset);
    *entry = value & 0x0FFFFFFF;
    
    return fat32_write_sectors(sector, sector_buffer, 1);
}

// Convert cluster number to LBA (Logical Block Address)
static uint32_t fat32_cluster_to_lba(FAT32FileSystem* fs, uint32_t cluster) {
    return fs->cluster_offset + ((cluster - 2) * fs->sectors_per_cluster);
}

// Initialize FAT32 filesystem structure
bool fat32_init(FAT32FileSystem* fs) {
    if (!fs) return false;
    
    memset(fs, 0, sizeof(FAT32FileSystem));
    g_fs = fs;
    return true;
}

// Mount FAT32 filesystem
bool fat32_mount(FAT32FileSystem* fs) {
    if (!fs) return false;
    
    uint8_t boot_sector_buffer[FAT32_SECTOR_SIZE];
    
    // Read boot sector (sector 0)
    if (!fat32_read_sectors(0, boot_sector_buffer, 1)) return false;
    
    // Copy boot sector structure
    memcpy(&fs->boot_sector, boot_sector_buffer, sizeof(FAT32BootSector));
    
    // Verify FAT32 filesystem signature
    if (fs->boot_sector.bytes_per_sector != FAT32_SECTOR_SIZE) return false;
    if (fs->boot_sector.sectors_per_fat_32 == 0) return false;
    
    // Calculate offsets
    fs->bytes_per_sector = fs->boot_sector.bytes_per_sector;
    fs->sectors_per_cluster = fs->boot_sector.sectors_per_cluster;
    fs->fat_offset = fs->boot_sector.reserved_sectors;
    fs->cluster_offset = fs->boot_sector.reserved_sectors + 
                        (fs->boot_sector.num_fats * fs->boot_sector.sectors_per_fat_32);
    fs->root_cluster = fs->boot_sector.root_cluster;
    
    fs->mounted = true;
    return true;
}

// Unmount FAT32 filesystem
bool fat32_umount(FAT32FileSystem* fs) {
    if (!fs) return false;
    fs->mounted = false;
    return true;
}

// Find directory entry by filename
static bool fat32_find_entry(FAT32FileSystem* fs, const char* filename, 
                             FAT32DirEntry* entry, uint32_t* cluster_out) {
    if (!fs || !fs->mounted || !filename || !entry) return false;
    
    uint32_t cluster = fs->root_cluster;
    uint32_t sector = fat32_cluster_to_lba(fs, cluster);
    
    uint8_t buffer[FAT32_SECTOR_SIZE];
    
    // Read directory entries
    if (!fat32_read_sectors(sector, buffer, fs->sectors_per_cluster)) return false;
    
    // Search for matching filename
    FAT32DirEntry* entries = (FAT32DirEntry*)buffer;
    int entries_per_sector = fs->bytes_per_sector / sizeof(FAT32DirEntry);
    
    for (int i = 0; i < entries_per_sector * fs->sectors_per_cluster; i++) {
        FAT32DirEntry* entry_ptr = &entries[i];
        
        // End of directory
        if (entry_ptr->filename[0] == 0x00) break;
        
        // Skip deleted entries
        if (entry_ptr->filename[0] == 0xE5) continue;
        
        // Skip long filename entries
        if (entry_ptr->attributes == 0x0F) continue;
        
        // Compare filename (simplified - doesn't handle long names)
        char entry_name[12];
        memcpy(entry_name, entry_ptr->filename, 11);
        entry_name[11] = '\0';
        
        if (strcmp(entry_name, filename) == 0) {
            memcpy(entry, entry_ptr, sizeof(FAT32DirEntry));
            if (cluster_out) *cluster_out = cluster;
            return true;
        }
    }
    
    return false;
}

// Open file
bool fat32_open(FAT32FileSystem* fs, const char* filename, FAT32File* file) {
    if (!fs || !filename || !file) return false;
    
    FAT32DirEntry entry;
    uint32_t cluster;
    
    if (!fat32_find_entry(fs, filename, &entry, &cluster)) return false;
    
    file->cluster = (entry.high_cluster << 16) | entry.low_cluster;
    file->size = entry.file_size;
    file->position = 0;
    file->attributes = entry.attributes;
    
    return true;
}

// Close file
bool fat32_close(FAT32File* file) {
    if (!file) return false;
    file->position = 0;
    return true;
}

// Read from file
bool fat32_read(FAT32FileSystem* fs, FAT32File* file, uint8_t* buffer, uint32_t size) {
    if (!fs || !file || !buffer || !fs->mounted) return false;
    
    uint32_t bytes_read = 0;
    uint32_t cluster = file->cluster;
    uint32_t cluster_offset = file->position % FAT32_CLUSTER_SIZE;
    uint32_t clusters_to_skip = file->position / FAT32_CLUSTER_SIZE;
    
    // Skip to correct cluster
    for (uint32_t i = 0; i < clusters_to_skip; i++) {
        cluster = fat32_get_fat_entry(fs, cluster);
        if (cluster >= FAT32_BAD_CLUSTER) return false;
    }
    
    static uint8_t cluster_buffer[FAT32_CLUSTER_SIZE];
    
    // Read data
    while (bytes_read < size && cluster < FAT32_BAD_CLUSTER) {
        uint32_t lba = fat32_cluster_to_lba(fs, cluster);
        
        if (!fat32_read_sectors(lba, cluster_buffer, fs->sectors_per_cluster)) return false;
        
        uint32_t bytes_to_copy = FAT32_CLUSTER_SIZE - cluster_offset;
        if (bytes_to_copy > size - bytes_read) bytes_to_copy = size - bytes_read;
        if (bytes_to_copy > file->size - file->position) bytes_to_copy = file->size - file->position;
        
        memcpy(buffer + bytes_read, cluster_buffer + cluster_offset, bytes_to_copy);
        
        bytes_read += bytes_to_copy;
        file->position += bytes_to_copy;
        cluster_offset = 0;
        
        // Next cluster
        cluster = fat32_get_fat_entry(fs, cluster);
    }
    
    return bytes_read > 0;
}

// Write to file (simplified)
bool fat32_write(FAT32FileSystem* fs, FAT32File* file, const uint8_t* buffer, uint32_t size) {
    if (!fs || !file || !buffer || !fs->mounted) return false;
    
    // This is a simplified implementation
    // Full implementation would need to allocate new clusters, update FAT, etc.
    return false;
}

// Seek in file
bool fat32_seek(FAT32File* file, uint32_t offset) {
    if (!file || offset > file->size) return false;
    file->position = offset;
    return true;
}

// List directory contents
bool fat32_list_directory(FAT32FileSystem* fs, uint32_t cluster, FAT32DirEntry* entries, uint32_t* count) {
    if (!fs || !entries || !count || !fs->mounted) return false;
    
    uint32_t lba = fat32_cluster_to_lba(fs, cluster);
    static uint8_t buffer[FAT32_CLUSTER_SIZE];
    
    if (!fat32_read_sectors(lba, buffer, fs->sectors_per_cluster)) return false;
    
    FAT32DirEntry* dir_entries = (FAT32DirEntry*)buffer;
    uint32_t entry_count = 0;
    int max_entries = FAT32_CLUSTER_SIZE / sizeof(FAT32DirEntry);
    
    for (int i = 0; i < max_entries && entry_count < *count; i++) {
        if (dir_entries[i].filename[0] == 0x00) break;  // End of directory
        if (dir_entries[i].filename[0] == 0xE5) continue;  // Deleted entry
        if (dir_entries[i].attributes == 0x0F) continue;  // Long filename entry
        
        memcpy(&entries[entry_count], &dir_entries[i], sizeof(FAT32DirEntry));
        entry_count++;
    }
    
    *count = entry_count;
    return true;
}

// Create file (simplified)
bool fat32_create_file(FAT32FileSystem* fs, const char* filename) {
    if (!fs || !filename || !fs->mounted) return false;
    // Full implementation would allocate clusters, update directory, etc.
    return false;
}

// Delete file (simplified)
bool fat32_delete_file(FAT32FileSystem* fs, const char* filename) {
    if (!fs || !filename || !fs->mounted) return false;
    // Full implementation would mark clusters as free, update directory, etc.
    return false;
}

// Get volume label
char* fat32_get_volume_label(FAT32FileSystem* fs) {
    if (!fs || !fs->mounted) return NULL;
    return (char*)fs->boot_sector.volume_label;
}

// Get free space
uint32_t fat32_get_free_space(FAT32FileSystem* fs) {
    if (!fs || !fs->mounted) return 0;
    
    uint32_t free_clusters = 0;
    uint32_t total_clusters = fs->boot_sector.total_sectors_32 / fs->sectors_per_cluster;
    
    // Scan FAT for free clusters (simplified)
    for (uint32_t i = 2; i < total_clusters && i < 1000; i++) {
        uint32_t entry = fat32_get_fat_entry(fs, i);
        if (entry == FAT32_FREE_CLUSTER) free_clusters++;
    }
    
    return free_clusters * FAT32_CLUSTER_SIZE;
}
