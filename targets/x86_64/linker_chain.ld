ENTRY(start)

SECTIONS 
{
    . = 0x80000;

  .text ALIGN(4K) : AT(ADDR(.text)) {
    *(.text*)
  }

  .rodata ALIGN(4K) : AT(ADDR(.rodata)) {
    *(.rodata*)
  }

  .data ALIGN(4K) : AT(ADDR(.data)) {
    *(.data*)
  }

  .bss ALIGN(4K) : AT(ADDR(.bss)) {
    *(COMMON)
    *(.bss*)
    _end = .;
  }

  /* Kernel stack */
  . += 0x1000; /* Optional padding */
  .stack (NOLOAD) : ALIGN(16K) {
    PROVIDE(stack_bottom = .);
    . = . + 16K;
    PROVIDE(stack_top = .);
  }
}
