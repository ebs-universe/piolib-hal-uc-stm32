

#if uC_DMA1_CH0_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch0_state = {0};

    static inline void _dma1_ch0_init(void){
        _dma_chn_init(
            uC_DMA1_CH0_INTFNUM,
            uC_DMA1_CH0_PRIORITY,
            PRIO_DMA1_CH0,
            uC_DMA1_CH0_ENABLE_CB_TC,
            uC_DMA1_CH0_ENABLE_CB_HC
        );
    };

#endif      

#if uC_DMA1_CH1_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch1_state = {0};

    static inline void _dma1_ch1_init(void){
        _dma_chn_init(
            uC_DMA1_CH1_INTFNUM,
            uC_DMA1_CH1_PRIORITY,
            PRIO_DMA1_CH1,
            uC_DMA1_CH1_ENABLE_CB_TC,
            uC_DMA1_CH1_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH2_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch2_state = {0};

    static inline void _dma1_ch2_init(void){
        _dma_chn_init(
            uC_DMA1_CH2_INTFNUM,
            uC_DMA1_CH2_PRIORITY,
            PRIO_DMA1_CH2,
            uC_DMA1_CH2_ENABLE_CB_TC,
            uC_DMA1_CH2_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH3_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch3_state = {0};

    static inline void _dma1_ch3_init(void){
        _dma_chn_init(
            uC_DMA1_CH3_INTFNUM,
            uC_DMA1_CH3_PRIORITY,
            PRIO_DMA1_CH3,
            uC_DMA1_CH3_ENABLE_CB_TC,
            uC_DMA1_CH3_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH4_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch4_state = {0};

    static inline void _dma1_ch4_init(void){
        _dma_chn_init(
            uC_DMA1_CH4_INTFNUM,
            uC_DMA1_CH4_PRIORITY,
            PRIO_DMA1_CH4,
            uC_DMA1_CH4_ENABLE_CB_TC,
            uC_DMA1_CH4_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH5_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch5_state = {0};

    static inline void _dma1_ch5_init(void){
        _dma_chn_init(
            uC_DMA1_CH5_INTFNUM,
            uC_DMA1_CH5_PRIORITY,
            PRIO_DMA1_CH5,
            uC_DMA1_CH5_ENABLE_CB_TC,
            uC_DMA1_CH5_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH6_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch6_state = {0};

    static inline void _dma1_ch6_init(void){
        _dma_chn_init(
            uC_DMA1_CH6_INTFNUM,
            uC_DMA1_CH6_PRIORITY,
            PRIO_DMA1_CH6,
            uC_DMA1_CH6_ENABLE_CB_TC,
            uC_DMA1_CH6_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH7_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch7_state = {0};

    static inline void _dma1_ch7_init(void){
        _dma_chn_init(
            uC_DMA1_CH7_INTFNUM,
            uC_DMA1_CH7_PRIORITY,
            PRIO_DMA1_CH7,
            uC_DMA1_CH7_ENABLE_CB_TC,
            uC_DMA1_CH7_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH8_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch8_state = {0};

    static inline void _dma1_ch8_init(void){
        _dma_chn_init(
            uC_DMA1_CH8_INTFNUM,
            uC_DMA1_CH8_PRIORITY,
            PRIO_DMA1_CH8,
            uC_DMA1_CH8_ENABLE_CB_TC,
            uC_DMA1_CH8_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH9_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch9_state = {0};

    static inline void _dma1_ch9_init(void){
        _dma_chn_init(
            uC_DMA1_CH9_INTFNUM,
            uC_DMA1_CH9_PRIORITY,
            PRIO_DMA1_CH9,
            uC_DMA1_CH9_ENABLE_CB_TC,
            uC_DMA1_CH9_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH10_ENABLED
    
    dma_chn_state_t FAS TDATA dma1_ch10_state = {0};

    static inline void _dma1_ch10_init(void){
        _dma_chn_init(
            uC_DMA1_CH10_INTFNUM,
            uC_DMA1_CH10_PRIORITY,
            PRIO_DMA1_CH10,
            uC_DMA1_CH10_ENABLE_CB_TC,
            uC_DMA1_CH10_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH11_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch11_state = {0};

    static inline void _dma1_ch11_init(void){
        _dma_chn_init(
            uC_DMA1_CH11_INTFNUM,
            uC_DMA1_CH11_PRIORITY,
            PRIO_DMA1_CH11,
            uC_DMA1_CH11_ENABLE_CB_TC,
            uC_DMA1_CH11_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH12_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch12_state = {0};

    static inline void _dma1_ch12_init(void){
        _dma_chn_init(
            uC_DMA1_CH12_INTFNUM,
            uC_DMA1_CH12_PRIORITY,
            PRIO_DMA1_CH12,
            uC_DMA1_CH12_ENABLE_CB_TC,
            uC_DMA1_CH12_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH13_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch13_state = {0};

    static inline void _dma1_ch13_init(void){
        _dma_chn_init(
            uC_DMA1_CH13_INTFNUM,
            uC_DMA1_CH13_PRIORITY,
            PRIO_DMA1_CH13,
            uC_DMA1_CH13_ENABLE_CB_TC,
            uC_DMA1_CH13_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH14_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch14_state = {0};

    static inline void _dma1_ch14_init(void){
        _dma_chn_init(
            uC_DMA1_CH14_INTFNUM,
            uC_DMA1_CH14_PRIORITY,
            PRIO_DMA1_CH14,
            uC_DMA1_CH14_ENABLE_CB_TC,
            uC_DMA1_CH14_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA1_CH15_ENABLED
    
    dma_chn_state_t FASTDATA dma1_ch15_state = {0};

    static inline void _dma1_ch15_init(void){
        _dma_chn_init(
            uC_DMA1_CH15_INTFNUM,
            uC_DMA1_CH15_PRIORITY,
            PRIO_DMA1_CH15,
            uC_DMA1_CH15_ENABLE_CB_TC,
            uC_DMA1_CH15_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH0_ENABLED

    dma_chn_state_t FASTDATA dma2_ch0_state = {0};

    static inline void _dma2_ch0_init(void){
        _dma_chn_init(
            uC_DMA2_CH0_INTFNUM,
            uC_DMA2_CH0_PRIORITY,
            PRIO_DMA2_CH0,
            uC_DMA2_CH0_ENABLE_CB_TC,
            uC_DMA2_CH0_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH1_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch1_state = {0};

    static inline void _dma2_ch1_init(void){
        _dma_chn_init(
            uC_DMA2_CH1_INTFNUM,
            uC_DMA2_CH1_PRIORITY,
            PRIO_DMA2_CH1,
            uC_DMA2_CH1_ENABLE_CB_TC,
            uC_DMA2_CH1_ENABLE_CB_HC
        );  
    };

#endif

#if uC_DMA2_CH2_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch2_state = {0};

    static inline void _dma2_ch2_init(void){
        _dma_chn_init(
            uC_DMA2_CH2_INTFNUM,
            uC_DMA2_CH2_PRIORITY,
            PRIO_DMA2_CH2,
            uC_DMA2_CH2_ENABLE_CB_TC,
            uC_DMA2_CH2_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH3_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch3_state = {0};

    static inline void _dma2_ch3_init(void){
        _dma_chn_init(
            uC_DMA2_CH3_INTFNUM,
            uC_DMA2_CH3_PRIORITY,
            PRIO_DMA2_CH3,
            uC_DMA2_CH3_ENABLE_CB_TC,
            uC_DMA2_CH3_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH4_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch4_state = {0};

    static inline void _dma2_ch4_init(void){
        _dma_chn_init(
            uC_DMA2_CH4_INTFNUM,
            uC_DMA2_CH4_PRIORITY,
            PRIO_DMA2_CH4,
            uC_DMA2_CH4_ENABLE_CB_TC,
            uC_DMA2_CH4_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH5_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch5_state = {0};

    static inline void _dma2_ch5_init(void){
        _dma_chn_init(
            uC_DMA2_CH5_INTFNUM,
            uC_DMA2_CH5_PRIORITY,
            PRIO_DMA2_CH5,
            uC_DMA2_CH5_ENABLE_CB_TC,
            uC_DMA2_CH5 _ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH6_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch6_state = {0};

    static inline void _dma2_ch6_init(void){
        _dma_chn_init(
            uC_DMA2_CH6_INTFNUM,
            uC_DMA2_CH6_PRIORITY,
            PRIO_DMA2_CH6,
            uC_DMA2_CH6_ENABLE_CB_TC,
            uC_DMA2_CH6_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH7_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch7_state = {0};

    static inline void _dma2_ch7_init(void){
        _dma_chn_init(
            uC_DMA2_CH7_INTFNUM,
            uC_DMA2_CH7_PRIORITY,
            PRIO_DMA2_CH7,
            uC_DMA2_CH7_ENABLE_CB_TC,
            uC_DMA2_CH7_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH8_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch8_state = {0};

    static inline void _dma2_ch8_init(void){
        _dma_chn_init(
            uC_DMA2_CH8_INTFNUM,
            uC_DMA2_CH8_PRIORITY,
            PRIO_DMA2_CH8,
            uC_DMA2_CH8_ENABLE_CB_TC,
            uC_DMA2_CH8_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH9_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch9_state = {0};

    static inline void _dma2_ch9_init(void){
        _dma_chn_init(
            uC_DMA2_CH9_INTFNUM,
            uC_DMA2_CH9_PRIORITY,
            PRIO_DMA2_CH9,
            uC_DMA2_CH9_ENABLE_CB_TC,
            uC_DMA2_CH9_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH10_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch10_state = {0};

    static inline void _dma2_ch10_init(void){
        _dma_chn_init(
            uC_DMA2_CH10_INTFNUM,
            uC_DMA2_CH10_PRIORITY,
            PRIO_DMA2_CH10,
            uC_DMA2_CH10_ENABLE_CB_TC,
            uC_DMA2_CH10_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH11_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch11_state = {0};

    static inline void _dma2_ch11_init(void){
        _dma_chn_init(
            uC_DMA2_CH11_INTFNUM,
            uC_DMA2_CH11_PRIORITY,
            PRIO_DMA2_CH11,
            uC_DMA2_CH11_ENABLE_CB_TC,
            uC_DMA2_CH11_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH12_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch12_state = {0};

    static inline void _dma2_ch12_init(void){
        _dma_chn_init(
            uC_DMA2_CH12_INTFNUM,
            uC_DMA2_CH12_PRIORITY,
            PRIO_DMA2_CH12,
            uC_DMA2_CH12_ENABLE_CB_TC,
            uC_DMA2_CH12_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH13_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch13_state = {0};

    static inline void _dma2_ch13_init(void){
        _dma_chn_init(
            uC_DMA2_CH13_INTFNUM,
            uC_DMA2_CH13_PRIORITY,
            PRIO_DMA2_CH13,
            uC_DMA2_CH13_ENABLE_CB_TC,
            uC_DMA2_CH13_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH14_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch14_state = {0};

    static inline void _dma2_ch14_init(void){
        _dma_chn_init(
            uC_DMA2_CH14_INTFNUM,
            uC_DMA2_CH14_PRIORITY,
            PRIO_DMA2_CH14,
            uC_DMA2_CH14_ENABLE_CB_TC,
            uC_DMA2_CH14_ENABLE_CB_HC
        );
    };

#endif

#if uC_DMA2_CH15_ENABLED
    
    dma_chn_state_t FASTDATA dma2_ch15_state = {0};

    static inline void _dma2_ch15_init(void){
        _dma_chn_init(
            uC_DMA2_CH15_INTFNUM,
            uC_DMA2_CH15_PRIORITY,
            PRIO_DMA2_CH15,
            uC_DMA2_CH15_ENABLE_CB_TC,
            uC_DMA2_CH15_ENABLE_CB_HC
        );
    };

#endif

const dma_chn_if_t dma_chn_if[] = {
    #if uC_DMA1_CH0_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel0_BASE,
                .irqn = HPDMA1_Channel0_IRQn,
                .core = &_dma1_hwif,
                .chnum = 0, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH0_TYPE,
            },
            .state = &dma1_ch0_state,
        },
    #endif
    #if uC_DMA1_CH1_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel1_BASE,
                .irqn = HPDMA1_Channel1_IRQn,
                .core = &_dma1_hwif,
                .chnum = 1, 
                .hwtype = DMA_HWIF_CHNTYPE_1
                .type = uC_DMA1_CH2_TYPE,
            },
            .state = &dma1_ch1_state,
        },
    #endif
    #if uC_DMA1_CH2_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel2_BASE,
                .irqn = HPDMA1_Channel2_IRQn,
                .core = &_dma1_hwif,
                .chnum = 2, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH2_TYPE,
            },
            .state = &dma1_ch2_state,
        },  
    #endif
    #if uC_DMA1_CH3_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel3_BASE,
                .irqn = HPDMA1_Channel3_IRQn,
                .core = &_dma1_hwif,
                .chnum = 3, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH3_TYPE,
            },
            .state = &dma1_ch3_state,
        },  
    #endif
    #if uC_DMA1_CH4_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel4_BASE,
                .irqn = HPDMA1_Channel4_IRQn,
                .core = &_dma1_hwif,
                .chnum = 4, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH4_TYPE,
            },
            .state = &dma1_ch4_state,
        },  
    #endif
    #if uC_DMA1_CH5_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel5_BASE,
                .irqn = HPDMA1_Channel5_IRQn,
                .core = &_dma1_hwif,
                .chnum = 5, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH5_TYPE
            },
            .state = &dma1_ch5_state,
        },  
    #endif
    #if uC_DMA1_CH6_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel6_BASE,
                .irqn = HPDMA1_Channel6_IRQn,
                .core = &_dma1_hwif,
                .chnum = 6, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH6_TYPE,
            },
            .state = &dma1_ch6_state,
        },  
    #endif
    #if uC_DMA1_CH7_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel7_BASE,
                .irqn = HPDMA1_Channel7_IRQn,
                .core = &_dma1_hwif,
                .chnum = 7, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH7_TYPE,
            },
            .state = &dma1_ch7_state,
        },  
    #endif
    #if uC_DMA1_CH8_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel8_BASE,
                .irqn = HPDMA1_Channel8_IRQn,
                .core = &_dma1_hwif,
                .chnum = 8, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH8_TYPE,
            },
            .state = &dma1_ch8_state,
        },  
    #endif
    #if uC_DMA1_CH9_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel9_BASE,
                .irqn = HPDMA1_Channel9_IRQn,
                .core = &_dma1_hwif,
                .chnum = 9, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH9_TYPE,
            },
            .state = &dma1_ch9_state,
        },  
    #endif
    #if uC_DMA1_CH10_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel10_BASE,
                .irqn = HPDMA1_Channel10_IRQn,
                .core = &_dma1_hwif,
                .chnum = 10, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH10_TYPE,
            },
            .state = &dma1_ch10_state,
        },  
    #endif
    #if uC_DMA1_CH11_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel11_BASE,
                .irqn = HPDMA1_Channel11_IRQn,
                .core = &_dma1_hwif,
                .chnum = 11, 
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA1_CH11_TYPE,
            },
            .state = &dma1_ch11_state,
        },  
    #endif
    #if uC_DMA1_CH12_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel12_BASE,
                .irqn = HPDMA1_Channel12_IRQn,
                .core = &_dma1_hwif,
                .chnum = 12, 
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA1_CH12_TYPE,
            },
            .state = &dma1_ch12_state,
        },  
    #endif
    #if uC_DMA1_CH13_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel13_BASE,
                .irqn = HPDMA1_Channel13_IRQn,
                .core = &_dma1_hwif,
                .chnum = 13, 
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA1_CH13_TYPE,
            },
            .state = &dma1_ch13_state,
        },  
    #endif
    #if uC_DMA1_CH14_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel14_BASE,
                .irqn = HPDMA1_Channel14_IRQn,
                .core = &_dma1_hwif,
                .chnum = 14, 
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA1_CH14_TYPE,
            },
            .state = &dma1_ch14_state,
        },  
    #endif
    #if uC_DMA1_CH15_ENABLED
        {
            .hwif = {       
                .base = HPDMA1_Channel15_BASE,
                .irqn = HPDMA1_Channel15_IRQn,
                .core = &_dma1_hwif,
                .chnum = 15, 
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA1_CH15_TYPE,
            },
            .state = &dma1_ch15_state,
        },  
    #endif
    #if uC_DMA2_CH0_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel0_BASE,
                .irqn = GPDMA1_Channel0_IRQn,
                .core = &_dma2_hwif,
                .chnum = 0,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH0_TYPE,
            },
            .state = &dma2_ch0_state,
        },
    #endif
    #if uC_DMA2_CH1_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel1_BASE,
                .irqn = GPDMA1_Channel1_IRQn,
                .core = &_dma2_hwif,
                .chnum = 1,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH1_TYPE,
            },
            .state = &dma2_ch1_state,
        },
    #endif
    #if uC_DMA2_CH2_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel2_BASE,
                .irqn = GPDMA1_Channel2_IRQn,
                .core = &_dma2_hwif,
                .chnum = 2,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH2_TYPE,
            },
            .state = &dma2_ch2_state,
        },
    #endif
    #if uC_DMA2_CH3_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel3_BASE,
                .irqn = GPDMA1_Channel3_IRQn,
                .core = &_dma2_hwif,
                .chnum = 3,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH3_TYPE,
            },
            .state = &dma2_ch3_state,
        },
    #endif
    #if uC_DMA2_CH4_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel4_BASE,
                .irqn = GPDMA1_Channel4_IRQn,
                .core = &_dma2_hwif,
                .chnum = 4,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH4_TYPE,
            },
            .state = &dma2_ch4_state,
        },
    #endif
    #if uC_DMA2_CH5_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel5_BASE,
                .irqn = GPDMA1_Channel5_IRQn,
                .core = &_dma2_hwif,
                .chnum = 5,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH5_TYPE,
            },
            .state = &dma2_ch5_state,
        },
    #endif
    #if uC_DMA2_CH6_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel6_BASE,
                .irqn = GPDMA1_Channel6_IRQn,
                .core = &_dma2_hwif,
                .chnum = 6,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH6_TYPE,
            },
            .state = &dma2_ch6_state,
        },
    #endif
    #if uC_DMA2_CH7_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel7_BASE,
                .irqn = GPDMA1_Channel7_IRQn,
                .core = &_dma2_hwif,
                .chnum = 7,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH7_TYPE,
            },
            .state = &dma2_ch7_state,
        },
    #endif
    #if uC_DMA2_CH8_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel8_BASE,
                .irqn = GPDMA1_Channel8_IRQn,
                .core = &_dma2_hwif,
                .chnum = 8,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH8_TYPE,
            },
            .state = &dma2_ch8_state,
        },
    #endif
    #if uC_DMA2_CH9_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel9_BASE,
                .irqn = GPDMA1_Channel9_IRQn,
                .core = &_dma2_hwif,
                .chnum = 9,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH9_TYPE,
            },
            .state = &dma2_ch9_state,
        },
    #endif
    #if uC_DMA2_CH10_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel10_BASE,
                .irqn = GPDMA1_Channel10_IRQn,
                .core = &_dma2_hwif,
                .chnum = 10,
                .hwtype = DMA_HWIF_CHNTYPE_1,
                .type = uC_DMA2_CH10_TYPE,
            },
            .state = &dma2_ch10_state,
        },
    #endif
    #if uC_DMA2_CH11_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel11_BASE,
                .irqn = GPDMA1_Channel11_IRQn,
                .core = &_dma2_hwif,
                .chnum = 11,
                .hwtype = DMA_HWIF_CHNTYPE_1
                .type = uC_DMA2_CH11_TYPE,
            },
            .state = &dma2_ch11_state,
        },
    #endif
    #if uC_DMA2_CH12_ENABLED
        {
            .hwif = {
                .base = GPDMA1_Channel12_BASE,
                .irqn = GPDMA1_Channel12_IRQn,
                .core = &_dma2_hwif,
                .chnum = 12,
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA2_CH12_TYPE,
            },
            .state = &dma2_ch12_state,
        },
    #endif
    #if uC_DMA2_CH13_ENABLED
        {
            .hwif = {       
                .base = GPDMA1_Channel13_BASE,
                .irqn = GPDMA1_Channel13_IRQn,
                .core = &_dma2_hwif,
                .chnum = 13,
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA2_CH13_TYPE,
            },
            .state = &dma2_ch13_state,
        },
    #endif
    #if uC_DMA2_CH14_ENABLED
        {
            .hwif = {       
                .base = GPDMA1_Channel14_BASE,
                .irqn = GPDMA1_Channel14_IRQn,
                .core = &_dma2_hwif,
                .chnum = 14,
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA2_CH14_TYPE,
            },
            .state = &dma2_ch14_state,
        },
    #endif
    #if uC_DMA2_CH15_ENABLED
        {
            .hwif = {       
                .base = GPDMA1_Channel15_BASE,
                .irqn = GPDMA1_Channel15_IRQn,
                .core = &_dma2_hwif,
                .chnum = 15,
                .hwtype = DMA_HWIF_CHNTYPE_2,
                .type = uC_DMA2_CH15_TYPE,
            },
            .state = &dma2_ch15_state,
        },
    #endif      
};    
