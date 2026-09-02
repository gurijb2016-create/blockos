# blockOS Makefile

CC = gcc
ASM = nasm
LD = ld
QEMU = qemu-system-i386


CFLAGS = -m32 -std=gnu99 -ffreestanding \
         -fno-stack-protector -fno-pic \
         -nostdlib -nostdinc -Ikernel

ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

SOURCES = kernel/main.c drivers/ps2_mouse.c gui/gui.c kernel/string.c
ASM_SOURCES = boot/io.asm

OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
KERNEL = kernel.elf
ISO = blockos.iso

.PHONY: all clean iso run menuconfig

all: $(KERNEL)

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

iso: $(KERNEL)
	mkisofs -R -b boot/stage1.bin -no-emul-boot -boot-load-size 4 -input-charset utf-8 -o $(ISO) .

run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) -m 256M -serial stdio

menuconfig:
	@echo "Select build options:"
	@echo "1. Build"
	@echo "2. Run"
	@echo "3. Clean"

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	@echo "Cleaned build artifacts"
