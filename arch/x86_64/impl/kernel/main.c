#include "print.h"
#include "idt.h"
#include "timer.h"

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

    initIdt();
    
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");
    print_str("Welcome to DaymOS (Daymian's Operating System)");


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