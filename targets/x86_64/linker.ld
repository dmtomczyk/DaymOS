ENTRY(start)

SECTIONS
{
    . = 1M;

    .boot ALIGN(4K) :
    {
        KEEP(*(.multiboot_header))
    }

    .text ALIGN(4K) :
    {
        *(.text)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(.bss COMMON)
        _end = .;
    }

    /* --- Kernel Stack Section --- */
    . += 0x1000; /* Padding in case _end is misaligned */
    .stack ALIGN(16K) : ALIGN(16K)
    {
        PROVIDE(stack_bottom = .);
        . = . + 16K;            /* 16KB kernel stack */
        PROVIDE(stack_top = .);
    }
}
