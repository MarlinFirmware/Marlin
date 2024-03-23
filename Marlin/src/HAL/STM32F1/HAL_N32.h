/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 * Specifically for VOXELAB_N32 (N32G452). TODO: Rework for generic N32 MCU.
 */

#include <STM32ADC.h>

typedef struct {
  uint32_t WorkMode;
  uint32_t MultiChEn;
  uint32_t ContinueConvEn;
  uint32_t ExtTrigSelect;
  uint32_t DatAlign;
  uint8_t ChsNumber;
} ADC_InitType;

typedef struct {
  __IO uint32_t STS;
  __IO uint32_t CTRL1;
  __IO uint32_t CTRL2;
  __IO uint32_t SAMPT1;
  __IO uint32_t SAMPT2;
  __IO uint32_t JOFFSET1;
  __IO uint32_t JOFFSET2;
  __IO uint32_t JOFFSET3;
  __IO uint32_t JOFFSET4;
  __IO uint32_t WDGHIGH;
  __IO uint32_t WDGLOW;
  __IO uint32_t RSEQ1;
  __IO uint32_t RSEQ2;
  __IO uint32_t RSEQ3;
  __IO uint32_t JSEQ;
  __IO uint32_t JDAT1;
  __IO uint32_t JDAT2;
  __IO uint32_t JDAT3;
  __IO uint32_t JDAT4;
  __IO uint32_t DAT;
  __IO uint32_t DIFSEL;
  __IO uint32_t CALFACT;
  __IO uint32_t CTRL3;
  __IO uint32_t SAMPT3;
} ADC_Module;

#define NS_ADC1_BASE ((uint32_t)0x40020800)
#define NS_ADC2_BASE ((uint32_t)0x40020c00)
#define NS_ADC3_BASE ((uint32_t)0x40021800)
#define NS_ADC4_BASE ((uint32_t)0x40021c00)

#define NS_ADC1 ((ADC_Module*)NS_ADC1_BASE)
#define NS_ADC2 ((ADC_Module*)NS_ADC2_BASE)
#define NS_ADC3 ((ADC_Module*)NS_ADC3_BASE)
#define NS_ADC4 ((ADC_Module*)NS_ADC4_BASE)

#define ADC1_Channel_01_PA0 ((uint8_t)0x01)
#define ADC1_Channel_02_PA1 ((uint8_t)0x02)
#define ADC1_Channel_03_PA6 ((uint8_t)0x03)
#define ADC1_Channel_04_PA3 ((uint8_t)0x04)
#define ADC1_Channel_05_PF4 ((uint8_t)0x05)
#define ADC1_Channel_06_PC0 ((uint8_t)0x06)
#define ADC1_Channel_07_PC1 ((uint8_t)0x07)
#define ADC1_Channel_08_PC2 ((uint8_t)0x08)
#define ADC1_Channel_09_PC3 ((uint8_t)0x09)
#define ADC1_Channel_10_PF2 ((uint8_t)0x0A)
#define ADC1_Channel_11_PA2 ((uint8_t)0x0B)

#define ADC2_Channel_01_PA4 ((uint8_t)0x01)
#define ADC2_Channel_02_PA5 ((uint8_t)0x02)
#define ADC2_Channel_03_PB1 ((uint8_t)0x03)
#define ADC2_Channel_04_PA7 ((uint8_t)0x04)
#define ADC2_Channel_05_PC4 ((uint8_t)0x05)
#define ADC2_Channel_06_PC0 ((uint8_t)0x06)
#define ADC2_Channel_07_PC1 ((uint8_t)0x07)
#define ADC2_Channel_08_PC2 ((uint8_t)0x08)
#define ADC2_Channel_09_PC3 ((uint8_t)0x09)
#define ADC2_Channel_10_PF2 ((uint8_t)0x0A)
#define ADC2_Channel_11_PA2 ((uint8_t)0x0B)
#define ADC2_Channel_12_PC5 ((uint8_t)0x0C)
#define ADC2_Channel_13_PB2 ((uint8_t)0x0D)

#define ADC3_Channel_01_PB11 ((uint8_t)0x01)
#define ADC3_Channel_02_PE9  ((uint8_t)0x02)
#define ADC3_Channel_03_PE13 ((uint8_t)0x03)
#define ADC3_Channel_04_PE12 ((uint8_t)0x04)
#define ADC3_Channel_05_PB13 ((uint8_t)0x05)
#define ADC3_Channel_06_PE8  ((uint8_t)0x06)
#define ADC3_Channel_07_PD10 ((uint8_t)0x07)
#define ADC3_Channel_08_PD11 ((uint8_t)0x08)
#define ADC3_Channel_09_PD12 ((uint8_t)0x09)
#define ADC3_Channel_10_PD13 ((uint8_t)0x0A)
#define ADC3_Channel_11_PD14 ((uint8_t)0x0B)
#define ADC3_Channel_12_PB0  ((uint8_t)0x0C)
#define ADC3_Channel_13_PE7  ((uint8_t)0x0D)
#define ADC3_Channel_14_PE10 ((uint8_t)0x0E)
#define ADC3_Channel_15_PE11 ((uint8_t)0x0F)

#define ADC4_Channel_01_PE14 ((uint8_t)0x01)
#define ADC4_Channel_02_PE15 ((uint8_t)0x02)
#define ADC4_Channel_03_PB12 ((uint8_t)0x03)
#define ADC4_Channel_04_PB14 ((uint8_t)0x04)
#define ADC4_Channel_05_PB15 ((uint8_t)0x05)
#define ADC4_Channel_06_PE8  ((uint8_t)0x06)
#define ADC4_Channel_07_PD10 ((uint8_t)0x07)
#define ADC4_Channel_08_PD11 ((uint8_t)0x08)
#define ADC4_Channel_09_PD12 ((uint8_t)0x09)
#define ADC4_Channel_10_PD13 ((uint8_t)0x0A)
#define ADC4_Channel_11_PD14 ((uint8_t)0x0B)
#define ADC4_Channel_12_PD8  ((uint8_t)0x0C)
#define ADC4_Channel_13_PD9  ((uint8_t)0x0D)

