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

/**
 * The purpose of this file is just include Marlin Configuration files,
 * to discover which FEATURES are enabled, without any HAL include.
 * Used by common-dependencies.py
 */

#include "../../../../Marlin/src/inc/MarlinConfig.h"

//
// Conditionals only used for [features]
//
#if ENABLED(SR_LCD_3W_NL)
  // Feature checks for SR_LCD_3W_NL
#elif ANY(LCD_I2C_TYPE_MCP23017, LCD_I2C_TYPE_MCP23008)
  #define USES_LIQUIDTWI2
#elif ENABLED(LCD_I2C_TYPE_PCA8574)
  #define USES_LIQUIDCRYSTAL_I2C
#elif ANY(HAS_MARLINUI_HD44780, LCD_I2C_TYPE_PCF8575, SR_LCD_2W_NL, LCM1602)
  #define USES_LIQUIDCRYSTAL
#endif

#if SAVED_POSITIONS
  #define HAS_SAVED_POSITIONS
#endif

#if ENABLED(DUET_SMART_EFFECTOR) && PIN_EXISTS(SMART_EFFECTOR_MOD)
  #define HAS_SMART_EFF_MOD
#endif

#if HAS_MARLINUI_MENU
  #if ENABLED(BACKLASH_GCODE)
    #define HAS_MENU_BACKLASH
  #endif
  #if ENABLED(LCD_BED_TRAMMING)
    #define HAS_MENU_BED_TRAMMING
  #endif
  #if ENABLED(CANCEL_OBJECTS)
    #define HAS_MENU_CANCELOBJECT
  #endif
  #if ANY(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)
    #define HAS_MENU_DELTA_CALIBRATE
  #endif
  #if ANY(LED_CONTROL_MENU, CASE_LIGHT_MENU)
    #define HAS_MENU_LED
  #endif
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #define HAS_MENU_FILAMENT
  #endif
  #if HAS_MEDIA
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
  #if ENABLED(ASSISTED_TRAMMING_WIZARD)
    #define HAS_MENU_TRAMMING_WIZARD
  #endif
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    #define HAS_MENU_UBL
  #endif
  #if ENABLED(ONE_CLICK_PRINT)
    #define HAS_MENU_ONE_CLICK_PRINT
  #endif
#endif

#if HAS_GRAPHICAL_TFT
  #include "../../../../Marlin/src/lcd/tft/fontdata/fontdata.h"
  #define UI_INCL_(W, H) STRINGIFY_(../../../../Marlin/src/lcd/tft/ui_##W##x##H.h)
  #define UI_INCL(W, H) UI_INCL_(W, H)
  #include UI_INCL(TFT_WIDTH, TFT_HEIGHT)

  #if TFT_FONT == NOTOSANS
    #if FONT_SIZE == 14
      #define TFT_FONT_NOTOSANS_14
    #elif FONT_SIZE == 16
      #define TFT_FONT_NOTOSANS_16
    #elif FONT_SIZE == 19
      #define TFT_FONT_NOTOSANS_19
    #elif FONT_SIZE == 26
      #define TFT_FONT_NOTOSANS_26
    #elif FONT_SIZE == 27
      #define TFT_FONT_NOTOSANS_27
    #elif FONT_SIZE == 28
      #define TFT_FONT_NOTOSANS_28
    #elif FONT_SIZE == 29
      #define TFT_FONT_NOTOSANS_29
    #endif
  #elif TFT_FONT == UNIFONT
    #if FONT_SIZE == 10
      #define TFT_FONT_UNIFONT_10
    #elif FONT_SIZE == 20
      #define TFT_FONT_UNIFONT_20
    #elif FONT_SIZE == 30
      #define TFT_FONT_UNIFONT_30
    #endif
  #elif TFT_FONT == HELVETICA
    #if FONT_SIZE == 14
      #define TFT_FONT_HELVETICA_14
    #elif FONT_SIZE == 19
      #define TFT_FONT_HELVETICA_19
    #endif
  #endif
#endif
