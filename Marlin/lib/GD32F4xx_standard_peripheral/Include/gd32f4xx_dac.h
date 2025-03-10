/*!
    \file    gd32f4xx_dac.h
    \brief   definitions for the DAC
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_DAC_H
#define GD32F4XX_DAC_H

#include "gd32f4xx.h"

/* DACx(x=0,1) definitions */
#define DAC                     DAC_BASE
#define DAC0                    0U
#define DAC1                    1U

/* registers definitions */
#define DAC_CTL                 REG32(DAC + 0x00U)          /*!< DAC control register */
#define DAC_SWT                 REG32(DAC + 0x04U)          /*!< DAC software trigger register */
#define DAC0_R12DH              REG32(DAC + 0x08U)          /*!< DAC0 12-bit right-aligned data holding register */
#define DAC0_L12DH              REG32(DAC + 0x0CU)          /*!< DAC0 12-bit left-aligned data holding register */
#define DAC0_R8DH               REG32(DAC + 0x10U)          /*!< DAC0 8-bit right-aligned data holding register */
#define DAC1_R12DH              REG32(DAC + 0x14U)          /*!< DAC1 12-bit right-aligned data holding register */
#define DAC1_L12DH              REG32(DAC + 0x18U)          /*!< DAC1 12-bit left-aligned data holding register */
#define DAC1_R8DH               REG32(DAC + 0x1CU)          /*!< DAC1 8-bit right-aligned data holding register */
#define DACC_R12DH              REG32(DAC + 0x20U)          /*!< DAC concurrent mode 12-bit right-aligned data holding register */
#define DACC_L12DH              REG32(DAC + 0x24U)          /*!< DAC concurrent mode 12-bit left-aligned data holding register */
#define DACC_R8DH               REG32(DAC + 0x28U)          /*!< DAC concurrent mode 8-bit right-aligned data holding register */
#define DAC0_DO                 REG32(DAC + 0x2CU)          /*!< DAC0 data output register */
#define DAC1_DO                 REG32(DAC + 0x30U)          /*!< DAC1 data output register */
#define DAC_STAT                REG32(DAC + 0x34U)          /*!< DAC status register */

/* bits definitions */
/* DAC_CTL */
#define DAC_CTL_DEN0            BIT(0)                      /*!< DAC0 enable/disable bit */
#define DAC_CTL_DBOFF0          BIT(1)                      /*!< DAC0 output buffer turn on/turn off bit */
#define DAC_CTL_DTEN0           BIT(2)                      /*!< DAC0 trigger enable/disable bit */
#define DAC_CTL_DTSEL0          BITS(3,5)                   /*!< DAC0 trigger source selection enable/disable bits */
#define DAC_CTL_DWM0            BITS(6,7)                   /*!< DAC0 noise wave mode */
#define DAC_CTL_DWBW0           BITS(8,11)                  /*!< DAC0 noise wave bit width */
#define DAC_CTL_DDMAEN0         BIT(12)                     /*!< DAC0 DMA enable/disable bit */
#define DAC_CTL_DDUDRIE0        BIT(13)                     /*!< DAC0 DMA underrun interrupt enable/disable bit */
#define DAC_CTL_DEN1            BIT(16)                     /*!< DAC1 enable/disable bit */ 
#define DAC_CTL_DBOFF1          BIT(17)                     /*!< DAC1 output buffer turn on/turn off bit */
#define DAC_CTL_DTEN1           BIT(18)                     /*!< DAC1 trigger enable/disable bit */
#define DAC_CTL_DTSEL1          BITS(19,21)                 /*!< DAC1 trigger source selection enable/disable bits */
#define DAC_CTL_DWM1            BITS(22,23)                 /*!< DAC1 noise wave mode */
#define DAC_CTL_DWBW1           BITS(24,27)                 /*!< DAC1 noise wave bit width */
#define DAC_CTL_DDMAEN1         BIT(28)                     /*!< DAC1 DMA enable/disable bit */
#define DAC_CTL_DDUDRIE1        BIT(29)                     /*!< DAC1 DMA underrun interrupt enable/disable bit */

/* DAC_SWT */
#define DAC_SWT_SWTR0           BIT(0)                      /*!< DAC0 software trigger bit, cleared by hardware */
#define DAC_SWT_SWTR1           BIT(1)                      /*!< DAC1 software trigger bit, cleared by hardware */

/* DAC0_R12DH */
#define DAC0_R12DH_DAC0_DH      BITS(0,11)                  /*!< DAC0 12-bit right-aligned data bits */