#define ADC_RCC_BASE          ((uint32_t)0x40021000)
#define ADC_RCC_CTRL          *((uint32_t*)(ADC_RCC_BASE + 0x00))
#define ADC_RCC_CFG           *((uint32_t*)(ADC_RCC_BASE + 0x04))
#define ADC_RCC_CLKINT        *((uint32_t*)(ADC_RCC_BASE + 0x08))
#define ADC_RCC_APB2PRST      *((uint32_t*)(ADC_RCC_BASE + 0x0c))
#define ADC_RCC_APB1PRST      *((uint32_t*)(ADC_RCC_BASE + 0x10))
#define ADC_RCC_AHBPCLKEN     *((uint32_t*)(ADC_RCC_BASE + 0x14))
#define ADC_RCC_APB2PCLKEN    *((uint32_t*)(ADC_RCC_BASE + 0x18))
#define ADC_RCC_APB1PCLKEN    *((uint32_t*)(ADC_RCC_BASE + 0x1c))
#define ADC_RCC_BDCTRL        *((uint32_t*)(ADC_RCC_BASE + 0x20))
#define ADC_RCC_CTRLSTS       *((uint32_t*)(ADC_RCC_BASE + 0x24))
#define ADC_RCC_AHBPRST       *((uint32_t*)(ADC_RCC_BASE + 0x28))
#define ADC_RCC_CFG2          *((uint32_t*)(ADC_RCC_BASE + 0x2c))
#define ADC_RCC_CFG3          *((uint32_t*)(ADC_RCC_BASE + 0x30))

#define NS_PWR_CR3            *((uint32_t*)(0x40007000 + 0x0C))
#define RCC_APB1Periph_PWR      ((uint32_t)0x10000000)

///////////////////////////////
#define NS_GPIOA_BASE         ((uint32_t)0x40010800)
#define NS_GPIOA_PL_CFG       *((uint32_t*)(NS_GPIOA_BASE + 0x00))
#define NS_GPIOA_PH_CFG       *((uint32_t*)(NS_GPIOA_BASE + 0x04))

#define NS_GPIOC_BASE         ((uint32_t)0x40011000)
#define NS_GPIOC_PL_CFG       *((uint32_t*)(NS_GPIOC_BASE + 0x00))
#define NS_GPIOC_PH_CFG       *((uint32_t*)(NS_GPIOC_BASE + 0x04))

/* CFG2 register bit mask */
#define CFG2_TIM18CLKSEL_SET_MASK   ((uint32_t)0x20000000)
#define CFG2_TIM18CLKSEL_RESET_MASK ((uint32_t)0xDFFFFFFF)
#define CFG2_RNGCPRES_SET_MASK      ((uint32_t)0x1F000000)
#define CFG2_RNGCPRES_RESET_MASK    ((uint32_t)0xE0FFFFFF)
#define CFG2_ADC1MSEL_SET_MASK      ((uint32_t)0x00000400)
#define CFG2_ADC1MSEL_RESET_MASK    ((uint32_t)0xFFFFFBFF)
#define CFG2_ADC1MPRES_SET_MASK     ((uint32_t)0x0000F800)
#define CFG2_ADC1MPRES_RESET_MASK   ((uint32_t)0xFFFF07FF)
#define CFG2_ADCPLLPRES_SET_MASK    ((uint32_t)0x000001F0)
#define CFG2_ADCPLLPRES_RESET_MASK  ((uint32_t)0xFFFFFE0F)
#define CFG2_ADCHPRES_SET_MASK      ((uint32_t)0x0000000F)
#define CFG2_ADCHPRES_RESET_MASK    ((uint32_t)0xFFFFFFF0)

#define RCC_ADCPLLCLK_DISABLE    ((uint32_t)0xFFFFFEFF)
#define RCC_ADCPLLCLK_DIV1       ((uint32_t)0x00000100)
#define RCC_ADCPLLCLK_DIV2       ((uint32_t)0x00000110)
#define RCC_ADCPLLCLK_DIV4       ((uint32_t)0x00000120)
#define RCC_ADCPLLCLK_DIV6       ((uint32_t)0x00000130)
#define RCC_ADCPLLCLK_DIV8       ((uint32_t)0x00000140)
#define RCC_ADCPLLCLK_DIV10      ((uint32_t)0x00000150)
#define RCC_ADCPLLCLK_DIV12      ((uint32_t)0x00000160)
#define RCC_ADCPLLCLK_DIV16      ((uint32_t)0x00000170)
#define RCC_ADCPLLCLK_DIV32      ((uint32_t)0x00000180)
#define RCC_ADCPLLCLK_DIV64      ((uint32_t)0x00000190)
#define RCC_ADCPLLCLK_DIV128     ((uint32_t)0x000001A0)
#define RCC_ADCPLLCLK_DIV256     ((uint32_t)0x000001B0)
#define RCC_ADCPLLCLK_DIV_OTHERS ((uint32_t)0x000001C0)

