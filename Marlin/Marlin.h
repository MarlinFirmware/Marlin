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
#ifndef MARLIN_H
#define MARLIN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "MarlinConfig.h"

#include "enum.h"
#include "types.h"
#include "fastio.h"
#include "utility.h"

#ifdef USBCON
  #include "HardwareSerial.h"
  #if ENABLED(BLUETOOTH)
    #define MYSERIAL bluetoothSerial
  #else
    #define MYSERIAL Serial
  #endif // BLUETOOTH
#else
  #include "MarlinSerial.h"
  #define MYSERIAL customizedSerial
#endif

#include "WString.h"

#if ENABLED(PRINTCOUNTER)
  #include "printcounter.h"
#else
  #include "stopwatch.h"
#endif

#define SERIAL_CHAR(x) MYSERIAL.write(x)
#define SERIAL_EOL SERIAL_CHAR('\n')

#define SERIAL_PROTOCOLCHAR(x) SERIAL_CHAR(x)
#define SERIAL_PROTOCOL(x) MYSERIAL.print(x)
#define SERIAL_PROTOCOL_F(x,y) MYSERIAL.print(x,y)
#define SERIAL_PROTOCOLPGM(x) serialprintPGM(PSTR(x))
#define SERIAL_PROTOCOLLN(x) do{ MYSERIAL.print(x); SERIAL_EOL; }while(0)
#define SERIAL_PROTOCOLLNPGM(x) do{ serialprintPGM(PSTR(x "\n")); }while(0)

#define SERIAL_PROTOCOLPAIR(name, value) SERIAL_ECHOPAIR(name, value)

extern const char errormagic[] PROGMEM;
extern const char echomagic[] PROGMEM;

#define SERIAL_ERROR_START serialprintPGM(errormagic)
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHO_START serialprintPGM(echomagic)
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))

void serial_echopair_P(const char* s_P, char v);
void serial_echopair_P(const char* s_P, int v);
void serial_echopair_P(const char* s_P, long v);
void serial_echopair_P(const char* s_P, float v);
void serial_echopair_P(const char* s_P, double v);
void serial_echopair_P(const char* s_P, unsigned long v);
FORCE_INLINE void serial_echopair_P(const char* s_P, uint8_t v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, uint16_t v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, bool v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, void *v) { serial_echopair_P(s_P, (unsigned long)v); }

// Things to write to serial from Program memory. Saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char* str) {
  char ch;
  while ((ch = pgm_read_byte(str))) {
    MYSERIAL.write(ch);
    str++;
  }
}

void idle(
  #if ENABLED(FILAMENT_CHANGE_FEATURE)
    bool no_stepper_sleep = false  // pass true to keep steppers from disabling on timeout
  #endif
);

void manage_inactivity(bool ignore_stepper_queue = false);

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  extern bool extruder_duplication_enabled;
#endif

#if HAS_X2_ENABLE
  #define  enable_x() do{ X_ENABLE_WRITE( X_ENABLE_ON); X2_ENABLE_WRITE( X_ENABLE_ON); }while(0)
  #define disable_x() do{ X_ENABLE_WRITE(!X_ENABLE_ON); X2_ENABLE_WRITE(!X_ENABLE_ON); axis_known_position[X_AXIS] = false; }while(0)
#elif HAS_X_ENABLE
  #define  enable_x() X_ENABLE_WRITE( X_ENABLE_ON)
  #define disable_x() do{ X_ENABLE_WRITE(!X_ENABLE_ON); axis_known_position[X_AXIS] = false; }while(0)
#else
  #define  enable_x() NOOP
  #define disable_x() NOOP
#endif

#if HAS_Y2_ENABLE
  #define  enable_y() do{ Y_ENABLE_WRITE( Y_ENABLE_ON); Y2_ENABLE_WRITE(Y_ENABLE_ON); }while(0)
  #define disable_y() do{ Y_ENABLE_WRITE(!Y_ENABLE_ON); Y2_ENABLE_WRITE(!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }while(0)
#elif HAS_Y_ENABLE
  #define  enable_y() Y_ENABLE_WRITE( Y_ENABLE_ON)
  #define disable_y() do{ Y_ENABLE_WRITE(!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }while(0)
#else
  #define  enable_y() NOOP
  #define disable_y() NOOP
#endif

