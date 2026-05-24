#include <stdint.h>
#include "io.h"
#include "arch_x86.h"

static int32_t g_dx;
static int32_t g_dy;
static uint8_t g_packet[3];
static uint8_t g_index;

static int mouse_wait_read(void) {
    for (uint32_t i = 0; i < 100000; ++i) if (inb(0x64) & 1) return 1;
    return 0;
}

static int mouse_wait_write(void) {
    for (uint32_t i = 0; i < 100000; ++i) if ((inb(0x64) & 2) == 0) return 1;
    return 0;
}

static void mouse_write(uint8_t data) {
    if (!mouse_wait_write()) return;
    outb(0x64, 0xD4);
    if (!mouse_wait_write()) return;
    outb(0x60, data);
}

static uint8_t mouse_read(void) {
    if (!mouse_wait_read()) return 0;
    return inb(0x60);
}

static void mouse_consume_packet(void) {
    uint8_t b1 = g_packet[0], b2 = g_packet[1], b3 = g_packet[2];
    int32_t dx = (b1 & 0x10) ? (int32_t)b2 - 256 : (int32_t)b2;
    int32_t dy = (b1 & 0x20) ? (int32_t)b3 - 256 : (int32_t)b3;
    g_dx += dx;
    g_dy += dy;
}

static void mouse_irq(struct interrupt_frame* frame) {
    (void)frame;
    if ((inb(0x64) & 1) == 0) return;
    uint8_t data = inb(0x60);
    if (g_index == 0 && (data & 0x08) == 0) return;
    g_packet[g_index++] = data;
    if (g_index == 3) {
        g_index = 0;
        mouse_consume_packet();
    }
}

void ps2_mouse_init(void) {
    outb(0x64, 0xA8);
    outb(0x64, 0x20);
    uint8_t status = mouse_read();
    status |= 2;
    outb(0x64, 0x60);
    outb(0x60, status);
    mouse_write(0xF6); (void)mouse_read();
    mouse_write(0xF4); (void)mouse_read();
    irq_register_handler(12, mouse_irq);
}

void ps2_mouse_poll(void) {}
int32_t ps2_mouse_consume_dx(void) { int32_t v = g_dx; g_dx = 0; return v; }
int32_t ps2_mouse_consume_dy(void) { int32_t v = g_dy; g_dy = 0; return v; }
