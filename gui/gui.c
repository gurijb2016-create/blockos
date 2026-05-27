#include "gui.h"
#include <string.h>

/* Simple 8x8 font data (basic ASCII) */
static const uint8_t font_data[256][8] = {
    /* Space */ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ! */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* " */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* # */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* $ */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* % */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* & */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ' */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ( */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ) */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* * */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* + */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* , */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* - */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* . */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* / */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 0 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 1 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 2 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 3 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 4 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 5 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 6 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 7 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 8 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* 9 */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* : */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ; */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* < */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* = */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* > */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* ? */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* @ */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* A */    {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00},
    /* B */    {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x7C, 0x00, 0x00},
    /* C */    {0x3C, 0x66, 0x60, 0x60, 0x66, 0x3C, 0x00, 0x00},
    /* D */    {0x78, 0x6C, 0x66, 0x66, 0x6C, 0x78, 0x00, 0x00},
    /* E */    {0x7E, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00, 0x00},
    /* F */    {0x7E, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00, 0x00},
    /* G */    {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x3C, 0x00, 0x00},
    /* H */    {0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00},
    /* I */    {0x7E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00},
    /* J */    {0x7E, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00},
    /* K */    {0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x00, 0x00},
    /* L */    {0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00, 0x00},
    /* M */    {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x00, 0x00},
    /* N */    {0x66, 0x76, 0x7E, 0x6E, 0x66, 0x66, 0x00, 0x00},
    /* O */    {0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00},
    /* P */    {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x00, 0x00},
    /* Q */    {0x3C, 0x66, 0x66, 0x6E, 0x3C, 0x0E, 0x00, 0x00},
    /* R */    {0x7C, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x00, 0x00},
    /* S */    {0x3C, 0x60, 0x3C, 0x06, 0x06, 0x3C, 0x00, 0x00},
    /* T */    {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},
    /* U */    {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00},
    /* V */    {0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00},
    /* W */    {0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00, 0x00},
    /* X */    {0x66, 0x66, 0x3C, 0x3C, 0x66, 0x66, 0x00, 0x00},
    /* Y */    {0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x00},
    /* Z */    {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x7E, 0x00, 0x00},
};

