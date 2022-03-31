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

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif


const PinName digitalPin[] = {
 PB_12,
 PB_13,
 PB_14,
 PB_15,
 PD_8,
 PD_9,
 PD_10,
 PD_11,
 PD_12,
 PD_13,
 PD_14,
 PD_15,
 PG_2,
 PG_3,
 PG_4,
 PG_5,
 PG_6,
 PG_7,
 PG_8,
 PC_6,
 PC_7,
 PC_8,
 PC_9,
 PA_8,
 PA_9,
 PA_10,
 PA_11,
 PA_12,
 PA_13,
 PA_14,
 PA_15,
 PC_10,
 PC_11,
 PC_12,
 PD_0,
 PD_1,
 PD_2,
 PD_3,
 PD_4,
 PD_5,
 PD_6,
 PD_7,
 PG_9,
 PG_10,
 PG_11,
 PG_12,
 PG_13,
 PG_14,
 PG_15,
 PB_3,
 PB_4,
 PB_5,
 PB_6,
 PB_7,
 PB_8,
 PB_9,
 PB_10,
 PB_11,
 PE_14,
 PE_15,
 PE_12,
 PE_13,
 PE_10,
 PE_11,
 PE_8,
 PE_9,
 PG_1,
 PE_7,
 PF_15,
 PG_0,
 PF_13,
 PF_14,
 PF_11,
 PF_12,
 PB_2,
 PB_1,
 PC_5,
 PB_0,
 PA_7,
 PC_4,
 PA_5,
 PA_6,
 PA_3,
 PA_4,
 PA_1,
 PA_2,
 PC_3,
 PA_0,
 PC_1,
 PC_2,
 PC_0,
 PF_8,
 PF_6,
 PF_7,
 PF_9,
 PF_10,
 PF_4,
 PF_5,
 PF_2,
 PF_3,
 PF_0,
 PF_1,
 PE_6,
 PC_13,
 PE_4,
 PE_5,
 PE_2,
 PE_3,
 PE_0,
 PE_1,
 PC_14,
 PC_15,
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#define __fatal_error(X)


/**
  * @brief  System Clock Configuration
  *
  *         The system Clock is configured for F4/F7 as follows:
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = HSE_VALUE
  *            PLL_M                          = HSE_VALUE/1000000
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  *
  *         The system Clock is configured for L4 as follows:
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = MSI_VALUE (4000000)
  *            LSE Frequency(Hz)              = 32768
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_P                          = 7
  *            PLL_Q                          = 2
  *            PLL_R                          = 2 <= This is the source for SysClk, not as on F4/7 PLL_P
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  *
  * PLL is configured as follows:
  *
  *     VCO_IN
  *         F4/F7 = HSE / M
  *         L4    = MSI / M
  *     VCO_OUT
  *         F4/F7 = HSE / M * N
  *         L4    = MSI / M * N
  *     PLLCLK
  *         F4/F7 = HSE / M * N / P
  *         L4    = MSI / M * N / R
  *     PLL48CK
  *         F4/F7 = HSE / M * N / Q
  *         L4    = MSI / M * N / Q  USB Clock is obtained over PLLSAI1
  *
  *     SYSCLK = PLLCLK
  *     HCLK   = SYSCLK / AHB_PRESC
  *     PCLKx  = HCLK / APBx_PRESC
  *
  * Constraints on parameters:
  *
  *     VCO_IN between 1MHz and 2MHz (2MHz recommended)
  *     VCO_OUT between 192MHz and 432MHz
  *     HSE = 8MHz
  *     M = 2 .. 63 (inclusive)
  *     N = 192 ... 432 (inclusive)
  *     P = 2, 4, 6, 8
  *     Q = 2 .. 15 (inclusive)
  *
  *     AHB_PRESC=1,2,4,8,16,64,128,256,512
  *     APBx_PRESC=1,2,4,8,16
  *
  * Output clocks:
  *
  * CPU             SYSCLK      max 168MHz
  * USB,RNG,SDIO    PLL48CK     must be 48MHz for USB
  * AHB             HCLK        max 168MHz
  * APB1            PCLK1       max 42MHz
  * APB2            PCLK2       max 84MHz
  *
  * Timers run from APBx if APBx_PRESC=1, else 2x APBx
  */
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
      __fatal_error("HAL_RCC_OscConfig");
    }

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
      __fatal_error("HAL_RCC_ClockConfig");
    }

    /**Configure the Systick interrupt time */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef __cplusplus
}
#endif
