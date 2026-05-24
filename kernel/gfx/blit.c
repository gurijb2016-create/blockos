#include "gfx.h"

void blit_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t c) {
    for (uint32_t yy = y; yy < y + h; ++yy)
        for (uint32_t xx = x; xx < x + w; ++xx)
            fb_putpixel(xx, yy, c);
}
