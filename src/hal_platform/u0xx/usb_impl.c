

#include "usb_impl.h"
#include <hal/uc/gpio.h>

#if uC_USB_ENABLED
#include <tusb.h>


uint8_t usb_status = USB_ST_DISCONNECTED;
PCD_HandleTypeDef hpcd_FS;


static void usb_clock_init(void){
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
}

static void usb_clock_deinit(void){
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
}

static void usb_gpio_init(void){
    gpio_conf_periph(USBFS_DM, USBFS_DM_AF);
    gpio_conf_periph(USBFS_DP, USBFS_DP_AF);
    gpio_conf_speed(USBFS_DM, GPIO_SPEED_FREQ_VERY_HIGH);
    gpio_conf_speed(USBFS_DP, GPIO_SPEED_FREQ_VERY_HIGH);
}

static void usb_gpio_deinit(void){
    gpio_conf_input(USBFS_DM);
    gpio_conf_input(USBFS_DP);
    gpio_conf_speed(USBFS_DM, GPIO_SPEED_FREQ_LOW);
    gpio_conf_speed(USBFS_DP, GPIO_SPEED_FREQ_LOW);
}

static void usb_nvic_init(void){
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

static void usb_nvic_deinit(void){
    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd){
    usb_clock_init();
    usb_gpio_init();
    usb_nvic_init();
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd){
    usb_nvic_deinit();
    usb_gpio_deinit();
    usb_clock_deinit();
}

#endif