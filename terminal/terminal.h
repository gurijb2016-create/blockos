#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stdbool.h>

// VGA text mode constants
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25
#define TERMINAL_BUFFER 0xB8000

// Color codes
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

// Terminal entry (character + attribute)
typedef struct {
    uint8_t character;
    uint8_t attribute;
} __attribute__((packed)) TerminalEntry;

// Terminal structure
typedef struct {
    TerminalEntry buffer[TERMINAL_WIDTH * TERMINAL_HEIGHT];
    uint32_t row;
    uint32_t column;
    uint8_t fg_color;
    uint8_t bg_color;
} Terminal;

// Function declarations
void terminal_init(void);
void terminal_putchar(char c);
void terminal_putstring(const char* str);
void terminal_set_color(uint8_t fg, uint8_t bg);
void terminal_clear(void);
void terminal_scroll(void);
void terminal_newline(void);
void terminal_set_cursor(uint32_t row, uint32_t col);
void terminal_update_cursor(void);

#endif
