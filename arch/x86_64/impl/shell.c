#include "shell.h"
#include "pit.h"
#include "print.h"

#define SHELL_BUFFER_SIZE 128

static char line_buffer[SHELL_BUFFER_SIZE];
static unsigned int line_length = 0;

static void shell_prompt(void) {
    print_str("daymos> ");
}

static int shell_str_equals(const char* a, const char* b) {
    unsigned int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

static void shell_execute_command(void) {
    if (shell_str_equals(line_buffer, "help")) {
        print_str("Commands: help, clear, ticks\n");
        return;
    }

    if (shell_str_equals(line_buffer, "clear")) {
        print_clear();
        return;
    }

    if (shell_str_equals(line_buffer, "ticks")) {
        print_str("ticks: ");
        print_dec(pit_get_ticks());
        print_char('\n');
        return;
    }

    print_str("Unknown command: ");
    print_str(line_buffer);
    print_char('\n');
}

void shell_init(void) {
    line_length = 0;
    line_buffer[0] = '\0';

    print_clear();
    print_str("DaymOS ready.\n");
    print_str("Type 'help' for commands.\n\n");
    shell_prompt();
}

void shell_handle_char(char c) {
    if (c == '\r') {
        return;
    }

    if (c == '\b') {
        if (line_length > 0) {
            line_length--;
            line_buffer[line_length] = '\0';
            print_backspace();
        }
        return;
    }

    if (c == '\n') {
        line_buffer[line_length] = '\0';
        print_char('\n');

        if (line_length != 0) {
            shell_execute_command();
        }

        line_length = 0;
        line_buffer[0] = '\0';
        shell_prompt();
        return;
    }

    if (line_length + 1 >= SHELL_BUFFER_SIZE) {
        return;
    }

    line_buffer[line_length++] = c;
    line_buffer[line_length] = '\0';
    print_char(c);
}
