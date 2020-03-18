/**
  ******************************************************************************
  * @file    stm32f10x_rcc.c
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    05-March-2012
  * @brief   This file provides all the RCC firmware functions.
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
#if defined(__STM32F1__) && defined(OVERCLOCK)

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup RCC
  * @brief RCC driver modules
  * @{
  */

/** @defgroup RCC_Private_Defines
  * @{
  */

/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR Register ---*/

/* Alias word address of HSION bit */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))

/* Alias word address of PLLON bit */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

#ifdef STM32F10X_CL
 /* Alias word address of PLL2ON bit */
 #define PLL2ON_BitNumber          0x1A
 #define CR_PLL2ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL2ON_BitNumber * 4))

 /* Alias word address of PLL3ON bit */
 #define PLL3ON_BitNumber          0x1C
 #define CR_PLL3ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL3ON_BitNumber * 4))
#endif /* STM32F10X_CL */

/* Alias word address of CSSON bit */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))

/* --- CFGR Register ---*/

/* Alias word address of USBPRE bit */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)

#ifndef STM32F10X_CL
 #define USBPRE_BitNumber          0x16
 #define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#else
 #define OTGFSPRE_BitNumber        0x16
 #define CFGR_OTGFSPRE_BB          (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (OTGFSPRE_BitNumber * 4))
#endif /* STM32F10X_CL */

/* --- BDCR Register ---*/

/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR Register ---*/

/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

#ifdef STM32F10X_CL
/* --- CFGR2 Register ---*/

 /* Alias word address of I2S2SRC bit */
 #define CFGR2_OFFSET              (RCC_OFFSET + 0x2C)
 #define I2S2SRC_BitNumber         0x11
 #define CFGR2_I2S2SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S2SRC_BitNumber * 4))

 /* Alias word address of I2S3SRC bit */
 #define I2S3SRC_BitNumber         0x12
 #define CFGR2_I2S3SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S3SRC_BitNumber * 4))
#endif /* STM32F10X_CL */

/* ---------------------- RCC registers bit mask ------------------------ */

/* CR register bit mask */
#define CR_HSEBYP_Reset           ((uint32_t)0xFFFBFFFF)
#define CR_HSEBYP_Set             ((uint32_t)0x00040000)
#define CR_HSEON_Reset            ((uint32_t)0xFFFEFFFF)
#define CR_HSEON_Set              ((uint32_t)0x00010000)
#define CR_HSITRIM_Mask           ((uint32_t)0xFFFFFF07)

/* CFGR register bit mask */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC2FFFF)
#else
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC0FFFF)
#endif /* STM32F10X_CL */

#define CFGR_PLLMull_Mask         ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask          ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask        ((uint32_t)0x00020000)
#define CFGR_SWS_Mask             ((uint32_t)0x0000000C)
#define CFGR_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask        ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask       ((uint32_t)0x00000700)
#define CFGR_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask       ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask      ((uint32_t)0x0000C000)

/* CSR register bit mask */
#define CSR_RMVF_Set              ((uint32_t)0x01000000)

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
/* CFGR2 register bit mask */
 #define CFGR2_PREDIV1SRC         ((uint32_t)0x00010000)
 #define CFGR2_PREDIV1            ((uint32_t)0x0000000F)
#endif
#ifdef STM32F10X_CL
 #define CFGR2_PREDIV2            ((uint32_t)0x000000F0)
 #define CFGR2_PLL2MUL            ((uint32_t)0x00000F00)
 #define CFGR2_PLL3MUL            ((uint32_t)0x0000F000)
#endif /* STM32F10X_CL */

/* RCC Flag Mask */
#define FLAG_Mask                 ((uint8_t)0x1F)

/* CIR register byte 2 (Bits[15:8]) base address */
#define CIR_BYTE2_ADDRESS         ((uint32_t)0x40021009)

/* CIR register byte 3 (Bits[23:16]) base address */
#define CIR_BYTE3_ADDRESS         ((uint32_t)0x4002100A)

/* CFGR register byte 4 (Bits[31:24]) base address */
#define CFGR_BYTE4_ADDRESS        ((uint32_t)0x40021007)

/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

/**
  * @}
  */

/** @defgroup RCC_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Private_Variables
  * @{
  */

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

/**
  * @}
  */

