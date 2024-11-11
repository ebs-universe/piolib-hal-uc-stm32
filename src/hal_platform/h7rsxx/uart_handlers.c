
#include <application.h>
#include <irq_handlers.h>
#include <hal/uc/uart.h>
#include "uart_handlers.h"

volatile uint8_t __uart_handler_inclusion;

#if uC_UART_ENABLED

static inline void _uart_handler_tx_norm(const HAL_BASE_t intfnum);
static inline void _uart_handler_rx_norm(const HAL_BASE_t intfnum);

static inline void _uart_handler_tx_fifo(const HAL_BASE_t intfnum);
static inline void _uart_handler_rx_fifo(const HAL_BASE_t intfnum);

static inline void _uart_handler_norm(const HAL_BASE_t intfnum);
static inline void _uart_handler_fifo(const HAL_BASE_t intfnum);


static inline void _uart_handler_tx_norm(const HAL_BASE_t intfnum){
    // TXE in single byte mode
    uint8_t wbyte;
    if (bytebuf_cPopulation(uart_if[intfnum]->txbuf) <= 1){
        *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_CR1) &= ~USART_CR1_TXEIE;
    }
    if (bytebuf_cPopulation(uart_if[intfnum]->txbuf)) {
        wbyte = bytebuf_cPopByte(uart_if[intfnum]->txbuf);
        *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_TDR) = wbyte;
    }
}

static inline void _uart_handler_rx_norm(const HAL_BASE_t intfnum){
    // RXNE in single byte mode
    uint8_t rval; 
    rval = (uint8_t) *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_RDR);
    if (bytebuf_cGetFree(uart_if[intfnum]->rxbuf)){
        bytebuf_cPushByte(uart_if[intfnum]->rxbuf, rval, 0x00);
    }
    else{
        uart_if[intfnum]->state->overrun_counter++;
    }
}

static inline void _uart_handler_tx_fifo(const HAL_BASE_t intfnum){
    // TXFT in fifo mode
    // upto uC_UART2_HWF_TX_CSIZE bytes can be written
    uint8_t wbyte;
    uint8_t n = bytebuf_cPopulation(uart_if[intfnum]->txbuf);
    while (n && (*(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_ISR) & USART_ISR_TXE_TXFNF)){
        wbyte = bytebuf_cPopByte(uart_if[intfnum]->txbuf);
        *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_TDR) = wbyte;
        n--;
    }
    if (!n){
        *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_CR3) &= ~USART_CR3_TXFTIE;
    }
}

static inline void _uart_handler_rx_fifo(const HAL_BASE_t intfnum){
    // RXFT in fifo mode
    // Ideally, upto uC_UART2_HWF_RX_CSIZE bytes can be read.
    // This just does a byte by byte read, though, almost exactly like norm. 
    // In this case we read multiple bytes if they happen to exist in the 
    // FIFO, though in practice that should never actually happen.
    //
    // TODO RX FIFO Threshold Interrupt never seems to fire unless TX is also 
    //      going on. Unsure why.
    //
    // TODO FIFO RX needs all messages to be a multiple of CSIZE, which can
    //      never be guaranteed. If < CSIZE bytes remain at message end,
    //      the interrupt will never fire. Use RTO or something to handle 
    //      partial chunk reception, maybe? For the moment, we set CSIZE 
    //      to 1, which makes this whole fifo thing pointless. 
    //
    // TODO Overrun and various reception errors are not handled. 
    //  
    uint8_t rbyte;
    uint8_t n = bytebuf_cGetFree(uart_if[intfnum]->rxbuf);
    while (n && (*(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_ISR) & USART_ISR_RXNE_RXFNE)) {
        rbyte = (uint8_t) *(HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_RDR);
        bytebuf_cPushByte(uart_if[intfnum]->rxbuf, rbyte, 0x00);
        n--;
    }
}

static inline void _uart_handler_norm(const HAL_BASE_t intfnum){
    uint32_t flags = *(HAL_SFR_t *)(uart_if[intfnum]->hwif->base + OFS_UART_ISR);
    if (flags & USART_ISR_TXE_TXFNF){
        _uart_handler_tx_norm(intfnum);
    } 
    if (flags & USART_ISR_RXNE_RXFNE){
        _uart_handler_rx_norm(intfnum);
    }
}

static inline void _uart_handler_fifo(const HAL_BASE_t intfnum){
    uint32_t flags = *(HAL_SFR_t *)(uart_if[intfnum]->hwif->base + OFS_UART_ISR);
    if (flags & USART_ISR_TXFT){
        _uart_handler_tx_fifo(intfnum);
    } 
    if (flags & USART_ISR_RXFT){
        _uart_handler_rx_fifo(intfnum);
    }
}


void USART1_IRQHandler(void){
    #if uC_UART1_ENABLED
        #if uC_UART1_USE_HWFIFO
        _uart_handler_fifo(uC_UART1_INTFNUM);
        #else
        _uart_handler_norm(uC_UART1_INTFNUM);
        #endif
    #endif
}

void USART2_IRQHandler(void){
    #if uC_UART2_ENABLED
        #if uC_UART2_USE_HWFIFO
        _uart_handler_fifo(uC_UART2_INTFNUM);
        #else
        _uart_handler_norm(uC_UART2_INTFNUM);
        #endif
    #endif
}

void USART3_IRQHandler(void){
    #if uC_UART3_ENABLED
        #if uC_UART3_USE_HWFIFO
        _uart_handler_fifo(uC_UART3_INTFNUM);
        #else
        _uart_handler_norm(uC_UART3_INTFNUM);
        #endif
    #endif
}

void UART4_IRQHandler(void){
    #if uC_UART4_ENABLED
        #if uC_UART4_USE_HWFIFO
        _uart_handler_fifo(uC_UART4_INTFNUM);
        #else
        _uart_handler_norm(uC_UART4_INTFNUM);
        #endif
    #endif
}

void UART5_IRQHandler(void){
    #if uC_UART5_ENABLED
        #if uC_UART5_USE_HWFIFO
        _uart_handler_fifo(uC_UART5_INTFNUM);
        #else
        _uart_handler_norm(uC_UART5_INTFNUM);
        #endif
    #endif
}

void UART7_IRQHandler(void){
    #if uC_UART7_ENABLED
        #if uC_UART7_USE_HWFIFO
        _uart_handler_fifo(uC_UART7_INTFNUM);
        #else
        _uart_handler_norm(uC_UART7_INTFNUM);
        #endif
    #endif
}

void UART8_IRQHandler(void){
    #if uC_UART8_ENABLED
        #if uC_UART8_USE_HWFIFO
        _uart_handler_fifo(uC_UART8_INTFNUM);
        #else
        _uart_handler_norm(uC_UART8_INTFNUM);
        #endif
    #endif
}

void LPUART1_IRQHandler(void){
    #if uC_UART6_ENABLED
        #if uC_UART6_USE_HWFIFO
        _uart_handler_fifo(uC_UART6_INTFNUM);
        #else
        _uart_handler_norm(uC_UART6_INTFNUM);
        #endif
    #endif
}

#endif