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
 *
 * About Marlin
 *
 * This firmware is a mashup between Sprinter and grbl.
 *  - https://github.com/kliment/Sprinter
 *  - https://github.com/simen/grbl/tree
 *
 * It has preliminary support for Matthew Roberts advance algorithm
 *  - http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
 */

#include "Marlin.h"

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  #include "vector_3.h"
  #if ENABLED(AUTO_BED_LEVELING_GRID)
    #include "qr_solve.h"
  #endif
#endif // AUTO_BED_LEVELING_FEATURE

#if ENABLED(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "cardreader.h"
#include "configuration_store.h"
#include "language.h"
#include "pins_arduino.h"
#include "math.h"
#include "buzzer.h"

#if ENABLED(USE_WATCHDOG)
  #include "watchdog.h"
#endif

#if ENABLED(BLINKM)
  #include "blinkm.h"
  #include "Wire.h"
#endif

#if HAS_SERVOS
  #include "servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "stepper_dac.h"
#endif

/**
 * Look here for descriptions of G-codes:
 *  - http://linuxcnc.org/handbook/gcode/g-code.html
 *  - http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes
 *
 * Help us document these G-codes online:
 *  - https://github.com/MarlinFirmware/Marlin/wiki/G-Code-in-Marlin
 *  - http://reprap.org/wiki/G-code
 *
 * -----------------
 * Implemented Codes
 * -----------------
 *
 * "G" Codes
 *
 * G0  -> G1
 * G1  - Coordinated Movement X Y Z E
 * G2  - CW ARC
 * G3  - CCW ARC
 * G4  - Dwell S<seconds> or P<milliseconds>
 * G10 - retract filament according to settings of M207
 * G11 - retract recover filament according to settings of M208
 * G28 - Home one or more axes
 * G29 - Detailed Z probe, probes the bed at 3 or more points.  Will fail if you haven't homed yet.
 * G30 - Single Z probe, probes bed at current XY location.
 * G31 - Dock sled (Z_PROBE_SLED only)
 * G32 - Undock sled (Z_PROBE_SLED only)
 * G90 - Use Absolute Coordinates
 * G91 - Use Relative Coordinates
 * G92 - Set current position to coordinates given
 *
 * "M" Codes
 *
 * M0   - Unconditional stop - Wait for user to press a button on the LCD (Only if ULTRA_LCD is enabled)
 * M1   - Same as M0
 * M17  - Enable/Power all stepper motors
 * M18  - Disable all stepper motors; same as M84
 * M20  - List SD card
 * M21  - Init SD card
 * M22  - Release SD card
 * M23  - Select SD file (M23 filename.g)
 * M24  - Start/resume SD print
 * M25  - Pause SD print
 * M26  - Set SD position in bytes (M26 S12345)
 * M27  - Report SD print status
 * M28  - Start SD write (M28 filename.g)
 * M29  - Stop SD write
 * M30  - Delete file from SD (M30 filename.g)
 * M31  - Output time since last M109 or SD card start to serial
 * M32  - Select file and start SD print (Can be used _while_ printing from SD card files):
 *        syntax "M32 /path/filename#", or "M32 S<startpos bytes> !filename#"
 *        Call gcode file : "M32 P !filename#" and return to caller file after finishing (similar to #include).
 *        The '#' is necessary when calling from within sd files, as it stops buffer prereading
 * M33  - Get the longname version of a path
 * M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
 * M48  - Measure Z_Probe repeatability. M48 [P # of points] [X position] [Y position] [V_erboseness #] [E_ngage Probe] [L # of legs of travel]
 * M80  - Turn on Power Supply
 * M81  - Turn off Power Supply
 * M82  - Set E codes absolute (default)
 * M83  - Set E codes relative while in Absolute Coordinates (G90) mode
 * M84  - Disable steppers until next move,
 *        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
 * M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 * M92  - Set axis_steps_per_unit - same syntax as G92
 * M104 - Set extruder target temp
 * M105 - Read current temp
 * M106 - Fan on
 * M107 - Fan off
 * M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
 *        Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
 *        IF AUTOTEMP is enabled, S<mintemp> B<maxtemp> F<factor>. Exit autotemp by any M109 without F
 * M110 - Set the current line number
 * M111 - Set debug flags with S<mask>. See flag bits defined in Marlin.h.
 * M112 - Emergency stop
 * M114 - Output current position to serial port
 * M115 - Capabilities string
 * M117 - Display a message on the controller screen
 * M119 - Output Endstop status to serial port
 * M120 - Enable endstop detection
 * M121 - Disable endstop detection
 * M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
 * M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
 * M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
 * M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
 * M140 - Set bed target temp
 * M145 - Set the heatup state H<hotend> B<bed> F<fan speed> for S<material> (0=PLA, 1=ABS)
 * M150 - Set BlinkM Color Output R: Red<0-255> U(!): Green<0-255> B: Blue<0-255> over i2c, G for green does not work.
 * M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
 *        Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
 * M200 - set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).:D<millimeters>-
 * M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 * M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
 * M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
 * M204 - Set default acceleration: P for Printing moves, R for Retract only (no X, Y, Z) moves and T for Travel (non printing) moves (ex. M204 P800 T3000 R9000) in mm/sec^2
 * M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
 * M206 - Set additional homing offset
 * M207 - Set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop], stays in mm regardless of M200 setting
 * M208 - Set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
 * M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
 * M218 - Set hotend offset (in mm): T<extruder_number> X<offset_on_X> Y<offset_on_Y>
 * M220 - Set speed factor override percentage: S<factor in percent>
 * M221 - Set extrude factor override percentage: S<factor in percent>
 * M226 - Wait until the specified pin reaches the state required: P<pin number> S<pin state>
 * M240 - Trigger a camera to take a photograph
 * M250 - Set LCD contrast C<contrast value> (value 0..63)
 * M280 - Set servo position absolute. P: servo index, S: angle or microseconds
 * M300 - Play beep sound S<frequency Hz> P<duration ms>
 * M301 - Set PID parameters P I and D
 * M302 - Allow cold extrudes, or set the minimum extrude S<temperature>.
 * M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
 * M304 - Set bed PID parameters P I and D
 * M380 - Activate solenoid on active extruder
 * M381 - Disable all solenoids
 * M400 - Finish all moves
 * M401 - Lower Z probe if present
 * M402 - Raise Z probe if present
 * M404 - N<dia in mm> Enter the nominal filament width (3mm, 1.75mm ) or will display nominal filament width without parameters
 * M405 - Turn on Filament Sensor extrusion control.  Optional D<delay in cm> to set delay in centimeters between sensor and extruder
 * M406 - Turn off Filament Sensor extrusion control
 * M407 - Display measured filament diameter
 * M410 - Quickstop. Abort all the planned moves
 * M420 - Enable/Disable Mesh Leveling (with current values) S1=enable S0=disable
 * M421 - Set a single Z coordinate in the Mesh Leveling grid. X<mm> Y<mm> Z<mm>
 * M428 - Set the home_offset logically based on the current_position
 * M500 - Store parameters in EEPROM
 * M501 - Read parameters from EEPROM (if you need reset them after you changed them temporarily).
 * M502 - Revert to the default "factory settings". You still need to store them in EEPROM afterwards if you want to.
 * M503 - Print the current settings (from memory not from EEPROM). Use S0 to leave off headings.
 * M540 - Use S[0|1] to enable or disable the stop SD card print on endstop hit (requires ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
 * M600 - Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
 * M665 - Set delta configurations: L<diagonal rod> R<delta radius> S<segments/s>
 * M666 - Set delta endstop adjustment
 * M605 - Set dual x-carriage movement mode: S<mode> [ X<duplication x-offset> R<duplication temp offset> ]
 * M907 - Set digital trimpot motor current using axis codes.
 * M908 - Control digital trimpot directly.
 * M909 - DAC_STEPPER_CURRENT: Print digipot/DAC current value
 * M910 - DAC_STEPPER_CURRENT: Commit digipot/DAC value to external EEPROM via I2C
 * M350 - Set microstepping mode.
 * M351 - Toggle MS1 MS2 pins directly.
 *
 * ************ SCARA Specific - This can change to suit future G-code regulations
 * M360 - SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
 * M361 - SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
 * M362 - SCARA calibration: Move to cal-position PsiA (0 deg calibration)
 * M363 - SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
 * M364 - SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
 * M365 - SCARA calibration: Scaling factor, X, Y, Z axis
 * ************* SCARA End ***************
 *
 * ************ Custom codes - This can change to suit future G-code regulations
 * M100 - Watch Free Memory (For Debugging Only)
 * M851 - Set Z probe's Z offset (mm above extruder -- The value will always be negative)


 * M928 - Start SD logging (M928 filename.g) - ended by M29
 * M999 - Restart after being stopped by error
 *
 * "T" Codes
 *
 * T0-T3 - Select a tool by index (usually an extruder) [ F<mm/min> ]
 *
 */

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  void gcode_M100();
#endif

#if ENABLED(SDSUPPORT)
  CardReader card;
#endif

bool Running = true;

uint8_t marlin_debug_flags = DEBUG_NONE;

static float feedrate = 1500.0, saved_feedrate;
float current_position[NUM_AXIS] = { 0.0 };
static float destination[NUM_AXIS] = { 0.0 };
bool axis_known_position[3] = { false };
bool axis_homed[3] = { false };

static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

static char* current_command, *current_command_args;
static int cmd_queue_index_r = 0;
static int cmd_queue_index_w = 0;
static int commands_in_queue = 0;
static char command_queue[BUFSIZE][MAX_CMD_SIZE];

const float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedrate_multiplier = 100; //100->1 200->2
int saved_feedrate_multiplier;
int extruder_multiplier[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(100);
bool volumetric_enabled = false;
float filament_size[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(DEFAULT_NOMINAL_FILAMENT_DIA);
float volumetric_multiplier[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(1.0);
float home_offset[3] = { 0 };
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };

#if FAN_COUNT > 0
  int fanSpeeds[FAN_COUNT] = { 0 };
#endif

uint8_t active_extruder = 0;
bool cancel_heatup = false;

const char errormagic[] PROGMEM = "Error:";
const char echomagic[] PROGMEM = "echo:";
const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};

static bool relative_mode = false;  //Determines Absolute or Relative Coordinates
static int serial_count = 0;
static char* seen_pointer; ///< A pointer to find chars in the command string (X, Y, Z, E, etc.)
const char* queued_commands_P = NULL; /* pointer to the current line in the active sequence of commands, or NULL when none */
const int sensitive_pins[] = SENSITIVE_PINS; ///< Sensitive pin list for M42
// Inactivity shutdown
millis_t previous_cmd_ms = 0;
static millis_t max_inactive_time = 0;
static millis_t stepper_inactive_time = (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000L;
millis_t print_job_start_ms = 0; ///< Print job start time
millis_t print_job_stop_ms = 0;  ///< Print job stop time
static uint8_t target_extruder;

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  int xy_travel_speed = XY_TRAVEL_SPEED;
  float zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif

#if ENABLED(Z_DUAL_ENDSTOPS) && DISABLED(DELTA)
  float z_endstop_adj = 0;
#endif

// Extruder offsets
#if EXTRUDERS > 1
  #ifndef EXTRUDER_OFFSET_X
    #define EXTRUDER_OFFSET_X { 0 }
  #endif
  #ifndef EXTRUDER_OFFSET_Y
    #define EXTRUDER_OFFSET_Y { 0 }
  #endif
  float extruder_offset[][EXTRUDERS] = {
    EXTRUDER_OFFSET_X,
    EXTRUDER_OFFSET_Y
    #if ENABLED(DUAL_X_CARRIAGE)
      , { 0 } // supports offsets in XYZ plane
    #endif
  };
#endif

#if HAS_SERVO_ENDSTOPS
  const int servo_endstop_id[] = SERVO_ENDSTOP_IDS;
  const int servo_endstop_angle[][2] = SERVO_ENDSTOP_ANGLES;
#endif

#if ENABLED(BARICUDA)
  int ValvePressure = 0;
  int EtoPPressure = 0;
#endif

#if ENABLED(FWRETRACT)

  bool autoretract_enabled = false;
  bool retracted[EXTRUDERS] = { false };
  bool retracted_swap[EXTRUDERS] = { false };

  float retract_length = RETRACT_LENGTH;
  float retract_length_swap = RETRACT_LENGTH_SWAP;
  float retract_feedrate = RETRACT_FEEDRATE;
  float retract_zlift = RETRACT_ZLIFT;
  float retract_recover_length = RETRACT_RECOVER_LENGTH;
  float retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
  float retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;

#endif // FWRETRACT

#if ENABLED(ULTIPANEL) && HAS_POWER_SWITCH
  bool powersupply =
    #if ENABLED(PS_DEFAULT_OFF)
      false
    #else
      true
    #endif
  ;
#endif

#if ENABLED(DELTA)

  #define TOWER_1 X_AXIS
  #define TOWER_2 Y_AXIS
  #define TOWER_3 Z_AXIS

  float delta[3] = { 0 };
  #define SIN_60 0.8660254037844386
  #define COS_60 0.5
  float endstop_adj[3] = { 0 };
  // these are the default values, can be overriden with M665
  float delta_radius = DELTA_RADIUS;
  float delta_tower1_x = -SIN_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_1); // front left tower
  float delta_tower1_y = -COS_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_1);
  float delta_tower2_x =  SIN_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_2); // front right tower
  float delta_tower2_y = -COS_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_2);
  float delta_tower3_x = 0;                                                    // back middle tower
  float delta_tower3_y = (delta_radius + DELTA_RADIUS_TRIM_TOWER_3);
  float delta_diagonal_rod = DELTA_DIAGONAL_ROD;
  float delta_diagonal_rod_trim_tower_1 = DELTA_DIAGONAL_ROD_TRIM_TOWER_1;
  float delta_diagonal_rod_trim_tower_2 = DELTA_DIAGONAL_ROD_TRIM_TOWER_2;
  float delta_diagonal_rod_trim_tower_3 = DELTA_DIAGONAL_ROD_TRIM_TOWER_3;
  float delta_diagonal_rod_2_tower_1 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_1);
  float delta_diagonal_rod_2_tower_2 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_2);
  float delta_diagonal_rod_2_tower_3 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_3);
  //float delta_diagonal_rod_2 = sq(delta_diagonal_rod);
  float delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    int delta_grid_spacing[2] = { 0, 0 };
    float bed_level[AUTO_BED_LEVELING_GRID_POINTS][AUTO_BED_LEVELING_GRID_POINTS];
  #endif
#else
  static bool home_all_axis = true;
#endif

#if ENABLED(SCARA)
  float delta_segments_per_second = SCARA_SEGMENTS_PER_SECOND;
  static float delta[3] = { 0 };
  float axis_scaling[3] = { 1, 1, 1 };    // Build size scaling, default to 1
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  //Variables for Filament Sensor input
  float filament_width_nominal = DEFAULT_NOMINAL_FILAMENT_DIA;  //Set nominal filament width, can be changed with M404
  bool filament_sensor = false;  //M405 turns on filament_sensor control, M406 turns it off
  float filament_width_meas = DEFAULT_MEASURED_FILAMENT_DIA; //Stores the measured filament diameter
  int8_t measurement_delay[MAX_MEASUREMENT_DELAY + 1]; //ring buffer to delay measurement  store extruder factor after subtracting 100
  int delay_index1 = 0;  //index into ring buffer
  int delay_index2 = -1;  //index into ring buffer - set to -1 on startup to indicate ring buffer needs to be initialized
  float delay_dist = 0; //delay distance counter
  int meas_delay_cm = MEASUREMENT_DELAY_CM;  //distance delay setting
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  static bool filrunoutEnqueued = false;
#endif

static bool send_ok[BUFSIZE];

#if HAS_SERVOS
  Servo servo[NUM_SERVOS];
#endif

#ifdef CHDK
  unsigned long chdkHigh = 0;
  boolean chdkActive = false;
#endif

#if ENABLED(PID_ADD_EXTRUSION_RATE)
  int lpq_len = 20;
#endif

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  // States for managing Marlin and host communication
  // Marlin sends messages if blocked or busy
  enum MarlinBusyState {
    NOT_BUSY,           // Not in a handler
    IN_HANDLER,         // Processing a GCode
    IN_PROCESS,         // Known to be blocking command input (as in G29)
    PAUSED_FOR_USER,    // Blocking pending any input
    PAUSED_FOR_INPUT    // Blocking pending text input (concept)
  };

  static MarlinBusyState busy_state = NOT_BUSY;
  static millis_t next_busy_signal_ms = -1;
  #define KEEPALIVE_STATE(n) do{ busy_state = n; }while(0)
#else
  #define host_keepalive() ;
  #define KEEPALIVE_STATE(n) ;
#endif // HOST_KEEPALIVE_FEATURE

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

void get_available_commands();
void process_next_command();

void plan_arc(float target[NUM_AXIS], float* offset, uint8_t clockwise);

bool setTargetedHotend(int code);

void serial_echopair_P(const char* s_P, int v)           { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, long v)          { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, float v)         { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, double v)        { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

void gcode_M114();

#if ENABLED(PREVENT_DANGEROUS_EXTRUDE)
  float extrude_min_temp = EXTRUDE_MINTEMP;
#endif

#if ENABLED(HAS_Z_MIN_PROBE)
  extern volatile bool z_probe_is_active;
#endif

#if ENABLED(SDSUPPORT)
  #include "SdFatUtil.h"
  int freeMemory() { return SdFatUtil::FreeRam(); }
#else
extern "C" {
  extern unsigned int __bss_end;
  extern unsigned int __heap_start;
  extern void* __brkval;

  int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
  }
}
#endif //!SDSUPPORT

/**
 * Inject the next "immediate" command, when possible.
 * Return true if any immediate commands remain to inject.
 */
static bool drain_queued_commands_P() {
  if (queued_commands_P != NULL) {
    size_t i = 0;
    char c, cmd[30];
    strncpy_P(cmd, queued_commands_P, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0';
    while ((c = cmd[i]) && c != '\n') i++; // find the end of this gcode command
    cmd[i] = '\0';
    if (enqueue_and_echo_command(cmd)) {   // success?
      if (c)                               // newline char?
        queued_commands_P += i + 1;        // advance to the next command
      else
        queued_commands_P = NULL;          // nul char? no more commands
    }
  }
  return (queued_commands_P != NULL);      // return whether any more remain
}

/**
 * Record one or many commands to run from program memory.
 * Aborts the current queue, if any.
 * Note: drain_queued_commands_P() must be called repeatedly to drain the commands afterwards
 */
void enqueue_and_echo_commands_P(const char* pgcode) {
  queued_commands_P = pgcode;
  drain_queued_commands_P(); // first command executed asap (when possible)
}

/**
 * Once a new command is in the ring buffer, call this to commit it
 */
inline void _commit_command(bool say_ok) {
  send_ok[cmd_queue_index_w] = say_ok;
  cmd_queue_index_w = (cmd_queue_index_w + 1) % BUFSIZE;
  commands_in_queue++;
}

/**
 * Copy a command directly into the main command buffer, from RAM.
 * Returns true if successfully adds the command
 */
inline bool _enqueuecommand(const char* cmd, bool say_ok=false) {
  if (*cmd == ';' || commands_in_queue >= BUFSIZE) return false;
  strcpy(command_queue[cmd_queue_index_w], cmd);
  _commit_command(say_ok);
  return true;
}

void enqueue_and_echo_command_now(const char* cmd) {
  while (!enqueue_and_echo_command(cmd)) idle();
}

/**
 * Enqueue with Serial Echo
 */
bool enqueue_and_echo_command(const char* cmd, bool say_ok/*=false*/) {
  if (_enqueuecommand(cmd, say_ok)) {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_Enqueueing);
    SERIAL_ECHO(cmd);
    SERIAL_ECHOLNPGM("\"");
    return true;
  }
  return false;
}

void setup_killpin() {
  #if HAS_KILL
    SET_INPUT(KILL_PIN);
    WRITE(KILL_PIN, HIGH);
  #endif
}

void setup_filrunoutpin() {
  #if HAS_FILRUNOUT
    pinMode(FILRUNOUT_PIN, INPUT);
    #if ENABLED(ENDSTOPPULLUP_FIL_RUNOUT)
      WRITE(FILRUNOUT_PIN, HIGH);
    #endif
  #endif
}

// Set home pin
void setup_homepin(void) {
  #if HAS_HOME
    SET_INPUT(HOME_PIN);
    WRITE(HOME_PIN, HIGH);
  #endif
}


void setup_photpin() {
  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif
}

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

   #if HAS_SERVO_ENDSTOPS

    z_probe_is_active = false;

    /**
     * Set position of all defined Servo Endstops
     *
     * ** UNSAFE! - NEEDS UPDATE! **
     *
     * The servo might be deployed and positioned too low to stow
     * when starting up the machine or rebooting the board.
     * There's no way to know where the nozzle is positioned until
     * homing has been done - no homing with z-probe without init!
     *
     */
    for (int i = 0; i < 3; i++)
      if (servo_endstop_id[i] >= 0)
        servo[servo_endstop_id[i]].move(servo_endstop_angle[i][1]);

  #endif // HAS_SERVO_ENDSTOPS

}

/**
 * Stepper Reset (RigidBoard, et.al.)
 */
#if HAS_STEPPER_RESET
  void disableStepperDrivers() {
    pinMode(STEPPER_RESET_PIN, OUTPUT);
    digitalWrite(STEPPER_RESET_PIN, LOW);  // drive it down to hold in reset motor driver chips
  }
  void enableStepperDrivers() { pinMode(STEPPER_RESET_PIN, INPUT); }  // set to input, which allows it to be pulled high by pullups
