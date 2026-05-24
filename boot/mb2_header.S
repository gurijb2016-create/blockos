.section .multiboot
.align 8
header_start:
.long 0xE85250D6
.long 0
.long header_end - header_start
.long -(0xE85250D6 + 0 + (header_end - header_start))

/* framebuffer request */
.align 8
.short 5
.short 0
.long 20
.long 1024
.long 768
.long 32

/* end tag */
.align 8
.short 0
.short 0
.long 8
header_end:
