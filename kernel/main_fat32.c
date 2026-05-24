#include "../drivers/ps2_mouse.h"
#include "../gui/gui.h"
#include "../fs/vfs.h"
#include <string.h>

static int cursor_x = 160;
static int cursor_y = 100;
static int click_count = 0;
static bool file_browser_active = false;
static char current_directory[256] = "/";

// Button callbacks
void on_button_click(void) {
    click_count++;
}

void on_file_browser_click(void) {
    file_browser_active = !file_browser_active;
}

void on_mount_click(void) {
    // Mount FAT32 filesystem
    if (vfs_mount_fat32(0)) {
        // Success - mounted
    }
}

void on_unmount_click(void) {
    VirtualFileSystem* vfs = vfs_get_instance();
    if (vfs && vfs->fat32_fs.mounted) {
        fat32_umount(&vfs->fat32_fs);
    }
}

void kmain(void) {
    // Initialize systems
    gui_init();
    ps2_mouse_init();
    vfs_init();
    
    // Create UI elements
    Button* start_btn = gui_create_button(10, 10, 100, 30, "START", COLOR_GREEN, on_button_click);
    Button* file_btn = gui_create_button(120, 10, 100, 30, "FILES", COLOR_CYAN, on_file_browser_click);
    Button* mount_btn = gui_create_button(230, 10, 80, 30, "MOUNT", COLOR_YELLOW, on_mount_click);
    Button* unmount_btn = gui_create_button(320, 10, 100, 30, "UNMOUNT", COLOR_RED, on_unmount_click);
    
    // Mount filesystem at startup
    vfs_mount_fat32(0);
    
    // Main event loop
    while (1) {
        // Clear screen
        gui_clear(COLOR_BLACK);
        
        // Get mouse state
        MouseData mouse = ps2_mouse_get_state();
        
        // Update cursor position
        cursor_x += mouse.x_movement;
        cursor_y -= mouse.y_movement;
        
        // Clamp cursor to screen
        if (cursor_x < 0) cursor_x = 0;
        if (cursor_x >= SCREEN_WIDTH) cursor_x = SCREEN_WIDTH - 1;
        if (cursor_y < 0) cursor_y = 0;
        if (cursor_y >= SCREEN_HEIGHT) cursor_y = SCREEN_HEIGHT - 1;
        
        // Handle button clicks
        if (mouse.left_button) {
            gui_handle_button_click(start_btn, cursor_x, cursor_y);
            gui_handle_button_click(file_btn, cursor_x, cursor_y);
            gui_handle_button_click(mount_btn, cursor_x, cursor_y);
            gui_handle_button_click(unmount_btn, cursor_x, cursor_y);
        }
        
        // Draw UI
        gui_draw_button(start_btn);
        gui_draw_button(file_btn);
        gui_draw_button(mount_btn);
        gui_draw_button(unmount_btn);
        
        // Draw title
        gui_draw_string(10, 50, "blockOS FAT32 File Manager", COLOR_WHITE);
        
        // Draw file browser if active
        if (file_browser_active) {
            gui_draw_string(10, 70, "Directory Listing:", COLOR_LIGHTCYAN);
            
            VirtualFileSystem* vfs = vfs_get_instance();
            if (vfs && vfs->fat32_fs.mounted) {
                gui_draw_string(10, 90, "Status: MOUNTED", COLOR_LIGHTGREEN);
                
                // List files
                FAT32DirEntry entries[32];
                uint32_t count = 32;
                if (fat32_list_directory(&vfs->fat32_fs, vfs->fat32_fs.root_cluster, entries, &count)) {
                    int y = 110;
                    for (uint32_t i = 0; i < count && y < SCREEN_HEIGHT - 10; i++) {
                        char filename[12];
                        memcpy(filename, entries[i].filename, 11);
                        filename[11] = '\0';
                        
                        // Draw file entry
                        if (entries[i].attributes & FAT32_ATTR_DIRECTORY) {
                            gui_draw_string(20, y, "[DIR] ", COLOR_LIGHTCYAN);
                        } else {
                            gui_draw_string(20, y, "[FILE]", COLOR_LIGHTGRAY);
                        }
                        
                        gui_draw_string(70, y, filename, COLOR_WHITE);
                        y += 15;
                    }
                }
            } else {
                gui_draw_string(10, 90, "Status: NOT MOUNTED", COLOR_LIGHTRED);
            }
        }
        
        // Draw mouse info
        gui_draw_string(10, SCREEN_HEIGHT - 30, "Mouse: ", COLOR_LIGHTCYAN);
        if (mouse.left_button) gui_draw_string(70, SCREEN_HEIGHT - 30, "L", COLOR_RED);
        if (mouse.right_button) gui_draw_string(85, SCREEN_HEIGHT - 30, "R", COLOR_RED);
        if (mouse.middle_button) gui_draw_string(100, SCREEN_HEIGHT - 30, "M", COLOR_RED);
        
        // Draw cursor
        gui_draw_cursor(cursor_x, cursor_y, COLOR_LIGHTRED);
        
        // Update display
        gui_update();
        
        // Small delay
        for (volatile int i = 0; i < 10000; i++);
    }
}
