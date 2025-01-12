
// ADV 16b 4CC DMA UpDown
#ifdef APP_ENABLE_TIM1   
    #define uC_TIM1_ENABLED     APP_ENABLE_TIM1   
#else
    #define uC_TIM1_ENABLED     0
#endif

// GP 32b 4CC DMA
#ifdef APP_ENABLE_TIM2
    #define uC_TIM2_ENABLED     APP_ENABLE_TIM2   
#else
    #define uC_TIM2_ENABLED     0
#endif

// GP 32b 4CC DMA
#ifdef APP_ENABLE_TIM3
    #define uC_TIM3_ENABLED     APP_ENABLE_TIM3   
#else
    #define uC_TIM3_ENABLED     0
#endif

// GP 32b 4CC DMA   
#ifdef APP_ENABLE_TIM4
    #define uC_TIM4_ENABLED     APP_ENABLE_TIM4   
#else
    #define uC_TIM4_ENABLED     0
#endif

// GP 32b 4CC DMA
#ifdef APP_ENABLE_TIM5
    #define uC_TIM5_ENABLED     APP_ENABLE_TIM5   
#else
    #define uC_TIM5_ENABLED     0
#endif

// Basic 16b 0CC DMA
#ifdef APP_ENABLE_TIM6
    #define uC_TIM6_ENABLED     APP_ENABLE_TIM6   
#else
    #define uC_TIM6_ENABLED     0
#endif

// Basic 16b 0CC DMA
#ifdef APP_ENABLE_TIM7
    #define uC_TIM7_ENABLED     APP_ENABLE_TIM7   
#else
    #define uC_TIM7_ENABLED     0
#endif

// GP 16b 2CC
#ifdef APP_ENABLE_TIM9
    #define uC_TIM9_ENABLED     APP_ENABLE_TIM9   
#else
    #define uC_TIM9_ENABLED     0
#endif

// GP 16b 2CC
#ifdef APP_ENABLE_TIM12
    #define uC_TIM12_ENABLED    APP_ENABLE_TIM12  
#else
    #define uC_TIM12_ENABLED    0
#endif

// GP 16b 1CC
#ifdef APP_ENABLE_TIM13
    #define uC_TIM13_ENABLED    APP_ENABLE_TIM13  
#else
    #define uC_TIM13_ENABLED    0
#endif

// GP 16b 1CC
#ifdef APP_ENABLE_TIM14
    #define uC_TIM14_ENABLED    APP_ENABLE_TIM14  
#else
    #define uC_TIM14_ENABLED    0
#endif

// GP 16b 2CC DMA
#ifdef APP_ENABLE_TIM15
    #define uC_TIM15_ENABLED    APP_ENABLE_TIM15  
#else
    #define uC_TIM15_ENABLED    0
#endif

// GP 16b 1CC DMA
#ifdef APP_ENABLE_TIM16
    #define uC_TIM16_ENABLED    APP_ENABLE_TIM16  
#else
    #define uC_TIM16_ENABLED    0
#endif

// GP 16b 1CC DMA
#ifdef APP_ENABLE_TIM17
    #define uC_TIM17_ENABLED    APP_ENABLE_TIM17  
#else
    #define uC_TIM17_ENABLED    0
#endif
    
#define uC_TIMERS_ENABLED   ( \
    uC_TIM2_ENABLED + uC_TIM3_ENABLED + uC_TIM4_ENABLED + uC_TIM5_ENABLED + \
    uC_TIM6_ENABLED + uC_TIM7_ENABLED + \
    uC_TIM9_ENABLED + uC_TIM12_ENABLED + uC_TIM13_ENABLED + uC_TIM14_ENABLED + \
    uC_TIM15_ENABLED + uC_TIM16_ENABLED + uC_TIM17_ENABLED \
)
    
#if uC_TIMERS_ENABLED
    #define uC_TIMER_ENABLED        1
#endif

// LP 16b 2CC DMA
#ifdef APP_ENABLE_LPTIM1
    #define uC_LPTIM1_ENABLED   APP_ENABLE_LPTIM1  
#else
    #define uC_LPTIM1_ENABLED   0
#endif

// LP 16b 2CC DMA
#ifdef APP_ENABLE_LPTIM2
    #define uC_LPTIM2_ENABLED   APP_ENABLE_LPTIM2  
#else
    #define uC_LPTIM2_ENABLED   0
#endif

// LP 16b 2CC DMA
#ifdef APP_ENABLE_LPTIM3
    #define uC_LPTIM3_ENABLED   APP_ENABLE_LPTIM3  
#else
    #define uC_LPTIM3_ENABLED   0
#endif

// LP 16b 2CC DMA
#ifdef APP_ENABLE_LPTIM4
    #define uC_LPTIM4_ENABLED   APP_ENABLE_LPTIM4  
#else
    #define uC_LPTIM4_ENABLED   0
#endif

// LP 16b 2CC DMA
#ifdef APP_ENABLE_LPTIM5
    #define uC_LPTIM5_ENABLED   APP_ENABLE_LPTIM5  
#else
    #define uC_LPTIM5_ENABLED   0
#endif

#define uC_LPTIMERS_ENABLED   ( \
    uC_LPTIM1_ENABLED + uC_LPTIM2_ENABLED + uC_LPTIM3_ENABLED + \
    uC_LPTIM4_ENABLED + uC_LPTIM4_ENABLED \
)

#if uC_LPTIMERS_ENABLED
    #define uC_LPTIMER_ENABLED      1
#endif

typedef enum {
    #if uC_TIM1_ENABLED
        uC_TIM1_INTFNUM,
    #endif
    #if uC_TIM2_ENABLED
        uC_TIM2_INTFNUM,
    #endif
    #if uC_TIM3_ENABLED
        uC_TIM3_INTFNUM,
    #endif
    #if uC_TIM4_ENABLED
        uC_TIM4_INTFNUM,
    #endif
    #if uC_TIM5_ENABLED
        uC_TIM5_INTFNUM,
    #endif
    #if uC_TIM6_ENABLED
        uC_TIM6_INTFNUM,
    #endif
    #if uC_TIM7_ENABLED
        uC_TIM7_INTFNUM,
    #endif
    #if uC_TIM9_ENABLED
        uC_TIM9_INTFNUM,
    #endif
    #if uC_TIM12_ENABLED
        uC_TIM12_INTFNUM,
    #endif
    #if uC_TIM13_ENABLED
        uC_TIM13_INTFNUM,
    #endif
    #if uC_TIM14_ENABLED
        uC_TIM14_INTFNUM,
    #endif
    #if uC_TIM15_ENABLED
        uC_TIM15_INTFNUM,
    #endif
    #if uC_TIM16_ENABLED
        uC_TIM16_INTFNUM,
    #endif
    #if uC_TIM17_ENABLED
        uC_TIM17_INTFNUM,
    #endif
} uC_TIMER_INTFNUM_t;
