# DaymOS Software Design Document

This is a living design note for DaymOS. It captures what the kernel does today, what design direction it is heading toward, and which subsystems are still intentionally unfinished.

## Table of Contents

1. Bootloader
2. Kernel Design
3. Code Structure
4. Near-Term TODOs

## 1. Bootloader

### 1.1 Multiboot2

DaymOS currently uses GRUB + Multiboot2 rather than a custom bootloader. That keeps early development focused on CPU bring-up and kernel structure instead of boot media details.

At boot, GRUB loads the kernel and enters the 32-bit bootstrap entrypoint. The bootstrap code:
- verifies the Multiboot2 magic value
- checks CPUID support
- checks long mode support
- builds a minimal paging structure
- enables paging + long mode
- loads a 64-bit GDT
- performs a far jump into 64-bit code

A custom bootloader is still an option later, but it is not a current priority.

## 2. Kernel Design

### 2.1 Current memory model

Despite older notes that mentioned a higher-half kernel, the implementation in this repository is currently a **lower-memory bootstrap kernel** linked at `0x00100000` (1 MiB).

The early boot code creates a minimal identity-mapped paging setup using 2 MiB pages so the processor can safely transition into long mode and continue executing the loaded kernel image.

This means:
- the current kernel is **not** a higher-half kernel yet
- the linker script and paging setup reflect the current lower-memory bootstrap design
- a higher-half layout remains a future architectural option, not a present feature

### 2.2 Console output

The kernel currently uses the VGA text buffer at `0xb8000` for output.

The console layer supports:
- clearing the screen
- changing foreground/background color
- writing characters and strings
- newline handling with scroll-up behavior

### 2.3 CPU tables and interrupts

A 64-bit GDT is present because it is required for the transition into long mode.

The IDT is **not implemented yet**. That means there is currently no interrupt or exception handling infrastructure beyond what GRUB/firmware already did before the kernel took over.

### 2.4 Timing

No timer is initialized yet. A future version should bring up a simple timer source (likely PIT first, then APIC/HPET later) behind a small abstraction.

## 3. Code Structure

### 3.1 Boot code

- `arch/x86_64/impl/boot/header.asm` — Multiboot2 header
- `arch/x86_64/impl/boot/main.asm` — 32-bit bootstrap
- `arch/x86_64/impl/boot/main64.asm` — 64-bit handoff stub

### 3.2 Kernel code

- `arch/x86_64/impl/kernel/main.c` — C kernel entrypoint
- `arch/x86_64/impl/print.c` — VGA text console implementation
- `arch/x86_64/intf/print.h` — console interface

### 3.3 Link and image layout

The linker script now places the following explicitly:
- Multiboot header
- `.text`
- `.rodata`
- `.data`
- `.bss`

This is more explicit and less fragile than relying on linker orphan-section placement.

## 4. Near-Term TODOs

1. Add an IDT and basic exception handlers.
2. Add a timer source and tick counter.
3. Add keyboard input through interrupts instead of only keeping an unused scan-code map.
4. Decide whether to stay with a low-memory bootstrap kernel for a while or move to a true higher-half layout.
5. Introduce a simple physical memory manager.
