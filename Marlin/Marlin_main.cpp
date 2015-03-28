/* -*- c++ -*- */

/*
    Reprap firmware based on Sprinter and grbl.
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 This firmware is a mashup between Sprinter and grbl.
  (https://github.com/kliment/Sprinter)
  (https://github.com/simen/grbl/tree)

 It has preliminary support for Matthew Roberts advance algorithm
    http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
 */

#include "Marlin.h"

#ifdef ENABLE_AUTO_BED_LEVELING
  #include "vector_3.h"
  #ifdef AUTO_BED_LEVELING_GRID
    #include "qr_solve.h"
  #endif
#endif // ENABLE_AUTO_BED_LEVELING

#define SERVO_LEVELING defined(ENABLE_AUTO_BED_LEVELING) && PROBE_SERVO_DEACTIVATION_DELAY > 0

#if defined(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif  // MESH_BED_LEVELING

#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"
#include "cardreader.h"
#include "watchdog.h"
#include "ConfigurationStore.h"
#include "language.h"
#include "pins_arduino.h"
#include "math.h"

#ifdef BLINKM
  #include "BlinkM.h"
  #include "Wire.h"
#endif

#if NUM_SERVOS > 0
  #include "Servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

// look here for descriptions of G-codes: http://linuxcnc.org/handbook/gcode/g-code.html
// http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes

//Implemented Codes
//-------------------
// G0  -> G1
// G1  - Coordinated Movement X Y Z E
// G2  - CW ARC
// G3  - CCW ARC
// G4  - Dwell S<seconds> or P<milliseconds>
// G10 - retract filament according to settings of M207
// G11 - retract recover filament according to settings of M208
// G28 - Home all Axis
// G29 - Detailed Z-Probe, probes the bed at 3 or more points.  Will fail if you haven't homed yet.
// G30 - Single Z Probe, probes bed at current XY location.
// G31 - Dock sled (Z_PROBE_SLED only)
// G32 - Undock sled (Z_PROBE_SLED only)
// G90 - Use Absolute Coordinates
// G91 - Use Relative Coordinates
// G92 - Set current position to coordinates given

// M Codes
// M0   - Unconditional stop - Wait for user to press a button on the LCD (Only if ULTRA_LCD is enabled)
// M1   - Same as M0
// M17  - Enable/Power all stepper motors
// M18  - Disable all stepper motors; same as M84
// M20  - List SD card
// M21  - Init SD card
// M22  - Release SD card
// M23  - Select SD file (M23 filename.g)
// M24  - Start/resume SD print
// M25  - Pause SD print
// M26  - Set SD position in bytes (M26 S12345)
// M27  - Report SD print status
// M28  - Start SD write (M28 filename.g)
// M29  - Stop SD write
// M30  - Delete file from SD (M30 filename.g)
// M31  - Output time since last M109 or SD card start to serial
// M32  - Select file and start SD print (Can be used _while_ printing from SD card files):
//        syntax "M32 /path/filename#", or "M32 S<startpos bytes> !filename#"
//        Call gcode file : "M32 P !filename#" and return to caller file after finishing (similar to #include).
//        The '#' is necessary when calling from within sd files, as it stops buffer prereading
// M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
// M80  - Turn on Power Supply
// M81  - Turn off Power Supply
// M82  - Set E codes absolute (default)
// M83  - Set E codes relative while in Absolute Coordinates (G90) mode
// M84  - Disable steppers until next move,
//        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
// M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
// M92  - Set axis_steps_per_unit - same syntax as G92
// M104 - Set extruder target temp
// M105 - Read current temp
// M106 - Fan on
// M107 - Fan off
// M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
//        Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
//        IF AUTOTEMP is enabled, S<mintemp> B<maxtemp> F<factor>. Exit autotemp by any M109 without F
// M112 - Emergency stop
// M114 - Output current position to serial port
// M115 - Capabilities string
// M117 - display message
// M119 - Output Endstop status to serial port
// M120 - Enable endstop detection
// M121 - Disable endstop detection
// M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
// M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
// M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
// M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
// M140 - Set bed target temp
// M150 - Set BlinkM Color Output R: Red<0-255> U(!): Green<0-255> B: Blue<0-255> over i2c, G for green does not work.
// M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
//        Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
// M200 D<millimeters>- set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
// M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
// M204 - Set default acceleration: P for Printing moves, R for Retract only (no X, Y, Z) moves and T for Travel (non printing) moves (ex. M204 P800 T3000 R9000) in mm/sec^2
// M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
// M206 - Set additional homing offset
// M207 - Set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop], stays in mm regardless of M200 setting
// M208 - Set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/sec]
// M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
// M218 - Set hotend offset (in mm): T<extruder_number> X<offset_on_X> Y<offset_on_Y>
// M220 S<factor in percent>- set speed factor override percentage
// M221 S<factor in percent>- set extrude factor override percentage
// M226 P<pin number> S<pin state>- Wait until the specified pin reaches the state required
// M240 - Trigger a camera to take a photograph
// M250 - Set LCD contrast C<contrast value> (value 0..63)
// M280 - Set servo position absolute. P: servo index, S: angle or microseconds
// M300 - Play beep sound S<frequency Hz> P<duration ms>
// M301 - Set PID parameters P I and D
// M302 - Allow cold extrudes, or set the minimum extrude S<temperature>.
// M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
// M304 - Set bed PID parameters P I and D
// M380 - Activate solenoid on active extruder
// M381 - Disable all solenoids
// M400 - Finish all moves
// M401 - Lower z-probe if present
// M402 - Raise z-probe if present
// M404 - N<dia in mm> Enter the nominal filament width (3mm, 1.75mm ) or will display nominal filament width without parameters
// M405 - Turn on Filament Sensor extrusion control.  Optional D<delay in cm> to set delay in centimeters between sensor and extruder 
// M406 - Turn off Filament Sensor extrusion control 
// M407 - Displays measured filament diameter 
// M500 - Store parameters in EEPROM
// M501 - Read parameters from EEPROM (if you need reset them after you changed them temporarily).
// M502 - Revert to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
// M503 - Print the current settings (from memory not from EEPROM). Use S0 to leave off headings.
// M540 - Use S[0|1] to enable or disable the stop SD card print on endstop hit (requires ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
// M600 - Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
// M665 - Set delta configurations
// M666 - Set delta endstop adjustment
// M605 - Set dual x-carriage movement mode: S<mode> [ X<duplication x-offset> R<duplication temp offset> ]
// M907 - Set digital trimpot motor current using axis codes.
// M908 - Control digital trimpot directly.
// M350 - Set microstepping mode.
// M351 - Toggle MS1 MS2 pins directly.

// ************ SCARA Specific - This can change to suit future G-code regulations
// M360 - SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
// M361 - SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
// M362 - SCARA calibration: Move to cal-position PsiA (0 deg calibration)
// M363 - SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
// M364 - SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
// M365 - SCARA calibration: Scaling factor, X, Y, Z axis
//************* SCARA End ***************

// M928 - Start SD logging (M928 filename.g) - ended by M29
// M999 - Restart after being stopped by error

#ifdef SDSUPPORT
  CardReader card;
#endif

float homing_feedrate[] = HOMING_FEEDRATE;
#ifdef ENABLE_AUTO_BED_LEVELING
  int xy_travel_speed = XY_TRAVEL_SPEED;
  float zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif
int homing_bump_divisor[] = HOMING_BUMP_DIVISOR;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedmultiply = 100; //100->1 200->2
int saved_feedmultiply;
int extrudemultiply = 100; //100->1 200->2
int extruder_multiply[EXTRUDERS] = { 100
  #if EXTRUDERS > 1
    , 100
    #if EXTRUDERS > 2
      , 100
      #if EXTRUDERS > 3
        , 100
      #endif
    #endif
  #endif
};
bool volumetric_enabled = false;
float filament_size[EXTRUDERS] = { DEFAULT_NOMINAL_FILAMENT_DIA
  #if EXTRUDERS > 1
      , DEFAULT_NOMINAL_FILAMENT_DIA
    #if EXTRUDERS > 2
       , DEFAULT_NOMINAL_FILAMENT_DIA
      #if EXTRUDERS > 3
        , DEFAULT_NOMINAL_FILAMENT_DIA
      #endif
    #endif
  #endif
};
float volumetric_multiplier[EXTRUDERS] = {1.0
  #if EXTRUDERS > 1
    , 1.0
    #if EXTRUDERS > 2
      , 1.0
      #if EXTRUDERS > 3
        , 1.0
      #endif
    #endif
  #endif
};
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float home_offset[3] = { 0, 0, 0 };
#ifdef DELTA
  float endstop_adj[3] = { 0, 0, 0 };
#elif defined(Z_DUAL_ENDSTOPS)
  float z_endstop_adj = 0;
#endif

float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
bool axis_known_position[3] = { false, false, false };

// Extruder offset
#if EXTRUDERS > 1
#ifndef DUAL_X_CARRIAGE
  #define NUM_EXTRUDER_OFFSETS 2 // only in XY plane
#else
  #define NUM_EXTRUDER_OFFSETS 3 // supports offsets in XYZ plane
#endif
float extruder_offset[NUM_EXTRUDER_OFFSETS][EXTRUDERS] = {
  #if defined(EXTRUDER_OFFSET_X)
    EXTRUDER_OFFSET_X
  #else
    0
  #endif
  ,
  #if defined(EXTRUDER_OFFSET_Y)
    EXTRUDER_OFFSET_Y
  #else
    0
  #endif
};
#endif

uint8_t active_extruder = 0;
int fanSpeed = 0;

#ifdef SERVO_ENDSTOPS
  int servo_endstops[] = SERVO_ENDSTOPS;
  int servo_endstop_angles[] = SERVO_ENDSTOP_ANGLES;
#endif

#ifdef BARICUDA
  int ValvePressure = 0;
  int EtoPPressure = 0;
#endif

#ifdef FWRETRACT

  bool autoretract_enabled = false;
  bool retracted[EXTRUDERS] = { false
    #if EXTRUDERS > 1
      , false
      #if EXTRUDERS > 2
        , false
        #if EXTRUDERS > 3
          , false
        #endif
      #endif
    #endif
  };
  bool retracted_swap[EXTRUDERS] = { false
    #if EXTRUDERS > 1
      , false
      #if EXTRUDERS > 2
        , false
        #if EXTRUDERS > 3
          , false
        #endif
      #endif
    #endif
  };

  float retract_length = RETRACT_LENGTH;
  float retract_length_swap = RETRACT_LENGTH_SWAP;
  float retract_feedrate = RETRACT_FEEDRATE;
  float retract_zlift = RETRACT_ZLIFT;
  float retract_recover_length = RETRACT_RECOVER_LENGTH;
  float retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
  float retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;

#endif // FWRETRACT

#ifdef ULTIPANEL
  bool powersupply = 
    #ifdef PS_DEFAULT_OFF
      false
    #else
      true
    #endif
  ;
#endif

#ifdef DELTA
  float delta[3] = { 0, 0, 0 };
  #define SIN_60 0.8660254037844386
  #define COS_60 0.5
  // these are the default values, can be overriden with M665
  float delta_radius = DELTA_RADIUS;
  float delta_tower1_x = -SIN_60 * delta_radius; // front left tower
  float delta_tower1_y = -COS_60 * delta_radius;     
  float delta_tower2_x =  SIN_60 * delta_radius; // front right tower
  float delta_tower2_y = -COS_60 * delta_radius;     
  float delta_tower3_x = 0;                      // back middle tower
  float delta_tower3_y = delta_radius;
  float delta_diagonal_rod = DELTA_DIAGONAL_ROD;
  float delta_diagonal_rod_2 = sq(delta_diagonal_rod);
  float delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
  #ifdef ENABLE_AUTO_BED_LEVELING
    float bed_level[AUTO_BED_LEVELING_GRID_POINTS][AUTO_BED_LEVELING_GRID_POINTS];
  #endif
#endif

#ifdef SCARA
  float axis_scaling[3] = { 1, 1, 1 };    // Build size scaling, default to 1
  static float delta[3] = { 0, 0, 0 };		
#endif        

bool cancel_heatup = false;

#ifdef FILAMENT_SENSOR
  //Variables for Filament Sensor input 
  float filament_width_nominal=DEFAULT_NOMINAL_FILAMENT_DIA;  //Set nominal filament width, can be changed with M404 
  bool filament_sensor=false;  //M405 turns on filament_sensor control, M406 turns it off 
  float filament_width_meas=DEFAULT_MEASURED_FILAMENT_DIA; //Stores the measured filament diameter 
  signed char measurement_delay[MAX_MEASUREMENT_DELAY+1];  //ring buffer to delay measurement  store extruder factor after subtracting 100 
  int delay_index1=0;  //index into ring buffer
  int delay_index2=-1;  //index into ring buffer - set to -1 on startup to indicate ring buffer needs to be initialized
  float delay_dist=0; //delay distance counter  
  int meas_delay_cm = MEASUREMENT_DELAY_CM;  //distance delay setting
#endif

#ifdef FILAMENT_RUNOUT_SENSOR
   static bool filrunoutEnqued = false;
#endif

const char errormagic[] PROGMEM = "Error:";
const char echomagic[] PROGMEM = "echo:";

const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};
static float destination[NUM_AXIS] = { 0, 0, 0, 0 };

static float offset[3] = { 0, 0, 0 };

#ifndef DELTA
  static bool home_all_axis = true;
#endif

static float feedrate = 1500.0, next_feedrate, saved_feedrate;
static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

static bool relative_mode = false;  //Determines Absolute or Relative Coordinates

static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
#ifdef SDSUPPORT
  static bool fromsd[BUFSIZE];
#endif
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;

static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *strchr_pointer; ///< A pointer to find chars in the command string (X, Y, Z, E, etc.)

const char* queued_commands_P= NULL; /* pointer to the current line in the active sequence of commands, or NULL when none */

const int sensitive_pins[] = SENSITIVE_PINS; ///< Sensitive pin list for M42

// Inactivity shutdown
static unsigned long previous_millis_cmd = 0;
static unsigned long max_inactive_time = 0;
static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000l;

unsigned long starttime = 0; ///< Print job start time
unsigned long stoptime = 0;  ///< Print job stop time

static uint8_t tmp_extruder;


bool Stopped = false;

#if NUM_SERVOS > 0
  Servo servos[NUM_SERVOS];
#endif

bool CooldownNoWait = true;
bool target_direction;

#ifdef CHDK
  unsigned long chdkHigh = 0;
  boolean chdkActive = false;
#endif

//===========================================================================
//=============================Routines======================================
//===========================================================================

void get_arc_coordinates();
bool setTargetedHotend(int code);

void serial_echopair_P(const char *s_P, float v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, double v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, unsigned long v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }

#ifdef SDSUPPORT
  #include "SdFatUtil.h"
  int freeMemory() { return SdFatUtil::FreeRam(); }
#else
  extern "C" {
    extern unsigned int __bss_end;
    extern unsigned int __heap_start;
    extern void *__brkval;

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

//Injects the next command from the pending sequence of commands, when possible
//Return false if and only if no command was pending
static bool drain_queued_commands_P()
{
  char cmd[30];
  if(!queued_commands_P)
    return false;
  // Get the next 30 chars from the sequence of gcodes to run
  strncpy_P(cmd, queued_commands_P, sizeof(cmd)-1);
  cmd[sizeof(cmd)-1]= 0;
  // Look for the end of line, or the end of sequence
  size_t i= 0;
  char c;
  while( (c= cmd[i]) && c!='\n' )
    ++i; // look for the end of this gcode command
  cmd[i]= 0;
  if(enquecommand(cmd)) // buffer was not full (else we will retry later)
  {
    if(c)
      queued_commands_P+= i+1; // move to next command
    else
      queued_commands_P= NULL; // will have no more commands in the sequence
  }
  return true;
}

//Record one or many commands to run from program memory.
//Aborts the current queue, if any.
//Note: drain_queued_commands_P() must be called repeatedly to drain the commands afterwards
void enquecommands_P(const char* pgcode)
{
    queued_commands_P= pgcode;
    drain_queued_commands_P(); // first command exectuted asap (when possible)
}

//adds a single command to the main command buffer, from RAM
//that is really done in a non-safe way.
//needs overworking someday
//Returns false if it failed to do so
bool enquecommand(const char *cmd)
{
  if(*cmd==';')
    return false;
  if(buflen >= BUFSIZE)
    return false;
  //this is dangerous if a mixing of serial and this happens
  strcpy(&(cmdbuffer[bufindw][0]),cmd);
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_Enqueing);
  SERIAL_ECHO(cmdbuffer[bufindw]);
  SERIAL_ECHOLNPGM("\"");
  bufindw= (bufindw + 1)%BUFSIZE;
  buflen += 1;
  return true;
}



void setup_killpin()
{
  #if defined(KILL_PIN) && KILL_PIN > -1
    SET_INPUT(KILL_PIN);
    WRITE(KILL_PIN,HIGH);
  #endif
}

void setup_filrunoutpin()
{
#if defined(FILRUNOUT_PIN) && FILRUNOUT_PIN > -1
   pinMode(FILRUNOUT_PIN,INPUT);
   #if defined(ENDSTOPPULLUP_FIL_RUNOUT)
      WRITE(FILLRUNOUT_PIN,HIGH);
   #endif
#endif
}

// Set home pin
void setup_homepin(void)
{
#if defined(HOME_PIN) && HOME_PIN > -1
   SET_INPUT(HOME_PIN);
   WRITE(HOME_PIN,HIGH);
#endif
}


void setup_photpin()
{
  #if defined(PHOTOGRAPH_PIN) && PHOTOGRAPH_PIN > -1
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif
}

void setup_powerhold()
{
  #if defined(SUICIDE_PIN) && SUICIDE_PIN > -1
    OUT_WRITE(SUICIDE_PIN, HIGH);
  #endif
  #if defined(PS_ON_PIN) && PS_ON_PIN > -1
    #if defined(PS_DEFAULT_OFF)
      OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
    #else
      OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE);
    #endif
  #endif
}

void suicide()
{
  #if defined(SUICIDE_PIN) && SUICIDE_PIN > -1
    OUT_WRITE(SUICIDE_PIN, LOW);
  #endif
}

void servo_init()
{
  #if (NUM_SERVOS >= 1) && defined(SERVO0_PIN) && (SERVO0_PIN > -1)
    servos[0].attach(SERVO0_PIN);
  #endif
  #if (NUM_SERVOS >= 2) && defined(SERVO1_PIN) && (SERVO1_PIN > -1)
    servos[1].attach(SERVO1_PIN);
  #endif
  #if (NUM_SERVOS >= 3) && defined(SERVO2_PIN) && (SERVO2_PIN > -1)
    servos[2].attach(SERVO2_PIN);
  #endif
  #if (NUM_SERVOS >= 4) && defined(SERVO3_PIN) && (SERVO3_PIN > -1)
    servos[3].attach(SERVO3_PIN);
  #endif
  #if (NUM_SERVOS >= 5)
    #error "TODO: enter initalisation code for more servos"
  #endif

  // Set position of Servo Endstops that are defined
  #ifdef SERVO_ENDSTOPS
  for(int8_t i = 0; i < 3; i++)
  {
    if(servo_endstops[i] > -1) {
      servos[servo_endstops[i]].write(servo_endstop_angles[i * 2 + 1]);
    }
  }
  #endif

  #if SERVO_LEVELING
    delay(PROBE_SERVO_DEACTIVATION_DELAY);
    servos[servo_endstops[Z_AXIS]].detach();
  #endif
}


