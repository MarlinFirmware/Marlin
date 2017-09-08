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
#include "module/temperature.h"
#include "sd/cardreader.h"
#include "module/configuration_store.h"
#ifdef ARDUINO
  #include <pins_arduino.h>
#endif
#include <math.h>
#include "libs/nozzle.h"
#include "libs/duration_t.h"

#include "gcode/gcode.h"
#include "gcode/parser.h"
#include "gcode/queue.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "libs/buzzer.h"
#endif

#if HAS_ABL
  #include "libs/vector_3.h"
  #if ENABLED(AUTO_BED_LEVELING_LINEAR)
    #include "libs/least_squares_fit.h"
  #endif
#elif ENABLED(MESH_BED_LEVELING)
  #include "feature/mbl/mesh_bed_leveling.h"
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

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "feature/ubl/ubl.h"
  extern bool defer_return_to_status;
  unified_bed_leveling ubl;
  #define UBL_MESH_VALID !( ( ubl.z_values[0][0] == ubl.z_values[0][1] && ubl.z_values[0][1] == ubl.z_values[0][2] \
                           && ubl.z_values[1][0] == ubl.z_values[1][1] && ubl.z_values[1][1] == ubl.z_values[1][2] \
                           && ubl.z_values[2][0] == ubl.z_values[2][1] && ubl.z_values[2][1] == ubl.z_values[2][2] \
                           && ubl.z_values[0][0] == 0 && ubl.z_values[1][0] == 0 && ubl.z_values[2][0] == 0 )  \
                           || isnan(ubl.z_values[0][0]))
#endif

#if ENABLED(SENSORLESS_HOMING)
  #include "feature/tmc2130.h"
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

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
static const float homing_feedrate_mm_s[] PROGMEM = {
  #if ENABLED(DELTA)
    MMM_TO_MMS(HOMING_FEEDRATE_Z), MMM_TO_MMS(HOMING_FEEDRATE_Z),
  #else
    MMM_TO_MMS(HOMING_FEEDRATE_XY), MMM_TO_MMS(HOMING_FEEDRATE_XY),
  #endif
  MMM_TO_MMS(HOMING_FEEDRATE_Z), 0
};
FORCE_INLINE float homing_feedrate(const AxisEnum a) { return pgm_read_float(&homing_feedrate_mm_s[a]); }

static float saved_feedrate_mm_s;
int16_t feedrate_percentage = 100, saved_feedrate_percentage,
    flow_percentage[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(100);

// Initialized by settings.load()
bool volumetric_enabled;
float filament_size[EXTRUDERS], volumetric_multiplier[EXTRUDERS];

#if HAS_WORKSPACE_OFFSET
  #if HAS_POSITION_SHIFT
    // The distance that XYZ has been offset by G92. Reset by G28.
    float position_shift[XYZ] = { 0 };
  #endif
  #if HAS_HOME_OFFSET
    // This offset is added to the configured home position.
    // Set by M206, M428, or menu item. Saved to EEPROM.
    float home_offset[XYZ] = { 0 };
  #endif
  #if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
    // The above two are combined to save on computes
    float workspace_offset[XYZ] = { 0 };
  #endif
#endif

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
millis_t previous_cmd_ms = 0;
static millis_t max_inactive_time = 0;
static millis_t stepper_inactive_time = (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000UL;

// Print Job Timer
#if ENABLED(PRINTCOUNTER)
  PrintCounter print_job_timer = PrintCounter();
#else
  Stopwatch print_job_timer = Stopwatch();
#endif

#if HAS_BED_PROBE
  float zprobe_zoffset; // Initialized by settings.load()
#endif

#if HAS_ABL
  float xy_probe_feedrate_mm_s = MMM_TO_MMS(XY_PROBE_SPEED);
  #define XY_PROBE_FEEDRATE_MM_S xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_SPEED)
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_SPEED)
#else
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE()
#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #if ENABLED(DELTA)
    #define ADJUST_DELTA(V) \
      if (planner.abl_enabled) { \
        const float zadj = bilinear_z_offset(V); \
        delta[A_AXIS] += zadj; \
        delta[B_AXIS] += zadj; \
        delta[C_AXIS] += zadj; \
      }
  #else
    #define ADJUST_DELTA(V) if (planner.abl_enabled) { delta[Z_AXIS] += bilinear_z_offset(V); }
  #endif
#elif IS_KINEMATIC
  #define ADJUST_DELTA(V) NOOP
#endif

#if ENABLED(Z_DUAL_ENDSTOPS)
  float z_endstop_adj;
#endif

// Extruder offsets
#if HOTENDS > 1
  float hotend_offset[XYZ][HOTENDS]; // Initialized by settings.load()
#endif

#if HAS_Z_SERVO_ENDSTOP
  const int z_servo_angle[2] = Z_SERVO_ANGLES;
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

#if ENABLED(DELTA)

  float delta[ABC],
        endstop_adj[ABC] = { 0 };

  // Initialized by settings.load()
  float delta_radius,
        delta_tower_angle_trim[2],
        delta_tower[ABC][2],
        delta_diagonal_rod,
        delta_calibration_radius,
        delta_diagonal_rod_2_tower[ABC],
        delta_segments_per_second,
        delta_clip_start_height = Z_MAX_POS;

  float delta_safe_distance_from_top();

#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  int bilinear_grid_spacing[2], bilinear_start[2];
  float bilinear_grid_factor[2],
        z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
#endif

#if IS_SCARA
  // Float constants for SCARA calculations
  const float L1 = SCARA_LINKAGE_1, L2 = SCARA_LINKAGE_2,
              L1_2 = sq(float(L1)), L1_2_2 = 2.0 * L1_2,
              L2_2 = sq(float(L2));

  float delta_segments_per_second = SCARA_SEGMENTS_PER_SECOND,
        delta[ABC];
#endif

float cartes[XYZ] = { 0 };

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  bool filament_sensor = false;                                 // M405 turns on filament sensor control. M406 turns it off.
  float filament_width_nominal = DEFAULT_NOMINAL_FILAMENT_DIA,  // Nominal filament width. Change with M404.
        filament_width_meas = DEFAULT_MEASURED_FILAMENT_DIA;    // Measured filament diameter
  uint8_t meas_delay_cm = MEASUREMENT_DELAY_CM,                 // Distance delay setting
          measurement_delay[MAX_MEASUREMENT_DELAY + 1];         // Ring buffer to delayed measurement. Store extruder factor after subtracting 100
  int8_t filwidth_delay_index[2] = { 0, -1 };                   // Indexes into ring buffer
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

#if HAS_SERVOS
  HAL_SERVO_LIB servo[NUM_SERVOS];
  #define MOVE_SERVO(I, P) servo[I].move(P)
  #if HAS_Z_SERVO_ENDSTOP
    #define DEPLOY_Z_SERVO() MOVE_SERVO(Z_ENDSTOP_SERVO_NR, z_servo_angle[0])
    #define STOW_Z_SERVO() MOVE_SERVO(Z_ENDSTOP_SERVO_NR, z_servo_angle[1])
  #endif
#endif

#ifdef CHDK
  millis_t chdkHigh = 0;
  bool chdkActive = false;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  int lpq_len = 20;
#endif

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  MarlinBusyState busy_state = NOT_BUSY;
  static millis_t next_busy_signal_ms = 0;
  uint8_t host_keepalive_interval = DEFAULT_KEEPALIVE_INTERVAL;
#else
  #define host_keepalive() NOOP
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  I2CPositionEncodersMgr I2CPEM;
  uint8_t blockBufferIndexRef = 0;
  millis_t lastUpdateMillis;
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)
  static WorkspacePlane workspace_plane = PLANE_XY;
#endif

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

void stop();

void get_cartesian_from_steppers();
void set_current_from_steppers_for_axis(const AxisEnum axis);

#if ENABLED(BEZIER_CURVE_SUPPORT)
  void plan_cubic_move(const float offset[4]);
#endif

void report_current_position();

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
    /**
     * Set position of Z Servo Endstop
     *
     * The servo might be deployed and positioned too low to stow
     * when starting up the machine or rebooting the board.
     * There's no way to know where the nozzle is positioned until
     * homing has been done - no homing with z-probe without init!
     *
     */
    STOW_Z_SERVO();
  #endif
}

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

#if HAS_WORKSPACE_OFFSET || ENABLED(DUAL_X_CARRIAGE)

  /**
   * Software endstops can be used to monitor the open end of
   * an axis that has a hardware endstop on the other end. Or
   * they can prevent axes from moving past endstops and grinding.
   *
   * To keep doing their job as the coordinate system changes,
   * the software endstop positions must be refreshed to remain
   * at the same positions relative to the machine.
   */
  void update_software_endstops(const AxisEnum axis) {
    const float offs = 0.0
      #if HAS_HOME_OFFSET
        + home_offset[axis]
      #endif
      #if HAS_POSITION_SHIFT
        + position_shift[axis]
      #endif
    ;

    #if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
      workspace_offset[axis] = offs;
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      if (axis == X_AXIS) {

        // In Dual X mode hotend_offset[X] is T1's home position
        float dual_max_x = max(hotend_offset[X_AXIS][1], X2_MAX_POS);

        if (active_extruder != 0) {
          // T1 can move from X2_MIN_POS to X2_MAX_POS or X2 home position (whichever is larger)
          soft_endstop_min[X_AXIS] = X2_MIN_POS + offs;
          soft_endstop_max[X_AXIS] = dual_max_x + offs;
        }
        else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
          // In Duplication Mode, T0 can move as far left as X_MIN_POS
          // but not so far to the right that T1 would move past the end
          soft_endstop_min[X_AXIS] = base_min_pos(X_AXIS) + offs;
          soft_endstop_max[X_AXIS] = min(base_max_pos(X_AXIS), dual_max_x - duplicate_extruder_x_offset) + offs;
        }
        else {
          // In other modes, T0 can move from X_MIN_POS to X_MAX_POS
          soft_endstop_min[axis] = base_min_pos(axis) + offs;
          soft_endstop_max[axis] = base_max_pos(axis) + offs;
        }
      }
    #elif ENABLED(DELTA)
      soft_endstop_min[axis] = base_min_pos(axis) + (axis == Z_AXIS ? 0 : offs);
      soft_endstop_max[axis] = base_max_pos(axis) + offs;
    #else
      soft_endstop_min[axis] = base_min_pos(axis) + offs;
      soft_endstop_max[axis] = base_max_pos(axis) + offs;
    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("For ", axis_codes[axis]);
        #if HAS_HOME_OFFSET
          SERIAL_ECHOPAIR(" axis:\n home_offset = ", home_offset[axis]);
        #endif
        #if HAS_POSITION_SHIFT
          SERIAL_ECHOPAIR("\n position_shift = ", position_shift[axis]);
        #endif
        SERIAL_ECHOPAIR("\n soft_endstop_min = ", soft_endstop_min[axis]);
        SERIAL_ECHOLNPAIR("\n soft_endstop_max = ", soft_endstop_max[axis]);
      }
    #endif

    #if ENABLED(DELTA)
      if (axis == Z_AXIS)
        delta_clip_start_height = soft_endstop_max[axis] - delta_safe_distance_from_top();
    #endif
  }

#endif // HAS_WORKSPACE_OFFSET || DUAL_X_CARRIAGE

#if HAS_M206_COMMAND
  /**
   * Change the home offset for an axis, update the current
   * position and the software endstops to retain the same
   * relative distance to the new home.
   *
   * Since this changes the current_position, code should
   * call sync_plan_position soon after this.
   */
  static void set_home_offset(const AxisEnum axis, const float v) {
    current_position[axis] += v - home_offset[axis];
    home_offset[axis] = v;
    update_software_endstops(axis);
  }
#endif // HAS_M206_COMMAND

/**
 * Set an axis' current position to its home position (after homing).
 *
 * For Core and Cartesian robots this applies one-to-one when an
 * individual axis has been homed.
 *
 * DELTA should wait until all homing is done before setting the XYZ
 * current_position to home, because homing is a single operation.
 * In the case where the axis positions are already known and previously
 * homed, DELTA could home to X or Y individually by moving either one
 * to the center. However, homing Z always homes XY and Z.
 *
 * SCARA should wait until all XY homing is done before setting the XY
 * current_position to home, because neither X nor Y is at home until
 * both are at home. Z can however be homed individually.
 *
 * Callers must sync the planner position after calling this!
 */
