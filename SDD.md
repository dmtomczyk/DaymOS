# DaymOS Software Design Document

This is a living document (that's a lie), that outlines design decisions and some implementation details about the operating system to help remind me why I chose something over another thing.

Table of Contents

1. Bootloader
2. Kernel Design
3. Code Structure

## 0. To Organize Later

## 0.1 GDT - Global Descriptor Table

The GDT is defined in main.asm by using the LGDT assembly instruction whose argument is a pointer to the GDT descriptor structure. As we're using GRUB, it will cache the GDT entries in the processor, but after a segment change we will lose that, so that's why we needed to define our own. See https://wiki.osdev.org/GDT for more info later.

## 0.2 IDT - Interrupt Descriptor Table

TODO: The IDT is used for task and interrupt descriptors (devices, etc.).

## 0.3 Timing - Programmable Internal Timer

TODO: Initialise a timer to be able to keep track of timing. Consider which timer you would like to support first (most beginners go with PIT, although it is ancient), and how would you like to set it up (most set it up to tick at a convenient interval, like 1 ms or 10 ms). However, make sure you abstract the interface so adding support for more timers is easier.

## 1. Bootloader

### 1.1 Multiboot2

I'd like to roll my own bootloader at some point, just for the heck of it, but for now I've chosen to use [MB2](https://wiki.osdev.org/Multiboot). For x86 targets, which this OS was originally developed against, the magic number is stored in EAX just as the kernel is invoked. At the time of writing this, this is defined in the header.asm file, and used in the linker.ld file

## 2 Kernel Design

### 2.1 Higher Half Kernel

I'm initially leaning toward a higher-half kernel design, but interested in considering other options. At the time of writing this, the kernel is at 0x80000000, leaving the lower addresses for user space. This also means Mnemonic invalid pointers like 0xCAFEBABE, 0xDEADBEEF, and 0xDEADC0DE can be used (funny).

Some useful notes on higher-half kernels from OSDev

- Higher half kernels allow the kernel to be mapped in every user process, which is good/traditional.
- If your OS is 64-bit (which we are), then 32-bit applications will be able to use the full 32-bit address space
- Mapping at 0x80000000 leaves 2 GiB for kernel data and 2 GiB for processes

## 3. Code Structure

### 3.1 Bootloader

### 3.2 Kernel

### 3.3 Interfaces & Includes

### 3.4 Permissions

TODO: .bss, .data, .rodata, and .text should have specific read/write permissions defined which can be done via linker.
