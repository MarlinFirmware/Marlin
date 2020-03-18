/**
  ******************************************************************************
  * @file    stm32f10x_rcc.h
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    05-March-2012
  * @brief   This file contains all the functions prototypes for the RCC firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_RCC_H
#define __STM32F10x_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */

/** @defgroup RCC_Exported_Types
  * @{
  */

typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t ADCCLK_Frequency;  /*!< returns ADCCLK clock frequency expressed in Hz */
}RCC_ClocksTypeDef;

/**
  * @}
  */

/** @defgroup RCC_Exported_Constants
  * @{
  */

/** @defgroup HSE_configuration
  * @{
  */

#define RCC_HSE_OFF                      ((uint32_t)0x00000000)
#define RCC_HSE_ON                       ((uint32_t)0x00010000)
#define RCC_HSE_Bypass                   ((uint32_t)0x00040000)
#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_Bypass))

/**
  * @}
  */

/** @defgroup PLL_entry_clock_source
  * @{
  */

#define RCC_PLLSource_HSI_Div2           ((uint32_t)0x00000000)

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL) && !defined (STM32F10X_CL)
 #define RCC_PLLSource_HSE_Div1           ((uint32_t)0x00010000)
 #define RCC_PLLSource_HSE_Div2           ((uint32_t)0x00030000)
 #define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_HSI_Div2) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div1) || \
                                   ((SOURCE) == RCC_PLLSource_HSE_Div2))
#else
 #define RCC_PLLSource_PREDIV1            ((uint32_t)0x00010000)
 #define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_HSI_Div2) || \
                                   ((SOURCE) == RCC_PLLSource_PREDIV1))
#endif /* STM32F10X_CL */

/**
  * @}
  */

/** @defgroup PLL_multiplication_factor
  * @{
  */
#ifndef STM32F10X_CL
 #define RCC_PLLMul_2                    ((uint32_t)0x00000000)
 #define RCC_PLLMul_3                    ((uint32_t)0x00040000)
 #define RCC_PLLMul_4                    ((uint32_t)0x00080000)
 #define RCC_PLLMul_5                    ((uint32_t)0x000C0000)
 #define RCC_PLLMul_6                    ((uint32_t)0x00100000)
 #define RCC_PLLMul_7                    ((uint32_t)0x00140000)
 #define RCC_PLLMul_8                    ((uint32_t)0x00180000)
 #define RCC_PLLMul_9                    ((uint32_t)0x001C0000)
 #define RCC_PLLMul_10                   ((uint32_t)0x00200000)
 #define RCC_PLLMul_11                   ((uint32_t)0x00240000)
 #define RCC_PLLMul_12                   ((uint32_t)0x00280000)
 #define RCC_PLLMul_13                   ((uint32_t)0x002C0000)
 #define RCC_PLLMul_14                   ((uint32_t)0x00300000)
 #define RCC_PLLMul_15                   ((uint32_t)0x00340000)
 #define RCC_PLLMul_16                   ((uint32_t)0x00380000)
 #define IS_RCC_PLL_MUL(MUL) (((MUL) == RCC_PLLMul_2) || ((MUL) == RCC_PLLMul_3)   || \
                              ((MUL) == RCC_PLLMul_4) || ((MUL) == RCC_PLLMul_5)   || \
                              ((MUL) == RCC_PLLMul_6) || ((MUL) == RCC_PLLMul_7)   || \
                              ((MUL) == RCC_PLLMul_8) || ((MUL) == RCC_PLLMul_9)   || \
                              ((MUL) == RCC_PLLMul_10) || ((MUL) == RCC_PLLMul_11) || \
                              ((MUL) == RCC_PLLMul_12) || ((MUL) == RCC_PLLMul_13) || \
                              ((MUL) == RCC_PLLMul_14) || ((MUL) == RCC_PLLMul_15) || \
                              ((MUL) == RCC_PLLMul_16))

