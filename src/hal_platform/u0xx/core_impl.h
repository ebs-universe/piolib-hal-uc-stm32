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

#ifndef CORE_IMPL_H
#define CORE_IMPL_H

#include <hal/uc/core.h>

// This is replicated from the STM32 HAL implementation because the HAL 
// implementation breaks with -Os. This only happens under some 
// circumstances, and it's not clear why exactly this happens or what 
// triggers it. 
HAL_StatusTypeDef hal_rcc_clockconfig(const RCC_ClkInitTypeDef  *const RCC_ClkInitStruct, uint32_t FLatency);

#if uC_WDT_ENABLED

static inline void watchdog_clear(void) {
    ;
}

#endif

#endif
