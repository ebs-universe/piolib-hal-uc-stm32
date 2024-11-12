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

#ifndef ADC_IMPL_H
#define ADC_IMPL_H

#include <hal/uc/adc.h>

#if uC_ADC_ENABLED

/*
 * This implementation is designed for multi-channel ADC sampling with 
 * relatively uniform conversion parameters. Specifically, all enabled 
 * channels are added into a single conversion group per ADC. Conversion 
 * can be triggered by some as yet unspecified sources, including by 
 * software. Each trigger might launch a single scan or a continuous 
 * scan based on the configured adc mode. Single channel conversion is
 * not presently supported.
 * 
 * Each conversion result is stored temporarily in the adc peripheral's 
 * state variable, and a handler function too be provided by the 
 * application is called with the ADC interface number, channel number, 
 * and a pointer to the stored result. The application must provide this
 * function to correctly dispatch the value to the actual consumer. 
 * 
 * This interface does not support at all the following features:
 *  - Analog Watchdog 
 *  - Delayed Conversion Mode
 *  - Discontinuous Conversion Mode
 *  - Reduced Resolution
 *  - DMA integration
 *  - ADF integration
 *  - Bulb, Trigger, and Plus sampling modes
 *  - Offsets
 * 
 * Some early steps are taken to implement the following features, but 
 * applications should treat them as unsupported:
 *  - Injected Groups (some hooks included)
 *  - Oversampling
 *
 * Most other options are static and are hard-coded or otherwise specified 
 * at compile time only. Enhancement of the interface to allow these 
 * missing features, variable sampling rate and sampling time, simultaneous 
 * sampling, DMA, etc. may be considered if serious use cases present 
 * themselves. For the moment, applications are expected to implement 
 * their own ADC interfaces as needed for complex use cases.
 * 
 * This implementation also does not support arbitrary channel ordering. 
 * Specifically, we restrict the channel configuration to each channel 
 * appearing at most once in the the regular sequence register, and the 
 * conversion sequence to always be a monotonic increasing channel 
 * number.
 * 
 * Implementation Notes
 * --------------------
 * Most ADC config changes can only be done when the ADC is disabled. 
 * This implementation is not currently designed to handle on-the-fly 
 * changes, and the init functions are generally expected to be called 
 * exactly once during system power up. 
 * 
 * ADC Modes
 * ---------
 * 
 * ADC_MODE_DISABLED:
 *      ADC remains off and unconfigured. If the mode is set to disabled,
 *      this interface will be unavailable to the application, even at 
 *      runtime. However, the application can use a fully independent
 *      interface, provided by iteself, though which it which can configure 
 *      and use the ADC with full flexibility. 
 * 
 * ADC_MODE_SINGLE: 
 *      Not presently supported by this implementation. It might later be
 *      added by using instead a dynamic conversion group with a single 
 *      channel. We'll probably need to couple it with some kind of mutex 
 *      or so to make sure individual calls do not clobber each other.
 * 
 * ADC_MODE_SCAN:
 *      Scan though all enabled channels once, stop when done.
 * 
 * ADC_MODE_CONTINUOUS:
 *      Scan though all enabled channels once, repeat when done.
 *
 * This chip also supports dual ADC modes, which can be used for 
 * simultaneous sampling. While we do want to support this, we're 
 * deferring it for the moment.
 */

typedef enum {
    ADC12_HWIF,
} ADC_HWIF_TYPE;

typedef struct _ADC_HWIF_t{
    const ADC_HWIF_TYPE type;
    const HAL_ADDRESS_t base;
    const HAL_ADDRESS_t common;
    const uint32_t chnmask;
} _adc_hwif_t;

typedef struct _ADC_STATE_t{
    ADC_MODE_t mode;
    uint8_t overrun;
    uint8_t nextchn;
    uint8_t firstchn;
    uint16_t lastresult;
    uint32_t seqstate;
    uint32_t chnmask;
    void (*handler_eoc)(HAL_BASE_t, void *);
    void (*handler_eos)(void);
} adc_state_t;

typedef struct ADC_IF_t {
    const _adc_hwif_t * const hwif;
    adc_state_t * const state;
    void (*const init)(void);
} adc_if_t;

