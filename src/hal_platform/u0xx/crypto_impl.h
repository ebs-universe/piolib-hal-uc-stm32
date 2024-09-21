

#ifndef AES_IMPL_H
#define AES_IMPL_H

#include <hal/uc/crypto.h>
#include <string.h>

/*
 * Algorithms supported by hardware:  
 *   - CRYPTO_AES_ECB
 *   - CRYPTO_AES_CBC
 *   - CRYPTO_AES_CTR
 *   - CRYPTO_AES_GCM
 *   - CRYPTO_AES_GMAC
 *   - CRYPTO_AES_CCM
 * 
 * 128 bit and 256 bit AES keys.
 */

#if uC_CRYPTO_ENABLED

#if uC_CRYPTO_DEFAULT_CTX
extern const uint8_t     crypto_key[uC_CRYPTO_DEFAULT_KEYSIZE];
extern const uint8_t     crypto_iv[uC_CRYPTO_DEFAULT_IVSIZE];
extern crypto_profile_t  crypto_profile;
extern crypto_ctx_t      crypto_ctx;
extern crypto_state_t    crypto_state;
#endif

CRYPTO_CTX_PHASE_t _set_phase(CRYPTO_CTX_PHASE_t phase);

#endif
#endif
