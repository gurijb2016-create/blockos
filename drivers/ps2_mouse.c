#include "ps2_mouse.h"
#include <string.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

#define PS2_CMD_ENABLE_MOUSE 0xA8
#define PS2_CMD_WRITE_MOUSE 0xD4
#define PS2_CMD_READ 0xC0

#define MOUSE_ACK 0xFA
#define MOUSE_RESEND 0xFE

static MouseData mouse_state = {0};
static uint8_t mouse_packet[3] = {0};
static uint8_t packet_index = 0;

// Port I/O functions
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);
extern void io_wait(void);

// Keyboard controller communication
static void ps2_wait_input(void) {
    for (int i = 0; i < 100000; i++) {
        if (!(inb(PS2_STATUS_PORT) & 0x02)) return;
    }
}

static void ps2_wait_output(void) {
    for (int i = 0; i < 100000; i++) {
        if (inb(PS2_STATUS_PORT) & 0x01) return;
    }
}

static void mouse_write(uint8_t byte) {
    ps2_wait_input();
    outb(PS2_COMMAND_PORT, PS2_CMD_WRITE_MOUSE);
    ps2_wait_input();
    outb(PS2_DATA_PORT, byte);
}

static uint8_t mouse_read(void) {
    ps2_wait_output();
    return inb(PS2_DATA_PORT);
}

// Initialize PS/2 mouse
void ps2_mouse_init(void) {
    // Enable mouse in keyboard controller
    ps2_wait_input();
    outb(PS2_COMMAND_PORT, PS2_CMD_ENABLE_MOUSE);
    mouse_read();  // ACK
    
    // Enable mouse data reporting
    mouse_write(0xF4);  // Enable data reporting command
    if (mouse_read() != MOUSE_ACK) return;  // Wait for ACK
    
    // Set sample rate
    mouse_write(0xF3);  // Set sample rate command
    mouse_read();  // ACK
    mouse_write(100);  // 100 Hz
    mouse_read();  // ACK
    
    memset(&mouse_state, 0, sizeof(MouseData));
    packet_index = 0;
}

// Parse mouse packet
static void parse_mouse_packet(void) {
    if (packet_index < 3) return;
    
    uint8_t status = mouse_packet[0];
    int8_t x_move = mouse_packet[1];
    int8_t y_move = mouse_packet[2];
    
    // Extract button states
    mouse_state.left_button = (status & 0x01) != 0;
    mouse_state.right_button = (status & 0x02) != 0;
    mouse_state.middle_button = (status & 0x04) != 0;
    mouse_state.buttons = status & 0x07;
    
    // Movement (handle overflow bit)
    if (status & 0x10) x_move |= 0xFF00;  // Sign extend
    if (status & 0x20) y_move |= 0xFF00;  // Sign extend
    
    mouse_state.x_movement = x_move;
    mouse_state.y_movement = y_move;
    
    packet_index = 0;
}

// IRQ 12 handler (PS/2 mouse)
void ps2_mouse_irq_handler(void) {
    uint8_t status = inb(PS2_STATUS_PORT);
    
    if (!(status & 0x20)) return;  // Not mouse data
    
    uint8_t data = inb(PS2_DATA_PORT);
    
    // Collect packet bytes
    if (packet_index == 0) {
        if ((data & 0x08) == 0) return;  // Sync byte
    }
    
    mouse_packet[packet_index] = data;
    packet_index++;
    
    if (packet_index >= 3) {
        parse_mouse_packet();
    }
}

// Get current mouse state
MouseData ps2_mouse_get_state(void) {
    return mouse_state;
}

// Get mouse position
void ps2_mouse_get_position(int* x, int* y) {
    if (x) *x = mouse_state.x_movement;
    if (y) *y = mouse_state.y_movement;
}

// Check if button pressed
bool ps2_mouse_button_pressed(uint8_t button) {
    switch (button) {
        case 0: return mouse_state.left_button;
        case 1: return mouse_state.right_button;
        case 2: return mouse_state.middle_button;
        default: return false;
    }
}
