/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
const PinName digitalPin[] = {
  PA_0,  //D0
  PA_1,  //D1
  PA_2,  //D2
  PA_3,  //D3
  PA_4,  //D4
  PA_5,  //D5
  PA_6,  //D6
  PA_7,  //D7
  PA_8,  //D8
  PA_9,  //D9
  PA_10, //D10
  PA_11, //D11
  PA_12, //D12
  PA_13, //D13
  PA_14, //D14
  PA_15, //D15
  PB_0,  //D16
  PB_1,  //D17
  PB_2,  //D18
  PB_3,  //D19
  PB_4,  //D20
  PB_5,  //D21
  PB_6,  //D22
  PB_7,  //D23
  PB_8,  //D24
  PB_9,  //D25
  PB_10, //D26
  PB_11, //D27
  PB_12, //D28
  PB_13, //D29
  PB_14, //D30
  PB_15, //D31
  PC_0,  //D32
  PC_1,  //D33
  PC_2,  //D34
  PC_3,  //D35
  PC_4,  //D36
  PC_5,  //D37
  PC_6,  //D38
  PC_7,  //D39
  PC_8,  //D40
  PC_9,  //D41
  PC_10, //D42
  PC_11, //D43
  PC_12, //D44
  PC_13, //D45
  PC_14, //D46
  PC_15, //D47
  PD_0,  //D48
  PD_1,  //D49
  PD_2,  //D50
  PD_3,  //D51
  PD_4,  //D52
  PD_5,  //D53
  PD_6,  //D54
  PD_7,  //D55
  PD_8,  //D56
  PD_9,  //D57
  PD_10, //D58
  PD_11, //D59
  PD_12, //D60
  PD_13, //D61
  PD_14, //D62
  PD_15, //D63
  PE_0,  //D64
  PE_1,  //D65
  PE_2,  //D66
  PE_3,  //D67
  PE_4,  //D68
  PE_5,  //D69
  PE_6,  //D70
  PE_7,  //D71
  PE_8,  //D72
  PE_9,  //D73
  PE_10, //D74
  PE_11, //D75
  PE_12, //D76
  PE_13, //D77
  PE_14, //D78
  PE_15, //D79
  PF_0,  //D80
  PF_1,  //D81
  PF_2,  //D82
  PF_3,  //D83
  PF_4,  //D84
  PF_5,  //D85
  PF_6,  //D86
  PF_7,  //D87
  PF_8,  //D88
  PF_9,  //D89
  PF_10, //D90
  PF_11, //D91
  PF_12, //D92
  PF_13, //D93
  PF_14, //D94
  PF_15, //D95
  PG_0,  //D96
  PG_1,  //D97
  PG_2,  //D98
  PG_3,  //D99
  PG_4,  //D100
  PG_5,  //D101
  PG_6,  //D102
  PG_7,  //D103
  PG_8,  //D104
  PG_9,  //D105
  PG_10, //D106
  PG_11, //D107
  PG_12, //D108
  PG_13, //D109
  PG_14, //D110
  PG_15, //D111
  PH_0,  //D112
  PH_1,  //D113
  PH_2,  //D114
  PH_3,  //D115
  PH_4,  //D116
  PH_5,  //D117
  PH_6,  //D118
  PH_7,  //D119
  PH_8,  //D120
  PH_9,  //D121
  PH_10, //D122
  PH_11, //D123
  PH_12, //D124
  PH_13, //D125
  PH_14, //D126
  PH_15, //D127

  //Duplicated ADC Pins
  PC_3,  //A0 T0 D128
  PC_0,  //A1 T1 D129
  PC_2,  //A2 BED D130
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

uint32_t myvar[] = {1,2,3,4,5,6,7,8};
void myshow(int fre, int times) // YSZ-WORK
{
  uint32_t index = 10;
  RCC->AHB1ENR |= 1 << 6; // port G clock
  GPIOG->MODER &= ~(3UL << 2 * index); // clear old mode
  GPIOG->MODER |= 1 << 2 * index; // mode is output
  GPIOG->OSPEEDR &= ~(3UL << 2 * index) // Clear old output speed
                    GPIOG->OSPEEDR |= 2 << 2 * index; // Set output speed
  GPIOG->OTYPER &= ~(1UL << index) // clear old output
                   GPIOG->OTYPER |= 0 << index; // Set the output mode to push-pull
  GPIOG->PUPDR &= ~(3 << 2 * index) // Clear the original settings first
                  GPIOG->PUPDR |= 1 << 2 * index; // Set new up and down
  while (times != 0) {
    GPIOG->BSRR = 1UL << index;
    for (int i = 0; i < fre; i++)
      for (int j = 0; j < 1000000; j++) __NOP();
    GPIOG->BSRR = 1UL << (index + 16);
    for (int i = 0; i < fre; i++)
      for (int j = 0; j < 1000000; j++) __NOP();
    if (times > 0) times--;
  }
}

HAL_StatusTypeDef SDMMC_IsProgramming(SDIO_TypeDef *SDIOx,uint32_t RCA)
{
  HAL_SD_CardStateTypeDef CardState;
  volatile uint32_t respR1 = 0, status = 0;
  SDIO_CmdInitTypeDef sdmmc_cmdinit;
  do {
    sdmmc_cmdinit.Argument         = RCA << 16;
    sdmmc_cmdinit.CmdIndex         = SDMMC_CMD_SEND_STATUS;
    sdmmc_cmdinit.Response         = SDIO_RESPONSE_SHORT;
    sdmmc_cmdinit.WaitForInterrupt = SDIO_WAIT_NO;
    sdmmc_cmdinit.CPSM             = SDIO_CPSM_ENABLE;
    SDIO_SendCommand(SDIOx,&sdmmc_cmdinit); // send CMD13
    do status = SDIOx->STA;
    while (!(status & ((1 << 0) | (1 << 6) | (1 << 2)))); // wait for the operation to complete
    if (status & (1 << 0)) { // CRC check failed
      SDIOx->ICR |= 1 << 0; // clear error flag
      return HAL_ERROR;
    }
    if (status & (1 << 2)) { // command timed out
      SDIOx->ICR |= 1 << 2; // clear error flag
      return HAL_ERROR;
    }
    if (SDIOx->RESPCMD != SDMMC_CMD_SEND_STATUS) return HAL_ERROR;
    SDIOx->ICR = 0X5FF;     // clear all tags
    respR1 = SDIOx->RESP1;
    CardState = (respR1 >> 9) & 0x0000000F;
  } while ((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING) || (CardState == HAL_SD_CARD_PROGRAMMING));
  return HAL_OK;
}

void debugStr(const char *str) {
  while (*str) {
    while ((USART1->SR & 0x40) == 0);
    USART1->DR = *str++;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows:
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000/120000000/180000000
  *            HCLK(Hz)                       = 168000000/120000000/180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8/4/8
  *            PLL_N                          = 336/120/360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7/5/7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  HAL_RCC_DeInit();

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if (ret != HAL_OK) myshow(10,-1);
  HAL_PWREx_EnableOverDrive();

  /* Select PLLSAI output as USB clock source */
  PeriphClkInitStruct.PLLSAI.PLLSAIM = 8;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48 | RCC_PERIPHCLK_SDIO;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CLK48; // SDIO Clock Mux
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if (ret != HAL_OK) myshow(10,-1);

  SystemCoreClockUpdate();
  /* Configure the Systick interrupt time */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  /* Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  __enable_irq(); // Turn on the interrupt here because it is turned off in the bootloader
}

#ifdef __cplusplus
}
#endif
