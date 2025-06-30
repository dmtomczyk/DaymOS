SHELL= /bin/sh
.SUFFIXES:
.SUFFIXES: .c .asm .o .elf .bin .ld

# Fallback to grub if no option chosen during `make` command
BOOT ?= grub

# DETECT ENVIRONMENT
ifeq ($(shell command -v x86_64-elf-gcc >/dev/null 2>&1 && echo yes),yes)
CROSS_PREFIX := x86_64-elf-
else
CROSS_PREFIX := x86_64-linux-gnu-
endif
CC := $(CROSS_PREFIX)gcc
LD := $(CROSS_PREFIX)ld
OBJCOPY := $(CROSS_PREFIX)objcopy

x86_64_c_source_files := $(shell find arch/x86_64/impl -name *.c)
x86_64_c_object_files := $(patsubst arch/x86_64/impl/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

# Daym-chain, Stage 1: Custom 512 byte MBR
stage1_src := arch/x86_64/impl/boot/chain/stage1.asm
stage1_bin := build/x86_64/stage1.bin

# Daym-chain, Stage 2: Mixed 16bit/32bit bootloader
stage2_src := arch/x86_64/impl/boot/chain/stage2.asm
stage2_obj := build/x86_64/stage2.o
stage2_elf := build/x86_64/stage2.elf
stage2_bin := build/x86_64/stage2.bin
linker_stage2 := targets/x86_64/linker_stage2.ld

# Other ASM source files (excluding 16-bit bootloader)
x86_64_asm_source_files := $(filter-out $(stage1_src) $(stage2_src), $(shell find arch/x86_64/impl -name '*.asm'))
x86_64_asm_object_files := $(patsubst arch/x86_64/impl/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

linker_chain := targets/x86_64/linker_chain.ld
linker_grub := targets/x86_64/linker_grub.ld

ifeq ($(BOOT),chain)
KERNEL_LINKER := $(linker_chain)
else
KERNEL_LINKER := $(linker_grub)
endif

kernel_bin := dist/x86_64/kernel.bin
kernel_elf := dist/x86_64/kernel.elf
os_img := dist/x86_64/os.img

# Assemble daym-chain stage 1 (flat 512 byte MBR binary) to stage1.bin
$(stage1_bin): $(stage1_src)
	mkdir -p $(dir $@) && nasm -f bin $< -o $@

# 1. Assemble daym-chain stage 2 (mixed 16bit/32bit) to stage2.elf (as elf32), 
# 2. Then generate stage2.elf, and update/map stage2.obj using linker_stage2 memory mapping.
# 3. Then objcopy stage2 into binary? TODO: Understand exactly what happens here. 
$(stage2_bin): $(stage2_src) $(linker_stage2)
	mkdir -p $(dir $@) && \
	nasm -f elf32 $< -o $(stage2_obj) && \
	$(LD) -m elf_i386 -T $(linker_stage2) -o $(stage2_elf) $(stage2_obj) && \
	$(OBJCOPY) -O binary $(stage2_elf) $@

# Build C source files
$(x86_64_c_object_files): build/x86_64/%.o : arch/x86_64/impl/%.c
	mkdir -p $(dir $@) && \
	$(CC) -c -I arch/x86_64/intf -ffreestanding -mno-red-zone -g -O0 $< -o $@

# Assemble other ASM files (32/64-bit ELF)
# TODO: Understand the nuance of assembling with -f elf64 or -f elf32, 
# TODO: (contd) as this may compile entry32.asm into 64bit elf, which may or may not be an issue. IDK.
build/x86_64/%.o: arch/x86_64/impl/%.asm
	mkdir -p $(dir $@) && nasm -f elf64 $< -o $@

# Explicitly force entry32.o first
entry32_obj := build/x86_64/boot/entry32.o

# Build kernel ELF + binary
.PHONY: kernel-kernel
kernel-kernel: $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	$(LD) -n -T $(KERNEL_LINKER) -o $(kernel_elf) $(entry32_obj) $(filter-out $(entry32_obj),$(x86_64_object_files)) && \
	$(OBJCOPY) -O binary $(kernel_elf) $(kernel_bin)

# Write to empty HDD, then fill with os contents.
.PHONY: daym-chain
daym-chain: kernel-kernel $(stage1_bin) $(stage2_bin)
	dd if=/dev/zero of=$(os_img) bs=512 count=100800
	dd if=$(stage1_bin) of=$(os_img) conv=notrunc bs=512 seek=0
	dd if=$(stage2_bin) of=$(os_img) conv=notrunc bs=512 seek=1
	dd if=$(kernel_bin) of=$(os_img) conv=notrunc bs=512 seek=1024

.PHONY: grub
grub: $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	$(LD) -n -o $(kernel_elf) -T $(linker_grub) $(x86_64_object_files) && \
	$(OBJCOPY) -O binary $(kernel_elf) $(kernel_bin) && \
	cp $(kernel_elf) targets/x86_64/iso/boot/kernel.elf && \
	cp $(kernel_bin) targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

# Usage: `make build BOOT=chain` or `make build BOOT=grub`
.PHONY: build
build:
ifeq ($(BOOT),chain)
	$(MAKE) daym-chain
else
	$(MAKE) grub
endif

.PHONY: clean
clean:
	rm -rf build/x86_64 dist/x86_64 targets/x86_64/iso/boot/kernel.{elf,bin} targets/x86_64/iso/boot/grub/stage2_eltorito $(bootloader_bin)
