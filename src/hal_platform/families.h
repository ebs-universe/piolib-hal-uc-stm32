

#ifndef __FAMILIES_H
#define __FAMILIES_H

#if defined STM32F4
    #define FAMILY f4xx
#elif defined STM32U0
    #define FAMILY u0xx
#elif defined STM32H7
    #define FAMILY h7xx
#else
    #error "STM32 family not recognized"
#endif

// Stringify macros to construct file paths dynamically
#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

#endif