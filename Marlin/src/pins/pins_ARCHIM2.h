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
 * ARCHIM2 pin assignment (pins_ARCHIM2.h)
 *
 * The Archim2 board requires Arduino Archim addons installed.
 * Add the following URL to Arduino IDE's Additional Board Manager URLs:
 * https://raw.githubusercontent.com/ultimachine/ArduinoAddons/master/package_ultimachine_index.json
 * In the Arduino IDE Board Manager search for Archim and install the package.
 * Change your target board to "Archim".
 *
 * Further information is provided by UltiMachine
 * https://github.com/ultimachine/Archim/wiki/Archim-v2.0
 * https://github.com/ultimachine/Archim/wiki
 *
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Archim' selected from the 'Tools -> Boards' menu."
#endif

//
// Items marked * have been altered from Archim v1.0
//

// TMC2130 Diag Pins (currently just for reference)
#define X_DIAG_PIN         59   // PA4 X_DIAG
#define Y_DIAG_PIN         48   // PC15 Y_DIAG
#define Z_DIAG_PIN         36   // PC4 Z_DIAG
#define E0_DIAG_PIN        78   // PB23 E1_DIAG
#define E1_DIAG_PIN        25   // PD0 E2_DIAG

//
// Servos
//
#define SERVO0_PIN          20   // D20 PB12 (Header J20 20)
#define SERVO1_PIN          21   // D21 PB13 (Header J20 19)

//
// Limit Switches
//
// Only use Diag Pins when SENSORLESS_HOMING is enabled for the TMC2130 drivers.
// Otherwise use a physical endstop based configuration.

#if DISABLED(SENSORLESS_HOMING)
 #define X_MIN_PIN          14   // PD4 MIN ES1
 #define X_MAX_PIN          32   // PD10 MAX ES1
 #define Y_MIN_PIN          29   // PD6 MIN ES2
 #define Y_MAX_PIN          15   // PD5 MAX ES2
#else
  #if X_HOME_DIR == -1
    #define X_MIN_PIN      X_DIAG_PIN
    #define X_MAX_PIN      32
  #else
    #define X_MIN_PIN      14
    #define X_MAX_PIN      X_DIAG_PIN
  #endif

  #if Y_HOME_DIR == -1
    #define Y_MIN_PIN      Y_DIAG_PIN
    #define Y_MAX_PIN      15
  #else
    #define Y_MIN_PIN      29
    #define Y_MAX_PIN      Y_DIAG_PIN
  #endif
#endif

 #define Z_MIN_PIN          31   // PA7 MIN ES3
 #define Z_MAX_PIN          30   // PD9 MAX ES3

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  32
#endif

//
// Steppers
//
#define X_STEP_PIN         38   // PC6 X-STEP *
#define X_DIR_PIN          37   // PC5 X-DIR *
#define X_ENABLE_PIN       41   // PC9 EN1
#ifndef X_CS_PIN
  #define X_CS_PIN         39   // PC7 X_nCS
#endif

#define Y_STEP_PIN         51   // PC12 Y-STEP *
#define Y_DIR_PIN          92   // PC11 Y-DIR -AddOns *
#define Y_ENABLE_PIN       49   // PC14 Y-EN *
#ifndef Y_CS_PIN
  #define Y_CS_PIN         50   // PC13 Y_nCS
#endif

#define Z_STEP_PIN         46   // PC17 Z-STEP *
#define Z_DIR_PIN          47   // PC16 Z-DIR *
#define Z_ENABLE_PIN       44   // PC19 Z-END *
#ifndef Z_CS_PIN
  #define Z_CS_PIN         45   // PC18 Z_nCS
#endif

#define E0_STEP_PIN       107   // PB10 E1-STEP -AddOns *
#define E0_DIR_PIN         96   // PC10 E1-DIR -AddOns *
#define E0_ENABLE_PIN     105   // PB22 E1-EN -AddOns *
#ifndef E0_CS_PIN
  #define E0_CS_PIN       104   // PC20 E1_nCS -AddOns *
#endif

#define E1_STEP_PIN        22   // PB26 E2_STEP *
#define E1_DIR_PIN         97   // PB24 E2_DIR -AddOns *
#define E1_ENABLE_PIN      18   // PA11 E2-EN
#ifndef E1_CS_PIN
  #define E1_CS_PIN        19   // PA10 E2_nCS
#endif

//
// Software SPI pins for TMC2130 stepper drivers.
// Required for the Archim2 board.
//
#if ENABLED(TMC_USE_SW_SPI)
  #define TMC_SW_MOSI      28   // PD3
  #define TMC_SW_MISO      26   // PD1
  #define TMC_SW_SCK       27   // PD2
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         10   // D10 PB19 THERM AN1 *
#define TEMP_1_PIN          9   // D9 PB18 THERM AN2 *
#define TEMP_2_PIN          8   // D8 PB17 THERM AN4 *
#define TEMP_BED_PIN       11   // D11 PB20 THERM AN3 *

//
// Heaters / Fans
//
#ifndef FAN_PIN
  #define FAN_PIN           4   // D4 PC26 FET_PWM1
#endif
#define FAN1_PIN            5   // D5 PC25 FET_PWM2

#define HEATER_0_PIN        6   // D6 PC24 FET_PWM3
#define HEATER_1_PIN        7   // D7 PC23 FET_PWM4
#define HEATER_2_PIN        8   // D8 PC22 FET_PWM5
#define HEATER_BED_PIN      9   // D9 PC21 BED_PWM

//
// Misc. Functions
//

// Internal MicroSD card reader on the PCB
#define INT_SCK_PIN         42   // D42 PA19/MCCK
#define INT_MISO_PIN        43   // D43 PA20/MCCDA
#define INT_MOSI_PIN        73   // D73 PA21/MCDA0
#define INT_SDSS            55   // D55 PA24/MCDA3

// The external SD card reader on SC2, as well as 2 Mbyte SPI Flash
#define SCK_PIN             76   // D76 PA27
#define MISO_PIN            74   // D74 PA25
#define MOSI_PIN            75   // D75 PA26
#define SDSS                87   // D87 PA29
#define SPI_FLASH_SS        52   // D52 PB21

// Filament runout sensor pins (or GPIO)
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN    66   // D66 PB15 (Header J20 15)
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN   67   // D67 PB16 (Header J20 16)
#endif

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)
  #define BEEPER_PIN       23   // D24 PA15_CTS1
  #define LCD_PINS_RS      17   // D17 PA12_RXD1
  #define LCD_PINS_ENABLE  24   // D23 PA14_RTS1
  #define LCD_PINS_D4      69   // D69 PA0_CANTX0
  #define LCD_PINS_D5      54   // D54 PA16_SCK1
  #define LCD_PINS_D6      68   // D68 PA1_CANRX0
  #define LCD_PINS_D7      34   // D34 PC2_PWML0

  #define SD_DETECT_PIN     2   // D2  PB25_TIOA0

  #if ENABLED(NEWPANEL)
    // Buttons on AUX-2
    #define BTN_EN1        60   // D60 PA3_TIOB1
    #define BTN_EN2        13   // D13 PB27_TIOB0
    #define BTN_ENC        16   // D16 PA13_TXD1 // the click
  #endif // NEWPANEL
#endif // ULTRA_LCD || EXTENSIBLE_UI
