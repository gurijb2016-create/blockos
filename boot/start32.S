.global _start
.extern kmain
.section .text
_start:
  cli
  mov $stack_top, %esp
  push %ebx
  push %eax
  call kmain
.hang:
  hlt
  jmp .hang

.section .bss
.align 16
stack_bottom:
  .skip 16384
stack_top:
