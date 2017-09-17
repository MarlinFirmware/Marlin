/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * About Marlin
 *
 * This firmware is a mashup between Sprinter and grbl.
 *  - https://github.com/kliment/Sprinter
 *  - https://github.com/simen/grbl
 */

#include "Marlin.h"

#include "lcd/ultralcd.h"
#include "module/motion.h"
#include "module/planner.h"
#include "module/stepper.h"
#include "module/endstops.h"
#include "module/probe.h"
#include "module/temperature.h"
#include "sd/cardreader.h"
#include "module/configuration_store.h"
#include "module/printcounter.h" // PrintCounter or Stopwatch
#ifdef ARDUINO
  #include <pins_arduino.h>
#endif
#include <math.h>
#include "libs/nozzle.h"

#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "gcode/queue.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "libs/buzzer.h"
#endif

#if (ENABLED(SWITCHING_EXTRUDER) && !DONT_SWITCH) || ENABLED(SWITCHING_NOZZLE)
  #include "module/tool_change.h"
#endif

#if ENABLED(BEZIER_CURVE_SUPPORT)
  #include "module/planner_bezier.h"
#endif

#if ENABLED(MAX7219_DEBUG)
  #include "feature/Max7219_Debug_LEDs.h"
#endif

#if HAS_COLOR_LEDS
  #include "feature/leds/leds.h"
#endif

#if HAS_SERVOS
  #include "HAL/servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "feature/dac/stepper_dac.h"
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "feature/twibus.h"
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "feature/I2CPositionEncoder.h"
#endif

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #include "HAL/HAL_endstop_interrupts.h"
#endif

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  void M100_dump_routine(const char * const title, const char *start, const char *end);
#endif

#if ENABLED(SDSUPPORT)
  CardReader card;
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS)
  TWIBus i2c;
#endif

#if ENABLED(G38_PROBE_TARGET)
  bool G38_move = false,
       G38_endstop_hit = false;
#endif

#if ENABLED(DELTA)
  #include "module/delta.h"
#elif IS_SCARA
  #include "module/scara.h"
#endif

#if HAS_LEVELING
  #include "feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(SENSORLESS_HOMING)
  #include "feature/tmc2130.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE) && ENABLED(PAUSE_PARK_NO_STEPPER_TIMEOUT)
  #include "feature/pause.h"
#endif

#if ENABLED(TEMP_STAT_LEDS)
  #include "feature/leds/tempstat.h"
#endif

bool Running = true;

/**
 * axis_homed
 *   Flags that each linear axis was homed.
 *   XYZ on cartesian, ABC on delta, ABZ on SCARA.
 *
 * axis_known_position
 *   Flags that the position is known in each linear axis. Set when homed.
 *   Cleared whenever a stepper powers off, potentially losing its position.
 */
bool axis_homed[XYZ] = { false }, axis_known_position[XYZ] = { false };

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  TempUnit input_temp_units = TEMPUNIT_C;
#endif

// Initialized by settings.load()
float filament_size[EXTRUDERS], volumetric_multiplier[EXTRUDERS];

#if FAN_COUNT > 0
  int16_t fanSpeeds[FAN_COUNT] = { 0 };
  #if ENABLED(PROBING_FANS_OFF)
    bool fans_paused = false;
    int16_t paused_fanSpeeds[FAN_COUNT] = { 0 };
  #endif
#endif

// For M109 and M190, this flag may be cleared (by M108) to exit the wait loop
volatile bool wait_for_heatup = true;

// For M0/M1, this flag may be cleared (by M108) to exit the wait-for-user loop
#if HAS_RESUME_CONTINUE
  volatile bool wait_for_user = false;
#endif

