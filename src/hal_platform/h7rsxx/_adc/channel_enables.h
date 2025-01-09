
#ifdef APP_ENABLE_AIN0          
    #define uC_ENABLE_AIN0          APP_ENABLE_AIN0
#else
    #define uC_ENABLE_AIN0          0
#endif

#ifdef APP_ENABLE_AIN1          
    #define uC_ENABLE_AIN1          APP_ENABLE_AIN1
#else
    #define uC_ENABLE_AIN1          0
#endif

#ifdef APP_ENABLE_AIN2          
    #define uC_ENABLE_AIN2          APP_ENABLE_AIN2
#else
    #define uC_ENABLE_AIN2          0
#endif

#ifdef APP_ENABLE_AIN3          
    #define uC_ENABLE_AIN3          APP_ENABLE_AIN3
#else
    #define uC_ENABLE_AIN3          0
#endif

#ifdef APP_ENABLE_AIN4
    #define uC_ENABLE_AIN4          APP_ENABLE_AIN4
#else
    #define uC_ENABLE_AIN4          0
#endif

#ifdef APP_ENABLE_AIN5
    #define uC_ENABLE_AIN5          APP_ENABLE_AIN5
#else
    #define uC_ENABLE_AIN5          0
#endif

#ifdef APP_ENABLE_AIN6
    #define uC_ENABLE_AIN6          APP_ENABLE_AIN6
#else
    #define uC_ENABLE_AIN6          0
#endif

#ifdef APP_ENABLE_AIN7
    #define uC_ENABLE_AIN7          APP_ENABLE_AIN7
#else
    #define uC_ENABLE_AIN7          0
#endif

#ifdef APP_ENABLE_AIN8
    #define uC_ENABLE_AIN8          APP_ENABLE_AIN8
#else
    #define uC_ENABLE_AIN8          0
#endif

#ifdef APP_ENABLE_AIN9
    #define uC_ENABLE_AIN9          APP_ENABLE_AIN9
#else
    #define uC_ENABLE_AIN9          0
#endif

#ifdef APP_ENABLE_AIN10
    #define uC_ENABLE_AIN10         APP_ENABLE_AIN10
#else
    #define uC_ENABLE_AIN10         0
#endif

#ifdef APP_ENABLE_AIN11
    #define uC_ENABLE_AIN11         APP_ENABLE_AIN11
#else
    #define uC_ENABLE_AIN11         0
#endif

#ifdef APP_ENABLE_AIN12
    #define uC_ENABLE_AIN12         APP_ENABLE_AIN12
#else
    #define uC_ENABLE_AIN12         0
#endif

#ifdef APP_ENABLE_AIN13
    #define uC_ENABLE_AIN13         APP_ENABLE_AIN13
#else
    #define uC_ENABLE_AIN13         0
#endif

#ifdef APP_ENABLE_AIN14
    #define uC_ENABLE_AIN14         APP_ENABLE_AIN14
#else
    #define uC_ENABLE_AIN14         0
#endif

#ifdef APP_ENABLE_AIN15
    #define uC_ENABLE_AIN15         APP_ENABLE_AIN15
#else
    #define uC_ENABLE_AIN15         0
#endif

#ifdef APP_ENABLE_AIN18
    #define uC_ENABLE_AIN18         APP_ENABLE_AIN18
#else
    #define uC_ENABLE_AIN18         0
#endif

#ifdef APP_ENABLE_AIN_TEMP
    #define uC_ENABLE_AIN_TEMP      APP_ENABLE_AIN_TEMP
#else
    #define uC_ENABLE_AIN_TEMP      0
#endif

#ifdef APP_ENABLE_AIN_IREF
    #define uC_ENABLE_AIN_IREF      APP_ENABLE_AIN_IREF
#else
    #define uC_ENABLE_AIN_IREF      0
#endif

#ifdef APP_ENABLE_AIN_VBAT
    #define uC_ENABLE_AIN_VBAT      APP_ENABLE_AIN_VBAT
