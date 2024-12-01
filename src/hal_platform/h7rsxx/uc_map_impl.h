/* 
   Copyright 2015 Quazar Technologies Pvt. Ltd.
   Copyright 2015 Chintalagiri Shashank
   
   This file is part of
   Embedded bootstraps : peripheral usage maps : TI Launchpad MSP420F5529
   
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
 * @file uc_map.h
 * @brief uC Peripheral Map for STM32F4xx MCUs
 *
 * This file contains the peripheral map for the uC. This map was written 
 * in the context of the STM32_F4VE Black based on the STM32F407VE. Ideally, 
 * we'd want to generalize this file to other MCUs as they get used. 
 */

#ifndef uC_MAP_F4xx
#define uC_MAP_F4xx

#include <hal/uc/map.h>
#include <application.h>
#include <stm32h7rsxx_hal.h>

#ifdef APP_ENABLE_GPIO
    #define uC_GPIO_ENABLED            APP_ENABLE_GPIO
#else
    #define uC_GPIO_ENABLED            1
#endif

#define uC_CORE_ENABLED                1

#ifdef APP_ENABLE_INSTRUCTION_CACHE
    #define uC_ICACHE_ENABLED          APP_ENABLE_INSTRUCTION_CACHE
#else
    #define uC_ICACHE_ENABLED          0
#endif

#ifdef APP_ENABLE_DATA_CACHE
    #define uC_DCACHE_ENABLED          APP_ENABLE_DATA_CACHE
#else 
    #define uC_DCACHE_ENABLED          0
#endif 

#ifdef APP_ENABLE_SPECULATIVE_EXECUTION
    #define uC_SPEC_EX_ENABLED         APP_ENABLE_SPECULATIVE_EXECUTION
#else 
    #define uC_SPEC_EX_ENABLED         0
#endif

#if APP_ENABLE_SYSTICK
    #define uC_SYSTICK_TIMER_ENABLED   1
    #define uC_SYSTICK_TIMER_IRQH      time_systick_handler
#else
    // The timer still needs to be enabled for the STM32 Cube HAL Tick. 
    #define uC_SYSTICK_TIMER_ENABLED   1
#endif

#if APP_ENABLE_RTC 
    #define uC_RTC_ENABLED              APP_ENABLE_RTC 
    #define uC_RTC_1HZ_OUT_ENABLED      APP_ENABLE_RTC_1HZ_OUT
#endif

#if APP_ENABLE_ENTROPY
    #define uC_ENTROPY_ENABLED          APP_ENABLE_ENTROPY
    #define uC_ENTROPY_POOL_SIZE        APP_ENTROPY_POOL_SIZE
    #define uC_ENTROPY_MODE             APP_ENTROPY_MODE
#endif

#if APP_ENABLE_EEPROM
    #define uC_EEPROM_ENABLED           1
    #define uC_EEPROM_PROVIDER          APP_EEPROM_PROVIDER
    #define uC_EEPROM_SIZE              APP_EEPROM_SIZE
    #define uC_EEPROM_SIZE_t            APP_EEPROM_SIZETYPE
#endif

#if APP_ENABLE_ID
    #define uC_ID_ENABLED               1
    #define uC_ID_TRIMFUNC              APP_ID_TRIMFUNC
#endif

#if APP_ENABLE_USB
    #define uC_USB_ENABLED              1
    #define uC_USB_DEVICE_RHPORT        APP_USB_DEVICE_RHPORT

    #define uC_USBCDC0_ENABLED          APP_ENABLE_USBCDC0
    #define uC_USBCDC1_ENABLED          APP_ENABLE_USBCDC1
    #define uC_USBCDC2_ENABLED          APP_ENABLE_USBCDC2
    
    #define uC_USBCDCS_ENABLED          (uC_USBCDC0_ENABLED + uC_USBCDC1_ENABLED + uC_USBCDC2_ENABLED)

    #if uC_USBCDCS_ENABLED
        #define uC_USBCDC_ENABLED           1
    #endif

    #define uC_USBCDC0_PT_ENABLED           APP_ENABLE_USBCDC0_PT
    #define uC_USBCDC1_PT_ENABLED           APP_ENABLE_USBCDC1_PT
    #define uC_USBCDC2_PT_ENABLED           APP_ENABLE_USBCDC2_PT

    #define uC_USBCDC_PTS_ENABLED           (uC_USBCDC0_PT_ENABLED + uC_USBCDC1_PT_ENABLED + uC_USBCDC2_PT_ENABLED)

    #if uC_USBCDC_PTS_ENABLED
        #define uC_USBCDC_PT_ENABLED        1
    #endif
