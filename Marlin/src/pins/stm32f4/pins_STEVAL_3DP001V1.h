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

// Source: https://github.com/stm32duino/Arduino_Core_STM32/blob/master/variants/ST3DP001_EVAL/variant.cpp

/**
 * HOW TO COMPILE
 *
 * PlatformIO - Use the STM32F401VE_STEVAL environment (or the "Auto Build Marlin" extension).
 *
 * Arduino - Tested with 1.8.10
 *      Install library per https://github.com/stm32duino/Arduino_Core_STM32
 *      Make the following selections under the TOOL menu in the Arduino IDE
 *          Board: "3D printer boards"
 *          Board part number: "STEVAL-3DP001V1"
 *          U(S)ART support: "Enabled (generic "Serial")"
 *          USB support (if available): "CDC (no generic "Serial")"
 *          Optimize: "Smallest (-Os default)"
 *          C Runtime Library: "newlib Nano (default)"
 */

#include "env_validate.h"

#ifndef MACHINE_NAME
  #define MACHINE_NAME "STEVAL-3DP001V1"
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PD8   // X_STOP
#define Y_MIN_PIN                           PD9   // Y_STOP
#define Z_MIN_PIN                           PD10  // Z_STOP

#define X_MAX_PIN                           PD0   // W_STOP
#define Y_MAX_PIN                           PA8   // V_STOP
#define Z_MAX_PIN                           PD11  // U_STOP

//
// Z Probe (when not Z_MIN_PIN)
//
//#ifndef Z_MIN_PROBE_PIN
//  #define Z_MIN_PROBE_PIN                 PA4   // SPI1_CS
//#endif

//
// Filament runout
//
//#define FIL_RUNOUT_PIN                    PA3   // BED_THERMISTOR_3

//
// Steppers
//

#define X_STEP_PIN                          PE14  // X_PWM
#define X_DIR_PIN                           PE15  // X_DIR
#define X_ENABLE_PIN                        PE13  // X_RESET
#define X_CS_PIN                            PA4   // SPI1_CS

#define Y_STEP_PIN                          PB10  // Y_PWM
#define Y_DIR_PIN                           PE9   // Y_DIR
#define Y_ENABLE_PIN                        PE10  // Y_RESET
#define Y_CS_PIN                            PA4   // SPI1_CS

#define Z_STEP_PIN                          PC6   // Z_PWM
#define Z_DIR_PIN                           PC0   // Z_DIR
#define Z_ENABLE_PIN                        PC15  // Z_RESET
#define Z_CS_PIN                            PA4   // SPI1_CS

#define E0_STEP_PIN                         PD12  // E1_PW
#define E0_DIR_PIN                          PC13  // E1_DIR
#define E0_ENABLE_PIN                       PC14  // E1_RESET
#define E0_CS_PIN                           PA4   // SPI1_CS

#define E1_STEP_PIN                         PE5   // E2_PWM
#define E1_DIR_PIN                          PE6   // E2_DIR
#define E1_ENABLE_PIN                       PE4   // E2_RESET
#define E1_CS_PIN                           PA4   // SPI1_CS

#define E2_STEP_PIN                         PB8   // E3_PWM
#define E2_DIR_PIN                          PE2   // E3_DIR
#define E2_ENABLE_PIN                       PE3   // E3_RESET
#define E2_CS_PIN                           PA4   // SPI1_CS

// needed to pass a sanity check
#define X2_CS_PIN                           PA4   // SPI1_CS
#define Y2_CS_PIN                           PA4   // SPI1_CS
#define Z2_CS_PIN                           PA4   // SPI1_CS
#define Z3_CS_PIN                           PA4   // SPI1_CS
#define E3_CS_PIN                           PA4   // SPI1_CS
#define E4_CS_PIN                           PA4   // SPI1_CS
#define E5_CS_PIN                           PA4   // SPI1_CS