#else
 #define RCC_PLLMul_4                    ((uint32_t)0x00080000)
 #define RCC_PLLMul_5                    ((uint32_t)0x000C0000)
 #define RCC_PLLMul_6                    ((uint32_t)0x00100000)
 #define RCC_PLLMul_7                    ((uint32_t)0x00140000)
 #define RCC_PLLMul_8                    ((uint32_t)0x00180000)
 #define RCC_PLLMul_9                    ((uint32_t)0x001C0000)
 #define RCC_PLLMul_6_5                  ((uint32_t)0x00340000)

 #define IS_RCC_PLL_MUL(MUL) (((MUL) == RCC_PLLMul_4) || ((MUL) == RCC_PLLMul_5) || \
                              ((MUL) == RCC_PLLMul_6) || ((MUL) == RCC_PLLMul_7) || \
                              ((MUL) == RCC_PLLMul_8) || ((MUL) == RCC_PLLMul_9) || \
                              ((MUL) == RCC_PLLMul_6_5))
#endif /* STM32F10X_CL */
/**
  * @}
  */

/** @defgroup PREDIV1_division_factor
  * @{
  */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
 #define  RCC_PREDIV1_Div1               ((uint32_t)0x00000000)
 #define  RCC_PREDIV1_Div2               ((uint32_t)0x00000001)
 #define  RCC_PREDIV1_Div3               ((uint32_t)0x00000002)
 #define  RCC_PREDIV1_Div4               ((uint32_t)0x00000003)
 #define  RCC_PREDIV1_Div5               ((uint32_t)0x00000004)
 #define  RCC_PREDIV1_Div6               ((uint32_t)0x00000005)
 #define  RCC_PREDIV1_Div7               ((uint32_t)0x00000006)
 #define  RCC_PREDIV1_Div8               ((uint32_t)0x00000007)
 #define  RCC_PREDIV1_Div9               ((uint32_t)0x00000008)
 #define  RCC_PREDIV1_Div10              ((uint32_t)0x00000009)
 #define  RCC_PREDIV1_Div11              ((uint32_t)0x0000000A)
 #define  RCC_PREDIV1_Div12              ((uint32_t)0x0000000B)
 #define  RCC_PREDIV1_Div13              ((uint32_t)0x0000000C)
 #define  RCC_PREDIV1_Div14              ((uint32_t)0x0000000D)
 #define  RCC_PREDIV1_Div15              ((uint32_t)0x0000000E)
 #define  RCC_PREDIV1_Div16              ((uint32_t)0x0000000F)

 #define IS_RCC_PREDIV1(PREDIV1) (((PREDIV1) == RCC_PREDIV1_Div1) || ((PREDIV1) == RCC_PREDIV1_Div2) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div3) || ((PREDIV1) == RCC_PREDIV1_Div4) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div5) || ((PREDIV1) == RCC_PREDIV1_Div6) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div7) || ((PREDIV1) == RCC_PREDIV1_Div8) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div9) || ((PREDIV1) == RCC_PREDIV1_Div10) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div11) || ((PREDIV1) == RCC_PREDIV1_Div12) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div13) || ((PREDIV1) == RCC_PREDIV1_Div14) || \
                                  ((PREDIV1) == RCC_PREDIV1_Div15) || ((PREDIV1) == RCC_PREDIV1_Div16))
#endif
/**
  * @}
  */


/** @defgroup PREDIV1_clock_source
  * @{
  */
#ifdef STM32F10X_CL
/* PREDIV1 clock source (for STM32 connectivity line devices) */
 #define  RCC_PREDIV1_Source_HSE         ((uint32_t)0x00000000)
 #define  RCC_PREDIV1_Source_PLL2        ((uint32_t)0x00010000)

 #define IS_RCC_PREDIV1_SOURCE(SOURCE) (((SOURCE) == RCC_PREDIV1_Source_HSE) || \
                                        ((SOURCE) == RCC_PREDIV1_Source_PLL2))
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
/* PREDIV1 clock source (for STM32 Value line devices) */
 #define  RCC_PREDIV1_Source_HSE         ((uint32_t)0x00000000)

 #define IS_RCC_PREDIV1_SOURCE(SOURCE) (((SOURCE) == RCC_PREDIV1_Source_HSE))
#endif
/**
  * @}
  */