// Inactivity shutdown
static millis_t max_inactive_time = 0;
static millis_t stepper_inactive_time = (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000UL;

#if ENABLED(Z_DUAL_ENDSTOPS)
  float z_endstop_adj;
#endif

#if ENABLED(BARICUDA)
  uint8_t baricuda_valve_pressure = 0,
          baricuda_e_to_p_pressure = 0;
#endif

#if HAS_POWER_SWITCH
  bool powersupply_on =
    #if ENABLED(PS_DEFAULT_OFF)
      false
    #else
      true
    #endif
  ;
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  static bool filament_ran_out = false;
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  AdvancedPauseMenuResponse advanced_pause_menu_response;
#endif

#if ENABLED(MIXING_EXTRUDER)
  float mixing_factor[MIXING_STEPPERS]; // Reciprocal of mix proportion. 0.0 = off, otherwise >= 1.0.
  #if MIXING_VIRTUAL_TOOLS > 1
    float mixing_virtual_tool_mix[MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];
  #endif
#endif

#ifdef CHDK
  millis_t chdkHigh = 0;
  bool chdkActive = false;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  int lpq_len = 20;
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  I2CPositionEncodersMgr I2CPEM;
  uint8_t blockBufferIndexRef = 0;
  millis_t lastUpdateMillis;
#endif

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

#if ENABLED(DIGIPOT_I2C)
  extern void digipot_i2c_set_current(uint8_t channel, float current);
  extern void digipot_i2c_init();
#endif

void setup_killpin() {
  #if HAS_KILL
    SET_INPUT_PULLUP(KILL_PIN);
  #endif
}

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

  void setup_filrunoutpin() {
    #if ENABLED(ENDSTOPPULLUP_FIL_RUNOUT)
      SET_INPUT_PULLUP(FIL_RUNOUT_PIN);
    #else
      SET_INPUT(FIL_RUNOUT_PIN);
    #endif
  }

#endif

void setup_powerhold() {
  #if HAS_SUICIDE
    OUT_WRITE(SUICIDE_PIN, HIGH);
  #endif
  #if HAS_POWER_SWITCH
    #if ENABLED(PS_DEFAULT_OFF)
      OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
    #else
      OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE);
    #endif
  #endif
}

void suicide() {
  #if HAS_SUICIDE
    OUT_WRITE(SUICIDE_PIN, LOW);
  #endif
}

#if HAS_SERVOS

  HAL_SERVO_LIB servo[NUM_SERVOS];

  void servo_init() {
    #if NUM_SERVOS >= 1 && HAS_SERVO_0
      servo[0].attach(SERVO0_PIN);
      servo[0].detach(); // Just set up the pin. We don't have a position yet. Don't move to a random position.
    #endif
    #if NUM_SERVOS >= 2 && HAS_SERVO_1
      servo[1].attach(SERVO1_PIN);
      servo[1].detach();
    #endif
    #if NUM_SERVOS >= 3 && HAS_SERVO_2
      servo[2].attach(SERVO2_PIN);
      servo[2].detach();
    #endif
    #if NUM_SERVOS >= 4 && HAS_SERVO_3
      servo[3].attach(SERVO3_PIN);
      servo[3].detach();
    #endif

    #if HAS_Z_SERVO_ENDSTOP
      servo_probe_init();
    #endif
  }

#endif // HAS_SERVOS

/**
 * Stepper Reset (RigidBoard, et.al.)
 */
#if HAS_STEPPER_RESET
  void disableStepperDrivers() {
    OUT_WRITE(STEPPER_RESET_PIN, LOW);  // drive it down to hold in reset motor driver chips
  }
  void enableStepperDrivers() { SET_INPUT(STEPPER_RESET_PIN); }  // set to input, which allows it to be pulled high by pullups
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0

  void i2c_on_receive(int bytes) { // just echo all bytes received to serial
    i2c.receive(bytes);
  }

  void i2c_on_request() {          // just send dummy data for now
    i2c.reply("Hello World!\n");
  }

#endif

#if ENABLED(MIXING_EXTRUDER)

  void normalize_mix() {
    float mix_total = 0.0;
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++) mix_total += RECIPROCAL(mixing_factor[i]);
    // Scale all values if they don't add up to ~1.0
    if (!NEAR(mix_total, 1.0)) {
      SERIAL_PROTOCOLLNPGM("Warning: Mix factors must add up to 1.0. Scaling.");
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++) mixing_factor[i] *= mix_total;
    }
  }

  #if ENABLED(DIRECT_MIXING_IN_G1)
    // Get mixing parameters from the GCode
    // The total "must" be 1.0 (but it will be normalized)
    // If no mix factors are given, the old mix is preserved
    void gcode_get_mix() {
      const char* mixing_codes = "ABCDHI";
      byte mix_bits = 0;
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++) {
        if (parser.seenval(mixing_codes[i])) {
          SBI(mix_bits, i);
          float v = parser.value_float();
          NOLESS(v, 0.0);
          mixing_factor[i] = RECIPROCAL(v);
        }
      }
      // If any mixing factors were included, clear the rest
      // If none were included, preserve the last mix
      if (mix_bits) {
        for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
          if (!TEST(mix_bits, i)) mixing_factor[i] = 0.0;
        normalize_mix();
      }
    }
  #endif

