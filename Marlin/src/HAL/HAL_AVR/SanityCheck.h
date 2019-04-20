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
#pragma once

/**
 * Test AVR specific configuration values for errors at compile-time.
 */

/**
 * Digipot requirement
 */
 #if ENABLED(DIGIPOT_MCP4018)
  #if !defined(DIGIPOTS_I2C_SDA_X) || !defined(DIGIPOTS_I2C_SDA_Y) || !defined(DIGIPOTS_I2C_SDA_Z) \
    || !defined(DIGIPOTS_I2C_SDA_E0) || !defined(DIGIPOTS_I2C_SDA_E1)
      #error "DIGIPOT_MCP4018 requires DIGIPOTS_I2C_SDA_* pins to be defined."
  #endif
#endif

/**
 * Checks for FAST PWM
 */
#if ENABLED(FAST_PWM_FAN) && (ENABLED(USE_OCR2A_AS_TOP) && defined(TCCR2))
    #error "USE_OCR2A_AS_TOP does not apply to devices with a single output TIMER2"
#endif

/**
 * Sanity checks for Spindle / Laser
 */
#if ENABLED(SPINDLE_LASER_ENABLE)
  #if !PIN_EXISTS(SPINDLE_LASER_ENA)
    #error "SPINDLE_LASER_ENABLE requires SPINDLE_LASER_ENA_PIN."
  #elif SPINDLE_DIR_CHANGE && !PIN_EXISTS(SPINDLE_DIR)
    #error "SPINDLE_DIR_PIN not defined."
  #elif ENABLED(SPINDLE_LASER_PWM) && PIN_EXISTS(SPINDLE_LASER_PWM)
    #if !(WITHIN(SPINDLE_LASER_PWM_PIN, 2, 13) || WITHIN(SPINDLE_LASER_PWM_PIN, 44, 46))
      #error "SPINDLE_LASER_PWM_PIN not assigned to a PWM pin."
    #elif SPINDLE_LASER_POWERUP_DELAY < 1
      #error "SPINDLE_LASER_POWERUP_DELAY must be greater than 0."
    #elif SPINDLE_LASER_POWERDOWN_DELAY < 1
      #error "SPINDLE_LASER_POWERDOWN_DELAY must be greater than 0."
    #elif !defined(SPINDLE_LASER_PWM_INVERT)
      #error "SPINDLE_LASER_PWM_INVERT missing."
    #elif !defined(SPEED_POWER_SLOPE) || !defined(SPEED_POWER_INTERCEPT) || !defined(SPEED_POWER_MIN) || !defined(SPEED_POWER_MAX)
      #error "SPINDLE_LASER_PWM equation constant(s) missing."
    #elif SPINDLE_LASER_PWM_PIN == 4 || WITHIN(SPINDLE_LASER_PWM_PIN, 11, 13)
      #error "Counter/Timer for SPINDLE_LASER_PWM_PIN is used by a system interrupt."
    #elif PIN_EXISTS(X_MAX) && X_MAX_PIN == SPINDLE_LASER_PWM_PIN
      #error "SPINDLE_LASER_PWM pin is in use by X_MAX endstop."
    #elif PIN_EXISTS(X_MIN) && X_MIN_PIN == SPINDLE_LASER_PWM_PIN
      #error "SPINDLE_LASER_PWM pin is in use by X_MIN endstop."
    #elif PIN_EXISTS(Z_STEP) && Z_STEP_PIN == SPINDLE_LASER_PWM_PIN
      #error "SPINDLE_LASER_PWM pin in use by Z_STEP."
    #elif NUM_SERVOS > 0 && (WITHIN(SPINDLE_LASER_PWM_PIN, 2, 3) || SPINDLE_LASER_PWM_PIN == 5)
      #error "Counter/Timer for SPINDLE_LASER_PWM_PIN is used by the servo system."
    #elif PIN_EXISTS(CASE_LIGHT) && SPINDLE_LASER_PWM_PIN == CASE_LIGHT_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by CASE_LIGHT_PIN."
    #elif PIN_EXISTS(E0_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E0_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E0_AUTO_FAN_PIN."
    #elif PIN_EXISTS(E1_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E1_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E1_AUTO_FAN_PIN."
    #elif PIN_EXISTS(E2_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E2_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E2_AUTO_FAN_PIN."
    #elif PIN_EXISTS(E3_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E3_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E3_AUTO_FAN_PIN."
    #elif PIN_EXISTS(E4_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E4_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E4_AUTO_FAN_PIN."
    #elif PIN_EXISTS(E5_AUTO_FAN) && SPINDLE_LASER_PWM_PIN == E5_AUTO_FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by E5_AUTO_FAN_PIN."
    #elif PIN_EXISTS(FAN) && SPINDLE_LASER_PWM_PIN == FAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by FAN_PIN."
    #elif PIN_EXISTS(FAN1) && SPINDLE_LASER_PWM_PIN == FAN1_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by FAN1_PIN."
    #elif PIN_EXISTS(FAN2) && SPINDLE_LASER_PWM_PIN == FAN2_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by FAN2_PIN."
    #elif PIN_EXISTS(CONTROLLERFAN) && SPINDLE_LASER_PWM_PIN == CONTROLLERFAN_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by CONTROLLERFAN_PIN."
    #elif PIN_EXISTS(MOTOR_CURRENT_PWM_XY) && SPINDLE_LASER_PWM_PIN == MOTOR_CURRENT_PWM_XY_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by MOTOR_CURRENT_PWM_XY."
    #elif PIN_EXISTS(MOTOR_CURRENT_PWM_Z) && SPINDLE_LASER_PWM_PIN == MOTOR_CURRENT_PWM_Z_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by MOTOR_CURRENT_PWM_Z."
    #elif PIN_EXISTS(MOTOR_CURRENT_PWM_E) && SPINDLE_LASER_PWM_PIN == MOTOR_CURRENT_PWM_E_PIN
      #error "SPINDLE_LASER_PWM_PIN is used by MOTOR_CURRENT_PWM_E."
    #endif
  #endif
#endif // SPINDLE_LASER_ENABLE

/**
 * The Trinamic library includes SoftwareSerial.h, leading to a compile error.
 */
#if HAS_TRINAMIC && ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #error "TMCStepper includes SoftwareSerial.h which is incompatible with ENDSTOP_INTERRUPTS_FEATURE. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
#endif
