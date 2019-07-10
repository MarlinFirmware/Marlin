/*
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
// This array allows to wrap Arduino pin number(Dx or x)
// to STM32 PinName (PX_n)
const PinName digitalPin[] = {
#if STM32F4X_PIN_NUM >= 64  //64 pins mcu, 51 gpio
  PC_13, //D0
  PC_14, //D1  - OSC32_IN
  PC_15, //D2  - OSC32_OUT
  PH_0,  //D3  - OSC_IN
  PH_1,  //D4  - OSC_OUT
  PC_0,  //D5  - 1:  2:ADC123_IN10
  PC_1,  //D6  - 1:  2:ADC123_IN11
  PC_2,  //D7  - 1:SPI2_MISO  2:ADC123_IN12
  PC_3,  //D8  - 1:SPI2_MOSI  2:ADC123_IN13
  PA_0,  //D9  - 1:UART4_TX / TIM5_CH1  2:ADC123_IN0
  PA_1,  //D10 - 1:UART4_RX / TIM5_CH2 / TIM2_CH2  2:ADC123_IN1
  PA_2,  //D11 - 1:USART2_TX /TIM5_CH3 / TIM9_CH1 / TIM2_CH3  2:ADC123_IN2
  PA_3,  //D12 - 1:USART2_RX /TIM5_CH4 / TIM9_CH2 / TIM2_CH4  2:ADC123_IN3
  PA_4,  //D13 - NOT FT 1:SPI1_NSS / SPI3_NSS / USART2_CK  2:ADC12_IN4 / DAC_OUT1
  PA_5,  //D14 - NOT FT 1:SPI1_SCK  2:ADC12_IN5 / DAC_OUT2
  PA_6,  //D15 - 1:SPI1_MISO / TIM13_CH1 / TIM3_CH1  2:ADC12_IN6
  PA_7,  //D16 - 1:SPI1_MOSI / TIM14_CH1 / TIM3_CH2  2:ADC12_IN7
  PC_4,  //D17 - 1:  2:ADC12_IN14
  PC_5,  //D18 - 1:  2:ADC12_IN15
  PB_0,  //D19 - 1:TIM3_CH3  2:ADC12_IN8
  PB_1,  //D20 - 1:TIM3_CH4  2:ADC12_IN9
  PB_2,  //D21 - BOOT1
  PB_10, //D22 - 1:SPI2_SCK / I2C2_SCL / USART3_TX / TIM2_CH3
  PB_11, //D23 - 1:I2C2_SDA / USART3_RX / TIM2_CH4
  PB_12, //D24 - 1:SPI2_NSS / OTG_HS_ID
  PB_13, //D25 - 1:SPI2_SCK  2:OTG_HS_VBUS
  PB_14, //D26 - 1:SPI2_MISO / TIM12_CH1 / OTG_HS_DM
  PB_15, //D27 - SPI2_MOSI / TIM12_CH2 / OTG_HS_DP
  PC_6,  //D28 - 1:TIM8_CH1 / SDIO_D6 / USART6_TX / TIM3_CH1
  PC_7,  //D29 - 1:TIM8_CH2 / SDIO_D7 / USART6_RX / TIM3_CH2
  PC_8,  //D30 - 1:TIM8_CH3 / SDIO_D0 / TIM3_CH3
  PC_9,  //D31 - 1:TIM8_CH4 / SDIO_D1 / TIM3_CH4
  PA_8,  //D32 - 1:TIM1_CH1 / I2C3_SCL / OTG_FS_SOF
  PA_9,  //D33 - 1:USART1_TX / TIM1_CH2  2:OTG_FS_VBUS
  PA_10, //34 - 1:USART1_RX / TIM1_CH3 / OTG_FS_ID
  PA_11, //D35 - 1:TIM1_CH4 / OTG_FS_DM
  PA_12, //D36 - 1:OTG_FS_DP
  PA_13, //D37 - 0:JTMS-SWDIO
  PA_14, //D38 - 0:JTCK-SWCLK
  PA_15, //D39 - 0:JTDI  1:SPI3_NSS / SPI1_NSS
  PC_10, //D40 - 1:UART4_TX / SPI3_SCK / SDIO_D2 / USART3_TX
  PC_11, //D41 - 1:UART4_RX / SPI3_MISO / SDIO_D3 / USART3_RX
  PC_12, //D42 - 1:UART5_TX / SPI3_MOSI / SDIO_CK
  PD_2,  //D43 - 1:UART5_RX / SDIO_CMD
  PB_3,  //D44 - 0:JTDO  1:SPI3_SCK / TIM2_CH2 / SPI1_SCK
  PB_4,  //D45 - 0:NJTRST  1:SPI3_MISO / TIM3_CH1 / SPI1_MISO
  PB_5,  //D45 - 1:TIM3_CH2 / SPI1_MOSI / SPI3_MOSI
  PB_6,  //D47 - 1:I2C1_SCL / TIM4_CH1 / USART1_TX
  PB_7,  //D48 - 1:I2C1_SDA / TIM4_CH2 / USART1_RX
  PB_8,  //D49 - 1:I2C1_SCL / TIM4_CH3 / SDIO_D4 / TIM10_CH1
  PB_9,  //D50 - 1:I2C1_SDA / TIM4_CH4 / SDIO_D5 / TIM11_CH1 / SPI2_NSS
#endif
#if STM32F4X_PIN_NUM >= 100  //100 pins mcu, 82 gpio
  PE_2,  //D51 - 1:FSMC_A23
  PE_3,  //D52 - 1:FSMC_A19
  PE_4,  //D53 - 1:FSMC_A20
  PE_5,  //D54 - 1:FSMC_A21
  PE_6,  //D55 - 1:FSMC_A22
  PE_7,  //D56 - 1:FSMC_D4
  PE_8,  //D57 - 1:FSMC_D5
  PE_9,  //D58 - 1:FSMC_D6 / TIM1_CH1
  PE_10, //D59 - 1:FSMC_D7
  PE_11, //D60 - 1:FSMC_D8 / TIM1_CH2
  PE_12, //D61 - 1:FSMC_D9
  PE_13, //D62 - 1:FSMC_D10 / TIM1_CH3
  PE_14, //D63 - 1:FSMC_D11 / TIM1_CH4
  PE_15, //D64 - 1:FSMC_D12
  PD_8,  //D65 - 1:FSMC_D13 / USART3_TX
  PD_9,  //D66 - 1:FSMC_D14 / USART3_RX
  PD_10, //D67 - 1:FSMC_D15
  PD_11, //D68 - 1:FSMC_A16
  PD_12, //D69 - 1:FSMC_A17 / TIM4_CH1
  PD_13, //D70 - 1:FSMC_A18 / TIM4_CH2
  PD_14, //D71 - 1:FSMC_D0 / TIM4_CH3
  PD_15, //D72 - 1:FSMC_D1 / TIM4_CH4
  PD_0,  //D73 - 1:FSMC_D2
  PD_1,  //D74 - 1:FSMC_D3
  PD_3,  //D75 - 1:FSMC_CLK
  PD_4,  //D76 - 1:FSMC_NOE
  PD_5,  //D77 - 1:USART2_TX
  PD_6,  //D78 - 1:USART2_RX
  PD_7,  //D79
  PE_0,  //D80
  PE_1,  //D81
#endif
#if STM32F4X_PIN_NUM >= 144  //144 pins mcu, 114 gpio
  PF_0,  //D82 - 1:FSMC_A0 / I2C2_SDA
  PF_1,  //D83 - 1:FSMC_A1 / I2C2_SCL
  PF_2,  //D84 - 1:FSMC_A2
  PF_3,  //D85 - 1:FSMC_A3  2:ADC3_IN9
  PF_4,  //D86 - 1:FSMC_A4  2:ADC3_IN14
  PF_5,  //D87 - 1:FSMC_A5  2:ADC3_IN15
  PF_6,  //D88 - 1:TIM10_CH1  2:ADC3_IN4
  PF_7,  //D89 - 1:TIM11_CH1  2:ADC3_IN5
  PF_8,  //D90 - 1:TIM13_CH1  2:ADC3_IN6
  PF_9,  //D91 - 1;TIM14_CH1  2:ADC3_IN7
  PF_10, //D92 - 2:ADC3_IN8
  PF_11, //D93
  PF_12, //D94 - 1:FSMC_A6
  PF_13, //D95 - 1:FSMC_A7
  PF_14, //D96 - 1:FSMC_A8
  PF_15, //D97 - 1:FSMC_A9
  PG_0,  //D98 - 1:FSMC_A10
  PG_1,  //D99 - 1:FSMC_A11
  PG_2,  //D100 - 1:FSMC_A12
  PG_3,  //D101 - 1:FSMC_A13
  PG_4,  //D102 - 1:FSMC_A14
  PG_5,  //D103 - 1:FSMC_A15
  PG_6,  //D104
  PG_7,  //D105
  PG_8,  //D106
  PG_9,  //D107 - 1:USART6_RX
  PG_10, //D108 - 1:FSMC_NE3
  PG_11, //D109
  PG_12, //D110 - 1:FSMC_NE4
  PG_13, //D111 - 1:FSMC_A24
  PG_14, //D112 - 1:FSMC_A25 / USART6_TX
  PG_15, //D113
#endif
#if STM32F4X_PIN_NUM >= 176  //176 pins mcu, 140 gpio
  PI_8,  //D114
  PI_9,  //D115
  PI_10, //D116
  PI_11, //D117
  PH_2,  //D118
  PH_3,  //D119
  PH_4,  //D120 - 1:I2C2_SCL
  PH_5,  //D121 - 1:I2C2_SDA
  PH_6,  //D122 - 1:TIM12_CH1
  PH_7,  //D123 - 1:I2C3_SCL
  PH_8,  //D124 - 1:I2C3_SDA
  PH_9,  //D125 - 1:TIM12_CH2
  PH_10, //D126 - 1:TIM5_CH1
  PH_11, //D127 - 1:TIM5_CH2
  PH_12, //D128 - 1:TIM5_CH3
  PH_13, //D129
  PH_14, //D130
  PH_15, //D131
  PI_0,  //D132 - 1:TIM5_CH4 / SPI2_NSS
  PI_1,  //D133 - 1:SPI2_SCK
  PI_2,  //D134 - 1:TIM8_CH4 /SPI2_MISO
  PI_3,  //D135 - 1:SPI2_MOS
  PI_4,  //D136
  PI_5,  //D137 - 1:TIM8_CH1
  PI_6,  //D138 - 1:TIM8_CH2
  PI_7,  //D139 - 1:TIM8_CH3
#endif
#if STM32F4X_PIN_NUM >= 64  //64 pins mcu, 51 gpio, 16 ADC
  PA_0,  //D140/A0 = D9  - 1:UART4_TX / TIM5_CH1  2:ADC123_IN0
  PA_1,  //D141/A1 = D10 - 1:UART4_RX / TIM5_CH2 / TIM2_CH2  2:ADC123_IN1
  PA_2,  //D142/A2 = D11 - 1:USART2_TX /TIM5_CH3 / TIM9_CH1 / TIM2_CH3  2:ADC123_IN2
  PA_3,  //D143/A3 = D12 - 1:USART2_RX /TIM5_CH4 / TIM9_CH2 / TIM2_CH4  2:ADC123_IN3
  PA_4,  //D144/A4 = D13 - NOT FT 1:SPI1_NSS / SPI3_NSS / USART2_CK  2:ADC12_IN4 / DAC_OUT1
  PA_5,  //D145/A5 = D14 - NOT FT 1:SPI1_SCK  2:ADC12_IN5 / DAC_OUT2
  PA_6,  //D146/A6 = D15 - 1:SPI1_MISO / TIM13_CH1 / TIM3_CH1  2:ADC12_IN6
  PA_7,  //D147/A7 = D16 - 1:SPI1_MOSI / TIM14_CH1 / TIM3_CH2  2:ADC12_IN7
  PB_0,  //D148/A8 = D19 - 1:TIM3_CH3  2:ADC12_IN8
  PB_1,  //D149/A9 = D20 - 1:TIM3_CH4  2:ADC12_IN9
  PC_0,  //D150/A10 = D5  - 1:  2:ADC123_IN10
  PC_1,  //D151/A11 = D6  - 1:  2:ADC123_IN11
  PC_2,  //D152/A12 = D7  - 1:SPI2_MISO  2:ADC123_IN12
  PC_3,  //D153/A13 = D8  - 1:SPI2_MOSI  2:ADC123_IN13
  PC_4,  //D154/A14 = D17 - 1:  2:ADC12_IN14
  PC_5,  //D155/A15 = D18 - 1:  2:ADC12_IN15
#endif
#if STM32F4X_PIN_NUM >= 144  //144 pins mcu, 114 gpio, 24 ADC
  PF_3,  //D156/A16 = D85 - 1:FSMC_A3  2:ADC3_IN9
  PF_4,  //D157/A17 = D86 - 1:FSMC_A4  2:ADC3_IN14
  PF_5,  //D158/A18 = D87 - 1:FSMC_A5  2:ADC3_IN15
  PF_6,  //D159/A19 = D88 - 1:TIM10_CH1  2:ADC3_IN4
  PF_7,  //D160/A20 = D89 - 1:TIM11_CH1  2:ADC3_IN5
  PF_8,  //D161/A21 = D90 - 1:TIM13_CH1  2:ADC3_IN6
  PF_9,  //D162/A22 = D91 - 1;TIM14_CH1  2:ADC3_IN7
  PF_10, //D163/A23 = D92 - 2:ADC3_IN8
#endif
};

#ifdef __cplusplus
}
#endif

// ------------------------

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /**Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Configure the Systick interrupt time
  */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  /**Configure the Systick
  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef __cplusplus
}
#endif
