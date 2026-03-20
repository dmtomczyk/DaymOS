#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "print.h"

void kernel_main(void)
{
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to DaymOS (Daymian's Operating System)\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Booted into 64-bit long mode. VGA text console is online.\n");

    idt_init();

    pic_mask_all();
    pic_remap(32, 40);
    pic_unmask_irq(0);

    pit_init(100);

    print_str("Enabling interrupts.\n");
    __asm__ volatile ("sti");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
