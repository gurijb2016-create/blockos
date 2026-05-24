#include "gfx.h"
#include "gfx_text.h"

void gfx_draw_block_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg) {
    (void)c;
    for (uint32_t yy = 0; yy < 12; ++yy) {
        for (uint32_t xx = 0; xx < 8; ++xx) {
            uint32_t col = (xx == 0 || yy == 0 || xx == 7 || yy == 11) ? fg : bg;
            fb_putpixel(x + xx, y + yy, col);
        }
    }
}

void gfx_draw_text(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg) {
    uint32_t cx = x;
    while (*s) {
        gfx_draw_block_char(cx, y, *s++, fg, bg);
        cx += 8;
    }
}
