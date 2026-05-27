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

/* Button widget */
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
    bool enabled;
} Button;

/* TextBox widget */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    Color bg_color;
    Color text_color;
    uint16_t cursor_pos;
    uint16_t max_length;
    bool focused;
    char buffer[256];
} TextBox;

/* Label widget */
typedef struct {
    uint16_t x;
    uint16_t y;
    const char* text;
    Color text_color;
} Label;

/* CheckBox widget */
typedef struct {
    uint16_t x;
    uint16_t y;
    const char* label;
    Color color;
    Color text_color;
    bool checked;
} CheckBox;

/* ProgressBar widget */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    Color bg_color;
    Color fill_color;
    uint8_t percentage;
} ProgressBar;

/* Window widget */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    const char* title;
    Color title_color;
    Color bg_color;
    bool visible;
    bool draggable;
} Window;

/* Core GUI functions */
void gui_init(void);
void gui_clear(Color bg_color);
void gui_set_pixel(uint16_t x, uint16_t y, Color color);
void gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);
void gui_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void gui_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void gui_draw_circle(uint16_t x, uint16_t y, uint16_t radius, Color color);
void gui_fill_circle(uint16_t x, uint16_t y, uint16_t radius, Color color);
void gui_draw_char(uint16_t x, uint16_t y, char c, Color color);
void gui_draw_string(uint16_t x, uint16_t y, const char* str, Color color);

/* Button functions */
Button* gui_create_button(uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
                          const char* label, Color color, void (*on_click)(void));
void gui_draw_button(Button* btn);
void gui_handle_button_click(Button* btn, uint16_t click_x, uint16_t click_y);
void gui_button_set_enabled(Button* btn, bool enabled);

/* TextBox functions */
TextBox* gui_create_textbox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t max_length);
void gui_draw_textbox(TextBox* box);
void gui_textbox_input_char(TextBox* box, char c);
void gui_textbox_backspace(TextBox* box);
void gui_textbox_set_focused(TextBox* box, bool focused);

/* Label functions */
Label* gui_create_label(uint16_t x, uint16_t y, const char* text, Color color);
void gui_draw_label(Label* lbl);
void gui_label_set_text(Label* lbl, const char* text);

/* CheckBox functions */
CheckBox* gui_create_checkbox(uint16_t x, uint16_t y, const char* label, Color color);
void gui_draw_checkbox(CheckBox* cb);
void gui_checkbox_toggle(CheckBox* cb);
void gui_handle_checkbox_click(CheckBox* cb, uint16_t click_x, uint16_t click_y);

/* ProgressBar functions */
ProgressBar* gui_create_progressbar(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                                    Color bg_color, Color fill_color);
void gui_draw_progressbar(ProgressBar* pb);
void gui_progressbar_set_value(ProgressBar* pb, uint8_t percentage);

/* Window functions */
Window* gui_create_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                          const char* title, Color title_color);
void gui_draw_window(Window* win);
void gui_window_set_visible(Window* win, bool visible);
void gui_window_move(Window* win, uint16_t new_x, uint16_t new_y);

/* Mouse cursor */
void gui_draw_cursor(uint16_t x, uint16_t y, Color color);

/* Screen update */
void gui_update(void);

#endif
