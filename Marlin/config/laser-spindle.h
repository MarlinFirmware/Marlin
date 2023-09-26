/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

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
 * See https://marlinfw.org/docs/configuration/2.0.9/laser_spindle.html for more config details.
 */
//#define SPINDLE_FEATURE
//#define LASER_FEATURE
#if ANY(SPINDLE_FEATURE, LASER_FEATURE)
  #define SPINDLE_LASER_ACTIVE_STATE    LOW    // Set to "HIGH" if SPINDLE_LASER_ENA_PIN is active HIGH

  #define SPINDLE_LASER_USE_PWM                // Enable if your controller supports setting the speed/power
  #if ENABLED(SPINDLE_LASER_USE_PWM)
    #define SPINDLE_LASER_PWM_INVERT    false  // Set to "true" if the speed/power goes up when you want it to go slower
    #define SPINDLE_LASER_FREQUENCY     2500   // (Hz) Spindle/laser frequency (only on supported HALs: AVR, ESP32, and LPC)
                                               // ESP32: If SPINDLE_LASER_PWM_PIN is onboard then <=78125Hz. For I2S expander
                                               //  the frequency determines the PWM resolution. 2500Hz = 0-100, 977Hz = 0-255, ...
                                               //  (250000 / SPINDLE_LASER_FREQUENCY) = max value.
  #endif

  //#define AIR_EVACUATION                     // Cutter Vacuum / Laser Blower motor control with G-codes M10-M11
  #if ENABLED(AIR_EVACUATION)
    #define AIR_EVACUATION_ACTIVE       LOW    // Set to "HIGH" if the on/off function is active HIGH
    //#define AIR_EVACUATION_PIN        42     // Override the default Cutter Vacuum or Laser Blower pin
  #endif

  //#define AIR_ASSIST                         // Air Assist control with G-codes M8-M9
  #if ENABLED(AIR_ASSIST)
    #define AIR_ASSIST_ACTIVE           LOW    // Active state on air assist pin
    //#define AIR_ASSIST_PIN            44     // Override the default Air Assist pin
  #endif

  //#define SPINDLE_SERVO                      // A servo converting an angle to spindle power
  #ifdef SPINDLE_SERVO
    #define SPINDLE_SERVO_NR   0               // Index of servo used for spindle control
    #define SPINDLE_SERVO_MIN 10               // Minimum angle for servo spindle
  #endif

  /**
   * Speed / Power can be set ('M3 S') and displayed in terms of:
   *  - PWM255  (S0 - S255)
   *  - PERCENT (S0 - S100)
   *  - RPM     (S0 - S50000)  Best for use with a spindle
   *  - SERVO   (S0 - S180)
   */
  #define CUTTER_POWER_UNIT PWM255

  /**
   * Relative Cutter Power
   * Normally, 'M3 O<power>' sets
   * OCR power is relative to the range SPEED_POWER_MIN...SPEED_POWER_MAX.
   * so input powers of 0...255 correspond to SPEED_POWER_MIN...SPEED_POWER_MAX
   * instead of normal range (0 to SPEED_POWER_MAX).
   * Best used with (e.g.) SuperPID router controller: S0 = 5,000 RPM and S255 = 30,000 RPM
   */
  //#define CUTTER_POWER_RELATIVE              // Set speed proportional to [SPEED_POWER_MIN...SPEED_POWER_MAX]

  #if ENABLED(SPINDLE_FEATURE)
    //#define SPINDLE_CHANGE_DIR               // Enable if your spindle controller can change spindle direction
    #define SPINDLE_CHANGE_DIR_STOP            // Enable if the spindle should stop before changing spin direction
    #define SPINDLE_INVERT_DIR          false  // Set to "true" if the spin direction is reversed

    #define SPINDLE_LASER_POWERUP_DELAY   5000 // (ms) Delay to allow the spindle/laser to come up to speed/power
    #define SPINDLE_LASER_POWERDOWN_DELAY 5000 // (ms) Delay to allow the spindle to stop

    /**
     * M3/M4 Power Equation
     *
     * Each tool uses different value ranges for speed / power control.
     * These parameters are used to convert between tool power units and PWM.
     *
     * Speed/Power = (PWMDC / 255 * 100 - SPEED_POWER_INTERCEPT) / SPEED_POWER_SLOPE
     * PWMDC = (spdpwr - SPEED_POWER_MIN) / (SPEED_POWER_MAX - SPEED_POWER_MIN) / SPEED_POWER_SLOPE
     */
    #if ENABLED(SPINDLE_LASER_USE_PWM)
      #define SPEED_POWER_INTERCEPT       0    // (%) 0-100 i.e., Minimum power percentage
      #define SPEED_POWER_MIN          5000    // (RPM)
      #define SPEED_POWER_MAX         30000    // (RPM) SuperPID router controller 0 - 30,000 RPM
      #define SPEED_POWER_STARTUP     25000    // (RPM) M3/M4 speed/power default (with no arguments)
    #endif

  #else

    #if ENABLED(SPINDLE_LASER_USE_PWM)
      #define SPEED_POWER_INTERCEPT       0    // (%) 0-100 i.e., Minimum power percentage
      #define SPEED_POWER_MIN             0    // (%) 0-100
      #define SPEED_POWER_MAX           100    // (%) 0-100
      #define SPEED_POWER_STARTUP        80    // (%) M3/M4 speed/power default (with no arguments)
    #endif

    // Define the minimum and maximum test pulse time values for a laser test fire function
    #define LASER_TEST_PULSE_MIN           1   // (ms) Used with Laser Control Menu
    #define LASER_TEST_PULSE_MAX         999   // (ms) Caution: Menu may not show more than 3 characters

    #define SPINDLE_LASER_POWERUP_DELAY   50   // (ms) Delay to allow the spindle/laser to come up to speed/power
    #define SPINDLE_LASER_POWERDOWN_DELAY 50   // (ms) Delay to allow the spindle to stop

   /**
    * Laser Safety Timeout
    *
    * The laser should be turned off when there is no movement for a period of time.
    * Consider material flammability, cut rate, and G-code order when setting this
    * value. Too low and it could turn off during a very slow move; too high and
    * the material could ignite.
    */
    #define LASER_SAFETY_TIMEOUT_MS     1000   // (ms)

    /**
     * Any M3 or G1/2/3/5 command with the 'I' parameter enables continuous inline power mode.
     *
     * e.g., 'M3 I' enables continuous inline power which is processed by the planner.
     * Power is stored in move blocks and applied when blocks are processed by the Stepper ISR.
     *
     * 'M4 I' sets dynamic mode which uses the current feedrate to calculate a laser power OCR value.
     *
     * Any move in dynamic mode will use the current feedrate to calculate the laser power.
     * Feed rates are set by the F parameter of a move command e.g. G1 X0 Y10 F6000
     * Laser power would be calculated by bit shifting off 8 LSB's. In binary this is div 256.
     * The calculation gives us ocr values from 0 to 255, values over F65535 will be set as 255 .
     * More refined power control such as compensation for accel/decel will be addressed in future releases.
     *
     * M5 I clears inline mode and set power to 0, M5 sets the power output to 0 but leaves inline mode on.
     */

    /**
     * Enable M3 commands for laser mode inline power planner syncing.
     * This feature enables any M3 S-value to be injected into the block buffers while in
     * CUTTER_MODE_CONTINUOUS. The option allows M3 laser power to be committed without waiting
     * for a planner synchronization
     */
    //#define LASER_POWER_SYNC

    /**
     * Scale the laser's power in proportion to the movement rate.
     *
     * - Sets the entry power proportional to the entry speed over the nominal speed.
     * - Ramps the power up every N steps to approximate the speed trapezoid.
     * - Due to the limited power resolution this is only approximate.
     */
    //#define LASER_POWER_TRAP

    //
    // Laser I2C Ammeter (High precision INA226 low/high side module)
    //
    //#define I2C_AMMETER
    #if ENABLED(I2C_AMMETER)
      #define I2C_AMMETER_IMAX            0.1    // (Amps) Calibration value for the expected current range
      #define I2C_AMMETER_SHUNT_RESISTOR  0.1    // (Ohms) Calibration shunt resistor value
    #endif

    //
    // Laser Coolant Flow Meter
    //
    //#define LASER_COOLANT_FLOW_METER
    #if ENABLED(LASER_COOLANT_FLOW_METER)
      #define FLOWMETER_PIN         20  // Requires an external interrupt-enabled pin (e.g., RAMPS 2,3,18,19,20,21)
      #define FLOWMETER_PPL       5880  // (pulses/liter) Flow meter pulses-per-liter on the input pin
      #define FLOWMETER_INTERVAL  1000  // (ms) Flow rate calculation interval in milliseconds
      #define FLOWMETER_SAFETY          // Prevent running the laser without the minimum flow rate set below
      #if ENABLED(FLOWMETER_SAFETY)
        #define FLOWMETER_MIN_LITERS_PER_MINUTE 1.5 // (liters/min) Minimum flow required when enabled
      #endif
    #endif

  #endif
#endif // SPINDLE_FEATURE || LASER_FEATURE

/**
 * Synchronous Laser Control with M106/M107
 *
 * Marlin normally applies M106/M107 fan speeds at a time "soon after" processing
 * a planner block. This is too inaccurate for a PWM/TTL laser attached to the fan
 * header (as with some add-on laser kits). Enable this option to set fan/laser
 * speeds with much more exact timing for improved print fidelity.
 *
 * NOTE: This option sacrifices some cooling fan speed options.
 */
//#define LASER_SYNCHRONOUS_M106_M107

/**
 * Coolant Control
 *
 * Add the M7, M8, and M9 commands to turn mist or flood coolant on and off.
 *
 * Note: COOLANT_MIST_PIN and/or COOLANT_FLOOD_PIN must also be defined.
 */
//#define COOLANT_CONTROL
#if ENABLED(COOLANT_CONTROL)
  #define COOLANT_MIST                // Enable if mist coolant is present
  #define COOLANT_FLOOD               // Enable if flood coolant is present
  #define COOLANT_MIST_INVERT  false  // Set "true" if the on/off function is reversed
  #define COOLANT_FLOOD_INVERT false  // Set "true" if the on/off function is reversed
#endif
