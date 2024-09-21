

#include "crypto_impl.h"

#if uC_CRYPTO_ENABLED
#if uC_CRYPTO_DEFAULT_CTX

/**
 * @name Default Crypto Context
 * 
 * A default context is instantiated here only for convenience in small 
 * applications needing a single context with constant key and iv. If any 
 * of the following are true (most applications), application code should 
 * instatiate its own context(s): 
 *   - multiple contexts are needed
 *   - variable key and/or iv is needed 
 *   - secure key storage is to be used  
 * 
 */
/**@{*/   
const uint8_t crypto_key[uC_CRYPTO_DEFAULT_KEYSIZE] = APP_CRYPTO_KEY;
const uint8_t crypto_iv[uC_CRYPTO_DEFAULT_IVSIZE] = {
    0x41, 0x42, 0x43,               // DLMS_MANUFACTURER_CODE
    0x12, 0x34, 0x56, 0x78, 0x90,   // DLMS_SERIAL_NUMBER
    0x00, 0x00, 0x00, 0x00          // DLMS_INVOCATION_COUNTER
};

crypto_profile_t crypto_profile = {
    uC_CRYPTO_DEFAULT_ALG,
    uC_CRYPTO_DEFAULT_SWAP,
    uC_CRYPTO_DEFAULT_KEYSIZE,
    uC_CRYPTO_DEFAULT_IVSIZE,
    uC_CRYPTO_DEFAULT_INGESTSIZE,
    uC_CRYPTO_DEFAULT_BLOCKSIZE,
    uC_CRYPTO_DEFAULT_HEADERSIZE,
    uC_CRYPTO_DEFAULT_MACSIZE,
    (void *const)(&crypto_key),
    (void *const)(&crypto_iv)
};

crypto_ctx_t crypto_ctx = {
    uC_CRYPTO_DEFAULT_MODE,
    CRYPTO_CTX_PHASE_INIT,
    &crypto_profile,
    NULL,
    NULL
};
/**@}*/ 

crypto_state_t crypto_state = {
    CRYPTO_PREINIT,
    NULL, NULL
};

#else
crypto_state_t crypto_state = {0};
#endif

static inline void _set_chaining_mode(HAL_BASE_t mode);
static inline void _set_alg(CRYPTO_ALG_t alg);
static inline void _set_swapping(CRYPTO_SWAP_t swap);
static inline void _set_keysize(uint8_t keysize);
static inline void _load_key_and_iv(CRYPTO_ALG_t alg);
static inline void _set_mode(CRYPTO_CTX_MODE_t mode, CRYPTO_ALG_t alg, CRYPTO_CTX_PHASE_t phase);


void crypto_init(void){
     __HAL_RCC_AES_CLK_ENABLE();
    #if uC_CRYPTO_INTERRUPT
    HAL_NVIC_SetPriority(RNG_CRYP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RNG_CRYP_IRQn);
    #endif
    crypto_state.state = CRYPTO_IDLE;
}


void crypto_reset_ctx(crypto_ctx_t * ctx){
    ctx->mode = CRYPTO_CTX_MODE_IDLE;
    ctx->phase = CRYPTO_CTX_PHASE_INIT;
    ctx->ingest_cb = NULL;
    if (ctx->reset){
        ctx->reset(ctx);
    }
    if (crypto_state.ctx == ctx) {
        crypto_state.state = CRYPTO_IDLE;
        AES->CR &= ~AES_CR_EN;
    }
}

