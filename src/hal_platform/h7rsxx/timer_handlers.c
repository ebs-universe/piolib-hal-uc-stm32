
#include <application.h>
#include <irq_handlers.h>
#include <hal/uc/timer.h>
#include <hal/uc/gpio.h>
#include "timer_handlers.h"
#include <platform/sections.h>

volatile uint8_t __timer_handler_inclusion;

#if uC_TIMER_ENABLED

FASTEXEC
void TIM15_IRQHandler(void){
    uint32_t flags = *(HAL_SFR_t *) (tim15_if.hwif->base + OFS_TIMx_SR);
    if (flags & TIM_SR_UIF){
        *(HAL_SFR_t *) (tim15_if.hwif->base + OFS_TIMx_SR) &= ~TIM_SR_UIF;
        gpio_set_output_toggle(GPIO_DBG_SCOPE1);
    }
}

#endif