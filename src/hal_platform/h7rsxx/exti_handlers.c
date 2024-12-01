
#include "stm32h7rsxx_hal.h"
#include <application.h>
#include <irq_handlers.h>
#include <platform/debug.h>
#include <hal/uc/gpio.h>
#include "exti_handlers.h"
#include <platform/sections.h>

// TODO Switch to ITCM/DTCM untested

volatile uint8_t __exti_handler_inclusion;
void (* exti_gpio_handlers[MAX_EXTI_GPIO_LINES])(uint8_t) FASTDATA;

FASTEXEC
__attribute__((target("thumb")))
static inline void exti_handler(uint8_t exti_line) { 
    if (EXTI->PR1 & (1U << exti_line)) { 
        EXTI->PR1 |= (1U << exti_line);
    }
    if (exti_gpio_handlers[exti_line]) { 
        exti_gpio_handlers[exti_line](exti_line);
    }
}

FASTEXEC
void EXTI0_IRQHandler(void)
{
    exti_handler(0);
}

FASTEXEC
void EXTI1_IRQHandler(void)
{
    exti_handler(1);
}

FASTEXEC
void EXTI2_IRQHandler(void)
{
    exti_handler(2);
}

FASTEXEC
void EXTI3_IRQHandler(void)
{
    exti_handler(3);
}

FASTEXEC
void EXTI4_IRQHandler(void)
{
    exti_handler(4);
}

FASTEXEC
void EXTI5_IRQHandler(void)
{
    exti_handler(5);
}

FASTEXEC
void EXTI6_IRQHandler(void)
{
    exti_handler(6);
}

FASTEXEC
void EXTI7_IRQHandler(void)
{
    exti_handler(7);
}

FASTEXEC
void EXTI8_IRQHandler(void)
{
    exti_handler(8);
}

FASTEXEC
void EXTI9_IRQHandler(void)
{
    exti_handler(9);
}

FASTEXEC
void EXTI10_IRQHandler(void)
{
    exti_handler(10);
}

FASTEXEC
void EXTI11_IRQHandler(void)
{
    exti_handler(11);
}

FASTEXEC
void EXTI12_IRQHandler(void)
{
    exti_handler(12);
}

FASTEXEC
void EXTI13_IRQHandler(void)
{
    exti_handler(13);
}

FASTEXEC
void EXTI14_IRQHandler(void)
{
    exti_handler(14);
}

FASTEXEC
void EXTI15_IRQHandler(void)
{
    exti_handler(15);
}
