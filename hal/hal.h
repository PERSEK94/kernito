#ifndef HAL_H
#define HAL_H

void hal_init(void);
void hal_putchar(char c);
char hal_getchar(void);
void hal_print(const char *str);
void hal_poweroff(void);

#endif