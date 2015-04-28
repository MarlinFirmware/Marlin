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

#include "gcode.h"

#ifdef ENABLE_AUTO_BED_LEVELING
  #include "vector_3.h"
  #ifdef AUTO_BED_LEVELING_GRID
    #include "qr_solve.h"
  #endif
#endif // ENABLE_AUTO_BED_LEVELING

#define SERVO_LEVELING (defined(ENABLE_AUTO_BED_LEVELING) && PROBE_SERVO_DEACTIVATION_DELAY > 0)

#ifdef MESH_BED_LEVELING
  #include "mesh_bed_leveling.h"
#endif

#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"
#include "cardreader.h"
#include "watchdog.h"
#include "configuration_store.h"
#include "language.h"
#include "pins_arduino.h"
#include "math.h"

#ifdef BLINKM
  #include "blinkm.h"
  #include "Wire.h"
#endif

#if NUM_SERVOS > 0
  #include "servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

/**
 * Look here for descriptions of G-codes:
 *  - http://linuxcnc.org/handbook/gcode/g-code.html
 *  - http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes
 *
 * Help us document these G-codes online:
 *  - http://reprap.org/wiki/G-code
 *  - https://github.com/MarlinFirmware/Marlin/wiki/Marlin-G-Code
 */

/**
 * Implemented Codes
 * -------------------
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
 * G29 - Detailed Z-Probe, probes the bed at 3 or more points.  Will fail if you haven't homed yet.
 * G30 - Single Z Probe, probes bed at current XY location.
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
 * M111 - Set debug flags with S<mask>. See flag bits defined in Marlin.h.
 * M112 - Emergency stop
 * M114 - Output current position to serial port
 * M115 - Capabilities string
 * M117 - display message
 * M119 - Output Endstop status to serial port
 * M120 - Enable endstop detection
 * M121 - Disable endstop detection
 * M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
 * M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
 * M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
 * M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
 * M140 - Set bed target temp
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
 * M208 - Set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/sec]
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
 * M401 - Lower z-probe if present
 * M402 - Raise z-probe if present
 * M404 - N<dia in mm> Enter the nominal filament width (3mm, 1.75mm ) or will display nominal filament width without parameters
 * M405 - Turn on Filament Sensor extrusion control.  Optional D<delay in cm> to set delay in centimeters between sensor and extruder
 * M406 - Turn off Filament Sensor extrusion control
 * M407 - Display measured filament diameter
 * M410 - Quickstop. Abort all the planned moves
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
 * M928 - Start SD logging (M928 filename.g) - ended by M29
 * M999 - Restart after being stopped by error
 */

#ifdef SDSUPPORT
  CardReader card;
#endif

bool Running = true;

uint8_t marlin_debug_flags = DEBUG_INFO|DEBUG_ERRORS;

float feedrate = 1500.0, saved_feedrate;
float current_position[NUM_AXIS] = { 0.0 };
float destination[NUM_AXIS] = { 0.0 };
bool axis_known_position[3] = { false };

float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedrate_multiplier = 100; //100->1 200->2
int saved_feedrate_multiplier;
int extruder_multiply[EXTRUDERS] = ARRAY_BY_EXTRUDERS(100, 100, 100, 100);
bool volumetric_enabled = false;
float filament_size[EXTRUDERS] = ARRAY_BY_EXTRUDERS(DEFAULT_NOMINAL_FILAMENT_DIA, DEFAULT_NOMINAL_FILAMENT_DIA, DEFAULT_NOMINAL_FILAMENT_DIA, DEFAULT_NOMINAL_FILAMENT_DIA);
float volumetric_multiplier[EXTRUDERS] = ARRAY_BY_EXTRUDERS(1.0, 1.0, 1.0, 1.0);
float home_offset[3] = { 0 };
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };

uint8_t active_extruder = 0;
int fanSpeed = 0;

const char errormagic[] PROGMEM = "Error:";
const char echomagic[] PROGMEM = "echo:";

float offset[3] = { 0 };
// Inactivity shutdown
millis_t previous_cmd_ms = 0;
millis_t max_inactive_time = 0;
millis_t stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME * 1000L;
millis_t print_job_start_ms = 0; ///< Print job start time
millis_t print_job_stop_ms = 0;  ///< Print job stop time

#ifdef ENABLE_AUTO_BED_LEVELING
  int xy_travel_speed = XY_TRAVEL_SPEED;
  float zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif

#if defined(Z_DUAL_ENDSTOPS) && !defined(DELTA)
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
    #ifdef DUAL_X_CARRIAGE
      , { 0 } // supports offsets in XYZ plane
    #endif
  };
#endif

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

#if defined(ULTIPANEL) && HAS_POWER_SWITCH
  bool powersupply = 
    #ifdef PS_DEFAULT_OFF
      false
    #else
      true
    #endif
  ;
