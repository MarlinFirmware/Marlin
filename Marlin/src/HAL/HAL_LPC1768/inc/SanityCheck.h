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
 *  Try to detect users copying over old pins file by checking if adc pins are defined to old values that are not ADC compatible pins
 */
#if TEMP_BED_PIN == 0 ||TEMP_BED_PIN == 1 ||TEMP_BED_PIN == 4 || TEMP_BED_PIN == 5 || TEMP_BED_PIN == 6 || TEMP_BED_PIN == 7
  #error "Please update your pins file, ADC pin definitions should now use the Pn_nn_An (or Pn_nn) format as used in the default pins files"
#endif
#if TEMP_0_PIN == 0 ||TEMP_0_PIN == 1 ||TEMP_0_PIN == 4 || TEMP_0_PIN == 5 || TEMP_0_PIN == 6 || TEMP_0_PIN == 7
  #error "Please update your pins file, ADC pin definitions should now use the Pn_nn_An (or Pn_nn) format as used in the default pins files"
#endif
#if TEMP_1_PIN == 0 ||TEMP_1_PIN == 1 ||TEMP_1_PIN == 4 || TEMP_1_PIN == 5 || TEMP_1_PIN == 6 || TEMP_1_PIN == 7
  #error "Please update your pins file, ADC pin definitions should now use the Pn_nn_An (or Pn_nn) format as used in the default pins files"
#endif

/**
 *  Due to the way the PWM hardware channels all share the same frequency, as do the software fallback
 *  channels, FAST_PWM_FAN is incompatible with Servos
 */
#if NUM_SERVOS > 0 && ENABLED(FAST_PWM_FAN)
   #error "Servos and BLTouch are incompatible with FAST_PWM_FAN on LPC176x boards"
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
