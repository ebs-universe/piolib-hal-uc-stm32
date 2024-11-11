

#include <application.h>
#include <irq_handlers.h>
#include <platform/debug.h>
#include <platform/types.h>


volatile uint8_t __shared_handler_inclusion;

/**
  * @brief This function is not an IRQ handler, but used by the 
  *        STM32 Cube HAL to report errors.
  */
void Error_Handler(void) {
    die();
}

void MissingISR_Handler(void) {
    uint32_t active_vector = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    if (active_vector > 15){
      active_vector = active_vector - 16;
    }
    (void) active_vector;
    // Now active_vector holds the number of the unhandled interrupt
    // For example, you could log it, or use it in debugging
    die();
}