#endif

#ifdef DELTA
  float delta[3] = { 0 };
  #define SIN_60 0.8660254037844386
  #define COS_60 0.5
  float endstop_adj[3] = { 0 };
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
    int delta_grid_spacing[2] = { 0, 0 };
    float bed_level[AUTO_BED_LEVELING_GRID_POINTS][AUTO_BED_LEVELING_GRID_POINTS];
  #endif
#endif

#ifdef SCARA
  float delta[3] = { 0 };
  float axis_scaling[3] = { 1, 1, 1 };    // Build size scaling, default to 1
#endif

#ifdef FILAMENT_SENSOR
  //Variables for Filament Sensor input
  float filament_width_nominal = DEFAULT_NOMINAL_FILAMENT_DIA;  //Set nominal filament width, can be changed with M404
  bool filament_sensor = false;  //M405 turns on filament_sensor control, M406 turns it off
  float filament_width_meas = DEFAULT_MEASURED_FILAMENT_DIA; //Stores the measured filament diameter
  signed char measurement_delay[MAX_MEASUREMENT_DELAY+1];  //ring buffer to delay measurement  store extruder factor after subtracting 100
  int delay_index1 = 0;  //index into ring buffer
  int delay_index2 = -1;  //index into ring buffer - set to -1 on startup to indicate ring buffer needs to be initialized
  float delay_dist = 0; //delay distance counter
  int meas_delay_cm = MEASUREMENT_DELAY_CM;  //distance delay setting
#endif

#ifdef FILAMENT_RUNOUT_SENSOR
   bool filrunoutEnqueued = false;
#endif

#if NUM_SERVOS > 0
  Servo servo[NUM_SERVOS];
#endif

#ifdef CHDK
  unsigned long chdkHigh = 0;
  boolean chdkActive = false;
#endif

//===========================================================================
//================================ Functions ================================
//===========================================================================

bool setTargetedHotend(int code);

void serial_echopair_P(const char *s_P, float v)         { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, double v)        { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

#ifdef PREVENT_DANGEROUS_EXTRUDE
  float extrude_min_temp = EXTRUDE_MINTEMP;
#endif

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

void setup_killpin() {
  #if HAS_KILL
    SET_INPUT(KILL_PIN);
    WRITE(KILL_PIN, HIGH);
  #endif
}

void setup_filrunoutpin() {
  #if HAS_FILRUNOUT
    pinMode(FILRUNOUT_PIN, INPUT);
    #ifdef ENDSTOPPULLUP_FIL_RUNOUT
      WRITE(FILLRUNOUT_PIN, HIGH);
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


void setup_photopin() {
  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif
}

void setup_powerhold() {
  #if HAS_SUICIDE
    OUT_WRITE(SUICIDE_PIN, HIGH);
  #endif
  #if HAS_POWER_SWITCH
    #ifdef PS_DEFAULT_OFF
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
  #endif
  #if NUM_SERVOS >= 2 && HAS_SERVO_1
    servo[1].attach(SERVO1_PIN);
  #endif
  #if NUM_SERVOS >= 3 && HAS_SERVO_2
    servo[2].attach(SERVO2_PIN);
  #endif
  #if NUM_SERVOS >= 4 && HAS_SERVO_3
    servo[3].attach(SERVO3_PIN);
  #endif

  // Set position of Servo Endstops that are defined
  #ifdef SERVO_ENDSTOPS
  for (int i = 0; i < 3; i++)
    if (servo_endstops[i] >= 0)
      servo[servo_endstops[i]].write(servo_endstop_angles[i * 2 + 1]);
  #endif

  #if SERVO_LEVELING
    delay(PROBE_SERVO_DEACTIVATION_DELAY);
    servo[servo_endstops[Z_AXIS]].detach();
  #endif
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
  setup_killpin();
  setup_filrunoutpin();
  setup_powerhold();
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
  SERIAL_ECHOLNPGM(" " STRING_VERSION);

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

  // loads data from EEPROM if available else uses defaults (and resets step acceleration rate)
  Config_RetrieveSettings();

  tp_init();    // Initialize temperature loop
  plan_init();  // Initialize planner;
  watchdog_init();
  st_init();    // Initialize stepper, this enables interrupts!
  setup_photopin();
  servo_init();

  lcd_init();
  _delay_ms(1000);  // wait 1sec to display the splash screen

  #if HAS_CONTROLLERFAN
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
  if (!gcode_queue_is_full()) get_command();

  #ifdef SDSUPPORT
    card.checkautostart(false);
  #endif

  if (!gcode_queue_is_empty()) {

    #ifdef SDSUPPORT

      if (card.saving) {
        char *command = gcode_queued_command();
        if (strstr_P(command, PSTR("M29"))) {
          // M29 closes the file
          card.closefile();
          SERIAL_PROTOCOLLNPGM(MSG_FILE_SAVED);
        }
        else {
          // Write the string from the read buffer to SD
          card.write_command(command);
          if (card.logging)
            process_commands(); // The card is saving because it's logging
          else
            SERIAL_PROTOCOLLNPGM(MSG_OK);
        }
      }
      else
        process_commands();

    #else

      process_commands();

    #endif // SDSUPPORT

    gcode_next_command();
  }
  // Check heater every n milliseconds
  manage_heater();
  manage_inactivity();
  checkHitEndstops();
  lcd_update();
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
  static millis_t delayed_move_time = 0; // used in mode 1
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
        float xoff = home_offset[X_AXIS];
        current_position[X_AXIS] = base_home_pos(X_AXIS) + xoff;
                 min_pos[X_AXIS] = base_min_pos(X_AXIS) + xoff;
                 max_pos[X_AXIS] = min(base_max_pos(X_AXIS) + xoff, max(extruder_offset[X_AXIS][1], X2_MAX_POS) - duplicate_extruder_x_offset);
        return;
      }
    }
  #endif

  #ifdef SCARA
   
    if (axis == X_AXIS || axis == Y_AXIS) {

      float homeposition[3];
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
    else
  #endif
  {
    current_position[axis] = base_home_pos(axis) + home_offset[axis];
    min_pos[axis] = base_min_pos(axis) + home_offset[axis];
    max_pos[axis] = base_max_pos(axis) + home_offset[axis];

    #if defined(ENABLE_AUTO_BED_LEVELING) && Z_HOME_DIR < 0
      if (axis == Z_AXIS) current_position[Z_AXIS] += zprobe_zoffset;
    #endif
  }
}

/**
 * Some planner shorthand inline functions
 */
inline void set_homing_bump_feedrate(AxisEnum axis) {
  const int homing_bump_divisor[] = HOMING_BUMP_DIVISOR;
  if (homing_bump_divisor[axis] >= 1)
    feedrate = homing_feedrate[axis] / homing_bump_divisor[axis];
  else {
    feedrate = homing_feedrate[axis] / 10;
    SERIAL_ECHOLN("Warning: The Homing Bump Feedrate Divisor cannot be less than 1");
  }
}
inline void line_to_current_position() {
  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate/60, active_extruder);
}
inline void line_to_z(float zPosition) {
  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate/60, active_extruder);
}
inline void line_to_destination(float mm_m) {
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], mm_m/60, active_extruder);
}
inline void line_to_destination() {
  line_to_destination(feedrate);
}
inline void sync_plan_position() {
  plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
#if defined(DELTA) || defined(SCARA)
  inline void sync_plan_position_delta() {
    calculate_delta(current_position);
    plan_set_position(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS]);
  }