static void set_axis_is_at_home(const AxisEnum axis) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> set_axis_is_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  axis_known_position[axis] = axis_homed[axis] = true;

  #if HAS_POSITION_SHIFT
    position_shift[axis] = 0;
    update_software_endstops(axis);
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    if (axis == X_AXIS && (active_extruder == 1 || dual_x_carriage_mode == DXC_DUPLICATION_MODE)) {
      current_position[X_AXIS] = x_home_pos(active_extruder);
      return;
    }
  #endif

  #if ENABLED(MORGAN_SCARA)

    /**
     * Morgan SCARA homes XY at the same time
     */
    if (axis == X_AXIS || axis == Y_AXIS) {

      float homeposition[XYZ];
      LOOP_XYZ(i) homeposition[i] = LOGICAL_POSITION(base_home_pos((AxisEnum)i), i);

      // SERIAL_ECHOPAIR("homeposition X:", homeposition[X_AXIS]);
      // SERIAL_ECHOLNPAIR(" Y:", homeposition[Y_AXIS]);

      /**
       * Get Home position SCARA arm angles using inverse kinematics,
       * and calculate homing offset using forward kinematics
       */
      inverse_kinematics(homeposition);
      forward_kinematics_SCARA(delta[A_AXIS], delta[B_AXIS]);

      // SERIAL_ECHOPAIR("Cartesian X:", cartes[X_AXIS]);
      // SERIAL_ECHOLNPAIR(" Y:", cartes[Y_AXIS]);

      current_position[axis] = LOGICAL_POSITION(cartes[axis], axis);

      /**
       * SCARA home positions are based on configuration since the actual
       * limits are determined by the inverse kinematic transform.
       */
      soft_endstop_min[axis] = base_min_pos(axis); // + (cartes[axis] - base_home_pos(axis));
      soft_endstop_max[axis] = base_max_pos(axis); // + (cartes[axis] - base_home_pos(axis));
    }
    else
  #endif
  {
    current_position[axis] = LOGICAL_POSITION(base_home_pos(axis), axis);
  }

  /**
   * Z Probe Z Homing? Account for the probe's Z offset.
   */
  #if HAS_BED_PROBE && Z_HOME_DIR < 0
    if (axis == Z_AXIS) {
      #if HOMING_Z_WITH_PROBE

        current_position[Z_AXIS] -= zprobe_zoffset;

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("*** Z HOMED WITH PROBE (Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN) ***");
            SERIAL_ECHOLNPAIR("> zprobe_zoffset = ", zprobe_zoffset);
          }
        #endif

      #elif ENABLED(DEBUG_LEVELING_FEATURE)

        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("*** Z HOMED TO ENDSTOP (Z_MIN_PROBE_ENDSTOP) ***");

      #endif
    }
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      #if HAS_HOME_OFFSET
        SERIAL_ECHOPAIR("> home_offset[", axis_codes[axis]);
        SERIAL_ECHOLNPAIR("] = ", home_offset[axis]);
      #endif
      DEBUG_POS("", current_position);
      SERIAL_ECHOPAIR("<<< set_axis_is_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    I2CPEM.homed(axis);
  #endif
}

/**
 * Some planner shorthand inline functions
 */
inline float get_homing_bump_feedrate(const AxisEnum axis) {
  static const uint8_t homing_bump_divisor[] PROGMEM = HOMING_BUMP_DIVISOR;
  uint8_t hbd = pgm_read_byte(&homing_bump_divisor[axis]);
  if (hbd < 1) {
    hbd = 10;
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM("Warning: Homing Bump Divisor < 1");
  }
  return homing_feedrate(axis) / hbd;
}

/**
 *  Plan a move to (X, Y, Z) and set the current_position
 *  The final current_position may not be the one that was requested
 */
void do_blocking_move_to(const float &lx, const float &ly, const float &lz, const float &fr_mm_s/*=0.0*/) {
  const float old_feedrate_mm_s = feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) print_xyz(PSTR(">>> do_blocking_move_to"), NULL, lx, ly, lz);
  #endif

  #if ENABLED(DELTA)

    if (!position_is_reachable_xy(lx, ly)) return;

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;

    set_destination_to_current();          // sync destination at the start

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("set_destination_to_current", destination);
    #endif

    // when in the danger zone
    if (current_position[Z_AXIS] > delta_clip_start_height) {
      if (lz > delta_clip_start_height) {   // staying in the danger zone
        destination[X_AXIS] = lx;           // move directly (uninterpolated)
        destination[Y_AXIS] = ly;
        destination[Z_AXIS] = lz;
        prepare_uninterpolated_move_to_destination(); // set_current_to_destination
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("danger zone move", current_position);
        #endif
        return;
      }
      else {
        destination[Z_AXIS] = delta_clip_start_height;
        prepare_uninterpolated_move_to_destination(); // set_current_to_destination
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("zone border move", current_position);
        #endif
      }
    }

    if (lz > current_position[Z_AXIS]) {    // raising?
      destination[Z_AXIS] = lz;
      prepare_uninterpolated_move_to_destination();   // set_current_to_destination
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z raise move", current_position);
      #endif
    }

    destination[X_AXIS] = lx;
    destination[Y_AXIS] = ly;
    prepare_move_to_destination();         // set_current_to_destination
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("xy move", current_position);
    #endif

    if (lz < current_position[Z_AXIS]) {    // lowering?
      destination[Z_AXIS] = lz;
      prepare_uninterpolated_move_to_destination();   // set_current_to_destination
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z lower move", current_position);
      #endif
    }

  #elif IS_SCARA

    if (!position_is_reachable_xy(lx, ly)) return;

    set_destination_to_current();

    // If Z needs to raise, do it before moving XY
    if (destination[Z_AXIS] < lz) {
      destination[Z_AXIS] = lz;
      prepare_uninterpolated_move_to_destination(fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS));
    }

    destination[X_AXIS] = lx;
    destination[Y_AXIS] = ly;
    prepare_uninterpolated_move_to_destination(fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S);

    // If Z needs to lower, do it after moving XY
    if (destination[Z_AXIS] > lz) {
      destination[Z_AXIS] = lz;
      prepare_uninterpolated_move_to_destination(fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS));
    }

  #else

    // If Z needs to raise, do it before moving XY
    if (current_position[Z_AXIS] < lz) {
      feedrate_mm_s = fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS);
      current_position[Z_AXIS] = lz;
      line_to_current_position();
    }

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;
    current_position[X_AXIS] = lx;
    current_position[Y_AXIS] = ly;
    line_to_current_position();

    // If Z needs to lower, do it after moving XY
    if (current_position[Z_AXIS] > lz) {
      feedrate_mm_s = fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS);
      current_position[Z_AXIS] = lz;
      line_to_current_position();
    }

  #endif

  stepper.synchronize();

  feedrate_mm_s = old_feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< do_blocking_move_to");
  #endif
}
void do_blocking_move_to_x(const float &lx, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(lx, current_position[Y_AXIS], current_position[Z_AXIS], fr_mm_s);
}
void do_blocking_move_to_z(const float &lz, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], lz, fr_mm_s);
}
void do_blocking_move_to_xy(const float &lx, const float &ly, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(lx, ly, current_position[Z_AXIS], fr_mm_s);
}

//
// Prepare to do endstop or probe moves
// with custom feedrates.
//
//  - Save current feedrates
//  - Reset the rate multiplier
//  - Reset the command timeout
//  - Enable the endstops (for endstop moves)
//
static void setup_for_endstop_or_probe_move() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("setup_for_endstop_or_probe_move", current_position);
  #endif
  saved_feedrate_mm_s = feedrate_mm_s;
  saved_feedrate_percentage = feedrate_percentage;
  feedrate_percentage = 100;
  gcode.refresh_cmd_timeout();
}

static void clean_up_after_endstop_or_probe_move() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("clean_up_after_endstop_or_probe_move", current_position);
  #endif
  feedrate_mm_s = saved_feedrate_mm_s;
  feedrate_percentage = saved_feedrate_percentage;
  gcode.refresh_cmd_timeout();
}

#if HAS_BED_PROBE
  /**
   * Raise Z to a minimum height to make room for a probe to move
   */
  inline void do_probe_raise(const float z_raise) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("do_probe_raise(", z_raise);
        SERIAL_CHAR(')');
        SERIAL_EOL();
      }
    #endif

    float z_dest = z_raise;
    if (zprobe_zoffset < 0) z_dest -= zprobe_zoffset;

    if (z_dest > current_position[Z_AXIS])
      do_blocking_move_to_z(z_dest);
  }

#endif // HAS_BED_PROBE

#if HAS_PROBING_PROCEDURE || HOTENDS > 1 || ENABLED(Z_PROBE_ALLEN_KEY) || ENABLED(Z_PROBE_SLED) || ENABLED(NOZZLE_CLEAN_FEATURE) || ENABLED(NOZZLE_PARK_FEATURE) || ENABLED(DELTA_AUTO_CALIBRATION)

  bool axis_unhomed_error(const bool x/*=true*/, const bool y/*=true*/, const bool z/*=true*/) {
    #if ENABLED(HOME_AFTER_DEACTIVATE)
      const bool xx = x && !axis_known_position[X_AXIS],
                 yy = y && !axis_known_position[Y_AXIS],
                 zz = z && !axis_known_position[Z_AXIS];
    #else
      const bool xx = x && !axis_homed[X_AXIS],
                 yy = y && !axis_homed[Y_AXIS],
                 zz = z && !axis_homed[Z_AXIS];
    #endif
    if (xx || yy || zz) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM(MSG_HOME " ");
      if (xx) SERIAL_ECHOPGM(MSG_X);
      if (yy) SERIAL_ECHOPGM(MSG_Y);
      if (zz) SERIAL_ECHOPGM(MSG_Z);
      SERIAL_ECHOLNPGM(" " MSG_FIRST);

      #if ENABLED(ULTRA_LCD)
        lcd_status_printf_P(0, PSTR(MSG_HOME " %s%s%s " MSG_FIRST), xx ? MSG_X : "", yy ? MSG_Y : "", zz ? MSG_Z : "");
      #endif
      return true;
    }
    return false;
  }

#endif

#if ENABLED(Z_PROBE_SLED)

  #ifndef SLED_DOCKING_OFFSET
    #define SLED_DOCKING_OFFSET 0
  #endif

  /**
   * Method to dock/undock a sled designed by Charles Bell.
   *
   * stow[in]     If false, move to MAX_X and engage the solenoid
   *              If true, move to MAX_X and release the solenoid
   */
  static void dock_sled(bool stow) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("dock_sled(", stow);
        SERIAL_CHAR(')');
        SERIAL_EOL();
      }
    #endif

    // Dock sled a bit closer to ensure proper capturing
    do_blocking_move_to_x(X_MAX_POS + SLED_DOCKING_OFFSET - ((stow) ? 1 : 0));

    #if HAS_SOLENOID_1 && DISABLED(EXT_SOLENOID)
      WRITE(SOL1_PIN, !stow); // switch solenoid
    #endif
  }

