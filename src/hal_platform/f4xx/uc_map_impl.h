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
#include "stm32f4xx_hal.h"

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
    #define uC_USBCDC_ENABLED           1
    #define uC_PT_USBCDC_ENABLED        0
    #define uC_USBCDC_IF0               1
    #define uC_USBCDC_IF1               0
    #define uC_USBCDC_IF2               0
#endif

// Peripherals not yet implemented for this platform
#define uC_WDT_ENABLED                  0
#define uC_TIMER_ENABLED                0
#define uC_ADC_ENABLED                  0
#define uC_UART_ENABLED                 0
#define uC_UART_PT_ENABLED              0
#define uC_SPI_ENABLED                  0
#define uC_SPI_PT_ENABLED               0
#define uC_I2C_ENABLED                  0
#define uC_I2C_PT_ENABLED               0


#endif
