

#ifndef uC_TIMER_DEFAULT_CLK_FREQ
  #define uC_TIMER_DEFAULT_CLK_FREQ    100000000ULL
#endif

#define uC_TIMER_DEFAULT_BUFFERED_TOP  EBS_TRUE


#ifndef uC_TIM15_CLK_FREQ
    #define uC_TIM15_CLK_FREQ           uC_TIMER_DEFAULT_CLK_FREQ
#endif

#ifndef uC_TIM15_PRESCALER
    #define uC_TIM15_PRESCALER          (CLOCKTREE_TIM15_FREQ / uC_TIM15_CLK_FREQ)
#endif
    
#ifndef uC_TIM15_INTERRUPTS_ENABLE
    #define uC_TIM15_INTERRUPTS_ENABLE     0
#endif
    
#ifndef uC_TIM15_INTERRUPT_TOP_ENABLE
    #define uC_TIM15_INTERRUPT_TOP_ENABLE  0
#endif

#ifndef uC_TIM15_CYCLE_FREQ 
    #define uC_TIM15_CYCLE_FREQ       100000
#endif

#ifndef uC_TIM15_CYCLE_TOP
    #define uC_TIM15_CYCLE_TOP        (uC_TIM15_CLK_FREQ / uC_TIM15_CYCLE_FREQ)
#endif

#ifndef uC_TIM15_BUFFERED_TOP
    #define uC_TIM15_BUFFERED_TOP     uC_TIMER_DEFAULT_BUFFERED_TOP
#endif

#ifndef uC_TIM15_CH1_OUTPUT_MODE
    #define uC_TIM15_CH1_OUTPUT_MODE  TIMER_OUTMODE_NONE
#endif

#ifndef uC_TIM15_CH2_OUTPUT_MODE
    #define uC_TIM15_CH2_OUTPUT_MODE  TIMER_OUTMODE_NONE
#endif

#ifndef uC_TIM15_TRIGGEN_SRC
    #define uC_TIM15_TRIGGEN_SRC      TIMER_TRIGGEN_NONE
#endif
