#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "print.h"

static const unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0,
    '*',
    0,
    ' ',
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0,
};

const char* keyboard_layout(void) {
    return "US basic";
}

void keyboard_handle_irq(void) {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        pic_send_eoi(1);
        return;
    }

    if (scancode < sizeof(keyboard_map)) {
        unsigned char c = keyboard_map[scancode];
        if (c != 0) {
            print_char((char)c);
        }
    }

    pic_send_eoi(1);
}
