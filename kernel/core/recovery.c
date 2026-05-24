#include <stdint.h>
#include "gfx.h"

void recovery_enter(const char* reason) {
    (void)reason;
    fb_clear(0x00330000);
    blit_rect(20, 20, 300, 80, 0x00AA0000);
    for (;;) {
        __asm__ volatile("cli; hlt");
    }
}
