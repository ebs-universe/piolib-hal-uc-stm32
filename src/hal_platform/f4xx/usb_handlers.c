

#include "usb_handlers.h"
#include <irq_handlers.h>


volatile uint8_t __usb_handler_inclusion;


void OTG_FS_IRQHandler(void) {
    #if APP_ENABLE_USB
    #if APP_ENABLE_USB_DEVICE
        tud_int_handler(APP_USB_DEVICE_RHPORT);
    #endif
    #if APP_ENABLE_USB_HOST
        tuh_int_handler(APP_USB_HOST_RHPORT);
    #endif
    #endif
}