/* DAC0_L12DH */
#define DAC0_L12DH_DAC0_DH      BITS(4,15)                  /*!< DAC0 12-bit left-aligned data bits */

/* DAC0_R8DH */
#define DAC0_R8DH_DAC0_DH       BITS(0,7)                   /*!< DAC0 8-bit right-aligned data bits */

/* DAC1_R12DH */
#define DAC1_R12DH_DAC1_DH      BITS(0,11)                  /*!< DAC1 12-bit right-aligned data bits */

/* DAC1_L12DH */
#define DAC1_L12DH_DAC1_DH      BITS(4,15)                  /*!< DAC1 12-bit left-aligned data bits */

/* DAC1_R8DH */
#define DAC1_R8DH_DAC1_DH       BITS(0,7)                   /*!< DAC1 8-bit right-aligned data bits */

/* DACC_R12DH */
#define DACC_R12DH_DAC0_DH      BITS(0,11)                  /*!< DAC concurrent mode DAC0 12-bit right-aligned data bits */
#define DACC_R12DH_DAC1_DH      BITS(16,27)                 /*!< DAC concurrent mode DAC1 12-bit right-aligned data bits */

/* DACC_L12DH */
#define DACC_L12DH_DAC0_DH      BITS(4,15)                  /*!< DAC concurrent mode DAC0 12-bit left-aligned data bits */
#define DACC_L12DH_DAC1_DH      BITS(20,31)                 /*!< DAC concurrent mode DAC1 12-bit left-aligned data bits */

/* DACC_R8DH */
#define DACC_R8DH_DAC0_DH       BITS(0,7)                   /*!< DAC concurrent mode DAC0 8-bit right-aligned data bits */
#define DACC_R8DH_DAC1_DH       BITS(8,15)                  /*!< DAC concurrent mode DAC1 8-bit right-aligned data bits */

/* DAC0_DO */
#define DAC0_DO_DAC0_DO         BITS(0,11)                  /*!< DAC0 12-bit output data bits */

/* DAC1_DO */
#define DAC1_DO_DAC1_DO         BITS(0,11)                  /*!< DAC1 12-bit output data bits */

/* DAC_STAT */
#define DAC_STAT_DDUDR0         BIT(13)                     /*!< DAC0 DMA underrun flag */
#define DAC_STAT_DDUDR1         BIT(29)                     /*!< DAC1 DMA underrun flag */

/* constants definitions */
/* DAC trigger source */
#define CTL_DTSEL(regval)       (BITS(3,5) & ((uint32_t)(regval) << 3))
#define DAC_TRIGGER_T5_TRGO     CTL_DTSEL(0)                /*!< TIMER5 TRGO */
#define DAC_TRIGGER_T7_TRGO     CTL_DTSEL(1)                /*!< TIMER7 TRGO */
#define DAC_TRIGGER_T6_TRGO     CTL_DTSEL(2)                /*!< TIMER6 TRGO */
#define DAC_TRIGGER_T4_TRGO     CTL_DTSEL(3)                /*!< TIMER4 TRGO */
#define DAC_TRIGGER_T1_TRGO     CTL_DTSEL(4)                /*!< TIMER1 TRGO */
#define DAC_TRIGGER_T3_TRGO     CTL_DTSEL(5)                /*!< TIMER3 TRGO */
#define DAC_TRIGGER_EXTI_9      CTL_DTSEL(6)                /*!< EXTI interrupt line9 event */
#define DAC_TRIGGER_SOFTWARE    CTL_DTSEL(7)                /*!< software trigger */

/* DAC noise wave mode */
#define CTL_DWM(regval)         (BITS(6,7) & ((uint32_t)(regval) << 6))
#define DAC_WAVE_DISABLE        CTL_DWM(0)                  /*!< wave disable */
#define DAC_WAVE_MODE_LFSR      CTL_DWM(1)                  /*!< LFSR noise mode */
#define DAC_WAVE_MODE_TRIANGLE  CTL_DWM(2)                  /*!< triangle noise mode */

