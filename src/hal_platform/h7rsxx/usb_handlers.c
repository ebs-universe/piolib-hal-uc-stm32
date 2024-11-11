

#include "usb_handlers.h"
#include <irq_handlers.h>


volatile uint8_t __usb_handler_inclusion;

#if uC_USB_ENABLED


static inline void _usb_device_handler (void) {
    tud_int_handler(uC_USB_DEVICE);
}

static inline void _usb_host_handler (void) {
    tuh_int_handler(uC_USB_HOST);
}

void OTG_HS_IRQHandler(void) {
    #if APP_ENABLE_USB 
        #if uC_USB_DEVICE == USB_HS
            _usb_device_handler();
        #elif uC_USB_HOST == USB_HS
            _usb_host_handler();
        #endif 
    #endif
}

void OTG_FS_IRQHandler(void) {
    #if APP_ENABLE_USB
        #if uC_USB_DEVICE == USB_FS
            _usb_device_handler();
        #elif uC_USB_HOST == USB_FS
            _usb_host_handler();
        #endif 
    #endif
}


void UCPD1_IRQHandler(void) {
    ;
}

#endif