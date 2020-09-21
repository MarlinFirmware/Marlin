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
 * The purpose of this file is just include Marlin Configuration files,
 * to discover which FEATURES are enabled, without any HAL include.
 * Used by common-dependencies.py
 */

#include <stdint.h>

// Include platform headers
//#include "../../../../Marlin/src/HAL/platforms.h"

#include "../../../../Marlin/src/core/boards.h"
#include "../../../../Marlin/src/core/macros.h"
#include "../../../../Marlin/Configuration.h"

#include "../../../../Marlin/Version.h"

#include "../../../../Marlin/src/inc/Conditionals_LCD.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_LCD.h)
#endif

#include "../../../../Marlin/src/core/drivers.h"
#include "../../../../Marlin/Configuration_adv.h"

#include "../../../../Marlin/src/inc/Conditionals_adv.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_adv.h)
#endif

//#include "../../../../Marlin/src/pins/pins.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, timers.h)
  #include HAL_PATH(../../../../Marlin/src/HAL, spi_pins.h)
#endif

#include "../../../../Marlin/src/inc/Conditionals_post.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_post.h)
#endif

//
// Conditionals only used for [features]
//
#if ENABLED(SR_LCD_3W_NL)
  // Feature checks for SR_LCD_3W_NL
#elif EITHER(LCD_I2C_TYPE_MCP23017, LCD_I2C_TYPE_MCP23008)
  #define USES_LIQUIDTWI2
#elif ANY(HAS_CHARACTER_LCD, LCD_I2C_TYPE_PCF8575, LCD_I2C_TYPE_PCA8574, SR_LCD_2W_NL, LCM1602)
  #define USES_LIQUIDCRYSTAL
#endif

#if BOTH(ANYCUBIC_LCD_I3MEGA, EXTENSIBLE_UI)
  #define HAS_ANYCUBIC_TFT_EXTUI
#endif

#if SAVED_POSITIONS
  #define HAS_SAVED_POSITIONS
#endif

#if ENABLED(HOST_PROMPT_SUPPORT) && DISABLED(EMERGENCY_PARSER)
  #define HAS_GCODE_M876
#endif

#if EXTRUDERS
  #define HAS_EXTRUDERS
#endif

#if HAS_LCD_MENU
  #if ENABLED(BACKLASH_GCODE)
    #define HAS_MENU_BACKLASH
  #endif
  #if ENABLED(LEVEL_BED_CORNERS)
    #define HAS_MENU_BED_CORNERS
  #endif
  #if ENABLED(CANCEL_OBJECTS)
    #define HAS_MENU_CANCELOBJECT
  #endif
  #if ENABLED(CUSTOM_USER_MENUS)
    #define HAS_MENU_CUSTOM
  #endif
  #if EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)
    #define HAS_MENU_DELTA_CALIBRATE
  #endif
  #if EITHER(LED_CONTROL_MENU, CASE_LIGHT_MENU)
    #define HAS_MENU_LED
  #endif
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #define HAS_MENU_FILAMENT
  #endif
  #if ENABLED(SDSUPPORT)
    #define HAS_MENU_MEDIA
  #endif
  #if ENABLED(MIXING_EXTRUDER)
    #define HAS_MENU_MIXER
  #endif
  #if ENABLED(POWER_LOSS_RECOVERY)
    #define HAS_MENU_JOB_RECOVERY
  #endif
  #if HAS_POWER_MONITOR
    #define HAS_MENU_POWER_MONITOR
  #endif
  #if HAS_CUTTER
    #define HAS_MENU_CUTTER
  #endif
  #if HAS_TEMPERATURE
    #define HAS_MENU_TEMPERATURE
  #endif
  #if ENABLED(MMU2_MENUS)
    #define HAS_MENU_MMU2
  #endif
  #if ENABLED(PASSWORD_FEATURE)
    #define HAS_MENU_PASSWORD
  #endif
  #if HAS_TRINAMIC_CONFIG
    #define HAS_MENU_TMC
  #endif
  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    #define HAS_MENU_TOUCH_SCREEN
  #endif
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    #define HAS_MENU_UBL
  #endif
#endif

// define all platform macros to include pins file
#define __PREBUILD_SCRIPT_RUNNING__
#define TARGET_LPC1768
#define IS_TEENSY41 1
#define IS_32BIT_TEENSY 1
#define MKS_HAS_LPC1769
#define ARDUINO_ARCH_ESP32
#define ARDUINO_ARCH_SAM
#define ARDUINO_GRAND_CENTRAL_M4
#define MCU_LPC1768
#define MCU_LPC1769
#define STM32F0xx
#define STM32F1
#define STM32F4
#define STM32F7
#define STM32F1xx
#define STM32F7xx
#define STM32F4xx
#define __STM32F4__
#define TARGET_STM32F1
#define __AVR_AT90USB1286P__
#define __AVR_ATmega2561__
#define __AVR_AT90USB1286__
#define __AVR_AT90USB646__
#define __AVR_ATmega1281__
#define __AVR_ATmega1284P__
#define __AVR_ATmega2560__
#define __AVR_ATmega644P__
#define __AVR_ATmega644__
#define __AVR_ATmega1280__
#define __SAM3X8E__
#define __STM32F1__
#include "../../../../Marlin/src/pins/pins.h"
