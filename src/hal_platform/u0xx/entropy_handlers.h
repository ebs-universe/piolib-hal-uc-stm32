

#include <hal/uc/entropy.h>
#include <ds/bytebuf.h>

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

#endif