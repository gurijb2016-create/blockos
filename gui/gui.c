#include "gui.h"
#include <string.h>

// Simple 8x8 font data (basic ASCII)
static const uint8_t font_data[256][8] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['A'] = {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00},
    ['B'] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x7C, 0x00, 0x00},
    ['O'] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00},
    ['K'] = {0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x00, 0x00},
};

static uint8_t* framebuffer = (uint8_t*)SCREEN_BUFFER;
static uint8_t backbuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void gui_init(void) {
    // Set VGA mode 13h (320x200x256)
    // This would be done by bootloader in real implementation
    gui_clear(COLOR_BLACK);
}

void gui_clear(Color bg_color) {
    memset(backbuffer, bg_color, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void gui_set_pixel(uint16_t x, uint16_t y, Color color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    backbuffer[y * SCREEN_WIDTH + x] = color;
}

void gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color) {
    int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    while (1) {
        gui_set_pixel(x, y, color);
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void gui_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    gui_draw_line(x, y, x + width, y, color);
    gui_draw_line(x + width, y, x + width, y + height, color);
    gui_draw_line(x + width, y + height, x, y + height, color);
    gui_draw_line(x, y + height, x, y, color);
}

void gui_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    for (uint16_t yy = y; yy < y + height && yy < SCREEN_HEIGHT; yy++) {
        for (uint16_t xx = x; xx < x + width && xx < SCREEN_WIDTH; xx++) {
            gui_set_pixel(xx, yy, color);
        }
    }
}

void gui_draw_circle(uint16_t x, uint16_t y, uint16_t radius, Color color) {
    int r = radius;
    int p = 1 - r;
    int px = 0, py = r;
    
    while (px <= py) {
        gui_set_pixel(x + px, y + py, color);
        gui_set_pixel(x - px, y + py, color);
        gui_set_pixel(x + px, y - py, color);
        gui_set_pixel(x - px, y - py, color);
        gui_set_pixel(x + py, y + px, color);
        gui_set_pixel(x - py, y + px, color);
        gui_set_pixel(x + py, y - px, color);
        gui_set_pixel(x - py, y - px, color);
        
        if (p < 0) {
            p += 2 * px + 1;
        } else {
            p += 2 * (px - py) + 1;
            py--;
        }
        px++;
    }
}

void gui_draw_char(uint16_t x, uint16_t y, char c, Color color) {
    uint8_t idx = (uint8_t)c;
    const uint8_t* glyph = font_data[idx];
    
    for (int yy = 0; yy < 8; yy++) {
        uint8_t row = glyph[yy];
        for (int xx = 0; xx < 8; xx++) {
            if (row & (1 << (7 - xx))) {
                gui_set_pixel(x + xx, y + yy, color);
            }
        }
    }
}

void gui_draw_string(uint16_t x, uint16_t y, const char* str, Color color) {
    for (int i = 0; str[i] != '\0'; i++) {
        gui_draw_char(x + (i * 8), y, str[i], color);
    }
}

Button* gui_create_button(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                          const char* label, Color color, void (*on_click)(void)) {
    static Button button_storage[16];
    static int button_count = 0;
    
    if (button_count >= 16) return NULL;
    
    Button* btn = &button_storage[button_count++];
    btn->x = x;
    btn->y = y;
    btn->width = width;
    btn->height = height;
    btn->label = label;
    btn->color = color;
    btn->text_color = COLOR_WHITE;
    btn->on_click = on_click;
    btn->pressed = false;
    
    return btn;
}

void gui_draw_button(Button* btn) {
    if (!btn) return;
    
    Color fill_color = btn->pressed ? COLOR_LIGHTBLUE : btn->color;
    gui_fill_rect(btn->x, btn->y, btn->width, btn->height, fill_color);
    gui_draw_rect(btn->x, btn->y, btn->width, btn->height, COLOR_DARKGRAY);
    
    if (btn->label) {
        uint16_t text_x = btn->x + 5;
        uint16_t text_y = btn->y + (btn->height / 2) - 4;
        gui_draw_string(text_x, text_y, btn->label, btn->text_color);
    }
}

void gui_handle_button_click(Button* btn, uint16_t click_x, uint16_t click_y) {
    if (!btn) return;
    
    if (click_x >= btn->x && click_x < btn->x + btn->width &&
        click_y >= btn->y && click_y < btn->y + btn->height) {
        btn->pressed = true;
        if (btn->on_click) {
            btn->on_click();
        }
    } else {
        btn->pressed = false;
    }
}

void gui_draw_cursor(uint16_t x, uint16_t y, Color color) {
    // Draw simple arrow cursor
    gui_draw_line(x, y, x + 8, y + 8, color);
    gui_draw_line(x, y, x + 4, y + 4, color);
    gui_draw_line(x + 4, y + 4, x + 8, y, color);
}

void gui_update(void) {
    memcpy(framebuffer, backbuffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}
