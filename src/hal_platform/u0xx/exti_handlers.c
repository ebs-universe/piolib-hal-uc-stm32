

#include <application.h>
#include <irq_handlers.h>
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
  * @brief This function handles EXTI lines 0-1 interrupts.
  * 
  */
void EXTI0_1_IRQHandler(void)
{
    uint32_t rpr = EXTI->RPR1;
    uint32_t fpr = EXTI->FPR1; 
    for (uint32_t line = 0; line <= 1; ++line) {
        if (rpr & (1 << line)) { 
            EXTI->RPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_RISING);
        }
        if (fpr & (1 << line)) { 
            EXTI->FPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_FALLING);
        }
    }
}

/**
  * @brief This function handles EXTI lines 2-3 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
    uint32_t rpr = EXTI->RPR1;
    uint32_t fpr = EXTI->FPR1; 
    for (uint32_t line = 2; line <= 3; ++line) {
        if (rpr & (1 << line)) { 
            EXTI->RPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_RISING);
        }
        if (fpr & (1 << line)) { 
            EXTI->FPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_FALLING);
        }
    }
}

/**
  * @brief This function handles EXTI line 4-15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
    uint32_t rpr = EXTI->RPR1;
    uint32_t fpr = EXTI->FPR1;   
    for (uint32_t line = 4; line <= 15; ++line) {
        if (rpr & (1 << line)) { 
            EXTI->RPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_RISING);
        }
        if (fpr & (1 << line)) { 
            EXTI->FPR1 |= (1 << line); 
            exti_handler(line | GPIO_INT_ON_FALLING);
        }
    }
}
