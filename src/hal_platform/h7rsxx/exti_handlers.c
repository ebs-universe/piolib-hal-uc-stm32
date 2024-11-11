
#include "stm32h7rsxx_hal.h"
#include <application.h>
#include <irq_handlers.h>
#include <platform/debug.h>
#include <hal/uc/gpio.h>
#include "exti_handlers.h"


volatile uint8_t __exti_handler_inclusion;
void (* exti_gpio_handlers[MAX_EXTI_GPIO_LINES])(uint8_t) = {0};

static inline void exti_handler(uint8_t exti_line) { 
    if (EXTI->PR1 & (1U << exti_line)) { 
        EXTI->PR1 |= (1U << exti_line);
    }
    if (exti_gpio_handlers[exti_line]) { 
        exti_gpio_handlers[exti_line](exti_line);
    }
}

void EXTI0_IRQHandler(void)
{
    exti_handler(0);
}

void EXTI1_IRQHandler(void)
{
    exti_handler(1);
}

void EXTI2_IRQHandler(void)
{
    exti_handler(2);
}

void EXTI3_IRQHandler(void)
{
    exti_handler(3);
}

void EXTI4_IRQHandler(void)
{
    exti_handler(4);
}

void EXTI5_IRQHandler(void)
{
    exti_handler(5);
}

void EXTI6_IRQHandler(void)
{
    exti_handler(6);
}

void EXTI7_IRQHandler(void)
{
    exti_handler(7);
}

void EXTI8_IRQHandler(void)
{
    exti_handler(8);
}

void EXTI9_IRQHandler(void)
{
    exti_handler(9);
}

void EXTI10_IRQHandler(void)
{
    exti_handler(10);
}

void EXTI11_IRQHandler(void)
{
    exti_handler(11);
}

void EXTI12_IRQHandler(void)
{
    exti_handler(12);
}

void EXTI13_IRQHandler(void)
{
    exti_handler(13);
}

void EXTI14_IRQHandler(void)
{
    exti_handler(14);
}

void EXTI15_IRQHandler(void)
{
    exti_handler(15);
}
