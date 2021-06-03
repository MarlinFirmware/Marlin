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

/**
 * endstops.cpp - A singleton object to manage endstops
 */

#include "endstops.h"
#include "stepper.h"

#include "../sd/cardreader.h"
#include "temperature.h"
#include "../lcd/marlinui.h"

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #include HAL_PATH(../HAL, endstop_interrupts.h)
#endif

#if BOTH(SD_ABORT_ON_ENDSTOP_HIT, SDSUPPORT)
  #include "printcounter.h" // for print_job_timer
#endif

#if ENABLED(BLTOUCH)
  #include "../feature/bltouch.h"
#endif

#if ENABLED(JOYSTICK)
  #include "../feature/joystick.h"
#endif

#if HAS_BED_PROBE
  #include "probe.h"
#endif

Endstops endstops;

// private:

bool Endstops::enabled, Endstops::enabled_globally; // Initialized by settings.load()

volatile Endstops::endstop_mask_t Endstops::hit_state;
Endstops::endstop_mask_t Endstops::live_state = 0;

#if ENDSTOP_NOISE_THRESHOLD
  Endstops::endstop_mask_t Endstops::validated_live_state;
  uint8_t Endstops::endstop_poll_count;
#endif

#if HAS_BED_PROBE
  volatile bool Endstops::z_probe_enabled = false;
#endif

// Initialized by settings.load()
#if ENABLED(X_DUAL_ENDSTOPS)
  float Endstops::x2_endstop_adj;
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  float Endstops::y2_endstop_adj;
#endif
#if ENABLED(Z_MULTI_ENDSTOPS)
  float Endstops::z2_endstop_adj;
  #if NUM_Z_STEPPER_DRIVERS >= 3
    float Endstops::z3_endstop_adj;
    #if NUM_Z_STEPPER_DRIVERS >= 4
      float Endstops::z4_endstop_adj;
    #endif
  #endif
#endif

#if ENABLED(SPI_ENDSTOPS)
  Endstops::tmc_spi_homing_t Endstops::tmc_spi_homing; // = 0
#endif
#if ENABLED(IMPROVE_HOMING_RELIABILITY)
  millis_t sg_guard_period; // = 0
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

  #if HAS_Z3_MIN
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      SET_INPUT_PULLUP(Z3_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN)
      SET_INPUT_PULLDOWN(Z3_MIN_PIN);
    #else
      SET_INPUT(Z3_MIN_PIN);
    #endif
  #endif

  #if HAS_Z4_MIN
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      SET_INPUT_PULLUP(Z4_MIN_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN)
      SET_INPUT_PULLDOWN(Z4_MIN_PIN);
    #else
      SET_INPUT(Z4_MIN_PIN);
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

  #if HAS_Z3_MAX
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      SET_INPUT_PULLUP(Z3_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMAX)
      SET_INPUT_PULLDOWN(Z3_MAX_PIN);
    #else
      SET_INPUT(Z3_MAX_PIN);
    #endif
  #endif

  #if HAS_Z4_MAX
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      SET_INPUT_PULLUP(Z4_MAX_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMAX)
      SET_INPUT_PULLDOWN(Z4_MAX_PIN);
    #else
      SET_INPUT(Z4_MAX_PIN);
    #endif
  #endif

  #if PIN_EXISTS(CALIBRATION)
    #if ENABLED(CALIBRATION_PIN_PULLUP)
      SET_INPUT_PULLUP(CALIBRATION_PIN);
    #elif ENABLED(CALIBRATION_PIN_PULLDOWN)
      SET_INPUT_PULLDOWN(CALIBRATION_PIN);
    #else
      SET_INPUT(CALIBRATION_PIN);
    #endif
  #endif

  #if HAS_CUSTOM_PROBE_PIN
    #if ENABLED(ENDSTOPPULLUP_ZMIN_PROBE)
      SET_INPUT_PULLUP(Z_MIN_PROBE_PIN);
    #elif ENABLED(ENDSTOPPULLDOWN_ZMIN_PROBE)
      SET_INPUT_PULLDOWN(Z_MIN_PROBE_PIN);
    #else
      SET_INPUT(Z_MIN_PROBE_PIN);
    #endif
  #endif

  #if ENABLED(PROBE_ACTIVATION_SWITCH)
    SET_INPUT(PROBE_ACTIVATION_SWITCH_PIN);
  #endif

  TERN_(PROBE_TARE, probe.tare());

  TERN_(ENDSTOP_INTERRUPTS_FEATURE, setup_endstop_interrupts());

  // Enable endstops
  enable_globally(ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT));

} // Endstops::init

// Called at ~1KHz from Temperature ISR: Poll endstop state if required
void Endstops::poll() {

  TERN_(PINS_DEBUGGING, run_monitor()); // Report changes in endstop status

  #if DISABLED(ENDSTOP_INTERRUPTS_FEATURE)
    update();
  #elif ENDSTOP_NOISE_THRESHOLD
    if (endstop_poll_count) update();
  #endif
}