void setup()
{
  setup_killpin();
  setup_filrunoutpin();
  setup_powerhold();
  MYSERIAL.begin(BAUDRATE);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START;

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = MCUSR;
  if(mcu & 1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if(mcu & 2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if(mcu & 4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if(mcu & 8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if(mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  MCUSR=0;

  SERIAL_ECHOPGM(MSG_MARLIN);
  SERIAL_ECHOLNPGM(STRING_VERSION);
  #ifdef STRING_VERSION_CONFIG_H
    #ifdef STRING_CONFIG_H_AUTHOR
      SERIAL_ECHO_START;
      SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
      SERIAL_ECHOPGM(STRING_VERSION_CONFIG_H);
      SERIAL_ECHOPGM(MSG_AUTHOR);
      SERIAL_ECHOLNPGM(STRING_CONFIG_H_AUTHOR);
      SERIAL_ECHOPGM("Compiled: ");
      SERIAL_ECHOLNPGM(__DATE__);
    #endif // STRING_CONFIG_H_AUTHOR
  #endif // STRING_VERSION_CONFIG_H
  SERIAL_ECHO_START;
  SERIAL_ECHOPGM(MSG_FREE_MEMORY);
  SERIAL_ECHO(freeMemory());
  SERIAL_ECHOPGM(MSG_PLANNER_BUFFER_BYTES);
  SERIAL_ECHOLN((int)sizeof(block_t)*BLOCK_BUFFER_SIZE);
  #ifdef SDSUPPORT
  for(int8_t i = 0; i < BUFSIZE; i++)
  {
    fromsd[i] = false;
  }
  #endif //!SDSUPPORT

  // loads data from EEPROM if available else uses defaults (and resets step acceleration rate)
  Config_RetrieveSettings();

  tp_init();    // Initialize temperature loop
  plan_init();  // Initialize planner;
  watchdog_init();
  st_init();    // Initialize stepper, this enables interrupts!
  setup_photpin();
  servo_init();
  

  lcd_init();
  _delay_ms(1000);  // wait 1sec to display the splash screen

  #if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1
    SET_OUTPUT(CONTROLLERFAN_PIN); //Set pin used for driver cooling fan
  #endif

  #ifdef DIGIPOT_I2C
    digipot_i2c_init();
  #endif
#ifdef Z_PROBE_SLED
  pinMode(SERVO0_PIN, OUTPUT);
  digitalWrite(SERVO0_PIN, LOW); // turn it off
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


void loop()
{
  if(buflen < (BUFSIZE-1))
    get_command();
  #ifdef SDSUPPORT
  card.checkautostart(false);
  #endif
  if(buflen)
  {
    #ifdef SDSUPPORT
      if(card.saving)
      {
        if(strstr_P(cmdbuffer[bufindr], PSTR("M29")) == NULL)
        {
          card.write_command(cmdbuffer[bufindr]);
          if(card.logging)
          {
            process_commands();
          }
          else
          {
            SERIAL_PROTOCOLLNPGM(MSG_OK);
          }
        }
        else
        {
          card.closefile();
          SERIAL_PROTOCOLLNPGM(MSG_FILE_SAVED);
        }
      }
      else
      {
        process_commands();
      }
    #else
      process_commands();
    #endif //SDSUPPORT
    buflen = (buflen-1);
    bufindr = (bufindr + 1)%BUFSIZE;
  }
  //check heater every n milliseconds
  manage_heater();
  manage_inactivity();
  checkHitEndstops();
  lcd_update();
}

void get_command()
{
  if(drain_queued_commands_P()) // priority is given to non-serial commands
    return;
  
  while( MYSERIAL.available() > 0  && buflen < BUFSIZE) {
    serial_char = MYSERIAL.read();
    if(serial_char == '\n' ||
       serial_char == '\r' ||
       serial_count >= (MAX_CMD_SIZE - 1) )
    {
      // end of line == end of comment
      comment_mode = false;

      if(!serial_count) {
        // short cut for empty lines
        return;
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      #ifdef SDSUPPORT
      fromsd[bufindw] = false;
      #endif //!SDSUPPORT
      if(strchr(cmdbuffer[bufindw], 'N') != NULL)
      {
        strchr_pointer = strchr(cmdbuffer[bufindw], 'N');
        gcode_N = (strtol(strchr_pointer + 1, NULL, 10));
        if(gcode_N != gcode_LastN+1 && (strstr_P(cmdbuffer[bufindw], PSTR("M110")) == NULL) ) {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_LINE_NO);
          SERIAL_ERRORLN(gcode_LastN);
          //Serial.println(gcode_N);
          FlushSerialRequestResend();
          serial_count = 0;
          return;
        }

        if(strchr(cmdbuffer[bufindw], '*') != NULL)
        {
          byte checksum = 0;
          byte count = 0;
          while(cmdbuffer[bufindw][count] != '*') checksum = checksum^cmdbuffer[bufindw][count++];
          strchr_pointer = strchr(cmdbuffer[bufindw], '*');

          if(strtol(strchr_pointer + 1, NULL, 10) != checksum) {
            SERIAL_ERROR_START;
            SERIAL_ERRORPGM(MSG_ERR_CHECKSUM_MISMATCH);
            SERIAL_ERRORLN(gcode_LastN);
            FlushSerialRequestResend();
            serial_count = 0;
            return;
          }
          //if no errors, continue parsing
        }
        else
        {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_NO_CHECKSUM);
          SERIAL_ERRORLN(gcode_LastN);
          FlushSerialRequestResend();
          serial_count = 0;
          return;
        }

        gcode_LastN = gcode_N;
        //if no errors, continue parsing
      }
      else  // if we don't receive 'N' but still see '*'
      {
        if((strchr(cmdbuffer[bufindw], '*') != NULL))
        {
          SERIAL_ERROR_START;
          SERIAL_ERRORPGM(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM);
          SERIAL_ERRORLN(gcode_LastN);
          serial_count = 0;
          return;
        }
      }
      if((strchr(cmdbuffer[bufindw], 'G') != NULL)){
        strchr_pointer = strchr(cmdbuffer[bufindw], 'G');
        switch(strtol(strchr_pointer + 1, NULL, 10)){
        case 0:
        case 1:
        case 2:
        case 3:
          if (Stopped == true) {
            SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
            LCD_MESSAGEPGM(MSG_STOPPED);
          }
          break;
        default:
          break;
        }

      }

      //If command was e-stop process now
      if(strcmp(cmdbuffer[bufindw], "M112") == 0)
        kill();

      bufindw = (bufindw + 1)%BUFSIZE;
      buflen += 1;

      serial_count = 0; //clear buffer
    }
    else if(serial_char == '\\') {  //Handle escapes
       
        if(MYSERIAL.available() > 0  && buflen < BUFSIZE) {
            // if we have one more character, copy it over
            serial_char = MYSERIAL.read();
            cmdbuffer[bufindw][serial_count++] = serial_char;
        }

        //otherwise do nothing        
    }
    else { // its not a newline, carriage return or escape char
        if(serial_char == ';') comment_mode = true;
        if(!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char;
    }
  }
  #ifdef SDSUPPORT
  if(!card.sdprinting || serial_count!=0){
    return;
  }

  //'#' stops reading from SD to the buffer prematurely, so procedural macro calls are possible
  // if it occurs, stop_buffering is triggered and the buffer is ran dry.
  // this character _can_ occur in serial com, due to checksums. however, no checksums are used in SD printing

  static bool stop_buffering=false;
  if(buflen==0) stop_buffering=false;

  while( !card.eof()  && buflen < BUFSIZE && !stop_buffering) {
    int16_t n=card.get();
    serial_char = (char)n;
    if(serial_char == '\n' ||
       serial_char == '\r' ||
       (serial_char == '#' && comment_mode == false) ||
       (serial_char == ':' && comment_mode == false) ||
       serial_count >= (MAX_CMD_SIZE - 1)||n==-1)
    {
      if(card.eof()){
        SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
        stoptime=millis();
        char time[30];
        unsigned long t=(stoptime-starttime)/1000;
        int hours, minutes;
        minutes=(t/60)%60;
        hours=t/60/60;
        sprintf_P(time, PSTR("%i hours %i minutes"),hours, minutes);
        SERIAL_ECHO_START;
        SERIAL_ECHOLN(time);
        lcd_setstatus(time);
        card.printingHasFinished();
        card.checkautostart(true);

      }
      if(serial_char=='#')
        stop_buffering=true;

      if(!serial_count)
      {
        comment_mode = false; //for new command
        return; //if empty line
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
//      if(!comment_mode){
        fromsd[bufindw] = true;
        buflen += 1;
        bufindw = (bufindw + 1)%BUFSIZE;
//      }
      comment_mode = false; //for new command
      serial_count = 0; //clear buffer
    }
    else
    {
      if(serial_char == ';') comment_mode = true;
      if(!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char;
    }
  }

  #endif //SDSUPPORT

}

float code_value() {
  float ret;
  char *e = strchr(strchr_pointer, 'E');
  if (e) {
    *e = 0;
    ret = strtod(strchr_pointer+1, NULL);
    *e = 'E';
  }
  else
    ret = strtod(strchr_pointer+1, NULL);
  return ret;
}

long code_value_long() { return (strtol(strchr_pointer + 1, NULL, 10)); }

bool code_seen(char code) {
  strchr_pointer = strchr(cmdbuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
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

XYZ_CONSTS_FROM_CONFIG(float, base_min_pos,    MIN_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_max_pos,    MAX_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_home_pos,   HOME_POS);
XYZ_CONSTS_FROM_CONFIG(float, max_length,      MAX_LENGTH);
XYZ_CONSTS_FROM_CONFIG(float, home_retract_mm, HOME_RETRACT_MM);
XYZ_CONSTS_FROM_CONFIG(signed char, home_dir,  HOME_DIR);

#ifdef DUAL_X_CARRIAGE

  #define DXC_FULL_CONTROL_MODE 0
  #define DXC_AUTO_PARK_MODE    1
  #define DXC_DUPLICATION_MODE  2

  static int dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;

  static float x_home_pos(int extruder) {
    if (extruder == 0)
    return base_home_pos(X_AXIS) + home_offset[X_AXIS];
    else
      // In dual carriage mode the extruder offset provides an override of the
      // second X-carriage offset when homed - otherwise X2_HOME_POS is used.
      // This allow soft recalibration of the second extruder offset position without firmware reflash
      // (through the M218 command).
      return (extruder_offset[X_AXIS][1] > 0) ? extruder_offset[X_AXIS][1] : X2_HOME_POS;
  }

  static int x_home_dir(int extruder) {
    return (extruder == 0) ? X_HOME_DIR : X2_HOME_DIR;
  }

  static float inactive_extruder_x_pos = X2_MAX_POS; // used in mode 0 & 1
  static bool active_extruder_parked = false; // used in mode 1 & 2
  static float raised_parked_position[NUM_AXIS]; // used in mode 1
  static unsigned long delayed_move_time = 0; // used in mode 1
  static float duplicate_extruder_x_offset = DEFAULT_DUPLICATION_X_OFFSET; // used in mode 2
  static float duplicate_extruder_temp_offset = 0; // used in mode 2
  bool extruder_duplication_enabled = false; // used in mode 2

#endif //DUAL_X_CARRIAGE

static void axis_is_at_home(int axis) {

  #ifdef DUAL_X_CARRIAGE
    if (axis == X_AXIS) {
      if (active_extruder != 0) {
        current_position[X_AXIS] = x_home_pos(active_extruder);
        min_pos[X_AXIS] = X2_MIN_POS;
        max_pos[X_AXIS] = max(extruder_offset[X_AXIS][1], X2_MAX_POS);
        return;
      }
      else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
        current_position[X_AXIS] = base_home_pos(X_AXIS) + home_offset[X_AXIS];
        min_pos[X_AXIS] = base_min_pos(X_AXIS) + home_offset[X_AXIS];
        max_pos[X_AXIS] = min(base_max_pos(X_AXIS) + home_offset[X_AXIS],
                                max(extruder_offset[X_AXIS][1], X2_MAX_POS) - duplicate_extruder_x_offset);
        return;
      }
    }
  #endif

  #ifdef SCARA
    float homeposition[3];
   
    if (axis < 2) {

      for (int i = 0; i < 3; i++) homeposition[i] = base_home_pos(i);

      // SERIAL_ECHOPGM("homeposition[x]= "); SERIAL_ECHO(homeposition[0]);
      // SERIAL_ECHOPGM("homeposition[y]= "); SERIAL_ECHOLN(homeposition[1]);
      // Works out real Homeposition angles using inverse kinematics, 
      // and calculates homing offset using forward kinematics
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
    
      // SCARA home positions are based on configuration since the actual limits are determined by the 
      // inverse kinematic transform.
      min_pos[axis] = base_min_pos(axis); // + (delta[axis] - base_home_pos(axis));
      max_pos[axis] = base_max_pos(axis); // + (delta[axis] - base_home_pos(axis));
    } 
    else {
      current_position[axis] = base_home_pos(axis) + home_offset[axis];
      min_pos[axis] = base_min_pos(axis) + home_offset[axis];
      max_pos[axis] = base_max_pos(axis) + home_offset[axis];
    }
  #else
    current_position[axis] = base_home_pos(axis) + home_offset[axis];
    min_pos[axis] = base_min_pos(axis) + home_offset[axis];
    max_pos[axis] = base_max_pos(axis) + home_offset[axis];
  #endif
}

#ifdef ENABLE_AUTO_BED_LEVELING
#ifdef AUTO_BED_LEVELING_GRID

#ifndef DELTA
static void set_bed_level_equation_lsq(double *plane_equation_coefficients)
{
    vector_3 planeNormal = vector_3(-plane_equation_coefficients[0], -plane_equation_coefficients[1], 1);
    planeNormal.debug("planeNormal");
    plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
    //bedLevel.debug("bedLevel");

    //plan_bed_level_matrix.debug("bed level before");
    //vector_3 uncorrected_position = plan_get_position_mm();
    //uncorrected_position.debug("position before");

    vector_3 corrected_position = plan_get_position();
//    corrected_position.debug("position after");
    current_position[X_AXIS] = corrected_position.x;
    current_position[Y_AXIS] = corrected_position.y;
    current_position[Z_AXIS] = corrected_position.z;

    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
#endif

#else // not AUTO_BED_LEVELING_GRID

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

    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}

#endif // AUTO_BED_LEVELING_GRID

static void run_z_probe() {
  #ifdef DELTA
    
    float start_z = current_position[Z_AXIS];
    long start_steps = st_get_position(Z_AXIS);
  
    // move down slowly until you find the bed
    feedrate = homing_feedrate[Z_AXIS] / 4;
    destination[Z_AXIS] = -10;
    prepare_move_raw();
    st_synchronize();
    endstops_hit_on_purpose();
    
    // we have to let the planner know where we are right now as it is not where we said to go.
    long stop_steps = st_get_position(Z_AXIS);
    float mm = start_z - float(start_steps - stop_steps) / axis_steps_per_unit[Z_AXIS];
    current_position[Z_AXIS] = mm;
    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
    
  #else

    plan_bed_level_matrix.set_to_identity();
    feedrate = homing_feedrate[Z_AXIS];

    // move down until you find the bed
    float zPosition = -10;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

        // we have to let the planner know where we are right now as it is not where we said to go.
    zPosition = st_get_position_mm(Z_AXIS);
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS]);

    // move up the retract distance
    zPosition += home_retract_mm(Z_AXIS);
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();
    endstops_hit_on_purpose();

    // move back down slowly to find bed
    
    if (homing_bump_divisor[Z_AXIS] >= 1)
    {
        feedrate = homing_feedrate[Z_AXIS]/homing_bump_divisor[Z_AXIS];
    } 
    else
    {
        feedrate = homing_feedrate[Z_AXIS]/10;
        SERIAL_ECHOLN("Warning: The Homing Bump Feedrate Divisor cannot be less then 1");
    }

    
    zPosition -= home_retract_mm(Z_AXIS) * 2;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();
    endstops_hit_on_purpose();

    current_position[Z_AXIS] = st_get_position_mm(Z_AXIS);
    // make sure the planner knows where we are as it may be a bit different than we last said to move to
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    
  #endif
}

static void do_blocking_move_to(float x, float y, float z) {
    float oldFeedRate = feedrate;

#ifdef DELTA

    feedrate = XY_TRAVEL_SPEED;
    
    destination[X_AXIS] = x;
    destination[Y_AXIS] = y;
    destination[Z_AXIS] = z;
    prepare_move_raw();
    st_synchronize();

#else

    feedrate = homing_feedrate[Z_AXIS];

    current_position[Z_AXIS] = z;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    feedrate = xy_travel_speed;

    current_position[X_AXIS] = x;
    current_position[Y_AXIS] = y;
    plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

#endif

    feedrate = oldFeedRate;
}

static void setup_for_endstop_move() {
    saved_feedrate = feedrate;
    saved_feedmultiply = feedmultiply;
    feedmultiply = 100;
    previous_millis_cmd = millis();

    enable_endstops(true);
}

static void clean_up_after_endstop_move() {
#ifdef ENDSTOPS_ONLY_FOR_HOMING
    enable_endstops(false);
#endif

    feedrate = saved_feedrate;
    feedmultiply = saved_feedmultiply;
    previous_millis_cmd = millis();
}

static void engage_z_probe() {
  // Engage Z Servo endstop if enabled
  #ifdef SERVO_ENDSTOPS
    if (servo_endstops[Z_AXIS] > -1) {
      #if SERVO_LEVELING
        servos[servo_endstops[Z_AXIS]].attach(0);
      #endif
      servos[servo_endstops[Z_AXIS]].write(servo_endstop_angles[Z_AXIS * 2]);
      #if SERVO_LEVELING
        delay(PROBE_SERVO_DEACTIVATION_DELAY);
        servos[servo_endstops[Z_AXIS]].detach();
      #endif
    }
  #elif defined(Z_PROBE_ALLEN_KEY)
    feedrate = homing_feedrate[X_AXIS];
    
    // Move to the start position to initiate deployment
    destination[X_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_X;
    destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_Y;
    destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_Z;
    prepare_move_raw();

    // Home X to touch the belt
    feedrate = homing_feedrate[X_AXIS]/10;
    destination[X_AXIS] = 0;
    prepare_move_raw();
    
    // Home Y for safety
    feedrate = homing_feedrate[X_AXIS]/2;
    destination[Y_AXIS] = 0;
    prepare_move_raw();
    
    st_synchronize();
    
    bool z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
    if (z_min_endstop)
    {
        if (!Stopped)
        {
            SERIAL_ERROR_START;
            SERIAL_ERRORLNPGM("Z-Probe failed to engage!");
            LCD_ALERTMESSAGEPGM("Err: ZPROBE");
        }
        Stop();
    }
  #endif

}

static void retract_z_probe() {
  // Retract Z Servo endstop if enabled
  #ifdef SERVO_ENDSTOPS
    if (servo_endstops[Z_AXIS] > -1)
    {
      #if Z_RAISE_AFTER_PROBING > 0
        do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], Z_RAISE_AFTER_PROBING);
        st_synchronize();
      #endif
    
      #if SERVO_LEVELING
        servos[servo_endstops[Z_AXIS]].attach(0);
      #endif
      servos[servo_endstops[Z_AXIS]].write(servo_endstop_angles[Z_AXIS * 2 + 1]);
      #if SERVO_LEVELING
        delay(PROBE_SERVO_DEACTIVATION_DELAY);
        servos[servo_endstops[Z_AXIS]].detach();
      #endif
    }
  #elif defined(Z_PROBE_ALLEN_KEY)
    // Move up for safety
    feedrate = homing_feedrate[X_AXIS];
    destination[Z_AXIS] = current_position[Z_AXIS] + Z_RAISE_AFTER_PROBING;
    prepare_move_raw();

    // Move to the start position to initiate retraction
    destination[X_AXIS] = Z_PROBE_ALLEN_KEY_RETRACT_X;
    destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_RETRACT_Y;
    destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_RETRACT_Z;
    prepare_move_raw();

    // Move the nozzle down to push the probe into retracted position
    feedrate = homing_feedrate[Z_AXIS]/10;
    destination[Z_AXIS] = current_position[Z_AXIS] - Z_PROBE_ALLEN_KEY_RETRACT_DEPTH;
    prepare_move_raw();
    
    // Move up for safety
    feedrate = homing_feedrate[Z_AXIS]/2;
    destination[Z_AXIS] = current_position[Z_AXIS] + Z_PROBE_ALLEN_KEY_RETRACT_DEPTH * 2;
    prepare_move_raw();
    
    // Home XY for safety
    feedrate = homing_feedrate[X_AXIS]/2;
    destination[X_AXIS] = 0;
    destination[Y_AXIS] = 0;
    prepare_move_raw();
    
    st_synchronize();
    
    bool z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
    if (!z_min_endstop)
    {
        if (!Stopped)
        {
            SERIAL_ERROR_START;
            SERIAL_ERRORLNPGM("Z-Probe failed to retract!");
            LCD_ALERTMESSAGEPGM("Err: ZPROBE");
        }
        Stop();
    }
  #endif

}

enum ProbeAction {
  ProbeStay             = 0,
  ProbeEngage           = BIT(0),
  ProbeRetract          = BIT(1),
  ProbeEngageAndRetract = (ProbeEngage | ProbeRetract)
};

/// Probe bed height at position (x,y), returns the measured z value
static float probe_pt(float x, float y, float z_before, ProbeAction retract_action=ProbeEngageAndRetract, int verbose_level=1) {
  // move to right place
  do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], z_before);
  do_blocking_move_to(x - X_PROBE_OFFSET_FROM_EXTRUDER, y - Y_PROBE_OFFSET_FROM_EXTRUDER, current_position[Z_AXIS]);

  #if !defined(Z_PROBE_SLED) && !defined(Z_PROBE_ALLEN_KEY)
    if (retract_action & ProbeEngage) engage_z_probe();
  #endif

  run_z_probe();
  float measured_z = current_position[Z_AXIS];

  #if !defined(Z_PROBE_SLED) && !defined(Z_PROBE_ALLEN_KEY)
    if (retract_action & ProbeRetract) retract_z_probe();
  #endif

  if (verbose_level > 2) {
    SERIAL_PROTOCOLPGM(MSG_BED);
    SERIAL_PROTOCOLPGM(" X: ");
    SERIAL_PROTOCOL_F(x, 3);
    SERIAL_PROTOCOLPGM(" Y: ");
    SERIAL_PROTOCOL_F(y, 3);
    SERIAL_PROTOCOLPGM(" Z: ");
    SERIAL_PROTOCOL_F(measured_z, 3);
    SERIAL_EOL;
  }
  return measured_z;
}

#ifdef DELTA
static void extrapolate_one_point(int x, int y, int xdir, int ydir) {
  if (bed_level[x][y] != 0.0) {
    return;  // Don't overwrite good values.
  }
  float a = 2*bed_level[x+xdir][y] - bed_level[x+xdir*2][y];  // Left to right.
  float b = 2*bed_level[x][y+ydir] - bed_level[x][y+ydir*2];  // Front to back.
  float c = 2*bed_level[x+xdir][y+ydir] - bed_level[x+xdir*2][y+ydir*2];  // Diagonal.
  float median = c;  // Median is robust (ignores outliers).
  if (a < b) {
    if (b < c) median = b;
    if (c < a) median = a;
  } else {  // b <= a
    if (c < b) median = b;
    if (a < c) median = a;
  }
  bed_level[x][y] = median;
}

// Fill in the unprobed points (corners of circular print surface)
// using linear extrapolation, away from the center.
static void extrapolate_unprobed_bed_level() {
  int half = (AUTO_BED_LEVELING_GRID_POINTS-1)/2;
  for (int y = 0; y <= half; y++) {
    for (int x = 0; x <= half; x++) {
      if (x + y < 3) continue;
      extrapolate_one_point(half-x, half-y, x>1?+1:0, y>1?+1:0);
      extrapolate_one_point(half+x, half-y, x>1?-1:0, y>1?+1:0);
      extrapolate_one_point(half-x, half+y, x>1?+1:0, y>1?-1:0);
      extrapolate_one_point(half+x, half+y, x>1?-1:0, y>1?-1:0);
    }
  }
}

// Print calibration results for plotting or manual frame adjustment.
static void print_bed_level() {
  for (int y = 0; y < AUTO_BED_LEVELING_GRID_POINTS; y++) {
    for (int x = 0; x < AUTO_BED_LEVELING_GRID_POINTS; x++) {
      SERIAL_PROTOCOL_F(bed_level[x][y], 2);
      SERIAL_PROTOCOLPGM(" ");
    }
    SERIAL_ECHOLN("");
  }
}

// Reset calibration results to zero.
void reset_bed_level() {
  for (int y = 0; y < AUTO_BED_LEVELING_GRID_POINTS; y++) {
    for (int x = 0; x < AUTO_BED_LEVELING_GRID_POINTS; x++) {
      bed_level[x][y] = 0.0;
    }
  }
}

#endif // DELTA

#endif // ENABLE_AUTO_BED_LEVELING

static void homeaxis(int axis) {
#define HOMEAXIS_DO(LETTER) \
  ((LETTER##_MIN_PIN > -1 && LETTER##_HOME_DIR==-1) || (LETTER##_MAX_PIN > -1 && LETTER##_HOME_DIR==1))

  if (axis==X_AXIS ? HOMEAXIS_DO(X) :
      axis==Y_AXIS ? HOMEAXIS_DO(Y) :
      axis==Z_AXIS ? HOMEAXIS_DO(Z) :
      0) {
    int axis_home_dir = home_dir(axis);
#ifdef DUAL_X_CARRIAGE
    if (axis == X_AXIS)
      axis_home_dir = x_home_dir(active_extruder);
#endif

    current_position[axis] = 0;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);


#ifndef Z_PROBE_SLED
    // Engage Servo endstop if enabled
    #ifdef SERVO_ENDSTOPS
      #if SERVO_LEVELING
        if (axis==Z_AXIS) {
          engage_z_probe();
        }
      else
      #endif
      if (servo_endstops[axis] > -1) {
        servos[servo_endstops[axis]].write(servo_endstop_angles[axis * 2]);
      }
    #endif
#endif // Z_PROBE_SLED
    #ifdef Z_DUAL_ENDSTOPS
      if (axis==Z_AXIS) In_Homing_Process(true);
    #endif
    destination[axis] = 1.5 * max_length(axis) * axis_home_dir;
    feedrate = homing_feedrate[axis];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    current_position[axis] = 0;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    destination[axis] = -home_retract_mm(axis) * axis_home_dir;
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    destination[axis] = 2*home_retract_mm(axis) * axis_home_dir;

    if (homing_bump_divisor[axis] >= 1)
    {
        feedrate = homing_feedrate[axis]/homing_bump_divisor[axis];
    } 
    else
    {
        feedrate = homing_feedrate[axis]/10;
        SERIAL_ECHOLN("Warning: The Homing Bump Feedrate Divisor cannot be less then 1");
    }

    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();
    #ifdef Z_DUAL_ENDSTOPS
      if (axis==Z_AXIS)
      {
        feedrate = homing_feedrate[axis];
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        if (axis_home_dir > 0)
        {
          destination[axis] = (-1) * fabs(z_endstop_adj);
          if (z_endstop_adj > 0) Lock_z_motor(true); else Lock_z2_motor(true);
        } else {
          destination[axis] = fabs(z_endstop_adj);
          if (z_endstop_adj < 0) Lock_z_motor(true); else Lock_z2_motor(true);        
        }
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
        st_synchronize();
        Lock_z_motor(false);
        Lock_z2_motor(false);
        In_Homing_Process(false);
      }
    #endif

#ifdef DELTA
    // retrace by the amount specified in endstop_adj
    if (endstop_adj[axis] * axis_home_dir < 0) {
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      destination[axis] = endstop_adj[axis];
      plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
      st_synchronize();
    }
#endif
    axis_is_at_home(axis);
    destination[axis] = current_position[axis];
    feedrate = 0.0;
    endstops_hit_on_purpose();
    axis_known_position[axis] = true;

    // Retract Servo endstop if enabled
    #ifdef SERVO_ENDSTOPS
      if (servo_endstops[axis] > -1) {
        servos[servo_endstops[axis]].write(servo_endstop_angles[axis * 2 + 1]);
      }
    #endif
#if SERVO_LEVELING
  #ifndef Z_PROBE_SLED
    if (axis==Z_AXIS) retract_z_probe();
  #endif
#endif

  }
}
#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

void refresh_cmd_timeout(void)
{
  previous_millis_cmd = millis();
}

#ifdef FWRETRACT
  void retract(bool retracting, bool swapretract = false) {
    if(retracting && !retracted[active_extruder]) {
      destination[X_AXIS]=current_position[X_AXIS];
      destination[Y_AXIS]=current_position[Y_AXIS];
      destination[Z_AXIS]=current_position[Z_AXIS];
      destination[E_AXIS]=current_position[E_AXIS];
      if (swapretract) {
        current_position[E_AXIS]+=retract_length_swap/volumetric_multiplier[active_extruder];
      } else {
        current_position[E_AXIS]+=retract_length/volumetric_multiplier[active_extruder];
      }
      plan_set_e_position(current_position[E_AXIS]);
      float oldFeedrate = feedrate;
      feedrate=retract_feedrate*60;
      retracted[active_extruder]=true;
      prepare_move();
      if(retract_zlift > 0.01) {
         current_position[Z_AXIS]-=retract_zlift;
#ifdef DELTA
         calculate_delta(current_position); // change cartesian kinematic to  delta kinematic;
         plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
#else
         plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
#endif
         prepare_move();
      }
      feedrate = oldFeedrate;
    } else if(!retracting && retracted[active_extruder]) {
      destination[X_AXIS]=current_position[X_AXIS];
      destination[Y_AXIS]=current_position[Y_AXIS];
      destination[Z_AXIS]=current_position[Z_AXIS];
      destination[E_AXIS]=current_position[E_AXIS];
      if(retract_zlift > 0.01) {
         current_position[Z_AXIS]+=retract_zlift;
#ifdef DELTA
         calculate_delta(current_position); // change cartesian kinematic  to  delta kinematic;
         plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
#else
         plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
#endif
         //prepare_move();
      }
      if (swapretract) {
        current_position[E_AXIS]-=(retract_length_swap+retract_recover_length_swap)/volumetric_multiplier[active_extruder]; 
      } else {
        current_position[E_AXIS]-=(retract_length+retract_recover_length)/volumetric_multiplier[active_extruder]; 
      }
      plan_set_e_position(current_position[E_AXIS]);
      float oldFeedrate = feedrate;
      feedrate=retract_recover_feedrate*60;
      retracted[active_extruder]=false;
      prepare_move();
      feedrate = oldFeedrate;
    }
  } //retract
#endif //FWRETRACT

#ifdef Z_PROBE_SLED

  #ifndef SLED_DOCKING_OFFSET
    #define SLED_DOCKING_OFFSET 0
  #endif

//
// Method to dock/undock a sled designed by Charles Bell.
//
// dock[in]     If true, move to MAX_X and engage the electromagnet
// offset[in]   The additional distance to move to adjust docking location
//
static void dock_sled(bool dock, int offset=0) {
 int z_loc;
 
 if (!((axis_known_position[X_AXIS]) && (axis_known_position[Y_AXIS]))) {
   LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
   SERIAL_ECHO_START;
   SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
   return;
 }

 if (dock) {
   do_blocking_move_to(X_MAX_POS + SLED_DOCKING_OFFSET + offset,
                       current_position[Y_AXIS],
                       current_position[Z_AXIS]);
   // turn off magnet
   digitalWrite(SERVO0_PIN, LOW);
 } else {
   if (current_position[Z_AXIS] < (Z_RAISE_BEFORE_PROBING + 5))
     z_loc = Z_RAISE_BEFORE_PROBING;
   else
     z_loc = current_position[Z_AXIS];
   do_blocking_move_to(X_MAX_POS + SLED_DOCKING_OFFSET + offset,
                       Y_PROBE_OFFSET_FROM_EXTRUDER, z_loc);
   // turn on magnet
   digitalWrite(SERVO0_PIN, HIGH);
 }
}
#endif

/**
 *
 * G-Code Handler functions
 *
 */

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
inline void gcode_G0_G1() {
  if (!Stopped) {
    get_coordinates(); // For X Y Z E F
    #ifdef FWRETRACT
      if (autoretract_enabled)
      if (!(code_seen('X') || code_seen('Y') || code_seen('Z')) && code_seen('E')) {
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
    //ClearToSend();
  }
}

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 */
inline void gcode_G2_G3(bool clockwise) {
  if (!Stopped) {
    get_arc_coordinates();
    prepare_arc_move(clockwise);
  }
}

/**
 * G4: Dwell S<seconds> or P<milliseconds>
 */
inline void gcode_G4() {
  unsigned long codenum=0;

  LCD_MESSAGEPGM(MSG_DWELL);

  if (code_seen('P')) codenum = code_value_long(); // milliseconds to wait
  if (code_seen('S')) codenum = code_value_long() * 1000; // seconds to wait

  st_synchronize();
  previous_millis_cmd = millis();
  codenum += previous_millis_cmd;  // keep track of when we started waiting
  while(millis() < codenum) {
    manage_heater();
    manage_inactivity();
    lcd_update();
  }
}

#ifdef FWRETRACT

  /**
   * G10 - Retract filament according to settings of M207
   * G11 - Recover filament according to settings of M208
   */
  inline void gcode_G10_G11(bool doRetract=false) {
    #if EXTRUDERS > 1
      if (doRetract) {
        retracted_swap[active_extruder] = (code_seen('S') && code_value_long() == 1); // checks for swap retract argument
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
 * G28: Home all axes, one at a time
 */
inline void gcode_G28() {
  #ifdef ENABLE_AUTO_BED_LEVELING
    #ifdef DELTA
      reset_bed_level();
    #else
      plan_bed_level_matrix.set_to_identity();  //Reset the plane ("erase" all leveling data)
    #endif
  #endif

  #if defined(MESH_BED_LEVELING)
    uint8_t mbl_was_active = mbl.active;
    mbl.active = 0;
  #endif  // MESH_BED_LEVELING

  saved_feedrate = feedrate;
  saved_feedmultiply = feedmultiply;
  feedmultiply = 100;
  previous_millis_cmd = millis();

  enable_endstops(true);

  for (int i = X_AXIS; i < NUM_AXIS; i++) destination[i] = current_position[i];

  feedrate = 0.0;

  #ifdef DELTA
    // A delta can only safely home all axis at the same time
    // all axis have to home at the same time

    // Move all carriages up together until the first endstop is hit.
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = 0;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

    for (int i = X_AXIS; i <= Z_AXIS; i++) destination[i] = 3 * Z_MAX_LENGTH;
    feedrate = 1.732 * homing_feedrate[X_AXIS];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();
    endstops_hit_on_purpose();

    // Destination reached
    for (int i = X_AXIS; i <= Z_AXIS; i++) current_position[i] = destination[i];

    // take care of back off and rehome now we are all at the top
    HOMEAXIS(X);
    HOMEAXIS(Y);
    HOMEAXIS(Z);

    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);

  #else // NOT DELTA

    home_all_axis = !(code_seen(axis_codes[X_AXIS]) || code_seen(axis_codes[Y_AXIS]) || code_seen(axis_codes[Z_AXIS]));

    #if Z_HOME_DIR > 0                      // If homing away from BED do Z first
      if (home_all_axis || code_seen(axis_codes[Z_AXIS])) {
        HOMEAXIS(Z);
      }
    #endif

    #ifdef QUICK_HOME
      if (home_all_axis || code_seen(axis_codes[X_AXIS] && code_seen(axis_codes[Y_AXIS]))) {  //first diagonal move
        current_position[X_AXIS] = current_position[Y_AXIS] = 0;

        #ifndef DUAL_X_CARRIAGE
          int x_axis_home_dir = home_dir(X_AXIS);
        #else
          int x_axis_home_dir = x_home_dir(active_extruder);
          extruder_duplication_enabled = false;
        #endif

        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        destination[X_AXIS] = 1.5 * max_length(X_AXIS) * x_axis_home_dir;
        destination[Y_AXIS] = 1.5 * max_length(Y_AXIS) * home_dir(Y_AXIS);
        feedrate = homing_feedrate[X_AXIS];
        if (homing_feedrate[Y_AXIS] < feedrate) feedrate = homing_feedrate[Y_AXIS];
        if (max_length(X_AXIS) > max_length(Y_AXIS)) {
          feedrate *= sqrt(pow(max_length(Y_AXIS) / max_length(X_AXIS), 2) + 1);
        } else {
          feedrate *= sqrt(pow(max_length(X_AXIS) / max_length(Y_AXIS), 2) + 1);
        }
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
        st_synchronize();

        axis_is_at_home(X_AXIS);
        axis_is_at_home(Y_AXIS);
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        destination[X_AXIS] = current_position[X_AXIS];
        destination[Y_AXIS] = current_position[Y_AXIS];
        plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
        feedrate = 0.0;
        st_synchronize();
        endstops_hit_on_purpose();

        current_position[X_AXIS] = destination[X_AXIS];
        current_position[Y_AXIS] = destination[Y_AXIS];
        #ifndef SCARA
          current_position[Z_AXIS] = destination[Z_AXIS];
        #endif
      }
    #endif //QUICK_HOME

    if ((home_all_axis) || (code_seen(axis_codes[X_AXIS]))) {
      #ifdef DUAL_X_CARRIAGE
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
    }

    if (home_all_axis || code_seen(axis_codes[Y_AXIS])) HOMEAXIS(Y);

    if (code_seen(axis_codes[X_AXIS])) {
      if (code_value_long() != 0) {
          current_position[X_AXIS] = code_value()
            #ifndef SCARA
              + home_offset[X_AXIS]
            #endif
          ;
      }
    }

    if (code_seen(axis_codes[Y_AXIS]) && code_value_long() != 0) {
      current_position[Y_AXIS] = code_value()
        #ifndef SCARA
          + home_offset[Y_AXIS]
        #endif
      ;
    }

    #if Z_HOME_DIR < 0                      // If homing towards BED do Z last

      #ifndef Z_SAFE_HOMING

        if (home_all_axis || code_seen(axis_codes[Z_AXIS])) {
          #if defined(Z_RAISE_BEFORE_HOMING) && Z_RAISE_BEFORE_HOMING > 0
            destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);    // Set destination away from bed
            feedrate = max_feedrate[Z_AXIS];
            plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
            st_synchronize();
          #endif
          HOMEAXIS(Z);
        }

      #else // Z_SAFE_HOMING

        if (home_all_axis) {
          destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT - X_PROBE_OFFSET_FROM_EXTRUDER);
          destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT - Y_PROBE_OFFSET_FROM_EXTRUDER);
          destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);    // Set destination away from bed
          feedrate = XY_TRAVEL_SPEED / 60;
          current_position[Z_AXIS] = 0;

          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
          st_synchronize();
          current_position[X_AXIS] = destination[X_AXIS];
          current_position[Y_AXIS] = destination[Y_AXIS];

          HOMEAXIS(Z);
        }

        // Let's see if X and Y are homed and probe is inside bed area.
        if (code_seen(axis_codes[Z_AXIS])) {

          if (axis_known_position[X_AXIS] && axis_known_position[Y_AXIS]) {

            float cpx = current_position[X_AXIS], cpy = current_position[Y_AXIS];
            if (   cpx >= X_MIN_POS - X_PROBE_OFFSET_FROM_EXTRUDER
                && cpx <= X_MAX_POS - X_PROBE_OFFSET_FROM_EXTRUDER
                && cpy >= Y_MIN_POS - Y_PROBE_OFFSET_FROM_EXTRUDER
                && cpy <= Y_MAX_POS - Y_PROBE_OFFSET_FROM_EXTRUDER) {
              current_position[Z_AXIS] = 0;
              plan_set_position(cpx, cpy, current_position[Z_AXIS], current_position[E_AXIS]);
              destination[Z_AXIS] = -Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS);    // Set destination away from bed
              feedrate = max_feedrate[Z_AXIS];
              plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
              st_synchronize();
              HOMEAXIS(Z);
            }
            else {
                LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
                SERIAL_ECHO_START;
                SERIAL_ECHOLNPGM(MSG_ZPROBE_OUT);
            }
          }
          else {
            LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
            SERIAL_ECHO_START;
            SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
          }
        }

      #endif // Z_SAFE_HOMING

    #endif // Z_HOME_DIR < 0


    if (code_seen(axis_codes[Z_AXIS]) && code_value_long() != 0)
      current_position[Z_AXIS] = code_value() + home_offset[Z_AXIS];

    #if defined(ENABLE_AUTO_BED_LEVELING) && (Z_HOME_DIR < 0)
      if (home_all_axis || code_seen(axis_codes[Z_AXIS]))
        current_position[Z_AXIS] += zprobe_zoffset;  //Add Z_Probe offset (the distance is negative)
    #endif
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

  #endif // else DELTA

  #ifdef SCARA
    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
  #endif

  #ifdef ENDSTOPS_ONLY_FOR_HOMING
    enable_endstops(false);
  #endif

  #if defined(MESH_BED_LEVELING)
    if (mbl_was_active) {
      current_position[X_AXIS] = mbl.get_x(0);
      current_position[Y_AXIS] = mbl.get_y(0);
      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = current_position[Z_AXIS];
      destination[E_AXIS] = current_position[E_AXIS];
      feedrate = homing_feedrate[X_AXIS];
      plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
      st_synchronize();
      current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      mbl.active = 1;
    }
  #endif

  feedrate = saved_feedrate;
  feedmultiply = saved_feedmultiply;
  previous_millis_cmd = millis();
  endstops_hit_on_purpose();
}

#ifdef MESH_BED_LEVELING

  /**
   * G29: Mesh-based Z-Probe, probes a grid and produces a
   *      mesh to compensate for variable bed height
   *
   * Parameters With MESH_BED_LEVELING:
   *
   *  S0 Produce a mesh report
   *  S1 Start probing mesh points
   *  S2 Probe the next mesh point
   *
   */
  inline void gcode_G29() {
    static int probe_point = -1;
    int state = 0;
    if (code_seen('S') || code_seen('s')) {
      state = code_value_long();
      if (state < 0 || state > 2) {
        SERIAL_PROTOCOLPGM("S out of range (0-2).\n");
        return;
      }
    }

    if (state == 0) { // Dump mesh_bed_leveling
      if (mbl.active) {
        SERIAL_PROTOCOLPGM("Num X,Y: ");
        SERIAL_PROTOCOL(MESH_NUM_X_POINTS);
        SERIAL_PROTOCOLPGM(",");
        SERIAL_PROTOCOL(MESH_NUM_Y_POINTS);
        SERIAL_PROTOCOLPGM("\nZ search height: ");
        SERIAL_PROTOCOL(MESH_HOME_SEARCH_Z);
        SERIAL_PROTOCOLPGM("\nMeasured points:\n");              
        for (int y=0; y<MESH_NUM_Y_POINTS; y++) {
          for (int x=0; x<MESH_NUM_X_POINTS; x++) {
            SERIAL_PROTOCOLPGM("  ");              
            SERIAL_PROTOCOL_F(mbl.z_values[y][x], 5);
          }
          SERIAL_EOL;
        }
      } else {
        SERIAL_PROTOCOLPGM("Mesh bed leveling not active.\n");
      }

    } else if (state == 1) { // Begin probing mesh points

      mbl.reset();
      probe_point = 0;
      enquecommands_P(PSTR("G28"));
      enquecommands_P(PSTR("G29 S2"));

    } else if (state == 2) { // Goto next point

      if (probe_point < 0) {
        SERIAL_PROTOCOLPGM("Start mesh probing with \"G29 S1\" first.\n");
        return;
      }
      int ix, iy;
      if (probe_point == 0) {
        current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      } else {
        ix = (probe_point-1) % MESH_NUM_X_POINTS;
        iy = (probe_point-1) / MESH_NUM_X_POINTS;
        if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
        mbl.set_z(ix, iy, current_position[Z_AXIS]);
        current_position[Z_AXIS] = MESH_HOME_SEARCH_Z;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS]/60, active_extruder);
        st_synchronize();
      }
      if (probe_point == MESH_NUM_X_POINTS * MESH_NUM_Y_POINTS) {
        SERIAL_PROTOCOLPGM("Mesh probing done.\n");
        probe_point = -1;
        mbl.active = 1;
        enquecommands_P(PSTR("G28"));
        return;
      }
      ix = probe_point % MESH_NUM_X_POINTS;
      iy = probe_point / MESH_NUM_X_POINTS;
      if (iy & 1) ix = (MESH_NUM_X_POINTS - 1) - ix; // zig-zag
      current_position[X_AXIS] = mbl.get_x(ix);
      current_position[Y_AXIS] = mbl.get_y(iy);
      plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], homing_feedrate[X_AXIS]/60, active_extruder);
      st_synchronize();
      probe_point++;
    }
  }

