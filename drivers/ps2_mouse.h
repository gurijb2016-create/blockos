#ifndef PS2_MOUSE_H
#define PS2_MOUSE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t buttons;
    int8_t x_movement;
    int8_t y_movement;
    bool left_button;
    bool right_button;
    bool middle_button;
} MouseData;

// Initialize PS/2 mouse driver
void ps2_mouse_init(void);

// IRQ handler for PS/2 mouse
void ps2_mouse_irq_handler(void);

// Get current mouse state
MouseData ps2_mouse_get_state(void);

// Get mouse position relative to last read
void ps2_mouse_get_position(int* x, int* y);

// Check if button is pressed (0=left, 1=right, 2=middle)
bool ps2_mouse_button_pressed(uint8_t button);

#endif
