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

#ifndef RTC_IMPL_H
#define RTC_IMPL_H

#include <hal/uc/rtc.h>

#if uC_RTC_ENABLED

#ifndef uC_RTC_USE_LPCAL    
#define uC_RTC_USE_LPCAL        1
#endif

#ifndef uC_RTC_USE_MIXEDMODE    
#define uC_RTC_USE_MIXEDMODE    0
#endif

#ifndef uC_RTC_USE_OUT2
#define uC_RTC_USE_OUT2         1
#endif

#ifndef uC_TAMP_ENABLED
#define uC_TAMP_ENABLED         1
#endif

static inline void rtc_poll(void){
  ;
}

#endif
#endif