#endif

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

  #ifdef DISABLE_JTAG
    // Disable JTAG on AT90USB chips to free up pins for IO
    MCUCR = 0x80;
    MCUCR = 0x80;
  #endif

  setup_killpin();
  setup_filrunoutpin();
  setup_powerhold();

  #if HAS_STEPPER_RESET
    disableStepperDrivers();
  #endif

  MYSERIAL.begin(BAUDRATE);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START;

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = MCUSR;
  if (mcu & 1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if (mcu & 2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if (mcu & 4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if (mcu & 8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if (mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  MCUSR = 0;

  SERIAL_ECHOPGM(MSG_MARLIN);
  SERIAL_ECHOLNPGM(" " SHORT_BUILD_VERSION);

  #ifdef STRING_DISTRIBUTION_DATE
    #ifdef STRING_CONFIG_H_AUTHOR
      SERIAL_ECHO_START;
      SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
      SERIAL_ECHOPGM(STRING_DISTRIBUTION_DATE);
      SERIAL_ECHOPGM(MSG_AUTHOR);
      SERIAL_ECHOLNPGM(STRING_CONFIG_H_AUTHOR);
      SERIAL_ECHOPGM("Compiled: ");
      SERIAL_ECHOLNPGM(__DATE__);
    #endif // STRING_CONFIG_H_AUTHOR
  #endif // STRING_DISTRIBUTION_DATE

  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_FREE_MEMORY);
  SERIAL_ECHO(freeMemory());
  SERIAL_ECHOPGM(MSG_PLANNER_BUFFER_BYTES);
  SERIAL_ECHOLN((int)sizeof(block_t)*BLOCK_BUFFER_SIZE);

  // Send "ok" after commands by default
  for (int8_t i = 0; i < BUFSIZE; i++) send_ok[i] = true;

  // loads data from EEPROM if available else uses defaults (and resets step acceleration rate)
  Config_RetrieveSettings();

  lcd_init();

  tp_init();    // Initialize temperature loop
  plan_init();  // Initialize planner;

  #if ENABLED(USE_WATCHDOG)
    watchdog_init();
  #endif

  st_init();    // Initialize stepper, this enables interrupts!
  setup_photpin();
  servo_init();

  #if HAS_CONTROLLERFAN
    SET_OUTPUT(CONTROLLERFAN_PIN); //Set pin used for driver cooling fan
  #endif

  #if HAS_STEPPER_RESET
    enableStepperDrivers();
  #endif

  #if ENABLED(DIGIPOT_I2C)
    digipot_i2c_init();
  #endif

  #if ENABLED(Z_PROBE_SLED)
    pinMode(SLED_PIN, OUTPUT);
    digitalWrite(SLED_PIN, LOW); // turn it off
  #endif // Z_PROBE_SLED

  setup_homepin();

  #ifdef STAT_LED_RED
    pinMode(STAT_LED_RED, OUTPUT);
    digitalWrite(STAT_LED_RED, LOW); // turn it off
  #endif

  #ifdef STAT_LED_BLUE
    pinMode(STAT_LED_BLUE, OUTPUT);
    digitalWrite(STAT_LED_BLUE, LOW); // turn it off
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

  if (commands_in_queue) {

    #if ENABLED(SDSUPPORT)

      if (card.saving) {
        char* command = command_queue[cmd_queue_index_r];
        if (strstr_P(command, PSTR("M29"))) {
          // M29 closes the file
          card.closefile();
          SERIAL_PROTOCOLLNPGM(MSG_FILE_SAVED);
          ok_to_send();
        }
        else {
          // Write the string from the read buffer to SD
          card.write_command(command);
          if (card.logging)
            process_next_command(); // The card is saving because it's logging
          else
            ok_to_send();
        }
      }
      else
        process_next_command();

    #else

      process_next_command();

    #endif // SDSUPPORT

    commands_in_queue--;
    cmd_queue_index_r = (cmd_queue_index_r + 1) % BUFSIZE;
  }
  checkHitEndstops();
  idle();
}

void gcode_line_error(const char* err, bool doFlush = true) {
  SERIAL_ERROR_START;
  serialprintPGM(err);
  SERIAL_ERRORLN(gcode_LastN);
  //Serial.println(gcode_N);
  if (doFlush) FlushSerialRequestResend();
  serial_count = 0;
}

inline void get_serial_commands() {
  static char serial_line_buffer[MAX_CMD_SIZE];
  static boolean serial_comment_mode = false;

  // If the command buffer is empty for too long,
  // send "wait" to indicate Marlin is still waiting.
  #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
    static millis_t last_command_time = 0;
    millis_t ms = millis();
    if (commands_in_queue == 0 && !MYSERIAL.available() && ms > last_command_time + NO_TIMEOUTS) {
      SERIAL_ECHOLNPGM(MSG_WAIT);
      last_command_time = ms;
    }
  #endif

  /**
   * Loop while serial characters are incoming and the queue is not full
   */
  while (commands_in_queue < BUFSIZE && MYSERIAL.available() > 0) {

    char serial_char = MYSERIAL.read();

    /**
     * If the character ends the line
     */
    if (serial_char == '\n' || serial_char == '\r') {

      serial_comment_mode = false; // end of line == end of comment

      if (!serial_count) continue; // skip empty lines

      serial_line_buffer[serial_count] = 0; // terminate string
      serial_count = 0; //reset buffer

      char* command = serial_line_buffer;

      while (*command == ' ') command++; // skip any leading spaces
      char* npos = (*command == 'N') ? command : NULL; // Require the N parameter to start the line
      char* apos = strchr(command, '*');

      if (npos) {

        boolean M110 = strstr_P(command, PSTR("M110")) != NULL;

        if (M110) {
          char* n2pos = strchr(command + 4, 'N');
          if (n2pos) npos = n2pos;
        }

        gcode_N = strtol(npos + 1, NULL, 10);

        if (gcode_N != gcode_LastN + 1 && !M110) {
          gcode_line_error(PSTR(MSG_ERR_LINE_NO));
          return;
        }

        if (apos) {
          byte checksum = 0, count = 0;
          while (command[count] != '*') checksum ^= command[count++];

          if (strtol(apos + 1, NULL, 10) != checksum) {
            gcode_line_error(PSTR(MSG_ERR_CHECKSUM_MISMATCH));
            return;
          }
          // if no errors, continue parsing
        }
        else {
          gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM));
          return;
        }

        gcode_LastN = gcode_N;
        // if no errors, continue parsing
      }
      else if (apos) { // No '*' without 'N'
        gcode_line_error(PSTR(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM), false);
        return;
      }

      // Movement commands alert when stopped
      if (IsStopped()) {
        char* gpos = strchr(command, 'G');
        if (gpos) {
          int codenum = strtol(gpos + 1, NULL, 10);
          switch (codenum) {
            case 0:
            case 1:
            case 2:
            case 3:
              SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
              LCD_MESSAGEPGM(MSG_STOPPED);
              break;
          }
        }
      }

      // If command was e-stop process now
      if (strcmp(command, "M112") == 0) kill(PSTR(MSG_KILLED));

      #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
        last_command_time = ms;
      #endif

      // Add the command to the queue
      _enqueuecommand(serial_line_buffer, true);
    }
    else if (serial_count >= MAX_CMD_SIZE - 1) {
      // Keep fetching, but ignore normal characters beyond the max length
      // The command will be injected when EOL is reached
    }
    else if (serial_char == '\\') {  // Handle escapes
      if (MYSERIAL.available() > 0) {
        // if we have one more character, copy it over
        serial_char = MYSERIAL.read();
        if (!serial_comment_mode) serial_line_buffer[serial_count++] = serial_char;
      }
      // otherwise do nothing
    }
    else { // it's not a newline, carriage return or escape char
      if (serial_char == ';') serial_comment_mode = true;
      if (!serial_comment_mode) serial_line_buffer[serial_count++] = serial_char;
    }

  } // queue has space, serial has data
}

#if ENABLED(SDSUPPORT)

  inline void get_sdcard_commands() {
    static bool stop_buffering = false,
                sd_comment_mode = false;

    if (!card.sdprinting) return;

    /**
     * '#' stops reading from SD to the buffer prematurely, so procedural
     * macro calls are possible. If it occurs, stop_buffering is triggered
     * and the buffer is run dry; this character _can_ occur in serial com
     * due to checksums, however, no checksums are used in SD printing.
     */

    if (commands_in_queue == 0) stop_buffering = false;

    uint16_t sd_count = 0;
    bool card_eof = card.eof();
    while (commands_in_queue < BUFSIZE && !card_eof && !stop_buffering) {
      int16_t n = card.get();
      char sd_char = (char)n;
      card_eof = card.eof();
      if (card_eof || n == -1
          || sd_char == '\n' || sd_char == '\r'
          || ((sd_char == '#' || sd_char == ':') && !sd_comment_mode)
      ) {
        if (card_eof) {
          SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
          print_job_stop(true);
          char time[30];
          millis_t t = print_job_timer();
          int hours = t / 60 / 60, minutes = (t / 60) % 60;
          sprintf_P(time, PSTR("%i " MSG_END_HOUR " %i " MSG_END_MINUTE), hours, minutes);
          SERIAL_ECHO_START;
          SERIAL_ECHOLN(time);
          lcd_setstatus(time, true);
          card.printingHasFinished();
          card.checkautostart(true);
        }
        if (sd_char == '#') stop_buffering = true;

        sd_comment_mode = false; //for new command

        if (!sd_count) continue; //skip empty lines

        command_queue[cmd_queue_index_w][sd_count] = '\0'; //terminate string
        sd_count = 0; //clear buffer

        _commit_command(false);
      }
      else if (sd_count >= MAX_CMD_SIZE - 1) {
        /**
         * Keep fetching, but ignore normal characters beyond the max length
         * The command will be injected when EOL is reached
         */
      }
      else {
        if (sd_char == ';') sd_comment_mode = true;
        if (!sd_comment_mode) command_queue[cmd_queue_index_w][sd_count++] = sd_char;
      }
    }
  }

#endif // SDSUPPORT

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queue (queued_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void get_available_commands() {

  // if any immediate commands remain, don't get other commands yet
  if (drain_queued_commands_P()) return;

  get_serial_commands();

  #if ENABLED(SDSUPPORT)
    get_sdcard_commands();
  #endif
}

bool code_has_value() {
  int i = 1;
  char c = seen_pointer[i];
  while (c == ' ') c = seen_pointer[++i];
  if (c == '-' || c == '+') c = seen_pointer[++i];
  if (c == '.') c = seen_pointer[++i];
  return NUMERIC(c);
}

float code_value() {
  float ret;
  char* e = strchr(seen_pointer, 'E');
  if (e) {
    *e = 0;
    ret = strtod(seen_pointer + 1, NULL);
    *e = 'E';
  }
  else
    ret = strtod(seen_pointer + 1, NULL);
  return ret;
}

long code_value_long() { return strtol(seen_pointer + 1, NULL, 10); }

int16_t code_value_short() { return (int16_t)strtol(seen_pointer + 1, NULL, 10); }

bool code_seen(char code) {
  seen_pointer = strchr(current_command_args, code);
  return (seen_pointer != NULL); // Return TRUE if the code-letter was found
}

#define DEFINE_PGM_READ_ANY(type, reader)       \
  static inline type pgm_read_any(const type *p)  \
  { return pgm_read_##reader##_near(p); }

DEFINE_PGM_READ_ANY(float,       float);
DEFINE_PGM_READ_ANY(signed char, byte);

#define XYZ_CONSTS_FROM_CONFIG(type, array, CONFIG) \
  static const PROGMEM type array##_P[3] =        \
      { X_##CONFIG, Y_##CONFIG, Z_##CONFIG };     \
  static inline type array(int axis)          \
  { return pgm_read_any(&array##_P[axis]); }

XYZ_CONSTS_FROM_CONFIG(float, base_min_pos,   MIN_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_max_pos,   MAX_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_home_pos,  HOME_POS);
XYZ_CONSTS_FROM_CONFIG(float, max_length,     MAX_LENGTH);
XYZ_CONSTS_FROM_CONFIG(float, home_bump_mm,   HOME_BUMP_MM);
XYZ_CONSTS_FROM_CONFIG(signed char, home_dir, HOME_DIR);

#if ENABLED(DUAL_X_CARRIAGE)

  #define DXC_FULL_CONTROL_MODE 0
  #define DXC_AUTO_PARK_MODE    1
  #define DXC_DUPLICATION_MODE  2

  static int dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;

  static float x_home_pos(int extruder) {
    if (extruder == 0)
      return base_home_pos(X_AXIS) + home_offset[X_AXIS];
    else
      /**
       * In dual carriage mode the extruder offset provides an override of the
       * second X-carriage offset when homed - otherwise X2_HOME_POS is used.
       * This allow soft recalibration of the second extruder offset position
       * without firmware reflash (through the M218 command).
       */
      return (extruder_offset[X_AXIS][1] > 0) ? extruder_offset[X_AXIS][1] : X2_HOME_POS;
  }

  static int x_home_dir(int extruder) {
    return (extruder == 0) ? X_HOME_DIR : X2_HOME_DIR;
  }

  static float inactive_extruder_x_pos = X2_MAX_POS; // used in mode 0 & 1
  static bool active_extruder_parked = false; // used in mode 1 & 2
  static float raised_parked_position[NUM_AXIS]; // used in mode 1
  static millis_t delayed_move_time = 0; // used in mode 1
  static float duplicate_extruder_x_offset = DEFAULT_DUPLICATION_X_OFFSET; // used in mode 2
  static float duplicate_extruder_temp_offset = 0; // used in mode 2
  bool extruder_duplication_enabled = false; // used in mode 2

#endif //DUAL_X_CARRIAGE

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void print_xyz(const char* prefix, const float x, const float y, const float z) {
    SERIAL_ECHO(prefix);
    SERIAL_ECHOPAIR(": (", x);
    SERIAL_ECHOPAIR(", ", y);
    SERIAL_ECHOPAIR(", ", z);
    SERIAL_ECHOLNPGM(")");
  }
  void print_xyz(const char* prefix, const float xyz[]) {
    print_xyz(prefix, xyz[X_AXIS], xyz[Y_AXIS], xyz[Z_AXIS]);
  }
#endif

static void set_axis_is_at_home(AxisEnum axis) {

  #if ENABLED(DUAL_X_CARRIAGE)
    if (axis == X_AXIS) {
      if (active_extruder != 0) {
        current_position[X_AXIS] = x_home_pos(active_extruder);
                 min_pos[X_AXIS] = X2_MIN_POS;
                 max_pos[X_AXIS] = max(extruder_offset[X_AXIS][1], X2_MAX_POS);
        return;
      }
      else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
        float xoff = home_offset[X_AXIS];
        current_position[X_AXIS] = base_home_pos(X_AXIS) + xoff;
                 min_pos[X_AXIS] = base_min_pos(X_AXIS) + xoff;
                 max_pos[X_AXIS] = min(base_max_pos(X_AXIS) + xoff, max(extruder_offset[X_AXIS][1], X2_MAX_POS) - duplicate_extruder_x_offset);
        return;
      }
    }
  #endif

  #if ENABLED(SCARA)

    if (axis == X_AXIS || axis == Y_AXIS) {

      float homeposition[3];
      for (int i = 0; i < 3; i++) homeposition[i] = base_home_pos(i);

      // SERIAL_ECHOPGM("homeposition[x]= "); SERIAL_ECHO(homeposition[0]);
      // SERIAL_ECHOPGM("homeposition[y]= "); SERIAL_ECHOLN(homeposition[1]);

      /**
       * Works out real Homeposition angles using inverse kinematics,
       * and calculates homing offset using forward kinematics
       */
      calculate_delta(homeposition);

      // SERIAL_ECHOPGM("base Theta= "); SERIAL_ECHO(delta[X_AXIS]);
      // SERIAL_ECHOPGM(" base Psi+Theta="); SERIAL_ECHOLN(delta[Y_AXIS]);

      for (int i = 0; i < 2; i++) delta[i] -= home_offset[i];

      // SERIAL_ECHOPGM("addhome X="); SERIAL_ECHO(home_offset[X_AXIS]);
      // SERIAL_ECHOPGM(" addhome Y="); SERIAL_ECHO(home_offset[Y_AXIS]);
      // SERIAL_ECHOPGM(" addhome Theta="); SERIAL_ECHO(delta[X_AXIS]);
      // SERIAL_ECHOPGM(" addhome Psi+Theta="); SERIAL_ECHOLN(delta[Y_AXIS]);

      calculate_SCARA_forward_Transform(delta);

      // SERIAL_ECHOPGM("Delta X="); SERIAL_ECHO(delta[X_AXIS]);
      // SERIAL_ECHOPGM(" Delta Y="); SERIAL_ECHOLN(delta[Y_AXIS]);

      current_position[axis] = delta[axis];

      /**
       * SCARA home positions are based on configuration since the actual
       * limits are determined by the inverse kinematic transform.
       */
      min_pos[axis] = base_min_pos(axis); // + (delta[axis] - base_home_pos(axis));
      max_pos[axis] = base_max_pos(axis); // + (delta[axis] - base_home_pos(axis));
    }
    else
  #endif
  {
    current_position[axis] = base_home_pos(axis) + home_offset[axis];
    min_pos[axis] = base_min_pos(axis) + home_offset[axis];
    max_pos[axis] = base_max_pos(axis) + home_offset[axis];

    #if ENABLED(AUTO_BED_LEVELING_FEATURE) && Z_HOME_DIR < 0
      if (axis == Z_AXIS) current_position[Z_AXIS] -= zprobe_zoffset;
    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("set_axis_is_at_home ", (unsigned long)axis);
        SERIAL_ECHOPAIR(" > (home_offset[axis]==", home_offset[axis]);
        print_xyz(") > current_position", current_position);
      }
    #endif
  }
}

/**
 * Some planner shorthand inline functions
 */
inline void set_homing_bump_feedrate(AxisEnum axis) {
  const int homing_bump_divisor[] = HOMING_BUMP_DIVISOR;
  int hbd = homing_bump_divisor[axis];
  if (hbd < 1) {
    hbd = 10;
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Warning: Homing Bump Divisor < 1");
  }
  feedrate = homing_feedrate[axis] / hbd;
}
inline void line_to_current_position() {
  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
}
inline void line_to_z(float zPosition) {
  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate / 60, active_extruder);
}
inline void line_to_destination(float mm_m) {
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], mm_m / 60, active_extruder);
}
inline void line_to_destination() {
  line_to_destination(feedrate);
}
inline void sync_plan_position() {
  plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
#if ENABLED(DELTA) || ENABLED(SCARA)
  inline void sync_plan_position_delta() {
    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
  }
#endif
inline void set_current_to_destination() { memcpy(current_position, destination, sizeof(current_position)); }
inline void set_destination_to_current() { memcpy(destination, current_position, sizeof(destination)); }

static void setup_for_endstop_move() {
  saved_feedrate = feedrate;
  saved_feedrate_multiplier = feedrate_multiplier;
  feedrate_multiplier = 100;
  refresh_cmd_timeout();
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOLNPGM("setup_for_endstop_move > enable_endstops(true)");
    }
  #endif
  enable_endstops(true);
}

#if ENABLED(AUTO_BED_LEVELING_FEATURE)

  #if ENABLED(DELTA)
    /**
     * Calculate delta, start a line, and set current_position to destination
     */
    void prepare_move_raw() {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("prepare_move_raw > destination", destination);
        }
      #endif
      refresh_cmd_timeout();
      calculate_delta(destination);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], (feedrate / 60) * (feedrate_multiplier / 100.0), active_extruder);
      set_current_to_destination();
    }
  #endif

  #if ENABLED(AUTO_BED_LEVELING_GRID)

    #if DISABLED(DELTA)

      static void set_bed_level_equation_lsq(double* plane_equation_coefficients) {
        vector_3 planeNormal = vector_3(-plane_equation_coefficients[0], -plane_equation_coefficients[1], 1);
        planeNormal.debug("planeNormal");
        plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
        //bedLevel.debug("bedLevel");

        //plan_bed_level_matrix.debug("bed level before");
        //vector_3 uncorrected_position = plan_get_position_mm();
        //uncorrected_position.debug("position before");

        vector_3 corrected_position = plan_get_position();
        //corrected_position.debug("position after");
        current_position[X_AXIS] = corrected_position.x;
        current_position[Y_AXIS] = corrected_position.y;
        current_position[Z_AXIS] = corrected_position.z;

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("set_bed_level_equation_lsq > current_position", current_position);
          }
        #endif

        sync_plan_position();
      }

    #endif // !DELTA

  #else // !AUTO_BED_LEVELING_GRID

    static void set_bed_level_equation_3pts(float z_at_pt_1, float z_at_pt_2, float z_at_pt_3) {

      plan_bed_level_matrix.set_to_identity();

      vector_3 pt1 = vector_3(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, z_at_pt_1);
      vector_3 pt2 = vector_3(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, z_at_pt_2);
      vector_3 pt3 = vector_3(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, z_at_pt_3);
      vector_3 planeNormal = vector_3::cross(pt1 - pt2, pt3 - pt2).get_normal();

      if (planeNormal.z < 0) {
        planeNormal.x = -planeNormal.x;
        planeNormal.y = -planeNormal.y;
        planeNormal.z = -planeNormal.z;
      }

      plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

      vector_3 corrected_position = plan_get_position();
      current_position[X_AXIS] = corrected_position.x;
      current_position[Y_AXIS] = corrected_position.y;
      current_position[Z_AXIS] = corrected_position.z;

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("set_bed_level_equation_3pts > current_position", current_position);
        }
      #endif

      sync_plan_position();
    }

  #endif // !AUTO_BED_LEVELING_GRID

  static void run_z_probe() {

    /**
     * To prevent stepper_inactive_time from running out and
     * EXTRUDER_RUNOUT_PREVENT from extruding
     */
    refresh_cmd_timeout();

    #if ENABLED(DELTA)

      float start_z = current_position[Z_AXIS];
      long start_steps = st_get_position(Z_AXIS);

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOLNPGM("run_z_probe (DELTA) 1");
        }
      #endif

      // move down slowly until you find the bed
      feedrate = homing_feedrate[Z_AXIS] / 4;
      destination[Z_AXIS] = -10;
      prepare_move_raw(); // this will also set_current_to_destination
      st_synchronize();
      endstops_hit_on_purpose(); // clear endstop hit flags

      /**
       * We have to let the planner know where we are right now as it
       * is not where we said to go.
       */
      long stop_steps = st_get_position(Z_AXIS);
      float mm = start_z - float(start_steps - stop_steps) / axis_steps_per_unit[Z_AXIS];
      current_position[Z_AXIS] = mm;

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("run_z_probe (DELTA) 2 > current_position", current_position);
        }
      #endif

      sync_plan_position_delta();

    #else // !DELTA

      plan_bed_level_matrix.set_to_identity();
      feedrate = homing_feedrate[Z_AXIS];

      // Move down until the Z probe (or endstop?) is triggered
      float zPosition = -(Z_MAX_LENGTH + 10);
      line_to_z(zPosition);
      st_synchronize();

      // Tell the planner where we ended up - Get this from the stepper handler
      zPosition = st_get_axis_position_mm(Z_AXIS);
      plan_set_position(
        current_position[X_AXIS], current_position[Y_AXIS], zPosition,
        current_position[E_AXIS]
      );

      // move up the retract distance
      zPosition += home_bump_mm(Z_AXIS);
      line_to_z(zPosition);
      st_synchronize();
      endstops_hit_on_purpose(); // clear endstop hit flags

      // move back down slowly to find bed
      set_homing_bump_feedrate(Z_AXIS);

      zPosition -= home_bump_mm(Z_AXIS) * 2;
      line_to_z(zPosition);
      st_synchronize();
      endstops_hit_on_purpose(); // clear endstop hit flags

      // Get the current stepper position after bumping an endstop
      current_position[Z_AXIS] = st_get_axis_position_mm(Z_AXIS);
      sync_plan_position();

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("run_z_probe > current_position", current_position);
        }
      #endif

    #endif // !DELTA
  }

  /**
   *  Plan a move to (X, Y, Z) and set the current_position
   *  The final current_position may not be the one that was requested
   */
  static void do_blocking_move_to(float x, float y, float z) {
    float oldFeedRate = feedrate;

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("do_blocking_move_to", x, y, z);
      }
    #endif

    #if ENABLED(DELTA)

      feedrate = XY_TRAVEL_SPEED;

      destination[X_AXIS] = x;
      destination[Y_AXIS] = y;
      destination[Z_AXIS] = z;
      prepare_move_raw(); // this will also set_current_to_destination
      st_synchronize();

    #else

      feedrate = homing_feedrate[Z_AXIS];

      current_position[Z_AXIS] = z;
      line_to_current_position();
      st_synchronize();

      feedrate = xy_travel_speed;

      current_position[X_AXIS] = x;
      current_position[Y_AXIS] = y;
      line_to_current_position();
      st_synchronize();

    #endif

    feedrate = oldFeedRate;
  }

  inline void do_blocking_move_to_xy(float x, float y) {
    do_blocking_move_to(x, y, current_position[Z_AXIS]);
  }

  inline void do_blocking_move_to_x(float x) {
    do_blocking_move_to(x, current_position[Y_AXIS], current_position[Z_AXIS]);
  }

  inline void do_blocking_move_to_z(float z) {
    do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], z);
  }

  inline void raise_z_after_probing() {
    do_blocking_move_to_z(current_position[Z_AXIS] + Z_RAISE_AFTER_PROBING);
  }

  static void clean_up_after_endstop_move() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("clean_up_after_endstop_move > ENDSTOPS_ONLY_FOR_HOMING > endstops_not_homing()");
      }
    #endif
    endstops_not_homing();
    feedrate = saved_feedrate;
    feedrate_multiplier = saved_feedrate_multiplier;
    refresh_cmd_timeout();
  }

  #if ENABLED(HAS_Z_MIN_PROBE)

  static void deploy_z_probe() {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("deploy_z_probe > current_position", current_position);
      }
    #endif

    if (z_probe_is_active) return;

    #if HAS_SERVO_ENDSTOPS

      // Engage Z Servo endstop if enabled
      if (servo_endstop_id[Z_AXIS] >= 0) servo[servo_endstop_id[Z_AXIS]].move(servo_endstop_angle[Z_AXIS][0]);

    #elif ENABLED(Z_PROBE_ALLEN_KEY)
      feedrate = Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE;

      // If endstop is already false, the Z probe is deployed
      #if ENABLED(Z_MIN_PROBE_ENDSTOP)
        bool z_probe_endstop = (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING);
        if (z_probe_endstop)
      #else
        bool z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
        if (z_min_endstop)
      #endif
        {

          // Move to the start position to initiate deployment
          destination[X_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_1_X;
          destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_1_Y;
          destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_1_Z;
          prepare_move_raw(); // this will also set_current_to_destination

          // Move to engage deployment
          if (Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE != Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE)
            feedrate = Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE;
          if (Z_PROBE_ALLEN_KEY_DEPLOY_2_X != Z_PROBE_ALLEN_KEY_DEPLOY_1_X)
            destination[X_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_2_X;
          if (Z_PROBE_ALLEN_KEY_DEPLOY_2_Y != Z_PROBE_ALLEN_KEY_DEPLOY_1_Y)
            destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_2_Y;
          if (Z_PROBE_ALLEN_KEY_DEPLOY_2_Z != Z_PROBE_ALLEN_KEY_DEPLOY_1_Z)
            destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_2_Z;
          prepare_move_raw();

          #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_3_X
            if (Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE != Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE)
              feedrate = Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE;

            // Move to trigger deployment
            if (Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE != Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE)
              feedrate = Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE;
            if (Z_PROBE_ALLEN_KEY_DEPLOY_3_X != Z_PROBE_ALLEN_KEY_DEPLOY_2_X)
              destination[X_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_3_X;
            if (Z_PROBE_ALLEN_KEY_DEPLOY_3_Y != Z_PROBE_ALLEN_KEY_DEPLOY_2_Y)
              destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_3_Y;
            if (Z_PROBE_ALLEN_KEY_DEPLOY_3_Z != Z_PROBE_ALLEN_KEY_DEPLOY_2_Z)
              destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_3_Z;

            prepare_move_raw();
          #endif
        }

      // Partially Home X,Y for safety
      destination[X_AXIS] = destination[X_AXIS] * 0.75;
      destination[Y_AXIS] = destination[Y_AXIS] * 0.75;
      prepare_move_raw(); // this will also set_current_to_destination

      st_synchronize();

      #if ENABLED(Z_MIN_PROBE_ENDSTOP)
        z_probe_endstop = (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING);
        if (z_probe_endstop)
      #else
        z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
        if (z_min_endstop)
      #endif
        {
          if (IsRunning()) {
            SERIAL_ERROR_START;
            SERIAL_ERRORLNPGM("Z-Probe failed to engage!");
            LCD_ALERTMESSAGEPGM("Err: ZPROBE");
          }
          Stop();
        }

    #endif // Z_PROBE_ALLEN_KEY

    #if ENABLED(FIX_MOUNTED_PROBE)
      // Noting to be done. Just set z_probe_is_active
    #endif

    z_probe_is_active = true;

  }

  static void stow_z_probe(bool doRaise = true) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("stow_z_probe > current_position", current_position);
      }
    #endif

    if (!z_probe_is_active) return;

    #if HAS_SERVO_ENDSTOPS

      // Retract Z Servo endstop if enabled
      if (servo_endstop_id[Z_AXIS] >= 0) {

        #if Z_RAISE_AFTER_PROBING > 0
          if (doRaise) {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("Raise Z (after) by ", (float)Z_RAISE_AFTER_PROBING);
                SERIAL_EOL;
                SERIAL_ECHO("> SERVO_ENDSTOPS > raise_z_after_probing()");
                SERIAL_EOL;
              }
            #endif
            raise_z_after_probing(); // this also updates current_position
            st_synchronize();
          }
        #endif

        // Change the Z servo angle
        servo[servo_endstop_id[Z_AXIS]].move(servo_endstop_angle[Z_AXIS][1]);
      }

    #elif ENABLED(Z_PROBE_ALLEN_KEY)

      // Move up for safety
      feedrate = Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE;

      #if Z_RAISE_AFTER_PROBING > 0
        destination[Z_AXIS] = current_position[Z_AXIS] + Z_RAISE_AFTER_PROBING;
        prepare_move_raw(); // this will also set_current_to_destination
      #endif

      // Move to the start position to initiate retraction
      destination[X_AXIS] = Z_PROBE_ALLEN_KEY_STOW_1_X;
      destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_STOW_1_Y;
      destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_STOW_1_Z;
      prepare_move_raw();

      // Move the nozzle down to push the Z probe into retracted position
      if (Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE != Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE)
        feedrate = Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE;
      if (Z_PROBE_ALLEN_KEY_STOW_2_X != Z_PROBE_ALLEN_KEY_STOW_1_X)
        destination[X_AXIS] = Z_PROBE_ALLEN_KEY_STOW_2_X;
      if (Z_PROBE_ALLEN_KEY_STOW_2_Y != Z_PROBE_ALLEN_KEY_STOW_1_Y)
        destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_STOW_2_Y;
      destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_STOW_2_Z;
      prepare_move_raw();

      // Move up for safety
      if (Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE != Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE)
        feedrate = Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE;
      if (Z_PROBE_ALLEN_KEY_STOW_3_X != Z_PROBE_ALLEN_KEY_STOW_2_X)
        destination[X_AXIS] = Z_PROBE_ALLEN_KEY_STOW_3_X;
      if (Z_PROBE_ALLEN_KEY_STOW_3_Y != Z_PROBE_ALLEN_KEY_STOW_2_Y)
        destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_STOW_3_Y;
      destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_STOW_3_Z;
      prepare_move_raw();

      // Home XY for safety
      feedrate = homing_feedrate[X_AXIS] / 2;
      destination[X_AXIS] = 0;
      destination[Y_AXIS] = 0;
      prepare_move_raw(); // this will also set_current_to_destination

      st_synchronize();

      #if ENABLED(Z_MIN_PROBE_ENDSTOP)
        bool z_probe_endstop = (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING);
        if (!z_probe_endstop)
      #else
        bool z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
        if (!z_min_endstop)
      #endif
        {
          if (IsRunning()) {
            SERIAL_ERROR_START;
            SERIAL_ERRORLNPGM("Z-Probe failed to retract!");
            LCD_ALERTMESSAGEPGM("Err: ZPROBE");
          }
          Stop();
        }
    #endif // Z_PROBE_ALLEN_KEY

    #if ENABLED(FIX_MOUNTED_PROBE)
      // Noting to be done. Just set z_probe_is_active
    #endif

    z_probe_is_active = false;
  }
  #endif // HAS_Z_MIN_PROBE

  enum ProbeAction {
    ProbeStay          = 0,
    ProbeDeploy        = _BV(0),
    ProbeStow          = _BV(1),
    ProbeDeployAndStow = (ProbeDeploy | ProbeStow)
  };

  // Probe bed height at position (x,y), returns the measured z value
  static float probe_pt(float x, float y, float z_before, ProbeAction probe_action = ProbeDeployAndStow, int verbose_level = 1) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("probe_pt >>>");
        SERIAL_ECHOPAIR("> ProbeAction:", (unsigned long)probe_action);
        SERIAL_EOL;
        print_xyz("> current_position", current_position);
      }
    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("Z Raise to z_before ", z_before);
        SERIAL_EOL;
        SERIAL_ECHOPAIR("> do_blocking_move_to_z ", z_before);
        SERIAL_EOL;
      }
    #endif

    // Move Z up to the z_before height, then move the Z probe to the given XY
    do_blocking_move_to_z(z_before); // this also updates current_position

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("> do_blocking_move_to_xy ", x - (X_PROBE_OFFSET_FROM_EXTRUDER));
        SERIAL_ECHOPAIR(", ", y - (Y_PROBE_OFFSET_FROM_EXTRUDER));
        SERIAL_EOL;
      }
    #endif

    // this also updates current_position
    do_blocking_move_to_xy(x - (X_PROBE_OFFSET_FROM_EXTRUDER), y - (Y_PROBE_OFFSET_FROM_EXTRUDER));

    #if DISABLED(Z_PROBE_SLED) && DISABLED(Z_PROBE_ALLEN_KEY)
      if (probe_action & ProbeDeploy) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> ProbeDeploy");
          }
        #endif
        deploy_z_probe();
      }
    #endif

    run_z_probe();
    float measured_z = current_position[Z_AXIS];

    #if DISABLED(Z_PROBE_SLED) && DISABLED(Z_PROBE_ALLEN_KEY)
      if (probe_action & ProbeStow) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> ProbeStow (stow_z_probe will do Z Raise)");
          }
        #endif
        stow_z_probe();
      }
    #endif

    if (verbose_level > 2) {
      SERIAL_PROTOCOLPGM("Bed X: ");
      SERIAL_PROTOCOL_F(x, 3);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL_F(y, 3);
      SERIAL_PROTOCOLPGM(" Z: ");
      SERIAL_PROTOCOL_F(measured_z, 3);
      SERIAL_EOL;
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("<<< probe_pt");
      }
    #endif

    return measured_z;
  }

  #if ENABLED(DELTA)

    /**
     * All DELTA leveling in the Marlin uses NONLINEAR_BED_LEVELING
     */
    static void extrapolate_one_point(int x, int y, int xdir, int ydir) {
      if (bed_level[x][y] != 0.0) {
        return;  // Don't overwrite good values.
      }
      float a = 2 * bed_level[x + xdir][y] - bed_level[x + xdir * 2][y]; // Left to right.
      float b = 2 * bed_level[x][y + ydir] - bed_level[x][y + ydir * 2]; // Front to back.
      float c = 2 * bed_level[x + xdir][y + ydir] - bed_level[x + xdir * 2][y + ydir * 2]; // Diagonal.
      float median = c;  // Median is robust (ignores outliers).
      if (a < b) {
        if (b < c) median = b;
        if (c < a) median = a;
      }
      else {  // b <= a
        if (c < b) median = b;
        if (a < c) median = a;
      }
      bed_level[x][y] = median;
    }

    /**
     * Fill in the unprobed points (corners of circular print surface)
     * using linear extrapolation, away from the center.
     */
    static void extrapolate_unprobed_bed_level() {
      int half = (AUTO_BED_LEVELING_GRID_POINTS - 1) / 2;
      for (int y = 0; y <= half; y++) {
        for (int x = 0; x <= half; x++) {
          if (x + y < 3) continue;
          extrapolate_one_point(half - x, half - y, x > 1 ? +1 : 0, y > 1 ? +1 : 0);
          extrapolate_one_point(half + x, half - y, x > 1 ? -1 : 0, y > 1 ? +1 : 0);
          extrapolate_one_point(half - x, half + y, x > 1 ? +1 : 0, y > 1 ? -1 : 0);
          extrapolate_one_point(half + x, half + y, x > 1 ? -1 : 0, y > 1 ? -1 : 0);
        }
      }
    }

    /**
     * Print calibration results for plotting or manual frame adjustment.
     */
    static void print_bed_level() {
      for (int y = 0; y < AUTO_BED_LEVELING_GRID_POINTS; y++) {
        for (int x = 0; x < AUTO_BED_LEVELING_GRID_POINTS; x++) {
          SERIAL_PROTOCOL_F(bed_level[x][y], 2);
          SERIAL_PROTOCOLCHAR(' ');
        }
        SERIAL_EOL;
      }
    }

    /**
     * Reset calibration results to zero.
     */
    void reset_bed_level() {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOLNPGM("reset_bed_level");
        }
      #endif
      for (int y = 0; y < AUTO_BED_LEVELING_GRID_POINTS; y++) {
        for (int x = 0; x < AUTO_BED_LEVELING_GRID_POINTS; x++) {
          bed_level[x][y] = 0.0;
        }
      }
    }

  #endif // DELTA

  #if HAS_SERVO_ENDSTOPS && DISABLED(Z_PROBE_SLED)

    void raise_z_for_servo() {
      float zpos = current_position[Z_AXIS], z_dest = Z_RAISE_BEFORE_PROBING;
      /**
       * The zprobe_zoffset is negative any switch below the nozzle, so
       * multiply by Z_HOME_DIR (-1) to move enough away from bed for the probe
       */
      z_dest += axis_homed[Z_AXIS] ? zprobe_zoffset * Z_HOME_DIR : zpos;
      if (zpos < z_dest) do_blocking_move_to_z(z_dest); // also updates current_position
    }

  #endif

#endif // AUTO_BED_LEVELING_FEATURE

static void axis_unhomed_error() {
  LCD_MESSAGEPGM(MSG_YX_UNHOMED);
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM(MSG_YX_UNHOMED);
}

#if ENABLED(Z_PROBE_SLED)

  #ifndef SLED_DOCKING_OFFSET
    #define SLED_DOCKING_OFFSET 0
  #endif

  /**
   * Method to dock/undock a sled designed by Charles Bell.
   *
   * dock[in]     If true, move to MAX_X and engage the electromagnet
   * offset[in]   The additional distance to move to adjust docking location
   */
  static void dock_sled(bool dock, int offset = 0) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("dock_sled", dock);
        SERIAL_EOL;
      }
    #endif

    if (z_probe_is_active == dock) return;

    if (!axis_homed[X_AXIS] || !axis_homed[Y_AXIS]) {
      axis_unhomed_error();
      return;
    }

    float oldXpos = current_position[X_AXIS]; // save x position
    if (dock) {
      #if Z_RAISE_AFTER_PROBING > 0
        raise_z_after_probing(); // raise Z
      #endif
      // Dock sled a bit closer to ensure proper capturing
      do_blocking_move_to_x(X_MAX_POS + SLED_DOCKING_OFFSET + offset - 1);
      digitalWrite(SLED_PIN, LOW); // turn off magnet
    }
    else {
      float z_loc = current_position[Z_AXIS];
      if (z_loc < Z_RAISE_BEFORE_PROBING + 5) z_loc = Z_RAISE_BEFORE_PROBING;
      do_blocking_move_to(X_MAX_POS + SLED_DOCKING_OFFSET + offset, current_position[Y_AXIS], z_loc); // this also updates current_position
      digitalWrite(SLED_PIN, HIGH); // turn on magnet
    }
    do_blocking_move_to_x(oldXpos); // return to position before docking

    z_probe_is_active = dock;
  }

