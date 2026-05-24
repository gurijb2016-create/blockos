#pragma once
#include <stdint.h>

void gfx_draw_block_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void gfx_draw_text(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg);
