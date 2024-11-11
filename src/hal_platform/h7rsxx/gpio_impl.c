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
    // Need to compare with actual port availability and 
    // usage, and work out which clocks actually need to be 
    // enabled and why.
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOM_CLK_ENABLE();
    __HAL_RCC_GPION_CLK_ENABLE();
    __HAL_RCC_GPIOO_CLK_ENABLE();
    __HAL_RCC_GPIOP_CLK_ENABLE();
}

static void enable_gpio_exti_irqs(void){
    // TODO Enable IRQs when interrupt is configured instead.
    // Consequences of simply enabling all IRQs unknown. 
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI5_IRQn);

    HAL_NVIC_SetPriority(EXTI6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI6_IRQn);

    HAL_NVIC_SetPriority(EXTI7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI7_IRQn);

    HAL_NVIC_SetPriority(EXTI8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI8_IRQn);

    HAL_NVIC_SetPriority(EXTI9_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_IRQn);

    HAL_NVIC_SetPriority(EXTI10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI10_IRQn);

    HAL_NVIC_SetPriority(EXTI11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI11_IRQn);

    HAL_NVIC_SetPriority(EXTI12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI12_IRQn);

    HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI13_IRQn);

    HAL_NVIC_SetPriority(EXTI14_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI14_IRQn);

    HAL_NVIC_SetPriority(EXTI15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_IRQn);
}

void gpio_init(void){
    enable_gpio_clocks();
    enable_gpio_exti_irqs();
    // This chip also requires HSLV configuration if operating at <= 2.5V
    // Since we generally just use 3.3V, we're not implementing this here.
}

void gpio_conf_output(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

void gpio_conf_input(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2)));
}

void gpio_conf_analog(PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder |= (3U << (pin * 2));                                
}

void gpio_conf_periph(PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t periph) 
{
    HAL_SFR_t *moder = (HAL_SFR_t *)(port + GPIO_OFS_MODE);
    *moder = (*moder & ~(3U << (pin * 2))) | (2U << (pin * 2));
    
    HAL_SFR_t *afr = (HAL_SFR_t *)(port + ((pin < 8) ? GPIO_OFS_AF0 : GPIO_OFS_AF1));
    uint32_t afr_position = (pin % 8) * 4;
    *afr = (*afr & ~(0xF << afr_position)) | ((periph & 0xF) << afr_position);
}

void gpio_conf_pullup(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (1U << (pin * 2));
}

void gpio_conf_pulldown(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2))) | (2U << (pin * 2));
}

void gpio_conf_hiz(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    HAL_SFR_t *pupdr = (HAL_SFR_t *)(port + GPIO_OFS_PUPD);
    *pupdr = (*pupdr & ~(3U << (pin * 2)));
}

void gpio_conf_outmode_pp(PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) &= ~(1 << pin);
}

void gpio_conf_outmode_od(PORTSELECTOR_t port, PINSELECTOR_t pin) 
{
    *(HAL_SFR_t *)(port + GPIO_OFS_OTYPE) |= (1 << pin);                            
}


void gpio_conf_speed(PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t speed)
{
    HAL_SFR_t *ospeedr = (HAL_SFR_t *)(port + GPIO_OFS_OSPEED);
    *ospeedr = (*ospeedr & ~(3U << (pin * 2))) | ((speed & 3U) << (pin * 2));
}

// EXTI elements here are ported relatively blindly from F4xx, with change 
// of register names alone. If there is a problem, check the implementation 
// against the datasheet. 

void gpio_conf_interrupt(PORTSELECTOR_t port, PINSELECTOR_t pin, HAL_BASE_t edge)
{
   // Configure EXTI Source Port Selection
   uint8_t word_index = pin >> 2U;
   HAL_SFR_t *exticr = (HAL_SFR_t *)(&(SBS->EXTICR[word_index]));
   uint32_t bit_position = (pin & 0x03U) * 4U;
   uint8_t port_index = gpio_get_port_index(port);
   uint32_t writing = (*exticr & ~(0x0FU << bit_position)) | (port_index << bit_position);
   *exticr = writing;

   uint32_t readback = *exticr;
   (void) readback;

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

void gpio_conf_interrupt_handler(PORTSELECTOR_t port, PINSELECTOR_t pin, void (*handler)(uint8_t))
{
   exti_gpio_handlers[pin] = handler;
}


void gpio_interrupt_arm(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_PR1) = (1 << pin);
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR1) |= (1 << pin);
}

void gpio_interrupt_disarm(PORTSELECTOR_t port, PINSELECTOR_t pin)
{
    *(HAL_SFR_t *)(EXTI_BASE + EXTI_OFS_IMR1) &= ~(1 << pin);
}


#endif