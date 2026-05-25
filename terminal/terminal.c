#include "terminal.h"
#include <string.h>

static Terminal terminal_instance = {0};

// Port I/O functions
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);

// Update cursor position in hardware
static void update_hw_cursor(void) {
    uint16_t pos = terminal_instance.row * TERMINAL_WIDTH + terminal_instance.column;
    
    // Write cursor position to CRTC (0x3D4/0x3D5)
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}

// Initialize terminal
void terminal_init(void) {
    memset(&terminal_instance, 0, sizeof(Terminal));
    terminal_instance.fg_color = COLOR_WHITE;
    terminal_instance.bg_color = COLOR_BLACK;
    terminal_instance.row = 0;
    terminal_instance.column = 0;
    
    terminal_clear();
    update_hw_cursor();
}

// Scroll terminal up one line
void terminal_scroll(void) {
    if (terminal_instance.row >= TERMINAL_HEIGHT) {
        // Copy all lines up by one
        for (uint32_t i = 0; i < (TERMINAL_HEIGHT - 1) * TERMINAL_WIDTH; i++) {
            terminal_instance.buffer[i] = terminal_instance.buffer[i + TERMINAL_WIDTH];
        }
        
        // Clear last line
        for (uint32_t i = (TERMINAL_HEIGHT - 1) * TERMINAL_WIDTH; i < TERMINAL_HEIGHT * TERMINAL_WIDTH; i++) {
            terminal_instance.buffer[i].character = ' ';
            terminal_instance.buffer[i].attribute = (terminal_instance.bg_color << 4) | terminal_instance.fg_color;
        }
        
        terminal_instance.row = TERMINAL_HEIGHT - 1;
    }
}

// Put single character
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_instance.column = 0;
        terminal_instance.row++;
        
        if (terminal_instance.row >= TERMINAL_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\r') {
        terminal_instance.column = 0;
    } else if (c == '\t') {
        // Tab = 4 spaces
        for (int i = 0; i < 4; i++) {
            terminal_putchar(' ');
        }
    } else if (c == '\b') {
        // Backspace
        if (terminal_instance.column > 0) {
            terminal_instance.column--;
        }
    } else {
        // Normal character
        uint32_t index = terminal_instance.row * TERMINAL_WIDTH + terminal_instance.column;
        
        if (index < TERMINAL_WIDTH * TERMINAL_HEIGHT) {
            terminal_instance.buffer[index].character = c;
            terminal_instance.buffer[index].attribute = (terminal_instance.bg_color << 4) | terminal_instance.fg_color;
        }
        
        terminal_instance.column++;
        
        if (terminal_instance.column >= TERMINAL_WIDTH) {
            terminal_instance.column = 0;
            terminal_instance.row++;
            
            if (terminal_instance.row >= TERMINAL_HEIGHT) {
                terminal_scroll();
            }
        }
    }
    
    update_hw_cursor();
}

// Put string
void terminal_putstring(const char* str) {
    if (!str) return;
    
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

// Set color
void terminal_set_color(uint8_t fg, uint8_t bg) {
    terminal_instance.fg_color = fg & 0x0F;
    terminal_instance.bg_color = bg & 0x0F;
}

// Clear screen
void terminal_clear(void) {
    for (uint32_t i = 0; i < TERMINAL_WIDTH * TERMINAL_HEIGHT; i++) {
        terminal_instance.buffer[i].character = ' ';
        terminal_instance.buffer[i].attribute = (terminal_instance.bg_color << 4) | terminal_instance.fg_color;
    }
    
    terminal_instance.row = 0;
    terminal_instance.column = 0;
    update_hw_cursor();
}

// New line
void terminal_newline(void) {
    terminal_instance.column = 0;
    terminal_instance.row++;
    
    if (terminal_instance.row >= TERMINAL_HEIGHT) {
        terminal_scroll();
    }
}

// Set cursor position
void terminal_set_cursor(uint32_t row, uint32_t col) {
    if (row < TERMINAL_HEIGHT && col < TERMINAL_WIDTH) {
        terminal_instance.row = row;
        terminal_instance.column = col;
        update_hw_cursor();
    }
}

// Update cursor
void terminal_update_cursor(void) {
    update_hw_cursor();
}
