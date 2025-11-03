#include <stdint.h>
#include "timer.h"
#include "gpio.h"


#define PERIPH_BASE        0x3F000000UL
#define INTC_BASE         (PERIPH_BASE + 0x00B200)

#define IRQ_BASIC_PENDING (*(volatile uint32_t*)(INTC_BASE + 0x00))
#define IRQ_PENDING1      (*(volatile uint32_t*)(INTC_BASE + 0x04))
#define IRQ_PENDING2      (*(volatile uint32_t*)(INTC_BASE + 0x08))
#define IRQ_ENABLE1       (*(volatile uint32_t*)(INTC_BASE + 0x10))
#define IRQ_ENABLE2       (*(volatile uint32_t*)(INTC_BASE + 0x14))
#define IRQ_ENABLE_BASIC  (*(volatile uint32_t*)(INTC_BASE + 0x18))
#define IRQ_DISABLE1      (*(volatile uint32_t*)(INTC_BASE + 0x1C))

#define SYS_TIMER_MATCH1_IRQ (1u << 1)

#define SYS_TIMER_BASE    (PERIPH_BASE + 0x003000)
#define TIMER_CS          (*(volatile uint32_t*)(SYS_TIMER_BASE + 0x00))
#define CS_M1             (1u << 1)

static uint32_t tick_count = 0;
static bool led_state = false;

void interrupts_init(void) {
    IRQ_ENABLE1 = SYS_TIMER_MATCH1_IRQ;
}

void irq_handler(void) {
    if (IRQ_PENDING1 & SYS_TIMER_MATCH1_IRQ) {
        if (TIMER_CS & CS_M1) {
            TIMER_CS = CS_M1;
            tick_count++;
            if (tick_count >= 100u) {
                tick_count = 0;
                led_state = !led_state;
                gpio_set_red(led_state);
            }
            timer_schedule_next();
        }
        return;
    }
}

