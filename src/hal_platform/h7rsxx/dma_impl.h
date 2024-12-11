

#ifndef DMA_IMPL_H
#define DMA_IMPL_H


#include <hal/uc/dma.h>

/**
 * 
 * The STM32H7S3RSxx has 2 DMA peripherals : 
 *  - HPDMA : intfnum 0
 *  - GPDMA : intfnum 1
 * 
 * It seems HPDMA is suited to DTCM and the large SRAM1/2/3/4 on AXI, 
 * while GPDMA is suited to the 16kb SRAM1/2 and AHB peripherals. 
 * Details to be worked out. 
 * 
 * It may be necessary to add an additional section to be able to use 
 * this. For the moment, plan is to use the 16kb SRAM1/2 for GPDMA use 
 * with SHAREDATA, while HPDMA can be used with FASTDATA in DTCM.
 */

#if uC_DMA_ENABLED 

typedef enum {
    DMA_HWIF_HPDMA,
    DMA_HWIF_GPDMA,
} DMA_HWIF_TYPE;

typedef enum {
    DMA_HWIF_CHNTYPE_1,
    DMA_HWIF_CHNTYPE_2,
} DMA_CHN_TYPE;

typedef struct _DMA_CORE_HWIF_t{
    const HAL_ADDRESS_t base;
    const DMA_HWIF_TYPE type;
} _dma_core_hwif_t;

typedef struct _DMA_CHN_HWIF_t {
    const HAL_ADDRESS_t base;
    const HAL_BASE_t irqn;
    const _dma_core_hwif_t *const core;
    const uint8_t chnum;
} _dma_chn_hwif_t;

typedef struct _DMA_CHN_STATE_t{
    EBS_BOOL_t armed;
    void (*cb_tc)(void);
    void (*cb_htc)(void);
} dma_chn_state_t;

typedef struct DMA_CHN_IF_t {
    const _dma_chn_hwif_t hwif;
    dma_chn_state_t * const state;
} dma_chn_if_t;

typedef struct DMA_XFER_t {
    HAL_ADDRESS_t src;
    HAL_ADDRESS_t dst;
    HAL_BASE_t size;
    DMA_TRANSFER_TYPE_t type;
    uint8_t src_opts;
    uint8_t dst_opts;
    uint8_t src_burst_len;
    uint8_t dst_burst_len;
    uint16_t dst_offset_incr;
    uint16_t src_offset_incr;
    uint8_t repeats;
} dma_xfer_t;

#define DMA_OPT_AP          0x10    // Access Port
#define DMA_OPT_DBX         0x01    // Dest Byte Exchange
#define DMA_OPT_DHX         0x02    // Dest Half Word Exchange
#define DMA_OPT_DWX         0x04    // Dest Word Exchange
#define DMA_OPT_SBX         0x08    // Source Byte Exchange
#define DMA_OPT_PAM_MSK     0x06    // Padding Mode
#define DMA_OPT_PAM_POS     1U
#define DMA_OPT_WIDTH_MSK   0xC0    // Width        
#define DMA_OPT_WIDTH_POS   6U

#define DMA_SRC_OPTS_MSK    0x1F
#define DMA_SRC_OPTS_POS    0U
#define DMA_DST_OPTS_MSK    0x1F
#define DMA_DST_OPTS_POS    0U

#define DMA_TR1_SRC_OPTS_POS    10U
#define DMA_TR1_DST_OPTS_POS    26U

typedef struct DMA_FLOW_CONTROL_t {
    DMA_REQUEST_TYPE_t req_type;
    uint8_t opts;
    uint8_t trig_sel;
    uint8_t req_sel;
}   dma_flow_control_t;

#define DMA_FC_TRIG_POL_MSK     0x0C
#define DMA_FC_TRIG_POL_POS     2U
#define DMA_FC_TRIG_MODE_MSK    0x03
#define DMA_FC_TRIG_MODE_POS    0U
#define DMA_FC_EVENT_MODE_MSK   0x30
#define DMA_FC_EVENT_MODE_POS   4U
#define DMA_FC_REQ_TYPE_MSK     0x40
#define DMA_FC_REQ_TYPE_POS     6U
// #define DMA_FC_REQ_DIR_MSK      0x80
// #define DMA_FC_REQ_DIR_POS      7

#define DMA_OPT_TRIG_RISING     0x04
#define DMA_OPT_TRIG_FALLING    0x08    
#define DMA_OPT_REQ_BLOCK       0x40
#define DMA_OPT_REQ_BURST       0x00
// #define DMA_OPT_REQ_DST         0x80
// #define DMA_OPT_REQ_SRC         0x00

