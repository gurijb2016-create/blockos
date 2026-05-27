#ifndef X11_H
#define X11_H

#include "types.h"

/* X11 Display structure */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t default_screen;
} x11_display_t;

/* X11 Window */
typedef struct {
    uint32_t xid;               /* X Window ID */
    int x, y;
    uint32_t width, height;
    uint32_t border_width;
    uint32_t background_pixel;
    uint32_t border_pixel;
    void* event_mask;
    void* property_list;
} x11_window_t;

/* X11 Event types */
#define X11_KeyPress        2
#define X11_KeyRelease      3
#define X11_ButtonPress     4
#define X11_ButtonRelease   5
#define X11_MotionNotify    6
#define X11_EnterNotify     7
#define X11_LeaveNotify     8
#define X11_FocusIn         9
#define X11_FocusOut        10
#define X11_KeymapNotify    11
#define X11_Expose          12
#define X11_GraphicsExpose  13
#define X11_NoExpose        14
#define X11_VisibilityNotify 15
#define X11_CreateNotify    16
#define X11_DestroyNotify   17
#define X11_UnmapNotify     18
#define X11_MapNotify       19
#define X11_MapRequest      20
#define X11_ReparentNotify  21
#define X11_ConfigureNotify 22
#define X11_ConfigureRequest 23
#define X11_GravityNotify   24
#define X11_ResizeRequest   25
#define X11_CirculateNotify 26
#define X11_CirculateRequest 27
#define X11_PropertyNotify  28
#define X11_SelectionClear  29
#define X11_SelectionRequest 30
#define X11_SelectionNotify 31
#define X11_ColormapNotify  32
#define X11_ClientMessage   33
#define X11_MappingNotify   34

/* X11 Event */
typedef struct {
    int type;
    uint32_t window;
    union {
        struct { int key; int state; } key_event;
        struct { int button; int x; int y; int state; } button_event;
        struct { int x; int y; int state; } motion_event;
        struct { int x; int y; uint32_t width; uint32_t height; } expose_event;
        struct { int x; int y; uint32_t width; uint32_t height; int border_width; } configure_event;
    } data;
} x11_event_t;

/* X11 Graphics Context */
typedef struct {
    uint32_t foreground;
    uint32_t background;
    uint32_t line_width;
    int line_style;
    int cap_style;
    int join_style;
    int fill_style;
} x11_gc_t;

/* X11 Protocol functions */
x11_display_t* x11_open_display(const char* display_name);
void x11_close_display(x11_display_t* display);

x11_window_t* x11_create_window(x11_display_t* display, int x, int y, 
                                uint32_t width, uint32_t height,
                                uint32_t border_width, uint32_t background);
void x11_destroy_window(x11_display_t* display, x11_window_t* window);
void x11_map_window(x11_display_t* display, x11_window_t* window);
void x11_unmap_window(x11_display_t* display, x11_window_t* window);

int x11_next_event(x11_display_t* display, x11_event_t* event);
void x11_send_event(x11_display_t* display, x11_window_t* window, x11_event_t* event);

x11_gc_t* x11_create_gc(x11_display_t* display, x11_window_t* window);
void x11_free_gc(x11_display_t* display, x11_gc_t* gc);

void x11_draw_point(x11_display_t* display, x11_window_t* window, x11_gc_t* gc, int x, int y);
void x11_draw_line(x11_display_t* display, x11_window_t* window, x11_gc_t* gc, 
                   int x1, int y1, int x2, int y2);
void x11_draw_rectangle(x11_display_t* display, x11_window_t* window, x11_gc_t* gc,
                        int x, int y, uint32_t width, uint32_t height, int filled);
void x11_fill_rectangle(x11_display_t* display, x11_window_t* window, x11_gc_t* gc,
                        int x, int y, uint32_t width, uint32_t height);
void x11_draw_string(x11_display_t* display, x11_window_t* window, x11_gc_t* gc,
                     int x, int y, const char* string);
void x11_clear_window(x11_display_t* display, x11_window_t* window);
void x11_flush(x11_display_t* display);

#endif
