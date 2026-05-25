#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT entry structure
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) GDTEntry;

// GDT descriptor
typedef struct {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) GDTDescriptor;

// Access byte bits
#define GDT_ACCESS_ACCESSED 0x01
#define GDT_ACCESS_READ_WRITE 0x02
#define GDT_ACCESS_DC 0x04
#define GDT_ACCESS_EXECUTABLE 0x08
#define GDT_ACCESS_DESCRIPTOR 0x10
#define GDT_ACCESS_DPL_RING0 0x00
#define GDT_ACCESS_DPL_RING3 0x60
#define GDT_ACCESS_PRESENT 0x80

// Granularity bits
#define GDT_GRAN_LIMIT_HIGH 0x0F
#define GDT_GRAN_AVAILABLE 0x10
#define GDT_GRAN_LONG_MODE 0x20
#define GDT_GRAN_4KB_BLOCKS 0x80

// Segment selectors
#define GDT_CODE_SELECTOR 0x08
#define GDT_DATA_SELECTOR 0x10
#define GDT_USER_CODE_SELECTOR 0x18
#define GDT_USER_DATA_SELECTOR 0x20
#define GDT_TSS_SELECTOR 0x28

// Function declarations
void gdt_init(void);
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif
