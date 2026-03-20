# DaymOS

Daymian's hobby operating system for x86_64, built with freestanding C plus bootstrap assembly.

![DaymOS GRUB Screenshot](/pics/GRUBMenu.png)
![DaymOS Screenshot](/pics/DaymOS-0.1.0.png)

## Current status

This repository currently boots a tiny 64-bit kernel through GRUB/Multiboot2, switches the CPU into long mode, and writes to the VGA text buffer.

What works right now:
- Multiboot2 header + GRUB boot flow
- 32-bit bootstrap that checks Multiboot, CPUID, and long mode support
- Basic page-table setup using 2 MiB identity-mapped pages
- Transition into 64-bit long mode
- VGA text-mode console output

What is still missing:
- Interrupt descriptor table (IDT)
- Keyboard interrupt handling / input loop
- Timer initialization
- Memory manager beyond the initial bootstrap mapping
- Scheduler, processes, syscalls, filesystems, drivers, etc.

## Prerequisites

### Native build (Linux host)
- `gcc`
- `ld`
- `grub-file`
- `grub-mkrescue`
- One of: `xorriso`, `mkisofs`, or `genisoimage` if you want `kernel.iso`
- `qemu-system-x86_64` if you want to emulate locally

### Docker build
- Docker
- QEMU (optional, for emulation outside the container)

## Build

### Option 1: native host build

```bash
make build-x86_64
```

Notes:
- If `x86_64-elf-gcc` is installed, the Makefile will use it.
- Otherwise it falls back to the host `gcc`/`ld` toolchain with freestanding flags.
- If no ISO creation tool is installed, the build still produces `dist/x86_64/kernel.bin` and validates the Multiboot2 image when possible.

### Option 2: Docker build environment

Build the image:

```bash
docker build buildenv -t myos-buildenv
```

Build the kernel inside the container:

```bash
docker run --rm -it -v "$(pwd)":/root/env myos-buildenv make build-x86_64
```

Windows (CMD):

```bat
docker run --rm -it -v "%cd%":/root/env myos-buildenv make build-x86_64
```

Windows (PowerShell):

```powershell
docker run --rm -it -v "${pwd}:/root/env" myos-buildenv make build-x86_64
```

## Emulate

If `dist/x86_64/kernel.iso` exists:

```bash
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
```

If you only built `kernel.bin`, you can still inspect it with:

```bash
grub-file --is-x86-multiboot2 dist/x86_64/kernel.bin
```

## Cleanup

```bash
make clean
```

To remove the Docker build image:

```bash
docker rmi myos-buildenv -f
```
