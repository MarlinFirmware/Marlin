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

#define  FORCE_INLINE __attribute__((always_inline)) inline
/**
 * Compiler warning on unused variable.
 */
#define UNUSED(x) (void) (x)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "fastio.h"
#include "Configuration.h"
#include "pins.h"

#ifndef SANITYCHECK_H
  #error Your Configuration.h and Configuration_adv.h files are outdated!
#endif

#include "Arduino.h"

typedef unsigned long millis_t;

// Arduino < 1.0.0 does not define this, so we need to do it ourselves
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p) + 0xA0)
#endif

#ifdef USBCON
  #include "HardwareSerial.h"
#endif

#include "MarlinSerial.h"

#include "WString.h"

#ifdef USBCON
  #if ENABLED(BLUETOOTH)
    #define MYSERIAL bluetoothSerial
  #else
    #define MYSERIAL Serial
  #endif // BLUETOOTH
#else
  #define MYSERIAL customizedSerial
#endif

#define SERIAL_CHAR(x) MYSERIAL.write(x)
#define SERIAL_EOL SERIAL_CHAR('\n')

#define SERIAL_PROTOCOLCHAR(x) SERIAL_CHAR(x)
#define SERIAL_PROTOCOL(x) MYSERIAL.print(x)
#define SERIAL_PROTOCOL_F(x,y) MYSERIAL.print(x,y)
#define SERIAL_PROTOCOLPGM(x) serialprintPGM(PSTR(x))
#define SERIAL_PROTOCOLLN(x) do{ MYSERIAL.print(x); SERIAL_EOL; }while(0)
#define SERIAL_PROTOCOLLNPGM(x) do{ serialprintPGM(PSTR(x)); SERIAL_EOL; }while(0)


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

#define SERIAL_ECHOPAIR(name,value) do{ serial_echopair_P(PSTR(name),(value)); }while(0)

void serial_echopair_P(const char* s_P, int v);
void serial_echopair_P(const char* s_P, long v);
void serial_echopair_P(const char* s_P, float v);
void serial_echopair_P(const char* s_P, double v);
void serial_echopair_P(const char* s_P, unsigned long v);

// Things to write to serial from Program memory. Saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char* str) {
  char ch;
  while ((ch = pgm_read_byte(str))) {
    MYSERIAL.write(ch);
    str++;
  }
}

void idle(
  #if ENABLED(FILAMENTCHANGEENABLE)
    bool no_stepper_sleep=false  // pass true to keep steppers from disabling on timeout
  #endif
);

void manage_inactivity(bool ignore_stepper_queue = false);

#if ENABLED(DUAL_X_CARRIAGE) && HAS_X_ENABLE && HAS_X2_ENABLE
  #define  enable_x() do { X_ENABLE_WRITE( X_ENABLE_ON); X2_ENABLE_WRITE( X_ENABLE_ON); } while (0)
  #define disable_x() do { X_ENABLE_WRITE(!X_ENABLE_ON); X2_ENABLE_WRITE(!X_ENABLE_ON); axis_known_position[X_AXIS] = false; } while (0)
#elif HAS_X_ENABLE
  #define  enable_x() X_ENABLE_WRITE( X_ENABLE_ON)
  #define disable_x() { X_ENABLE_WRITE(!X_ENABLE_ON); axis_known_position[X_AXIS] = false; }
#else
  #define enable_x() ;
  #define disable_x() ;
#endif

#if HAS_Y_ENABLE
  #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
    #define  enable_y() { Y_ENABLE_WRITE( Y_ENABLE_ON); Y2_ENABLE_WRITE(Y_ENABLE_ON); }
    #define disable_y() { Y_ENABLE_WRITE(!Y_ENABLE_ON); Y2_ENABLE_WRITE(!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #else
    #define  enable_y() Y_ENABLE_WRITE( Y_ENABLE_ON)
    #define disable_y() { Y_ENABLE_WRITE(!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #endif
#else
  #define enable_y() ;
  #define disable_y() ;
#endif

#if HAS_Z_ENABLE
  #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
    #define  enable_z() { Z_ENABLE_WRITE( Z_ENABLE_ON); Z2_ENABLE_WRITE(Z_ENABLE_ON); }
    #define disable_z() { Z_ENABLE_WRITE(!Z_ENABLE_ON); Z2_ENABLE_WRITE(!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
  #else
    #define  enable_z() Z_ENABLE_WRITE( Z_ENABLE_ON)
    #define disable_z() { Z_ENABLE_WRITE(!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
  #endif
