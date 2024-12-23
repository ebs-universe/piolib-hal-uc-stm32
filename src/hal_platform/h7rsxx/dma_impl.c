
#include "dma_impl.h"
#include <string.h>

#if uC_DMA_ENABLED

static inline void _dma_chn_init(
    HAL_BASE_t intfnum,    
    DMA_PRIORITY_t priority,
    EBS_PRIORITY_t int_priority,
    EBS_BOOL_t enable_cb_tc,
    EBS_BOOL_t enable_cb_hc
);

#if uC_DMA1_ENABLED

    static const _dma_core_hwif_t _dma1_hwif = {
        .base = HPDMA1_BASE,
        .type = DMA_HWIF_HPDMA,
    };          

    static inline void _dma1_init(void){
        __HAL_RCC_HPDMA1_CLK_ENABLE();
        // Freeze in Debug Mode
        DBGMCU->AHB5FZR = 0x0000FFFF;
    };

#endif

#if uC_DMA2_ENABLED

    static const _dma_core_hwif_t _dma2_hwif = {
        .base = GPDMA1_BASE,
        .type = DMA_HWIF_GPDMA,
    };

    static inline void _dma2_init(void){
        __HAL_RCC_GPDMA1_CLK_ENABLE();
        // Freeze in Debug Mode
        DBGMCU->AHB1FZR = 0x0000FFFF;
    };

#endif

#include "_dma/channel_if.h"

#if uC_DMA_MAX_LLIS
    dma_lli_t MM_DMA_LL __attribute__((aligned(4))) _lli[uC_DMA_MAX_LLIS] = {0};
#endif 

void dma_init(void){
    #if uC_DMA_MAX_LLIS
        memset(&_lli, 0, sizeof(_lli));
    #endif
    #if uC_DMA1_ENABLED
        _dma1_init();
    #endif
    #if uC_DMA2_ENABLED
        _dma2_init();
    #endif
    #if uC_DMA1_CH0_ENABLED     
        _dma1_ch0_init();
    #endif
    #if uC_DMA1_CH1_ENABLED     
        _dma1_ch1_init();
    #endif
    #if uC_DMA1_CH2_ENABLED     
        _dma1_ch2_init();
    #endif
    #if uC_DMA1_CH3_ENABLED     
        _dma1_ch3_init();
    #endif
    #if uC_DMA1_CH4_ENABLED     
        _dma1_ch4_init();
    #endif
    #if uC_DMA1_CH5_ENABLED     
        _dma1_ch5_init();
    #endif
    #if uC_DMA1_CH6_ENABLED     
        _dma1_ch6_init();
    #endif
    #if uC_DMA1_CH7_ENABLED     
        _dma1_ch7_init();
    #endif
    #if uC_DMA1_CH8_ENABLED     
        _dma1_ch8_init();
    #endif
    #if uC_DMA1_CH9_ENABLED     
        _dma1_ch9_init();
    #endif
    #if uC_DMA1_CH10_ENABLED     
        _dma1_ch10_init();
    #endif
    #if uC_DMA1_CH11_ENABLED     
        _dma1_ch11_init();
    #endif
    #if uC_DMA1_CH12_ENABLED     
        _dma1_ch12_init();
    #endif
    #if uC_DMA1_CH13_ENABLED     
        _dma1_ch13_init();
    #endif
    #if uC_DMA1_CH14_ENABLED     
        _dma1_ch14_init();
    #endif
    #if uC_DMA1_CH15_ENABLED     
        _dma1_ch15_init();
    #endif
    #if uC_DMA2_CH0_ENABLED
        _dma2_ch0_init();
    #endif
    #if uC_DMA2_CH1_ENABLED     
        _dma2_ch1_init();
    #endif
    #if uC_DMA2_CH2_ENABLED     
        _dma2_ch2_init();
    #endif
    #if uC_DMA2_CH3_ENABLED     
        _dma2_ch3_init();
    #endif
    #if uC_DMA2_CH4_ENABLED     
        _dma2_ch4_init();
    #endif
    #if uC_DMA2_CH5_ENABLED     
        _dma2_ch5_init();
    #endif
    #if uC_DMA2_CH6_ENABLED     
        _dma2_ch6_init();
    #endif
    #if uC_DMA2_CH7_ENABLED     
        _dma2_ch7_init();
    #endif
    #if uC_DMA2_CH8_ENABLED     
        _dma2_ch8_init();
    #endif
    #if uC_DMA2_CH9_ENABLED     
        _dma2_ch9_init();
    #endif
    #if uC_DMA2_CH10_ENABLED     
        _dma2_ch10_init();
    #endif
    #if uC_DMA2_CH11_ENABLED     
        _dma2_ch11_init();
    #endif
    #if uC_DMA2_CH12_ENABLED     
        _dma2_ch12_init();
    #endif
    #if uC_DMA2_CH13_ENABLED     
        _dma2_ch13_init();
    #endif
    #if uC_DMA2_CH14_ENABLED        
        _dma2_ch14_init();
    #endif
    #if uC_DMA2_CH15_ENABLED     
        _dma2_ch15_init();
    #endif
};

