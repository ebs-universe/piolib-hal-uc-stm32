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


#include "core_impl.h"

#if uC_CORE_ENABLED

/** 
 * Initialize CPU features to sane defaults. 
 *  
 * It is expected that applications needing something different will not 
 * use this portion of the HAL and instead use the platform specific library 
 * or code to set up desired features for the core.
 */
static void cpu_init(void);

/** 
 * Initialize the core interrupt controller. This does tend to be somewhat 
 * application specific, so HAL implementations for this function should 
 * generally just set up sane defaults.
 * 
 * On this platform, initializes the NVIC Priority Grouping to 2, ie, 
 * 
 * Sets priorities to 2 bits of Preemptible and 2 bits of Subpriority.
 * This approach should probably work for most Cortex M4 processors. 
 *  
 * It is expected that applications needing something different will not 
 * use the HAL and instead use the platform specific library or code to set 
 * up the NVIC or similar.
 * 
 */
static void nvic_init(void);

/**
 * Enable clocks needed for the core platform.
 */
static void enable_core_clocks(void);


static void cpu_init(void){
    #if (INSTRUCTION_CACHE_ENABLE != 0U)
        __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    #endif /* INSTRUCTION_CACHE_ENABLE */

    #if (DATA_CACHE_ENABLE != 0U)
        __HAL_FLASH_DATA_CACHE_ENABLE();
    #endif /* DATA_CACHE_ENABLE */

    #if (PREFETCH_ENABLE != 0U)
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    #endif /* PREFETCH_ENABLE */
}

static void enable_core_clocks(void){
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

static void nvic_init(void) {
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
}

void core_init(void) {
    cpu_init();
    nvic_init();
    enable_core_clocks();
}


/**
* @brief Sets the MCU Power controls to defaults.
* 
* Not yet implemented
*/
void power_set_full(void){
    ;
}

/**
* @brief Sets System Clock Prescaler
*  
* Not used by this implementation. See clock_set_default.
*/
void clock_set_prescaler(uint16_t ps){
    ;    
}

/**
* @brief Initialize all the clocks to some sane defaults. 
* 
*   Since CubeMX is generating much of this code for us, we leave this to the 
*   application. The contents of this function will normally be that of 
*   SystemClock_Config in main.c of CubeMX generated code. 
*
*/
__weak void clock_set_default(void){
    ;
}

#endif

#if uC_WDT_ENABLED == 1

/**
* @brief Initializes the Watchdog timer. Nothing to do here.
* 
*/
void watchdog_init(void) {
    ;
}


/**
* @brief Initializes the Watchdog timer. Nothing to do here.
* 
* Presently just sets the default values. This should be changed to 
* accept some parameter, perhaps interval, and set accordingly. 
* 
* Default should be an approximately 2 sec timeout.
*/
void watchdog_start(void){
    ;
}

void watchdog_hold(void){
    ;
}

#endif

#if uC_SYSTICK_TIMER_ENABLED

void core_systick_start(void) {
    HAL_InitTick(TICK_INT_PRIORITY);
}

#endif