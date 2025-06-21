#include "stdint.h"
#include "util.h"
#include "idt.h"
#include "print.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint64_t);

void initIdt(){
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint64_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

    //0x20 commands and 0x21 data
    //0xA0 commands and 0xA1 data
    outPortB(0x20, 0x11);
    outPortB(0xA0, 0x11);

    outPortB(0x21, 0x20);
    outPortB(0xA1, 0x28);

    outPortB(0x21,0x04);
    outPortB(0xA1,0x02);

    outPortB(0x21, 0x01);
    outPortB(0xA1, 0x01);

    outPortB(0x21, 0x0);
    outPortB(0xA1, 0x0);

    setIdtGate(0, (uint64_t)isr0,0x08, 0x8E);
    setIdtGate(1, (uint64_t)isr1,0x08, 0x8E);
    setIdtGate(2, (uint64_t)isr2,0x08, 0x8E);
    setIdtGate(3, (uint64_t)isr3,0x08, 0x8E);
    setIdtGate(4, (uint64_t)isr4, 0x08, 0x8E);
    setIdtGate(5, (uint64_t)isr5, 0x08, 0x8E);
    setIdtGate(6, (uint64_t)isr6, 0x08, 0x8E);
    setIdtGate(7, (uint64_t)isr7, 0x08, 0x8E);
    setIdtGate(8, (uint64_t)isr8, 0x08, 0x8E);
    setIdtGate(9, (uint64_t)isr9, 0x08, 0x8E);
    setIdtGate(10, (uint64_t)isr10, 0x08, 0x8E);
    setIdtGate(11, (uint64_t)isr11, 0x08, 0x8E);
    setIdtGate(12, (uint64_t)isr12, 0x08, 0x8E);
    setIdtGate(13, (uint64_t)isr13, 0x08, 0x8E);
    setIdtGate(14, (uint64_t)isr14, 0x08, 0x8E);
    setIdtGate(15, (uint64_t)isr15, 0x08, 0x8E);
    setIdtGate(16, (uint64_t)isr16, 0x08, 0x8E);
    setIdtGate(17, (uint64_t)isr17, 0x08, 0x8E);
    setIdtGate(18, (uint64_t)isr18, 0x08, 0x8E);
    setIdtGate(19, (uint64_t)isr19, 0x08, 0x8E);
    setIdtGate(20, (uint64_t)isr20, 0x08, 0x8E);
    setIdtGate(21, (uint64_t)isr21, 0x08, 0x8E);
    setIdtGate(22, (uint64_t)isr22, 0x08, 0x8E);
    setIdtGate(23, (uint64_t)isr23, 0x08, 0x8E);
    setIdtGate(24, (uint64_t)isr24, 0x08, 0x8E);
    setIdtGate(25, (uint64_t)isr25, 0x08, 0x8E);
    setIdtGate(26, (uint64_t)isr26, 0x08, 0x8E);
    setIdtGate(27, (uint64_t)isr27, 0x08, 0x8E);
    setIdtGate(28, (uint64_t)isr28, 0x08, 0x8E);
    setIdtGate(29, (uint64_t)isr29, 0x08, 0x8E);
    setIdtGate(30, (uint64_t)isr30, 0x08, 0x8E);
    setIdtGate(31, (uint64_t)isr31, 0x08, 0x8E);

    setIdtGate(32, (uint64_t)irq0, 0x08, 0x8E);
    setIdtGate(33, (uint64_t)irq1, 0x08, 0x8E);
    setIdtGate(34, (uint64_t)irq2, 0x08, 0x8E);
    setIdtGate(35, (uint64_t)irq3, 0x08, 0x8E);
    setIdtGate(36, (uint64_t)irq4, 0x08, 0x8E);
    setIdtGate(37, (uint64_t)irq5, 0x08, 0x8E);
    setIdtGate(38, (uint64_t)irq6, 0x08, 0x8E);
    setIdtGate(39, (uint64_t)irq7, 0x08, 0x8E);
    setIdtGate(40, (uint64_t)irq8, 0x08, 0x8E);
    setIdtGate(41, (uint64_t)irq9, 0x08, 0x8E);
    setIdtGate(42, (uint64_t)irq10, 0x08, 0x8E);
    setIdtGate(43, (uint64_t)irq11, 0x08, 0x8E);
    setIdtGate(44, (uint64_t)irq12, 0x08, 0x8E);
    setIdtGate(45, (uint64_t)irq13, 0x08, 0x8E);
    setIdtGate(46, (uint64_t)irq14, 0x08, 0x8E);
    setIdtGate(47, (uint64_t)irq15, 0x08, 0x8E);


    setIdtGate(128, (uint64_t)isr128, 0x08, 0x8E); //System calls
    setIdtGate(177, (uint64_t)isr177, 0x08, 0x8E); //System calls

    idt_flush((uint64_t)&idt_ptr);

}

void setIdtGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags){

    idt_entries[num].base_low  = base & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].ist       = 0;
    idt_entries[num].flags     = flags | 0x60;
    idt_entries[num].base_mid  = (base >> 16) & 0xFFFF;
    idt_entries[num].base_high = (base >> 32) & 0xFFFFFFFF;
    idt_entries[num].zero      = 0;

}

char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(struct InterruptRegisters* regs){
    if (regs->int_no < 32){
        print_str(exception_messages[regs->int_no]);
        print_str("\n");
        print_str("Exception! System Halted\n");
        for (;;);
    }
}

void *irq_routines[16] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void irq_install_handler (int irq, void (*handler)(struct InterruptRegisters *r)){
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters* regs){
    void (*handler)(struct InterruptRegisters *regs);

    handler = irq_routines[regs->int_no - 32];

    if (handler){
        handler(regs);
    }

    if (regs->int_no >= 40){
        outPortB(0xA0, 0x20);
    }

    outPortB(0x20,0x20);
}
