#include "../terminal/terminal.h"
#include "../drivers/ps2_mouse.h"
#include "../fs/vfs.h"
#include <string.h>

void kmain(void) {
    // Initialize systems
    terminal_init();
    ps2_mouse_init();
    vfs_init();
    
    // Print welcome banner
    terminal_set_color(COLOR_LIGHTCYAN, COLOR_BLACK);
    terminal_putstring("\n");
    terminal_putstring("  blockOS 1.0.0 - Linux-like Terminal Operating System\n\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    
    // Mount filesystem
    if (vfs_mount_fat32(0)) {
        terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
        terminal_putstring("[OK] FAT32 filesystem mounted on /dev/sda1\n");
        terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    } else {\n        terminal_set_color(COLOR_LIGHTRED, COLOR_BLACK);
        terminal_putstring("[ERROR] Failed to mount filesystem\n");
        terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    }
    
    terminal_putstring("\n");
    terminal_set_color(COLOR_LIGHTCYAN, COLOR_BLACK);
    terminal_putstring("Available Commands:\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_putstring("  help     - Show help message\n");
    terminal_putstring("  ls       - List directory contents\n");
    terminal_putstring("  pwd      - Print working directory\n");
    terminal_putstring("  echo     - Print text\n");
    terminal_putstring("  mount    - Mount filesystem\n");
    terminal_putstring("  df       - Show disk space\n");
    terminal_putstring("  uname    - System information\n");
    terminal_putstring("  clear    - Clear screen\n\n");
    
    // Demo commands
    terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
    terminal_putstring("root@blockos:/# ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_putstring("ls\n");
    terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
    terminal_putstring("total 1024\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    
    // List files
    VirtualFileSystem* vfs = vfs_get_instance();
    if (vfs && vfs->fat32_fs.mounted) {
        FAT32DirEntry entries[32];
        uint32_t count = 32;
        
        if (fat32_list_directory(&vfs->fat32_fs, vfs->fat32_fs.root_cluster, entries, &count)) {
            for (uint32_t i = 0; i < count && i < 10; i++) {
                char filename[12];
                memcpy(filename, entries[i].filename, 11);
                filename[11] = '\0';
                
                if (entries[i].attributes & FAT32_ATTR_DIRECTORY) {
                    terminal_set_color(COLOR_LIGHTBLUE, COLOR_BLACK);
                    terminal_putstring("d");
                } else {
                    terminal_putstring("-");
                }
                
                terminal_set_color(COLOR_WHITE, COLOR_BLACK);
                terminal_putstring("rw-rw-r--  1 root root ");
                terminal_putstring(filename);
                terminal_putstring("\n");
            }
        }
    }
    
    terminal_putstring("\n");
    terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
    terminal_putstring("root@blockos:/# ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_putstring("uname\n");
    terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
    terminal_putstring("blockOS 1.0.0 (Linux-like)\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    
    terminal_putstring("\n");
    terminal_set_color(COLOR_LIGHTGREEN, COLOR_BLACK);
    terminal_putstring("root@blockos:/# ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_putstring("df\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_putstring("Filesystem    Size  Free  Used Avail\n");
    terminal_putstring("/dev/sda1     1000M 512M  488M  51%\n");
    
    terminal_putstring("\nSystem ready. Waiting for input...\n\n");
    
    // Main loop
    while (1) {
        // Would receive keyboard input here
        for (volatile int i = 0; i < 1000000; i++);
    }
}
