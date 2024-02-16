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
}

void initTimer() {
    ticks = 0;
    irq_install_handler(0, &onIRQ0);


    print_set_color(PRINT_COLOR_MAGENTA, PRINT_COLOR_BLACK);
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");
    print_str("Timer ticked!");

    // Oscillator 1.1931816666 MHz
    uint32_t divisor = 1193180 / freq;

    // 0011 0110 (Mode/Command register)
    outPortB(0x43, 0x36);
    outPortB(0x40, (uint8_t)(divisor & 0xFF));
    outPortB(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}