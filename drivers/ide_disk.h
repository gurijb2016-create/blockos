#ifndef IDE_DISK_H
#define IDE_DISK_H

#include <stdint.h>
#include <stdbool.h>

// IDE/ATA Command registers
#define IDE_PRIMARY_IO 0x1F0
#define IDE_PRIMARY_CTRL 0x3F6
#define IDE_SECONDARY_IO 0x170
#define IDE_SECONDARY_CTRL 0x376

#define IDE_REG_DATA 0x00
#define IDE_REG_ERROR 0x01
#define IDE_REG_SECTOR_COUNT 0x02
#define IDE_REG_LBA_LOW 0x03
#define IDE_REG_LBA_MID 0x04
#define IDE_REG_LBA_HIGH 0x05
#define IDE_REG_DRIVE_HEAD 0x06
#define IDE_REG_STATUS 0x07
#define IDE_REG_CMD 0x07

#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_IDENTIFY 0xEC

#define IDE_STATUS_BSY 0x80
#define IDE_STATUS_RDY 0x40
#define IDE_STATUS_ERR 0x01

// Disk information
typedef struct {
    uint16_t port;
    uint8_t slave;
    uint16_t cylinders;
    uint8_t heads;
    uint8_t sectors;
    uint32_t total_sectors;
    bool present;
} IDEDrive;

// Function declarations
bool ide_init(void);
bool ide_read_sectors(uint8_t drive, uint32_t lba, uint8_t* buffer, uint8_t count);
bool ide_write_sectors(uint8_t drive, uint32_t lba, const uint8_t* buffer, uint8_t count);
bool ide_identify_drive(uint8_t drive, IDEDrive* info);

#endif
