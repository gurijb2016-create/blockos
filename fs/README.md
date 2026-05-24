# FS Layout (Linux-szerű, saját implementáció)

Ez a réteg Linux-szerű felépítést követ, de nem Linux-kód másolat:

- `kernel/fs/vfs.c` – VFS gyökér mount vezérlés
- `kernel/fs/ext2.c` – ext2 probe helye (stub)
- `kernel/fs/ramfs.c` – RAM alapú fallback rootfs

Cél: ha tartós fájlrendszer nem érhető el, a rendszer `ramfs` fallbacket használjon recovery útvonal előtt.
