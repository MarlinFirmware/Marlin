/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * endstops.cpp - A singleton object to manage endstops
 */

#include "endstops.h"
#include "stepper.h"

#include "../Marlin.h"
#include "../sd/cardreader.h"
#include "../module/temperature.h"
#include "../lcd/ultralcd.h"

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #include HAL_PATH(../HAL, endstop_interrupts.h)
#endif

#if HAS_BED_PROBE
  #define ENDSTOPS_ENABLED  (endstops.enabled || endstops.z_probe_enabled)
#else
  #define ENDSTOPS_ENABLED  endstops.enabled
#endif

Endstops endstops;

// public:

bool Endstops::enabled, Endstops::enabled_globally; // Initialized by settings.load()
volatile uint8_t Endstops::hit_state;

Endstops::esbits_t Endstops::live_state = 0;
#if ENABLED(ENDSTOP_NOISE_FILTER)
  Endstops::esbits_t Endstops::old_live_state,
                     Endstops::validated_live_state;
  uint8_t Endstops::endstop_poll_count;
#endif

#if HAS_BED_PROBE
  volatile bool Endstops::z_probe_enabled = false;
#endif

// Initialized by settings.load()
#if ENABLED(X_DUAL_ENDSTOPS)
  float Endstops::x_endstop_adj;
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  float Endstops::y_endstop_adj;
#endif
#if ENABLED(Z_DUAL_ENDSTOPS)
  float Endstops::z_endstop_adj;
#endif

/**
 * Class and Instance Methods
 */

void Endstops::init() {

  #if HAS_X_MIN
    #if ENABLED(ENDSTOPPULLUP_XMIN)
      SET_INPUT_PULLUP(X_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_XMIN)
      SET_INPUT_PULLDOWN(X_MIN_PIN);
    #else
      SET_INPUT(X_MIN_PIN);
    #endif
  #endif

  #if HAS_X2_MIN
    #if ENABLED(ENDSTOPPULLUP_XMIN)
      SET_INPUT_PULLUP(X2_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_XMIN)
      SET_INPUT_PULLDOWN(X2_MIN_PIN);
    #else
      SET_INPUT(X2_MIN_PIN);
    #endif
  #endif

  #if HAS_Y_MIN
    #if ENABLED(ENDSTOPPULLUP_YMIN)
      SET_INPUT_PULLUP(Y_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_YMIN)
      SET_INPUT_PULLDOWN(Y_MIN_PIN);
    #else
      SET_INPUT(Y_MIN_PIN);
    #endif
  #endif

  #if HAS_Y2_MIN
    #if ENABLED(ENDSTOPPULLUP_YMIN)
      SET_INPUT_PULLUP(Y2_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_YMIN)
      SET_INPUT_PULLDOWN(Y2_MIN_PIN);
    #else
      SET_INPUT(Y2_MIN_PIN);
    #endif
  #endif

  #if HAS_Z_MIN
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      SET_INPUT_PULLUP(Z_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN)
      SET_INPUT_PULLDOWN(Z_MIN_PIN);
    #else
      SET_INPUT(Z_MIN_PIN);
    #endif
  #endif

  #if HAS_Z2_MIN
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      SET_INPUT_PULLUP(Z2_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN)
      SET_INPUT_PULLDOWN(Z2_MIN_PIN);
    #else
      SET_INPUT(Z2_MIN_PIN);
    #endif
  #endif

  #if HAS_X_MAX
    #if ENABLED(ENDSTOPPULLUP_XMAX)
      SET_INPUT_PULLUP(X_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_XMAX)
      SET_INPUT_PULLDOWN(X_MAX_PIN);
    #else
      SET_INPUT(X_MAX_PIN);
    #endif
  #endif

  #if HAS_X2_MAX
    #if ENABLED(ENDSTOPPULLUP_XMAX)
      SET_INPUT_PULLUP(X2_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_XMAX)
      SET_INPUT_PULLDOWN(X2_MAX_PIN);
    #else
      SET_INPUT(X2_MAX_PIN);
    #endif
  #endif

  #if HAS_Y_MAX
    #if ENABLED(ENDSTOPPULLUP_YMAX)
      SET_INPUT_PULLUP(Y_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_YMAX)
      SET_INPUT_PULLDOWN(Y_MAX_PIN);
    #else
      SET_INPUT(Y_MAX_PIN);
    #endif
  #endif

  #if HAS_Y2_MAX
    #if ENABLED(ENDSTOPPULLUP_YMAX)
      SET_INPUT_PULLUP(Y2_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_YMAX)
      SET_INPUT_PULLDOWN(Y2_MAX_PIN);
    #else
      SET_INPUT(Y2_MAX_PIN);
    #endif
  #endif

  #if HAS_Z_MAX
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      SET_INPUT_PULLUP(Z_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMAX)
      SET_INPUT_PULLDOWN(Z_MAX_PIN);
    #else
      SET_INPUT(Z_MAX_PIN);
    #endif
  #endif

  #if HAS_Z2_MAX
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      SET_INPUT_PULLUP(Z2_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMAX)
      SET_INPUT_PULLDOWN(Z2_MAX_PIN);
    #else
      SET_INPUT(Z2_MAX_PIN);
    #endif
  #endif

  #if ENABLED(Z_MIN_PROBE_ENDSTOP)
    #if ENABLED(ENDSTOPPULLUP_ZMIN_PROBE)
      SET_INPUT_PULLUP(Z_MIN_PROBE_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN_PROBE)
      SET_INPUT_PULLDOWN(Z_MIN_PROBE_PIN);
    #else
      SET_INPUT(Z_MIN_PROBE_PIN);
    #endif
  #endif

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    setup_endstop_interrupts();
  #endif

  // Enable endstops
  enable_globally(
    #if ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
      true
    #else
      false
    #endif
  );

} // Endstops::init

// Called from ISR. A change was detected. Find out what happened!
void Endstops::check_possible_change() { if (ENDSTOPS_ENABLED) endstops.update(); }

// Called from ISR: Poll endstop state if required
void Endstops::poll() {

  #if ENABLED(PINS_DEBUGGING)
    endstops.run_monitor();  // report changes in endstop status
  #endif

  #if DISABLED(ENDSTOP_INTERRUPTS_FEATURE) || ENABLED(ENDSTOP_NOISE_FILTER)
    if (ENDSTOPS_ENABLED) endstops.update();
  #endif
}

void Endstops::enable_globally(const bool onoff) {
  enabled_globally = enabled = onoff;

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (onoff) endstops.update(); // If enabling, update state now
  #endif
}

// Enable / disable endstop checking
void Endstops::enable(const bool onoff) {
  enabled = onoff;

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (onoff) endstops.update(); // If enabling, update state now
  #endif
}


// Disable / Enable endstops based on ENSTOPS_ONLY_FOR_HOMING and global enable
void Endstops::not_homing() {
  enabled = enabled_globally;

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (enabled) endstops.update(); // If enabling, update state now
  #endif
}

// Clear endstops (i.e., they were hit intentionally) to suppress the report
void Endstops::hit_on_purpose() {
  hit_state = 0;

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (enabled) endstops.update(); // If enabling, update state now
  #endif
}

// Enable / disable endstop z-probe checking
#if HAS_BED_PROBE
  void Endstops::enable_z_probe(bool onoff) {
    z_probe_enabled = onoff;

    #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
      if (enabled) endstops.update(); // If enabling, update state now
    #endif
  }
#endif

#if ENABLED(PINS_DEBUGGING)
  void Endstops::run_monitor() {
    if (!monitor_flag) return;
    static uint8_t monitor_count = 16;  // offset this check from the others
    monitor_count += _BV(1);            //  15 Hz
    monitor_count &= 0x7F;
    if (!monitor_count) monitor();      // report changes in endstop status
  }
#endif