/** @defgroup RCC_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Private_Functions
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void RCC_DeInit(void)
{
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* Reset PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x00FF0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#else
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */

}

/**
  * @brief  Configures the External High Speed oscillator (HSE).
  * @note   HSE can not be stopped if it is used directly or through the PLL as system clock.
  * @param  RCC_HSE: specifies the new state of the HSE.
  *   This parameter can be one of the following values:
  *     @arg RCC_HSE_OFF: HSE oscillator OFF
  *     @arg RCC_HSE_ON: HSE oscillator ON
  *     @arg RCC_HSE_Bypass: HSE oscillator bypassed with external clock
  * @retval None
  */
void RCC_HSEConfig(uint32_t RCC_HSE)
{
  /* Check the parameters */
  assert_param(IS_RCC_HSE(RCC_HSE));
  /* Reset HSEON and HSEBYP bits before configuring the HSE ------------------*/
  /* Reset HSEON bit */
  RCC->CR &= CR_HSEON_Reset;
  /* Reset HSEBYP bit */
  RCC->CR &= CR_HSEBYP_Reset;
  /* Configure HSE (RCC_HSE_OFF is already covered by the code section above) */
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* Set HSEON bit */
      RCC->CR |= CR_HSEON_Set;
      break;

    case RCC_HSE_Bypass:
      /* Set HSEBYP and HSEON bits */
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;

    default:
      break;
  }
}

/**
  * @brief  Waits for HSE start-up.
  * @param  None
  * @retval An ErrorStatus enumuration value:
  * - SUCCESS: HSE oscillator is stable and ready to use
  * - ERROR: HSE oscillator not yet ready
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t StartUpCounter = 0;
  ErrorStatus status = ERROR;
  FlagStatus HSEStatus = RESET;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    StartUpCounter++;
  } while((StartUpCounter != HSE_STARTUP_TIMEOUT) && (HSEStatus == RESET));

  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return (status);
}

/**
  * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
  * @param  HSICalibrationValue: specifies the calibration trimming value.
  *   This parameter must be a number between 0 and 0x1F.
  * @retval None
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));
  tmpreg = RCC->CR;
  /* Clear HSITRIM[4:0] bits */
  tmpreg &= CR_HSITRIM_Mask;
  /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value */
  tmpreg |= (uint32_t)HSICalibrationValue << 3;
  /* Store the new value */
  RCC->CR = tmpreg;
}

/**
  * @brief  Enables or disables the Internal High Speed oscillator (HSI).
  * @note   HSI can not be stopped if it is used directly or through the PLL as system clock.
  * @param  NewState: new state of the HSI. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the PLL clock source and multiplication factor.
  * @note   This function must be used only when the PLL is disabled.
  * @param  RCC_PLLSource: specifies the PLL entry clock source.
  *   For @b STM32_Connectivity_line_devices or @b STM32_Value_line_devices,
  *   this parameter can be one of the following values:
  *     @arg RCC_PLLSource_HSI_Div2: HSI oscillator clock divided by 2 selected as PLL clock entry
  *     @arg RCC_PLLSource_PREDIV1: PREDIV1 clock selected as PLL clock entry
  *   For @b other_STM32_devices, this parameter can be one of the following values:
  *     @arg RCC_PLLSource_HSI_Div2: HSI oscillator clock divided by 2 selected as PLL clock entry
  *     @arg RCC_PLLSource_HSE_Div1: HSE oscillator clock selected as PLL clock entry
  *     @arg RCC_PLLSource_HSE_Div2: HSE oscillator clock divided by 2 selected as PLL clock entry
  * @param  RCC_PLLMul: specifies the PLL multiplication factor.
  *   For @b STM32_Connectivity_line_devices, this parameter can be RCC_PLLMul_x where x:{[4,9], 6_5}
  *   For @b other_STM32_devices, this parameter can be RCC_PLLMul_x where x:[2,16]
  * @retval None
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;
  /* Clear PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
  tmpreg &= CFGR_PLL_Mask;
  /* Set the PLL configuration bits */
  tmpreg |= RCC_PLLSource | RCC_PLLMul;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Enables or disables the PLL.
  * @note   The PLL can not be disabled if it is used as system clock.
  * @param  NewState: new state of the PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLLCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