#endif


#if APP_ENABLE_XSPIM1
    #define uC_POWER_XSPIM1             APP_ENABLE_XSPIM1
#endif
#if APP_ENABLE_XSPIM2
    #define uC_POWER_XSPIM2             APP_ENABLE_XSPIM2
#endif

#if APP_ENABLE_UART

    #define uC_UART1_ENABLED            APP_ENABLE_UART1
    #define uC_UART2_ENABLED            APP_ENABLE_UART2
    #define uC_UART3_ENABLED            APP_ENABLE_UART3
    #define uC_UART4_ENABLED            APP_ENABLE_UART4
    #define uC_UART5_ENABLED            APP_ENABLE_UART5
    #define uC_UART6_ENABLED            APP_ENABLE_UART6
    #define uC_UART7_ENABLED            APP_ENABLE_UART7
    #define uC_UART8_ENABLED            APP_ENABLE_UART8

    #define uC_UARTS_ENABLED            (uC_UART1_ENABLED + uC_UART2_ENABLED + uC_UART3_ENABLED + uC_UART4_ENABLED + uC_UART5_ENABLED + uC_UART6_ENABLED + uC_UART7_ENABLED + uC_UART8_ENABLED)

    #if uC_UARTS_ENABLED
        #define uC_UART_ENABLED         1
    #endif

    #define uC_UART1_PT_ENABLED         APP_ENABLE_UART1_PT
    #define uC_UART2_PT_ENABLED         APP_ENABLE_UART2_PT
    #define uC_UART3_PT_ENABLED         APP_ENABLE_UART3_PT
    #define uC_UART4_PT_ENABLED         APP_ENABLE_UART4_PT
    #define uC_UART5_PT_ENABLED         APP_ENABLE_UART5_PT
    #define uC_UART6_PT_ENABLED         APP_ENABLE_UART6_PT
    #define uC_UART7_PT_ENABLED         APP_ENABLE_UART7_PT
    #define uC_UART8_PT_ENABLED         APP_ENABLE_UART8_PT

    #define uC_UART_PTS_ENABLED         (uC_UART1_PT_ENABLED + uC_UART2_PT_ENABLED + uC_UART3_PT_ENABLED + uC_PT_UART4_ENABLED + uC_PT_UART5_ENABLED + uC_PT_UART6_ENABLED + uC_PT_UART7_ENABLED + uC_PT_UART8_ENABLED)


    #if uC_UART_PTS_ENABLED
        #define uC_UART_PT_ENABLED         1
    #endif
    
#endif

