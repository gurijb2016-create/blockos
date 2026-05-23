.section .multiboot
.align 8
.long 0xE85250D6
.long 0
.long header_end - header_start
.long -(0xE85250D6 + 0 + (header_end - header_start))
header_start:
  .short 0
  .short 0
  .long 8
header_end:
