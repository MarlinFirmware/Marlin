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

/*****************************************************************************
 * @file     lcd/e3v2/common/encoder.cpp
 * @brief    Rotary encoder functions
 *****************************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if HAS_DWIN_E3V2

#include "encoder.h"
#include "../../buttons.h"

#include "../../../MarlinCore.h"
#include "../../marlinui.h"
#include "../../../HAL/shared/Delay.h"

#if HAS_BUZZER
  #include "../../../libs/buzzer.h"
#endif

#include <stdlib.h>

#ifndef ENCODER_PULSES_PER_STEP
  #define ENCODER_PULSES_PER_STEP 4
#endif

ENCODER_Rate EncoderRate;

// TODO: Replace with ui.quick_feedback
void Encoder_tick() {
  #if PIN_EXISTS(BEEPER)
    if (ui.buzzer_enabled) {
      WRITE(BEEPER_PIN, HIGH);
      delay(10);
      WRITE(BEEPER_PIN, LOW);
    }
  #endif
}

// Encoder initialization
void Encoder_Configuration() {
  #if BUTTON_EXISTS(EN1)
    SET_INPUT_PULLUP(BTN_EN1);
  #endif
  #if BUTTON_EXISTS(EN2)
    SET_INPUT_PULLUP(BTN_EN2);
  #endif
  #if BUTTON_EXISTS(ENC)
    SET_INPUT_PULLUP(BTN_ENC);
  #endif
  #if PIN_EXISTS(BEEPER)
    SET_OUTPUT(BEEPER_PIN);     // TODO: Use buzzer.h which already inits this
  #endif
}

// Analyze encoder value and return state
EncoderState Encoder_ReceiveAnalyze() {
  const millis_t now = millis();
  static uint8_t lastEncoderBits;
  uint8_t newbutton = 0;
  static signed char temp_diff = 0;

  EncoderState temp_diffState = ENCODER_DIFF_NO;
  if (BUTTON_PRESSED(EN1)) newbutton |= EN_A;
  if (BUTTON_PRESSED(EN2)) newbutton |= EN_B;
  if (BUTTON_PRESSED(ENC)) {
    static millis_t next_click_update_ms;
    if (ELAPSED(now, next_click_update_ms)) {
      next_click_update_ms = millis() + 300;
      Encoder_tick();
      #if PIN_EXISTS(LCD_LED)
        //LED_Action();
      #endif
      if (!ui.backlight) ui.refresh_brightness();
      const bool was_waiting = wait_for_user;
      wait_for_user = false;
      return was_waiting ? ENCODER_DIFF_NO : ENCODER_DIFF_ENTER;
    }
    else return ENCODER_DIFF_NO;
  }
  if (newbutton != lastEncoderBits) {
    switch (newbutton) {
      case ENCODER_PHASE_0:
             if (lastEncoderBits == ENCODER_PHASE_3) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_1) temp_diff--;
        break;
      case ENCODER_PHASE_1:
             if (lastEncoderBits == ENCODER_PHASE_0) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_2) temp_diff--;
        break;
      case ENCODER_PHASE_2:
             if (lastEncoderBits == ENCODER_PHASE_1) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_3) temp_diff--;
        break;
      case ENCODER_PHASE_3:
             if (lastEncoderBits == ENCODER_PHASE_2) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_0) temp_diff--;
        break;
    }
    lastEncoderBits = newbutton;
  }

  if (ABS(temp_diff) >= ENCODER_PULSES_PER_STEP) {
    if (temp_diff > 0) temp_diffState = TERN(REVERSE_ENCODER_DIRECTION, ENCODER_DIFF_CCW, ENCODER_DIFF_CW);
    else temp_diffState = TERN(REVERSE_ENCODER_DIRECTION, ENCODER_DIFF_CW, ENCODER_DIFF_CCW);

    #if ENABLED(ENCODER_RATE_MULTIPLIER)

      millis_t ms = millis();
      int32_t encoderMultiplier = 1;

      // if must encoder rati multiplier
      if (EncoderRate.enabled) {
        const float abs_diff = ABS(temp_diff),
                    encoderMovementSteps = abs_diff / (ENCODER_PULSES_PER_STEP);
        if (EncoderRate.lastEncoderTime) {
          // Note that the rate is always calculated between two passes through the
          // loop and that the abs of the temp_diff value is tracked.
          const float encoderStepRate = encoderMovementSteps / float(ms - EncoderRate.lastEncoderTime) * 1000;
               if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC) encoderMultiplier = 100;
          else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC)  encoderMultiplier = 10;
          #if ENCODER_5X_STEPS_PER_SEC
            else if (encoderStepRate >= ENCODER_5X_STEPS_PER_SEC) encoderMultiplier = 5;
          #endif
        }
        EncoderRate.lastEncoderTime = ms;
      }

    #else

      constexpr int32_t encoderMultiplier = 1;

    #endif

    // EncoderRate.encoderMoveValue += (temp_diff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
    EncoderRate.encoderMoveValue = (temp_diff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
    if (EncoderRate.encoderMoveValue < 0) EncoderRate.encoderMoveValue = -EncoderRate.encoderMoveValue;

    temp_diff = 0;
  }
  return temp_diffState;
}

#if PIN_EXISTS(LCD_LED)

  // Take the low 24 valid bits  24Bit: G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
  uint16_t LED_DataArray[LED_NUM];

  // LED light operation
  void LED_Action() {
    LED_Control(RGB_SCALE_WARM_WHITE,0x0F);
    delay(30);
    LED_Control(RGB_SCALE_WARM_WHITE,0x00);
  }

  // LED initialization
  void LED_Configuration() {
    SET_OUTPUT(LCD_LED_PIN);
  }

  // LED write data
  void LED_WriteData() {
    uint8_t tempCounter_LED, tempCounter_Bit;
    for (tempCounter_LED = 0; tempCounter_LED < LED_NUM; tempCounter_LED++) {
      for (tempCounter_Bit = 0; tempCounter_Bit < 24; tempCounter_Bit++) {
        if (LED_DataArray[tempCounter_LED] & (0x800000 >> tempCounter_Bit)) {
          LED_DATA_HIGH;
          DELAY_NS(300);
          LED_DATA_LOW;
          DELAY_NS(200);
        }
        else {
          LED_DATA_HIGH;
          LED_DATA_LOW;
          DELAY_NS(200);
        }
      }
    }
  }

  // LED control
  //  RGB_Scale: RGB color ratio
  //  luminance: brightness (0~0xFF)
  void LED_Control(const uint8_t RGB_Scale, const uint8_t luminance) {
    for (uint8_t i = 0; i < LED_NUM; i++) {
      LED_DataArray[i] = 0;
      switch (RGB_Scale) {
        case RGB_SCALE_R10_G7_B5: LED_DataArray[i] = (luminance * 10/10) << 8 | (luminance * 7/10) << 16 | luminance * 5/10; break;
        case RGB_SCALE_R10_G7_B4: LED_DataArray[i] = (luminance * 10/10) << 8 | (luminance * 7/10) << 16 | luminance * 4/10; break;
        case RGB_SCALE_R10_G8_B7: LED_DataArray[i] = (luminance * 10/10) << 8 | (luminance * 8/10) << 16 | luminance * 7/10; break;
      }
    }
    LED_WriteData();
  }

  // LED gradient control
  //  RGB_Scale: RGB color ratio
  //  luminance: brightness (0~0xFF)
  //  change_Time: gradient time (ms)
  void LED_GraduallyControl(const uint8_t RGB_Scale, const uint8_t luminance, const uint16_t change_Interval) {
    struct { uint8_t g, r, b; } led_data[LED_NUM];
    for (uint8_t i = 0; i < LED_NUM; i++) {
      switch (RGB_Scale) {
        case RGB_SCALE_R10_G7_B5:
          led_data[i] = { luminance * 7/10, luminance * 10/10, luminance * 5/10 };
          break;
        case RGB_SCALE_R10_G7_B4:
          led_data[i] = { luminance * 7/10, luminance * 10/10, luminance * 4/10 };
          break;
        case RGB_SCALE_R10_G8_B7:
          led_data[i] = { luminance * 8/10, luminance * 10/10, luminance * 7/10 };
          break;
      }
    }

    struct { bool g, r, b; } led_flag = { false, false, false };
    for (uint8_t i = 0; i < LED_NUM; i++) {
      while (1) {
        const uint8_t g = uint8_t(LED_DataArray[i] >> 16),
                      r = uint8_t(LED_DataArray[i] >> 8),
                      b = uint8_t(LED_DataArray[i]);
        if (g == led_data[i].g) led_flag.g = true;
        else LED_DataArray[i] += (g > led_data[i].g) ? -0x010000 : 0x010000;
        if (r == led_data[i].r) led_flag.r = true;
        else LED_DataArray[i] += (r > led_data[i].r) ? -0x000100 : 0x000100;
        if (b == led_data[i].b) led_flag.b = true;
        else LED_DataArray[i] += (b > led_data[i].b) ? -0x000001 : 0x000001;
        LED_WriteData();
        if (led_flag.r && led_flag.g && led_flag.b) break;
        delay(change_Interval);
      }
    }
  }

#endif // LCD_LED

#endif // HAS_DWIN_E3V2
