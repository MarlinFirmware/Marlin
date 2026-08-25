/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

#if defined(STM32F401xC)
#include "pins_arduino.h"

// Digital PinName array
const PinName digitalPin[] = {
  PA_0,   // D0/A0
  PA_1,   // D1/A1
  PA_2,   // D2/A2
  PA_3,   // D3/A3
  PA_4,   // D4/A4
  PA_5,   // D5/A5
  PA_6,   // D6/A6
  PA_7,   // D7/A7
  PA_8,   // D8
  PA_9,   // D9
  PA_10,  // D10
  PA_11,  // D11
  PA_12,  // D12
  PA_13,  // D13
  PA_14,  // D14
  PA_15,  // D15
  PB_0,   // D16/A8
  PB_1,   // D17/A9
  PB_2,   // D18
  PB_3,   // D19
  PB_4,   // D20
  PB_5,   // D21
  PB_6,   // D22
  PB_7,   // D23
  PB_8,   // D24
  PB_9,   // D25
  PB_10,  // D26
  PB_12,  // D27
  PB_13,  // D28
  PB_14,  // D29
  PB_15,  // D30
  PC_0,   // D31/A10
  PC_1,   // D32/A11
  PC_2,   // D33/A12
  PC_3,   // D34/A13
  PC_4,   // D35/A14
  PC_5,   // D36/A15
  PC_6,   // D37
  PC_7,   // D38
  PC_8,   // D39
  PC_9,   // D40
  PC_10,  // D41
  PC_11,  // D42
  PC_12,  // D43
  PC_13,  // D44
  PC_14,  // D45
  PC_15,  // D46
  PD_2,   // D47
  PH_0,   // D48
  PH_1    // D49
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
  0,  // A0,  PA0
  1,  // A1,  PA1
  2,  // A2,  PA2
  3,  // A3,  PA3
  4,  // A4,  PA4
  5,  // A5,  PA5
  6,  // A6,  PA6
  7,  // A7,  PA7
  16, // A8,  PB0
  17, // A9,  PB1
  31, // A10, PC0
  32, // A11, PC1
  33, // A12, PC2
  34, // A13, PC3
  35, // A14, PC4
  36  // A15, PC5
};

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
 *               AHB/APBx prescalers and Flash settings
 * @note   This function should be called only once the RCC clock configuration
 *         is reset to the default reset state (done in SystemInit() function).
 * @param  None
 * @retval None
 */

/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
static uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet. */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  // Enable HSE oscillator and activate PLL with HSE as source
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
  if (bypass == 0) {
    RCC_OscInitStruct.HSEState          = RCC_HSE_ON; // External 8 MHz xtal on OSC_IN/OSC_OUT
  } else {
    RCC_OscInitStruct.HSEState          = RCC_HSE_BYPASS; // External 8 MHz clock on OSC_IN
  }

  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM            = HSE_VALUE / 1000000L; // Expects an 8 MHz external clock by default. Redefine HSE_VALUE if not
  RCC_OscInitStruct.PLL.PLLN            = 336;           // VCO output clock = 336 MHz (1 MHz * 336)
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4; // PLLCLK = 84 MHz (336 MHz / 4)
  RCC_OscInitStruct.PLL.PLLQ            = 7;             // USB clock = 48 MHz (336 MHz / 7) --> OK for USB
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    return 0; // FAIL
  }

  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 84 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 84 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // 42 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 84 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    return 0; // FAIL
  }

  /* Output clock on MCO1 pin(PA8) for debugging purpose */
  /*
  if (bypass == 0)
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2); // 4 MHz
  else
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); // 8 MHz
  */

  return 1; // OK
}

/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* The voltage scaling allows optimizing the power consumption when the device is
    clocked below the maximum system frequency, to update the voltage scaling value
    regarding system frequency refer to product datasheet. */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  // Enable HSI oscillator and activate PLL with HSI as source
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = 16;            // VCO input clock = 1 MHz (16 MHz / 16)
  RCC_OscInitStruct.PLL.PLLN            = 336;           // VCO output clock = 336 MHz (1 MHz * 336)
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4; // PLLCLK = 84 MHz (336 MHz / 4)
  RCC_OscInitStruct.PLL.PLLQ            = 7;             // USB clock = 48 MHz (336 MHz / 7) --> freq is ok but not precise enough
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    return 0; // FAIL
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 84 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 84 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // 42 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 84 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    return 0; // FAIL
  }

  /* Output clock on MCO1 pin(PA8) for debugging purpose */
  //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1); // 16 MHz

  return 1; // OK
}

WEAK void SystemClock_Config(void)
{
  /* 1- If fail try to start with HSE and external xtal */
  if (SetSysClock_PLL_HSE(0) == 0) {
    /* 2- Try to start with HSE and external clock */
    if (SetSysClock_PLL_HSE(1) == 0) {
      /* 3- If fail start with HSI clock */
      if (SetSysClock_PLL_HSI() == 0) {
        Error_Handler();
      }
    }
  }
  /* Output clock on MCO2 pin(PC9) for debugging purpose */
  //HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_4);
}

#ifdef __cplusplus
}
#endif
#endif /* STM32F401xC */