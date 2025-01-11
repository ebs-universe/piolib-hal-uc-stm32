

#define UART_DEFAULT_BAUD                 115200
#define UART_DEFAULT_PARITY               UART_PARITY_NONE
#define UART_DEFAULT_STOPBITS             UART_STOPBITS_1
#define UART_DEFAULT_WORDLENGTH           UART_WORDLENGTH_8B

#define UART_DEFAULT_ENDIANNESS           UART_ADVFEATURE_MSBFIRST_DISABLE
#define UART_DEFAULT_POLARITY_DATA        UART_ADVFEATURE_DATAINV_DISABLE
#define UART_DEFAULT_POLARITY_TX          UART_ADVFEATURE_TXINV_DISABLE          
#define UART_DEFAULT_POLARITY_RX          UART_ADVFEATURE_RXINV_DISABLE
#define UART_DEFAULT_SWAP_TXRX            UART_ADVFEATURE_SWAP_DISABLE

#define UART_DEFAULT_USE_HWFIFO           EBS_FALSE
#define UART_DEFAULT_TH_TX_HWFIFO         UART_TXFIFO_THRESHOLD_7_8
#define UART_DEFAULT_TH_RX_HWFIFO         UART_RXFIFO_THRESHOLD_1_8
#define UART_DEFAULT_HWF_TX_CSIZE         7
#define UART_DEFAULT_HWF_RX_CSIZE         1

#define UART_DEFAULT_OVERSAMPLING         UART_OVERSAMPLING_16
#define UART_DEFAULT_ONEBIT               UART_ONE_BIT_SAMPLE_DISABLE

#define UART_DEFAULT_TXBUF_LEN            255
#define UART_DEFAULT_RXBUF_LEN            255

#ifndef uC_UART1_BAUD
    #define uC_UART1_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART2_BAUD
    #define uC_UART2_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART3_BAUD
    #define uC_UART3_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART4_BAUD
    #define uC_UART4_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART5_BAUD
    #define uC_UART5_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART6_BAUD
    #define uC_UART6_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART7_BAUD
    #define uC_UART7_BAUD            UART_DEFAULT_BAUD
#endif
#ifndef uC_UART8_BAUD
    #define uC_UART8_BAUD            UART_DEFAULT_BAUD
#endif

#ifndef uC_UART1_PARITY
    #define uC_UART1_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART2_PARITY
    #define uC_UART2_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART3_PARITY
    #define uC_UART3_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART4_PARITY
    #define uC_UART4_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART5_PARITY
    #define uC_UART5_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART6_PARITY
    #define uC_UART6_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART7_PARITY
    #define uC_UART7_PARITY          UART_DEFAULT_PARITY
#endif
#ifndef uC_UART8_PARITY
    #define uC_UART8_PARITY          UART_DEFAULT_PARITY
#endif

#ifndef uC_UART1_STOPBITS
    #define uC_UART1_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART2_STOPBITS
    #define uC_UART2_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART3_STOPBITS
    #define uC_UART3_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART4_STOPBITS
    #define uC_UART4_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART5_STOPBITS
    #define uC_UART5_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART6_STOPBITS
    #define uC_UART6_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART7_STOPBITS
    #define uC_UART7_STOPBITS        UART_DEFAULT_STOPBITS
#endif
#ifndef uC_UART8_STOPBITS
    #define uC_UART8_STOPBITS        UART_DEFAULT_STOPBITS
#endif

#ifndef uC_UART1_WORDLENGTH
    #define uC_UART1_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART2_WORDLENGTH
    #define uC_UART2_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART3_WORDLENGTH
    #define uC_UART3_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART4_WORDLENGTH
    #define uC_UART4_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART5_WORDLENGTH
    #define uC_UART5_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART6_WORDLENGTH
    #define uC_UART6_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART7_WORDLENGTH
    #define uC_UART7_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif
#ifndef uC_UART8_WORDLENGTH
    #define uC_UART8_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
#endif

#ifndef uC_UART1_ENDIANNESS
    #define uC_UART1_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART2_ENDIANNESS
    #define uC_UART2_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART3_ENDIANNESS
    #define uC_UART3_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART4_ENDIANNESS
    #define uC_UART4_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART5_ENDIANNESS
    #define uC_UART5_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART6_ENDIANNESS
    #define uC_UART6_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART7_ENDIANNESS
    #define uC_UART7_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif
#ifndef uC_UART8_ENDIANNESS
    #define uC_UART8_ENDIANNESS      UART_DEFAULT_ENDIANNESS
#endif

#ifndef uC_UART1_POLARITY_DATA
    #define uC_UART1_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART2_POLARITY_DATA
    #define uC_UART2_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART3_POLARITY_DATA
    #define uC_UART3_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART4_POLARITY_DATA
    #define uC_UART4_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART5_POLARITY_DATA
    #define uC_UART5_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART6_POLARITY_DATA
    #define uC_UART6_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART7_POLARITY_DATA
    #define uC_UART7_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif
#ifndef uC_UART8_POLARITY_DATA
    #define uC_UART8_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
#endif

#ifndef uC_UART1_POLARITY_TX
    #define uC_UART1_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART2_POLARITY_TX
    #define uC_UART2_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART3_POLARITY_TX
    #define uC_UART3_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART4_POLARITY_TX
    #define uC_UART4_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART5_POLARITY_TX
    #define uC_UART5_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART6_POLARITY_TX
    #define uC_UART6_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART7_POLARITY_TX
    #define uC_UART7_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif
