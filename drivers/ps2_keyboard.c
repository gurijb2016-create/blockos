#include "ps2_keyboard.h"

static KeyboardState kb_state = {0};

// ASCII conversion table for US keyboard layout
static const char ascii_table[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Port I/O functions
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);

// Wait for keyboard ready
static void ps2_wait_ready(void) {
    for (int i = 0; i < 100000; i++) {
        if (!(inb(PS2_KB_STATUS_PORT) & 0x02)) return;
    }
}

// Wait for data available
static void ps2_wait_data(void) {
    for (int i = 0; i < 100000; i++) {
        if (inb(PS2_KB_STATUS_PORT) & 0x01) return;
    }
}

// Initialize PS/2 keyboard
void ps2_keyboard_init(void) {
    kb_state.last_code = 0;
    kb_state.modifiers = 0;
    kb_state.caps_lock = false;
    kb_state.num_lock = false;
    kb_state.scroll_lock = false;
    
    // Enable keyboard
    ps2_wait_ready();
    outb(PS2_KB_COMMAND_PORT, 0xAE);  // Enable keyboard
    ps2_wait_ready();
    
    // Set keyboard to scan code set 1
    outb(PS2_KB_DATA_PORT, 0xF0);
    ps2_wait_ready();
    outb(PS2_KB_DATA_PORT, 0x01);
    ps2_wait_ready();
}

// Read scancode from keyboard
uint8_t ps2_keyboard_read_scancode(void) {
    ps2_wait_data();
    uint8_t scancode = inb(PS2_KB_DATA_PORT);
    
    // Handle key release
    if (scancode & KEY_RELEASED) {
        uint8_t key = scancode & 0x7F;
        
        // Update modifier state
        if (key == KEY_LCTRL) kb_state.modifiers &= ~MOD_LCTRL;
        if (key == KEY_RCTRL) kb_state.modifiers &= ~MOD_RCTRL;
        if (key == KEY_LSHIFT) kb_state.modifiers &= ~MOD_LSHIFT;
        if (key == KEY_RSHIFT) kb_state.modifiers &= ~MOD_RSHIFT;
        if (key == KEY_LALT) kb_state.modifiers &= ~MOD_LALT;
    } else {
        // Handle key press
        kb_state.last_code = scancode;
        
        // Update modifier state
        if (scancode == KEY_LCTRL) kb_state.modifiers |= MOD_LCTRL;
        if (scancode == KEY_RCTRL) kb_state.modifiers |= MOD_RCTRL;
        if (scancode == KEY_LSHIFT) kb_state.modifiers |= MOD_LSHIFT;
        if (scancode == KEY_RSHIFT) kb_state.modifiers |= MOD_RSHIFT;
        if (scancode == KEY_LALT) kb_state.modifiers |= MOD_LALT;
        
        // Toggle lock keys
        if (scancode == KEY_CAPSLOCK) kb_state.caps_lock = !kb_state.caps_lock;
        if (scancode == KEY_NUMLOCK) kb_state.num_lock = !kb_state.num_lock;
        if (scancode == KEY_SCROLLLOCK) kb_state.scroll_lock = !kb_state.scroll_lock;
    }
    
    return scancode;
}

// Convert scancode to ASCII
char ps2_keyboard_scancode_to_ascii(uint8_t scancode) {
    if (scancode & KEY_RELEASED) return 0;  // Key release
    if (scancode >= 128) return 0;
    
    char c = ascii_table[scancode];
    
    // Handle shift
    if (kb_state.modifiers & (MOD_LSHIFT | MOD_RSHIFT)) {
        if (c >= 'a' && c <= 'z') return c - 32;
        if (c == '1') return '!';
        if (c == '2') return '@';
        if (c == '3') return '#';
        if (c == '4') return '$';
        if (c == '5') return '%';
        if (c == '6') return '^';
        if (c == '7') return '&';
        if (c == '8') return '*';
        if (c == '9') return '(';
        if (c == '0') return ')';
        if (c == '-') return '_';
        if (c == '=') return '+';
        if (c == '[') return '{';
        if (c == ']') return '}';
        if (c == ';') return ':';
        if (c == '\'') return '"';
        if (c == ',') return '<';
        if (c == '.') return '>';
        if (c == '/') return '?';
        if (c == '\\') return '|';
        if (c == '`') return '~';
    }
    
    // Handle caps lock
    if (kb_state.caps_lock && c >= 'a' && c <= 'z') {
        return c - 32;
    }
    
    return c;
}

// Get keyboard state
KeyboardState ps2_keyboard_get_state(void) {
    return kb_state;
}