#define RCC_ADCHCLK_DIV1       ((uint32_t)0x00000000)
#define RCC_ADCHCLK_DIV2       ((uint32_t)0x00000001)
#define RCC_ADCHCLK_DIV4       ((uint32_t)0x00000002)
#define RCC_ADCHCLK_DIV6       ((uint32_t)0x00000003)
#define RCC_ADCHCLK_DIV8       ((uint32_t)0x00000004)
#define RCC_ADCHCLK_DIV10      ((uint32_t)0x00000005)
#define RCC_ADCHCLK_DIV12      ((uint32_t)0x00000006)
#define RCC_ADCHCLK_DIV16      ((uint32_t)0x00000007)
#define RCC_ADCHCLK_DIV32      ((uint32_t)0x00000008)
#define RCC_ADCHCLK_DIV_OTHERS ((uint32_t)0x00000008)

#define SAMPT1_SMP_SET ((uint32_t)0x00000007)
#define SAMPT2_SMP_SET ((uint32_t)0x00000007)

#define SQR4_SEQ_SET ((uint32_t)0x0000001F)
#define SQR3_SEQ_SET ((uint32_t)0x0000001F)
#define SQR2_SEQ_SET ((uint32_t)0x0000001F)
#define SQR1_SEQ_SET ((uint32_t)0x0000001F)

#define CTRL1_CLR_MASK ((uint32_t)0xFFF0FEFF)
#define RSEQ1_CLR_MASK ((uint32_t)0xFF0FFFFF)
#define CTRL2_CLR_MASK ((uint32_t)0xFFF1F7FD)

#define ADC_CH_0  ((uint8_t)0x00)
#define ADC_CH_1  ((uint8_t)0x01)
#define ADC_CH_2  ((uint8_t)0x02)
#define ADC_CH_3  ((uint8_t)0x03)
#define ADC_CH_4  ((uint8_t)0x04)
#define ADC_CH_5  ((uint8_t)0x05)
#define ADC_CH_6  ((uint8_t)0x06)
#define ADC_CH_7  ((uint8_t)0x07)
#define ADC_CH_8  ((uint8_t)0x08)
#define ADC_CH_9  ((uint8_t)0x09)
#define ADC_CH_10 ((uint8_t)0x0A)
#define ADC_CH_11 ((uint8_t)0x0B)
#define ADC_CH_12 ((uint8_t)0x0C)
#define ADC_CH_13 ((uint8_t)0x0D)
#define ADC_CH_14 ((uint8_t)0x0E)
#define ADC_CH_15 ((uint8_t)0x0F)
#define ADC_CH_16 ((uint8_t)0x10)
#define ADC_CH_17 ((uint8_t)0x11)
#define ADC_CH_18 ((uint8_t)0x12)

#define ADC_WORKMODE_INDEPENDENT            ((uint32_t)0x00000000)
#define ADC_WORKMODE_REG_INJECT_SIMULT      ((uint32_t)0x00010000)
#define ADC_WORKMODE_REG_SIMULT_ALTER_TRIG  ((uint32_t)0x00020000)
#define ADC_WORKMODE_INJ_SIMULT_FAST_INTERL ((uint32_t)0x00030000)
#define ADC_WORKMODE_INJ_SIMULT_SLOW_INTERL ((uint32_t)0x00040000)
#define ADC_WORKMODE_INJ_SIMULT             ((uint32_t)0x00050000)
#define ADC_WORKMODE_REG_SIMULT             ((uint32_t)0x00060000)
#define ADC_WORKMODE_FAST_INTERL            ((uint32_t)0x00070000)
#define ADC_WORKMODE_SLOW_INTERL            ((uint32_t)0x00080000)
#define ADC_WORKMODE_ALTER_TRIG             ((uint32_t)0x00090000)

#define ADC_EXT_TRIGCONV_T1_CC3 ((uint32_t)0x00040000) //!< For ADC1, ADC2 , ADC3 and ADC4
#define ADC_EXT_TRIGCONV_NONE   ((uint32_t)0x000E0000) //!< For ADC1, ADC2 , ADC3 and ADC4

#define ADC_DAT_ALIGN_R      ((uint32_t)0x00000000)
#define ADC_DAT_ALIGN_L      ((uint32_t)0x00000800)

#define ADC_FLAG_RDY    ((uint8_t)0x20)
#define ADC_FLAG_PD_RDY ((uint8_t)0x40)

#define CTRL2_AD_ON_SET   ((uint32_t)0x00000001)
#define CTRL2_AD_ON_RESET ((uint32_t)0xFFFFFFFE)

#define CTRL2_CAL_SET ((uint32_t)0x00000004)

/* ADC Software start mask */
#define CTRL2_EXT_TRIG_SWSTART_SET   ((uint32_t)0x00500000)
#define CTRL2_EXT_TRIG_SWSTART_RESET ((uint32_t)0xFFAFFFFF)

#define ADC_SAMP_TIME_1CYCLES5   ((uint8_t)0x00)
#define ADC_SAMP_TIME_7CYCLES5   ((uint8_t)0x01)
#define ADC_SAMP_TIME_13CYCLES5  ((uint8_t)0x02)
#define ADC_SAMP_TIME_28CYCLES5  ((uint8_t)0x03)
#define ADC_SAMP_TIME_41CYCLES5  ((uint8_t)0x04)
#define ADC_SAMP_TIME_55CYCLES5  ((uint8_t)0x05)
#define ADC_SAMP_TIME_71CYCLES5  ((uint8_t)0x06)
#define ADC_SAMP_TIME_239CYCLES5 ((uint8_t)0x07)

#define ADC_FLAG_AWDG      ((uint8_t)0x01)
#define ADC_FLAG_ENDC      ((uint8_t)0x02)
#define ADC_FLAG_JENDC     ((uint8_t)0x04)
#define ADC_FLAG_JSTR      ((uint8_t)0x08)
#define ADC_FLAG_STR       ((uint8_t)0x10)
#define ADC_FLAG_EOC_ANY   ((uint8_t)0x20)
#define ADC_FLAG_JEOC_ANY  ((uint8_t)0x40)