#ifdef STM32F10X_CL
/** @defgroup PREDIV2_division_factor
  * @{
  */

 #define  RCC_PREDIV2_Div1               ((uint32_t)0x00000000)
 #define  RCC_PREDIV2_Div2               ((uint32_t)0x00000010)
 #define  RCC_PREDIV2_Div3               ((uint32_t)0x00000020)
 #define  RCC_PREDIV2_Div4               ((uint32_t)0x00000030)
 #define  RCC_PREDIV2_Div5               ((uint32_t)0x00000040)
 #define  RCC_PREDIV2_Div6               ((uint32_t)0x00000050)
 #define  RCC_PREDIV2_Div7               ((uint32_t)0x00000060)
 #define  RCC_PREDIV2_Div8               ((uint32_t)0x00000070)
 #define  RCC_PREDIV2_Div9               ((uint32_t)0x00000080)
 #define  RCC_PREDIV2_Div10              ((uint32_t)0x00000090)
 #define  RCC_PREDIV2_Div11              ((uint32_t)0x000000A0)
 #define  RCC_PREDIV2_Div12              ((uint32_t)0x000000B0)
 #define  RCC_PREDIV2_Div13              ((uint32_t)0x000000C0)
 #define  RCC_PREDIV2_Div14              ((uint32_t)0x000000D0)
 #define  RCC_PREDIV2_Div15              ((uint32_t)0x000000E0)
 #define  RCC_PREDIV2_Div16              ((uint32_t)0x000000F0)

 #define IS_RCC_PREDIV2(PREDIV2) (((PREDIV2) == RCC_PREDIV2_Div1) || ((PREDIV2) == RCC_PREDIV2_Div2) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div3) || ((PREDIV2) == RCC_PREDIV2_Div4) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div5) || ((PREDIV2) == RCC_PREDIV2_Div6) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div7) || ((PREDIV2) == RCC_PREDIV2_Div8) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div9) || ((PREDIV2) == RCC_PREDIV2_Div10) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div11) || ((PREDIV2) == RCC_PREDIV2_Div12) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div13) || ((PREDIV2) == RCC_PREDIV2_Div14) || \
                                  ((PREDIV2) == RCC_PREDIV2_Div15) || ((PREDIV2) == RCC_PREDIV2_Div16))
/**
  * @}
  */


/** @defgroup PLL2_multiplication_factor
  * @{
  */

 #define  RCC_PLL2Mul_8                  ((uint32_t)0x00000600)
 #define  RCC_PLL2Mul_9                  ((uint32_t)0x00000700)
 #define  RCC_PLL2Mul_10                 ((uint32_t)0x00000800)
 #define  RCC_PLL2Mul_11                 ((uint32_t)0x00000900)
 #define  RCC_PLL2Mul_12                 ((uint32_t)0x00000A00)
 #define  RCC_PLL2Mul_13                 ((uint32_t)0x00000B00)
 #define  RCC_PLL2Mul_14                 ((uint32_t)0x00000C00)
 #define  RCC_PLL2Mul_16                 ((uint32_t)0x00000E00)
 #define  RCC_PLL2Mul_20                 ((uint32_t)0x00000F00)

 #define IS_RCC_PLL2_MUL(MUL) (((MUL) == RCC_PLL2Mul_8) || ((MUL) == RCC_PLL2Mul_9)  || \
                               ((MUL) == RCC_PLL2Mul_10) || ((MUL) == RCC_PLL2Mul_11) || \
                               ((MUL) == RCC_PLL2Mul_12) || ((MUL) == RCC_PLL2Mul_13) || \
                               ((MUL) == RCC_PLL2Mul_14) || ((MUL) == RCC_PLL2Mul_16) || \
                               ((MUL) == RCC_PLL2Mul_20))
/**
  * @}
  */