static inline void _dma_chn_init(
    HAL_BASE_t intfnum,    
    DMA_PRIORITY_t priority,
    EBS_PRIORITY_t int_priority,
    EBS_BOOL_t enable_cb_tc,
    EBS_BOOL_t enable_cb_hc
) {

    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr &= ~DMA_CCR_RESET;

    // Set to privileged mode
    HAL_SFR_t * privcfgr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.core->base + OFS_DMA_PRIVCFGR);
    *privcfgr |= (1 << dma_chn_if[intfnum].hwif.chnum);

    // Clear all flags. Reset should take care of this. 
    // HAL_SFR_t * fcr = (HAL_SFR_t *)(hwif->base + OFS_DMA_Cx_FCR);
    // *fcr |= (0x00007F00);

    if (int_priority <= PRIO_INT_LOW){
        dma_chn_if->state->use_int = EBS_TRUE;
        HAL_NVIC_SetPriority(dma_chn_if->hwif.irqn, int_priority, 0);
        HAL_NVIC_EnableIRQ(dma_chn_if->hwif.irqn);
    }

    /**
     * CR Defaults:
     *   - HPDMA LL Port is port 0 (AXI)
     *   - GPDMA LL Port is port 0 (AHB)
     *
     * If LL is to be placed in DTCM, it can be done only for HPDMA 
     * and probably will need the HPDMA LL port be changed to 1. It 
     * does not seem that there will be any significant benefit to 
     * doing so, however.
     * 
     * All interrupts are    disabled here, unless a handler is installed.  
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

    if (enable_cb_tc){
        crval |= DMA_CCR_TCIE; 
    }

    if (enable_cb_hc){
        crval |= DMA_CCR_HTIE;      
    }

    *cr = crval;

    HAL_SFR_t * lbar = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LBAR);
    *lbar = MEMMAP_DMA_LLI_BASE & 0xFFFF0000;

    // switch (dma_chn_if[intfnum].hwif.core->type)
    // {
    // case DMA_HWIF_HPDMA:        
    //     break;
    // case DMA_HWIF_GPDMA:
    //     break;
    // default:
    //     die();
    //     break;
    // }

    // *cr != DMA_CCR_EN;
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

static dma_lli_t * _dma_request_lli(uint8_t token) {
    for (HAL_BASE_t i = 0; i < uC_DMA_MAX_LLIS; i++){
        if (!_lli[i].in_use) {
            _lli[i].token = token;
            _lli[i].in_use = EBS_TRUE;
            return &_lli[i];
        }   
    }
    die();
    return NULL; 
}

void dma_enqueue(HAL_BASE_t intfnum, dma_lli_t * lli){
    if (!dma_chn_if[intfnum].state->ll_pending){
        dma_chn_if[intfnum].state->ll_pending = lli;
        return;
    }

    uint32_t * walker = &(dma_chn_if[intfnum].state->ll_pending->llr);         
    uint32_t lbar = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LBAR);

    while (*walker) {
        dma_lli_t * next = (dma_lli_t *)(lbar | (*walker & 0x0000FFFC)) ;
        walker = &(next->llr);
    }           

    // Attach the provided item to the end
    const uint32_t llspec = (DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL);
    *walker = llspec | (((uint32_t)lli) & 0x0000FFFC);
}


dma_lli_t* dma_setup_xfer(HAL_BASE_t intfnum, dma_xfer_t * xfer, dma_flow_control_t * fc, uint8_t token){
    dma_lli_t * lli = _dma_request_lli(token);

    // Transfer Configuration

    lli->tr1 = 0;           
    lli->tr1 |= ((uint32_t)(xfer->src_opts & DMA_SRC_OPTS_MSK) >> DMA_SRC_OPTS_POS) << DMA_TR1_SRC_OPTS_POS;
    lli->tr1 |= ((uint32_t)(xfer->dst_opts & DMA_DST_OPTS_MSK) >> DMA_DST_OPTS_POS) << DMA_TR1_DST_OPTS_POS;
    lli->tr1 |= ((uint32_t)(xfer->dst_burst_len) << DMA_CTR1_DBL_1_Pos);
    lli->tr1 |= ((uint32_t)(xfer->src_burst_len) << DMA_CTR1_SBL_1_Pos);
    lli->tr1 |= ((uint32_t)(xfer->src_opts & DMA_OPT_WIDTH_MSK) >> DMA_OPT_WIDTH_POS) << DMA_CTR1_SDW_LOG2_Pos;
    lli->tr1 |= ((uint32_t)(xfer->dst_opts & DMA_OPT_WIDTH_MSK) >> DMA_OPT_WIDTH_POS) << DMA_CTR1_DDW_LOG2_Pos;

    /**
     * Memory is always incremented
     * Peripheral is always kept constant
     */
    switch(xfer->type){
        case DMA_XFER_MEMORY_MEMORY:
            lli->tr1 |= DMA_CTR1_SINC;
            lli->tr1 |= DMA_CTR1_DINC;
            break;
        case DMA_XFER_MEMORY_PERIPH:
            lli->tr1 |= DMA_CTR1_SINC;
            break;
        case DMA_XFER_PERIPH_MEMORY:
            lli->tr1 |= DMA_CTR1_DINC;
            break;
        case DMA_XFER_PERIPH_PERIPH:
            break;
        default:
            die();
    }

    /** 
     * Not handled: 
     *  - Options to decrement address instead of increment after each burst or each block.
     */
    lli->br1 = (uint32_t)(xfer->size & 0xFFFF) | ((xfer->repeats & 0x3FF) << DMA_CBR1_BRC_Pos);
    lli->sar = xfer->src;
    lli->dar = xfer->dst;
    lli->llr = 0;
    
    // Flow Control Configuration

    lli->tr2 = 0;
    switch (fc->req_type){
        case DMA_REQ_SOFTWARE:
            lli->tr2 |= DMA_CTR2_SWREQ;
            break;
        case DMA_REQ_DST:
            lli->tr2 |= DMA_CTR2_DREQ;
            break;
        case DMA_REQ_SRC:
        case DMA_REQ_NONE:
            break;
        default:
            die();
    };

    // We expect linear channels to use channel level event generation 
    // (end of LL). We do not support other event modes nor do we allow
    // mixing of different transfer types in the same channel.          
    uint8_t event_mode = 0b11;

    if (dma_chn_if[intfnum].hwif.type == DMA_CHN_TYPE_CIRCULAR){
        // We expect circular channels to use LLI level event generation 
        // (end of each LLI). We do not support other event modes nor do 
        // we allow mixing of different transfer types in the same channel.
        event_mode = 0b10;
    }
    
    uint8_t opts = fc->opts;
    
    uint8_t event_mode_requested = (opts & DMA_FC_EVENT_MODE_MSK) >> DMA_FC_EVENT_MODE_POS;
    if (event_mode_requested && event_mode_requested != event_mode) {
        die();
    }

    if (opts & DMA_OPT_REQ_BLOCK) {
        lli->tr2 |= DMA_CTR2_BREQ;
    }
    lli->tr2 |= (uint32_t)(fc->req_sel);
    lli->tr2 |= ((uint32_t)(opts & DMA_FC_TRIG_MODE_MSK) >> DMA_FC_TRIG_MODE_POS) << DMA_CTR2_TRIGM_Pos;
    lli->tr2 |= ((uint32_t)(opts & DMA_FC_TRIG_POL_MSK) >> DMA_FC_TRIG_POL_POS) << DMA_CTR2_TRIGPOL_Pos;
    lli->tr2 |= (uint32_t)(event_mode) << DMA_CTR2_TCEM_Pos;
    lli->tr2 |= (uint32_t)(fc->trig_sel) << DMA_CTR2_TRIGSEL_Pos;

    lli->cb_hc = fc->cb_hc;
    lli->cb_tc = fc->cb_tc;
    
    // 2D Addressing Configuration

    // tr3, br2 have to do with 2d addressing and is not part of the linked list 
    // structure for the low numbered channels. we decide to simply not support it 
    // for now, and will need to deal with it separately when a suitable use case 
    // presents itself.     

    // HAL_SFR_t * br2 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_BR2);
    // *br2 = 0;    

    // HAL  _SFR_t * tr3 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR3);
    // *tr3 = ((uint32_t)(xfer->dst_offset_incr) << 16) | (xfer->src_offset_incr)
    
    dma_enqueue(intfnum, lli);
    return lli; 
}   

