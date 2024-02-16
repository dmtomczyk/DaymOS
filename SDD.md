# DaymOS Software Design Document

This is a living document (that's a lie), that outlines design decisions and some implementation details about the operating system to help remind me why I chose something over another thing.

Table of Contents

1. Bootloader
2. Kernel Design
3. Code Structure

## 1. Bootloader

### 1.1 Multiboot2

I'd like to roll my own bootloader at some point, just for the heck of it, but for now I've chosen to use [MB2](https://wiki.osdev.org/Multiboot). For x86 targets, which this OS was originally developed against, the magic number is stored in EAX just as the kernel is invoked. At the time of writing this, this is defined in the header.asm file, and used in the linker.ld file

## 2 Kernel Design

### 2.1 Higher Half Kernel

I'm initially leaning toward a higher-half kernel design, but interested in considering other options. At the time of writing this, the kernel is at 0x80000000, leaving the lower addresses for user space. This also means Mnemonic invalid pointers like 0xCAFEBABE, 0xDEADBEEF, and 0xDEADC0DE can be used (funny).

## 3. Code Structure

### 3.1 Bootloader

### 3.2 Kernel

### 3.3 Interfaces & Includes