/* ADC DMA mask */
#define CTRL2_DMA_SET   ((uint32_t)0x00000100)
#define CTRL2_DMA_RESET ((uint32_t)0xFFFFFEFF)

typedef struct {
  uint32_t PeriphAddr;
  uint32_t MemAddr;
  uint32_t Direction;
  uint32_t BufSize;
  uint32_t PeriphInc;
  uint32_t DMA_MemoryInc;
  uint32_t PeriphDataSize;
  uint32_t MemDataSize;
  uint32_t CircularMode;
  uint32_t Priority;
  uint32_t Mem2Mem;
} DMA_InitType;

typedef struct {
  __IO uint32_t CHCFG;
  __IO uint32_t TXNUM;
  __IO uint32_t PADDR;
  __IO uint32_t MADDR;
  __IO uint32_t CHSEL;
} DMA_ChannelType;

#define DMA_DIR_PERIPH_DST ((uint32_t)0x00000010)
#define DMA_DIR_PERIPH_SRC ((uint32_t)0x00000000)

#define DMA_PERIPH_INC_ENABLE          ((uint32_t)0x00000040)
#define DMA_PERIPH_INC_DISABLE         ((uint32_t)0x00000000)

#define DMA_MEM_INC_ENABLE          ((uint32_t)0x00000080)
#define DMA_MEM_INC_DISABLE         ((uint32_t)0x00000000)

#define DMA_PERIPH_DATA_SIZE_BYTE     ((uint32_t)0x00000000)
#define DMA_PERIPH_DATA_SIZE_HALFWORD ((uint32_t)0x00000100)
#define DMA_PERIPH_DATA_SIZE_WORD     ((uint32_t)0x00000200)

#define DMA_MemoryDataSize_Byte     ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word     ((uint32_t)0x00000800)

#define DMA_MODE_CIRCULAR ((uint32_t)0x00000020)
#define DMA_MODE_NORMAL   ((uint32_t)0x00000000)

#define DMA_M2M_ENABLE          ((uint32_t)0x00004000)
#define DMA_M2M_DISABLE         ((uint32_t)0x00000000)

#define RCC_AHB_PERIPH_DMA1   ((uint32_t)0x00000001)
#define RCC_AHB_PERIPH_DMA2   ((uint32_t)0x00000002)

/*******************  Bit definition for DMA_CHCFG1 register  *******************/
#define DMA_CHCFG1_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG1_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG1_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG1_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG1_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG1_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG1_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG1_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define NS_DMA1_BASE     (0x40020000)
#define DMA1_CH1_BASE (NS_DMA1_BASE + 0x0008)
#define DMA1_CH2_BASE (NS_DMA1_BASE + 0x001C)
#define DMA1_CH3_BASE (NS_DMA1_BASE + 0x0030)
#define DMA1_CH4_BASE (NS_DMA1_BASE + 0x0044)
#define DMA1_CH5_BASE (NS_DMA1_BASE + 0x0058)
#define DMA1_CH6_BASE (NS_DMA1_BASE + 0x006C)
#define DMA1_CH7_BASE (NS_DMA1_BASE + 0x0080)
#define DMA1_CH8_BASE (NS_DMA1_BASE + 0x0094)

#define NS_DMA2_BASE     (0x40020400)
#define DMA2_CH1_BASE (NS_DMA2_BASE + 0x008)
#define DMA2_CH2_BASE (NS_DMA2_BASE + 0x01C)
#define DMA2_CH3_BASE (NS_DMA2_BASE + 0x0030)
#define DMA2_CH4_BASE (NS_DMA2_BASE + 0x0044)
#define DMA2_CH5_BASE (NS_DMA2_BASE + 0x0058)
#define DMA2_CH6_BASE (NS_DMA2_BASE + 0x006C)
#define DMA2_CH7_BASE (NS_DMA2_BASE + 0x0080)
#define DMA2_CH8_BASE (NS_DMA2_BASE + 0x0094)

#define DMA1        ((DMA_Module*)NS_DMA1_BASE)
#define DMA2        ((DMA_Module*)NS_DMA2_BASE)
#define DMA1_CH1    ((DMA_ChannelType*)DMA1_CH1_BASE)
#define DMA1_CH2    ((DMA_ChannelType*)DMA1_CH2_BASE)
#define DMA1_CH3    ((DMA_ChannelType*)DMA1_CH3_BASE)
#define DMA1_CH4    ((DMA_ChannelType*)DMA1_CH4_BASE)
#define DMA1_CH5    ((DMA_ChannelType*)DMA1_CH5_BASE)
#define DMA1_CH6    ((DMA_ChannelType*)DMA1_CH6_BASE)
#define DMA1_CH7    ((DMA_ChannelType*)DMA1_CH7_BASE)
#define DMA1_CH8    ((DMA_ChannelType*)DMA1_CH8_BASE)
#define DMA2_CH1    ((DMA_ChannelType*)DMA2_CH1_BASE)
#define DMA2_CH2    ((DMA_ChannelType*)DMA2_CH2_BASE)
#define DMA2_CH3    ((DMA_ChannelType*)DMA2_CH3_BASE)
#define DMA2_CH4    ((DMA_ChannelType*)DMA2_CH4_BASE)
#define DMA2_CH5    ((DMA_ChannelType*)DMA2_CH5_BASE)
#define DMA2_CH6    ((DMA_ChannelType*)DMA2_CH6_BASE)
#define DMA2_CH7    ((DMA_ChannelType*)DMA2_CH7_BASE)
#define DMA2_CH8    ((DMA_ChannelType*)DMA2_CH8_BASE)