/**
  * @brief  Configures the PREDIV1 division factor.
  * @note
  *   - This function must be used only when the PLL is disabled.
  *   - This function applies only to STM32 Connectivity line and Value line
  *     devices.
  * @param  RCC_PREDIV1_Source: specifies the PREDIV1 clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_PREDIV1_Source_HSE: HSE selected as PREDIV1 clock
  *     @arg RCC_PREDIV1_Source_PLL2: PLL2 selected as PREDIV1 clock
  * @note
  *   For @b STM32_Value_line_devices this parameter is always RCC_PREDIV1_Source_HSE
  * @param  RCC_PREDIV1_Div: specifies the PREDIV1 clock division factor.
  *   This parameter can be RCC_PREDIV1_Divx where x:[1,16]
  * @retval None
  */
void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PREDIV1_SOURCE(RCC_PREDIV1_Source));
  assert_param(IS_RCC_PREDIV1(RCC_PREDIV1_Div));

  tmpreg = RCC->CFGR2;
  /* Clear PREDIV1[3:0] and PREDIV1SRC bits */
  tmpreg &= ~(CFGR2_PREDIV1 | CFGR2_PREDIV1SRC);
  /* Set the PREDIV1 clock source and division factor */
  tmpreg |= RCC_PREDIV1_Source | RCC_PREDIV1_Div ;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}
#endif

#ifdef STM32F10X_CL
/**
  * @brief  Configures the PREDIV2 division factor.
  * @note
  *   - This function must be used only when both PLL2 and PLL3 are disabled.
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  RCC_PREDIV2_Div: specifies the PREDIV2 clock division factor.
  *   This parameter can be RCC_PREDIV2_Divx where x:[1,16]
  * @retval None
  */
void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PREDIV2(RCC_PREDIV2_Div));

  tmpreg = RCC->CFGR2;
  /* Clear PREDIV2[3:0] bits */
  tmpreg &= ~CFGR2_PREDIV2;
  /* Set the PREDIV2 division factor */
  tmpreg |= RCC_PREDIV2_Div;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}

/**
  * @brief  Configures the PLL2 multiplication factor.
  * @note
  *   - This function must be used only when the PLL2 is disabled.
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  RCC_PLL2Mul: specifies the PLL2 multiplication factor.
  *   This parameter can be RCC_PLL2Mul_x where x:{[8,14], 16, 20}
  * @retval None
  */
void RCC_PLL2Config(uint32_t RCC_PLL2Mul)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL2_MUL(RCC_PLL2Mul));

  tmpreg = RCC->CFGR2;
  /* Clear PLL2Mul[3:0] bits */
  tmpreg &= ~CFGR2_PLL2MUL;
  /* Set the PLL2 configuration bits */
  tmpreg |= RCC_PLL2Mul;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}


/**
  * @brief  Enables or disables the PLL2.
  * @note
  *   - The PLL2 can not be disabled if it is used indirectly as system clock
  *     (i.e. it is used as PLL clock entry that is used as System clock).
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  NewState: new state of the PLL2. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLL2Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLL2ON_BB = (uint32_t)NewState;
}


/**
  * @brief  Configures the PLL3 multiplication factor.
  * @note
  *   - This function must be used only when the PLL3 is disabled.
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  RCC_PLL3Mul: specifies the PLL3 multiplication factor.
  *   This parameter can be RCC_PLL3Mul_x where x:{[8,14], 16, 20}
  * @retval None
  */
void RCC_PLL3Config(uint32_t RCC_PLL3Mul)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PLL3_MUL(RCC_PLL3Mul));

  tmpreg = RCC->CFGR2;
  /* Clear PLL3Mul[3:0] bits */
  tmpreg &= ~CFGR2_PLL3MUL;
  /* Set the PLL3 configuration bits */
  tmpreg |= RCC_PLL3Mul;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}


/**
  * @brief  Enables or disables the PLL3.
  * @note   This function applies only to STM32 Connectivity line devices.
  * @param  NewState: new state of the PLL3. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLL3Cmd(FunctionalState NewState)
{
  /* Check the parameters */

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLL3ON_BB = (uint32_t)NewState;
}
#endif /* STM32F10X_CL */

