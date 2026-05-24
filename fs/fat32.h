#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>
#include <stdbool.h>

#define FAT32_SECTOR_SIZE 512
#define FAT32_MAX_FILENAME 255
#define FAT32_CLUSTER_SIZE 4096

#define FAT32_ATTR_READ_ONLY 0x01
#define FAT32_ATTR_HIDDEN 0x02
#define FAT32_ATTR_SYSTEM 0x04
#define FAT32_ATTR_VOLUME_ID 0x08
#define FAT32_ATTR_DIRECTORY 0x10
#define FAT32_ATTR_ARCHIVE 0x20

#define FAT32_EOC 0x0FFFFFFF
#define FAT32_BAD_CLUSTER 0x0FFFFFF7
#define FAT32_FREE_CLUSTER 0x00000000

// Boot sector structure
typedef struct {
    uint8_t jump[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_16;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat_16;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t sectors_per_fat_32;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t system_id[8];
} __attribute__((packed)) FAT32BootSector;

// Directory entry structure
typedef struct {
    uint8_t filename[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_accessed_date;
    uint16_t high_cluster;
    uint16_t last_write_time;
    uint16_t last_write_date;
    uint16_t low_cluster;
    uint32_t file_size;
} __attribute__((packed)) FAT32DirEntry;

// File structure
typedef struct {
    uint32_t cluster;
    uint32_t size;
    uint32_t position;
    uint8_t attributes;
} FAT32File;

// Filesystem structure
typedef struct {
    FAT32BootSector boot_sector;
    uint32_t fat_offset;
    uint32_t cluster_offset;
    uint32_t root_cluster;
    uint8_t sectors_per_cluster;
    uint16_t bytes_per_sector;
    bool mounted;
} FAT32FileSystem;

// Function declarations
bool fat32_init(FAT32FileSystem* fs);
bool fat32_mount(FAT32FileSystem* fs);
bool fat32_umount(FAT32FileSystem* fs);
bool fat32_open(FAT32FileSystem* fs, const char* filename, FAT32File* file);
bool fat32_close(FAT32File* file);
bool fat32_read(FAT32FileSystem* fs, FAT32File* file, uint8_t* buffer, uint32_t size);
bool fat32_write(FAT32FileSystem* fs, FAT32File* file, const uint8_t* buffer, uint32_t size);
bool fat32_seek(FAT32File* file, uint32_t offset);
bool fat32_list_directory(FAT32FileSystem* fs, uint32_t cluster, FAT32DirEntry* entries, uint32_t* count);
bool fat32_create_file(FAT32FileSystem* fs, const char* filename);
bool fat32_delete_file(FAT32FileSystem* fs, const char* filename);
char* fat32_get_volume_label(FAT32FileSystem* fs);
uint32_t fat32_get_free_space(FAT32FileSystem* fs);

#endif