/** @defgroup PLL3_multiplication_factor
  * @{
  */

 #define  RCC_PLL3Mul_8                  ((uint32_t)0x00006000)
 #define  RCC_PLL3Mul_9                  ((uint32_t)0x00007000)
 #define  RCC_PLL3Mul_10                 ((uint32_t)0x00008000)
 #define  RCC_PLL3Mul_11                 ((uint32_t)0x00009000)
 #define  RCC_PLL3Mul_12                 ((uint32_t)0x0000A000)
 #define  RCC_PLL3Mul_13                 ((uint32_t)0x0000B000)
 #define  RCC_PLL3Mul_14                 ((uint32_t)0x0000C000)
 #define  RCC_PLL3Mul_16                 ((uint32_t)0x0000E000)
 #define  RCC_PLL3Mul_20                 ((uint32_t)0x0000F000)

 #define IS_RCC_PLL3_MUL(MUL) (((MUL) == RCC_PLL3Mul_8) || ((MUL) == RCC_PLL3Mul_9)  || \
                               ((MUL) == RCC_PLL3Mul_10) || ((MUL) == RCC_PLL3Mul_11) || \
                               ((MUL) == RCC_PLL3Mul_12) || ((MUL) == RCC_PLL3Mul_13) || \
                               ((MUL) == RCC_PLL3Mul_14) || ((MUL) == RCC_PLL3Mul_16) || \
                               ((MUL) == RCC_PLL3Mul_20))
/**
  * @}
  */

#endif /* STM32F10X_CL */


/** @defgroup System_clock_source
  * @{
  */

#define RCC_SYSCLKSource_HSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSource_HSE             ((uint32_t)0x00000001)
#define RCC_SYSCLKSource_PLLCLK          ((uint32_t)0x00000002)
#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSource_HSI) || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSE) || \
                                      ((SOURCE) == RCC_SYSCLKSource_PLLCLK))
/**
  * @}
  */

/** @defgroup AHB_clock_source
  * @{
  */

#define RCC_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x00000080)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x00000090)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x000000A0)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div128                ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div256                ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div512                ((uint32_t)0x000000F0)
#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2) || \
                           ((HCLK) == RCC_SYSCLK_Div4) || ((HCLK) == RCC_SYSCLK_Div8) || \
                           ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                           ((HCLK) == RCC_SYSCLK_Div128) || ((HCLK) == RCC_SYSCLK_Div256) || \
                           ((HCLK) == RCC_SYSCLK_Div512))
/**
  * @}
  */

/** @defgroup APB1_APB2_clock_source
  * @{
  */

#define RCC_HCLK_Div1                    ((uint32_t)0x00000000)
#define RCC_HCLK_Div2                    ((uint32_t)0x00000400)
#define RCC_HCLK_Div4                    ((uint32_t)0x00000500)
#define RCC_HCLK_Div8                    ((uint32_t)0x00000600)
#define RCC_HCLK_Div16                   ((uint32_t)0x00000700)
#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                           ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8) || \
                           ((PCLK) == RCC_HCLK_Div16))
/**
  * @}
  */

/** @defgroup RCC_Interrupt_source
  * @{
  */

#define RCC_IT_LSIRDY                    ((uint8_t)0x01)
#define RCC_IT_LSERDY                    ((uint8_t)0x02)
#define RCC_IT_HSIRDY                    ((uint8_t)0x04)
#define RCC_IT_HSERDY                    ((uint8_t)0x08)
#define RCC_IT_PLLRDY                    ((uint8_t)0x10)
#define RCC_IT_CSS                       ((uint8_t)0x80)

#ifndef STM32F10X_CL
 #define IS_RCC_IT(IT) ((((IT) & (uint8_t)0xE0) == 0x00) && ((IT) != 0x00))
 #define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                            ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                            ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_CSS))
 #define IS_RCC_CLEAR_IT(IT) ((((IT) & (uint8_t)0x60) == 0x00) && ((IT) != 0x00))
#else
 #define RCC_IT_PLL2RDY                  ((uint8_t)0x20)
 #define RCC_IT_PLL3RDY                  ((uint8_t)0x40)
 #define IS_RCC_IT(IT) ((((IT) & (uint8_t)0x80) == 0x00) && ((IT) != 0x00))
 #define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                            ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                            ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_CSS) || \
                            ((IT) == RCC_IT_PLL2RDY) || ((IT) == RCC_IT_PLL3RDY))
 #define IS_RCC_CLEAR_IT(IT) ((IT) != 0x00)
#endif /* STM32F10X_CL */


/**
  * @}
  */

