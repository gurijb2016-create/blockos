/* Linker script for blockOS kernel */
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kernel_entry)

SECTIONS
{
    . = 0x10000;

    .boot :
    {
        *(.boot)
    }

    .text ALIGN(0x1000) :
    {
        *(.text)
    }

    .rodata ALIGN(0x1000) :
    {
        *(.rodata)
    }

    .data ALIGN(0x1000) :
    {
        *(.data)
    }

    .bss ALIGN(0x1000) :
    {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    }

    .stab :
    {
        *(.stab)
    }

    .stabstr :
    {
        *(.stabstr)
    }

    .debug_info :
    {
        *(.debug_info)
    }

    /DISCARD/ :
    {
        *(.note.GNU-stack)
    }
}
