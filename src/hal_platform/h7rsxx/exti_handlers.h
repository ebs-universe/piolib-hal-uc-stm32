

// Handlers for this platform are generated by CubeMX and declared in 
// include/irq_handlers.h  

extern volatile uint8_t __exti_handler_inclusion;
#define MAX_EXTI_GPIO_LINES  16
extern void (* exti_gpio_handlers[MAX_EXTI_GPIO_LINES])(uint8_t);