#endif // Z_PROBE_SLED



/**
 * Home an individual axis
 */

#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

static void homeaxis(AxisEnum axis) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> homeaxis(", (unsigned long)axis);
      SERIAL_CHAR(')');
      SERIAL_EOL;
    }
  #endif
  #define HOMEAXIS_DO(LETTER) \
    ((LETTER##_MIN_PIN > -1 && LETTER##_HOME_DIR==-1) || (LETTER##_MAX_PIN > -1 && LETTER##_HOME_DIR==1))

  if (axis == X_AXIS ? HOMEAXIS_DO(X) : axis == Y_AXIS ? HOMEAXIS_DO(Y) : axis == Z_AXIS ? HOMEAXIS_DO(Z) : 0) {

    int axis_home_dir =
      #if ENABLED(DUAL_X_CARRIAGE)
        (axis == X_AXIS) ? x_home_dir(active_extruder) :
      #endif
      home_dir(axis);

    // Set the axis position as setup for the move
    current_position[axis] = 0;
    sync_plan_position();

    #if ENABLED(Z_PROBE_SLED)
      #define _Z_SERVO_TEST       (axis != Z_AXIS)      // deploy Z, servo.move XY
      #define _Z_PROBE_SUBTEST    false                 // Z will never be invoked
      #define _Z_DEPLOY           (dock_sled(false))
      #define _Z_STOW             (dock_sled(true))
    #elif SERVO_LEVELING || ENABLED(FIX_MOUNTED_PROBE)
      #define _Z_SERVO_TEST       (axis != Z_AXIS)      // servo.move XY
      #define _Z_PROBE_SUBTEST    false                 // Z will never be invoked
      #define _Z_DEPLOY           (deploy_z_probe())
      #define _Z_STOW             (stow_z_probe())
    #elif HAS_SERVO_ENDSTOPS
      #define _Z_SERVO_TEST       true                  // servo.move X, Y, Z
      #define _Z_PROBE_SUBTEST    (axis == Z_AXIS)      // Z is a probe
    #endif

    if (axis == Z_AXIS) {
      // If there's a Z probe that needs deployment...
      #if ENABLED(Z_PROBE_SLED) || SERVO_LEVELING || ENABLED(FIX_MOUNTED_PROBE)
        // ...and homing Z towards the bed? Deploy it.
        if (axis_home_dir < 0) _Z_DEPLOY;
      #endif
    }

    #if HAS_SERVO_ENDSTOPS
      // Engage an X or Y Servo endstop if enabled
      if (_Z_SERVO_TEST && servo_endstop_id[axis] >= 0) {
        servo[servo_endstop_id[axis]].move(servo_endstop_angle[axis][0]);
        if (_Z_PROBE_SUBTEST) z_probe_is_active = true;
      }
    #endif

    // Set a flag for Z motor locking
    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (axis == Z_AXIS) In_Homing_Process(true);
    #endif

    // Move towards the endstop until an endstop is triggered
    destination[axis] = 1.5 * max_length(axis) * axis_home_dir;
    feedrate = homing_feedrate[axis];
    line_to_destination();
    st_synchronize();

    // Set the axis position as setup for the move
    current_position[axis] = 0;
    sync_plan_position();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("> enable_endstops(false)");
      }
    #endif
    enable_endstops(false); // Disable endstops while moving away

    // Move away from the endstop by the axis HOME_BUMP_MM
    destination[axis] = -home_bump_mm(axis) * axis_home_dir;
    line_to_destination();
    st_synchronize();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("> enable_endstops(true)");
      }
    #endif
    enable_endstops(true); // Enable endstops for next homing move

    // Slow down the feedrate for the next move
    set_homing_bump_feedrate(axis);

    // Move slowly towards the endstop until triggered
    destination[axis] = 2 * home_bump_mm(axis) * axis_home_dir;
    line_to_destination();
    st_synchronize();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("> TRIGGER ENDSTOP > current_position", current_position);
      }
    #endif

    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (axis == Z_AXIS) {
        float adj = fabs(z_endstop_adj);
        bool lockZ1;
        if (axis_home_dir > 0) {
          adj = -adj;
          lockZ1 = (z_endstop_adj > 0);
        }
        else
          lockZ1 = (z_endstop_adj < 0);

        if (lockZ1) Lock_z_motor(true); else Lock_z2_motor(true);
        sync_plan_position();

        // Move to the adjusted endstop height
        feedrate = homing_feedrate[axis];
        destination[Z_AXIS] = adj;
        line_to_destination();
        st_synchronize();

        if (lockZ1) Lock_z_motor(false); else Lock_z2_motor(false);
        In_Homing_Process(false);
      } // Z_AXIS
    #endif

    #if ENABLED(DELTA)
      // retrace by the amount specified in endstop_adj
      if (endstop_adj[axis] * axis_home_dir < 0) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> enable_endstops(false)");
          }
        #endif
        enable_endstops(false); // Disable endstops while moving away
        sync_plan_position();
        destination[axis] = endstop_adj[axis];
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("> endstop_adj = ", endstop_adj[axis]);
            print_xyz(" > destination", destination);
          }
        #endif
        line_to_destination();
        st_synchronize();
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> enable_endstops(true)");
          }
        #endif
        enable_endstops(true); // Enable endstops for next homing move
      }
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        else {
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("> endstop_adj * axis_home_dir = ", endstop_adj[axis] * axis_home_dir);
            SERIAL_EOL;
          }
        }
      #endif
    #endif

    // Set the axis position to its home position (plus home offsets)
    set_axis_is_at_home(axis);
    sync_plan_position();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("> AFTER set_axis_is_at_home > current_position", current_position);
      }
    #endif

    destination[axis] = current_position[axis];
    feedrate = 0.0;
    endstops_hit_on_purpose(); // clear endstop hit flags
    axis_known_position[axis] = true;
    axis_homed[axis] = true;

    // Put away the Z probe
    #if ENABLED(Z_PROBE_SLED) || SERVO_LEVELING || ENABLED(FIX_MOUNTED_PROBE)
      if (axis == Z_AXIS && axis_home_dir < 0) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> SERVO_LEVELING > " STRINGIFY(_Z_STOW));
          }
        #endif
        _Z_STOW;
      }
    #endif

    // Retract Servo endstop if enabled
    #if HAS_SERVO_ENDSTOPS
      if (_Z_SERVO_TEST && servo_endstop_id[axis] >= 0) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("> SERVO_ENDSTOPS > Stow with servo.move()");
          }
        #endif
        servo[servo_endstop_id[axis]].move(servo_endstop_angle[axis][1]);
        if (_Z_PROBE_SUBTEST) z_probe_is_active = false;
      }
    #endif

  }

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< homeaxis(", (unsigned long)axis);
      SERIAL_CHAR(')');
      SERIAL_EOL;
    }
  #endif
}

#if ENABLED(FWRETRACT)

  void retract(bool retracting, bool swapping = false) {

    if (retracting == retracted[active_extruder]) return;

    float oldFeedrate = feedrate;

    set_destination_to_current();

    if (retracting) {

      feedrate = retract_feedrate * 60;
      current_position[E_AXIS] += (swapping ? retract_length_swap : retract_length) / volumetric_multiplier[active_extruder];
      plan_set_e_position(current_position[E_AXIS]);
      prepare_move();

      if (retract_zlift > 0.01) {
        current_position[Z_AXIS] -= retract_zlift;
        #if ENABLED(DELTA)
          sync_plan_position_delta();
        #else
          sync_plan_position();
        #endif
        prepare_move();
      }
    }
    else {

      if (retract_zlift > 0.01) {
        current_position[Z_AXIS] += retract_zlift;
        #if ENABLED(DELTA)
          sync_plan_position_delta();
        #else
          sync_plan_position();
        #endif
        //prepare_move();
      }

      feedrate = retract_recover_feedrate * 60;
      float move_e = swapping ? retract_length_swap + retract_recover_length_swap : retract_length + retract_recover_length;
      current_position[E_AXIS] -= move_e / volumetric_multiplier[active_extruder];
      plan_set_e_position(current_position[E_AXIS]);
      prepare_move();
    }

    feedrate = oldFeedrate;
    retracted[active_extruder] = retracting;

  } // retract()

#endif // FWRETRACT

/**
 * ***************************************************************************
 * ***************************** G-CODE HANDLING *****************************
 * ***************************************************************************
 */

/**
 * Set XYZE destination and feedrate from the current GCode command
 *
 *  - Set destination from included axis codes
 *  - Set to current for missing axis codes
 *  - Set the feedrate, if included
 */
void gcode_get_destination() {
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i]))
      destination[i] = code_value() + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
    else
      destination[i] = current_position[i];
  }
  if (code_seen('F')) {
    float next_feedrate = code_value();
    if (next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

void unknown_command_error() {
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
  SERIAL_ECHO(current_command);
  SERIAL_ECHOPGM("\"\n");
}

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  /**
   * Output a "busy" message at regular intervals
   * while the machine is not accepting commands.
   */
  void host_keepalive() {
    millis_t ms = millis();
    if (busy_state != NOT_BUSY) {
      if (ms < next_busy_signal_ms) return;
      switch (busy_state) {
        case IN_HANDLER:
        case IN_PROCESS:
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_BUSY_PROCESSING);
          break;
        case PAUSED_FOR_USER:
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER);
          break;
        case PAUSED_FOR_INPUT:
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_INPUT);
          break;
      }
    }
    next_busy_signal_ms = ms + 10000UL; // "busy: ..." message every 10s
  }

#endif //HOST_KEEPALIVE_FEATURE

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
inline void gcode_G0_G1() {
  if (IsRunning()) {
    gcode_get_destination(); // For X Y Z E F

    #if ENABLED(FWRETRACT)

      if (autoretract_enabled && !(code_seen('X') || code_seen('Y') || code_seen('Z')) && code_seen('E')) {
        float echange = destination[E_AXIS] - current_position[E_AXIS];
        // Is this move an attempt to retract or recover?
        if ((echange < -MIN_RETRACT && !retracted[active_extruder]) || (echange > MIN_RETRACT && retracted[active_extruder])) {
          current_position[E_AXIS] = destination[E_AXIS]; // hide the slicer-generated retract/recover from calculations
          plan_set_e_position(current_position[E_AXIS]);  // AND from the planner
          retract(!retracted[active_extruder]);
          return;
        }
      }

    #endif //FWRETRACT

    prepare_move();
  }
}

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 */
inline void gcode_G2_G3(bool clockwise) {
  if (IsRunning()) {

    #if ENABLED(SF_ARC_FIX)
      bool relative_mode_backup = relative_mode;
      relative_mode = true;
    #endif

    gcode_get_destination();

    #if ENABLED(SF_ARC_FIX)
      relative_mode = relative_mode_backup;
    #endif

    // Center of arc as offset from current_position
    float arc_offset[2] = {
      code_seen('I') ? code_value() : 0,
      code_seen('J') ? code_value() : 0
    };

    // Send an arc to the planner
    plan_arc(destination, arc_offset, clockwise);

    refresh_cmd_timeout();
  }
}

/**
 * G4: Dwell S<seconds> or P<milliseconds>
 */
inline void gcode_G4() {
  millis_t codenum = 0;

  if (code_seen('P')) codenum = code_value_long(); // milliseconds to wait
  if (code_seen('S')) codenum = code_value() * 1000; // seconds to wait

  st_synchronize();
  refresh_cmd_timeout();
  codenum += previous_cmd_ms;  // keep track of when we started waiting

  if (!lcd_hasstatus()) LCD_MESSAGEPGM(MSG_DWELL);

  while (millis() < codenum) idle();
}

#if ENABLED(FWRETRACT)

  /**
   * G10 - Retract filament according to settings of M207
   * G11 - Recover filament according to settings of M208
   */
  inline void gcode_G10_G11(bool doRetract=false) {
    #if EXTRUDERS > 1
      if (doRetract) {
        retracted_swap[active_extruder] = (code_seen('S') && code_value_short() == 1); // checks for swap retract argument
      }
    #endif
    retract(doRetract
     #if EXTRUDERS > 1
      , retracted_swap[active_extruder]
     #endif
    );
  }

#endif //FWRETRACT

/**
 * G28: Home all axes according to settings
 *
 * Parameters
 *
 *  None  Home to all axes with no parameters.
 *        With QUICK_HOME enabled XY will home together, then Z.
 *
 * Cartesian parameters
 *
 *  X   Home to the X endstop
 *  Y   Home to the Y endstop
 *  Z   Home to the Z endstop
 *
 */
inline void gcode_G28() {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOLNPGM("gcode_G28 >>>");
    }
  #endif

  // Wait for planner moves to finish!
  st_synchronize();

  // For auto bed leveling, clear the level matrix
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    plan_bed_level_matrix.set_to_identity();
    #if ENABLED(DELTA)
      reset_bed_level();
    #endif
  #endif

  /**
   * For mesh bed leveling deactivate the mesh calculations, will be turned
   * on again when homing all axis
   */
  #if ENABLED(MESH_BED_LEVELING)
    uint8_t mbl_was_active = mbl.active;
    mbl.active = 0;
  #endif

  setup_for_endstop_move();

  /**
   * Directly after a reset this is all 0. Later we get a hint if we have
   * to raise z or not.
   */
  set_destination_to_current();

  feedrate = 0.0;

  #if ENABLED(DELTA)
    /**
     * A delta can only safely home all axis at the same time
     * all axis have to home at the same time
     */

    // Pretend the current position is 0,0,0
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = 0;
    sync_plan_position();

    // Move all carriages up together until the first endstop is hit.
    for (int i = X_AXIS; i <= Z_AXIS; i++) destination[i] = 3 * (Z_MAX_LENGTH);
    feedrate = 1.732 * homing_feedrate[X_AXIS];
    line_to_destination();
    st_synchronize();
    endstops_hit_on_purpose(); // clear endstop hit flags

    // Destination reached
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = destination[i];

    // take care of back off and rehome now we are all at the top
    HOMEAXIS(X);
    HOMEAXIS(Y);
    HOMEAXIS(Z);

    sync_plan_position_delta();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        print_xyz("(DELTA) > current_position", current_position);
      }
    #endif

  #else // NOT DELTA

    bool  homeX = code_seen(axis_codes[X_AXIS]),
          homeY = code_seen(axis_codes[Y_AXIS]),
          homeZ = code_seen(axis_codes[Z_AXIS]);

    home_all_axis = (!homeX && !homeY && !homeZ) || (homeX && homeY && homeZ);

    #if Z_HOME_DIR > 0  // If homing away from BED do Z first

      if (home_all_axis || homeZ) {
        HOMEAXIS(Z);
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> HOMEAXIS(Z) > current_position", current_position);
          }
        #endif
      }

    #elif defined(MIN_Z_HEIGHT_FOR_HOMING) && MIN_Z_HEIGHT_FOR_HOMING > 0

      // Raise Z before homing any other axes and z is not already high enough (never lower z)
      if (current_position[Z_AXIS] <= MIN_Z_HEIGHT_FOR_HOMING) {
        destination[Z_AXIS] = MIN_Z_HEIGHT_FOR_HOMING;
        feedrate = max_feedrate[Z_AXIS] * 60;  // feedrate (mm/m) = max_feedrate (mm/s)
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("Raise Z (before homing) to ", (float)(MIN_Z_HEIGHT_FOR_HOMING));
            SERIAL_EOL;
            print_xyz("> (home_all_axis || homeZ) > current_position", current_position);
            print_xyz("> (home_all_axis || homeZ) > destination", destination);
          }
        #endif
        line_to_destination();
        st_synchronize();

        /**
         * Update the current Z position even if it currently not real from
         * Z-home otherwise each call to line_to_destination() will want to
         * move Z-axis by MIN_Z_HEIGHT_FOR_HOMING.
         */
        current_position[Z_AXIS] = destination[Z_AXIS];
      }
    #endif

    #if ENABLED(QUICK_HOME)

      if (home_all_axis || (homeX && homeY)) {  // First diagonal move

        current_position[X_AXIS] = current_position[Y_AXIS] = 0;

        #if ENABLED(DUAL_X_CARRIAGE)
          int x_axis_home_dir = x_home_dir(active_extruder);
          extruder_duplication_enabled = false;
        #else
          int x_axis_home_dir = home_dir(X_AXIS);
        #endif

        sync_plan_position();

        float mlx = max_length(X_AXIS), mly = max_length(Y_AXIS),
              mlratio = mlx > mly ? mly / mlx : mlx / mly;

        destination[X_AXIS] = 1.5 * mlx * x_axis_home_dir;
        destination[Y_AXIS] = 1.5 * mly * home_dir(Y_AXIS);
        feedrate = min(homing_feedrate[X_AXIS], homing_feedrate[Y_AXIS]) * sqrt(mlratio * mlratio + 1);
        line_to_destination();
        st_synchronize();

        set_axis_is_at_home(X_AXIS);
        set_axis_is_at_home(Y_AXIS);
        sync_plan_position();

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> QUICK_HOME > current_position 1", current_position);
          }
        #endif

        destination[X_AXIS] = current_position[X_AXIS];
        destination[Y_AXIS] = current_position[Y_AXIS];
        line_to_destination();
        feedrate = 0.0;
        st_synchronize();
        endstops_hit_on_purpose(); // clear endstop hit flags

        current_position[X_AXIS] = destination[X_AXIS];
        current_position[Y_AXIS] = destination[Y_AXIS];
        #if DISABLED(SCARA)
          current_position[Z_AXIS] = destination[Z_AXIS];
        #endif

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> QUICK_HOME > current_position 2", current_position);
          }
        #endif
      }

    #endif // QUICK_HOME

    #if ENABLED(HOME_Y_BEFORE_X)
      // Home Y
      if (home_all_axis || homeY) HOMEAXIS(Y);
    #endif

    // Home X
    if (home_all_axis || homeX) {
      #if ENABLED(DUAL_X_CARRIAGE)
        int tmp_extruder = active_extruder;
        extruder_duplication_enabled = false;
        active_extruder = !active_extruder;
        HOMEAXIS(X);
        inactive_extruder_x_pos = current_position[X_AXIS];
        active_extruder = tmp_extruder;
        HOMEAXIS(X);
        // reset state used by the different modes
        memcpy(raised_parked_position, current_position, sizeof(raised_parked_position));
        delayed_move_time = 0;
        active_extruder_parked = true;
      #else
        HOMEAXIS(X);
      #endif
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("> homeX", current_position);
        }
      #endif
    }

    #if DISABLED(HOME_Y_BEFORE_X)
      // Home Y
      if (home_all_axis || homeY) {
        HOMEAXIS(Y);
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> homeY", current_position);
          }
        #endif
      }
    #endif

    // Home Z last if homing towards the bed
    #if Z_HOME_DIR < 0

      if (home_all_axis || homeZ) {

        #if ENABLED(Z_SAFE_HOMING)

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOLNPGM("> Z_SAFE_HOMING >>>");
            }
          #endif

          if (home_all_axis) {

            /**
             * At this point we already have Z at MIN_Z_HEIGHT_FOR_HOMING height
             * No need to move Z any more as this height should already be safe
             * enough to reach Z_SAFE_HOMING XY positions.
             * Just make sure the planner is in sync.
             */
            sync_plan_position();

            /**
             * Set the Z probe (or just the nozzle) destination to the safe
             *  homing point
             */
            destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT - (X_PROBE_OFFSET_FROM_EXTRUDER));
            destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT - (Y_PROBE_OFFSET_FROM_EXTRUDER));
            destination[Z_AXIS] = current_position[Z_AXIS]; //z is already at the right height
            feedrate = XY_TRAVEL_SPEED;

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                print_xyz("> Z_SAFE_HOMING > home_all_axis > current_position", current_position);
                print_xyz("> Z_SAFE_HOMING > home_all_axis > destination", destination);
              }
            #endif

            // Move in the XY plane
            line_to_destination();
            st_synchronize();

            /**
             * Update the current positions for XY, Z is still at least at
             * MIN_Z_HEIGHT_FOR_HOMING height, no changes there.
             */
            current_position[X_AXIS] = destination[X_AXIS];
            current_position[Y_AXIS] = destination[Y_AXIS];

            // Home the Z axis
            HOMEAXIS(Z);
          }

          else if (homeZ) { // Don't need to Home Z twice

            // Let's see if X and Y are homed
            if (axis_homed[X_AXIS] && axis_homed[Y_AXIS]) {

              /**
               * Make sure the Z probe is within the physical limits
               * NOTE: This doesn't necessarily ensure the Z probe is also
               * within the bed!
               */
              float cpx = current_position[X_AXIS], cpy = current_position[Y_AXIS];
              if (   cpx >= X_MIN_POS - (X_PROBE_OFFSET_FROM_EXTRUDER)
                  && cpx <= X_MAX_POS - (X_PROBE_OFFSET_FROM_EXTRUDER)
                  && cpy >= Y_MIN_POS - (Y_PROBE_OFFSET_FROM_EXTRUDER)
                  && cpy <= Y_MAX_POS - (Y_PROBE_OFFSET_FROM_EXTRUDER)) {

                // Home the Z axis
                HOMEAXIS(Z);
              }
              else {
                LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
                SERIAL_ECHO_START;
                SERIAL_ECHOLNPGM(MSG_ZPROBE_OUT);
              }
            }
            else {
              axis_unhomed_error();
            }

          } // !home_all_axes && homeZ

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOLNPGM("<<< Z_SAFE_HOMING");
            }
          #endif

        #else // !Z_SAFE_HOMING

          HOMEAXIS(Z);

        #endif // !Z_SAFE_HOMING

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> (home_all_axis || homeZ) > final", current_position);
          }
        #endif

      } // home_all_axis || homeZ

    #endif // Z_HOME_DIR < 0

    sync_plan_position();

  #endif // else DELTA

  #if ENABLED(SCARA)
    sync_plan_position_delta();
  #endif

  #if ENABLED(ENDSTOPS_ONLY_FOR_HOMING)
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("ENDSTOPS_ONLY_FOR_HOMING enable_endstops(false)");
      }
    #endif
    enable_endstops(false);
  #endif

  // For mesh leveling move back to Z=0
  #if ENABLED(MESH_BED_LEVELING)
    if (mbl_was_active && home_all_axis) {
      current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
      sync_plan_position();
      mbl.active = 1;
      current_position[Z_AXIS] = 0.0;
      set_destination_to_current();
      feedrate = homing_feedrate[Z_AXIS];
      line_to_destination();
      st_synchronize();
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("mbl_was_active > current_position", current_position);
        }
      #endif
    }
  #endif

  feedrate = saved_feedrate;
  feedrate_multiplier = saved_feedrate_multiplier;
  refresh_cmd_timeout();
  endstops_hit_on_purpose(); // clear endstop hit flags

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOLNPGM("<<< gcode_G28");
    }
  #endif

  gcode_M114(); // Send end position to RepetierHost

}

#if ENABLED(MESH_BED_LEVELING)

  enum MeshLevelingState { MeshReport, MeshStart, MeshNext, MeshSet, MeshSetZOffset };

  /**
   * G29: Mesh-based Z probe, probes a grid and produces a
   *      mesh to compensate for variable bed height
   *
   * Parameters With MESH_BED_LEVELING:
   *
   *  S0              Produce a mesh report
   *  S1              Start probing mesh points
   *  S2              Probe the next mesh point
   *  S3 Xn Yn Zn.nn  Manually modify a single point
   *  S4 Zn.nn        Set z offset. Positive away from bed, negative closer to bed.
   *
   * The S0 report the points as below
   *
   *  +----> X-axis  1-n
   *  |
   *  |
   *  v Y-axis  1-n
   *
   */
  inline void gcode_G29() {

    static int probe_point = -1;
    MeshLevelingState state = code_seen('S') ? (MeshLevelingState)code_value_short() : MeshReport;
    if (state < 0 || state > 4) {
      SERIAL_PROTOCOLLNPGM("S out of range (0-4).");
      return;
    }

    int ix, iy;
    float z;

    switch (state) {
      case MeshReport:
        if (mbl.active) {
          SERIAL_PROTOCOLPGM("Num X,Y: ");
          SERIAL_PROTOCOL(MESH_NUM_X_POINTS);
          SERIAL_PROTOCOLCHAR(',');
          SERIAL_PROTOCOL(MESH_NUM_Y_POINTS);
          SERIAL_PROTOCOLPGM("\nZ search height: ");
          SERIAL_PROTOCOL(MESH_HOME_SEARCH_Z);
          SERIAL_PROTOCOLPGM("\nZ offset: ");
          SERIAL_PROTOCOL_F(mbl.z_offset, 5);
          SERIAL_PROTOCOLLNPGM("\nMeasured points:");
          for (int y = 0; y < MESH_NUM_Y_POINTS; y++) {
            for (int x = 0; x < MESH_NUM_X_POINTS; x++) {
              SERIAL_PROTOCOLPGM("  ");
              SERIAL_PROTOCOL_F(mbl.z_values[y][x], 5);
            }
            SERIAL_EOL;
          }
        }
        else
          SERIAL_PROTOCOLLNPGM("Mesh bed leveling not active.");
        break;

      case MeshStart:
        mbl.reset();
        probe_point = 0;
        enqueue_and_echo_commands_P(PSTR("G28\nG29 S2"));
        break;

      case MeshNext:
        if (probe_point < 0) {
          SERIAL_PROTOCOLLNPGM("Start mesh probing with \"G29 S1\" first.");
          return;
        }
        if (probe_point == 0) {
          // Set Z to a positive value before recording the first Z.
          current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
          sync_plan_position();
        }
        else {
          // For others, save the Z of the previous point, then raise Z again.
          ix = (probe_point - 1) % (MESH_NUM_X_POINTS);
          iy = (probe_point - 1) / (MESH_NUM_X_POINTS);
          if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
          mbl.set_z(ix, iy, current_position[Z_AXIS]);
          current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
          plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS] / 60, active_extruder);
          st_synchronize();
        }
        // Is there another point to sample? Move there.
        if (probe_point < (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS)) {
          ix = probe_point % (MESH_NUM_X_POINTS);
          iy = probe_point / (MESH_NUM_X_POINTS);
          if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
          current_position[X_AXIS] = mbl.get_x(ix);
          current_position[Y_AXIS] = mbl.get_y(iy);
          plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS] / 60, active_extruder);
          st_synchronize();
          probe_point++;
        }
        else {
          // After recording the last point, activate the mbl and home
          SERIAL_PROTOCOLLNPGM("Mesh probing done.");
          probe_point = -1;
          mbl.active = 1;
          enqueue_and_echo_commands_P(PSTR("G28"));
        }
        break;

      case MeshSet:
        if (code_seen('X')) {
          ix = code_value_long() - 1;
          if (ix < 0 || ix >= MESH_NUM_X_POINTS) {
            SERIAL_PROTOCOLPGM("X out of range (1-" STRINGIFY(MESH_NUM_X_POINTS) ").\n");
            return;
          }
        }
        else {
          SERIAL_PROTOCOLPGM("X not entered.\n");
          return;
        }
        if (code_seen('Y')) {
          iy = code_value_long() - 1;
          if (iy < 0 || iy >= MESH_NUM_Y_POINTS) {
            SERIAL_PROTOCOLPGM("Y out of range (1-" STRINGIFY(MESH_NUM_Y_POINTS) ").\n");
            return;
          }
        }
        else {
          SERIAL_PROTOCOLPGM("Y not entered.\n");
          return;
        }
        if (code_seen('Z')) {
          z = code_value();
        }
        else {
          SERIAL_PROTOCOLPGM("Z not entered.\n");
          return;
        }
        mbl.z_values[iy][ix] = z;
        break;

      case MeshSetZOffset:
        if (code_seen('Z')) {
          z = code_value();
        }
        else {
          SERIAL_PROTOCOLPGM("Z not entered.\n");
          return;
        }
        mbl.z_offset = z;

    } // switch(state)
  }