static inline void _dma_exec(HAL_BASE_t intfnum, dma_lli_t * lli){
    HAL_SFR_t * tr1 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR1);
    *tr1 = lli->tr1;

    HAL_SFR_t * tr2 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_TR2);
    *tr2 = lli->tr2;

    HAL_SFR_t * br1 = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_BR1);
    *br1 = lli->br1;
    
    HAL_SFR_t * sar = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_SAR);
    *sar = lli->sar;

    HAL_SFR_t * dar = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_DAR);
    *dar = lli->dar;

    HAL_SFR_t * llr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LLR);
    *llr = lli->llr;
}

void dma_activate(HAL_BASE_t intfnum){
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    if (dma_chn_if[intfnum].state->ll_active){
        /** 
         * The last transaction is either in process or not yet handled. 
         * We don't do anything here. It will be upto the interrupt handlers 
         * or whatever mechanism is used to close out the transaction to 
         * trigger activation of the next pending ll.
         */ 
        return;
    }
    if (dma_chn_if[intfnum].state->ll_pending) {
        critical_enter();
        dma_chn_if[intfnum].state->ll_active = dma_chn_if[intfnum].state->ll_pending;
        dma_chn_if[intfnum].state->ll_pending = NULL;
        _dma_exec(intfnum, dma_chn_if[intfnum].state->ll_active);
        *cr |= DMA_CCR_EN;
        critical_exit();
    }       
}               