/**
  * @brief  Configures the system clock (SYSCLK).
  * @param  RCC_SYSCLKSource: specifies the clock source used as system clock.
  *   This parameter can be one of the following values:
  *     @arg RCC_SYSCLKSource_HSI: HSI selected as system clock
  *     @arg RCC_SYSCLKSource_HSE: HSE selected as system clock
  *     @arg RCC_SYSCLKSource_PLLCLK: PLL selected as system clock
  * @retval None
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));
  tmpreg = RCC->CFGR;
  /* Clear SW[1:0] bits */
  tmpreg &= CFGR_SW_Mask;
  /* Set SW[1:0] bits according to RCC_SYSCLKSource value */
  tmpreg |= RCC_SYSCLKSource;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Returns the clock source used as system clock.
  * @param  None
  * @retval The clock source used as system clock. The returned value can
  *   be one of the following:
  *     - 0x00: HSI used as system clock
  *     - 0x04: HSE used as system clock
  *     - 0x08: PLL used as system clock
  */
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & CFGR_SWS_Mask));
}

/**
  * @brief  Configures the AHB clock (HCLK).
  * @param  RCC_SYSCLK: defines the AHB clock divider. This clock is derived from
  *   the system clock (SYSCLK).
  *   This parameter can be one of the following values:
  *     @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *     @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *     @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *     @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *     @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *     @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *     @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *     @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *     @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @retval None
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));
  tmpreg = RCC->CFGR;
  /* Clear HPRE[3:0] bits */
  tmpreg &= CFGR_HPRE_Reset_Mask;
  /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
  tmpreg |= RCC_SYSCLK;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Configures the Low Speed APB clock (PCLK1).
  * @param  RCC_HCLK: defines the APB1 clock divider. This clock is derived from
  *   the AHB clock (HCLK).
  *   This parameter can be one of the following values:
  *     @arg RCC_HCLK_Div1: APB1 clock = HCLK
  *     @arg RCC_HCLK_Div2: APB1 clock = HCLK/2
  *     @arg RCC_HCLK_Div4: APB1 clock = HCLK/4
  *     @arg RCC_HCLK_Div8: APB1 clock = HCLK/8
  *     @arg RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @retval None
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* Clear PPRE1[2:0] bits */
  tmpreg &= CFGR_PPRE1_Reset_Mask;
  /* Set PPRE1[2:0] bits according to RCC_HCLK value */
  tmpreg |= RCC_HCLK;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Configures the High Speed APB clock (PCLK2).
  * @param  RCC_HCLK: defines the APB2 clock divider. This clock is derived from
  *   the AHB clock (HCLK).
  *   This parameter can be one of the following values:
  *     @arg RCC_HCLK_Div1: APB2 clock = HCLK
  *     @arg RCC_HCLK_Div2: APB2 clock = HCLK/2
  *     @arg RCC_HCLK_Div4: APB2 clock = HCLK/4
  *     @arg RCC_HCLK_Div8: APB2 clock = HCLK/8
  *     @arg RCC_HCLK_Div16: APB2 clock = HCLK/16
  * @retval None
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* Clear PPRE2[2:0] bits */
  tmpreg &= CFGR_PPRE2_Reset_Mask;
  /* Set PPRE2[2:0] bits according to RCC_HCLK value */
  tmpreg |= RCC_HCLK << 3;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Enables or disables the specified RCC interrupts.
  * @param  RCC_IT: specifies the RCC interrupt sources to be enabled or disabled.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be any combination
  *   of the following values
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_PLL2RDY: PLL2 ready interrupt
  *     @arg RCC_IT_PLL3RDY: PLL3 ready interrupt
  *
  *   For @b other_STM32_devices, this parameter can be any combination of the
  *   following values
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *
  * @param  NewState: new state of the specified RCC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Perform Byte access to RCC_CIR bits to enable the selected interrupts */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* Perform Byte access to RCC_CIR bits to disable the selected interrupts */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS &= (uint8_t)~RCC_IT;
  }
}

#ifndef STM32F10X_CL
/**
  * @brief  Configures the USB clock (USBCLK).
  * @param  RCC_USBCLKSource: specifies the USB clock source. This clock is
  *   derived from the PLL output.
  *   This parameter can be one of the following values:
  *     @arg RCC_USBCLKSource_PLLCLK_1Div5: PLL clock divided by 1,5 selected as USB
  *                                     clock source
  *     @arg RCC_USBCLKSource_PLLCLK_Div1: PLL clock selected as USB clock source
  * @retval None
  */
