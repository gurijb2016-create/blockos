#include "cursor.h"
#include "../kernel/types.h"
#include "../libc/libc.h"

/* Default cursor data (arrow) */
static uint8_t default_cursor[] = {
    /* 16x16 monochrome cursor */
    0x00, 0x00,  /* Line 0 */
    0x80, 0x00,  /* Line 1 - X */
    0xC0, 0x00,  /* Line 2 - XX */
    0xE0, 0x00,  /* Line 3 - XXX */
    0xF0, 0x00,  /* Line 4 - XXXX */
    0xF8, 0x00,  /* Line 5 - XXXXX */
    0xFC, 0x00,  /* Line 6 - XXXXXX */
    0xFE, 0x00,  /* Line 7 - XXXXXXX */
    0xFF, 0x00,  /* Line 8 - XXXXXXXX */
    0xFE, 0x00,  /* Line 9 - XXXXXXX */
    0xEE, 0x00,  /* Line 10 - XXX XXX */
    0xC6, 0x00,  /* Line 11 - XX  XX */
    0x86, 0x00,  /* Line 12 - X   XX */
    0x03, 0x00,  /* Line 13 -    XX */
    0x03, 0x00,  /* Line 14 -    XX */
    0x00, 0x00,  /* Line 15 */
};

cursor_theme_t* cursor_load_file(const char* filename) {
    if (!filename) return NULL;
    
    cursor_theme_t* theme = (cursor_theme_t*)malloc(sizeof(cursor_theme_t));
    if (!theme) return NULL;
    
    /* Open .cur file */
    int fd = open(filename, 0, 0);
    if (fd < 0) {
        free(theme);
        return NULL;
    }
    
    /* Read file header */
    cursor_file_header_t header;
    read(fd, &header, sizeof(cursor_file_header_t));
    
    if (header.reserved != 0 || header.type != 1) {
        close(fd);
        free(theme);
        return NULL;
    }
    
    theme->count = header.count;
    theme->images = (cursor_image_t*)malloc(sizeof(cursor_image_t) * header.count);
    if (!theme->images) {
        close(fd);
        free(theme);
        return NULL;
    }
    
    /* Read cursor directory entries */
    cursor_dir_entry_t* entries = (cursor_dir_entry_t*)malloc(
        sizeof(cursor_dir_entry_t) * header.count);
    if (!entries) {
        free(theme->images);
        close(fd);
        free(theme);
        return NULL;
    }
    
    read(fd, entries, sizeof(cursor_dir_entry_t) * header.count);
    
    /* Load cursor images */
    int i;
    for (i = 0; i < header.count; i++) {
        theme->images[i].width = entries[i].width;
        theme->images[i].height = entries[i].height;
        theme->images[i].hotspot_x = entries[i].hotspot_x;
        theme->images[i].hotspot_y = entries[i].hotspot_y;
        theme->images[i].size = entries[i].size;
        
        theme->images[i].pixels = (uint8_t*)malloc(entries[i].size);
        if (!theme->images[i].pixels) {
            int j;
            for (j = 0; j < i; j++) {
                free(theme->images[j].pixels);
            }
            free(entries);
            free(theme->images);
            close(fd);
            free(theme);
            return NULL;
        }
        
        /* Seek to image data and read */
        lseek(fd, entries[i].offset, 0);
        read(fd, theme->images[i].pixels, entries[i].size);
    }
    
    free(entries);
    close(fd);
    
    return theme;
}

void cursor_free_theme(cursor_theme_t* theme) {
    if (!theme) return;
    
    int i;
    for (i = 0; i < theme->count; i++) {
        if (theme->images[i].pixels) {
            free(theme->images[i].pixels);
        }
    }
    
    if (theme->images) {
        free(theme->images);
    }
    
    free(theme);
}

cursor_image_t* cursor_get_image(cursor_theme_t* theme, int index) {
    if (!theme || index < 0 || index >= theme->count) {
        return NULL;
    }
    
    return &theme->images[index];
}

void cursor_draw(uint8_t* framebuffer, cursor_image_t* cursor, int x, int y,
                 uint32_t fb_width, uint32_t fb_height) {
    if (!framebuffer || !cursor || !cursor->pixels) return;
    
    uint32_t cx, cy;
    for (cy = 0; cy < cursor->height; cy++) {
        for (cx = 0; cx < cursor->width; cx++) {
            int px = x + cx - cursor->hotspot_x;
            int py = y + cy - cursor->hotspot_y;
            
            if (px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                uint32_t pixel_offset = (py * fb_width + px) * 3;
                uint32_t cursor_offset = (cy * cursor->width + cx) * 3;
                
                /* Simple XOR blend for cursor visibility */
                framebuffer[pixel_offset] ^= cursor->pixels[cursor_offset];
                framebuffer[pixel_offset + 1] ^= cursor->pixels[cursor_offset + 1];
                framebuffer[pixel_offset + 2] ^= cursor->pixels[cursor_offset + 2];
            }
        }
    }
}

/* Default cursor creation */
cursor_image_t* cursor_create_default(void) {
    cursor_image_t* cursor = (cursor_image_t*)malloc(sizeof(cursor_image_t));
    if (!cursor) return NULL;
    
    cursor->width = 16;
    cursor->height = 16;
    cursor->hotspot_x = 0;
    cursor->hotspot_y = 0;
    cursor->size = 256;  /* 16x16 = 256 bytes (8-bit per pixel) */
    
    cursor->pixels = (uint8_t*)malloc(cursor->size);
    if (!cursor->pixels) {
        free(cursor);
        return NULL;
    }
    
    memcpy(cursor->pixels, default_cursor, cursor->size);
    
    return cursor;
}

/* Cursor manager */
typedef struct {
    cursor_theme_t* current_theme;
    cursor_image_t* current_image;
    cursor_image_t* default_cursor;
    int current_x;
    int current_y;
} cursor_manager_t;

static cursor_manager_t cursor_manager = {0};

void cursor_manager_init(void) {
    cursor_manager.current_theme = NULL;
    cursor_manager.current_image = NULL;
    cursor_manager.default_cursor = cursor_create_default();
    cursor_manager.current_x = 512;
    cursor_manager.current_y = 384;
    
    if (cursor_manager.default_cursor) {
        cursor_manager.current_image = cursor_manager.default_cursor;
    }
}

int cursor_manager_load_theme(const char* filename) {
    if (!filename) return -1;
    
    cursor_theme_t* theme = cursor_load_file(filename);
    if (!theme) return -1;
    
    if (cursor_manager.current_theme) {
        cursor_free_theme(cursor_manager.current_theme);
    }
    
    cursor_manager.current_theme = theme;
    if (theme->count > 0) {
        cursor_manager.current_image = &theme->images[0];
    }
    
    return 0;
}

void cursor_manager_set_position(int x, int y) {
    cursor_manager.current_x = x;
    cursor_manager.current_y = y;
}

void cursor_manager_draw(uint8_t* framebuffer, uint32_t width, uint32_t height) {
    if (!framebuffer || !cursor_manager.current_image) return;
    
    cursor_draw(framebuffer, cursor_manager.current_image,
                cursor_manager.current_x, cursor_manager.current_y,
                width, height);
}

cursor_image_t* cursor_manager_get_current(void) {
    return cursor_manager.current_image;
}
