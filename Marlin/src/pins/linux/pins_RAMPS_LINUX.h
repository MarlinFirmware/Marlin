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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Arduino Mega with RAMPS v1.4 (or v1.3) pin assignments
 *
 * Applies to the following boards:
 *
 *  RAMPS_14_EFB (Hotend, Fan, Bed)
 *  RAMPS_14_EEB (Hotend0, Hotend1, Bed)
 *  RAMPS_14_EFF (Hotend, Fan0, Fan1)
 *  RAMPS_14_EEF (Hotend0, Hotend1, Fan)
 *  RAMPS_14_SF  (Spindle, Controller Fan)
 *
 *  RAMPS_13_EFB (Hotend, Fan, Bed)
 *  RAMPS_13_EEB (Hotend0, Hotend1, Bed)
 *  RAMPS_13_EFF (Hotend, Fan0, Fan1)
 *  RAMPS_13_EEF (Hotend0, Hotend1, Fan)
 *  RAMPS_13_SF  (Spindle, Controller Fan)
 *
 *  Other pins_MYBOARD.h files may override these defaults
 *
 *  Differences between
 *  RAMPS_13 | RAMPS_14
 *         7 | 11
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "RAMPS 1.4"
#endif

#define E2END 0xFFF                               // 4KB

#define IS_RAMPS_EFB

//
// Servos
//
#ifdef IS_RAMPS_13
  #define SERVO0_PIN                           7  // RAMPS_13 // Will conflict with BTN_EN2 on LCD_I2C_VIKI
#else
  #define SERVO0_PIN                          11
#endif
#define SERVO1_PIN                             6
#define SERVO2_PIN                             5
#ifndef SERVO3_PIN
  #define SERVO3_PIN                           4
#endif

//
// Limit Switches
//
#define X_MIN_PIN                              3
#ifndef X_MAX_PIN
  #define X_MAX_PIN                            2
#endif
#define Y_MIN_PIN                             14
#define Y_MAX_PIN                             15
#define Z_MIN_PIN                             18
#define Z_MAX_PIN                             19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     32
#endif

//
// Steppers
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38
#ifndef X_CS_PIN
  #define X_CS_PIN                            53
#endif

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            49
#endif

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          62
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            40
#endif

#define E0_STEP_PIN                           26
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         24
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           42
#endif

#define E1_STEP_PIN                           36
#define E1_DIR_PIN                            34
#define E1_ENABLE_PIN                         30
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           44
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                             0  // Analog Input
#define TEMP_1_PIN                             1  // Analog Input
#define TEMP_BED_PIN                           2  // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS_PIN                      66  // Don't use 53 if using Display/SD card
#else
  #define MAX6675_SS_PIN                      66  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// Augmentation for auto-assigning RAMPS plugs
//
#if NONE(IS_RAMPS_EEB, IS_RAMPS_EEF, IS_RAMPS_EFB, IS_RAMPS_EFF, IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
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

//
// Heaters / Fans
//
#ifndef MOSFET_D_PIN
  #define MOSFET_D_PIN                        -1
#endif
#ifndef RAMPS_D8_PIN
  #define RAMPS_D8_PIN                         8
#endif
#ifndef RAMPS_D9_PIN
  #define RAMPS_D9_PIN                         9
#endif
#ifndef RAMPS_D10_PIN
  #define RAMPS_D10_PIN                       10
#endif

#define HEATER_0_PIN               RAMPS_D10_PIN

#if ENABLED(IS_RAMPS_EFB)                         // Hotend, Fan, Bed
  #define FAN_PIN                   RAMPS_D9_PIN
  #define HEATER_BED_PIN            RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EEF)                       // Hotend, Hotend, Fan
  #define HEATER_1_PIN              RAMPS_D9_PIN
  #define FAN_PIN                   RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EEB)                       // Hotend, Hotend, Bed
  #define HEATER_1_PIN              RAMPS_D9_PIN
  #define HEATER_BED_PIN            RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EFF)                       // Hotend, Fan, Fan
  #define FAN_PIN                   RAMPS_D9_PIN
  #define FAN1_PIN                  RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_SF)                        // Spindle, Fan
  #define FAN_PIN                   RAMPS_D8_PIN
#else                                             // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")
  #define FAN_PIN                   RAMPS_D9_PIN
  #define HEATER_BED_PIN            RAMPS_D8_PIN
  #if HOTENDS == 1
    #define FAN1_PIN                MOSFET_D_PIN
  #else
    #define HEATER_1_PIN            MOSFET_D_PIN
  #endif
#endif

#ifndef FAN_PIN
  #define FAN_PIN                              4  // IO pin. Buffer needed
#endif

