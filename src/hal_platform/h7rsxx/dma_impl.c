
#include "dma_impl.h"

#if uC_DMA_ENABLED

static inline void _dma_chn_init(
    HAL_BASE_t intfnum,    
    DMA_PRIORITY_t priority
) {
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr &= ~DMA_CCR_RESET;

    // Set to privileged mode
    HAL_SFR_t * privcfgr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.core->base + OFS_DMA_PRIVCFGR);
    *privcfgr |= (1 << dma_chn_if[intfnum].hwif.chnum);

    // Clear all flags. Reset should take care of this. 
    // HAL_SFR_t * fcr = (HAL_SFR_t *)(hwif->base + OFS_DMA_Cx_FCR);
    // *fcr |= (0x00007F00);

    /**
     * CR Defaults:
     *   - HPDMA LL Port is port 0 (AXI), for direct access to memories. 
     *   - GPDMA LL Port is port 0 (AHB), since no AXI port exists.
     *
     * All interrupts are disabled here, unless a handler is installed.  
     */

    uint32_t crval = 0;

    switch (priority){
        case DMA_PRIORITY_IMMEDIATE:
            crval |= (DMA_CCR_PRIO_1 | DMA_CCR_PRIO_0);
            break;
        case DMA_PRIORITY_HIGH:
            crval |= DMA_CCR_PRIO_1;
            break;
        case DMA_PRIORITY_LOW:
            crval |= DMA_CCR_PRIO_0;
            break;
        case DMA_PRIORITY_BACKGROUND:
        default:
            break;
    }

    if (dma_chn_if[intfnum].state->cb_htc){
        crval |= DMA_CCR_HTIE;  
    }   

    if (dma_chn_if[intfnum].state->cb_tc){
        crval |= DMA_CCR_TCIE;  
    }   

    *cr = crval;

    // *cr != DMA_CCR_EN;
}

#if uC_DMA1_CH0_ENABLED
    
    dma_chn_state_t dma1_ch0_state = {0};

    static inline void _dma1_ch0_init(void){
        _dma_chn_init(
            uC_DMA1_CH0_INTFNUM,
            uC_DMA1_CH0_PRIORITY
        );
    };

#endif

#if uC_DMA2_CH0_ENABLED

    dma_chn_state_t dma2_ch0_state = {0};

    static inline void _dma2_ch0_init(void){
        _dma_chn_init(
            uC_DMA2_CH0_INTFNUM,
            uC_DMA2_CH0_PRIORITY
        );
    };

#endif

#if uC_DMA1_ENABLED

    static const _dma_core_hwif_t _dma1_hwif = {
        .base = HPDMA1_BASE,
        .type = DMA_HWIF_HPDMA,
    };

    static inline void _dma1_init(void){
        __HAL_RCC_HPDMA1_CLK_ENABLE();
    };

#endif

#if uC_DMA2_ENABLED

    static const _dma_core_hwif_t _dma2_hwif = {
        .base = GPDMA1_BASE,
        .type = DMA_HWIF_GPDMA,
    };

    static inline void _dma2_init(void){
        __HAL_RCC_GPDMA1_CLK_ENABLE();
    };

#endif

const dma_chn_if_t dma_chn_if[] = {
    #if uC_DMA1_CH0_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel0_BASE,
                .irqn = HPDMA1_Channel0_IRQn,
                .core = &_dma1_hwif,
                .chnum = 0
            },
            .state = &dma2_ch0_state,
        },
    #endif
    #if uC_DMA2_CH0_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel0_BASE,
                .irqn = GPDMA1_Channel0_IRQn,
                .core = &_dma2_hwif,
                .chnum = 0
            },
            .state = &dma1_ch0_state,
        },
    #endif
};

void dma_init(void){
    #if uC_DMA1_ENABLED
        _dma1_init();
    #endif
    #if uC_DMA2_ENABLED
        _dma2_init();
    #endif
    #if uC_DMA1_CH0_ENABLED
        _dma1_ch0_init();
    #endif
    #if uC_DMA2_CH0_ENABLED
        _dma2_ch0_init();
    #endif
};

void dma_register_cb_tc(HAL_BASE_t intfnum, void (*cb)(void)){
    dma_chn_if[intfnum].state->cb_tc = cb;
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr |= DMA_CCR_TCIE;
}

void dma_register_cb_htc(HAL_BASE_t intfnum, void (*cb)(void)){
    dma_chn_if[intfnum].state->cb_htc = cb;
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr |= DMA_CCR_HTIE;
}

void dma_suspend_channel(HAL_BASE_t intfnum){
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr |= DMA_CCR_SUSP;
    while (!(*cr & DMA_CCR_SUSP)){}
}

void dma_resume_channel(HAL_BASE_t intfnum){
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr &= ~DMA_CCR_SUSP;
}

