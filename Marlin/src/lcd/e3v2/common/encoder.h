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
} EncoderRate;

extern EncoderRate encoderRate;

typedef enum {
  ENCODER_DIFF_NO    = 0,  // no state
  ENCODER_DIFF_CW    = 1,  // clockwise rotation
  ENCODER_DIFF_CCW   = 2,  // counterclockwise rotation
  ENCODER_DIFF_ENTER = 3   // click
} EncoderState;

#define ENCODER_WAIT_MS TERN(DWIN_LCD_PROUI, 10, 20)

// Encoder initialization
void encoderConfiguration();

// Analyze encoder value and return state
EncoderState encoderReceiveAnalyze();

inline EncoderState get_encoder_state() {
  static millis_t Encoder_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const EncoderState state = encoderReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT_MS;
  return state;
}

template<typename T>
inline bool applyEncoder(const EncoderState &encoder_diffState, T &valref) {
  if (encoder_diffState == ENCODER_DIFF_CW)
    valref += encoderRate.encoderMoveValue;
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    valref -= encoderRate.encoderMoveValue;
  return encoder_diffState == ENCODER_DIFF_ENTER;
}

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