/******************************************************************************/
/*                                                                            */
/*                             DMA Controller                                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_INTSTS register  ********************/
#define DMA_INTSTS_GLBF1 ((uint32_t)0x00000001) //!< Channel 1 Global interrupt flag
#define DMA_INTSTS_TXCF1 ((uint32_t)0x00000002) //!< Channel 1 Transfer Complete flag
#define DMA_INTSTS_HTXF1 ((uint32_t)0x00000004) //!< Channel 1 Half Transfer flag
#define DMA_INTSTS_ERRF1 ((uint32_t)0x00000008) //!< Channel 1 Transfer Error flag
#define DMA_INTSTS_GLBF2 ((uint32_t)0x00000010) //!< Channel 2 Global interrupt flag
#define DMA_INTSTS_TXCF2 ((uint32_t)0x00000020) //!< Channel 2 Transfer Complete flag
#define DMA_INTSTS_HTXF2 ((uint32_t)0x00000040) //!< Channel 2 Half Transfer flag
#define DMA_INTSTS_ERRF2 ((uint32_t)0x00000080) //!< Channel 2 Transfer Error flag
#define DMA_INTSTS_GLBF3 ((uint32_t)0x00000100) //!< Channel 3 Global interrupt flag
#define DMA_INTSTS_TXCF3 ((uint32_t)0x00000200) //!< Channel 3 Transfer Complete flag
#define DMA_INTSTS_HTXF3 ((uint32_t)0x00000400) //!< Channel 3 Half Transfer flag
#define DMA_INTSTS_ERRF3 ((uint32_t)0x00000800) //!< Channel 3 Transfer Error flag
#define DMA_INTSTS_GLBF4 ((uint32_t)0x00001000) //!< Channel 4 Global interrupt flag
#define DMA_INTSTS_TXCF4 ((uint32_t)0x00002000) //!< Channel 4 Transfer Complete flag
#define DMA_INTSTS_HTXF4 ((uint32_t)0x00004000) //!< Channel 4 Half Transfer flag
#define DMA_INTSTS_ERRF4 ((uint32_t)0x00008000) //!< Channel 4 Transfer Error flag
#define DMA_INTSTS_GLBF5 ((uint32_t)0x00010000) //!< Channel 5 Global interrupt flag
#define DMA_INTSTS_TXCF5 ((uint32_t)0x00020000) //!< Channel 5 Transfer Complete flag
#define DMA_INTSTS_HTXF5 ((uint32_t)0x00040000) //!< Channel 5 Half Transfer flag
#define DMA_INTSTS_ERRF5 ((uint32_t)0x00080000) //!< Channel 5 Transfer Error flag
#define DMA_INTSTS_GLBF6 ((uint32_t)0x00100000) //!< Channel 6 Global interrupt flag
#define DMA_INTSTS_TXCF6 ((uint32_t)0x00200000) //!< Channel 6 Transfer Complete flag
#define DMA_INTSTS_HTXF6 ((uint32_t)0x00400000) //!< Channel 6 Half Transfer flag
#define DMA_INTSTS_ERRF6 ((uint32_t)0x00800000) //!< Channel 6 Transfer Error flag
#define DMA_INTSTS_GLBF7 ((uint32_t)0x01000000) //!< Channel 7 Global interrupt flag
#define DMA_INTSTS_TXCF7 ((uint32_t)0x02000000) //!< Channel 7 Transfer Complete flag
#define DMA_INTSTS_HTXF7 ((uint32_t)0x04000000) //!< Channel 7 Half Transfer flag
#define DMA_INTSTS_ERRF7 ((uint32_t)0x08000000) //!< Channel 7 Transfer Error flag
#define DMA_INTSTS_GLBF8 ((uint32_t)0x10000000) //!< Channel 7 Global interrupt flag
#define DMA_INTSTS_TXCF8 ((uint32_t)0x20000000) //!< Channel 7 Transfer Complete flag
#define DMA_INTSTS_HTXF8 ((uint32_t)0x40000000) //!< Channel 7 Half Transfer flag
#define DMA_INTSTS_ERRF8 ((uint32_t)0x80000000) //!< Channel 7 Transfer Error flag