#elif ENABLED(Z_PROBE_ALLEN_KEY)

  FORCE_INLINE void do_blocking_move_to(const float logical[XYZ], const float &fr_mm_s) {
    do_blocking_move_to(logical[X_AXIS], logical[Y_AXIS], logical[Z_AXIS], fr_mm_s);
  }

  void run_deploy_moves_script() {
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE 0.0
      #endif
      const float deploy_1[] = { Z_PROBE_ALLEN_KEY_DEPLOY_1_X, Z_PROBE_ALLEN_KEY_DEPLOY_1_Y, Z_PROBE_ALLEN_KEY_DEPLOY_1_Z };
      do_blocking_move_to(deploy_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE 0.0
      #endif
      const float deploy_2[] = { Z_PROBE_ALLEN_KEY_DEPLOY_2_X, Z_PROBE_ALLEN_KEY_DEPLOY_2_Y, Z_PROBE_ALLEN_KEY_DEPLOY_2_Z };
      do_blocking_move_to(deploy_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE 0.0
      #endif
      const float deploy_3[] = { Z_PROBE_ALLEN_KEY_DEPLOY_3_X, Z_PROBE_ALLEN_KEY_DEPLOY_3_Y, Z_PROBE_ALLEN_KEY_DEPLOY_3_Z };
      do_blocking_move_to(deploy_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE 0.0
      #endif
      const float deploy_4[] = { Z_PROBE_ALLEN_KEY_DEPLOY_4_X, Z_PROBE_ALLEN_KEY_DEPLOY_4_Y, Z_PROBE_ALLEN_KEY_DEPLOY_4_Z };
      do_blocking_move_to(deploy_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE 0.0
      #endif
      const float deploy_5[] = { Z_PROBE_ALLEN_KEY_DEPLOY_5_X, Z_PROBE_ALLEN_KEY_DEPLOY_5_Y, Z_PROBE_ALLEN_KEY_DEPLOY_5_Z };
      do_blocking_move_to(deploy_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE));
    #endif
  }

  void run_stow_moves_script() {
    #if defined(Z_PROBE_ALLEN_KEY_STOW_1_X) || defined(Z_PROBE_ALLEN_KEY_STOW_1_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_1_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_X
        #define Z_PROBE_ALLEN_KEY_STOW_1_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_Y
        #define Z_PROBE_ALLEN_KEY_STOW_1_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_Z
        #define Z_PROBE_ALLEN_KEY_STOW_1_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE 0.0
      #endif
      const float stow_1[] = { Z_PROBE_ALLEN_KEY_STOW_1_X, Z_PROBE_ALLEN_KEY_STOW_1_Y, Z_PROBE_ALLEN_KEY_STOW_1_Z };
      do_blocking_move_to(stow_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_2_X) || defined(Z_PROBE_ALLEN_KEY_STOW_2_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_2_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_X
        #define Z_PROBE_ALLEN_KEY_STOW_2_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_Y
        #define Z_PROBE_ALLEN_KEY_STOW_2_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_Z
        #define Z_PROBE_ALLEN_KEY_STOW_2_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE 0.0
      #endif
      const float stow_2[] = { Z_PROBE_ALLEN_KEY_STOW_2_X, Z_PROBE_ALLEN_KEY_STOW_2_Y, Z_PROBE_ALLEN_KEY_STOW_2_Z };
      do_blocking_move_to(stow_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_3_X) || defined(Z_PROBE_ALLEN_KEY_STOW_3_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_3_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_X
        #define Z_PROBE_ALLEN_KEY_STOW_3_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_Y
        #define Z_PROBE_ALLEN_KEY_STOW_3_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_Z
        #define Z_PROBE_ALLEN_KEY_STOW_3_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE 0.0
      #endif
      const float stow_3[] = { Z_PROBE_ALLEN_KEY_STOW_3_X, Z_PROBE_ALLEN_KEY_STOW_3_Y, Z_PROBE_ALLEN_KEY_STOW_3_Z };
      do_blocking_move_to(stow_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_4_X) || defined(Z_PROBE_ALLEN_KEY_STOW_4_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_4_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_X
        #define Z_PROBE_ALLEN_KEY_STOW_4_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_Y
        #define Z_PROBE_ALLEN_KEY_STOW_4_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_Z
        #define Z_PROBE_ALLEN_KEY_STOW_4_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE 0.0
      #endif
      const float stow_4[] = { Z_PROBE_ALLEN_KEY_STOW_4_X, Z_PROBE_ALLEN_KEY_STOW_4_Y, Z_PROBE_ALLEN_KEY_STOW_4_Z };
      do_blocking_move_to(stow_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_5_X) || defined(Z_PROBE_ALLEN_KEY_STOW_5_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_5_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_X
        #define Z_PROBE_ALLEN_KEY_STOW_5_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_Y
        #define Z_PROBE_ALLEN_KEY_STOW_5_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_Z
        #define Z_PROBE_ALLEN_KEY_STOW_5_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE 0.0
      #endif
      const float stow_5[] = { Z_PROBE_ALLEN_KEY_STOW_5_X, Z_PROBE_ALLEN_KEY_STOW_5_Y, Z_PROBE_ALLEN_KEY_STOW_5_Z };
      do_blocking_move_to(stow_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE));
    #endif
  }

#endif

#if ENABLED(PROBING_FANS_OFF)

  void fans_pause(const bool p) {
    if (p != fans_paused) {
      fans_paused = p;
      if (p)
        for (uint8_t x = 0; x < FAN_COUNT; x++) {
          paused_fanSpeeds[x] = fanSpeeds[x];
          fanSpeeds[x] = 0;
        }
      else
        for (uint8_t x = 0; x < FAN_COUNT; x++)
          fanSpeeds[x] = paused_fanSpeeds[x];
    }
  }

#endif // PROBING_FANS_OFF

#if HAS_BED_PROBE

  // TRIGGERED_WHEN_STOWED_TEST can easily be extended to servo probes, ... if needed.
  #if ENABLED(PROBE_IS_TRIGGERED_WHEN_STOWED_TEST)
    #if ENABLED(Z_MIN_PROBE_ENDSTOP)
      #define _TRIGGERED_WHEN_STOWED_TEST (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING)
    #else
      #define _TRIGGERED_WHEN_STOWED_TEST (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING)
    #endif
  #endif

  #if QUIET_PROBING
    void probing_pause(const bool p) {
      #if ENABLED(PROBING_HEATERS_OFF)
        thermalManager.pause(p);
      #endif
      #if ENABLED(PROBING_FANS_OFF)
        fans_pause(p);
      #endif
      if (p) safe_delay(
        #if DELAY_BEFORE_PROBING > 25
          DELAY_BEFORE_PROBING
        #else
          25
        #endif
      );
    }
  #endif // QUIET_PROBING

  #if ENABLED(BLTOUCH)

    void bltouch_command(int angle) {
      MOVE_SERVO(Z_ENDSTOP_SERVO_NR, angle);  // Give the BL-Touch the command and wait
      safe_delay(BLTOUCH_DELAY);
    }

    bool set_bltouch_deployed(const bool deploy) {
      if (deploy && TEST_BLTOUCH()) {      // If BL-Touch says it's triggered
        bltouch_command(BLTOUCH_RESET);    //  try to reset it.
        bltouch_command(BLTOUCH_DEPLOY);   // Also needs to deploy and stow to
        bltouch_command(BLTOUCH_STOW);     //  clear the triggered condition.
        safe_delay(1500);                  // Wait for internal self-test to complete.
                                           //  (Measured completion time was 0.65 seconds
                                           //   after reset, deploy, and stow sequence)
        if (TEST_BLTOUCH()) {              // If it still claims to be triggered...
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM(MSG_STOP_BLTOUCH);
          stop();                          // punt!
          return true;
        }
      }

      bltouch_command(deploy ? BLTOUCH_DEPLOY : BLTOUCH_STOW);

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOPAIR("set_bltouch_deployed(", deploy);
          SERIAL_CHAR(')');
          SERIAL_EOL();
        }
      #endif

      return false;
    }

  #endif // BLTOUCH

  // returns false for ok and true for failure
  bool set_probe_deployed(bool deploy) {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        DEBUG_POS("set_probe_deployed", current_position);
        SERIAL_ECHOLNPAIR("deploy: ", deploy);
      }
    #endif

    if (endstops.z_probe_enabled == deploy) return false;

    // Make room for probe
    do_probe_raise(_Z_CLEARANCE_DEPLOY_PROBE);

    #if ENABLED(Z_PROBE_SLED) || ENABLED(Z_PROBE_ALLEN_KEY)
      #if ENABLED(Z_PROBE_SLED)
        #define _AUE_ARGS true, false, false
      #else
        #define _AUE_ARGS
      #endif
      if (axis_unhomed_error(_AUE_ARGS)) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_STOP_UNHOMED);
        stop();
        return true;
      }
    #endif

    const float oldXpos = current_position[X_AXIS],
                oldYpos = current_position[Y_AXIS];

    #ifdef _TRIGGERED_WHEN_STOWED_TEST

      // If endstop is already false, the Z probe is deployed
      if (_TRIGGERED_WHEN_STOWED_TEST == deploy) {     // closed after the probe specific actions.
                                                       // Would a goto be less ugly?
        //while (!_TRIGGERED_WHEN_STOWED_TEST) idle(); // would offer the opportunity
                                                       // for a triggered when stowed manual probe.

        if (!deploy) endstops.enable_z_probe(false); // Switch off triggered when stowed probes early
                                                     // otherwise an Allen-Key probe can't be stowed.
    #endif

        #if ENABLED(SOLENOID_PROBE)

          #if HAS_SOLENOID_1
            WRITE(SOL1_PIN, deploy);
          #endif

        #elif ENABLED(Z_PROBE_SLED)

          dock_sled(!deploy);

        #elif HAS_Z_SERVO_ENDSTOP && DISABLED(BLTOUCH)

          MOVE_SERVO(Z_ENDSTOP_SERVO_NR, z_servo_angle[deploy ? 0 : 1]);

        #elif ENABLED(Z_PROBE_ALLEN_KEY)

          deploy ? run_deploy_moves_script() : run_stow_moves_script();

        #endif

    #ifdef _TRIGGERED_WHEN_STOWED_TEST
      } // _TRIGGERED_WHEN_STOWED_TEST == deploy

      if (_TRIGGERED_WHEN_STOWED_TEST == deploy) { // State hasn't changed?

        if (IsRunning()) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM("Z-Probe failed");
          LCD_ALERTMESSAGEPGM("Err: ZPROBE");
        }
        stop();
        return true;

      } // _TRIGGERED_WHEN_STOWED_TEST == deploy

    #endif

    do_blocking_move_to(oldXpos, oldYpos, current_position[Z_AXIS]); // return to position before deploy
    endstops.enable_z_probe(deploy);
    return false;
  }

  /**
   * @brief Used by run_z_probe to do a single Z probe move.
   *
   * @param  z        Z destination
   * @param  fr_mm_s  Feedrate in mm/s
   * @return true to indicate an error
   */
  static bool do_probe_move(const float z, const float fr_mm_m) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS(">>> do_probe_move", current_position);
    #endif

    // Deploy BLTouch at the start of any probe
    #if ENABLED(BLTOUCH)
      if (set_bltouch_deployed(true)) return true;
    #endif

    #if QUIET_PROBING
      probing_pause(true);
    #endif

    // Move down until probe triggered
    do_blocking_move_to_z(z, MMM_TO_MMS(fr_mm_m));

    // Check to see if the probe was triggered
    const bool probe_triggered = TEST(Endstops::endstop_hit_bits,
      #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
        Z_MIN
      #else
        Z_MIN_PROBE
      #endif
    );

    #if QUIET_PROBING
      probing_pause(false);
    #endif

    // Retract BLTouch immediately after a probe if it was triggered
    #if ENABLED(BLTOUCH)
      if (probe_triggered && set_bltouch_deployed(false)) return true;
    #endif

    // Clear endstop flags
    endstops.hit_on_purpose();

    // Get Z where the steppers were interrupted
    set_current_from_steppers_for_axis(Z_AXIS);

    // Tell the planner where we actually are
    SYNC_PLAN_POSITION_KINEMATIC();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("<<< do_probe_move", current_position);
    #endif

    return !probe_triggered;
  }

  /**
   * @details Used by probe_pt to do a single Z probe.
   *          Leaves current_position[Z_AXIS] at the height where the probe triggered.
   *
   * @param  short_move Flag for a shorter probe move towards the bed
   * @return The raw Z position where the probe was triggered
   */
  static float run_z_probe(const bool short_move=true) {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS(">>> run_z_probe", current_position);
    #endif

    // Prevent stepper_inactive_time from running out and EXTRUDER_RUNOUT_PREVENT from extruding
    gcode.refresh_cmd_timeout();

    #if ENABLED(PROBE_DOUBLE_TOUCH)

      // Do a first probe at the fast speed
      if (do_probe_move(-10, Z_PROBE_SPEED_FAST)) return NAN;

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        float first_probe_z = current_position[Z_AXIS];
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("1st Probe Z:", first_probe_z);
      #endif

      // move up to make clearance for the probe
      do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));

    #else

      // If the nozzle is above the travel height then
      // move down quickly before doing the slow probe
      float z = Z_CLEARANCE_DEPLOY_PROBE;
      if (zprobe_zoffset < 0) z -= zprobe_zoffset;

      if (z < current_position[Z_AXIS]) {

        // If we don't make it to the z position (i.e. the probe triggered), move up to make clearance for the probe
        if (!do_probe_move(z, Z_PROBE_SPEED_FAST))
          do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      }
    #endif

    // move down slowly to find bed
    if (do_probe_move(-10 + (short_move ? 0 : -(Z_MAX_LENGTH)), Z_PROBE_SPEED_SLOW)) return NAN;

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("<<< run_z_probe", current_position);
    #endif

    // Debug: compare probe heights
    #if ENABLED(PROBE_DOUBLE_TOUCH) && ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("2nd Probe Z:", current_position[Z_AXIS]);
        SERIAL_ECHOLNPAIR(" Discrepancy:", first_probe_z - current_position[Z_AXIS]);
      }
    #endif

    return RAW_CURRENT_POSITION(Z) + zprobe_zoffset
      #if ENABLED(DELTA)
        + home_offset[Z_AXIS] // Account for delta height adjustment
      #endif
    ;
  }

  /**
   * - Move to the given XY
   * - Deploy the probe, if not already deployed
   * - Probe the bed, get the Z position
   * - Depending on the 'stow' flag
   *   - Stow the probe, or
   *   - Raise to the BETWEEN height
   * - Return the probed Z position
   */
  float probe_pt(const float &lx, const float &ly, const bool stow, const uint8_t verbose_level, const bool printable=true) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR(">>> probe_pt(", lx);
        SERIAL_ECHOPAIR(", ", ly);
        SERIAL_ECHOPAIR(", ", stow ? "" : "no ");
        SERIAL_ECHOLNPGM("stow)");
        DEBUG_POS("", current_position);
      }
    #endif

    const float nx = lx - (X_PROBE_OFFSET_FROM_EXTRUDER), ny = ly - (Y_PROBE_OFFSET_FROM_EXTRUDER);

    if (printable
      ? !position_is_reachable_xy(nx, ny)
      : !position_is_reachable_by_probe_xy(lx, ly)
    ) return NAN;


    const float old_feedrate_mm_s = feedrate_mm_s;

    #if ENABLED(DELTA)
      if (current_position[Z_AXIS] > delta_clip_start_height)
        do_blocking_move_to_z(delta_clip_start_height);
    #endif

    #if HAS_SOFTWARE_ENDSTOPS
      // Store the status of the soft endstops and disable if we're probing a non-printable location
      static bool enable_soft_endstops = soft_endstops_enabled;
      if (!printable) soft_endstops_enabled = false;
    #endif

    feedrate_mm_s = XY_PROBE_FEEDRATE_MM_S;

    // Move the probe to the given XY
    do_blocking_move_to_xy(nx, ny);

    float measured_z = NAN;
    if (!DEPLOY_PROBE()) {
      measured_z = run_z_probe(printable);

      if (!stow)
        do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      else
        if (STOW_PROBE()) measured_z = NAN;
    }

    #if HAS_SOFTWARE_ENDSTOPS
      // Restore the soft endstop status
      soft_endstops_enabled = enable_soft_endstops;
    #endif

    if (verbose_level > 2) {
      SERIAL_PROTOCOLPGM("Bed X: ");
      SERIAL_PROTOCOL_F(lx, 3);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL_F(ly, 3);
      SERIAL_PROTOCOLPGM(" Z: ");
      SERIAL_PROTOCOL_F(measured_z, 3);
      SERIAL_EOL();
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< probe_pt");
    #endif

    feedrate_mm_s = old_feedrate_mm_s;

    if (isnan(measured_z)) {
      LCD_MESSAGEPGM(MSG_ERR_PROBING_FAILED);
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_PROBING_FAILED);
    }

    return measured_z;
  }

