
#include "adc_handlers.h"
#include <irq_handlers.h>

volatile uint8_t __adc_handler_inclusion;

#if uC_ADC_ENABLED

// This interrupt handler is probably way too long. At the target
// sampling speed, there is significant ADC read overrun. 

#pragma GCC push_options
#pragma GCC optimize("O3")

// __attribute__((section(".itcm_irqs")))
static void _adc1_handler(void){
    uint32_t flags = *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR);
    uint16_t data;

    if (flags & ADC_FLAG_OVR){
        *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_ISR) |= ADC_ISR_OVR;
        adc1_state.overrun ++;
    }

    if (flags & ADC_FLAG_EOC){
        data = *(HAL_SFR_t *)(ADC1_BASE + OFS_ADCn_DR);
        adc1_state.lastresult = data;
        
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
}
#pragma GCC pop_options


// __attribute__((section(".itcm_irqs")))
void ADC1_2_IRQHandler(void)
{  
    #if uC_ADC1_ENABLED
        _adc1_handler();
    #endif
    #if uC_ADC2_ENABLED
        _adc2_handler();
    #endif
}

#endif
