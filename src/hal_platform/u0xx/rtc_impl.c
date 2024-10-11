/* 
    Copyright (c)
    (c) 2024 Chintalagiri Shashank
   
    This file is part of
    Embedded bootstraps : Peripheral driver implementations : STM32
   
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


#include "rtc_impl.h"

#if uC_RTC_ENABLED 

#include "stm32u0xx_hal.h"
#include <hal/uc/gpio.h>

rtc_state_t rtc_state = {0};

static void _rtc_state_reset(void){
    rtc_state.state = RTC_ST_IDLE;
    rtc_state.iobuffer = NULL;
    rtc_state.cb = NULL;
}

static uint8_t BCD_to_binary(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static uint8_t binary_to_BCD(uint8_t bin) {
    return ((bin / 10) << 4) | (bin % 10);
}

static inline void _rtc_unlock(void){
    if (!(RTC->WPR & 0xFF)){
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
    }
}

static inline void _rtc_lock(void){
    RTC->WPR = 0xFF; 
}

static inline void _rtc_wait_sync(void) {
    while (!(RTC->ICSR & RTC_ICSR_RSF)) {
        RTC->ICSR &= ~RTC_ICSR_RSF; 
    }
}

static inline void _rtc_1hz_out_gpio_init(void){
    gpio_conf_outmode_pp(RTC_1HZ_OUT);
    gpio_conf_speed(RTC_1HZ_OUT, GPIO_SPEED_FREQ_LOW);
    gpio_conf_periph(RTC_1HZ_OUT, GPIO_AF0_RTC);
}

static inline void _rtc_1hz_out_init(void){
    _rtc_1hz_out_gpio_init();
    _rtc_unlock();
    #if uC_RTC_USE_OUT2
        RTC->CR |= RTC_CR_OUT2EN_Msk | RTC_CR_COE_Msk | RTC_CR_COSEL_Msk;
    #else
        RTC->CR &= ~RTC_CR_OUT2EN_Msk;
        RTC->CR |= RTC_CR_COE_Msk | RTC_CR_COSEL_Msk;
    #endif
    _rtc_lock();
}

static inline void _rtc_tamp_init(void){
    ;
}

static inline void _rtc_calib_init(void){
    _rtc_unlock();
    #if uC_RTC_USE_LPCAL
        RTC->CALR |= (RTC_CALR_LPCAL_Msk);
    #else
        RTC->CALR &= ~(RTC_CALR_LPCAL_Msk);
    #endif
    _rtc_lock();
}

static inline void _rtc_read(){
    if (!rtc_state.iobuffer){
      return;
    }
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    rtc_state.iobuffer->century = 20;
    rtc_state.iobuffer->year =    BCD_to_binary((dr >> RTC_DR_YU_Pos) & 0xFF);
    rtc_state.iobuffer->month =   BCD_to_binary((dr >> RTC_DR_MU_Pos) & 0x1F);
    rtc_state.iobuffer->date =    BCD_to_binary((dr >> RTC_DR_DU_Pos) & 0x3F);
    rtc_state.iobuffer->hours =   BCD_to_binary((tr >> RTC_TR_HU_Pos) & 0x3F);
    rtc_state.iobuffer->minutes = BCD_to_binary((tr >> RTC_TR_MNU_Pos) & 0x7F);
    rtc_state.iobuffer->seconds = BCD_to_binary((tr >> RTC_TR_SU_Pos) & 0x7F);

    // SSR register can be used to handle some subsecond correction.
    // This is not implemented here though.
}

static inline void _rtc_write() {
    if (!rtc_state.iobuffer){
      return;
    }
    uint32_t year_bcd = binary_to_BCD(rtc_state.iobuffer->year);
    uint32_t month_bcd = binary_to_BCD(rtc_state.iobuffer->month);
    uint32_t date_bcd = binary_to_BCD(rtc_state.iobuffer->date);
    uint32_t hours_bcd = binary_to_BCD(rtc_state.iobuffer->hours);
    uint32_t minutes_bcd = binary_to_BCD(rtc_state.iobuffer->minutes);
    uint32_t seconds_bcd = binary_to_BCD(rtc_state.iobuffer->seconds);

    RTC->TR = (hours_bcd << RTC_TR_HU_Pos) |
              (minutes_bcd << RTC_TR_MNU_Pos) |
              (seconds_bcd << RTC_TR_SU_Pos);

    RTC->DR = (year_bcd << RTC_DR_YU_Pos) |
              (month_bcd << RTC_DR_MU_Pos) |
              (date_bcd << RTC_DR_DU_Pos);
}

static void _rtc_hw_init(){
    // TAMP and ALARM code can simply unlock and configure as needed. As 
    // long as it does not push into the init mode, the RTC should remain 
    // unaffected.
    _rtc_unlock();
    RTC->ICSR |= RTC_ICSR_INIT;
    while (!(RTC->ICSR & RTC_ICSR_INITF));

    RTC->PRER = 255 | (127 << RTC_PRER_PREDIV_A_Pos);

    #if uC_RTC_USE_MIXEDMODE
        RTC->ICSR = (RTC->ICSR & ~RTC_ICSR_BIN_Msk) | RTC_ICSR_BIN_1;
        // TODO BCDU needs to be calculated and set here
    #else
        RTC->ICSR = (RTC->ICSR & ~(RTC_ICSR_BIN_Msk | RTC_ICSR_BCDU_Msk));
    #endif
    RTC->CR &= ~RTC_CR_FMT_Msk;
    
    _rtc_write();

    RTC->ICSR &= ~RTC_ICSR_INIT;
    _rtc_lock();
}

void rtc_init(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    while(!(PWR->CR1 & PWR_CR1_DBP));
    // clk config probably isnt needed on every init
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();
    if (!(RTC->ICSR & RTC_ICSR_INITS)){
      // Year field is zero. Not Initialized.
      _rtc_hw_init();
    } else {
      _rtc_wait_sync();
      _rtc_read();
    }
    #if uC_RTC_1HZ_OUT_ENABLED
      _rtc_1hz_out_init();
    #endif
    _rtc_state_reset();
}

uint8_t rtc_read(tm_real_t * target, void(*cb)(void)){
    if (rtc_state.state != RTC_ST_IDLE) {
      return 2;
    }
    tm_clear_rtime(target);
    rtc_state.iobuffer = target;
    rtc_state.state = RTC_ST_WAIT_READ;
    _rtc_wait_sync();
    _rtc_read();
    if (cb) { cb(); }
    _rtc_state_reset();
    return 0;
}

uint8_t rtc_write(tm_real_t * source, void(*cb)(void)){
    if (rtc_state.state != RTC_ST_IDLE) {
      return 2;
    }
    rtc_state.iobuffer = source;
    rtc_state.state = RTC_ST_WAIT_WRITE;
    _rtc_hw_init();
    if (cb) { cb(); }
    _rtc_state_reset();
    return 0;
}

void rtc_sync(void(*cb)(void)){

}

#endif
