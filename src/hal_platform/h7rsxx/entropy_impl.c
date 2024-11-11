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


#include "entropy_impl.h"
#include <ds/bytebuf.h>

#if uC_ENTROPY_ENABLED


uint8_t entropy_buffer[uC_ENTROPY_POOL_SIZE];
bytebuf entropy_pool;
entropy_state_t entropy_state;

static inline void _entropy_state_init(void){
    if (!entropy_pool._size) {
        bytebuf_vInit(entropy_buffer, uC_ENTROPY_POOL_SIZE, &entropy_pool);
    }
    entropy_state.entropy = &entropy_pool;
}

void entropy_init(void) {
    _entropy_state_init();
    if (!bytebuf_cGetFree(entropy_state.entropy)){
        return;
    }
    __HAL_RCC_RNG_CLK_ENABLE();
    RNG->CR |= (RNG_CR_RNGEN | RNG_CR_IE);
    NVIC_EnableIRQ(RNG_IRQn);
    entropy_state.accumulating = 1;
}

void entropy_deinit(void) {
    RNG->CR &= ~(RNG_CR_RNGEN | RNG_CR_IE);
    __HAL_RCC_RNG_CLK_DISABLE();
    entropy_state.accumulating = 0;
}

#endif
