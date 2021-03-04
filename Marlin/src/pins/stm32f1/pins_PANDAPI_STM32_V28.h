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
 * PANDAPI STM32F1 pin assignments
 *  https://gitee.com/markyue/pandapi_wiki/raw/master/imges/doc/Schematic_PandaPiV2.8.pdf
 */

#if NOT_TARGET(__STM32F1__, STM32F1xx, STM32F0xx)
  #error "Oops! Select an STM32 board in your IDE."
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "Pandapi supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "PandaPi V28"
#endif

#define BOARD_WEBSITE_URL    "github.com/markniu/PandaPi"
#define DEFAULT_MACHINE_NAME "PANDAPI 3D"

// Release PB3/PB4 (TMC_SW Pins) from JTAG pins
#define DISABLE_JTAG

#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
// Enable EEPROM Emulation for this board as it doesn't have EEPROM


//#ifdef PANDAPI v2.8 
/* PANDAPI = 0 => U6 PANDAPI_STM32 */
/* PANDAPI = 1 => U8 PANDAPI */
/* PANDAPI = 2 => U8 KLIPPER */

//
// Servos
//
#define SERVO0_PIN                          PA11   // PWM0 SERVOS

//
// TMC StallGuard DIAG pins
//
#if PANDAPI < 0
  #define X_DIAG_PIN                          12
  #define Y_DIAG_PIN                          16
  #define Z_DIAG_PIN                          26
#else
  #define X_DIAG_PIN                          PA3   // "X-STOP"
  #define Y_DIAG_PIN                          PA4   // "Y-STOP"
  #define Z_DIAG_PIN                          PA1  // "Z-STOP"
  //#define E0_DIAG_PIN                       PC2  // E0DET
  //#define E1_DIAG_PIN                       P  // E1DET
#endif

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN                      X_DIAG_PIN  // X-MIN
  #else
    #define X_MAX_PIN                      X_DIAG_PIN  // E0DET
  #endif
#else
  #define X_STOP_PIN                       X_DIAG_PIN  // X-STOP
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN                      Y_DIAG_PIN  // Y-MIN
  #else
    #define Y_MAX_PIN                      Y_DIAG_PIN  // E0DET
  #endif
#else
  #define Y_STOP_PIN                       Y_DIAG_PIN  // Y-STOP
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN                      Z_DIAG_PIN  // Z-MIN
  #else
    #define Z_MAX_PIN                      Z_DIAG_PIN  // PWRDET
  #endif
#else
  #define Z_STOP_PIN                       Z_DIAG_PIN  // Y-STOP
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PIN                  Z_DIAG_PIN  // "PROBE"
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
    #define FIL_RUNOUT_PIN                    PA13   // SDA
    //#define FIL_RUNOUT_PIN                    PA14   // SCL
    //#define FIL_RUNOUT_PIN                    PC2   // "E0-STOP"
#endif

//
// Steppers
//
#if PANDAPI >0
  #define X_STEP_PIN         7 
  #define X_DIR_PIN          13 
  #define X_ENABLE_PIN       11  //0 

  #define Y_STEP_PIN         19 
  #define Y_DIR_PIN          18 
  #define Y_ENABLE_PIN       11  //0 

  #define Z_STEP_PIN         23 
  #define Z_DIR_PIN          21 
  #define Z_ENABLE_PIN       11  //0 

  #define E0_STEP_PIN        24 
  #define E0_DIR_PIN         22 
  #define E0_ENABLE_PIN      11  //0 

  #define E1_STEP_PIN        32
  #define E1_DIR_PIN         37
  #define E1_ENABLE_PIN      11  //0

  #define E2_STEP_PIN        28  //31
  #define E2_DIR_PIN         29
  #define E2_ENABLE_PIN      11  //0
#else
  #define X_STEP_PIN         PB8
  #define X_DIR_PIN          PB6
  #define X_ENABLE_PIN       PB9

  #define Y_STEP_PIN         PB5
  #define Y_DIR_PIN          PB4
  #define Y_ENABLE_PIN       PB9

  #define Z_STEP_PIN         PB3
  #define Z_DIR_PIN          PA15
  #define Z_ENABLE_PIN       PB9

  #define E0_STEP_PIN        PB15
  #define E0_DIR_PIN         PB14
  #define E0_ENABLE_PIN      PB9

  #define E1_STEP_PIN        PB2
  #define E1_DIR_PIN         PA7
  #define E1_ENABLE_PIN      PB9

  #define E2_STEP_PIN        PA6
  #define E2_DIR_PIN         PA5
  #define E2_ENABLE_PIN      PB9

#endif 

#if HAS_TRINAMIC
  #define TMC_BAUD_RATE                   19200
  #ifdef TMC_HARDWARE_SERIAL /*  TMC2209 */
    #define X_HARDWARE_SERIAL  MSerial1
  //  #define X2_HARDWARE_SERIAL MSerial1
    #define Y_HARDWARE_SERIAL  MSerial1
  //  #define Y2_HARDWARE_SERIAL MSerial1
    #define Z_HARDWARE_SERIAL  MSerial1
    // #define Z2_HARDWARE_SERIAL MSerial1
    #define E0_HARDWARE_SERIAL MSerial1
    #define E1_HARDWARE_SERIAL MSerial1
    #define E2_HARDWARE_SERIAL MSerial1
  #else
  //
  // Software serial
  //
    #define X_SERIAL_TX_PIN                  PB10
    #define X_SERIAL_RX_PIN                  PB11

    #define Y_SERIAL_TX_PIN                  PB10
    #define Y_SERIAL_RX_PIN                  PB11

    #define Z_SERIAL_TX_PIN                  PB10
    #define Z_SERIAL_RX_PIN                  PB11

    #define E0_SERIAL_TX_PIN                 PB10
    #define E0_SERIAL_RX_PIN                 PB11

    #define E1_SERIAL_TX_PIN                 PB10
    #define E1_SERIAL_RX_PIN                 PB11

    #define E2_SERIAL_TX_PIN                 PB10
    #define E2_SERIAL_RX_PIN                 PB11
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN     PB0  // Analog Input "TH0"
#define TEMP_1_PIN     PB1  //
#define TEMP_2_PIN     PA2  //255
#define DTH11           22