//
// Misc. Functions
//
#define SDSS                                  53
#define LED_PIN                               13

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                         5  // Analog Input on AUX2
#endif

// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                       4
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                           12
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !defined(CASE_LIGHT_PIN) && !defined(SPINDLE_LASER_ENA_PIN)
  #if NUM_SERVOS <= 1                             // Prefer the servo connector
    #define CASE_LIGHT_PIN                     6  // Hardware PWM
  #elif HAS_FREE_AUX2_PINS                        // try to use AUX 2
    #define CASE_LIGHT_PIN                    44  // Hardware PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if !defined(NUM_SERVOS) || NUM_SERVOS == 0     // Prefer the servo connector
    #define SPINDLE_LASER_ENA_PIN              4  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN              6  // Hardware PWM
    #define SPINDLE_DIR_PIN                    5
  #elif HAS_FREE_AUX2_PINS                        // try to use AUX 2
    #define SPINDLE_LASER_ENA_PIN             40  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN             44  // Hardware PWM
    #define SPINDLE_DIR_PIN                   65
  #endif
#endif

//
// Průša i3 MK2 Multiplexer Support
//
#ifndef E_MUX0_PIN
  #define E_MUX0_PIN                          40  // Z_CS_PIN
#endif
#ifndef E_MUX1_PIN
  #define E_MUX1_PIN                          42  // E0_CS_PIN
#endif
#ifndef E_MUX2_PIN
  #define E_MUX2_PIN                          44  // E1_CS_PIN
#endif

/**
 * Default pins for TMC software SPI
 */
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                       66
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                       44
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                        64
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  /**
   * Software serial
   */

  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                   40
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN                   63
  #endif
  #ifndef X2_SERIAL_TX_PIN
    #define X2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef X2_SERIAL_RX_PIN
    #define X2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                   59
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN                   64
  #endif
  #ifndef Y2_SERIAL_TX_PIN
    #define Y2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef Y2_SERIAL_RX_PIN
    #define Y2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                   42
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN                   65
  #endif
  #ifndef Z2_SERIAL_TX_PIN
    #define Z2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef Z2_SERIAL_RX_PIN
    #define Z2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN                  44
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN                  66
  #endif
  #ifndef E1_SERIAL_TX_PIN
    #define E1_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E1_SERIAL_RX_PIN
    #define E1_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E2_SERIAL_TX_PIN
    #define E2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E2_SERIAL_RX_PIN
    #define E2_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E3_SERIAL_TX_PIN
    #define E3_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E3_SERIAL_RX_PIN
    #define E3_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E4_SERIAL_TX_PIN
    #define E4_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E4_SERIAL_RX_PIN
    #define E4_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E5_SERIAL_TX_PIN
    #define E5_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E5_SERIAL_RX_PIN
    #define E5_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E6_SERIAL_TX_PIN
    #define E6_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E6_SERIAL_RX_PIN
    #define E6_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E7_SERIAL_TX_PIN
    #define E7_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E7_SERIAL_RX_PIN
    #define E7_SERIAL_RX_PIN                  -1
  #endif
#endif

//////////////////////////
// LCDs and Controllers //
//////////////////////////

