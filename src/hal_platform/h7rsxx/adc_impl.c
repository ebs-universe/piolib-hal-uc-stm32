

#include "adc_impl.h"
#include <time/time.h>
#include <platform/sections.h>

#if uC_ADC_ENABLED

/**
 * @name Hardcoded Initialization Functions
 * 
 */
/**@{*/ 
static void _adc_builtin_channels_enable(void);
static void _adc_dual_config(void);
static void _adc_power_up(const _adc_hwif_t *const hwif);
static void _adc_calibrate_internal(const _adc_hwif_t *const hwif);
static void _adc_configure_sampling_time(const _adc_hwif_t *const hwif);
static void _adc_configure_channels(HAL_BASE_t intfnum);
static void _adc_init(
        const _adc_hwif_t *const hwif,
        EBS_BOOL_t dataalignleft,
        EBS_BOOL_t en_injectq,
        EBS_BOOL_t en_enjectq_auto,
        EBS_BOOL_t overrun_latest,
        EBS_BOOL_t en_os_reg,
        EBS_BOOL_t en_os_inj,
        EBS_BOOL_t os_trig_each,
        uint32_t os_ratio,      
        uint8_t os_shift,
        uint8_t trigger_src
);
static void _adc_init_interrupt(const _adc_hwif_t *const hwif);
static void _adc_init_dma(const _adc_hwif_t *const hwif);
/**@}*/

__weak void adc_clock_init(void){
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2P;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { die(); }
    __HAL_RCC_ADC12_CLK_ENABLE();
    
    HAL_SFR_t *ccr = (HAL_SFR_t *)(ADC12_COMMON_BASE + OFS_ADC_CCR);
    *ccr = (*ccr & ~(ADC_CCR_PRESC | ADC_CCR_CKMODE)) | (uC_ADC_CLOCK_PRESCALER);
}

void adc_interrupt_enable(void){
    HAL_NVIC_SetPriority(ADC1_2_IRQn, PRIO_ADC, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

void adc_interrupt_disable(void){
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
}

static void _adc_builtin_channels_enable(void){
    HAL_SFR_t *ccr = (HAL_SFR_t *)(ADC12_COMMON_BASE + OFS_ADC_CCR);

    #if uC_ENABLE_AIN_TEMP
        *ccr |= ADC_CCR_TSEN;
    #endif
    #if uC_ENABLE_AIN_IREF
        *ccr |= ADC_CCR_VREFEN;
    #endif
    #if uC_ENABLE_AIN_VBAT
        *ccr |= ADC_CCR_VBATEN;
    #endif
    #if uC_ENABLE_AIN_VDDC
        HAL_SFR_t *or = (HAL_SFR_t *)(ADC2_BASE + OFS_ADCn_OR);
        *or |= ADC_OR_OP0;
    #endif
}

// Disable Dual mode and associated DMA config. Revisit later when needed.
static void _adc_dual_config(void){
    HAL_SFR_t *ccr = (HAL_SFR_t *)(ADC12_COMMON_BASE + OFS_ADC_CCR);
    *ccr &= ~(ADC_CCR_DUAL | ADC_CCR_MDMA | ADC_CCR_DELAY);
}

void adc_common_init(void){
    adc_clock_init();
    #if uC_ADC_ENABLE_INTERRUPT
        adc_interrupt_enable();
    #endif
    _adc_builtin_channels_enable();
    _adc_dual_config();
}

#if uC_ADC1_ENABLED
static const _adc_hwif_t _adc1_hwif = {
       .type = ADC12_HWIF, 
       .base = ADC1_BASE,
       .common = ADC12_COMMON_BASE,
       .chnmask = uC_ADC1_CHNMASK,
       .dmmode = uC_ADC1_DM_MODE
};

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
        _adc_init_dma(&_adc1_hwif);
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

static void _adc_power_up(const _adc_hwif_t *const hwif){
    HAL_SFR_t * cr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_CR);

    // Exit deep power down mode
    if (*cr & ADC_CR_DEEPPWD){
        *cr &= ~ADC_CR_DEEPPWD;
    }

    // Enable the ADC voltage regulator
    if (!(*cr & ADC_CR_ADVREGEN)){
        *cr |= ADC_CR_ADVREGEN;
        // Wait about 20us
        for (volatile uint32_t i = 0; i < ((20UL * CLOCKTREE_CPU_FREQ) / 1000000UL); i++) {
            __NOP();
        }
    }

    if (!(*cr & ADC_CR_ADVREGEN)){
        die();
    }
}

static void _adc_calibrate_internal(const _adc_hwif_t *const hwif){
    HAL_SFR_t * cr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_CR);
    if (*cr & ADC_CR_ADEN) {
        die();
    }
    *cr &= ~(ADC_CR_ADCALDIF);
    *cr |= ADC_CR_ADCAL;
    tm_system_t calib_timeout_expiry, ctime; 
    tm_sdelta_t calib_timeout = 2000;
    tm_current_time(&calib_timeout_expiry);
    tm_apply_sdelta(&calib_timeout_expiry, &calib_timeout);
    while (*cr & ADC_CR_ADCAL) {
        tm_current_time(&ctime);
        if (tm_cmp_stime(&ctime, &calib_timeout_expiry) > 0){
            die();
        }
        __NOP();
    };
}

