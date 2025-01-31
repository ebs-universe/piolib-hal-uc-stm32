
#include "adc_handlers.h"
#include <irq_handlers.h>
#include <hal/uc/gpio.h>
#include <platform/sections.h>

volatile uint8_t __adc_handler_inclusion;

#if uC_ADC_ENABLED

// This interrupt handler is probably way too long. At the target
// sampling speed, there is significant ADC read overrun. 

#pragma GCC push_options 
#pragma GCC optimize("O3")

/** 
 * This is still pretty strange. 
 * When set to prescaler _2 with FASTEXEC, the gpio_set functions seem to have 
 * no effect. This also seems to happen with the systick handler.
 * It is unclear if everything is actually working as intended.
 */ 

#if uC_ADC_SUPPORT_DIRECT

FASTEXEC
__attribute__((target("thumb")))
static inline void _adc1_handler(void){
    #ifdef uC_ADC_TIMING_MONITOR_GPIO
    gpio_set_output_high(uC_ADC_TIMING_MONITOR_GPIO);
    #endif

    uint32_t flags = *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR);

    if (flags & ADC_FLAG_EOC){
        adc1_state.lastresult = *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_DR);
        
        // Dispatch result if handler is set
        if (adc1_state.handler_eoc){
            adc1_state.handler_eoc(adc1_state.nextchn, &adc1_state.lastresult);
        }

        // Prepare for next
        uint32_t seqstate = adc1_state.seqstate;
        seqstate >>= 1;
        if (seqstate){
            uint8_t offset = __builtin_ctz(seqstate); 
            seqstate >>= offset;
            adc1_state.nextchn += (1 + offset);
            adc1_state.seqstate = seqstate;
        }
    }

    if (flags & ADC_FLAG_EOS){
        // Clear EOS flag
        *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR) |= ADC_ISR_EOS;
        if (adc1_state.mode != ADC_MODE_CONTINUOUS) {
            adc1_state.mode = ADC_MODE_IDLE;
        }
    
        if (adc1_state.handler_eos){
            adc1_state.handler_eos();
        }
    
        // Reset sequence state
        adc1_state.nextchn = adc1_state.chnmask;
        adc1_state.seqstate = adc1_state.firstchn;
    }

    if (flags & ADC_FLAG_OVR){
        *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR) |= ADC_ISR_OVR;
        adc1_state.overrun ++;
    }

    #ifdef uC_ADC_TIMING_MONITOR_GPIO
    gpio_set_output_low(uC_ADC_TIMING_MONITOR_GPIO);
    #endif
}

#endif

FASTEXEC
__attribute__((target("thumb")))
static inline void _adc1_timing_monitor(void){
    uint32_t flags = *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR);
    #if uC_ADC1_DM_MODE == ADC_DM_DMA
    if (flags & ADC_FLAG_EOS){
        *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR) |= ADC_ISR_EOS;
        #ifdef uC_ADC_TIMING_MONITOR_GPIO
        gpio_set_output_toggle(uC_ADC_TIMING_MONITOR_GPIO);
        #endif
    }
    #else
    if (flags & ADC_FLAG_EOC){
        *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR) |= ADC_ISR_EOC;
        #ifdef uC_ADC_TIMING_MONITOR_GPIO
        gpio_set_output_toggle(uC_ADC_TIMING_MONITOR_GPIO);
        #endif
    }
    #endif
}

#pragma GCC pop_options


FASTEXEC    
void ADC1_2_IRQHandler(void)
{   
    #if uC_ADC1_ENABLED
    #if uC_ADC1_DM_MODE == ADC_DM_INTERRUPT
        _adc1_handler();
    #else
        #ifdef uC_ADC_TIMING_MONITOR_GPIO
            _adc1_timing_monitor();
        #endif
    #endif
    #endif
    #if uC_ADC2_ENABLED
    #if uC_ADC2_DM_MODE == ADC_DM_INTERRUPT             
        _adc2_handler();
    #endif
    #endif
    }   
    
#endif