#endif // HAS_BED_PROBE

#if HAS_LEVELING

  bool leveling_is_valid() {
    return
      #if ENABLED(MESH_BED_LEVELING)
        mbl.has_mesh()
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
        !!bilinear_grid_spacing[X_AXIS]
      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        true
      #else // 3POINT, LINEAR
        true
      #endif
    ;
  }

  bool leveling_is_active() {
    return
      #if ENABLED(MESH_BED_LEVELING)
        mbl.active()
      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.state.active
      #else
        planner.abl_enabled
      #endif
    ;
  }

  /**
   * Turn bed leveling on or off, fixing the current
   * position as-needed.
   *
   * Disable: Current position = physical position
   *  Enable: Current position = "unleveled" physical position
   */
  void set_bed_leveling_enabled(const bool enable/*=true*/) {

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      const bool can_change = (!enable || leveling_is_valid());
    #else
      constexpr bool can_change = true;
    #endif

    if (can_change && enable != leveling_is_active()) {

      #if ENABLED(MESH_BED_LEVELING)

        if (!enable)
          planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

        const bool enabling = enable && leveling_is_valid();
        mbl.set_active(enabling);
        if (enabling) planner.unapply_leveling(current_position);

      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        #if PLANNER_LEVELING
          if (ubl.state.active) {                       // leveling from on to off
            // change unleveled current_position to physical current_position without moving steppers.
            planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);
            ubl.state.active = false;                   // disable only AFTER calling apply_leveling
          }
          else {                                        // leveling from off to on
            ubl.state.active = true;                    // enable BEFORE calling unapply_leveling, otherwise ignored
            // change physical current_position to unleveled current_position without moving steppers.
            planner.unapply_leveling(current_position);
          }
        #else
          ubl.state.active = enable;                    // just flip the bit, current_position will be wrong until next move.
        #endif

      #else // ABL

        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          // Force bilinear_z_offset to re-calculate next time
          const float reset[XYZ] = { -9999.999, -9999.999, 0 };
          (void)bilinear_z_offset(reset);
        #endif

        // Enable or disable leveling compensation in the planner
        planner.abl_enabled = enable;

        if (!enable)
          // When disabling just get the current position from the steppers.
          // This will yield the smallest error when first converted back to steps.
          set_current_from_steppers_for_axis(
            #if ABL_PLANAR
              ALL_AXES
            #else
              Z_AXIS
            #endif
          );
        else
          // When enabling, remove compensation from the current position,
          // so compensation will give the right stepper counts.
          planner.unapply_leveling(current_position);

      #endif // ABL
    }
  }

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

    void set_z_fade_height(const float zfh) {

      const bool level_active = leveling_is_active();

      #if ENABLED(AUTO_BED_LEVELING_UBL)

        if (level_active)
          set_bed_leveling_enabled(false);  // turn off before changing fade height for proper apply/unapply leveling to maintain current_position
        planner.z_fade_height = zfh;
        planner.inverse_z_fade_height = RECIPROCAL(zfh);
        if (level_active)
          set_bed_leveling_enabled(true);  // turn back on after changing fade height

      #else

        planner.z_fade_height = zfh;
        planner.inverse_z_fade_height = RECIPROCAL(zfh);

        if (level_active) {
          set_current_from_steppers_for_axis(
            #if ABL_PLANAR
              ALL_AXES
            #else
              Z_AXIS
            #endif
          );
        }
      #endif
    }

  #endif // LEVELING_FADE_HEIGHT

  /**
   * Reset calibration results to zero.
   */
  void reset_bed_level() {
    set_bed_leveling_enabled(false);
    #if ENABLED(MESH_BED_LEVELING)
      if (leveling_is_valid()) {
        mbl.reset();
        mbl.set_has_mesh(false);
      }
    #else
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("reset_bed_level");
      #endif
      #if ABL_PLANAR
        planner.bed_level_matrix.set_to_identity();
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
        bilinear_start[X_AXIS] = bilinear_start[Y_AXIS] =
        bilinear_grid_spacing[X_AXIS] = bilinear_grid_spacing[Y_AXIS] = 0;
        for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
          for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
            z_values[x][y] = NAN;
      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.reset();
      #endif
    #endif
  }

#endif // HAS_LEVELING

#if ENABLED(AUTO_BED_LEVELING_BILINEAR) || ENABLED(MESH_BED_LEVELING)

  /**
   * Enable to produce output in JSON format suitable
   * for SCAD or JavaScript mesh visualizers.
   *
   * Visualize meshes in OpenSCAD using the included script.
   *
   *   buildroot/shared/scripts/MarlinMesh.scad
   */
  //#define SCAD_MESH_OUTPUT

  /**
   * Print calibration results for plotting or manual frame adjustment.
   */
  static void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, float (*fn)(const uint8_t, const uint8_t)) {
    #ifndef SCAD_MESH_OUTPUT
      for (uint8_t x = 0; x < sx; x++) {
        for (uint8_t i = 0; i < precision + 2 + (x < 10 ? 1 : 0); i++)
          SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOL((int)x);
      }
      SERIAL_EOL();
    #endif
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_PROTOCOLLNPGM("measured_z = ["); // open 2D array
    #endif
    for (uint8_t y = 0; y < sy; y++) {
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_PROTOCOLPGM(" [");           // open sub-array
      #else
        if (y < 10) SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOL((int)y);
      #endif
      for (uint8_t x = 0; x < sx; x++) {
        SERIAL_PROTOCOLCHAR(' ');
        const float offset = fn(x, y);
        if (!isnan(offset)) {
          if (offset >= 0) SERIAL_PROTOCOLCHAR('+');
          SERIAL_PROTOCOL_F(offset, precision);
        }
        else {
          #ifdef SCAD_MESH_OUTPUT
            for (uint8_t i = 3; i < precision + 3; i++)
              SERIAL_PROTOCOLCHAR(' ');
            SERIAL_PROTOCOLPGM("NAN");
          #else
            for (uint8_t i = 0; i < precision + 3; i++)
              SERIAL_PROTOCOLCHAR(i ? '=' : ' ');
          #endif
        }
        #ifdef SCAD_MESH_OUTPUT
          if (x < sx - 1) SERIAL_PROTOCOLCHAR(',');
        #endif
      }
      #ifdef SCAD_MESH_OUTPUT
        SERIAL_PROTOCOLCHAR(' ');
        SERIAL_PROTOCOLCHAR(']');                     // close sub-array
        if (y < sy - 1) SERIAL_PROTOCOLCHAR(',');
      #endif
      SERIAL_EOL();
    }
    #ifdef SCAD_MESH_OUTPUT
      SERIAL_PROTOCOLPGM("];");                       // close 2D array
    #endif
    SERIAL_EOL();
  }

