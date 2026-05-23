#include <stdint.h>
#include "irq.h"
#include "mm.h"
#include "gfx.h"

void pit_init(void);
void ps2_kbd_init(void);
void ps2_mouse_init(void);

void kmain(void) {
    gdt_init();
    idt_init();
    irq_init();

    pmm_init();
    vmm_init();
    kheap_init();

    pit_init();
    ps2_kbd_init();
    ps2_mouse_init();

    fb_init(640, 480, 640 * 4, (uint8_t*)0xE0000000);
    fb_clear(0x00121212);
    blit_rect(80, 60, 280, 140, 0x003070E0);

    for (;;) {
        __asm__ volatile("hlt");
    }
}