#if APP_ENABLE_TIMER
    #define uC_TIMER_CCR_t      uint32_t

    #define uC_TIM2_ENABLED     APP_ENABLE_TIM2   // GP 32b 4CC DMA
    #define uC_TIM3_ENABLED     APP_ENABLE_TIM3   // GP 32b 4CC DMA
    #define uC_TIM4_ENABLED     APP_ENABLE_TIM4   // GP 32b 4CC DMA
    #define uC_TIM5_ENABLED     APP_ENABLE_TIM5   // GP 32b 4CC DMA
    
    #define uC_TIM6_ENABLED     APP_ENABLE_TIM6   // Basic 16b 0CC DMA
    #define uC_TIM7_ENABLED     APP_ENABLE_TIM7   // Basic 16b 0CC DMA
    
    #define uC_TIM9_ENABLED     APP_ENABLE_TIM9   // GP 16b 2CC
    #define uC_TIM12_ENABLED    APP_ENABLE_TIM12  // GP 16b 2CC
    #define uC_TIM13_ENABLED    APP_ENABLE_TIM13  // GP 16b 1CC
    #define uC_TIM14_ENABLED    APP_ENABLE_TIM14  // GP 16b 1CC

    #define uC_TIM15_ENABLED    APP_ENABLE_TIM15  // GP 16b 2CC DMA
    #define uC_TIM16_ENABLED    APP_ENABLE_TIM16  // GP 16b 1CC DMA
    #define uC_TIM17_ENABLED    APP_ENABLE_TIM17  // GP 16b 1CC DMA

    #define uC_LPTIM1_ENABLED   APP_ENABLE_LPTIM1  // LP 16b 2CC DMA
    #define uC_LPTIM2_ENABLED   APP_ENABLE_LPTIM2  // LP 16b 2CC DMA
    #define uC_LPTIM3_ENABLED   APP_ENABLE_LPTIM3  // LP 16b 2CC DMA
    #define uC_LPTIM4_ENABLED   APP_ENABLE_LPTIM4  // LP 16b 2CC DMA
    #define uC_LPTIM5_ENABLED   APP_ENABLE_LPTIM5  // LP 16b 2CC DMA

    #define uC_TIMERS_ENABLED   ( \
        uC_TIM2_ENABLED + uC_TIM3_ENABLED + uC_TIM4_ENABLED + uC_TIM5_ENABLED + \
        uC_TIM6_ENABLED + uC_TIM7_ENABLED + \
        uC_TIM9_ENABLED + uC_TIM12_ENABLED + uC_TIM13_ENABLED + uC_TIM14_ENABLED + \
        uC_TIM15_ENABLED + uC_TIM16_ENABLED + uC_TIM17_ENABLED \
    )
    
    #if uC_TIMERS_ENABLED
        #define uC_TIMER_ENABLED        1
    #endif

    #if uC_TIM15_CH1_OUTPUT_MODE != TIMER_OUTMODE_NONE
        #define uC_TIM15_CH1_OUTPUT_ENABLED     1
    #else
        #define uC_TIM15_CH1_OUTPUT_ENABLED     0
    #endif
    
#endif

#if APP_ENABLE_LPTIM
    // Not Implemented
    #define uC_LPTIM1_ENABLED   APP_ENABLE_LPTIM1  // LP 16b 2CC DMA
    #define uC_LPTIM2_ENABLED   APP_ENABLE_LPTIM2  // LP 16b 2CC DMA
    #define uC_LPTIM3_ENABLED   APP_ENABLE_LPTIM3  // LP 16b 2CC DMA
    #define uC_LPTIM4_ENABLED   APP_ENABLE_LPTIM4  // LP 16b 2CC DMA
    #define uC_LPTIM5_ENABLED   APP_ENABLE_LPTIM5  // LP 16b 2CC DMA

    #define uC_LPTIMERS_ENABLED   ( \
        uC_LPTIM1_ENABLED + uC_LPTIM2_ENABLED + uC_LPTIM3_ENABLED + \
        uC_LPTIM4_ENABLED + uC_LPTIM4_ENABLED \
    )
#endif