#ifndef STM32F10X_CL
/** @defgroup USB_Device_clock_source
  * @{
  */

 #define RCC_USBCLKSource_PLLCLK_1Div5   ((uint8_t)0x00)
 #define RCC_USBCLKSource_PLLCLK_Div1    ((uint8_t)0x01)

 #define IS_RCC_USBCLK_SOURCE(SOURCE) (((SOURCE) == RCC_USBCLKSource_PLLCLK_1Div5) || \
                                      ((SOURCE) == RCC_USBCLKSource_PLLCLK_Div1))
/**
  * @}
  */
#else
/** @defgroup USB_OTG_FS_clock_source
  * @{
  */
 #define RCC_OTGFSCLKSource_PLLVCO_Div3    ((uint8_t)0x00)
 #define RCC_OTGFSCLKSource_PLLVCO_Div2    ((uint8_t)0x01)

 #define IS_RCC_OTGFSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_OTGFSCLKSource_PLLVCO_Div3) || \
                                         ((SOURCE) == RCC_OTGFSCLKSource_PLLVCO_Div2))
/**
  * @}
  */
#endif /* STM32F10X_CL */


#ifdef STM32F10X_CL
/** @defgroup I2S2_clock_source
  * @{
  */
 #define RCC_I2S2CLKSource_SYSCLK        ((uint8_t)0x00)
 #define RCC_I2S2CLKSource_PLL3_VCO      ((uint8_t)0x01)

 #define IS_RCC_I2S2CLK_SOURCE(SOURCE) (((SOURCE) == RCC_I2S2CLKSource_SYSCLK) || \
                                        ((SOURCE) == RCC_I2S2CLKSource_PLL3_VCO))
/**
  * @}
  */

/** @defgroup I2S3_clock_source
  * @{
  */
 #define RCC_I2S3CLKSource_SYSCLK        ((uint8_t)0x00)
 #define RCC_I2S3CLKSource_PLL3_VCO      ((uint8_t)0x01)

 #define IS_RCC_I2S3CLK_SOURCE(SOURCE) (((SOURCE) == RCC_I2S3CLKSource_SYSCLK) || \
                                        ((SOURCE) == RCC_I2S3CLKSource_PLL3_VCO))
/**
  * @}
  */
#endif /* STM32F10X_CL */


/** @defgroup ADC_clock_source
  * @{
  */

#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)
#define IS_RCC_ADCCLK(ADCCLK) (((ADCCLK) == RCC_PCLK2_Div2) || ((ADCCLK) == RCC_PCLK2_Div4) || \
                               ((ADCCLK) == RCC_PCLK2_Div6) || ((ADCCLK) == RCC_PCLK2_Div8))
/**
  * @}
  */

/** @defgroup LSE_configuration
  * @{
  */

#define RCC_LSE_OFF                      ((uint8_t)0x00)
#define RCC_LSE_ON                       ((uint8_t)0x01)
#define RCC_LSE_Bypass                   ((uint8_t)0x04)
#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || ((LSE) == RCC_LSE_ON) || \
                         ((LSE) == RCC_LSE_Bypass))
/**
  * @}
  */

/** @defgroup RTC_clock_source
  * @{
  */

#define RCC_RTCCLKSource_LSE             ((uint32_t)0x00000100)
#define RCC_RTCCLKSource_LSI             ((uint32_t)0x00000200)
#define RCC_RTCCLKSource_HSE_Div128      ((uint32_t)0x00000300)
#define IS_RCC_RTCCLK_SOURCE(SOURCE) (((SOURCE) == RCC_RTCCLKSource_LSE) || \
                                      ((SOURCE) == RCC_RTCCLKSource_LSI) || \
                                      ((SOURCE) == RCC_RTCCLKSource_HSE_Div128))
/**
  * @}
  */

/** @defgroup AHB_peripheral
  * @{
  */

#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)
#define RCC_AHBPeriph_DMA2               ((uint32_t)0x00000002)
#define RCC_AHBPeriph_SRAM               ((uint32_t)0x00000004)
#define RCC_AHBPeriph_FLITF              ((uint32_t)0x00000010)
#define RCC_AHBPeriph_CRC                ((uint32_t)0x00000040)

#ifndef STM32F10X_CL
 #define RCC_AHBPeriph_FSMC              ((uint32_t)0x00000100)
 #define RCC_AHBPeriph_SDIO              ((uint32_t)0x00000400)
 #define IS_RCC_AHB_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFAA8) == 0x00) && ((PERIPH) != 0x00))