#elif ENABLED(AUTO_BED_LEVELING_FEATURE)

  void out_of_range_error(const char* p_edge) {
    SERIAL_PROTOCOLPGM("?Probe ");
    serialprintPGM(p_edge);
    SERIAL_PROTOCOLLNPGM(" position out of range.");
  }

  /**
   * G29: Detailed Z probe, probes the bed at 3 or more points.
   *      Will fail if the printer has not been homed with G28.
   *
   * Enhanced G29 Auto Bed Leveling Probe Routine
   *
   * Parameters With AUTO_BED_LEVELING_GRID:
   *
   *  P  Set the size of the grid that will be probed (P x P points).
   *     Not supported by non-linear delta printer bed leveling.
   *     Example: "G29 P4"
   *
   *  S  Set the XY travel speed between probe points (in mm/min)
   *
   *  D  Dry-Run mode. Just evaluate the bed Topology - Don't apply
   *     or clean the rotation Matrix. Useful to check the topology
   *     after a first run of G29.
   *
   *  V  Set the verbose level (0-4). Example: "G29 V3"
   *
   *  T  Generate a Bed Topology Report. Example: "G29 P5 T" for a detailed report.
   *     This is useful for manual bed leveling and finding flaws in the bed (to
   *     assist with part placement).
   *     Not supported by non-linear delta printer bed leveling.
   *
   *  F  Set the Front limit of the probing grid
   *  B  Set the Back limit of the probing grid
   *  L  Set the Left limit of the probing grid
   *  R  Set the Right limit of the probing grid
   *
   * Global Parameters:
   *
   * E/e By default G29 will engage the Z probe, test the bed, then disengage.
   *     Include "E" to engage/disengage the Z probe for each sample.
   *     There's no extra effect if you have a fixed Z probe.
   *     Usage: "G29 E" or "G29 e"
   *
   */
  inline void gcode_G29() {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("gcode_G29 >>>");
      }
    #endif

    // Don't allow auto-leveling without homing first
    if (!axis_homed[X_AXIS] || !axis_homed[Y_AXIS]) {
      axis_unhomed_error();
      return;
    }

    int verbose_level = code_seen('V') ? code_value_short() : 1;
    if (verbose_level < 0 || verbose_level > 4) {
      SERIAL_ECHOLNPGM("?(V)erbose Level is implausible (0-4).");
      return;
    }

    bool dryrun = code_seen('D'),
         deploy_probe_for_each_reading = code_seen('E');

    #if ENABLED(AUTO_BED_LEVELING_GRID)

      #if DISABLED(DELTA)
        bool do_topography_map = verbose_level > 2 || code_seen('T');
      #endif

      if (verbose_level > 0) {
        SERIAL_PROTOCOLPGM("G29 Auto Bed Leveling\n");
        if (dryrun) SERIAL_ECHOLNPGM("Running in DRY-RUN mode");
      }

      int auto_bed_leveling_grid_points = AUTO_BED_LEVELING_GRID_POINTS;

      #if DISABLED(DELTA)
        if (code_seen('P')) auto_bed_leveling_grid_points = code_value_short();
        if (auto_bed_leveling_grid_points < 2) {
          SERIAL_PROTOCOLPGM("?Number of probed (P)oints is implausible (2 minimum).\n");
          return;
        }
      #endif

      xy_travel_speed = code_seen('S') ? code_value_short() : XY_TRAVEL_SPEED;

      int left_probe_bed_position = code_seen('L') ? code_value_short() : LEFT_PROBE_BED_POSITION,
          right_probe_bed_position = code_seen('R') ? code_value_short() : RIGHT_PROBE_BED_POSITION,
          front_probe_bed_position = code_seen('F') ? code_value_short() : FRONT_PROBE_BED_POSITION,
          back_probe_bed_position = code_seen('B') ? code_value_short() : BACK_PROBE_BED_POSITION;

      bool left_out_l = left_probe_bed_position < MIN_PROBE_X,
           left_out = left_out_l || left_probe_bed_position > right_probe_bed_position - (MIN_PROBE_EDGE),
           right_out_r = right_probe_bed_position > MAX_PROBE_X,
           right_out = right_out_r || right_probe_bed_position < left_probe_bed_position + MIN_PROBE_EDGE,
           front_out_f = front_probe_bed_position < MIN_PROBE_Y,
           front_out = front_out_f || front_probe_bed_position > back_probe_bed_position - (MIN_PROBE_EDGE),
           back_out_b = back_probe_bed_position > MAX_PROBE_Y,
           back_out = back_out_b || back_probe_bed_position < front_probe_bed_position + MIN_PROBE_EDGE;

      if (left_out || right_out || front_out || back_out) {
        if (left_out) {
          out_of_range_error(PSTR("(L)eft"));
          left_probe_bed_position = left_out_l ? MIN_PROBE_X : right_probe_bed_position - (MIN_PROBE_EDGE);
        }
        if (right_out) {
          out_of_range_error(PSTR("(R)ight"));
          right_probe_bed_position = right_out_r ? MAX_PROBE_X : left_probe_bed_position + MIN_PROBE_EDGE;
        }
        if (front_out) {
          out_of_range_error(PSTR("(F)ront"));
          front_probe_bed_position = front_out_f ? MIN_PROBE_Y : back_probe_bed_position - (MIN_PROBE_EDGE);
        }
        if (back_out) {
          out_of_range_error(PSTR("(B)ack"));
          back_probe_bed_position = back_out_b ? MAX_PROBE_Y : front_probe_bed_position + MIN_PROBE_EDGE;
        }
        return;
      }

    #endif // AUTO_BED_LEVELING_GRID

    #if ENABLED(Z_PROBE_SLED)
      dock_sled(false); // engage (un-dock) the Z probe
    #elif ENABLED(Z_PROBE_ALLEN_KEY) || (ENABLED(DELTA) && SERVO_LEVELING)
      deploy_z_probe();
    #endif

    st_synchronize();

    if (!dryrun) {
      // make sure the bed_level_rotation_matrix is identity or the planner will get it wrong
      plan_bed_level_matrix.set_to_identity();

      #if ENABLED(DELTA)
        reset_bed_level();
      #else //!DELTA
        //vector_3 corrected_position = plan_get_position_mm();
        //corrected_position.debug("position before G29");
        vector_3 uncorrected_position = plan_get_position();
        //uncorrected_position.debug("position during G29");
        current_position[X_AXIS] = uncorrected_position.x;
        current_position[Y_AXIS] = uncorrected_position.y;
        current_position[Z_AXIS] = uncorrected_position.z;
        sync_plan_position();
      #endif // !DELTA
    }

    setup_for_endstop_move();

    feedrate = homing_feedrate[Z_AXIS];

    #if ENABLED(AUTO_BED_LEVELING_GRID)

      // probe at the points of a lattice grid
      const int xGridSpacing = (right_probe_bed_position - left_probe_bed_position) / (auto_bed_leveling_grid_points - 1),
                yGridSpacing = (back_probe_bed_position - front_probe_bed_position) / (auto_bed_leveling_grid_points - 1);

      #if ENABLED(DELTA)
        delta_grid_spacing[0] = xGridSpacing;
        delta_grid_spacing[1] = yGridSpacing;
        float z_offset = zprobe_zoffset;
        if (code_seen(axis_codes[Z_AXIS])) z_offset += code_value();
      #else // !DELTA
        /**
         * solve the plane equation ax + by + d = z
         * A is the matrix with rows [x y 1] for all the probed points
         * B is the vector of the Z positions
         * the normal vector to the plane is formed by the coefficients of the
         * plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
         * so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z
         */

        int abl2 = auto_bed_leveling_grid_points * auto_bed_leveling_grid_points;

        double eqnAMatrix[abl2 * 3], // "A" matrix of the linear system of equations
               eqnBVector[abl2],     // "B" vector of Z points
               mean = 0.0;
        int8_t indexIntoAB[auto_bed_leveling_grid_points][auto_bed_leveling_grid_points];
      #endif // !DELTA

      int probePointCounter = 0;
      bool zig = (auto_bed_leveling_grid_points & 1) ? true : false; //always end at [RIGHT_PROBE_BED_POSITION, BACK_PROBE_BED_POSITION]

      for (int yCount = 0; yCount < auto_bed_leveling_grid_points; yCount++) {
        double yProbe = front_probe_bed_position + yGridSpacing * yCount;
        int xStart, xStop, xInc;

        if (zig) {
          xStart = 0;
          xStop = auto_bed_leveling_grid_points;
          xInc = 1;
        }
        else {
          xStart = auto_bed_leveling_grid_points - 1;
          xStop = -1;
          xInc = -1;
        }

        zig = !zig;

        for (int xCount = xStart; xCount != xStop; xCount += xInc) {
          double xProbe = left_probe_bed_position + xGridSpacing * xCount;

          // raise extruder
          float measured_z,
                z_before = probePointCounter ? Z_RAISE_BETWEEN_PROBINGS + current_position[Z_AXIS] : Z_RAISE_BEFORE_PROBING;

          if (probePointCounter) {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("z_before = (between) ", (float)(Z_RAISE_BETWEEN_PROBINGS + current_position[Z_AXIS]));
                SERIAL_EOL;
              }
            #endif
          }
          else {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("z_before = (before) ", (float)Z_RAISE_BEFORE_PROBING);
                SERIAL_EOL;
              }
            #endif
          }

          #if ENABLED(DELTA)
            // Avoid probing the corners (outside the round or hexagon print surface) on a delta printer.
            float distance_from_center = sqrt(xProbe * xProbe + yProbe * yProbe);
            if (distance_from_center > DELTA_PROBEABLE_RADIUS) continue;
          #endif //DELTA

          ProbeAction act;
          if (deploy_probe_for_each_reading) // G29 E - Stow between probes
            act = ProbeDeployAndStow;
          else if (yCount == 0 && xCount == xStart)
            act = ProbeDeploy;
          else if (yCount == auto_bed_leveling_grid_points - 1 && xCount == xStop - xInc)
            act = ProbeStow;
          else
            act = ProbeStay;

          measured_z = probe_pt(xProbe, yProbe, z_before, act, verbose_level);

          #if DISABLED(DELTA)
            mean += measured_z;

            eqnBVector[probePointCounter] = measured_z;
            eqnAMatrix[probePointCounter + 0 * abl2] = xProbe;
            eqnAMatrix[probePointCounter + 1 * abl2] = yProbe;
            eqnAMatrix[probePointCounter + 2 * abl2] = 1;
            indexIntoAB[xCount][yCount] = probePointCounter;
          #else
            bed_level[xCount][yCount] = measured_z + z_offset;
          #endif

          probePointCounter++;

          idle();

        } //xProbe
      } //yProbe

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          print_xyz("> probing complete > current_position", current_position);
        }
      #endif

      clean_up_after_endstop_move();

      #if ENABLED(DELTA)

        if (!dryrun) extrapolate_unprobed_bed_level();
        print_bed_level();

      #else // !DELTA

        // solve lsq problem
        double plane_equation_coefficients[3];
        qr_solve(plane_equation_coefficients, abl2, 3, eqnAMatrix, eqnBVector);

        mean /= abl2;

        if (verbose_level) {
          SERIAL_PROTOCOLPGM("Eqn coefficients: a: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[0], 8);
          SERIAL_PROTOCOLPGM(" b: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[1], 8);
          SERIAL_PROTOCOLPGM(" d: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[2], 8);
          SERIAL_EOL;
          if (verbose_level > 2) {
            SERIAL_PROTOCOLPGM("Mean of sampled points: ");
            SERIAL_PROTOCOL_F(mean, 8);
            SERIAL_EOL;
          }
        }

        if (!dryrun) set_bed_level_equation_lsq(plane_equation_coefficients);

        // Show the Topography map if enabled
        if (do_topography_map) {

          SERIAL_PROTOCOLPGM(" \nBed Height Topography: \n");
          SERIAL_PROTOCOLPGM("   +--- BACK --+\n");
          SERIAL_PROTOCOLPGM("   |           |\n");
          SERIAL_PROTOCOLPGM(" L |    (+)    | R\n");
          SERIAL_PROTOCOLPGM(" E |           | I\n");
          SERIAL_PROTOCOLPGM(" F | (-) N (+) | G\n");
          SERIAL_PROTOCOLPGM(" T |           | H\n");
          SERIAL_PROTOCOLPGM("   |    (-)    | T\n");
          SERIAL_PROTOCOLPGM("   |           |\n");
          SERIAL_PROTOCOLPGM("   O-- FRONT --+\n");
          SERIAL_PROTOCOLPGM(" (0,0)\n");

          float min_diff = 999;

          for (int yy = auto_bed_leveling_grid_points - 1; yy >= 0; yy--) {
            for (int xx = 0; xx < auto_bed_leveling_grid_points; xx++) {
              int ind = indexIntoAB[xx][yy];
              float diff = eqnBVector[ind] - mean;

              float x_tmp = eqnAMatrix[ind + 0 * abl2],
                    y_tmp = eqnAMatrix[ind + 1 * abl2],
                    z_tmp = 0;

              apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);

              NOMORE(min_diff, eqnBVector[ind] - z_tmp);

              if (diff >= 0.0)
                SERIAL_PROTOCOLPGM(" +");   // Include + for column alignment
              else
                SERIAL_PROTOCOLCHAR(' ');
              SERIAL_PROTOCOL_F(diff, 5);
            } // xx
            SERIAL_EOL;
          } // yy
          SERIAL_EOL;
          if (verbose_level > 3) {
            SERIAL_PROTOCOLPGM(" \nCorrected Bed Height vs. Bed Topology: \n");

            for (int yy = auto_bed_leveling_grid_points - 1; yy >= 0; yy--) {
              for (int xx = 0; xx < auto_bed_leveling_grid_points; xx++) {
                int ind = indexIntoAB[xx][yy];
                float x_tmp = eqnAMatrix[ind + 0 * abl2],
                      y_tmp = eqnAMatrix[ind + 1 * abl2],
                      z_tmp = 0;

                apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);

                float diff = eqnBVector[ind] - z_tmp - min_diff;
                if (diff >= 0.0)
                  SERIAL_PROTOCOLPGM(" +");
                // Include + for column alignment
                else
                  SERIAL_PROTOCOLCHAR(' ');
                SERIAL_PROTOCOL_F(diff, 5);
              } // xx
              SERIAL_EOL;
            } // yy
            SERIAL_EOL;
          }
        } //do_topography_map
      #endif //!DELTA

    #else // !AUTO_BED_LEVELING_GRID

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOLNPGM("> 3-point Leveling");
        }
      #endif

      // Actions for each probe
      ProbeAction p1, p2, p3;
      if (deploy_probe_for_each_reading)
        p1 = p2 = p3 = ProbeDeployAndStow;
      else
        p1 = ProbeDeploy, p2 = ProbeStay, p3 = ProbeStow;

      // Probe at 3 arbitrary points
      float z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING, p1, verbose_level),
            z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, p2, verbose_level),
            z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, p3, verbose_level);
      clean_up_after_endstop_move();
      if (!dryrun) set_bed_level_equation_3pts(z_at_pt_1, z_at_pt_2, z_at_pt_3);

    #endif // !AUTO_BED_LEVELING_GRID

    #if ENABLED(DELTA)
      // Allen Key Probe for Delta
      #if ENABLED(Z_PROBE_ALLEN_KEY) || SERVO_LEVELING
        stow_z_probe();
      #elif Z_RAISE_AFTER_PROBING > 0
        raise_z_after_probing(); // ???
      #endif
    #else // !DELTA
      if (verbose_level > 0)
        plan_bed_level_matrix.debug(" \n\nBed Level Correction Matrix:");

      if (!dryrun) {
        /**
         * Correct the Z height difference from Z probe position and nozzle tip position.
         * The Z height on homing is measured by Z probe, but the Z probe is quite far
         * from the nozzle. When the bed is uneven, this height must be corrected.
         */
        float x_tmp = current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER,
              y_tmp = current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER,
              z_tmp = current_position[Z_AXIS],
              real_z = st_get_axis_position_mm(Z_AXIS);  //get the real Z (since plan_get_position is now correcting the plane)

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("> BEFORE apply_rotation_xyz > z_tmp  = ", z_tmp);
            SERIAL_EOL;
            SERIAL_ECHOPAIR("> BEFORE apply_rotation_xyz > real_z = ", real_z);
            SERIAL_EOL;
          }
        #endif

        // Apply the correction sending the Z probe offset
        apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);

        /*
         * Get the current Z position and send it to the planner.
         *
         * >> (z_tmp - real_z) : The rotated current Z minus the uncorrected Z
         * (most recent plan_set_position/sync_plan_position)
         *
         * >> zprobe_zoffset : Z distance from nozzle to Z probe
         * (set by default, M851, EEPROM, or Menu)
         *
         * >> Z_RAISE_AFTER_PROBING : The distance the Z probe will have lifted
         * after the last probe
         *
         * >> Should home_offset[Z_AXIS] be included?
         *
         *
         *   Discussion: home_offset[Z_AXIS] was applied in G28 to set the
         *   starting Z. If Z is not tweaked in G29 -and- the Z probe in G29 is
         *   not actually "homing" Z... then perhaps it should not be included
         *   here. The purpose of home_offset[] is to adjust for inaccurate
         *   endstops, not for reasonably accurate probes. If it were added
         *   here, it could be seen as a compensating factor for the Z probe.
         */
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("> AFTER apply_rotation_xyz > z_tmp  = ", z_tmp);
            SERIAL_EOL;
          }
        #endif

        current_position[Z_AXIS] = -zprobe_zoffset + (z_tmp - real_z)
          #if HAS_SERVO_ENDSTOPS || ENABLED(Z_PROBE_ALLEN_KEY) || ENABLED(Z_PROBE_SLED)
             + Z_RAISE_AFTER_PROBING
          #endif
          ;
        // current_position[Z_AXIS] += home_offset[Z_AXIS]; // The Z probe determines Z=0, not "Z home"
        sync_plan_position();

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            print_xyz("> corrected Z in G29", current_position);
          }
        #endif
      }

      // Sled assembly for Cartesian bots
      #if ENABLED(Z_PROBE_SLED)
        dock_sled(true); // dock the sled
      #elif Z_RAISE_AFTER_PROBING > 0
        // Raise Z axis for non-delta and non servo based probes
        #if !defined(HAS_SERVO_ENDSTOPS) && DISABLED(Z_PROBE_ALLEN_KEY) && DISABLED(Z_PROBE_SLED)
          raise_z_after_probing();
        #endif
      #endif

    #endif // !DELTA

    #ifdef Z_PROBE_END_SCRIPT
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHO("Z Probe End Script: ");
          SERIAL_ECHOLNPGM(Z_PROBE_END_SCRIPT);
        }
      #endif
      enqueue_and_echo_commands_P(PSTR(Z_PROBE_END_SCRIPT));
      #if ENABLED(HAS_Z_MIN_PROBE)
        z_probe_is_active = false;
      #endif
      st_synchronize();
    #endif

    KEEPALIVE_STATE(IN_HANDLER);

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("<<< gcode_G29");
      }
    #endif

    gcode_M114(); // Send end position to RepetierHost

  }

  #if DISABLED(Z_PROBE_SLED) // could be avoided

    /**
     * G30: Do a single Z probe at the current XY
     */
    inline void gcode_G30() {
      #if HAS_SERVO_ENDSTOPS
        raise_z_for_servo();
      #endif
      deploy_z_probe(); // Engage Z Servo endstop if available. Z_PROBE_SLED is missed here.

      st_synchronize();
      // TODO: clear the leveling matrix or the planner will be set incorrectly
      setup_for_endstop_move(); // Too late. Must be done before deploying.

      feedrate = homing_feedrate[Z_AXIS];

      run_z_probe();
      SERIAL_PROTOCOLPGM("Bed X: ");
      SERIAL_PROTOCOL(current_position[X_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL(current_position[Y_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Z: ");
      SERIAL_PROTOCOL(current_position[Z_AXIS] + 0.0001);
      SERIAL_EOL;

      clean_up_after_endstop_move(); // Too early. must be done after the stowing.

      #if HAS_SERVO_ENDSTOPS
        raise_z_for_servo();
      #endif
      stow_z_probe(false); // Retract Z Servo endstop if available. Z_PROBE_SLED is missed here.

      gcode_M114(); // Send end position to RepetierHost
    }

  #endif //!Z_PROBE_SLED

#endif //AUTO_BED_LEVELING_FEATURE

/**
 * G92: Set current position to given X Y Z E
 */
inline void gcode_G92() {
  if (!code_seen(axis_codes[E_AXIS]))
    st_synchronize();

  bool didXYZ = false;
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      float v = current_position[i] = code_value();
      if (i == E_AXIS)
        plan_set_e_position(v);
      else
        didXYZ = true;
    }
  }
  if (didXYZ) {
    #if ENABLED(DELTA) || ENABLED(SCARA)
      sync_plan_position_delta();
    #else
      sync_plan_position();
    #endif
  }
}

#if ENABLED(ULTIPANEL)

  /**
   * M0: // M0 - Unconditional stop - Wait for user button press on LCD
   * M1: // M1 - Conditional stop - Wait for user button press on LCD
   */
  inline void gcode_M0_M1() {
    char* args = current_command_args;

    millis_t codenum = 0;
    bool hasP = false, hasS = false;
    if (code_seen('P')) {
      codenum = code_value_short(); // milliseconds to wait
      hasP = codenum > 0;
    }
    if (code_seen('S')) {
      codenum = code_value() * 1000; // seconds to wait
      hasS = codenum > 0;
    }

    if (!hasP && !hasS && *args != '\0')
      lcd_setstatus(args, true);
    else {
      LCD_MESSAGEPGM(MSG_USERWAIT);
      #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
        dontExpireStatus();
      #endif
    }

    lcd_ignore_click();
    st_synchronize();
    refresh_cmd_timeout();
    if (codenum > 0) {
      codenum += previous_cmd_ms;  // wait until this time for a click
      KEEPALIVE_STATE(PAUSED_FOR_USER);
      while (millis() < codenum && !lcd_clicked()) idle();
      KEEPALIVE_STATE(IN_HANDLER);
      lcd_ignore_click(false);
    }
    else {
      if (!lcd_detected()) return;
      KEEPALIVE_STATE(PAUSED_FOR_USER);
      while (!lcd_clicked()) idle();
      KEEPALIVE_STATE(IN_HANDLER);
    }
    if (IS_SD_PRINTING)
      LCD_MESSAGEPGM(MSG_RESUMING);
    else
      LCD_MESSAGEPGM(WELCOME_MSG);
  }

#endif // ULTIPANEL

/**
 * M17: Enable power on all stepper motors
 */
inline void gcode_M17() {
  LCD_MESSAGEPGM(MSG_NO_MOVE);
  enable_all_steppers();
}

#if ENABLED(SDSUPPORT)

  /**
   * M20: List SD card to serial output
   */
  inline void gcode_M20() {
    SERIAL_PROTOCOLLNPGM(MSG_BEGIN_FILE_LIST);
    card.ls();
    SERIAL_PROTOCOLLNPGM(MSG_END_FILE_LIST);
  }

  /**
   * M21: Init SD Card
   */
  inline void gcode_M21() {
    card.initsd();
  }

  /**
   * M22: Release SD Card
   */
  inline void gcode_M22() {
    card.release();
  }

  /**
   * M23: Open a file
   */
  inline void gcode_M23() {
    card.openFile(current_command_args, true);
  }

  /**
   * M24: Start SD Print
   */
  inline void gcode_M24() {
    card.startFileprint();
    print_job_start();
  }

  /**
   * M25: Pause SD Print
   */
  inline void gcode_M25() {
    card.pauseSDPrint();
  }

  /**
   * M26: Set SD Card file index
   */
  inline void gcode_M26() {
    if (card.cardOK && code_seen('S'))
      card.setIndex(code_value_short());
  }

  /**
   * M27: Get SD Card status
   */
  inline void gcode_M27() {
    card.getStatus();
  }

  /**
   * M28: Start SD Write
   */
  inline void gcode_M28() {
    card.openFile(current_command_args, false);
  }

  /**
   * M29: Stop SD Write
   * Processed in write to file routine above
   */
  inline void gcode_M29() {
    // card.saving = false;
  }

  /**
   * M30 <filename>: Delete SD Card file
   */
  inline void gcode_M30() {
    if (card.cardOK) {
      card.closefile();
      card.removeFile(current_command_args);
    }
  }

#endif //SDSUPPORT

/**
 * M31: Get the time since the start of SD Print (or last M109)
 */
inline void gcode_M31() {
  millis_t t = print_job_timer();
  int min = t / 60, sec = t % 60;
  char time[30];
  sprintf_P(time, PSTR("%i min, %i sec"), min, sec);
  SERIAL_ECHO_START;
  SERIAL_ECHOLN(time);
  lcd_setstatus(time);
  autotempShutdown();
}

#if ENABLED(SDSUPPORT)

  /**
   * M32: Select file and start SD Print
   */
  inline void gcode_M32() {
    if (card.sdprinting)
      st_synchronize();

    char* namestartpos = strchr(current_command_args, '!');  // Find ! to indicate filename string start.
    if (!namestartpos)
      namestartpos = current_command_args; // Default name position, 4 letters after the M
    else
      namestartpos++; //to skip the '!'

    bool call_procedure = code_seen('P') && (seen_pointer < namestartpos);

    if (card.cardOK) {
      card.openFile(namestartpos, true, call_procedure);

      if (code_seen('S') && seen_pointer < namestartpos) // "S" (must occur _before_ the filename!)
        card.setIndex(code_value_short());

      card.startFileprint();

      // Procedure calls count as normal print time.
      if (!call_procedure) print_job_start();
    }
  }

  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)

    /**
     * M33: Get the long full path of a file or folder
     *
     * Parameters:
     *   <dospath> Case-insensitive DOS-style path to a file or folder
     *
     * Example:
     *   M33 miscel~1/armchair/armcha~1.gco
     *
     * Output:
     *   /Miscellaneous/Armchair/Armchair.gcode
     */
    inline void gcode_M33() {
      card.printLongPath(current_command_args);
    }

  #endif

  /**
   * M928: Start SD Write
   */
  inline void gcode_M928() {
    card.openLogFile(current_command_args);
  }

#endif // SDSUPPORT

/**
 * M42: Change pin status via GCode
 *
 *  P<pin>  Pin number (LED if omitted)
 *  S<byte> Pin status from 0 - 255
 */
inline void gcode_M42() {
  if (code_seen('S')) {
    int pin_status = code_value_short();
    if (pin_status < 0 || pin_status > 255) return;

    int pin_number = code_seen('P') ? code_value_short() : LED_PIN;
    if (pin_number < 0) return;

    for (uint8_t i = 0; i < COUNT(sensitive_pins); i++)
      if (pin_number == sensitive_pins[i]) return;

    pinMode(pin_number, OUTPUT);
    digitalWrite(pin_number, pin_status);
    analogWrite(pin_number, pin_status);

    #if FAN_COUNT > 0
      switch (pin_number) {
        #if HAS_FAN0
          case FAN_PIN: fanSpeeds[0] = pin_status; break;
        #endif
        #if HAS_FAN1
          case FAN1_PIN: fanSpeeds[1] = pin_status; break;
        #endif
        #if HAS_FAN2
          case FAN2_PIN: fanSpeeds[2] = pin_status; break;
        #endif
      }
    #endif

  } // code_seen('S')
}

#if ENABLED(AUTO_BED_LEVELING_FEATURE) && ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)

  /**
   * This is redundant since the SanityCheck.h already checks for a valid
   *  Z_MIN_PROBE_PIN, but here for clarity.
   */
  #if ENABLED(Z_MIN_PROBE_ENDSTOP)
    #if !HAS_Z_PROBE
      #error You must define Z_MIN_PROBE_PIN to enable Z probe repeatability calculation.
    #endif
  #elif !HAS_Z_MIN
    #error You must define Z_MIN_PIN to enable Z probe repeatability calculation.
  #endif

  /**
   * M48: Z probe repeatability measurement function.
   *
   * Usage:
   *   M48 <P#> <X#> <Y#> <V#> <E> <L#>
   *     P = Number of sampled points (4-50, default 10)
   *     X = Sample X position
   *     Y = Sample Y position
   *     V = Verbose level (0-4, default=1)
   *     E = Engage Z probe for each reading
   *     L = Number of legs of movement before probe
   *     S = Schizoid (Or Star if you prefer)
   *
   * This function assumes the bed has been homed.  Specifically, that a G28 command
   * as been issued prior to invoking the M48 Z probe repeatability measurement function.
   * Any information generated by a prior G29 Bed leveling command will be lost and need to be
   * regenerated.
   */
  inline void gcode_M48() {

    if (!axis_homed[X_AXIS] || !axis_homed[Y_AXIS] || !axis_homed[Z_AXIS]) {
      axis_unhomed_error();
      return;
    }

    double sum = 0.0, mean = 0.0, sigma = 0.0, sample_set[50];
    uint8_t verbose_level = 1, n_samples = 10, n_legs = 0, schizoid_flag = 0;

    if (code_seen('V')) {
      verbose_level = code_value_short();
      if (verbose_level < 0 || verbose_level > 4) {
        SERIAL_PROTOCOLPGM("?Verbose Level not plausible (0-4).\n");
        return;
      }
    }

    if (verbose_level > 0)
      SERIAL_PROTOCOLPGM("M48 Z-Probe Repeatability test\n");

    if (code_seen('P')) {
      n_samples = code_value_short();
      if (n_samples < 4 || n_samples > 50) {
        SERIAL_PROTOCOLPGM("?Sample size not plausible (4-50).\n");
        return;
      }
    }

    float  X_current = current_position[X_AXIS],
           Y_current = current_position[Y_AXIS],
           Z_current = current_position[Z_AXIS],
           X_probe_location = X_current + X_PROBE_OFFSET_FROM_EXTRUDER,
           Y_probe_location = Y_current + Y_PROBE_OFFSET_FROM_EXTRUDER,
           Z_start_location = Z_current + Z_RAISE_BEFORE_PROBING;
    bool deploy_probe_for_each_reading = code_seen('E');

    if (code_seen('X')) {
      X_probe_location = code_value();
      #if DISABLED(DELTA)
        if (X_probe_location < MIN_PROBE_X || X_probe_location > MAX_PROBE_X) {
          out_of_range_error(PSTR("X"));
          return;
        }
      #endif
    }

    if (code_seen('Y')) {
      Y_probe_location = code_value();
      #if DISABLED(DELTA)
        if (Y_probe_location < MIN_PROBE_Y || Y_probe_location > MAX_PROBE_Y) {
          out_of_range_error(PSTR("Y"));
          return;
        }
      #endif
    }

    #if ENABLED(DELTA)
      if (sqrt(X_probe_location * X_probe_location + Y_probe_location * Y_probe_location) > DELTA_PROBEABLE_RADIUS) {
        SERIAL_PROTOCOLPGM("? (X,Y) location outside of probeable radius.\n");
        return;
      }
    #endif

    bool seen_L = code_seen('L');

    if (seen_L) {
      n_legs = code_value_short();
      if (n_legs < 0 || n_legs > 15) {
        SERIAL_PROTOCOLPGM("?Number of legs in movement not plausible (0-15).\n");
        return;
      }
      if (n_legs == 1) n_legs = 2;
    }

    if (code_seen('S')) {
      schizoid_flag++;
      if (!seen_L) n_legs = 7;
    }

    /**
     * Now get everything to the specified probe point So we can safely do a
     * probe to get us close to the bed.  If the Z-Axis is far from the bed,
     * we don't want to use that as a starting point for each probe.
     */
    if (verbose_level > 2)
      SERIAL_PROTOCOLPGM("Positioning the probe...\n");

    #if ENABLED(DELTA)
      // we don't do bed level correction in M48 because we want the raw data when we probe
      reset_bed_level();
    #else
      // we don't do bed level correction in M48 because we want the raw data when we probe
      plan_bed_level_matrix.set_to_identity();
    #endif

    if (Z_start_location < Z_RAISE_BEFORE_PROBING * 2.0)
      do_blocking_move_to_z(Z_start_location);

    do_blocking_move_to_xy(X_probe_location - X_PROBE_OFFSET_FROM_EXTRUDER, Y_probe_location - Y_PROBE_OFFSET_FROM_EXTRUDER);

    /**
     * OK, do the initial probe to get us close to the bed.
     * Then retrace the right amount and use that in subsequent probes
     */
    setup_for_endstop_move();

    probe_pt(X_probe_location, Y_probe_location, Z_RAISE_BEFORE_PROBING,
      deploy_probe_for_each_reading ? ProbeDeployAndStow : ProbeDeploy,
      verbose_level);

    raise_z_after_probing();

    for (uint8_t n = 0; n < n_samples; n++) {
      randomSeed(millis());
      delay(500);
      if (n_legs) {
        float radius, angle = random(0.0, 360.0);
        int dir = (random(0, 10) > 5.0) ? -1 : 1;  // clockwise or counter clockwise

        radius = random(
          #if ENABLED(DELTA)
            DELTA_PROBEABLE_RADIUS / 8, DELTA_PROBEABLE_RADIUS / 3
          #else
            5, X_MAX_LENGTH / 8
          #endif
        );

        if (verbose_level > 3) {
          SERIAL_ECHOPAIR("Starting radius: ", radius);
          SERIAL_ECHOPAIR("   angle: ", angle);
          delay(100);
          if (dir > 0)
            SERIAL_ECHO(" Direction: Counter Clockwise \n");
          else
            SERIAL_ECHO(" Direction: Clockwise \n");
          delay(100);
        }

        for (uint8_t l = 0; l < n_legs - 1; l++) {
          double delta_angle;

          if (schizoid_flag)
            // The points of a 5 point star are 72 degrees apart.  We need to
            // skip a point and go to the next one on the star.
            delta_angle = dir * 2.0 * 72.0;

          else
            // If we do this line, we are just trying to move further
            // around the circle.
            delta_angle = dir * (float) random(25, 45);

          angle += delta_angle;

          while (angle > 360.0)   // We probably do not need to keep the angle between 0 and 2*PI, but the
            angle -= 360.0;       // Arduino documentation says the trig functions should not be given values
          while (angle < 0.0)     // outside of this range.   It looks like they behave correctly with
            angle += 360.0;       // numbers outside of the range, but just to be safe we clamp them.

          X_current = X_probe_location - X_PROBE_OFFSET_FROM_EXTRUDER + cos(RADIANS(angle)) * radius;
          Y_current = Y_probe_location - Y_PROBE_OFFSET_FROM_EXTRUDER + sin(RADIANS(angle)) * radius;

          #if DISABLED(DELTA)
            X_current = constrain(X_current, X_MIN_POS, X_MAX_POS);
            Y_current = constrain(Y_current, Y_MIN_POS, Y_MAX_POS);
          #else
            // If we have gone out too far, we can do a simple fix and scale the numbers
            // back in closer to the origin.
            while (sqrt(X_current * X_current + Y_current * Y_current) > DELTA_PROBEABLE_RADIUS) {
              X_current /= 1.25;
              Y_current /= 1.25;
              if (verbose_level > 3) {
                SERIAL_ECHOPAIR("Pulling point towards center:", X_current);
                SERIAL_ECHOPAIR(", ", Y_current);
                SERIAL_EOL;
                delay(50);
              }
            }
          #endif
          if (verbose_level > 3) {
            SERIAL_PROTOCOL("Going to:");
            SERIAL_ECHOPAIR("x: ", X_current);
            SERIAL_ECHOPAIR("y: ", Y_current);
            SERIAL_ECHOPAIR("  z: ", current_position[Z_AXIS]);
            SERIAL_EOL;
            delay(55);
          }
          do_blocking_move_to_xy(X_current, Y_current);
        } // n_legs loop
      } // n_legs

      /**
       * We don't really have to do this move, but if we don't we can see a
       * funny shift in the Z Height because the user might not have the
       * Z_RAISE_BEFORE_PROBING height identical to the Z_RAISE_BETWEEN_PROBING
       * height. This gets us back to the probe location at the same height that
       * we have been running around the circle at.
       */
      do_blocking_move_to_xy(X_probe_location - X_PROBE_OFFSET_FROM_EXTRUDER, Y_probe_location - Y_PROBE_OFFSET_FROM_EXTRUDER);
      if (deploy_probe_for_each_reading)
        sample_set[n] = probe_pt(X_probe_location, Y_probe_location, Z_RAISE_BEFORE_PROBING, ProbeDeployAndStow, verbose_level);
      else {
        if (n == n_samples - 1)
          sample_set[n] = probe_pt(X_probe_location, Y_probe_location, Z_RAISE_BEFORE_PROBING, ProbeStow, verbose_level); else
          sample_set[n] = probe_pt(X_probe_location, Y_probe_location, Z_RAISE_BEFORE_PROBING, ProbeStay, verbose_level);
      }

      /**
       * Get the current mean for the data points we have so far
       */
      sum = 0.0;
      for (uint8_t j = 0; j <= n; j++) sum += sample_set[j];
      mean = sum / (n + 1);

      /**
       * Now, use that mean to calculate the standard deviation for the
       * data points we have so far
       */
      sum = 0.0;
      for (uint8_t j = 0; j <= n; j++) {
        float ss = sample_set[j] - mean;
        sum += ss * ss;
      }
      sigma = sqrt(sum / (n + 1));
      if (verbose_level > 1) {
        SERIAL_PROTOCOL(n + 1);
        SERIAL_PROTOCOLPGM(" of ");
        SERIAL_PROTOCOL((int)n_samples);
        SERIAL_PROTOCOLPGM("   z: ");
        SERIAL_PROTOCOL_F(current_position[Z_AXIS], 6);
        delay(50);
        if (verbose_level > 2) {
          SERIAL_PROTOCOLPGM(" mean: ");
          SERIAL_PROTOCOL_F(mean, 6);
          SERIAL_PROTOCOLPGM("   sigma: ");
          SERIAL_PROTOCOL_F(sigma, 6);
        }
      }
      if (verbose_level > 0) SERIAL_EOL;
      delay(50);
      do_blocking_move_to_z(current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);
    }  // End of probe loop code

    // raise_z_after_probing();

    if (verbose_level > 0) {
      SERIAL_PROTOCOLPGM("Mean: ");
      SERIAL_PROTOCOL_F(mean, 6);
      SERIAL_EOL;
      delay(25);
    }

    SERIAL_PROTOCOLPGM("Standard Deviation: ");
    SERIAL_PROTOCOL_F(sigma, 6);
    SERIAL_EOL; SERIAL_EOL;
    delay(25);

    clean_up_after_endstop_move();

    gcode_M114(); // Send end position to RepetierHost
  }

#endif // AUTO_BED_LEVELING_FEATURE && Z_MIN_PROBE_REPEATABILITY_TEST

/**
 * M104: Set hot end temperature
 */
inline void gcode_M104() {
  if (setTargetedHotend(104)) return;
  if (DEBUGGING(DRYRUN)) return;

  // Start hook must happen before setTargetHotend()
  print_job_start();

  if (code_seen('S')) {
    float temp = code_value();
    setTargetHotend(temp, target_extruder);
    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        setTargetHotend1(temp == 0.0 ? 0.0 : temp + duplicate_extruder_temp_offset);
    #endif

    if (temp > degHotend(target_extruder)) LCD_MESSAGEPGM(MSG_HEATING);
  }

  if (print_job_stop()) LCD_MESSAGEPGM(WELCOME_MSG);
}

#if HAS_TEMP_0 || HAS_TEMP_BED || ENABLED(HEATER_0_USES_MAX6675)

  void print_heaterstates() {
    #if HAS_TEMP_0 || ENABLED(HEATER_0_USES_MAX6675)
      SERIAL_PROTOCOLPGM(" T:");
      SERIAL_PROTOCOL_F(degHotend(target_extruder), 1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetHotend(target_extruder), 1);
    #endif
    #if HAS_TEMP_BED
      SERIAL_PROTOCOLPGM(" B:");
      SERIAL_PROTOCOL_F(degBed(), 1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetBed(), 1);
    #endif
    #if EXTRUDERS > 1
      for (int8_t e = 0; e < EXTRUDERS; ++e) {
        SERIAL_PROTOCOLPGM(" T");
        SERIAL_PROTOCOL(e);
        SERIAL_PROTOCOLCHAR(':');
        SERIAL_PROTOCOL_F(degHotend(e), 1);
        SERIAL_PROTOCOLPGM(" /");
        SERIAL_PROTOCOL_F(degTargetHotend(e), 1);
      }
    #endif
    #if HAS_TEMP_BED
      SERIAL_PROTOCOLPGM(" B@:");
      #ifdef BED_WATTS
        SERIAL_PROTOCOL(((BED_WATTS) * getHeaterPower(-1)) / 127);
        SERIAL_PROTOCOLCHAR('W');
      #else
        SERIAL_PROTOCOL(getHeaterPower(-1));
      #endif
    #endif
    SERIAL_PROTOCOLPGM(" @:");
    #ifdef EXTRUDER_WATTS
      SERIAL_PROTOCOL(((EXTRUDER_WATTS) * getHeaterPower(target_extruder)) / 127);
      SERIAL_PROTOCOLCHAR('W');
    #else
      SERIAL_PROTOCOL(getHeaterPower(target_extruder));
    #endif
    #if EXTRUDERS > 1
      for (int8_t e = 0; e < EXTRUDERS; ++e) {
        SERIAL_PROTOCOLPGM(" @");
        SERIAL_PROTOCOL(e);
        SERIAL_PROTOCOLCHAR(':');
        #ifdef EXTRUDER_WATTS
          SERIAL_PROTOCOL(((EXTRUDER_WATTS) * getHeaterPower(e)) / 127);
          SERIAL_PROTOCOLCHAR('W');
        #else
          SERIAL_PROTOCOL(getHeaterPower(e));
        #endif
      }
    #endif
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      #if HAS_TEMP_BED
        SERIAL_PROTOCOLPGM("    ADC B:");
        SERIAL_PROTOCOL_F(degBed(), 1);
        SERIAL_PROTOCOLPGM("C->");
        SERIAL_PROTOCOL_F(rawBedTemp() / OVERSAMPLENR, 0);
      #endif
      for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
        SERIAL_PROTOCOLPGM("  T");
        SERIAL_PROTOCOL(cur_extruder);
        SERIAL_PROTOCOLCHAR(':');
        SERIAL_PROTOCOL_F(degHotend(cur_extruder), 1);
        SERIAL_PROTOCOLPGM("C->");
        SERIAL_PROTOCOL_F(rawHotendTemp(cur_extruder) / OVERSAMPLENR, 0);
      }
    #endif
  }
