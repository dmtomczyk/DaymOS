ENTRY(stage2_start)

SECTIONS {
  . = 0x8000;

  . = ALIGN(8);          /* Align start of .text too */
  .text : {
    *(.text)
  }

  . = ALIGN(8);
  .gdt : ALIGN(8) {
    *(.gdt)
  }

  . = ALIGN(8);          /* Optional: align .data section too */
  .data : {
    *(.data)
  }

  . = ALIGN(8);          /* Optional: align .bss section */
  .bss : {
    *(.bss)
    *(COMMON)
  }
}