void Endstops::enable_globally(const bool onoff) {
  enabled_globally = enabled = onoff;
  resync();
}

// Enable / disable endstop checking
void Endstops::enable(const bool onoff) {
  enabled = onoff;
  resync();
}

// Disable / Enable endstops based on ENSTOPS_ONLY_FOR_HOMING and global enable
void Endstops::not_homing() {
  enabled = enabled_globally;
}

#if ENABLED(VALIDATE_HOMING_ENDSTOPS)
  // If the last move failed to trigger an endstop, call kill
  void Endstops::validate_homing_move() {
    if (trigger_state()) hit_on_purpose();
    else kill(GET_TEXT(MSG_KILL_HOMING_FAILED));
  }
#endif

// Enable / disable endstop z-probe checking
#if HAS_BED_PROBE
  void Endstops::enable_z_probe(const bool onoff) {
    z_probe_enabled = onoff;
    resync();
  }
#endif

// Get the stable endstop states when enabled
void Endstops::resync() {
  if (!abort_enabled()) return;     // If endstops/probes are disabled the loop below can hang

  // Wait for Temperature ISR to run at least once (runs at 1KHz)
  TERN(ENDSTOP_INTERRUPTS_FEATURE, update(), safe_delay(2));
  while (TERN0(ENDSTOP_NOISE_THRESHOLD, endstop_poll_count)) safe_delay(1);
}

#if ENABLED(PINS_DEBUGGING)
  void Endstops::run_monitor() {
    if (!monitor_flag) return;
    static uint8_t monitor_count = 16;  // offset this check from the others
    monitor_count += _BV(1);            //  15 Hz
    monitor_count &= 0x7F;
    if (!monitor_count) monitor();      // report changes in endstop status
  }
#endif

