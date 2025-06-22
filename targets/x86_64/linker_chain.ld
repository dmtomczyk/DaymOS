ENTRY(start)

SECTIONS {
  . = 0x100000;

  .text : {
    *(.text*)
  }

  .rodata : {
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(.bss COMMON*)
  }

  . += 0x4000; /* reserve 16KB stack space */

  stack_bottom = .;
  . += 0x4000;
  stack_top = .;
}