#endif

/**************************************************
 ***************** GCode Handlers *****************
 **************************************************/

/**
 * Sensitive pin test for M42, M226
 */
bool pin_is_protected(const int8_t pin) {
  static const int8_t sensitive_pins[] PROGMEM = SENSITIVE_PINS;
  for (uint8_t i = 0; i < COUNT(sensitive_pins); i++)
    if (pin == (int8_t)pgm_read_byte(&sensitive_pins[i])) return true;
  return false;
}

#if ENABLED(BARICUDA)
  #if HAS_HEATER_1
    #include "gcode/feature/baricuda/M126.h"
    #include "gcode/feature/baricuda/M127.h"
  #endif
  #if HAS_HEATER_2
    #include "gcode/feature/baricuda/M128.h"
    #include "gcode/feature/baricuda/M129.h"
  #endif
#endif

#if ENABLED(ULTIPANEL)
  #include "gcode/lcd/M145.h"
#endif

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  #include "gcode/units/M149.h"
#endif

#if HAS_POWER_SWITCH
  #include "gcode/control/M80.h"
#endif

#include "gcode/control/M81.h"

#include "gcode/units/M82_M83.h"

#include "gcode/control/M18_M84.h"

#include "gcode/control/M85.h"

#include "gcode/config/M92.h"

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  #include "gcode/calibrate/M100.h"
#endif

#include "gcode/host/M114.h"
#include "gcode/host/M115.h"

#include "gcode/lcd/M117.h"

#include "gcode/host/M118.h"
#include "gcode/host/M119.h"

#include "gcode/control/M120_M121.h"

#if HAS_COLOR_LEDS
  #include "gcode/feature/leds/M150.h"
#endif

#include "gcode/config/M201.h"

#if 0 // Not used for Sprinter/grbl gen6
  #include "gcode/config/M202.h"
#endif

#include "gcode/config/M203.h"
#include "gcode/config/M204.h"
#include "gcode/config/M205.h"

#if HAS_M206_COMMAND
  #include "gcode/geometry/M206.h"
#endif

#if IS_KINEMATIC
  #include "gcode/calibrate/M665.h"
#endif

#if ENABLED(DELTA) || ENABLED(Z_DUAL_ENDSTOPS)
  #include "gcode/calibrate/M666.h"
#endif

#include "gcode/control/M211.h"

#include "gcode/config/M220.h"

#include "gcode/control/M226.h"

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "gcode/feature/i2c/M260_M261.h"
#endif

#if HAS_SERVOS
  #include "gcode/control/M280.h"
#endif

#if HAS_BUZZER
  #include "gcode/lcd/M300.h"
#endif

#if ENABLED(PIDTEMP)
  #include "gcode/config/M301.h"
#endif

#if ENABLED(PIDTEMPBED)
  #include "gcode/config/M304.h"
#endif

#if defined(CHDK) || HAS_PHOTOGRAPH
  #include "gcode/feature/camera/M240.h"
#endif

#if HAS_LCD_CONTRAST
  #include "gcode/lcd/M250.h"
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  #include "gcode/config/M302.h"
#endif

#if ENABLED(MORGAN_SCARA)
  #include "gcode/scara/M360-M364.h"
#endif

#if ENABLED(EXT_SOLENOID)
  #include "gcode/control/M380_M381.h"
#endif

#include "gcode/control/M400.h"

#if HAS_BED_PROBE
  #include "gcode/probe/M401_M402.h"
#endif

void quickstop_stepper() {
  stepper.quick_stop();
  stepper.synchronize();
  set_current_from_steppers_for_axis(ALL_AXES);
  SYNC_PLAN_POSITION_KINEMATIC();
}

#if HAS_M206_COMMAND
  #include "gcode/geometry/M428.h"
#endif

#include "gcode/eeprom/M500.h"
#include "gcode/eeprom/M501.h"
#include "gcode/eeprom/M502.h"
#if DISABLED(DISABLE_M503)
  #include "gcode/eeprom/M503.h"
#endif

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  #include "gcode/config/M540.h"
#endif

#if ENABLED(MK2_MULTIPLEXER)
  #include "gcode/feature/snmm/M702.h"