#endif
inline void set_current_to_destination() { memcpy(current_position, destination, sizeof(current_position)); }
inline void set_destination_to_current() { memcpy(destination, current_position, sizeof(destination)); }

#ifdef ENABLE_AUTO_BED_LEVELING

  #ifdef DELTA
    /**
     * Calculate delta, start a line, and set current_position to destination
     */
    void prepare_move_raw() {
      refresh_cmd_timeout();
      calculate_delta(destination);
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], (feedrate/60)*(feedrate_multiplier/100.0), active_extruder);
      set_current_to_destination();
    }
  #endif

  #ifdef AUTO_BED_LEVELING_GRID

    #ifndef DELTA

      static void set_bed_level_equation_lsq(double *plane_equation_coefficients) {
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

      sync_plan_position();
    }

  #endif // !AUTO_BED_LEVELING_GRID

  static void run_z_probe() {

    #ifdef DELTA
    
      float start_z = current_position[Z_AXIS];
      long start_steps = st_get_position(Z_AXIS);
    
      // move down slowly until you find the bed
      feedrate = homing_feedrate[Z_AXIS] / 4;
      destination[Z_AXIS] = -10;
      prepare_move_raw(); // this will also set_current_to_destination
      st_synchronize();
      endstops_hit_on_purpose(); // clear endstop hit flags
      
      // we have to let the planner know where we are right now as it is not where we said to go.
      long stop_steps = st_get_position(Z_AXIS);
      float mm = start_z - float(start_steps - stop_steps) / axis_steps_per_unit[Z_AXIS];
      current_position[Z_AXIS] = mm;
      sync_plan_position_delta();
      
    #else // !DELTA

      plan_bed_level_matrix.set_to_identity();
      feedrate = homing_feedrate[Z_AXIS];

      // move down until you find the bed
      float zPosition = -10;
      line_to_z(zPosition);
      st_synchronize();

      // we have to let the planner know where we are right now as it is not where we said to go.
      zPosition = st_get_position_mm(Z_AXIS);
      plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS]);

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
      current_position[Z_AXIS] = st_get_position_mm(Z_AXIS);
      sync_plan_position();
      
    #endif // !DELTA
  }

  /**
   *  Plan a move to (X, Y, Z) and set the current_position
   *  The final current_position may not be the one that was requested
   */
  static void do_blocking_move_to(float x, float y, float z) {
    float oldFeedRate = feedrate;

    #ifdef DELTA

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

  static void setup_for_endstop_move() {
    saved_feedrate = feedrate;
    saved_feedrate_multiplier = feedrate_multiplier;
    feedrate_multiplier = 100;
    refresh_cmd_timeout();
    enable_endstops(true);
  }

  static void clean_up_after_endstop_move() {
    #ifdef ENDSTOPS_ONLY_FOR_HOMING
      enable_endstops(false);
    #endif
    feedrate = saved_feedrate;
    feedrate_multiplier = saved_feedrate_multiplier;
    refresh_cmd_timeout();
  }

  static void deploy_z_probe() {

    #ifdef SERVO_ENDSTOPS

      // Engage Z Servo endstop if enabled
      if (servo_endstops[Z_AXIS] >= 0) {
        #if SERVO_LEVELING
          servo[servo_endstops[Z_AXIS]].attach(0);
        #endif
        servo[servo_endstops[Z_AXIS]].write(servo_endstop_angles[Z_AXIS * 2]);
        #if SERVO_LEVELING
          delay(PROBE_SERVO_DEACTIVATION_DELAY);
          servo[servo_endstops[Z_AXIS]].detach();
        #endif
      }

    #elif defined(Z_PROBE_ALLEN_KEY)

      feedrate = homing_feedrate[X_AXIS];

      // Move to the start position to initiate deployment
      destination[X_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_X;
      destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_Y;
      destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_DEPLOY_Z;
      prepare_move_raw(); // this will also set_current_to_destination

      // Home X to touch the belt
      feedrate = homing_feedrate[X_AXIS]/10;
      destination[X_AXIS] = 0;
      prepare_move_raw(); // this will also set_current_to_destination
      
      // Home Y for safety
      feedrate = homing_feedrate[X_AXIS]/2;
      destination[Y_AXIS] = 0;
      prepare_move_raw(); // this will also set_current_to_destination
      
      st_synchronize();

    #ifdef Z_PROBE_ENDSTOP
      bool z_probe_endstop = (READ(Z_PROBE_PIN) != Z_PROBE_ENDSTOP_INVERTING);
      if (z_probe_endstop)
    #else
      bool z_min_endstop = (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
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

  }

  static void stow_z_probe() {

    #ifdef SERVO_ENDSTOPS

      // Retract Z Servo endstop if enabled
      if (servo_endstops[Z_AXIS] >= 0) {

        #if Z_RAISE_AFTER_PROBING > 0
          do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] + Z_RAISE_AFTER_PROBING); // this also updates current_position
          st_synchronize();
        #endif

        #if SERVO_LEVELING
          servo[servo_endstops[Z_AXIS]].attach(0);
        #endif

        servo[servo_endstops[Z_AXIS]].write(servo_endstop_angles[Z_AXIS * 2 + 1]);

        #if SERVO_LEVELING
          delay(PROBE_SERVO_DEACTIVATION_DELAY);
          servo[servo_endstops[Z_AXIS]].detach();
        #endif
      }

    #elif defined(Z_PROBE_ALLEN_KEY)

      // Move up for safety
      feedrate = homing_feedrate[X_AXIS];
      destination[Z_AXIS] = current_position[Z_AXIS] + Z_RAISE_AFTER_PROBING;
      prepare_move_raw(); // this will also set_current_to_destination

      // Move to the start position to initiate retraction
      destination[X_AXIS] = Z_PROBE_ALLEN_KEY_STOW_X;
      destination[Y_AXIS] = Z_PROBE_ALLEN_KEY_STOW_Y;
      destination[Z_AXIS] = Z_PROBE_ALLEN_KEY_STOW_Z;
      prepare_move_raw(); // this will also set_current_to_destination

      // Move the nozzle down to push the probe into retracted position
      feedrate = homing_feedrate[Z_AXIS]/10;
      destination[Z_AXIS] = current_position[Z_AXIS] - Z_PROBE_ALLEN_KEY_STOW_DEPTH;
      prepare_move_raw(); // this will also set_current_to_destination
      
      // Move up for safety
      feedrate = homing_feedrate[Z_AXIS]/2;
      destination[Z_AXIS] = current_position[Z_AXIS] + Z_PROBE_ALLEN_KEY_STOW_DEPTH * 2;
      prepare_move_raw(); // this will also set_current_to_destination
      
      // Home XY for safety
      feedrate = homing_feedrate[X_AXIS]/2;
      destination[X_AXIS] = 0;
      destination[Y_AXIS] = 0;
      prepare_move_raw(); // this will also set_current_to_destination
      
      st_synchronize();

    #ifdef Z_PROBE_ENDSTOP
      bool z_probe_endstop = (READ(Z_PROBE_PIN) != Z_PROBE_ENDSTOP_INVERTING);
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

    #endif

  }

  enum ProbeAction {
    ProbeStay          = 0,
    ProbeDeploy        = BIT(0),
    ProbeStow          = BIT(1),
    ProbeDeployAndStow = (ProbeDeploy | ProbeStow)
  };

  // Probe bed height at position (x,y), returns the measured z value
  static float probe_pt(float x, float y, float z_before, ProbeAction retract_action=ProbeDeployAndStow, int verbose_level=1) {
    // move to right place
    do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], z_before); // this also updates current_position
    do_blocking_move_to(x - X_PROBE_OFFSET_FROM_EXTRUDER, y - Y_PROBE_OFFSET_FROM_EXTRUDER, current_position[Z_AXIS]); // this also updates current_position

    #if !defined(Z_PROBE_SLED) && !defined(Z_PROBE_ALLEN_KEY)
      if (retract_action & ProbeDeploy) deploy_z_probe();
    #endif

    run_z_probe();
    float measured_z = current_position[Z_AXIS];

    #if Z_RAISE_BETWEEN_PROBINGS > 0
      if (retract_action == ProbeStay) {
        do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS); // this also updates current_position
        st_synchronize();
      }
    #endif

    #if !defined(Z_PROBE_SLED) && !defined(Z_PROBE_ALLEN_KEY)
      if (retract_action & ProbeStow) stow_z_probe();
    #endif

    if (verbose_level > 2) {
      SERIAL_PROTOCOLPGM("Bed");
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

    /**
     * All DELTA leveling in the Marlin uses NONLINEAR_BED_LEVELING
     */

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
          SERIAL_PROTOCOLCHAR(' ');
        }
        SERIAL_EOL;
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