static inline void _dma_process_tc_single(dma_lli_t * lli){
    if (!lli) { 
        die();
    }
    if (lli->cb_tc){
        lli->cb_tc(lli->token);
    }
    lli->in_use = EBS_FALSE;
}

static inline void dma_process_tc_linear(HAL_BASE_t intfnum){
    // All of the LL is done
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    dma_lli_t * lli = dma_chn_if[intfnum].state->ll_active;
    _dma_process_tc_single(lli);

    uint32_t * walker = &(lli->llr);         
    uint32_t lbar = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LBAR);
    
    while (*walker) {
        dma_lli_t * next = (dma_lli_t *)(lbar | (*walker & 0x0000FFFC)) ;
        _dma_process_tc_single(next);
        walker = &(next->llr);
    }

    if (dma_chn_if[intfnum].state->ll_pending){
        dma_chn_if[intfnum].state->ll_active = dma_chn_if[intfnum].state->ll_pending;
        dma_chn_if[intfnum].state->ll_pending = NULL;
        _dma_exec(intfnum, dma_chn_if[intfnum].state->ll_active);
        *cr |= DMA_CCR_EN; 
    } else {
        dma_chn_if[intfnum].state->ll_active = NULL;
    }
}

static inline void dma_process_tc_circular(HAL_BASE_t intfnum){
    // Only one of the LLIs is done
    dma_lli_t * lli = dma_chn_if[intfnum].state->ll_active;
    if (!lli) { 
        die();
    }
    if (lli->cb_tc){
        lli->cb_tc(lli->token);
    }

    uint32_t lbar = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_LBAR);

    if (lli->llr){
        // Rotate active forward
        dma_chn_if[intfnum].state->ll_active = (dma_lli_t *)(lbar | (lli->llr & 0x0000FFFC));
    } else {
        // We don't expect to reach the end of this list
        die();
    }
}

void dma_process_events(HAL_BASE_t intfnum){
    uint32_t sr = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_SR);
    HAL_SFR_t * fcr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_FCR);

    if (!sr){
        return;
    }
    if ((sr & DMA_CSR_TCF) && !(dma_chn_if[intfnum].state->use_int)){
        // Clear the flag.  
        *fcr |= DMA_CFCR_TCF;
        
        switch (dma_chn_if[intfnum].hwif.type) {
            case DMA_CHN_TYPE_LINEAR:
                dma_process_tc_linear(intfnum);
                break;
            case DMA_CHN_TYPE_CIRCULAR:
                dma_process_tc_circular(intfnum);
                break;
            default:
                die();
        }
    }
        
    // Half transfer with linked list isnt going to work. We 
    // dont actually know which lli is at its halfway point by 
    // the time we get here. Half transfer will probably need
    // interrupt based handling.
    // if (sr & DMA_CSR_HTF){
    //     *fcr |= DMA_CFCR_HTF;
    //     if (dma_chn_if[intfnum].state->ll_active->cb_hc){
    //         dma_chn_if[intfnum].state->ll_active->cb_hc();
    //     };
    // }
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
        dma_process_events(i);
    }
}

