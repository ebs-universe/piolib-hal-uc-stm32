/* 
   Copyright (c)
     (c) 2024 Chintalagiri Shashank
   
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : AVR
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/


#include "id_impl.h"
#include <string.h>

#if uC_ID_ENABLED 

DeviceID_t device_id = {0};


void id_init(void){
    #if uC_ID_TRIMFUNC == 1
        const uint8_t* uid_base = (uint8_t*) UID_BASE;
        uint64_t lot_number = 0;
        
        for (HAL_BASE_t i = 0; i < 7; i ++) {
            uint8_t current_value;
            uint8_t current_char = uid_base[5 + i];
            if (current_char >= 'A' && current_char <= 'Z') {
                current_value = current_char - 'A';
            } else if (current_char >= '0' && current_char <= '9'){
                current_value = current_char - '0' + 26;
            } else if (current_char == ' ') {
                current_value = 0;
            } else {
                current_value = 0;
                die();
            }
            // Base 36 encoding
            lot_number = lot_number * 36 + current_value;
        }

        for (HAL_BASE_t i = 0; i <= 4; i++){
            device_id.bytes[i] = lot_number & 0xFF;
            lot_number = lot_number >> 8;
        }

        device_id.bytes[5] = uid_base[4];

        int16_t x_coord = *((int16_t*)(&uid_base[0]));
        int16_t y_coord = *((int16_t*)(&uid_base[2]));

        device_id.bytes[6] = (int8_t)(x_coord);
        device_id.bytes[7] = (int8_t)(y_coord);
    #endif
}

uint8_t id_read(uint8_t len, void * buffer){
    if (len == uC_ID_LENGTH) {
        memcpy(buffer, &device_id.bytes, 8);
        return 8;
    } 
    
    if (len > uC_ID_MAXLEN) len = uC_ID_MAXLEN;
    memcpy(buffer, (void *)UID_BASE, len);
    return len; 
}

uint8_t id_write(uint8_t len, void * content){
    return 0;
}

#endif