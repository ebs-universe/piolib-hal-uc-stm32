/* 
 Copyright (c)
     (c) 2024 Chintalagiri Shashank
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
 
 This file is part of
 Embedded bootstraps : Peripheral driver implementations : AVR
 
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

#ifndef CORE_IMPL_H
#define CORE_IMPL_H

#include <hal/uc/core.h>

#if uC_WDT_ENABLED

static inline void watchdog_clear(void) {
    ;
}

#endif

/** 
 * @brief Initialize MPU to sane defaults. 
 *  
 * These defaults presently assume a boot-only application, with fixed ITCM 
 * and DTCM sizes for 64KB and 128KB respectively. 
 * 
 * We use the default set by CubeMX and fallback to the default memory map. 
 * It is expected that this will prevent most instances of crashes due to 
 * speculative execution, since the present implementation more or less
 * disables all speculative execution.
 * 
 * This configuration likely needs additional work before it is reliable.
 * See : https://youtu.be/6IUfxSAFhlw. 
 * 
 * Various parameters related to memory regions are technically application 
 * dependent. The function implemented here is a weak default placeholder 
 * which can be used if the application does not care.
 *
 */
void mpu_init(void);

/** 
 * @brief Initialize Memory to sane defaults. 
 *  
 * Memory layout is technically application dependent. The function 
 * implemented here is a weak default placeholder which can be used if the 
 * application does not care.
 * 
 * These defaults presently assume a boot-only application, with fixed ITCM 
 * and DTCM sizes for 64KB and 128KB respectively. We also specify I2C instead 
 * of I3C (not sure of context) and disable ECC on SRAM.
 * 
 * Share of memory (RAM and Flash) provided for DTCM, ITCM is technically
 * user configurable. However, since this also requires multiple changes in 
 * the linker file, we do not provide this control. This may be revisited 
 * in the future, since the linker file is kept in the application and can 
 * in principle be edited without editing the reusable ebs hal library.
 * 
 * This is associated with the STM32 FLASH peripheral, and the implementation 
 * here is generated using CubeMX and copied from flash.c
 *
 */
void configure_memory_layout(void);

/** 
 * @brief Initialize CPU features to sane defaults. 
 * 
 * This function configures some core CPU features. Most of these 
 * features can be controlled by defines in application.h. It is  
 * implemented here as a weak default placeholder which can be 
 * overridden by the application if necessary, though this should 
 * not generally be required.
 * 
 * Enabling Data Cache requires certain variables to be forced into 
 * memory that is not cached, so that they can be used freely from ISRs
 * without dealing with cache coherence / invalidation. An 'easy' way to 
 * achieve this is to put these variables into the DTCM. 
 * 
 * Negative consequences of using the Instruction Cache are not presently
 * known.
 * 
 * Both caches disabled, as well as disabling Speculative Execution, is 
 * the safest (and slowest) combination. 
 * 
 * Enabling Speculative Exection is presently not implemented. It will 
 * need to be done in the ebs hal core peripheral. Specifically, the 
 * mpu_init function will need to be modified to enable execution on 
 * memory regions which should allow speculative execution. 
 * 
 */
void cpu_init(void);

/** 
 * Initialize the core interrupt controller. This does tend to be somewhat 
 * application specific, so it is implemented here as a weak default 
 * placeholder which can be overridden by the application if necessary, 
 * though this should not generally be required.
 * 
 * On this platform, initializes the NVIC Priority Grouping to 2, ie, 
 * 
 * Sets priorities to 2 bits of Preemptible and 2 bits of Subpriority.
 * This approach should probably work for most Cortex M processors. 
 * 
 * Note that while this grouping is not generally relied on, we do not
 * impose any explicit standards on how HAL implementations of libraries 
 * might use the interrupts. Changing it might break other HAL code or 
 * EBS libraries which expect this priority grouping. 
 * 
 */
void nvic_init(void);



#endif
