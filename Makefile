# kernel_source_files := $(shell find arch/x86_64/impl/kernel -name *.c)
# kernel_object_files := $(patsubst arch/x86_64/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_64_c_source_files := $(shell find arch/x86_64/impl -name *.c)
x86_64_c_object_files := $(patsubst arch/x86_64/impl/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

x86_64_asm_source_files := $(shell find arch/x86_64/impl -name *.asm)
x86_64_asm_object_files := $(patsubst arch/x86_64/impl/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

# $(kernel_object_files): build/kernel/%.o : arch/x86_64/impl/kernel/%.c
# 	mkdir -p $(dir $@) && \
# 	x86_64-elf-gcc -c -I src/intf -ffreestanding $(patsubst build/kernel/%.o, arch/x86_64/impl/kernel/%.c, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : arch/x86_64/impl/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I src/intf -ffreestanding $(patsubst build/x86_64/%.o, arch/x86_64/impl/%.c, $@) -o $@

$(x86_64_asm_object_files): build/x86_64/%.o : arch/x86_64/impl/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, arch/x86_64/impl/%.asm, $@) -o $@

.PHONY: build-x86_64
# build-x86_64: $(kernel_object_files) $(x86_64_object_files)
build-x86_64: $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso