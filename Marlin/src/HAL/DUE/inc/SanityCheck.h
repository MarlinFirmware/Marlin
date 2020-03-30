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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Test Arduino Due specific configuration values for errors at compile-time.
 */

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
#define _IS_HW_SPI(P) (defined(TMC_SW_##P) && (TMC_SW_##P == MOSI_PIN || TMC_SW_##P == MISO_PIN || TMC_SW_##P == SCK_PIN))

#if ENABLED(SDSUPPORT) && HAS_DRIVER(TMC2130)
  #if ENABLED(TMC_USE_SW_SPI)
    #if DISABLED(DUE_SOFTWARE_SPI) && (_IS_HW_SPI(MOSI) || _IS_HW_SPI(MISO) || _IS_HW_SPI(SCK))
      #error "DUE hardware SPI is required but is incompatible with TMC2130 software SPI. Either disable TMC_USE_SW_SPI or use separate pins for the two SPIs."
    #endif
  #elif ENABLED(DUE_SOFTWARE_SPI)
    #error "DUE software SPI is required but is incompatible with TMC2130 hardware SPI. Enable TMC_USE_SW_SPI to fix."
  #endif
#endif

#if ENABLED(FAST_PWM_FAN)
  #error "FAST_PWM_FAN is not yet implemented for this platform."
#endif

#if HAS_TMC_SW_SERIAL
  #error "TMC220x Software Serial is not supported on this platform."
#endif