/*******************  Bit definition for DMA_INTCLR register  *******************/
#define DMA_INTCLR_CGLBF1 ((uint32_t)0x00000001) //!< Channel 1 Global interrupt clear
#define DMA_INTCLR_CTXCF1 ((uint32_t)0x00000002) //!< Channel 1 Transfer Complete clear
#define DMA_INTCLR_CHTXF1 ((uint32_t)0x00000004) //!< Channel 1 Half Transfer clear
#define DMA_INTCLR_CERRF1 ((uint32_t)0x00000008) //!< Channel 1 Transfer Error clear
#define DMA_INTCLR_CGLBF2 ((uint32_t)0x00000010) //!< Channel 2 Global interrupt clear
#define DMA_INTCLR_CTXCF2 ((uint32_t)0x00000020) //!< Channel 2 Transfer Complete clear
#define DMA_INTCLR_CHTXF2 ((uint32_t)0x00000040) //!< Channel 2 Half Transfer clear
#define DMA_INTCLR_CERRF2 ((uint32_t)0x00000080) //!< Channel 2 Transfer Error clear
#define DMA_INTCLR_CGLBF3 ((uint32_t)0x00000100) //!< Channel 3 Global interrupt clear
#define DMA_INTCLR_CTXCF3 ((uint32_t)0x00000200) //!< Channel 3 Transfer Complete clear
#define DMA_INTCLR_CHTXF3 ((uint32_t)0x00000400) //!< Channel 3 Half Transfer clear
#define DMA_INTCLR_CERRF3 ((uint32_t)0x00000800) //!< Channel 3 Transfer Error clear
#define DMA_INTCLR_CGLBF4 ((uint32_t)0x00001000) //!< Channel 4 Global interrupt clear
#define DMA_INTCLR_CTXCF4 ((uint32_t)0x00002000) //!< Channel 4 Transfer Complete clear
#define DMA_INTCLR_CHTXF4 ((uint32_t)0x00004000) //!< Channel 4 Half Transfer clear
#define DMA_INTCLR_CERRF4 ((uint32_t)0x00008000) //!< Channel 4 Transfer Error clear
#define DMA_INTCLR_CGLBF5 ((uint32_t)0x00010000) //!< Channel 5 Global interrupt clear
#define DMA_INTCLR_CTXCF5 ((uint32_t)0x00020000) //!< Channel 5 Transfer Complete clear
#define DMA_INTCLR_CHTXF5 ((uint32_t)0x00040000) //!< Channel 5 Half Transfer clear
#define DMA_INTCLR_CERRF5 ((uint32_t)0x00080000) //!< Channel 5 Transfer Error clear
#define DMA_INTCLR_CGLBF6 ((uint32_t)0x00100000) //!< Channel 6 Global interrupt clear
#define DMA_INTCLR_CTXCF6 ((uint32_t)0x00200000) //!< Channel 6 Transfer Complete clear
#define DMA_INTCLR_CHTXF6 ((uint32_t)0x00400000) //!< Channel 6 Half Transfer clear
#define DMA_INTCLR_CERRF6 ((uint32_t)0x00800000) //!< Channel 6 Transfer Error clear
#define DMA_INTCLR_CGLBF7 ((uint32_t)0x01000000) //!< Channel 7 Global interrupt clear
#define DMA_INTCLR_CTXCF7 ((uint32_t)0x02000000) //!< Channel 7 Transfer Complete clear
#define DMA_INTCLR_CHTXF7 ((uint32_t)0x04000000) //!< Channel 7 Half Transfer clear
#define DMA_INTCLR_CERRF7 ((uint32_t)0x08000000) //!< Channel 7 Transfer Error clear
#define DMA_INTCLR_CGLBF8 ((uint32_t)0x10000000) //!< Channel 7 Global interrupt clear
#define DMA_INTCLR_CTXCF8 ((uint32_t)0x20000000) //!< Channel 7 Transfer Complete clear
#define DMA_INTCLR_CHTXF8 ((uint32_t)0x40000000) //!< Channel 7 Half Transfer clear
#define DMA_INTCLR_CERRF8 ((uint32_t)0x80000000) //!< Channel 7 Transfer Error clear

/*******************  Bit definition for DMA_CHCFG1 register  *******************/
#define DMA_CHCFG1_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG1_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG1_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG1_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG1_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG1_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG1_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG1_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG1_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG1_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG1_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG1_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG1_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG1_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG1_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits(Channel Priority level)
#define DMA_CHCFG1_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG1_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG1_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode

/*******************  Bit definition for DMA_CHCFG2 register  *******************/
#define DMA_CHCFG2_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG2_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG2_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG2_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG2_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG2_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG2_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG2_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG2_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG2_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG2_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG2_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG2_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG2_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG2_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG2_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG2_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG2_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode

/*******************  Bit definition for DMA_CHCFG3 register  *******************/
#define DMA_CHCFG3_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG3_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG3_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG3_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG3_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG3_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG3_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG3_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG3_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG3_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG3_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG3_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG3_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG3_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG3_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG3_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG3_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG3_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode

/*!<******************  Bit definition for DMA_CHCFG4 register  *******************/
#define DMA_CHCFG4_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG4_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG4_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG4_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG4_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG4_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG4_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG4_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG4_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG4_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG4_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG4_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG4_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG4_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG4_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG4_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG4_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG4_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode

/******************  Bit definition for DMA_CHCFG5 register  *******************/
#define DMA_CHCFG5_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG5_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG5_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG5_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG5_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG5_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG5_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG5_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG5_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG5_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG5_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG5_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG5_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG5_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG5_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG5_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG5_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG5_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode enable

/*******************  Bit definition for DMA_CHCFG6 register  *******************/
#define DMA_CHCFG6_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG6_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG6_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG6_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG6_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG6_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG6_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG6_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG6_PSIZE   ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG6_PSIZE_0 ((uint16_t)0x0100) //!< Bit 0
#define DMA_CHCFG6_PSIZE_1 ((uint16_t)0x0200) //!< Bit 1

#define DMA_CHCFG6_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG6_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG6_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG6_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG6_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG6_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG6_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode

/*******************  Bit definition for DMA_CHCFG7 register  *******************/
#define DMA_CHCFG7_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG7_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG7_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG7_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG7_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG7_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG7_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG7_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG7_PSIZE   , ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG7_PSIZE_0 ((uint16_t)0x0100)   //!< Bit 0
#define DMA_CHCFG7_PSIZE_1 ((uint16_t)0x0200)   //!< Bit 1

#define DMA_CHCFG7_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG7_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG7_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG7_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG7_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG7_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG7_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode enable

/*******************  Bit definition for DMA_CHCFG8 register  *******************/
#define DMA_CHCFG8_CHEN  ((uint16_t)0x0001) //!< Channel enable
#define DMA_CHCFG8_TXCIE ((uint16_t)0x0002) //!< Transfer complete interrupt enable
#define DMA_CHCFG8_HTXIE ((uint16_t)0x0004) //!< Half Transfer interrupt enable
#define DMA_CHCFG8_ERRIE ((uint16_t)0x0008) //!< Transfer error interrupt enable
#define DMA_CHCFG8_DIR   ((uint16_t)0x0010) //!< Data transfer direction
#define DMA_CHCFG8_CIRC  ((uint16_t)0x0020) //!< Circular mode
#define DMA_CHCFG8_PINC  ((uint16_t)0x0040) //!< Peripheral increment mode
#define DMA_CHCFG8_MINC  ((uint16_t)0x0080) //!< Memory increment mode

