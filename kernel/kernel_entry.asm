[BITS 32]

global kernel_entry
extern kmain

kernel_entry:
    ; Setup kernel stack
    mov esp, kernel_stack_top
    
    ; Clear BSS section
    mov edi, kernel_bss_start
    mov ecx, kernel_bss_size
    xor eax, eax
    cld
    rep stosd
    
    ; Jump to C code
    call kmain
    
    ; Halt if main returns
    hlt
    jmp $

; Kernel stack (8KB)
section .bss
    align 16
    kernel_stack_bottom:
    resb 8192
    kernel_stack_top:

; BSS section symbols
kernel_bss_start: equ __bss_start
kernel_bss_size: equ __bss_end - __bss_start
