

#ifndef CRYPTO_HANDLERS_H
#define CRYPTO_HANDLERS_H

#include <hal/uc/crypto.h>


#if uC_CRYPTO_ENABLED

static inline void crypto_isr(void) { 
    // Datasheet talks about an ICR register, but it does not 
    // show up in the register maps. The MCU headers include 
    // the register, but does not have all the bits. The bit 
    // named is shown in the CR register drawing.
    // Trusting the tables in the datasheet instead of the text. 
    //AES->ICR |= AES_ICR_CCF;
    AES->CR |= 0x1UL << 7; // Set AES_CR.CCF per refman pp538 

    uint8_t ingest_len = 16;
    uint8_t no_release = 0;

    switch (crypto_state.ctx->phase)
    {
    case CRYPTO_CTX_PHASE_PREPKEY:
        if (crypto_state.ctx->profile->header_size){
            crypto_set_ctx_phase(crypto_state.ctx, CRYPTO_CTX_PHASE_HEADER);
        } else {
            crypto_set_ctx_phase(crypto_state.ctx, CRYPTO_CTX_PHASE_PAYLOAD);
        }
        ingest_len = 0;
        AES->CR |= AES_CR_EN;
        break;
    default:
        break;
    }

    crypto_state.state = CRYPTO_READY;
    if (crypto_state.ctx->ingest_cb){
        no_release = crypto_state.ctx->ingest_cb(crypto_state.ctx, ingest_len);
    }
    if (!no_release){
        crypto_state.state = CRYPTO_BUSY;
    }
}

#endif
#endif
