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

static void shell_show_help(void) {
    print_str("Commands:\n");
    print_str("  help     - show this help text\n");
    print_str("  about    - show information about DaymOS\n");
    print_str("  version  - show the current version\n");
    print_str("  ticks    - show PIT tick count\n");
    print_str("  clear    - clear the screen\n");
    print_str("  reboot   - reboot the machine\n");
    print_str("  panic    - trigger an invalid opcode exception\n");
}

static void shell_show_about(void) {
    print_str("DaymOS is a hobby x86_64 operating system.\n");
    print_str("Current features: GRUB boot, VGA console, IDT, PIT, keyboard, and a tiny shell.\n");
}

static void shell_show_version(void) {
    print_str("DaymOS version 0.1.0-dev\n");
}

static void shell_show_ticks(void) {
    print_str("ticks: ");
    print_dec(pit_get_ticks());
    print_char('\n');
}

static void shell_reboot(void) {
    print_str("Rebooting...\n");
    __asm__ volatile ("cli");
    __asm__ volatile ("outb %0, $0x64" : : "a"((unsigned char)0xFE));

    for (;;) {
        __asm__ volatile ("hlt");
    }
}

static void shell_trigger_panic(void) {
    print_str("Triggering invalid opcode exception...\n");
    __asm__ volatile ("ud2");
}

static void shell_execute_command(void) {
    if (shell_str_equals(line_buffer, "help")) {
        shell_show_help();
        return;
    }

    if (shell_str_equals(line_buffer, "about")) {
        shell_show_about();
        return;
    }

    if (shell_str_equals(line_buffer, "version")) {
        shell_show_version();
        return;
    }

    if (shell_str_equals(line_buffer, "clear")) {
        print_clear();
        return;
    }

    if (shell_str_equals(line_buffer, "ticks")) {
        shell_show_ticks();
        return;
    }

    if (shell_str_equals(line_buffer, "reboot")) {
        shell_reboot();
        return;
    }

    if (shell_str_equals(line_buffer, "panic")) {
        shell_trigger_panic();
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