#ifndef uC_UART8_POLARITY_TX
    #define uC_UART8_POLARITY_TX     UART_DEFAULT_POLARITY_TX
#endif

#ifndef uC_UART1_POLARITY_RX
    #define uC_UART1_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART2_POLARITY_RX
    #define uC_UART2_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART3_POLARITY_RX
    #define uC_UART3_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART4_POLARITY_RX
    #define uC_UART4_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART5_POLARITY_RX
    #define uC_UART5_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART6_POLARITY_RX
    #define uC_UART6_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART7_POLARITY_RX
    #define uC_UART7_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif
#ifndef uC_UART8_POLARITY_RX
    #define uC_UART8_POLARITY_RX     UART_DEFAULT_POLARITY_RX
#endif

#ifndef uC_UART1_SWAP_TXRX
    #define uC_UART1_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART2_SWAP_TXRX
    #define uC_UART2_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART3_SWAP_TXRX
    #define uC_UART3_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART4_SWAP_TXRX
    #define uC_UART4_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART5_SWAP_TXRX
    #define uC_UART5_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART6_SWAP_TXRX
    #define uC_UART6_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART7_SWAP_TXRX
    #define uC_UART7_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif
#ifndef uC_UART8_SWAP_TXRX
    #define uC_UART3_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
#endif

#ifndef uC_UART1_USE_HWFIFO
    #define uC_UART1_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART2_USE_HWFIFO
    #define uC_UART2_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART3_USE_HWFIFO
    #define uC_UART3_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART4_USE_HWFIFO
    #define uC_UART4_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART5_USE_HWFIFO
    #define uC_UART5_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART6_USE_HWFIFO
    #define uC_UART6_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART7_USE_HWFIFO
    #define uC_UART7_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif
#ifndef uC_UART8_USE_HWFIFO
    #define uC_UART8_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
#endif

#ifndef uC_UART1_TH_TX_HWFIFO
    #define uC_UART1_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART2_TH_TX_HWFIFO
    #define uC_UART2_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART3_TH_TX_HWFIFO
    #define uC_UART3_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART4_TH_TX_HWFIFO
    #define uC_UART4_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART5_TH_TX_HWFIFO
    #define uC_UART5_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART6_TH_TX_HWFIFO
    #define uC_UART6_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART7_TH_TX_HWFIFO
    #define uC_UART7_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif
#ifndef uC_UART8_TH_TX_HWFIFO
    #define uC_UART8_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
#endif

#ifndef uC_UART1_TH_RX_HWFIFO
    #define uC_UART1_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART2_TH_RX_HWFIFO
    #define uC_UART2_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART3_TH_RX_HWFIFO
    #define uC_UART3_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART4_TH_RX_HWFIFO
    #define uC_UART4_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART5_TH_RX_HWFIFO
    #define uC_UART5_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART6_TH_RX_HWFIFO
    #define uC_UART6_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART7_TH_RX_HWFIFO
    #define uC_UART7_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif
#ifndef uC_UART8_TH_RX_HWFIFO
    #define uC_UART8_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
#endif

#ifndef uC_UART1_OVERSAMPLING
    #define uC_UART1_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART2_OVERSAMPLING
    #define uC_UART2_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART3_OVERSAMPLING
    #define uC_UART3_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART4_OVERSAMPLING
    #define uC_UART4_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART5_OVERSAMPLING
    #define uC_UART5_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART6_OVERSAMPLING
    #define uC_UART6_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART7_OVERSAMPLING
    #define uC_UART7_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif
#ifndef uC_UART8_OVERSAMPLING
    #define uC_UART8_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
#endif

#ifndef uC_UART1_ONEBIT
    #define uC_UART1_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART2_ONEBIT
    #define uC_UART2_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART3_ONEBIT
    #define uC_UART3_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART4_ONEBIT
    #define uC_UART4_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART5_ONEBIT
    #define uC_UART5_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART6_ONEBIT
    #define uC_UART6_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART7_ONEBIT
    #define uC_UART7_ONEBIT          UART_DEFAULT_ONEBIT
#endif
#ifndef uC_UART8_ONEBIT
    #define uC_UART8_ONEBIT          UART_DEFAULT_ONEBIT
#endif

#ifndef uC_UART1_TXBUF_LEN
    #define uC_UART1_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART2_TXBUF_LEN
    #define uC_UART2_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART3_TXBUF_LEN
    #define uC_UART3_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART4_TXBUF_LEN
    #define uC_UART4_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART5_TXBUF_LEN
    #define uC_UART5_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART6_TXBUF_LEN
    #define uC_UART6_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART7_TXBUF_LEN
    #define uC_UART7_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif
#ifndef uC_UART8_TXBUF_LEN
    #define uC_UART8_TXBUF_LEN       UART_DEFAULT_TXBUF_LEN
#endif

#ifndef uC_UART1_RXBUF_LEN
    #define uC_UART1_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART2_RXBUF_LEN
    #define uC_UART2_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART3_RXBUF_LEN
    #define uC_UART3_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART4_RXBUF_LEN
    #define uC_UART4_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART5_RXBUF_LEN
    #define uC_UART5_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART6_RXBUF_LEN
    #define uC_UART6_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART7_RXBUF_LEN
    #define uC_UART7_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
#ifndef uC_UART8_RXBUF_LEN
    #define uC_UART8_RXBUF_LEN       UART_DEFAULT_RXBUF_LEN
#endif