#if HAS_Z2_ENABLE
  #define  enable_z() do{ Z_ENABLE_WRITE( Z_ENABLE_ON); Z2_ENABLE_WRITE(Z_ENABLE_ON); }while(0)
  #define disable_z() do{ Z_ENABLE_WRITE(!Z_ENABLE_ON); Z2_ENABLE_WRITE(!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }while(0)
#elif HAS_Z_ENABLE
  #define  enable_z() Z_ENABLE_WRITE( Z_ENABLE_ON)
  #define disable_z() do{ Z_ENABLE_WRITE(!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }while(0)
#else
  #define  enable_z() NOOP
  #define disable_z() NOOP
#endif

#if ENABLED(MIXING_EXTRUDER)

  /**
   * Mixing steppers synchronize their enable (and direction) together
   */
  #if MIXING_STEPPERS > 3
    #define  enable_e0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); E3_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_e0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); E3_ENABLE_WRITE(!E_ENABLE_ON); }
  #elif MIXING_STEPPERS > 2
    #define  enable_e0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_e0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); }
  #else
    #define  enable_e0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_e0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); }
  #endif
  #define  enable_e1() NOOP
  #define disable_e1() NOOP
  #define  enable_e2() NOOP
  #define disable_e2() NOOP
  #define  enable_e3() NOOP
  #define disable_e3() NOOP

#else // !MIXING_EXTRUDER

  #if HAS_E0_ENABLE
    #define  enable_e0() E0_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_e0() E0_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_e0() NOOP
    #define disable_e0() NOOP
  #endif

  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define  enable_e1() E1_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_e1() E1_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_e1() NOOP
    #define disable_e1() NOOP
  #endif

  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  enable_e2() E2_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_e2() E2_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_e2() NOOP
    #define disable_e2() NOOP
  #endif

  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  enable_e3() E3_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_e3() E3_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_e3() NOOP
    #define disable_e3() NOOP
  #endif

#endif // !MIXING_EXTRUDER

/**
 * The axis order in all axis related arrays is X, Y, Z, E
 */
#define _AXIS(AXIS) AXIS ##_AXIS

void enable_all_steppers();
void disable_all_steppers();

void FlushSerialRequestResend();
void ok_to_send();

void reset_bed_level();
void kill(const char*);