#endif

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  #include "gcode/control/M605.h"
#endif

#if ENABLED(LIN_ADVANCE)
  #include "gcode/feature/advance/M900.h"
#endif

#if ENABLED(HAVE_TMC2130)
  #include "feature/tmc2130.h"
  #include "gcode/feature/trinamic/M906.h"
  #include "gcode/feature/trinamic/M911.h"
  #include "gcode/feature/trinamic/M912.h"
  #if ENABLED(HYBRID_THRESHOLD)
    #include "gcode/feature/trinamic/M913.h"
  #endif
  #if ENABLED(SENSORLESS_HOMING)
    #include "gcode/feature/trinamic/M914.h"
  #endif
#endif

#include "gcode/feature/digipot/M907.h"

#if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)
  #include "gcode/feature/digipot/M908.h"
  #if ENABLED(DAC_STEPPER_CURRENT) // As with Printrbot RevF
    #include "gcode/feature/digipot/M909.h"
    #include "gcode/feature/digipot/M910.h"
  #endif
#endif

#if HAS_MICROSTEPS
  #include "gcode/control/M350.h"
  #include "gcode/control/M351.h"
#endif

#include "gcode/feature/caselight/M355.h"

#if ENABLED(MIXING_EXTRUDER)
  #include "gcode/feature/mixing/M163.h"
  #if MIXING_VIRTUAL_TOOLS > 1
    #include "gcode/feature/mixing/M164.h"
  #endif
  #if ENABLED(DIRECT_MIXING_IN_G1)
    #include "gcode/feature/mixing/M165.h"
  #endif
#endif

#include "gcode/control/M999.h"

#include "gcode/control/T.h"

#if ENABLED(USE_CONTROLLER_FAN)

  void controllerFan() {
    static millis_t lastMotorOn = 0, // Last time a motor was turned on
                    nextMotorCheck = 0; // Last time the state was checked
    const millis_t ms = millis();
    if (ELAPSED(ms, nextMotorCheck)) {
      nextMotorCheck = ms + 2500UL; // Not a time critical function, so only check every 2.5s
      if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON || thermalManager.soft_pwm_amount_bed > 0
          || E0_ENABLE_READ == E_ENABLE_ON // If any of the drivers are enabled...
          #if E_STEPPERS > 1
            || E1_ENABLE_READ == E_ENABLE_ON
            #if HAS_X2_ENABLE
              || X2_ENABLE_READ == X_ENABLE_ON
            #endif
            #if E_STEPPERS > 2
              || E2_ENABLE_READ == E_ENABLE_ON
              #if E_STEPPERS > 3
                || E3_ENABLE_READ == E_ENABLE_ON
                #if E_STEPPERS > 4
                  || E4_ENABLE_READ == E_ENABLE_ON
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
      ) {
        lastMotorOn = ms; //... set time to NOW so the fan will turn on
      }

      // Fan off if no steppers have been enabled for CONTROLLERFAN_SECS seconds
      uint8_t speed = (!lastMotorOn || ELAPSED(ms, lastMotorOn + (CONTROLLERFAN_SECS) * 1000UL)) ? 0 : CONTROLLERFAN_SPEED;

      // allows digital or PWM fan output to be used (see M42 handling)
      WRITE(CONTROLLER_FAN_PIN, speed);
      analogWrite(CONTROLLER_FAN_PIN, speed);
    }
  }

#endif // USE_CONTROLLER_FAN

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

  void handle_filament_runout() {
    if (!filament_ran_out) {
      filament_ran_out = true;
      enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
      stepper.synchronize();
    }
  }

#endif // FILAMENT_RUNOUT_SENSOR

float calculate_volumetric_multiplier(const float diameter) {
  if (!parser.volumetric_enabled || diameter == 0) return 1.0;
  return 1.0 / (M_PI * sq(diameter * 0.5));
}

void calculate_volumetric_multipliers() {
  for (uint8_t i = 0; i < COUNT(filament_size); i++)
    volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
}

void enable_all_steppers() {
  enable_X();
  enable_Y();
  enable_Z();
  enable_E0();
  enable_E1();
  enable_E2();
  enable_E3();
  enable_E4();
}

void disable_e_steppers() {
  disable_E0();
  disable_E1();
  disable_E2();
  disable_E3();
  disable_E4();
}

void disable_all_steppers() {
  disable_X();
  disable_Y();
  disable_Z();
  disable_e_steppers();
}

/**
 * Manage several activities:
 *  - Check for Filament Runout
 *  - Keep the command buffer full
 *  - Check for maximum inactive time between commands
 *  - Check for maximum inactive time between stepper commands
 *  - Check if pin CHDK needs to go LOW
 *  - Check for KILL button held down
 *  - Check for HOME button held down
 *  - Check if cooling fan needs to be switched on
 *  - Check if an idle but hot extruder needs filament extruded (EXTRUDER_RUNOUT_PREVENT)
 */
void manage_inactivity(bool ignore_stepper_queue/*=false*/) {

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    if ((IS_SD_PRINTING || print_job_timer.isRunning()) && (READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING))
      handle_filament_runout();
  #endif

  if (commands_in_queue < BUFSIZE) get_available_commands();

  const millis_t ms = millis();

  if (max_inactive_time && ELAPSED(ms, gcode.previous_cmd_ms + max_inactive_time)) {
    SERIAL_ERROR_START();
    SERIAL_ECHOLNPAIR(MSG_KILL_INACTIVE_TIME, parser.command_ptr);
    kill(PSTR(MSG_KILLED));
  }

  // Prevent steppers timing-out in the middle of M600
  #if ENABLED(ADVANCED_PAUSE_FEATURE) && ENABLED(PAUSE_PARK_NO_STEPPER_TIMEOUT)
    #define MOVE_AWAY_TEST !move_away_flag
  #else
    #define MOVE_AWAY_TEST true
  #endif

  if (MOVE_AWAY_TEST && stepper_inactive_time && ELAPSED(ms, gcode.previous_cmd_ms + stepper_inactive_time)
      && !ignore_stepper_queue && !planner.blocks_queued()) {
    #if ENABLED(DISABLE_INACTIVE_X)
      disable_X();
    #endif
    #if ENABLED(DISABLE_INACTIVE_Y)
      disable_Y();
    #endif
    #if ENABLED(DISABLE_INACTIVE_Z)
      disable_Z();
    #endif
    #if ENABLED(DISABLE_INACTIVE_E)
      disable_e_steppers();
    #endif
    #if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(ULTRA_LCD)  // Only needed with an LCD
      ubl.lcd_map_control = defer_return_to_status = false;
    #endif
  }

  #ifdef CHDK // Check if pin should be set to LOW after M240 set it to HIGH
    if (chdkActive && ELAPSED(ms, chdkHigh + CHDK_DELAY)) {
      chdkActive = false;
      WRITE(CHDK, LOW);
    }
  #endif

  #if HAS_KILL

    // Check if the kill button was pressed and wait just in case it was an accidental
    // key kill key press
    // -------------------------------------------------------------------------------
    static int killCount = 0;   // make the inactivity button a bit less responsive
    const int KILL_DELAY = 750;
    if (!READ(KILL_PIN))
      killCount++;
    else if (killCount > 0)
      killCount--;

    // Exceeded threshold and we can confirm that it was not accidental
    // KILL the machine
    // ----------------------------------------------------------------
    if (killCount >= KILL_DELAY) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_KILL_BUTTON);
      kill(PSTR(MSG_KILLED));
    }
  #endif

  #if HAS_HOME
    // Check to see if we have to home, use poor man's debouncer
    // ---------------------------------------------------------
    static int homeDebounceCount = 0;   // poor man's debouncing count
    const int HOME_DEBOUNCE_DELAY = 2500;
    if (!IS_SD_PRINTING && !READ(HOME_PIN)) {
      if (!homeDebounceCount) {
        enqueue_and_echo_commands_P(PSTR("G28"));
        LCD_MESSAGEPGM(MSG_AUTO_HOME);
      }
      if (homeDebounceCount < HOME_DEBOUNCE_DELAY)
        homeDebounceCount++;
      else
        homeDebounceCount = 0;
    }
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    controllerFan(); // Check if fan should be turned on to cool stepper drivers down
  #endif

  #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
    if (ELAPSED(ms, gcode.previous_cmd_ms + (EXTRUDER_RUNOUT_SECONDS) * 1000UL)
      && thermalManager.degHotend(active_extruder) > EXTRUDER_RUNOUT_MINTEMP) {
      #if ENABLED(SWITCHING_EXTRUDER)
        const bool oldstatus = E0_ENABLE_READ;
        enable_E0();
      #else // !SWITCHING_EXTRUDER
        bool oldstatus;
        switch (active_extruder) {
          default: oldstatus = E0_ENABLE_READ; enable_E0(); break;
          #if E_STEPPERS > 1
            case 1: oldstatus = E1_ENABLE_READ; enable_E1(); break;
            #if E_STEPPERS > 2
              case 2: oldstatus = E2_ENABLE_READ; enable_E2(); break;
              #if E_STEPPERS > 3
                case 3: oldstatus = E3_ENABLE_READ; enable_E3(); break;
                #if E_STEPPERS > 4
                  case 4: oldstatus = E4_ENABLE_READ; enable_E4(); break;
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #endif // !SWITCHING_EXTRUDER

      gcode.refresh_cmd_timeout()

      const float olde = current_position[E_AXIS];
      current_position[E_AXIS] += EXTRUDER_RUNOUT_EXTRUDE;
      planner.buffer_line_kinematic(current_position, MMM_TO_MMS(EXTRUDER_RUNOUT_SPEED), active_extruder);
      current_position[E_AXIS] = olde;
      planner.set_e_position_mm(olde);
      stepper.synchronize();
      #if ENABLED(SWITCHING_EXTRUDER)
        E0_ENABLE_WRITE(oldstatus);
      #else
        switch (active_extruder) {
          case 0: E0_ENABLE_WRITE(oldstatus); break;
          #if E_STEPPERS > 1
            case 1: E1_ENABLE_WRITE(oldstatus); break;
            #if E_STEPPERS > 2
              case 2: E2_ENABLE_WRITE(oldstatus); break;
              #if E_STEPPERS > 3
                case 3: E3_ENABLE_WRITE(oldstatus); break;
                #if E_STEPPERS > 4
                  case 4: E4_ENABLE_WRITE(oldstatus); break;
                #endif // E_STEPPERS > 4
              #endif // E_STEPPERS > 3
            #endif // E_STEPPERS > 2
          #endif // E_STEPPERS > 1
        }
      #endif // !SWITCHING_EXTRUDER
    }
  #endif // EXTRUDER_RUNOUT_PREVENT

  #if ENABLED(DUAL_X_CARRIAGE)
    // handle delayed move timeout
    if (delayed_move_time && ELAPSED(ms, delayed_move_time + 1000UL) && IsRunning()) {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      set_destination_to_current();
      prepare_move_to_destination();
    }
  #endif

  #if ENABLED(TEMP_STAT_LEDS)
    handle_status_leds();
  #endif

  #if ENABLED(HAVE_TMC2130)
    tmc2130_checkOverTemp();
  #endif

  planner.check_axes_activity();
}

