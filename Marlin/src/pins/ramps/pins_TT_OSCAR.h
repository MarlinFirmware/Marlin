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

// ATmega2560

#include "env_validate.h"

#if HOTENDS > 5 || E_STEPPERS > 5
  #error "TTOSCAR supports up to 5 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "TT OSCAR"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Servos
//
#define SERVO0_PIN                            PinB5
#define SERVO1_PIN                            PinB6
#define SERVO2_PIN                            PinE3
#define SERVO3_PIN                            PinG5

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinJ1
#define Y_MAX_PIN                             PinJ0
#define Z_MIN_PIN                             PinD3
#define Z_MAX_PIN                             PinD2

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN             SERVO3_PIN
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF0
#define X_DIR_PIN                             PinF1
#define X_ENABLE_PIN                          PinD7
#define X_CS_PIN                              PinF3

#define Y_STEP_PIN                            PinF6
#define Y_DIR_PIN                             PinF7
#define Y_ENABLE_PIN                          PinF2
#define Y_CS_PIN                              PinF4

#define Z_STEP_PIN                            PinL3
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinK0
#define Z_CS_PIN                              PinB0

#define E0_STEP_PIN                           PinA4
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinA2
#define E0_CS_PIN                             PinL0

#define E1_STEP_PIN                           PinC1
#define E1_DIR_PIN                            PinC3
#define E1_ENABLE_PIN                         PinC7
#define E1_CS_PIN                      E0_CS_PIN

#define E2_STEP_PIN                           PinK1
#define E2_DIR_PIN                            PinA0
#define E2_ENABLE_PIN                         PinF5
#define E2_CS_PIN                      E0_CS_PIN

#define E3_STEP_PIN                           PinC5
#define E3_DIR_PIN                            PinG1
#define E3_ENABLE_PIN                         PinG2
#define E3_CS_PIN                      E0_CS_PIN

#define E4_STEP_PIN                           PinL6
#define E4_DIR_PIN                            PinL7
#define E4_ENABLE_PIN                         PinL2
#define E4_CS_PIN                      E0_CS_PIN

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
  //#define E3_HARDWARE_SERIAL Serial1

  #define X_SERIAL_TX_PIN                     -1  // 59
  #define X_SERIAL_RX_PIN                     -1  // 63
  #define X2_SERIAL_TX_PIN                    -1
  #define X2_SERIAL_RX_PIN                    -1

  #define Y_SERIAL_TX_PIN                     -1  // 64
  #define Y_SERIAL_RX_PIN                     -1  // 40
  #define Y2_SERIAL_TX_PIN                    -1
  #define Y2_SERIAL_RX_PIN                    -1

  #define Z_SERIAL_TX_PIN                     -1  // 44
  #define Z_SERIAL_RX_PIN                     -1  // 42
  #define Z2_SERIAL_TX_PIN                    -1
  #define Z2_SERIAL_RX_PIN                    -1

  #define E0_SERIAL_TX_PIN                    -1  // 66
  #define E0_SERIAL_RX_PIN                    -1  // 65
  #define E1_SERIAL_TX_PIN                    -1
  #define E1_SERIAL_RX_PIN                    -1
  #define E2_SERIAL_TX_PIN                    -1
  #define E2_SERIAL_RX_PIN                    -1
  #define E3_SERIAL_TX_PIN                    -1
  #define E3_SERIAL_RX_PIN                    -1
  #define E4_SERIAL_TX_PIN                    -1
  #define E4_SERIAL_RX_PIN                    -1
  #define E5_SERIAL_RX_PIN                    -1
  #define E6_SERIAL_RX_PIN                    -1
  #define E7_SERIAL_RX_PIN                    -1
#endif

//
// Default pins for TMC SPI
//
//#ifndef TMC_SPI_MOSI
//  #define TMC_SPI_MOSI                      PinK4
//#endif
//#ifndef TMC_SPI_MISO
//  #define TMC_SPI_MISO                      PinL5
//#endif
//#ifndef TMC_SPI_SCK
//  #define TMC_SPI_SCK                       PinK2
//#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7
#define TEMP_1_PIN                            PinJ0
#define TEMP_2_PIN                            PinB4
#define TEMP_3_PIN                            PinB5
#define TEMP_BED_PIN                          PinJ1

#if TEMP_SENSOR_CHAMBER > 0
  #define TEMP_CHAMBER_PIN                    PinB6
#else
  #define TEMP_4_PIN                          PinB6
#endif

// SPI for MAX Thermocouple
//#if DISABLED(SDSUPPORT)
//  #define TEMP_0_CS_PIN                     PinK4   // Don't use 53 if using Display/SD card
//#else
//  #define TEMP_0_CS_PIN                     PinK4   // Don't use 49 (SD_DETECT_PIN)
//#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB4
#define HEATER_1_PIN                          PinH4
#define HEATER_2_PIN                          PinL5
#define HEATER_BED_PIN                        PinH5

