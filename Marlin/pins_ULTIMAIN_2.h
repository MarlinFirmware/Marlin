/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Ultiboard v2.0 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#define DEFAULT_MACHINE_NAME    "Ultimaker"
#define DEFAULT_SOURCE_CODE_URL "https://github.com/Ultimaker/Marlin"

#define X_STEP_PIN 25
#define X_DIR_PIN 23
#define X_STOP_PIN 22
#define X_ENABLE_PIN 27

#define Y_STEP_PIN 32
#define Y_DIR_PIN 33
#define Y_STOP_PIN 26
#define Y_ENABLE_PIN 31

#define Z_STEP_PIN 35
#define Z_DIR_PIN 36
#define Z_STOP_PIN 29
#define Z_ENABLE_PIN 34

#define HEATER_BED_PIN 4
#define TEMP_BED_PIN 10

#define HEATER_0_PIN  2
#define TEMP_0_PIN 8

#define HEATER_1_PIN 3
#define TEMP_1_PIN 9

#define HEATER_2_PIN -1
#define TEMP_2_PIN -1

#define E0_STEP_PIN         42
#define E0_DIR_PIN          43
#define E0_ENABLE_PIN       37

#define E1_STEP_PIN         49
#define E1_DIR_PIN          47
#define E1_ENABLE_PIN       48

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            8
#define FAN_PIN            7
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.
#define SAFETY_TRIGGERED_PIN     28 //PIN to detect the safety circuit has triggered
#define MAIN_VOLTAGE_MEASURE_PIN 14 //Analogue PIN to measure the main voltage, with a 100k - 4k7 resitor divider.

#define MOTOR_CURRENT_PWM_XY_PIN 44
#define MOTOR_CURRENT_PWM_Z_PIN 45
#define MOTOR_CURRENT_PWM_E_PIN 46
//Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#define MOTOR_CURRENT_PWM_RANGE 2000
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

#define BEEPER_PIN 18

#define LCD_PINS_RS 20
#define LCD_PINS_ENABLE 15
#define LCD_PINS_D4 14
#define LCD_PINS_D5 21
#define LCD_PINS_D6 5
#define LCD_PINS_D7 6

//buttons are directly attached
#define BTN_EN1 40
#define BTN_EN2 41
#define BTN_ENC 19  //the click

#define BLEN_C 2
#define BLEN_B 1
#define BLEN_A 0

#define SD_DETECT_PIN 39