static void _adc_init(
        const _adc_hwif_t *const hwif,
        EBS_BOOL_t dataalignleft,
        EBS_BOOL_t en_injectq,
        EBS_BOOL_t en_injectq_auto,
        EBS_BOOL_t overrun_latest,
        EBS_BOOL_t en_os_reg,
        EBS_BOOL_t en_os_inj,
        EBS_BOOL_t os_trig_each,
        uint32_t os_ratio,
        uint8_t os_shift,
        uint8_t trigger_src
    )
{
    HAL_SFR_t * cfgr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_CFGR);
    HAL_SFR_t * cfgr2 = (HAL_SFR_t *)(hwif->base + OFS_ADCn_CFGR2);
    HAL_SFR_t * cr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_CR);
    HAL_SFR_t * isr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_ISR);

    uint32_t cfgrval=0, cfgr2val=0;

    if (!en_injectq){
        cfgrval |= ADC_CFGR_JQDIS;
    } else if (en_injectq_auto) {
        cfgrval |= ADC_CFGR_JAUTO;
    };

    if (dataalignleft){
        cfgrval |= ADC_CFGR_ALIGN;
    }

    if (overrun_latest){ cfgrval |= ADC_CFGR_OVRMOD; }
    
    if (trigger_src != 0xFF) {
        if (trigger_src & 0x80){
            cfgrval |= ADC_CFGR_EXTEN_1;
        }
        if (trigger_src & 0x40) {
            cfgrval |= ADC_CFGR_EXTEN_0;
        }
        cfgrval |= ((trigger_src & 0x1F) << ADC_CFGR_EXTSEL_Pos);
    }
    
    // Try to avoid overruns.
    cfgrval |= ADC_CFGR_AUTDLY;

    // This bit is set as configured to work with the trigger, but is 
    // overidden by individual software ADC start functions if they are used. 
    // #if APP_ADC1_MODE == ADC_MODE_SCAN || APP_ADC1_MODE == ADC_MODE_SINGLE
    //     cfgrval &= ~ADC_CFGR_CONT;
    // #elif APP_ADC1_MODE == ADC_MODE_CONTINUOUS 
    //     cfgrval |= ADC_CFGR_CONT;
    // #endif

    if (en_os_reg){      cfgr2val |= ADC_CFGR2_ROVSE;}
    if (en_os_inj){      cfgr2val |= ADC_CFGR2_JOVSE;}
    if (os_trig_each){   cfgr2val |= ADC_CFGR2_TROVS;}
    cfgr2val |= (ADC_CFGR2_ROVSM | os_ratio | (os_shift << ADC_CFGR2_OVSS_Pos));

    *cfgr = cfgrval;
    *cfgr2 = cfgr2val;

    while (!(*isr & ADC_ISR_ADRDY)){
        *cr |= ADC_CR_ADEN;
    }
};