/* DAC noise wave bit width */
#define DWBW(regval)            (BITS(8,11) & ((uint32_t)(regval) << 8))
#define DAC_WAVE_BIT_WIDTH_1    DWBW(0)                     /*!< bit width of the wave signal is 1 */
#define DAC_WAVE_BIT_WIDTH_2    DWBW(1)                     /*!< bit width of the wave signal is 2 */
#define DAC_WAVE_BIT_WIDTH_3    DWBW(2)                     /*!< bit width of the wave signal is 3 */
#define DAC_WAVE_BIT_WIDTH_4    DWBW(3)                     /*!< bit width of the wave signal is 4 */
#define DAC_WAVE_BIT_WIDTH_5    DWBW(4)                     /*!< bit width of the wave signal is 5 */
#define DAC_WAVE_BIT_WIDTH_6    DWBW(5)                     /*!< bit width of the wave signal is 6 */
#define DAC_WAVE_BIT_WIDTH_7    DWBW(6)                     /*!< bit width of the wave signal is 7 */
#define DAC_WAVE_BIT_WIDTH_8    DWBW(7)                     /*!< bit width of the wave signal is 8 */
#define DAC_WAVE_BIT_WIDTH_9    DWBW(8)                     /*!< bit width of the wave signal is 9 */
#define DAC_WAVE_BIT_WIDTH_10   DWBW(9)                     /*!< bit width of the wave signal is 10 */
#define DAC_WAVE_BIT_WIDTH_11   DWBW(10)                    /*!< bit width of the wave signal is 11 */
#define DAC_WAVE_BIT_WIDTH_12   DWBW(11)                    /*!< bit width of the wave signal is 12 */

/* unmask LFSR bits in DAC LFSR noise mode */
#define DAC_LFSR_BIT0           DAC_WAVE_BIT_WIDTH_1        /*!< unmask the LFSR bit0 */
#define DAC_LFSR_BITS1_0        DAC_WAVE_BIT_WIDTH_2        /*!< unmask the LFSR bits[1:0] */
#define DAC_LFSR_BITS2_0        DAC_WAVE_BIT_WIDTH_3        /*!< unmask the LFSR bits[2:0] */
#define DAC_LFSR_BITS3_0        DAC_WAVE_BIT_WIDTH_4        /*!< unmask the LFSR bits[3:0] */
#define DAC_LFSR_BITS4_0        DAC_WAVE_BIT_WIDTH_5        /*!< unmask the LFSR bits[4:0] */
#define DAC_LFSR_BITS5_0        DAC_WAVE_BIT_WIDTH_6        /*!< unmask the LFSR bits[5:0] */
#define DAC_LFSR_BITS6_0        DAC_WAVE_BIT_WIDTH_7        /*!< unmask the LFSR bits[6:0] */
#define DAC_LFSR_BITS7_0        DAC_WAVE_BIT_WIDTH_8        /*!< unmask the LFSR bits[7:0] */
#define DAC_LFSR_BITS8_0        DAC_WAVE_BIT_WIDTH_9        /*!< unmask the LFSR bits[8:0] */
#define DAC_LFSR_BITS9_0        DAC_WAVE_BIT_WIDTH_10       /*!< unmask the LFSR bits[9:0] */
#define DAC_LFSR_BITS10_0       DAC_WAVE_BIT_WIDTH_11       /*!< unmask the LFSR bits[10:0] */
#define DAC_LFSR_BITS11_0       DAC_WAVE_BIT_WIDTH_12       /*!< unmask the LFSR bits[11:0] */

/* DAC data alignment */
#define DATA_ALIGN(regval)      (BITS(0,1) & ((uint32_t)(regval) << 0))
#define DAC_ALIGN_12B_R         DATA_ALIGN(0)               /*!< data right 12 bit alignment */
#define DAC_ALIGN_12B_L         DATA_ALIGN(1)               /*!< data left 12 bit alignment */
#define DAC_ALIGN_8B_R          DATA_ALIGN(2)               /*!< data right 8 bit alignment */

/* triangle amplitude in DAC triangle noise mode */
#define DAC_TRIANGLE_AMPLITUDE_1    DAC_WAVE_BIT_WIDTH_1    /*!< triangle amplitude is 1 */
#define DAC_TRIANGLE_AMPLITUDE_3    DAC_WAVE_BIT_WIDTH_2    /*!< triangle amplitude is 3 */
#define DAC_TRIANGLE_AMPLITUDE_7    DAC_WAVE_BIT_WIDTH_3    /*!< triangle amplitude is 7 */
#define DAC_TRIANGLE_AMPLITUDE_15   DAC_WAVE_BIT_WIDTH_4    /*!< triangle amplitude is 15 */
#define DAC_TRIANGLE_AMPLITUDE_31   DAC_WAVE_BIT_WIDTH_5    /*!< triangle amplitude is 31 */
#define DAC_TRIANGLE_AMPLITUDE_63   DAC_WAVE_BIT_WIDTH_6    /*!< triangle amplitude is 63 */
#define DAC_TRIANGLE_AMPLITUDE_127  DAC_WAVE_BIT_WIDTH_7    /*!< triangle amplitude is 127 */
#define DAC_TRIANGLE_AMPLITUDE_255  DAC_WAVE_BIT_WIDTH_8    /*!< triangle amplitude is 255 */
#define DAC_TRIANGLE_AMPLITUDE_511  DAC_WAVE_BIT_WIDTH_9    /*!< triangle amplitude is 511 */
#define DAC_TRIANGLE_AMPLITUDE_1023 DAC_WAVE_BIT_WIDTH_10   /*!< triangle amplitude is 1023 */
#define DAC_TRIANGLE_AMPLITUDE_2047 DAC_WAVE_BIT_WIDTH_11   /*!< triangle amplitude is 2047 */
#define DAC_TRIANGLE_AMPLITUDE_4095 DAC_WAVE_BIT_WIDTH_12   /*!< triangle amplitude is 4095 */

