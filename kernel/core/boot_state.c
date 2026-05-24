#include "boot_state.h"

static boot_state_t g_state;

boot_state_t boot_state_snapshot(void) { return g_state; }
void boot_state_set_framebuffer(uint32_t ok) { g_state.framebuffer_ready = ok; }
void boot_state_set_fs(uint32_t ok) { g_state.fs_ready = ok; }
void boot_state_set_mouse(uint32_t ok) { g_state.mouse_ready = ok; }
void boot_state_set_keyboard(uint32_t ok) { g_state.keyboard_ready = ok; }
void boot_state_set_pit(uint32_t ok) { g_state.pit_ready = ok; }
