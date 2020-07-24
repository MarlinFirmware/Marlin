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
#pragma once

/**
  ******************************************************************************
  * @file     rotary_encoder.h
  * @author   LEO / Creality3D
  * @date     2019/07/06
  * @version  2.0.1
  * @brief    旋转编码器操作函数
  ******************************************************************************
**/

#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"

/*********************** Encoder Set ***********************/

#define ENCODER_PHASE_0  0
#define ENCODER_PHASE_1  2
#define ENCODER_PHASE_2  3
#define ENCODER_PHASE_3  1

#define ENCODER_PULSES_PER_STEP  4

#define BUTTON_PRESSED(BN) !READ(BTN_## BN)

typedef struct {
  bool encoderRateEnabled = 0;
  int encoderMoveValue = 0;
  millis_t lastEncoderTime = 0;
} ENCODER_Rate;

extern ENCODER_Rate EncoderRate;

typedef enum {
  ENCODER_DIFF_NO    = 0,
	ENCODER_DIFF_CW    = 1,
  ENCODER_DIFF_CCW   = 2,
	ENCODER_DIFF_ENTER = 3
} ENCODER_DiffState;

/*编码器初始化 PB12:Encoder_A PB13:Encoder_B PB14:Encoder_C*/
void Encoder_Configuration(void);

/*接收数据解析 返回值:ENCODER_DIFF_NO,无状态; ENCODER_DIFF_CW,顺时针旋转; ENCODER_DIFF_CCW,逆时针旋转; ENCODER_DIFF_ENTER,按下*/
ENCODER_DiffState Encoder_ReceiveAnalyze(void);


/*********************** Encoder LED ***********************/

#if PIN_EXISTS(LCD_LED)

  #define LED_NUM  4
  #define LED_DATA_HIGH  WRITE(LCD_LED_PIN, 1)
  #define LED_DATA_LOW   WRITE(LCD_LED_PIN, 0)

  #define RGB_SCALE_R10_G7_B5  1
  #define RGB_SCALE_R10_G7_B4  2
  #define RGB_SCALE_R10_G8_B7  3
  #define RGB_SCALE_NEUTRAL_WHITE  RGB_SCALE_R10_G7_B5 //正白
  #define RGB_SCALE_WARM_WHITE  RGB_SCALE_R10_G7_B4 //暖白
  #define RGB_SCALE_COOL_WHITE  RGB_SCALE_R10_G8_B7 //冷白

  extern unsigned int LED_DataArray[LED_NUM];

  /*状态LED初始化*/
  void STATE_LED_Configuration(void);

	/*LED灯操作*/
  void LED_Action(void);

  /*LED初始化*/
  void LED_Configuration(void);

  /*LED写数据*/
  void LED_WriteData(void);

  /*LED控制 RGB_Scale:RGB色彩配比 luminance:亮度(0~0xFF)*/
  void LED_Control(unsigned char RGB_Scale, unsigned char luminance);

  /*LED渐变控制 RGB_Scale:RGB色彩配比 luminance:亮度(0~0xFF) change_Time:渐变时间(ms)*/
  void LED_GraduallyControl(unsigned char RGB_Scale, unsigned char luminance, unsigned int change_Interval);

#endif
