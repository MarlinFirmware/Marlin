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

#ifndef __STM32F1__
  #error "Oops!  Make sure you have an STM32F1 board selected from the 'Tools -> Boards' menu."
#endif

/**
 * 21017 Victor Perez Marlin for stm32f1 test
 */

#define DEFAULT_MACHINE_NAME "3D Printer"
//#define BOARD_NAME "Marlin for STM32"

#ifndef BOARD_NAME
  #define BOARD_NAME "FYSETC AIO II"
  #define DEFAULT_WEBSITE_URL "https://fysetc.com"
#endif

#define pins_v2_20190128 // geo-f:add for new pins define 

// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define U_MIN_PIN          -1
#define V_MIN_PIN          -1
#define W_MIN_PIN          -1
#define X_STOP_PIN         PA1
#define Y_STOP_PIN         PA0
#define Z_STOP_PIN         PB14

//
// Steppers
//
#define X_STEP_PIN         PB8
#define X_DIR_PIN          PB9
#define X_ENABLE_PIN       PA8

#define Y_STEP_PIN         PB2
#ifdef pins_v2_20190128
#define Y_DIR_PIN          PB3
#else
#define Y_DIR_PIN          PB0
#endif
#define Y_ENABLE_PIN       PB1

#define Z_STEP_PIN         PC0
#define Z_DIR_PIN          PC1
#define Z_ENABLE_PIN       PC2

#define E0_STEP_PIN        PC15
#define E0_DIR_PIN         PC14
#define E0_ENABLE_PIN      PC13

//
// Misc. Functions
//
#define LED_PIN            -1

// 采用 SDIO PCB从左到右数
// 1:PC10 - SDIO_D2
// 2:PC11 - SDIO_D3
// 3:PD2 - SDIO_CMD
// 4:VCC
// 5:PC12 - SDIO_CK
// 6:VDD
// 7:PC8 - SDIO_D0
// 8:PC9 - SDIO_D1
// 9:PA15 - SD_DETECT_PIN

//
// Filament runout: geo-f:add
//
#ifdef pins_v2_20190128
#define FIL_RUNOUT_PIN     PB15
#else
#define FIL_RUNOUT_PIN     PB5
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC7   // EXTRUDER 1 
//#define HEATER_1_PIN       PB1

#define HEATER_BED_PIN     PC6   // BED

#ifndef FAN_PIN
  #define FAN_PIN          PC8
#endif
#define FAN1_PIN           -1


//
// Temperature Sensors
//
#define TEMP_BED_PIN       PC5   // ANALOG NUMBERING
#define TEMP_0_PIN         PC4   // ANALOG NUMBERING
//#define TEMP_1_PIN         PA2   // ANALOG NUMBERING

// X:PA2 Y:PA3 Z:PB12 E:PB13 // changed for test
#define MOTOR_CURRENT_PWM_XY_PIN PA3 
#define MOTOR_CURRENT_PWM_Z_PIN  PA2 // PB12
//#define MOTOR_CURRENT_PWM_XY_PIN PB6 
//#define MOTOR_CURRENT_PWM_Z_PIN  PB7 // PB12
#define MOTOR_CURRENT_PWM_E_PIN  -1  // PB13
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE 1500 // geo-f:old 2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {500, 500, 400} // geo-f:old 1300 1300 1250


//
// LCD Pins
//
#ifdef pins_v2_20190128
#define LCD_RESET_PIN			PB4 // geo-f : add
#define LCD_RED_PIN				PB0 // geo-f:add
#define LCD_GREEN_PIN			PB6 // geo-f:add
#define LCD_BLUE_PIN 			PB7 // geo-f:add
#else
#define LCD_RESET_PIN			PB6 // geo-f:add
#define LCD_RED_PIN				PB3 // geo-f:add
#define LCD_GREEN_PIN			PB4 // geo-f:add
#define LCD_BLUE_PIN 			PB5 // geo-f:add
#endif

#define BEEPER_PIN        PC9
// Pins for DOGM SPI LCD Support
#define DOGLCD_A0         PA15
#ifdef pins_v2_20190128
#define DOGLCD_CS         PB5
#else
#define DOGLCD_CS         PB7
#endif
#define LCD_BACKLIGHT_PIN -1   // backlight LED on A11/D65
#define SDSS              PA4

#define KILL_PIN          -1
// GLCD features
//#define LCD_CONTRAST   190
// Uncomment screen orientation
//#define LCD_SCREEN_ROT_90
//#define LCD_SCREEN_ROT_180
//#define LCD_SCREEN_ROT_270
// The encoder and click button
#define BTN_EN1           PC10
#define BTN_EN2           PC11
#define BTN_ENC           PC12
// not connected to a pin
#define SD_DETECT_PIN     PC3

