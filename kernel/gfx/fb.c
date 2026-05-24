#include "gfx.h"

static uint8_t* g_fb;
static uint32_t g_w, g_h, g_pitch;

void fb_init(uint32_t width, uint32_t height, uint32_t pitch, uint8_t* addr) {
    g_w = width;
    g_h = height;
    g_pitch = pitch;
    g_fb = addr;
}

uint32_t fb_width(void) { return g_w; }
uint32_t fb_height(void) { return g_h; }

void fb_putpixel(uint32_t x, uint32_t y, uint32_t c) {
    if (!g_fb || x >= g_w || y >= g_h) return;
    ((uint32_t*)(g_fb + y * g_pitch))[x] = c;
}

void fb_clear(uint32_t c) {
    for (uint32_t y = 0; y < g_h; ++y) {
        for (uint32_t x = 0; x < g_w; ++x) {
            fb_putpixel(x, y, c);
        }
    }
}
