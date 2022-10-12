/*
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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
#include "Arduino.h"
#include "PeripheralPins.h"

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have to know what you do
// =====

//*** ADC ***

#ifdef HAL_ADC_MODULE_ENABLED
WEAK const PinMap PinMap_ADC[] = {
   {PC_0,  ADC1,  STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC1_IN10   THBED
   {PC_1,  ADC1,  STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC1_IN11   TH0
   {PC_2,  ADC1,  STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC1_IN12   TH1
   {PC_3,  ADC1,  STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 13, 0)}, // ADC1_IN13   TH2
  {NC,    NP,    0}
};
#endif

//*** I2C ***

#ifdef HAL_I2C_MODULE_ENABLED
WEAK const PinMap PinMap_I2C_SDA[] = {
  {PB_9,  I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_I2C_SCL[] = {
  {PB_8,  I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
  {NC,    NP,    0}
};
#endif

//*** PWM ***

#ifdef HAL_TIM_MODULE_ENABLED
// Some pins can perform PWM from more than one timer. These were selected to utilize as many channels as
// possible from timers which were already dedicated to PWM output.

// TIM1 = HEATER0, HEATER1, [SERVO]
// TIM2 = FAN1, FAN2, [BEEPER]
// TIM4 = HEATER_BED
// TIM5 = HEATER2, FAN0

WEAK const PinMap PinMap_PWM[] = {
  {PA_0,  TIM2,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1        Fan2
  {PA_1,  TIM2,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2        Fan1
  {PA_2,  TIM5,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 3, 0)}, // TIM5_CH3        Fan0
  {PA_3,  TIM5,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 4, 0)}, // TIM5_CH4        HE2 
  {PA_8,  TIM1,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)}, // TIM1_CH1        Servo 
  {PB_0,  TIM1,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N       HE1
  {PB_1,  TIM1,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N       HE0
  {PB_2,  TIM2,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4        BEEPER
  {PD_12, TIM4,   STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)}, // TIM4_CH1        HOTBED
  {NC,    NP,    0}
};
#endif

//*** SERIAL ***

#ifdef HAL_UART_MODULE_ENABLED
WEAK const PinMap PinMap_UART_TX[] = {
  {PA_9,  USART1,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
  {PB_10, USART3,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_UART_RX[] = {
  {PA_10, USART1,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
  {PB_11, USART3,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_UART_RTS[] = {
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_UART_CTS[] = {
  {NC,    NP,    0}
};
#endif

//*** SPI ***

#ifdef HAL_SPI_MODULE_ENABLED
WEAK const PinMap PinMap_SPI_MOSI[] = {
  {PA_7,  SPI1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
  {PC_12, SPI3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_SPI_MISO[] = {
  {PA_6,  SPI1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
  {PC_11, SPI3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_SPI_SCLK[] = {
  {PA_5,  SPI1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
  {PC_10, SPI3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_SPI_SSEL[] = {
  {PA_4,  SPI1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
  {NC,    NP,    0}
};
#endif

//*** CAN ***

#ifdef HAL_CAN_MODULE_ENABLED
WEAK const PinMap PinMap_CAN_RD[] = {
  {PB_12, CAN2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)},
  {NC,    NP,    0}
};

const PinMap PinMap_CAN_TD[] = {
  {PB_13, CAN2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN2)},
  {NC,    NP,    0}
};
#endif

//*** USB ***

// If anyone for some unfathomable reason want to run gcode from Marlin's USB-C drive at 12Mbps - you can
#ifdef HAL_PCD_MODULE_ENABLED
WEAK const PinMap PinMap_USB_OTG_FS[] = {
  {PA_11, USB_OTG_FS, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF10_OTG_FS)}, // USB_OTG_FS_DM
  {PA_12, USB_OTG_FS, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF10_OTG_FS)}, // USB_OTG_FS_DP
  {NC,    NP,    0}
};

WEAK const PinMap PinMap_USB_OTG_HS[] = {
  {NC,    NP,    0}
};
#endif