void Endstops::event_handler() {
  static endstop_mask_t prev_hit_state; // = 0
  if (hit_state == prev_hit_state) return;
  prev_hit_state = hit_state;
  if (hit_state) {
    #if HAS_STATUS_MESSAGE
      char LINEAR_AXIS_LIST(chrX = ' ', chrY = ' ', chrZ = ' '),
           chrP = ' ';
      #define _SET_STOP_CHAR(A,C) (chr## A = C)
    #else
      #define _SET_STOP_CHAR(A,C) NOOP
    #endif

    #define _ENDSTOP_HIT_ECHO(A,C) do{ \
      SERIAL_ECHOPAIR(" " STRINGIFY(A) ":", planner.triggered_position_mm(_AXIS(A))); _SET_STOP_CHAR(A,C); }while(0)

    #define _ENDSTOP_HIT_TEST(A,C) \
      if (TERN0(HAS_##A##_MIN, TEST(hit_state, A##_MIN)) || TERN0(HAS_##A##_MAX, TEST(hit_state, A##_MAX))) \
        _ENDSTOP_HIT_ECHO(A,C)

    #define ENDSTOP_HIT_TEST_X() _ENDSTOP_HIT_TEST(X,'X')
    #define ENDSTOP_HIT_TEST_Y() _ENDSTOP_HIT_TEST(Y,'Y')
    #define ENDSTOP_HIT_TEST_Z() _ENDSTOP_HIT_TEST(Z,'Z')

    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(STR_ENDSTOPS_HIT);
    ENDSTOP_HIT_TEST_X();
    ENDSTOP_HIT_TEST_Y();
    ENDSTOP_HIT_TEST_Z();

    #if HAS_CUSTOM_PROBE_PIN
      #define P_AXIS Z_AXIS
      if (TEST(hit_state, Z_MIN_PROBE)) _ENDSTOP_HIT_ECHO(P, 'P');
    #endif
    SERIAL_EOL();

    TERN_(HAS_STATUS_MESSAGE,
      ui.status_printf_P(0,
        PSTR(S_FMT GANG_N_1(LINEAR_AXES, " %c") " %c"),
        GET_TEXT(MSG_LCD_ENDSTOPS),
        LINEAR_AXIS_LIST(chrX, chrY, chrZ), chrP
      )
    );

    #if BOTH(SD_ABORT_ON_ENDSTOP_HIT, SDSUPPORT)
      if (planner.abort_on_endstop_hit) {
        card.abortFilePrintNow();
        quickstop_stepper();
        thermalManager.disable_all_heaters();
        print_job_timer.stop();
      }
    #endif
  }
}

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

static void print_es_state(const bool is_hit, PGM_P const label=nullptr) {
  if (label) SERIAL_ECHOPGM_P(label);
  SERIAL_ECHOPGM(": ");
  SERIAL_ECHOLNPGM_P(is_hit ? PSTR(STR_ENDSTOP_HIT) : PSTR(STR_ENDSTOP_OPEN));
}

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

void _O2 Endstops::report_states() {
  TERN_(BLTOUCH, bltouch._set_SW_mode());
  SERIAL_ECHOLNPGM(STR_M119_REPORT);
  #define ES_REPORT(S) print_es_state(READ(S##_PIN) != S##_ENDSTOP_INVERTING, PSTR(STR_##S))
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
  #if HAS_Z3_MIN
    ES_REPORT(Z3_MIN);
  #endif
  #if HAS_Z4_MIN
    ES_REPORT(Z4_MIN);
  #endif
  #if HAS_Z_MAX
    ES_REPORT(Z_MAX);
  #endif
  #if HAS_Z2_MAX
    ES_REPORT(Z2_MAX);
  #endif
  #if HAS_Z3_MAX
    ES_REPORT(Z3_MAX);
  #endif
  #if HAS_Z4_MAX
    ES_REPORT(Z4_MAX);
  #endif
  #if BOTH(MARLIN_DEV_MODE, PROBE_ACTIVATION_SWITCH)
    print_es_state(probe_switch_activated(), PSTR(STR_PROBE_EN));
  #endif
  #if HAS_CUSTOM_PROBE_PIN
    print_es_state(PROBE_TRIGGERED(), PSTR(STR_Z_PROBE));
  #endif
  #if MULTI_FILAMENT_SENSOR
    #define _CASE_RUNOUT(N) case N: pin = FIL_RUNOUT##N##_PIN; state = FIL_RUNOUT##N##_STATE; break;
    LOOP_S_LE_N(i, 1, NUM_RUNOUT_SENSORS) {
      pin_t pin;
      uint8_t state;
      switch (i) {
        default: continue;
        REPEAT_1(NUM_RUNOUT_SENSORS, _CASE_RUNOUT)
      }
      SERIAL_ECHOPGM(STR_FILAMENT_RUNOUT_SENSOR);
      if (i > 1) SERIAL_CHAR(' ', '0' + i);
      print_es_state(extDigitalRead(pin) != state);
    }
    #undef _CASE_RUNOUT
  #elif HAS_FILAMENT_SENSOR
    print_es_state(READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE, PSTR(STR_FILAMENT_RUNOUT_SENSOR));
  #endif

  TERN_(BLTOUCH, bltouch._reset_SW_mode());
  TERN_(JOYSTICK_DEBUG, joystick.report());

} // Endstops::report_states

// The following routines are called from an ISR context. It could be the temperature ISR, the
// endstop ISR or the Stepper ISR.

#define _ENDSTOP(AXIS, MINMAX) AXIS ##_## MINMAX
#define _ENDSTOP_PIN(AXIS, MINMAX) AXIS ##_## MINMAX ##_PIN
#define _ENDSTOP_INVERTING(AXIS, MINMAX) AXIS ##_## MINMAX ##_ENDSTOP_INVERTING

// Check endstops - Could be called from Temperature ISR!
void Endstops::update() {

  #if !ENDSTOP_NOISE_THRESHOLD
    if (!abort_enabled()) return;
  #endif

  #define UPDATE_ENDSTOP_BIT(AXIS, MINMAX) SET_BIT_TO(live_state, _ENDSTOP(AXIS, MINMAX), (READ(_ENDSTOP_PIN(AXIS, MINMAX)) != _ENDSTOP_INVERTING(AXIS, MINMAX)))
  #define COPY_LIVE_STATE(SRC_BIT, DST_BIT) SET_BIT_TO(live_state, DST_BIT, TEST(live_state, SRC_BIT))

  #if BOTH(G38_PROBE_TARGET, HAS_Z_MIN_PROBE_PIN) && NONE(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY)
    // If G38 command is active check Z_MIN_PROBE for ALL movement
    if (G38_move) UPDATE_ENDSTOP_BIT(Z, MIN_PROBE);
  #endif

  // With Dual X, endstops are only checked in the homing direction for the active extruder
  #define X_MIN_TEST() TERN1(DUAL_X_CARRIAGE, TERN0(X_HOME_TO_MIN, stepper.last_moved_extruder == 0) || TERN0(X2_HOME_TO_MIN, stepper.last_moved_extruder != 0))
  #define X_MAX_TEST() TERN1(DUAL_X_CARRIAGE, TERN0(X_HOME_TO_MAX, stepper.last_moved_extruder == 0) || TERN0(X2_HOME_TO_MAX, stepper.last_moved_extruder != 0))

  // Use HEAD for core axes, AXIS for others
  #if ANY(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY)
    #define X_AXIS_HEAD X_HEAD
  #else
    #define X_AXIS_HEAD X_AXIS
  #endif
  #if ANY(CORE_IS_XY, CORE_IS_YZ, MARKFORGED_XY)
    #define Y_AXIS_HEAD Y_HEAD
  #else
    #define Y_AXIS_HEAD Y_AXIS
  #endif
  #if CORE_IS_XZ || CORE_IS_YZ
    #define Z_AXIS_HEAD Z_HEAD
  #else
    #define Z_AXIS_HEAD Z_AXIS
  #endif

  /**
   * Check and update endstops
   */
  #if HAS_X_MIN && !X_SPI_SENSORLESS
    UPDATE_ENDSTOP_BIT(X, MIN);
    #if ENABLED(X_DUAL_ENDSTOPS)
      #if HAS_X2_MIN
        UPDATE_ENDSTOP_BIT(X2, MIN);
      #else
        COPY_LIVE_STATE(X_MIN, X2_MIN);
      #endif
    #endif
  #endif

  #if HAS_X_MAX && !X_SPI_SENSORLESS
    UPDATE_ENDSTOP_BIT(X, MAX);
    #if ENABLED(X_DUAL_ENDSTOPS)
      #if HAS_X2_MAX
        UPDATE_ENDSTOP_BIT(X2, MAX);
      #else
        COPY_LIVE_STATE(X_MAX, X2_MAX);
      #endif
    #endif
  #endif

  #if HAS_Y_MIN && !Y_SPI_SENSORLESS
    UPDATE_ENDSTOP_BIT(Y, MIN);
    #if ENABLED(Y_DUAL_ENDSTOPS)
      #if HAS_Y2_MIN
        UPDATE_ENDSTOP_BIT(Y2, MIN);
      #else
        COPY_LIVE_STATE(Y_MIN, Y2_MIN);
      #endif
    #endif
  #endif

  #if HAS_Y_MAX && !Y_SPI_SENSORLESS
    UPDATE_ENDSTOP_BIT(Y, MAX);
    #if ENABLED(Y_DUAL_ENDSTOPS)
      #if HAS_Y2_MAX
        UPDATE_ENDSTOP_BIT(Y2, MAX);
      #else
        COPY_LIVE_STATE(Y_MAX, Y2_MAX);
      #endif
    #endif
  #endif

  #if HAS_Z_MIN && NONE(Z_SPI_SENSORLESS, Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    UPDATE_ENDSTOP_BIT(Z, MIN);
    #if ENABLED(Z_MULTI_ENDSTOPS)
      #if HAS_Z2_MIN
        UPDATE_ENDSTOP_BIT(Z2, MIN);
      #else
        COPY_LIVE_STATE(Z_MIN, Z2_MIN);
      #endif
      #if NUM_Z_STEPPER_DRIVERS >= 3
        #if HAS_Z3_MIN
          UPDATE_ENDSTOP_BIT(Z3, MIN);
        #else
          COPY_LIVE_STATE(Z_MIN, Z3_MIN);
        #endif
      #endif
      #if NUM_Z_STEPPER_DRIVERS >= 4
        #if HAS_Z4_MIN
          UPDATE_ENDSTOP_BIT(Z4, MIN);
        #else
          COPY_LIVE_STATE(Z_MIN, Z4_MIN);
        #endif
      #endif
    #endif
  #endif

  #if HAS_BED_PROBE
    // When closing the gap check the enabled probe
    if (probe_switch_activated())
      UPDATE_ENDSTOP_BIT(Z, TERN(HAS_CUSTOM_PROBE_PIN, MIN_PROBE, MIN));
  #endif

  #if HAS_Z_MAX && !Z_SPI_SENSORLESS
    // Check both Z dual endstops
    #if ENABLED(Z_MULTI_ENDSTOPS)
      UPDATE_ENDSTOP_BIT(Z, MAX);
      #if HAS_Z2_MAX
        UPDATE_ENDSTOP_BIT(Z2, MAX);
      #else
        COPY_LIVE_STATE(Z_MAX, Z2_MAX);
      #endif
      #if NUM_Z_STEPPER_DRIVERS >= 3
        #if HAS_Z3_MAX
          UPDATE_ENDSTOP_BIT(Z3, MAX);
        #else
          COPY_LIVE_STATE(Z_MAX, Z3_MAX);
        #endif
      #endif
      #if NUM_Z_STEPPER_DRIVERS >= 4
        #if HAS_Z4_MAX
          UPDATE_ENDSTOP_BIT(Z4, MAX);
        #else
          COPY_LIVE_STATE(Z_MAX, Z4_MAX);
        #endif
      #endif
    #elif !HAS_CUSTOM_PROBE_PIN || Z_MAX_PIN != Z_MIN_PROBE_PIN
      // If this pin isn't the bed probe it's the Z endstop
      UPDATE_ENDSTOP_BIT(Z, MAX);
    #endif
  #endif

  #if ENDSTOP_NOISE_THRESHOLD

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
    static endstop_mask_t old_live_state;
    if (old_live_state != live_state) {
      endstop_poll_count = ENDSTOP_NOISE_THRESHOLD;
      old_live_state = live_state;
    }
    else if (endstop_poll_count && !--endstop_poll_count)
      validated_live_state = live_state;

    if (!abort_enabled()) return;

  #endif

  // Test the current status of an endstop
  #define TEST_ENDSTOP(ENDSTOP) (TEST(state(), ENDSTOP))

  // Record endstop was hit
  #define _ENDSTOP_HIT(AXIS, MINMAX) SBI(hit_state, _ENDSTOP(AXIS, MINMAX))

  // Call the endstop triggered routine for single endstops
  #define PROCESS_ENDSTOP(AXIS, MINMAX) do { \
    if (TEST_ENDSTOP(_ENDSTOP(AXIS, MINMAX))) { \
      _ENDSTOP_HIT(AXIS, MINMAX); \
      planner.endstop_triggered(_AXIS(AXIS)); \
    } \
  }while(0)

  // Core Sensorless Homing needs to test an Extra Pin
  #define CORE_DIAG(QQ,A,MM) (CORE_IS_##QQ && A##_SENSORLESS && !A##_SPI_SENSORLESS && HAS_##A##_##MM)
  #define PROCESS_CORE_ENDSTOP(A1,M1,A2,M2) do { \
    if (TEST_ENDSTOP(_ENDSTOP(A1,M1))) { \
      _ENDSTOP_HIT(A2,M2); \
      planner.endstop_triggered(_AXIS(A2)); \
    } \
  }while(0)

  // Call the endstop triggered routine for dual endstops
  #define PROCESS_DUAL_ENDSTOP(A, MINMAX) do { \
    const byte dual_hit = TEST_ENDSTOP(_ENDSTOP(A, MINMAX)) | (TEST_ENDSTOP(_ENDSTOP(A##2, MINMAX)) << 1); \
    if (dual_hit) { \
      _ENDSTOP_HIT(A, MINMAX); \
      /* if not performing home or if both endstops were trigged during homing... */ \
      if (!stepper.separate_multi_axis || dual_hit == 0b11) \
        planner.endstop_triggered(_AXIS(A)); \
    } \
  }while(0)

  #define PROCESS_TRIPLE_ENDSTOP(A, MINMAX) do { \
    const byte triple_hit = TEST_ENDSTOP(_ENDSTOP(A, MINMAX)) | (TEST_ENDSTOP(_ENDSTOP(A##2, MINMAX)) << 1) | (TEST_ENDSTOP(_ENDSTOP(A##3, MINMAX)) << 2); \
    if (triple_hit) { \
      _ENDSTOP_HIT(A, MINMAX); \
      /* if not performing home or if both endstops were trigged during homing... */ \
      if (!stepper.separate_multi_axis || triple_hit == 0b111) \
        planner.endstop_triggered(_AXIS(A)); \
    } \
  }while(0)

  #define PROCESS_QUAD_ENDSTOP(A, MINMAX) do { \
    const byte quad_hit = TEST_ENDSTOP(_ENDSTOP(A, MINMAX)) | (TEST_ENDSTOP(_ENDSTOP(A##2, MINMAX)) << 1) | (TEST_ENDSTOP(_ENDSTOP(A##3, MINMAX)) << 2) | (TEST_ENDSTOP(_ENDSTOP(A##4, MINMAX)) << 3); \
    if (quad_hit) { \
      _ENDSTOP_HIT(A, MINMAX); \
      /* if not performing home or if both endstops were trigged during homing... */ \
      if (!stepper.separate_multi_axis || quad_hit == 0b1111) \
        planner.endstop_triggered(_AXIS(A)); \
    } \
  }while(0)

  #if ENABLED(X_DUAL_ENDSTOPS)
    #define PROCESS_ENDSTOP_X(MINMAX) PROCESS_DUAL_ENDSTOP(X, MINMAX)
  #else
    #define PROCESS_ENDSTOP_X(MINMAX) if (X_##MINMAX##_TEST()) PROCESS_ENDSTOP(X, MINMAX)
  #endif

  #if ENABLED(Y_DUAL_ENDSTOPS)
    #define PROCESS_ENDSTOP_Y(MINMAX) PROCESS_DUAL_ENDSTOP(Y, MINMAX)
  #else
    #define PROCESS_ENDSTOP_Y(MINMAX) PROCESS_ENDSTOP(Y, MINMAX)
  #endif

  #if DISABLED(Z_MULTI_ENDSTOPS)
    #define PROCESS_ENDSTOP_Z(MINMAX) PROCESS_ENDSTOP(Z, MINMAX)
  #elif NUM_Z_STEPPER_DRIVERS == 4
    #define PROCESS_ENDSTOP_Z(MINMAX) PROCESS_QUAD_ENDSTOP(Z, MINMAX)
  #elif NUM_Z_STEPPER_DRIVERS == 3
    #define PROCESS_ENDSTOP_Z(MINMAX) PROCESS_TRIPLE_ENDSTOP(Z, MINMAX)
  #else
    #define PROCESS_ENDSTOP_Z(MINMAX) PROCESS_DUAL_ENDSTOP(Z, MINMAX)
  #endif

  #if BOTH(G38_PROBE_TARGET, HAS_Z_MIN_PROBE_PIN) && NONE(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY)
    #if ENABLED(G38_PROBE_AWAY)
      #define _G38_OPEN_STATE (G38_move >= 4)
    #else
      #define _G38_OPEN_STATE LOW
    #endif
    // If G38 command is active check Z_MIN_PROBE for ALL movement
    if (G38_move && TEST_ENDSTOP(_ENDSTOP(Z, MIN_PROBE)) != _G38_OPEN_STATE) {
           if (stepper.axis_is_moving(X_AXIS)) { _ENDSTOP_HIT(X, MIN); planner.endstop_triggered(X_AXIS); }
      else if (stepper.axis_is_moving(Y_AXIS)) { _ENDSTOP_HIT(Y, MIN); planner.endstop_triggered(Y_AXIS); }
      else if (stepper.axis_is_moving(Z_AXIS)) { _ENDSTOP_HIT(Z, MIN); planner.endstop_triggered(Z_AXIS); }
      G38_did_trigger = true;
    }
  #endif

  // Signal, after validation, if an endstop limit is pressed or not

  if (stepper.axis_is_moving(X_AXIS)) {
    if (stepper.motor_direction(X_AXIS_HEAD)) { // -direction
      #if HAS_X_MIN || (X_SPI_SENSORLESS && X_HOME_TO_MIN)
        PROCESS_ENDSTOP_X(MIN);
        #if   CORE_DIAG(XY, Y, MIN)
          PROCESS_CORE_ENDSTOP(Y,MIN,X,MIN);
        #elif CORE_DIAG(XY, Y, MAX)
          PROCESS_CORE_ENDSTOP(Y,MAX,X,MIN);
        #elif CORE_DIAG(XZ, Z, MIN)
          PROCESS_CORE_ENDSTOP(Z,MIN,X,MIN);
        #elif CORE_DIAG(XZ, Z, MAX)
          PROCESS_CORE_ENDSTOP(Z,MAX,X,MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_X_MAX || (X_SPI_SENSORLESS && X_HOME_TO_MAX)
        PROCESS_ENDSTOP_X(MAX);
        #if   CORE_DIAG(XY, Y, MIN)
          PROCESS_CORE_ENDSTOP(Y,MIN,X,MAX);
        #elif CORE_DIAG(XY, Y, MAX)
          PROCESS_CORE_ENDSTOP(Y,MAX,X,MAX);
        #elif CORE_DIAG(XZ, Z, MIN)
          PROCESS_CORE_ENDSTOP(Z,MIN,X,MAX);
        #elif CORE_DIAG(XZ, Z, MAX)
          PROCESS_CORE_ENDSTOP(Z,MAX,X,MAX);
        #endif
      #endif
    }
  }

  if (stepper.axis_is_moving(Y_AXIS)) {
    if (stepper.motor_direction(Y_AXIS_HEAD)) { // -direction
      #if HAS_Y_MIN || (Y_SPI_SENSORLESS && Y_HOME_TO_MIN)
        PROCESS_ENDSTOP_Y(MIN);
        #if   CORE_DIAG(XY, X, MIN)
          PROCESS_CORE_ENDSTOP(X,MIN,Y,MIN);
        #elif CORE_DIAG(XY, X, MAX)
          PROCESS_CORE_ENDSTOP(X,MAX,Y,MIN);
        #elif CORE_DIAG(YZ, Z, MIN)
          PROCESS_CORE_ENDSTOP(Z,MIN,Y,MIN);
        #elif CORE_DIAG(YZ, Z, MAX)
          PROCESS_CORE_ENDSTOP(Z,MAX,Y,MIN);
        #endif
      #endif
    }
    else { // +direction
      #if HAS_Y_MAX || (Y_SPI_SENSORLESS && Y_HOME_TO_MAX)
        PROCESS_ENDSTOP_Y(MAX);
        #if   CORE_DIAG(XY, X, MIN)
          PROCESS_CORE_ENDSTOP(X,MIN,Y,MAX);
        #elif CORE_DIAG(XY, X, MAX)
          PROCESS_CORE_ENDSTOP(X,MAX,Y,MAX);
        #elif CORE_DIAG(YZ, Z, MIN)
          PROCESS_CORE_ENDSTOP(Z,MIN,Y,MAX);
        #elif CORE_DIAG(YZ, Z, MAX)
          PROCESS_CORE_ENDSTOP(Z,MAX,Y,MAX);
        #endif
      #endif
    }
  }

  if (stepper.axis_is_moving(Z_AXIS)) {
    if (stepper.motor_direction(Z_AXIS_HEAD)) { // Z -direction. Gantry down, bed up.

      #if HAS_Z_MIN || (Z_SPI_SENSORLESS && Z_HOME_TO_MIN)
        if ( TERN1(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN, z_probe_enabled)
          && TERN1(HAS_CUSTOM_PROBE_PIN, !z_probe_enabled)
        ) PROCESS_ENDSTOP_Z(MIN);
        #if   CORE_DIAG(XZ, X, MIN)
          PROCESS_CORE_ENDSTOP(X,MIN,Z,MIN);
        #elif CORE_DIAG(XZ, X, MAX)
          PROCESS_CORE_ENDSTOP(X,MAX,Z,MIN);
        #elif CORE_DIAG(YZ, Y, MIN)
          PROCESS_CORE_ENDSTOP(Y,MIN,Z,MIN);
        #elif CORE_DIAG(YZ, Y, MAX)
          PROCESS_CORE_ENDSTOP(Y,MAX,Z,MIN);
        #endif
      #endif

      // When closing the gap check the enabled probe
      #if HAS_CUSTOM_PROBE_PIN
        if (z_probe_enabled) PROCESS_ENDSTOP(Z, MIN_PROBE);
      #endif
    }
    else { // Z +direction. Gantry up, bed down.
      #if HAS_Z_MAX || (Z_SPI_SENSORLESS && Z_HOME_TO_MAX)
        #if ENABLED(Z_MULTI_ENDSTOPS)
          PROCESS_ENDSTOP_Z(MAX);
        #elif !HAS_CUSTOM_PROBE_PIN || Z_MAX_PIN != Z_MIN_PROBE_PIN  // No probe or probe is Z_MIN || Probe is not Z_MAX
          PROCESS_ENDSTOP(Z, MAX);
        #endif
        #if   CORE_DIAG(XZ, X, MIN)
          PROCESS_CORE_ENDSTOP(X,MIN,Z,MAX);
        #elif CORE_DIAG(XZ, X, MAX)
          PROCESS_CORE_ENDSTOP(X,MAX,Z,MAX);
        #elif CORE_DIAG(YZ, Y, MIN)
          PROCESS_CORE_ENDSTOP(Y,MIN,Z,MAX);
        #elif CORE_DIAG(YZ, Y, MAX)
          PROCESS_CORE_ENDSTOP(Y,MAX,Z,MAX);
        #endif
      #endif
    }
  }
} // Endstops::update()

#if ENABLED(SPI_ENDSTOPS)

  bool Endstops::tmc_spi_homing_check() {
    bool hit = false;
    #if X_SPI_SENSORLESS
      if (tmc_spi_homing.x && (stepperX.test_stall_status()
        #if ANY(CORE_IS_XY, MARKFORGED_XY) && Y_SPI_SENSORLESS
          || stepperY.test_stall_status()
        #elif CORE_IS_XZ && Z_SPI_SENSORLESS
          || stepperZ.test_stall_status()
        #endif
      )) {
        SBI(live_state, X_ENDSTOP);
        hit = true;
      }
    #endif
    #if Y_SPI_SENSORLESS
      if (tmc_spi_homing.y && (stepperY.test_stall_status()
        #if ANY(CORE_IS_XY, MARKFORGED_XY) && X_SPI_SENSORLESS
          || stepperX.test_stall_status()
        #elif CORE_IS_YZ && Z_SPI_SENSORLESS
          || stepperZ.test_stall_status()
        #endif
      )) {
        SBI(live_state, Y_ENDSTOP);
        hit = true;
      }
    #endif
    #if Z_SPI_SENSORLESS
      if (tmc_spi_homing.z && (stepperZ.test_stall_status()
        #if CORE_IS_XZ && X_SPI_SENSORLESS
          || stepperX.test_stall_status()
        #elif CORE_IS_YZ && Y_SPI_SENSORLESS
          || stepperY.test_stall_status()
        #endif
      )) {
        SBI(live_state, Z_ENDSTOP);
        hit = true;
      }
    #endif

    if (TERN0(ENDSTOP_INTERRUPTS_FEATURE, hit)) update();

    return hit;
  }

  void Endstops::clear_endstop_state() {
    TERN_(X_SPI_SENSORLESS, CBI(live_state, X_ENDSTOP));
    TERN_(Y_SPI_SENSORLESS, CBI(live_state, Y_ENDSTOP));
    TERN_(Z_SPI_SENSORLESS, CBI(live_state, Z_ENDSTOP));
  }

#endif // SPI_ENDSTOPS

#if ENABLED(PINS_DEBUGGING)

  bool Endstops::monitor_flag = false;

  /**
   * Monitor Endstops and Z Probe for changes
   *
   * If a change is detected then the LED is toggled and
   * a message is sent out the serial port.
   *
   * Yes, we could miss a rapid back & forth change but
   * that won't matter because this is all manual.
   */
  void Endstops::monitor() {

    static uint16_t old_live_state_local = 0;
    static uint8_t local_LED_status = 0;
    uint16_t live_state_local = 0;

    #define ES_GET_STATE(S) if (READ(S##_PIN)) SBI(live_state_local, S)

    #if HAS_X_MIN
      ES_GET_STATE(X_MIN);
    #endif
    #if HAS_X_MAX
      ES_GET_STATE(X_MAX);
    #endif
    #if HAS_Y_MIN
      ES_GET_STATE(Y_MIN);
    #endif
    #if HAS_Y_MAX
      ES_GET_STATE(Y_MAX);
    #endif
    #if HAS_Z_MIN
      ES_GET_STATE(Z_MIN);
    #endif
    #if HAS_Z_MAX
      ES_GET_STATE(Z_MAX);
    #endif
    #if HAS_Z_MIN_PROBE_PIN
      ES_GET_STATE(Z_MIN_PROBE);
    #endif
    #if HAS_X2_MIN
      ES_GET_STATE(X2_MIN);
    #endif
    #if HAS_X2_MAX
      ES_GET_STATE(X2_MAX);
    #endif
    #if HAS_Y2_MIN
      ES_GET_STATE(Y2_MIN);
    #endif
    #if HAS_Y2_MAX
      ES_GET_STATE(Y2_MAX);
    #endif
    #if HAS_Z2_MIN
      ES_GET_STATE(Z2_MIN);
    #endif
    #if HAS_Z2_MAX
      ES_GET_STATE(Z2_MAX);
    #endif
    #if HAS_Z3_MIN
      ES_GET_STATE(Z3_MIN);
    #endif
    #if HAS_Z3_MAX
      ES_GET_STATE(Z3_MAX);
    #endif
    #if HAS_Z4_MIN
      ES_GET_STATE(Z4_MIN);
    #endif
    #if HAS_Z4_MAX
      ES_GET_STATE(Z4_MAX);
    #endif

    uint16_t endstop_change = live_state_local ^ old_live_state_local;
    #define ES_REPORT_CHANGE(S) if (TEST(endstop_change, S)) SERIAL_ECHOPAIR("  " STRINGIFY(S) ":", TEST(live_state_local, S))

    if (endstop_change) {
      #if HAS_X_MIN
        ES_REPORT_CHANGE(X_MIN);
      #endif
      #if HAS_X_MAX
        ES_REPORT_CHANGE(X_MAX);
      #endif
      #if HAS_Y_MIN
        ES_REPORT_CHANGE(Y_MIN);
      #endif
      #if HAS_Y_MAX
        ES_REPORT_CHANGE(Y_MAX);
      #endif
      #if HAS_Z_MIN
        ES_REPORT_CHANGE(Z_MIN);
      #endif
      #if HAS_Z_MAX
        ES_REPORT_CHANGE(Z_MAX);
      #endif
      #if HAS_Z_MIN_PROBE_PIN
        ES_REPORT_CHANGE(Z_MIN_PROBE);
      #endif
      #if HAS_X2_MIN
        ES_REPORT_CHANGE(X2_MIN);
      #endif
      #if HAS_X2_MAX
        ES_REPORT_CHANGE(X2_MAX);
      #endif
      #if HAS_Y2_MIN
        ES_REPORT_CHANGE(Y2_MIN);
      #endif
      #if HAS_Y2_MAX
        ES_REPORT_CHANGE(Y2_MAX);
      #endif
      #if HAS_Z2_MIN
        ES_REPORT_CHANGE(Z2_MIN);
      #endif
      #if HAS_Z2_MAX
        ES_REPORT_CHANGE(Z2_MAX);
      #endif
      #if HAS_Z3_MIN
        ES_REPORT_CHANGE(Z3_MIN);
      #endif
      #if HAS_Z3_MAX
        ES_REPORT_CHANGE(Z3_MAX);
      #endif
      #if HAS_Z4_MIN
        ES_REPORT_CHANGE(Z4_MIN);
      #endif
      #if HAS_Z4_MAX
        ES_REPORT_CHANGE(Z4_MAX);
      #endif
      SERIAL_ECHOLNPGM("\n");
      analogWrite(pin_t(LED_PIN), local_LED_status);
      local_LED_status ^= 255;
      old_live_state_local = live_state_local;
    }
  }

#endif // PINS_DEBUGGING
