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

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT SKR E3 Turbo"
#endif

// Onboard I2C EEPROM
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB (AT24C32)

//
// Servos
//
#define SERVO0_PIN                         P1_23

//
// TMC StallGuard DIAG pins
//
#define X_DIAG_PIN                         P1_29  // X-STOP
#define Y_DIAG_PIN                         P1_28  // Y-STOP
#define Z_DIAG_PIN                         P1_27  // Z-STOP
#define E0_DIAG_PIN                        P1_26  // E0DET
#define E1_DIAG_PIN                        P1_25  // E1DET

//
// Limit Switches
#define X_STOP_PIN                    X_DIAG_PIN
#define Y_STOP_PIN                    Y_DIAG_PIN
#define Z_STOP_PIN                    Z_DIAG_PIN

//
// Z Probe
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  P1_22
#endif

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                     P1_26  // E0DET
#define FIL_RUNOUT2_PIN                    P1_25  // E1DET

//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN                        P1_21
#endif

// LED driving pin
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                     P1_24
#endif

//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                   P1_20  // PWRDET
#endif

//
// Steppers
//
#define X_STEP_PIN                         P1_04
#define X_DIR_PIN                          P1_08
#define X_ENABLE_PIN                       P1_00
#ifndef X_CS_PIN
  #define X_CS_PIN                         P1_01
#endif

#define Y_STEP_PIN                         P1_14
#define Y_DIR_PIN                          P1_15
#define Y_ENABLE_PIN                       P1_09
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         P1_10
#endif

#define Z_STEP_PIN                         P4_29
#define Z_DIR_PIN                          P4_28
#define Z_ENABLE_PIN                       P1_16
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         P1_17
#endif

#define E0_STEP_PIN                        P2_06
#define E0_DIR_PIN                         P2_07
#define E0_ENABLE_PIN                      P0_04
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        P0_05
#endif

#define E1_STEP_PIN                        P2_11
#define E1_DIR_PIN                         P2_12
#define E1_ENABLE_PIN                      P0_21
#ifndef E1_CS_PIN
  #define E1_CS_PIN                        P0_22
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                  P1_01
  #define X_SERIAL_RX_PIN                  P1_01

  #define Y_SERIAL_TX_PIN                  P1_10
  #define Y_SERIAL_RX_PIN                  P1_10

  #define Z_SERIAL_TX_PIN                  P1_17
  #define Z_SERIAL_RX_PIN                  P1_17

  #define E0_SERIAL_TX_PIN                 P0_05
  #define E0_SERIAL_RX_PIN                 P0_05

  #define E1_SERIAL_TX_PIN                 P0_22
  #define E1_SERIAL_RX_PIN                 P0_22

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// TMC Low Power Standby pins
//
#define X_STDBY_PIN                        P3_26
#define Y_STDBY_PIN                        P3_25
#define Z_STDBY_PIN                        P1_18
#define E0_STDBY_PIN                       P1_19
#define E1_STDBY_PIN                       P2_13

//
// Temperature Sensors
//
#define TEMP_0_PIN                         P0_24
#define TEMP_1_PIN                         P0_23
//#define TEMP_2_PIN                       P1_30  // Onboard thermistor
#define TEMP_BED_PIN                       P0_25

//
// Heaters / Fans
//
#define HEATER_0_PIN                       P2_03  // EXTRUDER 0
#define HEATER_1_PIN                       P2_04  // EXTRUDER 1
#define HEATER_BED_PIN                     P2_05  // BED
#define FAN_PIN                            P2_01
#define FAN1_PIN                           P2_02

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN            FAN1_PIN
#endif

/**
 *                  _____
 *              5V | 1 2 | GND
 *  (LCD_EN) P0_18 | 3 4 | P0_17 (LCD_RS)
 *  (LCD_D4) P0_15 | 5 6   P0_20 (BTN_EN2)
 *           RESET | 7 8 | P0_19 (BTN_EN1)
 * (BTN_ENC) P0_16 | 9 10| P2_08 (BEEPER)
 *                  -----
 *                   EXP
 */

#define EXP1_03_PIN                        P0_18
#define EXP1_04_PIN                        P0_17
#define EXP1_05_PIN                        P0_15
#define EXP1_06_PIN                        P0_20
#define EXP1_07_PIN                        -1
#define EXP1_08_PIN                        P0_19
#define EXP1_09_PIN                        P0_16
#define EXP1_10_PIN                        P2_08

#if ENABLED(DWIN_CREALITY_LCD)
  #error "DWIN_CREALITY_LCD requires a custom cable with TX = P0_15, RX = P0_16, and LCD_SERIAL_PORT 1. Comment out this line to continue."

  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_EN1                    EXP1_03_PIN
  #define BTN_EN2                    EXP1_04_PIN
  #define BTN_ENC                    EXP1_06_PIN

#elif HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN               EXP1_10_PIN

    #define BTN_EN1                  EXP1_08_PIN
    #define BTN_EN2                  EXP1_06_PIN
    #define BTN_ENC                  EXP1_09_PIN

    #define LCD_PINS_RS              EXP1_04_PIN
    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

  #elif ENABLED(ZONESTAR_LCD)                     // ANET A8 LCD Controller - Must convert to 3.3V - CONNECTING TO 5V WILL DAMAGE THE BOARD!

    #error "CAUTION! ZONESTAR_LCD requires wiring modifications. See 'pins_BTT_SKR_E3_TURBO.h' for details. Comment out this line to continue."

    #define LCD_PINS_RS              EXP1_05_PIN
    #define LCD_PINS_ENABLE          EXP1_09_PIN
    #define LCD_PINS_D4              EXP1_04_PIN
    #define LCD_PINS_D5              EXP1_06_PIN
    #define LCD_PINS_D6              EXP1_08_PIN
    #define LCD_PINS_D7              EXP1_10_PIN
    #define ADC_KEYPAD_PIN                 P1_23  // Repurpose servo pin for ADC - CONNECTING TO 5V WILL DAMAGE THE BOARD!

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    #define BTN_EN1                  EXP1_08_PIN
    #define BTN_EN2                  EXP1_06_PIN
    #define BTN_ENC                  EXP1_09_PIN

    #define DOGLCD_CS                EXP1_04_PIN
    #define DOGLCD_A0                EXP1_05_PIN
    #define DOGLCD_SCK               EXP1_10_PIN
    #define DOGLCD_MOSI              EXP1_03_PIN
    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN              -1

  #else

    #error "Only ZONESTAR_LCD, MKS_MINI_12864, ENDER2_STOCKDISPLAY, and CR10_STOCKDISPLAY are currently supported on the BTT_SKR_E3_TURBO."

  #endif

#endif // HAS_WIRED_LCD

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                    P2_00
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #define SD_SS_PIN                        P0_06
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "SD CUSTOM_CABLE is not compatible with SKR E3 Turbo."
#endif
