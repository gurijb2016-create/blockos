#include <stdint.h>
#include "arch_x86.h"
#include "pic.h"
#include "io.h"

static irq_handler_t g_handlers[16];

void irq_register_handler(uint8_t irq, irq_handler_t handler) {
    if (irq < 16) g_handlers[irq] = handler;
}

void irq_unregister_handler(uint8_t irq) {
    if (irq < 16) g_handlers[irq] = 0;
}

void irq_dispatch(struct interrupt_frame* frame, uint32_t int_no, uint32_t err) {
    (void)frame;
    (void)err;
    if (int_no < 32 || int_no >= 48) return;

    uint8_t irq = (uint8_t)(int_no - 32);
    if (g_handlers[irq]) g_handlers[irq](frame);
    pic_send_eoi(irq);
}

void irq_init(void) {
    pic_remap(0x20, 0x28);
    outb(0x21, 0xF8);
    outb(0xA1, 0xEF);
    __asm__ volatile("sti");
}
