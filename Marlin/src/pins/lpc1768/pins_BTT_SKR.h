/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef MCU_LPC1768
  #error "Oops! Make sure you have the LPC1768 environment selected in your IDE."
#endif

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Limit Switches
//
#define X_MIN_PIN          P1_29
#define X_MAX_PIN          P1_28
#define Y_MIN_PIN          P1_27
#define Y_MAX_PIN          P1_26
#define Z_MIN_PIN          P1_25
#define Z_MAX_PIN          P1_24

//
// Steppers
//

#define E1_STEP_PIN        P0_01
#define E1_DIR_PIN         P0_00
#define E1_ENABLE_PIN      P0_10

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_BED_PIN       P0_23_A0   // A0 (T0) - (67) - TEMP_BED_PIN
#define TEMP_0_PIN         P0_24_A1   // A1 (T1) - (68) - TEMP_0_PIN
#define TEMP_1_PIN         P0_25_A2   // A2 (T2) - (69) - TEMP_1_PIN

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN     P2_07
#endif
#if HOTENDS == 1
  #ifndef FAN1_PIN
    #define FAN1_PIN       P2_04
  #endif
#else
  #ifndef HEATER_1_PIN
    #define HEATER_1_PIN   P2_04
  #endif
#endif
#ifndef FAN_PIN
  #define FAN_PIN          P2_03
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN   P2_05
#endif

//
// LCD / Controller
//
#if HAS_SPI_LCD
  #define BEEPER_PIN       P1_30   // (37) not 5V tolerant
#endif

//
// SD Support
//
#define ONBOARD_SD_CS_PIN  P0_06   // Chip select for "System" SD card

#if SD_CONNECTION_IS(LCD)
  #define SCK_PIN          P0_15
  #define MISO_PIN         P0_17
  #define MOSI_PIN         P0_18
#elif SD_CONNECTION_IS(ONBOARD)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN    P0_27
  #define SCK_PIN          P0_07
  #define MISO_PIN         P0_08
  #define MOSI_PIN         P0_09
  #define SS_PIN           ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif
