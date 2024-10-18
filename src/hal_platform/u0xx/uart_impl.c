/* 
   Copyright (c) 
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : MSP430
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/



#include "uart_impl.h"

#if uC_UART_ENABLED

// TODO Instead of getting the actual bitfields from application, 
//      consider setting up standardized defaults in hal/constants/uart.
//      They can be handled inexpensively using static inline functions
//      with switches on the enum. It is not done now to avoid writing 
//      all that code and because we're not actually checking the 
//      generated assembly to ensure it is actually inexpensive.

static inline void _uart_conf_baud(
        const _uart_hwif_t *const hwif,
        uint32_t baudrate,
        uint32_t oversampling) 
{
    // PRESC : not used
    HAL_SFR_t * presc = (HAL_SFR_t *)(hwif->base + OFS_UART_PRESC);
    *presc = 0x0000;
    // BRR write the actual register
    HAL_SFR_t * brr = (HAL_SFR_t *)(hwif->base + OFS_UART_BRR);
    uint16_t usartdiv = CLOCKTREE_UART2_FREQ / baudrate;
    if (oversampling == UART_OVERSAMPLING_16){
        *brr = usartdiv;
    } else {
        *brr =  ((usartdiv & 0x000F) >> 1) | (usartdiv & 0xFFF0);
    }
}

static inline uint32_t _exp_uart_fifomode(EBS_BOOL_t fifomode){
    switch (fifomode)
    {
    case EBS_TRUE:
        return UART_FIFOMODE_ENABLE;
    case EBS_FALSE:
    default:
        return UART_FIFOMODE_DISABLE;
    }
}

static inline void _uart_init(
        const _uart_hwif_t *const hwif, 
        uint32_t wordlength,
        EBS_BOOL_t fifomode,
        uint32_t txfifo_threshold,
        uint32_t rxfifo_threshold,
        uint32_t stopbits,
        uint32_t baudrate,
        uint32_t oversampling,
        uint32_t onebit,
        uint32_t parity,
        uint32_t endianness,
        uint32_t polarity_data,
        uint32_t polarity_tx,
        uint32_t polarity_rx,
        uint32_t swap
        ) 
{
    _uart_conf_baud(hwif, baudrate, oversampling);

    HAL_SFR_t * cr1 = (HAL_SFR_t *)(hwif->base + OFS_UART_CR1);
    HAL_SFR_t * cr2 = (HAL_SFR_t *)(hwif->base + OFS_UART_CR2);
    HAL_SFR_t * cr3 = (HAL_SFR_t *)(hwif->base + OFS_UART_CR2);
    HAL_SFR_t * gtpr = (HAL_SFR_t *)(hwif->base + OFS_UART_GTPR);
    HAL_SFR_t * rtor = (HAL_SFR_t *)(hwif->base + OFS_UART_RTOR);
    HAL_SFR_t * rqr = (HAL_SFR_t *)(hwif->base + OFS_UART_RQR);

    
    *cr1 = 0x00000000;
    *cr2 = 0x00000000;
    *cr3 = 0x00000000;

    if (hwif->type == UART_HWIF_FULL){
        *gtpr = 0x00000000;  // unused
        *rtor = 0x00000000;  // unused
    }

    *rqr = 0x00000000; 
    
    /* CR1
     * Provided:
     *     wordlength | fifomode | oversampling | parity
     *     (ue=0)       (ue=0)     (ue=0)         (ue=0)
     * 
     * Impl Hardcoded Defaults:
     *     deat | dedt | mme | wake | uesm 
     * all set to disable / 0
     * 
     * Deferred:
     *     te | re | ue | all interrupt enables
     */    
    *cr1 = wordlength | oversampling | parity |
           _exp_uart_fifomode(fifomode) |
           (0x00000000 & USART_CR1_DEAT_Msk) | 
           (0x00000000 & USART_CR1_DEDT_Msk) |
           (0x00000000 & USART_CR1_MME_Msk)  | 
           (0x00000000 & USART_CR1_WAKE_Msk) |
           (0x00000000 & USART_CR1_UESM_Msk);

    /* CR2
     * 
     * (all (ue=0) unless otherwise specified)
     * 
     * Provided :
     *     stopbits | endianness | 
     *     polarity_data | polarity_tx | polarity_rx | swap
     *     
     * Impl Hardcoded Defaults:
     *     add | rtoen | abrmode | abren | linen | 
     *     clken | cpol | cpha | lbcl | lbdl | addm7 | 
     *     dis_nss | slven
     * 
     * all set to disable / 0
     */ 
    *cr2 = stopbits | endianness | polarity_data | polarity_tx | polarity_rx | swap |
           (0x00000000 & USART_CR2_ADD_Msk) | 
           (0x00000000 & USART_CR2_RTOEN_Msk) |
           (0x00000000 & USART_CR2_ABRMODE_Msk) |
           (0x00000000 & USART_CR2_ABREN_Msk) | 
           (0x00000000 & USART_CR2_LINEN_Msk) |
           (0x00000000 & USART_CR2_CLKEN_Msk) | 
           (0x00000000 & USART_CR2_CPOL_Msk) | 
           (0x00000000 & USART_CR2_CPHA_Msk) | 
           (0x00000000 & USART_CR2_LBCL_Msk) | 
           (0x00000000 & USART_CR2_LBDL_Msk) |
           (0x00000000 & USART_CR2_ADDM7_Msk) |
           (0x00000000 & USART_CR2_DIS_NSS_Msk) |
           (0x00000000 & USART_CR2_SLVEN_Msk);

    /* CR3
     * 
     * (all (ue=0) unless otherwise specified)
     * 
     * Provided:
     *     onebit | txfifo_threshold | rxfifo_threshold 
     *              (fifomode)         (fifomode)
     * 
     * Impl Hardcoded Defaults:
     *     wus | scarcnt | dep | dem | ddre | ovrdis | ctse | rtse | 
     *     dmat | dmar | scen | nack | hdsel | irlp | iren
     * all set to disable / 0
     * 
     */    
    *cr3 = onebit | 
           (0x00000000 & USART_CR3_WUS_Msk) |
           (0x00000000 & USART_CR3_SCARCNT_Msk) |
           (0x00000000 & USART_CR3_DEP_Msk) |
           (0x00000000 & USART_CR3_DEM_Msk) |
           (0x00000000 & USART_CR3_DDRE_Msk) | 
           (0x00000000 & USART_CR3_OVRDIS_Msk) |
           (0x00000000 & USART_CR3_CTSE_Msk) |
           (0x00000000 & USART_CR3_RTSE_Msk) |
           (0x00000000 & USART_CR3_DMAT_Msk) |
           (0x00000000 & USART_CR3_DMAR_Msk) |
           (0x00000000 & USART_CR3_SCEN_Msk) |
           (0x00000000 & USART_CR3_NACK_Msk) |
           (0x00000000 & USART_CR3_HDSEL_Msk) |
           (0x00000000 & USART_CR3_IRLP_Msk) |
           (0x00000000 & USART_CR3_IREN_Msk);

    if (fifomode){
        *cr3 |= txfifo_threshold | rxfifo_threshold;
    } 

    // Final Enable
    *cr1 |= USART_CR1_UE_Msk | USART_CR1_TE | USART_CR1_RE;

    /*
     * Interrupt Enables
     * 
     * Enable | Status | Availability | Description
     * -------|--------|--------------|-----------------------------------
     * RXFFIE | Unused | FULL+LP fifo | RX FIFO Completely Full 
     * TXFEIE | Unused | FULL+LP fifo | TX FIFO Completely Empty
     * EOBIE  | Unused | FULL         | Smartcard Mode
     * RTOIE  | Unused | FULL         | Reciever Timeout
     * CMIE   | Unused | ALL (?)      | RX Single Character Match
     * PEIE   | Unused | ALL          | RX Parity Error
     * TXFNFIE| Unused | FULL+LP fifo | TX FIFO has atleast one space
     * TXEIE  |  nrml  | FULL+LP nrml | TX SR empty 
     * RXFNEIE| Unused | FULL+LP fifo | RX FIFO has atleast one byte
     * RXNEIE |  nrml  | FULL+LP nrml | RX SR has data
     * TCIE   |  fifo  | ALL          | TX Complete. TX FIFO and SR empty 
     * IDLEIE | Unused | ALL          | Idle Line Detected
     * LBDIE  | Unused | FULL         | LIN mode
     * RXFTIE |  fifo  | FULL fifo    | RX FIFO Threshold
     * TCBGTIE| Unused | FULL         | Smartcard Mode
     * TXFTIE |  fifo  | FULL fifo    | TX FIFO Threshold
     * WUFIE  | Unused | FULL+LP      | Wakeup 
     * CTSIE  | Unused | ALL          | Hardware Flow Control CTS
     * EIE    | Unused | ALL          | Various Errors
     * 
     */
    if (fifomode) {
        // Use FIFO threshold interrupts when using the HW FIFO
        *cr3 |= USART_CR3_TXFTIE | USART_CR3_RXFTIE; 
        *cr1 |= USART_CR1_RXNEIE;
        // This flag could signal when the TX is actually done.
        //*cr1 |= USART_CR1_TCIE;
    } else {
        // Use regular interrupts when not using the HW FIFO
        *cr1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
    }
}

