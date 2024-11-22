/* 
Copyright (c)
  (c) 2024 Chintalagiri Shashank

This file is part of
Embedded bootstraps : Peripheral driver implementations : STM32

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/


#include "timer_impl.h"

#ifdef uC_TIMER_ENABLED

#if uC_TIM15_ENABLED

static const _timer_hwif_t _tim15_hwif = {
    .type = TIMER_GP_HWIF, 
    .base = TIM15_BASE,
    .ctype = TIMER_COUNTER_TYPE_UP,
    .width = TIMER_WIDTH_16B
};

timer_state_t tim15_state = {0};
void _tim15_init(void);

const timer_if_t tim15_if = {
    &_tim15_hwif,
    &tim15_state,
    _tim15_init
};

void _tim15_init(void){
    __HAL_RCC_TIM15_CLK_ENABLE();
    #if uC_TIMER15_INTERRUPT_ENABLE
    HAL_NVIC_SetPriority(TIM15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM15_IRQn);
    #endif
}

#endif

const timer_if_t *const timer_if[uC_TIMERS_ENABLED + 1] = {
    #if uC_TIM15_ENABLED
        &tim15_if,
    #endif
    NULL
};






#endif