uint8_t crypto_set_ctx(crypto_ctx_t * ctx, CRYPTO_CTX_MODE_t mode, 
                       uint8_t (*ingest_cb)(crypto_ctx_t *const, uint8_t)){
    uint8_t wait;
    if (mode == CRYPTO_CTX_MODE_IDLE || mode == CRYPTO_CTX_MODE_SUSPENDED){
        die();
    }
    if (crypto_state.state == CRYPTO_PREINIT){
        crypto_init();
    }
    if (crypto_state.ctx != ctx || crypto_state.ctx->mode != mode){
        // There is a change in context. 
        if (crypto_state.state == CRYPTO_BUSY){
            // We're in an active transaction. Do nothing. 
            return 0;
        }
        if (crypto_state.ctx){
            // Note that the old context might be the same as the new 
            // context, except the mode changed.
            if (crypto_state.ctx != ctx) {
                if (crypto_state.ctx->mode > CRYPTO_CTX_MODE_IDLE){
                    crypto_state.ctx->mode = CRYPTO_CTX_MODE_SUSPENDED;
                    // TODO Deal with the old context. 
                    // Suspend.
                }
            }
        }
        crypto_state.ctx = ctx;
        crypto_state.ctx->mode = mode;
        crypto_state.ctx->ingest_cb = ingest_cb;
        wait = crypto_apply_profile(ctx->profile, mode, ctx->phase);
        if (!wait){
            crypto_state.state = CRYPTO_READY;
            return 2;
        } else {
            crypto_state.state = CRYPTO_BUSY;
            return 1;
        }
    } else {
        // Nothing to do.
        return 3;
    }
}

static inline void _set_chaining_mode(HAL_BASE_t mode){
    AES->CR = (AES->CR & ~(AES_CR_CHMOD)) | mode;
}

static inline void _set_alg(CRYPTO_ALG_t alg){
    switch (alg) {
        case CRYPTO_AES_CBC:
            _set_chaining_mode(CRYP_AES_CBC);
            break;
        case CRYPTO_AES_ECB:
            _set_chaining_mode(CRYP_AES_ECB);
            break;
        case CRYPTO_AES_CTR:
            _set_chaining_mode(CRYP_AES_CTR);
            break;
        case CRYPTO_AES_GCM:
            _set_chaining_mode(CRYP_AES_GCM_GMAC);
            break;
        case CRYPTO_AES_GMAC:
            _set_chaining_mode(CRYP_AES_GCM_GMAC);
            break;
        case CRYPTO_AES_CCM:
            _set_chaining_mode(CRYP_AES_CCM);
            break;
        default:
            die();
    }
}

static inline void _set_swapping(CRYPTO_SWAP_t swap){
    switch (swap){
        case CRYPTO_SWAP_NONE:
            AES->CR = (AES->CR & ~(AES_CR_DATATYPE));
            break;
        case CRYPTO_SWAP_8B:
            AES->CR = (AES->CR & ~(AES_CR_DATATYPE)) | AES_CR_DATATYPE_1;
            break;
        case CRYPTO_SWAP_16B:
            AES->CR = (AES->CR & ~(AES_CR_DATATYPE)) | AES_CR_DATATYPE_0;
            break;
        case CRYPTO_SWAP_1B:
            AES->CR = (AES->CR & ~(AES_CR_DATATYPE)) | (AES_CR_DATATYPE_0 | AES_CR_DATATYPE_1);
            break;
        default:
            die();
    }
}

static inline void _set_keysize(uint8_t keysize){
    switch (keysize){
        case 16:
            AES->CR &= ~AES_CR_KEYSIZE;
            break;
        case 32:
            AES->CR |= AES_CR_KEYSIZE;
            break;
        default:
            die();
    }
}

uint8_t crypto_set_ctx_phase(crypto_ctx_t * ctx, CRYPTO_CTX_PHASE_t phase){
    CRYPTO_CTX_PHASE_t result;
    ctx->phase = phase;
    if (crypto_state.ctx == ctx){
        result = _set_phase(phase);
        if (result != phase){
            return 1;
        }
    }
    return 0;
}

/*
 * This should also execute or trigger any special phase-dependent 
 * actions for the profile, such as key derivation. If the action 
 * needed takes time, this function should trigger it, set a 
 * phase other than the commanded phase, and do something in the 
 * interrupt handler to get the actual phase to be set eventually.
 * When that happens, this function should return the actual phase 
 * set. Applications should then poll the actual phase until it 
 * reaches the necessary phase.  
 * 
 * This function can expect that the crypto core's activated context
 * is valid, other than the phase. 
 */ 