#elif defined(ENABLE_AUTO_BED_LEVELING)

  /**
   * G29: Detailed Z-Probe, probes the bed at 3 or more points.
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
   * E/e By default G29 engages / disengages the probe for each point.
   *     Include "E" to engage and disengage the probe just once.
   *     There's no extra effect if you have a fixed probe.
   *     Usage: "G29 E" or "G29 e"
   *
   */
  inline void gcode_G29() {

    // Prevent user from running a G29 without first homing in X and Y
    if (!axis_known_position[X_AXIS] || !axis_known_position[Y_AXIS]) {
      LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
      return;
    }

    int verbose_level = 1;

    if (code_seen('V') || code_seen('v')) {
      verbose_level = code_value_long();
      if (verbose_level < 0 || verbose_level > 4) {
        SERIAL_PROTOCOLPGM("?(V)erbose Level is implausible (0-4).\n");
        return;
      }
    }

    bool dryrun = code_seen('D') || code_seen('d');
    bool enhanced_g29 = code_seen('E') || code_seen('e');

    #ifdef AUTO_BED_LEVELING_GRID

      #ifndef DELTA
        bool do_topography_map = verbose_level > 2 || code_seen('T') || code_seen('t');
      #endif

      if (verbose_level > 0)
      {
        SERIAL_PROTOCOLPGM("G29 Auto Bed Leveling\n");
        if (dryrun) SERIAL_ECHOLN("Running in DRY-RUN mode");
      }

      int auto_bed_leveling_grid_points = AUTO_BED_LEVELING_GRID_POINTS;
      #ifndef DELTA
        if (code_seen('P')) auto_bed_leveling_grid_points = code_value_long();
        if (auto_bed_leveling_grid_points < 2) {
          SERIAL_PROTOCOLPGM("?Number of probed (P)oints is implausible (2 minimum).\n");
          return;
        }
      #endif

      xy_travel_speed = code_seen('S') ? code_value_long() : XY_TRAVEL_SPEED;

      int left_probe_bed_position = code_seen('L') ? code_value_long() : LEFT_PROBE_BED_POSITION,
          right_probe_bed_position = code_seen('R') ? code_value_long() : RIGHT_PROBE_BED_POSITION,
          front_probe_bed_position = code_seen('F') ? code_value_long() : FRONT_PROBE_BED_POSITION,
          back_probe_bed_position = code_seen('B') ? code_value_long() : BACK_PROBE_BED_POSITION;

      bool left_out_l = left_probe_bed_position < MIN_PROBE_X,
           left_out = left_out_l || left_probe_bed_position > right_probe_bed_position - MIN_PROBE_EDGE,
           right_out_r = right_probe_bed_position > MAX_PROBE_X,
           right_out = right_out_r || right_probe_bed_position < left_probe_bed_position + MIN_PROBE_EDGE,
           front_out_f = front_probe_bed_position < MIN_PROBE_Y,
           front_out = front_out_f || front_probe_bed_position > back_probe_bed_position - MIN_PROBE_EDGE,
           back_out_b = back_probe_bed_position > MAX_PROBE_Y,
           back_out = back_out_b || back_probe_bed_position < front_probe_bed_position + MIN_PROBE_EDGE;

      if (left_out || right_out || front_out || back_out) {
        if (left_out) {
          SERIAL_PROTOCOLPGM("?Probe (L)eft position out of range.\n");
          left_probe_bed_position = left_out_l ? MIN_PROBE_X : right_probe_bed_position - MIN_PROBE_EDGE;
        }
        if (right_out) {
          SERIAL_PROTOCOLPGM("?Probe (R)ight position out of range.\n");
          right_probe_bed_position = right_out_r ? MAX_PROBE_X : left_probe_bed_position + MIN_PROBE_EDGE;
        }
        if (front_out) {
          SERIAL_PROTOCOLPGM("?Probe (F)ront position out of range.\n");
          front_probe_bed_position = front_out_f ? MIN_PROBE_Y : back_probe_bed_position - MIN_PROBE_EDGE;
        }
        if (back_out) {
          SERIAL_PROTOCOLPGM("?Probe (B)ack position out of range.\n");
          back_probe_bed_position = back_out_b ? MAX_PROBE_Y : front_probe_bed_position + MIN_PROBE_EDGE;
        }
        return;
      }

    #endif // AUTO_BED_LEVELING_GRID

    #ifdef Z_PROBE_SLED
      dock_sled(false); // engage (un-dock) the probe
    #elif defined(Z_PROBE_ALLEN_KEY) //|| defined(SERVO_LEVELING)
      engage_z_probe();
    #endif

    st_synchronize();

    if (!dryrun)
    {
      #ifdef DELTA
        reset_bed_level();
      #else //!DELTA

        // make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly
        //vector_3 corrected_position = plan_get_position_mm();
        //corrected_position.debug("position before G29");
        plan_bed_level_matrix.set_to_identity();
        vector_3 uncorrected_position = plan_get_position();
        //uncorrected_position.debug("position during G29");
        current_position[X_AXIS] = uncorrected_position.x;
        current_position[Y_AXIS] = uncorrected_position.y;
        current_position[Z_AXIS] = uncorrected_position.z;
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

      #endif
    }
    
    setup_for_endstop_move();

    feedrate = homing_feedrate[Z_AXIS];

    #ifdef AUTO_BED_LEVELING_GRID

      // probe at the points of a lattice grid
      const int xGridSpacing = (right_probe_bed_position - left_probe_bed_position) / (auto_bed_leveling_grid_points-1);
      const int yGridSpacing = (back_probe_bed_position - front_probe_bed_position) / (auto_bed_leveling_grid_points-1);

      #ifdef DELTA
        delta_grid_spacing[0] = xGridSpacing;
        delta_grid_spacing[1] = yGridSpacing;
        float z_offset = Z_PROBE_OFFSET_FROM_EXTRUDER;
        if (code_seen(axis_codes[Z_AXIS])) z_offset += code_value();
      #else // !DELTA
        // solve the plane equation ax + by + d = z
        // A is the matrix with rows [x y 1] for all the probed points
        // B is the vector of the Z positions
        // the normal vector to the plane is formed by the coefficients of the plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
        // so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z

        int abl2 = auto_bed_leveling_grid_points * auto_bed_leveling_grid_points;

        double eqnAMatrix[abl2 * 3], // "A" matrix of the linear system of equations
               eqnBVector[abl2],     // "B" vector of Z points
               mean = 0.0;
      #endif // !DELTA

      int probePointCounter = 0;
      bool zig = true;

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

        #ifndef DELTA
          // If do_topography_map is set then don't zig-zag. Just scan in one direction.
          // This gets the probe points in more readable order.
          if (!do_topography_map) zig = !zig;
        #endif

        for (int xCount = xStart; xCount != xStop; xCount += xInc) {
          double xProbe = left_probe_bed_position + xGridSpacing * xCount;

          // raise extruder
          float measured_z,
                z_before = probePointCounter == 0 ? Z_RAISE_BEFORE_PROBING : current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS;

          #ifdef DELTA
            // Avoid probing the corners (outside the round or hexagon print surface) on a delta printer.
            float distance_from_center = sqrt(xProbe*xProbe + yProbe*yProbe);
            if (distance_from_center > DELTA_PROBABLE_RADIUS)
              continue;
          #endif //DELTA

          // Enhanced G29 - Do not retract servo between probes
          ProbeAction act;
          if (enhanced_g29) {
            if (yProbe == front_probe_bed_position && xCount == 0)
              act = ProbeEngage;
            else if (yProbe == front_probe_bed_position + (yGridSpacing * (auto_bed_leveling_grid_points - 1)) && xCount == auto_bed_leveling_grid_points - 1)
              act = ProbeRetract;
            else
              act = ProbeStay;
          }
          else
            act = ProbeEngageAndRetract;

          measured_z = probe_pt(xProbe, yProbe, z_before, act, verbose_level);

          #ifndef DELTA
            mean += measured_z;

            eqnBVector[probePointCounter] = measured_z;
            eqnAMatrix[probePointCounter + 0 * abl2] = xProbe;
            eqnAMatrix[probePointCounter + 1 * abl2] = yProbe;
            eqnAMatrix[probePointCounter + 2 * abl2] = 1;
          #else
            bed_level[xCount][yCount] = measured_z + z_offset;
          #endif

          probePointCounter++;
        } //xProbe
      } //yProbe

      clean_up_after_endstop_move();

      #ifdef DELTA

        if (!dryrun) extrapolate_unprobed_bed_level();
        print_bed_level();

      #else // !DELTA

        // solve lsq problem
        double *plane_equation_coefficients = qr_solve(abl2, 3, eqnAMatrix, eqnBVector);

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

        // Show the Topography map if enabled
        if (do_topography_map) {

          SERIAL_PROTOCOLPGM(" \nBed Height Topography: \n");
          SERIAL_PROTOCOLPGM("+-----------+\n");
          SERIAL_PROTOCOLPGM("|...Back....|\n");
          SERIAL_PROTOCOLPGM("|Left..Right|\n");
          SERIAL_PROTOCOLPGM("|...Front...|\n");
          SERIAL_PROTOCOLPGM("+-----------+\n");

          for (int yy = auto_bed_leveling_grid_points - 1; yy >= 0; yy--) {
            for (int xx = 0; xx < auto_bed_leveling_grid_points; xx++) {
              int ind = yy * auto_bed_leveling_grid_points + xx;
              float diff = eqnBVector[ind] - mean;
              if (diff >= 0.0)
                SERIAL_PROTOCOLPGM(" +");   // Include + for column alignment
              else
                SERIAL_PROTOCOLPGM(" ");
              SERIAL_PROTOCOL_F(diff, 5);
            } // xx
            SERIAL_EOL;
          } // yy
          SERIAL_EOL;

        } //do_topography_map


        if (!dryrun) set_bed_level_equation_lsq(plane_equation_coefficients);
        free(plane_equation_coefficients);

      #endif //!DELTA

    #else // !AUTO_BED_LEVELING_GRID

      // Probe at 3 arbitrary points
      float z_at_pt_1, z_at_pt_2, z_at_pt_3;

      if (enhanced_g29) {
        // Basic Enhanced G29
        z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING, ProbeEngage, verbose_level);
        z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, ProbeStay, verbose_level);
        z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, ProbeRetract, verbose_level);
      }
      else {
        z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING, ProbeEngageAndRetract, verbose_level);
        z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, ProbeEngageAndRetract, verbose_level);
        z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS, ProbeEngageAndRetract, verbose_level);
      }
      clean_up_after_endstop_move();
      if (!dryrun) set_bed_level_equation_3pts(z_at_pt_1, z_at_pt_2, z_at_pt_3);

    #endif // !AUTO_BED_LEVELING_GRID

    #ifndef DELTA
      if (verbose_level > 0)
        plan_bed_level_matrix.debug(" \n\nBed Level Correction Matrix:");

      // Correct the Z height difference from z-probe position and hotend tip position.
      // The Z height on homing is measured by Z-Probe, but the probe is quite far from the hotend.
      // When the bed is uneven, this height must be corrected.
      if (!dryrun)
      {
        float x_tmp, y_tmp, z_tmp, real_z;
        real_z = float(st_get_position(Z_AXIS)) / axis_steps_per_unit[Z_AXIS];  //get the real Z (since the auto bed leveling is already correcting the plane)
        x_tmp = current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER;
        y_tmp = current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER;
        z_tmp = current_position[Z_AXIS];

        apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);         //Apply the correction sending the probe offset
        current_position[Z_AXIS] = z_tmp - real_z + current_position[Z_AXIS];   //The difference is added to current position and sent to planner.
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      }
    #endif // !DELTA

    #ifdef Z_PROBE_SLED
      dock_sled(true, -SLED_DOCKING_OFFSET); // dock the probe, correcting for over-travel
    #elif defined(Z_PROBE_ALLEN_KEY) //|| defined(SERVO_LEVELING)
      retract_z_probe();
    #endif

    #ifdef Z_PROBE_END_SCRIPT
      enquecommands_P(PSTR(Z_PROBE_END_SCRIPT));
      st_synchronize();
    #endif
  }

  #ifndef Z_PROBE_SLED

    inline void gcode_G30() {
      engage_z_probe(); // Engage Z Servo endstop if available
      st_synchronize();
      // TODO: make sure the bed_level_rotation_matrix is identity or the planner will get set incorectly
      setup_for_endstop_move();

      feedrate = homing_feedrate[Z_AXIS];

      run_z_probe();
      SERIAL_PROTOCOLPGM(MSG_BED);
      SERIAL_PROTOCOLPGM(" X: ");
      SERIAL_PROTOCOL(current_position[X_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL(current_position[Y_AXIS] + 0.0001);
      SERIAL_PROTOCOLPGM(" Z: ");
      SERIAL_PROTOCOL(current_position[Z_AXIS] + 0.0001);
      SERIAL_EOL;

      clean_up_after_endstop_move();
      retract_z_probe(); // Retract Z Servo endstop if available
    }

  #endif //!Z_PROBE_SLED

#endif //ENABLE_AUTO_BED_LEVELING

/**
 * G92: Set current position to given X Y Z E
 */
inline void gcode_G92() {
  if (!code_seen(axis_codes[E_AXIS]))
    st_synchronize();

  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      current_position[i] = code_value();
      if (i == E_AXIS)
        plan_set_e_position(current_position[E_AXIS]);
      else
        plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    }
  }
}

