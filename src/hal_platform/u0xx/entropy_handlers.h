

#ifndef ENTROPY_HANDLERS_H
#define ENTROPY_HANDLERS_H

#include <hal/uc/entropy.h>
#include <ds/bytebuf.h>

#if uC_ENTROPY_ENABLED

static inline void entropy_isr(void) { 
    HAL_BASE_t random32;
    uint8_t have_lock;

    while (RNG->SR & RNG_SR_DRDY) {
        random32 = RNG->DR;
        have_lock = bytebuf_cPushReqLock(entropy_state.entropy, 4, 0x01);
        if (random32 && have_lock){
            bytebuf_cPushLen(entropy_state.entropy, (void *)(&random32), sizeof(HAL_BASE_t), 0x01);
        } else {
            #if uC_ENTROPY_MODE <= 2
            entropy_deinit();
            #endif
            break;
        }
    }   
}

#endif
#endif
