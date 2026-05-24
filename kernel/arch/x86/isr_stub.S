.section .text
.global isr_common_stub
.extern irq_dispatch

isr_common_stub:
  pusha
  push %ds
  push %es
  push %fs
  push %gs

  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  push $0
  push $32
  push %esp
  call irq_dispatch
  add $12, %esp

  pop %gs
  pop %fs
  pop %es
  pop %ds
  popa
  iret
