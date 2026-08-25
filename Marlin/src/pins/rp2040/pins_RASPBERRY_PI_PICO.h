/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define BOARD_INFO_NAME "Pi Pico"
#define DEFAULT_MACHINE_NAME "Pi Pico"

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE             0x1000U  // 4K
  #define FLASH_EEPROM_EMULATION
#endif

//
// Limit Switches
//
#define X_STOP_PIN                             0  // GP0
#define Y_STOP_PIN                             1  // GP1
#define Z_STOP_PIN                             2  // GP2

//
// Steppers
//
#define X_STEP_PIN                             3  // GP3
#define X_DIR_PIN                              6  // GP6
#define X_ENABLE_PIN                           7  // GP7

#define Y_STEP_PIN                             8  // GP8
#define Y_DIR_PIN                              9  // GP9
#define Y_ENABLE_PIN                X_ENABLE_PIN

#define Z_STEP_PIN                            10  // GP10
#define Z_DIR_PIN                             11  // GP11
#define Z_ENABLE_PIN                X_ENABLE_PIN

#define E0_STEP_PIN                           12  // GP12
#define E0_DIR_PIN                            13  // GP13
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                          14  // GP14
#define HEATER_BED_PIN                        15  // GP15
#define FAN0_PIN                              28  // GP28

//
// Temperature Sensors
//
#define TEMP_0_PIN                            A0  // GP26 - Analog Input
#define TEMP_BED_PIN                          A1  // GP27 - Analog Input

//
// Misc. Functions
//
#define LED_PIN                               25  // GP25

//
// SD Card
//
#define SD_DETECT_PIN                         -1  // No pins left
#define SDCARD_CONNECTION ONBOARD
#define SD_SS_PIN                             17  // GP17 - SPI0_CS for SD card chip select

//
// LCD / Controller
//

#if ENABLED(U8GLIB_SSD1306)
  #if !IS_ULTIPANEL
    #error "Add '#define IS_ULTIPANEL 1' to Configuration.h to enable Marlin UI menus and encoder support."
  #endif
  #if IS_NEWPANEL
    #define BTN_EN1                           20  // GP20 - Encoder A phase
    #define BTN_EN2                           21  // GP21 - Encoder B phase
    #define BTN_ENC                           22  // GP22 - Encoder push button
  #endif

  #define ALTERNATIVE_LCD // Use default hardware I2C port
#endif // U8GLIB_SSD1306

