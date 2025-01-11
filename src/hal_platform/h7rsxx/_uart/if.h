

#if uC_UART1_ENABLED

#endif

#if uC_UART2_ENABLED

#endif

#if uC_UART3_ENABLED
    uint8_t uart3_txbuffer[uC_UART3_TXBUF_LEN + 1] = {0};
    uint8_t uart3_rxbuffer[uC_UART3_RXBUF_LEN + 1] = {0};
    bytebuf uart3_txbuf = {0};
    bytebuf uart3_rxbuf = {0};

    static const _uart_hwif_t _uart3_hwif = {
        UART_HWIF_FULL, uC_UART3_USE_HWFIFO, USART3_BASE 
    };
    
    uart_state_t uart3_state = {0};
    
    void _uart3_init(void);
    
    const uart_if_t uart3_if = {
        &_uart3_hwif,
        &uart3_state,
        &uart3_txbuf,
        &uart3_rxbuf,
        _uart3_init
    };

    static inline void _uart3_buffer_init(void){
        bytebuf_vInit(uart3_txbuffer, uC_UART3_TXBUF_LEN, &uart3_txbuf);
        bytebuf_vInit(uart3_rxbuffer, uC_UART3_RXBUF_LEN, &uart3_rxbuf);
    }

    static inline void _uart3_clock_init(void){
        RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
        PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_PLL3Q;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { die(); }
        __HAL_RCC_USART3_CLK_ENABLE();
    }

    void _uart3_init(void){
        _uart3_buffer_init();
        _uart3_clock_init();
        _uart_init(
            uart3_if.hwif, 
            CLOCKTREE_UART3_FREQ,
            uC_UART3_WORDLENGTH,
            uC_UART3_USE_HWFIFO,
            uC_UART3_TH_TX_HWFIFO,
            uC_UART3_TH_RX_HWFIFO,
            uC_UART3_STOPBITS,
            uC_UART3_BAUD,
            uC_UART3_OVERSAMPLING,
            uC_UART3_ONEBIT,
            uC_UART3_PARITY,
            uC_UART3_ENDIANNESS,
            uC_UART3_POLARITY_DATA,
            uC_UART3_POLARITY_TX,
            uC_UART3_POLARITY_RX,
            uC_UART3_SWAP_TXRX
        );
        HAL_NVIC_SetPriority(USART3_IRQn, PRIO_USART3, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
#endif

#if uC_UART4_ENABLED

#endif

#if uC_UART5_ENABLED

#endif

#if uC_UART6_ENABLED

#endif

#if uC_UART7_ENABLED

#endif

const uart_if_t *const uart_if[uC_UARTS_ENABLED + 1] = {
    #if uC_UART1_ENABLED
        &uart1_if,
    #endif
    #if uC_UART2_ENABLED
        &uart2_if,  
    #endif
    #if uC_UART3_ENABLED
        &uart3_if,
    #endif
    #if uC_UART4_ENABLED
        &uart4_if,
    #endif
    #if uC_UART5_ENABLED
        &uart5_if,
    #endif
    #if uC_UART6_ENABLED
        &uart6_if,
    #endif
    #if uC_UART7_ENABLED
        &uart7_if,
    #endif
    NULL
};
