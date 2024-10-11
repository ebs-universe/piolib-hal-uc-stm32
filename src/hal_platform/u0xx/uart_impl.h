/* 
    Copyright (c)
     (c) 2024 Chintalagiri Shashank
   
    This file is part of
    Embedded bootstraps : Peripheral driver implementations : STM32
   
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

/*
 * Implementation Notes
 *
 * Wordlength and Parity
 * ---------------------
 * 
 * This implementation presently requires the application to correctly 
 * set the wordlength, compensating for the parity bit, if parity is enabled.
 * The defaults work fine for 8bit word, no parity. However, if parity is 
 * to be enabled, the application must configure the wordlength to 9 bit for
 * what is actually 8 bit data. 
 * 
 * This may be improved at a later date.
 * 
 * GPIO Configuration
 * ------------------
 * 
 * This implementation does not do anything to configure GPIO. Needed GPIO
 * configuration should be done by the application, typically in bsp/gpio.
 * 
 * Using the Hardware FIFO
 * -----------------------
 * 
 * Some basic implementation has been tried, but it does not work. Pure 
 * continuous tx without rx seems to work. rx through fifo threshold seems 
 * to work only while the tx is going on.
 * 
 * The implementation tries to use the normal FIFO interrupts, and while
 * rx itself works, tx does not seem to start under these circumstances.
 * 
 * I'm giving up on this for the moment until there is a robust use case 
 * for the hardware fifo. 
 * 
 * Configuring for Speed 
 * ---------------------
 * 
 * The default implementation prioritizes reliability. If speed is required 
 * instead, the following changes may be considered:
 *  - Enable the HW FIFO.
 *  - Reduce oversampling to 8
 *  - Consider use of the DMA. Doing so with the ebs bytebuf 
 *    implementation will be quite difficult, though, so this may need
 *    to be reserved for interfaces which do not expect an ebs bytebuf.
 * 
 * Bit Majority / One Bit
 * ----------------------
 * 
 * By default, we use majority vote to deal with potentially noisy 
 * environments. However, this reduces tolerance to clock deviations. 
 * This is a less clear tradeoff to make. This default is based on 
 * the assumption that bauds rate and crystals / clocks will be 
 * matched to generate accurate baud rates on both sides. 
 * 
 * At present, we don't actually do anything if the three measurements
 * are not equal. The hardware simply provides us with the majority.
 * 
 */

#ifndef UART_IMPL_H
#define UART_IMPL_H

#include <hal/uc/uart.h>
#include <hal/types/uart.h>
#include <ds/bytebuf.h>

#if uC_UART_ENABLED

typedef enum {
    UART_HWIF_BASIC,
    UART_HWIF_FULL,
    UART_HWIF_LP
} UART_HWIF_TYPE;

typedef struct _UART_HWIF_t{
    const UART_HWIF_TYPE type;
    const EBS_BOOL_t fifomode;
    const HAL_ADDRESS_t base;
} _uart_hwif_t;

typedef struct _UART_STATE_t{
    HAL_BASE_t triggered;
    uint16_t overrun_counter;
} uart_state_t;

typedef struct UART_IF_t {
    const _uart_hwif_t * const hwif;
    uart_state_t * const state;
    bytebuf * const txbuf;
    bytebuf * const rxbuf;
    void (*const init)(void);
} uart_if_t;

#ifndef CLOCKTREE_PCLK_FREQ     
#define CLOCKTREE_PCLK_FREQ     56000000UL     
#endif

#ifndef CLOCKTREE_UART2_FREQ 
#define CLOCKTREE_UART2_FREQ    56000000UL  
#endif

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

#define OFS_UART_CR1    0x00
#define OFS_UART_CR2    0x04
#define OFS_UART_CR3    0x08
#define OFS_UART_BRR    0x0C
#define OFS_UART_GTPR   0x10    // Not for LPUART
#define OFS_UART_RTOR   0x14    // Not for LPUART
#define OFS_UART_RQR    0x18
#define OFS_UART_ISR    0x1C
#define OFS_UART_ICR    0x20
#define OFS_UART_RDR    0x24
#define OFS_UART_TDR    0x28
#define OFS_UART_PRESC  0x2C


extern const uart_if_t *const uart_if[];

#if uC_UART1_ENABLED
    extern bytebuf uart1_txbuf;
    extern bytebuf uart1_rxbuf;
    extern const uart_if_t uart1_if;
#endif

#define CONCAT(prefix, num) prefix##num