#endif

/**
 * M105: Read hot end and bed temperature
 */
inline void gcode_M105() {
  if (setTargetedHotend(105)) return;

  #if HAS_TEMP_0 || HAS_TEMP_BED || ENABLED(HEATER_0_USES_MAX6675)
    SERIAL_PROTOCOLPGM(MSG_OK);
    print_heaterstates();
  #else // !HAS_TEMP_0 && !HAS_TEMP_BED
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_EOL;
}

#if FAN_COUNT > 0

  /**
   * M106: Set Fan Speed
   *
   *  S<int>   Speed between 0-255
   *  P<index> Fan index, if more than one fan
   */
  inline void gcode_M106() {
    uint16_t s = code_seen('S') ? code_value_short() : 255,
             p = code_seen('P') ? code_value_short() : 0;
    NOMORE(s, 255);
    if (p < FAN_COUNT) fanSpeeds[p] = s;
  }

  /**
   * M107: Fan Off
   */
  inline void gcode_M107() {
    uint16_t p = code_seen('P') ? code_value_short() : 0;
    if (p < FAN_COUNT) fanSpeeds[p] = 0;
  }

#endif // FAN_COUNT > 0

/**
 * M109: Sxxx Wait for extruder(s) to reach temperature. Waits only when heating.
 *       Rxxx Wait for extruder(s) to reach temperature. Waits when heating and cooling.
 */
inline void gcode_M109() {
  bool no_wait_for_cooling = true;

  if (setTargetedHotend(109)) return;
  if (DEBUGGING(DRYRUN)) return;

  // Start hook must happen before setTargetHotend()
  print_job_start();

  no_wait_for_cooling = code_seen('S');
  if (no_wait_for_cooling || code_seen('R')) {
    float temp = code_value();
    setTargetHotend(temp, target_extruder);
    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        setTargetHotend1(temp == 0.0 ? 0.0 : temp + duplicate_extruder_temp_offset);
    #endif

    if (temp > degHotend(target_extruder)) LCD_MESSAGEPGM(MSG_HEATING);
  }

  if (print_job_stop()) LCD_MESSAGEPGM(WELCOME_MSG);

  #if ENABLED(AUTOTEMP)
    autotemp_enabled = code_seen('F');
    if (autotemp_enabled) autotemp_factor = code_value();
    if (code_seen('S')) autotemp_min = code_value();
    if (code_seen('B')) autotemp_max = code_value();
  #endif

  // Exit if the temperature is above target and not waiting for cooling
  if (no_wait_for_cooling && !isHeatingHotend(target_extruder)) return;

  // Prevents a wait-forever situation if R is misused i.e. M109 R0
  // Try to calculate a ballpark safe margin by halving EXTRUDE_MINTEMP
  if (degTargetHotend(target_extruder) < (EXTRUDE_MINTEMP/2)) return;

  #ifdef TEMP_RESIDENCY_TIME
    long residency_start_ms = -1;
    // Loop until the temperature has stabilized
    #define TEMP_CONDITIONS (residency_start_ms < 0 || now < residency_start_ms + (TEMP_RESIDENCY_TIME) * 1000UL)
  #else
    // Loop until the temperature is very close target
    #define TEMP_CONDITIONS (isHeatingHotend(target_extruder))
  #endif //TEMP_RESIDENCY_TIME

  cancel_heatup = false;
  millis_t now = millis(), next_temp_ms = now + 1000UL;
  while (!cancel_heatup && TEMP_CONDITIONS) {
    now = millis();
    if (now > next_temp_ms) { //Print temp & remaining time every 1s while waiting
      next_temp_ms = now + 1000UL;
      #if HAS_TEMP_0 || HAS_TEMP_BED || ENABLED(HEATER_0_USES_MAX6675)
        print_heaterstates();
      #endif
      #ifdef TEMP_RESIDENCY_TIME
        SERIAL_PROTOCOLPGM(" W:");
        if (residency_start_ms >= 0) {
          long rem = (((TEMP_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL;
          SERIAL_PROTOCOLLN(rem);
        }
        else {
          SERIAL_PROTOCOLLNPGM("?");
        }
      #else
        SERIAL_EOL;
      #endif
    }

    idle();
    refresh_cmd_timeout(); // to prevent stepper_inactive_time from running out

    #ifdef TEMP_RESIDENCY_TIME
      // Start the TEMP_RESIDENCY_TIME timer when we reach target temp for the first time.
      // Restart the timer whenever the temperature falls outside the hysteresis.
      if (labs(degHotend(target_extruder) - degTargetHotend(target_extruder)) > ((residency_start_ms < 0) ? TEMP_WINDOW : TEMP_HYSTERESIS))
        residency_start_ms = millis();
    #endif //TEMP_RESIDENCY_TIME

  } // while(!cancel_heatup && TEMP_CONDITIONS)

  LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
}

#if HAS_TEMP_BED

  /**
   * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
   *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
   */
  inline void gcode_M190() {
    if (DEBUGGING(DRYRUN)) return;

    LCD_MESSAGEPGM(MSG_BED_HEATING);
    bool no_wait_for_cooling = code_seen('S');
    if (no_wait_for_cooling || code_seen('R'))
      setTargetBed(code_value());

    // Exit if the temperature is above target and not waiting for cooling
    if (no_wait_for_cooling && !isHeatingBed()) return;

    cancel_heatup = false;
    millis_t now = millis(), next_temp_ms = now + 1000UL;
    while (!cancel_heatup && isHeatingBed()) {
      millis_t now = millis();
      if (now > next_temp_ms) { //Print Temp Reading every 1 second while heating up.
        next_temp_ms = now + 1000UL;
        print_heaterstates();
        SERIAL_EOL;
      }
      idle();
      refresh_cmd_timeout(); // to prevent stepper_inactive_time from running out
    }
    LCD_MESSAGEPGM(MSG_BED_DONE);
  }

#endif // HAS_TEMP_BED

/**
 * M110: Set Current Line Number
 */
inline void gcode_M110() {
  if (code_seen('N')) gcode_N = code_value_long();
}

/**
 * M111: Set the debug level
 */
inline void gcode_M111() {
  marlin_debug_flags = code_seen('S') ? code_value_short() : DEBUG_NONE;

  const char str_debug_1[] PROGMEM = MSG_DEBUG_ECHO;
  const char str_debug_2[] PROGMEM = MSG_DEBUG_INFO;
  const char str_debug_4[] PROGMEM = MSG_DEBUG_ERRORS;
  const char str_debug_8[] PROGMEM = MSG_DEBUG_DRYRUN;
  const char str_debug_16[] PROGMEM = MSG_DEBUG_COMMUNICATION;
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    const char str_debug_32[] PROGMEM = MSG_DEBUG_LEVELING;
  #endif

  const char* const debug_strings[] PROGMEM = {
    str_debug_1, str_debug_2, str_debug_4, str_debug_8, str_debug_16,
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      str_debug_32
    #endif
  };

  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_DEBUG_PREFIX);
  if (marlin_debug_flags) {
    uint8_t comma = 0;
    for (uint8_t i = 0; i < COUNT(debug_strings); i++) {
      if (TEST(marlin_debug_flags, i)) {
        if (comma++) SERIAL_CHAR('|');
        serialprintPGM(debug_strings[i]);
      }
    }
  }
  else {
    SERIAL_ECHOPGM(MSG_DEBUG_OFF);
  }
  SERIAL_EOL;
}

/**
 * M112: Emergency Stop
 */
inline void gcode_M112() { kill(PSTR(MSG_KILLED)); }

#if ENABLED(BARICUDA)

  #if HAS_HEATER_1
    /**
     * M126: Heater 1 valve open
     */
    inline void gcode_M126() { ValvePressure = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }
    /**
     * M127: Heater 1 valve close
     */
    inline void gcode_M127() { ValvePressure = 0; }
  #endif

  #if HAS_HEATER_2
    /**
     * M128: Heater 2 valve open
     */
    inline void gcode_M128() { EtoPPressure = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }
    /**
     * M129: Heater 2 valve close
     */
    inline void gcode_M129() { EtoPPressure = 0; }
  #endif

#endif //BARICUDA

/**
 * M140: Set bed temperature
 */
inline void gcode_M140() {
  if (DEBUGGING(DRYRUN)) return;
  if (code_seen('S')) setTargetBed(code_value());
}

#if ENABLED(ULTIPANEL)

  /**
   * M145: Set the heatup state for a material in the LCD menu
   *   S<material> (0=PLA, 1=ABS)
   *   H<hotend temp>
   *   B<bed temp>
   *   F<fan speed>
   */
  inline void gcode_M145() {
    int8_t material = code_seen('S') ? code_value_short() : 0;
    if (material < 0 || material > 1) {
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM(MSG_ERR_MATERIAL_INDEX);
    }
    else {
      int v;
      switch (material) {
        case 0:
          if (code_seen('H')) {
            v = code_value_short();
            plaPreheatHotendTemp = constrain(v, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - 15);
          }
          if (code_seen('F')) {
            v = code_value_short();
            plaPreheatFanSpeed = constrain(v, 0, 255);
          }
          #if TEMP_SENSOR_BED != 0
            if (code_seen('B')) {
              v = code_value_short();
              plaPreheatHPBTemp = constrain(v, BED_MINTEMP, BED_MAXTEMP - 15);
            }
          #endif
          break;
        case 1:
          if (code_seen('H')) {
            v = code_value_short();
            absPreheatHotendTemp = constrain(v, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - 15);
          }
          if (code_seen('F')) {
            v = code_value_short();
            absPreheatFanSpeed = constrain(v, 0, 255);
          }
          #if TEMP_SENSOR_BED != 0
            if (code_seen('B')) {
              v = code_value_short();
              absPreheatHPBTemp = constrain(v, BED_MINTEMP, BED_MAXTEMP - 15);
            }
          #endif
          break;
      }
    }
  }

#endif

#if HAS_POWER_SWITCH

  /**
   * M80: Turn on Power Supply
   */
  inline void gcode_M80() {
    OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE); //GND

    /**
     * If you have a switch on suicide pin, this is useful
     * if you want to start another print with suicide feature after
     * a print without suicide...
     */
    #if HAS_SUICIDE
      OUT_WRITE(SUICIDE_PIN, HIGH);
    #endif

    #if ENABLED(ULTIPANEL)
      powersupply = true;
      LCD_MESSAGEPGM(WELCOME_MSG);
      lcd_update();
    #endif
  }

#endif // HAS_POWER_SWITCH

/**
 * M81: Turn off Power, including Power Supply, if there is one.
 *
 *      This code should ALWAYS be available for EMERGENCY SHUTDOWN!
 */
inline void gcode_M81() {
  disable_all_heaters();
  finishAndDisableSteppers();
  #if FAN_COUNT > 0
    #if FAN_COUNT > 1
      for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
    #else
      fanSpeeds[0] = 0;
    #endif
  #endif
  delay(1000); // Wait 1 second before switching off
  #if HAS_SUICIDE
    st_synchronize();
    suicide();
  #elif HAS_POWER_SWITCH
    OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
  #endif
  #if ENABLED(ULTIPANEL)
    #if HAS_POWER_SWITCH
      powersupply = false;
    #endif
    LCD_MESSAGEPGM(MACHINE_NAME " " MSG_OFF ".");
    lcd_update();
  #endif
}


/**
 * M82: Set E codes absolute (default)
 */
inline void gcode_M82() { axis_relative_modes[E_AXIS] = false; }

/**
 * M83: Set E codes relative while in Absolute Coordinates (G90) mode
 */
inline void gcode_M83() { axis_relative_modes[E_AXIS] = true; }

/**
 * M18, M84: Disable all stepper motors
 */
inline void gcode_M18_M84() {
  if (code_seen('S')) {
    stepper_inactive_time = code_value() * 1000;
  }
  else {
    bool all_axis = !((code_seen(axis_codes[X_AXIS])) || (code_seen(axis_codes[Y_AXIS])) || (code_seen(axis_codes[Z_AXIS])) || (code_seen(axis_codes[E_AXIS])));
    if (all_axis) {
      finishAndDisableSteppers();
    }
    else {
      st_synchronize();
      if (code_seen('X')) disable_x();
      if (code_seen('Y')) disable_y();
      if (code_seen('Z')) disable_z();
      #if ((E0_ENABLE_PIN != X_ENABLE_PIN) && (E1_ENABLE_PIN != Y_ENABLE_PIN)) // Only enable on boards that have seperate ENABLE_PINS
        if (code_seen('E')) {
          disable_e0();
          disable_e1();
          disable_e2();
          disable_e3();
        }
      #endif
    }
  }
}

/**
 * M85: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
inline void gcode_M85() {
  if (code_seen('S')) max_inactive_time = code_value() * 1000;
}

/**
 * M92: Set axis steps-per-unit for one or more axes, X, Y, Z, and E.
 *      (Follows the same syntax as G92)
 */
inline void gcode_M92() {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      if (i == E_AXIS) {
        float value = code_value();
        if (value < 20.0) {
          float factor = axis_steps_per_unit[i] / value; // increase e constants if M92 E14 is given for netfab.
          max_e_jerk *= factor;
          max_feedrate[i] *= factor;
          axis_steps_per_sqr_second[i] *= factor;
        }
        axis_steps_per_unit[i] = value;
      }
      else {
        axis_steps_per_unit[i] = code_value();
      }
    }
  }
}

/**
 * M114: Output current position to serial port
 */
