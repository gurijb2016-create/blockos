ARCH      ?= i686
CROSS     ?= $(ARCH)-elf-
CC        := $(CROSS)gcc
AS        := $(CROSS)gcc
LD        := $(CROSS)ld
OBJCOPY   := $(CROSS)objcopy
GRUB_MKRESCUE ?= grub-mkrescue

-include Config.mk

CONFIG_FB ?= 1
CONFIG_MOUSE ?= 1
CONFIG_FS ?= 1
CONFIG_RECOVERY ?= 1
CONFIG_TEXT ?= 1

CFLAGS := -std=gnu11 -O2 -Wall -Wextra -ffreestanding -fno-stack-protector -m32 -Iinclude
CFLAGS += -DCONFIG_FB=$(CONFIG_FB) -DCONFIG_MOUSE=$(CONFIG_MOUSE) -DCONFIG_FS=$(CONFIG_FS) -DCONFIG_RECOVERY=$(CONFIG_RECOVERY) -DCONFIG_TEXT=$(CONFIG_TEXT)
ASFLAGS := -m32 -ffreestanding
LDFLAGS := -m elf_i386 -T linker/kernel32.ld

BUILD_DIR := build
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
ISO_DIR := $(BUILD_DIR)/iso
ISO_IMG := $(BUILD_DIR)/mycoreos.iso

SRC_C := \
	kernel/core/kmain.c \
	kernel/core/log.c \
	kernel/core/panic.c \
	kernel/core/boot_state.c \
	kernel/arch/x86/gdt.c \
	kernel/arch/x86/idt.c \
	kernel/arch/x86/irq.c \
	kernel/arch/x86/pic.c \
	kernel/mm/pmm.c \
	kernel/mm/vmm.c \
	kernel/mm/kheap.c \
	kernel/drivers/pit.c \
	kernel/drivers/ps2_kbd.c

ifeq ($(CONFIG_MOUSE),1)
SRC_C += kernel/drivers/ps2_mouse.c
endif
ifeq ($(CONFIG_FS),1)
SRC_C += kernel/fs/vfs.c kernel/fs/ext2.c kernel/fs/ramfs.c
endif
ifeq ($(CONFIG_FB),1)
SRC_C += kernel/gfx/fb.c kernel/gfx/blit.c
endif
ifeq ($(CONFIG_TEXT),1)
SRC_C += kernel/gfx/text.c
endif
ifeq ($(CONFIG_RECOVERY),1)
SRC_C += kernel/core/recovery.c
endif

SRC_S := \
	boot/mb2_header.S \
	boot/start32.S \
	kernel/arch/x86/isr_stub.S \
	kernel/arch/x86/io.S

OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_C)) $(patsubst %.S,$(BUILD_DIR)/%.o,$(SRC_S))

all: $(ISO_IMG)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(KERNEL_ELF): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

$(ISO_IMG): $(KERNEL_ELF)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf
	cp iso/boot/grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

run: $(ISO_IMG)
	qemu-system-i386 -cdrom $(ISO_IMG)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean menuconfig showconfig


menuconfig:
	./scripts/menuconfig.sh Config.mk

showconfig:
	@echo "CONFIG_FB=$(CONFIG_FB)"
	@echo "CONFIG_MOUSE=$(CONFIG_MOUSE)"
	@echo "CONFIG_FS=$(CONFIG_FS)"
	@echo "CONFIG_RECOVERY=$(CONFIG_RECOVERY)"
	@echo "CONFIG_TEXT=$(CONFIG_TEXT)"