void quickstop_stepper();

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  void handle_filament_runout();
#endif

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (DEBUG_## F))

extern bool Running;
inline bool IsRunning() { return  Running; }
inline bool IsStopped() { return !Running; }

bool enqueue_and_echo_command(const char* cmd, bool say_ok=false); //put a single ASCII command at the end of the current buffer or return false when it is full
void enqueue_and_echo_command_now(const char* cmd); // enqueue now, only return when the command has been enqueued
void enqueue_and_echo_commands_P(const char* cmd); //put one or many ASCII commands at the end of the current buffer, read from flash
void clear_command_queue();

void clamp_to_software_endstops(float target[3]);

extern millis_t previous_cmd_ms;
inline void refresh_cmd_timeout() { previous_cmd_ms = millis(); }

#if ENABLED(FAST_PWM_FAN)
  void setPwmFrequency(uint8_t pin, int val);
#endif

/**
 * Feedrate scaling and conversion
 */
extern int feedrate_percentage;

#define MMM_TO_MMS(MM_M) ((MM_M)/60.0)
#define MMS_TO_MMM(MM_S) ((MM_S)*60.0)
#define MMM_SCALED(MM_M) ((MM_M)*feedrate_percentage*0.01)
#define MMS_SCALED(MM_S) MMM_SCALED(MM_S)
#define MMM_TO_MMS_SCALED(MM_M) (MMS_SCALED(MMM_TO_MMS(MM_M)))

extern bool axis_relative_modes[];
extern bool volumetric_enabled;
extern int extruder_multiplier[EXTRUDERS]; // sets extrude multiply factor (in percent) for each extruder individually
extern float filament_size[EXTRUDERS]; // cross-sectional area of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder.
extern float volumetric_multiplier[EXTRUDERS]; // reciprocal of cross-sectional area of filament (in square millimeters), stored this way to reduce computational burden in planner
extern bool axis_known_position[3]; // axis[n].is_known
extern bool axis_homed[3]; // axis[n].is_homed
extern volatile bool wait_for_heatup;

extern float current_position[NUM_AXIS];
extern float position_shift[3];
extern float home_offset[3];
extern float sw_endstop_min[3];
extern float sw_endstop_max[3];

#define LOGICAL_POSITION(POS, AXIS) (POS + home_offset[AXIS] + position_shift[AXIS])
#define RAW_POSITION(POS, AXIS)     (POS - home_offset[AXIS] - position_shift[AXIS])
#define LOGICAL_X_POSITION(POS)     LOGICAL_POSITION(POS, X_AXIS)
#define LOGICAL_Y_POSITION(POS)     LOGICAL_POSITION(POS, Y_AXIS)
#define LOGICAL_Z_POSITION(POS)     LOGICAL_POSITION(POS, Z_AXIS)
#define RAW_X_POSITION(POS)         RAW_POSITION(POS, X_AXIS)
#define RAW_Y_POSITION(POS)         RAW_POSITION(POS, Y_AXIS)
#define RAW_Z_POSITION(POS)         RAW_POSITION(POS, Z_AXIS)
#define RAW_CURRENT_POSITION(AXIS)  RAW_POSITION(current_position[AXIS], AXIS)

// GCode support for external objects
bool code_seen(char);
int code_value_int();
float code_value_temp_abs();
float code_value_temp_diff();

#if ENABLED(DELTA)
  extern float delta[3];
  extern float endstop_adj[3]; // axis[n].endstop_adj
  extern float delta_radius;
  extern float delta_diagonal_rod;
  extern float delta_segments_per_second;
  extern float delta_diagonal_rod_trim_tower_1;
  extern float delta_diagonal_rod_trim_tower_2;
  extern float delta_diagonal_rod_trim_tower_3;
  void inverse_kinematics(const float cartesian[3]);
  void recalc_delta_settings(float radius, float diagonal_rod);
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    extern int delta_grid_spacing[2];
    void adjust_delta(float cartesian[3]);
  #endif
#elif ENABLED(SCARA)
  extern float delta[3];
  extern float axis_scaling[3];  // Build size scaling
  void inverse_kinematics(const float cartesian[3]);
  void forward_kinematics_SCARA(float f_scara[3]);
#endif

#if ENABLED(Z_DUAL_ENDSTOPS)
  extern float z_endstop_adj;
#endif

#if HAS_BED_PROBE
  extern float zprobe_zoffset;
#endif

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  extern uint8_t host_keepalive_interval;
#endif

#if FAN_COUNT > 0
  extern int fanSpeeds[FAN_COUNT];
#endif

#if ENABLED(BARICUDA)
  extern int baricuda_valve_pressure;
  extern int baricuda_e_to_p_pressure;
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  extern float filament_width_nominal;  //holds the theoretical filament diameter i.e., 3.00 or 1.75
  extern bool filament_sensor;  //indicates that filament sensor readings should control extrusion
  extern float filament_width_meas; //holds the filament diameter as accurately measured
  extern int8_t measurement_delay[];  //ring buffer to delay measurement
  extern int filwidth_delay_index1, filwidth_delay_index2;  //ring buffer index. used by planner, temperature, and main code
  extern int meas_delay_cm; //delay distance
#endif

#if ENABLED(FILAMENT_CHANGE_FEATURE)
  extern FilamentChangeMenuResponse filament_change_menu_response;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  extern int lpq_len;
#endif

#if ENABLED(FWRETRACT)
  extern bool autoretract_enabled;
  extern bool retracted[EXTRUDERS]; // extruder[n].retracted
  extern float retract_length, retract_length_swap, retract_feedrate_mm_s, retract_zlift;
  extern float retract_recover_length, retract_recover_length_swap, retract_recover_feedrate_mm_s;
#endif

// Print job timer
#if ENABLED(PRINTCOUNTER)
  extern PrintCounter print_job_timer;
#else
  extern Stopwatch print_job_timer;
#endif

// Handling multiple extruders pins
extern uint8_t active_extruder;

#if HAS_TEMP_HOTEND || HAS_TEMP_BED
  void print_heaterstates();
#endif

#if ENABLED(MIXING_EXTRUDER)
  extern float mixing_factor[MIXING_STEPPERS];
#endif

void calculate_volumetric_multipliers();

// Buzzer
#if HAS_BUZZER && PIN_EXISTS(BEEPER)
  #include "buzzer.h"
#endif

/**
 * Blocking movement and shorthand functions
 */
inline void do_blocking_move_to(float x, float y, float z, float fr_mm_m=0.0);
inline void do_blocking_move_to_x(float x, float fr_mm_m=0.0);
inline void do_blocking_move_to_z(float z, float fr_mm_m=0.0);
inline void do_blocking_move_to_xy(float x, float y, float fr_mm_m=0.0);

#endif //MARLIN_H
