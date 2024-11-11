/* 
 Copyright 2024 Chintalagiri Shashank
 
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

#ifndef ID_IMPL_H
#define ID_IMPL_H

#include <hal/uc/id.h>

#if uC_ID_ENABLED

/**
 * STM32 ID is extracted from the 96-bit device ID. 
 * 
 *     4 bytes : [95:64] : Lot Number (ASCII)
 *     3 bytes : [63:40] : Lot Number (ASCII)
 *     1 byte  : [39:32] : Wafer Number 
 *     4 bytes : [31: 0] : X-Y on Wafer
 * 
 * In order to provide standard length IDs, we trim down the 12 byte 
 * device ID to 8 bytes by stripping away parts of the device ID with 
 * the least entropy.  
 * 
 *  - 7 bytes of Lot Number compressed into 40 bits (5 bytes) by 
 *    knowing that the ASCII sting contains only A-Z, 0-9, and it
 *    is treated simply as a base 36 encoded number.
 *  - X and Y might be BCD or signed binary, documentation seems to 
 *    be incorrect or conflicting. Assuming it is signed binary, we 
 *    take only the lower byte of X and Y it and apply the sign if 
 *    present, esentially converting int16_t to int8_t.
 * 
 * Applications critically requiring device ID uniqueness should use
 * the full 12 byte device ID.
 * 
 * An alternate implementation hashing the 96 bit ID into a 64 bit 
 * value may be discussed / considered in the future. 
 * 
 * This HAL implementation for ID provides the following controls: 
 *   - APP_ENABLE_ID    Non-zero (1) if this interface is enabled
 *   - APP_ID_TRIMFUNC  Method to use to trim the 12 byte ID to 8 bytes
 *      + 1             Structural truncation as described above
 *      + 2             Hash to a 8 byte value (not implemented)
 *  
 */

#define uC_ID_MAXLEN    12

#endif
#endif
