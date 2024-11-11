

// These register names look more like F4xx than U0xx, though the offsets 
// are all different. We use the F4xx handler implementation as a base. If  
// there is a problem, check against datasheet. 

#define EXTI_OFS_RTSR1   0x00    /*!< EXTI Rising trigger selection register 1  */
#define EXTI_OFS_FTSR1   0x04    /*!< EXTI Falling trigger selection register 1 */
#define EXTI_OFS_SWIER1  0x08    /*!< EXTI Software interrupt event register 1  */
#define EXTI_OFS_RTSR2   0x20    /*!< EXTI Rising trigger selection register 2  */
#define EXTI_OFS_FTSR2   0x24    /*!< EXTI Falling trigger selection register 2 */
#define EXTI_OFS_SWIER2  0x28    /*!< EXTI Software interrupt event register 2  */

#define EXTI_OFS_IMR1    0x80    /*!< EXTI Interrupt mask register 1            */
#define EXTI_OFS_EMR1    0x84    /*!< EXTI Event mask register 1                */
#define EXTI_OFS_PR1     0x88    /*!< EXTI Pending register 1                   */
#define EXTI_OFS_IMR2    0x90    /*!< EXTI Interrupt mask register 2            */
#define EXTI_OFS_EMR2    0x94    /*!< EXTI Event mask register 2                */
#define EXTI_OFS_PR2     0x98    /*!< EXTI Pending register 2                   */

#define EXTI_OFS_IMR3    0xA0    /*!< EXTI Interrupt mask register 3            */
#define EXTI_OFS_EMR3    0xA4    /*!< EXTI Event mask register 3                */
