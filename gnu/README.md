# GNU

GNU toolchain fókusz:

- `i686-elf-gcc`, `i686-elf-ld`, `grub-mkrescue`, `qemu-system-i386`
- Freestanding build `-ffreestanding -nostdlib` jellegű megközelítéssel

Hiányzó (következő) feladatok:
- valódi GDT/IDT/IRQ kezelők
- PIT megszakítás alapú scheduler tick
- PS/2 billentyűzet scancode feldolgozás
- ext2 olvasás implementálása
