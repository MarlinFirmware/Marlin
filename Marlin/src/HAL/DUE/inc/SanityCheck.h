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
 * Test Arduino Due specific configuration values for errors at compile-time.
 */

#if HAS_SPI_TFT || HAS_FSMC_TFT
  #error "Sorry! TFT displays are not available for HAL/DUE."
#endif

/**
 * Check for common serial pin conflicts
 */
#define CHECK_SERIAL_PIN(N) ( \
     X_STOP_PIN == N || Y_STOP_PIN == N || Z_STOP_PIN == N \
  || X_MIN_PIN  == N || Y_MIN_PIN  == N || Z_MIN_PIN  == N \
  || X_MAX_PIN  == N || Y_MAX_PIN  == N || Z_MAX_PIN  == N \
  || X_STEP_PIN == N || Y_STEP_PIN == N || Z_STEP_PIN == N \
  || X_DIR_PIN  == N || Y_DIR_PIN  == N || Z_DIR_PIN  == N \
  || X_ENA_PIN  == N || Y_ENA_PIN  == N || Z_ENA_PIN  == N \
)
#if SERIAL_IN_USE(0) // D0-D1. No known conflicts.
#endif
#if SERIAL_IN_USE(1) && (CHECK_SERIAL_PIN(18) || CHECK_SERIAL_PIN(19))
  #error "Serial Port 1 pin D18 and/or D19 conflicts with another pin on the board."
#endif
#if SERIAL_IN_USE(2) && (CHECK_SERIAL_PIN(16) || CHECK_SERIAL_PIN(17))
  #error "Serial Port 2 pin D16 and/or D17 conflicts with another pin on the board."
#endif
#if SERIAL_IN_USE(3) && (CHECK_SERIAL_PIN(14) || CHECK_SERIAL_PIN(15))
  #error "Serial Port 3 pin D14 and/or D15 conflicts with another pin on the board."
#endif
#undef CHECK_SERIAL_PIN

/**
 * HARDWARE VS. SOFTWARE SPI COMPATIBILITY
 *
 * DUE selects hardware vs. software SPI depending on whether one of the hardware-controllable SDSS pins is in use.
 *
 * The hardware SPI controller doesn't allow software SPIs to control any shared pins.
 *
 * When DUE software SPI is used then Trinamic drivers must use the TMC softSPI.
 *
 * When DUE hardware SPI is used then a Trinamic driver can use either its hardware SPI or, if there are no shared
 * pins, its software SPI.
 *
 * Usually the hardware SPI pins are only available to the LCD. This makes the DUE hard SPI used at the same time
 * as the TMC2130 soft SPI the most common setup.
 */
#define _IS_HW_SPI(P) (defined(TMC_SPI_##P) && (TMC_SPI_##P == SD_MOSI_PIN || TMC_SPI_##P == SD_MISO_PIN || TMC_SPI_##P == SD_SCK_PIN))

#if HAS_MEDIA && HAS_DRIVER(TMC2130)
  #if ENABLED(TMC_USE_SW_SPI)
    #if DISABLED(SOFTWARE_SPI) && (_IS_HW_SPI(MOSI) || _IS_HW_SPI(MISO) || _IS_HW_SPI(SCK))
      #error "DUE hardware SPI is required but is incompatible with TMC2130 software SPI. Either disable TMC_USE_SW_SPI or use separate pins for the two SPIs."
    #endif
  #elif ENABLED(SOFTWARE_SPI)
    #error "DUE software SPI is required but is incompatible with TMC2130 hardware SPI. Enable TMC_USE_SW_SPI to fix."
  #endif
#endif

#if ENABLED(FAST_PWM_FAN) || SPINDLE_LASER_FREQUENCY
  #error "Features requiring Hardware PWM (FAST_PWM_FAN, SPINDLE_LASER_FREQUENCY) are not yet supported for HAL/DUE."
#endif

#if HAS_TMC_SW_SERIAL
  #error "TMC220x Software Serial is not supported on the DUE platform."
#endif

#if USING_PULLDOWNS
  #error "PULLDOWN pin mode is not available on DUE boards."
#endif