#define DMA_CHCFG8_PSIZE   , ((uint16_t)0x0300) //!< PSIZE[1:0] bits (Peripheral size)
#define DMA_CHCFG8_PSIZE_0 ((uint16_t)0x0100)   //!< Bit 0
#define DMA_CHCFG8_PSIZE_1 ((uint16_t)0x0200)   //!< Bit 1

#define DMA_CHCFG8_MSIZE   ((uint16_t)0x0C00) //!< MSIZE[1:0] bits (Memory size)
#define DMA_CHCFG8_MSIZE_0 ((uint16_t)0x0400) //!< Bit 0
#define DMA_CHCFG8_MSIZE_1 ((uint16_t)0x0800) //!< Bit 1

#define DMA_CHCFG8_PRIOLVL   ((uint16_t)0x3000) //!< PL[1:0] bits (Channel Priority level)
#define DMA_CHCFG8_PRIOLVL_0 ((uint16_t)0x1000) //!< Bit 0
#define DMA_CHCFG8_PRIOLVL_1 ((uint16_t)0x2000) //!< Bit 1

#define DMA_CHCFG8_MEM2MEM ((uint16_t)0x4000) //!< Memory to memory mode enable

/******************  Bit definition for DMA_TXNUM1 register  ******************/
#define DMA_TXNUM1_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM2 register  ******************/
#define DMA_TXNUM2_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM3 register  ******************/
#define DMA_TXNUM3_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM4 register  ******************/
#define DMA_TXNUM4_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM5 register  ******************/
#define DMA_TXNUM5_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM6 register  ******************/
#define DMA_TXNUM6_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM7 register  ******************/
#define DMA_TXNUM7_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_TXNUM8 register  ******************/
#define DMA_TXNUM8_NDTX ((uint16_t)0xFFFF) //!< Number of data to Transfer

/******************  Bit definition for DMA_PADDR1 register  *******************/
#define DMA_PADDR1_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR2 register  *******************/
#define DMA_PADDR2_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR3 register  *******************/
#define DMA_PADDR3_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR4 register  *******************/
#define DMA_PADDR4_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR5 register  *******************/
#define DMA_PADDR5_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR6 register  *******************/
#define DMA_PADDR6_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR7 register  *******************/
#define DMA_PADDR7_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_PADDR8 register  *******************/
#define DMA_PADDR8_ADDR ((uint32_t)0xFFFFFFFF) //!< Peripheral Address

/******************  Bit definition for DMA_MADDR1 register  *******************/
#define DMA_MADDR1_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR2 register  *******************/
#define DMA_MADDR2_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR3 register  *******************/
#define DMA_MADDR3_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR4 register  *******************/
#define DMA_MADDR4_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR5 register  *******************/
#define DMA_MADDR5_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR6 register  *******************/
#define DMA_MADDR6_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR7 register  *******************/
#define DMA_MADDR7_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_MADDR8 register  *******************/
#define DMA_MADDR8_ADDR ((uint32_t)0xFFFFFFFF) //!< Memory Address

/******************  Bit definition for DMA_CHSEL1 register  *******************/
#define DMA_CHSEL1_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL2 register  *******************/
#define DMA_CHSEL2_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL3 register  *******************/
#define DMA_CHSEL3_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL4 register  *******************/
#define DMA_CHSEL4_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL5 register  *******************/
#define DMA_CHSEL5_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL6 register  *******************/
#define DMA_CHSEL6_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL7 register  *******************/
#define DMA_CHSEL7_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHSEL8 register  *******************/
#define DMA_CHSEL8_CH_SEL ((uint32_t)0x0000003F) //!< Channel Select

/******************  Bit definition for DMA_CHMAPEN register  *******************/
#define DMA_CHMAPEN_MAP_EN ((uint32_t)0x00000001) //!< Channel Map Enable

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_CH1_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF1 | DMA_INTSTS_TXCF1 | DMA_INTSTS_HTXF1 | DMA_INTSTS_ERRF1))
#define DMA1_CH2_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF2 | DMA_INTSTS_TXCF2 | DMA_INTSTS_HTXF2 | DMA_INTSTS_ERRF2))
#define DMA1_CH3_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF3 | DMA_INTSTS_TXCF3 | DMA_INTSTS_HTXF3 | DMA_INTSTS_ERRF3))
#define DMA1_CH4_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF4 | DMA_INTSTS_TXCF4 | DMA_INTSTS_HTXF4 | DMA_INTSTS_ERRF4))
#define DMA1_CH5_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF5 | DMA_INTSTS_TXCF5 | DMA_INTSTS_HTXF5 | DMA_INTSTS_ERRF5))
#define DMA1_CH6_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF6 | DMA_INTSTS_TXCF6 | DMA_INTSTS_HTXF6 | DMA_INTSTS_ERRF6))
#define DMA1_CH7_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF7 | DMA_INTSTS_TXCF7 | DMA_INTSTS_HTXF7 | DMA_INTSTS_ERRF7))
#define DMA1_CH8_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF8 | DMA_INTSTS_TXCF8 | DMA_INTSTS_HTXF8 | DMA_INTSTS_ERRF8))