#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

  /**
   * Extrapolate a single point from its neighbors
   */
  static void extrapolate_one_point(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPGM("Extrapolate [");
        if (x < 10) SERIAL_CHAR(' ');
        SERIAL_ECHO((int)x);
        SERIAL_CHAR(xdir ? (xdir > 0 ? '+' : '-') : ' ');
        SERIAL_CHAR(' ');
        if (y < 10) SERIAL_CHAR(' ');
        SERIAL_ECHO((int)y);
        SERIAL_CHAR(ydir ? (ydir > 0 ? '+' : '-') : ' ');
        SERIAL_CHAR(']');
      }
    #endif
    if (!isnan(z_values[x][y])) {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM(" (done)");
      #endif
      return;  // Don't overwrite good values.
    }
    SERIAL_EOL();

    // Get X neighbors, Y neighbors, and XY neighbors
    const uint8_t x1 = x + xdir, y1 = y + ydir, x2 = x1 + xdir, y2 = y1 + ydir;
    float a1 = z_values[x1][y ], a2 = z_values[x2][y ],
          b1 = z_values[x ][y1], b2 = z_values[x ][y2],
          c1 = z_values[x1][y1], c2 = z_values[x2][y2];

    // Treat far unprobed points as zero, near as equal to far
    if (isnan(a2)) a2 = 0.0; if (isnan(a1)) a1 = a2;
    if (isnan(b2)) b2 = 0.0; if (isnan(b1)) b1 = b2;
    if (isnan(c2)) c2 = 0.0; if (isnan(c1)) c1 = c2;

    const float a = 2 * a1 - a2, b = 2 * b1 - b2, c = 2 * c1 - c2;

    // Take the average instead of the median
    z_values[x][y] = (a + b + c) / 3.0;

    // Median is robust (ignores outliers).
    // z_values[x][y] = (a < b) ? ((b < c) ? b : (c < a) ? a : c)
    //                                : ((c < b) ? b : (a < c) ? a : c);
  }

  //Enable this if your SCARA uses 180° of total area
  //#define EXTRAPOLATE_FROM_EDGE

  #if ENABLED(EXTRAPOLATE_FROM_EDGE)
    #if GRID_MAX_POINTS_X < GRID_MAX_POINTS_Y
      #define HALF_IN_X
    #elif GRID_MAX_POINTS_Y < GRID_MAX_POINTS_X
      #define HALF_IN_Y
    #endif
  #endif

  /**
   * Fill in the unprobed points (corners of circular print surface)
   * using linear extrapolation, away from the center.
   */
  static void extrapolate_unprobed_bed_level() {
    #ifdef HALF_IN_X
      constexpr uint8_t ctrx2 = 0, xlen = GRID_MAX_POINTS_X - 1;
    #else
      constexpr uint8_t ctrx1 = (GRID_MAX_POINTS_X - 1) / 2, // left-of-center
                        ctrx2 = (GRID_MAX_POINTS_X) / 2,     // right-of-center
                        xlen = ctrx1;
    #endif

    #ifdef HALF_IN_Y
      constexpr uint8_t ctry2 = 0, ylen = GRID_MAX_POINTS_Y - 1;
    #else
      constexpr uint8_t ctry1 = (GRID_MAX_POINTS_Y - 1) / 2, // top-of-center
                        ctry2 = (GRID_MAX_POINTS_Y) / 2,     // bottom-of-center
                        ylen = ctry1;
    #endif

    for (uint8_t xo = 0; xo <= xlen; xo++)
      for (uint8_t yo = 0; yo <= ylen; yo++) {
        uint8_t x2 = ctrx2 + xo, y2 = ctry2 + yo;
        #ifndef HALF_IN_X
          const uint8_t x1 = ctrx1 - xo;
        #endif
        #ifndef HALF_IN_Y
          const uint8_t y1 = ctry1 - yo;
          #ifndef HALF_IN_X
            extrapolate_one_point(x1, y1, +1, +1);   //  left-below + +
          #endif
          extrapolate_one_point(x2, y1, -1, +1);     // right-below - +
        #endif
        #ifndef HALF_IN_X
          extrapolate_one_point(x1, y2, +1, -1);     //  left-above + -
        #endif
        extrapolate_one_point(x2, y2, -1, -1);       // right-above - -
      }

  }

  static void print_bilinear_leveling_grid() {
    SERIAL_ECHOLNPGM("Bilinear Leveling Grid:");
    print_2d_array(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y, 3,
      [](const uint8_t ix, const uint8_t iy) { return z_values[ix][iy]; }
    );
  }

  #if ENABLED(ABL_BILINEAR_SUBDIVISION)

    #define ABL_GRID_POINTS_VIRT_X (GRID_MAX_POINTS_X - 1) * (BILINEAR_SUBDIVISIONS) + 1
    #define ABL_GRID_POINTS_VIRT_Y (GRID_MAX_POINTS_Y - 1) * (BILINEAR_SUBDIVISIONS) + 1
    #define ABL_TEMP_POINTS_X (GRID_MAX_POINTS_X + 2)
    #define ABL_TEMP_POINTS_Y (GRID_MAX_POINTS_Y + 2)
    float z_values_virt[ABL_GRID_POINTS_VIRT_X][ABL_GRID_POINTS_VIRT_Y];
    int bilinear_grid_spacing_virt[2] = { 0 };
    float bilinear_grid_factor_virt[2] = { 0 };

    static void print_bilinear_leveling_grid_virt() {
      SERIAL_ECHOLNPGM("Subdivided with CATMULL ROM Leveling Grid:");
      print_2d_array(ABL_GRID_POINTS_VIRT_X, ABL_GRID_POINTS_VIRT_Y, 5,
        [](const uint8_t ix, const uint8_t iy) { return z_values_virt[ix][iy]; }
      );
    }

    #define LINEAR_EXTRAPOLATION(E, I) ((E) * 2 - (I))
    float bed_level_virt_coord(const uint8_t x, const uint8_t y) {
      uint8_t ep = 0, ip = 1;
      if (!x || x == ABL_TEMP_POINTS_X - 1) {
        if (x) {
          ep = GRID_MAX_POINTS_X - 1;
          ip = GRID_MAX_POINTS_X - 2;
        }
        if (WITHIN(y, 1, ABL_TEMP_POINTS_Y - 2))
          return LINEAR_EXTRAPOLATION(
            z_values[ep][y - 1],
            z_values[ip][y - 1]
          );
        else
          return LINEAR_EXTRAPOLATION(
            bed_level_virt_coord(ep + 1, y),
            bed_level_virt_coord(ip + 1, y)
          );
      }
      if (!y || y == ABL_TEMP_POINTS_Y - 1) {
        if (y) {
          ep = GRID_MAX_POINTS_Y - 1;
          ip = GRID_MAX_POINTS_Y - 2;
        }
        if (WITHIN(x, 1, ABL_TEMP_POINTS_X - 2))
          return LINEAR_EXTRAPOLATION(
            z_values[x - 1][ep],
            z_values[x - 1][ip]
          );
        else
          return LINEAR_EXTRAPOLATION(
            bed_level_virt_coord(x, ep + 1),
            bed_level_virt_coord(x, ip + 1)
          );
      }
      return z_values[x - 1][y - 1];
    }

    static float bed_level_virt_cmr(const float p[4], const uint8_t i, const float t) {
      return (
          p[i-1] * -t * sq(1 - t)
        + p[i]   * (2 - 5 * sq(t) + 3 * t * sq(t))
        + p[i+1] * t * (1 + 4 * t - 3 * sq(t))
        - p[i+2] * sq(t) * (1 - t)
      ) * 0.5;
    }

    static float bed_level_virt_2cmr(const uint8_t x, const uint8_t y, const float &tx, const float &ty) {
      float row[4], column[4];
      for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
          column[j] = bed_level_virt_coord(i + x - 1, j + y - 1);
        }
        row[i] = bed_level_virt_cmr(column, 1, ty);
      }
      return bed_level_virt_cmr(row, 1, tx);
    }

    void bed_level_virt_interpolate() {
      bilinear_grid_spacing_virt[X_AXIS] = bilinear_grid_spacing[X_AXIS] / (BILINEAR_SUBDIVISIONS);
      bilinear_grid_spacing_virt[Y_AXIS] = bilinear_grid_spacing[Y_AXIS] / (BILINEAR_SUBDIVISIONS);
      bilinear_grid_factor_virt[X_AXIS] = RECIPROCAL(bilinear_grid_spacing_virt[X_AXIS]);
      bilinear_grid_factor_virt[Y_AXIS] = RECIPROCAL(bilinear_grid_spacing_virt[Y_AXIS]);
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
          for (uint8_t ty = 0; ty < BILINEAR_SUBDIVISIONS; ty++)
            for (uint8_t tx = 0; tx < BILINEAR_SUBDIVISIONS; tx++) {
              if ((ty && y == GRID_MAX_POINTS_Y - 1) || (tx && x == GRID_MAX_POINTS_X - 1))
                continue;
              z_values_virt[x * (BILINEAR_SUBDIVISIONS) + tx][y * (BILINEAR_SUBDIVISIONS) + ty] =
                bed_level_virt_2cmr(
                  x + 1,
                  y + 1,
                  (float)tx / (BILINEAR_SUBDIVISIONS),
                  (float)ty / (BILINEAR_SUBDIVISIONS)
                );
            }
    }
  #endif // ABL_BILINEAR_SUBDIVISION

  // Refresh after other values have been updated
  void refresh_bed_level() {
    bilinear_grid_factor[X_AXIS] = RECIPROCAL(bilinear_grid_spacing[X_AXIS]);
    bilinear_grid_factor[Y_AXIS] = RECIPROCAL(bilinear_grid_spacing[Y_AXIS]);
    #if ENABLED(ABL_BILINEAR_SUBDIVISION)
      bed_level_virt_interpolate();
    #endif
  }

#endif // AUTO_BED_LEVELING_BILINEAR

/**
 * Home an individual linear axis
 */
static void do_homing_move(const AxisEnum axis, const float distance, const float fr_mm_s=0.0) {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> do_homing_move(", axis_codes[axis]);
      SERIAL_ECHOPAIR(", ", distance);
      SERIAL_ECHOPAIR(", ", fr_mm_s);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
    const bool deploy_bltouch = (axis == Z_AXIS && distance < 0);
    if (deploy_bltouch) set_bltouch_deployed(true);
  #endif

  #if QUIET_PROBING
    if (axis == Z_AXIS) probing_pause(true);
  #endif

  // Tell the planner we're at Z=0
  current_position[axis] = 0;

  #if IS_SCARA
    SYNC_PLAN_POSITION_KINEMATIC();
    current_position[axis] = distance;
    inverse_kinematics(current_position);
    planner.buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], current_position[E_AXIS], fr_mm_s ? fr_mm_s : homing_feedrate(axis), active_extruder);
  #else
    sync_plan_position();
    current_position[axis] = distance;
    planner.buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], fr_mm_s ? fr_mm_s : homing_feedrate(axis), active_extruder);
  #endif

  stepper.synchronize();

  #if QUIET_PROBING
    if (axis == Z_AXIS) probing_pause(false);
  #endif

  #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
    if (deploy_bltouch) set_bltouch_deployed(false);
  #endif

  endstops.hit_on_purpose();

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< do_homing_move(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif
}

/**
 * TMC2130 specific sensorless homing using stallGuard2.
 * stallGuard2 only works when in spreadCycle mode.
 * spreadCycle and stealthChop are mutually exclusive.
 */
#if ENABLED(SENSORLESS_HOMING)
  void tmc2130_sensorless_homing(TMC2130Stepper &st, bool enable=true) {
    #if ENABLED(STEALTHCHOP)
      if (enable) {
        st.coolstep_min_speed(1024UL * 1024UL - 1UL);
        st.stealthChop(0);
      }
      else {
        st.coolstep_min_speed(0);
        st.stealthChop(1);
      }
    #endif

    st.diag1_stall(enable ? 1 : 0);
  }
#endif

/**
 * Home an individual "raw axis" to its endstop.
 * This applies to XYZ on Cartesian and Core robots, and
 * to the individual ABC steppers on DELTA and SCARA.
 *
 * At the end of the procedure the axis is marked as
 * homed and the current position of that axis is updated.
 * Kinematic robots should wait till all axes are homed
 * before updating the current position.
 */

#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

