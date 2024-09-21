

#ifndef USBCDC_IMPL_H
#define USBCDC_IMPL_H

#include <hal/uc/usbcdc.h>

#if uC_USB_ENABLED && uC_USBCDC_ENABLED 
#include <tusb.h>


static inline void usbcdc_init(uint8_t intfnum){
    // TinyUSB seems to initialize everything on USB init, 
    // so we do nothing further here
    ; 
}


static inline void usbcdc_send_trigger(uint8_t intfnum){
    // Send trigger is not actually needed here. Packets 
    // will be sent when they are full. If partial packets
    // are to be transmitted, use send flush instead.
    ; 
}


static inline void usbcdc_send_flush(uint8_t intfnum){
    tud_cdc_n_write_flush(intfnum);
}


static inline HAL_BASE_t usbcdc_reqlock(uint8_t intfnum, HAL_BASE_t len, uint8_t token){
    // TODO Implement locking mechanism?
    HAL_BASE_t avail = tud_cdc_n_write_available(intfnum);
    if (len){
        if (avail >= len){
            return 0x01;
        }
        else{
            return 0x00;
        }
    }
    else {
        return avail;
    }
}


static inline HAL_BASE_t usbcdc_rellock(uint8_t intfnum, uint8_t token){
    return 1;
}

static inline HAL_BASE_t usbcdc_putc(uint8_t intfnum, uint8_t byte, uint8_t token, HAL_BASE_t handlelock){
    return tud_cdc_n_write_char(intfnum, byte);
}

static inline HAL_BASE_t usbcdc_write(uint8_t intfnum, void * buffer, HAL_BASE_t len, uint8_t token){
    return tud_cdc_n_write(intfnum, buffer, len);
}

static inline HAL_BASE_t usbcdc_read(uint8_t intfnum, void * buffer, HAL_BASE_t len){
    return tud_cdc_n_read(intfnum, (void *)buffer, len);
}

static inline uint8_t usbcdc_getc(uint8_t intfnum){
    return (uint8_t)(tud_cdc_n_read_char(intfnum));
}

static inline HAL_BASE_t usbcdc_population_rxb(uint8_t intfNum){
    return tud_cdc_n_available(intfNum);
}

static inline void usbcdc_discard_rxb(uint8_t intfNum){
    tud_cdc_n_read_flush(intfNum);
}

#endif
#endif  