#if HAS_L64XX
  #define L6470_CHAIN_SCK_PIN               PA5   // SPI1_SCK
  #define L6470_CHAIN_MISO_PIN              PA6   // SPI1_MISO
  #define L6470_CHAIN_MOSI_PIN              PA7   // SPI1_MOSI
  #define L6470_CHAIN_SS_PIN                PA4   // SPI1_CS

  //#define SD_SCK_PIN       L6470_CHAIN_SCK_PIN
  //#define SD_MISO_PIN     L6470_CHAIN_MISO_PIN
  //#define SD_MOSI_PIN     L6470_CHAIN_MOSI_PIN
#else
  //#define SD_SCK_PIN                      PB13  // SPI2_SCK
  //#define SD_MISO_PIN                     PB14  // SPI2_MISO
  //#define SD_MOSI_PIN                     PB15  // SPI2_MOSI
#endif

/**
 * Macro to reset/enable L6474 stepper drivers
 *
 * IMPORTANT - To disable (bypass) L6474s, install the corresponding
 *             resistors (R11 - R17) and change the "V" to "0" for the
 *             corresponding pins here:
 */
#define ENABLE_RESET_L64XX_CHIPS(V)   do{ OUT_WRITE(X_ENABLE_PIN, V); \
                                          OUT_WRITE(Y_ENABLE_PIN, V); \
                                          OUT_WRITE(Z_ENABLE_PIN, V); \
                                          OUT_WRITE(E0_ENABLE_PIN,V); \
                                          OUT_WRITE(E1_ENABLE_PIN,V); \
                                          OUT_WRITE(E2_ENABLE_PIN,V); \
                                        }while(0)

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // Analog Input 3
#define TEMP_1_PIN                          PA1   // Analog Input 4
#define TEMP_2_PIN                          PA2   // Analog Input 5
#define TEMP_BED_PIN                        PC2   // Analog Input 0
#define TEMP_BED_1_PIN                      PC3   // Analog Input 1
#define TEMP_BED_2_PIN                      PA3   // Analog Input 2

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC7   // E1_HEAT_PWM
#define HEATER_1_PIN                        PB0   // E2_HEAT_PWM
#define HEATER_2_PIN                        PB1   // E3_HEAT_PWM
#define HEATER_BED_PIN                      PD14  // BED_HEAT_1 FET
#define HEATER_BED_1_PIN                    PD13  // BED_HEAT_2 FET
#define HEATER_BED_2_PIN                    PD15  // BED_HEAT_3 FET

#define FAN_PIN                             PC4   // E1_FAN   PWM pin, Part cooling fan FET
#define FAN1_PIN                            PC5   // E2_FAN   PWM pin, Extruder fan FET
#define FAN2_PIN                            PE8   // E3_FAN   PWM pin, Controller fan FET

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PC5   // FAN1_PIN
#endif

//
// Misc functions
//
#define LED_PIN                             -1    // PE1  Green LED Heartbeat
#define PS_ON_PIN                           -1
#define KILL_PIN                            -1
#define POWER_LOSS_PIN                      -1    // PWR_LOSS / nAC_FAULT

//
// LCD / Controller
//
//#define SD_DETECT_PIN                     PA15  // SD_CARD_DETECT
//#define BEEPER_PIN                        PC9   // SDIO_D1
//#define LCD_PINS_RS                       PE9   // Y_DIR
//#define LCD_PINS_ENABLE                   PE8   // E3_FAN
//#define LCD_PINS_D4                       PB12  // SPI2_CS
//#define LCD_PINS_D5                       PB13  // SPI2_SCK
//#define LCD_PINS_D6                       PB14  // SPI2_MISO
//#define LCD_PINS_D7                       PB15  // SPI2_MOSI
//#define BTN_EN1                           PC4   // E1_FAN
//#define BTN_EN2                           PC5   // E2_FAN
//#define BTN_ENC                           PC3   // BED_THERMISTOR_2

//
// Extension pins
//
//#define EXT0_PIN                          PB0   // E2_HEAT
//#define EXT1_PIN                          PB1   // E3_HEAT
//#define EXT2_PIN                          PB2   // not used (tied to ground)
//#define EXT3_PIN                          PD8   // X_STOP
//#define EXT4_PIN                          PD9   // Y_STOP
//#define EXT5_PIN                          PD10  // Z_STOP
//#define EXT6_PIN                          PD11  // U_STOP
//#define EXT7_PIN                          PD12  // E1_PWM
//#define EXT8_PIN                          PB10  // Y_PWM