/*
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                  RASPBERRY PI PICO - PHYSICAL PIN MAPPING                                 │
├──────────────┬───────────┬───────────┬───────────┬───────────┬────────────────────────────────────────────┤
│ Physical Pin │   GPIO    │   ALT0    │   ALT1    │   ALT2    │                Marlin Usage                │
├──────────────┼───────────┼───────────┼───────────┼───────────┼────────────────────────────────────────────┤
│       1      │   GP0     │  I2C0_SDA │ SPI0_MISO │ UART0_TX  │ X_STOP_PIN                                 │
│       2      │   GP1     │  I2C0_SCL │ SPI0_CS   │ UART0_RX  │ Y_STOP_PIN                                 │
│       3      │   GND     │           │           │           │                                            │
│       4      │   GP2     │  I2C1_SDA │ SPI0_SCK  │           │ Z_STOP_PIN                                 │
│       5      │   GP3     │  I2C1_SCL │ SPI0_MOSI │           │ X_STEP_PIN                                 │
│       6      │   GP4     │  I2C0_SDA │ SPI0_MISO │ UART1_TX  │ SDA_PIN                                    │
│       7      │   GP5     │  I2C0_SCL │ SPI0_CS   │ UART1_RX  │ SCL_PIN                                    │
│       8      │   GND     │           │           │           │                                            │
│       9      │   GP6     │  I2C1_SDA │ SPI0_SCK  │           │ X_DIR_PIN                                  │
│      10      │   GP7     │  I2C1_SCL │ SPI0_MOSI │           │ X_ENABLE_PIN                               │
│      11      │   GP8     │  I2C0_SDA │ SPI1_MISO │ UART1_TX  │ Y_STEP_PIN                                 │
│      12      │   GP9     │  I2C0_SCL │ SPI1_CS   │ UART1_RX  │ Y_DIR_PIN                                  │
│      13      │   GND     │           │           │           │                                            │
│      14      │   GP10    │  I2C1_SDA │ SPI1_SCK  │           │ Z_STEP_PIN                                 │
│      15      │   GP11    │  I2C1_SCL │ SPI1_MOSI │           │ Z_DIR_PIN                                  │
│      16      │   GP12    │  I2C0_SDA │ SPI1_MISO │ UART0_TX  │ E0_STEP_PIN                                │
│      17      │   GP13    │  I2C0_SCL │ SPI1_CS   │ UART0_RX  │ E0_DIR_PIN                                 │
│      18      │   GND     │           │           │           │                                            │
│      19      │   GP14    │  I2C1_SDA │ SPI1_SCK  │           │ HEATER_0_PIN                               │
│      20      │   GP15    │  I2C1_SCL │ SPI1_MOSI │           │ HEATER_BED_PIN                             │
│      21      │   GP16    │  I2C0_SDA │ SPI0_MISO │ UART0_TX  │ SD_MISO_PIN                                │
│      22      │   GP17    │  I2C0_SCL │ SPI0_CS   │ UART0_RX  │ SD_SS_PIN                                  │
│      23      │   GND     │           │           │           │                                            │
│      24      │   GP18    │  I2C1_SDA │ SPI0_SCK  │           │ SD_SCK_PIN                                 │
│      25      │   GP19    │  I2C1_SCL │ SPI0_MOSI │           │ SD_MOSI_PIN                                │
│      26      │   GP20    │  I2C0_SDA │           │           │ BTN_EN1 (Encoder A)                        │
│      27      │   GP21    │  I2C0_SCL │           │           │ BTN_EN2 (Encoder B)                        │
│      28      │   GND     │           │           │           │                                            │
│      29      │   GP22    │           │           │           │ BTN_ENC (Encoder Button)                   │
│      30      │   RUN     │           │           │           │ Reset (Active Low)                         │
│      31      │   GP26    │   ADC0    │ I2C1_SDA  │           │ TEMP_0_PIN (A0)                            │
│      32      │   GP27    │   ADC1    │ I2C1_SCL  │           │ TEMP_BED_PIN (A1)                          │
│      33      │   GND     │           │           │           │                                            │
│      34      │   GP28    │   ADC2    │           │           │ FAN0_PIN                                   │
│      35      │   ADC_VREF│           │           │           │ ADC Reference Voltage                      │
│      36      │   3V3     │           │           │           │ 3.3V Power Output                          │
│      37      │   3V3_EN  │           │           │           │ 3.3V Enable (Active High)                  │
│      38      │   GND     │           │           │           │                                            │
│      39      │   VSYS    │           │           │           │ System Voltage Input (1.8-5.5V)            │
│      40      │   VBUS    │           │           │           │ USB Bus Voltage (5V when USB connected)    │
├──────────────┴───────────┴───────────┴───────────┴───────────┴────────────────────────────────────────────┤
│ Special pin notes for Raspberry Pi Pico                                                                   │
├───────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ • GP23 OP Controls the on-board SMPS Power Save pin. Should not be used for other purposes.               │
│ • GP24 IP VBUS sense - high if VBUS is present, else low. Should not be used for other purposes.          │
│ • GP25 OP Connected to user LED. Can be used with board modifications, is not broken out.                 │
│ • GP29 IP Used in ADC mode (ADC3) to measure VSYS/3. Should not be used for other purposes.               │
│ • ADC4 (Internal) is connected to the chip's internal temperature sensor.                                 │
├───────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ Marlin pin notes                                                                                          │
├───────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ Shared enable pins:                                                                                       │
│ • GP7: X_ENABLE_PIN. All stepper drivers use a shared single enable pin X_ENABLE_PIN for pin conservation │
│                                                                                                           │
│ Hardware I2C and SPI configuration:                                                                       │
│   These are hardcoded in:                                                                                 │
│ .platformio/packages/framework-arduino-mbed/variants/RASPBERRY_RASPBERRY_PI_PICO/pins_arduino.h           │
│ • GP4: SDA           default: "#define PIN_WIRE_SDA        (4u)"                                          │
│ • GP5: SCL           default: "#define PIN_WIRE_SCL        (5u)"                                          │
│ • GP16: MISO         default: "#define PIN_SPI_MISO        (16u)"                                         │
│ • GP18: SCK          default: "#define PIN_SPI_SCK         (18u)"                                         │
│ • GP19: MOSI         default: "#define PIN_SPI_MOSI        (19u)"                                         │
└───────────────────────────────────────────────────────────────────────────────────────────────────────────┘
*/