/**
 * Standard idle routine keeps the machine alive
 */
void idle(
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    bool no_stepper_sleep/*=false*/
  #endif
) {
  #if ENABLED(MAX7219_DEBUG)
    Max7219_idle_tasks();
  #endif  // MAX7219_DEBUG

  lcd_update();

  #if ENABLED(HOST_KEEPALIVE_FEATURE)
    gcode.host_keepalive();
  #endif

  #if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)
    thermalManager.auto_report_temperatures();
  #endif

  manage_inactivity(
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      no_stepper_sleep
    #endif
  );

  thermalManager.manage_heater();

  #if ENABLED(PRINTCOUNTER)
    print_job_timer.tick();
  #endif

  #if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
    buzzer.tick();
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    if (planner.blocks_queued() &&
        ( (blockBufferIndexRef != planner.block_buffer_head) ||
          ((lastUpdateMillis + I2CPE_MIN_UPD_TIME_MS) < millis())) ) {
      blockBufferIndexRef = planner.block_buffer_head;
      I2CPEM.update();
      lastUpdateMillis = millis();
    }
  #endif
}

/**
 * Kill all activity and lock the machine.
 * After this the machine will need to be reset.
 */
void kill(const char* lcd_msg) {
  SERIAL_ERROR_START();
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);

  thermalManager.disable_all_heaters();
  disable_all_steppers();

  #if ENABLED(ULTRA_LCD)
    kill_screen(lcd_msg);
  #else
    UNUSED(lcd_msg);
  #endif

  _delay_ms(600); // Wait a short time (allows messages to get out before shutting down.
  cli(); // Stop interrupts

  _delay_ms(250); //Wait to ensure all interrupts routines stopped
  thermalManager.disable_all_heaters(); //turn off heaters again

  #ifdef ACTION_ON_KILL
    SERIAL_ECHOLNPGM("//action:" ACTION_ON_KILL);
  #endif

  #if HAS_POWER_SWITCH
    SET_INPUT(PS_ON_PIN);
  #endif

  suicide();
  while (1) {
    #if ENABLED(USE_WATCHDOG)
      watchdog_reset();
    #endif
  } // Wait for reset
}

