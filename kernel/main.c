#include "../drivers/ps2_mouse.h"
#include "../gui/gui.h"

static int cursor_x = 160;
static int cursor_y = 100;
static int click_count = 0;

// Button click callback
void on_button_click(void) {
    click_count++;
}

void kmain(void) {
    // Initialize systems
    gui_init();
    ps2_mouse_init();
    
    // Create UI elements
    Button* start_btn = gui_create_button(50, 50, 100, 30, "START", COLOR_GREEN, on_button_click);
    Button* exit_btn = gui_create_button(50, 100, 100, 30, "EXIT", COLOR_RED, on_button_click);
    Button* test_btn = gui_create_button(50, 150, 100, 30, "TEST", COLOR_CYAN, on_button_click);
    
    // Main event loop
    while (1) {
        // Clear screen
        gui_clear(COLOR_BLACK);
        
        // Get mouse state
        MouseData mouse = ps2_mouse_get_state();
        
        // Update cursor position
        cursor_x += mouse.x_movement;
        cursor_y -= mouse.y_movement;  // Y is inverted
        
        // Clamp cursor to screen
        if (cursor_x < 0) cursor_x = 0;
        if (cursor_x >= SCREEN_WIDTH) cursor_x = SCREEN_WIDTH - 1;
        if (cursor_y < 0) cursor_y = 0;
        if (cursor_y >= SCREEN_HEIGHT) cursor_y = SCREEN_HEIGHT - 1;
        
        // Handle button clicks
        if (mouse.left_button) {
            gui_handle_button_click(start_btn, cursor_x, cursor_y);
            gui_handle_button_click(exit_btn, cursor_x, cursor_y);
            gui_handle_button_click(test_btn, cursor_x, cursor_y);
        }
        
        // Draw UI
        gui_draw_button(start_btn);
        gui_draw_button(exit_btn);
        gui_draw_button(test_btn);
        
        // Draw status
        gui_draw_string(200, 50, "blockOS GUI", COLOR_WHITE);
        
        // Draw mouse info
        char info[32];
        // Simple number-to-string (pseudo)
        gui_draw_string(200, 80, "Mouse:", COLOR_LIGHTCYAN);
        gui_draw_string(200, 90, "L=", mouse.left_button ? COLOR_RED : COLOR_GREEN);
        gui_draw_string(200, 100, "R=", mouse.right_button ? COLOR_RED : COLOR_GREEN);
        gui_draw_string(200, 110, "M=", mouse.middle_button ? COLOR_RED : COLOR_GREEN);
        
        // Draw click counter
        gui_draw_string(200, 130, "Clicks:", COLOR_YELLOW);
        
        // Draw cursor
        gui_draw_cursor(cursor_x, cursor_y, COLOR_LIGHTRED);
        
        // Update display
        gui_update();
        
        // Small delay to prevent CPU overload
        for (volatile int i = 0; i < 10000; i++);
    }
}