#else
  #define enable_z() ;
  #define disable_z() ;
#endif

#if HAS_E0_ENABLE
  #define enable_e0()  E0_ENABLE_WRITE( E_ENABLE_ON)
  #define disable_e0() E0_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define enable_e0()  /* nothing */
  #define disable_e0() /* nothing */
#endif

#if (EXTRUDERS > 1) && HAS_E1_ENABLE
  #define enable_e1()  E1_ENABLE_WRITE( E_ENABLE_ON)
  #define disable_e1() E1_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define enable_e1()  /* nothing */
  #define disable_e1() /* nothing */
#endif

#if (EXTRUDERS > 2) && HAS_E2_ENABLE
  #define enable_e2()  E2_ENABLE_WRITE( E_ENABLE_ON)
  #define disable_e2() E2_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define enable_e2()  /* nothing */
  #define disable_e2() /* nothing */
#endif

#if (EXTRUDERS > 3) && HAS_E3_ENABLE
  #define enable_e3()  E3_ENABLE_WRITE( E_ENABLE_ON)
  #define disable_e3() E3_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define enable_e3()  /* nothing */
  #define disable_e3() /* nothing */
#endif

/**
 * The axis order in all axis related arrays is X, Y, Z, E
 */
#define NUM_AXIS 4

/**
 * Axis indices as enumerated constants
 *
 * A_AXIS and B_AXIS are used by COREXY printers
 * X_HEAD and Y_HEAD is used for systems that don't have a 1:1 relationship between X_AXIS and X Head movement, like CoreXY bots.
 */
enum AxisEnum {X_AXIS = 0, A_AXIS = 0, Y_AXIS = 1, B_AXIS = 1, Z_AXIS = 2, C_AXIS = 2, E_AXIS = 3, X_HEAD = 4, Y_HEAD = 5, Z_HEAD = 5};

enum EndstopEnum {X_MIN = 0, Y_MIN = 1, Z_MIN = 2, Z_MIN_PROBE = 3, X_MAX = 4, Y_MAX = 5, Z_MAX = 6, Z2_MIN = 7, Z2_MAX = 8};

void enable_all_steppers();
void disable_all_steppers();

void FlushSerialRequestResend();
void ok_to_send();

void reset_bed_level();
void prepare_move();
void kill(const char*);
void Stop();

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  void filrunout();
#endif

/**
 * Debug flags - not yet widely applied
 */
