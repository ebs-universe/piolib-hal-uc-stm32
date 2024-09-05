
#include "stm32f4xx_hal.h"
#include <application.h>
#include <irq_handlers.h>
#include <platform/debug.h>
#include <hal/uc/gpio.h>
#include "exti_handlers.h"


volatile uint8_t __exti_handler_inclusion;
void (* exti_gpio_handlers[MAX_EXTI_GPIO_LINES])(uint8_t) = {0};

static inline void exti_handler(uint8_t exti_line) { 
    if (exti_gpio_handlers[exti_line]) { 
        exti_gpio_handlers[exti_line](exti_line);
    }
}

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR0) { 
        EXTI->PR |= EXTI_PR_PR0;
        exti_handler(0);
    }
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR1) { 
        EXTI->PR |= EXTI_PR_PR1;
        exti_handler(1);
    }
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR2) { 
        EXTI->PR |= EXTI_PR_PR2;
        exti_handler(2);
    }
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR3) { 
        EXTI->PR |= EXTI_PR_PR3;
        exti_handler(3);
    }
}

/**
  * @brief This function handles EXTI line4 interrupts.
  */
void EXTI4_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR4) { 
        EXTI->PR |= EXTI_PR_PR4;
        exti_handler(4);
    }
}

/**
  * @brief This function handles EXTI line 5-9 interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
    for (uint32_t line = 5; line <= 9; ++line) {
        if (EXTI->PR & (1 << line)) { 
            EXTI->PR |= (1 << line); 
            exti_handler(line);
        }
    }
}


/**
  * @brief This function handles EXTI line 10-15 interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
    for (uint32_t line = 10; line <= 15; ++line) {
        if (EXTI->PR & (1 << line)) { 
            EXTI->PR |= (1 << line); 
            exti_handler(line);
        }
    }
}