#else
    #define uC_ENABLE_AIN_IREF      0
#endif

#ifdef APP_ENABLE_AIN_VDDC
    #define uC_ENABLE_AIN_VDDC      APP_ENABLE_AIN_VDDC
#else
    #define uC_ENABLE_AIN_VDDC      0
#endif

#define uC_ADC1_CHNMASK ((uC_ENABLE_AIN0 == 1 ? (1U << 0) : 0) | \
                (uC_ENABLE_AIN1 == 1 ? (1U << 1) : 0) | \
                (uC_ENABLE_AIN2 == 1 ? (1U << 2) : 0) | \
                (uC_ENABLE_AIN3 == 1 ? (1U << 3) : 0) | \
                (uC_ENABLE_AIN4 == 1 ? (1U << 4) : 0) | \
                (uC_ENABLE_AIN5 == 1 ? (1U << 5) : 0) | \
                (uC_ENABLE_AIN6 == 1 ? (1U << 6) : 0) | \
                (uC_ENABLE_AIN7 == 1 ? (1U << 7) : 0) | \
                (uC_ENABLE_AIN8 == 1 ? (1U << 8) : 0) | \
                (uC_ENABLE_AIN9 == 1 ? (1U << 9) : 0) | \
                (uC_ENABLE_AIN10 == 1 ? (1U << 10) : 0) | \
                (uC_ENABLE_AIN11 == 1 ? (1U << 11) : 0) | \
                (uC_ENABLE_AIN12 == 1 ? (1U << 12) : 0) | \
                (uC_ENABLE_AIN13 == 1 ? (1U << 13) : 0) | \
                (uC_ENABLE_AIN14 == 1 ? (1U << 14) : 0) | \
                (uC_ENABLE_AIN15 == 1 ? (1U << 15) : 0) | \
                (uC_ENABLE_AIN_TEMP == 1 ? (1U << 16) : 0) | \
                (uC_ENABLE_AIN_IREF == 1 ? (1U << 17) : 0) | \
                (uC_ENABLE_AIN18 == 1 ? (1U << 18) : 0))

#define uC_ADC2_CHNMASK ((uC_ENABLE_AIN0 == 2 ? (1U << 0) : 0) | \
                (uC_ENABLE_AIN1 == 2 ? (1U << 1) : 0) | \
                (uC_ENABLE_AIN2 == 2 ? (1U << 2) : 0) | \
                (uC_ENABLE_AIN3 == 2 ? (1U << 3) : 0) | \
                (uC_ENABLE_AIN4 == 2 ? (1U << 4) : 0) | \
                (uC_ENABLE_AIN5 == 2 ? (1U << 5) : 0) | \
                (uC_ENABLE_AIN6 == 2 ? (1U << 6) : 0) | \
                (uC_ENABLE_AIN7 == 2 ? (1U << 7) : 0) | \
                (uC_ENABLE_AIN8 == 2 ? (1U << 8) : 0) | \
                (uC_ENABLE_AIN9 == 2 ? (1U << 9) : 0) | \
                (uC_ENABLE_AIN10 == 2 ? (1U << 10) : 0) | \
                (uC_ENABLE_AIN11 == 2 ? (1U << 11) : 0) | \
                (uC_ENABLE_AIN12 == 2 ? (1U << 12) : 0) | \
                (uC_ENABLE_AIN13 == 2 ? (1U << 13) : 0) | \
                (uC_ENABLE_AIN14 == 2 ? (1U << 14) : 0) | \
                (uC_ENABLE_AIN15 == 2 ? (1U << 15) : 0) | \
                (uC_ENABLE_AIN_VBAT == 2 ? (1U << 16) : 0) | \
                (uC_ENABLE_AIN_VDDC == 2 ? (1U << 17) : 0) | \
                (uC_ENABLE_AIN18 == 2 ? (1U << 18) : 0))