#define OFS_DMA_PRIVCFGR    0x004
#define OFS_DMA_RCFGLOCKR   0x008
#define OFS_DMA_MISR        0x00C
#define OFS_DMA_SMISR       0x010   // GPDMA only

#define OFS_DMA_Cx_LBAR     0x00
#define OFS_DMA_Cx_FCR      0x0C
#define OFS_DMA_Cx_SR       0x010
#define OFS_DMA_Cx_CR       0x014
#define OFS_DMA_Cx_TR1      0x040
#define OFS_DMA_Cx_TR2      0x044
#define OFS_DMA_Cx_BR1      0x048
#define OFS_DMA_Cx_SAR      0x04C
#define OFS_DMA_Cx_DAR      0x050
#define OFS_DMA_Cx_TR3      0x054
#define OFS_DMA_Cx_BR2      0x058
#define OFS_DMA_Cx_LLR      0x07C

#define uC_DMA_DEFAULT_PRIORITY     DMA_PRIORITY_LOW

#ifndef uC_DMA1_CH0_PRIORITY
    #define uC_DMA1_CH0_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH1_PRIORITY
    #define uC_DMA1_CH1_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH2_PRIORITY
    #define uC_DMA1_CH2_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH3_PRIORITY
    #define uC_DMA1_CH3_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH4_PRIORITY
    #define uC_DMA1_CH4_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH5_PRIORITY
    #define uC_DMA1_CH5_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH6_PRIORITY
    #define uC_DMA1_CH6_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH7_PRIORITY
    #define uC_DMA1_CH7_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH8_PRIORITY
    #define uC_DMA1_CH8_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH9_PRIORITY
    #define uC_DMA1_CH9_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH10_PRIORITY
    #define uC_DMA1_CH11_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH12_PRIORITY
    #define uC_DMA1_CH12_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH13_PRIORITY
    #define uC_DMA1_CH13_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH14_PRIORITY
    #define uC_DMA1_CH14_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA1_CH15_PRIORITY
    #define uC_DMA1_CH15_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif

#ifndef uC_DMA2_CH0_PRIORITY
    #define uC_DMA2_CH0_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH1_PRIORITY
    #define uC_DMA2_CH1_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH2_PRIORITY
    #define uC_DMA2_CH2_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH3_PRIORITY
    #define uC_DMA2_CH3_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH4_PRIORITY
    #define uC_DMA2_CH4_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH5_PRIORITY    
    #define uC_DMA2_CH5_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH6_PRIORITY
    #define uC_DMA2_CH6_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH7_PRIORITY
    #define uC_DMA2_CH7_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH8_PRIORITY
    #define uC_DMA2_CH8_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH9_PRIORITY
    #define uC_DMA2_CH9_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH10_PRIORITY
    #define uC_DMA2_CH11_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH12_PRIORITY
    #define uC_DMA2_CH12_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH13_PRIORITY
    #define uC_DMA2_CH13_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH14_PRIORITY
    #define uC_DMA2_CH14_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif
#ifndef uC_DMA2_CH15_PRIORITY
    #define uC_DMA2_CH15_PRIORITY    uC_DMA_DEFAULT_PRIORITY
#endif


extern const dma_chn_if_t dma_chn_if[];


void dma_init(void);

void dma_setup_xfer(HAL_BASE_t intfnum, dma_xfer_t * xfer);

void dma_setup_flow_control(HAL_BASE_t intfnum, dma_flow_control_t * fc);

void dma_suspend_channel(HAL_BASE_t intfnum);

void dma_resume_channel(HAL_BASE_t intfnum);

void dma_trigger_arm(HAL_BASE_t intfnum);

void dma_register_cb_tc(HAL_BASE_t intfnum, void (*cb)(void));

void dma_register_cb_htc(HAL_BASE_t intfnum, void (*cb)(void));

void dma_memcpy(HAL_BASE_t intfnum, void * to, void * from, HAL_BASE_t size, void (*cb)(void));

void dma_check_error(HAL_BASE_t intfnum);

void dma_poll(void);

static inline EBS_BOOL_t dma_check_ready(HAL_BASE_t intfnum);

static inline void dma_trigger(HAL_BASE_t intfnum);

static inline EBS_BOOL_t dma_check_ready(HAL_BASE_t intfnum){
    uint32_t sr = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_SR);
    return (sr & DMA_CSR_IDLEF) ? EBS_TRUE : EBS_FALSE;
}

static inline void dma_trigger(HAL_BASE_t intfnum){
    HAL_SFR_t * cr = (HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    *cr |= DMA_CCR_EN;
}


#endif
#endif