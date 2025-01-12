

#ifdef EBS_ADC_COMPILATION_UNIT

#if uC_ADC1_ENABLED                                 
static const _adc_hwif_t _adc1_hwif = {
       .type = ADC12_HWIF, 
       .base = ADC1_BASE,
       .common = ADC12_COMMON_BASE,
       .chnmask = uC_ADC1_CHNMASK,
       .dmareq_sel = uC_ADC1_DMA_REQSRC,
       .dmmode = uC_ADC1_DM_MODE
};

#if uC_ADC1_DM_MODE == ADC_DM_DMA

MM_ADC_DMA_BUFFER 
uint16_t adc1_dmabuf1[uC_ADC1_DMA_BUFSIZE + 1];

MM_ADC_DMA_BUFFER 
uint16_t adc1_dmabuf2[uC_ADC1_DMA_BUFSIZE + 1];

#endif

adc_state_t adc1_state FASTDATA;


void _adc1_init(void);

const adc_if_t adc1_if = {
    &_adc1_hwif,
    &adc1_state,
    _adc1_init
};

void _adc1_init(void){
    // adc_common_init needs to be called as well
    // we don't do it here because it must only be 
    // called once for all ADCs
    _adc_power_up(&_adc1_hwif);
    
    #if uC_ADC1_EN_CALIB
        _adc_calibrate_internal(&_adc1_hwif);
    #endif
    
    _adc_init(
        &_adc1_hwif,
        uC_ADC1_DATAALIGNLEFT,
        uC_ADC1_EN_INJECTQ,
        uC_ADC1_EN_INJECTQ_AUTO,
        uC_ADC1_OVERRUN_LATEST,
        uC_ADC1_EN_OS_REG,
        uC_ADC1_EN_OS_INJ,
        uC_ADC1_OS_TRIG_EACH,
        uC_ADC1_OS_RATIO,
        uC_ADC1_OS_SHIFT,
        uC_ADC1_TRIGSRC
    );
    
    #if uC_ADC_CONFIGURE_CHANNELS
        _adc_configure_channels(uC_ADC1_INTFNUM);
    #endif
    
    _adc_configure_sampling_time(&_adc1_hwif);
    adc1_state.mode = ADC_MODE_IDLE;

    #if uC_ADC_ENABLE_INTERRUPT
        _adc_init_interrupt(&_adc1_hwif);
    #endif 

    #if uC_ADC1_DM_MODE == ADC_DM_DMA
        _adc_init_dma(&adc1_if);
    #endif

    return;
}
#endif

#if uC_ADC2_ENABLED

#endif

const adc_if_t *const adc_if[uC_ADCS_ENABLED + 1] = {
    #if uC_ADC1_ENABLED
        &adc1_if,
    #endif
    #if uC_ADC2_ENABLED
        &adc2_if,
    #endif
    NULL
};

#endif
