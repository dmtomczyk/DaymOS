#include "print.h"
#include "idt.h"
#include "timer.h"

// Option 1: Dumb busy wait loop (CPU speed-dependent)
void sleep_loop(uint32_t loops) {
    for (volatile uint32_t i = 0; i < loops; ++i) {
        __asm__("pause");
    }
}


void kernel_main()
{
    // 1. Kernel Greeting
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)!!!\n");
    sleep_loop(5000000);  // Tune this based on CPU speed

    // 2. Initialize IDT
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("kernel-main - PRE initIdt()\n");
    initIdtDebug();
    print_str("kernel-main - POST initIdt()!!!\n");
    
    // 3. Initialize / Start PIT Timer
    print_str("kernel-main - PRE initTimer()\n");
    sleep_loop(5000000);
    initTimer();

    print_set_color(PRINT_COLOR_BLUE, PRINT_COLOR_BLACK);
    print_str("kernel-main - MISSION SUCCESS!!\n");
    sleep_loop(5000000);

    while(1);
}