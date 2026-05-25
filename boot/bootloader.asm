[BITS 16]
[ORG 0x7C00]

; Bootloader - First stage
; Loads kernel into memory and jumps to it

boot_start:
    jmp short main
    nop

    ; BIOS Parameter Block
    OEM_ID:         db "blockOS "
    bytes_per_sec:  dw 512
    secs_per_clus:  db 1
    reserved_secs:  dw 1
    num_fats:       db 2
    root_dir_ents:  dw 224
    total_secs_16:  dw 2880
    media:          db 0xF0
    secs_per_fat:   dw 9
    secs_per_track: dw 18
    num_heads:      dw 2
    hidden_secs:    dd 0
    total_secs_32:  dd 0
    drive_num:      db 0
    reserved:       db 0
    boot_sig:       db 0x29
    volume_id:      dd 0x12345678
    volume_label:   db "blockOS    "
    fs_type:        db "FAT12   "

main:
    cli                 ; Disable interrupts
    cld                 ; Clear direction flag
    
    ; Setup stack
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7C00
    
    ; Setup data/extra segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Clear screen
    mov ax, 0x0003
    int 0x10
    
    ; Print message
    mov si, msg_boot
    call print_string
    
    ; Load kernel from disk
    mov ax, 0x1000      ; Load to 0x10000
    mov es, ax
    mov bx, 0x0000
    
    mov ah, 0x02        ; Read sectors
    mov al, 16          ; Read 16 sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Sector 2 (skip boot sector)
    mov dh, 0           ; Head 0
    mov dl, 0x80        ; Drive 0x80 (first HDD)
    int 0x13
    
    jc disk_error
    
    mov si, msg_loaded
    call print_string
    
    ; Enter protected mode
    lgdt [gdt_descriptor]
    
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
    
    ; Jump to protected mode
    jmp 0x08:protected_mode

disk_error:
    mov si, msg_error
    call print_string
    jmp $

print_string:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

msg_boot:    db "blockOS Bootloader v1.0", 13, 10, 0
msg_loaded:  db "Kernel loaded! Entering protected mode...", 13, 10, 0
msg_error:   db "Disk read error!", 13, 10, 0

; GDT for protected mode
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0
    
    ; Code descriptor
    dw 0xFFFF           ; Limit
    dw 0x0000           ; Base low
    db 0x00             ; Base mid
    db 10011010b        ; Access
    db 11001111b        ; Granularity
    db 0x00             ; Base high
    
    ; Data descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
protected_mode:
    ; Setup segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    
    ; Jump to kernel entry point
    jmp 0x10000

; Bootloader signature
times 510 - ($ - $$) db 0
dw 0xAA55
