


#ifndef EEPROM_IMPL_H
#define EEPROM_IMPL_H

#include <hal/uc/eeprom.h>
#include <string.h>

#if uC_EEPROM_ENABLED

#define EEPROM_SIZE         0

static inline uint8_t eeprom_init(void){
    ;
}


static inline uint8_t eeprom_ready(void){
    return 0;
}


static inline uint8_t eeprom_write_byte(uC_EEPROM_SIZE_t address, uint8_t data){
    return 0; 
}


static inline uint8_t eeprom_read_byte(uC_EEPROM_SIZE_t address){
    return 0xff;
}


static inline uC_EEPROM_SIZE_t eeprom_write(uC_EEPROM_SIZE_t address, const void * data, uC_EEPROM_SIZE_t length){
    return 0x00;
}


static inline uC_EEPROM_SIZE_t eeprom_read(uC_EEPROM_SIZE_t address, void * data, uC_EEPROM_SIZE_t length){
    return 0x00;
}

#endif

#endif