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
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");

    sleep_loop(5000000);  // Tune this based on CPU speed

    
    print_str("Init IDT");
    print_str("Init IDT");
    print_str("Init IDT");
    initIdtDebug();
    // initIdt();
    print_str("Post IDT");
    print_str("Post IDT");
    print_str("Post IDT");

    sleep_loop(5000000);  // Tune this based on CPU speed


    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");

    sleep_loop(5000000);  // Tune this based on CPU speed

    initTimer();
    // kb_init();

    print_set_color(PRINT_COLOR_BLUE, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");

    while(1);
}