void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(__IO uint32_t *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}
#else
/**
  * @brief  Configures the USB OTG FS clock (OTGFSCLK).
  *   This function applies only to STM32 Connectivity line devices.
  * @param  RCC_OTGFSCLKSource: specifies the USB OTG FS clock source.
  *   This clock is derived from the PLL output.
  *   This parameter can be one of the following values:
  *     @arg  RCC_OTGFSCLKSource_PLLVCO_Div3: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  *     @arg  RCC_OTGFSCLKSource_PLLVCO_Div2: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  * @retval None
  */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_OTGFSCLK_SOURCE(RCC_OTGFSCLKSource));

  *(__IO uint32_t *) CFGR_OTGFSPRE_BB = RCC_OTGFSCLKSource;
}
#endif /* STM32F10X_CL */

/**
  * @brief  Configures the ADC clock (ADCCLK).
  * @param  RCC_PCLK2: defines the ADC clock divider. This clock is derived from
  *   the APB2 clock (PCLK2).
  *   This parameter can be one of the following values:
  *     @arg RCC_PCLK2_Div2: ADC clock = PCLK2/2
  *     @arg RCC_PCLK2_Div4: ADC clock = PCLK2/4
  *     @arg RCC_PCLK2_Div6: ADC clock = PCLK2/6
  *     @arg RCC_PCLK2_Div8: ADC clock = PCLK2/8
  * @retval None
  */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));
  tmpreg = RCC->CFGR;
  /* Clear ADCPRE[1:0] bits */
  tmpreg &= CFGR_ADCPRE_Reset_Mask;
  /* Set ADCPRE[1:0] bits according to RCC_PCLK2 value */
  tmpreg |= RCC_PCLK2;
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

#ifdef STM32F10X_CL
/**
  * @brief  Configures the I2S2 clock source(I2S2CLK).
  * @note
  *   - This function must be called before enabling I2S2 APB clock.
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  RCC_I2S2CLKSource: specifies the I2S2 clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_I2S2CLKSource_SYSCLK: system clock selected as I2S2 clock entry
  *     @arg RCC_I2S2CLKSource_PLL3_VCO: PLL3 VCO clock selected as I2S2 clock entry
  * @retval None
  */
void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_I2S2CLK_SOURCE(RCC_I2S2CLKSource));

  *(__IO uint32_t *) CFGR2_I2S2SRC_BB = RCC_I2S2CLKSource;
}

/**
  * @brief  Configures the I2S3 clock source(I2S2CLK).
  * @note
  *   - This function must be called before enabling I2S3 APB clock.
  *   - This function applies only to STM32 Connectivity line devices.
  * @param  RCC_I2S3CLKSource: specifies the I2S3 clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_I2S3CLKSource_SYSCLK: system clock selected as I2S3 clock entry
  *     @arg RCC_I2S3CLKSource_PLL3_VCO: PLL3 VCO clock selected as I2S3 clock entry
  * @retval None
  */
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_I2S3CLK_SOURCE(RCC_I2S3CLKSource));

  *(__IO uint32_t *) CFGR2_I2S3SRC_BB = RCC_I2S3CLKSource;
}
#endif /* STM32F10X_CL */

/**
  * @brief  Configures the External Low Speed oscillator (LSE).
  * @param  RCC_LSE: specifies the new state of the LSE.
  *   This parameter can be one of the following values:
  *     @arg RCC_LSE_OFF: LSE oscillator OFF
  *     @arg RCC_LSE_ON: LSE oscillator ON
  *     @arg RCC_LSE_Bypass: LSE oscillator bypassed with external clock
  * @retval None
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* Check the parameters */
  assert_param(IS_RCC_LSE(RCC_LSE));
  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* Reset LSEON bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* Reset LSEBYP bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;

    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;

    default:
      break;
  }
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  * @note   LSI can not be disabled if the IWDG is running.
  * @param  NewState: new state of the LSI. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the RTC clock (RTCCLK).
  * @note   Once the RTC clock is selected it can't be changed unless the Backup domain is reset.
  * @param  RCC_RTCCLKSource: specifies the RTC clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
  *     @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
  *     @arg RCC_RTCCLKSource_HSE_Div128: HSE clock divided by 128 selected as RTC clock
  * @retval None
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));
  /* Select the RTC clock source */
  RCC->BDCR |= RCC_RTCCLKSource;
}

