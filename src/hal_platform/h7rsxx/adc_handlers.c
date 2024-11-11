
#include "adc_handlers.h"
#include <irq_handlers.h>


volatile uint8_t __adc_handler_inclusion;

#if uC_ADC_ENABLED
// This interrupt handler is probably way too long. At the default
// sampling speed, there is significant ADC read overrun. This appears 
// to be resolved with -O1, but it is somewhat uncertain. 

// We appear to be generating one extra ADC interrupt on adc trigger 
// scan. This is screwing up the ADC data mapping.

static inline void _adc1_handler(void){
    const uint32_t hwif_base = adc1_if.hwif->base;
    adc_state_t *const state = adc1_if.state;
    uint32_t flags = *(HAL_SFR_t *)(hwif_base + OFS_ADCn_ISR);
    uint16_t data;
    if (flags & ADC_FLAG_OVR){
        state->overrun += 1;
    }
    if (flags & ADC_FLAG_EOC){
        data = *(HAL_SFR_t *)(hwif_base + OFS_ADCn_DR);
        state->lastresult = data;
        // dispatch result
        if (state->handler){
            state->handler(uC_ADC1_INTFNUM, state->nextchn, &state->lastresult);
        }
        // prepare for next
        uint32_t seqstate = state->seqstate;
        uint8_t nextchn = state->nextchn;
        seqstate >>= 1;
        nextchn ++;
        if (!seqstate){
            seqstate = state->chnmask;
            nextchn = 0;
        }
        while ((seqstate & 1) == 0){
            seqstate >>= 1;
            nextchn ++;
        }
        adc1_if.state->nextchn = nextchn;
        adc1_if.state->seqstate = seqstate;
    }
}

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
