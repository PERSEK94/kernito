#include "hal/hal.h"
#include "core/shell.h"

void __main(void){}

int main(void) {
    hal_init();
    shell_run();
    return 0;
}