/**
  * @brief  Enables or disables the RTC clock.
  * @note   This function must be used only after the RTC clock was selected using the RCC_RTCCLKConfig function.
  * @param  NewState: new state of the RTC clock. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @brief  Returns the frequencies of different on chip clocks.
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *         the clocks frequencies.
  * @note   The result of this function could be not correct when using
  *         fractional value for HSE crystal.
  * @retval None
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  uint32_t prediv1factor = 0;
#endif

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

#ifndef STM32F10X_CL
      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
 #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
       prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
       /* HSE oscillator clock selected as PREDIV1 clock entry */
       RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;
 #else
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18;

      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13 / 2;
      }

      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */

        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;

        if (prediv1source == 0)
        { /* HSE oscillator clock selected as PREDIV1 clock entry */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */

          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2;
          RCC_Clocks->SYSCLK_Frequency = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;
        }
      }
#endif /* STM32F10X_CL */
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
  }

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK clock frequency */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* Get PCLK1 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 clock frequency */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get PCLK2 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 clock frequency */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get ADCCLK prescaler */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK clock frequency */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/**
  * @brief  Enables or disables the AHB peripheral clock.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to gates its clock.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be any combination
  *   of the following values:
  *     @arg RCC_AHBPeriph_DMA1
  *     @arg RCC_AHBPeriph_DMA2
  *     @arg RCC_AHBPeriph_SRAM
  *     @arg RCC_AHBPeriph_FLITF
  *     @arg RCC_AHBPeriph_CRC
  *     @arg RCC_AHBPeriph_OTG_FS
  *     @arg RCC_AHBPeriph_ETH_MAC
  *     @arg RCC_AHBPeriph_ETH_MAC_Tx
  *     @arg RCC_AHBPeriph_ETH_MAC_Rx
  *
  *   For @b other_STM32_devices, this parameter can be any combination of the
  *   following values:
  *     @arg RCC_AHBPeriph_DMA1
  *     @arg RCC_AHBPeriph_DMA2
  *     @arg RCC_AHBPeriph_SRAM
  *     @arg RCC_AHBPeriph_FLITF
  *     @arg RCC_AHBPeriph_CRC
  *     @arg RCC_AHBPeriph_FSMC
  *     @arg RCC_AHBPeriph_SDIO
  *
  * @note SRAM and FLITF clock can be disabled only during sleep mode.
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/**
  * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
  *          RCC_APB2Periph_TIM15, RCC_APB2Periph_TIM16, RCC_APB2Periph_TIM17,
  *          RCC_APB2Periph_TIM9, RCC_APB2Periph_TIM10, RCC_APB2Periph_TIM11
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
  * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4,
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_CEC,
  *          RCC_APB1Periph_TIM12, RCC_APB1Periph_TIM13, RCC_APB1Periph_TIM14
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}

#ifdef STM32F10X_CL
/**
  * @brief  Forces or releases AHB peripheral reset.
  * @note   This function applies only to STM32 Connectivity line devices.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to reset.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_AHBPeriph_OTG_FS
  *     @arg RCC_AHBPeriph_ETH_MAC
  * @param  NewState: new state of the specified peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB_PERIPH_RESET(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBRSTR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBRSTR &= ~RCC_AHBPeriph;
  }
}
#endif /* STM32F10X_CL */

/**
  * @brief  Forces or releases High Speed APB (APB2) peripheral reset.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
  *          RCC_APB2Periph_TIM15, RCC_APB2Periph_TIM16, RCC_APB2Periph_TIM17,
  *          RCC_APB2Periph_TIM9, RCC_APB2Periph_TIM10, RCC_APB2Periph_TIM11
  * @param  NewState: new state of the specified peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
  * @param  RCC_APB1Periph: specifies the APB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4,
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_CEC,
  *          RCC_APB1Periph_TIM12, RCC_APB1Periph_TIM13, RCC_APB1Periph_TIM14
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/**
  * @brief  Forces or releases the Backup domain reset.
  * @param  NewState: new state of the Backup domain reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables the Clock Security System.
  * @param  NewState: new state of the Clock Security System..
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_CSSON_BB = (uint32_t)NewState;
}

/**
  * @brief  Selects the clock source to output on MCO pin.
  * @param  RCC_MCO: specifies the clock source to output.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be one of the
  *   following values:
  *     @arg RCC_MCO_NoClock: No clock selected
  *     @arg RCC_MCO_SYSCLK: System clock selected
  *     @arg RCC_MCO_HSI: HSI oscillator clock selected
  *     @arg RCC_MCO_HSE: HSE oscillator clock selected
  *     @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
  *     @arg RCC_MCO_PLL2CLK: PLL2 clock selected
  *     @arg RCC_MCO_PLL3CLK_Div2: PLL3 clock divided by 2 selected
  *     @arg RCC_MCO_XT1: External 3-25 MHz oscillator clock selected
  *     @arg RCC_MCO_PLL3CLK: PLL3 clock selected
  *
  *   For  @b other_STM32_devices, this parameter can be one of the following values:
  *     @arg RCC_MCO_NoClock: No clock selected
  *     @arg RCC_MCO_SYSCLK: System clock selected
  *     @arg RCC_MCO_HSI: HSI oscillator clock selected
  *     @arg RCC_MCO_HSE: HSE oscillator clock selected
  *     @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
  *
  * @retval None
  */
