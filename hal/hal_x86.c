#include "hal.h"
#include <stdint.h>

uint8_t shift_pressed = 0;
uint8_t caps_lock = 0;


volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static const char scancode_ascii[] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' '
};

static const char scancode_shift[] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~', 0,
    '|','Z','X','C','V','B','N','M','<','>','?', 0, '*', 0, ' '
};

void hal_update_cursor(void) {
    uint16_t pos = cursor_y * 80 + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void hal_clear_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (uint16_t)' ' | (uint16_t)(0x0A << 8);
    cursor_x = 0;
    cursor_y = 0;
    }
    hal_update_cursor();
}

void hal_init(void) {
    hal_clear_screen();
}

void hal_putchar(char c) {
    if (c == '\r') {
        cursor_x = 0;
        hal_update_cursor();
        return;
    }
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            uint16_t pos = cursor_y * 80 + cursor_x;
            vga_buffer[pos] = (uint16_t)' ' | (uint16_t)(0x0A << 8);
        }
    } else {
        uint16_t pos = cursor_y * 80 + cursor_x;
        vga_buffer[pos] = (uint16_t)c | (uint16_t)(0x0A << 8);
        cursor_x++;
    }

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= 25) {
        hal_clear_screen();
    }

    hal_update_cursor();
}
char hal_getchar(void) {
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t scancode = inb(0x60);
            if (scancode & 0x80) {
                uint8_t released = scancode & 0x7F;
                if (released == 0x2A || released == 0x36) {
                    shift_pressed = 0;
                }
                continue;
            }
            
            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressed = 1;
                continue;
            }
            if (scancode == 0x3A) {
                caps_lock = !caps_lock;
                continue;

            }
             if (scancode < sizeof(scancode_ascii)) {
                char c = scancode_ascii[scancode];
                if (c >= 'a' && c <= 'z') {
                    if (shift_pressed ^ caps_lock) {
                        return scancode_shift[scancode];
                    }
                    return c;
                }
                if (shift_pressed) {
                    return scancode_shift[scancode];
                }
                if (c != 0) return c;
            }

        }
    }
}

void hal_print(const char *str) {
    if (*str == '\033' && *(str + 1) == '[') {
        hal_clear_screen();
        return;
    }
    while (*str) {
        hal_putchar(*str++);
    }
}


static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}


void hal_poweroff(void) {
    hal_print("Apagando KERNITO x86...\r\n");
    outw(0x604, 0x2000);
    __asm__ volatile ("cli; hlt");
}