#ifdef ULTIPANEL

  /**
   * M0: // M0 - Unconditional stop - Wait for user button press on LCD
   * M1: // M1 - Conditional stop - Wait for user button press on LCD
   */
  inline void gcode_M0_M1() {
    char *src = strchr_pointer + 2;

    unsigned long codenum = 0;
    bool hasP = false, hasS = false;
    if (code_seen('P')) {
      codenum = code_value(); // milliseconds to wait
      hasP = codenum > 0;
    }
    if (code_seen('S')) {
      codenum = code_value() * 1000; // seconds to wait
      hasS = codenum > 0;
    }
    char* starpos = strchr(src, '*');
    if (starpos != NULL) *(starpos) = '\0';
    while (*src == ' ') ++src;
    if (!hasP && !hasS && *src != '\0')
      lcd_setstatus(src);
    else
      LCD_MESSAGEPGM(MSG_USERWAIT);

    lcd_ignore_click();
    st_synchronize();
    previous_millis_cmd = millis();
    if (codenum > 0) {
      codenum += previous_millis_cmd;  // keep track of when we started waiting
      while(millis() < codenum && !lcd_clicked()) {
        manage_heater();
        manage_inactivity();
        lcd_update();
      }
      lcd_ignore_click(false);
    }
    else {
      if (!lcd_detected()) return;
      while (!lcd_clicked()) {
        manage_heater();
        manage_inactivity();
        lcd_update();
      }
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
  enable_x();
  enable_y();
  enable_z();
  enable_e0();
  enable_e1();
  enable_e2();
  enable_e3();
}

#ifdef SDSUPPORT

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
   * M23: Select a file
   */
  inline void gcode_M23() {
    char* codepos = strchr_pointer + 4;
    char* starpos = strchr(codepos, '*');
    if (starpos) *starpos = '\0';
    card.openFile(codepos, true);
  }

  /**
   * M24: Start SD Print
   */
  inline void gcode_M24() {
    card.startFileprint();
    starttime = millis();
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
      card.setIndex(code_value_long());
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
    char* codepos = strchr_pointer + 4;
    char* starpos = strchr(codepos, '*');
    if (starpos) {
      char* npos = strchr(cmdbuffer[bufindr], 'N');
      strchr_pointer = strchr(npos, ' ') + 1;
      *(starpos) = '\0';
    }
    card.openFile(codepos, false);
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
      char* starpos = strchr(strchr_pointer + 4, '*');
      if (starpos) {
        char* npos = strchr(cmdbuffer[bufindr], 'N');
        strchr_pointer = strchr(npos, ' ') + 1;
        *(starpos) = '\0';
      }
      card.removeFile(strchr_pointer + 4);
    }
  }

#endif

/**
 * M31: Get the time since the start of SD Print (or last M109)
 */
inline void gcode_M31() {
  stoptime = millis();
  unsigned long t = (stoptime - starttime) / 1000;
  int min = t / 60, sec = t % 60;
  char time[30];
  sprintf_P(time, PSTR("%i min, %i sec"), min, sec);
  SERIAL_ECHO_START;
  SERIAL_ECHOLN(time);
  lcd_setstatus(time);
  autotempShutdown();
}

#ifdef SDSUPPORT

  /**
   * M32: Select file and start SD Print
   */
  inline void gcode_M32() {
    if (card.sdprinting)
      st_synchronize();

    char* codepos = strchr_pointer + 4;

    char* namestartpos = strchr(codepos, '!');   //find ! to indicate filename string start.
    if (! namestartpos)
      namestartpos = codepos; //default name position, 4 letters after the M
    else
      namestartpos++; //to skip the '!'

    char* starpos = strchr(codepos, '*');
    if (starpos) *(starpos) = '\0';

    bool call_procedure = code_seen('P') && (strchr_pointer < namestartpos);

    if (card.cardOK) {
      card.openFile(namestartpos, true, !call_procedure);

      if (code_seen('S') && strchr_pointer < namestartpos) // "S" (must occur _before_ the filename!)
        card.setIndex(code_value_long());

      card.startFileprint();
      if (!call_procedure)
        starttime = millis(); //procedure calls count as normal print time.
    }
  }

  /**
   * M928: Start SD Write
   */
  inline void gcode_M928() {
    char* starpos = strchr(strchr_pointer + 5, '*');
    if (starpos) {
      char* npos = strchr(cmdbuffer[bufindr], 'N');
      strchr_pointer = strchr(npos, ' ') + 1;
      *(starpos) = '\0';
    }
    card.openLogFile(strchr_pointer + 5);
  }

#endif // SDSUPPORT

/**
 * M42: Change pin status via GCode
 */
inline void gcode_M42() {
  if (code_seen('S')) {
    int pin_status = code_value(),
        pin_number = LED_PIN;

    if (code_seen('P') && pin_status >= 0 && pin_status <= 255)
      pin_number = code_value();

    for (int8_t i = 0; i < (int8_t)(sizeof(sensitive_pins) / sizeof(*sensitive_pins)); i++) {
      if (sensitive_pins[i] == pin_number) {
        pin_number = -1;
        break;
      }
    }

    #if defined(FAN_PIN) && FAN_PIN > -1
      if (pin_number == FAN_PIN) fanSpeed = pin_status;
    #endif

    if (pin_number > -1) {
      pinMode(pin_number, OUTPUT);
      digitalWrite(pin_number, pin_status);
      analogWrite(pin_number, pin_status);
    }
  } // code_seen('S')
}


