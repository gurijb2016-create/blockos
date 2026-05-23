.section .text
.global outb
outb:
  mov 4(%esp), %edx
  mov 8(%esp), %eax
  out %al, (%dx)
  ret