#define FAN_PIN                               PinH6

#if EXTRUDERS >= 5
  #define HEATER_4_PIN                        PinH3
#else
  #define FAN1_PIN                            PinH3
#endif

#if EXTRUDERS >= 4
  #define HEATER_3_PIN                        PinL4
#else
  #define FAN2_PIN                            PinL4
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7

//#ifndef FILWIDTH_PIN
//  #define FILWIDTH_PIN                       5   // Analog Input
//#endif

// DIO 4 (Servos plug) for the runout sensor.
//#define FIL_RUNOUT_PIN              SERVO3_PIN

#ifndef PS_ON_PIN
  #define PS_ON_PIN                           PinB6
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if !NUM_SERVOS                                 // Prefer the servo connector
    #define SPINDLE_LASER_ENA_PIN             PinG5  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN             PinH3  // Hardware PWM
    #define SPINDLE_DIR_PIN                   PinE3
  #elif HAS_FREE_AUX2_PINS                        // Try to use AUX 2
    #define SPINDLE_LASER_ENA_PIN             PinG1  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN             PinL5  // Hardware PWM
    #define SPINDLE_DIR_PIN                   PinK3
  #endif
#endif

//
// Case Light
//
#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT) && !defined(SPINDLE_LASER_ENA_PIN)
  #if !NUM_SERVOS                                 // Prefer the servo connector
    #define CASE_LIGHT_PIN                    PinH3  // Hardware PWM
  #elif HAS_FREE_AUX2_PINS                        // Try to use AUX 2
    #define CASE_LIGHT_PIN                    PinL5  // Hardware PWM
  #endif
#endif

//
// Průša i3 MK2 Multiplexer Support
//
#if 0 && HAS_PRUSA_MMU1
  #ifndef E_MUX0_PIN
    #define E_MUX0_PIN                        PinF4  // Y_CS_PIN
  #endif
  #ifndef E_MUX1_PIN
    #define E_MUX1_PIN                        PinB0  // Z_CS_PIN
  #endif
  #ifndef E_MUX2_PIN
    #define E_MUX2_PIN                        PinL0  // En_CS_PIN
  #endif
#endif

//////////////////////////
// LCDs and Controllers //
//////////////////////////