inline void gcode_M114() {
  SERIAL_PROTOCOLPGM("X:");
  SERIAL_PROTOCOL(current_position[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(current_position[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(current_position[Z_AXIS]);
  SERIAL_PROTOCOLPGM(" E:");
  SERIAL_PROTOCOL(current_position[E_AXIS]);

  CRITICAL_SECTION_START;
  extern volatile long count_position[NUM_AXIS];
  long xpos = count_position[X_AXIS],
       ypos = count_position[Y_AXIS],
       zpos = count_position[Z_AXIS];
  CRITICAL_SECTION_END;

  #if ENABLED(COREXY) || ENABLED(COREXZ)
    SERIAL_PROTOCOLPGM(MSG_COUNT_A);
  #else
    SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  #endif
  SERIAL_PROTOCOL(xpos);

  #if ENABLED(COREXY)
    SERIAL_PROTOCOLPGM(" B:");
  #else
    SERIAL_PROTOCOLPGM(" Y:");
  #endif
  SERIAL_PROTOCOL(ypos);

  #if ENABLED(COREXZ)
    SERIAL_PROTOCOLPGM(" C:");
  #else
    SERIAL_PROTOCOLPGM(" Z:");
  #endif
  SERIAL_PROTOCOL(zpos);

  SERIAL_EOL;

  #if ENABLED(SCARA)
    SERIAL_PROTOCOLPGM("SCARA Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL(delta[Y_AXIS]);
    SERIAL_EOL;

    SERIAL_PROTOCOLPGM("SCARA Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS] + home_offset[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta (90):");
    SERIAL_PROTOCOL(delta[Y_AXIS] - delta[X_AXIS] - 90 + home_offset[Y_AXIS]);
    SERIAL_EOL;

    SERIAL_PROTOCOLPGM("SCARA step Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS] / 90 * axis_steps_per_unit[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL((delta[Y_AXIS] - delta[X_AXIS]) / 90 * axis_steps_per_unit[Y_AXIS]);
    SERIAL_EOL; SERIAL_EOL;
  #endif
}

/**
 * M115: Capabilities string
 */
inline void gcode_M115() {
  SERIAL_PROTOCOLPGM(MSG_M115_REPORT);
}

/**
 * M117: Set LCD Status Message
 */
inline void gcode_M117() {
  lcd_setstatus(current_command_args);
}

/**
 * M119: Output endstop states to serial output
 */
inline void gcode_M119() {
  SERIAL_PROTOCOLLN(MSG_M119_REPORT);
  #if HAS_X_MIN
    SERIAL_PROTOCOLPGM(MSG_X_MIN);
    SERIAL_PROTOCOLLN(((READ(X_MIN_PIN)^X_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_X_MAX
    SERIAL_PROTOCOLPGM(MSG_X_MAX);
    SERIAL_PROTOCOLLN(((READ(X_MAX_PIN)^X_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MIN
    SERIAL_PROTOCOLPGM(MSG_Y_MIN);
    SERIAL_PROTOCOLLN(((READ(Y_MIN_PIN)^Y_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MAX
    SERIAL_PROTOCOLPGM(MSG_Y_MAX);
    SERIAL_PROTOCOLLN(((READ(Y_MAX_PIN)^Y_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MIN
    SERIAL_PROTOCOLPGM(MSG_Z_MIN);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PIN)^Z_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MAX
    SERIAL_PROTOCOLPGM(MSG_Z_MAX);
    SERIAL_PROTOCOLLN(((READ(Z_MAX_PIN)^Z_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z2_MAX
    SERIAL_PROTOCOLPGM(MSG_Z2_MAX);
    SERIAL_PROTOCOLLN(((READ(Z2_MAX_PIN)^Z2_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_PROBE
    SERIAL_PROTOCOLPGM(MSG_Z_PROBE);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PROBE_PIN)^Z_MIN_PROBE_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
}

/**
 * M120: Enable endstops and set non-homing endstop state to "enabled"
 */
inline void gcode_M120() { enable_endstops_globally(true); }

/**
 * M121: Disable endstops and set non-homing endstop state to "disabled"
 */
inline void gcode_M121() { enable_endstops_globally(false); }

#if ENABLED(BLINKM)

  /**
   * M150: Set Status LED Color - Use R-U-B for R-G-B
   */
  inline void gcode_M150() {
    SendColors(
      code_seen('R') ? (byte)code_value_short() : 0,
      code_seen('U') ? (byte)code_value_short() : 0,
      code_seen('B') ? (byte)code_value_short() : 0
    );
  }

#endif // BLINKM

/**
 * M200: Set filament diameter and set E axis units to cubic millimeters
 *
 *    T<extruder> - Optional extruder number. Current extruder if omitted.
 *    D<mm> - Diameter of the filament. Use "D0" to set units back to millimeters.
 */
inline void gcode_M200() {

  if (setTargetedHotend(200)) return;

  if (code_seen('D')) {
    float diameter = code_value();
    // setting any extruder filament size disables volumetric on the assumption that
    // slicers either generate in extruder values as cubic mm or as as filament feeds
    // for all extruders
    volumetric_enabled = (diameter != 0.0);
    if (volumetric_enabled) {
      filament_size[target_extruder] = diameter;
      // make sure all extruders have some sane value for the filament size
      for (int i = 0; i < EXTRUDERS; i++)
        if (! filament_size[i]) filament_size[i] = DEFAULT_NOMINAL_FILAMENT_DIA;
    }
  }
  else {
    //reserved for setting filament diameter via UFID or filament measuring device
    return;
  }
  calculate_volumetric_multipliers();
}

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 */
inline void gcode_M201() {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      max_acceleration_units_per_sq_second[i] = code_value();
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  reset_acceleration_rates();
}

#if 0 // Not used for Sprinter/grbl gen6
  inline void gcode_M202() {
    for (int8_t i = 0; i < NUM_AXIS; i++) {
      if (code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
    }
  }
#endif


/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
 */
inline void gcode_M203() {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      max_feedrate[i] = code_value();
    }
  }
}

/**
 * M204: Set Accelerations in mm/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 *
 *  Also sets minimum segment time in ms (B20000) to prevent buffer under-runs and M20 minimum feedrate
 */
inline void gcode_M204() {
  if (code_seen('S')) {  // Kept for legacy compatibility. Should NOT BE USED for new developments.
    travel_acceleration = acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Print and Travel Acceleration: ", acceleration);
    SERIAL_EOL;
  }
  if (code_seen('P')) {
    acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Print Acceleration: ", acceleration);
    SERIAL_EOL;
  }
  if (code_seen('R')) {
    retract_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Retract Acceleration: ", retract_acceleration);
    SERIAL_EOL;
  }
  if (code_seen('T')) {
    travel_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Travel Acceleration: ", travel_acceleration);
    SERIAL_EOL;
  }
}

/**
 * M205: Set Advanced Settings
 *
 *    S = Min Feed Rate (mm/s)
 *    T = Min Travel Feed Rate (mm/s)
 *    B = Min Segment Time (µs)
 *    X = Max XY Jerk (mm/s/s)
 *    Z = Max Z Jerk (mm/s/s)
 *    E = Max E Jerk (mm/s/s)
 */
inline void gcode_M205() {
  if (code_seen('S')) minimumfeedrate = code_value();
  if (code_seen('T')) mintravelfeedrate = code_value();
  if (code_seen('B')) minsegmenttime = code_value();
  if (code_seen('X')) max_xy_jerk = code_value();
  if (code_seen('Z')) max_z_jerk = code_value();
  if (code_seen('E')) max_e_jerk = code_value();
}

/**
 * M206: Set Additional Homing Offset (X Y Z). SCARA aliases T=X, P=Y
 */
inline void gcode_M206() {
  for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      home_offset[i] = code_value();
    }
  }
  #if ENABLED(SCARA)
    if (code_seen('T')) home_offset[X_AXIS] = code_value(); // Theta
    if (code_seen('P')) home_offset[Y_AXIS] = code_value(); // Psi
  #endif
}

#if ENABLED(DELTA)
  /**
   * M665: Set delta configurations
   *
   *    L = diagonal rod
   *    R = delta radius
   *    S = segments per second
   *    A = Alpha (Tower 1) diagonal rod trim
   *    B = Beta (Tower 2) diagonal rod trim
   *    C = Gamma (Tower 3) diagonal rod trim
   */
  inline void gcode_M665() {
    if (code_seen('L')) delta_diagonal_rod = code_value();
    if (code_seen('R')) delta_radius = code_value();
    if (code_seen('S')) delta_segments_per_second = code_value();
    if (code_seen('A')) delta_diagonal_rod_trim_tower_1 = code_value();
    if (code_seen('B')) delta_diagonal_rod_trim_tower_2 = code_value();
    if (code_seen('C')) delta_diagonal_rod_trim_tower_3 = code_value();
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  }
  /**
   * M666: Set delta endstop adjustment
   */
  inline void gcode_M666() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM(">>> gcode_M666");
      }
    #endif
    for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
      if (code_seen(axis_codes[i])) {
        endstop_adj[i] = code_value();
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPGM("endstop_adj[");
            SERIAL_ECHO(axis_codes[i]);
            SERIAL_ECHOPAIR("] = ", endstop_adj[i]);
            SERIAL_EOL;
          }
        #endif
      }
    }
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("<<< gcode_M666");
      }
    #endif
  }

#elif ENABLED(Z_DUAL_ENDSTOPS) // !DELTA && ENABLED(Z_DUAL_ENDSTOPS)

  /**
   * M666: For Z Dual Endstop setup, set z axis offset to the z2 axis.
   */
  inline void gcode_M666() {
    if (code_seen('Z')) z_endstop_adj = code_value();
    SERIAL_ECHOPAIR("Z Endstop Adjustment set to (mm):", z_endstop_adj);
    SERIAL_EOL;
  }

#endif // !DELTA && Z_DUAL_ENDSTOPS

#if ENABLED(FWRETRACT)

  /**
   * M207: Set firmware retraction values
   *
   *   S[+mm]    retract_length
   *   W[+mm]    retract_length_swap (multi-extruder)
   *   F[mm/min] retract_feedrate
   *   Z[mm]     retract_zlift
   */
  inline void gcode_M207() {
    if (code_seen('S')) retract_length = code_value();
    if (code_seen('F')) retract_feedrate = code_value() / 60;
    if (code_seen('Z')) retract_zlift = code_value();
    #if EXTRUDERS > 1
      if (code_seen('W')) retract_length_swap = code_value();
    #endif
  }

  /**
   * M208: Set firmware un-retraction values
   *
   *   S[+mm]    retract_recover_length (in addition to M207 S*)
   *   W[+mm]    retract_recover_length_swap (multi-extruder)
   *   F[mm/min] retract_recover_feedrate
   */
  inline void gcode_M208() {
    if (code_seen('S')) retract_recover_length = code_value();
    if (code_seen('F')) retract_recover_feedrate = code_value() / 60;
    #if EXTRUDERS > 1
      if (code_seen('W')) retract_recover_length_swap = code_value();
    #endif
  }

  /**
   * M209: Enable automatic retract (M209 S1)
   *       detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
   */
  inline void gcode_M209() {
    if (code_seen('S')) {
      int t = code_value_short();
      switch (t) {
        case 0:
          autoretract_enabled = false;
          break;
        case 1:
          autoretract_enabled = true;
          break;
        default:
          unknown_command_error();
          return;
      }
      for (int i = 0; i < EXTRUDERS; i++) retracted[i] = false;
    }
  }

#endif // FWRETRACT

#if EXTRUDERS > 1

  /**
   * M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
   */
  inline void gcode_M218() {
    if (setTargetedHotend(218)) return;

    if (code_seen('X')) extruder_offset[X_AXIS][target_extruder] = code_value();
    if (code_seen('Y')) extruder_offset[Y_AXIS][target_extruder] = code_value();

    #if ENABLED(DUAL_X_CARRIAGE)
      if (code_seen('Z')) extruder_offset[Z_AXIS][target_extruder] = code_value();
    #endif

    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    for (int e = 0; e < EXTRUDERS; e++) {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(extruder_offset[X_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO(extruder_offset[Y_AXIS][e]);
      #if ENABLED(DUAL_X_CARRIAGE)
        SERIAL_CHAR(',');
        SERIAL_ECHO(extruder_offset[Z_AXIS][e]);
      #endif
    }
    SERIAL_EOL;
  }

#endif // EXTRUDERS > 1

/**
 * M220: Set speed percentage factor, aka "Feed Rate" (M220 S95)
 */
inline void gcode_M220() {
  if (code_seen('S')) feedrate_multiplier = code_value();
}

/**
 * M221: Set extrusion percentage (M221 T0 S95)
 */
inline void gcode_M221() {
  if (code_seen('S')) {
    int sval = code_value();
    if (setTargetedHotend(221)) return;
    extruder_multiplier[target_extruder] = sval;
  }
}

/**
 * M226: Wait until the specified pin reaches the state required (M226 P<pin> S<state>)
 */
inline void gcode_M226() {
  if (code_seen('P')) {
    int pin_number = code_value();

    int pin_state = code_seen('S') ? code_value() : -1; // required pin state - default is inverted

    if (pin_state >= -1 && pin_state <= 1) {

      for (uint8_t i = 0; i < COUNT(sensitive_pins); i++) {
        if (sensitive_pins[i] == pin_number) {
          pin_number = -1;
          break;
        }
      }

      if (pin_number > -1) {
        int target = LOW;

        st_synchronize();

        pinMode(pin_number, INPUT);

        switch (pin_state) {
          case 1:
            target = HIGH;
            break;

          case 0:
            target = LOW;
            break;

          case -1:
            target = !digitalRead(pin_number);
            break;
        }

        while (digitalRead(pin_number) != target) idle();

      } // pin_number > -1
    } // pin_state -1 0 1
  } // code_seen('P')
}

#if HAS_SERVOS

  /**
   * M280: Get or set servo position. P<index> S<angle>
   */
  inline void gcode_M280() {
    int servo_index = code_seen('P') ? code_value_short() : -1;
    int servo_position = 0;
    if (code_seen('S')) {
      servo_position = code_value_short();
      if (servo_index >= 0 && servo_index < NUM_SERVOS)
        servo[servo_index].move(servo_position);
      else {
        SERIAL_ERROR_START;
        SERIAL_ERROR("Servo ");
        SERIAL_ERROR(servo_index);
        SERIAL_ERRORLN(" out of range");
      }
    }
    else if (servo_index >= 0) {
      SERIAL_ECHO_START;
      SERIAL_ECHO(" Servo ");
      SERIAL_ECHO(servo_index);
      SERIAL_ECHO(": ");
      SERIAL_ECHOLN(servo[servo_index].read());
    }
  }

#endif // HAS_SERVOS

#if HAS_BUZZER

  /**
   * M300: Play beep sound S<frequency Hz> P<duration ms>
   */
  inline void gcode_M300() {
    uint16_t beepS = code_seen('S') ? code_value_short() : 110;
    uint32_t beepP = code_seen('P') ? code_value_long() : 1000;
    if (beepP > 5000) beepP = 5000; // limit to 5 seconds
    buzz(beepP, beepS);
  }

#endif // HAS_BUZZER

#if ENABLED(PIDTEMP)

  /**
   * M301: Set PID parameters P I D (and optionally C, L)
   *
   *   P[float] Kp term
   *   I[float] Ki term (unscaled)
   *   D[float] Kd term (unscaled)
   *
   * With PID_ADD_EXTRUSION_RATE:
   *
   *   C[float] Kc term
   *   L[float] LPQ length
   */
  inline void gcode_M301() {

    // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
    // default behaviour (omitting E parameter) is to update for extruder 0 only
    int e = code_seen('E') ? code_value() : 0; // extruder being updated

    if (e < EXTRUDERS) { // catch bad input value
      if (code_seen('P')) PID_PARAM(Kp, e) = code_value();
      if (code_seen('I')) PID_PARAM(Ki, e) = scalePID_i(code_value());
      if (code_seen('D')) PID_PARAM(Kd, e) = scalePID_d(code_value());
      #if ENABLED(PID_ADD_EXTRUSION_RATE)
        if (code_seen('C')) PID_PARAM(Kc, e) = code_value();
        if (code_seen('L')) lpq_len = code_value();
        NOMORE(lpq_len, LPQ_MAX_LEN);
      #endif

      updatePID();
      SERIAL_ECHO_START;
      #if ENABLED(PID_PARAMS_PER_EXTRUDER)
        SERIAL_ECHO(" e:"); // specify extruder in serial output
        SERIAL_ECHO(e);
      #endif // PID_PARAMS_PER_EXTRUDER
      SERIAL_ECHO(" p:");
      SERIAL_ECHO(PID_PARAM(Kp, e));
      SERIAL_ECHO(" i:");
      SERIAL_ECHO(unscalePID_i(PID_PARAM(Ki, e)));
      SERIAL_ECHO(" d:");
      SERIAL_ECHO(unscalePID_d(PID_PARAM(Kd, e)));
      #if ENABLED(PID_ADD_EXTRUSION_RATE)
        SERIAL_ECHO(" c:");
        //Kc does not have scaling applied above, or in resetting defaults
        SERIAL_ECHO(PID_PARAM(Kc, e));
      #endif
      SERIAL_EOL;
    }
    else {
      SERIAL_ERROR_START;
      SERIAL_ERRORLN(MSG_INVALID_EXTRUDER);
    }
  }

#endif // PIDTEMP

#if ENABLED(PIDTEMPBED)

  inline void gcode_M304() {
    if (code_seen('P')) bedKp = code_value();
    if (code_seen('I')) bedKi = scalePID_i(code_value());
    if (code_seen('D')) bedKd = scalePID_d(code_value());

    updatePID();
    SERIAL_ECHO_START;
    SERIAL_ECHO(" p:");
    SERIAL_ECHO(bedKp);
    SERIAL_ECHO(" i:");
    SERIAL_ECHO(unscalePID_i(bedKi));
    SERIAL_ECHO(" d:");
    SERIAL_ECHOLN(unscalePID_d(bedKd));
  }

#endif // PIDTEMPBED

#if defined(CHDK) || HAS_PHOTOGRAPH

  /**
   * M240: Trigger a camera by emulating a Canon RC-1
   *       See http://www.doc-diy.net/photo/rc-1_hacked/
   */
  inline void gcode_M240() {
    #ifdef CHDK

      OUT_WRITE(CHDK, HIGH);
      chdkHigh = millis();
      chdkActive = true;

    #elif HAS_PHOTOGRAPH

      const uint8_t NUM_PULSES = 16;
      const float PULSE_LENGTH = 0.01524;
      for (int i = 0; i < NUM_PULSES; i++) {
        WRITE(PHOTOGRAPH_PIN, HIGH);
        _delay_ms(PULSE_LENGTH);
        WRITE(PHOTOGRAPH_PIN, LOW);
        _delay_ms(PULSE_LENGTH);
      }
      delay(7.33);
      for (int i = 0; i < NUM_PULSES; i++) {
        WRITE(PHOTOGRAPH_PIN, HIGH);
        _delay_ms(PULSE_LENGTH);
        WRITE(PHOTOGRAPH_PIN, LOW);
        _delay_ms(PULSE_LENGTH);
      }

    #endif // !CHDK && HAS_PHOTOGRAPH
  }

#endif // CHDK || PHOTOGRAPH_PIN

#if ENABLED(HAS_LCD_CONTRAST)

  /**
   * M250: Read and optionally set the LCD contrast
   */
  inline void gcode_M250() {
    if (code_seen('C')) lcd_setcontrast(code_value_short() & 0x3F);
    SERIAL_PROTOCOLPGM("lcd contrast value: ");
    SERIAL_PROTOCOL(lcd_contrast);
    SERIAL_EOL;
  }

#endif // HAS_LCD_CONTRAST

#if ENABLED(PREVENT_DANGEROUS_EXTRUDE)

  void set_extrude_min_temp(float temp) { extrude_min_temp = temp; }

  /**
   * M302: Allow cold extrudes, or set the minimum extrude S<temperature>.
   */
  inline void gcode_M302() {
    set_extrude_min_temp(code_seen('S') ? code_value() : 0);
  }

#endif // PREVENT_DANGEROUS_EXTRUDE

/**
 * M303: PID relay autotune
 *
 *       S<temperature> sets the target temperature. (default 150C)
 *       E<extruder> (-1 for the bed) (default 0)
 *       C<cycles>
 *       U<bool> with a non-zero value will apply the result to current settings
 */
inline void gcode_M303() {
  int e = code_seen('E') ? code_value_short() : 0;
  int c = code_seen('C') ? code_value_short() : 5;
  bool u = code_seen('U') && code_value_short() != 0;

  float temp = code_seen('S') ? code_value() : (e < 0 ? 70.0 : 150.0);

  if (e >= 0 && e < EXTRUDERS)
    target_extruder = e;

  KEEPALIVE_STATE(NOT_BUSY); // don't send "busy: processing" messages during autotune output

  PID_autotune(temp, e, c, u);

  KEEPALIVE_STATE(IN_HANDLER);
}

#if ENABLED(SCARA)
  bool SCARA_move_to_cal(uint8_t delta_x, uint8_t delta_y) {
    //SoftEndsEnabled = false;              // Ignore soft endstops during calibration
    //SERIAL_ECHOLN(" Soft endstops disabled ");
    if (IsRunning()) {
      //gcode_get_destination(); // For X Y Z E F
      delta[X_AXIS] = delta_x;
      delta[Y_AXIS] = delta_y;
      calculate_SCARA_forward_Transform(delta);
      destination[X_AXIS] = delta[X_AXIS] / axis_scaling[X_AXIS];
      destination[Y_AXIS] = delta[Y_AXIS] / axis_scaling[Y_AXIS];
      prepare_move();
      //ok_to_send();
      return true;
    }
    return false;
  }

  /**
   * M360: SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
   */
  inline bool gcode_M360() {
    SERIAL_ECHOLN(" Cal: Theta 0 ");
    return SCARA_move_to_cal(0, 120);
  }

  /**
   * M361: SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M361() {
    SERIAL_ECHOLN(" Cal: Theta 90 ");
    return SCARA_move_to_cal(90, 130);
  }

  /**
   * M362: SCARA calibration: Move to cal-position PsiA (0 deg calibration)
   */
  inline bool gcode_M362() {
    SERIAL_ECHOLN(" Cal: Psi 0 ");
    return SCARA_move_to_cal(60, 180);
  }

  /**
   * M363: SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M363() {
    SERIAL_ECHOLN(" Cal: Psi 90 ");
    return SCARA_move_to_cal(50, 90);
  }

  /**
   * M364: SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
   */
  inline bool gcode_M364() {
    SERIAL_ECHOLN(" Cal: Theta-Psi 90 ");
    return SCARA_move_to_cal(45, 135);
  }

  /**
   * M365: SCARA calibration: Scaling factor, X, Y, Z axis
   */
  inline void gcode_M365() {
    for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
      if (code_seen(axis_codes[i])) {
        axis_scaling[i] = code_value();
      }
    }
  }

#endif // SCARA

#if ENABLED(EXT_SOLENOID)

  void enable_solenoid(uint8_t num) {
    switch (num) {
      case 0:
        OUT_WRITE(SOL0_PIN, HIGH);
        break;
        #if HAS_SOLENOID_1
          case 1:
            OUT_WRITE(SOL1_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_2
          case 2:
            OUT_WRITE(SOL2_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_3
          case 3:
            OUT_WRITE(SOL3_PIN, HIGH);
            break;
        #endif
      default:
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_INVALID_SOLENOID);
        break;
    }
  }

  void enable_solenoid_on_active_extruder() { enable_solenoid(active_extruder); }

  void disable_all_solenoids() {
    OUT_WRITE(SOL0_PIN, LOW);
    OUT_WRITE(SOL1_PIN, LOW);
    OUT_WRITE(SOL2_PIN, LOW);
    OUT_WRITE(SOL3_PIN, LOW);
  }

  /**
   * M380: Enable solenoid on the active extruder
   */
  inline void gcode_M380() { enable_solenoid_on_active_extruder(); }

  /**
   * M381: Disable all solenoids
   */
  inline void gcode_M381() { disable_all_solenoids(); }

#endif // EXT_SOLENOID

/**
 * M400: Finish all moves
 */
inline void gcode_M400() { st_synchronize(); }

#if ENABLED(AUTO_BED_LEVELING_FEATURE) && DISABLED(Z_PROBE_SLED) && (HAS_SERVO_ENDSTOPS || ENABLED(Z_PROBE_ALLEN_KEY))

  /**
   * M401: Engage Z Servo endstop if available
   */
  inline void gcode_M401() {
    #if HAS_SERVO_ENDSTOPS
      raise_z_for_servo();
    #endif
    deploy_z_probe();
  }

  /**
   * M402: Retract Z Servo endstop if enabled
   */
  inline void gcode_M402() {
    #if HAS_SERVO_ENDSTOPS
      raise_z_for_servo();
    #endif
    stow_z_probe(false);
  }

#endif // AUTO_BED_LEVELING_FEATURE && (HAS_SERVO_ENDSTOPS || Z_PROBE_ALLEN_KEY) && !Z_PROBE_SLED

#if ENABLED(FILAMENT_WIDTH_SENSOR)

  /**
   * M404: Display or set the nominal filament width (3mm, 1.75mm ) W<3.0>
   */
  inline void gcode_M404() {
    if (code_seen('W')) {
      filament_width_nominal = code_value();
    }
    else {
      SERIAL_PROTOCOLPGM("Filament dia (nominal mm):");
      SERIAL_PROTOCOLLN(filament_width_nominal);
    }
  }

  /**
   * M405: Turn on filament sensor for control
   */
  inline void gcode_M405() {
    if (code_seen('D')) meas_delay_cm = code_value();
    NOMORE(meas_delay_cm, MAX_MEASUREMENT_DELAY);

    if (delay_index2 == -1) { //initialize the ring buffer if it has not been done since startup
      int temp_ratio = widthFil_to_size_ratio();

      for (delay_index1 = 0; delay_index1 < COUNT(measurement_delay); ++delay_index1)
        measurement_delay[delay_index1] = temp_ratio - 100;  //subtract 100 to scale within a signed byte

      delay_index1 = delay_index2 = 0;
    }

    filament_sensor = true;

    //SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
    //SERIAL_PROTOCOL(filament_width_meas);
    //SERIAL_PROTOCOLPGM("Extrusion ratio(%):");
    //SERIAL_PROTOCOL(extruder_multiplier[active_extruder]);
  }

  /**
   * M406: Turn off filament sensor for control
   */
  inline void gcode_M406() { filament_sensor = false; }

  /**
   * M407: Get measured filament diameter on serial output
   */
  inline void gcode_M407() {
    SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
    SERIAL_PROTOCOLLN(filament_width_meas);
  }

#endif // FILAMENT_WIDTH_SENSOR

/**
 * M410: Quickstop - Abort all planned moves
 *
 * This will stop the carriages mid-move, so most likely they
 * will be out of sync with the stepper position after this.
 */
inline void gcode_M410() { quickStop(); }


#if ENABLED(MESH_BED_LEVELING)

  /**
   * M420: Enable/Disable Mesh Bed Leveling
   */
  inline void gcode_M420() { if (code_seen('S') && code_has_value()) mbl.active = !!code_value_short(); }

  /**
   * M421: Set a single Mesh Bed Leveling Z coordinate
   */
  inline void gcode_M421() {
    float x, y, z;
    bool err = false, hasX, hasY, hasZ;
    if ((hasX = code_seen('X'))) x = code_value();
    if ((hasY = code_seen('Y'))) y = code_value();
    if ((hasZ = code_seen('Z'))) z = code_value();

    if (!hasX || !hasY || !hasZ) {
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM(MSG_ERR_M421_REQUIRES_XYZ);
      err = true;
    }

    if (x >= MESH_NUM_X_POINTS || y >= MESH_NUM_Y_POINTS) {
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM(MSG_ERR_MESH_INDEX_OOB);
      err = true;
    }

    if (!err) mbl.set_z(mbl.select_x_index(x), mbl.select_y_index(y), z);
  }

#endif

/**
 * M428: Set home_offset based on the distance between the
 *       current_position and the nearest "reference point."
 *       If an axis is past center its endstop position
 *       is the reference-point. Otherwise it uses 0. This allows
 *       the Z offset to be set near the bed when using a max endstop.
 *
 *       M428 can't be used more than 2cm away from 0 or an endstop.
 *
 *       Use M206 to set these values directly.
 */
inline void gcode_M428() {
  bool err = false;
  float new_offs[3], new_pos[3];
  memcpy(new_pos, current_position, sizeof(new_pos));
  memcpy(new_offs, home_offset, sizeof(new_offs));
  for (int8_t i = X_AXIS; i <= Z_AXIS; i++) {
    if (axis_homed[i]) {
      float base = (new_pos[i] > (min_pos[i] + max_pos[i]) / 2) ? base_home_pos(i) : 0,
            diff = new_pos[i] - base;
      if (diff > -20 && diff < 20) {
        new_offs[i] -= diff;
        new_pos[i] = base;
      }
      else {
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM(MSG_ERR_M428_TOO_FAR);
        LCD_ALERTMESSAGEPGM("Err: Too far!");
        #if HAS_BUZZER
          buzz(200, 40);
        #endif
        err = true;
        break;
      }
    }
  }

  if (!err) {
    memcpy(current_position, new_pos, sizeof(new_pos));
    memcpy(home_offset, new_offs, sizeof(new_offs));
    sync_plan_position();
    LCD_ALERTMESSAGEPGM("Offset applied.");
    #if HAS_BUZZER
      buzz(200, 659);
      buzz(200, 698);
    #endif
  }
}

/**
 * M500: Store settings in EEPROM
 */
inline void gcode_M500() {
  Config_StoreSettings();
}

/**
 * M501: Read settings from EEPROM
 */
inline void gcode_M501() {
  Config_RetrieveSettings();
}

/**
 * M502: Revert to default settings
 */
inline void gcode_M502() {
  Config_ResetDefault();
}

/**
 * M503: print settings currently in memory
 */
inline void gcode_M503() {
  Config_PrintSettings(code_seen('S') && code_value() == 0);
}

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)

  /**
   * M540: Set whether SD card print should abort on endstop hit (M540 S<0|1>)
   */
  inline void gcode_M540() {
    if (code_seen('S')) abort_on_endstop_hit = (code_value() > 0);
  }

#endif // ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

#ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

  inline void gcode_SET_Z_PROBE_OFFSET() {

    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_ZPROBE_ZOFFSET);
    SERIAL_CHAR(' ');

    if (code_seen('Z')) {
      float value = code_value();
      if (Z_PROBE_OFFSET_RANGE_MIN <= value && value <= Z_PROBE_OFFSET_RANGE_MAX) {
        zprobe_zoffset = value;
        SERIAL_ECHO(zprobe_zoffset);
      }
      else {
        SERIAL_ECHOPGM(MSG_Z_MIN);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MIN);
        SERIAL_ECHOPGM(MSG_Z_MAX);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MAX);
      }
    }
    else {
      SERIAL_ECHOPAIR(": ", zprobe_zoffset);
    }

    SERIAL_EOL;
  }

#endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

#if ENABLED(FILAMENTCHANGEENABLE)

  /**
   * M600: Pause for filament change
   *
   *  E[distance] - Retract the filament this far (negative value)
   *  Z[distance] - Move the Z axis by this distance
   *  X[position] - Move to this X position, with Y
   *  Y[position] - Move to this Y position, with X
   *  L[distance] - Retract distance for removal (manual reload)
   *
   *  Default values are used for omitted arguments.
   *
   */
  inline void gcode_M600() {

    if (degHotend(active_extruder) < extrude_min_temp) {
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM(MSG_TOO_COLD_FOR_M600);
      return;
    }

    float lastpos[NUM_AXIS], fr60 = feedrate / 60;

    for (int i = 0; i < NUM_AXIS; i++)
      lastpos[i] = destination[i] = current_position[i];

    #if ENABLED(DELTA)
      #define RUNPLAN calculate_delta(destination); \
                      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], fr60, active_extruder);
    #else
      #define RUNPLAN line_to_destination();
    #endif

    //retract by E
    if (code_seen('E')) destination[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FIRSTRETRACT
      else destination[E_AXIS] += FILAMENTCHANGE_FIRSTRETRACT;
    #endif

    RUNPLAN;

    //lift Z
    if (code_seen('Z')) destination[Z_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_ZADD
      else destination[Z_AXIS] += FILAMENTCHANGE_ZADD;
    #endif

    RUNPLAN;

    //move xy
    if (code_seen('X')) destination[X_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_XPOS
      else destination[X_AXIS] = FILAMENTCHANGE_XPOS;
    #endif

    if (code_seen('Y')) destination[Y_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_YPOS
      else destination[Y_AXIS] = FILAMENTCHANGE_YPOS;
    #endif

    RUNPLAN;

    if (code_seen('L')) destination[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else destination[E_AXIS] += FILAMENTCHANGE_FINALRETRACT;
    #endif

    RUNPLAN;

    //finish moves
    st_synchronize();
    //disable extruder steppers so filament can be removed
    disable_e0();
    disable_e1();
    disable_e2();
    disable_e3();
    delay(100);
    LCD_ALERTMESSAGEPGM(MSG_FILAMENTCHANGE);
    millis_t next_tick = 0;
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    while (!lcd_clicked()) {
      #if DISABLED(AUTO_FILAMENT_CHANGE)
        millis_t ms = millis();
        if (ms >= next_tick) {
          lcd_quick_feedback();
          next_tick = ms + 2500; // feedback every 2.5s while waiting
        }
        idle(true);
      #else
        current_position[E_AXIS] += AUTO_FILAMENT_CHANGE_LENGTH;
        destination[E_AXIS] = current_position[E_AXIS];
        line_to_destination(AUTO_FILAMENT_CHANGE_FEEDRATE);
        st_synchronize();
      #endif
    } // while(!lcd_clicked)
    KEEPALIVE_STATE(IN_HANDLER);
    lcd_quick_feedback(); // click sound feedback

    #if ENABLED(AUTO_FILAMENT_CHANGE)
      current_position[E_AXIS] = 0;
      st_synchronize();
    #endif

    //return to normal
    if (code_seen('L')) destination[E_AXIS] -= code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else destination[E_AXIS] -= FILAMENTCHANGE_FINALRETRACT;
    #endif

    current_position[E_AXIS] = destination[E_AXIS]; //the long retract of L is compensated by manual filament feeding
    plan_set_e_position(current_position[E_AXIS]);

    RUNPLAN; //should do nothing

    lcd_reset_alert_level();

    #if ENABLED(DELTA)
      // Move XYZ to starting position, then E
      calculate_delta(lastpos);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], fr60, active_extruder);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], lastpos[E_AXIS], fr60, active_extruder);
    #else
      // Move XY to starting position, then Z, then E
      destination[X_AXIS] = lastpos[X_AXIS];
      destination[Y_AXIS] = lastpos[Y_AXIS];
      line_to_destination();
      destination[Z_AXIS] = lastpos[Z_AXIS];
      line_to_destination();
      destination[E_AXIS] = lastpos[E_AXIS];
      line_to_destination();
    #endif

    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
      filrunoutEnqueued = false;
    #endif

  }

#endif // FILAMENTCHANGEENABLE

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * M605: Set dual x-carriage movement mode
   *
   *    M605 S0: Full control mode. The slicer has full control over x-carriage movement
   *    M605 S1: Auto-park mode. The inactive head will auto park/unpark without slicer involvement
   *    M605 S2 [Xnnn] [Rmmm]: Duplication mode. The second extruder will duplicate the first with nnn
   *                         millimeters x-offset and an optional differential hotend temperature of
   *                         mmm degrees. E.g., with "M605 S2 X100 R2" the second extruder will duplicate
   *                         the first with a spacing of 100mm in the x direction and 2 degrees hotter.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  inline void gcode_M605() {
    st_synchronize();
    if (code_seen('S')) dual_x_carriage_mode = code_value();
    switch (dual_x_carriage_mode) {
      case DXC_DUPLICATION_MODE:
        if (code_seen('X')) duplicate_extruder_x_offset = max(code_value(), X2_MIN_POS - x_home_pos(0));
        if (code_seen('R')) duplicate_extruder_temp_offset = code_value();
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(extruder_offset[X_AXIS][0]);
        SERIAL_CHAR(',');
        SERIAL_ECHO(extruder_offset[Y_AXIS][0]);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(duplicate_extruder_x_offset);
        SERIAL_CHAR(',');
        SERIAL_ECHOLN(extruder_offset[Y_AXIS][1]);
        break;
      case DXC_FULL_CONTROL_MODE:
      case DXC_AUTO_PARK_MODE:
        break;
      default:
        dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
        break;
    }
    active_extruder_parked = false;
    extruder_duplication_enabled = false;
    delayed_move_time = 0;
  }

#endif // DUAL_X_CARRIAGE

/**
 * M907: Set digital trimpot motor current using axis codes X, Y, Z, E, B, S
 */
inline void gcode_M907() {
  #if HAS_DIGIPOTSS
    for (int i = 0; i < NUM_AXIS; i++)
      if (code_seen(axis_codes[i])) digipot_current(i, code_value());
    if (code_seen('B')) digipot_current(4, code_value());
    if (code_seen('S')) for (int i = 0; i <= 4; i++) digipot_current(i, code_value());
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
    if (code_seen('X')) digipot_current(0, code_value());
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
    if (code_seen('Z')) digipot_current(1, code_value());
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
    if (code_seen('E')) digipot_current(2, code_value());
  #endif
  #if ENABLED(DIGIPOT_I2C)
    // this one uses actual amps in floating point
    for (int i = 0; i < NUM_AXIS; i++) if (code_seen(axis_codes[i])) digipot_i2c_set_current(i, code_value());
    // for each additional extruder (named B,C,D,E..., channels 4,5,6,7...)
    for (int i = NUM_AXIS; i < DIGIPOT_I2C_NUM_CHANNELS; i++) if (code_seen('B' + i - (NUM_AXIS))) digipot_i2c_set_current(i, code_value());
  #endif
  #if ENABLED(DAC_STEPPER_CURRENT)
    if (code_seen('S')) {
      float dac_percent = code_value();
      for (uint8_t i = 0; i <= 4; i++) dac_current_percent(i, dac_percent);
    }
    for (uint8_t i = 0; i < NUM_AXIS; i++) if (code_seen(axis_codes[i])) dac_current_percent(i, code_value());
  #endif
}

#if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)

  /**
   * M908: Control digital trimpot directly (M908 P<pin> S<current>)
   */
  inline void gcode_M908() {
    #if HAS_DIGIPOTSS
      digitalPotWrite(
        code_seen('P') ? code_value() : 0,
        code_seen('S') ? code_value() : 0
      );
    #endif
    #ifdef DAC_STEPPER_CURRENT
      dac_current_raw(
        code_seen('P') ? code_value_long() : -1,
        code_seen('S') ? code_value_short() : 0
      );
    #endif
  }

  #if ENABLED(DAC_STEPPER_CURRENT) // As with Printrbot RevF

    inline void gcode_M909() { dac_print_values(); }

    inline void gcode_M910() { dac_commit_eeprom(); }

  #endif

#endif // HAS_DIGIPOTSS || DAC_STEPPER_CURRENT

#if HAS_MICROSTEPS

  // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
  inline void gcode_M350() {
    if (code_seen('S')) for (int i = 0; i <= 4; i++) microstep_mode(i, code_value());
    for (int i = 0; i < NUM_AXIS; i++) if (code_seen(axis_codes[i])) microstep_mode(i, (uint8_t)code_value());
    if (code_seen('B')) microstep_mode(4, code_value());
    microstep_readings();
  }

  /**
   * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
   *       S# determines MS1 or MS2, X# sets the pin high/low.
   */
  inline void gcode_M351() {
    if (code_seen('S')) switch (code_value_short()) {
      case 1:
        for (int i = 0; i < NUM_AXIS; i++) if (code_seen(axis_codes[i])) microstep_ms(i, code_value(), -1);
        if (code_seen('B')) microstep_ms(4, code_value(), -1);
        break;
      case 2:
        for (int i = 0; i < NUM_AXIS; i++) if (code_seen(axis_codes[i])) microstep_ms(i, -1, code_value());
        if (code_seen('B')) microstep_ms(4, -1, code_value());
        break;
    }
    microstep_readings();
  }

#endif // HAS_MICROSTEPS

/**
 * M999: Restart after being stopped
 */
inline void gcode_M999() {
  Running = true;
  lcd_reset_alert_level();
  // gcode_LastN = Stopped_gcode_LastN;
  FlushSerialRequestResend();
}

/**
 * T0-T3: Switch tool, usually switching extruders
 *
 *   F[mm/min] Set the movement feedrate
 */
inline void gcode_T(uint8_t tmp_extruder) {
  if (tmp_extruder >= EXTRUDERS) {
    SERIAL_ECHO_START;
    SERIAL_CHAR('T');
    SERIAL_PROTOCOL_F(tmp_extruder, DEC);
    SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
  }
  else {
    target_extruder = tmp_extruder;

    #if EXTRUDERS > 1
      bool make_move = false;
    #endif

    if (code_seen('F')) {

      #if EXTRUDERS > 1
        make_move = true;
      #endif

      float next_feedrate = code_value();
      if (next_feedrate > 0.0) feedrate = next_feedrate;
    }
    #if EXTRUDERS > 1
      if (tmp_extruder != active_extruder) {
        // Save current position to return to after applying extruder offset
        set_destination_to_current();
        #if ENABLED(DUAL_X_CARRIAGE)
          if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE && IsRunning() &&
              (delayed_move_time != 0 || current_position[X_AXIS] != x_home_pos(active_extruder))) {
            // Park old head: 1) raise 2) move to park position 3) lower
            plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT,
                             current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
            plan_buffer_line(x_home_pos(active_extruder), current_position[Y_AXIS], current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT,
                             current_position[E_AXIS], max_feedrate[X_AXIS], active_extruder);
            plan_buffer_line(x_home_pos(active_extruder), current_position[Y_AXIS], current_position[Z_AXIS],
                             current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
            st_synchronize();
          }

          // apply Y & Z extruder offset (x offset is already used in determining home pos)
          current_position[Y_AXIS] -= extruder_offset[Y_AXIS][active_extruder] - extruder_offset[Y_AXIS][tmp_extruder];
          current_position[Z_AXIS] -= extruder_offset[Z_AXIS][active_extruder] - extruder_offset[Z_AXIS][tmp_extruder];
          active_extruder = tmp_extruder;

          // This function resets the max/min values - the current position may be overwritten below.
          set_axis_is_at_home(X_AXIS);

          if (dual_x_carriage_mode == DXC_FULL_CONTROL_MODE) {
            current_position[X_AXIS] = inactive_extruder_x_pos;
            inactive_extruder_x_pos = destination[X_AXIS];
          }
          else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
            active_extruder_parked = (active_extruder == 0); // this triggers the second extruder to move into the duplication position
            if (active_extruder == 0 || active_extruder_parked)
              current_position[X_AXIS] = inactive_extruder_x_pos;
            else
              current_position[X_AXIS] = destination[X_AXIS] + duplicate_extruder_x_offset;
            inactive_extruder_x_pos = destination[X_AXIS];
            extruder_duplication_enabled = false;
          }
          else {
            // record raised toolhead position for use by unpark
            memcpy(raised_parked_position, current_position, sizeof(raised_parked_position));
            raised_parked_position[Z_AXIS] += TOOLCHANGE_UNPARK_ZLIFT;
            active_extruder_parked = true;
            delayed_move_time = 0;
          }
        #else // !DUAL_X_CARRIAGE
          #if ENABLED(AUTO_BED_LEVELING_FEATURE)
            // Offset extruder, make sure to apply the bed level rotation matrix
            vector_3 tmp_offset_vec = vector_3(extruder_offset[X_AXIS][tmp_extruder],
                                               extruder_offset[Y_AXIS][tmp_extruder],
                                               extruder_offset[Z_AXIS][tmp_extruder]),
                     act_offset_vec = vector_3(extruder_offset[X_AXIS][active_extruder],
                                               extruder_offset[Y_AXIS][active_extruder],
                                               extruder_offset[Z_AXIS][active_extruder]),
                     offset_vec = tmp_offset_vec - act_offset_vec;
            offset_vec.apply_rotation(plan_bed_level_matrix.transpose(plan_bed_level_matrix));
            current_position[X_AXIS] += offset_vec.x;
            current_position[Y_AXIS] += offset_vec.y;
            current_position[Z_AXIS] += offset_vec.z;
          #else // !AUTO_BED_LEVELING_FEATURE
            // Offset extruder (only by XY)
            for (int i=X_AXIS; i<=Y_AXIS; i++)
              current_position[i] += extruder_offset[i][tmp_extruder] - extruder_offset[i][active_extruder];
          #endif // !AUTO_BED_LEVELING_FEATURE
          // Set the new active extruder and position
          active_extruder = tmp_extruder;
        #endif // !DUAL_X_CARRIAGE
        #if ENABLED(DELTA)
          sync_plan_position_delta();
        #else
          sync_plan_position();
        #endif
        // Move to the old position if 'F' was in the parameters
        if (make_move && IsRunning()) prepare_move();
      }

      #if ENABLED(EXT_SOLENOID)
        st_synchronize();
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif // EXT_SOLENOID

    #endif // EXTRUDERS > 1
    SERIAL_ECHO_START;
    SERIAL_ECHO(MSG_ACTIVE_EXTRUDER);
    SERIAL_PROTOCOLLN((int)active_extruder);
  }
}

/**
 * Process a single command and dispatch it to its handler
 * This is called from the main loop()
 */
void process_next_command() {
  current_command = command_queue[cmd_queue_index_r];

  if (DEBUGGING(ECHO)) {
    SERIAL_ECHO_START;
    SERIAL_ECHOLN(current_command);
  }

  // Sanitize the current command:
  //  - Skip leading spaces
  //  - Bypass N[-0-9][0-9]*[ ]*
  //  - Overwrite * with nul to mark the end
  while (*current_command == ' ') ++current_command;
  if (*current_command == 'N' && NUMERIC_SIGNED(current_command[1])) {
    current_command += 2; // skip N[-0-9]
    while (NUMERIC(*current_command)) ++current_command; // skip [0-9]*
    while (*current_command == ' ') ++current_command; // skip [ ]*
  }
  char* starpos = strchr(current_command, '*');  // * should always be the last parameter
  if (starpos) while (*starpos == ' ' || *starpos == '*') *starpos-- = '\0'; // nullify '*' and ' '

  char *cmd_ptr = current_command;

  // Get the command code, which must be G, M, or T
  char command_code = *cmd_ptr++;

  // Skip spaces to get the numeric part
  while (*cmd_ptr == ' ') cmd_ptr++;

  uint16_t codenum = 0; // define ahead of goto

  // Bail early if there's no code
  bool code_is_good = NUMERIC(*cmd_ptr);
  if (!code_is_good) goto ExitUnknownCommand;

  // Get and skip the code number
  do {
    codenum = (codenum * 10) + (*cmd_ptr - '0');
    cmd_ptr++;
  } while (NUMERIC(*cmd_ptr));

  // Skip all spaces to get to the first argument, or nul
  while (*cmd_ptr == ' ') cmd_ptr++;

  // The command's arguments (if any) start here, for sure!
  current_command_args = cmd_ptr;

  KEEPALIVE_STATE(IN_HANDLER);

  // Handle a known G, M, or T
  switch (command_code) {
    case 'G': switch (codenum) {

      // G0, G1
      case 0:
      case 1:
        gcode_G0_G1();
        break;

      // G2, G3
      #if DISABLED(SCARA)
        case 2: // G2  - CW ARC
        case 3: // G3  - CCW ARC
          gcode_G2_G3(codenum == 2);
          break;
      #endif

      // G4 Dwell
      case 4:
        gcode_G4();
        break;

      #if ENABLED(FWRETRACT)

        case 10: // G10: retract
        case 11: // G11: retract_recover
          gcode_G10_G11(codenum == 10);
          break;

      #endif //FWRETRACT

      case 28: // G28: Home all axes, one at a time
        gcode_G28();
        break;

      #if ENABLED(AUTO_BED_LEVELING_FEATURE) || ENABLED(MESH_BED_LEVELING)
        case 29: // G29 Detailed Z probe, probes the bed at 3 or more points.
          gcode_G29();
          break;
      #endif

      #if ENABLED(AUTO_BED_LEVELING_FEATURE)

        #if DISABLED(Z_PROBE_SLED)

          case 30: // G30 Single Z probe
            gcode_G30();
            break;

        #else // Z_PROBE_SLED

            case 31: // G31: dock the sled
            case 32: // G32: undock the sled
              dock_sled(codenum == 31);
              break;

        #endif // Z_PROBE_SLED

      #endif // AUTO_BED_LEVELING_FEATURE

      case 90: // G90
        relative_mode = false;
        break;
      case 91: // G91
        relative_mode = true;
        break;

      case 92: // G92
        gcode_G92();
        break;
    }
    break;

    case 'M': switch (codenum) {
      #if ENABLED(ULTIPANEL)
        case 0: // M0 - Unconditional stop - Wait for user button press on LCD
        case 1: // M1 - Conditional stop - Wait for user button press on LCD
          gcode_M0_M1();
          break;
      #endif // ULTIPANEL

      case 17:
        gcode_M17();
        break;

      #if ENABLED(SDSUPPORT)

        case 20: // M20 - list SD card
          gcode_M20(); break;
        case 21: // M21 - init SD card
          gcode_M21(); break;
        case 22: //M22 - release SD card
          gcode_M22(); break;
        case 23: //M23 - Select file
          gcode_M23(); break;
        case 24: //M24 - Start SD print
          gcode_M24(); break;
        case 25: //M25 - Pause SD print
          gcode_M25(); break;
        case 26: //M26 - Set SD index
          gcode_M26(); break;
        case 27: //M27 - Get SD status
          gcode_M27(); break;
        case 28: //M28 - Start SD write
          gcode_M28(); break;
        case 29: //M29 - Stop SD write
          gcode_M29(); break;
        case 30: //M30 <filename> Delete File
          gcode_M30(); break;
        case 32: //M32 - Select file and start SD print
          gcode_M32(); break;

        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          case 33: //M33 - Get the long full path to a file or folder
            gcode_M33(); break;
        #endif // LONG_FILENAME_HOST_SUPPORT

        case 928: //M928 - Start SD write
          gcode_M928(); break;

      #endif //SDSUPPORT

      case 31: //M31 take time since the start of the SD print or an M109 command
        gcode_M31();
        break;

      case 42: //M42 -Change pin status via gcode
        gcode_M42();
        break;

      #if ENABLED(AUTO_BED_LEVELING_FEATURE) && ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
        case 48: // M48 Z probe repeatability
          gcode_M48();
          break;
      #endif // AUTO_BED_LEVELING_FEATURE && Z_MIN_PROBE_REPEATABILITY_TEST

      #if ENABLED(M100_FREE_MEMORY_WATCHER)
        case 100:
          gcode_M100();
          break;
      #endif

      case 104: // M104
        gcode_M104();
        break;

      case 110: // M110: Set Current Line Number
        gcode_M110();
        break;

      case 111: // M111: Set debug level
        gcode_M111();
        break;

      case 112: // M112: Emergency Stop
        gcode_M112();
        break;

      case 140: // M140: Set bed temp
        gcode_M140();
        break;

      case 105: // M105: Read current temperature
        gcode_M105();
        KEEPALIVE_STATE(NOT_BUSY);
        return; // "ok" already printed

      case 109: // M109: Wait for temperature
        gcode_M109();
        break;

      #if HAS_TEMP_BED
        case 190: // M190: Wait for bed heater to reach target
          gcode_M190();
          break;
      #endif // HAS_TEMP_BED

      #if FAN_COUNT > 0
        case 106: // M106: Fan On
          gcode_M106();
          break;
        case 107: // M107: Fan Off
          gcode_M107();
          break;
      #endif // FAN_COUNT > 0

      #if ENABLED(BARICUDA)
        // PWM for HEATER_1_PIN
        #if HAS_HEATER_1
          case 126: // M126: valve open
            gcode_M126();
            break;
          case 127: // M127: valve closed
            gcode_M127();
            break;
        #endif // HAS_HEATER_1

        // PWM for HEATER_2_PIN
        #if HAS_HEATER_2
          case 128: // M128: valve open
            gcode_M128();
            break;
          case 129: // M129: valve closed
            gcode_M129();
            break;
        #endif // HAS_HEATER_2
      #endif // BARICUDA

      #if HAS_POWER_SWITCH

        case 80: // M80: Turn on Power Supply
          gcode_M80();
          break;

      #endif // HAS_POWER_SWITCH

      case 81: // M81: Turn off Power, including Power Supply, if possible
        gcode_M81();
        break;

      case 82:
        gcode_M82();
        break;
      case 83:
        gcode_M83();
        break;
      case 18: // (for compatibility)
      case 84: // M84
        gcode_M18_M84();
        break;
      case 85: // M85
        gcode_M85();
        break;
      case 92: // M92: Set the steps-per-unit for one or more axes
        gcode_M92();
        break;
      case 115: // M115: Report capabilities
        gcode_M115();
        break;
      case 117: // M117: Set LCD message text, if possible
        gcode_M117();
        break;
      case 114: // M114: Report current position
        gcode_M114();
        break;
      case 120: // M120: Enable endstops
        gcode_M120();
        break;
      case 121: // M121: Disable endstops
        gcode_M121();
        break;
      case 119: // M119: Report endstop states
        gcode_M119();
        break;

      #if ENABLED(ULTIPANEL)

        case 145: // M145: Set material heatup parameters
          gcode_M145();
          break;

      #endif

      #if ENABLED(BLINKM)

        case 150: // M150
          gcode_M150();
          break;

      #endif //BLINKM

      case 200: // M200 D<millimeters> set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
        gcode_M200();
        break;
      case 201: // M201
        gcode_M201();
        break;
      #if 0 // Not used for Sprinter/grbl gen6
        case 202: // M202
          gcode_M202();
          break;
      #endif
      case 203: // M203 max feedrate mm/sec
        gcode_M203();
        break;
      case 204: // M204 acclereration S normal moves T filmanent only moves
        gcode_M204();
        break;
      case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
        gcode_M205();
        break;
      case 206: // M206 additional homing offset
        gcode_M206();
        break;

      #if ENABLED(DELTA)
        case 665: // M665 set delta configurations L<diagonal_rod> R<delta_radius> S<segments_per_sec>
          gcode_M665();
          break;
      #endif

      #if ENABLED(DELTA) || ENABLED(Z_DUAL_ENDSTOPS)
        case 666: // M666 set delta / dual endstop adjustment
          gcode_M666();
          break;
      #endif

      #if ENABLED(FWRETRACT)
        case 207: //M207 - set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop]
          gcode_M207();
          break;
        case 208: // M208 - set retract recover length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
          gcode_M208();
          break;
        case 209: // M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
          gcode_M209();
          break;
      #endif // FWRETRACT

      #if EXTRUDERS > 1
        case 218: // M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
          gcode_M218();
          break;
      #endif

      case 220: // M220 S<factor in percent>- set speed factor override percentage
        gcode_M220();
        break;

      case 221: // M221 S<factor in percent>- set extrude factor override percentage
        gcode_M221();
        break;

      case 226: // M226 P<pin number> S<pin state>- Wait until the specified pin reaches the state required
        gcode_M226();
        break;

      #if HAS_SERVOS
        case 280: // M280 - set servo position absolute. P: servo index, S: angle or microseconds
          gcode_M280();
          break;
      #endif // HAS_SERVOS

      #if HAS_BUZZER
        case 300: // M300 - Play beep tone
          gcode_M300();
          break;
      #endif // HAS_BUZZER

      #if ENABLED(PIDTEMP)
        case 301: // M301
          gcode_M301();
          break;
      #endif // PIDTEMP

      #if ENABLED(PIDTEMPBED)
        case 304: // M304
          gcode_M304();
          break;
      #endif // PIDTEMPBED

      #if defined(CHDK) || HAS_PHOTOGRAPH
        case 240: // M240  Triggers a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
          gcode_M240();
          break;
      #endif // CHDK || PHOTOGRAPH_PIN

      #if ENABLED(HAS_LCD_CONTRAST)
        case 250: // M250  Set LCD contrast value: C<value> (value 0..63)
          gcode_M250();
          break;
      #endif // HAS_LCD_CONTRAST

      #if ENABLED(PREVENT_DANGEROUS_EXTRUDE)
        case 302: // allow cold extrudes, or set the minimum extrude temperature
          gcode_M302();
          break;
      #endif // PREVENT_DANGEROUS_EXTRUDE

      case 303: // M303 PID autotune
        gcode_M303();
        break;

      #if ENABLED(SCARA)
        case 360:  // M360 SCARA Theta pos1
          if (gcode_M360()) return;
          break;
        case 361:  // M361 SCARA Theta pos2
          if (gcode_M361()) return;
          break;
        case 362:  // M362 SCARA Psi pos1
          if (gcode_M362()) return;
          break;
        case 363:  // M363 SCARA Psi pos2
          if (gcode_M363()) return;
          break;
        case 364:  // M364 SCARA Psi pos3 (90 deg to Theta)
          if (gcode_M364()) return;
          break;
        case 365: // M365 Set SCARA scaling for X Y Z
          gcode_M365();
          break;
      #endif // SCARA

      case 400: // M400 finish all moves
        gcode_M400();
        break;

      #if ENABLED(AUTO_BED_LEVELING_FEATURE) && (HAS_SERVO_ENDSTOPS || ENABLED(Z_PROBE_ALLEN_KEY)) && DISABLED(Z_PROBE_SLED)
        case 401:
          gcode_M401();
          break;
        case 402:
          gcode_M402();
          break;
      #endif // AUTO_BED_LEVELING_FEATURE && (HAS_SERVO_ENDSTOPS || Z_PROBE_ALLEN_KEY) && !Z_PROBE_SLED

      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        case 404:  //M404 Enter the nominal filament width (3mm, 1.75mm ) N<3.0> or display nominal filament width
          gcode_M404();
          break;
        case 405:  //M405 Turn on filament sensor for control
          gcode_M405();
          break;
        case 406:  //M406 Turn off filament sensor for control
          gcode_M406();
          break;
        case 407:   //M407 Display measured filament diameter
          gcode_M407();
          break;
      #endif // ENABLED(FILAMENT_WIDTH_SENSOR)

      case 410: // M410 quickstop - Abort all the planned moves.
        gcode_M410();
        break;

      #if ENABLED(MESH_BED_LEVELING)
        case 420: // M420 Enable/Disable Mesh Bed Leveling
          gcode_M420();
          break;
        case 421: // M421 Set a Mesh Bed Leveling Z coordinate
          gcode_M421();
          break;
      #endif

      case 428: // M428 Apply current_position to home_offset
        gcode_M428();
        break;

      case 500: // M500 Store settings in EEPROM
        gcode_M500();
        break;
      case 501: // M501 Read settings from EEPROM
        gcode_M501();
        break;
      case 502: // M502 Revert to default settings
        gcode_M502();
        break;
      case 503: // M503 print settings currently in memory
        gcode_M503();
        break;

      #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
        case 540:
          gcode_M540();
          break;
      #endif

      #ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET
        case CUSTOM_M_CODE_SET_Z_PROBE_OFFSET:
          gcode_SET_Z_PROBE_OFFSET();
          break;
      #endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

      #if ENABLED(FILAMENTCHANGEENABLE)
        case 600: //Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
          gcode_M600();
          break;
      #endif // FILAMENTCHANGEENABLE

      #if ENABLED(DUAL_X_CARRIAGE)
        case 605:
          gcode_M605();
          break;
      #endif // DUAL_X_CARRIAGE

      case 907: // M907 Set digital trimpot motor current using axis codes.
        gcode_M907();
        break;

      #if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)

        case 908: // M908 Control digital trimpot directly.
          gcode_M908();
          break;

        #if ENABLED(DAC_STEPPER_CURRENT) // As with Printrbot RevF

          case 909: // M909 Print digipot/DAC current value
            gcode_M909();
            break;

          case 910: // M910 Commit digipot/DAC value to external EEPROM
            gcode_M910();
            break;

        #endif

      #endif // HAS_DIGIPOTSS || DAC_STEPPER_CURRENT

      #if HAS_MICROSTEPS

        case 350: // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
          gcode_M350();
          break;

        case 351: // M351 Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
          gcode_M351();
          break;

      #endif // HAS_MICROSTEPS

      case 999: // M999: Restart after being Stopped
        gcode_M999();
        break;
    }
    break;

    case 'T':
      gcode_T(codenum);
      break;

    default: code_is_good = false;
  }

  KEEPALIVE_STATE(NOT_BUSY);

ExitUnknownCommand:

  // Still unknown command? Throw an error
  if (!code_is_good) unknown_command_error();

  ok_to_send();
}

void FlushSerialRequestResend() {
  //char command_queue[cmd_queue_index_r][100]="Resend:";
  MYSERIAL.flush();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ok_to_send();
}

void ok_to_send() {
  refresh_cmd_timeout();
  if (!send_ok[cmd_queue_index_r]) return;
  SERIAL_PROTOCOLPGM(MSG_OK);
  #if ENABLED(ADVANCED_OK)
    char* p = command_queue[cmd_queue_index_r];
    if (*p == 'N') {
      SERIAL_PROTOCOL(' ');
      SERIAL_ECHO(*p++);
      while (NUMERIC_SIGNED(*p))
        SERIAL_ECHO(*p++);
    }
    SERIAL_PROTOCOLPGM(" P"); SERIAL_PROTOCOL(int(BLOCK_BUFFER_SIZE - movesplanned() - 1));
    SERIAL_PROTOCOLPGM(" B"); SERIAL_PROTOCOL(BUFSIZE - commands_in_queue);
  #endif
  SERIAL_EOL;
}

void clamp_to_software_endstops(float target[3]) {
  if (min_software_endstops) {
    NOLESS(target[X_AXIS], min_pos[X_AXIS]);
    NOLESS(target[Y_AXIS], min_pos[Y_AXIS]);

    float negative_z_offset = 0;
    #if ENABLED(AUTO_BED_LEVELING_FEATURE)
      if (zprobe_zoffset < 0) negative_z_offset += zprobe_zoffset;
      if (home_offset[Z_AXIS] < 0) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("> clamp_to_software_endstops > Add home_offset[Z_AXIS]:", home_offset[Z_AXIS]);
            SERIAL_EOL;
          }
        #endif
        negative_z_offset += home_offset[Z_AXIS];
      }
    #endif
    NOLESS(target[Z_AXIS], min_pos[Z_AXIS] + negative_z_offset);
  }

  if (max_software_endstops) {
    NOMORE(target[X_AXIS], max_pos[X_AXIS]);
    NOMORE(target[Y_AXIS], max_pos[Y_AXIS]);
    NOMORE(target[Z_AXIS], max_pos[Z_AXIS]);
  }
}

#if ENABLED(DELTA)

  void recalc_delta_settings(float radius, float diagonal_rod) {
    delta_tower1_x = -SIN_60 * (radius + DELTA_RADIUS_TRIM_TOWER_1);  // front left tower
    delta_tower1_y = -COS_60 * (radius + DELTA_RADIUS_TRIM_TOWER_1);
    delta_tower2_x =  SIN_60 * (radius + DELTA_RADIUS_TRIM_TOWER_2);  // front right tower
    delta_tower2_y = -COS_60 * (radius + DELTA_RADIUS_TRIM_TOWER_2);
    delta_tower3_x = 0.0;                                             // back middle tower
    delta_tower3_y = (radius + DELTA_RADIUS_TRIM_TOWER_3);
    delta_diagonal_rod_2_tower_1 = sq(diagonal_rod + delta_diagonal_rod_trim_tower_1);
    delta_diagonal_rod_2_tower_2 = sq(diagonal_rod + delta_diagonal_rod_trim_tower_2);
    delta_diagonal_rod_2_tower_3 = sq(diagonal_rod + delta_diagonal_rod_trim_tower_3);
  }

  void calculate_delta(float cartesian[3]) {

    delta[TOWER_1] = sqrt(delta_diagonal_rod_2_tower_1
                          - sq(delta_tower1_x - cartesian[X_AXIS])
                          - sq(delta_tower1_y - cartesian[Y_AXIS])
                         ) + cartesian[Z_AXIS];
    delta[TOWER_2] = sqrt(delta_diagonal_rod_2_tower_2
                          - sq(delta_tower2_x - cartesian[X_AXIS])
                          - sq(delta_tower2_y - cartesian[Y_AXIS])
                         ) + cartesian[Z_AXIS];
    delta[TOWER_3] = sqrt(delta_diagonal_rod_2_tower_3
                          - sq(delta_tower3_x - cartesian[X_AXIS])
                          - sq(delta_tower3_y - cartesian[Y_AXIS])
                         ) + cartesian[Z_AXIS];
    /**
    SERIAL_ECHOPGM("cartesian x="); SERIAL_ECHO(cartesian[X_AXIS]);
    SERIAL_ECHOPGM(" y="); SERIAL_ECHO(cartesian[Y_AXIS]);
    SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(cartesian[Z_AXIS]);

    SERIAL_ECHOPGM("delta a="); SERIAL_ECHO(delta[TOWER_1]);
    SERIAL_ECHOPGM(" b="); SERIAL_ECHO(delta[TOWER_2]);
    SERIAL_ECHOPGM(" c="); SERIAL_ECHOLN(delta[TOWER_3]);
    */
  }

  #if ENABLED(AUTO_BED_LEVELING_FEATURE)

    // Adjust print surface height by linear interpolation over the bed_level array.
    void adjust_delta(float cartesian[3]) {
      if (delta_grid_spacing[0] == 0 || delta_grid_spacing[1] == 0) return; // G29 not done!

      int half = (AUTO_BED_LEVELING_GRID_POINTS - 1) / 2;
      float h1 = 0.001 - half, h2 = half - 0.001,
            grid_x = max(h1, min(h2, cartesian[X_AXIS] / delta_grid_spacing[0])),
            grid_y = max(h1, min(h2, cartesian[Y_AXIS] / delta_grid_spacing[1]));
      int floor_x = floor(grid_x), floor_y = floor(grid_y);
      float ratio_x = grid_x - floor_x, ratio_y = grid_y - floor_y,
            z1 = bed_level[floor_x + half][floor_y + half],
            z2 = bed_level[floor_x + half][floor_y + half + 1],
            z3 = bed_level[floor_x + half + 1][floor_y + half],
            z4 = bed_level[floor_x + half + 1][floor_y + half + 1],
            left = (1 - ratio_y) * z1 + ratio_y * z2,
            right = (1 - ratio_y) * z3 + ratio_y * z4,
            offset = (1 - ratio_x) * left + ratio_x * right;

      delta[X_AXIS] += offset;
      delta[Y_AXIS] += offset;
      delta[Z_AXIS] += offset;

      /**
      SERIAL_ECHOPGM("grid_x="); SERIAL_ECHO(grid_x);
      SERIAL_ECHOPGM(" grid_y="); SERIAL_ECHO(grid_y);
      SERIAL_ECHOPGM(" floor_x="); SERIAL_ECHO(floor_x);
      SERIAL_ECHOPGM(" floor_y="); SERIAL_ECHO(floor_y);
      SERIAL_ECHOPGM(" ratio_x="); SERIAL_ECHO(ratio_x);
      SERIAL_ECHOPGM(" ratio_y="); SERIAL_ECHO(ratio_y);
      SERIAL_ECHOPGM(" z1="); SERIAL_ECHO(z1);
      SERIAL_ECHOPGM(" z2="); SERIAL_ECHO(z2);
      SERIAL_ECHOPGM(" z3="); SERIAL_ECHO(z3);
      SERIAL_ECHOPGM(" z4="); SERIAL_ECHO(z4);
      SERIAL_ECHOPGM(" left="); SERIAL_ECHO(left);
      SERIAL_ECHOPGM(" right="); SERIAL_ECHO(right);
      SERIAL_ECHOPGM(" offset="); SERIAL_ECHOLN(offset);
      */
    }
  #endif // AUTO_BED_LEVELING_FEATURE

#endif // DELTA

#if ENABLED(MESH_BED_LEVELING)

// This function is used to split lines on mesh borders so each segment is only part of one mesh area
void mesh_plan_buffer_line(float x, float y, float z, const float e, float feed_rate, const uint8_t& extruder, uint8_t x_splits = 0xff, uint8_t y_splits = 0xff) {
  if (!mbl.active) {
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    set_current_to_destination();
    return;
  }
  int pix = mbl.select_x_index(current_position[X_AXIS]);
  int piy = mbl.select_y_index(current_position[Y_AXIS]);
  int ix = mbl.select_x_index(x);
  int iy = mbl.select_y_index(y);
  pix = min(pix, MESH_NUM_X_POINTS - 2);
  piy = min(piy, MESH_NUM_Y_POINTS - 2);
  ix = min(ix, MESH_NUM_X_POINTS - 2);
  iy = min(iy, MESH_NUM_Y_POINTS - 2);
  if (pix == ix && piy == iy) {
    // Start and end on same mesh square
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    set_current_to_destination();
    return;
  }
  float nx, ny, nz, ne, normalized_dist;
  if (ix > pix && TEST(x_splits, ix)) {
    nx = mbl.get_x(ix);
    normalized_dist = (nx - current_position[X_AXIS]) / (x - current_position[X_AXIS]);
    ny = current_position[Y_AXIS] + (y - current_position[Y_AXIS]) * normalized_dist;
    nz = current_position[Z_AXIS] + (z - current_position[Z_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    CBI(x_splits, ix);
  }
  else if (ix < pix && TEST(x_splits, pix)) {
    nx = mbl.get_x(pix);
    normalized_dist = (nx - current_position[X_AXIS]) / (x - current_position[X_AXIS]);
    ny = current_position[Y_AXIS] + (y - current_position[Y_AXIS]) * normalized_dist;
    nz = current_position[Z_AXIS] + (z - current_position[Z_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    CBI(x_splits, pix);
  }
  else if (iy > piy && TEST(y_splits, iy)) {
    ny = mbl.get_y(iy);
    normalized_dist = (ny - current_position[Y_AXIS]) / (y - current_position[Y_AXIS]);
    nx = current_position[X_AXIS] + (x - current_position[X_AXIS]) * normalized_dist;
    nz = current_position[Z_AXIS] + (z - current_position[Z_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    CBI(y_splits, iy);
  }
  else if (iy < piy && TEST(y_splits, piy)) {
    ny = mbl.get_y(piy);
    normalized_dist = (ny - current_position[Y_AXIS]) / (y - current_position[Y_AXIS]);
    nx = current_position[X_AXIS] + (x - current_position[X_AXIS]) * normalized_dist;
    nz = current_position[Z_AXIS] + (z - current_position[Z_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    CBI(y_splits, piy);
  }
  else {
    // Already split on a border
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    set_current_to_destination();
    return;
  }
  // Do the split and look for more borders
  destination[X_AXIS] = nx;
  destination[Y_AXIS] = ny;
  destination[Z_AXIS] = nz;
  destination[E_AXIS] = ne;
  mesh_plan_buffer_line(nx, ny, nz, ne, feed_rate, extruder, x_splits, y_splits);
  destination[X_AXIS] = x;
  destination[Y_AXIS] = y;
  destination[Z_AXIS] = z;
  destination[E_AXIS] = e;
  mesh_plan_buffer_line(x, y, z, e, feed_rate, extruder, x_splits, y_splits);
}
#endif  // MESH_BED_LEVELING

#if ENABLED(PREVENT_DANGEROUS_EXTRUDE)

  inline void prevent_dangerous_extrude(float& curr_e, float& dest_e) {
    if (DEBUGGING(DRYRUN)) return;
    float de = dest_e - curr_e;
    if (de) {
      if (degHotend(active_extruder) < extrude_min_temp) {
        curr_e = dest_e; // Behave as if the move really took place, but ignore E part
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_ERR_COLD_EXTRUDE_STOP);
      }
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        if (labs(de) > EXTRUDE_MAXLENGTH) {
          curr_e = dest_e; // Behave as if the move really took place, but ignore E part
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_ERR_LONG_EXTRUDE_STOP);
        }
      #endif
    }
  }

#endif // PREVENT_DANGEROUS_EXTRUDE

#if ENABLED(DELTA) || ENABLED(SCARA)

  inline bool prepare_move_delta(float target[NUM_AXIS]) {
    float difference[NUM_AXIS];
    for (int8_t i = 0; i < NUM_AXIS; i++) difference[i] = target[i] - current_position[i];

    float cartesian_mm = sqrt(sq(difference[X_AXIS]) + sq(difference[Y_AXIS]) + sq(difference[Z_AXIS]));
    if (cartesian_mm < 0.000001) cartesian_mm = abs(difference[E_AXIS]);
    if (cartesian_mm < 0.000001) return false;
    float seconds = 6000 * cartesian_mm / feedrate / feedrate_multiplier;
    int steps = max(1, int(delta_segments_per_second * seconds));

    // SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
    // SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
    // SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);

    for (int s = 1; s <= steps; s++) {

      float fraction = float(s) / float(steps);

      for (int8_t i = 0; i < NUM_AXIS; i++)
        target[i] = current_position[i] + difference[i] * fraction;

      calculate_delta(target);

      #if ENABLED(AUTO_BED_LEVELING_FEATURE)
        adjust_delta(target);
      #endif

      //SERIAL_ECHOPGM("target[X_AXIS]="); SERIAL_ECHOLN(target[X_AXIS]);
      //SERIAL_ECHOPGM("target[Y_AXIS]="); SERIAL_ECHOLN(target[Y_AXIS]);
      //SERIAL_ECHOPGM("target[Z_AXIS]="); SERIAL_ECHOLN(target[Z_AXIS]);
      //SERIAL_ECHOPGM("delta[X_AXIS]="); SERIAL_ECHOLN(delta[X_AXIS]);
      //SERIAL_ECHOPGM("delta[Y_AXIS]="); SERIAL_ECHOLN(delta[Y_AXIS]);
      //SERIAL_ECHOPGM("delta[Z_AXIS]="); SERIAL_ECHOLN(delta[Z_AXIS]);

      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], target[E_AXIS], feedrate / 60 * feedrate_multiplier / 100.0, active_extruder);
    }
    return true;
  }

#endif // DELTA || SCARA

#if ENABLED(SCARA)
  inline bool prepare_move_scara(float target[NUM_AXIS]) { return prepare_move_delta(target); }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  inline bool prepare_move_dual_x_carriage() {
    if (active_extruder_parked) {
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && active_extruder == 0) {
        // move duplicate extruder into correct duplication position.
        plan_set_position(inactive_extruder_x_pos, current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        plan_buffer_line(current_position[X_AXIS] + duplicate_extruder_x_offset,
                         current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], max_feedrate[X_AXIS], 1);
        sync_plan_position();
        st_synchronize();
        extruder_duplication_enabled = true;
        active_extruder_parked = false;
      }
      else if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE) { // handle unparking of head
        if (current_position[E_AXIS] == destination[E_AXIS]) {
          // This is a travel move (with no extrusion)
          // Skip it, but keep track of the current position
          // (so it can be used as the start of the next non-travel move)
          if (delayed_move_time != 0xFFFFFFFFUL) {
            set_current_to_destination();
            NOLESS(raised_parked_position[Z_AXIS], destination[Z_AXIS]);
            delayed_move_time = millis();
            return false;
          }
        }
        delayed_move_time = 0;
        // unpark extruder: 1) raise, 2) move into starting XY position, 3) lower
        plan_buffer_line(raised_parked_position[X_AXIS], raised_parked_position[Y_AXIS], raised_parked_position[Z_AXIS], current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], raised_parked_position[Z_AXIS], current_position[E_AXIS], min(max_feedrate[X_AXIS], max_feedrate[Y_AXIS]), active_extruder);
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
        active_extruder_parked = false;
      }
    }
    return true;
  }

#endif // DUAL_X_CARRIAGE

#if DISABLED(DELTA) && DISABLED(SCARA)

  inline bool prepare_move_cartesian() {
    // Do not use feedrate_multiplier for E or Z only moves
    if (current_position[X_AXIS] == destination[X_AXIS] && current_position[Y_AXIS] == destination[Y_AXIS]) {
      line_to_destination();
    }
    else {
      #if ENABLED(MESH_BED_LEVELING)
        mesh_plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], (feedrate / 60) * (feedrate_multiplier / 100.0), active_extruder);
        return false;
      #else
        line_to_destination(feedrate * feedrate_multiplier / 100.0);
      #endif
    }
    return true;
  }

#endif // !DELTA && !SCARA

/**
 * Prepare a single move and get ready for the next one
 *
 * (This may call plan_buffer_line several times to put
 *  smaller moves into the planner for DELTA or SCARA.)
 */
void prepare_move() {
  clamp_to_software_endstops(destination);
  refresh_cmd_timeout();

  #if ENABLED(PREVENT_DANGEROUS_EXTRUDE)
    prevent_dangerous_extrude(current_position[E_AXIS], destination[E_AXIS]);
  #endif

  #if ENABLED(SCARA)
    if (!prepare_move_scara(destination)) return;
  #elif ENABLED(DELTA)
    if (!prepare_move_delta(destination)) return;
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    if (!prepare_move_dual_x_carriage()) return;
  #endif

  #if DISABLED(DELTA) && DISABLED(SCARA)
    if (!prepare_move_cartesian()) return;
  #endif

  set_current_to_destination();
}

/**
 * Plan an arc in 2 dimensions
 *
 * The arc is approximated by generating many small linear segments.
 * The length of each segment is configured in MM_PER_ARC_SEGMENT (Default 1mm)
 * Arcs should only be made relatively large (over 5mm), as larger arcs with
 * larger segments will tend to be more efficient. Your slicer should have
 * options for G2/G3 arc generation. In future these options may be GCode tunable.
 */
void plan_arc(
  float target[NUM_AXIS], // Destination position
  float* offset,          // Center of rotation relative to current_position
  uint8_t clockwise       // Clockwise?
) {

  float radius = hypot(offset[X_AXIS], offset[Y_AXIS]),
        center_axis0 = current_position[X_AXIS] + offset[X_AXIS],
        center_axis1 = current_position[Y_AXIS] + offset[Y_AXIS],
        linear_travel = target[Z_AXIS] - current_position[Z_AXIS],
        extruder_travel = target[E_AXIS] - current_position[E_AXIS],
        r_axis0 = -offset[X_AXIS],  // Radius vector from center to current location
        r_axis1 = -offset[Y_AXIS],
        rt_axis0 = target[X_AXIS] - center_axis0,
        rt_axis1 = target[Y_AXIS] - center_axis1;

  // CCW angle of rotation between position and target from the circle center. Only one atan2() trig computation required.
  float angular_travel = atan2(r_axis0 * rt_axis1 - r_axis1 * rt_axis0, r_axis0 * rt_axis0 + r_axis1 * rt_axis1);
  if (angular_travel < 0)  angular_travel += RADIANS(360);
  if (clockwise)  angular_travel -= RADIANS(360);

  // Make a circle if the angular rotation is 0
  if (current_position[X_AXIS] == target[X_AXIS] && current_position[Y_AXIS] == target[Y_AXIS] && angular_travel == 0)
    angular_travel += RADIANS(360);

  float mm_of_travel = hypot(angular_travel * radius, fabs(linear_travel));
  if (mm_of_travel < 0.001)  return;
  uint16_t segments = floor(mm_of_travel / (MM_PER_ARC_SEGMENT));
  if (segments == 0) segments = 1;

  float theta_per_segment = angular_travel / segments;
  float linear_per_segment = linear_travel / segments;
  float extruder_per_segment = extruder_travel / segments;

  /**
   * Vector rotation by transformation matrix: r is the original vector, r_T is the rotated vector,
   * and phi is the angle of rotation. Based on the solution approach by Jens Geisler.
   *     r_T = [cos(phi) -sin(phi);
   *            sin(phi)  cos(phi] * r ;
   *
   * For arc generation, the center of the circle is the axis of rotation and the radius vector is
   * defined from the circle center to the initial position. Each line segment is formed by successive
   * vector rotations. This requires only two cos() and sin() computations to form the rotation
   * matrix for the duration of the entire arc. Error may accumulate from numerical round-off, since
   * all double numbers are single precision on the Arduino. (True double precision will not have
   * round off issues for CNC applications.) Single precision error can accumulate to be greater than
   * tool precision in some cases. Therefore, arc path correction is implemented.
   *
   * Small angle approximation may be used to reduce computation overhead further. This approximation
   * holds for everything, but very small circles and large MM_PER_ARC_SEGMENT values. In other words,
   * theta_per_segment would need to be greater than 0.1 rad and N_ARC_CORRECTION would need to be large
   * to cause an appreciable drift error. N_ARC_CORRECTION~=25 is more than small enough to correct for
   * numerical drift error. N_ARC_CORRECTION may be on the order a hundred(s) before error becomes an
   * issue for CNC machines with the single precision Arduino calculations.
   *
   * This approximation also allows plan_arc to immediately insert a line segment into the planner
   * without the initial overhead of computing cos() or sin(). By the time the arc needs to be applied
   * a correction, the planner should have caught up to the lag caused by the initial plan_arc overhead.
   * This is important when there are successive arc motions.
   */
  // Vector rotation matrix values
  float cos_T = 1 - 0.5 * theta_per_segment * theta_per_segment; // Small angle approximation
  float sin_T = theta_per_segment;

  float arc_target[NUM_AXIS];
  float sin_Ti;
  float cos_Ti;
  float r_axisi;
  uint16_t i;
  int8_t count = 0;

  // Initialize the linear axis
  arc_target[Z_AXIS] = current_position[Z_AXIS];

  // Initialize the extruder axis
  arc_target[E_AXIS] = current_position[E_AXIS];

  float feed_rate = feedrate * feedrate_multiplier / 60 / 100.0;

  for (i = 1; i < segments; i++) { // Increment (segments-1)

    if (count < N_ARC_CORRECTION) {
      // Apply vector rotation matrix to previous r_axis0 / 1
      r_axisi = r_axis0 * sin_T + r_axis1 * cos_T;
      r_axis0 = r_axis0 * cos_T - r_axis1 * sin_T;
      r_axis1 = r_axisi;
      count++;
    }
    else {
      // Arc correction to radius vector. Computed only every N_ARC_CORRECTION increments.
      // Compute exact location by applying transformation matrix from initial radius vector(=-offset).
      cos_Ti = cos(i * theta_per_segment);
      sin_Ti = sin(i * theta_per_segment);
      r_axis0 = -offset[X_AXIS] * cos_Ti + offset[Y_AXIS] * sin_Ti;
      r_axis1 = -offset[X_AXIS] * sin_Ti - offset[Y_AXIS] * cos_Ti;
      count = 0;
    }

    // Update arc_target location
    arc_target[X_AXIS] = center_axis0 + r_axis0;
    arc_target[Y_AXIS] = center_axis1 + r_axis1;
    arc_target[Z_AXIS] += linear_per_segment;
    arc_target[E_AXIS] += extruder_per_segment;

    clamp_to_software_endstops(arc_target);

    #if ENABLED(DELTA) || ENABLED(SCARA)
      calculate_delta(arc_target);
      #if ENABLED(AUTO_BED_LEVELING_FEATURE)
        adjust_delta(arc_target);
      #endif
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], arc_target[E_AXIS], feed_rate, active_extruder);
    #else
      plan_buffer_line(arc_target[X_AXIS], arc_target[Y_AXIS], arc_target[Z_AXIS], arc_target[E_AXIS], feed_rate, active_extruder);
    #endif
  }

  // Ensure last segment arrives at target location.
  #if ENABLED(DELTA) || ENABLED(SCARA)
    calculate_delta(target);
    #if ENABLED(AUTO_BED_LEVELING_FEATURE)
      adjust_delta(target);
    #endif
    plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], target[E_AXIS], feed_rate, active_extruder);
  #else
    plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feed_rate, active_extruder);
  #endif

  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  set_current_to_destination();
}

#if HAS_CONTROLLERFAN

  void controllerFan() {
    static millis_t lastMotorOn = 0; // Last time a motor was turned on
    static millis_t nextMotorCheck = 0; // Last time the state was checked
    millis_t ms = millis();
    if (ms >= nextMotorCheck) {
      nextMotorCheck = ms + 2500; // Not a time critical function, so only check every 2.5s
      if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON || soft_pwm_bed > 0
          || E0_ENABLE_READ == E_ENABLE_ON // If any of the drivers are enabled...
          #if EXTRUDERS > 1
            || E1_ENABLE_READ == E_ENABLE_ON
            #if HAS_X2_ENABLE
              || X2_ENABLE_READ == X_ENABLE_ON
            #endif
            #if EXTRUDERS > 2
              || E2_ENABLE_READ == E_ENABLE_ON
              #if EXTRUDERS > 3
                || E3_ENABLE_READ == E_ENABLE_ON
              #endif
            #endif
          #endif
      ) {
        lastMotorOn = ms; //... set time to NOW so the fan will turn on
      }

      // Fan off if no steppers have been enabled for CONTROLLERFAN_SECS seconds
      uint8_t speed = (lastMotorOn == 0 || ms >= lastMotorOn + (CONTROLLERFAN_SECS) * 1000UL) ? 0 : CONTROLLERFAN_SPEED;

      // allows digital or PWM fan output to be used (see M42 handling)
      digitalWrite(CONTROLLERFAN_PIN, speed);
      analogWrite(CONTROLLERFAN_PIN, speed);
    }
  }

#endif // HAS_CONTROLLERFAN

#if ENABLED(SCARA)

  void calculate_SCARA_forward_Transform(float f_scara[3]) {
    // Perform forward kinematics, and place results in delta[3]
    // The maths and first version has been done by QHARLEY . Integrated into masterbranch 06/2014 and slightly restructured by Joachim Cerny in June 2014

    float x_sin, x_cos, y_sin, y_cos;

    //SERIAL_ECHOPGM("f_delta x="); SERIAL_ECHO(f_scara[X_AXIS]);
    //SERIAL_ECHOPGM(" y="); SERIAL_ECHO(f_scara[Y_AXIS]);

    x_sin = sin(f_scara[X_AXIS] / SCARA_RAD2DEG) * Linkage_1;
    x_cos = cos(f_scara[X_AXIS] / SCARA_RAD2DEG) * Linkage_1;
    y_sin = sin(f_scara[Y_AXIS] / SCARA_RAD2DEG) * Linkage_2;
    y_cos = cos(f_scara[Y_AXIS] / SCARA_RAD2DEG) * Linkage_2;

    //SERIAL_ECHOPGM(" x_sin="); SERIAL_ECHO(x_sin);
    //SERIAL_ECHOPGM(" x_cos="); SERIAL_ECHO(x_cos);
    //SERIAL_ECHOPGM(" y_sin="); SERIAL_ECHO(y_sin);
    //SERIAL_ECHOPGM(" y_cos="); SERIAL_ECHOLN(y_cos);

    delta[X_AXIS] = x_cos + y_cos + SCARA_offset_x;  //theta
    delta[Y_AXIS] = x_sin + y_sin + SCARA_offset_y;  //theta+phi

    //SERIAL_ECHOPGM(" delta[X_AXIS]="); SERIAL_ECHO(delta[X_AXIS]);
    //SERIAL_ECHOPGM(" delta[Y_AXIS]="); SERIAL_ECHOLN(delta[Y_AXIS]);
  }

  void calculate_delta(float cartesian[3]) {
    //reverse kinematics.
    // Perform reversed kinematics, and place results in delta[3]
    // The maths and first version has been done by QHARLEY . Integrated into masterbranch 06/2014 and slightly restructured by Joachim Cerny in June 2014

    float SCARA_pos[2];
    static float SCARA_C2, SCARA_S2, SCARA_K1, SCARA_K2, SCARA_theta, SCARA_psi;

    SCARA_pos[X_AXIS] = cartesian[X_AXIS] * axis_scaling[X_AXIS] - SCARA_offset_x;  //Translate SCARA to standard X Y
    SCARA_pos[Y_AXIS] = cartesian[Y_AXIS] * axis_scaling[Y_AXIS] - SCARA_offset_y;  // With scaling factor.

    #if (Linkage_1 == Linkage_2)
      SCARA_C2 = ((sq(SCARA_pos[X_AXIS]) + sq(SCARA_pos[Y_AXIS])) / (2 * (float)L1_2)) - 1;
    #else
      SCARA_C2 = (sq(SCARA_pos[X_AXIS]) + sq(SCARA_pos[Y_AXIS]) - (float)L1_2 - (float)L2_2) / 45000;
    #endif

    SCARA_S2 = sqrt(1 - sq(SCARA_C2));

    SCARA_K1 = Linkage_1 + Linkage_2 * SCARA_C2;
    SCARA_K2 = Linkage_2 * SCARA_S2;

    SCARA_theta = (atan2(SCARA_pos[X_AXIS], SCARA_pos[Y_AXIS]) - atan2(SCARA_K1, SCARA_K2)) * -1;
    SCARA_psi = atan2(SCARA_S2, SCARA_C2);

    delta[X_AXIS] = SCARA_theta * SCARA_RAD2DEG;  // Multiply by 180/Pi  -  theta is support arm angle
    delta[Y_AXIS] = (SCARA_theta + SCARA_psi) * SCARA_RAD2DEG;  //       -  equal to sub arm angle (inverted motor)
    delta[Z_AXIS] = cartesian[Z_AXIS];

    /**
    SERIAL_ECHOPGM("cartesian x="); SERIAL_ECHO(cartesian[X_AXIS]);
    SERIAL_ECHOPGM(" y="); SERIAL_ECHO(cartesian[Y_AXIS]);
    SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(cartesian[Z_AXIS]);

    SERIAL_ECHOPGM("scara x="); SERIAL_ECHO(SCARA_pos[X_AXIS]);
    SERIAL_ECHOPGM(" y="); SERIAL_ECHOLN(SCARA_pos[Y_AXIS]);

    SERIAL_ECHOPGM("delta x="); SERIAL_ECHO(delta[X_AXIS]);
    SERIAL_ECHOPGM(" y="); SERIAL_ECHO(delta[Y_AXIS]);
    SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(delta[Z_AXIS]);

    SERIAL_ECHOPGM("C2="); SERIAL_ECHO(SCARA_C2);
    SERIAL_ECHOPGM(" S2="); SERIAL_ECHO(SCARA_S2);
    SERIAL_ECHOPGM(" Theta="); SERIAL_ECHO(SCARA_theta);
    SERIAL_ECHOPGM(" Psi="); SERIAL_ECHOLN(SCARA_psi);
    SERIAL_EOL;
    */
  }

#endif // SCARA

#if ENABLED(TEMP_STAT_LEDS)

  static bool red_led = false;
  static millis_t next_status_led_update_ms = 0;

  void handle_status_leds(void) {
    float max_temp = 0.0;
    if (millis() > next_status_led_update_ms) {
      next_status_led_update_ms += 500; // Update every 0.5s
      for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder)
        max_temp = max(max(max_temp, degHotend(cur_extruder)), degTargetHotend(cur_extruder));
      #if HAS_TEMP_BED
        max_temp = max(max(max_temp, degTargetBed()), degBed());
      #endif
      bool new_led = (max_temp > 55.0) ? true : (max_temp < 54.0) ? false : red_led;
      if (new_led != red_led) {
        red_led = new_led;
        digitalWrite(STAT_LED_RED, new_led ? HIGH : LOW);
        digitalWrite(STAT_LED_BLUE, new_led ? LOW : HIGH);
      }
    }
  }

