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
 * Test AVR-specific configuration values for errors at compile-time.
 */

/**
 * Checks for FAST PWM
 */
#if ENABLED(FAST_PWM_FAN) && (ENABLED(USE_OCR2A_AS_TOP) && defined(TCCR2))
  #error "USE_OCR2A_AS_TOP does not apply to devices with a single output TIMER2"
#endif

/**
 * Sanity checks for Spindle / Laser PWM
 */
#if ENABLED(SPINDLE_LASER_PWM)
  #if SPINDLE_LASER_PWM_PIN == 4 || WITHIN(SPINDLE_LASER_PWM_PIN, 11, 13)
    #error "Counter/Timer for SPINDLE_LASER_PWM_PIN is used by a system interrupt."
  #elif NUM_SERVOS > 0 && (WITHIN(SPINDLE_LASER_PWM_PIN, 2, 3) || SPINDLE_LASER_PWM_PIN == 5)
    #error "Counter/Timer for SPINDLE_LASER_PWM_PIN is used by the servo system."
  #endif
#endif

/**
 * The Trinamic library includes SoftwareSerial.h, leading to a compile error.
 */
#if BOTH(HAS_TRINAMIC_CONFIG, ENDSTOP_INTERRUPTS_FEATURE)
  #error "TMCStepper includes SoftwareSerial.h which is incompatible with ENDSTOP_INTERRUPTS_FEATURE. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
#endif

#if BOTH(HAS_TMC_SW_SERIAL, MONITOR_DRIVER_STATUS)
  #error "MONITOR_DRIVER_STATUS causes performance issues when used with SoftwareSerial-connected drivers. Disable MONITOR_DRIVER_STATUS or use hardware serial to continue."
#endif