/**
 * Home an individual axis
 */

void homeaxis(AxisEnum axis) {
  #define HOMEAXIS_DO(LETTER) \
    ((LETTER##_MIN_PIN > -1 && LETTER##_HOME_DIR==-1) || (LETTER##_MAX_PIN > -1 && LETTER##_HOME_DIR==1))

  if (axis == X_AXIS ? HOMEAXIS_DO(X) : axis == Y_AXIS ? HOMEAXIS_DO(Y) : axis == Z_AXIS ? HOMEAXIS_DO(Z) : 0) {

    int axis_home_dir =
      #ifdef DUAL_X_CARRIAGE
        (axis == X_AXIS) ? x_home_dir(active_extruder) :
      #endif
      home_dir(axis);

    // Set the axis position as setup for the move
    current_position[axis] = 0;
    sync_plan_position();

    // Engage Servo endstop if enabled
    #if defined(SERVO_ENDSTOPS) && !defined(Z_PROBE_SLED)

      #if SERVO_LEVELING
        if (axis == Z_AXIS) deploy_z_probe(); else
      #endif
        {
          if (servo_endstops[axis] > -1)
            servo[servo_endstops[axis]].write(servo_endstop_angles[axis * 2]);
        }

    #endif // SERVO_ENDSTOPS && !Z_PROBE_SLED

    #ifdef Z_DUAL_ENDSTOPS
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

    // Move away from the endstop by the axis HOME_BUMP_MM
    destination[axis] = -home_bump_mm(axis) * axis_home_dir;
    line_to_destination();
    st_synchronize();

    // Slow down the feedrate for the next move
    set_homing_bump_feedrate(axis);

    // Move slowly towards the endstop until triggered
    destination[axis] = 2 * home_bump_mm(axis) * axis_home_dir;
    line_to_destination();
    st_synchronize();

    #ifdef Z_DUAL_ENDSTOPS
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

    #ifdef DELTA
      // retrace by the amount specified in endstop_adj
      if (endstop_adj[axis] * axis_home_dir < 0) {
        sync_plan_position();
        destination[axis] = endstop_adj[axis];
        line_to_destination();
        st_synchronize();
      }
    #endif

    // Set the axis position to its home position (plus home offsets)
    axis_is_at_home(axis);

    destination[axis] = current_position[axis];
    feedrate = 0.0;
    endstops_hit_on_purpose(); // clear endstop hit flags
    axis_known_position[axis] = true;

    // Retract Servo endstop if enabled
    #ifdef SERVO_ENDSTOPS
      if (servo_endstops[axis] > -1)
        servo[servo_endstops[axis]].write(servo_endstop_angles[axis * 2 + 1]);
    #endif

    #if SERVO_LEVELING && !defined(Z_PROBE_SLED)
      if (axis == Z_AXIS) stow_z_probe();
    #endif

  }
}

#ifdef FWRETRACT

  void retract(bool retracting, bool swapretract = false) {

    if (retracting == retracted[active_extruder]) return;

    float oldFeedrate = feedrate;

    set_destination_to_current();

    if (retracting) {

      feedrate = retract_feedrate * 60;
      current_position[E_AXIS] += (swapretract ? retract_length_swap : retract_length) / volumetric_multiplier[active_extruder];
      plan_set_e_position(current_position[E_AXIS]);
      prepare_move();

      if (retract_zlift > 0.01) {
        current_position[Z_AXIS] -= retract_zlift;
        #ifdef DELTA
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
        #ifdef DELTA
          sync_plan_position_delta();
        #else
          sync_plan_position();
        #endif
        //prepare_move();
      }

      feedrate = retract_recover_feedrate * 60;
      float move_e = swapretract ? retract_length_swap + retract_recover_length_swap : retract_length + retract_recover_length;
      current_position[E_AXIS] -= move_e / volumetric_multiplier[active_extruder];
      plan_set_e_position(current_position[E_AXIS]);
      prepare_move();
    }

    feedrate = oldFeedrate;
    retracted[active_extruder] = retracting;

  } // retract()

#endif // FWRETRACT

#ifdef Z_PROBE_SLED

  #ifndef SLED_DOCKING_OFFSET
    #define SLED_DOCKING_OFFSET 0
  #endif

  /**
   * Method to dock/undock a sled designed by Charles Bell.
   *
   * dock[in]     If true, move to MAX_X and engage the electromagnet
   * offset[in]   The additional distance to move to adjust docking location
   */
  static void dock_sled(bool dock, int offset=0) {
    if (!axis_known_position[X_AXIS] || !axis_known_position[Y_AXIS]) {
      LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
      return;
    }

    if (dock) {
      do_blocking_move_to(X_MAX_POS + SLED_DOCKING_OFFSET + offset, current_position[Y_AXIS], current_position[Z_AXIS]); // this also updates current_position
      digitalWrite(SERVO0_PIN, LOW); // turn off magnet
    } else {
      float z_loc = current_position[Z_AXIS];
      if (z_loc < Z_RAISE_BEFORE_PROBING + 5) z_loc = Z_RAISE_BEFORE_PROBING;
      do_blocking_move_to(X_MAX_POS + SLED_DOCKING_OFFSET + offset, Y_PROBE_OFFSET_FROM_EXTRUDER, z_loc); // this also updates current_position
      digitalWrite(SERVO0_PIN, HIGH); // turn on magnet
    }
  }

#endif // Z_PROBE_SLED

void clamp_to_software_endstops(float target[3]) {
  if (min_software_endstops) {
    NOLESS(target[X_AXIS], min_pos[X_AXIS]);
    NOLESS(target[Y_AXIS], min_pos[Y_AXIS]);
    
    float negative_z_offset = 0;
    #ifdef ENABLE_AUTO_BED_LEVELING
      if (Z_PROBE_OFFSET_FROM_EXTRUDER < 0) negative_z_offset += Z_PROBE_OFFSET_FROM_EXTRUDER;
      if (home_offset[Z_AXIS] < 0) negative_z_offset += home_offset[Z_AXIS];
    #endif
    NOLESS(target[Z_AXIS], min_pos[Z_AXIS] + negative_z_offset);
  }

  if (max_software_endstops) {
    NOMORE(target[X_AXIS], max_pos[X_AXIS]);
    NOMORE(target[Y_AXIS], max_pos[Y_AXIS]);
    NOMORE(target[Z_AXIS], max_pos[Z_AXIS]);
  }
}

#ifdef DELTA

  void recalc_delta_settings(float radius, float diagonal_rod) {
    delta_tower1_x = -SIN_60 * radius;  // front left tower
    delta_tower1_y = -COS_60 * radius;
    delta_tower2_x =  SIN_60 * radius;  // front right tower
    delta_tower2_y = -COS_60 * radius;
    delta_tower3_x = 0.0;               // back middle tower
    delta_tower3_y = radius;
    delta_diagonal_rod_2 = sq(diagonal_rod);
  }

  void calculate_delta(float cartesian[3]) {
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
  #endif // ENABLE_AUTO_BED_LEVELING

#endif // DELTA

#ifdef MESH_BED_LEVELING

  #if !defined(MIN)
    #define MIN(_v1, _v2) (((_v1) < (_v2)) ? (_v1) : (_v2))
  #endif  // ! MIN

// This function is used to split lines on mesh borders so each segment is only part of one mesh area
void mesh_plan_buffer_line(float x, float y, float z, const float e, float feed_rate, const uint8_t &extruder, uint8_t x_splits=0xff, uint8_t y_splits=0xff)
{
  if (!mbl.active) {
    plan_buffer_line(x, y, z, e, feed_rate, extruder);
    set_current_to_destination();
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
    set_current_to_destination();
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
    set_current_to_destination();
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

#ifdef PREVENT_DANGEROUS_EXTRUDE

  inline float prevent_dangerous_extrude(float &curr_e, float &dest_e) {
    float de = dest_e - curr_e;
    if (de) {
      if (degHotend(active_extruder) < extrude_min_temp) {
        curr_e = dest_e; // Behave as if the move really took place, but ignore E part
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM(MSG_ERR_COLD_EXTRUDE_STOP);
        return 0;
      }
      #ifdef PREVENT_LENGTHY_EXTRUDE
        if (labs(de) > EXTRUDE_MAXLENGTH) {
          curr_e = dest_e; // Behave as if the move really took place, but ignore E part
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_ERR_LONG_EXTRUDE_STOP);
          return 0;
        }
      #endif
    }
    return de;
  }

#endif // PREVENT_DANGEROUS_EXTRUDE

void prepare_move() {
  clamp_to_software_endstops(destination);
  refresh_cmd_timeout();

  #ifdef PREVENT_DANGEROUS_EXTRUDE
    (void)prevent_dangerous_extrude(current_position[E_AXIS], destination[E_AXIS]);
  #endif

  #ifdef SCARA //for now same as delta-code

    float difference[NUM_AXIS];
    for (int8_t i = 0; i < NUM_AXIS; i++) difference[i] = destination[i] - current_position[i];

    float cartesian_mm = sqrt(sq(difference[X_AXIS]) + sq(difference[Y_AXIS]) + sq(difference[Z_AXIS]));
    if (cartesian_mm < 0.000001) { cartesian_mm = abs(difference[E_AXIS]); }
    if (cartesian_mm < 0.000001) { return; }
    float seconds = 6000 * cartesian_mm / feedrate / feedrate_multiplier;
    int steps = max(1, int(scara_segments_per_second * seconds));

    //SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
    //SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
    //SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);

    for (int s = 1; s <= steps; s++) {
      float fraction = float(s) / float(steps);
      for (int8_t i = 0; i < NUM_AXIS; i++) destination[i] = current_position[i] + difference[i] * fraction;
  
      calculate_delta(destination);
      //SERIAL_ECHOPGM("destination[X_AXIS]="); SERIAL_ECHOLN(destination[X_AXIS]);
      //SERIAL_ECHOPGM("destination[Y_AXIS]="); SERIAL_ECHOLN(destination[Y_AXIS]);
      //SERIAL_ECHOPGM("destination[Z_AXIS]="); SERIAL_ECHOLN(destination[Z_AXIS]);
      //SERIAL_ECHOPGM("delta[X_AXIS]="); SERIAL_ECHOLN(delta[X_AXIS]);
      //SERIAL_ECHOPGM("delta[Y_AXIS]="); SERIAL_ECHOLN(delta[Y_AXIS]);
      //SERIAL_ECHOPGM("delta[Z_AXIS]="); SERIAL_ECHOLN(delta[Z_AXIS]);

      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], feedrate/60*feedrate_multiplier/100.0, active_extruder);
    }

  #endif // SCARA
  
  #ifdef DELTA

    float difference[NUM_AXIS];
    for (int8_t i=0; i < NUM_AXIS; i++) difference[i] = destination[i] - current_position[i];

    float cartesian_mm = sqrt(sq(difference[X_AXIS]) + sq(difference[Y_AXIS]) + sq(difference[Z_AXIS]));
    if (cartesian_mm < 0.000001) cartesian_mm = abs(difference[E_AXIS]);
    if (cartesian_mm < 0.000001) return;
    float seconds = 6000 * cartesian_mm / feedrate / feedrate_multiplier;
    int steps = max(1, int(delta_segments_per_second * seconds));

    // SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
    // SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
    // SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);

    for (int s = 1; s <= steps; s++) {
      float fraction = float(s) / float(steps);
      for (int8_t i = 0; i < NUM_AXIS; i++) destination[i] = current_position[i] + difference[i] * fraction;
      calculate_delta(destination);
      #ifdef ENABLE_AUTO_BED_LEVELING
        adjust_delta(destination);
      #endif
      plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], destination[E_AXIS], feedrate/60*feedrate_multiplier/100.0, active_extruder);
    }

  #endif // DELTA

  #ifdef DUAL_X_CARRIAGE
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
            return;
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
  #endif // DUAL_X_CARRIAGE

  #if !defined(DELTA) && !defined(SCARA)
    // Do not use feedrate_multiplier for E or Z only moves
    if (current_position[X_AXIS] == destination[X_AXIS] && current_position[Y_AXIS] == destination[Y_AXIS]) {
      line_to_destination();
    }
    else {
      #ifdef MESH_BED_LEVELING
        mesh_plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], (feedrate/60)*(feedrate_multiplier/100.0), active_extruder);
        return;
      #else
        line_to_destination(feedrate * feedrate_multiplier / 100.0);
      #endif  // MESH_BED_LEVELING
    }
  #endif // !(DELTA || SCARA)

  set_current_to_destination();
}