void Endstops::report_state() {
  if (hit_state) {
    #if ENABLED(ULTRA_LCD)
      char chrX = ' ', chrY = ' ', chrZ = ' ', chrP = ' ';
      #define _SET_STOP_CHAR(A,C) (chr## A = C)
    #else
      #define _SET_STOP_CHAR(A,C) ;
    #endif

    #define _ENDSTOP_HIT_ECHO(A,C) do{ \
      SERIAL_ECHOPAIR(" " STRINGIFY(A) ":", planner.triggered_position_mm(_AXIS(A))); \
      _SET_STOP_CHAR(A,C); }while(0)

    #define _ENDSTOP_HIT_TEST(A,C) \
      if (TEST(hit_state, A ##_MIN) || TEST(hit_state, A ##_MAX)) \
        _ENDSTOP_HIT_ECHO(A,C)

    #define ENDSTOP_HIT_TEST_X() _ENDSTOP_HIT_TEST(X,'X')
    #define ENDSTOP_HIT_TEST_Y() _ENDSTOP_HIT_TEST(Y,'Y')
    #define ENDSTOP_HIT_TEST_Z() _ENDSTOP_HIT_TEST(Z,'Z')

    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_ENDSTOPS_HIT);
    ENDSTOP_HIT_TEST_X();
    ENDSTOP_HIT_TEST_Y();
    ENDSTOP_HIT_TEST_Z();

    #if ENABLED(Z_MIN_PROBE_ENDSTOP)
      #define P_AXIS Z_AXIS
      if (TEST(hit_state, Z_MIN_PROBE)) _ENDSTOP_HIT_ECHO(P, 'P');
    #endif
    SERIAL_EOL();

    #if ENABLED(ULTRA_LCD)
      lcd_status_printf_P(0, PSTR(MSG_LCD_ENDSTOPS " %c %c %c %c"), chrX, chrY, chrZ, chrP);
    #endif

    hit_on_purpose();

    #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED) && ENABLED(SDSUPPORT)
      if (planner.abort_on_endstop_hit) {
        card.sdprinting = false;
        card.closefile();
        quickstop_stepper();
        thermalManager.disable_all_heaters(); // switch off all heaters.
      }
    #endif
  }
} // Endstops::report_state

