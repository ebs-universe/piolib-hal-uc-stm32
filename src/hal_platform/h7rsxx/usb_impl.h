

#ifndef USB_IMPL_H
#define USB_IMPL_H

#include <hal/uc/usb.h>
#include <tusb.h>

#include <hal/uc/id.h>
#include <iomap.h>


#if uC_USB_ENABLED

#define USB_FS            0
#define USB_HS            1
#define USB_DISABLE       -1

#ifdef APP_USB_DEVICE_RHPORT   
    #define uC_USB_DEVICE     APP_USB_DEVICE_RHPORT
#else
    #define uC_USB_DEVICE     USB_DISABLE
#endif

#ifdef APP_USB_HOST_RHPORT   
    #define uC_USB_HOST       APP_USB_HOST_RHPORT   
#else
    #define uC_USB_HOST       USB_DISABLE
#endif

#if uC_USB_DEVICE == USB_FS
    #define GPIO_USB_DM     GPIO_USBFS_DM
    #define GPIO_USB_DM_AF  USBFS_DM_AF
    #define GPIO_USB_DP     GPIO_USBFS_DP
    #define GPIO_USB_DP_AF  USBFS_DP_AF

    #define USB_IRQn        OTG_FS_IRQn
#elif uC_USB_DEVICE == USB_HS
    #define GPIO_USB_DM     GPIO_USBHS_DM
    #define GPIO_USB_DM_AF  USBHS_DM_AF
    #define GPIO_USB_DP     GPIO_USBHS_DP
    #define GPIO_USB_DP_AF  USBHS_DP_AF

    #define USB_IRQn        OTG_HS_IRQn
#endif


extern uint8_t usb_status;
extern PCD_HandleTypeDef usb_hpcd;


// Get board unique ID for USB serial number. Return number of bytes. 
// Note max_len is typically 16
static inline size_t board_get_unique_id(uint8_t id[], size_t max_len) {
    uint8_t len = id_read(8, (void *) (uintptr_t) id);
    return len;
}


// Get USB Serial number string from unique ID if available. Return number of character.
// Input is string descriptor from index 1 (index 0 is type + len)
static inline size_t board_usb_get_serial(uint16_t desc_str1[], size_t max_chars) {
    uint8_t uid[16] TU_ATTR_ALIGNED(4);
    size_t uid_len;

    uid_len = board_get_unique_id(uid, sizeof(uid));

    if ( uid_len > max_chars / 2 ) uid_len = max_chars / 2;

    for ( size_t i = 0; i < uid_len; i++ ) {
        for ( size_t j = 0; j < 2; j++ ) {
            const char nibble_to_hex[16] = {
                '0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
            };
            uint8_t const nibble = (uid[i] >> (j * 4)) & 0xf;
            desc_str1[i * 2 + (1 - j)] = nibble_to_hex[nibble]; // UTF-16-LE
        }
    }
    return 2 * uid_len;
}

void _usb_init(void);

static inline void usb_init(void){
    _usb_init();
}

static inline void usb_disable(void){
    HAL_PCD_DeInit(&usb_hpcd);
}


static inline void usb_connect(void){
    HAL_PCD_Start(&usb_hpcd);
}


static inline void usb_disconnect(void){
    HAL_PCD_Stop(&usb_hpcd);
}


static inline uint8_t usb_get_status(void){
    return usb_status;
}


static inline void usb_enable(void){
    ;
}

#endif
#endif