#if uC_UART2_ENABLED
    extern bytebuf uart2_txbuf;
    extern bytebuf uart2_rxbuf;
    extern const uart_if_t uart2_if;

    #ifndef uC_UART2_BAUD
    #define uC_UART2_BAUD            UART_DEFAULT_BAUD
    #endif

    #ifndef uC_UART2_PARITY
    #define uC_UART2_PARITY          UART_DEFAULT_PARITY
    #endif

    #ifndef uC_UART2_STOPBITS
    #define uC_UART2_STOPBITS        UART_DEFAULT_STOPBITS
    #endif

    #ifndef uC_UART2_WORDLENGTH
    #define uC_UART2_WORDLENGTH      UART_DEFAULT_WORDLENGTH          
    #endif

    #ifndef uC_UART2_ENDIANNESS
    #define uC_UART2_ENDIANNESS      UART_DEFAULT_ENDIANNESS
    #endif

    #ifndef uC_UART2_POLARITY_DATA
    #define uC_UART2_POLARITY_DATA   UART_DEFAULT_POLARITY_DATA
    #endif

    #ifndef uC_UART2_POLARITY_TX
    #define uC_UART2_POLARITY_TX     UART_DEFAULT_POLARITY_TX
    #endif

    #ifndef uC_UART2_POLARITY_RX
    #define uC_UART2_POLARITY_RX     UART_DEFAULT_POLARITY_RX
    #endif

    #ifndef uC_UART2_SWAP_TXRX
    #define uC_UART2_SWAP_TXRX       UART_DEFAULT_SWAP_TXRX
    #endif

    #ifndef uC_UART2_USE_HWFIFO
    #define uC_UART2_USE_HWFIFO      UART_DEFAULT_USE_HWFIFO          
    #endif

    #ifndef uC_UART2_TH_TX_HWFIFO
    #define uC_UART2_TH_TX_HWFIFO    UART_DEFAULT_TH_TX_HWFIFO
    #endif

    #ifndef uC_UART2_TH_RX_HWFIFO
    #define uC_UART2_TH_RX_HWFIFO    UART_DEFAULT_TH_RX_HWFIFO
    #endif

    #ifndef uC_UART2_OVERSAMPLING
    #define uC_UART2_OVERSAMPLING    UART_DEFAULT_OVERSAMPLING
    #endif

    #ifndef uC_UART2_ONEBIT
    #define uC_UART2_ONEBIT          UART_DEFAULT_ONEBIT
    #endif
    
#endif

#if uC_UART3_ENABLED
    extern bytebuf uart3_txbuf;
    extern bytebuf uart3_rxbuf;
    extern const uart_if_t uart3_if;
#endif

#if uC_UART4_ENABLED
    extern bytebuf uart4_txbuf;
    extern bytebuf uart4_rxbuf;
    extern const uart_if_t uart4_if;
#endif

#if uC_UART5_ENABLED
    extern bytebuf uart5_txbuf;
    extern bytebuf uart5_rxbuf;
    extern const uart_if_t uart5_if;
#endif

#if uC_UART6_ENABLED
    extern bytebuf uart6_txbuf;
    extern bytebuf uart6_rxbuf;
    extern const uart_if_t uart6_if;
#endif

#if uC_UART7_ENABLED
    extern bytebuf uart7_txbuf;
    extern bytebuf uart7_rxbuf;
    extern const uart_if_t uart7_if;
#endif

static inline HAL_BASE_t uart_putc(HAL_BASE_t intfnum, uint8_t byte, uint8_t token, HAL_BASE_t handlelock){
    uint8_t stat=1;
    if (handlelock){
        stat = bytebuf_cPushReqLock(uart_if[intfnum]->txbuf, 1, token);
    }
    if (stat){
        stat = bytebuf_cPushByte(uart_if[intfnum]->txbuf, byte, token);
        uart_send_trigger(intfnum);
        return stat;
    }
    return 0;
}

static inline HAL_BASE_t uart_write(HAL_BASE_t intfnum, uint8_t * buffer, HAL_BASE_t len, uint8_t token){
    uint8_t rval;
    rval = bytebuf_cPushLen(uart_if[intfnum]->txbuf, (void *)buffer, len, token);
    uart_send_trigger(intfnum);
    return rval;
}

static inline uint8_t uart_getc(HAL_BASE_t intfnum){
    return bytebuf_cPopByte(uart_if[intfnum]->rxbuf);
}

static inline HAL_BASE_t uart_read(HAL_BASE_t intfnum, uint8_t * buffer, HAL_BASE_t len){
    return bytebuf_cPopLen(uart_if[intfnum]->rxbuf, (void *)buffer, len);
}
    
static inline HAL_BASE_t uart_population_rxb(HAL_BASE_t intfnum){
    return bytebuf_cPopulation(uart_if[intfnum]->rxbuf);
}

static inline void uart_discard_rxb(HAL_BASE_t intfnum){
    bytebuf_vFlush(uart_if[intfnum]->rxbuf);
}

static inline void uart_send_trigger(HAL_BASE_t intfnum){
    if (!(uart_if[intfnum]->state->triggered)){
        if (uart_if[intfnum]->hwif->fifomode){
            * (HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_CR3) |= USART_CR3_TXFTIE;
        } else {
            * (HAL_SFR_t *) (uart_if[intfnum]->hwif->base + OFS_UART_CR1) |= USART_CR1_TXEIE;
        }
        uart_if[intfnum]->state->triggered = 1;
    }
}

static inline void uart_send_flush(HAL_BASE_t intfnum){
    uart_send_trigger(intfnum);
}

static inline HAL_BASE_t uart_reqlock(HAL_BASE_t intfnum, HAL_BASE_t len, uint8_t token){
    if (len){
        return bytebuf_cPushReqLock(uart_if[intfnum]->txbuf, len, token);
    }
    else{
        return bytebuf_cPushReqBlindLock(uart_if[intfnum]->txbuf, token);
    }
}

static inline HAL_BASE_t uart_rellock(HAL_BASE_t intfnum, uint8_t token){
    return bytebuf_cPushRelinquishLock(uart_if[intfnum]->txbuf, token);
}

static inline void uart_putc_bare(HAL_BASE_t intfnum, uint8_t byte){
    HAL_SFR_t * tdr = (HAL_SFR_t *)(uart_if[intfnum]->hwif->base + OFS_UART_TDR);
    *tdr = byte;
}

static inline uint8_t uart_getc_bare(HAL_BASE_t intfnum){
    HAL_SFR_t * rdr = (HAL_SFR_t *)(uart_if[intfnum]->hwif->base + OFS_UART_RDR);
    return *rdr & 0xFF;
}    

#endif
#endif
