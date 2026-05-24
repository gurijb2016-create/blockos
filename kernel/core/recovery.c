#include <stdint.h>
#include "gfx.h"
#include "gfx_text.h"
#include "boot_state.h"

void recovery_enter(const char* reason) {
    (void)reason;
    fb_clear(0x00330000);
    blit_rect(20, 20, 420, 140, 0x00AA0000);
    gfx_draw_text(32, 40, "RECOVERY MODE", 0x00FFFFFF, 0x00AA0000);
    boot_state_t st = boot_state_snapshot();
    if (!st.framebuffer_ready) gfx_draw_text(32, 60, "FB:FAIL", 0x00FFFFFF, 0x00AA0000);
    if (!st.fs_ready) gfx_draw_text(96, 60, "FS:FAIL", 0x00FFFFFF, 0x00AA0000);
    if (!st.mouse_ready) gfx_draw_text(160, 60, "MOUSE:FAIL", 0x00FFFFFF, 0x00AA0000);
    if (!st.keyboard_ready) gfx_draw_text(256, 60, "KBD:FAIL", 0x00FFFFFF, 0x00AA0000);
    if (!st.pit_ready) gfx_draw_text(336, 60, "PIT:FAIL", 0x00FFFFFF, 0x00AA0000);
    for (;;) {
        __asm__ volatile("cli; hlt");
    }
}
