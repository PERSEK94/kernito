#include "shell.h"
#include "../hal/hal.h"
#include <stdint.h>

static uint8_t stawit(const char *text, const char *prefix) {
    while (*prefix) {
        if (*text != *prefix) return 0;
        text++;
        prefix++;
    }
    return 1;
}

static uint8_t equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == *b);
}

void shell_run(void) {
    hal_print("\033[2J\033[H");
    hal_print("KERNITO Modular v0.4\r\n");
    hal_print("kernito> ");

    char buffer[32];
    uint8_t index = 0;

    while (1) {
        char key = hal_getchar();

        if (key == '\r' || key == '\n') {
            buffer[index] = '\0';
            hal_print("\r\n");

            if (equals(buffer, "help")) {
                hal_print("Comandos: help, info, clear, poweroff, didi\r\n");
            } else if (equals(buffer, "info")) {
                hal_print("kernito\r\n");
            } else if (equals(buffer, "clear")) {
                hal_print("\033[2J\033[H");
            } else if (equals(buffer, "poweroff") || equals(buffer, "exit")) {
                hal_poweroff();
            } else if (equals(buffer, "didi")) {
                hal_print("BOIIII WHY YOU SUS\r\n");
            } else if (stawit(buffer, "echo ")) {
                hal_print(&buffer[5]);
                hal_print("\r\n");
            } else if (index > 0) {
                hal_print("Comando inexistente\r\n");
            } 

            index = 0;
            hal_print("kernito> ");
        } else if (key == '\b' || key == 127) {
            if (index > 0) {
                index--;
                hal_print("\b \b");
            }
        } else if (index < 31) {
            buffer[index++] = key;
            hal_putchar(key);
        }
    }
}