extern const adc_if_t adc1_if;
extern adc_state_t adc1_state;
extern const adc_if_t adc2_if;
extern adc_state_t adc2_state;

extern const adc_if_t *const adc_if[uC_ADCS_ENABLED + 1];

#define uC_ADC_DEFAULT_CLOCK_PRESCALER      ADC_CCR_PRESC_3
#define uC_ADC_DEFAULT_DATAALIGNLEFT        EBS_FALSE
#define uC_ADC_DEFAULT_EN_CALIB             EBS_TRUE
#define uC_ADC_DEFAULT_EN_INJECTQ           EBS_TRUE
#define uC_ADC_DEFAULT_EN_INJECTQ_AUTO      EBS_TRUE
#define uC_ADC_DEFAULT_OVERRUN_LATEST       EBS_TRUE
#define uC_ADC_DEFAULT_EN_OS_REG            EBS_FALSE
#define uC_ADC_DEFAULT_EN_OS_INJ            EBS_FALSE
#define uC_ADC_DEFAULT_OS_TRIG_EACH         EBS_FALSE
#define uC_ADC_DEFAULT_OS_RATIO             ADC_OVERSAMPLING_RATIO_4     
#define uC_ADC_DEFAULT_OS_SHIFT             1

/** 
 * Minimum sample time chosen for: 
 *   - 100ohm RAIN 
 *   - 12bit Resolution
 *   - Slow Channels
 * 
 * per Reference Manual, the minimum sample time is about 65ns, 
 * so SMP = 000 (2.5CLK) should be fine for a 25MHz ADC clock. 
 * Since we're not presently in tight timings, we make it SMP = 001, 
 * corresponding to a sampling time of 6.5CLK or 260ns @ 25MHz.
 * 
 * This implementation assumes the sample time is is identical 
 * for both ADCs. 
 * 
 */
#ifndef uC_ADC_SAMPLETIME
#define uC_ADC_SAMPLETIME       0b001
#endif

// per ADC registers
#define OFS_ADCn_ISR        0x00
#define OFS_ADCn_IER        0x04
#define OFS_ADCn_CR         0x08
#define OFS_ADCn_CFGR       0x0C
#define OFS_ADCn_CFGR2      0x10
#define OFS_ADCn_SMPR1      0x14
#define OFS_ADCn_SMPR2      0x18
#define OFS_ADCn_TR1        0x20
#define OFS_ADCn_TR2        0x24
#define OFS_ADCn_TR3        0x28
#define OFS_ADCn_SQR1       0x30
#define OFS_ADCn_SQR2       0x34
#define OFS_ADCn_SQR3       0x38
#define OFS_ADCn_SQR4       0x3C
#define OFS_ADCn_DR         0x40
#define OFS_ADCn_JSQR       0x4C
#define OFS_ADCn_OFR1       0x60
#define OFS_ADCn_OFR2       0x64
#define OFS_ADCn_OFR3       0x68
#define OFS_ADCn_OFR4       0x6C
#define OFS_ADCn_JDR1       0x80
#define OFS_ADCn_JDR2       0x84
#define OFS_ADCn_JDR3       0x88
#define OFS_ADCn_JDR4       0x8C
#define OFS_ADCn_AWD2CR     0xA0
#define OFS_ADCn_AWD3CR     0xA4
#define OFS_ADCn_DIFSEL     0xB0
#define OFS_ADCn_CALFACT    0xB4
#define OFS_ADCn_OR         0xC8

// common ADC registers
#define OFS_ADC_CSR         0x00
#define OFS_ADC_CCR         0x08
#define OFS_ADC_CDR         0x0C
#define OFS_ADC_HWCFGR0     0xF0
#define OFS_ADC_VERR        0xF4
#define OFS_ADC_IPDR        0xF8
#define OFS_ADC_SIDR        0xFC

#ifndef uC_ADC_CLOCK_PRESCALER
    #define uC_ADC_CLOCK_PRESCALER      uC_ADC_DEFAULT_CLOCK_PRESCALER
#endif

