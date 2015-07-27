/**
 * SanityCheck.h
 *
 * Test configuration values for errors at compile-time.
 */
#ifndef SANITYCHECK_H
  #define SANITYCHECK_H

  /**
   * Dual Stepper Drivers
   */
  #if defined(Z_DUAL_STEPPER_DRIVERS) && defined(Y_DUAL_STEPPER_DRIVERS)
    #error You cannot have dual stepper drivers for both Y and Z.
  #endif

  /**
   * Progress Bar
   */
  #ifdef LCD_PROGRESS_BAR
    #ifndef SDSUPPORT
      #error LCD_PROGRESS_BAR requires SDSUPPORT.
    #endif
    #ifdef DOGLCD
      #error LCD_PROGRESS_BAR does not apply to graphical displays.
    #endif
    #ifdef FILAMENT_LCD_DISPLAY
      #error LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both.
    #endif
  #endif

  /**
   * Babystepping
   */
  #ifdef BABYSTEPPING
    #ifdef COREXY
      #error BABYSTEPPING not implemented for COREXY yet.
    #endif
    #ifdef SCARA
      #error BABYSTEPPING is not implemented for SCARA yet.
    #endif
    #if defined(DELTA) && defined(BABYSTEP_XY)
      #error BABYSTEPPING only implemented for Z axis on deltabots.
    #endif
  #endif

  /**
   * Filament Change with Extruder Runout Prevention
   */
  #if defined(FILAMENTCHANGEENABLE) && defined(EXTRUDER_RUNOUT_PREVENT)
    #error EXTRUDER_RUNOUT_PREVENT currently incompatible with FILAMENTCHANGE.
  #endif

  /**
   * Options only for EXTRUDERS > 1
   */
  #if EXTRUDERS > 1

    #if EXTRUDERS > 4
      #error The maximum number of EXTRUDERS in Marlin is 4.
    #endif

    #ifdef TEMP_SENSOR_1_AS_REDUNDANT
      #error EXTRUDERS must be 1 with TEMP_SENSOR_1_AS_REDUNDANT.
    #endif

    #ifdef HEATERS_PARALLEL
      #error EXTRUDERS must be 1 with HEATERS_PARALLEL.
    #endif

    #ifdef Y_DUAL_STEPPER_DRIVERS
      #error EXTRUDERS must be 1 with Y_DUAL_STEPPER_DRIVERS.
    #endif

    #ifdef Z_DUAL_STEPPER_DRIVERS
      #error EXTRUDERS must be 1 with Z_DUAL_STEPPER_DRIVERS.
    #endif

  #endif // EXTRUDERS > 1

  /**
   * Limited number of servos
   */
  #if NUM_SERVOS > 4
    #error The maximum number of SERVOS in Marlin is 4.
  #endif
  #if defined(NUM_SERVOS) && NUM_SERVOS > 0
    #if X_ENDSTOP_SERVO_NR >= 0 || Y_ENDSTOP_SERVO_NR >= 0 || Z_ENDSTOP_SERVO_NR >= 0
      #if X_ENDSTOP_SERVO_NR >= NUM_SERVOS
        #error X_ENDSTOP_SERVO_NR must be smaller than NUM_SERVOS.
      #elif Y_ENDSTOP_SERVO_NR >= NUM_SERVOS
        #error Y_ENDSTOP_SERVO_NR must be smaller than NUM_SERVOS.
      #elif Z_ENDSTOP_SERVO_NR >= NUM_SERVOS
        #error Z_ENDSTOP_SERVO_NR must be smaller than NUM_SERVOS.
      #endif
    #endif
  #endif

  /**
   * Servo deactivation depends on servo endstops
   */
  #if defined(DEACTIVATE_SERVOS_AFTER_MOVE) && !defined(SERVO_ENDSTOPS)
    #error At least one of the ?_ENDSTOP_SERVO_NR is required for DEACTIVATE_SERVOS_AFTER_MOVE.
  #endif

  /**
   * Required LCD language
   */
  #if !defined(DOGLCD) && defined(ULTRA_LCD) && !defined(DISPLAY_CHARSET_HD44780_JAPAN) && !defined(DISPLAY_CHARSET_HD44780_WESTERN)&& !defined(DISPLAY_CHARSET_HD44780_CYRILLIC)
    #error You must enable either DISPLAY_CHARSET_HD44780_JAPAN or DISPLAY_CHARSET_HD44780_WESTERN  or DISPLAY_CHARSET_HD44780_CYRILLIC for your LCD controller.
  #endif

  /**
   * Mesh Bed Leveling
   */
  #ifdef MESH_BED_LEVELING
    #ifdef DELTA
      #error MESH_BED_LEVELING does not yet support DELTA printers.
    #endif
    #ifdef ENABLE_AUTO_BED_LEVELING
      #error Select ENABLE_AUTO_BED_LEVELING or MESH_BED_LEVELING, not both.
    #endif
    #if MESH_NUM_X_POINTS > 7 || MESH_NUM_Y_POINTS > 7
      #error MESH_NUM_X_POINTS and MESH_NUM_Y_POINTS need to be less than 8.
    #endif
  #endif

  /**
   * Auto Bed Leveling
   */
  #ifdef ENABLE_AUTO_BED_LEVELING

    /**
     * Require a Z Min pin
     */
    #if Z_MIN_PIN == -1
      #if Z_PROBE_PIN == -1 || (!defined(Z_PROBE_ENDSTOP) || defined(DISABLE_Z_PROBE_ENDSTOP)) // It's possible for someone to set a pin for the Z Probe, but not enable it.
        #ifdef Z_PROBE_REPEATABILITY_TEST
          #error You must have a Z_MIN or Z_PROBE endstop to enable Z_PROBE_REPEATABILITY_TEST.
        #else
          #error ENABLE_AUTO_BED_LEVELING requires a Z_MIN or Z_PROBE endstop. Z_MIN_PIN or Z_PROBE_PIN must point to a valid hardware pin.
        #endif
      #endif
    #endif

    /**
     * Require a Z Probe Pin if Z_PROBE_ENDSTOP is enabled.
     */
    #if defined(Z_PROBE_ENDSTOP)
      #ifndef Z_PROBE_PIN
        #error You must have a Z_PROBE_PIN defined in your pins_XXXX.h file if you enable Z_PROBE_ENDSTOP.
      #endif
      #if Z_PROBE_PIN == -1
        #error You must set Z_PROBE_PIN to a valid pin if you enable Z_PROBE_ENDSTOP.
      #endif
