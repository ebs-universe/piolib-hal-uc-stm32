/*  
   Copyright (c)
     (c) 2024 Chintalagiri Shashank
   
   This file is part of
      EBS : HAL Implementation : STM32H7RS
   
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

/**
 * 
 * This impementation does not presently support LP Timers
 * 
 * Timer Clock Configuration
 * -------------------------
 * 
 *  - Use prescaler to produce a fixed 100MHz timer counter period. 
 *  - ARR can be set to set actual period, with overflow at ARR
 */

#ifndef TIMER_IMPL_H
#define TIMER_IMPL_H

#include <hal/uc/timer.h>

#ifdef uC_TIMER_ENABLED

typedef enum {
    TIMER_LP_HWIF,
    TIMER_ADV_HWIF,
    TIMER_GP1_HWIF,
    TIMER_GP2_HWIF,
    TIMER_BASIC_HWIF,
} TIMER_HWIF_TYPE;

typedef struct _TIMER_HWIF_t{
    const TIMER_HWIF_TYPE type;
    const HAL_ADDRESS_t base;
    const TIMER_COUNTER_TYPE_t ctype;
    const TIMER_WIDTH_t width;
    const uint8_t cc_channels;
} _timer_hwif_t;

typedef struct _TIMER_STATE_t{
    TIMER_MODE_t mode;
} timer_state_t;

typedef struct TIMER_IF_t {
    const _timer_hwif_t * const hwif;
    timer_state_t * const state;
    void (*init)(void);
} timer_if_t;

extern const timer_if_t *const timer_if[uC_TIMERS_ENABLED + 1];

#include "_timer/registers.h"
#include "_timer/defaults.h"

#if uC_TIM15_ENABLED
    extern const timer_if_t tim15_if;
    extern timer_state_t tim15_state;
#endif

void _timer_set_mode(const _timer_hwif_t *const hwif, TIMER_MODE_t mode);

static inline void _timer_set_prescaler(const _timer_hwif_t *const hwif, uint16_t prescaler){
    HAL_SFR_t * psc = (HAL_SFR_t *)(hwif->base + OFS_TIMx_PSC);
    *psc = (prescaler - 1);
}

static inline void timer_init(HAL_BASE_t intfnum){
    timer_if[intfnum]->init();
}

static inline void timer_set_prescaler(HAL_BASE_t intfnum, uint16_t prescaler){
    _timer_set_prescaler(timer_if[intfnum]->hwif, prescaler);
}

static inline void timer_set_mode(HAL_BASE_t intfnum, TIMER_MODE_t mode){
    if (timer_if[intfnum]->state->mode != mode) {
        timer_if[intfnum]->state->mode = mode;
        _timer_set_mode(timer_if[intfnum]->hwif, mode);
    }
}

static inline void timer_enable_int_overflow(HAL_BASE_t intfnum){
    // Not implemented yet
    ;
}

static inline void timer_disable_int_overflow(HAL_BASE_t intfnum){
    // Not implemented yet
    ;
}

static inline void timer_enable_int_top(HAL_BASE_t intfnum){
    HAL_SFR_t * dier = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_DIER);
    *dier |= TIM_DIER_UIE;
}

static inline void timer_disable_int_top(HAL_BASE_t intfnum){
    HAL_SFR_t * dier = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_DIER);
    *dier &= ~TIM_DIER_UIE;
}

static inline void timer_set_top(HAL_BASE_t intfnum, HAL_BASE_t top){
    // Set the ARR. Other configurations, such as use of 
    // repetition, are not implemented.
    HAL_SFR_t * arr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_ARR);
    *arr = top;
};

// Get TOP for the timer.
static inline HAL_BASE_t timer_get_top(HAL_BASE_t intfnum){
    HAL_SFR_t * arr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_ARR);
    return *arr;
};

