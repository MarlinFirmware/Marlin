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

#if defined(ARDUINO_STEVAL)
// Pin number
// This array allows to wrap Arduino pin number(Dx or x)
// to STM32 PinName (PX_n)
const PinName digitalPin[] = {
  PA_9,  // TX
  PA_10, // RX

  // WIFI
  PD_3,  // CTS
  PD_4,  // RTS
  PD_5,  // TX
  PD_6,  // RX
  PB_5,  // WIFI_WAKEUP
  PE_11, // WIFI_RESET
  PE_12, // WIFI_BOOT

  // STATUS_LED
  PE_1,  //STATUS_LED

  // SPI USER
  PB_12, // SPI_CS
  PB_15, // SPI_MOSI
  PB_14, // SPI_MISO
  PB_13, // SPI_SCK

  // I2C USER
  PB_7,  // SDA
  PB_6,  // SCL

  // SPI
  PA_4,  // SPI_CS
  PA_5,  // SPI_SCK
  PA_6,  // SPI_MISO
  PA_7,  // SPI_MOSI

  // JTAG
  PA_13, // JTAG_TMS/SWDIO
  PA_14, // JTAG_TCK/SWCLK
  PB_3,  // JTAG_TDO/SWO

  // SDCARD
  PC_8,  // SDIO_D0
  PC_9,  // SDIO_D1
  PA_15, // SD_CARD_DETECT
  PC_10, // SDIO_D2
  PC_11, // SDIO_D3
  PC_12, // SDIO_CK
  PD_2,  // SDIO_CMD

  // OTG
  PA_11, // OTG_DM
  PA_12, // OTG_DP

  // IR/PROBE
  PD_1,  // IR_OUT
  PC_1,  // IR_ON

  // USER_PINS
  PD_7,  // USER3
  PB_9,  // USER1
  PE_0,  // USER2
  PB_4,  // USER4

  // USERKET
  PE_7,  // USER_BUTTON

  // ENDSTOPS
  PD_8,  // X_STOP
  PD_9,  // Y_STOP
  PD_10, // Z_STOP
  PD_11, // U_STOP
  PA_8,  // V_STOP
  PD_0,  // W_STOP

  // HEATERS
  PD_13, // BED_HEAT_2
  PD_14, // BED_HEAT_1
  PD_15, // BED_HEAT_3
  PC_7,  // E1_HEAT_PWM
  PB_0,  // E2_HEAT_PWM
  PB_1,  // E3_HEAT_PWM

  // THERMISTOR
  PC_2,  // BED_THERMISTOR_1
  PC_3,  // BED_THERMISTOR_2
  PA_3,  // BED_THERMISTOR_3
  PA_0,  // E1_THERMISTOR
  PA_1,  // E2_THERMISTOR
  PA_2,  // E3_THERMISTOR

  // FANS
  PC_4,  // E1_FAN
  PC_5,  // E2_FAN
  PE_8,  // E3_FAN

  // X_MOTOR
  PE_13, // X_RESET
  PE_14, // X_PWM
  PE_15, // X_DIR

  // Y_MOTOR
  PE_10, // Y_RESET
  PB_10, // Y_PWM
  PE_9,  // Y_DIR

  // Z_MOTOR
  PC_15, // Z_RESET
  PC_6,  // Z_PWM
  PC_0,  // Z_DIR

  // E1_MOTOR
  PC_14, // E1_RESET
  PC_13, // E1_DIR
  PD_12, // E1_PWM

  // E2_MOTOR
  PE_4,  // E2_RESET
  PE_5,  // E2_PWM
  PE_6,  // E2_DIR

  // E3_MOTOR
  PE_3,  // E3_RESET
  PE_2,  // E3_DIR
  PB_8   // E3_PWM
};
#endif

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

  /* Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
}

#ifdef __cplusplus
}
#endif


// PA_0  54 // E1_THERMISTOR
// PA_1  55 // E2_THERMISTOR
// PA_2  56 // E3_THERMISTOR
// PA_3  53 // BED_THERMISTOR_3
// PA_4  16 // SPI_CS
// PA_5  17 // SPI_SCK
// PA_6  18 // SPI_MISO
// PA_7  19 // SPI_MOSI
// PA_8  43 // V_STOP
// PA_9   0 //TX
// PA_10  1 //RX
// PA_11 30 //OTG_DM
// PA_12 31 //OTG_DP
// PA_13 20 // JTAG_TMS/SWDIO
// PA_14 21 // JTAG_TCK/SWCLK
// PA_15 25 // SD_CARD_DETECT
// PB_0  49 // E2_HEAT_PWM
// PB_1  50 // E3_HEAT_PWM
// PB_3  22 // JTAG_TDO/SWO
// PB_4  37 // USER4
// PB_5   6 // WIFI_WAKEUP
// PB_6  15 // SCL
// PB_7  14 // SDA
// PB_8  77 // E3_PWM
// PB_9  35 // USER1
// PB_10 64 // Y_PWM
// PB_12 10 // SPI_CS
// PB_13 13 // SPI_SCK
// PB_14 12 // SPI_MISO
// PB_15 11 // SPI_MOSI
// PC_0  68 // Z_DIR
// PC_1  33 //IR_ON
// PC_2  51 // BED_THERMISTOR_1
// PC_3  52 // BED_THERMISTOR_2
// PC_4  57 // E1_FAN
// PC_5  58 // E2_FAN
// PC_6  67 // Z_PWM
// PC_7  48 // E1_HEAT_PWM
// PC_8  23 // SDIO_D0
// PC_9  24 // SDIO_D1
// PC_10 26 // SDIO_D2
// PC_11 27 // SDIO_D3
// PC_12 28 // SDIO_CK
// PC_13 70 // E1_DIR
// PC_14 69 // E1_RESET
// PC_15 66 // Z_RESET
// PD_0  44 // W_STOP
// PD_1  32 //IR_OUT
// PD_2  29 // SDIO_CMD
// PD_3   2 // CTS
// PD_4   3 // RTS
// PD_5   4 // TX
// PD_6   5 // RX
// PD_7  34 // USER3
// PD_8  39 // X_STOP
// PD_9  40 // Y_STOP
// PD_10 41 // Z_STOP
// PD_11 42 // U_STOP
// PD_12 71 // E1_PWM
// PD_13 45 // BED_HEAT_2
// PD_14 46 // BED_HEAT_1
// PD_15 47 // BED_HEAT_3
// PE_0  36 // USER2
// PE_1   9 // STATUS_LED
// PE_2  76 // E3_DIR
// PE_3  75 // E3_RESET
// PE_4  72 // E2_RESET
// PE_5  73 // E2_PWM
// PE_6  74 // E2_DIR
// PE_7  38 // USER_BUTTON
// PE_8  59 // E3_FAN
// PE_9  65 // Y_DIR
// PE_10 63 // Y_RESET
// PE_11  7 // WIFI_RESET
// PE_12  8 // WIFI_BOOT
// PE_13 60 // X_RESET
// PE_14 61 // X_PWM
// PE_15 62 // X_DIR
