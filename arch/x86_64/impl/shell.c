#include "shell.h"
#include "print.h"

#define SHELL_BUFFER_SIZE 128

static char line_buffer[SHELL_BUFFER_SIZE];
static unsigned int line_length = 0;

static void shell_prompt(void) {
    print_str("daymos> ");
}

void shell_init(void) {
    line_length = 0;
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

        if (line_length == 0) {
            shell_prompt();
            return;
        }

        print_str("You typed: ");
        print_str(line_buffer);
        print_char('\n');

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
