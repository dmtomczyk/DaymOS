#include "print.h"

void kernel_main(void)
{
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Booted into 64-bit long mode. VGA text console is online.\n");
}