CRYPTO_CTX_PHASE_t _set_phase(CRYPTO_CTX_PHASE_t phase){
    switch (crypto_state.ctx->profile->alg){
        case CRYPTO_AES_CBC:
        case CRYPTO_AES_ECB:
            if (phase == CRYPTO_CTX_PHASE_INIT){
                // TODO actually fire off the derivation here?
                crypto_state.ctx->phase = CRYPTO_CTX_PHASE_DERIVATION;
            }
            break;
        case CRYPTO_AES_CTR:
            break;
        case CRYPTO_AES_GCM:
        case CRYPTO_AES_GMAC:
        case CRYPTO_AES_CCM:
            switch (phase){
                case CRYPTO_CTX_PHASE_INIT:
                    AES->CR = (AES->CR & ~(AES_CR_GCMPH));
                    crypto_state.ctx->phase = CRYPTO_CTX_PHASE_PREPKEY;
                    break;
                case CRYPTO_CTX_PHASE_HEADER:
                    AES->CR = (AES->CR & ~(AES_CR_GCMPH)) | (AES_CR_GCMPH_0);
                    break;
                case CRYPTO_CTX_PHASE_PAYLOAD:
                    AES->CR = (AES->CR & ~(AES_CR_GCMPH)) | (AES_CR_GCMPH_1);
                    break;
                case CRYPTO_CTX_PHASE_FINAL:
                    AES->CR |= (AES_CR_GCMPH_1 | AES_CR_GCMPH_1);
                    break;
                default:
                    die();
            }
            break;
        default:
            die(); 
    }
    return crypto_state.ctx->phase;
}

static void _set_mode(
        CRYPTO_CTX_MODE_t mode, 
        CRYPTO_ALG_t alg, 
        CRYPTO_CTX_PHASE_t phase)
{
    uint32_t mode_bits= 0UL;
    
    switch (mode) {
        case CRYPTO_CTX_MODE_ENCRYPTION:
            mode_bits = 0x0UL;
            break;
        case CRYPTO_CTX_MODE_DECRYPTION:
            switch (alg) {
                case CRYPTO_AES_CBC:
                case CRYPTO_AES_ECB:
                    if (phase == CRYPTO_CTX_PHASE_DERIVATION){
                        mode_bits = AES_CR_MODE_1 | AES_CR_MODE_0;
                        break;}
                case CRYPTO_AES_CTR:
                case CRYPTO_AES_GCM:
                case CRYPTO_AES_GMAC:
                case CRYPTO_AES_CCM:
                    mode_bits = AES_CR_MODE_1;
                    break;
                default:
                    die();
            }
            break;
        default:
            die();    
    }
    AES->CR = (AES->CR & ~(AES_CR_MODE)) | mode_bits;
}

static inline void _load_key_and_iv(CRYPTO_ALG_t alg){
    uint32_t *key_ptr = (uint32_t *)(crypto_state.ctx->profile->key);
    
    AES->KEYR0 = key_ptr[0];
    AES->KEYR1 = key_ptr[1];
    AES->KEYR2 = key_ptr[2];
    AES->KEYR3 = key_ptr[3];

    if (crypto_state.ctx->profile->key_size == 32) { // 256-bit key (32 bytes)
        AES->KEYR4 = key_ptr[4];
        AES->KEYR5 = key_ptr[5];
        AES->KEYR6 = key_ptr[6];
        AES->KEYR7 = key_ptr[7];
    }

    uint32_t *iv_ptr = (uint32_t *)(crypto_state.ctx->profile->iv);
    // Hardcoded 96 bit IV.
    // uint8_t iv_words = crypto_state.ctx->profile->iv_size / 4;  
    AES->IVR0 = 0x0002U;
    AES->IVR1 = iv_ptr[0];  
    AES->IVR2 = iv_ptr[1];
    AES->IVR3 = iv_ptr[2];  
}

