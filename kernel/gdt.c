#include "gdt.h"

// Global GDT
static GDTEntry gdt[6];
static GDTDescriptor gdt_descriptor;

void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    if (index >= 6) return;
    
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_mid = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= granularity & 0xF0;
    
    gdt[index].access = access;
}

void gdt_init(void) {
    gdt_descriptor.size = sizeof(gdt) - 1;
    gdt_descriptor.offset = (uint32_t)&gdt;
    
    // Null descriptor
    gdt_set_entry(0, 0, 0, 0, 0);
    
    // Kernel code segment
    gdt_set_entry(1, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READ_WRITE,
                  GDT_GRAN_4KB_BLOCKS);
    
    // Kernel data segment
    gdt_set_entry(2, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR | GDT_ACCESS_READ_WRITE,
                  GDT_GRAN_4KB_BLOCKS);
    
    // User code segment
    gdt_set_entry(3, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_DPL_RING3 | GDT_ACCESS_DESCRIPTOR | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READ_WRITE,
                  GDT_GRAN_4KB_BLOCKS);
    
    // User data segment
    gdt_set_entry(4, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_DPL_RING3 | GDT_ACCESS_DESCRIPTOR | GDT_ACCESS_READ_WRITE,
                  GDT_GRAN_4KB_BLOCKS);
    
    // Null TSS (placeholder)
    gdt_set_entry(5, 0, 0, 0, 0);
    
    // Load GDT
    asm volatile("lgdt %0" : : "m"(gdt_descriptor));
}