#else
 #define RCC_AHBPeriph_OTG_FS            ((uint32_t)0x00001000)
 #define RCC_AHBPeriph_ETH_MAC           ((uint32_t)0x00004000)
 #define RCC_AHBPeriph_ETH_MAC_Tx        ((uint32_t)0x00008000)
 #define RCC_AHBPeriph_ETH_MAC_Rx        ((uint32_t)0x00010000)

 #define IS_RCC_AHB_PERIPH(PERIPH) ((((PERIPH) & 0xFFFE2FA8) == 0x00) && ((PERIPH) != 0x00))
 #define IS_RCC_AHB_PERIPH_RESET(PERIPH) ((((PERIPH) & 0xFFFFAFFF) == 0x00) && ((PERIPH) != 0x00))
#endif /* STM32F10X_CL */
/**
  * @}
  */

/** @defgroup APB2_peripheral
  * @{
  */

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2              ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3              ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM15             ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM16             ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM17             ((uint32_t)0x00040000)
#define RCC_APB2Periph_TIM9              ((uint32_t)0x00080000)
#define RCC_APB2Periph_TIM10             ((uint32_t)0x00100000)
#define RCC_APB2Periph_TIM11             ((uint32_t)0x00200000)

#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFC00002) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */

/** @defgroup APB1_peripheral
  * @{
  */

#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3              ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5              ((uint32_t)0x00000008)
#define RCC_APB1Periph_TIM6              ((uint32_t)0x00000010)
#define RCC_APB1Periph_TIM7              ((uint32_t)0x00000020)
#define RCC_APB1Periph_TIM12             ((uint32_t)0x00000040)
#define RCC_APB1Periph_TIM13             ((uint32_t)0x00000080)
#define RCC_APB1Periph_TIM14             ((uint32_t)0x00000100)
#define RCC_APB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCC_APB1Periph_SPI2              ((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3              ((uint32_t)0x00008000)
#define RCC_APB1Periph_USART2            ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4             ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5             ((uint32_t)0x00100000)
#define RCC_APB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2              ((uint32_t)0x00400000)
#define RCC_APB1Periph_USB               ((uint32_t)0x00800000)
#define RCC_APB1Periph_CAN1              ((uint32_t)0x02000000)
#define RCC_APB1Periph_CAN2              ((uint32_t)0x04000000)
#define RCC_APB1Periph_BKP               ((uint32_t)0x08000000)
#define RCC_APB1Periph_PWR               ((uint32_t)0x10000000)
#define RCC_APB1Periph_DAC               ((uint32_t)0x20000000)
#define RCC_APB1Periph_CEC               ((uint32_t)0x40000000)

#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH) & 0x81013600) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */

/** @defgroup Clock_source_to_output_on_MCO_pin
  * @{
  */

#define RCC_MCO_NoClock                  ((uint8_t)0x00)
#define RCC_MCO_SYSCLK                   ((uint8_t)0x04)
#define RCC_MCO_HSI                      ((uint8_t)0x05)
#define RCC_MCO_HSE                      ((uint8_t)0x06)
#define RCC_MCO_PLLCLK_Div2              ((uint8_t)0x07)

#ifndef STM32F10X_CL
 #define IS_RCC_MCO(MCO) (((MCO) == RCC_MCO_NoClock) || ((MCO) == RCC_MCO_HSI) || \
                          ((MCO) == RCC_MCO_SYSCLK)  || ((MCO) == RCC_MCO_HSE) || \
                          ((MCO) == RCC_MCO_PLLCLK_Div2))
#else
 #define RCC_MCO_PLL2CLK                 ((uint8_t)0x08)
 #define RCC_MCO_PLL3CLK_Div2            ((uint8_t)0x09)
 #define RCC_MCO_XT1                     ((uint8_t)0x0A)
 #define RCC_MCO_PLL3CLK                 ((uint8_t)0x0B)

 #define IS_RCC_MCO(MCO) (((MCO) == RCC_MCO_NoClock) || ((MCO) == RCC_MCO_HSI) || \
                          ((MCO) == RCC_MCO_SYSCLK)  || ((MCO) == RCC_MCO_HSE) || \
                          ((MCO) == RCC_MCO_PLLCLK_Div2) || ((MCO) == RCC_MCO_PLL2CLK) || \
                          ((MCO) == RCC_MCO_PLL3CLK_Div2) || ((MCO) == RCC_MCO_XT1) || \
                          ((MCO) == RCC_MCO_PLL3CLK))
