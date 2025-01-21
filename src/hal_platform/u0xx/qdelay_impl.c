/* 
   Copyright (c)
     (c) 2024 Chintalagiri Shashank
   
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


#include "qdelay_impl.h"
#include <string.h>

#if uC_QDELAY_ENABLED 

qdelay_spec_t qdelay_spec = {0};
qdelay_t _delay = {0};

#if uC_QDELAY_PROVIDER == QDELAY_PROVIDER_CPU
    
#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_SYSTICK
 
    void qdelay_init(void) {
        qdelay_spec.max = *(HAL_SFR_t *)(SysTick_BASE + OFS_SYSTICK_LOAD);
        qdelay_spec.tick_freq = CLOCKTREE_SYSTICK_CLK / 1000000;
    }

    void qdelay_start(qdelay_t * delay, uint16_t us) { 
        delay->start = *(HAL_SFR_t *)(SysTick_BASE + OFS_SYSTICK_VAL);
        HAL_BASE_t ticks = ((uint32_t)us * qdelay_spec.tick_freq);
        
        if (ticks > qdelay_spec.max) {
            die();
        }

        if (ticks > delay->start) {
            ticks -= delay->start;
            delay->end = qdelay_spec.max - ticks;
        } else {
            delay->end = delay->start - ticks;
        }
    }

    bool qdelay_check_done(qdelay_t * delay) {
        uint32_t now = *(HAL_SFR_t *)(SysTick_BASE + OFS_SYSTICK_VAL);
        bool result;
        if (delay->end > delay->start) {
            result = (now <= delay->end && now > delay->start);
        } else {
            result = (now <= delay->end || now > delay->start);
        }
        return result;
    }

#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_TIMER

    #include <hal/uc/timer.h>

    void qdelay_init(void) {
        // TODO Also initialize timer here?

        qdelay_spec.max = timer_get_top(APP_QDELAY_TIMER_INTFNUM);
        qdelay_spec.tick_freq   = APP_QDELAY_TIMER_CLK_FREQ / 1000000;
    }

    void qdelay_start(qdelay_t * delay, int us) {

    }

    bool qdelay_check_done(qdelay_t * delay) {
        return false;
    }

#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_DWT

    void qdelay_init(void) {
        ;
    }

    void qdelay_start(qdelay_t * delay, int us) {

    }

    bool qdelay_check_done(qdelay_t * delay) {
        return false;
    }

#else
    #error "qdelay provider not recognized"
#endif
#endif
