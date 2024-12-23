

#define uC_ADC_DEFAULT_CLOCK_PRESCALER      ADC_CCR_PRESC_3
#define uC_ADC_DEFAULT_CLOCK_PRESCALER_VAL  8
#define uC_ADC_DEFAULT_DATAALIGNLEFT        EBS_FALSE
#define uC_ADC_DEFAULT_EN_CALIB             EBS_TRUE
#define uC_ADC_DEFAULT_EN_INJECTQ           EBS_FALSE
#define uC_ADC_DEFAULT_EN_INJECTQ_AUTO      EBS_FALSE
#define uC_ADC_DEFAULT_OVERRUN_LATEST       EBS_TRUE
#define uC_ADC_DEFAULT_EN_OS_REG            EBS_FALSE
#define uC_ADC_DEFAULT_EN_OS_INJ            EBS_FALSE
#define uC_ADC_DEFAULT_OS_TRIG_EACH         EBS_FALSE   // Not sure what exactly this is
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

#ifndef uC_ADC_CLOCK_PRESCALER
    #define uC_ADC_CLOCK_PRESCALER      uC_ADC_DEFAULT_CLOCK_PRESCALER
    #define uC_ADC_CLOCK_PRESCALER_VAL  uC_ADC_DEFAULT_CLOCK_PRESCALER_VAL
#endif

#define uC_ADC_CLOCK_FREQUENCY          (CLOCKTREE_ADC_FREQ / uC_ADC_CLOCK_PRESCALER_VAL)

#if uC_ADC1_ENABLED
    #ifndef uC_ADC1_DATAALIGNLEFT
        #define uC_ADC1_DATAALIGNLEFT   uC_ADC_DEFAULT_DATAALIGNLEFT
    #endif      
    #ifndef uC_ADC1_EN_CALIB
        #define uC_ADC1_EN_CALIB        uC_ADC_DEFAULT_EN_CALIB
    #endif
    #ifndef uC_ADC1_EN_INJECTQ
        #define uC_ADC1_EN_INJECTQ          uC_ADC_DEFAULT_EN_INJECTQ
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
    #ifndef uC_ADC1_MARK_POLL_OVERRUN
        #define uC_ADC1_MARK_POLL_OVERRUN   uC_ADC_DEFAULT_MARK_POLL_OVERRUN
    #endif
#endif

#if uC_ADC2_ENABLED

#endif
