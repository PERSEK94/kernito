#include "hal.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void hal_init(void) {
    uint16_t ubrr = 103;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    _delay_ms(200);
}

void hal_putchar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

char hal_getchar(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void hal_print(const char *str) {
    while (*str) {
        hal_putchar(*str++);
    }
}

void hal_poweroff(void) {
    hal_print("Apagando KERNITO... Sistema detenido.\r\n");
    hal_putchar(4);
    _delay_ms(100);
    cli();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
}