#if HAS_SPI_LCD

  //
  // LCD Display output pins
  //
  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

    #define LCD_PINS_RS                       49  // CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE                   51  // SID (MOSI)
    #define LCD_PINS_D4                       52  // SCK (CLK) clock

  #elif BOTH(NEWPANEL, PANEL_ONE)

    #define LCD_PINS_RS                       40
    #define LCD_PINS_ENABLE                   42
    #define LCD_PINS_D4                       65
    #define LCD_PINS_D5                       66
    #define LCD_PINS_D6                       44
    #define LCD_PINS_D7                       64

  #else

    #if ENABLED(CR10_STOCKDISPLAY)

      #define LCD_PINS_RS                     27
      #define LCD_PINS_ENABLE                 29
      #define LCD_PINS_D4                     25

      #if DISABLED(NEWPANEL)
        #define BEEPER_PIN                    37
      #endif

    #elif ENABLED(ZONESTAR_LCD)

      #define LCD_PINS_RS                     64
      #define LCD_PINS_ENABLE                 44
      #define LCD_PINS_D4                     63
      #define LCD_PINS_D5                     40
      #define LCD_PINS_D6                     42
      #define LCD_PINS_D7                     65

    #else

      #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
        #define LCD_PINS_DC                   25  // Set as output on init
        #define LCD_PINS_RS                   27  // Pull low for 1s to init
        // DOGM SPI LCD Support
        #define DOGLCD_CS                     16
        #define DOGLCD_MOSI                   17
        #define DOGLCD_SCK                    23
        #define DOGLCD_A0            LCD_PINS_DC
      #else
        #define LCD_PINS_RS                   16
        #define LCD_PINS_ENABLE               17
        #define LCD_PINS_D4                   23
        #define LCD_PINS_D5                   25
        #define LCD_PINS_D6                   27
      #endif

      #define LCD_PINS_D7                     29

      #if DISABLED(NEWPANEL)
        #define BEEPER_PIN                    33
      #endif

    #endif

    #if DISABLED(NEWPANEL)
      // Buttons attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK                     38
      //#define SHIFT_LD                      42
      //#define SHIFT_OUT                     40
      //#define SHIFT_EN                      17
    #endif

  #endif

  //
  // LCD Display input pins
  //
  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

      #define BEEPER_PIN                      37

      #if ENABLED(CR10_STOCKDISPLAY)
        #define BTN_EN1                       17
        #define BTN_EN2                       23
      #else
        #define BTN_EN1                       31
        #define BTN_EN2                       33
      #endif

      #define BTN_ENC                         35
      #define SD_DETECT_PIN                   49
      #define KILL_PIN                        41

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_BACKLIGHT_PIN             39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      #define BTN_EN1                         64
      #define BTN_EN2                         59
      #define BTN_ENC                         63
      #define SD_DETECT_PIN                   42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      #define BTN_EN1                         47
      #define BTN_EN2                         43
      #define BTN_ENC                         32
      #define LCD_SDSS                      SDSS
      #define KILL_PIN                        41

    #elif ENABLED(LCD_I2C_VIKI)

      #define BTN_EN1                         22  // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      #define BTN_EN2                          7  // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.
      #define BTN_ENC                         -1

      #define LCD_SDSS                      SDSS
      #define SD_DETECT_PIN                   49

    #elif ANY(VIKI2, miniVIKI)

      #define DOGLCD_CS                       45
      #define DOGLCD_A0                       44
      #define LCD_SCREEN_ROT_180

      #define BEEPER_PIN                      33
      #define STAT_LED_RED_PIN                32
      #define STAT_LED_BLUE_PIN               35

      #define BTN_EN1                         22
      #define BTN_EN2                          7
      #define BTN_ENC                         39

      #define SD_DETECT_PIN                   -1  // Pin 49 for display sd interface, 72 for easy adapter board
      #define KILL_PIN                        31

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

      #define DOGLCD_CS                       29
      #define DOGLCD_A0                       27

      #define BEEPER_PIN                      23
      #define LCD_BACKLIGHT_PIN               33

      #define BTN_EN1                         35
      #define BTN_EN2                         37
      #define BTN_ENC                         31

      #define LCD_SDSS                      SDSS
      #define SD_DETECT_PIN                   49
      #define KILL_PIN                        41

    #elif ENABLED(MKS_MINI_12864)

      #define DOGLCD_A0                       27
      #define DOGLCD_CS                       25

      // GLCD features
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270

      #define BEEPER_PIN                      37
      // not connected to a pin
      #define LCD_BACKLIGHT_PIN               65  // backlight LED on A11/D65

      #define BTN_EN1                         31
      #define BTN_EN2                         33
      #define BTN_ENC                         35

      #define SD_DETECT_PIN                   49
      #define KILL_PIN                        64

    #elif ENABLED(MINIPANEL)

      #define BEEPER_PIN                      42
      // not connected to a pin
      #define LCD_BACKLIGHT_PIN               65  // backlight LED on A11/D65

      #define DOGLCD_A0                       44
      #define DOGLCD_CS                       66

      // GLCD features
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270

      #define BTN_EN1                         40
      #define BTN_EN2                         63
      #define BTN_ENC                         59

      #define SD_DETECT_PIN                   49
      #define KILL_PIN                        64

    #elif ENABLED(ZONESTAR_LCD)

      #define ADC_KEYPAD_PIN                  12

    #elif ENABLED(AZSMZ_12864)

      // Pins only defined for RAMPS_SMART currently

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN                      33

      // Buttons are directly attached to AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define SHIFT_OUT                     40
        #define SHIFT_CLK                     44
        #define SHIFT_LD                      42
        #define BTN_EN1                       64
        #define BTN_EN2                       59
        #define BTN_ENC                       63
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1                       59  // AUX2 PIN 3
        #define BTN_EN2                       63  // AUX2 PIN 4
        #define BTN_ENC                       49  // AUX3 PIN 7
      #else
        #define BTN_EN1                       37
        #define BTN_EN2                       35
        #define BTN_ENC                       31
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN                 49
        #define KILL_PIN                      41
      #endif

    #endif
  #endif // NEWPANEL

#endif // HAS_SPI_LCD
