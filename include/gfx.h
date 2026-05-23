#pragma once
#include <stdint.h>
void fb_init(uint32_t width, uint32_t height, uint32_t pitch, uint8_t* addr);
void fb_putpixel(uint32_t x, uint32_t y, uint32_t c);
void fb_clear(uint32_t c);
void blit_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t c);
