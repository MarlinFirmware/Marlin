/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/*****************************************************************************
  * @file     lcd/e3v2/common/encoder.h
  * @brief    Rotary encoder functions
  ****************************************************************************/

#include "../../../inc/MarlinConfig.h"

/*********************** Encoder Set ***********************/

typedef struct {
  bool enabled = false;
  int encoderMoveValue = 0;
  millis_t lastEncoderTime = 0;
} ENCODER_Rate;

extern ENCODER_Rate EncoderRate;

typedef enum {
  ENCODER_DIFF_NO    = 0,  // no state
  ENCODER_DIFF_CW    = 1,  // clockwise rotation
  ENCODER_DIFF_CCW   = 2,  // counterclockwise rotation
  ENCODER_DIFF_ENTER = 3   // click
} EncoderState;

// Encoder initialization
void Encoder_Configuration();

// Analyze encoder value and return state
EncoderState Encoder_ReceiveAnalyze();

/*********************** Encoder LED ***********************/

#if PIN_EXISTS(LCD_LED)

  #define LED_NUM  4
  #define LED_DATA_HIGH  WRITE(LCD_LED_PIN, 1)
  #define LED_DATA_LOW   WRITE(LCD_LED_PIN, 0)

  #define RGB_SCALE_R10_G7_B5  1
  #define RGB_SCALE_R10_G7_B4  2
  #define RGB_SCALE_R10_G8_B7  3
  #define RGB_SCALE_NEUTRAL_WHITE RGB_SCALE_R10_G7_B5
  #define RGB_SCALE_WARM_WHITE    RGB_SCALE_R10_G7_B4
  #define RGB_SCALE_COOL_WHITE    RGB_SCALE_R10_G8_B7

  extern unsigned int LED_DataArray[LED_NUM];

  // LED light operation
  void LED_Action();

  // LED initialization
  void LED_Configuration();

  // LED write data
  void LED_WriteData();

  // LED control
  //  RGB_Scale: RGB color ratio
  //  luminance: brightness (0~0xFF)
  void LED_Control(const uint8_t RGB_Scale, const uint8_t luminance);

  // LED gradient control
  //  RGB_Scale: RGB color ratio
  //  luminance: brightness (0~0xFF)
  //  change_Time: gradient time (ms)
  void LED_GraduallyControl(const uint8_t RGB_Scale, const uint8_t luminance, const uint16_t change_Interval);

#endif // LCD_LED
