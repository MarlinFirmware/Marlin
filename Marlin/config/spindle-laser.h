/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Spindle & Laser control
 *
 * Add the M3, M4, and M5 commands to turn the spindle/laser on and off, and
 * to set spindle speed, spindle direction, and laser power.
 *
 * SuperPid is a router/spindle speed controller used in the CNC milling community.
 * Marlin can be used to turn the spindle on and off. It can also be used to set
 * the spindle speed from 5,000 to 30,000 RPM.
 *
 * You'll need to select a pin for the ON/OFF function and optionally choose a 0-5V
 * hardware PWM pin for the speed control and a pin for the rotation direction.
 *
 * See http://marlinfw.org/docs/configuration/laser_spindle.html for more config details.
 */
//#define SPINDLE_LASER_ENABLE
#if ENABLED(SPINDLE_LASER_ENABLE)

  #define SPINDLE_LASER_ENABLE_INVERT   false  // set to "true" if the on/off function is reversed
  #define SPINDLE_LASER_PWM             true   // set to true if your controller supports setting the speed/power
  #define SPINDLE_LASER_PWM_INVERT      true   // set to "true" if the speed/power goes up when you want it to go slower
  #define SPINDLE_LASER_POWERUP_DELAY   5000   // delay in milliseconds to allow the spindle/laser to come up to speed/power
  #define SPINDLE_LASER_POWERDOWN_DELAY 5000   // delay in milliseconds to allow the spindle to stop
  #define SPINDLE_DIR_CHANGE            true   // set to true if your spindle controller supports changing spindle direction
  #define SPINDLE_INVERT_DIR            false
  #define SPINDLE_STOP_ON_DIR_CHANGE    true   // set to true if Marlin should stop the spindle before changing rotation direction

  /**
   *  The M3 & M4 commands use the following equation to convert PWM duty cycle to speed/power
   *
   *  SPEED/POWER = PWM duty cycle * SPEED_POWER_SLOPE + SPEED_POWER_INTERCEPT
   *    where PWM duty cycle varies from 0 to 255
   *
   *  set the following for your controller (ALL MUST BE SET)
   */

  #define SPEED_POWER_SLOPE    118.4
  #define SPEED_POWER_INTERCEPT  0
  #define SPEED_POWER_MIN     5000
  #define SPEED_POWER_MAX    30000    // SuperPID router controller 0 - 30,000 RPM

  //#define SPEED_POWER_SLOPE      0.3922
  //#define SPEED_POWER_INTERCEPT  0
  //#define SPEED_POWER_MIN       10
  //#define SPEED_POWER_MAX      100      // 0-100%
#endif
