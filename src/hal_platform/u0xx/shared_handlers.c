

#include <application.h>
#include <irq_handlers.h>
#include <platform/debug.h>

volatile uint8_t __shared_handler_inclusion;


void RNG_CRYP_IRQHandler(void){
    #if uC_ENTROPY_ENABLED == 1
        if (RNG->SR & RNG_SR_DRDY) {
            entropy_isr();
        }
        if (RNG->SR & RNG_SR_SEIS) {
            // Seed error. We leave auto-reset enabled. 
            // entropy_reset();
            RNG->SR &= ~RNG_SR_SEIS;
        } 
        if (RNG->SR & RNG_SR_CEIS) {
            // Clock error
            RNG->SR &= ~RNG_SR_CEIS;
        }
    #endif
    #if uC_CRYPTO_ENABLED == 1
        if (AES->SR & AES_SR_CCF) {
            crypto_isr();   
        }
        if (AES->SR & (AES_SR_RDERR | AES_SR_WRERR)) {
            // AES->ICR |= AES_ICR_ERRC; // see comment in isr impl
            AES->CR |= (0x1UL << 8); // Set AES_CR.ERRC per refman pp538 
        }
    #endif
}


/**
  * @brief This function is not an IRQ handler, but used by the 
  *        STM32 Cube HAL to report errors.
  */
void Error_Handler(void) {
    die();
}
