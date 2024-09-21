/* 
   Copyright (c)
   (c) 2024 Chintalagiri Shashank
      
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : AVR
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

/**
 * @file entropy_impl.h
 * @brief STM32F4xx Entropy Implementation Notes
 *
 * TODO: 
 * We use a bytebuf here, but a bytebuf is suboptimal for the 32 bit processor.
 * Consider switching to a 32bit ringbuf instead once implemented in the ds lib.
 * 
 */


#ifndef ENTROPY_IMPL_H
#define ENTROPY_IMPL_H

#include <hal/uc/entropy.h>
#include <ds/bytebuf.h>

#ifndef uC_ENTROPY_POOL_SIZE
    #define uC_ENTROPY_POOL_SIZE    16
#endif

#if uC_ENTROPY_ENABLED

typedef struct {
    volatile uint8_t accumulating;
    bytebuf * entropy;
} entropy_state_t;

extern entropy_state_t entropy_state;

void entropy_reset(void);

static inline uint8_t entropy_available(void) {
    return bytebuf_cPopulation(entropy_state.entropy);
}

static inline uint8_t entropy_get_byte(void){
    if (!bytebuf_cPopulation(entropy_state.entropy)) {
        return 0x00;
    }
    uint8_t byte = bytebuf_cPopByte(entropy_state.entropy);
    #if uC_ENTROPY_MODE == 2
        // If there is atleast 32 bits of free space, trigger refill.
        if (bytebuf_cGetFree(entropy_state.entropy) >= 4U) {
            entropy_reset();
        }
    #endif
    return byte;
}

#endif
#endif