void RCC_MCOConfig(uint8_t RCC_MCO)
{
  /* Check the parameters */
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* Perform Byte access to MCO bits to select the MCO source */
  *(__IO uint8_t *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  RCC_FLAG: specifies the flag to check.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be one of the
  *   following values:
  *     @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *     @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *     @arg RCC_FLAG_PLLRDY: PLL clock ready
  *     @arg RCC_FLAG_PLL2RDY: PLL2 clock ready
  *     @arg RCC_FLAG_PLL3RDY: PLL3 clock ready
  *     @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *     @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *     @arg RCC_FLAG_PINRST: Pin reset
  *     @arg RCC_FLAG_PORRST: POR/PDR reset
  *     @arg RCC_FLAG_SFTRST: Software reset
  *     @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *     @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *     @arg RCC_FLAG_LPWRRST: Low Power reset
  *
  *   For @b other_STM32_devices, this parameter can be one of the following values:
  *     @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *     @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *     @arg RCC_FLAG_PLLRDY: PLL clock ready
  *     @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *     @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *     @arg RCC_FLAG_PINRST: Pin reset
  *     @arg RCC_FLAG_PORRST: POR/PDR reset
  *     @arg RCC_FLAG_SFTRST: Software reset
  *     @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *     @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *     @arg RCC_FLAG_LPWRRST: Low Power reset
  *
  * @retval The new state of RCC_FLAG (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* Get the RCC register index */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* The flag to check is in CR register */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* The flag to check is in BDCR register */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* The flag to check is in CSR register */
  {
    statusreg = RCC->CSR;
  }

  /* Get the flag position */
  tmp = RCC_FLAG & FLAG_Mask;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief  Clears the RCC reset flags.
  * @note   The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST, RCC_FLAG_SFTRST,
  *   RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
  * @param  None
  * @retval None
  */
void RCC_ClearFlag(void)
{
  /* Set RMVF bit to clear the reset flags */
  RCC->CSR |= CSR_RMVF_Set;
}

/**
  * @brief  Checks whether the specified RCC interrupt has occurred or not.
  * @param  RCC_IT: specifies the RCC interrupt source to check.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be one of the
  *   following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_PLL2RDY: PLL2 ready interrupt
  *     @arg RCC_IT_PLL3RDY: PLL3 ready interrupt
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  *
  *   For @b other_STM32_devices, this parameter can be one of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  *
  * @retval The new state of RCC_IT (SET or RESET).
  */
ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
  ITStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* Check the status of the specified RCC interrupt */
  if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the RCC_IT status */
  return  bitstatus;
}

/**
  * @brief  Clears the RCC's interrupt pending bits.
  * @param  RCC_IT: specifies the interrupt pending bit to clear.
  *
  *   For @b STM32_Connectivity_line_devices, this parameter can be any combination
  *   of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_PLL2RDY: PLL2 ready interrupt
  *     @arg RCC_IT_PLL3RDY: PLL3 ready interrupt
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  *
  *   For @b other_STM32_devices, this parameter can be any combination of the
  *   following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  * @retval None
  */
void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
  /* Check the parameters */
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* Perform Byte access to RCC_CIR[23:16] bits to clear the selected interrupt
     pending bits */
  *(__IO uint8_t *) CIR_BYTE3_ADDRESS = RCC_IT;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif // __STM32F1__ && OVERCLOCK

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
