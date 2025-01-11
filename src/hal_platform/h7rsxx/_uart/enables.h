

#ifdef APP_ENABLE_UART1
    #define uC_UART1_ENABLED            APP_ENABLE_UART1
    #ifdef APP_ENABLE_UART1_PT
        #define uC_UART1_PT_ENABLED     APP_ENABLE_UART1_PT
    #else
        #define uC_UART1_PT_ENABLED     0
    #endif
#else
    #define uC_UART1_ENABLED            0
    #define uC_UART1_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART2
    #define uC_UART2_ENABLED            APP_ENABLE_UART2
    #ifdef APP_ENABLE_UART2_PT
        #define uC_UART2_PT_ENABLED     APP_ENABLE_UART2_PT
    #else
        #define uC_UART2_PT_ENABLED     0
    #endif
#else
    #define uC_UART2_ENABLED            0
    #define uC_UART2_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART3
    #define uC_UART3_ENABLED            APP_ENABLE_UART3
    #ifdef APP_ENABLE_UART3_PT
        #define uC_UART3_PT_ENABLED     APP_ENABLE_UART3_PT
    #else
        #define uC_UART3_PT_ENABLED     0
    #endif
#else
    #define uC_UART3_ENABLED            0
    #define uC_UART3_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART4
    #define uC_UART4_ENABLED            APP_ENABLE_UART4
    #ifdef APP_ENABLE_UART4_PT
        #define uC_UART4_PT_ENABLED     APP_ENABLE_UART4_PT
    #else
        #define uC_UART4_PT_ENABLED     0
    #endif
#else
    #define uC_UART4_ENABLED            0
    #define uC_UART4_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART5
    #define uC_UART5_ENABLED            APP_ENABLE_UART5
    #ifdef APP_ENABLE_UART5_PT
        #define uC_UART5_PT_ENABLED     APP_ENABLE_UART5_PT
    #else
        #define uC_UART5_PT_ENABLED     0
    #endif
#else
    #define uC_UART5_ENABLED            0
    #define uC_UART5_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART6
    #define uC_UART6_ENABLED            APP_ENABLE_UART6
    #ifdef APP_ENABLE_UART6_PT
        #define uC_UART6_PT_ENABLED     APP_ENABLE_UART6_PT
    #else
        #define uC_UART6_PT_ENABLED     0
    #endif
#else
    #define uC_UART6_ENABLED            0
    #define uC_UART6_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART7
    #define uC_UART7_ENABLED            APP_ENABLE_UART7
    #ifdef APP_ENABLE_UART7_PT
        #define uC_UART7_PT_ENABLED     APP_ENABLE_UART7_PT
    #else
        #define uC_UART7_PT_ENABLED     0
    #endif
#else
    #define uC_UART7_ENABLED            0
    #define uC_UART7_PT_ENABLED         0
#endif

#ifdef APP_ENABLE_UART8
    #define uC_UART8_ENABLED            APP_ENABLE_UART8
    #ifdef APP_ENABLE_UART8_PT
        #define uC_UART8_PT_ENABLED     APP_ENABLE_UART8_PT
    #else
        #define uC_UART8_PT_ENABLED     0
    #endif
#else
    #define uC_UART8_ENABLED            0
    #define uC_UART8_PT_ENABLED         0
#endif

#define uC_UARTS_ENABLED            (uC_UART1_ENABLED + uC_UART2_ENABLED + uC_UART3_ENABLED + uC_UART4_ENABLED + uC_UART5_ENABLED + uC_UART6_ENABLED + uC_UART7_ENABLED + uC_UART8_ENABLED)
#define uC_UART_PTS_ENABLED         (uC_UART1_PT_ENABLED + uC_UART2_PT_ENABLED + uC_UART3_PT_ENABLED + uC_PT_UART4_ENABLED + uC_PT_UART5_ENABLED + uC_PT_UART6_ENABLED + uC_PT_UART7_ENABLED + uC_PT_UART8_ENABLED)

#if uC_UARTS_ENABLED
    #define uC_UART_ENABLED         1
#endif

#if uC_UART_PTS_ENABLED
    #define uC_UART_PT_ENABLED         1
#endif

#if uC_UART_ENABLED

typedef enum {
    #if uC_UART1_ENABLED
        uC_UART1_INTFNUM,
    #endif
    #if uC_UART2_ENABLED
        uC_UART2_INTFNUM,
    #endif
    #if uC_UART3_ENABLED
        uC_UART3_INTFNUM,
    #endif
    #if uC_UART4_ENABLED
        uC_UART4_INTFNUM,
    #endif
    #if uC_UART5_ENABLED
        uC_UART5_INTFNUM,
    #endif
    #if uC_UART6_ENABLED
        uC_UART6_INTFNUM,
    #endif
    #if uC_UART7_ENABLED
        uC_UART7_INTFNUM,
    #endif
    #if uC_UART8_ENABLED
        uC_UART8_INTFNUM,
    #endif
} uC_UART_INTFNUM_t;

#endif

#if uC_UART_PT_ENABLED

typedef enum {
    #if uC_UART1_PT_ENABLED
        uC_UART1_PT_INTFNUM,
    #endif
    #if uC_UART2_PT_ENABLED
        uC_UART2_PT_INTFNUM,
    #endif
    #if uC_UART3_PT_ENABLED
        uC_UART3_PT_INTFNUM,
    #endif
    #if uC_UART4_PT_ENABLED
        uC_UART4_PT_INTFNUM,
    #endif
    #if uC_UART5_PT_ENABLED
        uC_UART5_PT_INTFNUM,
    #endif
    #if uC_UART6_PT_ENABLED
        uC_UART6_PT_INTFNUM,
    #endif
    #if uC_UART7_PT_ENABLED
        uC_UART7_PT_INTFNUM,
    #endif
    #if uC_UART8_PT_ENABLED 
        uC_UART8_PT_INTFNUM,
    #endif
} uC_UART_PT_INTFNUM_t;

#endif