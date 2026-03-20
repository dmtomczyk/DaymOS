#include "idt.h"
#include "interrupts.h"
#include "pic.h"
#include "pit.h"
#include "print.h"

extern void isr0(void);
extern void isr6(void);
extern void isr8(void);
extern void isr13(void);
extern void isr14(void);
extern void irq0(void);

static struct idt_entry idt[256];

static void idt_set_gate(int vector, void (*handler)(void), uint8_t flags) {
    uint64_t addr = (uint64_t)handler;

    idt[vector].offset_low = (uint16_t)(addr & 0xFFFF);
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].type_attr = flags;
    idt[vector].offset_mid = (uint16_t)((addr >> 16) & 0xFFFF);
    idt[vector].offset_high = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    idt[vector].zero = 0;
}

static void lidt(const struct idt_ptr* idtr) {
    __asm__ volatile ("lidt (%0)" : : "r"(idtr));
}

static void halt_forever(void) {
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

static const char* exception_name(uint64_t vector) {
    switch (vector) {
        case 0: return "Divide by zero";
        case 6: return "Invalid opcode";
        case 8: return "Double fault";
        case 13: return "General protection fault";
        case 14: return "Page fault";
        default: return "Unknown exception";
    }
}

void idt_init(void) {
    for (int i = 0; i < 256; i++) {
        idt[i] = (struct idt_entry){0};
    }

    idt_set_gate(0, isr0, 0x8E);
    idt_set_gate(6, isr6, 0x8E);
    idt_set_gate(8, isr8, 0x8E);
    idt_set_gate(13, isr13, 0x8E);
    idt_set_gate(14, isr14, 0x8E);
    idt_set_gate(32, irq0, 0x8E);

    struct idt_ptr idtr = {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)&idt[0],
    };

    lidt(&idtr);
    print_str("IDT loaded.\n");
}

void isr_exception_handler(uint64_t vector, uint64_t error_code, struct interrupt_frame* frame) {
    (void)error_code;
    (void)frame;

    print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
    print_str("\nEXCEPTION: ");
    print_str(exception_name(vector));
    print_str("\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    halt_forever();
}

void irq_handler(uint64_t vector, uint64_t error_code, struct interrupt_frame* frame) {
    (void)error_code;
    (void)frame;

    if (vector == 32) {
        pit_on_tick();

        static uint64_t tick_print_gate = 0;
        tick_print_gate++;
        if ((tick_print_gate % 100) == 0) {
            print_char('.');
        }

        pic_send_eoi(0);
        return;
    }

    print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
    print_str("Unhandled IRQ\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    halt_forever();
}
