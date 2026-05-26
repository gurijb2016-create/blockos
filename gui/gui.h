#ifndef GUI_H
#define GUI_H

#include "../kernel/types.h"

/* VGA mode constants */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_BUFFER 0xA0000

/* Color palette (256 color mode) */
typedef uint8_t Color;

#define COLOR_BLACK 0x00
#define COLOR_BLUE 0x01
#define COLOR_GREEN 0x02
#define COLOR_CYAN 0x03
#define COLOR_RED 0x04
#define COLOR_MAGENTA 0x05
#define COLOR_BROWN 0x06
#define COLOR_LIGHTGRAY 0x07
#define COLOR_DARKGRAY 0x08
#define COLOR_LIGHTBLUE 0x09
#define COLOR_LIGHTGREEN 0x0A
#define COLOR_LIGHTCYAN 0x0B
#define COLOR_LIGHTRED 0x0C
#define COLOR_LIGHTMAGENTA 0x0D
#define COLOR_YELLOW 0x0E
#define COLOR_WHITE 0x0F

/* Button structure */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    Color color;
    Color text_color;
    const char* label;
    void (*on_click)(void);
    bool pressed;
} Button;

/* GUI functions */
void gui_init(void);
void gui_clear(Color bg_color);
void gui_set_pixel(uint16_t x, uint16_t y, Color color);
void gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);
void gui_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void gui_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void gui_draw_circle(uint16_t x, uint16_t y, uint16_t radius, Color color);
void gui_draw_char(uint16_t x, uint16_t y, char c, Color color);
void gui_draw_string(uint16_t x, uint16_t y, const char* str, Color color);

/* Button functions */
Button* gui_create_button(uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
                          const char* label, Color color, void (*on_click)(void));
void gui_draw_button(Button* btn);
void gui_handle_button_click(Button* btn, uint16_t click_x, uint16_t click_y);

/* Mouse cursor */
void gui_draw_cursor(uint16_t x, uint16_t y, Color color);

/* Screen update */
void gui_update(void);

#endif
