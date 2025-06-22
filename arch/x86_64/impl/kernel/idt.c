#include "stdint.h"
#include "util.h"
#include "idt.h"
#include "print.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;


// CPU pause
static inline void io_wait() {
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}

// Halt for a moment (can be called multiple times)
void debug_pause() {
    for (int i = 0; i < 100000; ++i) io_wait();
}

// Infinite halt loop for breakpoints
void halt_forever() {
    while (1) __asm__("hlt");
}

// Print, then sleep a bit
#define dbg(msg) do { print_str(msg); print_str("\n"); debug_pause(); } while (0)
#define dbg_halt(msg) do { print_str(msg); print_str("\n"); halt_forever(); } while (0)


extern void idt_flush(uint64_t);

void irq_handler_stub(void) {
    // Minimal side-effect-free test
    print_str("[IRQ0 fired]\n");
}

// Example usage in initIdtDebug()
void initIdtDebug() {
    dbg("IDT start");

    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint64_t)&idt_entries;
    dbg("Set IDT pointer");

    memset(&idt_entries, 0, sizeof(idt_entries));
    dbg("Cleared IDT");

    // Remap the PICs
    outPortB(0x20, 0x11); // Start init of master PIC
    outPortB(0xA0, 0x11); // Start init of slave PIC
    outPortB(0x21, 0x20); // Master PIC vector offset = 0x20
    outPortB(0xA1, 0x28); // Slave PIC vector offset = 0x28
    outPortB(0x21, 0x04); // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outPortB(0xA1, 0x02); // Tell Slave PIC its cascade identity (0000 0010)
    outPortB(0x21, 0x01); // Set Master PIC to 8086/88 mode
    outPortB(0xA1, 0x01); // Set Slave PIC to 8086/88 mode

    // Mask all IRQs initially (we'll unmask selectively later)
    outPortB(0x21, 0xFF); // All IRQs masked on master
    outPortB(0xA1, 0xFF); // All IRQs masked on slave
    
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

    print_hex("IDT base: ", idt_ptr.base);
    print_hex("IDT limit: ", idt_ptr.limit);
    print_str("\n");

    print_hex("IDT[0] base_low: ", idt_entries[0].base_low);
    print_hex("IDT[0] sel: ", idt_entries[0].sel);
    print_hex("IDT[0] flags: ", idt_entries[0].flags);
    print_hex("IDT[0] base_high: ", idt_entries[0].base_high);
    
    print_hex("IDT[32] base_low: ", idt_entries[32].base_low);
    print_hex("IDT[32] sel: ", idt_entries[32].sel);
    print_hex("IDT[32] flags: ", idt_entries[32].flags);
    print_hex("IDT[32] base_high: ", idt_entries[32].base_high);
    
    idt_flush((uint64_t)&idt_ptr);
    
    
    uint64_t base = ((uint64_t)idt_entries[32].base_low) |
                ((uint64_t)idt_entries[32].base_mid << 16) |
                ((uint64_t)idt_entries[32].base_high << 32);
                print_hex("Vector 32 addr: ", base);


    dbg("Flushed IDT and halting");
}


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
    idt_entries[num].flags     = flags;
    // idt_entries[num].flags     = flags | 0x60;
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
    dbg("INSTALLING IRQ HANDLER");
    irq_routines[irq] = handler;
    print_hex("Installing IRQ handler at ", (uint64_t)handler);
    print_hex("Stored in irq_routines[0]: ", (uint64_t)irq_routines[0]);
}

void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters* regs) {
    dbg("irq_handler...");
    if (((uint64_t)regs & 0xF) != 0) {
        // Will crash or infinite loop cleanly if stack is misaligned
        while (1) { __asm__("cli; hlt"); }
    }

    // Optional: manually verify expected int_no
    if (regs->int_no != 32) {
        while (1) { __asm__("cli; hlt"); }
    }

    // outPortB(0x20, 0x20);  // Send EOI

    // print_str("IN IRQ HANDLER\n");
    // print_hex("int_no: ", regs->int_no);
    // print_hex("err_cd: ", regs->err_code);
    
    // // Print what iretq will return to
    // uint64_t* stack = (uint64_t*)regs;
    // print_hex("RIP:    ", stack[17]);
    // print_hex("CS:     ", stack[18]);
    // print_hex("RFLAGS: ", stack[19]);

    // // Optional: print int number
    // print_hex("int_no: ", stack[15]);
    // print_hex("err_cd: ", stack[16]);

    // while (1);  // <== HALT SYSTEM BEFORE IRETQ

    // int irq_num = regs->int_no - 32;

    // // Defensive check: only handle valid IRQs 0-15
    // if (irq_num >= 0 && irq_num < 16) {
    //     void (*handler)(struct InterruptRegisters *regs) = (void (*)(struct InterruptRegisters *)) irq_routines[irq_num];
    //     if (handler) {
    //         handler(regs);
    //     }
    // }

    // // Send End-of-Interrupt (EOI) to PICs
    // if (regs->int_no >= 40) {
    //     outPortB(0xA0, 0x20); // EOI to slave PIC
    // }
    // outPortB(0x20, 0x20);     // EOI to master PIC
}