uint8_t crypto_apply_profile(
        const crypto_profile_t *const profile, 
        CRYPTO_CTX_MODE_t mode, 
        CRYPTO_CTX_PHASE_t phase
){
    AES->CR &= ~AES_CR_EN; 
    // Clear any error flags
    AES->CR |= (0x1UL << 8) | (0x1UL << 7);
    _set_swapping(profile->swap);
    _set_keysize(profile->key_size);
    _set_alg(profile->alg);
    _load_key_and_iv(profile->alg);
    CRYPTO_CTX_PHASE_t res = _set_phase(phase);
    _set_mode(mode, profile->alg, res);
    
    AES->CR |= AES_CR_CCFIE;
    AES->CR |= AES_CR_EN;
    if (res != phase){
        return 1;
    } else {
        return 0;
    }
}


uint8_t crypto_ingest(void * data, uint8_t nvbytes, uint8_t override_busy){
    if (crypto_state.state != CRYPTO_READY){
        die();
    }
    uint32_t buffer[4] = {0};
    memcpy(buffer, data, nvbytes);
    for (uint8_t i = 0; i < 4; ++i) {
        AES->DINR = buffer[i]; 
    }
    return nvbytes;
}

void crypto_egest(void * data, uint8_t length){
    uint32_t buffer[4];
    for (uint8_t i = 0; i < 4; ++i) {
        buffer[i] = AES->DOUTR;
    }
    memcpy(data, buffer, length);
}


static void _crypto_tr_ingest(uint8_t len){
    crypto_transaction_t *const t = crypto_state.transaction;
    crypto_ingest(t->input, len, 0);
    t->input += len;
    t->input_len_done += len;            
}

static void _crypto_tr_egest(uint8_t len){
    crypto_transaction_t *const t = crypto_state.transaction;
    crypto_egest(t->output, len);
    t->output += len;
    t->output_len += len;            
}


uint8_t _crypto_tr_ingest_cb(crypto_ctx_t *const ctx, uint8_t ingest_len){
    uint8_t read_for_last = 0;
    crypto_state_t const* s = &crypto_state;
    crypto_profile_t const*const p = s->ctx->profile;
    crypto_transaction_t *const t = s->transaction;

    t->state = CRYPTO_TR_INPROGRESS;

    if (s->ctx->phase == CRYPTO_CTX_PHASE_HEADER){
        if (p->header_size > t->input_len_done){
            uint16_t header_remain = p->header_size - t->input_len_done;
            if (header_remain > 16) {
                _crypto_tr_ingest(16);
            } else {
                _crypto_tr_ingest(header_remain);
            }
        } else if (p->header_size == t->input_len_done) {
            // header is now done, switch to payload
            crypto_set_ctx_phase(s->ctx, CRYPTO_CTX_PHASE_PAYLOAD);
            // dout is also not valid right now
            read_for_last = 1;
        }
    }

    if (s->ctx->phase == CRYPTO_CTX_PHASE_PAYLOAD){
        if (!read_for_last && t->input_len_done > 0){
            _crypto_tr_egest(ingest_len);
        } 

        uint16_t payload_remain = t->input_len - t->input_len_done;
        if (payload_remain > 16) {
            _crypto_tr_ingest(16);
        } else if (payload_remain) {
            _crypto_tr_ingest(payload_remain);
        } else {
            crypto_set_ctx_phase(s->ctx, CRYPTO_CTX_PHASE_FINAL);
            read_for_last = 1;
        }
    }

    if (s->ctx->phase == CRYPTO_CTX_PHASE_FINAL){
        if (read_for_last){
            // The ingest length is incorrect here for the last block
            AES->DINR = 0x0UL;
            AES->DINR = p->header_size * 8;
            AES->DINR = 0x0UL;
            AES->DINR = (t->input_len - p->header_size) * 8;
        } else {
            //Whatever is read here is the mac.
            _crypto_tr_egest(16);
            crypto_reset_ctx(s->ctx);
            t->state = CRYPTO_TR_DONE;
        }
    }

    return 0;
}


uint8_t crypto_trigger_transaction(crypto_transaction_t *const transaction){
    // TODO make sure there isn't already a transaction.
    crypto_state.transaction = transaction;
    return crypto_set_ctx(transaction->ctx, transaction->mode, &_crypto_tr_ingest_cb);
}



#endif