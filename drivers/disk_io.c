// Disk I/O stubs for FAT32 filesystem
// These would interface with actual disk hardware drivers

#include <stdint.h>
#include <stdbool.h>
#include "../drivers/ide_disk.h"

// Read sectors from disk
bool disk_read(uint32_t lba, uint8_t* buffer, uint32_t count) {
    // Use primary master IDE drive
    return ide_read_sectors(0, lba, buffer, (uint8_t)count);
}

// Write sectors to disk
bool disk_write(uint32_t lba, const uint8_t* buffer, uint32_t count) {
    // Use primary master IDE drive
    return ide_write_sectors(0, lba, buffer, (uint8_t)count);
}
