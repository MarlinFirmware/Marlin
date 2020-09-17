/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * @file     rotary_encoder.cpp
 * @author   LEO / Creality3D
 * @date     2019/07/06
 * @version  2.0.1
 * @brief    Rotary encoder functions
 *****************************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#include "rotary_encoder.h"

#include "../../../MarlinCore.h"
#include "../../../HAL/shared/Delay.h"

#if HAS_BUZZER
  #include "../../../libs/buzzer.h"
#endif

#include <stdlib.h>

ENCODER_Rate EncoderRate;

// Buzzer
void Encoder_tick(void) {
  WRITE(BEEPER_PIN, 1);
  delay(10);
  WRITE(BEEPER_PIN, 0);
}

// Encoder initialization
void Encoder_Configuration(void) {
  #if BUTTON_EXISTS(EN1)
    SET_INPUT_PULLUP(BTN_EN1);
  #endif
  #if BUTTON_EXISTS(EN2)
    SET_INPUT_PULLUP(BTN_EN2);
  #endif
  #if BUTTON_EXISTS(ENC)
    SET_INPUT_PULLUP(BTN_ENC);
  #endif
  #ifdef BEEPER_PIN
    SET_OUTPUT(BEEPER_PIN);
  #endif
}

// Analyze encoder value and return state
ENCODER_DiffState Encoder_ReceiveAnalyze(void) {
  const millis_t now = millis();
  static unsigned char lastEncoderBits;
  unsigned char newbutton = 0;
  static signed char temp_diff = 0;

  ENCODER_DiffState temp_diffState = ENCODER_DIFF_NO;
  if (BUTTON_PRESSED(EN1)) newbutton |= 0x01;
  if (BUTTON_PRESSED(EN2)) newbutton |= 0x02;
  if (BUTTON_PRESSED(ENC)) {
    static millis_t next_click_update_ms;
    if (ELAPSED(now, next_click_update_ms)) {
      next_click_update_ms = millis() + 300;
      Encoder_tick();
      #if PIN_EXISTS(LCD_LED)
        //LED_Action();
      #endif
      const bool was_waiting = wait_for_user;
      wait_for_user = false;
      return was_waiting ? ENCODER_DIFF_NO : ENCODER_DIFF_ENTER;
    }
    else return ENCODER_DIFF_NO;
  }
  if (newbutton != lastEncoderBits) {
    switch (newbutton) {
      case ENCODER_PHASE_0: {
        if (lastEncoderBits == ENCODER_PHASE_3) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_1) temp_diff--;
      }break;
      case ENCODER_PHASE_1: {
        if (lastEncoderBits == ENCODER_PHASE_0) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_2) temp_diff--;
      }break;
      case ENCODER_PHASE_2: {
        if (lastEncoderBits == ENCODER_PHASE_1) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_3) temp_diff--;
      }break;
      case ENCODER_PHASE_3: {
        if (lastEncoderBits == ENCODER_PHASE_2) temp_diff++;
        else if (lastEncoderBits == ENCODER_PHASE_0) temp_diff--;
      }break;
    }
    lastEncoderBits = newbutton;
  }

  if (abs(temp_diff) >= ENCODER_PULSES_PER_STEP) {
    if (temp_diff > 0) temp_diffState = ENCODER_DIFF_CW;
    else temp_diffState = ENCODER_DIFF_CCW;

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
          else if (encoderStepRate >= ENCODER_5X_STEPS_PER_SEC)   encoderMultiplier = 5;
        }
        EncoderRate.lastEncoderTime = ms;
      }
    #else
      constexpr int32_t encoderMultiplier = 1;
    #endif // ENCODER_RATE_MULTIPLIER

    // EncoderRate.encoderMoveValue += (temp_diff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
    EncoderRate.encoderMoveValue = (temp_diff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
    if (EncoderRate.encoderMoveValue < 0) EncoderRate.encoderMoveValue = -EncoderRate.encoderMoveValue;

    temp_diff = 0;
  }
  return temp_diffState;
}