#define TEMP_BED_PIN     TEMP_1_PIN
#define TEMP_BOARD_PIN   TEMP_3_PIN  //255

#if HOTENDS == 1
  #if TEMP_SENSOR_PROBE
    #define TEMP_PROBE_PIN            TEMP_1_PIN
  #elif TEMP_SENSOR_CHAMBER
    #define TEMP_CHAMBER_PIN          TEMP_1_PIN
  #endif
#endif

//
// Heaters / Fans
//

#define HEATER_0_PIN     PB12 //255
#define HEATER_1_PIN     PA12 //25

#define HEATER_BED_PIN   PB13 //255

#define FAN_SOFT_PWM
#define FAN_PIN          PA8  //255
#define FAN1_PIN         PA9  //255
#define FAN2_PIN         PA10 //255
#define FAN3_PIN         PB7  //255

//#define FAN_PIN          FAN0_PIN  //255

//#ifndef MOSFET_D_PIN
//  #define MOSFET_D_PIN     -1
//#endif

// heat connector index
//#define HOTBED_CODE       0  
//#define HOTEND_0_CODE     1  
//#define HOTEND_1_CODE     2  

#ifdef MAX31856_PANDAPI
  #define HEATER_0_PIN     30
#else
/* U6 PI*/

#endif

//
// Misc. Functions
//
//
//#define BEEPER_PIN       PA0
#define CASE_LIGHT_PIN 255

#ifdef MAX31856_PANDAPI
  #define MAX31856_CLK_PIN  29
  #define MAX31856_MISO_PIN 24
  #define MAX31856_MOSI_PIN 28
  #define MAX31856_CS_PIN   27
#endif

//
// Power Supply Control
//

//
// LED / NEOPixel
//

//
// SD Card
//
//


//
// LCD / Controller
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                        PA0
    #define BTN_ENC                           PC11

    #if ENABLED(CR10_STOCKDISPLAY)
      #define LCD_PINS_RS                     PC15

      #define BTN_EN1                         PB6
      #define BTN_EN2                         PC13

      #define LCD_PINS_ENABLE                 PC14
      #define LCD_PINS_D4                     PB7

    #else

      #define LCD_PINS_RS                     PC12

      #define BTN_EN1                         PD2
      #define BTN_EN2                         PB8

      #define LCD_PINS_ENABLE                 PB6

      #if ENABLED(FYSETC_MINI_12864)

        #define LCD_BACKLIGHT_PIN             -1
        #define LCD_RESET_PIN                 PC13
        #define DOGLCD_A0                     PC12
        #define DOGLCD_CS                     PB6
        #define DOGLCD_SCK                    PB3
        #define DOGLCD_MOSI                   PB5

        #define FORCE_SOFT_SPI                      // SPI MODE3

        #define LED_PIN                       PB7   // red pwm
        //#define LED_PIN                     PC15  // green
        //#define LED_PIN                     PC14  // blue

        //#if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        //  #ifndef RGB_LED_R_PIN
        //    #define RGB_LED_R_PIN PB7
        //  #endif
        //  #ifndef RGB_LED_G_PIN
        //    #define RGB_LED_G_PIN PC15
        //  #endif
        //  #ifndef RGB_LED_B_PIN
        //    #define RGB_LED_B_PIN PC14
        //  #endif
        //#elif ENABLED(FYSETC_MINI_12864_2_1)
        //  #define NEOPIXEL_PIN    PB7
        //#endif

      #else                                         // !FYSETC_MINI_12864

        #define LCD_PINS_D4                   PC13
        #if IS_ULTIPANEL
          #define LCD_PINS_D5                 PB7
          #define LCD_PINS_D6                 PC15
          #define LCD_PINS_D7                 PC14

          #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
            #define BTN_ENC_EN         LCD_PINS_D7  // Detect the presence of the encoder
          #endif

        #endif

      #endif // !FYSETC_MINI_12864

      #if HAS_MARLINUI_U8GLIB
        #ifndef BOARD_ST7920_DELAY_1
          #define BOARD_ST7920_DELAY_1 DELAY_NS(125)
        #endif
        #ifndef BOARD_ST7920_DELAY_2
          #define BOARD_ST7920_DELAY_2 DELAY_NS(125)
        #endif
        #ifndef BOARD_ST7920_DELAY_3
          #define BOARD_ST7920_DELAY_3 DELAY_NS(125)
        #endif
      #endif

    #endif

#endif // HAS_WIRED_LCD
// Augmentation for auto-assigning RAMPS plugs
//
/**
#if DISABLED(IS_RAMPS_EEB) && DISABLED(IS_RAMPS_EEF) && DISABLED(IS_RAMPS_EFB) && DISABLED(IS_RAMPS_EFF) && DISABLED(IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  //#if HOTENDS > 1
  #if HAS_MULTI_HOTEND
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif
*/
