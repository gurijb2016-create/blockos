global inb
global outb
global io_wait

; uint8_t inb(uint16_t port)
inb:
    mov edx, [esp + 4]
    in al, dx
    ret

; void outb(uint16_t port, uint8_t value)
outb:
    mov edx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

; void io_wait(void)
io_wait:
    nop
    nop
    nop
    ret