// Forcing Servo definitions can break some hall effect sensor setups. Leaving these here for further comment.
//      #ifndef NUM_SERVOS
//        #error You must have NUM_SERVOS defined and there must be at least 1 configured to use Z_PROBE_ENDSTOP.
//      #endif
//      #if defined(NUM_SERVOS) && NUM_SERVOS < 1
//        #error You must have at least 1 servo defined for NUM_SERVOS to use Z_PROBE_ENDSTOP.
//      #endif
//      #if Z_ENDSTOP_SERVO_NR < 0
//        #error You must have Z_ENDSTOP_SERVO_NR set to at least 0 or above to use Z_PROBE_ENDSTOP.
//      #endif
//      #ifndef SERVO_ENDSTOP_ANGLES
//        #error You must have SERVO_ENDSTOP_ANGLES defined for Z Extend and Retract to use Z_PROBE_ENDSTOP.
//      #endif
    #endif
    /**
     * Check if Probe_Offset * Grid Points is greater than Probing Range
     */
    #ifdef AUTO_BED_LEVELING_GRID
      #ifndef DELTA_PROBABLE_RADIUS
        // Be sure points are in the right order
        #if LEFT_PROBE_BED_POSITION > RIGHT_PROBE_BED_POSITION
          #error LEFT_PROBE_BED_POSITION must be less than RIGHT_PROBE_BED_POSITION.
        #elif FRONT_PROBE_BED_POSITION > BACK_PROBE_BED_POSITION
          #error BACK_PROBE_BED_POSITION must be less than FRONT_PROBE_BED_POSITION.
        #endif
        // Make sure probing points are reachable
        #if LEFT_PROBE_BED_POSITION < MIN_PROBE_X
          #error "The given LEFT_PROBE_BED_POSITION can't be reached by the probe."
        #elif RIGHT_PROBE_BED_POSITION > MAX_PROBE_X
          #error "The given RIGHT_PROBE_BED_POSITION can't be reached by the probe."
        #elif FRONT_PROBE_BED_POSITION < MIN_PROBE_Y
          #error "The given FRONT_PROBE_BED_POSITION can't be reached by the probe."
        #elif BACK_PROBE_BED_POSITION > MAX_PROBE_Y
          #error "The given BACK_PROBE_BED_POSITION can't be reached by the probe."
        #endif
      #endif
    #else // !AUTO_BED_LEVELING_GRID

      // Check the triangulation points
      #if ABL_PROBE_PT_1_X < MIN_PROBE_X || ABL_PROBE_PT_1_X > MAX_PROBE_X
        #error "The given ABL_PROBE_PT_1_X can't be reached by the probe."
      #elif ABL_PROBE_PT_2_X < MIN_PROBE_X || ABL_PROBE_PT_2_X > MAX_PROBE_X
        #error "The given ABL_PROBE_PT_2_X can't be reached by the probe."
      #elif ABL_PROBE_PT_3_X < MIN_PROBE_X || ABL_PROBE_PT_3_X > MAX_PROBE_X
        #error "The given ABL_PROBE_PT_3_X can't be reached by the probe."
      #elif ABL_PROBE_PT_1_Y < MIN_PROBE_Y || ABL_PROBE_PT_1_Y > MAX_PROBE_Y
        #error "The given ABL_PROBE_PT_1_Y can't be reached by the probe."
      #elif ABL_PROBE_PT_2_Y < MIN_PROBE_Y || ABL_PROBE_PT_2_Y > MAX_PROBE_Y
        #error "The given ABL_PROBE_PT_2_Y can't be reached by the probe."
      #elif ABL_PROBE_PT_3_Y < MIN_PROBE_Y || ABL_PROBE_PT_3_Y > MAX_PROBE_Y
        #error "The given ABL_PROBE_PT_3_Y can't be reached by the probe."
      #endif

    #endif // !AUTO_BED_LEVELING_GRID

  #endif // ENABLE_AUTO_BED_LEVELING

  /**
   * ULTIPANEL encoder
   */
  #if defined(ULTIPANEL) && !defined(NEWPANEL) && !defined(SR_LCD_2W_NL) && !defined(SHIFT_CLK)
    #error ULTIPANEL requires some kind of encoder.
  #endif

  /**
   * Delta has limited bed leveling options
   */
  #ifdef DELTA

    #ifdef ENABLE_AUTO_BED_LEVELING

      #ifndef AUTO_BED_LEVELING_GRID
        #error Only AUTO_BED_LEVELING_GRID is supported with DELTA.
      #endif

      #ifdef Z_PROBE_SLED
        #error You cannot use Z_PROBE_SLED with DELTA.
      #endif

      #ifdef Z_PROBE_REPEATABILITY_TEST
        #error Z_PROBE_REPEATABILITY_TEST is not supported with DELTA yet.
      #endif

    #endif

  #endif

  /**
   * Allen Key Z Probe requires Auto Bed Leveling grid and Delta
   */
  #if defined(Z_PROBE_ALLEN_KEY) && !(defined(AUTO_BED_LEVELING_GRID) && defined(DELTA))
    #error Invalid use of Z_PROBE_ALLEN_KEY.
  #endif

  /**
   * Dual X Carriage requirements
   */
  #ifdef DUAL_X_CARRIAGE
    #if EXTRUDERS == 1 || defined(COREXY) \
        || !HAS_X2_ENABLE || !HAS_X2_STEP || !HAS_X2_DIR \
        || !defined(X2_HOME_POS) || !defined(X2_MIN_POS) || !defined(X2_MAX_POS) \
        || !HAS_X_MAX
      #error Missing or invalid definitions for DUAL_X_CARRIAGE mode.
    #endif
    #if X_HOME_DIR != -1 || X2_HOME_DIR != 1
      #error Please use canonical x-carriage assignment.
    #endif
  #endif // DUAL_X_CARRIAGE

  /**
   * Make sure auto fan pins don't conflict with the fan pin
   */
  #if HAS_AUTO_FAN && HAS_FAN
    #if EXTRUDER_0_AUTO_FAN_PIN == FAN_PIN
      #error You cannot set EXTRUDER_0_AUTO_FAN_PIN equal to FAN_PIN.
    #elif EXTRUDER_1_AUTO_FAN_PIN == FAN_PIN
      #error You cannot set EXTRUDER_1_AUTO_FAN_PIN equal to FAN_PIN.
    #elif EXTRUDER_2_AUTO_FAN_PIN == FAN_PIN
      #error You cannot set EXTRUDER_2_AUTO_FAN_PIN equal to FAN_PIN.
    #elif EXTRUDER_3_AUTO_FAN_PIN == FAN_PIN
      #error You cannot set EXTRUDER_3_AUTO_FAN_PIN equal to FAN_PIN.
    #endif
  #endif

  #if HAS_FAN && CONTROLLERFAN_PIN == FAN_PIN
    #error You cannot set CONTROLLERFAN_PIN equal to FAN_PIN.
  #endif

  /**
   * Test Heater, Temp Sensor, and Extruder Pins; Sensor Type must also be set.
   */
  #if EXTRUDERS > 3
    #if !HAS_HEATER_3
      #error HEATER_3_PIN not defined for this board.
    #elif !PIN_EXISTS(TEMP_3)
      #error TEMP_3_PIN not defined for this board.
    #elif !PIN_EXISTS(E3_STEP) || !PIN_EXISTS(E3_DIR) || !PIN_EXISTS(E3_ENABLE)
      #error E3_STEP_PIN, E3_DIR_PIN, or E3_ENABLE_PIN not defined for this board.
    #elif TEMP_SENSOR_3 == 0
      #error TEMP_SENSOR_3 is required with 4 EXTRUDERS.
    #endif
  #elif EXTRUDERS > 2
    #if !HAS_HEATER_2
      #error HEATER_2_PIN not defined for this board.
    #elif !PIN_EXISTS(TEMP_2)
      #error TEMP_2_PIN not defined for this board.
    #elif !PIN_EXISTS(E2_STEP) || !PIN_EXISTS(E2_DIR) || !PIN_EXISTS(E2_ENABLE)
      #error E2_STEP_PIN, E2_DIR_PIN, or E2_ENABLE_PIN not defined for this board.
    #elif TEMP_SENSOR_2 == 0
      #error TEMP_SENSOR_2 is required with 3 or more EXTRUDERS.
    #endif
  #elif EXTRUDERS > 1
    #if !PIN_EXISTS(TEMP_1)
      #error TEMP_1_PIN not defined for this board.
    #elif !PIN_EXISTS(E1_STEP) || !PIN_EXISTS(E1_DIR) || !PIN_EXISTS(E1_ENABLE)
      #error E1_STEP_PIN, E1_DIR_PIN, or E1_ENABLE_PIN not defined for this board.
    #endif
  #endif

  #if EXTRUDERS > 1 || defined(HEATERS_PARALLEL)
    #if !HAS_HEATER_1
      #error HEATER_1_PIN not defined for this board.
    #endif
  #endif

  #if TEMP_SENSOR_1 == 0
    #if EXTRUDERS > 1
      #error TEMP_SENSOR_1 is required with 2 or more EXTRUDERS.
    #elif ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      #error TEMP_SENSOR_1 is required with TEMP_SENSOR_1_AS_REDUNDANT.
    #endif
  #endif

  #if !HAS_HEATER_0
    #error HEATER_0_PIN not defined for this board.
  #elif !PIN_EXISTS(TEMP_0)
    #error TEMP_0_PIN not defined for this board.
  #elif !PIN_EXISTS(E0_STEP) || !PIN_EXISTS(E0_DIR) || !PIN_EXISTS(E0_ENABLE)
    #error E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board.
  #elif TEMP_SENSOR_0 == 0
    #error TEMP_SENSOR_0 is required.
  #endif

  /**
   * Warnings for old configurations
   */
  #ifdef X_HOME_RETRACT_MM
    #error [XYZ]_HOME_RETRACT_MM settings have been renamed [XYZ]_HOME_BUMP_MM.
  #endif

  #if WATCH_TEMP_PERIOD > 500
    #error WATCH_TEMP_PERIOD now uses seconds instead of milliseconds.
  #endif

  #if !defined(THERMAL_PROTECTION_HOTENDS) && (defined(WATCH_TEMP_PERIOD) || defined(THERMAL_PROTECTION_PERIOD))
    #error Thermal Runaway Protection for hotends must now be enabled with THERMAL_PROTECTION_HOTENDS.
  #endif

  #if !defined(THERMAL_PROTECTION_BED) && defined(THERMAL_PROTECTION_BED_PERIOD)
    #error Thermal Runaway Protection for the bed must now be enabled with THERMAL_PROTECTION_BED.
  #endif

  #ifdef PROBE_SERVO_DEACTIVATION_DELAY
    #error PROBE_SERVO_DEACTIVATION_DELAY has been replaced with DEACTIVATE_SERVOS_AFTER_MOVE and SERVO_DEACTIVATION_DELAY.
  #endif

  #if defined(COREXZ) && defined(Z_LATE_ENABLE)
    #error "Z_LATE_ENABLE can't be used with COREXZ."
  #endif

#endif //SANITYCHECK_H
