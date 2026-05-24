#pragma once
#include <stdint.h>

void ps2_mouse_init(void);
void ps2_mouse_poll(void);
int32_t ps2_mouse_consume_dx(void);
int32_t ps2_mouse_consume_dy(void);
void ps2_kbd_init(void);
void pit_init(void);