static void _adc_init_interrupt(const _adc_hwif_t *const hwif){
    HAL_SFR_t * ier = (HAL_SFR_t *)(hwif->base + OFS_ADCn_IER);
    *ier |= (ADC_IER_EOCIE);
}

static void _adc_init_dma(const _adc_hwif_t *const hwif){

}

void adc_init(){
    #if uC_ADC_ENABLED
        adc_common_init();
        #if uC_ADC1_ENABLED
            _adc1_init();
        #endif
        #if uC_ADC2_ENABLED
            _adc2_init();
        #endif
    #endif
}

static void _adc_configure_sampling_time(const _adc_hwif_t *const hwif){
    (void)uC_ADC_SAMPLETIME;
    HAL_SFR_t * smpr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_SMPR1);
    *smpr = 0;
    for (HAL_BASE_t i = 0; i < 10; i++){
        *smpr |= (uC_ADC_SAMPLETIME << (i*3));
    }
    smpr = (HAL_SFR_t *)(hwif->base + OFS_ADCn_SMPR2);
    *smpr = 0;
    for (HAL_BASE_t i = 0; i < 9; i++){
        *smpr |= (uC_ADC_SAMPLETIME << (i*3));
    }
}

void _adc_write_regseq(const adc_if_t * const aif) {
    uint32_t chnmask = aif->state->chnmask;
    uint8_t chncount = 0;

    HAL_SFR_t * adc_sqr[4];
    adc_sqr[0] = (volatile uint32_t *)(aif->hwif->base + OFS_ADCn_SQR1);
    adc_sqr[1] = (volatile uint32_t *)(aif->hwif->base + OFS_ADCn_SQR2);
    adc_sqr[2] = (volatile uint32_t *)(aif->hwif->base + OFS_ADCn_SQR3);
    adc_sqr[3] = (volatile uint32_t *)(aif->hwif->base + OFS_ADCn_SQR4);
    
    for (int r = 0; r < 4; r++) {
        *adc_sqr[r] = 0;
    }

    const uint8_t bit_positions[5] = {0, 6, 12, 18, 24};  

    uint8_t reg_index = 0;
    uint8_t slot_index = 1;

    // Loop through each bit in chnmask to find set bits
    for (uint8_t i = 0; i < 19; i++) {
        if (chnmask & (1 << i)) {  
            // Insert channel i at reg:slot
            *adc_sqr[reg_index] |= (i << bit_positions[slot_index]);
            if (slot_index == 4) {
                slot_index = 0;
                reg_index ++;
                if (reg_index > 3) {
                    die();
                }
            } else {
                slot_index ++;
            }
            chncount ++;
            if (chncount >= 16){
                die();
            }
        }
    }

    *adc_sqr[0] |= (chncount - 1) & 0x0F;
}

static void _adc_configure_channels(HAL_BASE_t intfnum){
    adc_if[intfnum]->state->chnmask = adc_if[intfnum]->hwif->chnmask;
    _adc_write_regseq(adc_if[intfnum]);
};

void adc_enable_channel(HAL_BASE_t intfnum, HAL_BASE_t chnum) {
    uint32_t newchn = (1 << chnum);
    
    // Check channel is included in the hwif
    if (!(adc_if[intfnum]->hwif->chnmask & newchn)) {
        die();
    }
    
    // Check channel is not already enabled in chnmask
    if (adc_if[intfnum]->state->chnmask & newchn) {
        return;
    }

    // Add channel to chnmask
    adc_if[intfnum]->state->chnmask |= newchn;
    
    // Rewrite sequence
    _adc_write_regseq(adc_if[intfnum]);
}

void adc_disable_channel(HAL_BASE_t intfnum, HAL_BASE_t chnum) {
    uint32_t oldchn = (1 << chnum);
    
    // Check channel is enabled in chnmask to start with
    if (!(adc_if[intfnum]->state->chnmask & oldchn)) {
        return;
    }

    // Remove channel from chnmask
    adc_if[intfnum]->state->chnmask &= ~oldchn;

    // Rewrite sequence
    _adc_write_regseq(adc_if[intfnum]);
}

void adc_install_eoc_handler(HAL_BASE_t intfnum, void (*handler)(HAL_BASE_t, void *)){
    adc_if[uC_ADC1_INTFNUM]->state->handler_eoc = handler;
};

