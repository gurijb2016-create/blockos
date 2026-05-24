# blockOS - Bootable Operating System with GUI & PS/2 Mouse Support

**blockOS** is an open-source operating system in development, featuring:
- ✅ **PS/2 Mouse Driver** - Full protocol support with 3-byte packet parsing
- ✅ **GUI Framework** - Graphics primitives, buttons, and event handling
- ✅ **Interactive Interface** - Mouse-driven UI with clickable buttons
- ✅ **VGA Graphics** - 320x200 256-color mode support

## Project Structure

```
blockos/
├── boot/
│   └── io.asm              # I/O port assembly functions
├── drivers/
│   ├── ps2_mouse.h         # Mouse driver header
│   └── ps2_mouse.c         # PS/2 protocol implementation
├── gui/
│   ├── gui.h               # GUI framework header
│   └── gui.c               # Graphics and UI components
├── kernel/
│   └── main.c              # Main OS kernel and event loop
├── Makefile                # Build system
└── README.md               # This file
```

## Features

### PS/2 Mouse Driver
- IRQ 12 interrupt handler
- Full 3-byte packet protocol support
- Button detection (left, right, middle)
- X/Y movement tracking
- Real-time mouse state queries

### GUI Framework
- **Drawing primitives**: pixels, lines, rectangles, circles
- **Button system**: interactive buttons with callbacks
- **Text rendering**: character and string drawing
- **Mouse cursor**: custom cursor rendering
- **Event handling**: mouse click detection

### Main Application
- Real-time mouse tracking and display
- Interactive button interface
- Visual feedback on button clicks
- Click counter
- Status information display

## Building blockOS

### Prerequisites
```bash
sudo apt-get install build-essential nasm qemu-system-i386 xorriso
```

### Build Commands

**Full build:**
```bash
make all
```

**Run in QEMU:**
```bash
make run
```

**Build bootable ISO:**
```bash
make iso
```

**Clean build artifacts:**
```bash
make clean
```

## Usage

1. **Compile the kernel:**
   ```bash
   make all
   ```

2. **Run in emulator:**
   ```bash
   make run
   ```

3. **Interact with GUI:**
   - Move mouse to position cursor
   - Click buttons for interaction
   - Watch status updates in real-time

## Hardware Support

- **Input**: PS/2 Mouse (IRQ 12)
- **Output**: VGA 320x200 256-color mode
- **Processor**: x86/x86-64
- **Memory**: 256MB (configurable in Makefile)

## Next Steps

- [ ] Add PS/2 keyboard support
- [ ] Implement file system (FAT32)
- [ ] Add window management
- [ ] Implement multi-tasking
- [ ] Add network support
- [ ] Create system utilities

## Contributing

Contributions are welcome! Feel free to submit issues and pull requests.

## License

Open source - see repository for details

## Author

**guri creator** - blockOS Development Team

---

**Status**: In Active Development 🚀
