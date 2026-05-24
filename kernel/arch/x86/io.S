.section .text
.global outb
outb:
  mov 4(%esp), %edx
  mov 8(%esp), %eax
  out %al, (%dx)
  ret

.global inb
inb:
  mov 4(%esp), %edx
  xor %eax, %eax
  in (%dx), %al
  ret
