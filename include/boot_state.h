#pragma once
#include <stdint.h>

typedef struct {
    uint32_t framebuffer_ready;
    uint32_t fs_ready;
    uint32_t mouse_ready;
    uint32_t keyboard_ready;
    uint32_t pit_ready;
} boot_state_t;

boot_state_t boot_state_snapshot(void);
void boot_state_set_framebuffer(uint32_t ok);
void boot_state_set_fs(uint32_t ok);
void boot_state_set_mouse(uint32_t ok);
void boot_state_set_keyboard(uint32_t ok);
void boot_state_set_pit(uint32_t ok);
