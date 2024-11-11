


#ifndef EEPROM_IMPL_H
#define EEPROM_IMPL_H

#include <hal/uc/eeprom.h>
#include <string.h>

#if uC_EEPROM_ENABLED

#define EEPROM_START_ADDR   BKPSRAM_BASE
#ifdef uC_EEPROM_SIZE
    #define EEPROM_SIZE         uC_EEPROM_SIZE
#else
    #define EEPROM_SIZE         (4 * 1024)
#endif

static inline uint8_t eeprom_init(void){
    __HAL_RCC_BKPRAM_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    return HAL_OK;
}


static inline uint8_t eeprom_ready(void){
    return 1;
}


static inline uint8_t eeprom_write_byte(uC_EEPROM_SIZE_t address, uint8_t data){
    if (address >= EEPROM_SIZE) { return 0; }
    *(uint8_t *)(EEPROM_START_ADDR + address) = data;
    return 1; 
}


static inline uint8_t eeprom_read_byte(uC_EEPROM_SIZE_t address){
    if (address >= EEPROM_SIZE) { return 0xFF; }
    return *(uint8_t *)(EEPROM_START_ADDR + address);
}


static inline uC_EEPROM_SIZE_t eeprom_write(uC_EEPROM_SIZE_t address, const void * data, uC_EEPROM_SIZE_t length){
    if ((address + length) > EEPROM_SIZE) { return 0; }
    memcpy((void *)(EEPROM_START_ADDR + address), data, length);
    return length;
}


static inline uC_EEPROM_SIZE_t eeprom_read(uC_EEPROM_SIZE_t address, void * data, uC_EEPROM_SIZE_t length){
    if ((address + length) > EEPROM_SIZE) { return 0; }
    memcpy(data, (void *)(EEPROM_START_ADDR + address), length);
    return length;
}

#endif

#endif