#include <stdint.h>
#include "irq.h"
#include "mm.h"
#include "gfx.h"
#include "input.h"
#include "multiboot2.h"
#include "fs.h"
#include "recovery.h"
#include "boot_state.h"


static int fb_from_multiboot(uint32_t magic, uint32_t mbi_addr) {
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) return 0;

    mb2_info_t* info = (mb2_info_t*)(uintptr_t)mbi_addr;
    mb2_tag_t* tag = (mb2_tag_t*)(info + 1);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            mb2_tag_framebuffer_t* fb = (mb2_tag_framebuffer_t*)tag;
            if (fb->framebuffer_bpp == 32) {
                fb_init(fb->framebuffer_width, fb->framebuffer_height, fb->framebuffer_pitch,
                        (uint8_t*)(uintptr_t)fb->framebuffer_addr);
                return 1;
            }
        }
        tag = (mb2_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    return 0;
}

static void draw_cursor(int32_t x, int32_t y, uint32_t c) {
    for (int32_t i = 0; i < 10; ++i) fb_putpixel((uint32_t)(x + i), (uint32_t)y, c);
    for (int32_t i = 0; i < 10; ++i) fb_putpixel((uint32_t)x, (uint32_t)(y + i), c);
}

void kmain(uint32_t magic, uint32_t mbi_addr) {
    gdt_init();
    idt_init();
    irq_init();

    pmm_init();
    vmm_init();
    kheap_init();

    pit_init();
    boot_state_set_pit(1);
    ps2_kbd_init();
    boot_state_set_keyboard(1);
    ps2_mouse_init();
    boot_state_set_mouse(1);

    fs_init();
    if (vfs_mount_root() != FS_OK) {
        if (ext2_probe() != FS_OK && ramfs_mount() != FS_OK) {
            boot_state_set_fs(0);
            recovery_enter("fs mount failed");
        }
    }
    boot_state_set_fs(1);

    if (!fb_from_multiboot(magic, mbi_addr)) {
        boot_state_set_framebuffer(0);
        recovery_enter("framebuffer not available");
    }
    boot_state_set_framebuffer(1);

    int32_t mx = (int32_t)(fb_width() / 2);
    int32_t my = (int32_t)(fb_height() / 2);

    for (;;) {
        ps2_mouse_poll();
        mx += ps2_mouse_consume_dx();
        my -= ps2_mouse_consume_dy();

        if (mx < 0) mx = 0;
        if (my < 0) my = 0;
        if ((uint32_t)mx >= fb_width()) mx = (int32_t)fb_width() - 1;
        if ((uint32_t)my >= fb_height()) my = (int32_t)fb_height() - 1;

        fb_clear(0x00121212);
        blit_rect(80, 60, 280, 140, 0x003070E0);
        draw_cursor(mx, my, 0x00FFFFFF);
        __asm__ volatile("hlt");
    }
}