/**
 * Turn off heaters and stop the print in progress
 * After a stop the machine may be resumed with M999
 */
void stop() {
  thermalManager.disable_all_heaters(); // 'unpause' taken care of in here

  #if ENABLED(PROBING_FANS_OFF)
    if (fans_paused) fans_pause(false); // put things back the way they were
  #endif

  if (IsRunning()) {
    Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
    safe_delay(350);       // allow enough time for messages to get out before stopping
    Running = false;
  }
}

/**
 * Marlin entry-point: Set up before the program loop
 *  - Set up the kill pin, filament runout, power hold
 *  - Start the serial port
 *  - Print startup messages and diagnostics
 *  - Get EEPROM or default settings
 *  - Initialize managers for:
 *    • temperature
 *    • planner
 *    • watchdog
 *    • stepper
 *    • photo pin
 *    • servos
 *    • LCD controller
 *    • Digipot I2C
 *    • Z probe sled
 *    • status LEDs
 */
void setup() {

  #if ENABLED(MAX7219_DEBUG)
    Max7219_init();
  #endif

  #ifdef DISABLE_JTAG
    // Disable JTAG on AT90USB chips to free up pins for IO
    MCUCR = 0x80;
    MCUCR = 0x80;
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    setup_filrunoutpin();
  #endif

  setup_killpin();

  setup_powerhold();

  #if HAS_STEPPER_RESET
    disableStepperDrivers();
  #endif

  MYSERIAL.begin(BAUDRATE);
  while(!MYSERIAL);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START();

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = HAL_get_reset_source();
  if (mcu &  1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if (mcu &  2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if (mcu &  4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if (mcu &  8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if (mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  HAL_clear_reset_source();

  #if ENABLED(USE_WATCHDOG) //reinit watchdog after HAL_get_reset_source call
    watchdog_init();
  #endif

  SERIAL_ECHOPGM(MSG_MARLIN);
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(SHORT_BUILD_VERSION);
  SERIAL_EOL();

  #if defined(STRING_DISTRIBUTION_DATE) && defined(STRING_CONFIG_H_AUTHOR)
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
    SERIAL_ECHOPGM(STRING_DISTRIBUTION_DATE);
    SERIAL_ECHOLNPGM(MSG_AUTHOR STRING_CONFIG_H_AUTHOR);
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM("Compiled: " __DATE__);
  #endif

  SERIAL_ECHO_START();
  SERIAL_ECHOPAIR(MSG_FREE_MEMORY, freeMemory());
  SERIAL_ECHOLNPAIR(MSG_PLANNER_BUFFER_BYTES, (int)sizeof(block_t)*BLOCK_BUFFER_SIZE);

  queue_setup();

  // Load data from EEPROM if available (or use defaults)
  // This also updates variables in the planner, elsewhere
  (void)settings.load();

  #if HAS_M206_COMMAND
    // Initialize current position based on home_offset
    COPY(current_position, home_offset);
  #else
    ZERO(current_position);
  #endif

  // Vital to init stepper/planner equivalent for current_position
  SYNC_PLAN_POSITION_KINEMATIC();

  thermalManager.init();    // Initialize temperature loop

  stepper.init();    // Initialize stepper, this enables interrupts!

  #if HAS_SERVOS
    servo_init();
  #endif

  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif

  #if HAS_CASE_LIGHT
    case_light_on = CASE_LIGHT_DEFAULT_ON;
    case_light_brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS;
    update_case_light();
  #endif

  #if ENABLED(SPINDLE_LASER_ENABLE)
    OUT_WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);  // init spindle to off
    #if SPINDLE_DIR_CHANGE
      OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);  // init rotation to clockwise (M3)
    #endif
    #if ENABLED(SPINDLE_LASER_PWM) && defined(SPINDLE_LASER_PWM_PIN) && SPINDLE_LASER_PWM_PIN >= 0
      SET_OUTPUT(SPINDLE_LASER_PWM_PIN);
      analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);  // set to lowest speed
    #endif
  #endif

  #if HAS_BED_PROBE
    endstops.enable_z_probe(false);
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    SET_OUTPUT(CONTROLLER_FAN_PIN); //Set pin used for driver cooling fan
  #endif

  #if HAS_STEPPER_RESET
    enableStepperDrivers();
  #endif

  #if ENABLED(DIGIPOT_I2C)
    digipot_i2c_init();
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    dac_init();
  #endif

  #if (ENABLED(Z_PROBE_SLED) || ENABLED(SOLENOID_PROBE)) && HAS_SOLENOID_1
    OUT_WRITE(SOL1_PIN, LOW); // turn it off
  #endif

  #if HAS_HOME
    SET_INPUT_PULLUP(HOME_PIN);
  #endif

  #if PIN_EXISTS(STAT_LED_RED)
    OUT_WRITE(STAT_LED_RED_PIN, LOW); // turn it off
  #endif

  #if PIN_EXISTS(STAT_LED_BLUE)
    OUT_WRITE(STAT_LED_BLUE_PIN, LOW); // turn it off
  #endif

  #if ENABLED(NEOPIXEL_RGBW_LED)
    SET_OUTPUT(NEOPIXEL_PIN);
    setup_neopixel();
  #endif

  #if ENABLED(RGB_LED) || ENABLED(RGBW_LED)
    SET_OUTPUT(RGB_LED_R_PIN);
    SET_OUTPUT(RGB_LED_G_PIN);
    SET_OUTPUT(RGB_LED_B_PIN);
    #if ENABLED(RGBW_LED)
      SET_OUTPUT(RGB_LED_W_PIN);
    #endif
  #endif

  #if ENABLED(MK2_MULTIPLEXER)
    SET_OUTPUT(E_MUX0_PIN);
    SET_OUTPUT(E_MUX1_PIN);
    SET_OUTPUT(E_MUX2_PIN);
  #endif

  #if HAS_FANMUX
    fanmux_init();
  #endif

  lcd_init();

  #ifndef CUSTOM_BOOTSCREEN_TIMEOUT
    #define CUSTOM_BOOTSCREEN_TIMEOUT 2500
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    #if ENABLED(DOGLCD)                           // On DOGM the first bootscreen is already drawn
      #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
        safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);    // Custom boot screen pause
        lcd_bootscreen();                         // Show Marlin boot screen
      #endif
      safe_delay(BOOTSCREEN_TIMEOUT);             // Pause
    #elif ENABLED(ULTRA_LCD)
      lcd_bootscreen();
      #if DISABLED(SDSUPPORT)
        lcd_init();
      #endif
    #endif
  #endif

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    // Initialize mixing to 100% color 1
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
      mixing_factor[i] = (i == 0) ? 1.0 : 0.0;
    for (uint8_t t = 0; t < MIXING_VIRTUAL_TOOLS; t++)
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        mixing_virtual_tool_mix[t][i] = mixing_factor[i];
  #endif

  #if ENABLED(BLTOUCH)
    bltouch_init();
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    I2CPEM.init();
  #endif

  #if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
    i2c.onReceive(i2c_on_receive);
    i2c.onRequest(i2c_on_request);
  #endif

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    setup_endstop_interrupts();
  #endif

  #if ENABLED(SWITCHING_EXTRUDER) && !DONT_SWITCH
    move_extruder_servo(0);  // Initialize extruder servo
  #endif

  #if ENABLED(SWITCHING_NOZZLE)
    move_nozzle_servo(0);  // Initialize nozzle servo
  #endif

  #if ENABLED(PARKING_EXTRUDER)
    #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
      pe_activate_magnet(0);
      pe_activate_magnet(1);
    #else
      pe_deactivate_magnet(0);
      pe_deactivate_magnet(1);
    #endif
  #endif
}

/**
 * The main Marlin program loop
 *
 *  - Save or log commands to SD
 *  - Process available commands (if not saving)
 *  - Call heater manager
 *  - Call inactivity manager
 *  - Call endstop manager
 *  - Call LCD update
 */
void loop() {
  if (commands_in_queue < BUFSIZE) get_available_commands();

  #if ENABLED(SDSUPPORT)
    card.checkautostart(false);
  #endif

  advance_command_queue();

  endstops.report_state();
  idle();
}