static uint8_t* framebuffer = (uint8_t*)SCREEN_BUFFER;
static uint8_t backbuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void gui_init(void) {
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

void gui_fill_circle(uint16_t x, uint16_t y, uint16_t radius, Color color) {
    int r = radius;
    int py;
    
    for (py = -r; py <= r; py++) {
        int px_dist = (r * r - py * py);
        if (px_dist < 0) continue;
        
        int px = 0;
        while (px * px <= px_dist) {
            gui_set_pixel(x + px, y + py, color);
            gui_set_pixel(x - px, y + py, color);
            px++;
        }
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
    btn->pressed = 0;
    btn->enabled = 1;
    
    return btn;
}

void gui_draw_button(Button* btn) {
    if (!btn) return;
    
    Color fill_color = btn->pressed ? COLOR_LIGHTBLUE : btn->color;
    if (!btn->enabled) fill_color = COLOR_DARKGRAY;
    
    gui_fill_rect(btn->x, btn->y, btn->width, btn->height, fill_color);
    gui_draw_rect(btn->x, btn->y, btn->width, btn->height, COLOR_DARKGRAY);
    
    if (btn->label) {
        uint16_t text_x = btn->x + 5;
        uint16_t text_y = btn->y + (btn->height / 2) - 4;
        gui_draw_string(text_x, text_y, btn->label, btn->text_color);
    }
}

void gui_handle_button_click(Button* btn, uint16_t click_x, uint16_t click_y) {
    if (!btn || !btn->enabled) return;
    
    if (click_x >= btn->x && click_x < btn->x + btn->width &&
        click_y >= btn->y && click_y < btn->y + btn->height) {
        btn->pressed = 1;
        if (btn->on_click) {
            btn->on_click();
        }
    } else {
        btn->pressed = 0;
    }
}

void gui_button_set_enabled(Button* btn, bool enabled) {
    if (btn) btn->enabled = enabled;
}

TextBox* gui_create_textbox(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                            uint16_t max_length) {
    static TextBox textbox_storage[8];
    static int textbox_count = 0;
    
    if (textbox_count >= 8) return NULL;
    
    TextBox* box = &textbox_storage[textbox_count++];
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
    box->bg_color = COLOR_WHITE;
    box->text_color = COLOR_BLACK;
    box->cursor_pos = 0;
    box->max_length = max_length;
    box->focused = 0;
    memset(box->buffer, 0, 256);
    
    return box;
}

void gui_draw_textbox(TextBox* box) {
    if (!box) return;
    
    Color border_color = box->focused ? COLOR_LIGHTBLUE : COLOR_DARKGRAY;
    gui_fill_rect(box->x, box->y, box->width, box->height, box->bg_color);
    gui_draw_rect(box->x, box->y, box->width, box->height, border_color);
    gui_draw_string(box->x + 2, box->y + (box->height / 2) - 4, box->buffer, box->text_color);
    
    if (box->focused) {
        uint16_t cursor_x = box->x + 2 + (box->cursor_pos * 8);
        gui_draw_line(cursor_x, box->y + 2, cursor_x, box->y + box->height - 2, COLOR_BLACK);
    }
}

void gui_textbox_input_char(TextBox* box, char c) {
    if (!box || box->cursor_pos >= box->max_length) return;
    box->buffer[box->cursor_pos++] = c;
    box->buffer[box->cursor_pos] = 0;
}

void gui_textbox_backspace(TextBox* box) {
    if (!box || box->cursor_pos == 0) return;
    box->cursor_pos--;
    box->buffer[box->cursor_pos] = 0;
}

void gui_textbox_set_focused(TextBox* box, bool focused) {
    if (box) box->focused = focused;
}

Label* gui_create_label(uint16_t x, uint16_t y, const char* text, Color color) {
    static Label label_storage[32];
    static int label_count = 0;
    
    if (label_count >= 32) return NULL;
    
    Label* lbl = &label_storage[label_count++];
    lbl->x = x;
    lbl->y = y;
    lbl->text = text;
    lbl->text_color = color;
    
    return lbl;
}

void gui_draw_label(Label* lbl) {
    if (lbl) gui_draw_string(lbl->x, lbl->y, lbl->text, lbl->text_color);
}

void gui_label_set_text(Label* lbl, const char* text) {
    if (lbl) lbl->text = text;
}

CheckBox* gui_create_checkbox(uint16_t x, uint16_t y, const char* label, Color color) {
    static CheckBox checkbox_storage[16];
    static int checkbox_count = 0;
    
    if (checkbox_count >= 16) return NULL;
    
    CheckBox* cb = &checkbox_storage[checkbox_count++];
    cb->x = x;
    cb->y = y;
    cb->label = label;
    cb->color = color;
    cb->text_color = COLOR_WHITE;
    cb->checked = 0;
    
    return cb;
}

void gui_draw_checkbox(CheckBox* cb) {
    if (!cb) return;
    
    gui_draw_rect(cb->x, cb->y, 12, 12, cb->color);
    if (cb->checked) {
        gui_fill_rect(cb->x + 2, cb->y + 2, 8, 8, cb->color);
    }
    if (cb->label) {
        gui_draw_string(cb->x + 16, cb->y + 2, cb->label, cb->text_color);
    }
}

void gui_checkbox_toggle(CheckBox* cb) {
    if (cb) cb->checked = !cb->checked;
}

void gui_handle_checkbox_click(CheckBox* cb, uint16_t click_x, uint16_t click_y) {
    if (!cb) return;
    if (click_x >= cb->x && click_x < cb->x + 12 &&
        click_y >= cb->y && click_y < cb->y + 12) {
        gui_checkbox_toggle(cb);
    }
}

ProgressBar* gui_create_progressbar(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                                    Color bg_color, Color fill_color) {
    static ProgressBar progressbar_storage[8];
    static int progressbar_count = 0;
    
    if (progressbar_count >= 8) return NULL;
    
    ProgressBar* pb = &progressbar_storage[progressbar_count++];
    pb->x = x;
    pb->y = y;
    pb->width = width;
    pb->height = height;
    pb->bg_color = bg_color;
    pb->fill_color = fill_color;
    pb->percentage = 0;
    
    return pb;
}

void gui_draw_progressbar(ProgressBar* pb) {
    if (!pb) return;
    
    gui_fill_rect(pb->x, pb->y, pb->width, pb->height, pb->bg_color);
    uint16_t fill_width = (pb->width * pb->percentage) / 100;
    gui_fill_rect(pb->x, pb->y, fill_width, pb->height, pb->fill_color);
    gui_draw_rect(pb->x, pb->y, pb->width, pb->height, COLOR_BLACK);
}

void gui_progressbar_set_value(ProgressBar* pb, uint8_t percentage) {
    if (pb && percentage <= 100) pb->percentage = percentage;
}

Window* gui_create_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                          const char* title, Color title_color) {
    static Window window_storage[4];
    static int window_count = 0;
    
    if (window_count >= 4) return NULL;
    
    Window* win = &window_storage[window_count++];
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
    win->title = title;
    win->title_color = title_color;
    win->bg_color = COLOR_LIGHTGRAY;
    win->visible = 1;
    win->draggable = 1;
    
    return win;
}

void gui_draw_window(Window* win) {
    if (!win || !win->visible) return;
    
    gui_fill_rect(win->x, win->y, win->width, 20, win->title_color);
    gui_fill_rect(win->x, win->y + 20, win->width, win->height - 20, win->bg_color);
    gui_draw_rect(win->x, win->y, win->width, win->height, COLOR_BLACK);
    
    if (win->title) {
        gui_draw_string(win->x + 4, win->y + 6, win->title, COLOR_WHITE);
    }
}

void gui_window_set_visible(Window* win, bool visible) {
    if (win) win->visible = visible;
}

void gui_window_move(Window* win, uint16_t new_x, uint16_t new_y) {
    if (win) {
        win->x = new_x;
        win->y = new_y;
    }
}

void gui_draw_cursor(uint16_t x, uint16_t y, Color color) {
    gui_draw_line(x, y, x + 8, y + 8, color);
    gui_draw_line(x, y, x + 4, y + 4, color);
    gui_draw_line(x + 4, y + 4, x + 8, y, color);
}

void gui_update(void) {
    memcpy(framebuffer, backbuffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}