enum DebugFlags {
  DEBUG_NONE          = 0,
  DEBUG_ECHO          = _BV(0),
  DEBUG_INFO          = _BV(1),
  DEBUG_ERRORS        = _BV(2),
  DEBUG_DRYRUN        = _BV(3),
  DEBUG_COMMUNICATION = _BV(4),
  DEBUG_LEVELING      = _BV(5)
};
extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (DEBUG_## F))

extern bool Running;
inline bool IsRunning() { return  Running; }
inline bool IsStopped() { return !Running; }

bool enqueue_and_echo_command(const char* cmd, bool say_ok=false); //put a single ASCII command at the end of the current buffer or return false when it is full
void enqueue_and_echo_command_now(const char* cmd); // enqueue now, only return when the command has been enqueued
void enqueue_and_echo_commands_P(const char* cmd); //put one or many ASCII commands at the end of the current buffer, read from flash

void prepare_arc_move(char isclockwise);
void clamp_to_software_endstops(float target[3]);

extern millis_t previous_cmd_ms;
inline void refresh_cmd_timeout() { previous_cmd_ms = millis(); }

#if ENABLED(FAST_PWM_FAN)
  void setPwmFrequency(uint8_t pin, int val);
#endif

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif

extern bool axis_relative_modes[];
extern int feedrate_multiplier;
extern bool volumetric_enabled;
extern int extruder_multiplier[EXTRUDERS]; // sets extrude multiply factor (in percent) for each extruder individually
extern float filament_size[EXTRUDERS]; // cross-sectional area of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder.
extern float volumetric_multiplier[EXTRUDERS]; // reciprocal of cross-sectional area of filament (in square millimeters), stored this way to reduce computational burden in planner
extern float current_position[NUM_AXIS];
extern float home_offset[3]; // axis[n].home_offset
extern float min_pos[3]; // axis[n].min_pos
extern float max_pos[3]; // axis[n].max_pos
extern bool axis_known_position[3]; // axis[n].is_known
extern bool axis_homed[3]; // axis[n].is_homed

#if ENABLED(DELTA)
  #ifndef DELTA_RADIUS_TRIM_TOWER_1
    #define DELTA_RADIUS_TRIM_TOWER_1 0.0
  #endif
  #ifndef DELTA_RADIUS_TRIM_TOWER_2
    #define DELTA_RADIUS_TRIM_TOWER_2 0.0
  #endif
  #ifndef DELTA_RADIUS_TRIM_TOWER_3
    #define DELTA_RADIUS_TRIM_TOWER_3 0.0
  #endif
  #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_1
    #define DELTA_DIAGONAL_ROD_TRIM_TOWER_1 0.0
  #endif
  #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_2
    #define DELTA_DIAGONAL_ROD_TRIM_TOWER_2 0.0
  #endif
  #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_3
    #define DELTA_DIAGONAL_ROD_TRIM_TOWER_3 0.0
  #endif
  extern float delta[3];
  extern float endstop_adj[3]; // axis[n].endstop_adj
  extern float delta_radius;
  extern float delta_diagonal_rod;
  extern float delta_segments_per_second;
  extern float delta_diagonal_rod_trim_tower_1;
  extern float delta_diagonal_rod_trim_tower_2;
  extern float delta_diagonal_rod_trim_tower_3;
  void calculate_delta(float cartesian[3]);
  void recalc_delta_settings(float radius, float diagonal_rod);
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    extern int delta_grid_spacing[2];
    void adjust_delta(float cartesian[3]);
  #endif
#elif ENABLED(SCARA)
  extern float axis_scaling[3];  // Build size scaling
  void calculate_delta(float cartesian[3]);
  void calculate_SCARA_forward_Transform(float f_scara[3]);
#endif

#if ENABLED(Z_DUAL_ENDSTOPS)
  extern float z_endstop_adj;
#endif

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  extern float zprobe_zoffset;
#endif

#if ENABLED(PREVENT_DANGEROUS_EXTRUDE)
  extern float extrude_min_temp;
#endif

#if FAN_COUNT > 0
  extern int fanSpeeds[FAN_COUNT];
#endif

#if ENABLED(BARICUDA)
  extern int ValvePressure;
  extern int EtoPPressure;
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  extern float filament_width_nominal;  //holds the theoretical filament diameter i.e., 3.00 or 1.75
  extern bool filament_sensor;  //indicates that filament sensor readings should control extrusion
  extern float filament_width_meas; //holds the filament diameter as accurately measured
  extern int8_t measurement_delay[];  //ring buffer to delay measurement
  extern int delay_index1, delay_index2;  //ring buffer index. used by planner, temperature, and main code
  extern float delay_dist; //delay distance counter
  extern int meas_delay_cm; //delay distance
#endif

#if ENABLED(PID_ADD_EXTRUSION_RATE)
  extern int lpq_len;
#endif

#if ENABLED(FWRETRACT)
  extern bool autoretract_enabled;
  extern bool retracted[EXTRUDERS]; // extruder[n].retracted
  extern float retract_length, retract_length_swap, retract_feedrate, retract_zlift;
  extern float retract_recover_length, retract_recover_length_swap, retract_recover_feedrate;
#endif

extern millis_t print_job_start_ms;
extern millis_t print_job_stop_ms;

// Handling multiple extruders pins
extern uint8_t active_extruder;

#if ENABLED(DIGIPOT_I2C)
  extern void digipot_i2c_set_current(int channel, float current);
  extern void digipot_i2c_init();
#endif

#if HAS_TEMP_0 || HAS_TEMP_BED || ENABLED(HEATER_0_USES_MAX6675)
  void print_heaterstates();
#endif

extern void calculate_volumetric_multipliers();

// Print job timer related functions
millis_t print_job_timer();
bool print_job_start(millis_t t = 0);
bool print_job_stop(bool force = false);

#endif //MARLIN_H
