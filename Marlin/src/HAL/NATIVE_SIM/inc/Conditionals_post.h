/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Pins the simulator treats as protected (M43 skips them unless the I flag is given).
 *
 * Marlin's own list (sensitive_pins.h) already covers steppers, heaters, fans and
 * temperature sensors. Nothing on this platform can be damaged, so the HAL adds only
 * the pins whose virtual hardware would be disturbed or whose use has a side effect.
 * This file comes after pins.h, so each pin can be tested for existence here.
 */
  // User-driven inputs: the encoder, buttons (including Back) and card-detect switch are fed from the simulator window.
  #if defined(BTN_EN1) && (BTN_EN1) >= 0
    #define _SIMP_BTN_EN1 BTN_EN1,
  #else
    #define _SIMP_BTN_EN1
  #endif
  #if defined(BTN_EN2) && (BTN_EN2) >= 0
    #define _SIMP_BTN_EN2 BTN_EN2,
  #else
    #define _SIMP_BTN_EN2
  #endif
  #if defined(BTN_ENC) && (BTN_ENC) >= 0
    #define _SIMP_BTN_ENC BTN_ENC,
  #else
    #define _SIMP_BTN_ENC
  #endif
  #if defined(BTN_BACK) && (BTN_BACK) >= 0
    #define _SIMP_BTN_BACK BTN_BACK,
  #else
    #define _SIMP_BTN_BACK
  #endif
  #if defined(SD_DETECT_PIN) && (SD_DETECT_PIN) >= 0
    #define _SIMP_SD_DETECT_PIN SD_DETECT_PIN,
  #else
    #define _SIMP_SD_DETECT_PIN
  #endif

  // Display and SD buses: simulated devices decode the edges, so stray pulses corrupt their state.
  #if defined(LCD_PINS_RS) && (LCD_PINS_RS) >= 0
    #define _SIMP_LCD_PINS_RS LCD_PINS_RS,
  #else
    #define _SIMP_LCD_PINS_RS
  #endif
  #if defined(LCD_PINS_EN) && (LCD_PINS_EN) >= 0
    #define _SIMP_LCD_PINS_EN LCD_PINS_EN,
  #else
    #define _SIMP_LCD_PINS_EN
  #endif
  #if defined(LCD_PINS_D4) && (LCD_PINS_D4) >= 0
    #define _SIMP_LCD_PINS_D4 LCD_PINS_D4,
  #else
    #define _SIMP_LCD_PINS_D4
  #endif
  #if defined(LCD_PINS_D5) && (LCD_PINS_D5) >= 0
    #define _SIMP_LCD_PINS_D5 LCD_PINS_D5,
  #else
    #define _SIMP_LCD_PINS_D5
  #endif
  #if defined(LCD_PINS_D6) && (LCD_PINS_D6) >= 0
    #define _SIMP_LCD_PINS_D6 LCD_PINS_D6,
  #else
    #define _SIMP_LCD_PINS_D6
  #endif
  #if defined(LCD_PINS_D7) && (LCD_PINS_D7) >= 0
    #define _SIMP_LCD_PINS_D7 LCD_PINS_D7,
  #else
    #define _SIMP_LCD_PINS_D7
  #endif
  #if defined(DOGLCD_A0) && (DOGLCD_A0) >= 0
    #define _SIMP_DOGLCD_A0 DOGLCD_A0,
  #else
    #define _SIMP_DOGLCD_A0
  #endif
  #if defined(DOGLCD_CS) && (DOGLCD_CS) >= 0
    #define _SIMP_DOGLCD_CS DOGLCD_CS,
  #else
    #define _SIMP_DOGLCD_CS
  #endif
  #if defined(DOGLCD_SCK) && (DOGLCD_SCK) >= 0
    #define _SIMP_DOGLCD_SCK DOGLCD_SCK,
  #else
    #define _SIMP_DOGLCD_SCK
  #endif
  #if defined(DOGLCD_MOSI) && (DOGLCD_MOSI) >= 0
    #define _SIMP_DOGLCD_MOSI DOGLCD_MOSI,
  #else
    #define _SIMP_DOGLCD_MOSI
  #endif
  #if defined(SD_SCK_PIN) && (SD_SCK_PIN) >= 0
    #define _SIMP_SD_SCK_PIN SD_SCK_PIN,
  #else
    #define _SIMP_SD_SCK_PIN
  #endif
  #if defined(SD_MISO_PIN) && (SD_MISO_PIN) >= 0
    #define _SIMP_SD_MISO_PIN SD_MISO_PIN,
  #else
    #define _SIMP_SD_MISO_PIN
  #endif
  #if defined(SD_MOSI_PIN) && (SD_MOSI_PIN) >= 0
    #define _SIMP_SD_MOSI_PIN SD_MOSI_PIN,
  #else
    #define _SIMP_SD_MOSI_PIN
  #endif
  #if defined(SD_SS_PIN) && (SD_SS_PIN) >= 0
    #define _SIMP_SD_SS_PIN SD_SS_PIN,
  #else
    #define _SIMP_SD_SS_PIN
  #endif

  // Kill: pulling it active stops the machine, just as on real hardware.
  #if defined(KILL_PIN) && (KILL_PIN) >= 0
    #define _SIMP_KILL_PIN KILL_PIN,
  #else
    #define _SIMP_KILL_PIN
  #endif

#define HAL_SENSITIVE_PINS \
    _SIMP_BTN_EN1 \
    _SIMP_BTN_EN2 \
    _SIMP_BTN_ENC \
    _SIMP_BTN_BACK \
    _SIMP_SD_DETECT_PIN \
    _SIMP_LCD_PINS_RS \
    _SIMP_LCD_PINS_EN \
    _SIMP_LCD_PINS_D4 \
    _SIMP_LCD_PINS_D5 \
    _SIMP_LCD_PINS_D6 \
    _SIMP_LCD_PINS_D7 \
    _SIMP_DOGLCD_A0 \
    _SIMP_DOGLCD_CS \
    _SIMP_DOGLCD_SCK \
    _SIMP_DOGLCD_MOSI \
    _SIMP_SD_SCK_PIN \
    _SIMP_SD_MISO_PIN \
    _SIMP_SD_MOSI_PIN \
    _SIMP_SD_SS_PIN \
    _SIMP_KILL_PIN
