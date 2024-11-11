

#include "usb_impl.h"
#include <hal/uc/gpio.h>
#include <tusb.h>


#if uC_USB_ENABLED

uint8_t usb_status = USB_ST_DISCONNECTED;
PCD_HandleTypeDef usb_hpcd;

// This does not actually support host + device, otg, multiple interface, etc. 
// Code will need to be refactored to allow this. 

static void usb_clock_init(void){
    #if (uC_USB_DEVICE == USB_FS) || (uC_USB_HOST == USB_FS)
    
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    
    #elif (uC_USB_DEVICE == USB_HS) || (uC_USB_HOST == USB_HS)
    
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_USBPHYC;
    PeriphClkInit.UsbPhycClockSelection = RCC_USBPHYCCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { die(); }
    __HAL_RCC_USBPHYC_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    
    #endif
}

static void usb_clock_deinit(void){
    #if (uC_USB_DEVICE == USB_FS) || (uC_USB_HOST == USB_FS)
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    #elif (uC_USB_DEVICE == USB_HS) || (uC_USB_HOST == USB_HS)
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    __HAL_RCC_USBPHYC_CLK_DISABLE();
    #endif
}

static void usb_gpio_init(void){
    // Appears to be not needed?
    // gpio_conf_periph(GPIO_USB_DM, GPIO_USB_DM_AF);
    // gpio_conf_periph(GPIO_USB_DP, GPIO_USB_DP_AF);
    // gpio_conf_speed(GPIO_USB_DM, GPIO_SPEED_FREQ_VERY_HIGH);
    // gpio_conf_speed(GPIO_USB_DP, GPIO_SPEED_FREQ_VERY_HIGH);
}

static void usb_gpio_deinit(void){
    gpio_conf_input(GPIO_USB_DM);
    gpio_conf_input(GPIO_USB_DP);
    gpio_conf_speed(GPIO_USB_DM, GPIO_SPEED_FREQ_LOW);
    gpio_conf_speed(GPIO_USB_DP, GPIO_SPEED_FREQ_LOW);
}

static void usb_nvic_init(void){
    HAL_NVIC_SetPriority(USB_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
}

static void usb_nvic_deinit(void){
    HAL_NVIC_DisableIRQ(USB_IRQn);
}

void _usb_init(void){
    if(HAL_PWREx_EnableUSBVoltageDetector() != HAL_OK) { die(); }

    #if uC_USB_DEVICE == USB_FS
        usb_hpcd.Instance = USB_OTG_FS;
        usb_hpcd.Init.speed = PCD_SPEED_FULL;
    #elif uC_USB_DEVICE == USB_HS
        usb_hpcd.Instance = USB_OTG_HS;
        usb_hpcd.Init.speed = PCD_SPEED_HIGH;
    #endif

    usb_hpcd.Init.dev_endpoints = CFG_TUD_ENDPOINTS;
    usb_hpcd.Init.dma_enable = DISABLE;
    usb_hpcd.Init.phy_itface = USB_OTG_HS_EMBEDDED_PHY;
    usb_hpcd.Init.Sof_enable = DISABLE;
    usb_hpcd.Init.low_power_enable = DISABLE;
    usb_hpcd.Init.lpm_enable = ENABLE;
    usb_hpcd.Init.vbus_sensing_enable = DISABLE;
    usb_hpcd.Init.use_dedicated_ep1 = DISABLE;
    usb_hpcd.Init.vbus_sensing_enable = DISABLE;
    HAL_StatusTypeDef result = HAL_PCD_Init(&usb_hpcd); 
    if (result != HAL_OK){
        die();
    };   
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