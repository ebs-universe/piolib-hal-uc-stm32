

#include "core_handlers.h"
#include <irq_handlers.h>
#include <hal/uc/gpio.h>
#include <platform/sections.h>

volatile uint8_t __core_handler_inclusion;

#if uC_CORE_ENABLED

/**
 * @name Cortex-M4 Processor Interruption and Exception Handlers
 */
/**@{*/ 
/**
 * @brief This function handles Non maskable interrupt. 
 * 
 * Just drops to the failure state. Debug and inspect the stack. 
 */ 
void NMI_Handler(void){
    die();
}

/**
 * @brief This function handles Hard Fault interrupt. 
 * 
 * Just drops to the failure state. Debug and inspect the stack. 
 */ 
void HardFault_Handler(void){
    die();
}

/**
 * @brief This function handles Memory Management Faults. 
 * 
 * Just drops to the failure state. Debug and inspect the stack. 
 */ 
void MemManage_Handler(void){
    die();
}

/**
 * @brief This function handles Pre-fetch and Memory Access Faults. 
 * 
 * Just drops to the failure state. Debug and inspect the stack. 
 */ 
void BusFault_Handler(void){
    die();
}

/**
 * @brief This function handles Undefined Instruction or Illegal State Faults. 
 * 
 * Just drops to the failure state. Debug and inspect the stack. 
 */ 
void UsageFault_Handler(void){
    die();
}

/**
 * @brief This function handles System Service Call via SWI Instruction. 
 * 
 * A System Service architecture needs to be implemented?
 *  
 */
void SVC_Handler(void){
    ;
}

/**
 * @brief This function handles Pendable Request for System Service.
 * 
 */
void PendSV_Handler(void){
    ;
}

/**
 * @brief This function handles Debug Monitor. 
 * 
 */
void DebugMon_Handler(void){
    ;
}

#endif

#if uC_SYSTICK_TIMER_ENABLED

/**
  * @brief This function handles System Tick Timer Interrupts.
  */
FASTEXEC
void SysTick_Handler(void) {
    // HAL IncTick is only needed if we're using HAL timing primitives 
    //  (directly or indirectly). Ideally, most of those uses should
    //  be modified to use the considerably faster EBS systick.
    //  
    // Switching the STM32 HAL to use the EBS systick can probably be 
    //  done using the weak functions it provides.   
    //  
    // For now, keeping both running.  
    // HAL_IncTick();
    // gpio_set_output_high(GPIO_DBG_SCOPE1);
    #if APP_USE_CORE_SYSTICK
        #ifdef uC_SYSTICK_TIMER_IRQH
            uC_SYSTICK_TIMER_IRQH();
        #endif
    #endif
    // gpio_set_output_low(GPIO_DBG_SCOPE1);
}

#endif
