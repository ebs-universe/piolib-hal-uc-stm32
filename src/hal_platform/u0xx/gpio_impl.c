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


#include "gpio_impl.h"


#if uC_GPIO_ENABLED

static void enable_gpio_clocks(void);
static void enable_gpio_exti_irqs(void);


static void enable_gpio_clocks(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
}

static void enable_gpio_exti_irqs(void){
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void gpio_init(void){
    enable_gpio_clocks();
    enable_gpio_exti_irqs();
}

void gpio_conf_output(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

void gpio_conf_input(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2)));
}

void gpio_conf_analog(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder |= (3U << (pin * 2));                                
}

void gpio_conf_periph(
    PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t periph) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (2U << (pin * 2));
    
    HAL_SFR_t *afr = (HAL_SFR_t *)(port + ((pin < 8) ? GPIO_OFS_AF0 : GPIO_OFS_AF1));
    uint32_t afr_position = (pin % 8) * 4;
    *afr = (*afr & ~(0xF << afr_position)) | ((periph & 0xF) << afr_position);
}

void gpio_conf_pullup(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

void gpio_conf_pulldown(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (2U << (pin * 2));
}

void gpio_conf_hiz(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2)));
}

void gpio_conf_outmode_pp(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) &= ~(1 << pin);
}

void gpio_conf_outmode_od(
    PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) |= (1 << pin);                            
}

void gpio_conf_speed(
    PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t speed)
{
    HAL_SFR_t *ospeedr = (HAL_SFR_t *)(port + GPIO_OFS_OSPEED);
    *ospeedr = (*ospeedr & ~(3U << (pin * 2))) | ((speed & 3U) << (pin * 2));
}

void gpio_conf_interrupt(
    PORTSELECTOR_t port, PINSELECTOR_t pin, HAL_BASE_t edge)
{
   // Configure EXTI Source Port Selection
   HAL_SFR_t *exticr = (HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_EXTICR1 + 4U * (pin >> 2));
   uint32_t bit_position = (pin & 0x03U) * 8U;
   *exticr = (*exticr & ~(0xFFU << bit_position)) | (gpio_get_port_index(port) << bit_position);

   // Configure Trigger Edge Selection
   if (edge & GPIO_INT_EDGE_RISING) { 
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_RTSR1) |= (1 << pin);                            
   } else {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_RTSR1) &= ~(1 << pin);                            
   }

   if (edge & GPIO_INT_EDGE_FALLING) {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_FTSR1) |= (1 << pin);                            
   } else {
        *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_FTSR1) &= ~(1 << pin);                            
   }
}

void gpio_conf_interrupt_handler(
    PORTSELECTOR_t port, PINSELECTOR_t pin, void (*handler)(uint8_t))
{
   exti_gpio_handlers[pin] = handler;
}


void gpio_interrupt_arm(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_RPR1) = (1 << pin);
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_FPR1) = (1 << pin);
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR1) |= (1 << pin);
}

void gpio_interrupt_disarm(
    PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR1) &= ~(1 << pin);
}

#endif