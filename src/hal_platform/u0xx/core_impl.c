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
 * Enable clocks needed for the core platform.
 */
static void enable_core_clocks(void);


static void cpu_init(void){
    #if (INSTRUCTION_CACHE_ENABLE != 0U)
        __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    #endif /* INSTRUCTION_CACHE_ENABLE */

    #if (PREFETCH_ENABLE != 0U)
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    #endif /* PREFETCH_ENABLE */
}

static void enable_core_clocks(void){
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}


void core_init(void) {
    cpu_init();
    enable_core_clocks();
}

#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000U) /* 5 s    */

HAL_StatusTypeDef hal_rcc_clockconfig(const RCC_ClkInitTypeDef  *const RCC_ClkInitStruct, uint32_t FLatency)
{
  HAL_StatusTypeDef halstatus;
  uint32_t tickstart;

  /* Check the parameters */
  assert_param(RCC_ClkInitStruct != NULL);
  assert_param(IS_RCC_CLOCKTYPE((uint8_t)RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
    must be correctly programmed according to the frequency of the CPU clock
    (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if (FLatency > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    uint32_t timeout = 100;
    
    while (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      timeout --;
    }

    if (!timeout) {
        die();
    }
  }

  /*------------------------- SYSCLK Configuration ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));

    /* PLL is selected as System Clock Source */
    if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* Check the PLL ready flag */
      if (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0U)
      {
        die();
      }
    }
    else
    {
      if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        /* Check the HSE ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0U)
        {
          die();
        }
      }
      /* MSI is selected as System Clock Source */
      else if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_MSI)
      {
        /* Check the MSI ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_MSIRDY) == 0U)
        {
          die();
        }
      }
      /* HSI is selected as System Clock Source */
      else if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSI)
      {
        /* Check the HSI ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
        {
          die();
        }
      }

      /* LSI is selected as System Clock Source */
      else if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_LSI)
      {
        /* Check the LSI ready flag */
        if (READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) == 0U)
        {
          die();
        }
      }

      /* LSE is selected as System Clock Source */
      else
      {
        /* Check the LSE ready flag */
        if (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
        {
          die();
        }
      }
    }

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_ClkInitStruct->SYSCLKSource);

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
    {
      if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
      {
        die();
      }
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    /* Set the new HCLK clock divider */
    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
  }

  /* Decreasing the number of wait states because of lower CPU frequency */
  if (FLatency < __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      die();
    }
  }

  /*-------------------------- PCLK Configuration ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_ClkInitStruct->APB1CLKDivider);
  }

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) \
                                                                >> RCC_CFGR_HPRE_Pos] & 0x1FU);

  /* Configure the source of time base considering new system clocks settings*/
  halstatus = HAL_InitTick(TICK_INT_PRIORITY);

  return halstatus;
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