void prepare_arc_move(char isclockwise) {
  float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc

  // Trace the arc
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedrate_multiplier/60/100.0, r, isclockwise, active_extruder);

  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  set_current_to_destination();
  refresh_cmd_timeout();
}

#if HAS_CONTROLLERFAN

millis_t lastMotor = 0; // Last time a motor was turned on
millis_t lastMotorCheck = 0; // Last time the state was checked

void controllerFan() {
  millis_t ms = millis();
  if (ms >= lastMotorCheck + 2500) { // Not a time critical function, so we only check every 2500ms
    lastMotorCheck = ms;
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
    if (card.sdprinting && !(READ(FILRUNOUT_PIN) ^ FIL_RUNOUT_INVERTING))
      filrunout();
  #endif

  if (commands_in_queue < BUFSIZE - 1) get_command();

  millis_t ms = millis();

  if (max_inactive_time && ms > previous_cmd_ms + max_inactive_time) kill();

  if (stepper_inactive_time && ms > previous_cmd_ms + stepper_inactive_time
      && !ignore_stepper_queue && !blocks_queued())
    disable_all_steppers();

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
    if (killCount >= KILL_DELAY) kill();
  #endif

  #if HAS_HOME
    // Check to see if we have to home, use poor man's debouncer
    // ---------------------------------------------------------
    static int homeDebounceCount = 0;   // poor man's debouncing count
    const int HOME_DEBOUNCE_DELAY = 750;
    if (!READ(HOME_PIN)) {
      if (!homeDebounceCount) {
        enqueuecommands_P(PSTR("G28"));
        LCD_ALERTMESSAGEPGM(MSG_AUTO_HOME);
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

  #ifdef EXTRUDER_RUNOUT_PREVENT
    if (ms > previous_cmd_ms + EXTRUDER_RUNOUT_SECONDS * 1000)
    if (degHotend(active_extruder) > EXTRUDER_RUNOUT_MINTEMP) {
      bool oldstatus;
      switch(active_extruder) {
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
                      destination[E_AXIS] + EXTRUDER_RUNOUT_EXTRUDE * EXTRUDER_RUNOUT_ESTEPS / axis_steps_per_unit[E_AXIS],
                      EXTRUDER_RUNOUT_SPEED / 60. * EXTRUDER_RUNOUT_ESTEPS / axis_steps_per_unit[E_AXIS], active_extruder);
      current_position[E_AXIS] = oldepos;
      destination[E_AXIS] = oldedes;
      plan_set_e_position(oldepos);
      previous_cmd_ms = ms; // refresh_cmd_timeout()
      st_synchronize();
      switch(active_extruder) {
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

  #ifdef DUAL_X_CARRIAGE
    // handle delayed move timeout
    if (delayed_move_time && ms > delayed_move_time + 1000 && IsRunning()) {
      // travel moves have been received so enact them
      delayed_move_time = 0xFFFFFFFFUL; // force moves to be done
      set_destination_to_current();
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
  disable_all_heaters();

  disable_all_steppers();

  #if HAS_POWER_SWITCH
    pinMode(PS_ON_PIN, INPUT);
  #endif

  SERIAL_ERROR_START;
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);
  LCD_ALERTMESSAGEPGM(MSG_KILLED);
  
  // FMC small patch to update the LCD before ending
  sei();   // enable interrupts
  for (int i = 5; i--; lcd_update()) delay(200); // Wait a short time
  cli();   // disable interrupts
  suicide();
  while(1) { /* Intentionally left empty */ } // Wait for reset
}

#ifdef FILAMENT_RUNOUT_SENSOR

  void filrunout() {
    if (!filrunoutEnqueued) {
      filrunoutEnqueued = true;
      enqueuecommand("M600");
    }
  }

#endif

void Stop() {
  disable_all_heaters();
  if (IsRunning()) {
    Running = false;
    SaveStoppedGCodeIndex();
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
    LCD_MESSAGEPGM(MSG_STOPPED);
  }
}

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

float calculate_volumetric_multiplier(float diameter) {
  if (!volumetric_enabled || diameter == 0) return 1.0;
  float d2 = diameter * 0.5;
  return 1.0 / (M_PI * d2 * d2);
}

void calculate_volumetric_multipliers() {
  for (int i=0; i<EXTRUDERS; i++)
    volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
}
