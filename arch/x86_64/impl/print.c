#include "print.h"

static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

static struct Char* const buffer = (struct Char*) 0xb8000;
static size_t col = 0;
static size_t row = 0;
static uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);

static void clear_row(size_t row_index) {
    struct Char empty = {
        .character = ' ',
        .color = color,
    };

    for (size_t current_col = 0; current_col < NUM_COLS; current_col++) {
        buffer[current_col + (NUM_COLS * row_index)] = empty;
    }
}

void print_clear(void) {
    col = 0;
    row = 0;

    for (size_t current_row = 0; current_row < NUM_ROWS; current_row++) {
        clear_row(current_row);
    }
}

static void print_newline(void) {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t current_row = 1; current_row < NUM_ROWS; current_row++) {
        for (size_t current_col = 0; current_col < NUM_COLS; current_col++) {
            buffer[current_col + (NUM_COLS * (current_row - 1))] =
                buffer[current_col + (NUM_COLS * current_row)];
        }
    }

    clear_row(NUM_ROWS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col >= NUM_COLS) {
        print_newline();
    }

    buffer[col + (NUM_COLS * row)] = (struct Char) {
        .character = (uint8_t) character,
        .color = color,
    };

    col++;
}

void print_str(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | (background << 4);
}