#if APP_ENABLE_ADC
    #define uC_ADC_ENABLED                  1
    #define uC_ADC_CONFIGURE_CHANNELS       APP_ADC_CONFIGURE_CHANNELS

    #define uC_ADC1_ENABLED                 (APP_ADC1_MODE ? 1 : 0)
    #define uC_ADC2_ENABLED                 (APP_ADC2_MODE ? 1 : 0)
    #define uC_ADCS_ENABLED                 (uC_ADC1_ENABLED + uC_ADC2_ENABLED)

    #ifdef APP_ENABLE_AIN0          
        #define uC_ENABLE_AIN0          APP_ENABLE_AIN0
    #else
        #define uC_ENABLE_AIN0          0
    #endif

    #ifndef uC_ADC1_TRIGSRC
        #define uC_ADC1_TRIGSRC         0xFF
    #endif

    #ifndef uC_ADC2_TRIGSRC
        #define uC_ADC2_TRIGSRC         0xFF
    #endif

    #ifdef APP_ENABLE_AIN1          
        #define uC_ENABLE_AIN1          APP_ENABLE_AIN1
    #else
        #define uC_ENABLE_AIN1          0
    #endif

    #ifdef APP_ENABLE_AIN2          
        #define uC_ENABLE_AIN2          APP_ENABLE_AIN2
    #else
        #define uC_ENABLE_AIN2          0
    #endif

    #ifdef APP_ENABLE_AIN3          
        #define uC_ENABLE_AIN3          APP_ENABLE_AIN3
    #else
        #define uC_ENABLE_AIN3          0
    #endif

    #ifdef APP_ENABLE_AIN4
        #define uC_ENABLE_AIN4          APP_ENABLE_AIN4
    #else
        #define uC_ENABLE_AIN4          0
    #endif
    
    #ifdef APP_ENABLE_AIN5
        #define uC_ENABLE_AIN5          APP_ENABLE_AIN5
    #else
        #define uC_ENABLE_AIN5          0
    #endif

    #ifdef APP_ENABLE_AIN6
        #define uC_ENABLE_AIN6          APP_ENABLE_AIN6
    #else
        #define uC_ENABLE_AIN6          0
    #endif

    #ifdef APP_ENABLE_AIN7
        #define uC_ENABLE_AIN7          APP_ENABLE_AIN7
    #else
        #define uC_ENABLE_AIN7          0
    #endif

    #ifdef APP_ENABLE_AIN8
        #define uC_ENABLE_AIN8          APP_ENABLE_AIN8
    #else
        #define uC_ENABLE_AIN8          0
    #endif

    #ifdef APP_ENABLE_AIN9
        #define uC_ENABLE_AIN9          APP_ENABLE_AIN9
    #else
        #define uC_ENABLE_AIN9          0
    #endif

    #ifdef APP_ENABLE_AIN10
        #define uC_ENABLE_AIN10         APP_ENABLE_AIN10
    #else
        #define uC_ENABLE_AIN10         0
    #endif

    #ifdef APP_ENABLE_AIN11
        #define uC_ENABLE_AIN11         APP_ENABLE_AIN11
    #else
        #define uC_ENABLE_AIN11         0
    #endif

    #ifdef APP_ENABLE_AIN12
        #define uC_ENABLE_AIN12         APP_ENABLE_AIN12
    #else
        #define uC_ENABLE_AIN12         0
    #endif

    #ifdef APP_ENABLE_AIN13
        #define uC_ENABLE_AIN13         APP_ENABLE_AIN13
    #else
        #define uC_ENABLE_AIN13         0
    #endif

    #ifdef APP_ENABLE_AIN14
        #define uC_ENABLE_AIN14         APP_ENABLE_AIN14
    #else
        #define uC_ENABLE_AIN14         0
    #endif

    #ifdef APP_ENABLE_AIN15
        #define uC_ENABLE_AIN15         APP_ENABLE_AIN15
    #else
        #define uC_ENABLE_AIN15         0
    #endif

    #ifdef APP_ENABLE_AIN18
        #define uC_ENABLE_AIN18         APP_ENABLE_AIN18
    #else
        #define uC_ENABLE_AIN18         0
    #endif

    #ifdef APP_ENABLE_AIN_TEMP
        #define uC_ENABLE_AIN_TEMP      APP_ENABLE_AIN_TEMP
    #else
        #define uC_ENABLE_AIN_TEMP      0
    #endif

    #ifdef APP_ENABLE_AIN_IREF
        #define uC_ENABLE_AIN_IREF      APP_ENABLE_AIN_IREF
    #else
        #define uC_ENABLE_AIN_IREF      0
    #endif

    #ifdef APP_ENABLE_AIN_VBAT
        #define uC_ENABLE_AIN_VBAT      APP_ENABLE_AIN_VBAT
    #else
        #define uC_ENABLE_AIN_IREF      0
    #endif

    #ifdef APP_ENABLE_AIN_VDDC
        #define uC_ENABLE_AIN_VDDC      APP_ENABLE_AIN_VDDC
    #else
        #define uC_ENABLE_AIN_VDDC      0
    #endif

    #define uC_ADC1_CHNMASK ((uC_ENABLE_AIN0 == 1 ? (1U << 0) : 0) | \
                   (uC_ENABLE_AIN1 == 1 ? (1U << 1) : 0) | \
                   (uC_ENABLE_AIN2 == 1 ? (1U << 2) : 0) | \
                   (uC_ENABLE_AIN3 == 1 ? (1U << 3) : 0) | \
                   (uC_ENABLE_AIN4 == 1 ? (1U << 4) : 0) | \
                   (uC_ENABLE_AIN5 == 1 ? (1U << 5) : 0) | \
                   (uC_ENABLE_AIN6 == 1 ? (1U << 6) : 0) | \
                   (uC_ENABLE_AIN7 == 1 ? (1U << 7) : 0) | \
                   (uC_ENABLE_AIN8 == 1 ? (1U << 8) : 0) | \
                   (uC_ENABLE_AIN9 == 1 ? (1U << 9) : 0) | \
                   (uC_ENABLE_AIN10 == 1 ? (1U << 10) : 0) | \
                   (uC_ENABLE_AIN11 == 1 ? (1U << 11) : 0) | \
                   (uC_ENABLE_AIN12 == 1 ? (1U << 12) : 0) | \
                   (uC_ENABLE_AIN13 == 1 ? (1U << 13) : 0) | \
                   (uC_ENABLE_AIN14 == 1 ? (1U << 14) : 0) | \
                   (uC_ENABLE_AIN15 == 1 ? (1U << 15) : 0) | \
                   (uC_ENABLE_AIN_TEMP == 1 ? (1U << 16) : 0) | \
                   (uC_ENABLE_AIN_IREF == 1 ? (1U << 17) : 0) | \
                   (uC_ENABLE_AIN18 == 1 ? (1U << 18) : 0))

    #define uC_ADC2_CHNMASK ((uC_ENABLE_AIN0 == 2 ? (1U << 0) : 0) | \
                   (uC_ENABLE_AIN1 == 2 ? (1U << 1) : 0) | \
                   (uC_ENABLE_AIN2 == 2 ? (1U << 2) : 0) | \
                   (uC_ENABLE_AIN3 == 2 ? (1U << 3) : 0) | \
                   (uC_ENABLE_AIN4 == 2 ? (1U << 4) : 0) | \
                   (uC_ENABLE_AIN5 == 2 ? (1U << 5) : 0) | \
                   (uC_ENABLE_AIN6 == 2 ? (1U << 6) : 0) | \
                   (uC_ENABLE_AIN7 == 2 ? (1U << 7) : 0) | \
                   (uC_ENABLE_AIN8 == 2 ? (1U << 8) : 0) | \
                   (uC_ENABLE_AIN9 == 2 ? (1U << 9) : 0) | \
                   (uC_ENABLE_AIN10 == 2 ? (1U << 10) : 0) | \
                   (uC_ENABLE_AIN11 == 2 ? (1U << 11) : 0) | \
                   (uC_ENABLE_AIN12 == 2 ? (1U << 12) : 0) | \
                   (uC_ENABLE_AIN13 == 2 ? (1U << 13) : 0) | \
                   (uC_ENABLE_AIN14 == 2 ? (1U << 14) : 0) | \
                   (uC_ENABLE_AIN15 == 2 ? (1U << 15) : 0) | \
                   (uC_ENABLE_AIN_VBAT == 2 ? (1U << 16) : 0) | \
                   (uC_ENABLE_AIN_VDDC == 2 ? (1U << 17) : 0) | \
                   (uC_ENABLE_AIN18 == 2 ? (1U << 18) : 0))
#endif


// Peripherals not yet implemented for this platform
#define uC_WDT_ENABLED                  0
#define uC_SPI_ENABLED                  0
#define uC_SPI_PT_ENABLED               0
#define uC_I2C_ENABLED                  0
#define uC_I2C_PT_ENABLED               0


#endif
