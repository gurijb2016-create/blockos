#include "drivers/ps2_mouse.h"
#include "gui/gui.h"
#include <stdint.h>

int mouse_x = 160;
int mouse_y = 100;
Button test_button;
Button exit_button;

void button_callback(void) {
    // Placeholder callback
    gui_draw_rect(50, 50, 100, 50, 12, true);
}

void exit_callback(void) {
    // Placeholder exit callback
    gui_clear_screen(0);
}

void main(void) {
    // Initialize graphics and input
    gui_init();
    ps2_mouse_init();
    
    // Create UI elements
    gui_create_button(&test_button, 100, 50, 120, 30, "Click Me", button_callback);
    gui_create_button(&exit_button, 100, 100, 120, 30, "Exit", exit_callback);
    
    // Main loop
    while (1) {
        gui_clear_screen(0);
        
        // Draw title
        gui_draw_text(130, 10, "BlockOS GUI", 15);
        
        // Get mouse state
        MouseData mouse = ps2_mouse_get_state();
        mouse_x += mouse.x_movement;
        mouse_y -= mouse.y_movement; // Invert Y axis
        
        // Clamp mouse position
        if (mouse_x < 0) mouse_x = 0;
        if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_y >= SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT - 1;
        
        // Handle button interactions
        gui_handle_mouse_click(&test_button, mouse_x, mouse_y, mouse.left_button);
        gui_handle_mouse_click(&exit_button, mouse_x, mouse_y, mouse.left_button);
        
        // Draw UI
        gui_draw_button(&test_button);
        gui_draw_button(&exit_button);
        
        // Draw cursor
        gui_draw_cursor(mouse_x, mouse_y, 15);
        
        // Display mouse info
        gui_draw_text(10, 180, "X:", 15);
        gui_draw_text(30, 180, "Y:", 15);
        gui_draw_text(50, 180, "BTN:", 15);
        
        // Refresh screen
        gui_refresh();
        
        // Small delay
        for (volatile int i = 0; i < 1000000; i++);
    }
}
