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

#if PIO_PLATFORM_VERSION < 1001
  #error "nxplpc-arduino-lpc176x package is out of date, Please update the PlatformIO platforms, frameworks and libraries. You may need to remove the platform and let it reinstall automatically."
#endif
#if PIO_FRAMEWORK_VERSION < 2002
  #error "framework-arduino-lpc176x package is out of date, Please update the PlatformIO platforms, frameworks and libraries."
#endif

/**
 * Detect an old pins file by checking for old ADC pins values.
 */
#if PIN_EXISTS(TEMP_BED) && TEMP_BED_PIN <= 7
  #error "TEMP_BED_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_0) && TEMP_0_PIN <= 7
  #error "TEMP_0_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_1) && TEMP_1_PIN <= 7
  #error "TEMP_1_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_2) && TEMP_2_PIN <= 7
  #error "TEMP_2_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_3) && TEMP_3_PIN <= 7
  #error "TEMP_3_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_4) && TEMP_4_PIN <= 7
  #error "TEMP_4_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#elif PIN_EXISTS(TEMP_5) && TEMP_5_PIN <= 7
  #error "TEMP_5_PIN must be defined using the Pn_nn or Pn_nn_An format. (See the included pins files)."
#endif

/**
 * Because PWM hardware channels all share the same frequency, along with the
 * fallback software channels, FAST_PWM_FAN is incompatible with Servos.
 */
#if NUM_SERVOS > 0 && ENABLED(FAST_PWM_FAN)
  #error "BLTOUCH and Servos are incompatible with FAST_PWM_FAN on LPC176x boards."
#endif

/**
 * Test LPC176x-specific configuration values for errors at compile-time.
 */

//#if ENABLED(SPINDLE_LASER_PWM) && !(SPINDLE_LASER_PWM_PIN == 4 || SPINDLE_LASER_PWM_PIN == 6 || SPINDLE_LASER_PWM_PIN == 11)
//  #error "SPINDLE_LASER_PWM_PIN must use SERVO0, SERVO1 or SERVO3 connector"
//#endif

#if IS_RE_ARM_BOARD && ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) && HAS_DRIVER(TMC2130) && DISABLED(TMC_USE_SW_SPI)
  #error "Re-ARM with REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER and TMC2130 require TMC_USE_SW_SPI"
#endif
