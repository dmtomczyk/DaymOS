FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
    nasm \
    xorriso \
    grub-pc-bin \
    grub-common \
    mtools \
    make \
    binutils \
    bochs \
    bochs-sdl \
    bochs-x \
    bochs-term \
    gdb \
    python3 \
    git \
    zip \
    unzip \
    curl \
    vim \
    tmux \
    qemu-system-x86 \
    qemu-utils

VOLUME /root/env
WORKDIR /root/env