static void homeaxis(const AxisEnum axis) {

  #if IS_SCARA
    // Only Z homing (with probe) is permitted
    if (axis != Z_AXIS) { BUZZ(100, 880); return; }
  #else
    #define CAN_HOME(A) \
      (axis == A##_AXIS && ((A##_MIN_PIN > -1 && A##_HOME_DIR < 0) || (A##_MAX_PIN > -1 && A##_HOME_DIR > 0)))
    if (!CAN_HOME(X) && !CAN_HOME(Y) && !CAN_HOME(Z)) return;
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> homeaxis(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  const int axis_home_dir =
    #if ENABLED(DUAL_X_CARRIAGE)
      (axis == X_AXIS) ? x_home_dir(active_extruder) :
    #endif
    home_dir(axis);

  // Homing Z towards the bed? Deploy the Z probe or endstop.
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS && DEPLOY_PROBE()) return;
  #endif

  // Set a flag for Z motor locking
  #if ENABLED(Z_DUAL_ENDSTOPS)
    if (axis == Z_AXIS) stepper.set_homing_flag(true);
  #endif

  // Disable stealthChop if used. Enable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    #if ENABLED(X_IS_TMC2130)
      if (axis == X_AXIS) tmc2130_sensorless_homing(stepperX);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      if (axis == Y_AXIS) tmc2130_sensorless_homing(stepperY);
    #endif
  #endif

  // Fast move towards endstop until triggered
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Home 1 Fast:");
  #endif
  do_homing_move(axis, 1.5 * max_length(axis) * axis_home_dir);

  // When homing Z with probe respect probe clearance
  const float bump = axis_home_dir * (
    #if HOMING_Z_WITH_PROBE
      (axis == Z_AXIS) ? max(Z_CLEARANCE_BETWEEN_PROBES, home_bump_mm(Z_AXIS)) :
    #endif
    home_bump_mm(axis)
  );

  // If a second homing move is configured...
  if (bump) {
    // Move away from the endstop by the axis HOME_BUMP_MM
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Move Away:");
    #endif
    do_homing_move(axis, -bump);

    // Slow move towards endstop until triggered
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Home 2 Slow:");
    #endif
    do_homing_move(axis, 2 * bump, get_homing_bump_feedrate(axis));
  }

  #if ENABLED(Z_DUAL_ENDSTOPS)
    if (axis == Z_AXIS) {
      float adj = FABS(z_endstop_adj);
      bool lockZ1;
      if (axis_home_dir > 0) {
        adj = -adj;
        lockZ1 = (z_endstop_adj > 0);
      }
      else
        lockZ1 = (z_endstop_adj < 0);

      if (lockZ1) stepper.set_z_lock(true); else stepper.set_z2_lock(true);

      // Move to the adjusted endstop height
      do_homing_move(axis, adj);

      if (lockZ1) stepper.set_z_lock(false); else stepper.set_z2_lock(false);
      stepper.set_homing_flag(false);
    } // Z_AXIS
  #endif

  #if IS_SCARA

    set_axis_is_at_home(axis);
    SYNC_PLAN_POSITION_KINEMATIC();

  #elif ENABLED(DELTA)

    // Delta has already moved all three towers up in G28
    // so here it re-homes each tower in turn.
    // Delta homing treats the axes as normal linear axes.

    // retrace by the amount specified in endstop_adj + additional 0.1mm in order to have minimum steps
    if (endstop_adj[axis] * Z_HOME_DIR <= 0) {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("endstop_adj:");
      #endif
      do_homing_move(axis, endstop_adj[axis] - 0.1);
    }

  #else

    // For cartesian/core machines,
    // set the axis to its home position
    set_axis_is_at_home(axis);
    sync_plan_position();

    destination[axis] = current_position[axis];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("> AFTER set_axis_is_at_home", current_position);
    #endif

  #endif

  // Re-enable stealthChop if used. Disable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    #if ENABLED(X_IS_TMC2130)
      if (axis == X_AXIS) tmc2130_sensorless_homing(stepperX, false);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      if (axis == Y_AXIS) tmc2130_sensorless_homing(stepperY, false);
    #endif
  #endif

  // Put away the Z probe
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS && STOW_PROBE()) return;
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< homeaxis(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif
} // homeaxis()

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

/**
 * ***************************************************************************
 * ***************************** G-CODE HANDLING *****************************
 * ***************************************************************************
 */

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  /**
   * Output a "busy" message at regular intervals
   * while the machine is not accepting commands.
   */
  void host_keepalive() {
    const millis_t ms = millis();
    if (host_keepalive_interval && busy_state != NOT_BUSY) {
      if (PENDING(ms, next_busy_signal_ms)) return;
      switch (busy_state) {
        case IN_HANDLER:
        case IN_PROCESS:
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_BUSY_PROCESSING);
          break;
        case PAUSED_FOR_USER:
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER);
          break;
        case PAUSED_FOR_INPUT:
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_INPUT);
          break;
        default:
          break;
      }
    }
    next_busy_signal_ms = ms + host_keepalive_interval * 1000UL;
  }

#endif // HOST_KEEPALIVE_FEATURE


/**************************************************
 ***************** GCode Handlers *****************
 **************************************************/

#if ENABLED(ARC_SUPPORT)
  #include "gcode/motion/G2_G3.h"
#endif

void dwell(millis_t time) {
  gcode.refresh_cmd_timeout();
  time += previous_cmd_ms;
  while (PENDING(millis(), time)) idle();
}

#include "gcode/motion/G4.h"

#if ENABLED(BEZIER_CURVE_SUPPORT)
  #include "gcode/motion/G5.h"
#endif

#if ENABLED(NOZZLE_CLEAN_FEATURE)
  #include "gcode/feature/clean/G12.h"
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)
  #include "gcode/geometry/G17-G19.h"
#endif

#if ENABLED(INCH_MODE_SUPPORT)
  #include "gcode/units/G20_G21.h"
#endif

#if ENABLED(UBL_G26_MESH_VALIDATION)
  #include "gcode/calibrate/G26.h"
#endif

#if ENABLED(NOZZLE_PARK_FEATURE)
  #include "gcode/feature/pause/G27.h"
#endif

#if ENABLED(PROBE_MANUALLY)
  bool g29_in_progress = false;
#else
  constexpr bool g29_in_progress = false;
#endif

#include "gcode/calibrate/G28.h"

void home_all_axes() { gcode_G28(true); }

#if HAS_PROBING_PROCEDURE

  void out_of_range_error(const char* p_edge) {
    SERIAL_PROTOCOLPGM("?Probe ");
    serialprintPGM(p_edge);
    SERIAL_PROTOCOLLNPGM(" position out of range.");
  }

#endif

#include "gcode/calibrate/G29.h"

#if HAS_BED_PROBE
  #include "gcode/probe/G30.h"
  #if ENABLED(Z_PROBE_SLED)
    #include "gcode/probe/G31_G32.h"
  #endif
#endif

#if PROBE_SELECTED && ENABLED(DELTA_AUTO_CALIBRATION)
  #include "gcode/calibrate/G33.h"
#endif

#if ENABLED(G38_PROBE_TARGET)
  #include "gcode/probe/G38.h"
#endif

#if HAS_MESH
  #include "gcode/probe/G42.h"
#endif

#include "gcode/geometry/G92.h"

#if HAS_RESUME_CONTINUE
  #include "gcode/lcd/M0_M1.h"
#endif

#if ENABLED(SPINDLE_LASER_ENABLE)
  #include "gcode/control/M3-M5.h"
#endif

#include "gcode/control/M17.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  // For M125, M600, M24
  #include "gcode/feature/pause/common.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "gcode/sdcard/M20.h"           // M20  - List SD card. (Requires SDSUPPORT)
  #include "gcode/sdcard/M21.h"           // M21  - Init SD card. (Requires SDSUPPORT)
  #include "gcode/sdcard/M22.h"           // M22  - Release SD card. (Requires SDSUPPORT)
  #include "gcode/sdcard/M23.h"           // M23  - Select SD file: "M23 /path/file.gco". (Requires SDSUPPORT)
  #include "gcode/sdcard/M24.h"           // M24  - Start/resume SD print. (Requires SDSUPPORT)
  #include "gcode/sdcard/M25.h"           // M25  - Pause SD print. (Requires SDSUPPORT)
  #include "gcode/sdcard/M26.h"           // M26  - Set SD position in bytes: "M26 S12345". (Requires SDSUPPORT)
  #include "gcode/sdcard/M27.h"           // M27  - Report SD print status. (Requires SDSUPPORT)
  #include "gcode/sdcard/M28.h"           // M28  - Start SD write: "M28 /path/file.gco". (Requires SDSUPPORT)
  #include "gcode/sdcard/M29.h"           // M29  - Stop SD write. (Requires SDSUPPORT)
  #include "gcode/sdcard/M30.h"           // M30  - Delete file from SD: "M30 /path/file.gco"
#endif

#include "gcode/stats/M31.h"              // M31: Get the time since the start of SD Print (or last M109)

#if ENABLED(SDSUPPORT)
  #include "gcode/sdcard/M32.h"
  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
    #include "gcode/sdcard/M33.h"
  #endif
  #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
    #include "gcode/sdcard/M34.h"
  #endif
  #include "gcode/sdcard/M928.h"
#endif

/**
 * Sensitive pin test for M42, M226
 */
static bool pin_is_protected(const int8_t pin) {
  static const int8_t sensitive_pins[] PROGMEM = SENSITIVE_PINS;
  for (uint8_t i = 0; i < COUNT(sensitive_pins); i++)
    if (pin == (int8_t)pgm_read_byte(&sensitive_pins[i])) return true;
  return false;
}

#include "gcode/control/M42.h"

#if ENABLED(PINS_DEBUGGING)
  #include "gcode/config/M43.h"
#endif

#if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
  #include "gcode/calibrate/M48.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(UBL_G26_MESH_VALIDATION)
  #include "gcode/calibrate/M49.h"
#endif

#include "gcode/stats/M75.h"
#include "gcode/stats/M76.h"
#include "gcode/stats/M77.h"

#if ENABLED(PRINTCOUNTER)
  #include "gcode/stats/M78.h"
#endif

#if HAS_TEMP_HOTEND || HAS_TEMP_BED

  void print_heater_state(const float &c, const float &t,
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      const float r,
    #endif
    const int8_t e=-2
  ) {
    #if !(HAS_TEMP_BED && HAS_TEMP_HOTEND) && HOTENDS <= 1
      UNUSED(e);
    #endif

    SERIAL_PROTOCOLCHAR(' ');
    SERIAL_PROTOCOLCHAR(
      #if HAS_TEMP_BED && HAS_TEMP_HOTEND
        e == -1 ? 'B' : 'T'
      #elif HAS_TEMP_HOTEND
        'T'
      #else
        'B'
      #endif
    );
    #if HOTENDS > 1
      if (e >= 0) SERIAL_PROTOCOLCHAR('0' + e);
    #endif
    SERIAL_PROTOCOLCHAR(':');
    SERIAL_PROTOCOL(c);
    SERIAL_PROTOCOLPAIR(" /" , t);
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      SERIAL_PROTOCOLPAIR(" (", r / OVERSAMPLENR);
      SERIAL_PROTOCOLCHAR(')');
    #endif
  }

  void print_heaterstates() {
    #if HAS_TEMP_HOTEND
      print_heater_state(thermalManager.degHotend(gcode.target_extruder), thermalManager.degTargetHotend(gcode.target_extruder)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , thermalManager.rawHotendTemp(gcode.target_extruder)
        #endif
      );
    #endif
    #if HAS_TEMP_BED
      print_heater_state(thermalManager.degBed(), thermalManager.degTargetBed(),
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          thermalManager.rawBedTemp(),
        #endif
        -1 // BED
      );
    #endif
    #if HOTENDS > 1
      HOTEND_LOOP() print_heater_state(thermalManager.degHotend(e), thermalManager.degTargetHotend(e),
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          thermalManager.rawHotendTemp(e),
        #endif
        e
      );
    #endif
    SERIAL_PROTOCOLPGM(" @:");
    SERIAL_PROTOCOL(thermalManager.getHeaterPower(gcode.target_extruder));
    #if HAS_TEMP_BED
      SERIAL_PROTOCOLPGM(" B@:");
      SERIAL_PROTOCOL(thermalManager.getHeaterPower(-1));
    #endif
    #if HOTENDS > 1
      HOTEND_LOOP() {
        SERIAL_PROTOCOLPAIR(" @", e);
        SERIAL_PROTOCOLCHAR(':');
        SERIAL_PROTOCOL(thermalManager.getHeaterPower(e));
      }
    #endif
  }

#endif // HAS_TEMP_HOTEND || HAS_TEMP_BED

#include "gcode/temperature/M105.h"

#if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)

  static uint8_t auto_report_temp_interval;
  static millis_t next_temp_report_ms;

  inline void auto_report_temperatures() {
    if (auto_report_temp_interval && ELAPSED(millis(), next_temp_report_ms)) {
      next_temp_report_ms = millis() + 1000UL * auto_report_temp_interval;
      print_heaterstates();
      SERIAL_EOL();
    }
  }

  #include "gcode/temperature/M155.h"