void dma_setup_xfer(HAL_BASE_t intfnum, dma_xfer_t * xfer){
    /**
     * TODO: 
     *  - Check if there's a transfer in progress. Error out or queue LL if there is.
     */

    uint32_t tr1val = 0;
    tr1val |= ((uint32_t)(xfer->src_opts & DMA_SRC_OPTS_MSK) >> DMA_SRC_OPTS_POS) << DMA_TR1_SRC_OPTS_POS;
    tr1val |= ((uint32_t)(xfer->dst_opts & DMA_DST_OPTS_MSK) >> DMA_DST_OPTS_POS) << DMA_TR1_DST_OPTS_POS;
    tr1val |= ((uint32_t)(xfer->dst_burst_len) << DMA_CTR1_DBL_1_Pos);
    tr1val |= ((uint32_t)(xfer->src_burst_len) << DMA_CTR1_SBL_1_Pos);
    tr1val |= ((uint32_t)(xfer->src_opts & DMA_OPT_WIDTH_MSK) >> DMA_OPT_WIDTH_POS) << DMA_CTR1_SDW_LOG2_Pos;
    tr1val |= ((uint32_t)(xfer->dst_opts & DMA_OPT_WIDTH_MSK) >> DMA_OPT_WIDTH_POS) << DMA_CTR1_DDW_LOG2_Pos;

    /**
     * Memory is always incremented
     * Peripheral is always kept constant
     */
    switch(xfer->type){
        case DMA_XFER_MEMORY_MEMORY:
            tr1val |= DMA_CTR1_SINC;
            tr1val |= DMA_CTR1_DINC;
            break;
        case DMA_XFER_MEMORY_PERIPH:
            tr1val |= DMA_CTR1_SINC;
            break;
        case DMA_XFER_PERIPH_MEMORY:
            tr1val |= DMA_CTR1_DINC;
            break;
        case DMA_XFER_PERIPH_PERIPH:
            break;
        default:
            die();
    }

    HAL_SFR_t * tr1 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR1);
    *tr1 = tr1val;
    
    HAL_SFR_t * tr3 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR3);
    *tr3 = ((uint32_t)(xfer->dst_offset_incr) << 16) | (xfer->src_offset_incr);

    /**
     * Not handled: 
     *  - Options to decrement address instead of increment after each burst or each block.
     */
    HAL_SFR_t * br1 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_BR1);
    *br1 = (uint32_t)(xfer->size & 0xFFFF) | ((xfer->repeats & 0x3FF) << DMA_CBR1_BRC_Pos);

    HAL_SFR_t * sar = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_SAR);
    *sar = xfer->src;

    HAL_SFR_t * dar = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_DAR);
    *dar = xfer->dst;

    /**
     * Not handled: 
     *  - Address offsets for 2D addressing.
     */
    HAL_SFR_t * br2 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_BR2);
    *br2 = 0;

    /**
     * Not handled: 
     *  - Linked List configuration
     */
    HAL_SFR_t * llr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LLR);
    *llr = 0;
}

void dma_setup_flow_control(HAL_BASE_t intfnum, dma_flow_control_t * fc){
    uint32_t tr2val = 0;
    
    switch (fc->req_type){
        case DMA_REQ_SOFTWARE:
            tr2val |= DMA_CTR2_SWREQ;
            break;
        case DMA_REQ_DST:
            tr2val |= DMA_CTR2_DREQ;
            break;
        case DMA_REQ_SRC:
        case DMA_REQ_NONE:
            break;
        default:
            die();
    }

    uint8_t opts = fc->opts;
    if (opts & DMA_OPT_REQ_BLOCK){
        tr2val |= DMA_CTR2_BREQ;
    }
    tr2val |= (uint32_t)(fc->req_sel);
    tr2val |= ((uint32_t)(opts & DMA_FC_TRIG_MODE_MSK) >> DMA_FC_TRIG_MODE_POS) << DMA_CTR2_TRIGM_Pos;
    tr2val |= ((uint32_t)(opts & DMA_FC_TRIG_POL_MSK) >> DMA_FC_TRIG_POL_POS) << DMA_CTR2_TRIGPOL_Pos;
    tr2val |= ((uint32_t)(opts & DMA_FC_EVENT_MODE_MSK) >> DMA_FC_EVENT_MODE_POS) << DMA_CTR2_TCEM_Pos;
    tr2val |= (uint32_t)(fc->trig_sel) << DMA_CTR2_TRIGSEL_Pos;
    
    HAL_SFR_t * tr2 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR2);
    *tr2 = tr2val;
}

void dma_trigger_arm(HAL_BASE_t intfnum){

}

void dma_memcpy(HAL_BASE_t intfnum, void * to, void * from, HAL_BASE_t size, void (*cb)(void));

void dma_check_error(HAL_BASE_t intfnum){
    uint32_t sr = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_SR);
    if (sr & DMA_CSR_USEF){
        // User Setting Error
        die();
    }
    if (sr & DMA_CSR_TOF){
        // Trigger Overrun
        die();
    }
    if (sr & DMA_CSR_ULEF){
        // Update Link Transfer Error
        die();
    }
    if (sr & DMA_CSR_DTEF){
        // Data Transfer Error
        die();
    }
}

void dma_poll(void){
    if (!sizeof(dma_chn_if)){
        return;
    }
    for (int i=0; i <= (sizeof(dma_chn_if)/sizeof(dma_chn_if_t)); i++){
        dma_check_error(i);
    }
}

#endif

