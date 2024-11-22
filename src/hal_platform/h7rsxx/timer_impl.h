/* 
   Copyright (c)
     (c) 2024 Chintalagiri Shashank
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : AVR
   
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
 *  - 
 */

#ifndef TIMER_IMPL_H
#define TIMER_IMPL_H

#include <hal/uc/timer.h>

#ifdef uC_TIMER_ENABLED

typedef enum {
    TIMER_LP_HWIF,
    TIMER_ADV_HWIF,
    TIMER_GP_HWIF,
    TIMER_BASIC_HWIF,
} TIMER_HWIF_TYPE;

typedef struct _TIMER_HWIF_t{
    const TIMER_HWIF_TYPE type;
    const HAL_ADDRESS_t base;
    const TIMER_COUNTER_TYPE_t ctype;
    const TIMER_WIDTH_t width;
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

#define OFS_TIMx_CR1    0x000
#define OFS_TIMx_CR2    0x004
#define OFS_TIMx_SMCR   0x008
#define OFS_TIMx_DIER   0x00C
#define OFS_TIMx_SR     0x010
#define OFS_TIMx_EGR    0x014
#define OFS_TIMx_CCMR1  0x018
#define OFS_TIMx_CCMR2  0x01C
#define OFS_TIMx_CCER   0x020
#define OFS_TIMx_CNT    0x024
#define OFS_TIMx_PSC    0x028
#define OFS_TIMx_ARR    0x02C
#define OFS_TIMx_RCR    0x030 
#define OFS_TIMx_CCR1   0x034
#define OFS_TIMx_CCR2   0x038
#define OFS_TIMx_CCR3   0x03C
#define OFS_TIMx_CCR4   0x040
#define OFS_TIMx_BDTR   0x044
#define OFS_TIMx_CCR5   0x048
#define OFS_TIMx_CCR6   0x04C
#define OFS_TIMx_CCMR3  0x050
#define OFS_TIMx_DTR2   0x054
#define OFS_TIMx_ECR    0x058
#define OFS_TIMx_TISEL  0x05C
#define OFS_TIMx_AF1    0x060
#define OFS_TIMx_AF2    0x064
#define OFS_TIMx_DCR    0x3DC
#define OFS_TIMx_DMAR   0x3E0

#ifndef uC_TIMER_DEFAULT_CLK_FREQ
  #define uC_TIMER_DEFAULT_CLK_FREQ    100000000ULL
#endif

#if uC_TIM15_ENABLED
    #ifndef uC_TIMER15_CLK_FREQ
      #define uC_TIMER15_CLK_FREQ     uC_TIMER_DEFAULT_CLK_FREQ
    #endif
    #ifndef uC_TIMER15_PRESCALER
      #define uC_TIMER15_PRESCALER    CLOCKTREE_TIM15_FREQ / uC_TIMER15_CLK_FREQ
    #endif
    #ifndef uC_TIMER15_INTERRUPT_ENABLE
      #define uC_TIMER15_INTERRUPT_ENABLE 0
    #endif
#endif

static inline void timer_init(uint8_t intfnum){
    timer_if[intfnum]->init();
}

// Change timer prescaler.
static inline void timer_set_prescaler(uint8_t intfnum, uint16_t prescaler);

// Start the timer in the provided mode. 
static inline void timer_set_mode(uint8_t intfnum, uint8_t mode);

// Enable the timer overflow interrupt.
static inline void timer_enable_int_overflow(uint8_t intfnum);

// Disable the timer overflow interrupt.
static inline void timer_disable_int_overflow(uint8_t intfnum);

// Enable the timer top interrupt.
static inline void timer_enable_int_top(uint8_t intfnum);

// Disable the timer top interrupt.
static inline void timer_disable_int_top(uint8_t intfnum);

// Set TOP for the timer.
static inline void timer_set_top(uint8_t intfnum, uint16_t top);

// Get TOP for the timer.
static inline uint16_t timer_get_top(uint8_t intfnum);

// Set output mode for the timer channel.
static inline void timer_set_outmode_ch( uint8_t intfnum, uint8_t channel, uint8_t outmode);

// Enable the compare match interrupt for the timer channel.
static inline void timer_enable_int_ch ( uint8_t intfnum, uint8_t channel);

// Disable the compare match interrupt for the timer channel.
static inline void timer_disable_int_ch( uint8_t intfnum, uint8_t channel);

// Set the channel compare match value.
static inline void timer_set_cmr_ch(uint8_t intfnum, uint8_t channel, uint16_t cm);

// Get the channel compare match value.
static inline uint16_t timer_get_cmr_ch(uint8_t intfnum, uint8_t channel);


#endif
#endif