#endif // AUTO_REPORT_TEMPERATURES && (HAS_TEMP_HOTEND || HAS_TEMP_BED)

#if FAN_COUNT > 0
  #include "gcode/temperature/M106.h"
  #include "gcode/temperature/M107.h"
#endif

#if DISABLED(EMERGENCY_PARSER)
  #include "gcode/control/M108.h"
  #include "gcode/control/M112.h"
  #include "gcode/control/M410.h"
#endif

#if HAS_TEMP_BED
  #include "gcode/temperature/M190.h"
#endif

#include "gcode/host/M110.h"

#include "gcode/control/M111.h"

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  #include "gcode/host/M113.h"
#endif

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

#include "gcode/temperature/M140.h"

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

/**
 * Output the current position to serial
 */
void report_current_position() {
  SERIAL_PROTOCOLPGM("X:");
  SERIAL_PROTOCOL(current_position[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(current_position[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(current_position[Z_AXIS]);
  SERIAL_PROTOCOLPGM(" E:");
  SERIAL_PROTOCOL(current_position[E_AXIS]);

  stepper.report_positions();

  #if IS_SCARA
    SERIAL_PROTOCOLPAIR("SCARA Theta:", stepper.get_axis_position_degrees(A_AXIS));
    SERIAL_PROTOCOLLNPAIR("   Psi+Theta:", stepper.get_axis_position_degrees(B_AXIS));
    SERIAL_EOL();
  #endif
}

#include "gcode/host/M114.h"
#include "gcode/host/M115.h"

#include "gcode/lcd/M117.h"

#include "gcode/host/M118.h"
#include "gcode/host/M119.h"

#include "gcode/control/M120_M121.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "gcode/feature/pause/M125.h"
#endif

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

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #include "gcode/sensor/M404.h"
  #include "gcode/sensor/M405.h"
  #include "gcode/sensor/M406.h"
  #include "gcode/sensor/M407.h"
#endif

void quickstop_stepper() {
  stepper.quick_stop();
  stepper.synchronize();
  set_current_from_steppers_for_axis(ALL_AXES);
  SYNC_PLAN_POSITION_KINEMATIC();
}

#if HAS_LEVELING
  #include "gcode/calibrate/M420.h"
  #include "gcode/calibrate/M421.h"
#endif

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

#if HAS_BED_PROBE
  #include "gcode/probe/M851.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "gcode/feature/pause/M600.h"
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

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    #define ABL_BG_SPACING(A) bilinear_grid_spacing_virt[A]
    #define ABL_BG_FACTOR(A)  bilinear_grid_factor_virt[A]
    #define ABL_BG_POINTS_X   ABL_GRID_POINTS_VIRT_X
    #define ABL_BG_POINTS_Y   ABL_GRID_POINTS_VIRT_Y
    #define ABL_BG_GRID(X,Y)  z_values_virt[X][Y]
  #else
    #define ABL_BG_SPACING(A) bilinear_grid_spacing[A]
    #define ABL_BG_FACTOR(A)  bilinear_grid_factor[A]
    #define ABL_BG_POINTS_X   GRID_MAX_POINTS_X
    #define ABL_BG_POINTS_Y   GRID_MAX_POINTS_Y
    #define ABL_BG_GRID(X,Y)  z_values[X][Y]
  #endif

  // Get the Z adjustment for non-linear bed leveling
  float bilinear_z_offset(const float logical[XYZ]) {

    static float z1, d2, z3, d4, L, D, ratio_x, ratio_y,
                 last_x = -999.999, last_y = -999.999;

    // Whole units for the grid line indices. Constrained within bounds.
    static int8_t gridx, gridy, nextx, nexty,
                  last_gridx = -99, last_gridy = -99;

    // XY relative to the probed area
    const float x = RAW_X_POSITION(logical[X_AXIS]) - bilinear_start[X_AXIS],
                y = RAW_Y_POSITION(logical[Y_AXIS]) - bilinear_start[Y_AXIS];

    #if ENABLED(EXTRAPOLATE_BEYOND_GRID)
      // Keep using the last grid box
      #define FAR_EDGE_OR_BOX 2
    #else
      // Just use the grid far edge
      #define FAR_EDGE_OR_BOX 1
    #endif

    if (last_x != x) {
      last_x = x;
      ratio_x = x * ABL_BG_FACTOR(X_AXIS);
      const float gx = constrain(FLOOR(ratio_x), 0, ABL_BG_POINTS_X - FAR_EDGE_OR_BOX);
      ratio_x -= gx;      // Subtract whole to get the ratio within the grid box

      #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
        // Beyond the grid maintain height at grid edges
        NOLESS(ratio_x, 0); // Never < 0.0. (> 1.0 is ok when nextx==gridx.)
      #endif

      gridx = gx;
      nextx = min(gridx + 1, ABL_BG_POINTS_X - 1);
    }

    if (last_y != y || last_gridx != gridx) {

      if (last_y != y) {
        last_y = y;
        ratio_y = y * ABL_BG_FACTOR(Y_AXIS);
        const float gy = constrain(FLOOR(ratio_y), 0, ABL_BG_POINTS_Y - FAR_EDGE_OR_BOX);
        ratio_y -= gy;

        #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
          // Beyond the grid maintain height at grid edges
          NOLESS(ratio_y, 0); // Never < 0.0. (> 1.0 is ok when nexty==gridy.)
        #endif

        gridy = gy;
        nexty = min(gridy + 1, ABL_BG_POINTS_Y - 1);
      }

      if (last_gridx != gridx || last_gridy != gridy) {
        last_gridx = gridx;
        last_gridy = gridy;
        // Z at the box corners
        z1 = ABL_BG_GRID(gridx, gridy);       // left-front
        d2 = ABL_BG_GRID(gridx, nexty) - z1;  // left-back (delta)
        z3 = ABL_BG_GRID(nextx, gridy);       // right-front
        d4 = ABL_BG_GRID(nextx, nexty) - z3;  // right-back (delta)
      }

      // Bilinear interpolate. Needed since y or gridx has changed.
                  L = z1 + d2 * ratio_y;   // Linear interp. LF -> LB
      const float R = z3 + d4 * ratio_y;   // Linear interp. RF -> RB

      D = R - L;
    }

    const float offset = L + ratio_x * D;   // the offset almost always changes

    /*
    static float last_offset = 0;
    if (FABS(last_offset - offset) > 0.2) {
      SERIAL_ECHOPGM("Sudden Shift at ");
      SERIAL_ECHOPAIR("x=", x);
      SERIAL_ECHOPAIR(" / ", bilinear_grid_spacing[X_AXIS]);
      SERIAL_ECHOLNPAIR(" -> gridx=", gridx);
      SERIAL_ECHOPAIR(" y=", y);
      SERIAL_ECHOPAIR(" / ", bilinear_grid_spacing[Y_AXIS]);
      SERIAL_ECHOLNPAIR(" -> gridy=", gridy);
      SERIAL_ECHOPAIR(" ratio_x=", ratio_x);
      SERIAL_ECHOLNPAIR(" ratio_y=", ratio_y);
      SERIAL_ECHOPAIR(" z1=", z1);
      SERIAL_ECHOPAIR(" z2=", z2);
      SERIAL_ECHOPAIR(" z3=", z3);
      SERIAL_ECHOLNPAIR(" z4=", z4);
      SERIAL_ECHOPAIR(" L=", L);
      SERIAL_ECHOPAIR(" R=", R);
      SERIAL_ECHOLNPAIR(" offset=", offset);
    }
    last_offset = offset;
    //*/

    return offset;
  }

#endif // AUTO_BED_LEVELING_BILINEAR

#if ENABLED(DELTA)

  /**
   * Recalculate factors used for delta kinematics whenever
   * settings have been changed (e.g., by M665).
   */
  void recalc_delta_settings(float radius, float diagonal_rod) {
    const float trt[ABC] = DELTA_RADIUS_TRIM_TOWER,
                drt[ABC] = DELTA_DIAGONAL_ROD_TRIM_TOWER;
    delta_tower[A_AXIS][X_AXIS] = cos(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (radius + trt[A_AXIS]); // front left tower
    delta_tower[A_AXIS][Y_AXIS] = sin(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (radius + trt[A_AXIS]);
    delta_tower[B_AXIS][X_AXIS] = cos(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (radius + trt[B_AXIS]); // front right tower
    delta_tower[B_AXIS][Y_AXIS] = sin(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (radius + trt[B_AXIS]);
    delta_tower[C_AXIS][X_AXIS] = 0.0; // back middle tower
    delta_tower[C_AXIS][Y_AXIS] = (radius + trt[C_AXIS]);
    delta_diagonal_rod_2_tower[A_AXIS] = sq(diagonal_rod + drt[A_AXIS]);
    delta_diagonal_rod_2_tower[B_AXIS] = sq(diagonal_rod + drt[B_AXIS]);
    delta_diagonal_rod_2_tower[C_AXIS] = sq(diagonal_rod + drt[C_AXIS]);
  }

  #if ENABLED(DELTA_FAST_SQRT) && defined(ARDUINO_ARCH_AVR)
    /**
     * Fast inverse sqrt from Quake III Arena
     * See: https://en.wikipedia.org/wiki/Fast_inverse_square_root
     */
    float Q_rsqrt(float number) {
      long i;
      float x2, y;
      const float threehalfs = 1.5f;
      x2 = number * 0.5f;
      y  = number;
      i  = * ( long * ) &y;                       // evil floating point bit level hacking
      i  = 0x5F3759DF - ( i >> 1 );               // what the f***?
      y  = * ( float * ) &i;
      y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
      // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
      return y;
    }

    #define _SQRT(n) (1.0f / Q_rsqrt(n))

  #else

    #define _SQRT(n) SQRT(n)

  #endif

  /**
   * Delta Inverse Kinematics
   *
   * Calculate the tower positions for a given logical
   * position, storing the result in the delta[] array.
   *
   * This is an expensive calculation, requiring 3 square
   * roots per segmented linear move, and strains the limits
   * of a Mega2560 with a Graphical Display.
   *
   * Suggested optimizations include:
   *
   * - Disable the home_offset (M206) and/or position_shift (G92)
   *   features to remove up to 12 float additions.
   *
   * - Use a fast-inverse-sqrt function and add the reciprocal.
   *   (see above)
   */

  // Macro to obtain the Z position of an individual tower
  #define DELTA_Z(T) raw[Z_AXIS] + _SQRT(     \
    delta_diagonal_rod_2_tower[T] - HYPOT2(   \
        delta_tower[T][X_AXIS] - raw[X_AXIS], \
        delta_tower[T][Y_AXIS] - raw[Y_AXIS]  \
      )                                       \
    )

  #define DELTA_RAW_IK() do {        \
    delta[A_AXIS] = DELTA_Z(A_AXIS); \
    delta[B_AXIS] = DELTA_Z(B_AXIS); \
    delta[C_AXIS] = DELTA_Z(C_AXIS); \
  }while(0)

  #define DELTA_LOGICAL_IK() do {      \
    const float raw[XYZ] = {           \
      RAW_X_POSITION(logical[X_AXIS]), \
      RAW_Y_POSITION(logical[Y_AXIS]), \
      RAW_Z_POSITION(logical[Z_AXIS])  \
    };                                 \
    DELTA_RAW_IK();                    \
  }while(0)

  #define DELTA_DEBUG() do { \
      SERIAL_ECHOPAIR("cartesian X:", raw[X_AXIS]); \
      SERIAL_ECHOPAIR(" Y:", raw[Y_AXIS]);          \
      SERIAL_ECHOLNPAIR(" Z:", raw[Z_AXIS]);        \
      SERIAL_ECHOPAIR("delta A:", delta[A_AXIS]);   \
      SERIAL_ECHOPAIR(" B:", delta[B_AXIS]);        \
      SERIAL_ECHOLNPAIR(" C:", delta[C_AXIS]);      \
    }while(0)

  void inverse_kinematics(const float logical[XYZ]) {
    DELTA_LOGICAL_IK();
    // DELTA_DEBUG();
  }

  /**
   * Calculate the highest Z position where the
   * effector has the full range of XY motion.
   */
  float delta_safe_distance_from_top() {
    float cartesian[XYZ] = {
      LOGICAL_X_POSITION(0),
      LOGICAL_Y_POSITION(0),
      LOGICAL_Z_POSITION(0)
    };
    inverse_kinematics(cartesian);
    float distance = delta[A_AXIS];
    cartesian[Y_AXIS] = LOGICAL_Y_POSITION(DELTA_PRINTABLE_RADIUS);
    inverse_kinematics(cartesian);
    return FABS(distance - delta[A_AXIS]);
  }

  /**
   * Delta Forward Kinematics
   *
   * See the Wikipedia article "Trilateration"
   * https://en.wikipedia.org/wiki/Trilateration
   *
   * Establish a new coordinate system in the plane of the
   * three carriage points. This system has its origin at
   * tower1, with tower2 on the X axis. Tower3 is in the X-Y
   * plane with a Z component of zero.
   * We will define unit vectors in this coordinate system
   * in our original coordinate system. Then when we calculate
   * the Xnew, Ynew and Znew values, we can translate back into
   * the original system by moving along those unit vectors
   * by the corresponding values.
   *
   * Variable names matched to Marlin, c-version, and avoid the
   * use of any vector library.
   *
   * by Andreas Hardtung 2016-06-07
   * based on a Java function from "Delta Robot Kinematics V3"
   * by Steve Graves
   *
   * The result is stored in the cartes[] array.
   */
  void forward_kinematics_DELTA(float z1, float z2, float z3) {
    // Create a vector in old coordinates along x axis of new coordinate
    float p12[3] = { delta_tower[B_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS], delta_tower[B_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS], z2 - z1 };

    // Get the Magnitude of vector.
    float d = SQRT( sq(p12[0]) + sq(p12[1]) + sq(p12[2]) );

    // Create unit vector by dividing by magnitude.
    float ex[3] = { p12[0] / d, p12[1] / d, p12[2] / d };

    // Get the vector from the origin of the new system to the third point.
    float p13[3] = { delta_tower[C_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS], delta_tower[C_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS], z3 - z1 };

    // Use the dot product to find the component of this vector on the X axis.
    float i = ex[0] * p13[0] + ex[1] * p13[1] + ex[2] * p13[2];

    // Create a vector along the x axis that represents the x component of p13.
    float iex[3] = { ex[0] * i, ex[1] * i, ex[2] * i };

    // Subtract the X component from the original vector leaving only Y. We use the
    // variable that will be the unit vector after we scale it.
    float ey[3] = { p13[0] - iex[0], p13[1] - iex[1], p13[2] - iex[2] };

    // The magnitude of Y component
    float j = SQRT( sq(ey[0]) + sq(ey[1]) + sq(ey[2]) );

    // Convert to a unit vector
    ey[0] /= j; ey[1] /= j;  ey[2] /= j;

    // The cross product of the unit x and y is the unit z
    // float[] ez = vectorCrossProd(ex, ey);
    float ez[3] = {
      ex[1] * ey[2] - ex[2] * ey[1],
      ex[2] * ey[0] - ex[0] * ey[2],
      ex[0] * ey[1] - ex[1] * ey[0]
    };

    // We now have the d, i and j values defined in Wikipedia.
    // Plug them into the equations defined in Wikipedia for Xnew, Ynew and Znew
    float Xnew = (delta_diagonal_rod_2_tower[A_AXIS] - delta_diagonal_rod_2_tower[B_AXIS] + sq(d)) / (d * 2),
          Ynew = ((delta_diagonal_rod_2_tower[A_AXIS] - delta_diagonal_rod_2_tower[C_AXIS] + HYPOT2(i, j)) / 2 - i * Xnew) / j,
          Znew = SQRT(delta_diagonal_rod_2_tower[A_AXIS] - HYPOT2(Xnew, Ynew));

    // Start from the origin of the old coordinates and add vectors in the
    // old coords that represent the Xnew, Ynew and Znew to find the point
    // in the old system.
    cartes[X_AXIS] = delta_tower[A_AXIS][X_AXIS] + ex[0] * Xnew + ey[0] * Ynew - ez[0] * Znew;
    cartes[Y_AXIS] = delta_tower[A_AXIS][Y_AXIS] + ex[1] * Xnew + ey[1] * Ynew - ez[1] * Znew;
    cartes[Z_AXIS] =             z1 + ex[2] * Xnew + ey[2] * Ynew - ez[2] * Znew;
  }

  void forward_kinematics_DELTA(float point[ABC]) {
    forward_kinematics_DELTA(point[A_AXIS], point[B_AXIS], point[C_AXIS]);
  }

#endif // DELTA

/**
 * Get the stepper positions in the cartes[] array.
 * Forward kinematics are applied for DELTA and SCARA.
 *
 * The result is in the current coordinate space with
 * leveling applied. The coordinates need to be run through
 * unapply_leveling to obtain the "ideal" coordinates
 * suitable for current_position, etc.
 */
void get_cartesian_from_steppers() {
  #if ENABLED(DELTA)
    forward_kinematics_DELTA(
      stepper.get_axis_position_mm(A_AXIS),
      stepper.get_axis_position_mm(B_AXIS),
      stepper.get_axis_position_mm(C_AXIS)
    );
    cartes[X_AXIS] += LOGICAL_X_POSITION(0);
    cartes[Y_AXIS] += LOGICAL_Y_POSITION(0);
    cartes[Z_AXIS] += LOGICAL_Z_POSITION(0);
  #elif IS_SCARA
    forward_kinematics_SCARA(
      stepper.get_axis_position_degrees(A_AXIS),
      stepper.get_axis_position_degrees(B_AXIS)
    );
    cartes[X_AXIS] += LOGICAL_X_POSITION(0);
    cartes[Y_AXIS] += LOGICAL_Y_POSITION(0);
    cartes[Z_AXIS] = stepper.get_axis_position_mm(Z_AXIS);
  #else
    cartes[X_AXIS] = stepper.get_axis_position_mm(X_AXIS);
    cartes[Y_AXIS] = stepper.get_axis_position_mm(Y_AXIS);
    cartes[Z_AXIS] = stepper.get_axis_position_mm(Z_AXIS);
  #endif
}

/**
 * Set the current_position for an axis based on
 * the stepper positions, removing any leveling that
 * may have been applied.
 */
void set_current_from_steppers_for_axis(const AxisEnum axis) {
  get_cartesian_from_steppers();
  #if PLANNER_LEVELING
    planner.unapply_leveling(cartes);
  #endif
  if (axis == ALL_AXES)
    COPY(current_position, cartes);
  else
    current_position[axis] = cartes[axis];
}

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

#if ENABLED(MORGAN_SCARA)

  /**
   * Morgan SCARA Forward Kinematics. Results in cartes[].
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void forward_kinematics_SCARA(const float &a, const float &b) {

    float a_sin = sin(RADIANS(a)) * L1,
          a_cos = cos(RADIANS(a)) * L1,
          b_sin = sin(RADIANS(b)) * L2,
          b_cos = cos(RADIANS(b)) * L2;

    cartes[X_AXIS] = a_cos + b_cos + SCARA_OFFSET_X;  //theta
    cartes[Y_AXIS] = a_sin + b_sin + SCARA_OFFSET_Y;  //theta+phi

    /*
      SERIAL_ECHOPAIR("SCARA FK Angle a=", a);
      SERIAL_ECHOPAIR(" b=", b);
      SERIAL_ECHOPAIR(" a_sin=", a_sin);
      SERIAL_ECHOPAIR(" a_cos=", a_cos);
      SERIAL_ECHOPAIR(" b_sin=", b_sin);
      SERIAL_ECHOLNPAIR(" b_cos=", b_cos);
      SERIAL_ECHOPAIR(" cartes[X_AXIS]=", cartes[X_AXIS]);
      SERIAL_ECHOLNPAIR(" cartes[Y_AXIS]=", cartes[Y_AXIS]);
    //*/
  }

  /**
   * Morgan SCARA Inverse Kinematics. Results in delta[].
   *
   * See http://forums.reprap.org/read.php?185,283327
   *
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void inverse_kinematics(const float logical[XYZ]) {

    static float C2, S2, SK1, SK2, THETA, PSI;

    float sx = RAW_X_POSITION(logical[X_AXIS]) - SCARA_OFFSET_X,  // Translate SCARA to standard X Y
          sy = RAW_Y_POSITION(logical[Y_AXIS]) - SCARA_OFFSET_Y;  // With scaling factor.

    if (L1 == L2)
      C2 = HYPOT2(sx, sy) / L1_2_2 - 1;
    else
      C2 = (HYPOT2(sx, sy) - (L1_2 + L2_2)) / (2.0 * L1 * L2);

    S2 = SQRT(1 - sq(C2));

    // Unrotated Arm1 plus rotated Arm2 gives the distance from Center to End
    SK1 = L1 + L2 * C2;

    // Rotated Arm2 gives the distance from Arm1 to Arm2
    SK2 = L2 * S2;

    // Angle of Arm1 is the difference between Center-to-End angle and the Center-to-Elbow
    THETA = ATAN2(SK1, SK2) - ATAN2(sx, sy);

    // Angle of Arm2
    PSI = ATAN2(S2, C2);

    delta[A_AXIS] = DEGREES(THETA);        // theta is support arm angle
    delta[B_AXIS] = DEGREES(THETA + PSI);  // equal to sub arm angle (inverted motor)
    delta[C_AXIS] = logical[Z_AXIS];

    /*
      DEBUG_POS("SCARA IK", logical);
      DEBUG_POS("SCARA IK", delta);
      SERIAL_ECHOPAIR("  SCARA (x,y) ", sx);
      SERIAL_ECHOPAIR(",", sy);
      SERIAL_ECHOPAIR(" C2=", C2);
      SERIAL_ECHOPAIR(" S2=", S2);
      SERIAL_ECHOPAIR(" Theta=", THETA);
      SERIAL_ECHOLNPAIR(" Phi=", PHI);
    //*/
  }

#endif // MORGAN_SCARA

#if ENABLED(TEMP_STAT_LEDS)

  static bool red_led = false;
  static millis_t next_status_led_update_ms = 0;

  void handle_status_leds(void) {
    if (ELAPSED(millis(), next_status_led_update_ms)) {
      next_status_led_update_ms += 500; // Update every 0.5s
      float max_temp = 0.0;
      #if HAS_TEMP_BED
        max_temp = MAX3(max_temp, thermalManager.degTargetBed(), thermalManager.degBed());
      #endif
      HOTEND_LOOP()
        max_temp = MAX3(max_temp, thermalManager.degHotend(e), thermalManager.degTargetHotend(e));
      const bool new_led = (max_temp > 55.0) ? true : (max_temp < 54.0) ? false : red_led;
      if (new_led != red_led) {
        red_led = new_led;
        #if PIN_EXISTS(STAT_LED_RED)
          WRITE(STAT_LED_RED_PIN, new_led ? HIGH : LOW);
          #if PIN_EXISTS(STAT_LED_BLUE)
            WRITE(STAT_LED_BLUE_PIN, new_led ? LOW : HIGH);
          #endif
        #else
          WRITE(STAT_LED_BLUE_PIN, new_led ? HIGH : LOW);
        #endif
      }
    }
  }

#endif

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
  if (!volumetric_enabled || diameter == 0) return 1.0;
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

  if (max_inactive_time && ELAPSED(ms, previous_cmd_ms + max_inactive_time)) {
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

  if (MOVE_AWAY_TEST && stepper_inactive_time && ELAPSED(ms, previous_cmd_ms + stepper_inactive_time)
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
      ubl_lcd_map_control = defer_return_to_status = false;
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
    if (ELAPSED(ms, previous_cmd_ms + (EXTRUDER_RUNOUT_SECONDS) * 1000UL)
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

  host_keepalive();

  #if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)
    auto_report_temperatures();
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
  servo_init();

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
    // Make sure any BLTouch error condition is cleared
    bltouch_command(BLTOUCH_RESET);
    set_bltouch_deployed(true);
    set_bltouch_deployed(false);
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
