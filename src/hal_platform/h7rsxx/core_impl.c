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


__weak void mpu_init(void){
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    HAL_MPU_Disable();

    // Background protection for preventing speculative execution
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct = (MPU_Region_InitTypeDef){0};    
    
    // Read Access for ID
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x08fff000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4KB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

__weak void configure_memory_layout(void){
    FLASH_OBProgramInitTypeDef pOBInit = {0};
    
    if (HAL_FLASH_Unlock() != HAL_OK) { die(); }
    if (HAL_FLASH_OB_Unlock() != HAL_OK) { die(); }
    
    pOBInit.OptionType = OPTIONBYTE_USER;
    pOBInit.USERType = OB_USER_SRAM_ECC 
                     | OB_USER_I2C_NI3C
                     | OB_USER_DTCM_AXI_SHARE 
                     | OB_USER_ITCM_AXI_SHARE;
    
    pOBInit.USERConfig2 = OB_I2C_NI3C_I2C
                        |OB_DTCM_AXI_SHARE_1
                        |OB_ITCM_AXI_SHARE_0
                        |OB_AXISRAM_ECC_DISABLE;

    if (HAL_FLASHEx_OBProgram(&pOBInit) != HAL_OK) { die(); }
    if (HAL_FLASH_OB_Lock() != HAL_OK) { die(); }
    if (HAL_FLASH_Lock() != HAL_OK)    { die(); }
}

__weak void cpu_init(void){
    #if uC_ICACHE_ENABLED == 1U
        SCB_EnableICache();
    #else
        SCB_DisableICache();
    #endif /* INSTRUCTION_CACHE_ENABLE */

    #if uC_DCACHE_ENABLED == 1U
        SCB_EnableDCache();
    #else
        // TODO This hard faults. 
        // // Clean and disable DCache
        // SCB_CleanDCache();
        // __DSB(); // Data Synchronization Barrier
        // SCB_DisableDCache();
    #endif /* DATA_CACHE_ENABLE */
    __DSB(); // Data Synchronization Barrier
    __ISB(); // Instruction Synchronization Barrier
}

__weak void nvic_init(void) {
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
}

static void enable_core_clocks(void){
    __HAL_RCC_SBS_CLK_ENABLE();
}

void core_init(void) {
    mpu_init();
    cpu_init();
    nvic_init();
    power_set_full();
    enable_core_clocks();
    configure_memory_layout();
}


/** 
 * @brief Initialize MCU Power Controls to sane defaults. 
 * 
 * The defaults we use here involves:
 *  - Use LDO supply for the core, and not the SMPS, etc.
 *  - Enable power supply to XSPIM1 and XSPIM2
 *  - Enable the USB voltage detector (not sure exactly why)
 *  
 * This is the default power configuration for the Nucleo H7S3L8, 
 * and seems to be a reasonable approach if efficiency is not an
 * overriding concern.
 * 
 * Note that XSPIMn is included here because it might be used for
 * external memories in a way not controlled by the HAL. If, on 
 * the other hand, using these memories requires adding a HAL 
 * module, these power controls should move there.
 * 
 */
void power_set_full(void){
    if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK) { die(); }
    
    #if uC_POWER_XSPIM1
    HAL_PWREx_EnableXSPIM1();
    #endif

    #if uC_POWER_XSPIM2
    HAL_PWREx_EnableXSPIM2();
    #endif

    #if uC_USB_ENABLED 
    HAL_PWREx_EnableUSBHSregulator();
    // Nucleo seems to provide external 3v3. 
    // Actual configuration needed for regulator not known here.
    // HAL_PWREx_EnableUSBReg();
    #endif

    /* Enable USB Voltage detector */
    if(HAL_PWREx_EnableUSBVoltageDetector() != HAL_OK) { die(); }
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