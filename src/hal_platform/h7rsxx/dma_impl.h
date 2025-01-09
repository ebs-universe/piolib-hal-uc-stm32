

#ifndef DMA_IMPL_H
#define DMA_IMPL_H

#include <hal/uc/dma.h>
#include <platform/sections.h>

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
 * 
 * Implementation Assumptions : 
 * 
 *  - Channels are either entirely linear or entirely circular
 *  - Channels perpetually have the same mode
 *  - Circular channels are continuous and perpetual. Callback is 
 *    called after completion of each LLI.
 *  - Linear channels operate in short LL sequences, callbacks called 
 *    after the LL is fully executed.       
 *  - Interrupt support is not implemented yet
 *  - Linked list is always used
 */

#if uC_DMA_ENABLED 

typedef enum {
    DMA_HWIF_HPDMA,
    DMA_HWIF_GPDMA,         
} DMA_HWIF_TYPE;

typedef enum {
    DMA_HWIF_CHNTYPE_1,
    DMA_HWIF_CHNTYPE_2,
} DMA_CHN_HWTYPE;

typedef struct DMA_LLI_t {
    uint32_t tr1;   
    uint32_t tr2;
    uint32_t br1;
    uint32_t sar;
    uint32_t dar;
    uint32_t llr;
    EBS_BOOL_t in_use: 1;
    uint8_t token;
    void (*cb_tc)(uint8_t);
    void (*cb_hc)(uint8_t);
} __attribute__((aligned(4))) dma_lli_t;    

typedef struct _DMA_CORE_HWIF_t{
    const HAL_ADDRESS_t base;
    const DMA_HWIF_TYPE type;
} _dma_core_hwif_t;

typedef struct _DMA_CHN_HWIF_t {
    const HAL_ADDRESS_t base;
    const HAL_BASE_t irqn;
    const _dma_core_hwif_t *const core;
    const DMA_CHN_HWTYPE hwtype;
    const DMA_CHN_TYPE_t type;
    const uint8_t chnum;
} _dma_chn_hwif_t;  

typedef struct _DMA_CHN_STATE_t{
    EBS_BOOL_t use_int;
    dma_lli_t * ll_active;      // Current linked list
    dma_lli_t * ll_pending;     // Linked List for the next group of transfers
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
    uint8_t repeats;
} dma_xfer_t;

#define DMA_OPT_AP              0x10    // Access Port
#define DMA_OPT_DBX             0x01    // Dest Byte Exchange
#define DMA_OPT_DHX             0x02    // Dest Half Word Exchange
#define DMA_OPT_DWX             0x04    // Dest Word Exchange
#define DMA_OPT_SBX             0x08    // Source Byte Exchange
#define DMA_OPT_PAM_MSK         0x06    // Padding Mode
#define DMA_OPT_PAM_POS         1U
#define DMA_OPT_WIDTH_MSK       0xC0    // Width        
#define DMA_OPT_WIDTH_POS       6U

#define DMA_OPT_WIDTH_1B        (0b00 << DMA_OPT_WIDTH_POS)
#define DMA_OPT_WIDTH_2B        (0b01 << DMA_OPT_WIDTH_POS)
#define DMA_OPT_WIDTH_4B        (0b10 << DMA_OPT_WIDTH_POS)
#define DMA_OPT_WIDTH_8B        (0b11 << DMA_OPT_WIDTH_POS)

#define DMA_SRC_OPTS_MSK        0x1F
#define DMA_SRC_OPTS_POS        0U
#define DMA_DST_OPTS_MSK        0x1F
#define DMA_DST_OPTS_POS        0U

#define DMA_TR1_SRC_OPTS_POS    10U
#define DMA_TR1_DST_OPTS_POS    26U

typedef struct DMA_FLOW_CONTROL_t {
    DMA_REQUEST_TYPE_t req_type;
    uint8_t opts;
    uint8_t trig_sel;
    uint8_t req_sel;
    void (*cb_tc)(uint8_t);
    void (*cb_hc)(uint8_t);
} dma_flow_control_t;

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

#define DMA_FC_OPT_EVTMODE_ITEM    (0b10 << DMA_FC_EVENT_MODE_POS)
#define DMA_FC_OPT_EVTMODE_LIST    (0b11 << DMA_FC_EVENT_MODE_POS)
#define DMA_FC_OPT_TRIG_RISING     0x04
#define DMA_FC_OPT_TRIG_FALLING    0x08     
#define DMA_FC_OPT_REQ_BLOCK       0x40
#define DMA_FC_OPT_REQ_BURST       0x00
// #define DMA_OPT_REQ_DST         0x80 
// #define DMA_OPT_REQ_SRC         0x00

#include "_dma/registers.h"

#define uC_DMA_DEFAULT_PRIORITY         DMA_PRIORITY_LOW
#define uC_DMA_DEFAULT_ENABLE_CB_TC     EBS_TRUE
#define uC_DMA_DEFAULT_ENABLE_CB_HC     EBS_FALSE
#define uC_DMA_DEFAULT_CHANNEL_TYPE     DMA_CHN_TYPE_LINEAR

#include "_dma/channel_defaults.h"

extern const dma_chn_if_t dma_chn_if[];

void dma_init(void);

void dma_enqueue(HAL_BASE_t intfnum, dma_lli_t * lli);

dma_lli_t* dma_setup_xfer(HAL_BASE_t intfnum, dma_xfer_t * xfer, dma_flow_control_t * fc, uint8_t token);

void dma_suspend_channel(HAL_BASE_t intfnum);

void dma_resume_channel(HAL_BASE_t intfnum);

void dma_activate(HAL_BASE_t intfnum);

void dma_poll(void);

static inline EBS_BOOL_t dma_check_ready(HAL_BASE_t intfnum);

/**
 * @name DMA Helper Functions
 * 
 * The application would generally be expected to implement these function or 
 * something similar for each DMA use, configuring the specifics of the DMA 
 * transfer. The HAL constants and structures allow a small degree of 
 * standardization to be had, but in general expect these to be highly 
 * platform dependent.  
 * 
 * The functions here are provided for convenience to maintain some small
 * degree of standardization for non-critical uses. Note that these functions
 * are likely not optimal. 
 * 
 * It is unclear if the use of the DMA abstraction layer is of any real value. 
 * The standardization provided by these functions comes at the expense of a
 * significant performance hit.
 * 
 */
/**@{*/ 

EBS_BOOL_t _memmap_check_tcm(HAL_ADDRESS_t ptr);
    
void dma_memcpy(HAL_BASE_t intfnum, void * to, void * from, HAL_BASE_t size, void (*cb)(uint8_t), uint8_t token);

/**@}*/ 

static inline EBS_BOOL_t dma_check_ready(HAL_BASE_t intfnum){
    uint32_t cr = *(HAL_SFR_t *)(dma_chn_if[intfnum].hwif.base + OFS_DMA_Cx_CR);
    return (cr & DMA_CCR_EN) ? EBS_FALSE : EBS_TRUE;
}

// // When the transfer is done, use this ptr from the TC interrupt 
// // to execute any callbacks and clear in_use as appropriate
// if (dma_chn_if[intfnum].state->ll_pending) {
//     dma_chn_if[intfnum].state->ll_active = dma_chn_if[intfnum].state->ll_pending;
//     dma_chn_if[intfnum].state->ll_pending = NULL;
// }


#endif
#endif