#if HAS_WIRED_LCD

  //
  // LCD Display output pins
  //
  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

    #define LCD_PINS_RS                       PinL0  // CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE                   PinB2  // SID (MOSI)
    #define LCD_PINS_D4                       PinB1  // SCK (CLK) clock

  #elif BOTH(IS_NEWPANEL, PANEL_ONE)

    #define LCD_PINS_RS                       PinG1
    #define LCD_PINS_ENABLE                   PinL7
    #define LCD_PINS_D4                       PinK3
    #define LCD_PINS_D5                       PinK4
    #define LCD_PINS_D6                       PinL5
    #define LCD_PINS_D7                       PinK2

  #elif ENABLED(ZONESTAR_LCD)

    #define LCD_PINS_RS                       PinK2
    #define LCD_PINS_ENABLE                   PinL5
    #define LCD_PINS_D4                       PinK1
    #define LCD_PINS_D5                       PinG1
    #define LCD_PINS_D6                       PinL7
    #define LCD_PINS_D7                       PinK3
    #define ADC_KEYPAD_PIN                    PinB6

  #else

    #if ENABLED(CR10_STOCKDISPLAY)

      #define LCD_PINS_RS                     PinA5
      #define LCD_PINS_ENABLE                 PinA7
      #define LCD_PINS_D4                     PinA3

      #if !IS_NEWPANEL
        #define BEEPER_PIN                    PinC0
      #endif

    #else

      #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
        #define LCD_PINS_DC                   PinA3  // Set as output on init
        #define LCD_PINS_RS                   PinA5  // Pull low for 1s to init
        // DOGM SPI LCD Support
        #define DOGLCD_CS                     PinH1
        #define DOGLCD_MOSI                   PinH0
        #define DOGLCD_SCK                    PinA1
        #define DOGLCD_A0            LCD_PINS_DC
      #else
        #define LCD_PINS_RS                   PinH1
        #define LCD_PINS_ENABLE               PinH0
        #define LCD_PINS_D4                   PinA1
        #define LCD_PINS_D5                   PinA3
        #define LCD_PINS_D6                   PinA5
      #endif

      #define LCD_PINS_D7                     PinA7

      #if !IS_NEWPANEL
        #define BEEPER_PIN                    PinC4
      #endif

    #endif

    #if !IS_NEWPANEL
      // Buttons attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK_PIN                 PinD7
      //#define SHIFT_LD_PIN                  PinL7
      //#define SHIFT_OUT_PIN                 PinG1
      //#define SHIFT_EN_PIN                  PinH0
    #endif

  #endif

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define BTN_ENC_EN               LCD_PINS_D7  // Detect the presence of the encoder
  #endif

  //
  // LCD Display input pins
  //
  #if IS_NEWPANEL

    #if IS_RRD_SC

      #define BEEPER_PIN                      PinC0

      #if ENABLED(CR10_STOCKDISPLAY)
        #define BTN_EN1                       PinH0
        #define BTN_EN2                       PinA1
      #else
        #define BTN_EN1                       PinC6
        #define BTN_EN2                       PinC4
      #endif

      #define BTN_ENC                         PinC2
      #define SD_DETECT_PIN                   PinL0
      //#define KILL_PIN                      PinG0

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_BACKLIGHT_PIN             PinG2
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      #define BTN_EN1                         PinK2
      #define BTN_EN2                         PinF5
      #define BTN_ENC                         PinK1
      #define SD_DETECT_PIN                   PinL7

    #elif ENABLED(LCD_I2C_PANELOLU2)

      #define BTN_EN1                         PinL2
      #define BTN_EN2                         PinL6
      #define BTN_ENC                         PinC5
      #define LCD_SDSS                        PinB0
      //#define KILL_PIN                      PinG0

    #elif ENABLED(LCD_I2C_VIKI)

      #define BTN_EN1                         PinA0  // https://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      #define BTN_EN2                         PinH4  // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.
      #define BTN_ENC                         -1

      #define LCD_SDSS                        PinB0
      #define SD_DETECT_PIN                   PinL0

    #elif EITHER(VIKI2, miniVIKI)

      #define DOGLCD_CS                       PinL4
      #define DOGLCD_A0                       PinL5

      #define BEEPER_PIN                      PinC4
      #define STAT_LED_RED_PIN                PinC5
      #define STAT_LED_BLUE_PIN               PinC2

      #define BTN_EN1                         PinA0
      #define BTN_EN2                         PinH4
      #define BTN_ENC                         PinG2

      #define SDSS                            PinB0
      #define SD_DETECT_PIN                   -1  // Pin 49 for display SD interface, 72 for easy adapter board
      //#define KILL_PIN                      PinC6

      #define LCD_SCREEN_ROTATE              180  // 0, 90, 180, 270

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

      #define DOGLCD_CS                       PinA7
      #define DOGLCD_A0                       PinA5

      #define BEEPER_PIN                      PinA1
      #define LCD_BACKLIGHT_PIN               PinC4

      #define BTN_EN1                         PinC2
      #define BTN_EN2                         PinC0
      #define BTN_ENC                         PinC6

      #define LCD_SDSS                        PinB0
      #define SD_DETECT_PIN                   PinL0
      //#define KILL_PIN                      PinG0

    #elif ENABLED(MKS_MINI_12864)

      #define DOGLCD_A0                       PinA5
      #define DOGLCD_CS                       PinA3

      #define BEEPER_PIN                      PinC0

      #define LCD_BACKLIGHT_PIN               PinK3  // backlight LED on A11/D65

      #define BTN_EN1                         PinC6
      #define BTN_EN2                         PinC4
      #define BTN_ENC                         PinC2
      //#define SDSS                          PinB0
      #define SD_DETECT_PIN                   PinL0
      //#define KILL_PIN                      PinK2

      //#define LCD_CONTRAST_INIT            190
      //#define LCD_SCREEN_ROTATE            180  // 0, 90, 180, 270

    #elif ENABLED(MINIPANEL)

      #define BEEPER_PIN                      PinL7
      // not connected to a pin
      #define LCD_BACKLIGHT_PIN               PinK3  // backlight LED on A11/D65

      #define DOGLCD_A0                       PinL5
      #define DOGLCD_CS                       PinK4

      #define BTN_EN1                         PinG1
      #define BTN_EN2                         PinK1
      #define BTN_ENC                         PinF5

      #define SDSS                            PinB0
      #define SD_DETECT_PIN                   PinL0
      //#define KILL_PIN                      PinK2

      //#define LCD_CONTRAST_INIT            190
      //#define LCD_SCREEN_ROTATE            180  // 0, 90, 180, 270

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN                      PinC4

      // Buttons are directly attached to AUX-2
      #if IS_RRW_KEYPAD
        #define SHIFT_OUT_PIN                 PinG1
        #define SHIFT_CLK_PIN                 PinL5
        #define SHIFT_LD_PIN                  PinL7
        #define BTN_EN1                       PinK2
        #define BTN_EN2                       PinF5
        #define BTN_ENC                       PinK1
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1                       PinF5  // AUX2 PIN 3
        #define BTN_EN2                       PinK1  // AUX2 PIN 4
        #define BTN_ENC                       PinL0  // AUX3 PIN 7
      #else
        #define BTN_EN1                       PinC0
        #define BTN_EN2                       PinC2
        #define BTN_ENC                       PinC6
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN                 PinL0
        //#define KILL_PIN                    PinG0
      #endif

    #endif

  #endif // IS_NEWPANEL

#endif
