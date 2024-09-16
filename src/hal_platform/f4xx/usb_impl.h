

#ifndef USB_IMPL_H
#define USB_IMPL_H

#include <hal/uc/usb.h>
#include <tusb.h>

#include <hal/uc/id.h>
#include <iomap.h>


#if uC_USB_ENABLED

extern uint8_t usb_status;
extern PCD_HandleTypeDef hpcd_FS;


// Get board unique ID for USB serial number. Return number of bytes. Note max_len is typically 16
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


static inline void usb_init(void){
    hpcd_FS.Instance = USB_OTG_FS;
    hpcd_FS.Init.dev_endpoints = CFG_TUD_ENDPOINTS;
    hpcd_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_FS.Init.dma_enable = DISABLE;
    hpcd_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_FS.Init.Sof_enable = DISABLE;
    hpcd_FS.Init.low_power_enable = DISABLE;
    hpcd_FS.Init.lpm_enable = DISABLE;
    hpcd_FS.Init.vbus_sensing_enable = DISABLE;
    hpcd_FS.Init.use_dedicated_ep1 = DISABLE;
    HAL_StatusTypeDef result = HAL_PCD_Init(&hpcd_FS); 
    if (result != HAL_OK){
        die();
    };   
}


static inline void usb_disable(void){
    HAL_PCD_DeInit(&hpcd_FS);
}


static inline void usb_connect(void){
    HAL_PCD_Start(&hpcd_FS);
}


static inline void usb_disconnect(void){
    HAL_PCD_Stop(&hpcd_FS);
}


static inline uint8_t usb_get_status(void){
    return usb_status;
}


static inline void usb_enable(void){
    ;
}

#endif
#endif