#endif /* STM32F10X_CL */

/**
  * @}
  */

/** @defgroup RCC_Flag
  * @{
  */

#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7F)

#ifndef STM32F10X_CL
 #define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_HSIRDY) || ((FLAG) == RCC_FLAG_HSERDY) || \
                            ((FLAG) == RCC_FLAG_PLLRDY) || ((FLAG) == RCC_FLAG_LSERDY) || \
                            ((FLAG) == RCC_FLAG_LSIRDY) || ((FLAG) == RCC_FLAG_PINRST) || \
                            ((FLAG) == RCC_FLAG_PORRST) || ((FLAG) == RCC_FLAG_SFTRST) || \
                            ((FLAG) == RCC_FLAG_IWDGRST)|| ((FLAG) == RCC_FLAG_WWDGRST)|| \
                            ((FLAG) == RCC_FLAG_LPWRRST))
#else
 #define RCC_FLAG_PLL2RDY                ((uint8_t)0x3B)
 #define RCC_FLAG_PLL3RDY                ((uint8_t)0x3D)
 #define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_HSIRDY) || ((FLAG) == RCC_FLAG_HSERDY) || \
                            ((FLAG) == RCC_FLAG_PLLRDY) || ((FLAG) == RCC_FLAG_LSERDY) || \
                            ((FLAG) == RCC_FLAG_PLL2RDY) || ((FLAG) == RCC_FLAG_PLL3RDY) || \
                            ((FLAG) == RCC_FLAG_LSIRDY) || ((FLAG) == RCC_FLAG_PINRST) || \
                            ((FLAG) == RCC_FLAG_PORRST) || ((FLAG) == RCC_FLAG_SFTRST) || \
                            ((FLAG) == RCC_FLAG_IWDGRST)|| ((FLAG) == RCC_FLAG_WWDGRST)|| \
                            ((FLAG) == RCC_FLAG_LPWRRST))
#endif /* STM32F10X_CL */

#define IS_RCC_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RCC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions
  * @{
  */

void RCC_DeInit(void);
void RCC_HSEConfig(uint32_t RCC_HSE);
ErrorStatus RCC_WaitForHSEStartUp(void);
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void RCC_HSICmd(FunctionalState NewState);
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul);
void RCC_PLLCmd(FunctionalState NewState);

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
 void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div);
#endif

#ifdef  STM32F10X_CL
 void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div);
 void RCC_PLL2Config(uint32_t RCC_PLL2Mul);
 void RCC_PLL2Cmd(FunctionalState NewState);
 void RCC_PLL3Config(uint32_t RCC_PLL3Mul);
 void RCC_PLL3Cmd(FunctionalState NewState);
#endif /* STM32F10X_CL */

void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
uint8_t RCC_GetSYSCLKSource(void);
void RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void RCC_PCLK1Config(uint32_t RCC_HCLK);
void RCC_PCLK2Config(uint32_t RCC_HCLK);
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);

#ifndef STM32F10X_CL
 void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource);
#else
 void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource);
#endif /* STM32F10X_CL */

void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);

#ifdef STM32F10X_CL
 void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource);
 void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource);
#endif /* STM32F10X_CL */

void RCC_LSEConfig(uint8_t RCC_LSE);
void RCC_LSICmd(FunctionalState NewState);
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_RTCCLKCmd(FunctionalState NewState);
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

#ifdef STM32F10X_CL
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
#endif /* STM32F10X_CL */

void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_BackupResetCmd(FunctionalState NewState);
void RCC_ClockSecuritySystemCmd(FunctionalState NewState);
void RCC_MCOConfig(uint8_t RCC_MCO);
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);
void RCC_ClearFlag(void);
ITStatus RCC_GetITStatus(uint8_t RCC_IT);
void RCC_ClearITPendingBit(uint8_t RCC_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_RCC_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