/* DMA2 Channelx interrupt pending bit masks */
#define DMA2_CH1_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF1 | DMA_INTSTS_TXCF1 | DMA_INTSTS_HTXF1 | DMA_INTSTS_ERRF1))
#define DMA2_CH2_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF2 | DMA_INTSTS_TXCF2 | DMA_INTSTS_HTXF2 | DMA_INTSTS_ERRF2))
#define DMA2_CH3_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF3 | DMA_INTSTS_TXCF3 | DMA_INTSTS_HTXF3 | DMA_INTSTS_ERRF3))
#define DMA2_CH4_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF4 | DMA_INTSTS_TXCF4 | DMA_INTSTS_HTXF4 | DMA_INTSTS_ERRF4))
#define DMA2_CH5_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF5 | DMA_INTSTS_TXCF5 | DMA_INTSTS_HTXF5 | DMA_INTSTS_ERRF5))
#define DMA2_CH6_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF6 | DMA_INTSTS_TXCF6 | DMA_INTSTS_HTXF6 | DMA_INTSTS_ERRF6))
#define DMA2_CH7_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF7 | DMA_INTSTS_TXCF7 | DMA_INTSTS_HTXF7 | DMA_INTSTS_ERRF7))
#define DMA2_CH8_INT_MASK ((uint32_t)(DMA_INTSTS_GLBF8 | DMA_INTSTS_TXCF8 | DMA_INTSTS_HTXF8 | DMA_INTSTS_ERRF8))

typedef struct {
  __IO uint32_t INTSTS;
  __IO uint32_t INTCLR;
  __IO DMA_ChannelType DMA_Channel[8];
  __IO uint32_t CHMAPEN;
} DMA_Module;

#define RCC_AHB_PERIPH_ADC1   ((uint32_t)0x00001000)
#define RCC_AHB_PERIPH_ADC2   ((uint32_t)0x00002000)
#define RCC_AHB_PERIPH_ADC3   ((uint32_t)0x00004000)
#define RCC_AHB_PERIPH_ADC4   ((uint32_t)0x00008000)

void ADC_Init(ADC_Module* NS_ADCx, ADC_InitType* ADC_InitStruct);

/**================================================================
 *        ADC reset
 ================================================================*/
void ADC_DeInit(ADC_Module* NS_ADCx);

/**================================================================
 *        ADC module enable
 ================================================================*/
void ADC_Enable(ADC_Module* NS_ADCx, uint32_t Cmd);

/**================================================================
 *        Get the ADC status logo bit
 ================================================================*/
uint32_t ADC_GetFlagStatusNew(ADC_Module* NS_ADCx, uint8_t ADC_FLAG_NEW);

/**================================================================
 *        Open ADC calibration
 ================================================================*/
void ADC_StartCalibration(ADC_Module* NS_ADCx);

/**================================================================
 *        Enable ADC DMA
 ================================================================*/
void ADC_EnableDMA(ADC_Module* NS_ADCx, uint32_t Cmd);

/**================================================================
 *        Configure ADC interrupt enable enable
 ================================================================*/
void ADC_ConfigInt(ADC_Module* NS_ADCx, uint16_t ADC_IT, uint32_t Cmd);

/**================================================================
 *        Get ADC calibration status
 ================================================================*/
uint32_t ADC_GetCalibrationStatus(ADC_Module* NS_ADCx);

/**================================================================
 *        Configure the ADC channel
 ================================================================*/
void ADC_ConfigRegularChannel(ADC_Module* NS_ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

/**================================================================
 *        Start ADC conversion
 ================================================================*/
void ADC_EnableSoftwareStartConv(ADC_Module* NS_ADCx, uint32_t Cmd);

/**================================================================
 *        Get the ADC status logo bit
 ================================================================*/
uint32_t ADC_GetFlagStatus(ADC_Module* NS_ADCx, uint8_t ADC_FLAG);

/**================================================================
 *        Clear status logo bit
 ================================================================*/
void ADC_ClearFlag(ADC_Module* NS_ADCx, uint8_t ADC_FLAG);

/**================================================================
 *        Get ADC sampling value
 ================================================================*/
uint16_t ADC_GetDat(ADC_Module* NS_ADCx);

////////////////////////////////////////////////////////////////////////////////

typedef struct {
  __IO uint32_t CR;         /* Completely compatible */
  __IO uint32_t CFGR;       /* Not compatible: ADC frequency is not set here */
  __IO uint32_t CIR;        /* Completely compatible */

  __IO uint32_t APB2RSTR;   /* Completely compatible */
  __IO uint32_t APB1RSTR;   /* Completely compatible */

  __IO uint32_t AHBENR;     /* Not compatible: ADC clock enables settings here */
  __IO uint32_t APB2ENR;    /* Not compatible: ADC clock enables to be here */
  __IO uint32_t APB1ENR;    /* compatible */
  __IO uint32_t BDCR;       /* compatible */
  __IO uint32_t CSR;        /* compatible */

  __IO uint32_t AHBRSTR;    /* Not compatible, ADC reset here settings */
  __IO uint32_t CFGR2;      /* Not compatible, ADC clock settings here */
  __IO uint32_t CFGR3;      /* Not compatible, add a new register */

} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *) ADC_RCC_BASE)

/**================================================================
 *        Initialize ADC clock
 ================================================================*/

void enable_adc_clk(uint8_t cmd);

/**================================================================
 *        Initialize ADC peripheral parameters
 ================================================================*/
void ADC_Initial(ADC_Module* NS_ADCx);

/**================================================================
 *        Single independent sampling
 ================================================================*/
uint16_t ADC_GetData(ADC_Module* NS_ADCx, uint8_t ADC_Channel);

void DMA_DeInit(DMA_ChannelType* DMAyChx);

#define CCR_CLEAR_Mask ((uint32_t)0xFFFF800F)

void DMA_Init(DMA_ChannelType* DMAyChx, DMA_InitType* DMA_InitParam);

void DMA_EnableChannel(DMA_ChannelType* DMAyChx, uint32_t Cmd);

#define USE_ADC       NS_ADC2
#define USE_DMA_CH    DMA1_CH8

/**================================================================
 *        Initialize the DMA of ADC
 ================================================================*/
void ADC_DMA_init();