void Endstops::M119() {
  SERIAL_PROTOCOLLNPGM(MSG_M119_REPORT);
  #define ES_REPORT(AXIS) do{ \
    SERIAL_PROTOCOLPGM(MSG_##AXIS); \
    SERIAL_PROTOCOLLN(((READ(AXIS##_PIN)^AXIS##_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN)); \
  }while(0)
  #if HAS_X_MIN
    ES_REPORT(X_MIN);
  #endif
  #if HAS_X2_MIN
    ES_REPORT(X2_MIN);
  #endif
  #if HAS_X_MAX
    ES_REPORT(X_MAX);
  #endif
  #if HAS_X2_MAX
    ES_REPORT(X2_MAX);
  #endif
  #if HAS_Y_MIN
    ES_REPORT(Y_MIN);
  #endif
  #if HAS_Y2_MIN
    ES_REPORT(Y2_MIN);
  #endif
  #if HAS_Y_MAX
    ES_REPORT(Y_MAX);
  #endif
  #if HAS_Y2_MAX
    ES_REPORT(Y2_MAX);
  #endif
  #if HAS_Z_MIN
    ES_REPORT(Z_MIN);
  #endif
  #if HAS_Z2_MIN
    ES_REPORT(Z2_MIN);
  #endif
  #if HAS_Z_MAX
    ES_REPORT(Z_MAX);
  #endif
  #if HAS_Z2_MAX
    ES_REPORT(Z2_MAX);
  #endif
  #if ENABLED(Z_MIN_PROBE_ENDSTOP)
    SERIAL_PROTOCOLPGM(MSG_Z_PROBE);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PROBE_PIN)^Z_MIN_PROBE_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    SERIAL_PROTOCOLPGM(MSG_FILAMENT_RUNOUT_SENSOR);
    SERIAL_PROTOCOLLN(((READ(FIL_RUNOUT_PIN)^FIL_RUNOUT_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
} // Endstops::M119

// The following routines are called from an ISR context. It could be the temperature ISR, the
// endstop ISR or the Stepper ISR.

#define _ENDSTOP(AXIS, MINMAX) AXIS ##_## MINMAX
#define _ENDSTOP_PIN(AXIS, MINMAX) AXIS ##_## MINMAX ##_PIN
#define _ENDSTOP_INVERTING(AXIS, MINMAX) AXIS ##_## MINMAX ##_ENDSTOP_INVERTING

// Check endstops - Could be called from ISR!
void Endstops::update() {

  #define SET_BIT(N,B,TF) do{ if (TF) SBI(N,B); else CBI(N,B); }while(0)
  // UPDATE_ENDSTOP_BIT: set the current endstop bits for an endstop to its status
  #define UPDATE_ENDSTOP_BIT(AXIS, MINMAX) SET_BIT(live_state, _ENDSTOP(AXIS, MINMAX), (READ(_ENDSTOP_PIN(AXIS, MINMAX)) != _ENDSTOP_INVERTING(AXIS, MINMAX)))
  // COPY_BIT: copy the value of SRC_BIT to DST_BIT in DST
  #define COPY_BIT(DST, SRC_BIT, DST_BIT) SET_BIT(DST, DST_BIT, TEST(DST, SRC_BIT))

  #if ENABLED(G38_PROBE_TARGET) && PIN_EXISTS(Z_MIN_PROBE) && !(CORE_IS_XY || CORE_IS_XZ)
    // If G38 command is active check Z_MIN_PROBE for ALL movement
    if (G38_move) UPDATE_ENDSTOP_BIT(Z, MIN_PROBE);
  #endif

  /**
   * Define conditions for checking endstops
   */

  #if IS_CORE
    #define S_(N) stepper.movement_non_null(CORE_AXIS_##N)
    #define D_(N) stepper.motor_direction(CORE_AXIS_##N)
  #endif

  #if CORE_IS_XY || CORE_IS_XZ
    /**
     * Head direction in -X axis for CoreXY and CoreXZ bots.
     *
     * If steps differ, both axes are moving.
     * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Y or Z, handled below)
     * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X)
     */
    #if ENABLED(COREXY) || ENABLED(COREXZ)
      #define X_CMP ==
    #else
      #define X_CMP !=
    #endif
    #define X_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && D_(1) X_CMP D_(2)) )
    #define X_AXIS_HEAD X_HEAD
  #else
    #define X_MOVE_TEST stepper.movement_non_null(X_AXIS)
    #define X_AXIS_HEAD X_AXIS
  #endif

  #if CORE_IS_XY || CORE_IS_YZ
    /**
     * Head direction in -Y axis for CoreXY / CoreYZ bots.
     *
     * If steps differ, both axes are moving
     * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X or Y)
     * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Y or Z)
     */
    #if ENABLED(COREYX) || ENABLED(COREYZ)
      #define Y_CMP ==
    #else
      #define Y_CMP !=
    #endif
    #define Y_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && D_(1) Y_CMP D_(2)) )
    #define Y_AXIS_HEAD Y_HEAD
  #else
    #define Y_MOVE_TEST stepper.movement_non_null(Y_AXIS)
    #define Y_AXIS_HEAD Y_AXIS
  #endif

  #if CORE_IS_XZ || CORE_IS_YZ
    /**
     * Head direction in -Z axis for CoreXZ or CoreYZ bots.
     *
     * If steps differ, both axes are moving
     * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X or Y, already handled above)
     * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Z)
     */
    #if ENABLED(COREZX) || ENABLED(COREZY)
      #define Z_CMP ==
    #else
      #define Z_CMP !=
    #endif
    #define Z_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && D_(1) Z_CMP D_(2)) )
    #define Z_AXIS_HEAD Z_HEAD
  #else
    #define Z_MOVE_TEST stepper.movement_non_null(Z_AXIS)
    #define Z_AXIS_HEAD Z_AXIS
  #endif

  // With Dual X, endstops are only checked in the homing direction for the active extruder
  #if ENABLED(DUAL_X_CARRIAGE)
    #define E0_ACTIVE stepper.movement_extruder() == 0
    #define X_MIN_TEST ((X_HOME_DIR < 0 && E0_ACTIVE) || (X2_HOME_DIR < 0 && !E0_ACTIVE))
    #define X_MAX_TEST ((X_HOME_DIR > 0 && E0_ACTIVE) || (X2_HOME_DIR > 0 && !E0_ACTIVE))
  #else
    #define X_MIN_TEST true
    #define X_MAX_TEST true
  #endif

  /**
   * Check and update endstops according to conditions
   */
  if (X_MOVE_TEST) {
    if (stepper.motor_direction(X_AXIS_HEAD)) { // -direction
      #if HAS_X_MIN
        #if ENABLED(X_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(X, MIN);
          #if HAS_X2_MIN
            UPDATE_ENDSTOP_BIT(X2, MIN);
          #else
            COPY_BIT(live_state, X_MIN, X2_MIN);
          #endif
        #else
          if (X_MIN_TEST) UPDATE_ENDSTOP_BIT(X, MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_X_MAX
        #if ENABLED(X_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(X, MAX);
          #if HAS_X2_MAX
            UPDATE_ENDSTOP_BIT(X2, MAX);
          #else
            COPY_BIT(live_state, X_MAX, X2_MAX);
          #endif
        #else
          if (X_MAX_TEST) UPDATE_ENDSTOP_BIT(X, MAX);
        #endif
      #endif
    }
  }

  if (Y_MOVE_TEST) {
    if (stepper.motor_direction(Y_AXIS_HEAD)) { // -direction
      #if HAS_Y_MIN
        #if ENABLED(Y_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(Y, MIN);
          #if HAS_Y2_MIN
            UPDATE_ENDSTOP_BIT(Y2, MIN);
          #else
            COPY_BIT(live_state, Y_MIN, Y2_MIN);
          #endif
        #else
          UPDATE_ENDSTOP_BIT(Y, MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_Y_MAX
        #if ENABLED(Y_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(Y, MAX);
          #if HAS_Y2_MAX
            UPDATE_ENDSTOP_BIT(Y2, MAX);
          #else
            COPY_BIT(live_state, Y_MAX, Y2_MAX);
          #endif
        #else
          UPDATE_ENDSTOP_BIT(Y, MAX);
        #endif
      #endif
    }
  }

  if (Z_MOVE_TEST) {
    if (stepper.motor_direction(Z_AXIS_HEAD)) { // Z -direction. Gantry down, bed up.
      #if HAS_Z_MIN
        #if ENABLED(Z_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(Z, MIN);
          #if HAS_Z2_MIN
            UPDATE_ENDSTOP_BIT(Z2, MIN);
          #else
            COPY_BIT(live_state, Z_MIN, Z2_MIN);
          #endif
        #else
          #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
            if (z_probe_enabled) UPDATE_ENDSTOP_BIT(Z, MIN);
          #else
            UPDATE_ENDSTOP_BIT(Z, MIN);
          #endif
        #endif
      #endif

      // When closing the gap check the enabled probe
      #if ENABLED(Z_MIN_PROBE_ENDSTOP)
        if (z_probe_enabled) {
          UPDATE_ENDSTOP_BIT(Z, MIN_PROBE);
        }
      #endif
    }
    else { // Z +direction. Gantry up, bed down.
      #if HAS_Z_MAX
        // Check both Z dual endstops
        #if ENABLED(Z_DUAL_ENDSTOPS)
          UPDATE_ENDSTOP_BIT(Z, MAX);
          #if HAS_Z2_MAX
            UPDATE_ENDSTOP_BIT(Z2, MAX);
          #else
            COPY_BIT(live_state, Z_MAX, Z2_MAX);
          #endif
        // If this pin is not hijacked for the bed probe
        // then it belongs to the Z endstop
        #elif DISABLED(Z_MIN_PROBE_ENDSTOP) || Z_MAX_PIN != Z_MIN_PROBE_PIN
          UPDATE_ENDSTOP_BIT(Z, MAX);
        #endif
      #endif
    }
  }

  // All endstops were updated.
  #if ENABLED(ENDSTOP_NOISE_FILTER)
    if (old_live_state != live_state) { // We detected a change. Reinit the timeout
      /**
       * Filtering out noise on endstops requires a delayed decision. Let's assume, due to noise,
       * that 50% of endstop signal samples are good and 50% are bad (assuming normal distribution
       * of random noise). Then the first sample has a 50% chance to be good or bad. The 2nd sample
       * also has a 50% chance to be good or bad. The chances of 2 samples both being bad becomes
       * 50% of 50%, or 25%. That was the previous implementation of Marlin endstop handling. It
       * reduces chances of bad readings in half, at the cost of 1 extra sample period, but chances
       * still exist. The only way to reduce them further is to increase the number of samples.
       * To reduce the chance to 1% (1/128th) requires 7 samples (adding 7ms of delay).
       */
      endstop_poll_count = 7;
      old_live_state = live_state;
    }
    else if (endstop_poll_count && !--endstop_poll_count)
      validated_live_state = live_state;

  #else

    // Lets accept the new endstop values as valid - We assume hardware filtering of lines
    esbits_t validated_live_state = live_state;

  #endif

  // Endstop readings are validated in validated_live_state

  // Test the current status of an endstop
  #define TEST_ENDSTOP(ENDSTOP) (TEST(validated_live_state, ENDSTOP))

  // Record endstop was hit
  #define _ENDSTOP_HIT(AXIS, MINMAX) SBI(hit_state, _ENDSTOP(AXIS, MINMAX))

  // Call the endstop triggered routine for single endstops
  #define PROCESS_ENDSTOP(AXIS,MINMAX) do { \
      if (TEST_ENDSTOP(_ENDSTOP(AXIS, MINMAX))) { \
        _ENDSTOP_HIT(AXIS, MINMAX); \
        planner.endstop_triggered(_AXIS(AXIS)); \
      } \
    }while(0)

  // Call the endstop triggered routine for single endstops
  #define PROCESS_DUAL_ENDSTOP(AXIS1, AXIS2, MINMAX) do { \
      if (TEST_ENDSTOP(_ENDSTOP(AXIS1, MINMAX)) || TEST_ENDSTOP(_ENDSTOP(AXIS2, MINMAX))) { \
        _ENDSTOP_HIT(AXIS1, MINMAX); \
        planner.endstop_triggered(_AXIS(AXIS1)); \
      } \
    }while(0)

  #if ENABLED(G38_PROBE_TARGET) && PIN_EXISTS(Z_MIN_PROBE) && !(CORE_IS_XY || CORE_IS_XZ)
    // If G38 command is active check Z_MIN_PROBE for ALL movement
    if (G38_move) {
      if (TEST_ENDSTOP(_ENDSTOP(Z, MIN_PROBE))) {
        if      (stepper.movement_non_null(_AXIS(X))) { _ENDSTOP_HIT(X, MIN); planner.endstop_triggered(_AXIS(X)); }
        else if (stepper.movement_non_null(_AXIS(Y))) { _ENDSTOP_HIT(Y, MIN); planner.endstop_triggered(_AXIS(Y)); }
        else if (stepper.movement_non_null(_AXIS(Z))) { _ENDSTOP_HIT(Z, MIN); planner.endstop_triggered(_AXIS(Z)); }
        G38_endstop_hit = true;
      }
    }
  #endif

  // Now, we must signal, after validation, if an endstop limit is pressed or not
  if (X_MOVE_TEST) {
    if (stepper.motor_direction(X_AXIS_HEAD)) { // -direction
      #if HAS_X_MIN
        #if ENABLED(X_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(X, X2, MIN);
        #else
          if (X_MIN_TEST) PROCESS_ENDSTOP(X, MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_X_MAX
        #if ENABLED(X_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(X, X2, MAX);
        #else
          if (X_MAX_TEST) PROCESS_ENDSTOP(X, MAX);
        #endif
      #endif
    }
  }

  if (Y_MOVE_TEST) {
    if (stepper.motor_direction(Y_AXIS_HEAD)) { // -direction
      #if HAS_Y_MIN
        #if ENABLED(Y_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(Y, Y2, MIN);
        #else
          PROCESS_ENDSTOP(Y, MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_Y_MAX
        #if ENABLED(Y_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(Y, Y2, MAX);
        #else
          PROCESS_ENDSTOP(Y, MAX);
        #endif
      #endif
    }
  }

  if (Z_MOVE_TEST) {
    if (stepper.motor_direction(Z_AXIS_HEAD)) { // Z -direction. Gantry down, bed up.
      #if HAS_Z_MIN
        #if ENABLED(Z_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(Z, Z2, MIN);
        #else
          #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
            if (z_probe_enabled) PROCESS_ENDSTOP(Z, MIN);
          #else
            PROCESS_ENDSTOP(Z, MIN);
          #endif
        #endif
      #endif

      // When closing the gap check the enabled probe
      #if ENABLED(Z_MIN_PROBE_ENDSTOP)
        if (z_probe_enabled) PROCESS_ENDSTOP(Z, MIN_PROBE);
      #endif
    }
    else { // Z +direction. Gantry up, bed down.
      #if HAS_Z_MAX
        #if ENABLED(Z_DUAL_ENDSTOPS)
          PROCESS_DUAL_ENDSTOP(Z, Z2, MAX);
        #elif DISABLED(Z_MIN_PROBE_ENDSTOP) || Z_MAX_PIN != Z_MIN_PROBE_PIN
          // If this pin is not hijacked for the bed probe
          // then it belongs to the Z endstop
          PROCESS_ENDSTOP(Z, MAX);
        #endif
      #endif
    }
  }
} // Endstops::update()

#if ENABLED(PINS_DEBUGGING)

  bool Endstops::monitor_flag = false;

  /**
   * monitors endstops & Z probe for changes
   *
   * If a change is detected then the LED is toggled and
   * a message is sent out the serial port
   *
   * Yes, we could miss a rapid back & forth change but
   * that won't matter because this is all manual.
   *
   */
  void Endstops::monitor() {

    static uint16_t old_live_state_local = 0;
    static uint8_t local_LED_status = 0;
    uint16_t live_state_local = 0;

    #if HAS_X_MIN
      if (READ(X_MIN_PIN)) SBI(live_state_local, X_MIN);
    #endif
    #if HAS_X_MAX
      if (READ(X_MAX_PIN)) SBI(live_state_local, X_MAX);
    #endif
    #if HAS_Y_MIN
      if (READ(Y_MIN_PIN)) SBI(live_state_local, Y_MIN);
    #endif
    #if HAS_Y_MAX
      if (READ(Y_MAX_PIN)) SBI(live_state_local, Y_MAX);
    #endif
    #if HAS_Z_MIN
      if (READ(Z_MIN_PIN)) SBI(live_state_local, Z_MIN);
    #endif
    #if HAS_Z_MAX
      if (READ(Z_MAX_PIN)) SBI(live_state_local, Z_MAX);
    #endif
    #if HAS_Z_MIN_PROBE_PIN
      if (READ(Z_MIN_PROBE_PIN)) SBI(live_state_local, Z_MIN_PROBE);
    #endif
    #if HAS_X2_MIN
      if (READ(X2_MIN_PIN)) SBI(live_state_local, X2_MIN);
    #endif
    #if HAS_X2_MAX
      if (READ(X2_MAX_PIN)) SBI(live_state_local, X2_MAX);
    #endif
    #if HAS_Y2_MIN
      if (READ(Y2_MIN_PIN)) SBI(live_state_local, Y2_MIN);
    #endif
    #if HAS_Y2_MAX
      if (READ(Y2_MAX_PIN)) SBI(live_state_local, Y2_MAX);
    #endif
    #if HAS_Z2_MIN
      if (READ(Z2_MIN_PIN)) SBI(live_state_local, Z2_MIN);
    #endif
    #if HAS_Z2_MAX
      if (READ(Z2_MAX_PIN)) SBI(live_state_local, Z2_MAX);
    #endif

    uint16_t endstop_change = live_state_local ^ old_live_state_local;

    if (endstop_change) {
      #if HAS_X_MIN
        if (TEST(endstop_change, X_MIN)) SERIAL_PROTOCOLPAIR("  X_MIN:", TEST(live_state_local, X_MIN));
      #endif
      #if HAS_X_MAX
        if (TEST(endstop_change, X_MAX)) SERIAL_PROTOCOLPAIR("  X_MAX:", TEST(live_state_local, X_MAX));
      #endif
      #if HAS_Y_MIN
        if (TEST(endstop_change, Y_MIN)) SERIAL_PROTOCOLPAIR("  Y_MIN:", TEST(live_state_local, Y_MIN));
      #endif
      #if HAS_Y_MAX
        if (TEST(endstop_change, Y_MAX)) SERIAL_PROTOCOLPAIR("  Y_MAX:", TEST(live_state_local, Y_MAX));
      #endif
      #if HAS_Z_MIN
        if (TEST(endstop_change, Z_MIN)) SERIAL_PROTOCOLPAIR("  Z_MIN:", TEST(live_state_local, Z_MIN));
      #endif
      #if HAS_Z_MAX
        if (TEST(endstop_change, Z_MAX)) SERIAL_PROTOCOLPAIR("  Z_MAX:", TEST(live_state_local, Z_MAX));
      #endif
      #if HAS_Z_MIN_PROBE_PIN
        if (TEST(endstop_change, Z_MIN_PROBE)) SERIAL_PROTOCOLPAIR("  PROBE:", TEST(live_state_local, Z_MIN_PROBE));
      #endif
      #if HAS_X2_MIN
        if (TEST(endstop_change, X2_MIN)) SERIAL_PROTOCOLPAIR("  X2_MIN:", TEST(live_state_local, X2_MIN));
      #endif
      #if HAS_X2_MAX
        if (TEST(endstop_change, X2_MAX)) SERIAL_PROTOCOLPAIR("  X2_MAX:", TEST(live_state_local, X2_MAX));
      #endif
      #if HAS_Y2_MIN
        if (TEST(endstop_change, Y2_MIN)) SERIAL_PROTOCOLPAIR("  Y2_MIN:", TEST(live_state_local, Y2_MIN));
      #endif
      #if HAS_Y2_MAX
        if (TEST(endstop_change, Y2_MAX)) SERIAL_PROTOCOLPAIR("  Y2_MAX:", TEST(live_state_local, Y2_MAX));
      #endif
      #if HAS_Z2_MIN
        if (TEST(endstop_change, Z2_MIN)) SERIAL_PROTOCOLPAIR("  Z2_MIN:", TEST(live_state_local, Z2_MIN));
      #endif
      #if HAS_Z2_MAX
        if (TEST(endstop_change, Z2_MAX)) SERIAL_PROTOCOLPAIR("  Z2_MAX:", TEST(live_state_local, Z2_MAX));
      #endif
      SERIAL_PROTOCOLPGM("\n\n");
      analogWrite(LED_PIN, local_LED_status);
      local_LED_status ^= 255;
      old_live_state_local = live_state_local;
    }
  }

#endif // PINS_DEBUGGING
