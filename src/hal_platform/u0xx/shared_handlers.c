

#include <application.h>
#include <irq_handlers.h>


volatile uint8_t __shared_handler_inclusion;


void HASH_RNG_IRQHandler(void){
    #if uC_ENTROPY_ENABLED == 1
        if (RNG->SR & RNG_SR_DRDY) {
            entropy_isr();
        }
        if (RNG->SR & (RNG_SR_SEIS | RNG_SR_CEIS)) {
            RNG->SR = RNG_SR_SEIS | RNG_SR_CEIS;
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