#endif

void enable_all_steppers() {
  enable_x();
  enable_y();
  enable_z();
  enable_e0();
  enable_e1();
  enable_e2();
  enable_e3();
}

void disable_all_steppers() {
  disable_x();
  disable_y();
  disable_z();
  disable_e0();
  disable_e1();
  disable_e2();
  disable_e3();
}

/**
 * Standard idle routine keeps the machine alive
 */
void idle(
  #if ENABLED(FILAMENTCHANGEENABLE)
    bool no_stepper_sleep/*=false*/
  #endif
) {
  manage_heater();
  manage_inactivity(
    #if ENABLED(FILAMENTCHANGEENABLE)
      no_stepper_sleep
    #endif
  );
  host_keepalive();
  lcd_update();
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

  #if HAS_FILRUNOUT
    if (IS_SD_PRINTING && !(READ(FILRUNOUT_PIN) ^ FIL_RUNOUT_INVERTING))
      filrunout();
  #endif

  if (commands_in_queue < BUFSIZE) get_available_commands();

  millis_t ms = millis();

  if (max_inactive_time && ms > previous_cmd_ms + max_inactive_time) kill(PSTR(MSG_KILLED));

  if (stepper_inactive_time && ms > previous_cmd_ms + stepper_inactive_time
      && !ignore_stepper_queue && !blocks_queued()) {
    #if ENABLED(DISABLE_INACTIVE_X)
      disable_x();
    #endif
    #if ENABLED(DISABLE_INACTIVE_Y)
      disable_y();
    #endif
    #if ENABLED(DISABLE_INACTIVE_Z)
      disable_z();
    #endif
    #if ENABLED(DISABLE_INACTIVE_E)
      disable_e0();
      disable_e1();
      disable_e2();
      disable_e3();
    #endif
  }

  #ifdef CHDK // Check if pin should be set to LOW after M240 set it to HIGH
    if (chdkActive && ms > chdkHigh + CHDK_DELAY) {
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
    if (killCount >= KILL_DELAY) kill(PSTR(MSG_KILLED));
  #endif

  #if HAS_HOME
    // Check to see if we have to home, use poor man's debouncer
    // ---------------------------------------------------------
    static int homeDebounceCount = 0;   // poor man's debouncing count
    const int HOME_DEBOUNCE_DELAY = 2500;
    if (!READ(HOME_PIN)) {
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

  #if HAS_CONTROLLERFAN
    controllerFan(); // Check if fan should be turned on to cool stepper drivers down
  #endif

  #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
    if (ms > previous_cmd_ms + (EXTRUDER_RUNOUT_SECONDS) * 1000)
      if (degHotend(active_extruder) > EXTRUDER_RUNOUT_MINTEMP) {
        bool oldstatus;
        switch (active_extruder) {
          case 0:
            oldstatus = E0_ENABLE_READ;
            enable_e0();
            break;
          #if EXTRUDERS > 1
            case 1:
              oldstatus = E1_ENABLE_READ;
              enable_e1();
              break;
            #if EXTRUDERS > 2
              case 2:
                oldstatus = E2_ENABLE_READ;
                enable_e2();
                break;
              #if EXTRUDERS > 3
                case 3:
                  oldstatus = E3_ENABLE_READ;
                  enable_e3();
                  break;
              #endif
            #endif
          #endif
        }
        float oldepos = current_position[E_AXIS], oldedes = destination[E_AXIS];
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS],
                         destination[E_AXIS] + (EXTRUDER_RUNOUT_EXTRUDE) * (EXTRUDER_RUNOUT_ESTEPS) / axis_steps_per_unit[E_AXIS],
                         (EXTRUDER_RUNOUT_SPEED) / 60. * (EXTRUDER_RUNOUT_ESTEPS) / axis_steps_per_unit[E_AXIS], active_extruder);
      current_position[E_AXIS] = oldepos;
      destination[E_AXIS] = oldedes;
      plan_set_e_position(oldepos);
      previous_cmd_ms = ms; // refresh_cmd_timeout()
      st_synchronize();
      switch (active_extruder) {
        case 0:
          E0_ENABLE_WRITE(oldstatus);
          break;
        #if EXTRUDERS > 1
          case 1:
            E1_ENABLE_WRITE(oldstatus);
            break;
          #if EXTRUDERS > 2
            case 2:
              E2_ENABLE_WRITE(oldstatus);
              break;
            #if EXTRUDERS > 3
              case 3:
                E3_ENABLE_WRITE(oldstatus);
                break;
            #endif
          #endif
        #endif
      }
    }
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    // handle delayed move timeout
    if (delayed_move_time && ms > delayed_move_time + 1000 && IsRunning()) {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      set_destination_to_current();
      prepare_move();
    }
  #endif

  #if ENABLED(TEMP_STAT_LEDS)
    handle_status_leds();
  #endif

  check_axes_activity();
}

