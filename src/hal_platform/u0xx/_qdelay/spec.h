

#ifndef QDELAY_SPEC_H
#define QDELAY_SPEC_H

#if uC_QDELAY_PROVIDER == QDELAY_PROVIDER_CPU
    #error "qdelay provider not implemented"
    #define uC_QDELAY_NONBLOCKING       0
#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_SYSTICK
    #define uC_QDELAY_NONBLOCKING       1
    #define uC_QDELAY_TICKCOUNT_t       uint32_t
#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_TIMER
    #error "qdelay provider not implemented"
    #define uC_QDELAY_NONBLOCKING       1
    #define uC_QDELAY_TICKCOUNT_t       uint16_t
#elif uC_QDELAY_PROVIDER == QDELAY_PROVIDER_DWT
    #error "qdelay provider not implemented"
    #define uC_QDELAY_NONBLOCKING       1
    #define uC_QDELAY_TICKCOUNT_t       uint16_t
#else
    #error "qdelay provider not recognized"
#endif

#endif