#if uC_UART1_ENABLED

#endif

#if uC_UART2_ENABLED
    uint8_t uart2_txbuffer[uC_UART2_TXBUF_LEN + 1];
    uint8_t uart2_rxbuffer[uC_UART2_RXBUF_LEN + 1];
    bytebuf uart2_txbuf;
    bytebuf uart2_rxbuf;
  
    static const _uart_hwif_t _uart2_hwif = {
        UART_HWIF_FULL, uC_UART2_USE_HWFIFO, USART2_BASE 
    };
    
    uart_state_t uart2_state = {0};
    
    void _uart2_init(void);
    
    const uart_if_t uart2_if = {
        &_uart2_hwif,
        &uart2_state,
        &uart2_txbuf,
        &uart2_rxbuf,
        _uart2_init
    };

    static inline void _uart2_buffer_init(void){
        bytebuf_vInit(uart2_txbuffer, uC_UART2_TXBUF_LEN, &uart2_txbuf);
        bytebuf_vInit(uart2_rxbuffer, uC_UART2_RXBUF_LEN, &uart2_rxbuf);
    }

    void _uart2_init(void){
        _uart2_buffer_init();
        __HAL_RCC_USART2_CLK_ENABLE();
        _uart_init(
            uart2_if.hwif, 
            uC_UART2_WORDLENGTH,
            uC_UART2_USE_HWFIFO,
            uC_UART2_TH_TX_HWFIFO,
            uC_UART2_TH_RX_HWFIFO,
            uC_UART2_STOPBITS,
            uC_UART2_BAUD,
            uC_UART2_OVERSAMPLING,
            uC_UART2_ONEBIT,
            uC_UART2_PARITY,
            uC_UART2_ENDIANNESS,
            uC_UART2_POLARITY_DATA,
            uC_UART2_POLARITY_TX,
            uC_UART2_POLARITY_RX,
            uC_UART2_SWAP_TXRX
        );
        HAL_NVIC_SetPriority(USART2_LPUART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_LPUART2_IRQn);
    }

#endif

#if uC_UART3_ENABLED

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

void uart_init(HAL_BASE_t intfnum){
    uart_if[intfnum]->init();
}

#endif