#if defined(ENABLE_AUTO_BED_LEVELING) && defined(Z_PROBE_REPEATABILITY_TEST)

  #if Z_MIN_PIN == -1
    #error "You must have a Z_MIN endstop in order to enable calculation of Z-Probe repeatability."
  #endif

  /**
   * M48: Z-Probe repeatability measurement function.
   *
   * Usage:
   *   M48 <n#> <X#> <Y#> <V#> <E> <L#>
   *     n = Number of samples (4-50, default 10)
   *     X = Sample X position
   *     Y = Sample Y position
   *     V = Verbose level (0-4, default=1)
   *     E = Engage probe for each reading
   *     L = Number of legs of movement before probe
   *  
   * This function assumes the bed has been homed.  Specificaly, that a G28 command
   * as been issued prior to invoking the M48 Z-Probe repeatability measurement function.
   * Any information generated by a prior G29 Bed leveling command will be lost and need to be
   * regenerated.
   *
   * The number of samples will default to 10 if not specified.  You can use upper or lower case
   * letters for any of the options EXCEPT n.  n must be in lower case because Marlin uses a capital
   * N for its communication protocol and will get horribly confused if you send it a capital N.
   */
  inline void gcode_M48() {

    double sum = 0.0, mean = 0.0, sigma = 0.0, sample_set[50];
    int verbose_level = 1, n = 0, j, n_samples = 10, n_legs = 0, engage_probe_for_each_reading = 0;
    double X_current, Y_current, Z_current;
    double X_probe_location, Y_probe_location, Z_start_location, ext_position;
    
    if (code_seen('V') || code_seen('v')) {
      verbose_level = code_value();
      if (verbose_level < 0 || verbose_level > 4 ) {
        SERIAL_PROTOCOLPGM("?Verbose Level not plausible (0-4).\n");
        return;
      }
    }

    if (verbose_level > 0)
      SERIAL_PROTOCOLPGM("M48 Z-Probe Repeatability test\n");

    if (code_seen('n')) {
      n_samples = code_value();
      if (n_samples < 4 || n_samples > 50) {
        SERIAL_PROTOCOLPGM("?Specified sample size not plausible (4-50).\n");
        return;
      }
    }

    X_current = X_probe_location = st_get_position_mm(X_AXIS);
    Y_current = Y_probe_location = st_get_position_mm(Y_AXIS);
    Z_current = st_get_position_mm(Z_AXIS);
    Z_start_location = st_get_position_mm(Z_AXIS) + Z_RAISE_BEFORE_PROBING;
    ext_position = st_get_position_mm(E_AXIS);

    if (code_seen('E') || code_seen('e'))
      engage_probe_for_each_reading++;

    if (code_seen('X') || code_seen('x')) {
      X_probe_location = code_value() - X_PROBE_OFFSET_FROM_EXTRUDER;
      if (X_probe_location < X_MIN_POS || X_probe_location > X_MAX_POS) {
        SERIAL_PROTOCOLPGM("?Specified X position out of range.\n");
        return;
      }
    }

    if (code_seen('Y') || code_seen('y')) {
      Y_probe_location = code_value() -  Y_PROBE_OFFSET_FROM_EXTRUDER;
      if (Y_probe_location < Y_MIN_POS || Y_probe_location > Y_MAX_POS) {
        SERIAL_PROTOCOLPGM("?Specified Y position out of range.\n");
        return;
      }
    }

    if (code_seen('L') || code_seen('l')) {
      n_legs = code_value();
      if (n_legs == 1) n_legs = 2;
      if (n_legs < 0 || n_legs > 15) {
        SERIAL_PROTOCOLPGM("?Specified number of legs in movement not plausible (0-15).\n");
        return;
      }
    }

    //
    // Do all the preliminary setup work.   First raise the probe.
    //

    st_synchronize();
    plan_bed_level_matrix.set_to_identity();
    plan_buffer_line(X_current, Y_current, Z_start_location,
        ext_position,
        homing_feedrate[Z_AXIS] / 60,
        active_extruder);
    st_synchronize();

    //
    // Now get everything to the specified probe point So we can safely do a probe to
    // get us close to the bed.  If the Z-Axis is far from the bed, we don't want to 
    // use that as a starting point for each probe.
    //
    if (verbose_level > 2)
      SERIAL_PROTOCOL("Positioning probe for the test.\n");

    plan_buffer_line( X_probe_location, Y_probe_location, Z_start_location,
        ext_position,
        homing_feedrate[X_AXIS]/60,
        active_extruder);
    st_synchronize();

    current_position[X_AXIS] = X_current = st_get_position_mm(X_AXIS);
    current_position[Y_AXIS] = Y_current = st_get_position_mm(Y_AXIS);
    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);
    current_position[E_AXIS] = ext_position = st_get_position_mm(E_AXIS);

    // 
    // OK, do the inital probe to get us close to the bed.
    // Then retrace the right amount and use that in subsequent probes
    //

    engage_z_probe();

    setup_for_endstop_move();
    run_z_probe();

    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);
    Z_start_location = st_get_position_mm(Z_AXIS) + Z_RAISE_BEFORE_PROBING;

    plan_buffer_line( X_probe_location, Y_probe_location, Z_start_location,
        ext_position,
        homing_feedrate[X_AXIS]/60,
        active_extruder);
    st_synchronize();
    current_position[Z_AXIS] = Z_current = st_get_position_mm(Z_AXIS);

    if (engage_probe_for_each_reading) retract_z_probe();

    for (n=0; n < n_samples; n++) {

      do_blocking_move_to( X_probe_location, Y_probe_location, Z_start_location); // Make sure we are at the probe location

      if (n_legs) {
        double radius=0.0, theta=0.0;
        int l;
        int rotational_direction = (unsigned long) millis() & 0x0001;     // clockwise or counter clockwise
        radius = (unsigned long)millis() % (long)(X_MAX_LENGTH / 4);      // limit how far out to go
        theta = (float)((unsigned long)millis() % 360L) / (360. / (2 * 3.1415926)); // turn into radians

        //SERIAL_ECHOPAIR("starting radius: ",radius);
        //SERIAL_ECHOPAIR("   theta: ",theta);
        //SERIAL_ECHOPAIR("   direction: ",rotational_direction);
        //SERIAL_PROTOCOLLNPGM("");

        float dir = rotational_direction ? 1 : -1;
        for (l = 0; l < n_legs - 1; l++) {
          theta += dir * (float)((unsigned long)millis() % 20L) / (360.0/(2*3.1415926)); // turn into radians

          radius += (float)(((long)((unsigned long) millis() % 10L)) - 5L);
          if (radius < 0.0) radius = -radius;

          X_current = X_probe_location + cos(theta) * radius;
          Y_current = Y_probe_location + sin(theta) * radius;

          // Make sure our X & Y are sane
          X_current = constrain(X_current, X_MIN_POS, X_MAX_POS);
          Y_current = constrain(Y_current, Y_MIN_POS, Y_MAX_POS);

          if (verbose_level > 3) {
            SERIAL_ECHOPAIR("x: ", X_current);
            SERIAL_ECHOPAIR("y: ", Y_current);
            SERIAL_PROTOCOLLNPGM("");
          }

          do_blocking_move_to( X_current, Y_current, Z_current );
        }
        do_blocking_move_to( X_probe_location, Y_probe_location, Z_start_location); // Go back to the probe location
      }

      if (engage_probe_for_each_reading)  {
        engage_z_probe(); 
        delay(1000);
      }

      setup_for_endstop_move();
      run_z_probe();

      sample_set[n] = current_position[Z_AXIS];

      //
      // Get the current mean for the data points we have so far
      //
      sum = 0.0;
      for (j=0; j<=n; j++) sum += sample_set[j];
      mean = sum / (double (n+1));

      //
      // Now, use that mean to calculate the standard deviation for the
      // data points we have so far
      //
      sum = 0.0;
      for (j=0; j<=n; j++) sum += (sample_set[j]-mean) * (sample_set[j]-mean);
      sigma = sqrt( sum / (double (n+1)) );

      if (verbose_level > 1) {
        SERIAL_PROTOCOL(n+1);
        SERIAL_PROTOCOL(" of ");
        SERIAL_PROTOCOL(n_samples);
        SERIAL_PROTOCOLPGM("   z: ");
        SERIAL_PROTOCOL_F(current_position[Z_AXIS], 6);
      }

      if (verbose_level > 2) {
        SERIAL_PROTOCOL(" mean: ");
        SERIAL_PROTOCOL_F(mean,6);
        SERIAL_PROTOCOL("   sigma: ");
        SERIAL_PROTOCOL_F(sigma,6);
      }

      if (verbose_level > 0) SERIAL_EOL;

      plan_buffer_line(X_probe_location, Y_probe_location, Z_start_location,
          current_position[E_AXIS], homing_feedrate[Z_AXIS]/60, active_extruder);
      st_synchronize();

      if (engage_probe_for_each_reading) {
        retract_z_probe();  
        delay(1000);
      }
    }

    retract_z_probe();
    delay(1000);

    clean_up_after_endstop_move();

    // enable_endstops(true);

    if (verbose_level > 0) {
      SERIAL_PROTOCOLPGM("Mean: ");
      SERIAL_PROTOCOL_F(mean, 6);
      SERIAL_EOL;
    }

    SERIAL_PROTOCOLPGM("Standard Deviation: ");
    SERIAL_PROTOCOL_F(sigma, 6);
    SERIAL_EOL; SERIAL_EOL;
  }

#endif // ENABLE_AUTO_BED_LEVELING && Z_PROBE_REPEATABILITY_TEST

/**
 * M104: Set hot end temperature
 */
inline void gcode_M104() {
  if (setTargetedHotend(104)) return;

  if (code_seen('S')) setTargetHotend(code_value(), tmp_extruder);
  #ifdef DUAL_X_CARRIAGE
    if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && tmp_extruder == 0)
      setTargetHotend1(code_value() == 0.0 ? 0.0 : code_value() + duplicate_extruder_temp_offset);
  #endif
  setWatch();
}

/**
 * M105: Read hot end and bed temperature
 */
inline void gcode_M105() {
  if (setTargetedHotend(105)) return;

  #if defined(TEMP_0_PIN) && TEMP_0_PIN > -1
    SERIAL_PROTOCOLPGM("ok T:");
    SERIAL_PROTOCOL_F(degHotend(tmp_extruder),1);
    SERIAL_PROTOCOLPGM(" /");
    SERIAL_PROTOCOL_F(degTargetHotend(tmp_extruder),1);
    #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
      SERIAL_PROTOCOLPGM(" B:");
      SERIAL_PROTOCOL_F(degBed(),1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetBed(),1);
    #endif //TEMP_BED_PIN
    for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
      SERIAL_PROTOCOLPGM(" T");
      SERIAL_PROTOCOL(cur_extruder);
      SERIAL_PROTOCOLPGM(":");
      SERIAL_PROTOCOL_F(degHotend(cur_extruder),1);
      SERIAL_PROTOCOLPGM(" /");
      SERIAL_PROTOCOL_F(degTargetHotend(cur_extruder),1);
    }
  #else
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_PROTOCOLPGM(" @:");
  #ifdef EXTRUDER_WATTS
    SERIAL_PROTOCOL((EXTRUDER_WATTS * getHeaterPower(tmp_extruder))/127);
    SERIAL_PROTOCOLPGM("W");
  #else
    SERIAL_PROTOCOL(getHeaterPower(tmp_extruder));
  #endif

  SERIAL_PROTOCOLPGM(" B@:");
  #ifdef BED_WATTS
    SERIAL_PROTOCOL((BED_WATTS * getHeaterPower(-1))/127);
    SERIAL_PROTOCOLPGM("W");
  #else
    SERIAL_PROTOCOL(getHeaterPower(-1));
  #endif

  #ifdef SHOW_TEMP_ADC_VALUES
    #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
      SERIAL_PROTOCOLPGM("    ADC B:");
      SERIAL_PROTOCOL_F(degBed(),1);
      SERIAL_PROTOCOLPGM("C->");
      SERIAL_PROTOCOL_F(rawBedTemp()/OVERSAMPLENR,0);
    #endif
    for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
      SERIAL_PROTOCOLPGM("  T");
      SERIAL_PROTOCOL(cur_extruder);
      SERIAL_PROTOCOLPGM(":");
      SERIAL_PROTOCOL_F(degHotend(cur_extruder),1);
      SERIAL_PROTOCOLPGM("C->");
      SERIAL_PROTOCOL_F(rawHotendTemp(cur_extruder)/OVERSAMPLENR,0);
    }
  #endif

  SERIAL_PROTOCOLLN("");
}

#if defined(FAN_PIN) && FAN_PIN > -1

  /**
   * M106: Set Fan Speed
   */
  inline void gcode_M106() { fanSpeed = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }

  /**
   * M107: Fan Off
   */
  inline void gcode_M107() { fanSpeed = 0; }

#endif //FAN_PIN

/**
 * M109: Wait for extruder(s) to reach temperature
 */
inline void gcode_M109() {
  if (setTargetedHotend(109)) return;

  LCD_MESSAGEPGM(MSG_HEATING);

  CooldownNoWait = code_seen('S');
  if (CooldownNoWait || code_seen('R')) {
    setTargetHotend(code_value(), tmp_extruder);
    #ifdef DUAL_X_CARRIAGE
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && tmp_extruder == 0)
        setTargetHotend1(code_value() == 0.0 ? 0.0 : code_value() + duplicate_extruder_temp_offset);
    #endif
  }

  #ifdef AUTOTEMP
    autotemp_enabled = code_seen('F');
    if (autotemp_enabled) autotemp_factor = code_value();
    if (code_seen('S')) autotemp_min = code_value();
    if (code_seen('B')) autotemp_max = code_value();
  #endif

  setWatch();

  unsigned long timetemp = millis();

  /* See if we are heating up or cooling down */
  target_direction = isHeatingHotend(tmp_extruder); // true if heating, false if cooling

  cancel_heatup = false;

  #ifdef TEMP_RESIDENCY_TIME
    long residencyStart = -1;
    /* continue to loop until we have reached the target temp
      _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
    while((!cancel_heatup)&&((residencyStart == -1) ||
          (residencyStart >= 0 && (((unsigned int) (millis() - residencyStart)) < (TEMP_RESIDENCY_TIME * 1000UL)))) )
  #else
    while ( target_direction ? (isHeatingHotend(tmp_extruder)) : (isCoolingHotend(tmp_extruder)&&(CooldownNoWait==false)) )
  #endif //TEMP_RESIDENCY_TIME

    { // while loop
      if (millis() > timetemp + 1000UL) { //Print temp & remaining time every 1s while waiting
        SERIAL_PROTOCOLPGM("T:");
        SERIAL_PROTOCOL_F(degHotend(tmp_extruder),1);
        SERIAL_PROTOCOLPGM(" E:");
        SERIAL_PROTOCOL((int)tmp_extruder);
        #ifdef TEMP_RESIDENCY_TIME
          SERIAL_PROTOCOLPGM(" W:");
          if (residencyStart > -1) {
            timetemp = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residencyStart)) / 1000UL;
            SERIAL_PROTOCOLLN( timetemp );
          }
          else {
            SERIAL_PROTOCOLLN( "?" );
          }
        #else
          SERIAL_PROTOCOLLN("");
        #endif
        timetemp = millis();
      }
      manage_heater();
      manage_inactivity();
      lcd_update();
      #ifdef TEMP_RESIDENCY_TIME
        // start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
        // or when current temp falls outside the hysteresis after target temp was reached
        if ((residencyStart == -1 &&  target_direction && (degHotend(tmp_extruder) >= (degTargetHotend(tmp_extruder)-TEMP_WINDOW))) ||
            (residencyStart == -1 && !target_direction && (degHotend(tmp_extruder) <= (degTargetHotend(tmp_extruder)+TEMP_WINDOW))) ||
            (residencyStart > -1 && labs(degHotend(tmp_extruder) - degTargetHotend(tmp_extruder)) > TEMP_HYSTERESIS) )
        {
          residencyStart = millis();
        }
      #endif //TEMP_RESIDENCY_TIME
    }

  LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
  starttime = previous_millis_cmd = millis();
}

#if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1

  /**
   * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
   *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
   */
  inline void gcode_M190() {
    LCD_MESSAGEPGM(MSG_BED_HEATING);
    CooldownNoWait = code_seen('S');
    if (CooldownNoWait || code_seen('R'))
      setTargetBed(code_value());

    unsigned long timetemp = millis();
    
    cancel_heatup = false;
    target_direction = isHeatingBed(); // true if heating, false if cooling

    while ( (target_direction)&&(!cancel_heatup) ? (isHeatingBed()) : (isCoolingBed()&&(CooldownNoWait==false)) ) {
      unsigned long ms = millis();
      if (ms > timetemp + 1000UL) { //Print Temp Reading every 1 second while heating up.
        timetemp = ms;
        float tt = degHotend(active_extruder);
        SERIAL_PROTOCOLPGM("T:");
        SERIAL_PROTOCOL(tt);
        SERIAL_PROTOCOLPGM(" E:");
        SERIAL_PROTOCOL((int)active_extruder);
        SERIAL_PROTOCOLPGM(" B:");
        SERIAL_PROTOCOL_F(degBed(), 1);
        SERIAL_PROTOCOLLN("");
      }
      manage_heater();
      manage_inactivity();
      lcd_update();
    }
    LCD_MESSAGEPGM(MSG_BED_DONE);
    previous_millis_cmd = millis();
  }

#endif // TEMP_BED_PIN > -1

/**
 * M112: Emergency Stop
 */
inline void gcode_M112() {
  kill();
}

#ifdef BARICUDA

  #if defined(HEATER_1_PIN) && HEATER_1_PIN > -1
    /**
     * M126: Heater 1 valve open
     */
    inline void gcode_M126() { ValvePressure = code_seen('S') ? constrain(code_value(), 0, 255) : 255; }
    /**
     * M127: Heater 1 valve close
     */
    inline void gcode_M127() { ValvePressure = 0; }
  #endif

  #if defined(HEATER_2_PIN) && HEATER_2_PIN > -1
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
  if (code_seen('S')) setTargetBed(code_value());
}

#if defined(PS_ON_PIN) && PS_ON_PIN > -1

  /**
   * M80: Turn on Power Supply
   */
  inline void gcode_M80() {
    OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE); //GND

    // If you have a switch on suicide pin, this is useful
    // if you want to start another print with suicide feature after
    // a print without suicide...
    #if defined(SUICIDE_PIN) && SUICIDE_PIN > -1
      OUT_WRITE(SUICIDE_PIN, HIGH);
    #endif

    #ifdef ULTIPANEL
      powersupply = true;
      LCD_MESSAGEPGM(WELCOME_MSG);
      lcd_update();
    #endif
  }

#endif // PS_ON_PIN

/**
 * M81: Turn off Power Supply
 */
inline void gcode_M81() {
  disable_heater();
  st_synchronize();
  disable_e0();
  disable_e1();
  disable_e2();
  disable_e3();
  finishAndDisableSteppers();
  fanSpeed = 0;
  delay(1000); // Wait 1 second before switching off
  #if defined(SUICIDE_PIN) && SUICIDE_PIN > -1
    st_synchronize();
    suicide();
  #elif defined(PS_ON_PIN) && PS_ON_PIN > -1
    OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
  #endif
  #ifdef ULTIPANEL
    powersupply = false;
    LCD_MESSAGEPGM(MACHINE_NAME " " MSG_OFF ".");
    lcd_update();
  #endif
}

/**
 * M82: Set E codes absolute (default)
 */
inline void gcode_M82() { axis_relative_modes[E_AXIS] = false; }

/**
 * M82: Set E codes relative while in Absolute Coordinates (G90) mode
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
    bool all_axis = !((code_seen(axis_codes[X_AXIS])) || (code_seen(axis_codes[Y_AXIS])) || (code_seen(axis_codes[Z_AXIS]))|| (code_seen(axis_codes[E_AXIS])));
    if (all_axis) {
      st_synchronize();
      disable_e0();
      disable_e1();
      disable_e2();
      disable_e3();
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
 * M92: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
inline void gcode_M92() {
  for(int8_t i=0; i < NUM_AXIS; i++) {
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

  SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  SERIAL_PROTOCOL(float(st_get_position(X_AXIS))/axis_steps_per_unit[X_AXIS]);
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(float(st_get_position(Y_AXIS))/axis_steps_per_unit[Y_AXIS]);
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(float(st_get_position(Z_AXIS))/axis_steps_per_unit[Z_AXIS]);

  SERIAL_PROTOCOLLN("");

  #ifdef SCARA
    SERIAL_PROTOCOLPGM("SCARA Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL(delta[Y_AXIS]);
    SERIAL_PROTOCOLLN("");
    
    SERIAL_PROTOCOLPGM("SCARA Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]+home_offset[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta (90):");
    SERIAL_PROTOCOL(delta[Y_AXIS]-delta[X_AXIS]-90+home_offset[Y_AXIS]);
    SERIAL_PROTOCOLLN("");
    
    SERIAL_PROTOCOLPGM("SCARA step Cal - Theta:");
    SERIAL_PROTOCOL(delta[X_AXIS]/90*axis_steps_per_unit[X_AXIS]);
    SERIAL_PROTOCOLPGM("   Psi+Theta:");
    SERIAL_PROTOCOL((delta[Y_AXIS]-delta[X_AXIS])/90*axis_steps_per_unit[Y_AXIS]);
    SERIAL_PROTOCOLLN("");
    SERIAL_PROTOCOLLN("");
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
  char* codepos = strchr_pointer + 5;
  char* starpos = strchr(codepos, '*');
  if (starpos) *starpos = '\0';
  lcd_setstatus(codepos);
}

/**
 * M119: Output endstop states to serial output
 */