#if uC_ADC_SUPPORT_DIRECT
void adc_install_eos_handler(HAL_BASE_t intfnum, void (*handler)(void)){
    adc_if[uC_ADC1_INTFNUM]->state->handler_eos = handler;
};
#endif

#if uC_ADC_SUPPORT_DMA
void adc_install_eob_handler(HAL_BASE_t intfnum, void (*handler)(void *)){
    adc_if[uC_ADC1_INTFNUM]->state->handler_eob = handler;
};
#endif

void adc_poll(void){
    #if uC_ADC1_ENABLED
    if (adc1_state.overrun) {
        die();
    }
    #endif
    #if uC_ADC2_ENABLED
    if (adc2_state.overrun) {
        die();
    }
    #endif
}

void adc_trigger_single(HAL_BASE_t intfnum, HAL_BASE_t chnum){
    if (adc_if[intfnum]->state->mode != ADC_MODE_IDLE){
        return;
    }
    if (!(adc_if[intfnum]->hwif->chnmask & (1 << chnum))){
        return;
    }
    // Not implemented          
    die();
}

static inline void _adc_prep_conv(HAL_BASE_t intfnum){
    uint32_t seqstate = adc_if[intfnum]->state->chnmask;
    uint8_t nextchn = __builtin_ctz(seqstate);
    if (nextchn) {
        seqstate >>= nextchn;
    }
    adc_if[intfnum]->state->overrun = 0;
    
    #if uC_ADC_SUPPORT_DIRECT
    if (adc_if[intfnum]->hwif->dmmode == ADC_DM_INTERRUPT || adc_if[intfnum]->hwif->dmmode == ADC_DM_POLLING){
        adc_if[intfnum]->state->lastresult = 0;
        adc_if[intfnum]->state->firstchn = nextchn;
        adc_if[intfnum]->state->nextchn = nextchn;
        adc_if[intfnum]->state->seqstate = seqstate;
    } 
    #endif

    #if uC_ADC_SUPPORT_DMA
    if (adc_if[intfnum]->hwif->dmmode == ADC_DM_DMA){
        ;
    }
    #endif
}

void adc_arm_trigger(HAL_BASE_t intfnum){
    if (adc_if[intfnum]->state->mode != ADC_MODE_IDLE){
        return;
    }
    if (adc_if[intfnum]->state->chnmask == 0) {
        return;
    }
    
    _adc_prep_conv(intfnum);
    adc_if[intfnum]->state->mode = ADC_MODE_CONTINUOUS;
    
    HAL_SFR_t * cfgr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CFGR);
    *cfgr &= ~ADC_CFGR_CONT;

    HAL_SFR_t * cr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CR);
    *cr |= ADC_CR_ADSTART;
}   

void adc_trigger_scan(HAL_BASE_t intfnum){
    if (adc_if[intfnum]->state->mode != ADC_MODE_IDLE){
        return;
    }
    if (adc_if[intfnum]->state->chnmask == 0) {
        return;
    }
    
    _adc_prep_conv(intfnum);
    adc_if[intfnum]->state->mode = ADC_MODE_SCAN;

    HAL_SFR_t * cfgr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CFGR);
    *cfgr &= ~ADC_CFGR_CONT;

    HAL_SFR_t * cr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CR);
    *cr |= ADC_CR_ADSTART;
}

void adc_trigger_autoscan(HAL_BASE_t intfnum){
    if (adc_if[intfnum]->state->mode != ADC_MODE_IDLE){
        return;
    }
    if (adc_if[intfnum]->state->chnmask == 0) {
        return;
    }
    
    _adc_prep_conv(intfnum);
    adc_if[intfnum]->state->mode = ADC_MODE_CONTINUOUS;
    
    HAL_SFR_t * cfgr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CFGR);
    *cfgr |= ADC_CFGR_CONT;

    HAL_SFR_t * cr = (HAL_SFR_t *)(adc_if[intfnum]->hwif->base + OFS_ADCn_CR);
    *cr |= ADC_CR_ADSTART;
}

#endif