#if uC_ADC1_ENABLED
    #ifndef uC_ADC1_DATAALIGNLEFT
        #define uC_ADC1_DATAALIGNLEFT   uC_ADC_DEFAULT_DATAALIGNLEFT
    #endif
    #ifndef uC_ADC1_EN_CALIB
        #define uC_ADC1_EN_CALIB        uC_ADC_DEFAULT_EN_CALIB
    #endif
    #ifndef uC_ADC1_EN_INJECTQ
        #define uC_ADC1_EN_INJECTQ      uC_ADC_DEFAULT_EN_INJECTQ
    #endif
    #ifndef uC_ADC1_EN_INJECTQ_AUTO
        #define uC_ADC1_EN_INJECTQ_AUTO uC_ADC_DEFAULT_EN_INJECTQ_AUTO
    #endif
    #ifndef uC_ADC1_OVERRUN_LATEST
        #define uC_ADC1_OVERRUN_LATEST  uC_ADC_DEFAULT_OVERRUN_LATEST
    #endif
    #ifndef uC_ADC1_EN_OS_REG
        #define uC_ADC1_EN_OS_REG       uC_ADC_DEFAULT_EN_OS_REG
    #endif
    #ifndef uC_ADC1_EN_OS_INJ
        #define uC_ADC1_EN_OS_INJ       uC_ADC_DEFAULT_EN_OS_INJ
    #endif
    #ifndef uC_ADC1_OS_TRIG_EACH
        #define uC_ADC1_OS_TRIG_EACH    uC_ADC_DEFAULT_OS_TRIG_EACH
    #endif
    #ifndef uC_ADC1_OS_RATIO
        #define uC_ADC1_OS_RATIO        uC_ADC_DEFAULT_OS_RATIO
    #endif
    #ifndef uC_ADC1_OS_SHIFT
        #define uC_ADC1_OS_SHIFT        uC_ADC_DEFAULT_OS_SHIFT
    #endif
#endif

#if uC_ADC2_ENABLED

#endif

/**
 * @brief Configure ADC Clock
 *
 * Sync clocks derived from AHB1 ( 300MHz )
 * Async clocks derived from PLL2P ( 200MHz )
 * Max Clock of 75MHz (?)
 * 
 * Ideally, we should use /4 prescaler in sync mode to prevent timer sync 
 * jitter. This puts us in grey areas in terms of sampling rates and clocks. 
 * Specifically, see Datasheet pp.245. Target configuration is 12bit reslution, 
 * fADC = 75MHz, Single or Discontinous mode. Datasheet and reference manual 
 * are incredibly vague on what is allowed. 
 * 
 * Since high speed sampling is a preliminary test requirement, and since we 
 * also don't want to delve deeply into the Fast and Slow channels on the STM32 
 * ADC right now, we use the async clock mode which has more prescaler options 
 * and live with the potential sampling jitter. Alternative is to drop HCLK 
 * itself, which reduces all AHB, AXI, and APB clocks.
 * 
 * Implemented (__weak) Default Clock Configuration : 
 *   - Async clock derived from PLL2P
 *   - Prescaler of /32
 *   - ADC clk of 25MHz
 * 
 * This should be fine for all channels at max sampling rate for this clock, 
 * which is probably somewhere between 0.25 and 0.5 MSPS (cumulative) depending 
 * on sampling time and other configurations.
 * 
 * The target clock, however, is a prescaler of /8, which should produce 
 * between 1 and 2 MSPS. At present, however, the ADC interupt handler 
 * implemented here is not fast enough and causes ADC overrun. The interrupt
 * handler does seem to work in scan mode at -O1, but breaks in continous 
 * mode. This needs to be investigated more closely. Use of DMA is probably
 * required.
 * 
 * If the clock speed needs to be changed, the ADC clock configured by the 
 * default implementation of adc_clock_init() can be adjusted. This can 
 * be done by implementing the same function in the application. Also, the 
 * ADC clock prescaler can be overridden in application.h (uC_ADC_CLOCK_PRESCALER).
 * 
 * Note that changing the clock may have other unhandled side effects, for 
 * example: 
 *   - mimimum sample time will change
 * 
 */
void adc_clock_init(void);


#endif

#endif
