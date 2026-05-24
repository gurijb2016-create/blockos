#include "ide_disk.h"
#include <string.h>

// Port I/O functions
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);
extern uint16_t inw(uint16_t port);
extern void outw(uint16_t port, uint16_t value);
extern void io_wait(void);

static IDEDrive primary_master = {.port = IDE_PRIMARY_IO, .slave = 0, .present = false};
static IDEDrive primary_slave = {.port = IDE_PRIMARY_IO, .slave = 1, .present = false};
static IDEDrive secondary_master = {.port = IDE_SECONDARY_IO, .slave = 0, .present = false};
static IDEDrive secondary_slave = {.port = IDE_SECONDARY_IO, .slave = 1, .present = false};

// Wait for drive to be ready
static bool ide_wait_ready(uint16_t port) {
    for (int i = 0; i < 100000; i++) {
        uint8_t status = inb(port + IDE_REG_STATUS);
        if ((status & IDE_STATUS_BSY) == 0) return true;
    }
    return false;
}

// Wait for drive to report data ready
static bool ide_wait_data_ready(uint16_t port) {
    for (int i = 0; i < 100000; i++) {
        uint8_t status = inb(port + IDE_REG_STATUS);
        if (status & IDE_STATUS_RDY) return true;
    }
    return false;
}

// Initialize IDE controller
bool ide_init(void) {
    // Reset primary controller
    outb(IDE_PRIMARY_CTRL, 0x04);
    io_wait();
    outb(IDE_PRIMARY_CTRL, 0x00);
    io_wait();
    
    // Detect drives on primary channel
    if (ide_identify_drive(0, &primary_master)) {
        primary_master.present = true;
    }
    if (ide_identify_drive(1, &primary_slave)) {
        primary_slave.present = true;
    }
    
    // Reset secondary controller
    outb(IDE_SECONDARY_CTRL, 0x04);
    io_wait();
    outb(IDE_SECONDARY_CTRL, 0x00);
    io_wait();
    
    // Detect drives on secondary channel
    if (ide_identify_drive(2, &secondary_master)) {
        secondary_master.present = true;
    }
    if (ide_identify_drive(3, &secondary_slave)) {
        secondary_slave.present = true;
    }
    
    return primary_master.present || primary_slave.present || 
           secondary_master.present || secondary_slave.present;
}

// Identify drive
bool ide_identify_drive(uint8_t drive, IDEDrive* info) {
    if (!info) return false;
    
    IDEDrive* drives[] = {&primary_master, &primary_slave, &secondary_master, &secondary_slave};
    if (drive >= 4) return false;
    
    IDEDrive* target = drives[drive];
    uint16_t port = target->port;
    uint8_t slave = target->slave;
    
    if (!ide_wait_ready(port)) return false;
    
    // Select drive
    outb(port + IDE_REG_DRIVE_HEAD, slave ? 0xB0 : 0xA0);
    io_wait();
    
    // Send IDENTIFY command
    outb(port + IDE_REG_CMD, IDE_CMD_IDENTIFY);
    io_wait();
    
    // Wait for data
    uint8_t status = inb(port + IDE_REG_STATUS);
    if (status == 0) return false;  // Drive doesn't exist
    
    if (!ide_wait_data_ready(port)) return false;
    
    // Read identify data
    uint16_t identify_data[256];
    for (int i = 0; i < 256; i++) {
        identify_data[i] = inw(port + IDE_REG_DATA);
    }
    
    // Extract drive info
    info->cylinders = identify_data[1];
    info->heads = identify_data[3];
    info->sectors = identify_data[6];
    info->total_sectors = *(uint32_t*)&identify_data[60];
    info->port = port;
    info->slave = slave;
    info->present = true;
    
    return true;
}

// Read sectors
bool ide_read_sectors(uint8_t drive, uint32_t lba, uint8_t* buffer, uint8_t count) {
    if (!buffer || count == 0 || count > 256) return false;
    
    IDEDrive* drives[] = {&primary_master, &primary_slave, &secondary_master, &secondary_slave};
    if (drive >= 4 || !drives[drive]->present) return false;
    
    IDEDrive* disk = drives[drive];
    uint16_t port = disk->port;
    uint8_t slave = disk->slave;
    
    if (!ide_wait_ready(port)) return false;
    
    // Set sector count
    outb(port + IDE_REG_SECTOR_COUNT, count);
    io_wait();
    
    // Set LBA address (28-bit LBA mode)
    outb(port + IDE_REG_LBA_LOW, lba & 0xFF);
    outb(port + IDE_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(port + IDE_REG_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(port + IDE_REG_DRIVE_HEAD, 0xE0 | (slave ? 0x10 : 0x00) | ((lba >> 24) & 0x0F));
    io_wait();
    
    // Send READ command
    outb(port + IDE_REG_CMD, IDE_CMD_READ);
    io_wait();
    
    // Read sectors
    uint8_t* buf_ptr = buffer;
    for (int s = 0; s < count; s++) {
        if (!ide_wait_data_ready(port)) return false;
        
        // Read 512 bytes per sector
        for (int i = 0; i < 256; i++) {
            uint16_t word = inw(port + IDE_REG_DATA);
            *buf_ptr++ = word & 0xFF;
            *buf_ptr++ = (word >> 8) & 0xFF;
        }
    }
    
    return true;
}

// Write sectors
bool ide_write_sectors(uint8_t drive, uint32_t lba, const uint8_t* buffer, uint8_t count) {
    if (!buffer || count == 0 || count > 256) return false;
    
    IDEDrive* drives[] = {&primary_master, &primary_slave, &secondary_master, &secondary_slave};
    if (drive >= 4 || !drives[drive]->present) return false;
    
    IDEDrive* disk = drives[drive];
    uint16_t port = disk->port;
    uint8_t slave = disk->slave;
    
    if (!ide_wait_ready(port)) return false;
    
    // Set sector count
    outb(port + IDE_REG_SECTOR_COUNT, count);
    io_wait();
    
    // Set LBA address
    outb(port + IDE_REG_LBA_LOW, lba & 0xFF);
    outb(port + IDE_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(port + IDE_REG_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(port + IDE_REG_DRIVE_HEAD, 0xE0 | (slave ? 0x10 : 0x00) | ((lba >> 24) & 0x0F));
    io_wait();
    
    // Send WRITE command
    outb(port + IDE_REG_CMD, IDE_CMD_WRITE);
    io_wait();
    
    // Write sectors
    const uint8_t* buf_ptr = buffer;
    for (int s = 0; s < count; s++) {
        if (!ide_wait_data_ready(port)) return false;
        
        // Write 512 bytes per sector
        for (int i = 0; i < 256; i++) {
            uint16_t word = (buf_ptr[1] << 8) | buf_ptr[0];
            outw(port + IDE_REG_DATA, word);
            buf_ptr += 2;
        }
    }
    
    return true;
}