static EBS_BOOL_t _memmap_check_tcm(HAL_ADDRESS_t ptr){
    #if MEMMAP_ENABLE_FASTDATA
        if (ptr >= MEMMAP_FASTDATA_START && ptr <= MEMMAP_FASTDATA_END){
            return EBS_TRUE;
        }
    #endif
    #if MEMMAP_ENABLE_FASTEXEC
        if (ptr >= MEMMAP_FASTEXEC_START && ptr <= MEMMAP_FASTEXEC_END){
            return EBS_TRUE;
        }
    #endif
    return EBS_FALSE;
}

void dma_memcpy(HAL_BASE_t intfnum, void * to, void * from, HAL_BASE_t size, void (*cb)(uint8_t), uint8_t token){
    /**
     * Implementation Notes
     *  - AP 0 is AXI for HPDMA, AHB for GPDMA
     *  - AP 1 is AHB for HPDMA, AHB for GPDMA
     *  - DTCM can be accessed only using AHB with HPDMA. To read / write 
     *    DTCM, use a HPDMA channel (DMA1) and add DMA_OPT_AP to the 
     *    appropriate src_opts or dst_opts
     *  - Not enabling any exchange / reordering / padding
     *  - Using 4 byte width
     *  - Trigger is not used
     *  - Software Request is used
     *  - length needs to be a multiple of the appropriate beat / burst size, 
     *    a user error is generated if not.  
     *  - Underlying implementation hardcodes channels to privileged mode
     * 
     * TODO : 
     * 
     *  - HPDMA over AXI should be able to handle 8 byte width. This 
     *    triggers a user error, however. It could be an alignment 
     *    issue. Check.
     *  - chntype should allow longer burst lengths when CHNTYPE2
     *    is used. 
     */
    
    // DMA_CHN_TYPE chntype = dma_chn_if[intfnum].hwif.hwtype;
    DMA_HWIF_TYPE dmatype = dma_chn_if[intfnum].hwif.core->type;
    
    dma_flow_control_t fc = {
        .opts = DMA_OPT_REQ_BLOCK | (0b11 << DMA_FC_EVENT_MODE_POS),
        .req_type = DMA_REQ_SOFTWARE,
        .req_sel = 0,
        .trig_sel = 0,
        .cb_hc = NULL,
        .cb_tc = cb
    };  

    dma_xfer_t xfer = {
        .src = (HAL_ADDRESS_t)from, 
        .dst = (HAL_ADDRESS_t)to,
        .size = size,
        .type = DMA_XFER_MEMORY_MEMORY,
        .src_burst_len = 8,
        .dst_burst_len = 8,
        .repeats = 0
    };    

    switch (dmatype)
    {
        case DMA_HWIF_GPDMA:
            if (_memmap_check_tcm(xfer.src) || _memmap_check_tcm(xfer.dst)){
                die();
            }
            xfer.src_opts = (0b10 << DMA_OPT_WIDTH_POS);
            xfer.dst_opts = (0b10 << DMA_OPT_WIDTH_POS) | DMA_OPT_AP;
            break;  
        case DMA_HWIF_HPDMA:
            if (_memmap_check_tcm(xfer.src)){
                xfer.src_opts = (0b10 << DMA_OPT_WIDTH_POS) | DMA_OPT_AP;
            } else {    
                xfer.src_opts = (0b10 << DMA_OPT_WIDTH_POS);
            }
            if (_memmap_check_tcm(xfer.dst)){
                xfer.dst_opts = (0b10 << DMA_OPT_WIDTH_POS) | DMA_OPT_AP;
            } else {
                xfer.dst_opts = (0b10 << DMA_OPT_WIDTH_POS);
            }
            break;
        default:
            break;
    }
    
    // Configure the DMA peripheral for the transfer
    dma_setup_xfer(intfnum, &xfer, &fc, token);

    // Software request for the DMA transfer
    // dma_activate(intfnum);
}

#endif

