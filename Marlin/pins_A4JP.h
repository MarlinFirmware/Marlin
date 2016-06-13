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

/************************************************
 * Rambo pin assignments MODIFIED FOR A4JP
 ************************************************/

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

// Servo support
#define SERVO0_PIN 22 // Motor header MX1
#define SERVO1_PIN 23 // Motor header MX2
#define SERVO2_PIN 24 // Motor header MX3
#define SERVO3_PIN  5 // PWM header pin 5

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN -1
#endif

//Fan_2 2

/*****************
#if ENABLED(ULTRA_LCD)

  #define KILL_PIN -1 //was 80 Glen maybe a mistake

#endif // ULTRA_LCD */

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN 44
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0  70
  #define DOGLCD_CS  71
  #define LCD_SCREEN_ROT_180

  #define SD_DETECT_PIN -1 // Pin 72 if using easy adapter board

  #if ENABLED(TEMP_STAT_LEDS)
    #define STAT_LED_RED      22
    #define STAT_LED_BLUE     32
  #endif
#endif // VIKI2/miniVIKI

#define FILWIDTH_PIN        3  // ANALOG NUMBERING

/************************************************
 * Rambo pin assignments old
 ************************************************/

#define LARGE_FLASH true
#define X_STEP_PIN    37
#define X_DIR_PIN     48
#define X_MIN_PIN     12
#define X_MAX_PIN     24
#define X_ENABLE_PIN  29
#define X_MS1_PIN     40
#define X_MS2_PIN     41
#define Y_STEP_PIN    36
#define Y_DIR_PIN     49
#define Y_MIN_PIN     11
#define Y_MAX_PIN     23
#define Y_ENABLE_PIN  28
#define Y_MS1_PIN     69
#define Y_MS2_PIN     39
#define Z_STEP_PIN    35
#define Z_DIR_PIN     47
#define Z_MIN_PIN     10
#define Z_MAX_PIN     30
#define Z_ENABLE_PIN  27
#define Z_MS1_PIN     68
#define Z_MS2_PIN     67

#define HEATER_BED_PIN 3
#define TEMP_BED_PIN   7 //2014/02/04  0:T0 / 1:T1 / 2:T2 / 7:T3
#define HEATER_0_PIN   9
#define TEMP_0_PIN     0
#define HEATER_1_PIN   7

#define E0_STEP_PIN   34
#define E0_DIR_PIN    43
#define E0_ENABLE_PIN 26
#define E0_MS1_PIN    65
#define E0_MS2_PIN    66
#define E1_STEP_PIN   33
#define E1_DIR_PIN    42
#define E1_ENABLE_PIN 25
#define E1_MS1_PIN    63
#define E1_MS2_PIN    64

#define DIGIPOTSS_PIN 38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

#define SDSS          53
#define LED_PIN       13
#define FAN_PIN        8
#define PS_ON_PIN      4
#define FAN_0_PIN      6 //Glen
#define FAN_1_PIN      2 //Glen

// 2015/12/23

#define LCD_PINS_RS     70 //ext2_5
#define LCD_PINS_ENABLE 71 //ext2_7
#define LCD_PINS_D4     72 ///////Ext2 9 ?
#define LCD_PINS_D5     73 ///////Ext2 11 ?
#define LCD_PINS_D6     74 //ext2_13
#define LCD_PINS_D7     75 ///////Ext2 15 ?
#define BEEPER_PIN      -1

#define BTN_HOME        80 //ext_16
#define BTN_CENTER      81 //ext_14
#define BTN_ENC         BTN_CENTER
#define BTN_RIGHT       82 //ext_12
#define BTN_LEFT        83 //ext_10
#define BTN_UP          84 //ext2_8
#define BTN_DOWN        85 //ext2_6

#define HOME_PIN        BTN_HOME
