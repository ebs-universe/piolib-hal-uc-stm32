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

static inline void _entropy_clocks_init(void){
    // TODO This may need to be modified to use clock configuration 
    //   infrastructure in core instead of hardcoding STMCube HAL calls 
    RCC_PeriphCLKInitTypeDef clk_cfg_rng = {0};
    clk_cfg_rng.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    clk_cfg_rng.RngClockSelection = RCC_RNGCLKSOURCE_MSI;
    if (HAL_RCCEx_PeriphCLKConfig(&clk_cfg_rng) != HAL_OK){
        die();
    }
}

/*
 * Configure for NIST SP 800‑90B per ST AN4230
 *     CR 0x00F00D00
 *   HTCR 0xAAC7 @ 48MHz RNG clk
 * Note that the datasheet suggests this is is configuration C, optimized 
 * for speed. Validation may be needed for critical applications.
 * 
 * Additionally, we set bit 13 of CR since we expect the TRNG will typically
 * be left off post entropy pool filling. This may need to be configurable. 
 * 
 * This function is provided by the HAL implementation in addition to the 
 * usual API for use in case the hardware reports a seed error and the TRNG 
 * needs to be reset.
 * 
 * We don't actually do the reset if the entropy buffer is full for modes 1 
 * and 2. Here, just deinit the TRNG until it is needed next.
 */
void entropy_reset(void){
    #if uC_ENTROPY_MODE <= 2
    if (!bytebuf_cGetFree(entropy_state.entropy)){
        return;
    }
    #endif
    __HAL_RCC_RNG_CLK_ENABLE();
    NVIC_DisableIRQ(RNG_CRYP_IRQn);
    RNG->CR &= ~(RNG_CR_RNGEN | RNG_CR_IE);
    RNG->CR |= (RNG_CR_CONDRST | 
                (0x0F << RNG_CR_RNG_CONFIG1_Pos) | 
                (0x0D << RNG_CR_RNG_CONFIG3_Pos) |
                (0x01 << RNG_CR_RNG_CONFIG2_Pos));
    RNG->HTCR = 0xAAC7;
    RNG->CR &= ~RNG_CR_CONDRST;
    RNG->CR |= (RNG_CR_RNGEN | RNG_CR_IE);
    NVIC_EnableIRQ(RNG_CRYP_IRQn);
    entropy_state.accumulating = 1;
}

void entropy_init(void) {
    _entropy_state_init();
    _entropy_clocks_init();
    HAL_NVIC_SetPriority(RNG_CRYP_IRQn, 0, 0);
    entropy_reset();
}

void entropy_deinit(void) {
    RNG->CR &= ~(RNG_CR_RNGEN | RNG_CR_IE);
    __HAL_RCC_RNG_CLK_DISABLE();
    entropy_state.accumulating = 0;
}

#endif