inline void gcode_M119() {
  SERIAL_PROTOCOLLN(MSG_M119_REPORT);
  #if defined(X_MIN_PIN) && X_MIN_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_X_MIN);
    SERIAL_PROTOCOLLN(((READ(X_MIN_PIN)^X_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(X_MAX_PIN) && X_MAX_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_X_MAX);
    SERIAL_PROTOCOLLN(((READ(X_MAX_PIN)^X_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(Y_MIN_PIN) && Y_MIN_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_Y_MIN);
    SERIAL_PROTOCOLLN(((READ(Y_MIN_PIN)^Y_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(Y_MAX_PIN) && Y_MAX_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_Y_MAX);
    SERIAL_PROTOCOLLN(((READ(Y_MAX_PIN)^Y_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(Z_MIN_PIN) && Z_MIN_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_Z_MIN);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PIN)^Z_MIN_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(Z_MAX_PIN) && Z_MAX_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_Z_MAX);
    SERIAL_PROTOCOLLN(((READ(Z_MAX_PIN)^Z_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  #if defined(Z2_MAX_PIN) && Z2_MAX_PIN > -1
    SERIAL_PROTOCOLPGM(MSG_Z2_MAX);
    SERIAL_PROTOCOLLN(((READ(Z2_MAX_PIN)^Z2_MAX_ENDSTOP_INVERTING)?MSG_ENDSTOP_HIT:MSG_ENDSTOP_OPEN));
  #endif
  
}

/**
 * M120: Enable endstops
 */
inline void gcode_M120() { enable_endstops(false); }

/**
 * M121: Disable endstops
 */
inline void gcode_M121() { enable_endstops(true); }

#ifdef BLINKM

  /**
   * M150: Set Status LED Color - Use R-U-B for R-G-B
   */
  inline void gcode_M150() {
    SendColors(
      code_seen('R') ? (byte)code_value() : 0,
      code_seen('U') ? (byte)code_value() : 0,
      code_seen('B') ? (byte)code_value() : 0
    );
  }

#endif // BLINKM

/**
 * M200: Set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
 *       T<extruder>
 *       D<millimeters>
 */
inline void gcode_M200() {
  tmp_extruder = active_extruder;
  if (code_seen('T')) {
    tmp_extruder = code_value();
    if (tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      SERIAL_ECHO(MSG_M200_INVALID_EXTRUDER);
      return;
    }
  }

  if (code_seen('D')) {
    float diameter = code_value();
    // setting any extruder filament size disables volumetric on the assumption that
    // slicers either generate in extruder values as cubic mm or as as filament feeds
    // for all extruders
    volumetric_enabled = (diameter != 0.0);
    if (volumetric_enabled) {
      filament_size[tmp_extruder] = diameter;
      // make sure all extruders have some sane value for the filament size
      for (int i=0; i<EXTRUDERS; i++)
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
  for (int8_t i=0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      max_acceleration_units_per_sq_second[i] = code_value();
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  reset_acceleration_rates();
}

#if 0 // Not used for Sprinter/grbl gen6
  inline void gcode_M202() {
    for(int8_t i=0; i < NUM_AXIS; i++) {
      if(code_seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
    }
  }
#endif


/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
 */
inline void gcode_M203() {
  for (int8_t i=0; i < NUM_AXIS; i++) {
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
  if (code_seen('S'))   // Kept for legacy compatibility. Should NOT BE USED for new developments.
  {
    acceleration = code_value();
    travel_acceleration = acceleration;
    SERIAL_ECHOPAIR("Setting Printing and Travelling Acceleration: ", acceleration );
    SERIAL_EOL;
  }
  if (code_seen('P'))
  {
    acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Printing Acceleration: ", acceleration );
    SERIAL_EOL;
  }
  if (code_seen('R'))
  {
    retract_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Retract Acceleration: ", retract_acceleration );
    SERIAL_EOL;
  }
  if (code_seen('T'))
  {
    travel_acceleration = code_value();
    SERIAL_ECHOPAIR("Setting Travel Acceleration: ", travel_acceleration );
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
  for (int8_t i=X_AXIS; i <= Z_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      home_offset[i] = code_value();
    }
  }
  #ifdef SCARA
    if (code_seen('T')) home_offset[X_AXIS] = code_value(); // Theta
    if (code_seen('P')) home_offset[Y_AXIS] = code_value(); // Psi
  #endif
}

#ifdef DELTA
  /**
   * M665: Set delta configurations
   *
   *    L = diagonal rod
   *    R = delta radius
   *    S = segments per second
   */
  inline void gcode_M665() {
    if (code_seen('L')) delta_diagonal_rod = code_value();
    if (code_seen('R')) delta_radius = code_value();
    if (code_seen('S')) delta_segments_per_second = code_value();
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  }
  /**
   * M666: Set delta endstop adjustment
   */
  inline void gcode_M666() {
    for (int8_t i = 0; i < 3; i++) {
      if (code_seen(axis_codes[i])) {
        endstop_adj[i] = code_value();
      }
    }
  }
#elif defined(Z_DUAL_ENDSTOPS)
  /**
   * M666: For Z Dual Endstop setup, set z axis offset to the z2 axis.
   */
  inline void gcode_M666() {
   if (code_seen('Z')) z_endstop_adj = code_value();
   SERIAL_ECHOPAIR("Z Endstop Adjustment set to (mm):", z_endstop_adj );
   SERIAL_EOL;
  }
  
#endif // DELTA

#ifdef FWRETRACT

  /**
   * M207: Set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop]
   */
  inline void gcode_M207() {
    if (code_seen('S')) retract_length = code_value();
    if (code_seen('F')) retract_feedrate = code_value() / 60;
    if (code_seen('Z')) retract_zlift = code_value();
  }

  /**
   * M208: Set retract recover length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
   */
  inline void gcode_M208() {
    if (code_seen('S')) retract_recover_length = code_value();
    if (code_seen('F')) retract_recover_feedrate = code_value() / 60;
  }

  /**
   * M209: Enable automatic retract (M209 S1)
   *       detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
   */
  inline void gcode_M209() {
    if (code_seen('S')) {
      int t = code_value();
      switch(t) {
        case 0:
          autoretract_enabled = false;
          break;
        case 1:
          autoretract_enabled = true;
          break;
        default:
          SERIAL_ECHO_START;
          SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
          SERIAL_ECHO(cmdbuffer[bufindr]);
          SERIAL_ECHOLNPGM("\"");
          return;
      }
      for (int i=0; i<EXTRUDERS; i++) retracted[i] = false;
    }
  }

#endif // FWRETRACT

#if EXTRUDERS > 1

  /**
   * M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
   */
  inline void gcode_M218() {
    if (setTargetedHotend(218)) return;

    if (code_seen('X')) extruder_offset[X_AXIS][tmp_extruder] = code_value();
    if (code_seen('Y')) extruder_offset[Y_AXIS][tmp_extruder] = code_value();

    #ifdef DUAL_X_CARRIAGE
      if (code_seen('Z')) extruder_offset[Z_AXIS][tmp_extruder] = code_value();
    #endif

    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    for (tmp_extruder = 0; tmp_extruder < EXTRUDERS; tmp_extruder++) {
      SERIAL_ECHO(" ");
      SERIAL_ECHO(extruder_offset[X_AXIS][tmp_extruder]);
      SERIAL_ECHO(",");
      SERIAL_ECHO(extruder_offset[Y_AXIS][tmp_extruder]);
      #ifdef DUAL_X_CARRIAGE
        SERIAL_ECHO(",");
        SERIAL_ECHO(extruder_offset[Z_AXIS][tmp_extruder]);
      #endif
    }
    SERIAL_EOL;
  }

#endif // EXTRUDERS > 1

/**
 * M220: Set speed percentage factor, aka "Feed Rate" (M220 S95)
 */
inline void gcode_M220() {
  if (code_seen('S')) feedmultiply = code_value();
}

/**
 * M221: Set extrusion percentage (M221 T0 S95)
 */
inline void gcode_M221() {
  if (code_seen('S')) {
    int sval = code_value();
    if (code_seen('T')) {
      if (setTargetedHotend(221)) return;
      extruder_multiply[tmp_extruder] = sval;
    }
    else {
      extrudemultiply = sval;
    }
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

      for (int8_t i = 0; i < (int8_t)(sizeof(sensitive_pins)/sizeof(*sensitive_pins)); i++) {
        if (sensitive_pins[i] == pin_number) {
          pin_number = -1;
          break;
        }
      }

      if (pin_number > -1) {
        int target = LOW;

        st_synchronize();

        pinMode(pin_number, INPUT);

        switch(pin_state){
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

        while(digitalRead(pin_number) != target) {
          manage_heater();
          manage_inactivity();
          lcd_update();
        }

      } // pin_number > -1
    } // pin_state -1 0 1
  } // code_seen('P')
}

#if NUM_SERVOS > 0

  /**
   * M280: Set servo position absolute. P: servo index, S: angle or microseconds
   */
  inline void gcode_M280() {
    int servo_index = code_seen('P') ? code_value() : -1;
    int servo_position = 0;
    if (code_seen('S')) {
      servo_position = code_value();
      if ((servo_index >= 0) && (servo_index < NUM_SERVOS)) {
        #if SERVO_LEVELING
          servos[servo_index].attach(0);
        #endif
        servos[servo_index].write(servo_position);
        #if SERVO_LEVELING
          delay(PROBE_SERVO_DEACTIVATION_DELAY);
          servos[servo_index].detach();
        #endif
      }
      else {
        SERIAL_ECHO_START;
        SERIAL_ECHO("Servo ");
        SERIAL_ECHO(servo_index);
        SERIAL_ECHOLN(" out of range");
      }
    }
    else if (servo_index >= 0) {
      SERIAL_PROTOCOL(MSG_OK);
      SERIAL_PROTOCOL(" Servo ");
      SERIAL_PROTOCOL(servo_index);
      SERIAL_PROTOCOL(": ");
      SERIAL_PROTOCOL(servos[servo_index].read());
      SERIAL_PROTOCOLLN("");
    }
  }

#endif // NUM_SERVOS > 0

#if defined(LARGE_FLASH) && (BEEPER > 0 || defined(ULTRALCD) || defined(LCD_USE_I2C_BUZZER))

  /**
   * M300: Play beep sound S<frequency Hz> P<duration ms>
   */
  inline void gcode_M300() {
    int beepS = code_seen('S') ? code_value() : 110;
    int beepP = code_seen('P') ? code_value() : 1000;
    if (beepS > 0) {
      #if BEEPER > 0
        tone(BEEPER, beepS);
        delay(beepP);
        noTone(BEEPER);
      #elif defined(ULTRALCD)
        lcd_buzz(beepS, beepP);
      #elif defined(LCD_USE_I2C_BUZZER)
        lcd_buzz(beepP, beepS);
      #endif
    }
    else {
      delay(beepP);
    }
  }

#endif // LARGE_FLASH && (BEEPER>0 || ULTRALCD || LCD_USE_I2C_BUZZER)

#ifdef PIDTEMP

  /**
   * M301: Set PID parameters P I D (and optionally C)
   */
  inline void gcode_M301() {

    // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
    // default behaviour (omitting E parameter) is to update for extruder 0 only
    int e = code_seen('E') ? code_value() : 0; // extruder being updated

    if (e < EXTRUDERS) { // catch bad input value
      if (code_seen('P')) PID_PARAM(Kp, e) = code_value();
      if (code_seen('I')) PID_PARAM(Ki, e) = scalePID_i(code_value());
      if (code_seen('D')) PID_PARAM(Kd, e) = scalePID_d(code_value());
      #ifdef PID_ADD_EXTRUSION_RATE
        if (code_seen('C')) PID_PARAM(Kc, e) = code_value();
      #endif      

      updatePID();
      SERIAL_PROTOCOL(MSG_OK);
      #ifdef PID_PARAMS_PER_EXTRUDER
        SERIAL_PROTOCOL(" e:"); // specify extruder in serial output
        SERIAL_PROTOCOL(e);
      #endif // PID_PARAMS_PER_EXTRUDER
      SERIAL_PROTOCOL(" p:");
      SERIAL_PROTOCOL(PID_PARAM(Kp, e));
      SERIAL_PROTOCOL(" i:");
      SERIAL_PROTOCOL(unscalePID_i(PID_PARAM(Ki, e)));
      SERIAL_PROTOCOL(" d:");
      SERIAL_PROTOCOL(unscalePID_d(PID_PARAM(Kd, e)));
      #ifdef PID_ADD_EXTRUSION_RATE
        SERIAL_PROTOCOL(" c:");
        //Kc does not have scaling applied above, or in resetting defaults
        SERIAL_PROTOCOL(PID_PARAM(Kc, e));
      #endif
      SERIAL_PROTOCOLLN("");    
    }
    else {
      SERIAL_ECHO_START;
      SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
    }
  }

#endif // PIDTEMP

#ifdef PIDTEMPBED

  inline void gcode_M304() {
    if (code_seen('P')) bedKp = code_value();
    if (code_seen('I')) bedKi = scalePID_i(code_value());
    if (code_seen('D')) bedKd = scalePID_d(code_value());

    updatePID();
    SERIAL_PROTOCOL(MSG_OK);
    SERIAL_PROTOCOL(" p:");
    SERIAL_PROTOCOL(bedKp);
    SERIAL_PROTOCOL(" i:");
    SERIAL_PROTOCOL(unscalePID_i(bedKi));
    SERIAL_PROTOCOL(" d:");
    SERIAL_PROTOCOL(unscalePID_d(bedKd));
    SERIAL_PROTOCOLLN("");
  }

#endif // PIDTEMPBED

#if defined(CHDK) || (defined(PHOTOGRAPH_PIN) && PHOTOGRAPH_PIN > -1)

  /**
   * M240: Trigger a camera by emulating a Canon RC-1
   *       See http://www.doc-diy.net/photo/rc-1_hacked/
   */
  inline void gcode_M240() {
    #ifdef CHDK
     
       OUT_WRITE(CHDK, HIGH);
       chdkHigh = millis();
       chdkActive = true;
     
    #elif defined(PHOTOGRAPH_PIN) && PHOTOGRAPH_PIN > -1

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

    #endif // !CHDK && PHOTOGRAPH_PIN > -1
  }

#endif // CHDK || PHOTOGRAPH_PIN

#ifdef DOGLCD

  /**
   * M250: Read and optionally set the LCD contrast
   */
  inline void gcode_M250() {
    if (code_seen('C')) lcd_setcontrast(code_value_long() & 0x3F);
    SERIAL_PROTOCOLPGM("lcd contrast value: ");
    SERIAL_PROTOCOL(lcd_contrast);
    SERIAL_PROTOCOLLN("");
  }

#endif // DOGLCD

#ifdef PREVENT_DANGEROUS_EXTRUDE

  /**
   * M302: Allow cold extrudes, or set the minimum extrude S<temperature>.
   */
  inline void gcode_M302() {
    set_extrude_min_temp(code_seen('S') ? code_value() : 0);
  }

#endif // PREVENT_DANGEROUS_EXTRUDE

/**
 * M303: PID relay autotune
 *       S<temperature> sets the target temperature. (default target temperature = 150C)
 *       E<extruder> (-1 for the bed)
 *       C<cycles>
 */
inline void gcode_M303() {
  int e = code_seen('E') ? code_value_long() : 0;
  int c = code_seen('C') ? code_value_long() : 5;
  float temp = code_seen('S') ? code_value() : (e < 0 ? 70.0 : 150.0);
  PID_autotune(temp, e, c);
}

#ifdef SCARA
  bool SCARA_move_to_cal(uint8_t delta_x, uint8_t delta_y) {
    //SoftEndsEnabled = false;              // Ignore soft endstops during calibration
    //SERIAL_ECHOLN(" Soft endstops disabled ");
    if (! Stopped) {
      //get_coordinates(); // For X Y Z E F
      delta[X_AXIS] = delta_x;
      delta[Y_AXIS] = delta_y;
      calculate_SCARA_forward_Transform(delta);
      destination[X_AXIS] = delta[X_AXIS]/axis_scaling[X_AXIS];
      destination[Y_AXIS] = delta[Y_AXIS]/axis_scaling[Y_AXIS];
      prepare_move();
      //ClearToSend();
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

#ifdef EXT_SOLENOID

  void enable_solenoid(uint8_t num) {
    switch(num) {
      case 0:
        OUT_WRITE(SOL0_PIN, HIGH);
        break;
        #if defined(SOL1_PIN) && SOL1_PIN > -1
          case 1:
            OUT_WRITE(SOL1_PIN, HIGH);
            break;
        #endif
        #if defined(SOL2_PIN) && SOL2_PIN > -1
          case 2:
            OUT_WRITE(SOL2_PIN, HIGH);
            break;
        #endif
        #if defined(SOL3_PIN) && SOL3_PIN > -1
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

#if defined(ENABLE_AUTO_BED_LEVELING) && (defined(SERVO_ENDSTOPS) || defined(Z_PROBE_ALLEN_KEY)) && not defined(Z_PROBE_SLED)

  /**
   * M401: Engage Z Servo endstop if available
   */
  inline void gcode_M401() { engage_z_probe(); }
  /**
   * M402: Retract Z Servo endstop if enabled
   */
  inline void gcode_M402() { retract_z_probe(); }

#endif

#ifdef FILAMENT_SENSOR

  /**
   * M404: Display or set the nominal filament width (3mm, 1.75mm ) W<3.0>
   */
  inline void gcode_M404() {
    #if FILWIDTH_PIN > -1
      if (code_seen('W')) {
        filament_width_nominal = code_value();
      }
      else {
        SERIAL_PROTOCOLPGM("Filament dia (nominal mm):");
        SERIAL_PROTOCOLLN(filament_width_nominal);
      }
    #endif
  }
    
  /**
   * M405: Turn on filament sensor for control
   */
  inline void gcode_M405() {
    if (code_seen('D')) meas_delay_cm = code_value();
    if (meas_delay_cm > MAX_MEASUREMENT_DELAY) meas_delay_cm = MAX_MEASUREMENT_DELAY;

    if (delay_index2 == -1) { //initialize the ring buffer if it has not been done since startup
      int temp_ratio = widthFil_to_size_ratio();

      for (delay_index1 = 0; delay_index1 < MAX_MEASUREMENT_DELAY + 1; ++delay_index1)
        measurement_delay[delay_index1] = temp_ratio - 100;  //subtract 100 to scale within a signed byte

      delay_index1 = delay_index2 = 0;
    }

    filament_sensor = true;

    //SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
    //SERIAL_PROTOCOL(filament_width_meas);
    //SERIAL_PROTOCOLPGM("Extrusion ratio(%):");
    //SERIAL_PROTOCOL(extrudemultiply);
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

#endif // FILAMENT_SENSOR

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

#ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

  /**
   * M540: Set whether SD card print should abort on endstop hit (M540 S<0|1>)
   */
  inline void gcode_M540() {
    if (code_seen('S')) abort_on_endstop_hit = (code_value() > 0);
  }

#endif // ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

#ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

  inline void gcode_SET_Z_PROBE_OFFSET() {
    float value;
    if (code_seen('Z')) {
      value = code_value();
      if (Z_PROBE_OFFSET_RANGE_MIN <= value && value <= Z_PROBE_OFFSET_RANGE_MAX) {
        zprobe_zoffset = -value; // compare w/ line 278 of ConfigurationStore.cpp
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_ZPROBE_ZOFFSET " " MSG_OK);
        SERIAL_PROTOCOLLN("");
      }
      else {
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM(MSG_ZPROBE_ZOFFSET);
        SERIAL_ECHOPGM(MSG_Z_MIN);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MIN);
        SERIAL_ECHOPGM(MSG_Z_MAX);
        SERIAL_ECHO(Z_PROBE_OFFSET_RANGE_MAX);
        SERIAL_PROTOCOLLN("");
      }
    }
    else {
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_ZPROBE_ZOFFSET " : ");
      SERIAL_ECHO(-zprobe_zoffset);
      SERIAL_PROTOCOLLN("");
    }
  }

#endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

#ifdef FILAMENTCHANGEENABLE

  /**
   * M600: Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
   */
  inline void gcode_M600() {
    float target[NUM_AXIS], lastpos[NUM_AXIS], fr60 = feedrate / 60;
    for (int i=0; i<NUM_AXIS; i++)
      target[i] = lastpos[i] = current_position[i];

    #define BASICPLAN plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], fr60, active_extruder);
    #ifdef DELTA
      #define RUNPLAN calculate_delta(target); BASICPLAN
    #else
      #define RUNPLAN BASICPLAN
    #endif

    //retract by E
    if (code_seen('E')) target[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FIRSTRETRACT
      else target[E_AXIS] += FILAMENTCHANGE_FIRSTRETRACT;
    #endif

    RUNPLAN;

    //lift Z
    if (code_seen('Z')) target[Z_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_ZADD
      else target[Z_AXIS] += FILAMENTCHANGE_ZADD;
    #endif

    RUNPLAN;

    //move xy
    if (code_seen('X')) target[X_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_XPOS
      else target[X_AXIS] = FILAMENTCHANGE_XPOS;
    #endif

    if (code_seen('Y')) target[Y_AXIS] = code_value();
    #ifdef FILAMENTCHANGE_YPOS
      else target[Y_AXIS] = FILAMENTCHANGE_YPOS;
    #endif

    RUNPLAN;

    if (code_seen('L')) target[E_AXIS] += code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else target[E_AXIS] += FILAMENTCHANGE_FINALRETRACT;
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
    uint8_t cnt = 0;
    while (!lcd_clicked()) {
      cnt++;
      manage_heater();
      manage_inactivity(true);
      lcd_update();
      if (cnt == 0) {
        #if BEEPER > 0
          OUT_WRITE(BEEPER,HIGH);
          delay(3);
          WRITE(BEEPER,LOW);
          delay(3);
        #else
          #if !defined(LCD_FEEDBACK_FREQUENCY_HZ) || !defined(LCD_FEEDBACK_FREQUENCY_DURATION_MS)
            lcd_buzz(1000/6, 100);
          #else
            lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);
          #endif
        #endif
      }
    } // while(!lcd_clicked)

    //return to normal
    if (code_seen('L')) target[E_AXIS] -= code_value();
    #ifdef FILAMENTCHANGE_FINALRETRACT
      else target[E_AXIS] -= FILAMENTCHANGE_FINALRETRACT;
    #endif

    current_position[E_AXIS] = target[E_AXIS]; //the long retract of L is compensated by manual filament feeding
    plan_set_e_position(current_position[E_AXIS]);

    RUNPLAN; //should do nothing

    lcd_reset_alert_level();

    #ifdef DELTA
      calculate_delta(lastpos);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move xyz back
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], lastpos[E_AXIS], fr60, active_extruder); //final untretract
    #else
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], target[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move xy back
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], target[E_AXIS], fr60, active_extruder); //move z back
      plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], fr60, active_extruder); //final untretract
    #endif        

    #ifdef FILAMENT_RUNOUT_SENSOR
      filrunoutEnqued = false;
    #endif
    
  }

#endif // FILAMENTCHANGEENABLE

#ifdef DUAL_X_CARRIAGE

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
    switch(dual_x_carriage_mode) {
      case DXC_DUPLICATION_MODE:
        if (code_seen('X')) duplicate_extruder_x_offset = max(code_value(), X2_MIN_POS - x_home_pos(0));
        if (code_seen('R')) duplicate_extruder_temp_offset = code_value();
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
        SERIAL_ECHO(" ");
        SERIAL_ECHO(extruder_offset[X_AXIS][0]);
        SERIAL_ECHO(",");
        SERIAL_ECHO(extruder_offset[Y_AXIS][0]);
        SERIAL_ECHO(" ");
        SERIAL_ECHO(duplicate_extruder_x_offset);
        SERIAL_ECHO(",");
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
    for (int i=0;i<NUM_AXIS;i++)
      if (code_seen(axis_codes[i])) digipot_current(i, code_value());
    if (code_seen('B')) digipot_current(4, code_value());
    if (code_seen('S')) for (int i=0; i<=4; i++) digipot_current(i, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_XY_PIN
    if (code_seen('X')) digipot_current(0, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_Z_PIN
    if (code_seen('Z')) digipot_current(1, code_value());
  #endif
  #ifdef MOTOR_CURRENT_PWM_E_PIN
    if (code_seen('E')) digipot_current(2, code_value());
  #endif
  #ifdef DIGIPOT_I2C
    // this one uses actual amps in floating point
    for (int i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) digipot_i2c_set_current(i, code_value());
    // for each additional extruder (named B,C,D,E..., channels 4,5,6,7...)
    for (int i=NUM_AXIS;i<DIGIPOT_I2C_NUM_CHANNELS;i++) if(code_seen('B'+i-NUM_AXIS)) digipot_i2c_set_current(i, code_value());
  #endif
}

#if HAS_DIGIPOTSS

  /**
   * M908: Control digital trimpot directly (M908 P<pin> S<current>)
   */
  inline void gcode_M908() {
      digitalPotWrite(
        code_seen('P') ? code_value() : 0,
        code_seen('S') ? code_value() : 0
      );
  }

#endif // HAS_DIGIPOTSS

// M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
inline void gcode_M350() {
  #if defined(X_MS1_PIN) && X_MS1_PIN > -1
    if(code_seen('S')) for(int i=0;i<=4;i++) microstep_mode(i,code_value());
    for(int i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_mode(i,(uint8_t)code_value());
    if(code_seen('B')) microstep_mode(4,code_value());
    microstep_readings();
  #endif
}

/**
 * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
 *       S# determines MS1 or MS2, X# sets the pin high/low.
 */
inline void gcode_M351() {
  #if defined(X_MS1_PIN) && X_MS1_PIN > -1
    if (code_seen('S')) switch(code_value_long()) {
      case 1:
        for(int i=0;i<NUM_AXIS;i++) if (code_seen(axis_codes[i])) microstep_ms(i, code_value(), -1);
        if (code_seen('B')) microstep_ms(4, code_value(), -1);
        break;
      case 2:
        for(int i=0;i<NUM_AXIS;i++) if (code_seen(axis_codes[i])) microstep_ms(i, -1, code_value());
        if (code_seen('B')) microstep_ms(4, -1, code_value());
        break;
    }
    microstep_readings();
  #endif
}

/**
 * M999: Restart after being stopped
 */
inline void gcode_M999() {
  Stopped = false;
  lcd_reset_alert_level();
  gcode_LastN = Stopped_gcode_LastN;
  FlushSerialRequestResend();
}

inline void gcode_T() {
  tmp_extruder = code_value();
  if (tmp_extruder >= EXTRUDERS) {
    SERIAL_ECHO_START;
    SERIAL_ECHO("T");
    SERIAL_ECHO(tmp_extruder);
    SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
  }
  else {
    #if EXTRUDERS > 1
      bool make_move = false;
    #endif
    if (code_seen('F')) {
      #if EXTRUDERS > 1
        make_move = true;
      #endif
      next_feedrate = code_value();
      if (next_feedrate > 0.0) feedrate = next_feedrate;
    }
    #if EXTRUDERS > 1
      if (tmp_extruder != active_extruder) {
        // Save current position to return to after applying extruder offset
        memcpy(destination, current_position, sizeof(destination));
        #ifdef DUAL_X_CARRIAGE
          if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE && Stopped == false &&
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
          current_position[Y_AXIS] = current_position[Y_AXIS] -
                       extruder_offset[Y_AXIS][active_extruder] +
                       extruder_offset[Y_AXIS][tmp_extruder];
          current_position[Z_AXIS] = current_position[Z_AXIS] -
                       extruder_offset[Z_AXIS][active_extruder] +
                       extruder_offset[Z_AXIS][tmp_extruder];

          active_extruder = tmp_extruder;

          // This function resets the max/min values - the current position may be overwritten below.
          axis_is_at_home(X_AXIS);

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
          // Offset extruder (only by XY)
          for (int i=X_AXIS; i<=Y_AXIS; i++)
            current_position[i] += extruder_offset[i][tmp_extruder] - extruder_offset[i][active_extruder];
          // Set the new active extruder and position
          active_extruder = tmp_extruder;
        #endif // !DUAL_X_CARRIAGE
        #ifdef DELTA
          calculate_delta(current_position); // change cartesian kinematic  to  delta kinematic;
          //sent position to plan_set_position();
          plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS],current_position[E_AXIS]);
        #else
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
        #endif
        // Move to the old position if 'F' was in the parameters
        if (make_move && !Stopped) prepare_move();
      }

      #ifdef EXT_SOLENOID
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
 * Process Commands and dispatch them to handlers
 */
void process_commands() {
  if (code_seen('G')) {

    int gCode = code_value_long();

    switch(gCode) {

    // G0, G1
    case 0:
    case 1:
      gcode_G0_G1();
      break;

    // G2, G3
    #ifndef SCARA
      case 2: // G2  - CW ARC
      case 3: // G3  - CCW ARC
        gcode_G2_G3(gCode == 2);
        break;
    #endif

    // G4 Dwell
    case 4:
      gcode_G4();
      break;

    #ifdef FWRETRACT

      case 10: // G10: retract
      case 11: // G11: retract_recover
        gcode_G10_G11(gCode == 10);
        break;

    #endif //FWRETRACT

    case 28: // G28: Home all axes, one at a time
      gcode_G28();
      break;

    #if defined(MESH_BED_LEVELING)
      case 29: // G29 Handle mesh based leveling
        gcode_G29();
        break;
    #endif

    #ifdef ENABLE_AUTO_BED_LEVELING

      case 29: // G29 Detailed Z-Probe, probes the bed at 3 or more points.
        gcode_G29();
        break;

      #ifndef Z_PROBE_SLED

        case 30: // G30 Single Z Probe
          gcode_G30();
          break;

      #else // Z_PROBE_SLED

          case 31: // G31: dock the sled
          case 32: // G32: undock the sled
            dock_sled(gCode == 31);
            break;

      #endif // Z_PROBE_SLED

    #endif // ENABLE_AUTO_BED_LEVELING

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
  }

  else if (code_seen('M')) {
    switch( code_value_long() ) {
      #ifdef ULTIPANEL
        case 0: // M0 - Unconditional stop - Wait for user button press on LCD
        case 1: // M1 - Conditional stop - Wait for user button press on LCD
          gcode_M0_M1();
          break;
      #endif // ULTIPANEL

      case 17:
        gcode_M17();
        break;

      #ifdef SDSUPPORT

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
        case 928: //M928 - Start SD write
          gcode_M928(); break;

      #endif //SDSUPPORT

      case 31: //M31 take time since the start of the SD print or an M109 command
        gcode_M31();
        break;

      case 42: //M42 -Change pin status via gcode
        gcode_M42();
        break;

      #if defined(ENABLE_AUTO_BED_LEVELING) && defined(Z_PROBE_REPEATABILITY_TEST)
        case 48: // M48 Z-Probe repeatability
          gcode_M48();
          break;
      #endif // ENABLE_AUTO_BED_LEVELING && Z_PROBE_REPEATABILITY_TEST

      case 104: // M104
        gcode_M104();
        break;

      case 112: //  M112 Emergency Stop
        gcode_M112();
        break;

      case 140: // M140 Set bed temp
        gcode_M140();
        break;

      case 105: // M105 Read current temperature
        gcode_M105();
        return;
        break;

      case 109: // M109 Wait for temperature
        gcode_M109();
        break;

      #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
        case 190: // M190 - Wait for bed heater to reach target.
          gcode_M190();
          break;
      #endif //TEMP_BED_PIN

      #if defined(FAN_PIN) && FAN_PIN > -1
        case 106: //M106 Fan On
          gcode_M106();
          break;
        case 107: //M107 Fan Off
          gcode_M107();
          break;
      #endif //FAN_PIN

      #ifdef BARICUDA
        // PWM for HEATER_1_PIN
        #if defined(HEATER_1_PIN) && HEATER_1_PIN > -1
          case 126: // M126 valve open
            gcode_M126();
            break;
          case 127: // M127 valve closed
            gcode_M127();
            break;
        #endif //HEATER_1_PIN

        // PWM for HEATER_2_PIN
        #if defined(HEATER_2_PIN) && HEATER_2_PIN > -1
          case 128: // M128 valve open
            gcode_M128();
            break;
          case 129: // M129 valve closed
            gcode_M129();
            break;
        #endif //HEATER_2_PIN
      #endif //BARICUDA

      #if defined(PS_ON_PIN) && PS_ON_PIN > -1

        case 80: // M80 - Turn on Power Supply
          gcode_M80();
          break;

      #endif // PS_ON_PIN

      case 81: // M81 - Turn off Power Supply
        gcode_M81();
        break;

      case 82:
        gcode_M82();
        break;
      case 83:
        gcode_M83();
        break;
      case 18: //compatibility
      case 84: // M84
        gcode_M18_M84();
        break;
      case 85: // M85
        gcode_M85();
        break;
      case 92: // M92
        gcode_M92();
        break;
      case 115: // M115
        gcode_M115();
        break;
      case 117: // M117 display message
        gcode_M117();
        break;
      case 114: // M114
        gcode_M114();
        break;
      case 120: // M120
        gcode_M120();
        break;
      case 121: // M121
        gcode_M121();
        break;
      case 119: // M119
        gcode_M119();
        break;
        //TODO: update for all axis, use for loop

      #ifdef BLINKM

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

      #ifdef DELTA
        case 665: // M665 set delta configurations L<diagonal_rod> R<delta_radius> S<segments_per_sec>
          gcode_M665();
          break;
        case 666: // M666 set delta endstop adjustment
          gcode_M666();
          break;
      #elif defined(Z_DUAL_ENDSTOPS)
        case 666: // M666 set delta endstop adjustment
          gcode_M666();
          break;
      #endif // DELTA

      #ifdef FWRETRACT
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

      #if NUM_SERVOS > 0
        case 280: // M280 - set servo position absolute. P: servo index, S: angle or microseconds
          gcode_M280();
          break;
      #endif // NUM_SERVOS > 0

      #if defined(LARGE_FLASH) && (BEEPER > 0 || defined(ULTRALCD) || defined(LCD_USE_I2C_BUZZER))
        case 300: // M300 - Play beep tone
          gcode_M300();
          break;
      #endif // LARGE_FLASH && (BEEPER>0 || ULTRALCD || LCD_USE_I2C_BUZZER)

      #ifdef PIDTEMP
        case 301: // M301
          gcode_M301();
          break;
      #endif // PIDTEMP

      #ifdef PIDTEMPBED
        case 304: // M304
          gcode_M304();
          break;
      #endif // PIDTEMPBED

      #if defined(CHDK) || (defined(PHOTOGRAPH_PIN) && PHOTOGRAPH_PIN > -1)
        case 240: // M240  Triggers a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
          gcode_M240();
          break;
      #endif // CHDK || PHOTOGRAPH_PIN

      #ifdef DOGLCD
        case 250: // M250  Set LCD contrast value: C<value> (value 0..63)
          gcode_M250();
          break;
      #endif // DOGLCD

      #ifdef PREVENT_DANGEROUS_EXTRUDE
        case 302: // allow cold extrudes, or set the minimum extrude temperature
          gcode_M302();
          break;
      #endif // PREVENT_DANGEROUS_EXTRUDE

      case 303: // M303 PID autotune
        gcode_M303();
        break;

      #ifdef SCARA
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

      #if defined(ENABLE_AUTO_BED_LEVELING) && (defined(SERVO_ENDSTOPS) || defined(Z_PROBE_ALLEN_KEY)) && not defined(Z_PROBE_SLED)
        case 401:
          gcode_M401();
          break;
        case 402:
          gcode_M402();
          break;
      #endif

      #ifdef FILAMENT_SENSOR
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
      #endif // FILAMENT_SENSOR

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

      #ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED
        case 540:
          gcode_M540();
          break;
      #endif

      #ifdef CUSTOM_M_CODE_SET_Z_PROBE_OFFSET
        case CUSTOM_M_CODE_SET_Z_PROBE_OFFSET:
          gcode_SET_Z_PROBE_OFFSET();
          break;
      #endif // CUSTOM_M_CODE_SET_Z_PROBE_OFFSET

      #ifdef FILAMENTCHANGEENABLE
        case 600: //Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
          gcode_M600();
          break;
      #endif // FILAMENTCHANGEENABLE

      #ifdef DUAL_X_CARRIAGE
        case 605:
          gcode_M605();
          break;
      #endif // DUAL_X_CARRIAGE

      case 907: // M907 Set digital trimpot motor current using axis codes.
        gcode_M907();
        break;

      #if HAS_DIGIPOTSS
        case 908: // M908 Control digital trimpot directly.
          gcode_M908();
          break;
      #endif // HAS_DIGIPOTSS

      case 350: // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
        gcode_M350();
        break;

      case 351: // M351 Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
        gcode_M351();
        break;

      case 999: // M999: Restart after being Stopped
        gcode_M999();
        break;
    }
  }

  else if (code_seen('T')) {
    gcode_T();
  }

  else {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
    SERIAL_ECHO(cmdbuffer[bufindr]);
    SERIAL_ECHOLNPGM("\"");
  }

  ClearToSend();
}

void FlushSerialRequestResend()
{
  //char cmdbuffer[bufindr][100]="Resend:";
  MYSERIAL.flush();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ClearToSend();
}

void ClearToSend()
{
  previous_millis_cmd = millis();
  #ifdef SDSUPPORT
  if(fromsd[bufindr])
    return;
  #endif //SDSUPPORT
  SERIAL_PROTOCOLLNPGM(MSG_OK);
}

void get_coordinates() {
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i]))
      destination[i] = code_value() + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
    else
      destination[i] = current_position[i];
  }
  if (code_seen('F')) {
    next_feedrate = code_value();
    if (next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

void get_arc_coordinates()
{
#ifdef SF_ARC_FIX
   bool relative_mode_backup = relative_mode;
   relative_mode = true;
#endif
   get_coordinates();
#ifdef SF_ARC_FIX
   relative_mode=relative_mode_backup;
#endif

   if(code_seen('I')) {
     offset[0] = code_value();
   }
   else {
     offset[0] = 0.0;
   }
   if(code_seen('J')) {
     offset[1] = code_value();
   }
   else {
     offset[1] = 0.0;
   }
}

void clamp_to_software_endstops(float target[3])
{
  if (min_software_endstops) {
    if (target[X_AXIS] < min_pos[X_AXIS]) target[X_AXIS] = min_pos[X_AXIS];
    if (target[Y_AXIS] < min_pos[Y_AXIS]) target[Y_AXIS] = min_pos[Y_AXIS];
    
    float negative_z_offset = 0;
    #ifdef ENABLE_AUTO_BED_LEVELING
      if (Z_PROBE_OFFSET_FROM_EXTRUDER < 0) negative_z_offset = negative_z_offset + Z_PROBE_OFFSET_FROM_EXTRUDER;
      if (home_offset[Z_AXIS] < 0) negative_z_offset = negative_z_offset + home_offset[Z_AXIS];
    #endif
    
    if (target[Z_AXIS] < min_pos[Z_AXIS]+negative_z_offset) target[Z_AXIS] = min_pos[Z_AXIS]+negative_z_offset;
  }

  if (max_software_endstops) {
    if (target[X_AXIS] > max_pos[X_AXIS]) target[X_AXIS] = max_pos[X_AXIS];
    if (target[Y_AXIS] > max_pos[Y_AXIS]) target[Y_AXIS] = max_pos[Y_AXIS];
    if (target[Z_AXIS] > max_pos[Z_AXIS]) target[Z_AXIS] = max_pos[Z_AXIS];
  }
}

#ifdef DELTA
void recalc_delta_settings(float radius, float diagonal_rod)
{
   delta_tower1_x= -SIN_60*radius; // front left tower
   delta_tower1_y= -COS_60*radius;     
   delta_tower2_x=  SIN_60*radius; // front right tower
   delta_tower2_y= -COS_60*radius;     
   delta_tower3_x= 0.0;                  // back middle tower
   delta_tower3_y= radius;
   delta_diagonal_rod_2= sq(diagonal_rod);
}

void calculate_delta(float cartesian[3])
{
  delta[X_AXIS] = sqrt(delta_diagonal_rod_2
                       - sq(delta_tower1_x-cartesian[X_AXIS])
                       - sq(delta_tower1_y-cartesian[Y_AXIS])
                       ) + cartesian[Z_AXIS];
  delta[Y_AXIS] = sqrt(delta_diagonal_rod_2
                       - sq(delta_tower2_x-cartesian[X_AXIS])
                       - sq(delta_tower2_y-cartesian[Y_AXIS])
                       ) + cartesian[Z_AXIS];
  delta[Z_AXIS] = sqrt(delta_diagonal_rod_2
                       - sq(delta_tower3_x-cartesian[X_AXIS])
                       - sq(delta_tower3_y-cartesian[Y_AXIS])
                       ) + cartesian[Z_AXIS];
  /*
  SERIAL_ECHOPGM("cartesian x="); SERIAL_ECHO(cartesian[X_AXIS]);
  SERIAL_ECHOPGM(" y="); SERIAL_ECHO(cartesian[Y_AXIS]);
  SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(cartesian[Z_AXIS]);

  SERIAL_ECHOPGM("delta x="); SERIAL_ECHO(delta[X_AXIS]);
  SERIAL_ECHOPGM(" y="); SERIAL_ECHO(delta[Y_AXIS]);
  SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(delta[Z_AXIS]);
  */
}

#ifdef ENABLE_AUTO_BED_LEVELING
// Adjust print surface height by linear interpolation over the bed_level array.
int delta_grid_spacing[2] = { 0, 0 };
void adjust_delta(float cartesian[3])
{
  if (delta_grid_spacing[0] == 0 || delta_grid_spacing[1] == 0)
    return; // G29 not done

  int half = (AUTO_BED_LEVELING_GRID_POINTS - 1) / 2;
  float grid_x = max(0.001-half, min(half-0.001, cartesian[X_AXIS] / delta_grid_spacing[0]));
  float grid_y = max(0.001-half, min(half-0.001, cartesian[Y_AXIS] / delta_grid_spacing[1]));
  int floor_x = floor(grid_x);
  int floor_y = floor(grid_y);
  float ratio_x = grid_x - floor_x;
  float ratio_y = grid_y - floor_y;
  float z1 = bed_level[floor_x+half][floor_y+half];
  float z2 = bed_level[floor_x+half][floor_y+half+1];
  float z3 = bed_level[floor_x+half+1][floor_y+half];
  float z4 = bed_level[floor_x+half+1][floor_y+half+1];
  float left = (1-ratio_y)*z1 + ratio_y*z2;
  float right = (1-ratio_y)*z3 + ratio_y*z4;
  float offset = (1-ratio_x)*left + ratio_x*right;

  delta[X_AXIS] += offset;
  delta[Y_AXIS] += offset;
  delta[Z_AXIS] += offset;

  /*
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
#endif //ENABLE_AUTO_BED_LEVELING

void prepare_move_raw()
{
  previous_millis_cmd = millis();
  calculate_delta(destination);
  plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS],
                   destination[E_AXIS], feedrate*feedmultiply/60/100.0,
                   active_extruder);
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}
#endif //DELTA

#if defined(MESH_BED_LEVELING)
#if !defined(MIN)
#define MIN(_v1, _v2) (((_v1) < (_v2)) ? (_v1) : (_v2))
#endif  // ! MIN
// This function is used to split lines on mesh borders so each segment is only part of one mesh area
void mesh_plan_buffer_line(float x, float y, float z, const float e, float feed_rate, const uint8_t &extruder, uint8_t x_splits=0xff, uint8_t y_splits=0xff)
{
  if (!mbl.active) {
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    for(int8_t i=0; i < NUM_AXIS; i++) {
      current_position[i] = destination[i];
    }
    return;
  }
  int pix = mbl.select_x_index(current_position[X_AXIS]);
  int piy = mbl.select_y_index(current_position[Y_AXIS]);
  int ix = mbl.select_x_index(x);
  int iy = mbl.select_y_index(y);
  pix = MIN(pix, MESH_NUM_X_POINTS-2);
  piy = MIN(piy, MESH_NUM_Y_POINTS-2);
  ix = MIN(ix, MESH_NUM_X_POINTS-2);
  iy = MIN(iy, MESH_NUM_Y_POINTS-2);
  if (pix == ix && piy == iy) {
    // Start and end on same mesh square
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    for(int8_t i=0; i < NUM_AXIS; i++) {
      current_position[i] = destination[i];
    }
    return;
  }
  float nx, ny, ne, normalized_dist;
  if (ix > pix && (x_splits) & BIT(ix)) {
    nx = mbl.get_x(ix);
    normalized_dist = (nx - current_position[X_AXIS])/(x - current_position[X_AXIS]);
    ny = current_position[Y_AXIS] + (y - current_position[Y_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    x_splits ^= BIT(ix);
  } else if (ix < pix && (x_splits) & BIT(pix)) {
    nx = mbl.get_x(pix);
    normalized_dist = (nx - current_position[X_AXIS])/(x - current_position[X_AXIS]);
    ny = current_position[Y_AXIS] + (y - current_position[Y_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    x_splits ^= BIT(pix);
  } else if (iy > piy && (y_splits) & BIT(iy)) {
    ny = mbl.get_y(iy);
    normalized_dist = (ny - current_position[Y_AXIS])/(y - current_position[Y_AXIS]);
    nx = current_position[X_AXIS] + (x - current_position[X_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    y_splits ^= BIT(iy);
  } else if (iy < piy && (y_splits) & BIT(piy)) {
    ny = mbl.get_y(piy);
    normalized_dist = (ny - current_position[Y_AXIS])/(y - current_position[Y_AXIS]);
    nx = current_position[X_AXIS] + (x - current_position[X_AXIS]) * normalized_dist;
    ne = current_position[E_AXIS] + (e - current_position[E_AXIS]) * normalized_dist;
    y_splits ^= BIT(piy);
  } else {
    // Already split on a border
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    for(int8_t i=0; i < NUM_AXIS; i++) {
      current_position[i] = destination[i];
    }
    return;
  }
  // Do the split and look for more borders
  destination[X_AXIS] = nx;
  destination[Y_AXIS] = ny;
  destination[E_AXIS] = ne;
  mesh_plan_buffer_line(nx, ny, z, ne, feed_rate, extruder, x_splits, y_splits);
  destination[X_AXIS] = x;
  destination[Y_AXIS] = y;
  destination[E_AXIS] = e;
  mesh_plan_buffer_line(x, y, z, e, feed_rate, extruder, x_splits, y_splits);
}
#endif  // MESH_BED_LEVELING

void prepare_move()
{
  clamp_to_software_endstops(destination);
  previous_millis_cmd = millis();
  
  #ifdef SCARA //for now same as delta-code

float difference[NUM_AXIS];
for (int8_t i=0; i < NUM_AXIS; i++) {
  difference[i] = destination[i] - current_position[i];
}

float cartesian_mm = sqrt(  sq(difference[X_AXIS]) +
              sq(difference[Y_AXIS]) +
              sq(difference[Z_AXIS]));
if (cartesian_mm < 0.000001) { cartesian_mm = abs(difference[E_AXIS]); }
if (cartesian_mm < 0.000001) { return; }
float seconds = 6000 * cartesian_mm / feedrate / feedmultiply;
int steps = max(1, int(scara_segments_per_second * seconds));
 //SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
 //SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
 //SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);
for (int s = 1; s <= steps; s++) {
  float fraction = float(s) / float(steps);
  for(int8_t i=0; i < NUM_AXIS; i++) {
    destination[i] = current_position[i] + difference[i] * fraction;
  }

  
  calculate_delta(destination);
         //SERIAL_ECHOPGM("destination[X_AXIS]="); SERIAL_ECHOLN(destination[X_AXIS]);
         //SERIAL_ECHOPGM("destination[Y_AXIS]="); SERIAL_ECHOLN(destination[Y_AXIS]);
         //SERIAL_ECHOPGM("destination[Z_AXIS]="); SERIAL_ECHOLN(destination[Z_AXIS]);
         //SERIAL_ECHOPGM("delta[X_AXIS]="); SERIAL_ECHOLN(delta[X_AXIS]);
         //SERIAL_ECHOPGM("delta[Y_AXIS]="); SERIAL_ECHOLN(delta[Y_AXIS]);
         //SERIAL_ECHOPGM("delta[Z_AXIS]="); SERIAL_ECHOLN(delta[Z_AXIS]);
         
  plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS],
  destination[E_AXIS], feedrate*feedmultiply/60/100.0,
  active_extruder);
}
#endif // SCARA
  
#ifdef DELTA
  float difference[NUM_AXIS];
  for (int8_t i=0; i < NUM_AXIS; i++) {
    difference[i] = destination[i] - current_position[i];
  }
  float cartesian_mm = sqrt(sq(difference[X_AXIS]) +
                            sq(difference[Y_AXIS]) +
                            sq(difference[Z_AXIS]));
  if (cartesian_mm < 0.000001) { cartesian_mm = abs(difference[E_AXIS]); }
  if (cartesian_mm < 0.000001) { return; }
  float seconds = 6000 * cartesian_mm / feedrate / feedmultiply;
  int steps = max(1, int(delta_segments_per_second * seconds));
  // SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
  // SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
  // SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);
  for (int s = 1; s <= steps; s++) {
    float fraction = float(s) / float(steps);
    for(int8_t i=0; i < NUM_AXIS; i++) {
      destination[i] = current_position[i] + difference[i] * fraction;
    }
    calculate_delta(destination);
    plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS],
                     destination[E_AXIS], feedrate*feedmultiply/60/100.0,
                     active_extruder);
  }
  
#endif // DELTA

#ifdef DUAL_X_CARRIAGE
  if (active_extruder_parked)
  {
    if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && active_extruder == 0)
    {
      // move duplicate extruder into correct duplication position.
      plan_set_position(inactive_extruder_x_pos, current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      plan_buffer_line(current_position[X_AXIS] + duplicate_extruder_x_offset, current_position[Y_AXIS], current_position[Z_AXIS],
          current_position[E_AXIS], max_feedrate[X_AXIS], 1);
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
      st_synchronize();
      extruder_duplication_enabled = true;
      active_extruder_parked = false;
    }
    else if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE) // handle unparking of head
    {
      if (current_position[E_AXIS] == destination[E_AXIS])
      {
        // this is a travel move - skit it but keep track of current position (so that it can later
        // be used as start of first non-travel move)
        if (delayed_move_time != 0xFFFFFFFFUL)
        {
          memcpy(current_position, destination, sizeof(current_position));
          if (destination[Z_AXIS] > raised_parked_position[Z_AXIS])
            raised_parked_position[Z_AXIS] = destination[Z_AXIS];
          delayed_move_time = millis();
          return;
        }
      }
      delayed_move_time = 0;
      // unpark extruder: 1) raise, 2) move into starting XY position, 3) lower
      plan_buffer_line(raised_parked_position[X_AXIS], raised_parked_position[Y_AXIS], raised_parked_position[Z_AXIS],    current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
      plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], raised_parked_position[Z_AXIS],
          current_position[E_AXIS], min(max_feedrate[X_AXIS],max_feedrate[Y_AXIS]), active_extruder);
      plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],
          current_position[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
      active_extruder_parked = false;
    }
  }
#endif //DUAL_X_CARRIAGE

#if ! (defined DELTA || defined SCARA)
  // Do not use feedmultiply for E or Z only moves
  if( (current_position[X_AXIS] == destination [X_AXIS]) && (current_position[Y_AXIS] == destination [Y_AXIS])) {
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
  } else {
#if defined(MESH_BED_LEVELING)
    mesh_plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60/100.0, active_extruder);
    return;
#else
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60/100.0, active_extruder);
#endif  // MESH_BED_LEVELING
  }
#endif // !(DELTA || SCARA)

  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void prepare_arc_move(char isclockwise) {
  float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc

  // Trace the arc
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedmultiply/60/100.0, r, isclockwise, active_extruder);

  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
  previous_millis_cmd = millis();
}

#if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1

#if defined(FAN_PIN)
  #if CONTROLLERFAN_PIN == FAN_PIN
    #error "You cannot set CONTROLLERFAN_PIN equal to FAN_PIN"
  #endif
#endif

unsigned long lastMotor = 0; // Last time a motor was turned on
unsigned long lastMotorCheck = 0; // Last time the state was checked

void controllerFan() {
  uint32_t ms = millis();
  if (ms >= lastMotorCheck + 2500) { // Not a time critical function, so we only check every 2500ms
    lastMotorCheck = ms;
    if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON || soft_pwm_bed > 0
      || E0_ENABLE_READ == E_ENABLE_ON // If any of the drivers are enabled...
      #if EXTRUDERS > 1
        || E1_ENABLE_READ == E_ENABLE_ON
        #if defined(X2_ENABLE_PIN) && X2_ENABLE_PIN > -1
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
      lastMotor = ms; //... set time to NOW so the fan will turn on
    }
    uint8_t speed = (lastMotor == 0 || ms >= lastMotor + (CONTROLLERFAN_SECS * 1000UL)) ? 0 : CONTROLLERFAN_SPEED;
    // allows digital or PWM fan output to be used (see M42 handling)
    digitalWrite(CONTROLLERFAN_PIN, speed);
    analogWrite(CONTROLLERFAN_PIN, speed);
  }
}
#endif

#ifdef SCARA
void calculate_SCARA_forward_Transform(float f_scara[3])
{
  // Perform forward kinematics, and place results in delta[3]
  // The maths and first version has been done by QHARLEY . Integrated into masterbranch 06/2014 and slightly restructured by Joachim Cerny in June 2014
  
  float x_sin, x_cos, y_sin, y_cos;
  
    //SERIAL_ECHOPGM("f_delta x="); SERIAL_ECHO(f_scara[X_AXIS]);
    //SERIAL_ECHOPGM(" y="); SERIAL_ECHO(f_scara[Y_AXIS]);
  
    x_sin = sin(f_scara[X_AXIS]/SCARA_RAD2DEG) * Linkage_1;
    x_cos = cos(f_scara[X_AXIS]/SCARA_RAD2DEG) * Linkage_1;
    y_sin = sin(f_scara[Y_AXIS]/SCARA_RAD2DEG) * Linkage_2;
    y_cos = cos(f_scara[Y_AXIS]/SCARA_RAD2DEG) * Linkage_2;
   
  //  SERIAL_ECHOPGM(" x_sin="); SERIAL_ECHO(x_sin);
  //  SERIAL_ECHOPGM(" x_cos="); SERIAL_ECHO(x_cos);
  //  SERIAL_ECHOPGM(" y_sin="); SERIAL_ECHO(y_sin);
  //  SERIAL_ECHOPGM(" y_cos="); SERIAL_ECHOLN(y_cos);
  
    delta[X_AXIS] = x_cos + y_cos + SCARA_offset_x;  //theta
    delta[Y_AXIS] = x_sin + y_sin + SCARA_offset_y;  //theta+phi
  
    //SERIAL_ECHOPGM(" delta[X_AXIS]="); SERIAL_ECHO(delta[X_AXIS]);
    //SERIAL_ECHOPGM(" delta[Y_AXIS]="); SERIAL_ECHOLN(delta[Y_AXIS]);
}  

void calculate_delta(float cartesian[3]){
  //reverse kinematics.
  // Perform reversed kinematics, and place results in delta[3]
  // The maths and first version has been done by QHARLEY . Integrated into masterbranch 06/2014 and slightly restructured by Joachim Cerny in June 2014
  
  float SCARA_pos[2];
  static float SCARA_C2, SCARA_S2, SCARA_K1, SCARA_K2, SCARA_theta, SCARA_psi; 
  
  SCARA_pos[X_AXIS] = cartesian[X_AXIS] * axis_scaling[X_AXIS] - SCARA_offset_x;  //Translate SCARA to standard X Y
  SCARA_pos[Y_AXIS] = cartesian[Y_AXIS] * axis_scaling[Y_AXIS] - SCARA_offset_y;  // With scaling factor.
  
  #if (Linkage_1 == Linkage_2)
    SCARA_C2 = ( ( sq(SCARA_pos[X_AXIS]) + sq(SCARA_pos[Y_AXIS]) ) / (2 * (float)L1_2) ) - 1;
  #else
    SCARA_C2 =   ( sq(SCARA_pos[X_AXIS]) + sq(SCARA_pos[Y_AXIS]) - (float)L1_2 - (float)L2_2 ) / 45000; 
  #endif
  
  SCARA_S2 = sqrt( 1 - sq(SCARA_C2) );
  
  SCARA_K1 = Linkage_1 + Linkage_2 * SCARA_C2;
  SCARA_K2 = Linkage_2 * SCARA_S2;
  
  SCARA_theta = ( atan2(SCARA_pos[X_AXIS],SCARA_pos[Y_AXIS])-atan2(SCARA_K1, SCARA_K2) ) * -1;
  SCARA_psi   =   atan2(SCARA_S2,SCARA_C2);
  
  delta[X_AXIS] = SCARA_theta * SCARA_RAD2DEG;  // Multiply by 180/Pi  -  theta is support arm angle
  delta[Y_AXIS] = (SCARA_theta + SCARA_psi) * SCARA_RAD2DEG;  //       -  equal to sub arm angle (inverted motor)
  delta[Z_AXIS] = cartesian[Z_AXIS];
  
  /*
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
  SERIAL_ECHOLN(" ");*/
}

#endif

#ifdef TEMP_STAT_LEDS
static bool blue_led = false;
static bool red_led = false;
static uint32_t stat_update = 0;

void handle_status_leds(void) {
  float max_temp = 0.0;
  if(millis() > stat_update) {
    stat_update += 500; // Update every 0.5s
    for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
       max_temp = max(max_temp, degHotend(cur_extruder));
       max_temp = max(max_temp, degTargetHotend(cur_extruder));
    }
    #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
      max_temp = max(max_temp, degTargetBed());
      max_temp = max(max_temp, degBed());
    #endif
    if((max_temp > 55.0) && (red_led == false)) {
      digitalWrite(STAT_LED_RED, 1);
      digitalWrite(STAT_LED_BLUE, 0);
      red_led = true;
      blue_led = false;
    }
    if((max_temp < 54.0) && (blue_led == false)) {
      digitalWrite(STAT_LED_RED, 0);
      digitalWrite(STAT_LED_BLUE, 1);
      red_led = false;
      blue_led = true;
    }
  }
}
#endif

void manage_inactivity(bool ignore_stepper_queue/*=false*/) //default argument set in Marlin.h
{
  
#if defined(KILL_PIN) && KILL_PIN > -1
  static int killCount = 0;   // make the inactivity button a bit less responsive
   const int KILL_DELAY = 750;
#endif

#if defined(FILRUNOUT_PIN) && FILRUNOUT_PIN > -1
    if(card.sdprinting) {
      if(!(READ(FILRUNOUT_PIN))^FIL_RUNOUT_INVERTING)
      filrunout();        }
#endif

#if defined(HOME_PIN) && HOME_PIN > -1
   static int homeDebounceCount = 0;   // poor man's debouncing count
   const int HOME_DEBOUNCE_DELAY = 750;
#endif
   
  
  if(buflen < (BUFSIZE-1))
    get_command();

  if( (millis() - previous_millis_cmd) >  max_inactive_time )
    if(max_inactive_time)
      kill();
  if(stepper_inactive_time)  {
    if( (millis() - previous_millis_cmd) >  stepper_inactive_time )
    {
      if(blocks_queued() == false && ignore_stepper_queue == false) {
        disable_x();
        disable_y();
        disable_z();
        disable_e0();
        disable_e1();
        disable_e2();
        disable_e3();
      }
    }
  }
  
  #ifdef CHDK //Check if pin should be set to LOW after M240 set it to HIGH
    if (chdkActive && (millis() - chdkHigh > CHDK_DELAY))
    {
      chdkActive = false;
      WRITE(CHDK, LOW);
    }
  #endif
  
  #if defined(KILL_PIN) && KILL_PIN > -1
    
    // Check if the kill button was pressed and wait just in case it was an accidental
    // key kill key press
    // -------------------------------------------------------------------------------
    if( 0 == READ(KILL_PIN) )
    {
       killCount++;
    }
    else if (killCount > 0)
    {
       killCount--;
    }
    // Exceeded threshold and we can confirm that it was not accidental
    // KILL the machine
    // ----------------------------------------------------------------
    if ( killCount >= KILL_DELAY)
    {
       kill();
    }
  #endif

#if defined(HOME_PIN) && HOME_PIN > -1
    // Check to see if we have to home, use poor man's debouncer
    // ---------------------------------------------------------
    if ( 0 == READ(HOME_PIN) )
    {
       if (homeDebounceCount == 0)
       {
          enquecommands_P((PSTR("G28")));
          homeDebounceCount++;
          LCD_ALERTMESSAGEPGM(MSG_AUTO_HOME);
       }
       else if (homeDebounceCount < HOME_DEBOUNCE_DELAY)
       {
          homeDebounceCount++;
       }
       else
       {
          homeDebounceCount = 0;
       }
    }
#endif
    
  #if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1
    controllerFan(); //Check if fan should be turned on to cool stepper drivers down
  #endif
  #ifdef EXTRUDER_RUNOUT_PREVENT
    if( (millis() - previous_millis_cmd) >  EXTRUDER_RUNOUT_SECONDS*1000 )
    if(degHotend(active_extruder)>EXTRUDER_RUNOUT_MINTEMP)
    {
     bool oldstatus=E0_ENABLE_READ;
     enable_e0();
     float oldepos=current_position[E_AXIS];
     float oldedes=destination[E_AXIS];
     plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS],
                      destination[E_AXIS]+EXTRUDER_RUNOUT_EXTRUDE*EXTRUDER_RUNOUT_ESTEPS/axis_steps_per_unit[E_AXIS],
                      EXTRUDER_RUNOUT_SPEED/60.*EXTRUDER_RUNOUT_ESTEPS/axis_steps_per_unit[E_AXIS], active_extruder);
     current_position[E_AXIS]=oldepos;
     destination[E_AXIS]=oldedes;
     plan_set_e_position(oldepos);
     previous_millis_cmd=millis();
     st_synchronize();
     E0_ENABLE_WRITE(oldstatus);
    }
  #endif
  #if defined(DUAL_X_CARRIAGE)
    // handle delayed move timeout
    if (delayed_move_time != 0 && (millis() - delayed_move_time) > 1000 && Stopped == false)
    {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      memcpy(destination,current_position,sizeof(destination));
      prepare_move();
    }
  #endif
  #ifdef TEMP_STAT_LEDS
      handle_status_leds();
  #endif
  check_axes_activity();
}

void kill()
{
  cli(); // Stop interrupts
  disable_heater();

  disable_x();
  disable_y();
  disable_z();
  disable_e0();
  disable_e1();
  disable_e2();
  disable_e3();

#if defined(PS_ON_PIN) && PS_ON_PIN > -1
  pinMode(PS_ON_PIN,INPUT);
#endif
  SERIAL_ERROR_START;
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);
  LCD_ALERTMESSAGEPGM(MSG_KILLED);
  
  // FMC small patch to update the LCD before ending
  sei();   // enable interrupts
  for ( int i=5; i--; lcd_update())
  {
     delay(200);  
  }
  cli();   // disable interrupts
  suicide();
  while(1) { /* Intentionally left empty */ } // Wait for reset
}

#ifdef FILAMENT_RUNOUT_SENSOR
   void filrunout()
   {
      if filrunoutEnqued == false {
         filrunoutEnqued = true;
         enquecommand("M600");
      }
   }
#endif

void Stop()
{
  disable_heater();
  if(Stopped == false) {
    Stopped = true;
    Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
  }
}

bool IsStopped() { return Stopped; };

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val)
{
  val &= 0x07;
  switch(digitalPinToTimer(pin))
  {

    #if defined(TCCR0A)
    case TIMER0A:
    case TIMER0B:
//         TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
//         TCCR0B |= val;
         break;
    #endif

    #if defined(TCCR1A)
    case TIMER1A:
    case TIMER1B:
//         TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
//         TCCR1B |= val;
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
#endif //FAST_PWM_FAN

bool setTargetedHotend(int code){
  tmp_extruder = active_extruder;
  if(code_seen('T')) {
    tmp_extruder = code_value();
    if(tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      switch(code){
        case 104:
          SERIAL_ECHO(MSG_M104_INVALID_EXTRUDER);
          break;
        case 105:
          SERIAL_ECHO(MSG_M105_INVALID_EXTRUDER);
          break;
        case 109:
          SERIAL_ECHO(MSG_M109_INVALID_EXTRUDER);
          break;
        case 218:
          SERIAL_ECHO(MSG_M218_INVALID_EXTRUDER);
          break;
        case 221:
          SERIAL_ECHO(MSG_M221_INVALID_EXTRUDER);
          break;
      }
      SERIAL_ECHOLN(tmp_extruder);
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
  for (int i=0; i<EXTRUDERS; i++)
    volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
}