#define RGB_LED_R_PIN PB0
#define RGB_LED_G_PIN PB6
#define RGB_LED_B_PIN PB7


/*
#if ENABLED(ULTRA_LCD)

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    #define LCD_PINS_RS         49   // CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE     51   // SID (MOSI)
    #define LCD_PINS_D4         52   // SCK (CLK) clock
  #elif ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    #define LCD_PINS_RS         PB8
    #define LCD_PINS_ENABLE     PD2
    #define LCD_PINS_D4         PB12
    #define LCD_PINS_D5         PB13
    #define LCD_PINS_D6         PB14
    #define LCD_PINS_D7         PB15
  #else
    #define LCD_PINS_RS         PB9
    #define LCD_PINS_ENABLE     PD2
    #define LCD_PINS_D4         PC9
    #define LCD_PINS_D5         PC10
    #define LCD_PINS_D6         PB5
    #define LCD_PINS_D7         PB6
    #if DISABLED(NEWPANEL)
      #define BEEPER_PIN        33
      // Buttons are attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17
    #endif
  #endif

  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

      #define BEEPER_PIN        PC12

      #define BTN_EN1           PB4
      #define BTN_EN2           PC8
      #define BTN_ENC           PC11

      #define SD_DETECT_PIN     PB8
      #define KILL_PIN          -1

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_BACKLIGHT_PIN 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      #define BTN_EN1           64
      #define BTN_EN2           59
      #define BTN_ENC           63
      #define SD_DETECT_PIN     42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      #define BTN_EN1           47
      #define BTN_EN2           43
      #define BTN_ENC           32
      #define LCD_SDSS          53
      #define SD_DETECT_PIN     -1
      #define KILL_PIN          41

    #elif ENABLED(LCD_I2C_VIKI)

      #define BTN_EN1           22   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      #define BTN_EN2            7   // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.

      #define BTN_ENC           -1
      #define LCD_SDSS          53
      #define SD_DETECT_PIN     49

    #elif ENABLED(VIKI2) || ENABLED(miniVIKI)

      #define BEEPER_PIN        33

      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0         44
      #define DOGLCD_CS         45
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1           22
      #define BTN_EN2            7
      #define BTN_ENC           39

      #define SDSS              53
      #define SD_DETECT_PIN     -1   // Pin 49 for display sd interface, 72 for easy adapter board

      #define KILL_PIN          31

      #define STAT_LED_RED_PIN  32
      #define STAT_LED_BLUE_PIN 35

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #define BTN_EN1           35
      #define BTN_EN2           37
      #define BTN_ENC           31
      #define SD_DETECT_PIN     49
      #define LCD_SDSS          53
      #define KILL_PIN          41
      #define BEEPER_PIN        23
      #define DOGLCD_CS         29
      #define DOGLCD_A0         27
      #define LCD_BACKLIGHT_PIN 33

    #elif ENABLED(MINIPANEL)
		
			#define LCD_RESET_PIN			PB12 // geo-f:add
			#define LCD_RED 					PB13 // geo-f:add
			#define LCD_GREEN 				PB14 // geo-f:add
			#define LCD_BLUE		 			PB15 // geo-f:add

      #define BEEPER_PIN        PC12
      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0         PB8
      #define DOGLCD_CS         PD2
      #define LCD_BACKLIGHT_PIN -1   // backlight LED on A11/D65
      #define SDSS              PB7

      #define KILL_PIN          -1
      // GLCD features
      //#define LCD_CONTRAST   190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      // The encoder and click button
			#define BTN_EN1           PB4
      #define BTN_EN2           PC8
      #define BTN_ENC           PC11
      // not connected to a pin
      #define SD_DETECT_PIN     PB8

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN        33

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define BTN_EN1         64
        #define BTN_EN2         59
        #define BTN_ENC         63
        #define SHIFT_OUT       40
        #define SHIFT_CLK       44
        #define SHIFT_LD        42
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1         59   // AUX2 PIN 3
        #define BTN_EN2         63   // AUX2 PIN 4
        #define BTN_ENC         49   // AUX3 PIN 7
      #else
        #define BTN_EN1         37
        #define BTN_EN2         35
        #define BTN_ENC         31
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN   49
        #define KILL_PIN        41
      #else
        //#define SD_DETECT_PIN -1   // Ramps doesn't use this
      #endif

    #endif
  #endif // NEWPANEL

#endif // ULTRA_LCD
*/
