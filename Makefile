x86_64_c_source_files := $(shell find arch/x86_64/impl -name *.c)
x86_64_c_object_files := $(patsubst arch/x86_64/impl/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

# Bootloader stage 1 (MBR)
bootloader_mbr_src := arch/x86_64/impl/boot/bootloader_mbr.asm
bootloader_mbr_bin := build/x86_64/bootloader_mbr.bin

# Bootloader stage 2
bootloader_stage2_src := arch/x86_64/impl/boot/bootloader_stage2.asm
bootloader_stage2_bin := build/x86_64/bootloader_stage2.bin

# All other asm sources except stage2 loader
x86_64_asm_source_files := $(filter-out $(bootloader_stage2_src) $(bootloader_mbr_src), $(shell find arch/x86_64/impl -name '*.asm'))
x86_64_asm_object_files := $(patsubst arch/x86_64/impl/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

linker_chain := targets/x86_64/linker_chain.ld
linker_grub := targets/x86_64/linker.ld

kernel_bin := dist/x86_64/kernel.bin
kernel_elf := dist/x86_64/kernel.elf
os_img := dist/x86_64/os.img

# Build C sources
$(x86_64_c_object_files): build/x86_64/%.o : arch/x86_64/impl/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I arch/x86_64/intf -ffreestanding -mno-red-zone -g -O0 $(patsubst build/x86_64/%.o, arch/x86_64/impl/%.c, $@) -o $@


# MBR build as flat binary
$(bootloader_mbr_bin): $(bootloader_mbr_src)
	mkdir -p $(dir $@) && nasm -f bin $< -o $@

# Stage2 build as flat binary
$(bootloader_stage2_bin): $(bootloader_stage2_src)
	mkdir -p $(dir $@) && nasm -f bin $< -o $@

# Now build other ASM sources as ELF
$(x86_64_asm_object_files): build/x86_64/%.o : arch/x86_64/impl/%.asm
	mkdir -p $(dir $@) && nasm -f elf64 $< -o $@
# # Build ASM sources (except stage2 loader)
# $(x86_64_asm_object_files): build/x86_64/%.o : arch/x86_64/impl/%.asm
# 	mkdir -p $(dir $@) && \
# 	nasm -f elf64 $(patsubst build/x86_64/%.o, arch/x86_64/impl/%.asm, $@) -o $@

# Build kernel ELF and binary
.PHONY: kernel-chain
kernel-chain: $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -T $(linker_chain) -o $(kernel_elf) $(x86_64_object_files) && \
	x86_64-elf-objcopy -O binary $(kernel_elf) $(kernel_bin)

# Build full disk image with MBR + stage 2 + kernel
.PHONY: build-chain-x86_64
build-chain-x86_64: kernel-chain $(bootloader_mbr_bin) $(bootloader_stage2_bin)
	dd if=/dev/zero of=$(os_img) bs=512 count=2880
	dd if=$(bootloader_mbr_bin) of=$(os_img) conv=notrunc bs=512 count=1
	dd if=$(bootloader_stage2_bin) of=$(os_img) conv=notrunc bs=512 seek=1 count=10
	dd if=$(kernel_bin) of=$(os_img) conv=notrunc bs=512 seek=11

# Run QEMU with GDB stub
.PHONY: run-chain-x86_64
run-chain-x86_64: build-chain-x86_64
	qemu-system-x86_64 -drive format=raw,file=$(os_img) -s -S

# GRUB / Multiboot build (unchanged)
.PHONY: dbg-build-x86_64
dbg-build-x86_64: $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o $(kernel_elf) -T $(linker_grub) $(x86_64_object_files) && \
	x86_64-elf-objcopy -O binary $(kernel_elf) $(kernel_bin) && \
	cp $(kernel_elf) targets/x86_64/iso/boot/kernel.elf && \
	cp $(kernel_bin) targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

.PHONY: build-x86_64
build-x86_64: dbg-build-x86_64

# Clean all generated files
.PHONY: clean
clean:
	rm -rf build/x86_64 dist/x86_64 targets/x86_64/iso/boot/kernel.{elf,bin} targets/x86_64/iso/boot/grub/stage2_eltorito $(bootloader_mbr_bin) $(bootloader_stage2_bin)
