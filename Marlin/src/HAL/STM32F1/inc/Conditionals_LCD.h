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

#if ENABLED(USE_USB_COMPOSITE)
  //#warning "SD_CHECK_AND_RETRY isn't needed with USE_USB_COMPOSITE."
  #undef SD_CHECK_AND_RETRY
#endif

#if HAS_SPI_TFT
  #error "Sorry! SPI TFT displays are not available for HAL/STM32F1 (yet)."
#endif

// This platform has 'touch/xpt2046', not 'tft/xpt2046'
#if ENABLED(TOUCH_SCREEN) && !HAS_FSMC_TFT
  #undef TOUCH_SCREEN
  #undef TOUCH_SCREEN_CALIBRATION
  #define HAS_TOUCH_XPT2046 1
#endif