// WIFI
// PD3   CTS
// PD4   RTS
// PD5   TX
// PD6   RX
// PB5   WIFI_WAKEUP
// PE11  WIFI_RESET
// PE12  WIFI_BOOT

// I2C USER
// PB7   SDA
// PB6   SCL

// JTAG
// PA13  JTAG_TMS/SWDIO
// PA14  JTAG_TCK/SWCLK
// PB3   JTAG_TDO/SWO

//
// Onboard SD support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)

  #define SDIO_SUPPORT                            // Use SDIO for onboard SD
  #if DISABLED(SDIO_SUPPORT)
    #define SOFTWARE_SPI                          // Use soft SPI for onboard SD
    #define SDSS                            PC11
    #define SD_SCK_PIN                      PC12
    #define SD_MISO_PIN                     PC8
    #define SD_MOSI_PIN                     PD2
  #endif

  //#define SD_DETECT_PIN                   PA15

#endif

#ifndef SDSS
  #define SDSS                              PA4   // SPI1_CS
#endif

// OTG
// PA11   OTG_DM
// PA12   OTG_DP

// USER_PINS
// PD7    USER3
// PB9    USER1
// PE0    USER2
// PB4    USER4

// USERKET
// PE7    USER_BUTTON

// PA9    TX
// PA10   RX

// IR/PROBE
// PD1    IR_OUT
// PC1    IR_ON

/**
 * Logical pin vs. port/pin cross reference
 *
 * PA0    E1_THERMISTOR           PD0    W_STOP
 * PA1    E2_THERMISTOR           PD1    IR_OUT
 * PA2    E3_THERMISTOR           PD2    SDIO_CMD
 * PA3    BED_THERMISTOR_3        PD3    CTS
 * PA4    SPI1_CS                 PD4    RTS
 * PA5    SPI1_SCK                PD5    TX
 * PA6    SPI1_MISO               PD6    RX
 * PA7    SPI1_MOSI               PD7    USER3
 * PA8    V_STOP                  PD8    X_STOP
 * PA9    TX                      PD9    Y_STOP
 * PA10   RX                      PD10   Z_STOP
 * PA11   OTG_DM                  PD11   U_STOP
 * PA12   OTG_DP                  PD12   E1_PWM
 * PA13   JTAG_TMS/SWDIO          PD13   BED_HEAT_2
 * PA14   JTAG_TCK/SWCLK          PD14   BED_HEAT_1
 * PA15   SD_CARD_DETECT          PD15   BED_HEAT_3
 *
 * PB0    E2_HEAT_PWM             PE0    USER2
 * PB1    E3_HEAT_PWM             PE1    STATUS_LED
 * PB2    ---                     PE2    E3_DIR
 * PB3    JTAG_TDO/SWO            PE3    E3_RESET
 * PB4    USER4                   PE4    E2_RESET
 * PB5    WIFI_WAKEUP             PE5    E2_PWM
 * PB6    SCL                     PE6    E2_DIR
 * PB7    SDA                     PE7    USER_BUTTON
 * PB8    E3_PWM                  PE8    E3_FAN
 * PB9    USER1                   PE9    Y_DIR
 * PB10   Y_PWM                   PE10   Y_RESET
 * PB11   ---                     PE11   WIFI_RESET
 * PB12   SPI2_CS                 PE12   WIFI_BOOT
 * PB13   SPI2_SCK                PE13   X_RESET
 * PB14   SPI2_MISO               PE14   X_PWM
 * PB15   SPI2_MOSI               PE15   X_DIR
 *
 * PC0    Z_DIR
 * PC1    IR_ON
 * PC2    BED_THERMISTOR_1
 * PC3    BED_THERMISTOR_2
 * PC4    E1_FAN
 * PC5    E2_FAN
 * PC6    Z_PWM
 * PC7    E1_HEAT_PWM
 * PC8    SDIO_D0
 * PC9    SDIO_D1
 * PC10   SDIO_D2
 * PC11   SDIO_D3
 * PC12   SDIO_CK
 * PC13   E1_DIR
 * PC14   E1_RESET
 * PC15   Z_RESET
 */
