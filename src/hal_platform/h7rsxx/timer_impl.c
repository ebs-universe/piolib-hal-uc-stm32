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


static inline void _timer_init(
        const _timer_hwif_t *const hwif, 
        uint32_t prescaler,
        EBS_BOOL_t buffered_top,
        TIMER_TRIGGEN_t triggen_src
    ) 
{
    /**
     * Following registers are unused and left at 0 : 
     *  SMCR
     */
    
    _timer_set_prescaler(hwif, prescaler);
    
    HAL_SFR_t * cr1 = (HAL_SFR_t *)(hwif->base + OFS_TIMx_CR1);    
    /**
     * CR1 Defaults : 
     * 
     * DITHEN   | 0 | 
     * UIREFMAP | 1 | We don't use it. Application might?
     * URS      | 0 | 
     * UDIS     | 0 | 
     * CKD      | 0 | Dead time not supported by EBS
     * 
     */
    *cr1 = 0x0000;
    *cr1 |= TIM_CR1_UIFREMAP;
    if (buffered_top) {
        *cr1 |= TIM_CR1_ARPE;
    }

    HAL_SFR_t * cr2 = (HAL_SFR_t *)(hwif->base + OFS_TIMx_CR1);    
    /**
     * CR2 Defaults : 
     * 
     * CCPC     | 0 | Default, not preloaded.
     * CCUS     | 0 | Default, not preloaded.
     * CCDS     | 1 | DMA request triggered by update (TOP)
     * TI1S     | 0 | Default, tim_t1_in -> tim_t1
     * OIS1     | 0 | Default
     * OIS1N    | 0 | Default
     * OIS2     | 0 | Default
     * 
     * MMS controlled by tiggen_src and applied from 
     * _timer_set_triggen_src.
     */
    *cr2 = 0x0000;
    *cr2 |= TIM_CR2_CCDS;

}

static void _timer_set_triggen_src(
        const _timer_hwif_t *const hwif, 
        uint8_t trignum, TIMER_TRIGGEN_t triggen_src)
{
    // WARNING This may be timer peripheral dependent. 
    // We may need to check HWIF type first. 

    uint8_t triggen_src_bits;
    switch (triggen_src)
    {
        case TIMER_TRIGGEN_START:
            triggen_src_bits = 0b0001;
            break;
        case TIMER_TRIGGEN_TOP:
            triggen_src_bits = 0b0010;
            break;
        case TIMER_TRIGGEN_CM1:
            triggen_src_bits = 0b0100;
            break;
        case TIMER_TRIGGEN_CM2:
            triggen_src_bits = 0b0101;
            break;
        case TIMER_TRIGGEN_CM3:
            triggen_src_bits = 0b0110;
            break;
        case TIMER_TRIGGEN_CM4:
            triggen_src_bits = 0b0111;
            break;
        case TIMER_TRIGGEN_CM5:
            triggen_src_bits = 0b1000;
            break;
        case TIMER_TRIGGEN_CM6:
            triggen_src_bits = 0b1001;
            break;
        case TIMER_TRIGGEN_NONE:
        default:
            triggen_src_bits = 0b0000;
            break;
    }

    HAL_SFR_t * cr2 = (HAL_SFR_t *)(hwif->base + OFS_TIMx_CR2);

    if (trignum == 0) {
        *cr2 &= ~TIM_CR2_MMS_Msk;
        *cr2 |= (((triggen_src_bits & 0x7) << 4) | ((triggen_src_bits & 0x8) << 22));
    } else if (trignum == 1) {                              
        *cr2 &= ~TIM_CR2_MMS2_Msk;
        *cr2 |= ((triggen_src_bits) << 20);
    }
}

#if uC_TIM15_ENABLED

static const _timer_hwif_t _tim15_hwif = {
    .type = TIMER_GP2_HWIF, 
    .base = TIM15_BASE,
    .ctype = TIMER_COUNTER_TYPE_UP,
    .width = TIMER_WIDTH_16B,
    .cc_channels = 2
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
    
    #if uC_TIM15_INTERRUPTS_ENABLE
    HAL_NVIC_SetPriority(TIM15_IRQn, PRIO_TIMER15, 0);
    HAL_NVIC_EnableIRQ(TIM15_IRQn);
    #endif
    
    _timer_init(
            tim15_if.hwif, 
            uC_TIM15_PRESCALER,
            uC_TIM15_BUFFERED_TOP,
            uC_TIM15_TRIGGEN_SRC
    );
    timer_set_top(uC_TIM15_INTFNUM, uC_TIM15_CYCLE_TOP);
    tim15_if.state->mode = TIMER_MODE_STOPPED;
    
    #if uC_TIM15_INTERRUPT_TOP_ENABLE
    timer_enable_int_top(uC_TIM15_INTFNUM);
    #endif
    
    #if uC_TIM15_CH1_OUTPUT_ENABLED
    timer_set_outmode_ch(uC_TIM15_INTFNUM, 1, uC_TIM15_CH1_OUTPUT_MODE);
    #endif

    #if uC_TIM15_CH1_OUTPUT_ENABLED || uC_TIM15_CH2_OUTPUT_ENABLED || uC_TIM15_TRIGGEN_SRC
      HAL_SFR_t * bdtr = (HAL_SFR_t *)(TIM15_BASE + OFS_TIMx_BDTR);
      *bdtr |= TIM_BDTR_MOE;
    #endif

    #ifdef uC_TIM15_TRIGGEN_SRC
        _timer_set_triggen_src(tim15_if.hwif, 0, uC_TIM15_TRIGGEN_SRC);
    #endif
}

#endif

const timer_if_t *const timer_if[uC_TIMERS_ENABLED + 1] = {
    #if uC_TIM15_ENABLED
        &tim15_if,
    #endif
    NULL
};

void _timer_set_mode(const _timer_hwif_t *const hwif, TIMER_MODE_t mode){
    HAL_SFR_t * cr1 = (HAL_SFR_t *)(hwif->base + OFS_TIMx_CR1);
    HAL_SFR_t * sr = (HAL_SFR_t *)(hwif->base + OFS_TIMx_SR);

    // We only implement UP for now. 
    // ADV and GP1 timers allow UPDOWN. 
    // Check HWIF type while implementing. 

    *sr = 0x00;

    if (mode == TIMER_MODE_ONESHOT){
        *cr1 |= TIM_CR1_OPM;
    } else { 
        *cr1 &= ~TIM_CR1_OPM;
    }

    if (mode == TIMER_MODE_STOPPED){
        *cr1 &= ~TIM_CR1_CEN;
        return;
    } else {
        *(HAL_SFR_t *)(hwif->base + OFS_TIMx_CNT) = 0;
        *cr1 |= TIM_CR1_CEN;
    }
}

#endif