/* function declarations */
/* initialization functions */
/* deinitialize DAC */
void dac_deinit(void);
/* enable DAC */
void dac_enable(uint32_t dac_periph);
/* disable DAC */
void dac_disable(uint32_t dac_periph);
/* enable DAC DMA */
void dac_dma_enable(uint32_t dac_periph);
/* disable DAC DMA */
void dac_dma_disable(uint32_t dac_periph); 
/* enable DAC output buffer */
void dac_output_buffer_enable(uint32_t dac_periph);
/* disable DAC output buffer */
void dac_output_buffer_disable(uint32_t dac_periph);
/* get the last data output value */
uint16_t dac_output_value_get(uint32_t dac_periph);
/* set DAC data holding register value */
void dac_data_set(uint32_t dac_periph, uint32_t dac_align, uint16_t data);

/* DAC trigger configuration */
/* enable DAC trigger */
void dac_trigger_enable(uint32_t dac_periph);
/* disable DAC trigger */
void dac_trigger_disable(uint32_t dac_periph);
/* configure DAC trigger source */
void dac_trigger_source_config(uint32_t dac_periph, uint32_t triggersource);
/* enable DAC software trigger */
void dac_software_trigger_enable(uint32_t dac_periph);
/* disable DAC software trigger */
void dac_software_trigger_disable(uint32_t dac_periph);

/* DAC wave mode configuration */
/* configure DAC wave mode */
void dac_wave_mode_config(uint32_t dac_periph, uint32_t wave_mode);
/* configure DAC wave bit width */
void dac_wave_bit_width_config(uint32_t dac_periph, uint32_t bit_width);
/* configure DAC LFSR noise mode */
void dac_lfsr_noise_config(uint32_t dac_periph, uint32_t unmask_bits);
/* configure DAC triangle noise mode */
void dac_triangle_noise_config(uint32_t dac_periph, uint32_t amplitude);

/* DAC concurrent mode configuration */
/* enable DAC concurrent mode */
void dac_concurrent_enable(void);
/* disable DAC concurrent mode */
void dac_concurrent_disable(void);
/* enable DAC concurrent software trigger */
void dac_concurrent_software_trigger_enable(void);
/* disable DAC concurrent software trigger */
void dac_concurrent_software_trigger_disable(void);
/* enable DAC concurrent buffer function */
void dac_concurrent_output_buffer_enable(void);
/* disable DAC concurrent buffer function */
void dac_concurrent_output_buffer_disable(void);
/* set DAC concurrent mode data holding register value */
void dac_concurrent_data_set(uint32_t dac_align, uint16_t data0, uint16_t data1);
/* enable DAC concurrent interrupt */
void dac_concurrent_interrupt_enable(void);
/* disable DAC concurrent interrupt */
void dac_concurrent_interrupt_disable(void);

/* DAC interrupt configuration */
/* enable DAC interrupt(DAC DMA underrun interrupt) */
void dac_interrupt_enable(uint32_t dac_periph);
/* disable DAC interrupt(DAC DMA underrun interrupt) */
void dac_interrupt_disable(uint32_t dac_periph);
/* get the specified DAC flag(DAC DMA underrun flag) */
FlagStatus dac_flag_get(uint32_t dac_periph);
/* clear the specified DAC flag(DAC DMA underrun flag) */
void dac_flag_clear(uint32_t dac_periph);
/* get the specified DAC interrupt flag(DAC DMA underrun interrupt flag) */
FlagStatus dac_interrupt_flag_get(uint32_t dac_periph);
/* clear the specified DAC interrupt flag(DAC DMA underrun interrupt flag) */
void dac_interrupt_flag_clear(uint32_t dac_periph);

#endif /* GD32F4XX_DAC_H */