// Set output mode for the timer channel.
static inline void timer_set_outmode_ch(HAL_BASE_t intfnum, HAL_BASE_t channel, TIMER_OUTMODE_t outmode){
    const _timer_hwif_t *const hwif = timer_if[intfnum]->hwif;
    
    if (channel >= hwif->cc_channels) {
        die();
    }

    uint8_t chn_ccmr_ofs = (channel & 0x1) ? 0 : 8;
    uint8_t chn_ccer_ofs = (channel - 1) << 2;
    HAL_SFR_t * ccmr;
    HAL_SFR_t * ccer = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_CCER);
    *ccer &= ~(0x1F << chn_ccer_ofs);
    
    switch (channel >> 2){
      case 0: 
          ccmr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_CCMR1);
          break;
      case 1:
          ccmr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_CCMR2);
          break;
      case 2:
          ccmr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_CCMR3);
          break;
      default:
          die();
    }

    *ccmr &= ~(0x100FF << chn_ccmr_ofs);

    /**
     * CCMR Bits: 
     * CCnS     00 Set as Output
     * OCnFE    0  Unsure. Leaving default until otherwise determined
     * OCnPE    0  Unsure. Leaving default until otherwise determined
     * OCnCE    0  Unsure. Leaving default until otherwise determined
     */
    uint8_t outmode_bits = 0;
    switch (outmode)
    {
      case TIMER_OUTMODE_TOGGLE:
        outmode_bits = 0b0011;
        break;
      case TIMER_OUTMODE_PWM:
        outmode_bits = 0b0110;
        break;
      case TIMER_OUTMODE_NPWM:
        outmode_bits = 0b0111;
        break;
      case TIMER_OUTMODE_NONE:
        outmode_bits = 0b0000;
        break;
      default:
        die();
    }
    *ccmr |= (outmode_bits << (4 + chn_ccmr_ofs));
    
    /**
     * CCER Bits: 
     * CCnE     1  Enable CCn Output
     * CCnP     0  Leaving default
     * CCnNE    0  Not enabling complementary output
     * CCnNP    0  Leaving default 
     */
    *ccer |= ((TIM_CCER_CC1E) << chn_ccer_ofs);
};

// // Enable the compare match interrupt for the timer channel.
static inline void timer_enable_int_ch(HAL_BASE_t intfnum, HAL_BASE_t channel){
    const _timer_hwif_t *const hwif = timer_if[intfnum]->hwif;
    
    if (channel >= hwif->cc_channels || channel > 4 || channel == 0){
        die();
    }

    HAL_SFR_t * dier = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_DIER);
    *dier |= (1 << channel);
}

// // Disable the compare match interrupt for the timer channel.
static inline void timer_disable_int_ch(HAL_BASE_t intfnum, HAL_BASE_t channel){
    const _timer_hwif_t *const hwif = timer_if[intfnum]->hwif;
    
    if (channel >= hwif->cc_channels || channel > 4 || channel == 0){
        die();
    }

    HAL_SFR_t * dier = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + OFS_TIMx_DIER);
    *dier &= ~(1 << channel);
}

// Set the channel compare match value.
static inline void timer_set_cmr_ch(HAL_BASE_t intfnum, HAL_BASE_t channel, uC_TIMER_CCR_t cm){
    const _timer_hwif_t *const hwif = timer_if[intfnum]->hwif;
    uint8_t ccr_ofs;

    if (channel >= hwif->cc_channels) {
        die();
    }

    switch (channel)
    {
        case 1:
            ccr_ofs = OFS_TIMx_CCR1;
            break;
        case 2:
            ccr_ofs = OFS_TIMx_CCR2;
            break;
        case 3:
            ccr_ofs = OFS_TIMx_CCR3;
            break;
        case 4:
            ccr_ofs = OFS_TIMx_CCR4;
            break;
        case 5:
            ccr_ofs = OFS_TIMx_CCR5;
            break;
        case 6:
            ccr_ofs = OFS_TIMx_CCR6;
            break;
        default:
            die();
            break;
    }

    HAL_SFR_t * ccr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + ccr_ofs);
    *ccr = cm;
}

// Get the channel compare match value.
static inline uC_TIMER_CCR_t timer_get_cmr_ch(HAL_BASE_t intfnum, HAL_BASE_t channel){
    const _timer_hwif_t *const hwif = timer_if[intfnum]->hwif;
    uint8_t ccr_ofs;

    if (channel >= hwif->cc_channels) {
        die();
    }

    switch (channel)
    {
        case 1:
            ccr_ofs = OFS_TIMx_CCR1;
            break;
        case 2:
            ccr_ofs = OFS_TIMx_CCR2;
            break;
        case 3:
            ccr_ofs = OFS_TIMx_CCR3;
            break;
        case 4:
            ccr_ofs = OFS_TIMx_CCR4;
            break;
        case 5:
            ccr_ofs = OFS_TIMx_CCR5;
            break;
        case 6:
            ccr_ofs = OFS_TIMx_CCR6;
            break;
        default:
            die();
            break;
    }
    
    HAL_SFR_t * ccr = (HAL_SFR_t *)(timer_if[intfnum]->hwif->base + ccr_ofs);
    return *ccr;
}

#endif
#endif
