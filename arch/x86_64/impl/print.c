#include "print.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

static struct Char* const buffer = (struct Char*) 0xb8000;
static size_t col = 0;
static size_t row = 0;
static uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);

void clear_row(size_t r) {
    struct Char empty = {
        .character = ' ',
        .color = color,
    };

    for (size_t c = 0; c < NUM_COLS; c++) {
        buffer[c + NUM_COLS * r] = empty;
    }
}

void print_clear() {
    for (size_t r = 0; r < NUM_ROWS; r++) {
        clear_row(r);
    }
    row = 0;
    col = 0;
}

void print_newline() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    // Scroll up
    for (size_t r = 1; r < NUM_ROWS; r++) {
        for (size_t c = 0; c < NUM_COLS; c++) {
            buffer[c + NUM_COLS * (r - 1)] = buffer[c + NUM_COLS * r];
        }
    }

    clear_row(NUM_ROWS - 1); // ← Corrected from NUM_COLS - 1
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col >= NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char){
        .character = (uint8_t)character,
        .color = color,
    };
    col++;
}

void print_str(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

static const char* hex_digits = "0123456789ABCDEF";

void print_hex(const char* label, uint64_t value) {
    print_str(label);
    print_str("0x");
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        print_char(hex_digits[nibble]);
    }
    print_str("\n");
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | (background << 4);
}