#if PIN_EXISTS(LCD_LED)

  // Take the low 24 valid bits  24Bit: G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
  unsigned int LED_DataArray[LED_NUM];

  // LED light operation
  void LED_Action(void) {
    LED_Control(RGB_SCALE_WARM_WHITE,0x0F);
    delay(30);
    LED_Control(RGB_SCALE_WARM_WHITE,0x00);
  }

  // LED initialization
  void LED_Configuration(void) {
    SET_OUTPUT(LCD_LED_PIN);
  }

  // LED write data
  void LED_WriteData(void) {
    unsigned char tempCounter_LED, tempCounter_Bit;
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
  void LED_Control(unsigned char RGB_Scale, unsigned char luminance) {
    unsigned char temp_Counter;
    for (temp_Counter = 0; temp_Counter < LED_NUM; temp_Counter++) {
      LED_DataArray[temp_Counter] = 0;
      switch(RGB_Scale) {
        case RGB_SCALE_R10_G7_B5: LED_DataArray[temp_Counter] = (luminance*10/10) << 8 | (luminance*7/10) << 16 | luminance*5/10; break;
        case RGB_SCALE_R10_G7_B4: LED_DataArray[temp_Counter] = (luminance*10/10) << 8 | (luminance*7/10) << 16 | luminance*4/10; break;
        case RGB_SCALE_R10_G8_B7: LED_DataArray[temp_Counter] = (luminance*10/10) << 8 | (luminance*8/10) << 16 | luminance*7/10; break;
      }
    }
    LED_WriteData();
  }

  // LED gradient control
  //  RGB_Scale: RGB color ratio
  //  luminance: brightness (0~0xFF)
  //  change_Time: gradient time (ms)
  void LED_GraduallyControl(unsigned char RGB_Scale, unsigned char luminance, unsigned int change_Interval) {
    unsigned char temp_Counter;
    unsigned char LED_R_Data[LED_NUM], LED_G_Data[LED_NUM], LED_B_Data[LED_NUM];
    bool LED_R_Flag = 0, LED_G_Flag = 0, LED_B_Flag = 0;

    for (temp_Counter = 0; temp_Counter < LED_NUM; temp_Counter++) {
      switch(RGB_Scale) {
        case RGB_SCALE_R10_G7_B5: {
          LED_R_Data[temp_Counter] = luminance*10/10;
          LED_G_Data[temp_Counter] = luminance*7/10;
          LED_B_Data[temp_Counter] = luminance*5/10;
        }break;
        case RGB_SCALE_R10_G7_B4: {
          LED_R_Data[temp_Counter] = luminance*10/10;
          LED_G_Data[temp_Counter] = luminance*7/10;
          LED_B_Data[temp_Counter] = luminance*4/10;
        }break;
        case RGB_SCALE_R10_G8_B7: {
          LED_R_Data[temp_Counter] = luminance*10/10;
          LED_G_Data[temp_Counter] = luminance*8/10;
          LED_B_Data[temp_Counter] = luminance*7/10;
        }break;
      }
    }
      for (temp_Counter = 0; temp_Counter < LED_NUM; temp_Counter++) {
        if ((unsigned char)(LED_DataArray[temp_Counter] >> 8) > LED_R_Data[temp_Counter]) LED_DataArray[temp_Counter] -= 0x000100;
        else if ((unsigned char)(LED_DataArray[temp_Counter] >> 8) < LED_R_Data[temp_Counter]) LED_DataArray[temp_Counter] += 0x000100;
    while (1) {
        else LED_R_Flag = 1;
        if ((unsigned char)(LED_DataArray[temp_Counter]>>16) > LED_G_Data[temp_Counter]) LED_DataArray[temp_Counter] -= 0x010000;
        else if ((unsigned char)(LED_DataArray[temp_Counter]>>16) < LED_G_Data[temp_Counter]) LED_DataArray[temp_Counter] += 0x010000;
        else LED_G_Flag = 1;
        if ((unsigned char)LED_DataArray[temp_Counter] > LED_B_Data[temp_Counter]) LED_DataArray[temp_Counter] -= 0x000001;
        else if ((unsigned char)LED_DataArray[temp_Counter] < LED_B_Data[temp_Counter]) LED_DataArray[temp_Counter] += 0x000001;
        else LED_B_Flag = 1;
      }
      LED_WriteData();
      if (LED_R_Flag && LED_G_Flag && LED_B_Flag) break;
      else delay(change_Interval);
    }
  }

#endif // LCD_LED

#endif // DWIN_CREALITY_LCD
