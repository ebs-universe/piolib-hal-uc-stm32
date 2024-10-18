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
#include "stm32u0xx_hal.h"

// All applications will need these
#define uC_CORE_ENABLED                1
#define uC_GPIO_ENABLED                1

#if APP_ENABLE_SYSTICK
    #define uC_SYSTICK_TIMER_ENABLED   1
    #define uC_SYSTICK_TIMER_IRQH      time_systick_handler
#else
    // The timer still needs to be enabled for the STM32 Cube HAL Tick. 
    #define uC_SYSTICK_TIMER_ENABLED   1
#endif

#if APP_ENABLE_ENTROPY
    #define uC_ENTROPY_ENABLED          APP_ENABLE_ENTROPY
    #define uC_ENTROPY_POOL_SIZE        APP_ENTROPY_POOL_SIZE
    #define uC_ENTROPY_MODE             APP_ENTROPY_MODE
#endif

#if APP_ENABLE_EEPROM
    #error "STM32U0 has no EEPROM, no backup SRAM. Use external nvstorage or the 9 backup registers instead."
    #define uC_EEPROM_ENABLED           1
    #define uC_EEPROM_PROVIDER          0
    #define uC_EEPROM_SIZE              0
    #define uC_EEPROM_SIZE_t            uint16_t
#endif

#if APP_ENABLE_ID
    #define uC_ID_ENABLED               1
    #define uC_ID_TRIMFUNC              APP_ID_TRIMFUNC
#endif

#if APP_ENABLE_CRYPTO
    #define uC_CRYPTO_ENABLED              APP_ENABLE_CRYPTO
    #define uC_CRYPTO_HARDWARE             APP_CRYPTO_HARDWARE
    #define uC_CRYPTO_INTERRUPT            APP_CRYPTO_INTERRUPT
    #ifndef APP_CRYPTO_NO_DEFAULT_CONTEXT
        #define uC_CRYPTO_DEFAULT_CTX          1
        #define uC_CRYPTO_DEFAULT_ALG          APP_CRYPTO_ALG
        #define uC_CRYPTO_DEFAULT_KEYSIZE      APP_CRYPTO_KEYSIZE / 8
        #define uC_CRYPTO_DEFAULT_IVSIZE       APP_CRYPTO_IVSIZE  / 8
        #define uC_CRYPTO_DEFAULT_INGESTSIZE   APP_CRYPTO_INGESTSIZE / 8
        #define uC_CRYPTO_DEFAULT_BLOCKSIZE    APP_CRYPTO_BLOCKSIZE / 8
        #define uC_CRYPTO_DEFAULT_HEADERSIZE   APP_CRYPTO_HEADERSIZE / 8 
        #define uC_CRYPTO_DEFAULT_MACSIZE      APP_CRYPTO_MACSIZE / 8
        #define uC_CRYPTO_DEFAULT_SWAP         APP_CRYPTO_SWAP
        #define uC_CRYPTO_DEFAULT_MODE         APP_CRYPTO_MODE
    #endif
#endif

#if APP_ENABLE_RTC 
    #define uC_RTC_ENABLED              APP_ENABLE_RTC 
    #define uC_RTC_1HZ_OUT_ENABLED      APP_ENABLE_RTC_1HZ_OUT
#endif

#if APP_ENABLE_UART

    #define uC_UART1_ENABLED            APP_ENABLE_UART1
    #define uC_UART2_ENABLED            APP_ENABLE_UART2
    #define uC_UART3_ENABLED            APP_ENABLE_UART3
    #define uC_UART4_ENABLED            APP_ENABLE_UART4
    #define uC_UART5_ENABLED            APP_ENABLE_UART5
    #define uC_UART6_ENABLED            APP_ENABLE_UART6
    #define uC_UART7_ENABLED            APP_ENABLE_UART7

    #define uC_UARTS_ENABLED            (uC_UART1_ENABLED + uC_UART2_ENABLED + uC_UART3_ENABLED + uC_UART4_ENABLED + uC_UART5_ENABLED + uC_UART6_ENABLED + uC_UART7_ENABLED)

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

    #define uC_UART_PTS_ENABLED         (uC_UART1_PT_ENABLED + uC_UART2_PT_ENABLED + uC_UART3_PT_ENABLED + uC_PT_UART4_ENABLED + uC_PT_UART5_ENABLED + uC_PT_UART6_ENABLED + uC_PT_UART7_ENABLED)


    #if uC_UART_PTS_ENABLED
        #define uC_UART_PT_ENABLED         1
    #endif
    
#endif


// Peripherals not yet implemented for this platform
#define uC_WDT_ENABLED                  0
#define uC_TIMER_ENABLED                0
#define uC_ADC_ENABLED                  0
#define uC_SPI_ENABLED                  0
#define uC_SPI_PT_ENABLED               0
#define uC_I2C_ENABLED                  0
#define uC_I2C_PT_ENABLED               0

#if APP_ENABLE_USB
    #define uC_USB_ENABLED              1
    #define uC_USBCDC_ENABLED           1
    #define uC_PT_USBCDC_ENABLED        0
    #define uC_USBCDC_IF0               1
    #define uC_USBCDC_IF1               0
    #define uC_USBCDC_IF2               0
#endif

#endif
