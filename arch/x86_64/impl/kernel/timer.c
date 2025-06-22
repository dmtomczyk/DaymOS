#include "idt.h"
#include "timer.h"
#include "stdint.h"
#include "util.h"
#include "print.h"

uint64_t ticks;
const uint32_t freq = 100;

void onIRQ0(struct InterruptRegisters *regs) {
    ticks += 1;
    print_str("Timer ticked!");
    // outPortB(0x20, 0x20); // Send EOI to master PIC
}

void initTimer() {
    // Raise IOPL to 3 to allow inB/outB in ring0 (TODO: Add IOPL later)
    asm volatile (
        "pushfq\n\t"
        "pop %%rax\n\t"
        "or $(3 << 12), %%rax\n\t"
        "push %%rax\n\t"
        "popfq"
        :
        :
        : "rax", "memory"
    );

    // for (uint64_t addr = 0; addr < 0x400000; addr += 0x1000) {
    //     map_page(addr, addr); // identity map 4MiB
    // }
    
    print_set_color(PRINT_COLOR_MAGENTA, PRINT_COLOR_BLACK);
    print_str("Initializing Timer!\n");

    // Debug: print current CS and IOPL
    uint16_t cs;
    uint64_t rflags;
    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("pushfq; pop %0" : "=r"(rflags));

    print_hex("CS register: ", cs);
    print_hex("RFLAGS: ", rflags);

    if ((rflags & (3 << 12)) != 0) {
        print_str("IOPL is not 0! This may cause outb to fail.\n");
    } else {
        print_str("IOPL is 0 — good.\n");
    }

    ticks = 0;
    print_str("Installing IRQ0 Handler!\n");
    // irq_install_handler(0, &onIRQ0);

    // Oscillator 1.1931816666 MHz
    uint32_t divisor = 1193180 / freq;

    // 0011 0110 (Mode/Command register)
    print_str("Mode/Command Register!\n");
    outPortB(0x43, 0x36); // binary, mode 3, lobyte/hibyte, channel 0
    outPortB(0x40, (uint8_t)(divisor & 0xFF));
    outPortB(0x40, (uint8_t)((divisor >> 8) & 0xFF));

    // Unmask IRQ0 on the PIC (clear bit 0)
    print_str("Unmasking IRQ0 to start handling interrupts!\n");
    uint8_t mask = inPortB(0x21); // Read current PIC master mask
    print_str("FOO\n");
    mask &= ~0x01; // Clear bit 0 to unmask
    print_hex("Original PIC mask: ", mask);
    print_str("BAR!\n");
    // TODO: outPortB(0x21, mask); // write updated mask
    print_str("BAZ!\n");

    uint64_t rsp_val;
    asm volatile("mov %%rsp, %0" : "=r"(rsp_val));
    print_hex("Current RSP before sti: ", rsp_val);

    __asm__("sti"); // Enable interrupts here instead of inside idt_flush?
}