void kill(const char* lcd_msg) {
  #if ENABLED(ULTRA_LCD)
    lcd_setalertstatuspgm(lcd_msg);
  #else
    UNUSED(lcd_msg);
  #endif

  cli(); // Stop interrupts
  disable_all_heaters();
  disable_all_steppers();

  #if HAS_POWER_SWITCH
    pinMode(PS_ON_PIN, INPUT);
  #endif

  SERIAL_ERROR_START;
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);

  // FMC small patch to update the LCD before ending
  sei();   // enable interrupts
  for (int i = 5; i--; lcd_update()) delay(200); // Wait a short time
  cli();   // disable interrupts
  suicide();
  while (1) {
    #if ENABLED(USE_WATCHDOG)
      watchdog_reset();
    #endif
  } // Wait for reset
}

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

  void filrunout() {
    if (!filrunoutEnqueued) {
      filrunoutEnqueued = true;
      enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
      st_synchronize();
    }
  }

#endif // FILAMENT_RUNOUT_SENSOR

#if ENABLED(FAST_PWM_FAN)

  void setPwmFrequency(uint8_t pin, int val) {
    val &= 0x07;
    switch (digitalPinToTimer(pin)) {
      #if defined(TCCR0A)
        case TIMER0A:
        case TIMER0B:
          // TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
          // TCCR0B |= val;
          break;
      #endif
      #if defined(TCCR1A)
        case TIMER1A:
        case TIMER1B:
          // TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
          // TCCR1B |= val;
          break;
      #endif
      #if defined(TCCR2)
        case TIMER2:
        case TIMER2:
          TCCR2 &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
          TCCR2 |= val;
          break;
      #endif
      #if defined(TCCR2A)
        case TIMER2A:
        case TIMER2B:
          TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));
          TCCR2B |= val;
          break;
      #endif
      #if defined(TCCR3A)
        case TIMER3A:
        case TIMER3B:
        case TIMER3C:
          TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
          TCCR3B |= val;
          break;
      #endif
      #if defined(TCCR4A)
        case TIMER4A:
        case TIMER4B:
        case TIMER4C:
          TCCR4B &= ~(_BV(CS40) | _BV(CS41) | _BV(CS42));
          TCCR4B |= val;
          break;
      #endif
      #if defined(TCCR5A)
        case TIMER5A:
        case TIMER5B:
        case TIMER5C:
          TCCR5B &= ~(_BV(CS50) | _BV(CS51) | _BV(CS52));
          TCCR5B |= val;
          break;
      #endif
    }
  }
#endif // FAST_PWM_FAN

void Stop() {
  disable_all_heaters();
  if (IsRunning()) {
    Running = false;
    Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
  }
}

/**
 * Set target_extruder from the T parameter or the active_extruder
 *
 * Returns TRUE if the target is invalid
 */
bool setTargetedHotend(int code) {
  target_extruder = active_extruder;
  if (code_seen('T')) {
    target_extruder = code_value_short();
    if (target_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      SERIAL_CHAR('M');
      SERIAL_ECHO(code);
      SERIAL_ECHOPGM(" " MSG_INVALID_EXTRUDER " ");
      SERIAL_ECHOLN((int)target_extruder);
      return true;
    }
  }
  return false;
}

float calculate_volumetric_multiplier(float diameter) {
  if (!volumetric_enabled || diameter == 0) return 1.0;
  float d2 = diameter * 0.5;
  return 1.0 / (M_PI * d2 * d2);
}

void calculate_volumetric_multipliers() {
  for (int i = 0; i < EXTRUDERS; i++)
    volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
}

/**
 * Start the print job timer
 *
 * The print job is only started if all extruders have their target temp at zero
 * otherwise the print job timew would be reset everytime a M109 is received.
 *
 * @param t start timer timestamp
 *
 * @return true if the timer was started at function call
 */
bool print_job_start(millis_t t /* = 0 */) {
  for (int i = 0; i < EXTRUDERS; i++) if (degTargetHotend(i) > 0) return false;
  print_job_start_ms = (t) ? t : millis();
  print_job_stop_ms = 0;
  return true;
}

/**
 * Check if the running print job has finished and stop the timer
 *
 * When the target temperature for all extruders is zero then we assume that the
 * print job has finished printing. There are some special conditions under which
 * this assumption may not be valid: If during a print job for some reason the
 * user decides to bring a nozzle temp down and only then heat the other afterwards.
 *
 * @param force stops the timer ignoring all pre-checks
 *
 * @return boolean true if the print job has finished printing
 */
bool print_job_stop(bool force /* = false */) {
  if (!print_job_start_ms) return false;
  if (!force) for (int i = 0; i < EXTRUDERS; i++) if (degTargetHotend(i) > 0) return false;
  print_job_stop_ms = millis();
  return true;
}

/**
 * Output the print job timer in seconds
 *
 * @return the number of seconds
 */
millis_t print_job_timer() {
  if (!print_job_start_ms) return 0;
  return (((print_job_stop_ms > print_job_start_ms)
    ? print_job_stop_ms : millis()) - print_job_start_ms) / 1000;
}
