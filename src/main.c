#include <stdint.h>
#include <stdbool.h>

void gpio_init(void);
void timer_init(void);
void interrupts_init(void);

int main(void) {
    gpio_init();
    interrupts_init();
    timer_init();

    for (;;) {
        __asm__ volatile ("wfi");
    }
}
