
#include "entropy_handlers.h"
#include <irq_handlers.h>

volatile uint8_t __entropy_handler_inclusion;

#if uC_ENTROPY_ENABLED

static inline void entropy_isr(void) { 
    HAL_BASE_t random32 = RNG->DR;
    uint8_t have_lock = bytebuf_cPushReqLock(entropy_state.entropy, 4, 0x01);
    if (have_lock){
        bytebuf_cPushLen(entropy_state.entropy, (void *)(&random32), sizeof(HAL_BASE_t), 0x01);
    } else {
        entropy_deinit();
    }
}


void RNG_IRQHandler(void){
    #if uC_ENTROPY_ENABLED == 1
        if (RNG->SR & RNG_SR_DRDY) {
            entropy_isr();
        }
        if (RNG->SR & (RNG_SR_SEIS | RNG_SR_CEIS)) {
            RNG->SR = RNG_SR_SEIS | RNG_SR_CEIS;
        }
    #endif
}

#endif