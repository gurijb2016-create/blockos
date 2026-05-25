#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT entry structure
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) IDTEntry;

// IDT descriptor
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDTDescriptor;

// Flags
#define IDT_FLAGS_GATE_TASK 0x05
#define IDT_FLAGS_GATE_INT16 0x06
#define IDT_FLAGS_GATE_TRAP16 0x07
#define IDT_FLAGS_GATE_INT32 0x0E
#define IDT_FLAGS_GATE_TRAP32 0x0F
#define IDT_FLAGS_RING0 0x00
#define IDT_FLAGS_RING3 0x60
#define IDT_FLAGS_PRESENT 0x80

// Interrupt numbers
#define INT_DIVIDE_ERROR 0x00
#define INT_DEBUG 0x01
#define INT_NMI 0x02
#define INT_BREAKPOINT 0x03
#define INT_OVERFLOW 0x04
#define INT_BOUND_RANGE 0x05
#define INT_INVALID_OPCODE 0x06
#define INT_DEVICE_NOT_AVAILABLE 0x07
#define INT_DOUBLE_FAULT 0x08
#define INT_COPROCESSOR_SEGMENT_OVERRUN 0x09
#define INT_INVALID_TSS 0x0A
#define INT_SEGMENT_NOT_PRESENT 0x0B
#define INT_STACK_SEGMENT_FAULT 0x0C
#define INT_GENERAL_PROTECTION 0x0D
#define INT_PAGE_FAULT 0x0E
#define INT_FLOATING_POINT 0x10
#define INT_ALIGNMENT_CHECK 0x11
#define INT_MACHINE_CHECK 0x12
#define INT_SIMD_FP 0x13

// IRQ numbers (mapped to interrupts 0x20-0x2F)
#define IRQ_TIMER 0x20
#define IRQ_KEYBOARD 0x21
#define IRQ_CASCADE 0x22
#define IRQ_SERIAL2 0x23
#define IRQ_SERIAL1 0x24
#define IRQ_PARALLEL2 0x25
#define IRQ_FLOPPY 0x26
#define IRQ_PARALLEL1 0x27
#define IRQ_RTC 0x28
#define IRQ_ACPI 0x29
#define IRQ_AVAILABLE1 0x2A
#define IRQ_AVAILABLE2 0x2B
#define IRQ_PS2_MOUSE 0x2C
#define IRQ_COPROCESSOR 0x2D
#define IRQ_ATA_PRIMARY 0x2E
#define IRQ_ATA_SECONDARY 0x2F

// Interrupt handler function pointer
typedef void (*interrupt_handler_t)(void);

// Function declarations
void idt_init(void);
void idt_set_entry(int index, uint32_t handler, uint16_t selector, uint8_t flags);
void idt_install_handler(int interrupt, interrupt_handler_t handler);

#endif
