#include <stdint.h>
#include "io.h"
#include "idt.h"

struct __attribute__((packed)) idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
};

struct __attribute__((packed)) idt_ptr {
    uint16_t limit;
    uint32_t base;
};

extern void isr_common_stub(void);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

static inline void lidt(void* base, uint16_t size) {
    struct __attribute__((packed)) { uint16_t length; void* base; } IDTR = { size, base };
    __asm__ volatile("lidt %0" : : "m"(IDTR));
}

void idt_init(void) {
    idtp.limit = (uint16_t)(sizeof(struct idt_entry) * 256 - 1);
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; ++i) idt_set_gate((uint8_t)i, (uint32_t)isr_common_stub, 0x08, 0x8E);

    lidt(&idt, idtp.limit);
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}
