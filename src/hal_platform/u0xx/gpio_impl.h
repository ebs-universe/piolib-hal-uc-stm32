/* 
   Copyright 2024 Chintalagiri Shashank
   Copyright 2015 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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

#ifndef GPIO_IMPL_H
#define GPIO_IMPL_H

#include <hal/uc/gpio.h>
#include "exti.h"
#include "exti_handlers.h"

#if uC_GPIO_ENABLED

#define GPIO_OFS_MODE    0x00    /*!< GPIO port mode register                   */
#define GPIO_OFS_OTYPE   0x04    /*!< GPIO port output type register            */
#define GPIO_OFS_OSPEED  0x08    /*!< GPIO port output speed register           */
#define GPIO_OFS_PUPD    0x0C    /*!< GPIO port pull-up/pull-down register      */
#define GPIO_OFS_ID      0x10    /*!< GPIO port input data register             */
#define GPIO_OFS_OD      0x14    /*!< GPIO port output data register            */
#define GPIO_OFS_BSR     0x18    /*!< GPIO port bit set/reset register          */  
#define GPIO_OFS_LCK     0x1C    /*!< GPIO port configuration lock register     */
#define GPIO_OFS_AF0     0x20    /*!< GPIO alternate function register 0        */
#define GPIO_OFS_AF1     0x24    /*!< GPIO alternate function register 1        */


static inline void gpio_conf_output(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

static inline void gpio_conf_input(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2)));
}

static inline void gpio_conf_analog(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder |= (3U << (pin * 2));                                
}

static inline void gpio_conf_periph(
    PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t periph) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (2U << (pin * 2));
    
    HAL_SFR_t *afr = (HAL_SFR_t *)(port + ((pin < 8) ? GPIO_OFS_AF0 : GPIO_OFS_AF1));
    uint32_t afr_position = (pin % 8) * 4;
    *afr = (*afr & ~(0xF << afr_position)) | ((periph & 0xF) << afr_position);
}

static inline void gpio_conf_pullup(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

static inline void gpio_conf_pulldown(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (2U << (pin * 2));
}

static inline void gpio_conf_hiz(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2)));
}

static inline void gpio_conf_outmode_pp(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) &= ~(1 << pin);
}

static inline void gpio_conf_outmode_od(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) |= (1 << pin);                            
}


static inline void gpio_conf_speed(
    PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t speed)
{
    HAL_SFR_t *ospeedr = (HAL_SFR_t *)(port + GPIO_OFS_OSPEED);
    *ospeedr = (*ospeedr & ~(3U << (pin * 2))) | ((speed & 3U) << (pin * 2));
}

static inline void gpio_set_output_high(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(port + GPIO_OFS_BSR) = (1 << pin);
}

static inline void gpio_set_output_low(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(port + GPIO_OFS_BSR) = (uint32_t)(1 << pin) << 16U;
}

static inline void gpio_set_output_toggle(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    // The HAL implementation reads OD and uses BSR. We're just 
    // manipulating OD directly. 
    *(HAL_SFR_t *)(port + GPIO_OFS_OD) ^= (1 << pin);
}

static inline HAL_BASE_t gpio_get_input(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    return (*(HAL_SFR_t *)(port + GPIO_OFS_ID) & (1 << pin)) ? 1 : 0;
}

static inline uint8_t gpio_get_port_index(PORTSELECTOR_t port) {
   return (uint8_t)((port - GPIOA_BASE) / (0x0400));
}

static inline void gpio_conf_interrupt(
    PORTSELECTOR_t port, PINSELECTOR_t pin, HAL_BASE_t edge)
{
   // Configure EXTI Source Port Selection
   HAL_SFR_t *exticr = (HAL_SFR_t *)(&(SYSCFG->EXTICR[pin >> 2U]));
   uint32_t bit_position = (pin & 0x03U) * 4U;
   *exticr = (*exticr & ~(0x0FU << bit_position)) | (gpio_get_port_index(port) << bit_position);

   // Configure Trigger Edge Selection
   if (edge & GPIO_INT_EDGE_RISING) { 
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_RTSR) |= (1 << pin);                            
   } else {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_RTSR) &= ~(1 << pin);                            
   }

   if (edge & GPIO_INT_EDGE_FALLING) {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_FTSR) |= (1 << pin);                            
   } else {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_FTSR) &= ~(1 << pin);                            
   }
}

static inline void gpio_conf_interrupt_handler(
    PORTSELECTOR_t port, PINSELECTOR_t pin, void (*handler)(uint8_t))
{
   exti_gpio_handlers[pin] = handler;
}


static inline void gpio_interrupt_arm(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_PR) = (1 << pin);
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR) |= (1 << pin);
}

static inline void gpio_interrupt_disarm(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR) &= ~(1 << pin);
}

#endif
#endif
