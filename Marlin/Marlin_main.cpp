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
 *  - https://github.com/grbl/grbl
 */

/**
 * -----------------
 * G-Codes in Marlin
 * -----------------
 *
 * Helpful G-code references:
 *  - http://linuxcnc.org/handbook/gcode/g-code.html
 *  - http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes
 *
 * Help to document Marlin's G-codes online:
 *  - http://reprap.org/wiki/G-code
 *  - https://github.com/MarlinFirmware/MarlinDocumentation
 *
 * -----------------
 *
 * "G" Codes
 *
 * G0   -> G1
 * G1   - Coordinated Movement X Y Z E
 * G2   - CW ARC
 * G3   - CCW ARC
 * G4   - Dwell S<seconds> or P<milliseconds>
 * G5   - Cubic B-spline with XYZE destination and IJPQ offsets
 * G10  - Retract filament according to settings of M207 (Requires FWRETRACT)
 * G11  - Retract recover filament according to settings of M208 (Requires FWRETRACT)
 * G12  - Clean tool (Requires NOZZLE_CLEAN_FEATURE)
 * G17  - Select Plane XY (Requires CNC_WORKSPACE_PLANES)
 * G18  - Select Plane ZX (Requires CNC_WORKSPACE_PLANES)
 * G19  - Select Plane YZ (Requires CNC_WORKSPACE_PLANES)
 * G20  - Set input units to inches (Requires INCH_MODE_SUPPORT)
 * G21  - Set input units to millimeters (Requires INCH_MODE_SUPPORT)
 * G26  - Mesh Validation Pattern (Requires G26_MESH_VALIDATION)
 * G27  - Park Nozzle (Requires NOZZLE_PARK_FEATURE)
 * G28  - Home one or more axes
 * G29  - Start or continue the bed leveling probe procedure (Requires bed leveling)
 * G30  - Single Z probe, probes bed at X Y location (defaults to current XY location)
 * G31  - Dock sled (Z_PROBE_SLED only)
 * G32  - Undock sled (Z_PROBE_SLED only)
 * G33  - Delta Auto-Calibration (Requires DELTA_AUTO_CALIBRATION)
 * G38  - Probe in any direction using the Z_MIN_PROBE (Requires G38_PROBE_TARGET)
 * G42  - Coordinated move to a mesh point (Requires MESH_BED_LEVELING, AUTO_BED_LEVELING_BLINEAR, or AUTO_BED_LEVELING_UBL)
 * G90  - Use Absolute Coordinates
 * G91  - Use Relative Coordinates
 * G92  - Set current position to coordinates given
 *
 * "M" Codes
 *
 * M0   - Unconditional stop - Wait for user to press a button on the LCD (Only if ULTRA_LCD is enabled)
 * M1   -> M0
 * M3   - Turn laser/spindle on, set spindle/laser speed/power, set rotation to clockwise
 * M4   - Turn laser/spindle on, set spindle/laser speed/power, set rotation to counter-clockwise
 * M5   - Turn laser/spindle off
 * M17  - Enable/Power all stepper motors
 * M18  - Disable all stepper motors; same as M84
 * M20  - List SD card. (Requires SDSUPPORT)
 * M21  - Init SD card. (Requires SDSUPPORT)
 * M22  - Release SD card. (Requires SDSUPPORT)
 * M23  - Select SD file: "M23 /path/file.gco". (Requires SDSUPPORT)
 * M24  - Start/resume SD print. (Requires SDSUPPORT)
 * M25  - Pause SD print. (Requires SDSUPPORT)
 * M26  - Set SD position in bytes: "M26 S12345". (Requires SDSUPPORT)
 * M27  - Report SD print status. (Requires SDSUPPORT)
 * M28  - Start SD write: "M28 /path/file.gco". (Requires SDSUPPORT)
 * M29  - Stop SD write. (Requires SDSUPPORT)
 * M30  - Delete file from SD: "M30 /path/file.gco"
 * M31  - Report time since last M109 or SD card start to serial.
 * M32  - Select file and start SD print: "M32 [S<bytepos>] !/path/file.gco#". (Requires SDSUPPORT)
 *        Use P to run other files as sub-programs: "M32 P !filename#"
 *        The '#' is necessary when calling from within sd files, as it stops buffer prereading
 * M33  - Get the longname version of a path. (Requires LONG_FILENAME_HOST_SUPPORT)
 * M34  - Set SD Card sorting options. (Requires SDCARD_SORT_ALPHA)
 * M42  - Change pin status via gcode: M42 P<pin> S<value>. LED pin assumed if P is omitted.
 * M43  - Display pin status, watch pins for changes, watch endstops & toggle LED, Z servo probe test, toggle pins
 * M48  - Measure Z Probe repeatability: M48 P<points> X<pos> Y<pos> V<level> E<engage> L<legs> S<chizoid>. (Requires Z_MIN_PROBE_REPEATABILITY_TEST)
 * M75  - Start the print job timer.
 * M76  - Pause the print job timer.
 * M77  - Stop the print job timer.
 * M78  - Show statistical information about the print jobs. (Requires PRINTCOUNTER)
 * M80  - Turn on Power Supply. (Requires POWER_SUPPLY > 0)
 * M81  - Turn off Power Supply. (Requires POWER_SUPPLY > 0)
 * M82  - Set E codes absolute (default).
 * M83  - Set E codes relative while in Absolute (G90) mode.
 * M84  - Disable steppers until next move, or use S<seconds> to specify an idle
 *        duration after which steppers should turn off. S0 disables the timeout.
 * M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 * M92  - Set planner.axis_steps_per_mm for one or more axes.
 * M100 - Watch Free Memory (for debugging) (Requires M100_FREE_MEMORY_WATCHER)
 * M104 - Set extruder target temp.
 * M105 - Report current temperatures.
 * M106 - Set print fan speed.
 * M107 - Print fan off.
 * M108 - Break out of heating loops (M109, M190, M303). With no controller, breaks out of M0/M1. (Requires EMERGENCY_PARSER)
 * M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
 *        Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
 *        If AUTOTEMP is enabled, S<mintemp> B<maxtemp> F<factor>. Exit autotemp by any M109 without F
 * M110 - Set the current line number. (Used by host printing)
 * M111 - Set debug flags: "M111 S<flagbits>". See flag bits defined in enum.h.
 * M112 - Emergency stop.
 * M113 - Get or set the timeout interval for Host Keepalive "busy" messages. (Requires HOST_KEEPALIVE_FEATURE)
 * M114 - Report current position.
 * M115 - Report capabilities. (Extended capabilities requires EXTENDED_CAPABILITIES_REPORT)
 * M117 - Display a message on the controller screen. (Requires an LCD)
 * M118 - Display a message in the host console.
 * M119 - Report endstops status.
 * M120 - Enable endstops detection.
 * M121 - Disable endstops detection.
 * M122 - Debug stepper (Requires HAVE_TMC2130)
 * M125 - Save current position and move to filament change position. (Requires PARK_HEAD_ON_PAUSE)
 * M126 - Solenoid Air Valve Open. (Requires BARICUDA)
 * M127 - Solenoid Air Valve Closed. (Requires BARICUDA)
 * M128 - EtoP Open. (Requires BARICUDA)
 * M129 - EtoP Closed. (Requires BARICUDA)
 * M140 - Set bed target temp. S<temp>
 * M145 - Set heatup values for materials on the LCD. H<hotend> B<bed> F<fan speed> for S<material> (0=PLA, 1=ABS)
 * M149 - Set temperature units. (Requires TEMPERATURE_UNITS_SUPPORT)
 * M150 - Set Status LED Color as R<red> U<green> B<blue> P<bright>. Values 0-255. (Requires BLINKM, RGB_LED, RGBW_LED, NEOPIXEL_LED, or PCA9632).
 * M155 - Auto-report temperatures with interval of S<seconds>. (Requires AUTO_REPORT_TEMPERATURES)
 * M163 - Set a single proportion for a mixing extruder. (Requires MIXING_EXTRUDER)
 * M164 - Save the mix as a virtual extruder. (Requires MIXING_EXTRUDER and MIXING_VIRTUAL_TOOLS)
 * M165 - Set the proportions for a mixing extruder. Use parameters ABCDHI to set the mixing factors. (Requires MIXING_EXTRUDER)
 * M190 - Sxxx Wait for bed current temp to reach target temp. ** Waits only when heating! **
 *        Rxxx Wait for bed current temp to reach target temp. ** Waits for heating or cooling. **
 * M200 - Set filament diameter, D<diameter>, setting E axis units to cubic. (Use S0 to revert to linear units.)
 * M201 - Set max acceleration in units/s^2 for print moves: "M201 X<accel> Y<accel> Z<accel> E<accel>"
 * M202 - Set max acceleration in units/s^2 for travel moves: "M202 X<accel> Y<accel> Z<accel> E<accel>" ** UNUSED IN MARLIN! **
 * M203 - Set maximum feedrate: "M203 X<fr> Y<fr> Z<fr> E<fr>" in units/sec.
 * M204 - Set default acceleration in units/sec^2: P<printing> R<extruder_only> T<travel>
 * M205 - Set advanced settings. Current units apply:
            S<print> T<travel> minimum speeds
            B<minimum segment time>
            X<max X jerk>, Y<max Y jerk>, Z<max Z jerk>, E<max E jerk>
 * M206 - Set additional homing offset. (Disabled by NO_WORKSPACE_OFFSETS or DELTA)
 * M207 - Set Retract Length: S<length>, Feedrate: F<units/min>, and Z lift: Z<distance>. (Requires FWRETRACT)
 * M208 - Set Recover (unretract) Additional (!) Length: S<length> and Feedrate: F<units/min>. (Requires FWRETRACT)
 * M209 - Turn Automatic Retract Detection on/off: S<0|1> (For slicers that don't support G10/11). (Requires FWRETRACT)
          Every normal extrude-only move will be classified as retract depending on the direction.
 * M211 - Enable, Disable, and/or Report software endstops: S<0|1> (Requires MIN_SOFTWARE_ENDSTOPS or MAX_SOFTWARE_ENDSTOPS)
 * M218 - Set a tool offset: "M218 T<index> X<offset> Y<offset>". (Requires 2 or more extruders)
 * M220 - Set Feedrate Percentage: "M220 S<percent>" (i.e., "FR" on the LCD)
 * M221 - Set Flow Percentage: "M221 S<percent>"
 * M226 - Wait until a pin is in a given state: "M226 P<pin> S<state>"
 * M240 - Trigger a camera to take a photograph. (Requires CHDK or PHOTOGRAPH_PIN)
 * M250 - Set LCD contrast: "M250 C<contrast>" (0-63). (Requires LCD support)
 * M260 - i2c Send Data (Requires EXPERIMENTAL_I2CBUS)
 * M261 - i2c Request Data (Requires EXPERIMENTAL_I2CBUS)
 * M280 - Set servo position absolute: "M280 P<index> S<angle|µs>". (Requires servos)
 * M290 - Babystepping (Requires BABYSTEPPING)
 * M300 - Play beep sound S<frequency Hz> P<duration ms>
 * M301 - Set PID parameters P I and D. (Requires PIDTEMP)
 * M302 - Allow cold extrudes, or set the minimum extrude S<temperature>. (Requires PREVENT_COLD_EXTRUSION)
 * M303 - PID relay autotune S<temperature> sets the target temperature. Default 150C. (Requires PIDTEMP)
 * M304 - Set bed PID parameters P I and D. (Requires PIDTEMPBED)
 * M350 - Set microstepping mode. (Requires digital microstepping pins.)
 * M351 - Toggle MS1 MS2 pins directly. (Requires digital microstepping pins.)
 * M355 - Set Case Light on/off and set brightness. (Requires CASE_LIGHT_PIN)
 * M380 - Activate solenoid on active extruder. (Requires EXT_SOLENOID)
 * M381 - Disable all solenoids. (Requires EXT_SOLENOID)
 * M400 - Finish all moves.
 * M401 - Deploy and activate Z probe. (Requires a probe)
 * M402 - Deactivate and stow Z probe. (Requires a probe)
 * M404 - Display or set the Nominal Filament Width: "W<diameter>". (Requires FILAMENT_WIDTH_SENSOR)
 * M405 - Enable Filament Sensor flow control. "M405 D<delay_cm>". (Requires FILAMENT_WIDTH_SENSOR)
 * M406 - Disable Filament Sensor flow control. (Requires FILAMENT_WIDTH_SENSOR)
 * M407 - Display measured filament diameter in millimeters. (Requires FILAMENT_WIDTH_SENSOR)
 * M410 - Quickstop. Abort all planned moves.
 * M420 - Enable/Disable Leveling (with current values) S1=enable S0=disable (Requires MESH_BED_LEVELING or ABL)
 * M421 - Set a single Z coordinate in the Mesh Leveling grid. X<units> Y<units> Z<units> (Requires MESH_BED_LEVELING or AUTO_BED_LEVELING_UBL)
 * M428 - Set the home_offset based on the current_position. Nearest edge applies. (Disabled by NO_WORKSPACE_OFFSETS or DELTA)
 * M500 - Store parameters in EEPROM. (Requires EEPROM_SETTINGS)
 * M501 - Restore parameters from EEPROM. (Requires EEPROM_SETTINGS)
 * M502 - Revert to the default "factory settings". ** Does not write them to EEPROM! **
 * M503 - Print the current settings (in memory): "M503 S<verbose>". S0 specifies compact output.
 * M540 - Enable/disable SD card abort on endstop hit: "M540 S<state>". (Requires ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
 * M600 - Pause for filament change: "M600 X<pos> Y<pos> Z<raise> E<first_retract> L<later_retract>". (Requires ADVANCED_PAUSE_FEATURE)
 * M603 - Configure filament change: "M603 T<tool> U<unload_length> L<load_length>". (Requires ADVANCED_PAUSE_FEATURE)
 * M665 - Set delta configurations: "M665 L<diagonal rod> R<delta radius> S<segments/s> A<rod A trim mm> B<rod B trim mm> C<rod C trim mm> I<tower A trim angle> J<tower B trim angle> K<tower C trim angle>" (Requires DELTA)
 * M666 - Set delta endstop adjustment. (Requires DELTA)
 * M605 - Set dual x-carriage movement mode: "M605 S<mode> [X<x_offset>] [R<temp_offset>]". (Requires DUAL_X_CARRIAGE)
 * M701 - Load filament (requires FILAMENT_LOAD_UNLOAD_GCODES)
 * M702 - Unload filament (requires FILAMENT_LOAD_UNLOAD_GCODES)
 * M851 - Set Z probe's Z offset in current units. (Negative = below the nozzle.)
 * M852 - Set skew factors: "M852 [I<xy>] [J<xz>] [K<yz>]". (Requires SKEW_CORRECTION_GCODE, and SKEW_CORRECTION_FOR_Z for IJ)
 * M860 - Report the position of position encoder modules.
 * M861 - Report the status of position encoder modules.
 * M862 - Perform an axis continuity test for position encoder modules.
 * M863 - Perform steps-per-mm calibration for position encoder modules.
 * M864 - Change position encoder module I2C address.
 * M865 - Check position encoder module firmware version.
 * M866 - Report or reset position encoder module error count.
 * M867 - Enable/disable or toggle error correction for position encoder modules.
 * M868 - Report or set position encoder module error correction threshold.
 * M869 - Report position encoder module error.
 * M900 - Get or Set Linear Advance K-factor. (Requires LIN_ADVANCE)
 * M906 - Set or get motor current in milliamps using axis codes X, Y, Z, E. Report values if no axis codes given. (Requires HAVE_TMC2130 or HAVE_TMC2208)
 * M907 - Set digital trimpot motor current using axis codes. (Requires a board with digital trimpots)
 * M908 - Control digital trimpot directly. (Requires DAC_STEPPER_CURRENT or DIGIPOTSS_PIN)
 * M909 - Print digipot/DAC current value. (Requires DAC_STEPPER_CURRENT)
 * M910 - Commit digipot/DAC value to external EEPROM via I2C. (Requires DAC_STEPPER_CURRENT)
 * M911 - Report stepper driver overtemperature pre-warn condition. (Requires HAVE_TMC2130 or HAVE_TMC2208)
 * M912 - Clear stepper driver overtemperature pre-warn condition flag. (Requires HAVE_TMC2130 or HAVE_TMC2208)
 * M913 - Set HYBRID_THRESHOLD speed. (Requires HYBRID_THRESHOLD)
 * M914 - Set SENSORLESS_HOMING sensitivity. (Requires SENSORLESS_HOMING)
 *
 * M360 - SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
 * M361 - SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
 * M362 - SCARA calibration: Move to cal-position PsiA (0 deg calibration)
 * M363 - SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
 * M364 - SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
 *
 * ************ Custom codes - This can change to suit future G-code regulations
 * M928 - Start SD logging: "M928 filename.gco". Stop with M29. (Requires SDSUPPORT)
 * M999 - Restart after being stopped by error
 *
 * "T" Codes
 *
 * T0-T3 - Select an extruder (tool) by index: "T<n> F<units/min>"
 *
 */

#include "Marlin.h"

#include "ultralcd.h"
#include "planner.h"
#include "stepper.h"
#include "endstops.h"
#include "temperature.h"
#include "cardreader.h"
#include "configuration_store.h"
#include "language.h"
#include "pins_arduino.h"
#include "math.h"
#include "nozzle.h"
#include "duration_t.h"
#include "types.h"
#include "gcode.h"

#if ENABLED(AUTO_POWER_CONTROL)
  #include "power.h"
#endif

#if HAS_ABL
  #include "vector_3.h"
  #if ENABLED(AUTO_BED_LEVELING_LINEAR)
    #include "least_squares_fit.h"
  #endif
#elif ENABLED(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

#if ENABLED(BEZIER_CURVE_SUPPORT)
  #include "planner_bezier.h"
#endif

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "buzzer.h"
#endif

#if ENABLED(USE_WATCHDOG)
  #include "watchdog.h"
#endif

#if ENABLED(MAX7219_DEBUG)
  #include "Max7219_Debug_LEDs.h"
#endif

#if HAS_COLOR_LEDS
  #include "leds.h"
#endif

#if HAS_SERVOS
  #include "servo.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

#if HAS_TRINAMIC
  #include "tmc_util.h"
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "stepper_dac.h"
#endif

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "twibus.h"
#endif

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "I2CPositionEncoder.h"
#endif

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  #include "endstop_interrupts.h"
#endif

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  void gcode_M100();
  void M100_dump_routine(const char * const title, const char *start, const char *end);
#endif

#if ENABLED(G26_MESH_VALIDATION)
  bool g26_debug_flag; // =false
  void gcode_G26();
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
  #include "ubl.h"
#endif

#if ENABLED(CNC_COORDINATE_SYSTEMS)
  int8_t active_coordinate_system = -1; // machine space
  float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ];
#endif

bool Running = true;

uint8_t marlin_debug_flags = DEBUG_NONE;

/**
 * Cartesian Current Position
 *   Used to track the native machine position as moves are queued.
 *   Used by 'buffer_line_to_current_position' to do a move after changing it.
 *   Used by 'SYNC_PLAN_POSITION_KINEMATIC' to update 'planner.position'.
 */
float current_position[XYZE] = { 0.0 };

/**
 * Cartesian Destination
 *   The destination for a move, filled in by G-code movement commands,
 *   and expected by functions like 'prepare_move_to_destination'.
 *   Set with 'gcode_get_destination' or 'set_destination_from_current'.
 */
float destination[XYZE] = { 0.0 };

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

/**
 * GCode line number handling. Hosts may opt to include line numbers when
 * sending commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The process_next_command function parses the next
 * command and hands off execution to individual handler functions.
 */
uint8_t commands_in_queue = 0; // Count of commands in the queue
static uint8_t cmd_queue_index_r = 0, // Ring buffer read position
               cmd_queue_index_w = 0; // Ring buffer write position
#if ENABLED(M100_FREE_MEMORY_WATCHER)
  char command_queue[BUFSIZE][MAX_CMD_SIZE];  // Necessary so M100 Free Memory Dumper can show us the commands and any corruption
#else                                         // This can be collapsed back to the way it was soon.
static char command_queue[BUFSIZE][MAX_CMD_SIZE];
#endif

/**
 * Next Injected Command pointer. NULL if no commands are being injected.
 * Used by Marlin internally to ensure that commands initiated from within
 * are enqueued ahead of any pending serial or sd card commands.
 */
static const char *injected_commands_P = NULL;

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

float feedrate_mm_s = MMM_TO_MMS(1500.0);
static float saved_feedrate_mm_s;
int16_t feedrate_percentage = 100, saved_feedrate_percentage;

// Initialized by settings.load()
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;

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

// Software Endstops are based on the configured limits.
float soft_endstop_min[XYZ] = { X_MIN_BED, Y_MIN_BED, Z_MIN_POS },
      soft_endstop_max[XYZ] = { X_MAX_BED, Y_MAX_BED, Z_MAX_POS };
#if HAS_SOFTWARE_ENDSTOPS
  bool soft_endstops_enabled = true;
  #if IS_KINEMATIC
    float soft_endstop_radius, soft_endstop_radius_2;
  #endif
#endif

#if FAN_COUNT > 0
  int16_t fanSpeeds[FAN_COUNT] = { 0 };
  #if ENABLED(EXTRA_FAN_SPEED)
    int16_t old_fanSpeeds[FAN_COUNT],
            new_fanSpeeds[FAN_COUNT];
  #endif
  #if ENABLED(PROBING_FANS_OFF)
    bool fans_paused = false;
    int16_t paused_fanSpeeds[FAN_COUNT] = { 0 };
  #endif
#endif

#if ENABLED(USE_CONTROLLER_FAN)
  int controllerFanSpeed = 0;
#endif

// The active extruder (tool). Set with T<extruder> command.
uint8_t active_extruder = 0;

// Relative Mode. Enable with G91, disable with G90.
static bool relative_mode = false;

// For M109 and M190, this flag may be cleared (by M108) to exit the wait loop
volatile bool wait_for_heatup = true;

// For M0/M1, this flag may be cleared (by M108) to exit the wait-for-user loop
#if HAS_RESUME_CONTINUE
  volatile bool wait_for_user = false;
#endif

const char axis_codes[XYZE] = { 'X', 'Y', 'Z', 'E' };

// Number of characters read in the current line of serial input
static int serial_count = 0;

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

// Auto Power Control
#if ENABLED(AUTO_POWER_CONTROL)
  #define PSU_ON()  powerManager.power_on()
  #define PSU_OFF() powerManager.power_off()
#else
  #define PSU_ON()  OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE)
  #define PSU_OFF() OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP)
#endif

// Buzzer - I2C on the LCD or a BEEPER_PIN
#if ENABLED(LCD_USE_I2C_BUZZER)
  #define BUZZ(d,f) lcd_buzz(d, f)
#elif PIN_EXISTS(BEEPER)
  Buzzer buzzer;
  #define BUZZ(d,f) buzzer.tone(d, f)
#else
  #define BUZZ(d,f) NOOP
#endif

uint8_t target_extruder;

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
      if (planner.leveling_active) { \
        const float zadj = bilinear_z_offset(V); \
        delta[A_AXIS] += zadj; \
        delta[B_AXIS] += zadj; \
        delta[C_AXIS] += zadj; \
      }
  #else
    #define ADJUST_DELTA(V) if (planner.leveling_active) { delta[Z_AXIS] += bilinear_z_offset(V); }
  #endif
#elif IS_KINEMATIC
  #define ADJUST_DELTA(V) NOOP
#endif

#if ENABLED(X_DUAL_ENDSTOPS)
  float x_endstop_adj;                // Initialized by settings.load()
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  float y_endstop_adj;                // Initialized by settings.load()
#endif
#if ENABLED(Z_DUAL_ENDSTOPS)
  float z_endstop_adj;                // Initialized by settings.load()
#endif

// Extruder offsets
#if HOTENDS > 1
  float hotend_offset[XYZ][HOTENDS];  // Initialized by settings.load()
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

  float delta[ABC];

  // Initialized by settings.load()
  float delta_height,
        delta_endstop_adj[ABC] = { 0 },
        delta_radius,
        delta_tower_angle_trim[ABC],
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
  uint8_t meas_delay_cm = MEASUREMENT_DELAY_CM;                 // Distance delay setting
  int8_t measurement_delay[MAX_MEASUREMENT_DELAY + 1],          // Ring buffer to delayed measurement. Store extruder factor after subtracting 100
         filwidth_delay_index[2] = { 0, -1 };                   // Indexes into ring buffer
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  static bool filament_ran_out = false;
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  AdvancedPauseMenuResponse advanced_pause_menu_response;
  float filament_change_unload_length[EXTRUDERS],
        filament_change_load_length[EXTRUDERS];
#endif

#if ENABLED(MIXING_EXTRUDER)
  float mixing_factor[MIXING_STEPPERS]; // Reciprocal of mix proportion. 0.0 = off, otherwise >= 1.0.
  #if MIXING_VIRTUAL_TOOLS > 1
    float mixing_virtual_tool_mix[MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];
  #endif
#endif

static bool send_ok[BUFSIZE];

#if HAS_SERVOS
  Servo servo[NUM_SERVOS];
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
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)
  static WorkspacePlane workspace_plane = PLANE_XY;
#endif

FORCE_INLINE float pgm_read_any(const float *p) { return pgm_read_float_near(p); }
FORCE_INLINE signed char pgm_read_any(const signed char *p) { return pgm_read_byte_near(p); }

#define XYZ_CONSTS_FROM_CONFIG(type, array, CONFIG) \
  static const PROGMEM type array##_P[XYZ] = { X_##CONFIG, Y_##CONFIG, Z_##CONFIG }; \
  static inline type array(const AxisEnum axis) { return pgm_read_any(&array##_P[axis]); } \
  typedef void __void_##CONFIG##__

XYZ_CONSTS_FROM_CONFIG(float, base_min_pos,   MIN_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_max_pos,   MAX_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_home_pos,  HOME_POS);
XYZ_CONSTS_FROM_CONFIG(float, max_length,     MAX_LENGTH);
XYZ_CONSTS_FROM_CONFIG(float, home_bump_mm,   HOME_BUMP_MM);
XYZ_CONSTS_FROM_CONFIG(signed char, home_dir, HOME_DIR);

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */

void stop();

void get_available_commands();
void process_next_command();
void process_parsed_command();

void get_cartesian_from_steppers();
void set_current_from_steppers_for_axis(const AxisEnum axis);

#if ENABLED(ARC_SUPPORT)
  void plan_arc(const float (&cart)[XYZE], const float (&offset)[2], const bool clockwise);
#endif

#if ENABLED(BEZIER_CURVE_SUPPORT)
  void plan_cubic_move(const float (&offset)[4]);
#endif

void tool_change(const uint8_t tmp_extruder, const float fr_mm_s=0.0, bool no_move=false);
void report_current_position();
void report_current_position_detail();

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void print_xyz(const char* prefix, const char* suffix, const float x, const float y, const float z) {
    serialprintPGM(prefix);
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_ECHOPAIR(", ", y);
    SERIAL_ECHOPAIR(", ", z);
    SERIAL_CHAR(')');
    if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
  }

  void print_xyz(const char* prefix, const char* suffix, const float xyz[]) {
    print_xyz(prefix, suffix, xyz[X_AXIS], xyz[Y_AXIS], xyz[Z_AXIS]);
  }

  #if HAS_ABL
    void print_xyz(const char* prefix, const char* suffix, const vector_3 &xyz) {
      print_xyz(prefix, suffix, xyz.x, xyz.y, xyz.z);
    }
  #endif

  #define DEBUG_POS(SUFFIX,VAR) do { \
    print_xyz(PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n"), VAR); }while(0)
#endif

/**
 * sync_plan_position
 *
 * Set the planner/stepper positions directly from current_position with
 * no kinematic translation. Used for homing axes and cartesian/core syncing.
 */
void sync_plan_position() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("sync_plan_position", current_position);
  #endif
  planner.set_position_mm(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
void sync_plan_position_e() { planner.set_e_position_mm(current_position[E_AXIS]); }

#if IS_KINEMATIC
  inline void sync_plan_position_kinematic() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("sync_plan_position_kinematic", current_position);
    #endif
    planner.set_position_mm_kinematic(current_position);
  }
#endif

#if ENABLED(SDSUPPORT)
  #include "SdFatUtil.h"
  int freeMemory() { return SdFatUtil::FreeRam(); }
#else
extern "C" {
  extern char __bss_end;
  extern char __heap_start;
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
#endif // !SDSUPPORT

#if ENABLED(DIGIPOT_I2C)
  extern void digipot_i2c_set_current(uint8_t channel, float current);
  extern void digipot_i2c_init();
#endif

/**
 * Inject the next "immediate" command, when possible, onto the front of the queue.
 * Return true if any immediate commands remain to inject.
 */
static bool drain_injected_commands_P() {
  if (injected_commands_P != NULL) {
    size_t i = 0;
    char c, cmd[30];
    strncpy_P(cmd, injected_commands_P, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0';
    while ((c = cmd[i]) && c != '\n') i++; // find the end of this gcode command
    cmd[i] = '\0';
    if (enqueue_and_echo_command(cmd))     // success?
      injected_commands_P = c ? injected_commands_P + i + 1 : NULL; // next command or done
  }
  return (injected_commands_P != NULL);    // return whether any more remain
}

/**
 * Record one or many commands to run from program memory.
 * Aborts the current queue, if any.
 * Note: drain_injected_commands_P() must be called repeatedly to drain the commands afterwards
 */
void enqueue_and_echo_commands_P(const char * const pgcode) {
  injected_commands_P = pgcode;
  (void)drain_injected_commands_P(); // first command executed asap (when possible)
}

/**
 * Clear the Marlin command queue
 */
void clear_command_queue() {
  cmd_queue_index_r = cmd_queue_index_w = commands_in_queue = 0;
}

/**
 * Once a new command is in the ring buffer, call this to commit it
 */
inline void _commit_command(bool say_ok) {
  send_ok[cmd_queue_index_w] = say_ok;
  if (++cmd_queue_index_w >= BUFSIZE) cmd_queue_index_w = 0;
  commands_in_queue++;
}

/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
inline bool _enqueuecommand(const char* cmd, bool say_ok=false) {
  if (*cmd == ';' || commands_in_queue >= BUFSIZE) return false;
  strcpy(command_queue[cmd_queue_index_w], cmd);
  _commit_command(say_ok);
  return true;
}

/**
 * Enqueue with Serial Echo
 */
bool enqueue_and_echo_command(const char* cmd, bool say_ok/*=false*/) {
  if (_enqueuecommand(cmd, say_ok)) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR(MSG_ENQUEUEING, cmd);
    SERIAL_CHAR('"');
    SERIAL_EOL();
    return true;
  }
  return false;
}

#if HAS_QUEUE_NOW
  void enqueue_and_echo_command_now(const char* cmd, bool say_ok/*=false*/) {
    while (!enqueue_and_echo_command(cmd, say_ok)) idle();
  }
  #if HAS_LCD_QUEUE_NOW
    void enqueue_and_echo_commands_P_now(const char * const pgcode) {
      enqueue_and_echo_commands_P(pgcode);
      while (drain_injected_commands_P()) idle();
    }
  #endif
#endif

void setup_killpin() {
  #if HAS_KILL
    SET_INPUT_PULLUP(KILL_PIN);
  #endif
}

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

  void setup_filament_runout_pins() {

    #if ENABLED(FIL_RUNOUT_PULLUP)
      #define INIT_RUNOUT_PIN(P) SET_INPUT_PULLUP(P)
    #else
      #define INIT_RUNOUT_PIN(P) SET_INPUT(P)
    #endif

    INIT_RUNOUT_PIN(FIL_RUNOUT_PIN);
    #if NUM_RUNOUT_SENSORS > 1
      INIT_RUNOUT_PIN(FIL_RUNOUT2_PIN);
      #if NUM_RUNOUT_SENSORS > 2
        INIT_RUNOUT_PIN(FIL_RUNOUT3_PIN);
        #if NUM_RUNOUT_SENSORS > 3
          INIT_RUNOUT_PIN(FIL_RUNOUT4_PIN);
          #if NUM_RUNOUT_SENSORS > 4
            INIT_RUNOUT_PIN(FIL_RUNOUT5_PIN);
          #endif
        #endif
      #endif
    #endif
  }

#endif // FILAMENT_RUNOUT_SENSOR

void setup_powerhold() {
  #if HAS_SUICIDE
    OUT_WRITE(SUICIDE_PIN, HIGH);
  #endif
  #if HAS_POWER_SWITCH
    #if ENABLED(PS_DEFAULT_OFF)
      PSU_OFF();
    #else
      PSU_ON();
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

void gcode_line_error(const char* err, bool doFlush = true) {
  SERIAL_ERROR_START();
  serialprintPGM(err);
  SERIAL_ERRORLN(gcode_LastN);
  //Serial.println(gcode_N);
  if (doFlush) FlushSerialRequestResend();
  serial_count = 0;
}

/**
 * Get all commands waiting on the serial port and queue them.
 * Exit when the buffer is full or when no more characters are
 * left on the serial port.
 */
inline void get_serial_commands() {
  static char serial_line_buffer[MAX_CMD_SIZE];
  static bool serial_comment_mode = false;

  // If the command buffer is empty for too long,
  // send "wait" to indicate Marlin is still waiting.
  #if NO_TIMEOUTS > 0
    static millis_t last_command_time = 0;
    const millis_t ms = millis();
    if (commands_in_queue == 0 && !MYSERIAL0.available() && ELAPSED(ms, last_command_time + NO_TIMEOUTS)) {
      SERIAL_ECHOLNPGM(MSG_WAIT);
      last_command_time = ms;
    }
  #endif

  /**
   * Loop while serial characters are incoming and the queue is not full
   */
  int c;
  while (commands_in_queue < BUFSIZE && (c = MYSERIAL0.read()) >= 0) {

    char serial_char = c;

    /**
     * If the character ends the line
     */
    if (serial_char == '\n' || serial_char == '\r') {

      serial_comment_mode = false;                      // end of line == end of comment

      // Skip empty lines and comments
      if (!serial_count) { thermalManager.manage_heater(); continue; }

      serial_line_buffer[serial_count] = 0;             // Terminate string
      serial_count = 0;                                 // Reset buffer

      char* command = serial_line_buffer;

      while (*command == ' ') command++;                // Skip leading spaces
      char *npos = (*command == 'N') ? command : NULL;  // Require the N parameter to start the line

      if (npos) {

        bool M110 = strstr_P(command, PSTR("M110")) != NULL;

        if (M110) {
          char* n2pos = strchr(command + 4, 'N');
          if (n2pos) npos = n2pos;
        }

        gcode_N = strtol(npos + 1, NULL, 10);

        if (gcode_N != gcode_LastN + 1 && !M110) {
          gcode_line_error(PSTR(MSG_ERR_LINE_NO));
          return;
        }

        char *apos = strrchr(command, '*');
        if (apos) {
          uint8_t checksum = 0, count = uint8_t(apos - command);
          while (count) checksum ^= command[--count];
          if (strtol(apos + 1, NULL, 10) != checksum) {
            gcode_line_error(PSTR(MSG_ERR_CHECKSUM_MISMATCH));
            return;
          }
        }
        else {
          gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM));
          return;
        }

        gcode_LastN = gcode_N;
      }

      // Movement commands alert when stopped
      if (IsStopped()) {
        char* gpos = strchr(command, 'G');
        if (gpos) {
          const int codenum = strtol(gpos + 1, NULL, 10);
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

      #if DISABLED(EMERGENCY_PARSER)
        // Process critical commands early
        if (strcmp(command, "M108") == 0) {
          wait_for_heatup = false;
          #if ENABLED(NEWPANEL)
            wait_for_user = false;
          #endif
        }
        if (strcmp(command, "M112") == 0) kill(PSTR(MSG_KILLED));
        if (strcmp(command, "M410") == 0) quickstop_stepper();
      #endif

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
    else if (serial_char == '\\') {   // Handle escapes
      if ((c = MYSERIAL0.read()) >= 0 && !serial_comment_mode) // if we have one more character, copy it over
        serial_line_buffer[serial_count++] = (char)c;
      // otherwise do nothing
    }
    else { // it's not a newline, carriage return or escape char
      if (serial_char == ';') serial_comment_mode = true;
      if (!serial_comment_mode) serial_line_buffer[serial_count++] = serial_char;
    }

  } // queue has space, serial has data
}

#if ENABLED(SDSUPPORT)

  /**
   * Get commands from the SD Card until the command buffer is full
   * or until the end of the file is reached. The special character '#'
   * can also interrupt buffering.
   */
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
      const int16_t n = card.get();
      char sd_char = (char)n;
      card_eof = card.eof();
      if (card_eof || n == -1
          || sd_char == '\n' || sd_char == '\r'
          || ((sd_char == '#' || sd_char == ':') && !sd_comment_mode)
      ) {
        if (card_eof) {

          card.printingHasFinished();

          if (card.sdprinting)
            sd_count = 0; // If a sub-file was printing, continue from call point
          else {
            SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
            #if ENABLED(PRINTER_EVENT_LEDS)
              LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
              leds.set_green();
              #if HAS_RESUME_CONTINUE
                enqueue_and_echo_commands_P(PSTR("M0")); // end of the queue!
              #else
                safe_delay(1000);
              #endif
              leds.set_off();
            #endif
            card.checkautostart(true);
          }
        }
        else if (n == -1) {
          SERIAL_ERROR_START();
          SERIAL_ECHOLNPGM(MSG_SD_ERR_READ);
        }
        if (sd_char == '#') stop_buffering = true;

        sd_comment_mode = false; // for new command

        // Skip empty lines and comments
        if (!sd_count) { thermalManager.manage_heater(); continue; }

        command_queue[cmd_queue_index_w][sd_count] = '\0'; // terminate string
        sd_count = 0; // clear sd line buffer

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
 *  - The command-injection queue (injected_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void get_available_commands() {

  // if any immediate commands remain, don't get other commands yet
  if (drain_injected_commands_P()) return;

  get_serial_commands();

  #if ENABLED(SDSUPPORT)
    get_sdcard_commands();
  #endif
}

/**
 * Set target_extruder from the T parameter or the active_extruder
 *
 * Returns TRUE if the target is invalid
 */
bool get_target_extruder_from_command(const uint16_t code) {
  if (parser.seenval('T')) {
    const int8_t e = parser.value_byte();
    if (e >= EXTRUDERS) {
      SERIAL_ECHO_START();
      SERIAL_CHAR('M');
      SERIAL_ECHO(code);
      SERIAL_ECHOLNPAIR(" " MSG_INVALID_EXTRUDER " ", e);
      return true;
    }
    target_extruder = e;
  }
  else
    target_extruder = active_extruder;

  return false;
}

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  bool extruder_duplication_enabled = false; // Used in Dual X mode 2
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  static DualXMode dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;

  static float x_home_pos(const int extruder) {
    if (extruder == 0)
      return base_home_pos(X_AXIS);
    else
      /**
       * In dual carriage mode the extruder offset provides an override of the
       * second X-carriage position when homed - otherwise X2_HOME_POS is used.
       * This allows soft recalibration of the second extruder home position
       * without firmware reflash (through the M218 command).
       */
      return hotend_offset[X_AXIS][1] > 0 ? hotend_offset[X_AXIS][1] : X2_HOME_POS;
  }

  static int x_home_dir(const int extruder) { return extruder ? X2_HOME_DIR : X_HOME_DIR; }

  static float inactive_extruder_x_pos = X2_MAX_POS; // used in mode 0 & 1
  static bool active_extruder_parked = false;        // used in mode 1 & 2
  static float raised_parked_position[XYZE];         // used in mode 1
  static millis_t delayed_move_time = 0;             // used in mode 1
  static float duplicate_extruder_x_offset = DEFAULT_DUPLICATION_X_OFFSET; // used in mode 2
  static int16_t duplicate_extruder_temp_offset = 0; // used in mode 2

#endif // DUAL_X_CARRIAGE

#if HAS_WORKSPACE_OFFSET || ENABLED(DUAL_X_CARRIAGE) || ENABLED(DELTA)

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
    #if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
      workspace_offset[axis] = home_offset[axis] + position_shift[axis];
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      if (axis == X_AXIS) {

        // In Dual X mode hotend_offset[X] is T1's home position
        float dual_max_x = max(hotend_offset[X_AXIS][1], X2_MAX_POS);

        if (active_extruder != 0) {
          // T1 can move from X2_MIN_POS to X2_MAX_POS or X2 home position (whichever is larger)
          soft_endstop_min[X_AXIS] = X2_MIN_POS;
          soft_endstop_max[X_AXIS] = dual_max_x;
        }
        else if (dual_x_carriage_mode == DXC_DUPLICATION_MODE) {
          // In Duplication Mode, T0 can move as far left as X_MIN_POS
          // but not so far to the right that T1 would move past the end
          soft_endstop_min[X_AXIS] = base_min_pos(X_AXIS);
          soft_endstop_max[X_AXIS] = min(base_max_pos(X_AXIS), dual_max_x - duplicate_extruder_x_offset);
        }
        else {
          // In other modes, T0 can move from X_MIN_POS to X_MAX_POS
          soft_endstop_min[axis] = base_min_pos(axis);
          soft_endstop_max[axis] = base_max_pos(axis);
        }
      }
    #elif ENABLED(DELTA)
      soft_endstop_min[axis] = base_min_pos(axis);
      soft_endstop_max[axis] = axis == Z_AXIS ? delta_height : base_max_pos(axis);
    #else
      soft_endstop_min[axis] = base_min_pos(axis);
      soft_endstop_max[axis] = base_max_pos(axis);
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
      switch(axis) {
        #if HAS_SOFTWARE_ENDSTOPS
          case X_AXIS:
          case Y_AXIS:
            // Get a minimum radius for clamping
            soft_endstop_radius = MIN3(FABS(max(soft_endstop_min[X_AXIS], soft_endstop_min[Y_AXIS])), soft_endstop_max[X_AXIS], soft_endstop_max[Y_AXIS]);
            soft_endstop_radius_2 = sq(soft_endstop_radius);
            break;
        #endif
        case Z_AXIS:
          delta_clip_start_height = soft_endstop_max[axis] - delta_safe_distance_from_top();
        default: break;
      }
    #endif
  }

#endif // HAS_WORKSPACE_OFFSET || DUAL_X_CARRIAGE || DELTA

#if HAS_M206_COMMAND
  /**
   * Change the home offset for an axis.
   * Also refreshes the workspace offset.
   */
  static void set_home_offset(const AxisEnum axis, const float v) {
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

      float homeposition[XYZ] = {
        base_home_pos(X_AXIS),
        base_home_pos(Y_AXIS),
        base_home_pos(Z_AXIS)
      };

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

      current_position[axis] = cartes[axis];

      /**
       * SCARA home positions are based on configuration since the actual
       * limits are determined by the inverse kinematic transform.
       */
      soft_endstop_min[axis] = base_min_pos(axis); // + (cartes[axis] - base_home_pos(axis));
      soft_endstop_max[axis] = base_max_pos(axis); // + (cartes[axis] - base_home_pos(axis));
    }
    else
  #elif ENABLED(DELTA)
    if (axis == Z_AXIS)
      current_position[axis] = delta_height;
    else
  #endif
  {
    current_position[axis] = base_home_pos(axis);
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
 * Move the planner to the current position from wherever it last moved
 * (or from wherever it has been told it is located).
 */
inline void buffer_line_to_current_position() {
  planner.buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate_mm_s, active_extruder);
}

/**
 * Move the planner to the position stored in the destination array, which is
 * used by G0/G1/G2/G3/G5 and many other functions to set a destination.
 */
inline void buffer_line_to_destination(const float &fr_mm_s) {
  planner.buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], fr_mm_s, active_extruder);
}

#if IS_KINEMATIC
  /**
   * Calculate delta, start a line, and set current_position to destination
   */
  void prepare_uninterpolated_move_to_destination(const float fr_mm_s=0.0) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("prepare_uninterpolated_move_to_destination", destination);
    #endif

    refresh_cmd_timeout();

    #if UBL_SEGMENTED
      // ubl segmented line will do z-only moves in single segment
      ubl.prepare_segmented_line_to(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s));
    #else
      if ( current_position[X_AXIS] == destination[X_AXIS]
        && current_position[Y_AXIS] == destination[Y_AXIS]
        && current_position[Z_AXIS] == destination[Z_AXIS]
        && current_position[E_AXIS] == destination[E_AXIS]
      ) return;

      planner.buffer_line_kinematic(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s), active_extruder);
    #endif

    set_current_from_destination();
  }
#endif // IS_KINEMATIC

/**
 *  Plan a move to (X, Y, Z) and set the current_position
 *  The final current_position may not be the one that was requested
 */
void do_blocking_move_to(const float &rx, const float &ry, const float &rz, const float &fr_mm_s/*=0.0*/) {
  const float old_feedrate_mm_s = feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) print_xyz(PSTR(">>> do_blocking_move_to"), NULL, LOGICAL_X_POSITION(rx), LOGICAL_Y_POSITION(ry), LOGICAL_Z_POSITION(rz));
  #endif

  const float z_feedrate = fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS);

  #if ENABLED(DELTA)

    if (!position_is_reachable(rx, ry)) return;

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;

    set_destination_from_current();          // sync destination at the start

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("set_destination_from_current", destination);
    #endif

    // when in the danger zone
    if (current_position[Z_AXIS] > delta_clip_start_height) {
      if (rz > delta_clip_start_height) {   // staying in the danger zone
        destination[X_AXIS] = rx;           // move directly (uninterpolated)
        destination[Y_AXIS] = ry;
        destination[Z_AXIS] = rz;
        prepare_uninterpolated_move_to_destination(); // set_current_from_destination
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("danger zone move", current_position);
        #endif
        return;
      }
      destination[Z_AXIS] = delta_clip_start_height;
      prepare_uninterpolated_move_to_destination(); // set_current_from_destination
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("zone border move", current_position);
      #endif
    }

    if (rz > current_position[Z_AXIS]) {    // raising?
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);   // set_current_from_destination
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z raise move", current_position);
      #endif
    }

    destination[X_AXIS] = rx;
    destination[Y_AXIS] = ry;
    prepare_move_to_destination();         // set_current_from_destination
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("xy move", current_position);
    #endif

    if (rz < current_position[Z_AXIS]) {    // lowering?
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);   // set_current_from_destination
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z lower move", current_position);
      #endif
    }

  #elif IS_SCARA

    if (!position_is_reachable(rx, ry)) return;

    set_destination_from_current();

    // If Z needs to raise, do it before moving XY
    if (destination[Z_AXIS] < rz) {
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);
    }

    destination[X_AXIS] = rx;
    destination[Y_AXIS] = ry;
    prepare_uninterpolated_move_to_destination(fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S);

    // If Z needs to lower, do it after moving XY
    if (destination[Z_AXIS] > rz) {
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);
    }

  #else

    // If Z needs to raise, do it before moving XY
    if (current_position[Z_AXIS] < rz) {
      feedrate_mm_s = z_feedrate;
      current_position[Z_AXIS] = rz;
      buffer_line_to_current_position();
    }

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;
    current_position[X_AXIS] = rx;
    current_position[Y_AXIS] = ry;
    buffer_line_to_current_position();

    // If Z needs to lower, do it after moving XY
    if (current_position[Z_AXIS] > rz) {
      feedrate_mm_s = z_feedrate;
      current_position[Z_AXIS] = rz;
      buffer_line_to_current_position();
    }

  #endif

  stepper.synchronize();

  feedrate_mm_s = old_feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< do_blocking_move_to");
  #endif
}
void do_blocking_move_to_x(const float &rx, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(rx, current_position[Y_AXIS], current_position[Z_AXIS], fr_mm_s);
}
void do_blocking_move_to_z(const float &rz, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], rz, fr_mm_s);
}
void do_blocking_move_to_xy(const float &rx, const float &ry, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(rx, ry, current_position[Z_AXIS], fr_mm_s);
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
  refresh_cmd_timeout();
}

static void clean_up_after_endstop_or_probe_move() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("clean_up_after_endstop_or_probe_move", current_position);
  #endif
  feedrate_mm_s = saved_feedrate_mm_s;
  feedrate_percentage = saved_feedrate_percentage;
  refresh_cmd_timeout();
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

#if HAS_AXIS_UNHOMED_ERR

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

#endif // HAS_AXIS_UNHOMED_ERR

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

  FORCE_INLINE void do_blocking_move_to(const float (&raw)[XYZ], const float &fr_mm_s) {
    do_blocking_move_to(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS], fr_mm_s);
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

#endif // Z_PROBE_ALLEN_KEY

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
  bool set_probe_deployed(const bool deploy) {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        DEBUG_POS("set_probe_deployed", current_position);
        SERIAL_ECHOLNPAIR("deploy: ", deploy);
      }
    #endif

    if (endstops.z_probe_enabled == deploy) return false;

    // Make room for probe to deploy (or stow)
    // Fix-mounted probe should only raise for deploy
    if (
      #if ENABLED(FIX_MOUNTED_PROBE)
        deploy
      #else
        true
      #endif
    ) do_probe_raise(max(Z_CLEARANCE_BETWEEN_PROBES, Z_CLEARANCE_DEPLOY_PROBE));

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
   * @details Used by probe_pt to do a single Z probe at the current position.
   *          Leaves current_position[Z_AXIS] at the height where the probe triggered.
   *
   * @return The raw Z position where the probe was triggered
   */
  static float run_z_probe() {

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS(">>> run_z_probe", current_position);
    #endif

    // Prevent stepper_inactive_time from running out and EXTRUDER_RUNOUT_PREVENT from extruding
    refresh_cmd_timeout();

    // Double-probing does a fast probe followed by a slow probe
    #if MULTIPLE_PROBING == 2

      // Do a first probe at the fast speed
      if (do_probe_move(-10, Z_PROBE_SPEED_FAST)) return NAN;

      float first_probe_z = current_position[Z_AXIS];

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("1st Probe Z:", first_probe_z);
      #endif

      // move up to make clearance for the probe
      do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));

    #else

      // If the nozzle is well over the travel height then
      // move down quickly before doing the slow probe
      float z = Z_CLEARANCE_DEPLOY_PROBE + 5.0;
      if (zprobe_zoffset < 0) z -= zprobe_zoffset;

      if (current_position[Z_AXIS] > z) {
        // If we don't make it to the z position (i.e. the probe triggered), move up to make clearance for the probe
        if (!do_probe_move(z, Z_PROBE_SPEED_FAST))
          do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      }
    #endif

    #if MULTIPLE_PROBING > 2
      float probes_total = 0;
      for (uint8_t p = MULTIPLE_PROBING + 1; --p;) {
    #endif

        // move down slowly to find bed
        if (do_probe_move(-10, Z_PROBE_SPEED_SLOW)) return NAN;

    #if MULTIPLE_PROBING > 2
        probes_total += current_position[Z_AXIS];
        if (p > 1) do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      }
    #endif

    #if MULTIPLE_PROBING > 2

      // Return the average value of all probes
      return probes_total * (1.0 / (MULTIPLE_PROBING));

    #elif MULTIPLE_PROBING == 2

      const float z2 = current_position[Z_AXIS];

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          SERIAL_ECHOPAIR("2nd Probe Z:", z2);
          SERIAL_ECHOLNPAIR(" Discrepancy:", first_probe_z - z2);
        }
      #endif

      // Return a weighted average of the fast and slow probes
      return (z2 * 3.0 + first_probe_z * 2.0) * 0.2;

    #else

      // Return the single probe result
      return current_position[Z_AXIS];

    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("<<< run_z_probe", current_position);
    #endif
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
  float probe_pt(const float &rx, const float &ry, const bool stow, const uint8_t verbose_level, const bool probe_relative=true) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR(">>> probe_pt(", LOGICAL_X_POSITION(rx));
        SERIAL_ECHOPAIR(", ", LOGICAL_Y_POSITION(ry));
        SERIAL_ECHOPAIR(", ", stow ? "" : "no ");
        SERIAL_ECHOLNPGM("stow)");
        DEBUG_POS("", current_position);
      }
    #endif

    // TODO: Adapt for SCARA, where the offset rotates
    float nx = rx, ny = ry;
    if (probe_relative) {
      if (!position_is_reachable_by_probe(rx, ry)) return NAN;  // The given position is in terms of the probe
      nx -= (X_PROBE_OFFSET_FROM_EXTRUDER);                     // Get the nozzle position
      ny -= (Y_PROBE_OFFSET_FROM_EXTRUDER);
    }
    else if (!position_is_reachable(nx, ny)) return NAN;        // The given position is in terms of the nozzle

    const float nz =
      #if ENABLED(DELTA)
        // Move below clip height or xy move will be aborted by do_blocking_move_to
        min(current_position[Z_AXIS], delta_clip_start_height)
      #else
        current_position[Z_AXIS]
      #endif
    ;

    const float old_feedrate_mm_s = feedrate_mm_s;
    feedrate_mm_s = XY_PROBE_FEEDRATE_MM_S;

    // Move the probe to the starting XYZ
    do_blocking_move_to(nx, ny, nz);

    float measured_z = NAN;
    if (!DEPLOY_PROBE()) {
      measured_z = run_z_probe() + zprobe_zoffset;

      if (!stow)
        do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      else
        if (STOW_PROBE()) measured_z = NAN;
    }

    if (verbose_level > 2) {
      SERIAL_PROTOCOLPGM("Bed X: ");
      SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(rx), 3);
      SERIAL_PROTOCOLPGM(" Y: ");
      SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(ry), 3);
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

    if (can_change && enable != planner.leveling_active) {

      #if ENABLED(MESH_BED_LEVELING)

        if (!enable)
          planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

        const bool enabling = enable && leveling_is_valid();
        planner.leveling_active = enabling;
        if (enabling) planner.unapply_leveling(current_position);

      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        #if PLANNER_LEVELING
          if (planner.leveling_active) {                       // leveling from on to off
            // change unleveled current_position to physical current_position without moving steppers.
            planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);
            planner.leveling_active = false;                   // disable only AFTER calling apply_leveling
          }
          else {                                               // leveling from off to on
            planner.leveling_active = true;                    // enable BEFORE calling unapply_leveling, otherwise ignored
            // change physical current_position to unleveled current_position without moving steppers.
            planner.unapply_leveling(current_position);
          }
        #else
          planner.leveling_active = enable;                    // just flip the bit, current_position will be wrong until next move.
        #endif

      #else // ABL

        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          // Force bilinear_z_offset to re-calculate next time
          const float reset[XYZ] = { -9999.999, -9999.999, 0 };
          (void)bilinear_z_offset(reset);
        #endif

        // Enable or disable leveling compensation in the planner
        planner.leveling_active = enable;

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

        SYNC_PLAN_POSITION_KINEMATIC();

      #endif // ABL
    }
  }

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

    void set_z_fade_height(const float zfh, const bool do_report/*=true*/) {

      if (planner.z_fade_height == zfh) return; // do nothing if no change

      const bool level_active = planner.leveling_active;

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (level_active) set_bed_leveling_enabled(false);  // turn off before changing fade height for proper apply/unapply leveling to maintain current_position
      #endif

      planner.set_z_fade_height(zfh);

      if (level_active) {
        const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          set_bed_leveling_enabled(true);  // turn back on after changing fade height
        #else
          set_current_from_steppers_for_axis(
            #if ABL_PLANAR
              ALL_AXES
            #else
              Z_AXIS
            #endif
          );
          SYNC_PLAN_POSITION_KINEMATIC();
        #endif
        if (do_report && memcmp(oldpos, current_position, sizeof(oldpos)))
          report_current_position();
      }
    }

  #endif // LEVELING_FADE_HEIGHT

  /**
   * Reset calibration results to zero.
   */
  void reset_bed_level() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("reset_bed_level");
    #endif
    set_bed_leveling_enabled(false);
    #if ENABLED(MESH_BED_LEVELING)
      mbl.reset();
    #elif ENABLED(AUTO_BED_LEVELING_UBL)
      ubl.reset();
    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
      bilinear_start[X_AXIS] = bilinear_start[Y_AXIS] =
      bilinear_grid_spacing[X_AXIS] = bilinear_grid_spacing[Y_AXIS] = 0;
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
          z_values[x][y] = NAN;
    #elif ABL_PLANAR
      planner.bed_level_matrix.set_to_identity();
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
  void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, const element_2d_fn fn) {
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

#if ENABLED(SENSORLESS_HOMING)

  /**
   * Set sensorless homing if the axis has it, accounting for Core Kinematics.
   */
  void sensorless_homing_per_axis(const AxisEnum axis, const bool enable=true) {
    switch (axis) {
      #if X_SENSORLESS
        case X_AXIS:
          tmc_sensorless_homing(stepperX, enable);
          #if CORE_IS_XY && Y_SENSORLESS
            tmc_sensorless_homing(stepperY, enable);
          #elif CORE_IS_XZ && Z_SENSORLESS
            tmc_sensorless_homing(stepperZ, enable);
          #endif
          break;
      #endif
      #if Y_SENSORLESS
        case Y_AXIS:
          tmc_sensorless_homing(stepperY, enable);
          #if CORE_IS_XY && X_SENSORLESS
            tmc_sensorless_homing(stepperX, enable);
          #elif CORE_IS_YZ && Z_SENSORLESS
            tmc_sensorless_homing(stepperZ, enable);
          #endif
          break;
      #endif
      #if Z_SENSORLESS
        case Z_AXIS:
          tmc_sensorless_homing(stepperZ, enable);
          #if CORE_IS_XZ && X_SENSORLESS
            tmc_sensorless_homing(stepperX, enable);
          #elif CORE_IS_YZ && Y_SENSORLESS
            tmc_sensorless_homing(stepperY, enable);
          #endif
          break;
      #endif
    }
  }

#endif // SENSORLESS_HOMING

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

  // Disable stealthChop if used. Enable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    sensorless_homing_per_axis(axis);
  #endif

  // Tell the planner the axis is at 0
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

  // Re-enable stealthChop if used. Disable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    sensorless_homing_per_axis(axis, false);
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< do_homing_move(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif
}

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

  // Set flags for X, Y, Z motor locking
  #if ENABLED(X_DUAL_ENDSTOPS)
    if (axis == X_AXIS) stepper.set_homing_flag_x(true);
  #endif
  #if ENABLED(Y_DUAL_ENDSTOPS)
    if (axis == Y_AXIS) stepper.set_homing_flag_y(true);
  #endif
  #if ENABLED(Z_DUAL_ENDSTOPS)
    if (axis == Z_AXIS) stepper.set_homing_flag_z(true);
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

  /**
   * Home axes that have dual endstops... differently
   */
  #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
    const bool pos_dir = axis_home_dir > 0;
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (axis == X_AXIS) {
        const bool lock_x1 = pos_dir ? (x_endstop_adj > 0) : (x_endstop_adj < 0);
        const float adj = FABS(x_endstop_adj);
        if (lock_x1) stepper.set_x_lock(true); else stepper.set_x2_lock(true);
        do_homing_move(axis, pos_dir ? -adj : adj);
        if (lock_x1) stepper.set_x_lock(false); else stepper.set_x2_lock(false);
        stepper.set_homing_flag_x(false);
      }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (axis == Y_AXIS) {
        const bool lock_y1 = pos_dir ? (y_endstop_adj > 0) : (y_endstop_adj < 0);
        const float adj = FABS(y_endstop_adj);
        if (lock_y1) stepper.set_y_lock(true); else stepper.set_y2_lock(true);
        do_homing_move(axis, pos_dir ? -adj : adj);
        if (lock_y1) stepper.set_y_lock(false); else stepper.set_y2_lock(false);
        stepper.set_homing_flag_y(false);
      }
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (axis == Z_AXIS) {
        const bool lock_z1 = pos_dir ? (z_endstop_adj > 0) : (z_endstop_adj < 0);
        const float adj = FABS(z_endstop_adj);
        if (lock_z1) stepper.set_z_lock(true); else stepper.set_z2_lock(true);
        do_homing_move(axis, pos_dir ? -adj : adj);
        if (lock_z1) stepper.set_z_lock(false); else stepper.set_z2_lock(false);
        stepper.set_homing_flag_z(false);
      }
    #endif
  #endif

  #if IS_SCARA

    set_axis_is_at_home(axis);
    SYNC_PLAN_POSITION_KINEMATIC();

  #elif ENABLED(DELTA)

    // Delta has already moved all three towers up in G28
    // so here it re-homes each tower in turn.
    // Delta homing treats the axes as normal linear axes.

    // retrace by the amount specified in delta_endstop_adj + additional 0.1mm in order to have minimum steps
    if (delta_endstop_adj[axis] * Z_HOME_DIR <= 0) {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("delta_endstop_adj:");
      #endif
      do_homing_move(axis, delta_endstop_adj[axis] - 0.1 * Z_HOME_DIR);
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

  // Put away the Z probe
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS && STOW_PROBE()) return;
  #endif

  // Clear retracted status if homing the Z axis
  #if ENABLED(FWRETRACT)
    if (axis == Z_AXIS)
      for (uint8_t i = 0; i < EXTRUDERS; i++) fwretract.retracted[i] = false;
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

/**
 * Set XYZE destination and feedrate from the current GCode command
 *
 *  - Set destination from included axis codes
 *  - Set to current for missing axis codes
 *  - Set the feedrate, if included
 */
void gcode_get_destination() {
  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      const float v = parser.value_axis_units((AxisEnum)i);
      destination[i] = (axis_relative_modes[i] || relative_mode)
        ? current_position[i] + v
        : (i == E_AXIS) ? v : LOGICAL_TO_NATIVE(v, i);
    }
    else
      destination[i] = current_position[i];
  }

  if (parser.linearval('F') > 0.0)
    feedrate_mm_s = MMM_TO_MMS(parser.value_feedrate());

  #if ENABLED(PRINTCOUNTER)
    if (!DEBUGGING(DRYRUN))
      print_job_timer.incFilamentUsed(destination[E_AXIS] - current_position[E_AXIS]);
  #endif

  // Get ABCDHI mixing factors
  #if ENABLED(MIXING_EXTRUDER) && ENABLED(DIRECT_MIXING_IN_G1)
    gcode_get_mix();
  #endif
}

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

#if ENABLED(NO_MOTION_BEFORE_HOMING)
  #define G0_G1_CONDITION !axis_unhomed_error(parser.seen('X'), parser.seen('Y'), parser.seen('Z'))
#else
  #define G0_G1_CONDITION true
#endif

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
inline void gcode_G0_G1(
  #if IS_SCARA
    bool fast_move=false
  #endif
) {
  if (IsRunning() && G0_G1_CONDITION) {
    gcode_get_destination(); // For X Y Z E F

    #if ENABLED(FWRETRACT)
      if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
        // When M209 Autoretract is enabled, convert E-only moves to firmware retract/prime moves
        if (fwretract.autoretract_enabled && parser.seen('E') && !(parser.seen('X') || parser.seen('Y') || parser.seen('Z'))) {
          const float echange = destination[E_AXIS] - current_position[E_AXIS];
          // Is this a retract or prime move?
          if (WITHIN(FABS(echange), MIN_AUTORETRACT, MAX_AUTORETRACT) && fwretract.retracted[active_extruder] == (echange > 0.0)) {
            current_position[E_AXIS] = destination[E_AXIS]; // Hide a G1-based retract/prime from calculations
            sync_plan_position_e();                         // AND from the planner
            return fwretract.retract(echange < 0.0);        // Firmware-based retract/prime (double-retract ignored)
          }
        }
      }
    #endif // FWRETRACT

    #if IS_SCARA
      fast_move ? prepare_uninterpolated_move_to_destination() : prepare_move_to_destination();
    #else
      prepare_move_to_destination();
    #endif

    #if ENABLED(NANODLP_Z_SYNC)
      #if ENABLED(NANODLP_ALL_AXIS)
        #define _MOVE_SYNC true                 // For any move wait and output sync message
      #else
        #define _MOVE_SYNC parser.seenval('Z')  // Only for Z move
      #endif
      if (_MOVE_SYNC) {
        stepper.synchronize();
        SERIAL_ECHOLNPGM(MSG_Z_MOVE_COMP);
      }
    #endif
  }
}

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 *
 * This command has two forms: IJ-form and R-form.
 *
 *  - I specifies an X offset. J specifies a Y offset.
 *    At least one of the IJ parameters is required.
 *    X and Y can be omitted to do a complete circle.
 *    The given XY is not error-checked. The arc ends
 *     based on the angle of the destination.
 *    Mixing I or J with R will throw an error.
 *
 *  - R specifies the radius. X or Y is required.
 *    Omitting both X and Y will throw an error.
 *    X or Y must differ from the current XY.
 *    Mixing R with I or J will throw an error.
 *
 *  - P specifies the number of full circles to do
 *    before the specified arc move.
 *
 *  Examples:
 *
 *    G2 I10           ; CW circle centered at X+10
 *    G3 X20 Y12 R14   ; CCW circle with r=14 ending at X20 Y12
 */
#if ENABLED(ARC_SUPPORT)

  inline void gcode_G2_G3(const bool clockwise) {
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      if (axis_unhomed_error()) return;
    #endif

    if (IsRunning()) {

      #if ENABLED(SF_ARC_FIX)
        const bool relative_mode_backup = relative_mode;
        relative_mode = true;
      #endif

      gcode_get_destination();

      #if ENABLED(SF_ARC_FIX)
        relative_mode = relative_mode_backup;
      #endif

      float arc_offset[2] = { 0.0, 0.0 };
      if (parser.seenval('R')) {
        const float r = parser.value_linear_units(),
                    p1 = current_position[X_AXIS], q1 = current_position[Y_AXIS],
                    p2 = destination[X_AXIS], q2 = destination[Y_AXIS];
        if (r && (p2 != p1 || q2 != q1)) {
          const float e = clockwise ^ (r < 0) ? -1 : 1,           // clockwise -1/1, counterclockwise 1/-1
                      dx = p2 - p1, dy = q2 - q1,                 // X and Y differences
                      d = HYPOT(dx, dy),                          // Linear distance between the points
                      h = SQRT(sq(r) - sq(d * 0.5)),              // Distance to the arc pivot-point
                      mx = (p1 + p2) * 0.5, my = (q1 + q2) * 0.5, // Point between the two points
                      sx = -dy / d, sy = dx / d,                  // Slope of the perpendicular bisector
                      cx = mx + e * h * sx, cy = my + e * h * sy; // Pivot-point of the arc
          arc_offset[0] = cx - p1;
          arc_offset[1] = cy - q1;
        }
      }
      else {
        if (parser.seenval('I')) arc_offset[0] = parser.value_linear_units();
        if (parser.seenval('J')) arc_offset[1] = parser.value_linear_units();
      }

      if (arc_offset[0] || arc_offset[1]) {

        #if ENABLED(ARC_P_CIRCLES)
          // P indicates number of circles to do
          int8_t circles_to_do = parser.byteval('P');
          if (!WITHIN(circles_to_do, 0, 100)) {
            SERIAL_ERROR_START();
            SERIAL_ERRORLNPGM(MSG_ERR_ARC_ARGS);
          }
          while (circles_to_do--)
            plan_arc(current_position, arc_offset, clockwise);
        #endif

        // Send the arc to the planner
        plan_arc(destination, arc_offset, clockwise);
        refresh_cmd_timeout();
      }
      else {
        // Bad arguments
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_ERR_ARC_ARGS);
      }
    }
  }

#endif // ARC_SUPPORT

void dwell(millis_t time) {
  refresh_cmd_timeout();
  time += previous_cmd_ms;
  while (PENDING(millis(), time)) idle();
}

/**
 * G4: Dwell S<seconds> or P<milliseconds>
 */
inline void gcode_G4() {
  millis_t dwell_ms = 0;

  if (parser.seenval('P')) dwell_ms = parser.value_millis(); // milliseconds to wait
  if (parser.seenval('S')) dwell_ms = parser.value_millis_from_seconds(); // seconds to wait

  stepper.synchronize();
  #if ENABLED(NANODLP_Z_SYNC)
    SERIAL_ECHOLNPGM(MSG_Z_MOVE_COMP);
  #endif

  if (!lcd_hasstatus()) LCD_MESSAGEPGM(MSG_DWELL);

  dwell(dwell_ms);
}

#if ENABLED(BEZIER_CURVE_SUPPORT)

  /**
   * Parameters interpreted according to:
   * http://linuxcnc.org/docs/2.6/html/gcode/gcode.html#sec:G5-Cubic-Spline
   * However I, J omission is not supported at this point; all
   * parameters can be omitted and default to zero.
   */

  /**
   * G5: Cubic B-spline
   */
  inline void gcode_G5() {
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      if (axis_unhomed_error()) return;
    #endif

    if (IsRunning()) {

      #if ENABLED(CNC_WORKSPACE_PLANES)
        if (workspace_plane != PLANE_XY) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM(MSG_ERR_BAD_PLANE_MODE);
          return;
        }
      #endif

      gcode_get_destination();

      const float offset[] = {
        parser.linearval('I'),
        parser.linearval('J'),
        parser.linearval('P'),
        parser.linearval('Q')
      };

      plan_cubic_move(offset);
    }
  }

#endif // BEZIER_CURVE_SUPPORT

#if ENABLED(FWRETRACT)

  /**
   * G10 - Retract filament according to settings of M207
   */
  inline void gcode_G10() {
    #if EXTRUDERS > 1
      const bool rs = parser.boolval('S');      
    #endif
    fwretract.retract(true
      #if EXTRUDERS > 1
        , rs
      #endif
    );
  }

  /**
   * G11 - Recover filament according to settings of M208
   */
  inline void gcode_G11() { fwretract.retract(false); }

#endif // FWRETRACT

#if ENABLED(NOZZLE_CLEAN_FEATURE)
  /**
   * G12: Clean the nozzle
   */
  inline void gcode_G12() {
    // Don't allow nozzle cleaning without homing first
    if (axis_unhomed_error()) return;

    const uint8_t pattern = parser.ushortval('P', 0),
                  strokes = parser.ushortval('S', NOZZLE_CLEAN_STROKES),
                  objects = parser.ushortval('T', NOZZLE_CLEAN_TRIANGLES);
    const float radius = parser.floatval('R', NOZZLE_CLEAN_CIRCLE_RADIUS);

    Nozzle::clean(pattern, strokes, radius, objects);
  }
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)

  inline void report_workspace_plane() {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Workspace Plane ");
    serialprintPGM(
      workspace_plane == PLANE_YZ ? PSTR("YZ\n") :
      workspace_plane == PLANE_ZX ? PSTR("ZX\n") :
                                    PSTR("XY\n")
    );
  }

  inline void set_workspace_plane(const WorkspacePlane plane) {
    workspace_plane = plane;
    if (DEBUGGING(INFO)) report_workspace_plane();
  }

  /**
   * G17: Select Plane XY
   * G18: Select Plane ZX
   * G19: Select Plane YZ
   */
  inline void gcode_G17() { set_workspace_plane(PLANE_XY); }
  inline void gcode_G18() { set_workspace_plane(PLANE_ZX); }
  inline void gcode_G19() { set_workspace_plane(PLANE_YZ); }

#endif // CNC_WORKSPACE_PLANES

#if ENABLED(CNC_COORDINATE_SYSTEMS)

  /**
   * Select a coordinate system and update the workspace offset.
   * System index -1 is used to specify machine-native.
   */
  bool select_coordinate_system(const int8_t _new) {
    if (active_coordinate_system == _new) return false;
    float old_offset[XYZ] = { 0 }, new_offset[XYZ] = { 0 };
    if (WITHIN(active_coordinate_system, 0, MAX_COORDINATE_SYSTEMS - 1))
      COPY(old_offset, coordinate_system[active_coordinate_system]);
    if (WITHIN(_new, 0, MAX_COORDINATE_SYSTEMS - 1))
      COPY(new_offset, coordinate_system[_new]);
    active_coordinate_system = _new;
    LOOP_XYZ(i) {
      const float diff = new_offset[i] - old_offset[i];
      if (diff) {
        position_shift[i] += diff;
        update_software_endstops((AxisEnum)i);
      }
    }
    return true;
  }

  /**
   * In CNC G-code G53 is like a modifier
   * It precedes a movement command (or other modifiers) on the same line.
   * This is the first command to use parser.chain() to make this possible.
   */
  inline void gcode_G53() {
    // If this command has more following...
    if (parser.chain()) {
      const int8_t _system = active_coordinate_system;
      active_coordinate_system = -1;
      process_parsed_command();
      active_coordinate_system = _system;
    }
  }

  /**
   * G54-G59.3: Select a new workspace
   *
   * A workspace is an XYZ offset to the machine native space.
   * All workspaces default to 0,0,0 at start, or with EEPROM
   * support they may be restored from a previous session.
   *
   * G92 is used to set the current workspace's offset.
   */
  inline void gcode_G54_59(uint8_t subcode=0) {
    const int8_t _space = parser.codenum - 54 + subcode;
    if (select_coordinate_system(_space)) {
      SERIAL_PROTOCOLLNPAIR("Select workspace ", _space);
      report_current_position();
    }
  }
  FORCE_INLINE void gcode_G54() { gcode_G54_59(); }
  FORCE_INLINE void gcode_G55() { gcode_G54_59(); }
  FORCE_INLINE void gcode_G56() { gcode_G54_59(); }
  FORCE_INLINE void gcode_G57() { gcode_G54_59(); }
  FORCE_INLINE void gcode_G58() { gcode_G54_59(); }
  FORCE_INLINE void gcode_G59() { gcode_G54_59(parser.subcode); }

#endif

#if ENABLED(INCH_MODE_SUPPORT)
  /**
   * G20: Set input mode to inches
   */
  inline void gcode_G20() { parser.set_input_linear_units(LINEARUNIT_INCH); }

  /**
   * G21: Set input mode to millimeters
   */
  inline void gcode_G21() { parser.set_input_linear_units(LINEARUNIT_MM); }
#endif

#if ENABLED(NOZZLE_PARK_FEATURE)
  /**
   * G27: Park the nozzle
   */
  inline void gcode_G27() {
    // Don't allow nozzle parking without homing first
    if (axis_unhomed_error()) return;
    Nozzle::park(parser.ushortval('P'));
  }
#endif // NOZZLE_PARK_FEATURE

#if ENABLED(QUICK_HOME)

  static void quick_home_xy() {

    // Pretend the current position is 0,0
    current_position[X_AXIS] = current_position[Y_AXIS] = 0.0;
    sync_plan_position();

    const int x_axis_home_dir =
      #if ENABLED(DUAL_X_CARRIAGE)
        x_home_dir(active_extruder)
      #else
        home_dir(X_AXIS)
      #endif
    ;

    const float mlx = max_length(X_AXIS),
                mly = max_length(Y_AXIS),
                mlratio = mlx > mly ? mly / mlx : mlx / mly,
                fr_mm_s = min(homing_feedrate(X_AXIS), homing_feedrate(Y_AXIS)) * SQRT(sq(mlratio) + 1.0);

    #if ENABLED(SENSORLESS_HOMING)
      sensorless_homing_per_axis(X_AXIS);
      sensorless_homing_per_axis(Y_AXIS);
    #endif

    do_blocking_move_to_xy(1.5 * mlx * x_axis_home_dir, 1.5 * mly * home_dir(Y_AXIS), fr_mm_s);
    endstops.hit_on_purpose(); // clear endstop hit flags
    current_position[X_AXIS] = current_position[Y_AXIS] = 0.0;

    #if ENABLED(SENSORLESS_HOMING)
      sensorless_homing_per_axis(X_AXIS, false);
      sensorless_homing_per_axis(Y_AXIS, false);
      safe_delay(500); // Short delay needed to settle
    #endif
  }

#endif // QUICK_HOME

#if ENABLED(DEBUG_LEVELING_FEATURE)

  void log_machine_info() {
    SERIAL_ECHOPGM("Machine Type: ");
    #if ENABLED(DELTA)
      SERIAL_ECHOLNPGM("Delta");
    #elif IS_SCARA
      SERIAL_ECHOLNPGM("SCARA");
    #elif IS_CORE
      SERIAL_ECHOLNPGM("Core");
    #else
      SERIAL_ECHOLNPGM("Cartesian");
    #endif

    SERIAL_ECHOPGM("Probe: ");
    #if ENABLED(PROBE_MANUALLY)
      SERIAL_ECHOLNPGM("PROBE_MANUALLY");
    #elif ENABLED(FIX_MOUNTED_PROBE)
      SERIAL_ECHOLNPGM("FIX_MOUNTED_PROBE");
    #elif ENABLED(BLTOUCH)
      SERIAL_ECHOLNPGM("BLTOUCH");
    #elif HAS_Z_SERVO_ENDSTOP
      SERIAL_ECHOLNPGM("SERVO PROBE");
    #elif ENABLED(Z_PROBE_SLED)
      SERIAL_ECHOLNPGM("Z_PROBE_SLED");
    #elif ENABLED(Z_PROBE_ALLEN_KEY)
      SERIAL_ECHOLNPGM("Z_PROBE_ALLEN_KEY");
    #else
      SERIAL_ECHOLNPGM("NONE");
    #endif

    #if HAS_BED_PROBE
      SERIAL_ECHOPAIR("Probe Offset X:", X_PROBE_OFFSET_FROM_EXTRUDER);
      SERIAL_ECHOPAIR(" Y:", Y_PROBE_OFFSET_FROM_EXTRUDER);
      SERIAL_ECHOPAIR(" Z:", zprobe_zoffset);
      #if X_PROBE_OFFSET_FROM_EXTRUDER > 0
        SERIAL_ECHOPGM(" (Right");
      #elif X_PROBE_OFFSET_FROM_EXTRUDER < 0
        SERIAL_ECHOPGM(" (Left");
      #elif Y_PROBE_OFFSET_FROM_EXTRUDER != 0
        SERIAL_ECHOPGM(" (Middle");
      #else
        SERIAL_ECHOPGM(" (Aligned With");
      #endif
      #if Y_PROBE_OFFSET_FROM_EXTRUDER > 0
        SERIAL_ECHOPGM("-Back");
      #elif Y_PROBE_OFFSET_FROM_EXTRUDER < 0
        SERIAL_ECHOPGM("-Front");
      #elif X_PROBE_OFFSET_FROM_EXTRUDER != 0
        SERIAL_ECHOPGM("-Center");
      #endif
      if (zprobe_zoffset < 0)
        SERIAL_ECHOPGM(" & Below");
      else if (zprobe_zoffset > 0)
        SERIAL_ECHOPGM(" & Above");
      else
        SERIAL_ECHOPGM(" & Same Z as");
      SERIAL_ECHOLNPGM(" Nozzle)");
    #endif

    #if HAS_ABL
      SERIAL_ECHOPGM("Auto Bed Leveling: ");
      #if ENABLED(AUTO_BED_LEVELING_LINEAR)
        SERIAL_ECHOPGM("LINEAR");
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
        SERIAL_ECHOPGM("BILINEAR");
      #elif ENABLED(AUTO_BED_LEVELING_3POINT)
        SERIAL_ECHOPGM("3POINT");
      #elif ENABLED(AUTO_BED_LEVELING_UBL)
        SERIAL_ECHOPGM("UBL");
      #endif
      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height)
            SERIAL_ECHOLNPAIR("Z Fade: ", planner.z_fade_height);
        #endif
        #if ABL_PLANAR
          const float diff[XYZ] = {
            stepper.get_axis_position_mm(X_AXIS) - current_position[X_AXIS],
            stepper.get_axis_position_mm(Y_AXIS) - current_position[Y_AXIS],
            stepper.get_axis_position_mm(Z_AXIS) - current_position[Z_AXIS]
          };
          SERIAL_ECHOPGM("ABL Adjustment X");
          if (diff[X_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[X_AXIS]);
          SERIAL_ECHOPGM(" Y");
          if (diff[Y_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[Y_AXIS]);
          SERIAL_ECHOPGM(" Z");
          if (diff[Z_AXIS] > 0) SERIAL_CHAR('+');
          SERIAL_ECHO(diff[Z_AXIS]);
        #else
          #if ENABLED(AUTO_BED_LEVELING_UBL)
            SERIAL_ECHOPGM("UBL Adjustment Z");
            const float rz = ubl.get_z_correction(current_position[X_AXIS], current_position[Y_AXIS]);
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            SERIAL_ECHOPAIR("Bilinear Grid X", bilinear_start[X_AXIS]);
            SERIAL_ECHOPAIR(" Y", bilinear_start[Y_AXIS]);
            SERIAL_ECHOPAIR(" W", ABL_BG_SPACING(X_AXIS));
            SERIAL_ECHOLNPAIR(" H", ABL_BG_SPACING(Y_AXIS));
            SERIAL_ECHOPGM("ABL Adjustment Z");
            const float rz = bilinear_z_offset(current_position);
          #endif
          SERIAL_ECHO(ftostr43sign(rz, '+'));
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            if (planner.z_fade_height) {
              SERIAL_ECHOPAIR(" (", ftostr43sign(rz * planner.fade_scaling_factor_for_z(current_position[Z_AXIS]), '+'));
              SERIAL_CHAR(')');
            }
          #endif
        #endif
      }
      else
        SERIAL_ECHOLNPGM(" (disabled)");

      SERIAL_EOL();

    #elif ENABLED(MESH_BED_LEVELING)

      SERIAL_ECHOPGM("Mesh Bed Leveling");
      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        SERIAL_ECHOPAIR("MBL Adjustment Z", ftostr43sign(mbl.get_z(current_position[X_AXIS], current_position[Y_AXIS], 1.0), '+'));
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height) {
            SERIAL_ECHOPAIR(" (", ftostr43sign(
              mbl.get_z(current_position[X_AXIS], current_position[Y_AXIS], planner.fade_scaling_factor_for_z(current_position[Z_AXIS])), '+'
            ));
            SERIAL_CHAR(')');
          }
        #endif
      }
      else
        SERIAL_ECHOPGM(" (disabled)");

      SERIAL_EOL();

    #endif // MESH_BED_LEVELING
  }

#endif // DEBUG_LEVELING_FEATURE

#if ENABLED(DELTA)

  /**
   * A delta can only safely home all axes at the same time
   * This is like quick_home_xy() but for 3 towers.
   */
  inline bool home_delta() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS(">>> home_delta", current_position);
    #endif
    // Init the current position of all carriages to 0,0,0
    ZERO(current_position);
    sync_plan_position();

    // Disable stealthChop if used. Enable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      sensorless_homing_per_axis(A_AXIS);
      sensorless_homing_per_axis(B_AXIS);
      sensorless_homing_per_axis(C_AXIS);
    #endif

    // Move all carriages together linearly until an endstop is hit.
    current_position[X_AXIS] = current_position[Y_AXIS] = current_position[Z_AXIS] = (delta_height + 10);
    feedrate_mm_s = homing_feedrate(X_AXIS);
    buffer_line_to_current_position();
    stepper.synchronize();

    // Re-enable stealthChop if used. Disable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      sensorless_homing_per_axis(A_AXIS, false);
      sensorless_homing_per_axis(B_AXIS, false);
      sensorless_homing_per_axis(C_AXIS, false);
    #endif

    // If an endstop was not hit, then damage can occur if homing is continued.
    // This can occur if the delta height not set correctly.
    if (!(Endstops::endstop_hit_bits & (_BV(X_MAX) | _BV(Y_MAX) | _BV(Z_MAX)))) {
      LCD_MESSAGEPGM(MSG_ERR_HOMING_FAILED);
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_HOMING_FAILED);
      return false;
    }

    endstops.hit_on_purpose(); // clear endstop hit flags

    // At least one carriage has reached the top.
    // Now re-home each carriage separately.
    HOMEAXIS(A);
    HOMEAXIS(B);
    HOMEAXIS(C);

    // Set all carriages to their home positions
    // Do this here all at once for Delta, because
    // XYZ isn't ABC. Applying this per-tower would
    // give the impression that they are the same.
    LOOP_XYZ(i) set_axis_is_at_home((AxisEnum)i);

    SYNC_PLAN_POSITION_KINEMATIC();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("<<< home_delta", current_position);
    #endif

    return true;
  }

#endif // DELTA

#if ENABLED(Z_SAFE_HOMING)

  inline void home_z_safely() {

    // Disallow Z homing if X or Y are unknown
    if (!axis_known_position[X_AXIS] || !axis_known_position[Y_AXIS]) {
      LCD_MESSAGEPGM(MSG_ERR_Z_HOMING);
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM(MSG_ERR_Z_HOMING);
      return;
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Z_SAFE_HOMING >>>");
    #endif

    SYNC_PLAN_POSITION_KINEMATIC();

    /**
     * Move the Z probe (or just the nozzle) to the safe homing point
     */
    destination[X_AXIS] = Z_SAFE_HOMING_X_POINT;
    destination[Y_AXIS] = Z_SAFE_HOMING_Y_POINT;
    destination[Z_AXIS] = current_position[Z_AXIS]; // Z is already at the right height

    #if HOMING_Z_WITH_PROBE
      destination[X_AXIS] -= X_PROBE_OFFSET_FROM_EXTRUDER;
      destination[Y_AXIS] -= Y_PROBE_OFFSET_FROM_EXTRUDER;
    #endif

    if (position_is_reachable(destination[X_AXIS], destination[Y_AXIS])) {

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("Z_SAFE_HOMING", destination);
      #endif

      // This causes the carriage on Dual X to unpark
      #if ENABLED(DUAL_X_CARRIAGE)
        active_extruder_parked = false;
      #endif

      #if ENABLED(SENSORLESS_HOMING)
        safe_delay(500); // Short delay needed to settle
      #endif

      do_blocking_move_to_xy(destination[X_AXIS], destination[Y_AXIS]);
      HOMEAXIS(Z);
    }
    else {
      LCD_MESSAGEPGM(MSG_ZPROBE_OUT);
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM(MSG_ZPROBE_OUT);
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< Z_SAFE_HOMING");
    #endif
  }

#endif // Z_SAFE_HOMING

#if ENABLED(PROBE_MANUALLY)
  bool g29_in_progress = false;
#else
  constexpr bool g29_in_progress = false;
#endif

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
inline void gcode_G28(const bool always_home_all) {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOLNPGM(">>> gcode_G28");
      log_machine_info();
    }
  #endif

  // Wait for planner moves to finish!
  stepper.synchronize();

  // Cancel the active G29 session
  #if ENABLED(PROBE_MANUALLY)
    g29_in_progress = false;
  #endif

  // Disable the leveling matrix before homing
  #if HAS_LEVELING
    #if ENABLED(RESTORE_LEVELING_AFTER_G28)
      const bool leveling_state_at_entry = planner.leveling_active;
    #endif
    set_bed_leveling_enabled(false);
  #endif

  #if ENABLED(CNC_WORKSPACE_PLANES)
    workspace_plane = PLANE_XY;
  #endif

  // Always home with tool 0 active
  #if HOTENDS > 1
    const uint8_t old_tool_index = active_extruder;
    tool_change(0, 0, true);
  #endif

  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
    extruder_duplication_enabled = false;
  #endif

  setup_for_endstop_or_probe_move();
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("> endstops.enable(true)");
  #endif
  endstops.enable(true); // Enable endstops for next homing move

  #if ENABLED(DELTA)

    home_delta();
    UNUSED(always_home_all);

  #else // NOT DELTA

    const bool homeX = always_home_all || parser.seen('X'),
               homeY = always_home_all || parser.seen('Y'),
               homeZ = always_home_all || parser.seen('Z'),
               home_all = (!homeX && !homeY && !homeZ) || (homeX && homeY && homeZ);

    set_destination_from_current();

    #if Z_HOME_DIR > 0  // If homing away from BED do Z first

      if (home_all || homeZ) HOMEAXIS(Z);

    #endif

    if (home_all || homeX || homeY) {
      // Raise Z before homing any other axes and z is not already high enough (never lower z)
      destination[Z_AXIS] = Z_HOMING_HEIGHT;
      if (destination[Z_AXIS] > current_position[Z_AXIS]) {

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING))
            SERIAL_ECHOLNPAIR("Raise Z (before homing) to ", destination[Z_AXIS]);
        #endif

        do_blocking_move_to_z(destination[Z_AXIS]);
      }
    }

    #if ENABLED(QUICK_HOME)

      if (home_all || (homeX && homeY)) quick_home_xy();

    #endif

    // Home Y (before X)
    #if ENABLED(HOME_Y_BEFORE_X)

      if (home_all || homeY
        #if ENABLED(CODEPENDENT_XY_HOMING)
          || homeX
        #endif
      ) HOMEAXIS(Y);

    #endif

    // Home X
    if (home_all || homeX
      #if ENABLED(CODEPENDENT_XY_HOMING) && DISABLED(HOME_Y_BEFORE_X)
        || homeY
      #endif
    ) {

      #if ENABLED(DUAL_X_CARRIAGE)

        // Always home the 2nd (right) extruder first
        active_extruder = 1;
        HOMEAXIS(X);

        // Remember this extruder's position for later tool change
        inactive_extruder_x_pos = current_position[X_AXIS];

        // Home the 1st (left) extruder
        active_extruder = 0;
        HOMEAXIS(X);

        // Consider the active extruder to be parked
        COPY(raised_parked_position, current_position);
        delayed_move_time = 0;
        active_extruder_parked = true;

      #else

        HOMEAXIS(X);

      #endif
    }

    // Home Y (after X)
    #if DISABLED(HOME_Y_BEFORE_X)
      if (home_all || homeY) HOMEAXIS(Y);
    #endif

    // Home Z last if homing towards the bed
    #if Z_HOME_DIR < 0
      if (home_all || homeZ) {
        #if ENABLED(Z_SAFE_HOMING)
          home_z_safely();
        #else
          HOMEAXIS(Z);
        #endif
      } // home_all || homeZ
    #endif // Z_HOME_DIR < 0

    SYNC_PLAN_POSITION_KINEMATIC();

  #endif // !DELTA (gcode_G28)

  endstops.not_homing();

  #if ENABLED(DELTA) && ENABLED(DELTA_HOME_TO_SAFE_ZONE)
    // move to a height where we can use the full xy-area
    do_blocking_move_to_z(delta_clip_start_height);
  #endif

  #if ENABLED(RESTORE_LEVELING_AFTER_G28)
    set_bed_leveling_enabled(leveling_state_at_entry);
  #endif

  clean_up_after_endstop_or_probe_move();

  // Restore the active tool after homing
  #if HOTENDS > 1
    #if ENABLED(PARKING_EXTRUDER)
      #define NO_FETCH false // fetch the previous toolhead
    #else
      #define NO_FETCH true
    #endif
    tool_change(old_tool_index, 0, NO_FETCH);
  #endif

  lcd_refresh();

  report_current_position();

  #if ENABLED(NANODLP_Z_SYNC)
    #if ENABLED(NANODLP_ALL_AXIS)
      #define _HOME_SYNC true                 // For any axis, output sync text.
    #else
      #define _HOME_SYNC (home_all || homeZ)  // Only for Z-axis
    #endif
    if (_HOME_SYNC)
      SERIAL_ECHOLNPGM(MSG_Z_MOVE_COMP);
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< gcode_G28");
  #endif
} // G28

void home_all_axes() { gcode_G28(true); }

#if HAS_PROBING_PROCEDURE

  void out_of_range_error(const char* p_edge) {
    SERIAL_PROTOCOLPGM("?Probe ");
    serialprintPGM(p_edge);
    SERIAL_PROTOCOLLNPGM(" position out of range.");
  }

#endif

#if ENABLED(MESH_BED_LEVELING) || ENABLED(PROBE_MANUALLY)

  inline void _manual_goto_xy(const float &rx, const float &ry) {

    #if MANUAL_PROBE_HEIGHT > 0
      const float prev_z = current_position[Z_AXIS];
      do_blocking_move_to(rx, ry, MANUAL_PROBE_HEIGHT);
      do_blocking_move_to_z(prev_z);
    #else
      do_blocking_move_to_xy(rx, ry);
    #endif

    current_position[X_AXIS] = rx;
    current_position[Y_AXIS] = ry;

    #if ENABLED(LCD_BED_LEVELING)
      lcd_wait_for_move = false;
    #endif
  }

#endif

#if ENABLED(MESH_BED_LEVELING)

  // Save 130 bytes with non-duplication of PSTR
  void echo_not_entered() { SERIAL_PROTOCOLLNPGM(" not entered."); }

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
   *  S5              Reset and disable mesh
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

    static int mbl_probe_index = -1;
    #if HAS_SOFTWARE_ENDSTOPS
      static bool enable_soft_endstops;
    #endif

    const MeshLevelingState state = (MeshLevelingState)parser.byteval('S', (int8_t)MeshReport);
    if (!WITHIN(state, 0, 5)) {
      SERIAL_PROTOCOLLNPGM("S out of range (0-5).");
      return;
    }

    int8_t px, py;

    switch (state) {
      case MeshReport:
        if (leveling_is_valid()) {
          SERIAL_PROTOCOLLNPAIR("State: ", planner.leveling_active ? MSG_ON : MSG_OFF);
          mbl.report_mesh();
        }
        else
          SERIAL_PROTOCOLLNPGM("Mesh bed leveling has no data.");
        break;

      case MeshStart:
        mbl.reset();
        mbl_probe_index = 0;
        enqueue_and_echo_commands_P(lcd_wait_for_move ? PSTR("G29 S2") : PSTR("G28\nG29 S2"));
        break;

      case MeshNext:
        if (mbl_probe_index < 0) {
          SERIAL_PROTOCOLLNPGM("Start mesh probing with \"G29 S1\" first.");
          return;
        }
        // For each G29 S2...
        if (mbl_probe_index == 0) {
          #if HAS_SOFTWARE_ENDSTOPS
            // For the initial G29 S2 save software endstop state
            enable_soft_endstops = soft_endstops_enabled;
          #endif
        }
        else {
          // For G29 S2 after adjusting Z.
          mbl.set_zigzag_z(mbl_probe_index - 1, current_position[Z_AXIS]);
          #if HAS_SOFTWARE_ENDSTOPS
            soft_endstops_enabled = enable_soft_endstops;
          #endif
        }
        // If there's another point to sample, move there with optional lift.
        if (mbl_probe_index < GRID_MAX_POINTS) {
          mbl.zigzag(mbl_probe_index, px, py);
          _manual_goto_xy(mbl.index_to_xpos[px], mbl.index_to_ypos[py]);

          #if HAS_SOFTWARE_ENDSTOPS
            // Disable software endstops to allow manual adjustment
            // If G29 is not completed, they will not be re-enabled
            soft_endstops_enabled = false;
          #endif

          mbl_probe_index++;
        }
        else {
          // One last "return to the bed" (as originally coded) at completion
          current_position[Z_AXIS] = Z_MIN_POS + MANUAL_PROBE_HEIGHT;
          buffer_line_to_current_position();
          stepper.synchronize();

          // After recording the last point, activate home and activate
          mbl_probe_index = -1;
          SERIAL_PROTOCOLLNPGM("Mesh probing done.");
          BUZZ(100, 659);
          BUZZ(100, 698);

          home_all_axes();
          set_bed_leveling_enabled(true);

          #if ENABLED(MESH_G28_REST_ORIGIN)
            current_position[Z_AXIS] = Z_MIN_POS;
            set_destination_from_current();
            buffer_line_to_destination(homing_feedrate(Z_AXIS));
            stepper.synchronize();
          #endif

          #if ENABLED(LCD_BED_LEVELING)
            lcd_wait_for_move = false;
          #endif
        }
        break;

      case MeshSet:
        if (parser.seenval('X')) {
          px = parser.value_int() - 1;
          if (!WITHIN(px, 0, GRID_MAX_POINTS_X - 1)) {
            SERIAL_PROTOCOLLNPGM("X out of range (1-" STRINGIFY(GRID_MAX_POINTS_X) ").");
            return;
          }
        }
        else {
          SERIAL_CHAR('X'); echo_not_entered();
          return;
        }

        if (parser.seenval('Y')) {
          py = parser.value_int() - 1;
          if (!WITHIN(py, 0, GRID_MAX_POINTS_Y - 1)) {
            SERIAL_PROTOCOLLNPGM("Y out of range (1-" STRINGIFY(GRID_MAX_POINTS_Y) ").");
            return;
          }
        }
        else {
          SERIAL_CHAR('Y'); echo_not_entered();
          return;
        }

        if (parser.seenval('Z'))
          mbl.z_values[px][py] = parser.value_linear_units();
        else {
          SERIAL_CHAR('Z'); echo_not_entered();
          return;
        }
        break;

      case MeshSetZOffset:
        if (parser.seenval('Z'))
          mbl.z_offset = parser.value_linear_units();
        else {
          SERIAL_CHAR('Z'); echo_not_entered();
          return;
        }
        break;

      case MeshReset:
        reset_bed_level();
        break;

    } // switch(state)

    if (state == MeshStart || state == MeshNext) {
      SERIAL_PROTOCOLPAIR("MBL G29 point ", min(mbl_probe_index, GRID_MAX_POINTS));
      SERIAL_PROTOCOLLNPAIR(" of ", int(GRID_MAX_POINTS));
    }

    report_current_position();
  }

#elif OLDSCHOOL_ABL

  #if ABL_GRID
    #if ENABLED(PROBE_Y_FIRST)
      #define PR_OUTER_VAR xCount
      #define PR_OUTER_END abl_grid_points_x
      #define PR_INNER_VAR yCount
      #define PR_INNER_END abl_grid_points_y
    #else
      #define PR_OUTER_VAR yCount
      #define PR_OUTER_END abl_grid_points_y
      #define PR_INNER_VAR xCount
      #define PR_INNER_END abl_grid_points_x
    #endif
  #endif

  /**
   * G29: Detailed Z probe, probes the bed at 3 or more points.
   *      Will fail if the printer has not been homed with G28.
   *
   * Enhanced G29 Auto Bed Leveling Probe Routine
   *
   *  D  Dry-Run mode. Just evaluate the bed Topology - Don't apply
   *     or alter the bed level data. Useful to check the topology
   *     after a first run of G29.
   *
   *  J  Jettison current bed leveling data
   *
   *  V  Set the verbose level (0-4). Example: "G29 V3"
   *
   * Parameters With LINEAR leveling only:
   *
   *  P  Set the size of the grid that will be probed (P x P points).
   *     Example: "G29 P4"
   *
   *  X  Set the X size of the grid that will be probed (X x Y points).
   *     Example: "G29 X7 Y5"
   *
   *  Y  Set the Y size of the grid that will be probed (X x Y points).
   *
   *  T  Generate a Bed Topology Report. Example: "G29 P5 T" for a detailed report.
   *     This is useful for manual bed leveling and finding flaws in the bed (to
   *     assist with part placement).
   *     Not supported by non-linear delta printer bed leveling.
   *
   * Parameters With LINEAR and BILINEAR leveling only:
   *
   *  S  Set the XY travel speed between probe points (in units/min)
   *
   *  F  Set the Front limit of the probing grid
   *  B  Set the Back limit of the probing grid
   *  L  Set the Left limit of the probing grid
   *  R  Set the Right limit of the probing grid
   *
   * Parameters with DEBUG_LEVELING_FEATURE only:
   *
   *  C  Make a totally fake grid with no actual probing.
   *     For use in testing when no probing is possible.
   *
   * Parameters with BILINEAR leveling only:
   *
   *  Z  Supply an additional Z probe offset
   *
   * Extra parameters with PROBE_MANUALLY:
   *
   *  To do manual probing simply repeat G29 until the procedure is complete.
   *  The first G29 accepts parameters. 'G29 Q' for status, 'G29 A' to abort.
   *
   *  Q  Query leveling and G29 state
   *
   *  A  Abort current leveling procedure
   *
   * Extra parameters with BILINEAR only:
   *
   *  W  Write a mesh point. (If G29 is idle.)
   *  I  X index for mesh point
   *  J  Y index for mesh point
   *  X  X for mesh point, overrides I
   *  Y  Y for mesh point, overrides J
   *  Z  Z for mesh point. Otherwise, raw current Z.
   *
   * Without PROBE_MANUALLY:
   *
   *  E  By default G29 will engage the Z probe, test the bed, then disengage.
   *     Include "E" to engage/disengage the Z probe for each sample.
   *     There's no extra effect if you have a fixed Z probe.
   *
   */
  inline void gcode_G29() {

    #if ENABLED(DEBUG_LEVELING_FEATURE) || ENABLED(PROBE_MANUALLY)
      const bool seenQ = parser.seen('Q');
    #else
      constexpr bool seenQ = false;
    #endif

    // G29 Q is also available if debugging
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      const uint8_t old_debug_flags = marlin_debug_flags;
      if (seenQ) marlin_debug_flags |= DEBUG_LEVELING;
      if (DEBUGGING(LEVELING)) {
        DEBUG_POS(">>> G29", current_position);
        log_machine_info();
      }
      marlin_debug_flags = old_debug_flags;
      #if DISABLED(PROBE_MANUALLY)
        if (seenQ) return;
      #endif
    #endif

    #if ENABLED(PROBE_MANUALLY)
      const bool seenA = parser.seen('A');
    #else
      constexpr bool seenA = false;
    #endif

    const bool  no_action = seenA || seenQ,
                faux =
                  #if ENABLED(DEBUG_LEVELING_FEATURE) && DISABLED(PROBE_MANUALLY)
                    parser.boolval('C')
                  #else
                    no_action
                  #endif
                ;

    // Don't allow auto-leveling without homing first
    if (axis_unhomed_error()) return;

    // Define local vars 'static' for manual probing, 'auto' otherwise
    #if ENABLED(PROBE_MANUALLY)
      #define ABL_VAR static
    #else
      #define ABL_VAR
    #endif

    ABL_VAR int verbose_level;
    ABL_VAR float xProbe, yProbe, measured_z;
    ABL_VAR bool dryrun, abl_should_enable;

    #if ENABLED(PROBE_MANUALLY) || ENABLED(AUTO_BED_LEVELING_LINEAR)
      ABL_VAR int abl_probe_index;
    #endif

    #if HAS_SOFTWARE_ENDSTOPS && ENABLED(PROBE_MANUALLY)
      ABL_VAR bool enable_soft_endstops = true;
    #endif

    #if ABL_GRID

      #if ENABLED(PROBE_MANUALLY)
        ABL_VAR uint8_t PR_OUTER_VAR;
        ABL_VAR  int8_t PR_INNER_VAR;
      #endif

      ABL_VAR int left_probe_bed_position, right_probe_bed_position, front_probe_bed_position, back_probe_bed_position;
      ABL_VAR float xGridSpacing = 0, yGridSpacing = 0;

      #if ENABLED(AUTO_BED_LEVELING_LINEAR)
        ABL_VAR uint8_t abl_grid_points_x = GRID_MAX_POINTS_X,
                        abl_grid_points_y = GRID_MAX_POINTS_Y;
        ABL_VAR bool do_topography_map;
      #else // Bilinear
        uint8_t constexpr abl_grid_points_x = GRID_MAX_POINTS_X,
                          abl_grid_points_y = GRID_MAX_POINTS_Y;
      #endif

      #if ENABLED(AUTO_BED_LEVELING_LINEAR)
        ABL_VAR int abl2;
      #elif ENABLED(PROBE_MANUALLY) // Bilinear
        int constexpr abl2 = GRID_MAX_POINTS;
      #endif

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        ABL_VAR float zoffset;

      #elif ENABLED(AUTO_BED_LEVELING_LINEAR)

        ABL_VAR int indexIntoAB[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

        ABL_VAR float eqnAMatrix[GRID_MAX_POINTS * 3], // "A" matrix of the linear system of equations
                      eqnBVector[GRID_MAX_POINTS],     // "B" vector of Z points
                      mean;
      #endif

    #elif ENABLED(AUTO_BED_LEVELING_3POINT)

      #if ENABLED(PROBE_MANUALLY)
        int constexpr abl2 = 3; // used to show total points
      #endif

      // Probe at 3 arbitrary points
      ABL_VAR vector_3 points[3] = {
        vector_3(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, 0),
        vector_3(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, 0),
        vector_3(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, 0)
      };

    #endif // AUTO_BED_LEVELING_3POINT

    #if ENABLED(AUTO_BED_LEVELING_LINEAR)
      struct linear_fit_data lsf_results;
      incremental_LSF_reset(&lsf_results);
    #endif

    /**
     * On the initial G29 fetch command parameters.
     */
    if (!g29_in_progress) {

      #if ENABLED(PROBE_MANUALLY) || ENABLED(AUTO_BED_LEVELING_LINEAR)
        abl_probe_index = -1;
      #endif

      abl_should_enable = planner.leveling_active;

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        const bool seen_w = parser.seen('W');
        if (seen_w) {
          if (!leveling_is_valid()) {
            SERIAL_ERROR_START();
            SERIAL_ERRORLNPGM("No bilinear grid");
            return;
          }

          const float rz = parser.seenval('Z') ? RAW_Z_POSITION(parser.value_linear_units()) : current_position[Z_AXIS];
          if (!WITHIN(rz, -10, 10)) {
            SERIAL_ERROR_START();
            SERIAL_ERRORLNPGM("Bad Z value");
            return;
          }

          const float rx = RAW_X_POSITION(parser.linearval('X', NAN)),
                      ry = RAW_Y_POSITION(parser.linearval('Y', NAN));
          int8_t i = parser.byteval('I', -1),
                 j = parser.byteval('J', -1);

          if (!isnan(rx) && !isnan(ry)) {
            // Get nearest i / j from rx / ry
            i = (rx - bilinear_start[X_AXIS] + 0.5 * xGridSpacing) / xGridSpacing;
            j = (ry - bilinear_start[Y_AXIS] + 0.5 * yGridSpacing) / yGridSpacing;
            i = constrain(i, 0, GRID_MAX_POINTS_X - 1);
            j = constrain(j, 0, GRID_MAX_POINTS_Y - 1);
          }
          if (WITHIN(i, 0, GRID_MAX_POINTS_X - 1) && WITHIN(j, 0, GRID_MAX_POINTS_Y)) {
            set_bed_leveling_enabled(false);
            z_values[i][j] = rz;
            #if ENABLED(ABL_BILINEAR_SUBDIVISION)
              bed_level_virt_interpolate();
            #endif
            set_bed_leveling_enabled(abl_should_enable);
            if (abl_should_enable) report_current_position();
          }
          return;
        } // parser.seen('W')

      #else

        constexpr bool seen_w = false;

      #endif

      // Jettison bed leveling data
      if (!seen_w && parser.seen('J')) {
        reset_bed_level();
        return;
      }

      verbose_level = parser.intval('V');
      if (!WITHIN(verbose_level, 0, 4)) {
        SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).");
        return;
      }

      dryrun = parser.boolval('D')
        #if ENABLED(PROBE_MANUALLY)
          || no_action
        #endif
      ;

      #if ENABLED(AUTO_BED_LEVELING_LINEAR)

        do_topography_map = verbose_level > 2 || parser.boolval('T');

        // X and Y specify points in each direction, overriding the default
        // These values may be saved with the completed mesh
        abl_grid_points_x = parser.intval('X', GRID_MAX_POINTS_X);
        abl_grid_points_y = parser.intval('Y', GRID_MAX_POINTS_Y);
        if (parser.seenval('P')) abl_grid_points_x = abl_grid_points_y = parser.value_int();

        if (!WITHIN(abl_grid_points_x, 2, GRID_MAX_POINTS_X)) {
          SERIAL_PROTOCOLLNPGM("?Probe points (X) is implausible (2-" STRINGIFY(GRID_MAX_POINTS_X) ").");
          return;
        }
        if (!WITHIN(abl_grid_points_y, 2, GRID_MAX_POINTS_Y)) {
          SERIAL_PROTOCOLLNPGM("?Probe points (Y) is implausible (2-" STRINGIFY(GRID_MAX_POINTS_Y) ").");
          return;
        }

        abl2 = abl_grid_points_x * abl_grid_points_y;
        mean = 0;

      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        zoffset = parser.linearval('Z');

      #endif

      #if ABL_GRID

        xy_probe_feedrate_mm_s = MMM_TO_MMS(parser.linearval('S', XY_PROBE_SPEED));

        left_probe_bed_position  = parser.seenval('L') ? (int)RAW_X_POSITION(parser.value_linear_units()) : LEFT_PROBE_BED_POSITION;
        right_probe_bed_position = parser.seenval('R') ? (int)RAW_X_POSITION(parser.value_linear_units()) : RIGHT_PROBE_BED_POSITION;
        front_probe_bed_position = parser.seenval('F') ? (int)RAW_Y_POSITION(parser.value_linear_units()) : FRONT_PROBE_BED_POSITION;
        back_probe_bed_position  = parser.seenval('B') ? (int)RAW_Y_POSITION(parser.value_linear_units()) : BACK_PROBE_BED_POSITION;

        const bool left_out_l = left_probe_bed_position < MIN_PROBE_X,
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

        // probe at the points of a lattice grid
        xGridSpacing = (right_probe_bed_position - left_probe_bed_position) / (abl_grid_points_x - 1);
        yGridSpacing = (back_probe_bed_position - front_probe_bed_position) / (abl_grid_points_y - 1);

      #endif // ABL_GRID

      if (verbose_level > 0) {
        SERIAL_PROTOCOLPGM("G29 Auto Bed Leveling");
        if (dryrun) SERIAL_PROTOCOLPGM(" (DRYRUN)");
        SERIAL_EOL();
      }

      stepper.synchronize();

      // Disable auto bed leveling during G29.
      // Be formal so G29 can be done successively without G28.
      if (!no_action) set_bed_leveling_enabled(false);

      #if HAS_BED_PROBE
        // Deploy the probe. Probe will raise if needed.
        if (DEPLOY_PROBE()) {
          set_bed_leveling_enabled(abl_should_enable);
          return;
        }
      #endif

      if (!faux) setup_for_endstop_or_probe_move();

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        #if ENABLED(PROBE_MANUALLY)
          if (!no_action)
        #endif
        if ( xGridSpacing != bilinear_grid_spacing[X_AXIS]
          || yGridSpacing != bilinear_grid_spacing[Y_AXIS]
          || left_probe_bed_position != bilinear_start[X_AXIS]
          || front_probe_bed_position != bilinear_start[Y_AXIS]
        ) {
          // Reset grid to 0.0 or "not probed". (Also disables ABL)
          reset_bed_level();

          // Initialize a grid with the given dimensions
          bilinear_grid_spacing[X_AXIS] = xGridSpacing;
          bilinear_grid_spacing[Y_AXIS] = yGridSpacing;
          bilinear_start[X_AXIS] = left_probe_bed_position;
          bilinear_start[Y_AXIS] = front_probe_bed_position;

          // Can't re-enable (on error) until the new grid is written
          abl_should_enable = false;
        }

      #endif // AUTO_BED_LEVELING_BILINEAR

      #if ENABLED(AUTO_BED_LEVELING_3POINT)

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("> 3-point Leveling");
        #endif

        // Probe at 3 arbitrary points
        points[0].z = points[1].z = points[2].z = 0;

      #endif // AUTO_BED_LEVELING_3POINT

    } // !g29_in_progress

    #if ENABLED(PROBE_MANUALLY)

      // For manual probing, get the next index to probe now.
      // On the first probe this will be incremented to 0.
      if (!no_action) {
        ++abl_probe_index;
        g29_in_progress = true;
      }

      // Abort current G29 procedure, go back to idle state
      if (seenA && g29_in_progress) {
        SERIAL_PROTOCOLLNPGM("Manual G29 aborted");
        #if HAS_SOFTWARE_ENDSTOPS
          soft_endstops_enabled = enable_soft_endstops;
        #endif
        set_bed_leveling_enabled(abl_should_enable);
        g29_in_progress = false;
        #if ENABLED(LCD_BED_LEVELING)
          lcd_wait_for_move = false;
        #endif
      }

      // Query G29 status
      if (verbose_level || seenQ) {
        SERIAL_PROTOCOLPGM("Manual G29 ");
        if (g29_in_progress) {
          SERIAL_PROTOCOLPAIR("point ", min(abl_probe_index + 1, abl2));
          SERIAL_PROTOCOLLNPAIR(" of ", abl2);
        }
        else
          SERIAL_PROTOCOLLNPGM("idle");
      }

      if (no_action) return;

      if (abl_probe_index == 0) {
        // For the initial G29 save software endstop state
        #if HAS_SOFTWARE_ENDSTOPS
          enable_soft_endstops = soft_endstops_enabled;
        #endif
      }
      else {
        // For G29 after adjusting Z.
        // Save the previous Z before going to the next point
        measured_z = current_position[Z_AXIS];

        #if ENABLED(AUTO_BED_LEVELING_LINEAR)

          mean += measured_z;
          eqnBVector[abl_probe_index] = measured_z;
          eqnAMatrix[abl_probe_index + 0 * abl2] = xProbe;
          eqnAMatrix[abl_probe_index + 1 * abl2] = yProbe;
          eqnAMatrix[abl_probe_index + 2 * abl2] = 1;

          incremental_LSF(&lsf_results, xProbe, yProbe, measured_z);

        #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

          z_values[xCount][yCount] = measured_z + zoffset;

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_PROTOCOLPAIR("Save X", xCount);
              SERIAL_PROTOCOLPAIR(" Y", yCount);
              SERIAL_PROTOCOLLNPAIR(" Z", measured_z + zoffset);
            }
          #endif

        #elif ENABLED(AUTO_BED_LEVELING_3POINT)

          points[abl_probe_index].z = measured_z;

        #endif
      }

      //
      // If there's another point to sample, move there with optional lift.
      //

      #if ABL_GRID

        // Skip any unreachable points
        while (abl_probe_index < abl2) {

          // Set xCount, yCount based on abl_probe_index, with zig-zag
          PR_OUTER_VAR = abl_probe_index / PR_INNER_END;
          PR_INNER_VAR = abl_probe_index - (PR_OUTER_VAR * PR_INNER_END);

          // Probe in reverse order for every other row/column
          bool zig = (PR_OUTER_VAR & 1); // != ((PR_OUTER_END) & 1);

          if (zig) PR_INNER_VAR = (PR_INNER_END - 1) - PR_INNER_VAR;

          const float xBase = xCount * xGridSpacing + left_probe_bed_position,
                      yBase = yCount * yGridSpacing + front_probe_bed_position;

          xProbe = FLOOR(xBase + (xBase < 0 ? 0 : 0.5));
          yProbe = FLOOR(yBase + (yBase < 0 ? 0 : 0.5));

          #if ENABLED(AUTO_BED_LEVELING_LINEAR)
            indexIntoAB[xCount][yCount] = abl_probe_index;
          #endif

          // Keep looping till a reachable point is found
          if (position_is_reachable(xProbe, yProbe)) break;
          ++abl_probe_index;
        }

        // Is there a next point to move to?
        if (abl_probe_index < abl2) {
          _manual_goto_xy(xProbe, yProbe); // Can be used here too!
          #if HAS_SOFTWARE_ENDSTOPS
            // Disable software endstops to allow manual adjustment
            // If G29 is not completed, they will not be re-enabled
            soft_endstops_enabled = false;
          #endif
          return;
        }
        else {

          // Leveling done! Fall through to G29 finishing code below

          SERIAL_PROTOCOLLNPGM("Grid probing done.");

          // Re-enable software endstops, if needed
          #if HAS_SOFTWARE_ENDSTOPS
            soft_endstops_enabled = enable_soft_endstops;
          #endif
        }

      #elif ENABLED(AUTO_BED_LEVELING_3POINT)

        // Probe at 3 arbitrary points
        if (abl_probe_index < abl2) {
          xProbe = points[abl_probe_index].x;
          yProbe = points[abl_probe_index].y;
          _manual_goto_xy(xProbe, yProbe);
          #if HAS_SOFTWARE_ENDSTOPS
            // Disable software endstops to allow manual adjustment
            // If G29 is not completed, they will not be re-enabled
            soft_endstops_enabled = false;
          #endif
          return;
        }
        else {

          SERIAL_PROTOCOLLNPGM("3-point probing done.");

          // Re-enable software endstops, if needed
          #if HAS_SOFTWARE_ENDSTOPS
            soft_endstops_enabled = enable_soft_endstops;
          #endif

          if (!dryrun) {
            vector_3 planeNormal = vector_3::cross(points[0] - points[1], points[2] - points[1]).get_normal();
            if (planeNormal.z < 0) {
              planeNormal.x *= -1;
              planeNormal.y *= -1;
              planeNormal.z *= -1;
            }
            planner.bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

            // Can't re-enable (on error) until the new grid is written
            abl_should_enable = false;
          }

        }

      #endif // AUTO_BED_LEVELING_3POINT

    #else // !PROBE_MANUALLY
    {
      const bool stow_probe_after_each = parser.boolval('E');

      measured_z = 0;

      #if ABL_GRID

        bool zig = PR_OUTER_END & 1;  // Always end at RIGHT and BACK_PROBE_BED_POSITION

        measured_z = 0;

        // Outer loop is Y with PROBE_Y_FIRST disabled
        for (uint8_t PR_OUTER_VAR = 0; PR_OUTER_VAR < PR_OUTER_END && !isnan(measured_z); PR_OUTER_VAR++) {

          int8_t inStart, inStop, inInc;

          if (zig) { // away from origin
            inStart = 0;
            inStop = PR_INNER_END;
            inInc = 1;
          }
          else {     // towards origin
            inStart = PR_INNER_END - 1;
            inStop = -1;
            inInc = -1;
          }

          zig ^= true; // zag

          // Inner loop is Y with PROBE_Y_FIRST enabled
          for (int8_t PR_INNER_VAR = inStart; PR_INNER_VAR != inStop; PR_INNER_VAR += inInc) {

            float xBase = left_probe_bed_position + xGridSpacing * xCount,
                  yBase = front_probe_bed_position + yGridSpacing * yCount;

            xProbe = FLOOR(xBase + (xBase < 0 ? 0 : 0.5));
            yProbe = FLOOR(yBase + (yBase < 0 ? 0 : 0.5));

            #if ENABLED(AUTO_BED_LEVELING_LINEAR)
              indexIntoAB[xCount][yCount] = ++abl_probe_index; // 0...
            #endif

            #if IS_KINEMATIC
              // Avoid probing outside the round or hexagonal area
              if (!position_is_reachable_by_probe(xProbe, yProbe)) continue;
            #endif

            measured_z = faux ? 0.001 * random(-100, 101) : probe_pt(xProbe, yProbe, stow_probe_after_each, verbose_level);

            if (isnan(measured_z)) {
              set_bed_leveling_enabled(abl_should_enable);
              break;
            }

            #if ENABLED(AUTO_BED_LEVELING_LINEAR)

              mean += measured_z;
              eqnBVector[abl_probe_index] = measured_z;
              eqnAMatrix[abl_probe_index + 0 * abl2] = xProbe;
              eqnAMatrix[abl_probe_index + 1 * abl2] = yProbe;
              eqnAMatrix[abl_probe_index + 2 * abl2] = 1;

              incremental_LSF(&lsf_results, xProbe, yProbe, measured_z);

            #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

              z_values[xCount][yCount] = measured_z + zoffset;

            #endif

            abl_should_enable = false;
            idle();

          } // inner
        } // outer

      #elif ENABLED(AUTO_BED_LEVELING_3POINT)

        // Probe at 3 arbitrary points

        for (uint8_t i = 0; i < 3; ++i) {
          // Retain the last probe position
          xProbe = points[i].x;
          yProbe = points[i].y;
          measured_z = faux ? 0.001 * random(-100, 101) : probe_pt(xProbe, yProbe, stow_probe_after_each, verbose_level);
          if (isnan(measured_z)) {
            set_bed_leveling_enabled(abl_should_enable);
            break;
          }
          points[i].z = measured_z;
        }

        if (!dryrun && !isnan(measured_z)) {
          vector_3 planeNormal = vector_3::cross(points[0] - points[1], points[2] - points[1]).get_normal();
          if (planeNormal.z < 0) {
            planeNormal.x *= -1;
            planeNormal.y *= -1;
            planeNormal.z *= -1;
          }
          planner.bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

          // Can't re-enable (on error) until the new grid is written
          abl_should_enable = false;
        }

      #endif // AUTO_BED_LEVELING_3POINT

      // Raise to _Z_CLEARANCE_DEPLOY_PROBE. Stow the probe.
      if (STOW_PROBE()) {
        set_bed_leveling_enabled(abl_should_enable);
        measured_z = NAN;
      }
    }
    #endif // !PROBE_MANUALLY

    //
    // G29 Finishing Code
    //
    // Unless this is a dry run, auto bed leveling will
    // definitely be enabled after this point.
    //
    // If code above wants to continue leveling, it should
    // return or loop before this point.
    //

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("> probing complete", current_position);
    #endif

    #if ENABLED(PROBE_MANUALLY)
      g29_in_progress = false;
      #if ENABLED(LCD_BED_LEVELING)
        lcd_wait_for_move = false;
      #endif
    #endif

    // Calculate leveling, print reports, correct the position
    if (!isnan(measured_z)) {
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (!dryrun) extrapolate_unprobed_bed_level();
        print_bilinear_leveling_grid();

        refresh_bed_level();

        #if ENABLED(ABL_BILINEAR_SUBDIVISION)
          print_bilinear_leveling_grid_virt();
        #endif

      #elif ENABLED(AUTO_BED_LEVELING_LINEAR)

        // For LINEAR leveling calculate matrix, print reports, correct the position

        /**
         * solve the plane equation ax + by + d = z
         * A is the matrix with rows [x y 1] for all the probed points
         * B is the vector of the Z positions
         * the normal vector to the plane is formed by the coefficients of the
         * plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
         * so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z
         */
        float plane_equation_coefficients[3];

        finish_incremental_LSF(&lsf_results);
        plane_equation_coefficients[0] = -lsf_results.A;  // We should be able to eliminate the '-' on these three lines and down below
        plane_equation_coefficients[1] = -lsf_results.B;  // but that is not yet tested.
        plane_equation_coefficients[2] = -lsf_results.D;

        mean /= abl2;

        if (verbose_level) {
          SERIAL_PROTOCOLPGM("Eqn coefficients: a: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[0], 8);
          SERIAL_PROTOCOLPGM(" b: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[1], 8);
          SERIAL_PROTOCOLPGM(" d: ");
          SERIAL_PROTOCOL_F(plane_equation_coefficients[2], 8);
          SERIAL_EOL();
          if (verbose_level > 2) {
            SERIAL_PROTOCOLPGM("Mean of sampled points: ");
            SERIAL_PROTOCOL_F(mean, 8);
            SERIAL_EOL();
          }
        }

        // Create the matrix but don't correct the position yet
        if (!dryrun)
          planner.bed_level_matrix = matrix_3x3::create_look_at(
            vector_3(-plane_equation_coefficients[0], -plane_equation_coefficients[1], 1)    // We can eliminate the '-' here and up above
          );

        // Show the Topography map if enabled
        if (do_topography_map) {

          SERIAL_PROTOCOLLNPGM("\nBed Height Topography:\n"
                                 "   +--- BACK --+\n"
                                 "   |           |\n"
                                 " L |    (+)    | R\n"
                                 " E |           | I\n"
                                 " F | (-) N (+) | G\n"
                                 " T |           | H\n"
                                 "   |    (-)    | T\n"
                                 "   |           |\n"
                                 "   O-- FRONT --+\n"
                                 " (0,0)");

          float min_diff = 999;

          for (int8_t yy = abl_grid_points_y - 1; yy >= 0; yy--) {
            for (uint8_t xx = 0; xx < abl_grid_points_x; xx++) {
              int ind = indexIntoAB[xx][yy];
              float diff = eqnBVector[ind] - mean,
                    x_tmp = eqnAMatrix[ind + 0 * abl2],
                    y_tmp = eqnAMatrix[ind + 1 * abl2],
                    z_tmp = 0;

              apply_rotation_xyz(planner.bed_level_matrix, x_tmp, y_tmp, z_tmp);

              NOMORE(min_diff, eqnBVector[ind] - z_tmp);

              if (diff >= 0.0)
                SERIAL_PROTOCOLPGM(" +");   // Include + for column alignment
              else
                SERIAL_PROTOCOLCHAR(' ');
              SERIAL_PROTOCOL_F(diff, 5);
            } // xx
            SERIAL_EOL();
          } // yy
          SERIAL_EOL();

          if (verbose_level > 3) {
            SERIAL_PROTOCOLLNPGM("\nCorrected Bed Height vs. Bed Topology:");

            for (int8_t yy = abl_grid_points_y - 1; yy >= 0; yy--) {
              for (uint8_t xx = 0; xx < abl_grid_points_x; xx++) {
                int ind = indexIntoAB[xx][yy];
                float x_tmp = eqnAMatrix[ind + 0 * abl2],
                      y_tmp = eqnAMatrix[ind + 1 * abl2],
                      z_tmp = 0;

                apply_rotation_xyz(planner.bed_level_matrix, x_tmp, y_tmp, z_tmp);

                float diff = eqnBVector[ind] - z_tmp - min_diff;
                if (diff >= 0.0)
                  SERIAL_PROTOCOLPGM(" +");
                // Include + for column alignment
                else
                  SERIAL_PROTOCOLCHAR(' ');
                SERIAL_PROTOCOL_F(diff, 5);
              } // xx
              SERIAL_EOL();
            } // yy
            SERIAL_EOL();
          }
        } //do_topography_map

      #endif // AUTO_BED_LEVELING_LINEAR

      #if ABL_PLANAR

        // For LINEAR and 3POINT leveling correct the current position

        if (verbose_level > 0)
          planner.bed_level_matrix.debug(PSTR("\n\nBed Level Correction Matrix:"));

        if (!dryrun) {
          //
          // Correct the current XYZ position based on the tilted plane.
          //

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("G29 uncorrected XYZ", current_position);
          #endif

          float converted[XYZ];
          COPY(converted, current_position);

          planner.leveling_active = true;
          planner.unapply_leveling(converted); // use conversion machinery
          planner.leveling_active = false;

          // Use the last measured distance to the bed, if possible
          if ( NEAR(current_position[X_AXIS], xProbe - (X_PROBE_OFFSET_FROM_EXTRUDER))
            && NEAR(current_position[Y_AXIS], yProbe - (Y_PROBE_OFFSET_FROM_EXTRUDER))
          ) {
            const float simple_z = current_position[Z_AXIS] - measured_z;
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("Z from Probe:", simple_z);
                SERIAL_ECHOPAIR("  Matrix:", converted[Z_AXIS]);
                SERIAL_ECHOLNPAIR("  Discrepancy:", simple_z - converted[Z_AXIS]);
              }
            #endif
            converted[Z_AXIS] = simple_z;
          }

          // The rotated XY and corrected Z are now current_position
          COPY(current_position, converted);

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("G29 corrected XYZ", current_position);
          #endif
        }

      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (!dryrun) {
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("G29 uncorrected Z:", current_position[Z_AXIS]);
          #endif

          // Unapply the offset because it is going to be immediately applied
          // and cause compensation movement in Z
          current_position[Z_AXIS] -= bilinear_z_offset(current_position);

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR(" corrected Z:", current_position[Z_AXIS]);
          #endif
        }

      #endif // ABL_PLANAR

      #ifdef Z_PROBE_END_SCRIPT
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("Z Probe End Script: ", Z_PROBE_END_SCRIPT);
        #endif
        enqueue_and_echo_commands_P(PSTR(Z_PROBE_END_SCRIPT));
        stepper.synchronize();
      #endif

      // Auto Bed Leveling is complete! Enable if possible.
      planner.leveling_active = dryrun ? abl_should_enable : true;
    } // !isnan(measured_z)

    // Restore state after probing
    if (!faux) clean_up_after_endstop_or_probe_move();

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< G29");
    #endif

    report_current_position();

    KEEPALIVE_STATE(IN_HANDLER);

    if (planner.leveling_active)
      SYNC_PLAN_POSITION_KINEMATIC();
  }

#endif // OLDSCHOOL_ABL

#if HAS_BED_PROBE

  /**
   * G30: Do a single Z probe at the current XY
   *
   * Parameters:
   *
   *   X   Probe X position (default current X)
   *   Y   Probe Y position (default current Y)
   *   E   Engage the probe for each probe
   */
  inline void gcode_G30() {
    const float xpos = parser.linearval('X', current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER),
                ypos = parser.linearval('Y', current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER);

    if (!position_is_reachable_by_probe(xpos, ypos)) return;

    // Disable leveling so the planner won't mess with us
    #if HAS_LEVELING
      set_bed_leveling_enabled(false);
    #endif

    setup_for_endstop_or_probe_move();

    const float measured_z = probe_pt(xpos, ypos, parser.boolval('E'), 1);

    if (!isnan(measured_z)) {
      SERIAL_PROTOCOLPAIR("Bed X: ", FIXFLOAT(xpos));
      SERIAL_PROTOCOLPAIR(" Y: ", FIXFLOAT(ypos));
      SERIAL_PROTOCOLLNPAIR(" Z: ", FIXFLOAT(measured_z));
    }

    clean_up_after_endstop_or_probe_move();

    report_current_position();
  }

  #if ENABLED(Z_PROBE_SLED)

    /**
     * G31: Deploy the Z probe
     */
    inline void gcode_G31() { DEPLOY_PROBE(); }

    /**
     * G32: Stow the Z probe
     */
    inline void gcode_G32() { STOW_PROBE(); }

  #endif // Z_PROBE_SLED

#endif // HAS_BED_PROBE

#if ENABLED(DELTA_AUTO_CALIBRATION)

  constexpr uint8_t _7P_STEP = 1,              // 7-point step - to change number of calibration points
                    _4P_STEP = _7P_STEP * 2,   // 4-point step
                    NPP      = _7P_STEP * 6;   // number of calibration points on the radius
  enum CalEnum {                               // the 7 main calibration points - add definitions if needed
    CEN      = 0,
    __A      = 1,
    _AB      = __A + _7P_STEP,
    __B      = _AB + _7P_STEP,
    _BC      = __B + _7P_STEP,
    __C      = _BC + _7P_STEP,
    _CA      = __C + _7P_STEP,
  };

  #define LOOP_CAL_PT(VAR, S, N) for (uint8_t VAR=S; VAR<=NPP; VAR+=N)
  #define F_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR<NPP+0.9999; VAR+=N)
  #define I_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR>CEN+0.9999; VAR-=N)
  #define LOOP_CAL_ALL(VAR) LOOP_CAL_PT(VAR, CEN, 1)
  #define LOOP_CAL_RAD(VAR) LOOP_CAL_PT(VAR, __A, _7P_STEP)
  #define LOOP_CAL_ACT(VAR, _4P, _OP) LOOP_CAL_PT(VAR, _OP ? _AB : __A, _4P ? _4P_STEP : _7P_STEP)

  static void print_signed_float(const char * const prefix, const float &f) {
    SERIAL_PROTOCOLPGM("  ");
    serialprintPGM(prefix);
    SERIAL_PROTOCOLCHAR(':');
    if (f >= 0) SERIAL_CHAR('+');
    SERIAL_PROTOCOL_F(f, 2);
  }

  static void print_G33_settings(const bool end_stops, const bool tower_angles) {
    SERIAL_PROTOCOLPAIR(".Height:", delta_height);
    if (end_stops) {
      print_signed_float(PSTR("Ex"), delta_endstop_adj[A_AXIS]);
      print_signed_float(PSTR("Ey"), delta_endstop_adj[B_AXIS]);
      print_signed_float(PSTR("Ez"), delta_endstop_adj[C_AXIS]);
    }
    if (end_stops && tower_angles) {
      SERIAL_PROTOCOLPAIR("  Radius:", delta_radius);
      SERIAL_EOL();
      SERIAL_CHAR('.');
      SERIAL_PROTOCOL_SP(13);
    }
    if (tower_angles) {
      print_signed_float(PSTR("Tx"), delta_tower_angle_trim[A_AXIS]);
      print_signed_float(PSTR("Ty"), delta_tower_angle_trim[B_AXIS]);
      print_signed_float(PSTR("Tz"), delta_tower_angle_trim[C_AXIS]);
    }
    if ((!end_stops && tower_angles) || (end_stops && !tower_angles)) { // XOR
      SERIAL_PROTOCOLPAIR("  Radius:", delta_radius);
    }
    SERIAL_EOL();
  }

  static void print_G33_results(const float z_at_pt[NPP + 1], const bool tower_points, const bool opposite_points) {
    SERIAL_PROTOCOLPGM(".    ");
    print_signed_float(PSTR("c"), z_at_pt[CEN]);
    if (tower_points) {
      print_signed_float(PSTR(" x"), z_at_pt[__A]);
      print_signed_float(PSTR(" y"), z_at_pt[__B]);
      print_signed_float(PSTR(" z"), z_at_pt[__C]);
    }
    if (tower_points && opposite_points) {
      SERIAL_EOL();
      SERIAL_CHAR('.');
      SERIAL_PROTOCOL_SP(13);
    }
    if (opposite_points) {
      print_signed_float(PSTR("yz"), z_at_pt[_BC]);
      print_signed_float(PSTR("zx"), z_at_pt[_CA]);
      print_signed_float(PSTR("xy"), z_at_pt[_AB]);
    }
    SERIAL_EOL();
  }

  /**
   * After G33:
   *  - Move to the print ceiling (DELTA_HOME_TO_SAFE_ZONE only)
   *  - Stow the probe
   *  - Restore endstops state
   *  - Select the old tool, if needed
   */
  static void G33_cleanup(
    #if HOTENDS > 1
      const uint8_t old_tool_index
    #endif
  ) {
    #if ENABLED(DELTA_HOME_TO_SAFE_ZONE)
      do_blocking_move_to_z(delta_clip_start_height);
    #endif
    STOW_PROBE();
    clean_up_after_endstop_or_probe_move();
    #if HOTENDS > 1
      tool_change(old_tool_index, 0, true);
    #endif
  }

  inline float calibration_probe(const float nx, const float ny, const bool stow) {
    #if HAS_BED_PROBE
      return probe_pt(nx, ny, stow, 0, false);
    #else
      UNUSED(stow);
      return lcd_probe_pt(nx, ny);
    #endif
  }

  static float probe_G33_points(float z_at_pt[NPP + 1], const int8_t probe_points, const bool towers_set, const bool stow_after_each) {
    const bool _0p_calibration      = probe_points == 0,
               _1p_calibration      = probe_points == 1,
               _4p_calibration      = probe_points == 2,
               _4p_opposite_points  = _4p_calibration && !towers_set,
               _7p_calibration      = probe_points >= 3 || probe_points == 0,
               _7p_no_intermediates = probe_points == 3,
               _7p_1_intermediates  = probe_points == 4,
               _7p_2_intermediates  = probe_points == 5,
               _7p_4_intermediates  = probe_points == 6,
               _7p_6_intermediates  = probe_points == 7,
               _7p_8_intermediates  = probe_points == 8,
               _7p_11_intermediates = probe_points == 9,
               _7p_14_intermediates = probe_points == 10,
               _7p_intermed_points  = probe_points >= 4,
               _7p_6_centre         = probe_points >= 5 && probe_points <= 7,
               _7p_9_centre         = probe_points >= 8;

    LOOP_CAL_ALL(axis) z_at_pt[axis] = 0.0;

    if (!_0p_calibration) {

      if (!_7p_no_intermediates && !_7p_4_intermediates && !_7p_11_intermediates) { // probe the center
        z_at_pt[CEN] += calibration_probe(0, 0, stow_after_each);
        if (isnan(z_at_pt[CEN])) return NAN;
      }

      if (_7p_calibration) { // probe extra center points
        const float start  = _7p_9_centre ? _CA + _7P_STEP / 3.0 : _7p_6_centre ? _CA : __C,
                    steps  = _7p_9_centre ? _4P_STEP / 3.0 : _7p_6_centre ? _7P_STEP : _4P_STEP;
        I_LOOP_CAL_PT(axis, start, steps) {
          const float a = RADIANS(210 + (360 / NPP) *  (axis - 1)),
                      r = delta_calibration_radius * 0.1;
          z_at_pt[CEN] += calibration_probe(cos(a) * r, sin(a) * r, stow_after_each);
          if (isnan(z_at_pt[CEN])) return NAN;
       }
        z_at_pt[CEN] /= float(_7p_2_intermediates ? 7 : probe_points);
      }

      if (!_1p_calibration) {  // probe the radius
        const CalEnum start  = _4p_opposite_points ? _AB : __A;
        const float   steps  = _7p_14_intermediates ? _7P_STEP / 15.0 : // 15r * 6 + 10c = 100
                               _7p_11_intermediates ? _7P_STEP / 12.0 : // 12r * 6 +  9c = 81
                               _7p_8_intermediates  ? _7P_STEP /  9.0 : //  9r * 6 + 10c = 64
                               _7p_6_intermediates  ? _7P_STEP /  7.0 : //  7r * 6 +  7c = 49
                               _7p_4_intermediates  ? _7P_STEP /  5.0 : //  5r * 6 +  6c = 36
                               _7p_2_intermediates  ? _7P_STEP /  3.0 : //  3r * 6 +  7c = 25
                               _7p_1_intermediates  ? _7P_STEP /  2.0 : //  2r * 6 +  4c = 16
                               _7p_no_intermediates ? _7P_STEP :        //  1r * 6 +  3c = 9
                               _4P_STEP;                                // .5r * 6 +  1c = 4
        bool zig_zag = true;
        F_LOOP_CAL_PT(axis, start, _7p_9_centre ? steps * 3 : steps) {
          const int8_t offset = _7p_9_centre ? 1 : 0;
          for (int8_t circle = -offset; circle <= offset; circle++) {
            const float a = RADIANS(210 + (360 / NPP) *  (axis - 1)),
                        r = delta_calibration_radius * (1 + 0.1 * (zig_zag ? circle : - circle)),
                        interpol = fmod(axis, 1);
            const float z_temp = calibration_probe(cos(a) * r, sin(a) * r, stow_after_each);
            if (isnan(z_temp)) return NAN;
            // split probe point to neighbouring calibration points
            z_at_pt[uint8_t(round(axis - interpol + NPP - 1)) % NPP + 1] += z_temp * sq(cos(RADIANS(interpol * 90)));
            z_at_pt[uint8_t(round(axis - interpol))           % NPP + 1] += z_temp * sq(sin(RADIANS(interpol * 90)));
          }
          zig_zag = !zig_zag;
        }
        if (_7p_intermed_points)
          LOOP_CAL_RAD(axis)
            z_at_pt[axis] /= _7P_STEP / steps;
      }

      float S1 = z_at_pt[CEN],
            S2 = sq(z_at_pt[CEN]);
      int16_t N = 1;
      if (!_1p_calibration) { // std dev from zero plane
        LOOP_CAL_ACT(axis, _4p_calibration, _4p_opposite_points) {
          S1 += z_at_pt[axis];
          S2 += sq(z_at_pt[axis]);
          N++;
        }
        return round(SQRT(S2 / N) * 1000.0) / 1000.0 + 0.00001;
      }
    }

    return 0.00001;
  }

  #if HAS_BED_PROBE

    static bool G33_auto_tune() {
      float z_at_pt[NPP + 1]      = { 0.0 },
            z_at_pt_base[NPP + 1] = { 0.0 },
            z_temp, h_fac = 0.0, r_fac = 0.0, a_fac = 0.0, norm = 0.8;

      #define ZP(N,I) ((N) * z_at_pt[I])
      #define Z06(I)  ZP(6, I)
      #define Z03(I)  ZP(3, I)
      #define Z02(I)  ZP(2, I)
      #define Z01(I)  ZP(1, I)
      #define Z32(I)  ZP(3/2, I)

      SERIAL_PROTOCOLPGM("AUTO TUNE baseline");
      SERIAL_EOL();
      if (isnan(probe_G33_points(z_at_pt_base, 3, true, false))) return false;
      print_G33_results(z_at_pt_base, true, true);

      LOOP_XYZ(axis) {
        delta_endstop_adj[axis] -= 1.0;
        recalc_delta_settings();

        endstops.enable(true);
        if (!home_delta()) return false;
        endstops.not_homing();

        SERIAL_PROTOCOLPGM("Tuning E");
        SERIAL_CHAR(tolower(axis_codes[axis]));
        SERIAL_EOL();

        if (isnan(probe_G33_points(z_at_pt, 3, true, false))) return false;
        LOOP_CAL_ALL(axis) z_at_pt[axis] -= z_at_pt_base[axis];
        print_G33_results(z_at_pt, true, true);
        delta_endstop_adj[axis] += 1.0;
        recalc_delta_settings();
        switch (axis) {
          case A_AXIS :
            h_fac += 4.0 / (Z03(CEN) +Z01(__A)                               +Z32(_CA) +Z32(_AB)); // Offset by X-tower end-stop
            break;
          case B_AXIS :
            h_fac += 4.0 / (Z03(CEN)           +Z01(__B)           +Z32(_BC)           +Z32(_AB)); // Offset by Y-tower end-stop
            break;
          case C_AXIS :
            h_fac += 4.0 / (Z03(CEN)                     +Z01(__C) +Z32(_BC) +Z32(_CA)          ); // Offset by Z-tower end-stop
            break;
        }
      }
      h_fac /= 3.0;
      h_fac *= norm; // Normalize to 1.02 for Kossel mini

      for (int8_t zig_zag = -1; zig_zag < 2; zig_zag += 2) {
        delta_radius += 1.0 * zig_zag;
        recalc_delta_settings();

        endstops.enable(true);
        if (!home_delta()) return false;
        endstops.not_homing();

        SERIAL_PROTOCOLPGM("Tuning R");
        SERIAL_PROTOCOL(zig_zag == -1 ? "-" : "+");
        SERIAL_EOL();
        if (isnan(probe_G33_points(z_at_pt, 3, true, false))) return false;
        LOOP_CAL_ALL(axis) z_at_pt[axis] -= z_at_pt_base[axis];
        print_G33_results(z_at_pt, true, true);
        delta_radius -= 1.0 * zig_zag;
        recalc_delta_settings();
        r_fac -= zig_zag * 6.0 / (Z03(__A) +Z03(__B) +Z03(__C) +Z03(_BC) +Z03(_CA) +Z03(_AB)); // Offset by delta radius
      }
      r_fac /= 2.0;
      r_fac *= 3 * norm; // Normalize to 2.25 for Kossel mini

      LOOP_XYZ(axis) {
        delta_tower_angle_trim[axis] += 1.0;
        delta_endstop_adj[(axis + 1) % 3] -= 1.0 / 4.5;
        delta_endstop_adj[(axis + 2) % 3] += 1.0 / 4.5;
        z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
        delta_height -= z_temp;
        LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
        recalc_delta_settings();

        endstops.enable(true);
        if (!home_delta()) return false;
        endstops.not_homing();

        SERIAL_PROTOCOLPGM("Tuning T");
        SERIAL_CHAR(tolower(axis_codes[axis]));
        SERIAL_EOL();

        if (isnan(probe_G33_points(z_at_pt, 3, true, false))) return false;
        LOOP_CAL_ALL(axis) z_at_pt[axis] -= z_at_pt_base[axis];
        print_G33_results(z_at_pt, true, true);

        delta_tower_angle_trim[axis] -= 1.0;
        delta_endstop_adj[(axis+1) % 3] += 1.0/4.5;
        delta_endstop_adj[(axis+2) % 3] -= 1.0/4.5;
        z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
        delta_height -= z_temp;
        LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
        recalc_delta_settings();
        switch (axis) {
          case A_AXIS :
            a_fac += 4.0 / (          Z06(__B) -Z06(__C)           +Z06(_CA) -Z06(_AB)); // Offset by alpha tower angle
            break;
          case B_AXIS :
            a_fac += 4.0 / (-Z06(__A)          +Z06(__C) -Z06(_BC)           +Z06(_AB)); // Offset by beta tower angle
            break;
          case C_AXIS :
            a_fac += 4.0 / (Z06(__A) -Z06(__B)           +Z06(_BC) -Z06(_CA)          ); // Offset by gamma tower angle
            break;
        }
      }
      a_fac /= 3.0;
      a_fac *= norm; // Normalize to 0.83 for Kossel mini

      endstops.enable(true);
      if (!home_delta()) return false;
      endstops.not_homing();
      print_signed_float(PSTR( "H_FACTOR: "), h_fac);
      print_signed_float(PSTR(" R_FACTOR: "), r_fac);
      print_signed_float(PSTR(" A_FACTOR: "), a_fac);
      SERIAL_EOL();
      SERIAL_PROTOCOLPGM("Copy these values to Configuration.h");
      SERIAL_EOL();
      return true;
    }

  #endif // HAS_BED_PROBE

  /**
   * G33 - Delta '1-4-7-point' Auto-Calibration
   *       Calibrate height, endstops, delta radius, and tower angles.
   *
   * Parameters:
   *
   *   Pn  Number of probe points:
   *      P0     No probe. Normalize only.
   *      P1     Probe center and set height only.
   *      P2     Probe center and towers. Set height, endstops and delta radius.
   *      P3     Probe all positions: center, towers and opposite towers. Set all.
   *      P4-P10 Probe all positions + at different itermediate locations and average them.
   *
   *   T   Don't calibrate tower angle corrections
   *
   *   Cn.nn  Calibration precision; when omitted calibrates to maximum precision
   *
   *   Fn  Force to run at least n iterations and takes the best result
   *
   *   A   Auto tune calibartion factors (set in Configuration.h)
   *
   *   Vn  Verbose level:
   *      V0  Dry-run mode. Report settings and probe results. No calibration.
   *      V1  Report start and end settings only
   *      V2  Report settings at each iteration
   *      V3  Report settings and probe results
   *
   *   E   Engage the probe for each point
   */
  inline void gcode_G33() {

    const int8_t probe_points = parser.intval('P', DELTA_CALIBRATION_DEFAULT_POINTS);
    if (!WITHIN(probe_points, 0, 10)) {
      SERIAL_PROTOCOLLNPGM("?(P)oints is implausible (0-10).");
      return;
    }

    const int8_t verbose_level = parser.byteval('V', 1);
    if (!WITHIN(verbose_level, 0, 3)) {
      SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-3).");
      return;
    }

    const float calibration_precision = parser.floatval('C', 0.0);
    if (calibration_precision < 0) {
      SERIAL_PROTOCOLLNPGM("?(C)alibration precision is implausible (>=0).");
      return;
    }

    const int8_t force_iterations = parser.intval('F', 0);
    if (!WITHIN(force_iterations, 0, 30)) {
      SERIAL_PROTOCOLLNPGM("?(F)orce iteration is implausible (0-30).");
      return;
    }

    const bool towers_set           = !parser.boolval('T'),
               auto_tune            = parser.boolval('A'),
               stow_after_each      = parser.boolval('E'),
               _0p_calibration      = probe_points == 0,
               _1p_calibration      = probe_points == 1,
               _4p_calibration      = probe_points == 2,
               _7p_9_centre         = probe_points >= 8,
               _tower_results       = (_4p_calibration && towers_set)
                                      || probe_points >= 3 || probe_points == 0,
               _opposite_results    = (_4p_calibration && !towers_set)
                                      || probe_points >= 3 || probe_points == 0,
               _endstop_results     = probe_points != 1,
               _angle_results       = (probe_points >= 3 || probe_points == 0) && towers_set;
    const static char save_message[] PROGMEM = "Save with M500 and/or copy to Configuration.h";
    int8_t iterations = 0;
    float test_precision,
          zero_std_dev = (verbose_level ? 999.0 : 0.0), // 0.0 in dry-run mode : forced end
          zero_std_dev_min = zero_std_dev,
          e_old[ABC] = {
            delta_endstop_adj[A_AXIS],
            delta_endstop_adj[B_AXIS],
            delta_endstop_adj[C_AXIS]
          },
          dr_old = delta_radius,
          zh_old = delta_height,
          ta_old[ABC] = {
            delta_tower_angle_trim[A_AXIS],
            delta_tower_angle_trim[B_AXIS],
            delta_tower_angle_trim[C_AXIS]
          };

    SERIAL_PROTOCOLLNPGM("G33 Auto Calibrate");

    if (!_1p_calibration && !_0p_calibration) {  // test if the outer radius is reachable
      LOOP_CAL_RAD(axis) {
        const float a = RADIANS(210 + (360 / NPP) *  (axis - 1)),
                    r = delta_calibration_radius * (1 + (_7p_9_centre ? 0.1 : 0.0));
        if (!position_is_reachable(cos(a) * r, sin(a) * r)) {
          SERIAL_PROTOCOLLNPGM("?(M665 B)ed radius is implausible.");
          return;
        }
      }
    }

    stepper.synchronize();
    #if HAS_LEVELING
      reset_bed_level(); // After calibration bed-level data is no longer valid
    #endif

    #if HOTENDS > 1
      const uint8_t old_tool_index = active_extruder;
      tool_change(0, 0, true);
      #define G33_CLEANUP() G33_cleanup(old_tool_index)
    #else
      #define G33_CLEANUP() G33_cleanup()
    #endif

    setup_for_endstop_or_probe_move();
    endstops.enable(true);
    if (!_0p_calibration) {
      if (!home_delta())
        return;
      endstops.not_homing();
    }

    if (auto_tune) {
      #if HAS_BED_PROBE
        G33_auto_tune();
      #else
        SERIAL_PROTOCOLLNPGM("A probe is needed for auto-tune");
      #endif
      G33_CLEANUP();
      return;
    }

    // Report settings

    PGM_P checkingac = PSTR("Checking... AC"); // TODO: Make translatable string
    serialprintPGM(checkingac);
    if (verbose_level == 0) SERIAL_PROTOCOLPGM(" (DRY-RUN)");
    SERIAL_EOL();
    lcd_setstatusPGM(checkingac);

    print_G33_settings(_endstop_results, _angle_results);

    do {

      float z_at_pt[NPP + 1] = { 0.0 };

      test_precision = zero_std_dev;

      iterations++;

      // Probe the points

      zero_std_dev = probe_G33_points(z_at_pt, probe_points, towers_set, stow_after_each);
      if (isnan(zero_std_dev)) {
        SERIAL_PROTOCOLPGM("Correct delta_radius with M665 R or end-stops with M666 X Y Z");
        SERIAL_EOL();
        return G33_CLEANUP();
      }

      // Solve matrices

      if ((zero_std_dev < test_precision || iterations <= force_iterations) && zero_std_dev > calibration_precision) {
        if (zero_std_dev < zero_std_dev_min) {
          COPY(e_old, delta_endstop_adj);
          dr_old = delta_radius;
          zh_old = delta_height;
          COPY(ta_old, delta_tower_angle_trim);
        }

        float e_delta[ABC] = { 0.0 }, r_delta = 0.0, t_delta[ABC] = { 0.0 };
        const float r_diff = delta_radius - delta_calibration_radius,
                    h_factor = 1 / 6.0 *
                      #ifdef H_FACTOR
                        (H_FACTOR),                                       // Set in Configuration.h
                      #else
                        (1.00 + r_diff * 0.001),                          // 1.02 for r_diff = 20mm
                      #endif
                    r_factor = 1 / 6.0 *
                      #ifdef R_FACTOR
                        -(R_FACTOR),                                      // Set in Configuration.h
                      #else
                        -(1.75 + 0.005 * r_diff + 0.001 * sq(r_diff)),    // 2.25 for r_diff = 20mm
                      #endif
                    a_factor = 1 / 6.0 *
                      #ifdef A_FACTOR
                        (A_FACTOR);                                       // Set in Configuration.h
                      #else
                        (66.66 / delta_calibration_radius);               // 0.83 for cal_rd = 80mm
                      #endif

        #define ZP(N,I) ((N) * z_at_pt[I])
        #define Z6(I) ZP(6, I)
        #define Z4(I) ZP(4, I)
        #define Z2(I) ZP(2, I)
        #define Z1(I) ZP(1, I)

        #if !HAS_BED_PROBE
          test_precision = 0.00; // forced end
        #endif

        switch (probe_points) {
          case 0:
            test_precision = 0.00; // forced end
            break;

          case 1:
            test_precision = 0.00; // forced end
            LOOP_XYZ(axis) e_delta[axis] = Z1(CEN);
            break;

          case 2:
            if (towers_set) {
              e_delta[A_AXIS] = (Z6(CEN) +Z4(__A) -Z2(__B) -Z2(__C)) * h_factor;
              e_delta[B_AXIS] = (Z6(CEN) -Z2(__A) +Z4(__B) -Z2(__C)) * h_factor;
              e_delta[C_AXIS] = (Z6(CEN) -Z2(__A) -Z2(__B) +Z4(__C)) * h_factor;
              r_delta         = (Z6(CEN) -Z2(__A) -Z2(__B) -Z2(__C)) * r_factor;
            }
            else {
              e_delta[A_AXIS] = (Z6(CEN) -Z4(_BC) +Z2(_CA) +Z2(_AB)) * h_factor;
              e_delta[B_AXIS] = (Z6(CEN) +Z2(_BC) -Z4(_CA) +Z2(_AB)) * h_factor;
              e_delta[C_AXIS] = (Z6(CEN) +Z2(_BC) +Z2(_CA) -Z4(_AB)) * h_factor;
              r_delta         = (Z6(CEN) -Z2(_BC) -Z2(_CA) -Z2(_AB)) * r_factor;
            }
            break;

          default:
            e_delta[A_AXIS] = (Z6(CEN) +Z2(__A) -Z1(__B) -Z1(__C) -Z2(_BC) +Z1(_CA) +Z1(_AB)) * h_factor;
            e_delta[B_AXIS] = (Z6(CEN) -Z1(__A) +Z2(__B) -Z1(__C) +Z1(_BC) -Z2(_CA) +Z1(_AB)) * h_factor;
            e_delta[C_AXIS] = (Z6(CEN) -Z1(__A) -Z1(__B) +Z2(__C) +Z1(_BC) +Z1(_CA) -Z2(_AB)) * h_factor;
            r_delta         = (Z6(CEN) -Z1(__A) -Z1(__B) -Z1(__C) -Z1(_BC) -Z1(_CA) -Z1(_AB)) * r_factor;

            if (towers_set) {
              t_delta[A_AXIS] = (         -Z4(__B) +Z4(__C)          -Z4(_CA) +Z4(_AB)) * a_factor;
              t_delta[B_AXIS] = ( Z4(__A)          -Z4(__C) +Z4(_BC)          -Z4(_AB)) * a_factor;
              t_delta[C_AXIS] = (-Z4(__A) +Z4(__B)          -Z4(_BC) +Z4(_CA)         ) * a_factor;
              e_delta[A_AXIS] += (t_delta[B_AXIS] - t_delta[C_AXIS]) / 4.5;
              e_delta[B_AXIS] += (t_delta[C_AXIS] - t_delta[A_AXIS]) / 4.5;
              e_delta[C_AXIS] += (t_delta[A_AXIS] - t_delta[B_AXIS]) / 4.5;
            }
            break;
        }

        LOOP_XYZ(axis) delta_endstop_adj[axis] += e_delta[axis];
        delta_radius += r_delta;
        LOOP_XYZ(axis) delta_tower_angle_trim[axis] += t_delta[axis];
      }
      else if (zero_std_dev >= test_precision) {   // step one back
        COPY(delta_endstop_adj, e_old);
        delta_radius = dr_old;
        delta_height = zh_old;
        COPY(delta_tower_angle_trim, ta_old);
      }

      if (verbose_level != 0) {                                    // !dry run
        // normalise angles to least squares
        if (_angle_results) {
          float a_sum = 0.0;
          LOOP_XYZ(axis) a_sum += delta_tower_angle_trim[axis];
          LOOP_XYZ(axis) delta_tower_angle_trim[axis] -= a_sum / 3.0;
        }

        // adjust delta_height and endstops by the max amount
        const float z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
        delta_height -= z_temp;
        LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
      }
      recalc_delta_settings();
      NOMORE(zero_std_dev_min, zero_std_dev);

      // print report

      if (verbose_level > 2)
        print_G33_results(z_at_pt, _tower_results, _opposite_results);

      if (verbose_level != 0) {                                    // !dry run
        if ((zero_std_dev >= test_precision && iterations > force_iterations) || zero_std_dev <= calibration_precision) {  // end iterations
          SERIAL_PROTOCOLPGM("Calibration OK");
          SERIAL_PROTOCOL_SP(32);
          #if HAS_BED_PROBE
            if (zero_std_dev >= test_precision && !_1p_calibration)
              SERIAL_PROTOCOLPGM("rolling back.");
            else
          #endif
            {
              SERIAL_PROTOCOLPGM("std dev:");
              SERIAL_PROTOCOL_F(zero_std_dev_min, 3);
            }
          SERIAL_EOL();
          char mess[21];
          strcpy_P(mess, PSTR("Calibration sd:"));
          if (zero_std_dev_min < 1)
            sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev_min * 1000.0));
          else
            sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev_min));
          lcd_setstatus(mess);
          print_G33_settings(_endstop_results, _angle_results);
          serialprintPGM(save_message);
          SERIAL_EOL();
        }
        else {                                                     // !end iterations
          char mess[15];
          if (iterations < 31)
            sprintf_P(mess, PSTR("Iteration : %02i"), (int)iterations);
          else
            strcpy_P(mess, PSTR("No convergence"));
          SERIAL_PROTOCOL(mess);
          SERIAL_PROTOCOL_SP(32);
          SERIAL_PROTOCOLPGM("std dev:");
          SERIAL_PROTOCOL_F(zero_std_dev, 3);
          SERIAL_EOL();
          lcd_setstatus(mess);
          if (verbose_level > 1)
            print_G33_settings(_endstop_results, _angle_results);
        }
      }
      else {                                                       // dry run
        PGM_P enddryrun = PSTR("End DRY-RUN");
        serialprintPGM(enddryrun);
        SERIAL_PROTOCOL_SP(35);
        SERIAL_PROTOCOLPGM("std dev:");
        SERIAL_PROTOCOL_F(zero_std_dev, 3);
        SERIAL_EOL();

        char mess[21];
        strcpy_P(mess, enddryrun);
        strcpy_P(&mess[11], PSTR(" sd:"));
        if (zero_std_dev < 1)
          sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev * 1000.0));
        else
          sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev));
        lcd_setstatus(mess);
      }

      endstops.enable(true);
      if (!home_delta())
        return;
      endstops.not_homing();

    }
    while (((zero_std_dev < test_precision && iterations < 31) || iterations <= force_iterations) && zero_std_dev > calibration_precision);

    G33_CLEANUP();
  }

#endif // DELTA_AUTO_CALIBRATION

#if ENABLED(G38_PROBE_TARGET)

  static bool G38_run_probe() {

    bool G38_pass_fail = false;

    #if MULTIPLE_PROBING > 1
      // Get direction of move and retract
      float retract_mm[XYZ];
      LOOP_XYZ(i) {
        float dist = destination[i] - current_position[i];
        retract_mm[i] = FABS(dist) < G38_MINIMUM_MOVE ? 0 : home_bump_mm((AxisEnum)i) * (dist > 0 ? -1 : 1);
      }
    #endif

    stepper.synchronize();  // wait until the machine is idle

    // Move until destination reached or target hit
    endstops.enable(true);
    G38_move = true;
    G38_endstop_hit = false;
    prepare_move_to_destination();
    stepper.synchronize();
    G38_move = false;

    endstops.hit_on_purpose();
    set_current_from_steppers_for_axis(ALL_AXES);
    SYNC_PLAN_POSITION_KINEMATIC();

    if (G38_endstop_hit) {

      G38_pass_fail = true;

      #if MULTIPLE_PROBING > 1
        // Move away by the retract distance
        set_destination_from_current();
        LOOP_XYZ(i) destination[i] += retract_mm[i];
        endstops.enable(false);
        prepare_move_to_destination();
        stepper.synchronize();

        feedrate_mm_s /= 4;

        // Bump the target more slowly
        LOOP_XYZ(i) destination[i] -= retract_mm[i] * 2;

        endstops.enable(true);
        G38_move = true;
        prepare_move_to_destination();
        stepper.synchronize();
        G38_move = false;

        set_current_from_steppers_for_axis(ALL_AXES);
        SYNC_PLAN_POSITION_KINEMATIC();
      #endif
    }

    endstops.hit_on_purpose();
    endstops.not_homing();
    return G38_pass_fail;
  }

  /**
   * G38.2 - probe toward workpiece, stop on contact, signal error if failure
   * G38.3 - probe toward workpiece, stop on contact
   *
   * Like G28 except uses Z min probe for all axes
   */
  inline void gcode_G38(bool is_38_2) {
    // Get X Y Z E F
    gcode_get_destination();

    setup_for_endstop_or_probe_move();

    // If any axis has enough movement, do the move
    LOOP_XYZ(i)
      if (FABS(destination[i] - current_position[i]) >= G38_MINIMUM_MOVE) {
        if (!parser.seenval('F')) feedrate_mm_s = homing_feedrate((AxisEnum)i);
        // If G38.2 fails throw an error
        if (!G38_run_probe() && is_38_2) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM("Failed to reach target");
        }
        break;
      }

    clean_up_after_endstop_or_probe_move();
  }

#endif // G38_PROBE_TARGET

#if HAS_MESH

  /**
   * G42: Move X & Y axes to mesh coordinates (I & J)
   */
  inline void gcode_G42() {
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      if (axis_unhomed_error()) return;
    #endif

    if (IsRunning()) {
      const bool hasI = parser.seenval('I');
      const int8_t ix = hasI ? parser.value_int() : 0;
      const bool hasJ = parser.seenval('J');
      const int8_t iy = hasJ ? parser.value_int() : 0;

      if ((hasI && !WITHIN(ix, 0, GRID_MAX_POINTS_X - 1)) || (hasJ && !WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1))) {
        SERIAL_ECHOLNPGM(MSG_ERR_MESH_XY);
        return;
      }

      set_destination_from_current();
      if (hasI) destination[X_AXIS] = _GET_MESH_X(ix);
      if (hasJ) destination[Y_AXIS] = _GET_MESH_Y(iy);
      if (parser.boolval('P')) {
        if (hasI) destination[X_AXIS] -= X_PROBE_OFFSET_FROM_EXTRUDER;
        if (hasJ) destination[Y_AXIS] -= Y_PROBE_OFFSET_FROM_EXTRUDER;
      }

      const float fval = parser.linearval('F');
      if (fval > 0.0) feedrate_mm_s = MMM_TO_MMS(fval);

      // SCARA kinematic has "safe" XY raw moves
      #if IS_SCARA
        prepare_uninterpolated_move_to_destination();
      #else
        prepare_move_to_destination();
      #endif
    }
  }

#endif // HAS_MESH

/**
 * G92: Set current position to given X Y Z E
 */
inline void gcode_G92() {

  stepper.synchronize();

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    switch (parser.subcode) {
      case 1:
        // Zero the G92 values and restore current position
        #if !IS_SCARA
          LOOP_XYZ(i) {
            const float v = position_shift[i];
            if (v) {
              position_shift[i] = 0;
              update_software_endstops((AxisEnum)i);
            }
          }
        #endif // Not SCARA
        return;
    }
  #endif

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    #define IS_G92_0 (parser.subcode == 0)
  #else
    #define IS_G92_0 true
  #endif

  bool didE = false;
  #if IS_SCARA || !HAS_POSITION_SHIFT
    bool didXYZ = false;
  #else
    constexpr bool didXYZ = false;
  #endif

  if (IS_G92_0) LOOP_XYZE(i) {
    if (parser.seenval(axis_codes[i])) {
      const float l = parser.value_axis_units((AxisEnum)i),
                  v = i == E_AXIS ? l : LOGICAL_TO_NATIVE(l, i),
                  d = v - current_position[i];
      if (!NEAR_ZERO(d)) {
        #if IS_SCARA || !HAS_POSITION_SHIFT
          if (i == E_AXIS) didE = true; else didXYZ = true;
          current_position[i] = v;        // Without workspaces revert to Marlin 1.0 behavior
        #elif HAS_POSITION_SHIFT
          if (i == E_AXIS) {
            didE = true;
            current_position[E_AXIS] = v; // When using coordinate spaces, only E is set directly
          }
          else {
            position_shift[i] += d;       // Other axes simply offset the coordinate space
            update_software_endstops((AxisEnum)i);
          }
        #endif
      }
    }
  }

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    // Apply workspace offset to the active coordinate system
    if (WITHIN(active_coordinate_system, 0, MAX_COORDINATE_SYSTEMS - 1))
      COPY(coordinate_system[active_coordinate_system], position_shift);
  #endif

  if (didXYZ)
    SYNC_PLAN_POSITION_KINEMATIC();
  else if (didE)
    sync_plan_position_e();

  report_current_position();
}

#if HAS_RESUME_CONTINUE

  /**
   * M0: Unconditional stop - Wait for user button press on LCD
   * M1: Conditional stop   - Wait for user button press on LCD
   */
  inline void gcode_M0_M1() {
    const char * const args = parser.string_arg;

    millis_t ms = 0;
    bool hasP = false, hasS = false;
    if (parser.seenval('P')) {
      ms = parser.value_millis(); // milliseconds to wait
      hasP = ms > 0;
    }
    if (parser.seenval('S')) {
      ms = parser.value_millis_from_seconds(); // seconds to wait
      hasS = ms > 0;
    }

    #if ENABLED(ULTIPANEL)

      if (!hasP && !hasS && args && *args)
        lcd_setstatus(args, true);
      else {
        LCD_MESSAGEPGM(MSG_USERWAIT);
        #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
          dontExpireStatus();
        #endif
      }

    #else

      if (!hasP && !hasS && args && *args) {
        SERIAL_ECHO_START();
        SERIAL_ECHOLN(args);
      }

    #endif

    KEEPALIVE_STATE(PAUSED_FOR_USER);
    wait_for_user = true;

    stepper.synchronize();
    refresh_cmd_timeout();

    if (ms > 0) {
      ms += previous_cmd_ms;  // wait until this time for a click
      while (PENDING(millis(), ms) && wait_for_user) idle();
    }
    else {
      #if ENABLED(ULTIPANEL)
        if (lcd_detected()) {
          while (wait_for_user) idle();
          print_job_timer.isPaused() ? LCD_MESSAGEPGM(WELCOME_MSG) : LCD_MESSAGEPGM(MSG_RESUMING);
        }
      #else
        while (wait_for_user) idle();
      #endif
    }

    wait_for_user = false;
    KEEPALIVE_STATE(IN_HANDLER);
  }

#endif // HAS_RESUME_CONTINUE

#if ENABLED(SPINDLE_LASER_ENABLE)
  /**
   * M3: Spindle Clockwise
   * M4: Spindle Counter-clockwise
   *
   *  S0 turns off spindle.
   *
   *  If no speed PWM output is defined then M3/M4 just turns it on.
   *
   *  At least 12.8KHz (50Hz * 256) is needed for spindle PWM.
   *  Hardware PWM is required. ISRs are too slow.
   *
   * NOTE: WGM for timers 3, 4, and 5 must be either Mode 1 or Mode 5.
   *       No other settings give a PWM signal that goes from 0 to 5 volts.
   *
   *       The system automatically sets WGM to Mode 1, so no special
   *       initialization is needed.
   *
   *       WGM bits for timer 2 are automatically set by the system to
   *       Mode 1. This produces an acceptable 0 to 5 volt signal.
   *       No special initialization is needed.
   *
   * NOTE: A minimum PWM frequency of 50 Hz is needed. All prescaler
   *       factors for timers 2, 3, 4, and 5 are acceptable.
   *
   *  SPINDLE_LASER_ENABLE_PIN needs an external pullup or it may power on
   *  the spindle/laser during power-up or when connecting to the host
   *  (usually goes through a reset which sets all I/O pins to tri-state)
   *
   *  PWM duty cycle goes from 0 (off) to 255 (always on).
   */

  // Wait for spindle to come up to speed
  inline void delay_for_power_up() { dwell(SPINDLE_LASER_POWERUP_DELAY); }

  // Wait for spindle to stop turning
  inline void delay_for_power_down() { dwell(SPINDLE_LASER_POWERDOWN_DELAY); }

  /**
   * ocr_val_mode() is used for debugging and to get the points needed to compute the RPM vs ocr_val line
   *
   * it accepts inputs of 0-255
   */

  inline void ocr_val_mode() {
    uint8_t spindle_laser_power = parser.value_byte();
    WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT); // turn spindle on (active low)
    if (SPINDLE_LASER_PWM_INVERT) spindle_laser_power = 255 - spindle_laser_power;
    analogWrite(SPINDLE_LASER_PWM_PIN, spindle_laser_power);
  }

  inline void gcode_M3_M4(bool is_M3) {

    stepper.synchronize();   // wait until previous movement commands (G0/G0/G2/G3) have completed before playing with the spindle
    #if SPINDLE_DIR_CHANGE
      const bool rotation_dir = (is_M3 && !SPINDLE_INVERT_DIR || !is_M3 && SPINDLE_INVERT_DIR) ? HIGH : LOW;
      if (SPINDLE_STOP_ON_DIR_CHANGE \
         && READ(SPINDLE_LASER_ENABLE_PIN) == SPINDLE_LASER_ENABLE_INVERT \
         && READ(SPINDLE_DIR_PIN) != rotation_dir
      ) {
        WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);  // turn spindle off
        delay_for_power_down();
      }
      WRITE(SPINDLE_DIR_PIN, rotation_dir);
    #endif

    /**
     * Our final value for ocr_val is an unsigned 8 bit value between 0 and 255 which usually means uint8_t.
     * Went to uint16_t because some of the uint8_t calculations would sometimes give 1000 0000 rather than 1111 1111.
     * Then needed to AND the uint16_t result with 0x00FF to make sure we only wrote the byte of interest.
     */
    #if ENABLED(SPINDLE_LASER_PWM)
      if (parser.seen('O')) ocr_val_mode();
      else {
        const float spindle_laser_power = parser.floatval('S');
        if (spindle_laser_power == 0) {
          WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);                                    // turn spindle off (active low)
          analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);                           // only write low byte
          delay_for_power_down();
        }
        else {
          int16_t ocr_val = (spindle_laser_power - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));  // convert RPM to PWM duty cycle
          NOMORE(ocr_val, 255);                                                                             // limit to max the Atmel PWM will support
          if (spindle_laser_power <= SPEED_POWER_MIN)
            ocr_val = (SPEED_POWER_MIN - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));            // minimum setting
          if (spindle_laser_power >= SPEED_POWER_MAX)
            ocr_val = (SPEED_POWER_MAX - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));            // limit to max RPM
          if (SPINDLE_LASER_PWM_INVERT) ocr_val = 255 - ocr_val;
          WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT);                                     // turn spindle on (active low)
          analogWrite(SPINDLE_LASER_PWM_PIN, ocr_val & 0xFF);                                               // only write low byte
          delay_for_power_up();
        }
      }
    #else
      WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT); // turn spindle on (active low) if spindle speed option not enabled
      delay_for_power_up();
    #endif
  }

 /**
  * M5 turn off spindle
  */
  inline void gcode_M5() {
    stepper.synchronize();
    WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);
    #if ENABLED(SPINDLE_LASER_PWM)
      analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);
    #endif
    delay_for_power_down();
  }

#endif // SPINDLE_LASER_ENABLE

/**
 * M17: Enable power on all stepper motors
 */
inline void gcode_M17() {
  LCD_MESSAGEPGM(MSG_NO_MOVE);
  enable_all_steppers();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void do_pause_e_move(const float &length, const float &fr) {
    set_destination_from_current();
    destination[E_AXIS] += length / planner.e_factor[active_extruder];
    planner.buffer_line_kinematic(destination, fr, active_extruder);
    stepper.synchronize();
    set_current_from_destination();
  }

  static float resume_position[XYZE];
  int8_t did_pause_print = 0;

  #if HAS_BUZZER
    static void filament_change_beep(const int8_t max_beep_count, const bool init=false) {
      static millis_t next_buzz = 0;
      static int8_t runout_beep = 0;

      if (init) next_buzz = runout_beep = 0;

      const millis_t ms = millis();
      if (ELAPSED(ms, next_buzz)) {
        if (max_beep_count < 0 || runout_beep < max_beep_count + 5) { // Only beep as long as we're supposed to
          next_buzz = ms + ((max_beep_count < 0 || runout_beep < max_beep_count) ? 1000 : 500);
          BUZZ(50, 880 - (runout_beep & 1) * 220);
          runout_beep++;
        }
      }
    }
  #endif

  /**
   * Ensure a safe temperature for extrusion
   *
   * - Fail if the TARGET temperature is too low
   * - Display LCD placard with temperature status
   * - Return when heating is done or aborted
   *
   * Returns 'true' if heating was completed, 'false' for abort
   */
  static bool ensure_safe_temperature(const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT) {

    #if ENABLED(PREVENT_COLD_EXTRUSION)
      if (!DEBUGGING(DRYRUN) && thermalManager.targetTooColdToExtrude(active_extruder)) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_HOTEND_TOO_COLD);
        return false;
      }
    #endif

    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT, mode);
    #else
      UNUSED(mode);
    #endif

    wait_for_heatup = true; // M108 will clear this
    while (wait_for_heatup && thermalManager.wait_for_heating(active_extruder)) idle();
    const bool status = wait_for_heatup;
    wait_for_heatup = false;

    return status;
  }

  /**
   * Load filament into the hotend
   *
   * - Fail if the a safe temperature was not reached
   * - If pausing for confirmation, wait for a click or M108
   * - Show "wait for load" placard
   * - Load and purge filament
   * - Show "Purge more" / "Continue" menu
   * - Return when "Continue" is selected
   *
   * Returns 'true' if load was completed, 'false' for abort
   */
  static bool load_filament(const float &load_length=0, const float &purge_length=0, const int8_t max_beep_count=0,
                            const bool show_lcd=false, const bool pause_for_user=false,
                            const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT
  ) {
    #if DISABLED(ULTIPANEL)
      UNUSED(show_lcd);
    #endif

    if (!ensure_safe_temperature(mode)) {
      #if ENABLED(ULTIPANEL)
        if (show_lcd) // Show status screen
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
      #endif

      return false;
    }

    if (pause_for_user) {
      #if ENABLED(ULTIPANEL)
        if (show_lcd) // Show "insert filament"
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT, mode);
      #endif
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM(MSG_FILAMENT_CHANGE_INSERT);

      #if HAS_BUZZER
        filament_change_beep(max_beep_count, true);
      #else
        UNUSED(max_beep_count);
      #endif

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      wait_for_user = true;    // LCD click or M108 will clear this
      while (wait_for_user) {
        #if HAS_BUZZER
          filament_change_beep(max_beep_count);
        #endif
        idle(true);
      }
      KEEPALIVE_STATE(IN_HANDLER);
    }

    #if ENABLED(ULTIPANEL)
      if (show_lcd) // Show "wait for load" message
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_LOAD, mode);
    #endif

    // Load filament
    if (load_length) do_pause_e_move(load_length, FILAMENT_CHANGE_LOAD_FEEDRATE);

    do {
      if (purge_length > 0) {
        // "Wait for filament purge"
        #if ENABLED(ULTIPANEL)
          if (show_lcd)
            lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_PURGE, mode);
        #endif

        // Extrude filament to get into hotend
        do_pause_e_move(purge_length, ADVANCED_PAUSE_EXTRUDE_FEEDRATE);
      }

      // Show "Purge More" / "Resume" menu and wait for reply
      #if ENABLED(ULTIPANEL)
        if (show_lcd) {
          KEEPALIVE_STATE(PAUSED_FOR_USER);
          wait_for_user = false;
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_OPTION, mode);
          while (advanced_pause_menu_response == ADVANCED_PAUSE_RESPONSE_WAIT_FOR) idle(true);
          KEEPALIVE_STATE(IN_HANDLER);
        }
      #endif

      // Keep looping if "Purge More" was selected
    } while (
      #if ENABLED(ULTIPANEL)
        show_lcd && advanced_pause_menu_response == ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE
      #else
        0
      #endif
    );

    return true;
  }

  /**
   * Unload filament from the hotend
   *
   * - Fail if the a safe temperature was not reached
   * - Show "wait for unload" placard
   * - Retract, pause, then unload filament
   * - Disable E stepper (on most machines)
   *
   * Returns 'true' if unload was completed, 'false' for abort
   */
  static bool unload_filament(const float &unload_length, const bool show_lcd=false,
                              const AdvancedPauseMode mode=ADVANCED_PAUSE_MODE_PAUSE_PRINT
  ) {
    if (!ensure_safe_temperature(mode)) {
      #if ENABLED(ULTIPANEL)
        if (show_lcd) // Show status screen
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
      #endif

      return false;
    }

    #if DISABLED(ULTIPANEL)
      UNUSED(show_lcd);
    #else
      if (show_lcd)
        lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_UNLOAD, mode);
    #endif

    // Retract filament
    do_pause_e_move(-FILAMENT_UNLOAD_RETRACT_LENGTH, PAUSE_PARK_RETRACT_FEEDRATE);

    // Wait for filament to cool
    safe_delay(FILAMENT_UNLOAD_DELAY);

    // Quickly purge
    do_pause_e_move(FILAMENT_UNLOAD_RETRACT_LENGTH + FILAMENT_UNLOAD_PURGE_LENGTH, planner.max_feedrate_mm_s[E_AXIS]);

    // Unload filament
    do_pause_e_move(unload_length, FILAMENT_CHANGE_UNLOAD_FEEDRATE);

    // Disable extruders steppers for manual filament changing (only on boards that have separate ENABLE_PINS)
    #if E0_ENABLE_PIN != X_ENABLE_PIN && E1_ENABLE_PIN != Y_ENABLE_PIN
      disable_e_stepper(active_extruder);
      safe_delay(100);
    #endif

    return true;
  }

  /**
   * Pause procedure
   *
   * - Abort if already paused
   * - Send host action for pause, if configured
   * - Abort if TARGET temperature is too low
   * - Display "wait for start of filament change" (if a length was specified)
   * - Initial retract, if current temperature is hot enough
   * - Park the nozzle at the given position
   * - Call unload_filament (if a length was specified)
   *
   * Returns 'true' if pause was completed, 'false' for abort
   */
  static bool pause_print(const float &retract, const point_t &park_point, const float &unload_length=0, const bool show_lcd=false) {
    if (did_pause_print) return false; // already paused

    #ifdef ACTION_ON_PAUSE
      SERIAL_ECHOLNPGM("//action:" ACTION_ON_PAUSE);
    #endif

    if (!DEBUGGING(DRYRUN) && unload_length && thermalManager.targetTooColdToExtrude(active_extruder)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_HOTEND_TOO_COLD);

      #if ENABLED(ULTIPANEL)
        if (show_lcd) // Show status screen
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
          LCD_MESSAGEPGM(MSG_M600_TOO_COLD);
      #endif

      return false; // unable to reach safe temperature
    }

    // Indicate that the printer is paused
    ++did_pause_print;

    // Pause the print job and timer
    #if ENABLED(SDSUPPORT)
      if (card.sdprinting) {
        card.pauseSDPrint();
        ++did_pause_print; // Indicate SD pause also
      }
    #endif
    print_job_timer.pause();

    // Wait for synchronize steppers
    stepper.synchronize();

    // Save current position
    COPY(resume_position, current_position);

    // Initial retract before move to filament change position
    if (retract && thermalManager.hotEnoughToExtrude(active_extruder))
      do_pause_e_move(retract, PAUSE_PARK_RETRACT_FEEDRATE);

    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      if (!axis_unhomed_error())
    #endif
        // Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
        Nozzle::park(2, park_point);

    // Unload the filament
    if (unload_length)
      unload_filament(unload_length, show_lcd);

    return true;
  }

  /**
   * - Show "Insert filament and press button to continue"
   * - Wait for a click before returning
   * - Heaters can time out, reheated before accepting a click
   *
   * Used by M125 and M600
   */
  static void wait_for_filament_reload(const int8_t max_beep_count=0) {
    bool nozzle_timed_out = false;

    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT);
    #endif
    SERIAL_ECHO_START();
    SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_INSERT);

    #if HAS_BUZZER
      filament_change_beep(max_beep_count, true);
    #endif

    // Start the heater idle timers
    const millis_t nozzle_timeout = (millis_t)(PAUSE_PARK_NOZZLE_TIMEOUT) * 1000UL;

    HOTEND_LOOP()
      thermalManager.start_heater_idle_timer(e, nozzle_timeout);

    // Wait for filament insert by user and press button
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    wait_for_user = true;    // LCD click or M108 will clear this
    while (wait_for_user) {
      #if HAS_BUZZER
        filament_change_beep(max_beep_count);
      #endif

      // If the nozzle has timed out, wait for the user to press the button to re-heat the nozzle, then
      // re-heat the nozzle, re-show the insert screen, restart the idle timers, and start over
      if (!nozzle_timed_out)
        HOTEND_LOOP()
          nozzle_timed_out |= thermalManager.is_heater_idle(e);

      if (nozzle_timed_out) {
        #if ENABLED(ULTIPANEL)
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE);
        #endif
        SERIAL_ECHO_START();
        #if ENABLED(ULTIPANEL) && ENABLED(EMERGENCY_PARSER)
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_HEAT);
        #elif ENABLED(EMERGENCY_PARSER)
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_HEAT_M108);
        #else
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_HEAT_LCD);
        #endif

        // Wait for LCD click or M108
        while (wait_for_user) idle(true);

        // Re-enable the heaters if they timed out
        HOTEND_LOOP() thermalManager.reset_heater_idle_timer(e);

        // Wait for the heaters to reach the target temperatures
        ensure_safe_temperature();

        #if ENABLED(ULTIPANEL)
          lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INSERT);
        #endif
        SERIAL_ECHO_START();
        #if ENABLED(ULTIPANEL) && ENABLED(EMERGENCY_PARSER)
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_INSERT);
        #elif ENABLED(EMERGENCY_PARSER)
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_INSERT_M108);
        #else
          SERIAL_ERRORLNPGM(MSG_FILAMENT_CHANGE_INSERT_LCD);
        #endif

        // Start the heater idle timers
        const millis_t nozzle_timeout = (millis_t)(PAUSE_PARK_NOZZLE_TIMEOUT) * 1000UL;

        HOTEND_LOOP()
          thermalManager.start_heater_idle_timer(e, nozzle_timeout);

        wait_for_user = true; // Wait for user to load filament
        nozzle_timed_out = false;

        #if HAS_BUZZER
          filament_change_beep(max_beep_count, true);
        #endif
      }

      idle(true);
    }
    KEEPALIVE_STATE(IN_HANDLER);
  }

  /**
   * Resume or Start print procedure
   *
   * - Abort if not paused
   * - Reset heater idle timers
   * - Load filament if specified, but only if:
   *   - a nozzle timed out, or
   *   - the nozzle is already heated.
   * - Display "wait for print to resume"
   * - Re-prime the nozzle...
   *   -  FWRETRACT: Recover/prime from the prior G10.
   *   - !FWRETRACT: Retract by resume_position[E], if negative.
   *                 Not sure how this logic comes into use.
   * - Move the nozzle back to resume_position
   * - Sync the planner E to resume_position[E]
   * - Send host action for resume, if configured
   * - Resume the current SD print job, if any
   */
  static void resume_print(const float &load_length=0, const float &purge_length=ADVANCED_PAUSE_EXTRUDE_LENGTH, const int8_t max_beep_count=0) {
    if (!did_pause_print) return;

    // Re-enable the heaters if they timed out
    bool nozzle_timed_out = false;
    HOTEND_LOOP() {
      nozzle_timed_out |= thermalManager.is_heater_idle(e);
      thermalManager.reset_heater_idle_timer(e);
    }

    if (nozzle_timed_out || thermalManager.hotEnoughToExtrude(active_extruder)) {
      // Load the new filament
      load_filament(load_length, purge_length, max_beep_count, true, nozzle_timed_out);
    }

    #if ENABLED(ULTIPANEL)
      // "Wait for print to resume"
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_RESUME);
    #endif

    // Intelligent resuming
    #if ENABLED(FWRETRACT)
      // If retracted before goto pause
      if (fwretract.retracted[active_extruder])
        do_pause_e_move(-fwretract.retract_length, fwretract.retract_feedrate_mm_s);
    #endif

    // If resume_position is negative
    if (resume_position[E_AXIS] < 0) do_pause_e_move(resume_position[E_AXIS], PAUSE_PARK_RETRACT_FEEDRATE);

    // Move XY to starting position, then Z
    do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

    // Set Z_AXIS to saved position
    do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);

    // Now all extrusion positions are resumed and ready to be confirmed
    // Set extruder to saved position
    planner.set_e_position_mm((destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]));

    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
      filament_ran_out = false;
    #endif

    #if ENABLED(ULTIPANEL)
      // Show status screen
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
    #endif

    #ifdef ACTION_ON_RESUME
      SERIAL_ECHOLNPGM("//action:" ACTION_ON_RESUME);
    #endif

    --did_pause_print;

    #if ENABLED(SDSUPPORT)
      if (did_pause_print) {
        card.startFileprint();
        --did_pause_print;
      }
    #endif
  }

#endif // ADVANCED_PAUSE_FEATURE

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
  inline void gcode_M21() { card.initsd(); }

  /**
   * M22: Release SD Card
   */
  inline void gcode_M22() { card.release(); }

  /**
   * M23: Open a file
   */
  inline void gcode_M23() {
    // Simplify3D includes the size, so zero out all spaces (#7227)
    for (char *fn = parser.string_arg; *fn; ++fn) if (*fn == ' ') *fn = '\0';
    card.openFile(parser.string_arg, true);
  }

  /**
   * M24: Start or Resume SD Print
   */
  inline void gcode_M24() {
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      resume_print();
    #endif

    card.startFileprint();
    print_job_timer.start();
  }

  /**
   * M25: Pause SD Print
   */
  inline void gcode_M25() {
    card.pauseSDPrint();
    print_job_timer.pause();

    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M125")); // Must be enqueued with pauseSDPrint set to be last in the buffer
    #endif
  }

  /**
   * M26: Set SD Card file index
   */
  inline void gcode_M26() {
    if (card.cardOK && parser.seenval('S'))
      card.setIndex(parser.value_long());
  }

  /**
   * M27: Get SD Card status
   */
  inline void gcode_M27() { card.getStatus(); }

  /**
   * M28: Start SD Write
   */
  inline void gcode_M28() { card.openFile(parser.string_arg, false); }

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
      card.removeFile(parser.string_arg);
    }
  }

#endif // SDSUPPORT

/**
 * M31: Get the time since the start of SD Print (or last M109)
 */
inline void gcode_M31() {
  char buffer[21];
  duration_t elapsed = print_job_timer.duration();
  elapsed.toString(buffer);
  lcd_setstatus(buffer);

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR("Print time: ", buffer);
}

#if ENABLED(SDSUPPORT)

  /**
   * M32: Select file and start SD Print
   *
   * Examples:
   *
   *    M32 !PATH/TO/FILE.GCO#      ; Start FILE.GCO
   *    M32 P !PATH/TO/FILE.GCO#    ; Start FILE.GCO as a procedure
   *    M32 S60 !PATH/TO/FILE.GCO#  ; Start FILE.GCO at byte 60
   *
   */
  inline void gcode_M32() {
    if (card.sdprinting) stepper.synchronize();

    if (card.cardOK) {
      const bool call_procedure = parser.boolval('P');

      card.openFile(parser.string_arg, true, call_procedure);

      if (parser.seenval('S')) card.setIndex(parser.value_long());

      card.startFileprint();

      // Procedure calls count as normal print time.
      if (!call_procedure) print_job_timer.start();
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
      card.printLongPath(parser.string_arg);
    }

  #endif

  #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
    /**
     * M34: Set SD Card Sorting Options
     */
    inline void gcode_M34() {
      if (parser.seen('S')) card.setSortOn(parser.value_bool());
      if (parser.seenval('F')) {
        const int v = parser.value_long();
        card.setSortFolders(v < 0 ? -1 : v > 0 ? 1 : 0);
      }
      //if (parser.seen('R')) card.setSortReverse(parser.value_bool());
    }
  #endif // SDCARD_SORT_ALPHA && SDSORT_GCODE

  /**
   * M928: Start SD Write
   */
  inline void gcode_M928() {
    card.openLogFile(parser.string_arg);
  }

#endif // SDSUPPORT

/**
 * Sensitive pin test for M42, M226
 */
static bool pin_is_protected(const pin_t pin) {
  static const pin_t sensitive_pins[] PROGMEM = SENSITIVE_PINS;
  for (uint8_t i = 0; i < COUNT(sensitive_pins); i++)
    if (pin == (pin_t)pgm_read_byte(&sensitive_pins[i])) return true;
  return false;
}

/**
 * M42: Change pin status via GCode
 *
 *  P<pin>  Pin number (LED if omitted)
 *  S<byte> Pin status from 0 - 255
 */
inline void gcode_M42() {
  if (!parser.seenval('S')) return;
  const byte pin_status = parser.value_byte();

  const pin_t pin_number = parser.byteval('P', LED_PIN);
  if (pin_number < 0) return;

  if (pin_is_protected(pin_number)) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_PROTECTED_PIN);
    return;
  }

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
}

#if ENABLED(PINS_DEBUGGING)

  #include "pinsDebug.h"

  inline void toggle_pins() {
    const bool I_flag = parser.boolval('I');
    const int repeat = parser.intval('R', 1),
              start = parser.intval('S'),
              end = parser.intval('L', NUM_DIGITAL_PINS - 1),
              wait = parser.intval('W', 500);

    for (uint8_t pin = start; pin <= end; pin++) {
      //report_pin_state_extended(pin, I_flag, false);

      if (!I_flag && pin_is_protected(pin)) {
        report_pin_state_extended(pin, I_flag, true, "Untouched ");
        SERIAL_EOL();
      }
      else {
        report_pin_state_extended(pin, I_flag, true, "Pulsing   ");
        #if AVR_AT90USB1286_FAMILY // Teensy IDEs don't know about these pins so must use FASTIO
          if (pin == TEENSY_E2) {
            SET_OUTPUT(TEENSY_E2);
            for (int16_t j = 0; j < repeat; j++) {
              WRITE(TEENSY_E2, LOW);  safe_delay(wait);
              WRITE(TEENSY_E2, HIGH); safe_delay(wait);
              WRITE(TEENSY_E2, LOW);  safe_delay(wait);
            }
          }
          else if (pin == TEENSY_E3) {
            SET_OUTPUT(TEENSY_E3);
            for (int16_t j = 0; j < repeat; j++) {
              WRITE(TEENSY_E3, LOW);  safe_delay(wait);
              WRITE(TEENSY_E3, HIGH); safe_delay(wait);
              WRITE(TEENSY_E3, LOW);  safe_delay(wait);
            }
          }
          else
        #endif
        {
          pinMode(pin, OUTPUT);
          for (int16_t j = 0; j < repeat; j++) {
            digitalWrite(pin, 0); safe_delay(wait);
            digitalWrite(pin, 1); safe_delay(wait);
            digitalWrite(pin, 0); safe_delay(wait);
          }
        }

      }
      SERIAL_EOL();
    }
    SERIAL_ECHOLNPGM("Done.");

  } // toggle_pins

  inline void servo_probe_test() {
    #if !(NUM_SERVOS > 0 && HAS_SERVO_0)

      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("SERVO not setup");

    #elif !HAS_Z_SERVO_ENDSTOP

      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Z_ENDSTOP_SERVO_NR not setup");

    #else // HAS_Z_SERVO_ENDSTOP

      const uint8_t probe_index = parser.byteval('P', Z_ENDSTOP_SERVO_NR);

      SERIAL_PROTOCOLLNPGM("Servo probe test");
      SERIAL_PROTOCOLLNPAIR(".  using index:  ", probe_index);
      SERIAL_PROTOCOLLNPAIR(".  deploy angle: ", z_servo_angle[0]);
      SERIAL_PROTOCOLLNPAIR(".  stow angle:   ", z_servo_angle[1]);

      bool probe_inverting;

      #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)

        #define PROBE_TEST_PIN Z_MIN_PIN

        SERIAL_PROTOCOLLNPAIR(". probe uses Z_MIN pin: ", PROBE_TEST_PIN);
        SERIAL_PROTOCOLLNPGM(". uses Z_MIN_ENDSTOP_INVERTING (ignores Z_MIN_PROBE_ENDSTOP_INVERTING)");
        SERIAL_PROTOCOLPGM(". Z_MIN_ENDSTOP_INVERTING: ");

        #if Z_MIN_ENDSTOP_INVERTING
          SERIAL_PROTOCOLLNPGM("true");
        #else
          SERIAL_PROTOCOLLNPGM("false");
        #endif

        probe_inverting = Z_MIN_ENDSTOP_INVERTING;

      #elif ENABLED(Z_MIN_PROBE_ENDSTOP)

        #define PROBE_TEST_PIN Z_MIN_PROBE_PIN
        SERIAL_PROTOCOLLNPAIR(". probe uses Z_MIN_PROBE_PIN: ", PROBE_TEST_PIN);
        SERIAL_PROTOCOLLNPGM(". uses Z_MIN_PROBE_ENDSTOP_INVERTING (ignores Z_MIN_ENDSTOP_INVERTING)");
        SERIAL_PROTOCOLPGM(". Z_MIN_PROBE_ENDSTOP_INVERTING: ");

        #if Z_MIN_PROBE_ENDSTOP_INVERTING
          SERIAL_PROTOCOLLNPGM("true");
        #else
          SERIAL_PROTOCOLLNPGM("false");
        #endif

        probe_inverting = Z_MIN_PROBE_ENDSTOP_INVERTING;

      #endif

      SERIAL_PROTOCOLLNPGM(". deploy & stow 4 times");
      SET_INPUT_PULLUP(PROBE_TEST_PIN);
      bool deploy_state, stow_state;
      for (uint8_t i = 0; i < 4; i++) {
        MOVE_SERVO(probe_index, z_servo_angle[0]); //deploy
        safe_delay(500);
        deploy_state = READ(PROBE_TEST_PIN);
        MOVE_SERVO(probe_index, z_servo_angle[1]); //stow
        safe_delay(500);
        stow_state = READ(PROBE_TEST_PIN);
      }
      if (probe_inverting != deploy_state) SERIAL_PROTOCOLLNPGM("WARNING - INVERTING setting probably backwards");

      refresh_cmd_timeout();

      if (deploy_state != stow_state) {
        SERIAL_PROTOCOLLNPGM("BLTouch clone detected");
        if (deploy_state) {
          SERIAL_PROTOCOLLNPGM(".  DEPLOYED state: HIGH (logic 1)");
          SERIAL_PROTOCOLLNPGM(".  STOWED (triggered) state: LOW (logic 0)");
        }
        else {
          SERIAL_PROTOCOLLNPGM(".  DEPLOYED state: LOW (logic 0)");
          SERIAL_PROTOCOLLNPGM(".  STOWED (triggered) state: HIGH (logic 1)");
        }
        #if ENABLED(BLTOUCH)
          SERIAL_PROTOCOLLNPGM("ERROR: BLTOUCH enabled - set this device up as a Z Servo Probe with inverting as true.");
        #endif

      }
      else {                                           // measure active signal length
        MOVE_SERVO(probe_index, z_servo_angle[0]);     // deploy
        safe_delay(500);
        SERIAL_PROTOCOLLNPGM("please trigger probe");
        uint16_t probe_counter = 0;

        // Allow 30 seconds max for operator to trigger probe
        for (uint16_t j = 0; j < 500 * 30 && probe_counter == 0 ; j++) {

          safe_delay(2);

          if (0 == j % (500 * 1)) // keep cmd_timeout happy
            refresh_cmd_timeout();

          if (deploy_state != READ(PROBE_TEST_PIN)) { // probe triggered

            for (probe_counter = 1; probe_counter < 50 && deploy_state != READ(PROBE_TEST_PIN); ++probe_counter)
              safe_delay(2);

            if (probe_counter == 50)
              SERIAL_PROTOCOLLNPGM("Z Servo Probe detected"); // >= 100mS active time
            else if (probe_counter >= 2)
              SERIAL_PROTOCOLLNPAIR("BLTouch compatible probe detected - pulse width (+/- 4mS): ", probe_counter * 2); // allow 4 - 100mS pulse
            else
              SERIAL_PROTOCOLLNPGM("noise detected - please re-run test"); // less than 2mS pulse

            MOVE_SERVO(probe_index, z_servo_angle[1]); //stow

          }  // pulse detected

        } // for loop waiting for trigger

        if (probe_counter == 0) SERIAL_PROTOCOLLNPGM("trigger not detected");

      } // measure active signal length

    #endif

  } // servo_probe_test

  /**
   * M43: Pin debug - report pin state, watch pins, toggle pins and servo probe test/report
   *
   *  M43         - report name and state of pin(s)
   *                  P<pin>  Pin to read or watch. If omitted, reads all pins.
   *                  I       Flag to ignore Marlin's pin protection.
   *
   *  M43 W       - Watch pins -reporting changes- until reset, click, or M108.
   *                  P<pin>  Pin to read or watch. If omitted, read/watch all pins.
   *                  I       Flag to ignore Marlin's pin protection.
   *
   *  M43 E<bool> - Enable / disable background endstop monitoring
   *                  - Machine continues to operate
   *                  - Reports changes to endstops
   *                  - Toggles LED_PIN when an endstop changes
   *                  - Can not reliably catch the 5mS pulse from BLTouch type probes
   *
   *  M43 T       - Toggle pin(s) and report which pin is being toggled
   *                  S<pin>  - Start Pin number.   If not given, will default to 0
   *                  L<pin>  - End Pin number.   If not given, will default to last pin defined for this board
   *                  I<bool> - Flag to ignore Marlin's pin protection.   Use with caution!!!!
   *                  R       - Repeat pulses on each pin this number of times before continueing to next pin
   *                  W       - Wait time (in miliseconds) between pulses.  If not given will default to 500
   *
   *  M43 S       - Servo probe test
   *                  P<index> - Probe index (optional - defaults to 0
   */
  inline void gcode_M43() {

    if (parser.seen('T')) {   // must be first or else its "S" and "E" parameters will execute endstop or servo test
      toggle_pins();
      return;
    }

    // Enable or disable endstop monitoring
    if (parser.seen('E')) {
      endstop_monitor_flag = parser.value_bool();
      SERIAL_PROTOCOLPGM("endstop monitor ");
      serialprintPGM(endstop_monitor_flag ? PSTR("en") : PSTR("dis"));
      SERIAL_PROTOCOLLNPGM("abled");
      return;
    }

    if (parser.seen('S')) {
      servo_probe_test();
      return;
    }

    // Get the range of pins to test or watch
    const pin_t first_pin = parser.byteval('P'),
                last_pin = parser.seenval('P') ? first_pin : NUM_DIGITAL_PINS - 1;

    if (first_pin > last_pin) return;

    const bool ignore_protection = parser.boolval('I');

    // Watch until click, M108, or reset
    if (parser.boolval('W')) {
      SERIAL_PROTOCOLLNPGM("Watching pins");
      byte pin_state[last_pin - first_pin + 1];
      for (pin_t pin = first_pin; pin <= last_pin; pin++) {
        if (pin_is_protected(pin) && !ignore_protection) continue;
        pinMode(pin, INPUT_PULLUP);
        delay(1);
        /*
          if (IS_ANALOG(pin))
            pin_state[pin - first_pin] = analogRead(pin - analogInputToDigitalPin(0)); // int16_t pin_state[...]
          else
        //*/
            pin_state[pin - first_pin] = digitalRead(pin);
      }

      #if HAS_RESUME_CONTINUE
        wait_for_user = true;
        KEEPALIVE_STATE(PAUSED_FOR_USER);
      #endif

      for (;;) {
        for (pin_t pin = first_pin; pin <= last_pin; pin++) {
          if (pin_is_protected(pin) && !ignore_protection) continue;
          const byte val =
            /*
              IS_ANALOG(pin)
                ? analogRead(pin - analogInputToDigitalPin(0)) : // int16_t val
                :
            //*/
              digitalRead(pin);
          if (val != pin_state[pin - first_pin]) {
            report_pin_state_extended(pin, ignore_protection, false);
            pin_state[pin - first_pin] = val;
          }
        }

        #if HAS_RESUME_CONTINUE
          if (!wait_for_user) {
            KEEPALIVE_STATE(IN_HANDLER);
            break;
          }
        #endif

        safe_delay(200);
      }
      return;
    }

    // Report current state of selected pin(s)
    for (pin_t pin = first_pin; pin <= last_pin; pin++)
      report_pin_state_extended(pin, ignore_protection, true);
  }

#endif // PINS_DEBUGGING

#if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)

  /**
   * M48: Z probe repeatability measurement function.
   *
   * Usage:
   *   M48 <P#> <X#> <Y#> <V#> <E> <L#> <S>
   *     P = Number of sampled points (4-50, default 10)
   *     X = Sample X position
   *     Y = Sample Y position
   *     V = Verbose level (0-4, default=1)
   *     E = Engage Z probe for each reading
   *     L = Number of legs of movement before probe
   *     S = Schizoid (Or Star if you prefer)
   *
   * This function requires the machine to be homed before invocation.
   */
  inline void gcode_M48() {

    if (axis_unhomed_error()) return;

    const int8_t verbose_level = parser.byteval('V', 1);
    if (!WITHIN(verbose_level, 0, 4)) {
      SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).");
      return;
    }

    if (verbose_level > 0)
      SERIAL_PROTOCOLLNPGM("M48 Z-Probe Repeatability Test");

    const int8_t n_samples = parser.byteval('P', 10);
    if (!WITHIN(n_samples, 4, 50)) {
      SERIAL_PROTOCOLLNPGM("?Sample size not plausible (4-50).");
      return;
    }

    const bool stow_probe_after_each = parser.boolval('E');

    float X_current = current_position[X_AXIS],
          Y_current = current_position[Y_AXIS];

    const float X_probe_location = parser.linearval('X', X_current + X_PROBE_OFFSET_FROM_EXTRUDER),
                Y_probe_location = parser.linearval('Y', Y_current + Y_PROBE_OFFSET_FROM_EXTRUDER);

    #if DISABLED(DELTA)
      if (!WITHIN(X_probe_location, MIN_PROBE_X, MAX_PROBE_X)) {
        out_of_range_error(PSTR("X"));
        return;
      }
      if (!WITHIN(Y_probe_location, MIN_PROBE_Y, MAX_PROBE_Y)) {
        out_of_range_error(PSTR("Y"));
        return;
      }
    #else
      if (!position_is_reachable_by_probe(X_probe_location, Y_probe_location)) {
        SERIAL_PROTOCOLLNPGM("? (X,Y) location outside of probeable radius.");
        return;
      }
    #endif

    bool seen_L = parser.seen('L');
    uint8_t n_legs = seen_L ? parser.value_byte() : 0;
    if (n_legs > 15) {
      SERIAL_PROTOCOLLNPGM("?Number of legs in movement not plausible (0-15).");
      return;
    }
    if (n_legs == 1) n_legs = 2;

    const bool schizoid_flag = parser.boolval('S');
    if (schizoid_flag && !seen_L) n_legs = 7;

    /**
     * Now get everything to the specified probe point So we can safely do a
     * probe to get us close to the bed.  If the Z-Axis is far from the bed,
     * we don't want to use that as a starting point for each probe.
     */
    if (verbose_level > 2)
      SERIAL_PROTOCOLLNPGM("Positioning the probe...");

    // Disable bed level correction in M48 because we want the raw data when we probe

    #if HAS_LEVELING
      const bool was_enabled = planner.leveling_active;
      set_bed_leveling_enabled(false);
    #endif

    setup_for_endstop_or_probe_move();

    double mean = 0.0, sigma = 0.0, min = 99999.9, max = -99999.9, sample_set[n_samples];

    // Move to the first point, deploy, and probe
    const float t = probe_pt(X_probe_location, Y_probe_location, stow_probe_after_each, verbose_level);
    bool probing_good = !isnan(t);

    if (probing_good) {
      randomSeed(millis());

      for (uint8_t n = 0; n < n_samples; n++) {
        if (n_legs) {
          const int dir = (random(0, 10) > 5.0) ? -1 : 1;  // clockwise or counter clockwise
          float angle = random(0.0, 360.0);
          const float radius = random(
            #if ENABLED(DELTA)
              0.1250000000 * (DELTA_PROBEABLE_RADIUS),
              0.3333333333 * (DELTA_PROBEABLE_RADIUS)
            #else
              5.0, 0.125 * min(X_BED_SIZE, Y_BED_SIZE)
            #endif
          );

          if (verbose_level > 3) {
            SERIAL_ECHOPAIR("Starting radius: ", radius);
            SERIAL_ECHOPAIR("   angle: ", angle);
            SERIAL_ECHOPGM(" Direction: ");
            if (dir > 0) SERIAL_ECHOPGM("Counter-");
            SERIAL_ECHOLNPGM("Clockwise");
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

            X_current = X_probe_location - (X_PROBE_OFFSET_FROM_EXTRUDER) + cos(RADIANS(angle)) * radius;
            Y_current = Y_probe_location - (Y_PROBE_OFFSET_FROM_EXTRUDER) + sin(RADIANS(angle)) * radius;

            #if DISABLED(DELTA)
              X_current = constrain(X_current, X_MIN_POS, X_MAX_POS);
              Y_current = constrain(Y_current, Y_MIN_POS, Y_MAX_POS);
            #else
              // If we have gone out too far, we can do a simple fix and scale the numbers
              // back in closer to the origin.
              while (!position_is_reachable_by_probe(X_current, Y_current)) {
                X_current *= 0.8;
                Y_current *= 0.8;
                if (verbose_level > 3) {
                  SERIAL_ECHOPAIR("Pulling point towards center:", X_current);
                  SERIAL_ECHOLNPAIR(", ", Y_current);
                }
              }
            #endif
            if (verbose_level > 3) {
              SERIAL_PROTOCOLPGM("Going to:");
              SERIAL_ECHOPAIR(" X", X_current);
              SERIAL_ECHOPAIR(" Y", Y_current);
              SERIAL_ECHOLNPAIR(" Z", current_position[Z_AXIS]);
            }
            do_blocking_move_to_xy(X_current, Y_current);
          } // n_legs loop
        } // n_legs

        // Probe a single point
        sample_set[n] = probe_pt(X_probe_location, Y_probe_location, stow_probe_after_each, 0);

        // Break the loop if the probe fails
        probing_good = !isnan(sample_set[n]);
        if (!probing_good) break;

        /**
         * Get the current mean for the data points we have so far
         */
        double sum = 0.0;
        for (uint8_t j = 0; j <= n; j++) sum += sample_set[j];
        mean = sum / (n + 1);

        NOMORE(min, sample_set[n]);
        NOLESS(max, sample_set[n]);

        /**
         * Now, use that mean to calculate the standard deviation for the
         * data points we have so far
         */
        sum = 0.0;
        for (uint8_t j = 0; j <= n; j++)
          sum += sq(sample_set[j] - mean);

        sigma = SQRT(sum / (n + 1));
        if (verbose_level > 0) {
          if (verbose_level > 1) {
            SERIAL_PROTOCOL(n + 1);
            SERIAL_PROTOCOLPGM(" of ");
            SERIAL_PROTOCOL((int)n_samples);
            SERIAL_PROTOCOLPGM(": z: ");
            SERIAL_PROTOCOL_F(sample_set[n], 3);
            if (verbose_level > 2) {
              SERIAL_PROTOCOLPGM(" mean: ");
              SERIAL_PROTOCOL_F(mean, 4);
              SERIAL_PROTOCOLPGM(" sigma: ");
              SERIAL_PROTOCOL_F(sigma, 6);
              SERIAL_PROTOCOLPGM(" min: ");
              SERIAL_PROTOCOL_F(min, 3);
              SERIAL_PROTOCOLPGM(" max: ");
              SERIAL_PROTOCOL_F(max, 3);
              SERIAL_PROTOCOLPGM(" range: ");
              SERIAL_PROTOCOL_F(max-min, 3);
            }
            SERIAL_EOL();
          }
        }

      } // n_samples loop
    }

    STOW_PROBE();

    if (probing_good) {
      SERIAL_PROTOCOLLNPGM("Finished!");

      if (verbose_level > 0) {
        SERIAL_PROTOCOLPGM("Mean: ");
        SERIAL_PROTOCOL_F(mean, 6);
        SERIAL_PROTOCOLPGM(" Min: ");
        SERIAL_PROTOCOL_F(min, 3);
        SERIAL_PROTOCOLPGM(" Max: ");
        SERIAL_PROTOCOL_F(max, 3);
        SERIAL_PROTOCOLPGM(" Range: ");
        SERIAL_PROTOCOL_F(max-min, 3);
        SERIAL_EOL();
      }

      SERIAL_PROTOCOLPGM("Standard Deviation: ");
      SERIAL_PROTOCOL_F(sigma, 6);
      SERIAL_EOL();
      SERIAL_EOL();
    }

    clean_up_after_endstop_or_probe_move();

    // Re-enable bed level correction if it had been on
    #if HAS_LEVELING
      set_bed_leveling_enabled(was_enabled);
    #endif

    report_current_position();
  }

#endif // Z_MIN_PROBE_REPEATABILITY_TEST

#if ENABLED(G26_MESH_VALIDATION)

  inline void gcode_M49() {
    g26_debug_flag ^= true;
    SERIAL_PROTOCOLPGM("G26 Debug ");
    serialprintPGM(g26_debug_flag ? PSTR("on.\n") : PSTR("off.\n"));
  }

#endif // G26_MESH_VALIDATION

#if ENABLED(ULTRA_LCD) && ENABLED(LCD_SET_PROGRESS_MANUALLY)
  /**
   * M73: Set percentage complete (for display on LCD)
   *
   * Example:
   *   M73 P25 ; Set progress to 25%
   *
   * Notes:
   *   This has no effect during an SD print job
   */
  inline void gcode_M73() {
    if (!IS_SD_PRINTING && parser.seen('P')) {
      progress_bar_percent = parser.value_byte();
      NOMORE(progress_bar_percent, 100);
    }
  }
#endif // ULTRA_LCD && LCD_SET_PROGRESS_MANUALLY

/**
 * M75: Start print timer
 */
inline void gcode_M75() { print_job_timer.start(); }

/**
 * M76: Pause print timer
 */
inline void gcode_M76() { print_job_timer.pause(); }

/**
 * M77: Stop print timer
 */
inline void gcode_M77() { print_job_timer.stop(); }

#if ENABLED(PRINTCOUNTER)
  /**
   * M78: Show print statistics
   */
  inline void gcode_M78() {
    // "M78 S78" will reset the statistics
    if (parser.intval('S') == 78)
      print_job_timer.initStats();
    else
      print_job_timer.showStats();
  }
#endif

/**
 * M104: Set hot end temperature
 */
inline void gcode_M104() {
  if (get_target_extruder_from_command(104)) return;
  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(SINGLENOZZLE)
    if (target_extruder != active_extruder) return;
  #endif

  if (parser.seenval('S')) {
    const int16_t temp = parser.value_celsius();
    thermalManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Stop the timer at the end of print. Start is managed by 'heat and wait' M109.
       * We use half EXTRUDE_MINTEMP here to allow nozzles to be put into hot
       * standby mode, for instance in a dual extruder setup, without affecting
       * the running print timer.
       */
      if (parser.value_celsius() <= (EXTRUDE_MINTEMP) / 2) {
        print_job_timer.stop();
        LCD_MESSAGEPGM(WELCOME_MSG);
      }
    #endif

    #if ENABLED(ULTRA_LCD)
      if (parser.value_celsius() > thermalManager.degHotend(target_extruder))
        lcd_status_printf_P(0, PSTR("E%i %s"), target_extruder + 1, MSG_HEATING);
    #endif
  }

  #if ENABLED(AUTOTEMP)
    planner.autotemp_M104_M109();
  #endif
}

/**
 * M105: Read hot end and bed temperature
 */
inline void gcode_M105() {
  if (get_target_extruder_from_command(105)) return;

  #if HAS_TEMP_HOTEND || HAS_TEMP_BED
    SERIAL_PROTOCOLPGM(MSG_OK);
    thermalManager.print_heaterstates();
  #else // !HAS_TEMP_HOTEND && !HAS_TEMP_BED
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_EOL();
}

#if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)

  /**
   * M155: Set temperature auto-report interval. M155 S<seconds>
   */
  inline void gcode_M155() {
    if (parser.seenval('S'))
      thermalManager.set_auto_report_interval(parser.value_byte());
  }

#endif // AUTO_REPORT_TEMPERATURES

#if FAN_COUNT > 0

  /**
   * M106: Set Fan Speed
   *
   *  S<int>   Speed between 0-255
   *  P<index> Fan index, if more than one fan
   *
   * With EXTRA_FAN_SPEED enabled:
   *
   *  T<int>   Restore/Use/Set Temporary Speed:
   *           1     = Restore previous speed after T2
   *           2     = Use temporary speed set with T3-255
   *           3-255 = Set the speed for use with T2
   */
  inline void gcode_M106() {
    const uint8_t p = parser.byteval('P');
    if (p < FAN_COUNT) {
      #if ENABLED(EXTRA_FAN_SPEED)
        const int16_t t = parser.intval('T');
        if (t > 0) {
          switch (t) {
            case 1:
              fanSpeeds[p] = old_fanSpeeds[p];
              break;
            case 2:
              old_fanSpeeds[p] = fanSpeeds[p];
              fanSpeeds[p] = new_fanSpeeds[p];
              break;
            default:
              new_fanSpeeds[p] = min(t, 255);
              break;
          }
          return;
        }
      #endif // EXTRA_FAN_SPEED
      const uint16_t s = parser.ushortval('S', 255);
      fanSpeeds[p] = min(s, 255);
    }
  }

  /**
   * M107: Fan Off
   */
  inline void gcode_M107() {
    const uint16_t p = parser.ushortval('P');
    if (p < FAN_COUNT) fanSpeeds[p] = 0;
  }

#endif // FAN_COUNT > 0

#if DISABLED(EMERGENCY_PARSER)

  /**
   * M108: Stop the waiting for heaters in M109, M190, M303. Does not affect the target temperature.
   */
  inline void gcode_M108() { wait_for_heatup = false; }


  /**
   * M112: Emergency Stop
   */
  inline void gcode_M112() { kill(PSTR(MSG_KILLED)); }


  /**
   * M410: Quickstop - Abort all planned moves
   *
   * This will stop the carriages mid-move, so most likely they
   * will be out of sync with the stepper position after this.
   */
  inline void gcode_M410() { quickstop_stepper(); }

#endif

/**
 * M109: Sxxx Wait for extruder(s) to reach temperature. Waits only when heating.
 *       Rxxx Wait for extruder(s) to reach temperature. Waits when heating and cooling.
 */

#ifndef MIN_COOLING_SLOPE_DEG
  #define MIN_COOLING_SLOPE_DEG 1.50
#endif
#ifndef MIN_COOLING_SLOPE_TIME
  #define MIN_COOLING_SLOPE_TIME 60
#endif

inline void gcode_M109() {

  if (get_target_extruder_from_command(109)) return;
  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(SINGLENOZZLE)
    if (target_extruder != active_extruder) return;
  #endif

  const bool no_wait_for_cooling = parser.seenval('S');
  if (no_wait_for_cooling || parser.seenval('R')) {
    const int16_t temp = parser.value_celsius();
    thermalManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dual_x_carriage_mode == DXC_DUPLICATION_MODE && target_extruder == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Use half EXTRUDE_MINTEMP to allow nozzles to be put into hot
       * standby mode, (e.g., in a dual extruder setup) without affecting
       * the running print timer.
       */
      if (parser.value_celsius() <= (EXTRUDE_MINTEMP) / 2) {
        print_job_timer.stop();
        LCD_MESSAGEPGM(WELCOME_MSG);
      }
      else
        print_job_timer.start();
    #endif

    #if ENABLED(ULTRA_LCD)
      if (thermalManager.isHeatingHotend(target_extruder))
        lcd_status_printf_P(0, PSTR("E%i %s"), target_extruder + 1, MSG_HEATING);
    #endif
  }
  else return;

  #if ENABLED(AUTOTEMP)
    planner.autotemp_M104_M109();
  #endif

  #if TEMP_RESIDENCY_TIME > 0
    millis_t residency_start_ms = 0;
    // Loop until the temperature has stabilized
    #define TEMP_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_RESIDENCY_TIME) * 1000UL))
  #else
    // Loop until the temperature is very close target
    #define TEMP_CONDITIONS (wants_to_cool ? thermalManager.isCoolingHotend(target_extruder) : thermalManager.isHeatingHotend(target_extruder))
  #endif

  float target_temp = -1.0, old_temp = 9999.0;
  bool wants_to_cool = false;
  wait_for_heatup = true;
  millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  #if ENABLED(PRINTER_EVENT_LEDS)
    const float start_temp = thermalManager.degHotend(target_extruder);
    uint8_t old_blue = 0;
  #endif

  do {
    // Target temperature might be changed during the loop
    if (target_temp != thermalManager.degTargetHotend(target_extruder)) {
      wants_to_cool = thermalManager.isCoolingHotend(target_extruder);
      target_temp = thermalManager.degTargetHotend(target_extruder);

      // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
      if (no_wait_for_cooling && wants_to_cool) break;
    }

    now = millis();
    if (ELAPSED(now, next_temp_ms)) { //Print temp & remaining time every 1s while waiting
      next_temp_ms = now + 1000UL;
      thermalManager.print_heaterstates();
      #if TEMP_RESIDENCY_TIME > 0
        SERIAL_PROTOCOLPGM(" W:");
        if (residency_start_ms)
          SERIAL_PROTOCOL(long((((TEMP_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
        else
          SERIAL_PROTOCOLCHAR('?');
      #endif
      SERIAL_EOL();
    }

    idle();
    refresh_cmd_timeout(); // to prevent stepper_inactive_time from running out

    const float temp = thermalManager.degHotend(target_extruder);

    #if ENABLED(PRINTER_EVENT_LEDS)
      // Gradually change LED strip from violet to red as nozzle heats up
      if (!wants_to_cool) {
        const uint8_t blue = map(constrain(temp, start_temp, target_temp), start_temp, target_temp, 255, 0);
        if (blue != old_blue) {
          old_blue = blue;
          leds.set_color(
            MakeLEDColor(255, 0, blue, 0, pixels.getBrightness())
            #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
              , true
            #endif
          );
        }
      }
    #endif

    #if TEMP_RESIDENCY_TIME > 0

      const float temp_diff = FABS(target_temp - temp);

      if (!residency_start_ms) {
        // Start the TEMP_RESIDENCY_TIME timer when we reach target temp for the first time.
        if (temp_diff < TEMP_WINDOW) residency_start_ms = now;
      }
      else if (temp_diff > TEMP_HYSTERESIS) {
        // Restart the timer whenever the temperature falls outside the hysteresis.
        residency_start_ms = now;
      }

    #endif

    // Prevent a wait-forever situation if R is misused i.e. M109 R0
    if (wants_to_cool) {
      // break after MIN_COOLING_SLOPE_TIME seconds
      // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG
      if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
        if (old_temp - temp < MIN_COOLING_SLOPE_DEG) break;
        next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME;
        old_temp = temp;
      }
    }

  } while (wait_for_heatup && TEMP_CONDITIONS);

  if (wait_for_heatup) {
    LCD_MESSAGEPGM(MSG_HEATING_COMPLETE);
    #if ENABLED(PRINTER_EVENT_LEDS)
      leds.set_white();
    #endif
  }

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(IN_HANDLER);
  #endif
}

#if HAS_TEMP_BED

  #ifndef MIN_COOLING_SLOPE_DEG_BED
    #define MIN_COOLING_SLOPE_DEG_BED 1.50
  #endif
  #ifndef MIN_COOLING_SLOPE_TIME_BED
    #define MIN_COOLING_SLOPE_TIME_BED 60
  #endif

  /**
   * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
   *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
   */
  inline void gcode_M190() {
    if (DEBUGGING(DRYRUN)) return;

    LCD_MESSAGEPGM(MSG_BED_HEATING);
    const bool no_wait_for_cooling = parser.seenval('S');
    if (no_wait_for_cooling || parser.seenval('R')) {
      thermalManager.setTargetBed(parser.value_celsius());
      #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
        if (parser.value_celsius() > BED_MINTEMP)
          print_job_timer.start();
      #endif
    }
    else return;

    #if TEMP_BED_RESIDENCY_TIME > 0
      millis_t residency_start_ms = 0;
      // Loop until the temperature has stabilized
      #define TEMP_BED_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_BED_RESIDENCY_TIME) * 1000UL))
    #else
      // Loop until the temperature is very close target
      #define TEMP_BED_CONDITIONS (wants_to_cool ? thermalManager.isCoolingBed() : thermalManager.isHeatingBed())
    #endif

    float target_temp = -1.0, old_temp = 9999.0;
    bool wants_to_cool = false;
    wait_for_heatup = true;
    millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

    #if DISABLED(BUSY_WHILE_HEATING)
      KEEPALIVE_STATE(NOT_BUSY);
    #endif

    target_extruder = active_extruder; // for print_heaterstates

    #if ENABLED(PRINTER_EVENT_LEDS)
      const float start_temp = thermalManager.degBed();
      uint8_t old_red = 255;
    #endif

    do {
      // Target temperature might be changed during the loop
      if (target_temp != thermalManager.degTargetBed()) {
        wants_to_cool = thermalManager.isCoolingBed();
        target_temp = thermalManager.degTargetBed();

        // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
        if (no_wait_for_cooling && wants_to_cool) break;
      }

      now = millis();
      if (ELAPSED(now, next_temp_ms)) { //Print Temp Reading every 1 second while heating up.
        next_temp_ms = now + 1000UL;
        thermalManager.print_heaterstates();
        #if TEMP_BED_RESIDENCY_TIME > 0
          SERIAL_PROTOCOLPGM(" W:");
          if (residency_start_ms)
            SERIAL_PROTOCOL(long((((TEMP_BED_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
          else
            SERIAL_PROTOCOLCHAR('?');
        #endif
        SERIAL_EOL();
      }

      idle();
      refresh_cmd_timeout(); // to prevent stepper_inactive_time from running out

      const float temp = thermalManager.degBed();

      #if ENABLED(PRINTER_EVENT_LEDS)
        // Gradually change LED strip from blue to violet as bed heats up
        if (!wants_to_cool) {
          const uint8_t red = map(constrain(temp, start_temp, target_temp), start_temp, target_temp, 0, 255);
          if (red != old_red) {
            old_red = red;
            leds.set_color(
              MakeLEDColor(red, 0, 255, 0, pixels.getBrightness())
              #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
                , true
              #endif
            );
          }
        }
      #endif

      #if TEMP_BED_RESIDENCY_TIME > 0

        const float temp_diff = FABS(target_temp - temp);

        if (!residency_start_ms) {
          // Start the TEMP_BED_RESIDENCY_TIME timer when we reach target temp for the first time.
          if (temp_diff < TEMP_BED_WINDOW) residency_start_ms = now;
        }
        else if (temp_diff > TEMP_BED_HYSTERESIS) {
          // Restart the timer whenever the temperature falls outside the hysteresis.
          residency_start_ms = now;
        }

      #endif // TEMP_BED_RESIDENCY_TIME > 0

      // Prevent a wait-forever situation if R is misused i.e. M190 R0
      if (wants_to_cool) {
        // Break after MIN_COOLING_SLOPE_TIME_BED seconds
        // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG_BED
        if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
          if (old_temp - temp < MIN_COOLING_SLOPE_DEG_BED) break;
          next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME_BED;
          old_temp = temp;
        }
      }

    } while (wait_for_heatup && TEMP_BED_CONDITIONS);

    if (wait_for_heatup) LCD_MESSAGEPGM(MSG_BED_DONE);
    #if DISABLED(BUSY_WHILE_HEATING)
      KEEPALIVE_STATE(IN_HANDLER);
    #endif
  }

#endif // HAS_TEMP_BED

/**
 * M110: Set Current Line Number
 */
inline void gcode_M110() {
  if (parser.seenval('N')) gcode_LastN = parser.value_long();
}

/**
 * M111: Set the debug level
 */
inline void gcode_M111() {
  if (parser.seen('S')) marlin_debug_flags = parser.byteval('S');

  const static char str_debug_1[] PROGMEM = MSG_DEBUG_ECHO,
                    str_debug_2[] PROGMEM = MSG_DEBUG_INFO,
                    str_debug_4[] PROGMEM = MSG_DEBUG_ERRORS,
                    str_debug_8[] PROGMEM = MSG_DEBUG_DRYRUN,
                    str_debug_16[] PROGMEM = MSG_DEBUG_COMMUNICATION
                    #if ENABLED(DEBUG_LEVELING_FEATURE)
                      , str_debug_32[] PROGMEM = MSG_DEBUG_LEVELING
                    #endif
                    ;

  const static char* const debug_strings[] PROGMEM = {
    str_debug_1, str_debug_2, str_debug_4, str_debug_8, str_debug_16
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      , str_debug_32
    #endif
  };

  SERIAL_ECHO_START();
  SERIAL_ECHOPGM(MSG_DEBUG_PREFIX);
  if (marlin_debug_flags) {
    uint8_t comma = 0;
    for (uint8_t i = 0; i < COUNT(debug_strings); i++) {
      if (TEST(marlin_debug_flags, i)) {
        if (comma++) SERIAL_CHAR(',');
        serialprintPGM((char*)pgm_read_word(&debug_strings[i]));
      }
    }
  }
  else {
    SERIAL_ECHOPGM(MSG_DEBUG_OFF);
  }
  SERIAL_EOL();
}

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  /**
   * M113: Get or set Host Keepalive interval (0 to disable)
   *
   *   S<seconds> Optional. Set the keepalive interval.
   */
  inline void gcode_M113() {
    if (parser.seenval('S')) {
      host_keepalive_interval = parser.value_byte();
      NOMORE(host_keepalive_interval, 60);
    }
    else {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR("M113 S", (unsigned long)host_keepalive_interval);
    }
  }

#endif

#if ENABLED(BARICUDA)

  #if HAS_HEATER_1
    /**
     * M126: Heater 1 valve open
     */
    inline void gcode_M126() { baricuda_valve_pressure = parser.byteval('S', 255); }
    /**
     * M127: Heater 1 valve close
     */
    inline void gcode_M127() { baricuda_valve_pressure = 0; }
  #endif

  #if HAS_HEATER_2
    /**
     * M128: Heater 2 valve open
     */
    inline void gcode_M128() { baricuda_e_to_p_pressure = parser.byteval('S', 255); }
    /**
     * M129: Heater 2 valve close
     */
    inline void gcode_M129() { baricuda_e_to_p_pressure = 0; }
  #endif

#endif // BARICUDA

/**
 * M140: Set bed temperature
 */
inline void gcode_M140() {
  if (DEBUGGING(DRYRUN)) return;
  if (parser.seenval('S')) thermalManager.setTargetBed(parser.value_celsius());
}

#if ENABLED(ULTIPANEL)

  /**
   * M145: Set the heatup state for a material in the LCD menu
   *
   *   S<material> (0=PLA, 1=ABS)
   *   H<hotend temp>
   *   B<bed temp>
   *   F<fan speed>
   */
  inline void gcode_M145() {
    const uint8_t material = (uint8_t)parser.intval('S');
    if (material >= COUNT(lcd_preheat_hotend_temp)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_MATERIAL_INDEX);
    }
    else {
      int v;
      if (parser.seenval('H')) {
        v = parser.value_int();
        lcd_preheat_hotend_temp[material] = constrain(v, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - 15);
      }
      if (parser.seenval('F')) {
        v = parser.value_int();
        lcd_preheat_fan_speed[material] = constrain(v, 0, 255);
      }
      #if TEMP_SENSOR_BED != 0
        if (parser.seenval('B')) {
          v = parser.value_int();
          lcd_preheat_bed_temp[material] = constrain(v, BED_MINTEMP, BED_MAXTEMP - 15);
        }
      #endif
    }
  }

#endif // ULTIPANEL

#if ENABLED(TEMPERATURE_UNITS_SUPPORT)
  /**
   * M149: Set temperature units
   */
  inline void gcode_M149() {
         if (parser.seenval('C')) parser.set_input_temp_units(TEMPUNIT_C);
    else if (parser.seenval('K')) parser.set_input_temp_units(TEMPUNIT_K);
    else if (parser.seenval('F')) parser.set_input_temp_units(TEMPUNIT_F);
  }
#endif

#if HAS_POWER_SWITCH

  /**
   * M80   : Turn on the Power Supply
   * M80 S : Report the current state and exit
   */
  inline void gcode_M80() {

    // S: Report the current power supply state and exit
    if (parser.seen('S')) {
      serialprintPGM(powersupply_on ? PSTR("PS:1\n") : PSTR("PS:0\n"));
      return;
    }

    PSU_ON();

    /**
     * If you have a switch on suicide pin, this is useful
     * if you want to start another print with suicide feature after
     * a print without suicide...
     */
    #if HAS_SUICIDE
      OUT_WRITE(SUICIDE_PIN, HIGH);
    #endif

    #if ENABLED(HAVE_TMC2130)
      delay(100);
      tmc2130_init(); // Settings only stick when the driver has power
    #endif

    powersupply_on = true;

    #if ENABLED(ULTIPANEL)
      LCD_MESSAGEPGM(WELCOME_MSG);
    #endif

    #if ENABLED(HAVE_TMC2208)
      delay(100);
      tmc2208_init();
    #endif
  }

#endif // HAS_POWER_SWITCH

/**
 * M81: Turn off Power, including Power Supply, if there is one.
 *
 *      This code should ALWAYS be available for EMERGENCY SHUTDOWN!
 */
inline void gcode_M81() {
  thermalManager.disable_all_heaters();
  stepper.finish_and_disable();

  #if FAN_COUNT > 0
    for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
    #if ENABLED(PROBING_FANS_OFF)
      fans_paused = false;
      ZERO(paused_fanSpeeds);
    #endif
  #endif

  safe_delay(1000); // Wait 1 second before switching off

  #if HAS_SUICIDE
    stepper.synchronize();
    suicide();
  #elif HAS_POWER_SWITCH
    PSU_OFF();
    powersupply_on = false;
  #endif

  #if ENABLED(ULTIPANEL)
    LCD_MESSAGEPGM(MACHINE_NAME " " MSG_OFF ".");
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
 * M18, M84: Disable stepper motors
 */
inline void gcode_M18_M84() {
  if (parser.seenval('S')) {
    stepper_inactive_time = parser.value_millis_from_seconds();
  }
  else {
    bool all_axis = !(parser.seen('X') || parser.seen('Y') || parser.seen('Z') || parser.seen('E'));
    if (all_axis) {
      stepper.finish_and_disable();
    }
    else {
      stepper.synchronize();
      if (parser.seen('X')) disable_X();
      if (parser.seen('Y')) disable_Y();
      if (parser.seen('Z')) disable_Z();
      #if E0_ENABLE_PIN != X_ENABLE_PIN && E1_ENABLE_PIN != Y_ENABLE_PIN // Only disable on boards that have separate ENABLE_PINS
        if (parser.seen('E')) disable_e_steppers();
      #endif
    }

    #if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(ULTIPANEL)  // Only needed with an LCD
      if (ubl.lcd_map_control) ubl.lcd_map_control = defer_return_to_status = false;
    #endif
  }
}

/**
 * M85: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
inline void gcode_M85() {
  if (parser.seen('S')) max_inactive_time = parser.value_millis_from_seconds();
}

/**
 * Multi-stepper support for M92, M201, M203
 */
#if ENABLED(DISTINCT_E_FACTORS)
  #define GET_TARGET_EXTRUDER(CMD) if (get_target_extruder_from_command(CMD)) return
  #define TARGET_EXTRUDER target_extruder
#else
  #define GET_TARGET_EXTRUDER(CMD) NOOP
  #define TARGET_EXTRUDER 0
#endif

/**
 * M92: Set axis steps-per-unit for one or more axes, X, Y, Z, and E.
 *      (Follows the same syntax as G92)
 *
 *      With multiple extruders use T to specify which one.
 */
inline void gcode_M92() {

  GET_TARGET_EXTRUDER(92);

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      if (i == E_AXIS) {
        const float value = parser.value_per_axis_unit((AxisEnum)(E_AXIS + TARGET_EXTRUDER));
        if (value < 20.0) {
          float factor = planner.axis_steps_per_mm[E_AXIS + TARGET_EXTRUDER] / value; // increase e constants if M92 E14 is given for netfab.
          planner.max_jerk[E_AXIS] *= factor;
          planner.max_feedrate_mm_s[E_AXIS + TARGET_EXTRUDER] *= factor;
          planner.max_acceleration_steps_per_s2[E_AXIS + TARGET_EXTRUDER] *= factor;
        }
        planner.axis_steps_per_mm[E_AXIS + TARGET_EXTRUDER] = value;
      }
      else {
        planner.axis_steps_per_mm[i] = parser.value_per_axis_unit((AxisEnum)i);
      }
    }
  }
  planner.refresh_positioning();
}

/**
 * Output the current position to serial
 */
void report_current_position() {
  SERIAL_PROTOCOLPGM("X:");
  SERIAL_PROTOCOL(LOGICAL_X_POSITION(current_position[X_AXIS]));
  SERIAL_PROTOCOLPGM(" Y:");
  SERIAL_PROTOCOL(LOGICAL_Y_POSITION(current_position[Y_AXIS]));
  SERIAL_PROTOCOLPGM(" Z:");
  SERIAL_PROTOCOL(LOGICAL_Z_POSITION(current_position[Z_AXIS]));
  SERIAL_PROTOCOLPGM(" E:");
  SERIAL_PROTOCOL(current_position[E_AXIS]);

  stepper.report_positions();

  #if IS_SCARA
    SERIAL_PROTOCOLPAIR("SCARA Theta:", stepper.get_axis_position_degrees(A_AXIS));
    SERIAL_PROTOCOLLNPAIR("   Psi+Theta:", stepper.get_axis_position_degrees(B_AXIS));
    SERIAL_EOL();
  #endif
}

#ifdef M114_DETAIL

  void report_xyze(const float pos[], const uint8_t n = 4, const uint8_t precision = 3) {
    char str[12];
    for (uint8_t i = 0; i < n; i++) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[i]);
      SERIAL_CHAR(':');
      SERIAL_PROTOCOL(dtostrf(pos[i], 8, precision, str));
    }
    SERIAL_EOL();
  }

  inline void report_xyz(const float pos[]) { report_xyze(pos, 3); }

  void report_current_position_detail() {

    stepper.synchronize();

    SERIAL_PROTOCOLPGM("\nLogical:");
    const float logical[XYZ] = {
      LOGICAL_X_POSITION(current_position[X_AXIS]),
      LOGICAL_Y_POSITION(current_position[Y_AXIS]),
      LOGICAL_Z_POSITION(current_position[Z_AXIS])
    };
    report_xyz(logical);

    SERIAL_PROTOCOLPGM("Raw:    ");
    report_xyz(current_position);

    float leveled[XYZ] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

    #if PLANNER_LEVELING
      SERIAL_PROTOCOLPGM("Leveled:");
      planner.apply_leveling(leveled);
      report_xyz(leveled);

      SERIAL_PROTOCOLPGM("UnLevel:");
      float unleveled[XYZ] = { leveled[X_AXIS], leveled[Y_AXIS], leveled[Z_AXIS] };
      planner.unapply_leveling(unleveled);
      report_xyz(unleveled);
    #endif

    #if IS_KINEMATIC
      #if IS_SCARA
        SERIAL_PROTOCOLPGM("ScaraK: ");
      #else
        SERIAL_PROTOCOLPGM("DeltaK: ");
      #endif
      inverse_kinematics(leveled);  // writes delta[]
      report_xyz(delta);
    #endif

    SERIAL_PROTOCOLPGM("Stepper:");
    LOOP_XYZE(i) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[i]);
      SERIAL_CHAR(':');
      SERIAL_PROTOCOL(stepper.position((AxisEnum)i));
    }
    SERIAL_EOL();

    #if IS_SCARA
      const float deg[XYZ] = {
        stepper.get_axis_position_degrees(A_AXIS),
        stepper.get_axis_position_degrees(B_AXIS)
      };
      SERIAL_PROTOCOLPGM("Degrees:");
      report_xyze(deg, 2);
    #endif

    SERIAL_PROTOCOLPGM("FromStp:");
    get_cartesian_from_steppers();  // writes cartes[XYZ] (with forward kinematics)
    const float from_steppers[XYZE] = { cartes[X_AXIS], cartes[Y_AXIS], cartes[Z_AXIS], stepper.get_axis_position_mm(E_AXIS) };
    report_xyze(from_steppers);

    const float diff[XYZE] = {
      from_steppers[X_AXIS] - leveled[X_AXIS],
      from_steppers[Y_AXIS] - leveled[Y_AXIS],
      from_steppers[Z_AXIS] - leveled[Z_AXIS],
      from_steppers[E_AXIS] - current_position[E_AXIS]
    };
    SERIAL_PROTOCOLPGM("Differ: ");
    report_xyze(diff);
  }
#endif // M114_DETAIL

/**
 * M114: Report current position to host
 */
inline void gcode_M114() {

  #ifdef M114_DETAIL
    if (parser.seen('D')) {
      report_current_position_detail();
      return;
    }
  #endif

  stepper.synchronize();
  report_current_position();
}

/**
 * M115: Capabilities string
 */

#if ENABLED(EXTENDED_CAPABILITIES_REPORT)
  static void cap_line(const char * const name, bool ena=false) {
    SERIAL_PROTOCOLPGM("Cap:");
    serialprintPGM(name);
    SERIAL_PROTOCOLPGM(":");
    SERIAL_PROTOCOLLN(int(ena ? 1 : 0));
  }
#endif

inline void gcode_M115() {
  SERIAL_PROTOCOLLNPGM(MSG_M115_REPORT);

  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)

    // SERIAL_XON_XOFF
    cap_line(PSTR("SERIAL_XON_XOFF")
      #if ENABLED(SERIAL_XON_XOFF)
        , true
      #endif
    );

    // EEPROM (M500, M501)
    cap_line(PSTR("EEPROM")
      #if ENABLED(EEPROM_SETTINGS)
        , true
      #endif
    );

    // Volumetric Extrusion (M200)
    cap_line(PSTR("VOLUMETRIC")
      #if DISABLED(NO_VOLUMETRICS)
        , true
      #endif
    );

    // AUTOREPORT_TEMP (M155)
    cap_line(PSTR("AUTOREPORT_TEMP")
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        , true
      #endif
    );

    // PROGRESS (M530 S L, M531 <file>, M532 X L)
    cap_line(PSTR("PROGRESS"));

    // Print Job timer M75, M76, M77
    cap_line(PSTR("PRINT_JOB"), true);

    // AUTOLEVEL (G29)
    cap_line(PSTR("AUTOLEVEL")
      #if HAS_AUTOLEVEL
        , true
      #endif
    );

    // Z_PROBE (G30)
    cap_line(PSTR("Z_PROBE")
      #if HAS_BED_PROBE
        , true
      #endif
    );

    // MESH_REPORT (M420 V)
    cap_line(PSTR("LEVELING_DATA")
      #if HAS_LEVELING
        , true
      #endif
    );

    // BUILD_PERCENT (M73)
    cap_line(PSTR("BUILD_PERCENT")
      #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
        , true
      #endif
    );

    // SOFTWARE_POWER (M80, M81)
    cap_line(PSTR("SOFTWARE_POWER")
      #if HAS_POWER_SWITCH
        , true
      #endif
    );

    // CASE LIGHTS (M355)
    cap_line(PSTR("TOGGLE_LIGHTS")
      #if HAS_CASE_LIGHT
        , true
      #endif
    );
    cap_line(PSTR("CASE_LIGHT_BRIGHTNESS")
      #if HAS_CASE_LIGHT
        , USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)
      #endif
    );

    // EMERGENCY_PARSER (M108, M112, M410)
    cap_line(PSTR("EMERGENCY_PARSER")
      #if ENABLED(EMERGENCY_PARSER)
        , true
      #endif
    );

  #endif // EXTENDED_CAPABILITIES_REPORT
}

/**
 * M117: Set LCD Status Message
 */
inline void gcode_M117() { lcd_setstatus(parser.string_arg); }

/**
 * M118: Display a message in the host console.
 *
 *  A1  Append '// ' for an action command, as in OctoPrint
 *  E1  Have the host 'echo:' the text
 */
inline void gcode_M118() {
  if (parser.seenval('E') && parser.value_bool()) SERIAL_ECHO_START();
  if (parser.seenval('A') && parser.value_bool()) SERIAL_ECHOPGM("// ");
  SERIAL_ECHOLN(parser.string_arg);
}

/**
 * M119: Output endstop states to serial output
 */
inline void gcode_M119() { endstops.M119(); }

/**
 * M120: Enable endstops and set non-homing endstop state to "enabled"
 */
inline void gcode_M120() { endstops.enable_globally(true); }

/**
 * M121: Disable endstops and set non-homing endstop state to "disabled"
 */
inline void gcode_M121() { endstops.enable_globally(false); }

#if ENABLED(PARK_HEAD_ON_PAUSE)

  /**
   * M125: Store current position and move to filament change position.
   *       Called on pause (by M25) to prevent material leaking onto the
   *       object. On resume (M24) the head will be moved back and the
   *       print will resume.
   *
   *       If Marlin is compiled without SD Card support, M125 can be
   *       used directly to pause the print and move to park position,
   *       resuming with a button click or M108.
   *
   *    L = override retract length
   *    X = override X
   *    Y = override Y
   *    Z = override Z raise
   */
  inline void gcode_M125() {

    // Initial retract before move to filament change position
    const float retract = -FABS(parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef PAUSE_PARK_RETRACT_LENGTH
        + (PAUSE_PARK_RETRACT_LENGTH)
      #endif
    );

    point_t park_point = NOZZLE_PARK_POINT;

    // Move XY axes to filament change position or given position
    if (parser.seenval('X')) park_point.x = parser.linearval('X');
    if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

    // Lift Z axis
    if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

    #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE)
      park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
      park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
    #endif

    #if DISABLED(SDSUPPORT)
      const bool job_running = print_job_timer.isRunning();
    #endif

    if (pause_print(retract, park_point)) {
      #if DISABLED(SDSUPPORT)
        // Wait for lcd click or M108
        wait_for_filament_reload();

        // Return to print position and continue
        resume_print();

        if (job_running) print_job_timer.start();
      #endif
    }
  }

#endif // PARK_HEAD_ON_PAUSE

#if HAS_COLOR_LEDS

  /**
   * M150: Set Status LED Color - Use R-U-B-W for R-G-B-W
   *       and Brightness       - Use P (for NEOPIXEL only)
   *
   * Always sets all 3 or 4 components. If a component is left out, set to 0.
   *                                    If brightness is left out, no value changed
   *
   * Examples:
   *
   *   M150 R255       ; Turn LED red
   *   M150 R255 U127  ; Turn LED orange (PWM only)
   *   M150            ; Turn LED off
   *   M150 R U B      ; Turn LED white
   *   M150 W          ; Turn LED white using a white LED
   *   M150 P127       ; Set LED 50% brightness
   *   M150 P          ; Set LED full brightness
   */
  inline void gcode_M150() {
    leds.set_color(MakeLEDColor(
      parser.seen('R') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
      parser.seen('U') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
      parser.seen('B') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
      parser.seen('W') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
      parser.seen('P') ? (parser.has_value() ? parser.value_byte() : 255) : pixels.getBrightness()
    ));
  }

#endif // HAS_COLOR_LEDS

#if DISABLED(NO_VOLUMETRICS)

  /**
   * M200: Set filament diameter and set E axis units to cubic units
   *
   *    T<extruder> - Optional extruder number. Current extruder if omitted.
   *    D<linear> - Diameter of the filament. Use "D0" to switch back to linear units on the E axis.
   */
  inline void gcode_M200() {

    if (get_target_extruder_from_command(200)) return;

    if (parser.seen('D')) {
      // setting any extruder filament size disables volumetric on the assumption that
      // slicers either generate in extruder values as cubic mm or as as filament feeds
      // for all extruders
      if ( (parser.volumetric_enabled = (parser.value_linear_units() != 0.0)) )
        planner.set_filament_size(target_extruder, parser.value_linear_units());
    }
    planner.calculate_volumetric_multipliers();
  }

#endif // !NO_VOLUMETRICS

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 *
 *       With multiple extruders use T to specify which one.
 */
inline void gcode_M201() {

  GET_TARGET_EXTRUDER(201);

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = i + (i == E_AXIS ? TARGET_EXTRUDER : 0);
      planner.max_acceleration_mm_per_s2[a] = parser.value_axis_units((AxisEnum)a);
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  planner.reset_acceleration_rates();
}

#if 0 // Not used for Sprinter/grbl gen6
  inline void gcode_M202() {
    LOOP_XYZE(i) {
      if (parser.seen(axis_codes[i])) axis_travel_steps_per_sqr_second[i] = parser.value_axis_units((AxisEnum)i) * planner.axis_steps_per_mm[i];
    }
  }
#endif


/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in units/sec
 *
 *       With multiple extruders use T to specify which one.
 */
inline void gcode_M203() {

  GET_TARGET_EXTRUDER(203);

  LOOP_XYZE(i)
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = i + (i == E_AXIS ? TARGET_EXTRUDER : 0);
      planner.max_feedrate_mm_s[a] = parser.value_axis_units((AxisEnum)a);
    }
}

/**
 * M204: Set Accelerations in units/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 */
inline void gcode_M204() {
  if (parser.seen('S')) {  // Kept for legacy compatibility. Should NOT BE USED for new developments.
    planner.travel_acceleration = planner.acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Print and Travel Acceleration: ", planner.acceleration);
  }
  if (parser.seen('P')) {
    planner.acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Print Acceleration: ", planner.acceleration);
  }
  if (parser.seen('R')) {
    planner.retract_acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Retract Acceleration: ", planner.retract_acceleration);
  }
  if (parser.seen('T')) {
    planner.travel_acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Travel Acceleration: ", planner.travel_acceleration);
  }
}

/**
 * M205: Set Advanced Settings
 *
 *    S = Min Feed Rate (units/s)
 *    T = Min Travel Feed Rate (units/s)
 *    B = Min Segment Time (µs)
 *    X = Max X Jerk (units/sec^2)
 *    Y = Max Y Jerk (units/sec^2)
 *    Z = Max Z Jerk (units/sec^2)
 *    E = Max E Jerk (units/sec^2)
 */
inline void gcode_M205() {
  if (parser.seen('S')) planner.min_feedrate_mm_s = parser.value_linear_units();
  if (parser.seen('T')) planner.min_travel_feedrate_mm_s = parser.value_linear_units();
  if (parser.seen('B')) planner.min_segment_time_us = parser.value_ulong();
  if (parser.seen('X')) planner.max_jerk[X_AXIS] = parser.value_linear_units();
  if (parser.seen('Y')) planner.max_jerk[Y_AXIS] = parser.value_linear_units();
  if (parser.seen('Z')) planner.max_jerk[Z_AXIS] = parser.value_linear_units();
  if (parser.seen('E')) planner.max_jerk[E_AXIS] = parser.value_linear_units();
}

#if HAS_M206_COMMAND

  /**
   * M206: Set Additional Homing Offset (X Y Z). SCARA aliases T=X, P=Y
   *
   * *** @thinkyhead: I recommend deprecating M206 for SCARA in favor of M665.
   * ***              M206 for SCARA will remain enabled in 1.1.x for compatibility.
   * ***              In the next 1.2 release, it will simply be disabled by default.
   */
  inline void gcode_M206() {
    LOOP_XYZ(i)
      if (parser.seen(axis_codes[i]))
        set_home_offset((AxisEnum)i, parser.value_linear_units());

    #if ENABLED(MORGAN_SCARA)
      if (parser.seen('T')) set_home_offset(A_AXIS, parser.value_float()); // Theta
      if (parser.seen('P')) set_home_offset(B_AXIS, parser.value_float()); // Psi
    #endif

    report_current_position();
  }

#endif // HAS_M206_COMMAND

#if ENABLED(DELTA)
  /**
   * M665: Set delta configurations
   *
   *    H = delta height
   *    L = diagonal rod
   *    R = delta radius
   *    S = segments per second
   *    B = delta calibration radius
   *    X = Alpha (Tower 1) angle trim
   *    Y = Beta (Tower 2) angle trim
   *    Z = Rotate A and B by this angle
   */
  inline void gcode_M665() {
    if (parser.seen('H')) delta_height                   = parser.value_linear_units();
    if (parser.seen('L')) delta_diagonal_rod             = parser.value_linear_units();
    if (parser.seen('R')) delta_radius                   = parser.value_linear_units();
    if (parser.seen('S')) delta_segments_per_second      = parser.value_float();
    if (parser.seen('B')) delta_calibration_radius       = parser.value_float();
    if (parser.seen('X')) delta_tower_angle_trim[A_AXIS] = parser.value_float();
    if (parser.seen('Y')) delta_tower_angle_trim[B_AXIS] = parser.value_float();
    if (parser.seen('Z')) delta_tower_angle_trim[C_AXIS] = parser.value_float();
    recalc_delta_settings();
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
    LOOP_XYZ(i) {
      if (parser.seen(axis_codes[i])) {
        if (parser.value_linear_units() * Z_HOME_DIR <= 0)
          delta_endstop_adj[i] = parser.value_linear_units();
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("delta_endstop_adj[", axis_codes[i]);
            SERIAL_ECHOLNPAIR("] = ", delta_endstop_adj[i]);
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

#elif IS_SCARA

  /**
   * M665: Set SCARA settings
   *
   * Parameters:
   *
   *   S[segments-per-second] - Segments-per-second
   *   P[theta-psi-offset]    - Theta-Psi offset, added to the shoulder (A/X) angle
   *   T[theta-offset]        - Theta     offset, added to the elbow    (B/Y) angle
   *
   *   A, P, and X are all aliases for the shoulder angle
   *   B, T, and Y are all aliases for the elbow angle
   */
  inline void gcode_M665() {
    if (parser.seen('S')) delta_segments_per_second = parser.value_float();

    const bool hasA = parser.seen('A'), hasP = parser.seen('P'), hasX = parser.seen('X');
    const uint8_t sumAPX = hasA + hasP + hasX;
    if (sumAPX == 1)
      home_offset[A_AXIS] = parser.value_float();
    else if (sumAPX > 1) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Only one of A, P, or X is allowed.");
      return;
    }

    const bool hasB = parser.seen('B'), hasT = parser.seen('T'), hasY = parser.seen('Y');
    const uint8_t sumBTY = hasB + hasT + hasY;
    if (sumBTY == 1)
      home_offset[B_AXIS] = parser.value_float();
    else if (sumBTY > 1) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Only one of B, T, or Y is allowed.");
      return;
    }
  }



#elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)

  /**
   * M666: For Z Dual Endstop setup, set z axis offset to the z2 axis.
   */
  inline void gcode_M666() {
    SERIAL_ECHOPGM("Dual Endstop Adjustment (mm): ");
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (parser.seen('X')) x_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" X", x_endstop_adj);
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (parser.seen('Y')) y_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" Y", y_endstop_adj);
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (parser.seen('Z')) z_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" Z", z_endstop_adj);
    #endif
    SERIAL_EOL();
  }

#endif // !DELTA && Z_DUAL_ENDSTOPS

#if ENABLED(FWRETRACT)

  /**
   * M207: Set firmware retraction values
   *
   *   S[+units]    retract_length
   *   W[+units]    swap_retract_length (multi-extruder)
   *   F[units/min] retract_feedrate_mm_s
   *   Z[units]     retract_zlift
   */
  inline void gcode_M207() {
    if (parser.seen('S')) fwretract.retract_length = parser.value_axis_units(E_AXIS);
    if (parser.seen('F')) fwretract.retract_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
    if (parser.seen('Z')) fwretract.retract_zlift = parser.value_linear_units();
    if (parser.seen('W')) fwretract.swap_retract_length = parser.value_axis_units(E_AXIS);
  }

  /**
   * M208: Set firmware un-retraction values
   *
   *   S[+units]    retract_recover_length (in addition to M207 S*)
   *   W[+units]    swap_retract_recover_length (multi-extruder)
   *   F[units/min] retract_recover_feedrate_mm_s
   *   R[units/min] swap_retract_recover_feedrate_mm_s
   */
  inline void gcode_M208() {
    if (parser.seen('S')) fwretract.retract_recover_length = parser.value_axis_units(E_AXIS);
    if (parser.seen('F')) fwretract.retract_recover_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
    if (parser.seen('R')) fwretract.swap_retract_recover_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
    if (parser.seen('W')) fwretract.swap_retract_recover_length = parser.value_axis_units(E_AXIS);
  }

  /**
   * M209: Enable automatic retract (M209 S1)
   *   For slicers that don't support G10/11, reversed extrude-only
   *   moves will be classified as retraction.
   */
  inline void gcode_M209() {
    if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
      if (parser.seen('S')) {
        fwretract.autoretract_enabled = parser.value_bool();
        for (uint8_t i = 0; i < EXTRUDERS; i++) fwretract.retracted[i] = false;
      }
    }
  }

#endif // FWRETRACT

/**
 * M211: Enable, Disable, and/or Report software endstops
 *
 * Usage: M211 S1 to enable, M211 S0 to disable, M211 alone for report
 */
inline void gcode_M211() {
  SERIAL_ECHO_START();
  #if HAS_SOFTWARE_ENDSTOPS
    if (parser.seen('S')) soft_endstops_enabled = parser.value_bool();
    SERIAL_ECHOPGM(MSG_SOFT_ENDSTOPS);
    serialprintPGM(soft_endstops_enabled ? PSTR(MSG_ON) : PSTR(MSG_OFF));
  #else
    SERIAL_ECHOPGM(MSG_SOFT_ENDSTOPS);
    SERIAL_ECHOPGM(MSG_OFF);
  #endif
  SERIAL_ECHOPGM(MSG_SOFT_MIN);
  SERIAL_ECHOPAIR(    MSG_X, LOGICAL_X_POSITION(soft_endstop_min[X_AXIS]));
  SERIAL_ECHOPAIR(" " MSG_Y, LOGICAL_Y_POSITION(soft_endstop_min[Y_AXIS]));
  SERIAL_ECHOPAIR(" " MSG_Z, LOGICAL_Z_POSITION(soft_endstop_min[Z_AXIS]));
  SERIAL_ECHOPGM(MSG_SOFT_MAX);
  SERIAL_ECHOPAIR(    MSG_X, LOGICAL_X_POSITION(soft_endstop_max[X_AXIS]));
  SERIAL_ECHOPAIR(" " MSG_Y, LOGICAL_Y_POSITION(soft_endstop_max[Y_AXIS]));
  SERIAL_ECHOLNPAIR(" " MSG_Z, LOGICAL_Z_POSITION(soft_endstop_max[Z_AXIS]));
}

#if HOTENDS > 1

  /**
   * M218 - set hotend offset (in linear units)
   *
   *   T<tool>
   *   X<xoffset>
   *   Y<yoffset>
   *   Z<zoffset> - Available with DUAL_X_CARRIAGE and SWITCHING_NOZZLE
   */
  inline void gcode_M218() {
    if (get_target_extruder_from_command(218) || target_extruder == 0) return;

    if (parser.seenval('X')) hotend_offset[X_AXIS][target_extruder] = parser.value_linear_units();
    if (parser.seenval('Y')) hotend_offset[Y_AXIS][target_extruder] = parser.value_linear_units();

    #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
      if (parser.seenval('Z')) hotend_offset[Z_AXIS][target_extruder] = parser.value_linear_units();
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    HOTEND_LOOP() {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(hotend_offset[X_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO(hotend_offset[Y_AXIS][e]);
      #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
        SERIAL_CHAR(',');
        SERIAL_ECHO(hotend_offset[Z_AXIS][e]);
      #endif
    }
    SERIAL_EOL();
  }

#endif // HOTENDS > 1

/**
 * M220: Set speed percentage factor, aka "Feed Rate" (M220 S95)
 */
inline void gcode_M220() {
  if (parser.seenval('S')) feedrate_percentage = parser.value_int();
}

/**
 * M221: Set extrusion percentage (M221 T0 S95)
 */
inline void gcode_M221() {
  if (get_target_extruder_from_command(221)) return;
  if (parser.seenval('S')) {
    planner.flow_percentage[target_extruder] = parser.value_int();
    planner.refresh_e_factor(target_extruder);
  }
}

/**
 * M226: Wait until the specified pin reaches the state required (M226 P<pin> S<state>)
 */
inline void gcode_M226() {
  if (parser.seen('P')) {
    const int pin = parser.value_int(),
              pin_state = parser.intval('S', -1); // required pin state - default is inverted

    if (WITHIN(pin_state, -1, 1) && pin > -1 && !pin_is_protected(pin)) {

      int target = LOW;

      stepper.synchronize();

      pinMode(pin, INPUT);
      switch (pin_state) {
        case 1:
          target = HIGH;
          break;
        case 0:
          target = LOW;
          break;
        case -1:
          target = !digitalRead(pin);
          break;
      }

      while (digitalRead(pin) != target) idle();

    } // pin_state -1 0 1 && pin > -1
  } // parser.seen('P')
}

#if ENABLED(EXPERIMENTAL_I2CBUS)

  /**
   * M260: Send data to a I2C slave device
   *
   * This is a PoC, the formating and arguments for the GCODE will
   * change to be more compatible, the current proposal is:
   *
   *  M260 A<slave device address base 10> ; Sets the I2C slave address the data will be sent to
   *
   *  M260 B<byte-1 value in base 10>
   *  M260 B<byte-2 value in base 10>
   *  M260 B<byte-3 value in base 10>
   *
   *  M260 S1 ; Send the buffered data and reset the buffer
   *  M260 R1 ; Reset the buffer without sending data
   *
   */
  inline void gcode_M260() {
    // Set the target address
    if (parser.seen('A')) i2c.address(parser.value_byte());

    // Add a new byte to the buffer
    if (parser.seen('B')) i2c.addbyte(parser.value_byte());

    // Flush the buffer to the bus
    if (parser.seen('S')) i2c.send();

    // Reset and rewind the buffer
    else if (parser.seen('R')) i2c.reset();
  }

  /**
   * M261: Request X bytes from I2C slave device
   *
   * Usage: M261 A<slave device address base 10> B<number of bytes>
   */
  inline void gcode_M261() {
    if (parser.seen('A')) i2c.address(parser.value_byte());

    uint8_t bytes = parser.byteval('B', 1);

    if (i2c.addr && bytes && bytes <= TWIBUS_BUFFER_SIZE) {
      i2c.relay(bytes);
    }
    else {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Bad i2c request");
    }
  }

#endif // EXPERIMENTAL_I2CBUS

#if HAS_SERVOS

  /**
   * M280: Get or set servo position. P<index> [S<angle>]
   */
  inline void gcode_M280() {
    if (!parser.seen('P')) return;
    const int servo_index = parser.value_int();
    if (WITHIN(servo_index, 0, NUM_SERVOS - 1)) {
      if (parser.seen('S'))
        MOVE_SERVO(servo_index, parser.value_int());
      else {
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR(" Servo ", servo_index);
        SERIAL_ECHOLNPAIR(": ", servo[servo_index].read());
      }
    }
    else {
      SERIAL_ERROR_START();
      SERIAL_ECHOPAIR("Servo ", servo_index);
      SERIAL_ECHOLNPGM(" out of range");
    }
  }

#endif // HAS_SERVOS

#if ENABLED(BABYSTEPPING)

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    FORCE_INLINE void mod_zprobe_zoffset(const float &offs) {
      zprobe_zoffset += offs;
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR(MSG_PROBE_Z_OFFSET ": ", zprobe_zoffset);
    }
  #endif

  /**
   * M290: Babystepping
   */
  inline void gcode_M290() {
    #if ENABLED(BABYSTEP_XY)
      for (uint8_t a = X_AXIS; a <= Z_AXIS; a++)
        if (parser.seenval(axis_codes[a]) || (a == Z_AXIS && parser.seenval('S'))) {
          const float offs = constrain(parser.value_axis_units((AxisEnum)a), -2, 2);
          thermalManager.babystep_axis((AxisEnum)a, offs * planner.axis_steps_per_mm[a]);
          #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
            if (a == Z_AXIS && (!parser.seen('P') || parser.value_bool())) mod_zprobe_zoffset(offs);
          #endif
        }
    #else
      if (parser.seenval('Z') || parser.seenval('S')) {
        const float offs = constrain(parser.value_axis_units(Z_AXIS), -2, 2);
        thermalManager.babystep_axis(Z_AXIS, offs * planner.axis_steps_per_mm[Z_AXIS]);
        #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
          if (!parser.seen('P') || parser.value_bool()) mod_zprobe_zoffset(offs);
        #endif
      }
    #endif
  }

#endif // BABYSTEPPING

#if HAS_BUZZER

  /**
   * M300: Play beep sound S<frequency Hz> P<duration ms>
   */
  inline void gcode_M300() {
    uint16_t const frequency = parser.ushortval('S', 260);
    uint16_t duration = parser.ushortval('P', 1000);

    // Limits the tone duration to 0-5 seconds.
    NOMORE(duration, 5000);

    BUZZ(duration, frequency);
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
   * With PID_EXTRUSION_SCALING:
   *
   *   C[float] Kc term
   *   L[float] LPQ length
   */
  inline void gcode_M301() {

    // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
    // default behaviour (omitting E parameter) is to update for extruder 0 only
    const uint8_t e = parser.byteval('E'); // extruder being updated

    if (e < HOTENDS) { // catch bad input value
      if (parser.seen('P')) PID_PARAM(Kp, e) = parser.value_float();
      if (parser.seen('I')) PID_PARAM(Ki, e) = scalePID_i(parser.value_float());
      if (parser.seen('D')) PID_PARAM(Kd, e) = scalePID_d(parser.value_float());
      #if ENABLED(PID_EXTRUSION_SCALING)
        if (parser.seen('C')) PID_PARAM(Kc, e) = parser.value_float();
        if (parser.seen('L')) lpq_len = parser.value_float();
        NOMORE(lpq_len, LPQ_MAX_LEN);
      #endif

      thermalManager.updatePID();
      SERIAL_ECHO_START();
      #if ENABLED(PID_PARAMS_PER_HOTEND)
        SERIAL_ECHOPAIR(" e:", e); // specify extruder in serial output
      #endif // PID_PARAMS_PER_HOTEND
      SERIAL_ECHOPAIR(" p:", PID_PARAM(Kp, e));
      SERIAL_ECHOPAIR(" i:", unscalePID_i(PID_PARAM(Ki, e)));
      SERIAL_ECHOPAIR(" d:", unscalePID_d(PID_PARAM(Kd, e)));
      #if ENABLED(PID_EXTRUSION_SCALING)
        //Kc does not have scaling applied above, or in resetting defaults
        SERIAL_ECHOPAIR(" c:", PID_PARAM(Kc, e));
      #endif
      SERIAL_EOL();
    }
    else {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_INVALID_EXTRUDER);
    }
  }

#endif // PIDTEMP

#if ENABLED(PIDTEMPBED)

  inline void gcode_M304() {
    if (parser.seen('P')) thermalManager.bedKp = parser.value_float();
    if (parser.seen('I')) thermalManager.bedKi = scalePID_i(parser.value_float());
    if (parser.seen('D')) thermalManager.bedKd = scalePID_d(parser.value_float());

    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR(" p:", thermalManager.bedKp);
    SERIAL_ECHOPAIR(" i:", unscalePID_i(thermalManager.bedKi));
    SERIAL_ECHOLNPAIR(" d:", unscalePID_d(thermalManager.bedKd));
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

#if HAS_LCD_CONTRAST

  /**
   * M250: Read and optionally set the LCD contrast
   */
  inline void gcode_M250() {
    if (parser.seen('C')) set_lcd_contrast(parser.value_int());
    SERIAL_PROTOCOLPGM("lcd contrast value: ");
    SERIAL_PROTOCOL(lcd_contrast);
    SERIAL_EOL();
  }

#endif // HAS_LCD_CONTRAST

#if ENABLED(PREVENT_COLD_EXTRUSION)

  /**
   * M302: Allow cold extrudes, or set the minimum extrude temperature
   *
   *       S<temperature> sets the minimum extrude temperature
   *       P<bool> enables (1) or disables (0) cold extrusion
   *
   *  Examples:
   *
   *       M302         ; report current cold extrusion state
   *       M302 P0      ; enable cold extrusion checking
   *       M302 P1      ; disables cold extrusion checking
   *       M302 S0      ; always allow extrusion (disables checking)
   *       M302 S170    ; only allow extrusion above 170
   *       M302 S170 P1 ; set min extrude temp to 170 but leave disabled
   */
  inline void gcode_M302() {
    const bool seen_S = parser.seen('S');
    if (seen_S) {
      thermalManager.extrude_min_temp = parser.value_celsius();
      thermalManager.allow_cold_extrude = (thermalManager.extrude_min_temp == 0);
    }

    if (parser.seen('P'))
      thermalManager.allow_cold_extrude = (thermalManager.extrude_min_temp == 0) || parser.value_bool();
    else if (!seen_S) {
      // Report current state
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("Cold extrudes are ", (thermalManager.allow_cold_extrude ? "en" : "dis"));
      SERIAL_ECHOPAIR("abled (min temp ", thermalManager.extrude_min_temp);
      SERIAL_ECHOLNPGM("C)");
    }
  }

#endif // PREVENT_COLD_EXTRUSION

/**
 * M303: PID relay autotune
 *
 *       S<temperature> sets the target temperature. (default 150C / 70C)
 *       E<extruder> (-1 for the bed) (default 0)
 *       C<cycles>
 *       U<bool> with a non-zero value will apply the result to current settings
 */
inline void gcode_M303() {
  #if HAS_PID_HEATING
    const int e = parser.intval('E'), c = parser.intval('C', 5);
    const bool u = parser.boolval('U');

    int16_t temp = parser.celsiusval('S', e < 0 ? 70 : 150);

    if (WITHIN(e, 0, HOTENDS - 1))
      target_extruder = e;

    #if DISABLED(BUSY_WHILE_HEATING)
      KEEPALIVE_STATE(NOT_BUSY);
    #endif

    thermalManager.PID_autotune(temp, e, c, u);

    #if DISABLED(BUSY_WHILE_HEATING)
      KEEPALIVE_STATE(IN_HANDLER);
    #endif
  #else
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_M303_DISABLED);
  #endif
}

#if ENABLED(MORGAN_SCARA)

  bool SCARA_move_to_cal(const uint8_t delta_a, const uint8_t delta_b) {
    if (IsRunning()) {
      forward_kinematics_SCARA(delta_a, delta_b);
      destination[X_AXIS] = cartes[X_AXIS];
      destination[Y_AXIS] = cartes[Y_AXIS];
      destination[Z_AXIS] = current_position[Z_AXIS];
      prepare_move_to_destination();
      return true;
    }
    return false;
  }

  /**
   * M360: SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
   */
  inline bool gcode_M360() {
    SERIAL_ECHOLNPGM(" Cal: Theta 0");
    return SCARA_move_to_cal(0, 120);
  }

  /**
   * M361: SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M361() {
    SERIAL_ECHOLNPGM(" Cal: Theta 90");
    return SCARA_move_to_cal(90, 130);
  }

  /**
   * M362: SCARA calibration: Move to cal-position PsiA (0 deg calibration)
   */
  inline bool gcode_M362() {
    SERIAL_ECHOLNPGM(" Cal: Psi 0");
    return SCARA_move_to_cal(60, 180);
  }

  /**
   * M363: SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
   */
  inline bool gcode_M363() {
    SERIAL_ECHOLNPGM(" Cal: Psi 90");
    return SCARA_move_to_cal(50, 90);
  }

  /**
   * M364: SCARA calibration: Move to cal-position PsiC (90 deg to Theta calibration position)
   */
  inline bool gcode_M364() {
    SERIAL_ECHOLNPGM(" Cal: Theta-Psi 90");
    return SCARA_move_to_cal(45, 135);
  }

#endif // SCARA

#if ENABLED(EXT_SOLENOID)

  void enable_solenoid(const uint8_t num) {
    switch (num) {
      case 0:
        OUT_WRITE(SOL0_PIN, HIGH);
        break;
        #if HAS_SOLENOID_1 && EXTRUDERS > 1
          case 1:
            OUT_WRITE(SOL1_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_2 && EXTRUDERS > 2
          case 2:
            OUT_WRITE(SOL2_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_3 && EXTRUDERS > 3
          case 3:
            OUT_WRITE(SOL3_PIN, HIGH);
            break;
        #endif
        #if HAS_SOLENOID_4 && EXTRUDERS > 4
          case 4:
            OUT_WRITE(SOL4_PIN, HIGH);
            break;
        #endif
      default:
        SERIAL_ECHO_START();
        SERIAL_ECHOLNPGM(MSG_INVALID_SOLENOID);
        break;
    }
  }

  void enable_solenoid_on_active_extruder() { enable_solenoid(active_extruder); }

  void disable_all_solenoids() {
    OUT_WRITE(SOL0_PIN, LOW);
    #if HAS_SOLENOID_1 && EXTRUDERS > 1
      OUT_WRITE(SOL1_PIN, LOW);
    #endif
    #if HAS_SOLENOID_2 && EXTRUDERS > 2
      OUT_WRITE(SOL2_PIN, LOW);
    #endif
    #if HAS_SOLENOID_3 && EXTRUDERS > 3
      OUT_WRITE(SOL3_PIN, LOW);
    #endif
    #if HAS_SOLENOID_4 && EXTRUDERS > 4
      OUT_WRITE(SOL4_PIN, LOW);
    #endif
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
inline void gcode_M400() { stepper.synchronize(); }

#if HAS_BED_PROBE

  /**
   * M401: Deploy and activate the Z probe
   */
  inline void gcode_M401() { DEPLOY_PROBE(); }

  /**
   * M402: Deactivate and stow the Z probe
   */
  inline void gcode_M402() { STOW_PROBE(); }

#endif // HAS_BED_PROBE

#if ENABLED(FILAMENT_WIDTH_SENSOR)

  /**
   * M404: Display or set (in current units) the nominal filament width (3mm, 1.75mm ) W<3.0>
   */
  inline void gcode_M404() {
    if (parser.seen('W')) {
      filament_width_nominal = parser.value_linear_units();
      planner.volumetric_area_nominal = CIRCLE_AREA(filament_width_nominal * 0.5);
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
    // This is technically a linear measurement, but since it's quantized to centimeters and is a different
    // unit than everything else, it uses parser.value_byte() instead of parser.value_linear_units().
    if (parser.seen('D')) {
      meas_delay_cm = parser.value_byte();
      NOMORE(meas_delay_cm, MAX_MEASUREMENT_DELAY);
    }

    if (filwidth_delay_index[1] == -1) { // Initialize the ring buffer if not done since startup
      const int8_t temp_ratio = thermalManager.widthFil_to_size_ratio();

      for (uint8_t i = 0; i < COUNT(measurement_delay); ++i)
        measurement_delay[i] = temp_ratio;

      filwidth_delay_index[0] = filwidth_delay_index[1] = 0;
    }

    filament_sensor = true;
  }

  /**
   * M406: Turn off filament sensor for control
   */
  inline void gcode_M406() {
    filament_sensor = false;
    planner.calculate_volumetric_multipliers();   // Restore correct 'volumetric_multiplier' value
  }

  /**
   * M407: Get measured filament diameter on serial output
   */
  inline void gcode_M407() {
    SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
    SERIAL_PROTOCOLLN(filament_width_meas);
  }

#endif // FILAMENT_WIDTH_SENSOR

void quickstop_stepper() {
  stepper.quick_stop();
  stepper.synchronize();
  set_current_from_steppers_for_axis(ALL_AXES);
  SYNC_PLAN_POSITION_KINEMATIC();
}

#if HAS_LEVELING
  /**
   * M420: Enable/Disable Bed Leveling and/or set the Z fade height.
   *
   *   S[bool]   Turns leveling on or off
   *   Z[height] Sets the Z fade height (0 or none to disable)
   *   V[bool]   Verbose - Print the leveling grid
   *
   * With AUTO_BED_LEVELING_UBL only:
   *
   *   L[index]  Load UBL mesh from index (0 is default)
   */
  inline void gcode_M420() {

    const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

    #if ENABLED(AUTO_BED_LEVELING_UBL)

      // L to load a mesh from the EEPROM
      if (parser.seen('L')) {

        #if ENABLED(EEPROM_SETTINGS)
          const int8_t storage_slot = parser.has_value() ? parser.value_int() : ubl.storage_slot;
          const int16_t a = settings.calc_num_meshes();

          if (!a) {
            SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
            return;
          }

          if (!WITHIN(storage_slot, 0, a - 1)) {
            SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
            SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
            return;
          }

          settings.load_mesh(storage_slot);
          ubl.storage_slot = storage_slot;

        #else

          SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
          return;

        #endif
      }

      // L to load a mesh from the EEPROM
      if (parser.seen('L') || parser.seen('V')) {
        ubl.display_map(0);  // Currently only supports one map type
        SERIAL_ECHOLNPAIR("ubl.mesh_is_valid = ", ubl.mesh_is_valid());
        SERIAL_ECHOLNPAIR("ubl.storage_slot = ", ubl.storage_slot);
      }

    #endif // AUTO_BED_LEVELING_UBL

    // V to print the matrix or mesh
    if (parser.seen('V')) {
      #if ABL_PLANAR
        planner.bed_level_matrix.debug(PSTR("Bed Level Correction Matrix:"));
      #else
        if (leveling_is_valid()) {
          #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
            print_bilinear_leveling_grid();
            #if ENABLED(ABL_BILINEAR_SUBDIVISION)
              print_bilinear_leveling_grid_virt();
            #endif
          #elif ENABLED(MESH_BED_LEVELING)
            SERIAL_ECHOLNPGM("Mesh Bed Level data:");
            mbl.report_mesh();
          #endif
        }
      #endif
    }

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      if (parser.seen('Z')) set_z_fade_height(parser.value_linear_units(), false);
    #endif

    bool to_enable = false;
    if (parser.seen('S')) {
      to_enable = parser.value_bool();
      set_bed_leveling_enabled(to_enable);
    }

    const bool new_status = planner.leveling_active;

    if (to_enable && !new_status) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_M420_FAILED);
    }

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR("Bed Leveling ", new_status ? MSG_ON : MSG_OFF);

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("Fade Height ");
      if (planner.z_fade_height > 0.0)
        SERIAL_ECHOLN(planner.z_fade_height);
      else
        SERIAL_ECHOLNPGM(MSG_OFF);
    #endif

    // Report change in position
    if (memcmp(oldpos, current_position, sizeof(oldpos)))
      report_current_position();
  }
#endif

#if ENABLED(MESH_BED_LEVELING)

  /**
   * M421: Set a single Mesh Bed Leveling Z coordinate
   *
   * Usage:
   *   M421 X<linear> Y<linear> Z<linear>
   *   M421 X<linear> Y<linear> Q<offset>
   *   M421 I<xindex> J<yindex> Z<linear>
   *   M421 I<xindex> J<yindex> Q<offset>
   */
  inline void gcode_M421() {
    const bool hasX = parser.seen('X'), hasI = parser.seen('I');
    const int8_t ix = hasI ? parser.value_int() : hasX ? mbl.probe_index_x(parser.value_linear_units()) : -1;
    const bool hasY = parser.seen('Y'), hasJ = parser.seen('J');
    const int8_t iy = hasJ ? parser.value_int() : hasY ? mbl.probe_index_y(parser.value_linear_units()) : -1;
    const bool hasZ = parser.seen('Z'), hasQ = !hasZ && parser.seen('Q');

    if (int(hasI && hasJ) + int(hasX && hasY) != 1 || !(hasZ || hasQ)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_M421_PARAMETERS);
    }
    else if (ix < 0 || iy < 0) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_MESH_XY);
    }
    else
      mbl.set_z(ix, iy, parser.value_linear_units() + (hasQ ? mbl.z_values[ix][iy] : 0));
  }

#elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

  /**
   * M421: Set a single Mesh Bed Leveling Z coordinate
   *
   * Usage:
   *   M421 I<xindex> J<yindex> Z<linear>
   *   M421 I<xindex> J<yindex> Q<offset>
   */
  inline void gcode_M421() {
    int8_t ix = parser.intval('I', -1), iy = parser.intval('J', -1);
    const bool hasI = ix >= 0,
               hasJ = iy >= 0,
               hasZ = parser.seen('Z'),
               hasQ = !hasZ && parser.seen('Q');

    if (!hasI || !hasJ || !(hasZ || hasQ)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_M421_PARAMETERS);
    }
    else if (!WITHIN(ix, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_MESH_XY);
    }
    else {
      z_values[ix][iy] = parser.value_linear_units() + (hasQ ? z_values[ix][iy] : 0);
      #if ENABLED(ABL_BILINEAR_SUBDIVISION)
        bed_level_virt_interpolate();
      #endif
    }
  }

#elif ENABLED(AUTO_BED_LEVELING_UBL)

  /**
   * M421: Set a single Mesh Bed Leveling Z coordinate
   *
   * Usage:
   *   M421 I<xindex> J<yindex> Z<linear>
   *   M421 I<xindex> J<yindex> Q<offset>
   *   M421 C Z<linear>
   *   M421 C Q<offset>
   */
  inline void gcode_M421() {
    int8_t ix = parser.intval('I', -1), iy = parser.intval('J', -1);
    const bool hasI = ix >= 0,
               hasJ = iy >= 0,
               hasC = parser.seen('C'),
               hasZ = parser.seen('Z'),
               hasQ = !hasZ && parser.seen('Q');

    if (hasC) {
      const mesh_index_pair location = ubl.find_closest_mesh_point_of_type(REAL, current_position[X_AXIS], current_position[Y_AXIS], USE_NOZZLE_AS_REFERENCE, NULL);
      ix = location.x_index;
      iy = location.y_index;
    }

    if (int(hasC) + int(hasI && hasJ) != 1 || !(hasZ || hasQ)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_M421_PARAMETERS);
    }
    else if (!WITHIN(ix, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_MESH_XY);
    }
    else
      ubl.z_values[ix][iy] = parser.value_linear_units() + (hasQ ? ubl.z_values[ix][iy] : 0);
  }

#endif // AUTO_BED_LEVELING_UBL

#if HAS_M206_COMMAND

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
    if (axis_unhomed_error()) return;

    float diff[XYZ];
    LOOP_XYZ(i) {
      diff[i] = base_home_pos((AxisEnum)i) - current_position[i];
      if (!WITHIN(diff[i], -20, 20) && home_dir((AxisEnum)i) > 0)
        diff[i] = -current_position[i];
      if (!WITHIN(diff[i], -20, 20)) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_ERR_M428_TOO_FAR);
        LCD_ALERTMESSAGEPGM("Err: Too far!");
        BUZZ(200, 40);
        return;
      }
    }

    LOOP_XYZ(i) set_home_offset((AxisEnum)i, diff[i]);
    report_current_position();
    LCD_MESSAGEPGM(MSG_HOME_OFFSETS_APPLIED);
    BUZZ(100, 659);
    BUZZ(100, 698);
  }

#endif // HAS_M206_COMMAND

/**
 * M500: Store settings in EEPROM
 */
inline void gcode_M500() {
  (void)settings.save();
}

/**
 * M501: Read settings from EEPROM
 */
inline void gcode_M501() {
  (void)settings.load();
}

/**
 * M502: Revert to default settings
 */
inline void gcode_M502() {
  (void)settings.reset();
}

#if DISABLED(DISABLE_M503)
  /**
   * M503: print settings currently in memory
   */
  inline void gcode_M503() {
    (void)settings.report(parser.seen('S') && !parser.value_bool());
  }
#endif

#if ENABLED(EEPROM_SETTINGS)
  /**
   * M504: Validate EEPROM Contents
   */
  inline void gcode_M504() {
    if (settings.validate()) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("EEPROM OK");
    }
  }
#endif

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)

  /**
   * M540: Set whether SD card print should abort on endstop hit (M540 S<0|1>)
   */
  inline void gcode_M540() {
    if (parser.seen('S')) stepper.abort_on_endstop_hit = parser.value_bool();
  }

#endif // ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

#if HAS_BED_PROBE

  inline void gcode_M851() {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_PROBE_Z_OFFSET);
    if (parser.seen('Z')) {
      const float value = parser.value_linear_units();
      if (!WITHIN(value, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        SERIAL_ECHOLNPGM(" " MSG_Z_MIN " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MIN) " " MSG_Z_MAX " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MAX));
        return;
      }
      zprobe_zoffset = value;
    }
    SERIAL_ECHOLNPAIR(": ", zprobe_zoffset);
  }

#endif // HAS_BED_PROBE

#if ENABLED(SKEW_CORRECTION_GCODE)

  /**
   * M852: Get or set the machine skew factors. Reports current values with no arguments.
   *
   *  S[xy_factor] - Alias for 'I'
   *  I[xy_factor] - New XY skew factor
   *  J[xz_factor] - New XZ skew factor
   *  K[yz_factor] - New YZ skew factor
   */
  inline void gcode_M852() {
    uint8_t ijk = 0, badval = 0, setval = 0;

    if (parser.seen('I') || parser.seen('S')) {
      ++ijk;
      const float value = parser.value_linear_units();
      if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
        if (planner.xy_skew_factor != value) {
          planner.xy_skew_factor = value;
          ++setval;
        }
      }
      else
        ++badval;
    }

    #if ENABLED(SKEW_CORRECTION_FOR_Z)

      if (parser.seen('J')) {
        ++ijk;
        const float value = parser.value_linear_units();
        if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
          if (planner.xz_skew_factor != value) {
            planner.xz_skew_factor = value;
            ++setval;
          }
        }
        else
          ++badval;
      }

      if (parser.seen('K')) {
        ++ijk;
        const float value = parser.value_linear_units();
        if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
          if (planner.yz_skew_factor != value) {
            planner.yz_skew_factor = value;
            ++setval;
          }
        }
        else
          ++badval;
      }

    #endif

    if (badval)
      SERIAL_ECHOLNPGM(MSG_SKEW_MIN " " STRINGIFY(SKEW_FACTOR_MIN) " " MSG_SKEW_MAX " " STRINGIFY(SKEW_FACTOR_MAX));

    // When skew is changed the current position changes
    if (setval) {
      set_current_from_steppers_for_axis(ALL_AXES);
      SYNC_PLAN_POSITION_KINEMATIC();
      report_current_position();
    }

    if (!ijk) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM(MSG_SKEW_FACTOR " XY: ");
      SERIAL_ECHO_F(planner.xy_skew_factor, 6);
      SERIAL_EOL();
      #if ENABLED(SKEW_CORRECTION_FOR_Z)
        SERIAL_ECHOPAIR(" XZ: ", planner.xz_skew_factor);
        SERIAL_ECHOLNPAIR(" YZ: ", planner.yz_skew_factor);
      #else
        SERIAL_EOL();
      #endif
    }
  }

#endif // SKEW_CORRECTION_GCODE

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  /**
   * M600: Pause for filament change
   *
   *  E[distance] - Retract the filament this far
   *  Z[distance] - Move the Z axis by this distance
   *  X[position] - Move to this X position, with Y
   *  Y[position] - Move to this Y position, with X
   *  U[distance] - Retract distance for removal (manual reload)
   *  L[distance] - Extrude distance for insertion (manual reload)
   *  B[count]    - Number of times to beep, -1 for indefinite (if equipped with a buzzer)
   *  T[toolhead] - Select extruder for filament change
   *
   *  Default values are used for omitted arguments.
   */
  inline void gcode_M600() {
    point_t park_point = NOZZLE_PARK_POINT;

    if (get_target_extruder_from_command(600)) return;

    // Show initial message
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT, ADVANCED_PAUSE_MODE_PAUSE_PRINT, target_extruder);
    #endif

    #if ENABLED(HOME_BEFORE_FILAMENT_CHANGE)
      // Don't allow filament change without homing first
      if (axis_unhomed_error()) home_all_axes();
    #endif

    #if EXTRUDERS > 1
      // Change toolhead if specified
      uint8_t active_extruder_before_filament_change = active_extruder;
      if (active_extruder != target_extruder)
        tool_change(target_extruder, 0, true);
    #endif

    // Initial retract before move to filament change position
    const float retract = -FABS(parser.seen('E') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef PAUSE_PARK_RETRACT_LENGTH
        + (PAUSE_PARK_RETRACT_LENGTH)
      #endif
    );

    // Lift Z axis
    if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

    // Move XY axes to filament change position or given position
    if (parser.seenval('X')) park_point.x = parser.linearval('X');
    if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

    #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE)
      park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
      park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
    #endif

    // Unload filament
    const float unload_length = -FABS(parser.seen('U') ? parser.value_axis_units(E_AXIS) :
                                                         filament_change_unload_length[active_extruder]);

    // Load filament
    const float load_length = FABS(parser.seen('L') ? parser.value_axis_units(E_AXIS) :
                                                      filament_change_load_length[active_extruder]);

    const int beep_count = parser.intval('B',
      #ifdef FILAMENT_CHANGE_ALERT_BEEPS
        FILAMENT_CHANGE_ALERT_BEEPS
      #else
        -1
      #endif
    );

    const bool job_running = print_job_timer.isRunning();

    if (pause_print(retract, park_point, unload_length, true)) {
      wait_for_filament_reload(beep_count);
      resume_print(load_length, ADVANCED_PAUSE_EXTRUDE_LENGTH, beep_count);
    }

    #if EXTRUDERS > 1
      // Restore toolhead if it was changed
      if (active_extruder_before_filament_change != active_extruder)
        tool_change(active_extruder_before_filament_change, 0, true);
    #endif

    // Resume the print job timer if it was running
    if (job_running) print_job_timer.start();
  }

  /**
   * M603: Configure filament change
   *
   *  T[toolhead] - Select extruder to configure, active extruder if not specified
   *  U[distance] - Retract distance for removal, for the specified extruder
   *  L[distance] - Extrude distance for insertion, for the specified extruder
   *
   */
  inline void gcode_M603() {

    if (get_target_extruder_from_command(603)) return;

    // Unload length
    if (parser.seen('U')) {
      filament_change_unload_length[target_extruder] = FABS(parser.value_axis_units(E_AXIS));
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        NOMORE(filament_change_unload_length[target_extruder], EXTRUDE_MAXLENGTH);
      #endif
    }

    // Load length
    if (parser.seen('L')) {
      filament_change_load_length[target_extruder] = FABS(parser.value_axis_units(E_AXIS));
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        NOMORE(filament_change_load_length[target_extruder], EXTRUDE_MAXLENGTH);
      #endif
    }
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(MK2_MULTIPLEXER)

  inline void select_multiplexed_stepper(const uint8_t e) {
    stepper.synchronize();
    disable_e_steppers();
    WRITE(E_MUX0_PIN, TEST(e, 0) ? HIGH : LOW);
    WRITE(E_MUX1_PIN, TEST(e, 1) ? HIGH : LOW);
    WRITE(E_MUX2_PIN, TEST(e, 2) ? HIGH : LOW);
    safe_delay(100);
  }

#endif // MK2_MULTIPLEXER

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * M605: Set dual x-carriage movement mode
   *
   *    M605 S0: Full control mode. The slicer has full control over x-carriage movement
   *    M605 S1: Auto-park mode. The inactive head will auto park/unpark without slicer involvement
   *    M605 S2 [Xnnn] [Rmmm]: Duplication mode. The second extruder will duplicate the first with nnn
   *                         units x-offset and an optional differential hotend temperature of
   *                         mmm degrees. E.g., with "M605 S2 X100 R2" the second extruder will duplicate
   *                         the first with a spacing of 100mm in the x direction and 2 degrees hotter.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  inline void gcode_M605() {
    stepper.synchronize();
    if (parser.seen('S')) dual_x_carriage_mode = (DualXMode)parser.value_byte();
    switch (dual_x_carriage_mode) {
      case DXC_FULL_CONTROL_MODE:
      case DXC_AUTO_PARK_MODE:
        break;
      case DXC_DUPLICATION_MODE:
        if (parser.seen('X')) duplicate_extruder_x_offset = max(parser.value_linear_units(), X2_MIN_POS - x_home_pos(0));
        if (parser.seen('R')) duplicate_extruder_temp_offset = parser.value_celsius_diff();
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(hotend_offset[X_AXIS][0]);
        SERIAL_CHAR(',');
        SERIAL_ECHO(hotend_offset[Y_AXIS][0]);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(duplicate_extruder_x_offset);
        SERIAL_CHAR(',');
        SERIAL_ECHOLN(hotend_offset[Y_AXIS][1]);
        break;
      default:
        dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
        break;
    }
    active_extruder_parked = false;
    extruder_duplication_enabled = false;
    delayed_move_time = 0;
  }

#elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

  inline void gcode_M605() {
    stepper.synchronize();
    extruder_duplication_enabled = parser.intval('S') == (int)DXC_DUPLICATION_MODE;
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_DUPLICATION_MODE, extruder_duplication_enabled ? MSG_ON : MSG_OFF);
  }

#endif // DUAL_NOZZLE_DUPLICATION_MODE

#if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)

  /**
   * M701: Load filament
   *
   *  T[extruder] - Optional extruder number. Current extruder if omitted.
   *  Z[distance] - Move the Z axis by this distance
   *  L[distance] - Extrude distance for insertion (positive value) (manual reload)
   *
   *  Default values are used for omitted arguments.
   */
  inline void gcode_M701() {
    point_t park_point = NOZZLE_PARK_POINT;

    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      // Only raise Z if the machine is homed
      if (axis_unhomed_error()) park_point.z = 0;
    #endif

    if (get_target_extruder_from_command(701)) return;

    // Z axis lift
    if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

    // Load filament
    const float load_length = FABS(parser.seen('L') ? parser.value_axis_units(E_AXIS) :
                                                      filament_change_load_length[target_extruder]);

    // Show initial "wait for load" message
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_LOAD, ADVANCED_PAUSE_MODE_LOAD_FILAMENT, target_extruder);
    #endif

    #if EXTRUDERS > 1
      // Change toolhead if specified
      uint8_t active_extruder_before_filament_change = active_extruder;
      if (active_extruder != target_extruder)
        tool_change(target_extruder, 0, true);
    #endif

    // Lift Z axis
    if (park_point.z > 0)
      do_blocking_move_to_z(min(current_position[Z_AXIS] + park_point.z, Z_MAX_POS), NOZZLE_PARK_Z_FEEDRATE);

    load_filament(load_length, ADVANCED_PAUSE_EXTRUDE_LENGTH, FILAMENT_CHANGE_ALERT_BEEPS, true,
                  thermalManager.wait_for_heating(target_extruder), ADVANCED_PAUSE_MODE_LOAD_FILAMENT);

    // Restore Z axis
    if (park_point.z > 0)
      do_blocking_move_to_z(max(current_position[Z_AXIS] - park_point.z, Z_MIN_POS), NOZZLE_PARK_Z_FEEDRATE);

    #if EXTRUDERS > 1
      // Restore toolhead if it was changed
      if (active_extruder_before_filament_change != active_extruder)
        tool_change(active_extruder_before_filament_change, 0, true);
    #endif

    // Show status screen
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
    #endif
  }

  /**
   * M702: Unload filament
   *
   *  T[extruder] - Optional extruder number. If omitted, current extruder
   *                (or ALL extruders with FILAMENT_UNLOAD_ALL_EXTRUDERS).
   *  Z[distance] - Move the Z axis by this distance
   *  U[distance] - Retract distance for removal (manual reload)
   *
   *  Default values are used for omitted arguments.
   */
  inline void gcode_M702() {
    point_t park_point = NOZZLE_PARK_POINT;

    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      // Only raise Z if the machine is homed
      if (axis_unhomed_error()) park_point.z = 0;
    #endif

    if (get_target_extruder_from_command(702)) return;

    // Z axis lift
    if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

    // Show initial message
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_UNLOAD, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT, target_extruder);
    #endif

    #if EXTRUDERS > 1
      // Change toolhead if specified
      uint8_t active_extruder_before_filament_change = active_extruder;
      if (active_extruder != target_extruder)
        tool_change(target_extruder, 0, true);
    #endif

    // Lift Z axis
    if (park_point.z > 0)
      do_blocking_move_to_z(min(current_position[Z_AXIS] + park_point.z, Z_MAX_POS), NOZZLE_PARK_Z_FEEDRATE);

    // Unload filament
    #if EXTRUDERS > 1 && ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
      if (!parser.seenval('T')) {
        HOTEND_LOOP() {
          if (e != active_extruder) tool_change(e, 0, true);
          unload_filament(-filament_change_unload_length[e], true, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT);
        }
      }
      else
    #endif
    {
      // Unload length
      const float unload_length = -FABS(parser.seen('U') ? parser.value_axis_units(E_AXIS) :
                                                          filament_change_unload_length[target_extruder]);

      unload_filament(unload_length, true, ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT);
    }

    // Restore Z axis
    if (park_point.z > 0)
      do_blocking_move_to_z(max(current_position[Z_AXIS] - park_point.z, Z_MIN_POS), NOZZLE_PARK_Z_FEEDRATE);

    #if EXTRUDERS > 1
      // Restore toolhead if it was changed
      if (active_extruder_before_filament_change != active_extruder)
        tool_change(active_extruder_before_filament_change, 0, true);
    #endif

    // Show status screen
    #if ENABLED(ULTIPANEL)
      lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_STATUS);
    #endif
  }

#endif // FILAMENT_LOAD_UNLOAD_GCODES

#if ENABLED(LIN_ADVANCE)
  /**
   * M900: Get or Set Linear Advance K-factor
   *
   *  K<factor>   Set advance K factor
   */
  inline void gcode_M900() {
    if (parser.seenval('K')) {
      const float newK = parser.floatval('K');
      if (WITHIN(newK, 0, 10)) {
        stepper.synchronize();
        planner.extruder_advance_K = newK;
      }
      else
        SERIAL_PROTOCOLLNPGM("?K value out of range (0-10).");
    }
    else {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR("Advance K=", planner.extruder_advance_K);
    }
  }
#endif // LIN_ADVANCE

#if HAS_TRINAMIC
  #if ENABLED(TMC_DEBUG)
    inline void gcode_M122() {
      if (parser.seen('S'))
        tmc_set_report_status(parser.value_bool());
      else
        tmc_report_all();
    }
  #endif // TMC_DEBUG

  /**
   * M906: Set motor current in milliamps using axis codes X, Y, Z, E
   * Report driver currents when no axis specified
   */
  inline void gcode_M906() {
    uint16_t values[XYZE];
    LOOP_XYZE(i) values[i] = parser.intval(axis_codes[i]);

    #define TMC_SET_GET_CURRENT(P,Q) do { \
      if (values[P##_AXIS]) tmc_set_current(stepper##Q, extended_axis_codes[TMC_##Q], values[P##_AXIS]); \
      else tmc_get_current(stepper##Q, extended_axis_codes[TMC_##Q]); } while(0)

    #if X_IS_TRINAMIC
      TMC_SET_GET_CURRENT(X,X);
    #endif
    #if X2_IS_TRINAMIC
      TMC_SET_GET_CURRENT(X,X2);
    #endif
    #if Y_IS_TRINAMIC
      TMC_SET_GET_CURRENT(Y,Y);
    #endif
    #if Y2_IS_TRINAMIC
      TMC_SET_GET_CURRENT(Y,Y2);
    #endif
    #if Z_IS_TRINAMIC
      TMC_SET_GET_CURRENT(Z,Z);
    #endif
    #if Z2_IS_TRINAMIC
      TMC_SET_GET_CURRENT(Z,Z2);
    #endif
    #if E0_IS_TRINAMIC
      TMC_SET_GET_CURRENT(E,E0);
    #endif
    #if E1_IS_TRINAMIC
      TMC_SET_GET_CURRENT(E,E1);
    #endif
    #if E2_IS_TRINAMIC
      TMC_SET_GET_CURRENT(E,E2);
    #endif
    #if E3_IS_TRINAMIC
      TMC_SET_GET_CURRENT(E,E3);
    #endif
    #if E4_IS_TRINAMIC
      TMC_SET_GET_CURRENT(E,E4);
    #endif
  }

  /**
   * M911: Report TMC stepper driver overtemperature pre-warn flag
   * The flag is held by the library and persist until manually cleared by M912
   */
  inline void gcode_M911() {
    #if ENABLED(X_IS_TMC2130) || (ENABLED(X_IS_TMC2208) && PIN_EXISTS(X_SERIAL_RX)) || ENABLED(IS_TRAMS)
      tmc_report_otpw(stepperX, extended_axis_codes[TMC_X]);
    #endif
    #if ENABLED(Y_IS_TMC2130) || (ENABLED(Y_IS_TMC2208) && PIN_EXISTS(Y_SERIAL_RX)) || ENABLED(IS_TRAMS)
      tmc_report_otpw(stepperY, extended_axis_codes[TMC_Y]);
    #endif
    #if ENABLED(Z_IS_TMC2130) || (ENABLED(Z_IS_TMC2208) && PIN_EXISTS(Z_SERIAL_RX)) || ENABLED(IS_TRAMS)
      tmc_report_otpw(stepperZ, extended_axis_codes[TMC_Z]);
    #endif
    #if ENABLED(E0_IS_TMC2130) || (ENABLED(E0_IS_TMC2208) && PIN_EXISTS(E0_SERIAL_RX)) || ENABLED(IS_TRAMS)
      tmc_report_otpw(stepperE0, extended_axis_codes[TMC_E0]);
    #endif
  }

  /**
   * M912: Clear TMC stepper driver overtemperature pre-warn flag held by the library
   */
  inline void gcode_M912() {
    const bool clearX = parser.seen(axis_codes[X_AXIS]), clearY = parser.seen(axis_codes[Y_AXIS]), clearZ = parser.seen(axis_codes[Z_AXIS]), clearE = parser.seen(axis_codes[E_AXIS]),
             clearAll = (!clearX && !clearY && !clearZ && !clearE) || (clearX && clearY && clearZ && clearE);
    #if ENABLED(X_IS_TMC2130) || ENABLED(IS_TRAMS) || (ENABLED(X_IS_TMC2208) && PIN_EXISTS(X_SERIAL_RX))
      if (clearX || clearAll) tmc_clear_otpw(stepperX, extended_axis_codes[TMC_X]);
    #endif
    #if ENABLED(X2_IS_TMC2130) || (ENABLED(X2_IS_TMC2208) && PIN_EXISTS(X_SERIAL_RX))
      if (clearX || clearAll) tmc_clear_otpw(stepperX, extended_axis_codes[TMC_X]);
    #endif

    #if ENABLED(Y_IS_TMC2130) || (ENABLED(Y_IS_TMC2208) && PIN_EXISTS(Y_SERIAL_RX))
      if (clearY || clearAll) tmc_clear_otpw(stepperY, extended_axis_codes[TMC_Y]);
    #endif

    #if ENABLED(Z_IS_TMC2130) || (ENABLED(Z_IS_TMC2208) && PIN_EXISTS(Z_SERIAL_RX))
      if (clearZ || clearAll) tmc_clear_otpw(stepperZ, extended_axis_codes[TMC_Z]);
    #endif

    #if ENABLED(E0_IS_TMC2130) || (ENABLED(E0_IS_TMC2208) && PIN_EXISTS(E0_SERIAL_RX))
      if (clearE || clearAll) tmc_clear_otpw(stepperE0, extended_axis_codes[TMC_E0]);
    #endif
  }

  /**
   * M913: Set HYBRID_THRESHOLD speed.
   */
  #if ENABLED(HYBRID_THRESHOLD)
    inline void gcode_M913() {
      uint16_t values[XYZE];
      LOOP_XYZE(i) values[i] = parser.intval(axis_codes[i]);

      #define TMC_SET_GET_PWMTHRS(P,Q) do { \
        if (values[P##_AXIS]) tmc_set_pwmthrs(stepper##Q, extended_axis_codes[TMC_##Q], values[P##_AXIS], planner.axis_steps_per_mm[P##_AXIS]); \
        else tmc_get_pwmthrs(stepper##Q, extended_axis_codes[TMC_##Q], planner.axis_steps_per_mm[P##_AXIS]); } while(0)

      #if X_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(X,X);
      #endif
      #if X2_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(X,X2);
      #endif
      #if Y_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(Y,Y);
      #endif
      #if Y2_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(Y,Y2);
      #endif
      #if Z_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(Z,Z);
      #endif
      #if Z2_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(Z,Z2);
      #endif
      #if E0_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(E,E0);
      #endif
      #if E1_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(E,E1);
      #endif
      #if E2_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(E,E2);
      #endif
      #if E3_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(E,E3);
      #endif
      #if E4_IS_TRINAMIC
        TMC_SET_GET_PWMTHRS(E,E4);
      #endif
    }
  #endif // HYBRID_THRESHOLD

  /**
   * M914: Set SENSORLESS_HOMING sensitivity.
   */
  #if ENABLED(SENSORLESS_HOMING)
    inline void gcode_M914() {
      #define TMC_SET_GET_SGT(P,Q) do { \
        if (parser.seen(axis_codes[P##_AXIS])) tmc_set_sgt(stepper##Q, extended_axis_codes[TMC_##Q], parser.value_int()); \
        else tmc_get_sgt(stepper##Q, extended_axis_codes[TMC_##Q]); } while(0)

      #ifdef X_HOMING_SENSITIVITY
        #if ENABLED(X_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SET_GET_SGT(X,X);
        #endif
        #if ENABLED(X2_IS_TMC2130)
          TMC_SET_GET_SGT(X,X2);
        #endif
      #endif
      #ifdef Y_HOMING_SENSITIVITY
        #if ENABLED(Y_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SET_GET_SGT(Y,Y);
        #endif
        #if ENABLED(Y2_IS_TMC2130)
          TMC_SET_GET_SGT(Y,Y2);
        #endif
      #endif
      #ifdef Z_HOMING_SENSITIVITY
        #if ENABLED(Z_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SET_GET_SGT(Z,Z);
        #endif
        #if ENABLED(Z2_IS_TMC2130)
          TMC_SET_GET_SGT(Z,Z2);
        #endif
      #endif
    }
  #endif // SENSORLESS_HOMING

  /**
   * TMC Z axis calibration routine
   */
  #if ENABLED(TMC_Z_CALIBRATION)
    inline void gcode_M915() {
      uint16_t _rms = parser.seenval('S') ? parser.value_int() : CALIBRATION_CURRENT;
      uint16_t _z = parser.seenval('Z') ? parser.value_int() : CALIBRATION_EXTRA_HEIGHT;

      if (!axis_known_position[Z_AXIS]) {
        SERIAL_ECHOLNPGM("\nPlease home Z axis first");
        return;
      }

      #if Z_IS_TRINAMIC
        uint16_t Z_current_1 = stepperZ.getCurrent(),
        stepperZ.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
      #endif
      #if Z2_IS_TRINAMIC
        uint16_t Z2_current_1 = stepperZ.getCurrent();
        stepperZ2.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
      #endif

      SERIAL_ECHOPAIR("\nCalibration current: Z", _rms);

      soft_endstops_enabled = false;

      do_blocking_move_to_z(Z_MAX_POS+_z);

      #if Z_IS_TRINAMIC
        stepperZ.setCurrent(Z_current_1, R_SENSE, HOLD_MULTIPLIER);
      #endif
      #if Z2_IS_TRINAMIC
        stepperZ2.setCurrent(Z2_current_1, R_SENSE, HOLD_MULTIPLIER);
      #endif

      do_blocking_move_to_z(Z_MAX_POS);
      soft_endstops_enabled = true;

      SERIAL_ECHOLNPGM("\nHoming Z due to lost steps");
      enqueue_and_echo_commands_P(PSTR("G28 Z"));
    }
  #endif

#endif // HAS_TRINAMIC

/**
 * M907: Set digital trimpot motor current using axis codes X, Y, Z, E, B, S
 */
inline void gcode_M907() {
  #if HAS_DIGIPOTSS

    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.digipot_current(i, parser.value_int());
    if (parser.seen('B')) stepper.digipot_current(4, parser.value_int());
    if (parser.seen('S')) for (uint8_t i = 0; i <= 4; i++) stepper.digipot_current(i, parser.value_int());

  #elif HAS_MOTOR_CURRENT_PWM

    #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
      if (parser.seen('X')) stepper.digipot_current(0, parser.value_int());
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
      if (parser.seen('Z')) stepper.digipot_current(1, parser.value_int());
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
      if (parser.seen('E')) stepper.digipot_current(2, parser.value_int());
    #endif

  #endif

  #if ENABLED(DIGIPOT_I2C)
    // this one uses actual amps in floating point
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) digipot_i2c_set_current(i, parser.value_float());
    // for each additional extruder (named B,C,D,E..., channels 4,5,6,7...)
    for (uint8_t i = NUM_AXIS; i < DIGIPOT_I2C_NUM_CHANNELS; i++) if (parser.seen('B' + i - (NUM_AXIS))) digipot_i2c_set_current(i, parser.value_float());
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    if (parser.seen('S')) {
      const float dac_percent = parser.value_float();
      for (uint8_t i = 0; i <= 4; i++) dac_current_percent(i, dac_percent);
    }
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) dac_current_percent(i, parser.value_float());
  #endif
}

#if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)

  /**
   * M908: Control digital trimpot directly (M908 P<pin> S<current>)
   */
  inline void gcode_M908() {
    #if HAS_DIGIPOTSS
      stepper.digitalPotWrite(
        parser.intval('P'),
        parser.intval('S')
      );
    #endif
    #ifdef DAC_STEPPER_CURRENT
      dac_current_raw(
        parser.byteval('P', -1),
        parser.ushortval('S', 0)
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
    if (parser.seen('S')) for (int i = 0; i <= 4; i++) stepper.microstep_mode(i, parser.value_byte());
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.microstep_mode(i, parser.value_byte());
    if (parser.seen('B')) stepper.microstep_mode(4, parser.value_byte());
    stepper.microstep_readings();
  }

  /**
   * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
   *       S# determines MS1 or MS2, X# sets the pin high/low.
   */
  inline void gcode_M351() {
    if (parser.seenval('S')) switch (parser.value_byte()) {
      case 1:
        LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, parser.value_byte(), -1);
        if (parser.seenval('B')) stepper.microstep_ms(4, parser.value_byte(), -1);
        break;
      case 2:
        LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, -1, parser.value_byte());
        if (parser.seenval('B')) stepper.microstep_ms(4, -1, parser.value_byte());
        break;
    }
    stepper.microstep_readings();
  }

#endif // HAS_MICROSTEPS

#if HAS_CASE_LIGHT

  #ifndef INVERT_CASE_LIGHT
    #define INVERT_CASE_LIGHT false
  #endif
  uint8_t case_light_brightness;  // LCD routine wants INT
  bool case_light_on;

  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
    LEDColor case_light_color =
      #ifdef CASE_LIGHT_NEOPIXEL_COLOR
        CASE_LIGHT_NEOPIXEL_COLOR
      #else
        { 255, 255, 255, 255 }
      #endif
    ;
  #endif

  void update_case_light() {
    const uint8_t i = case_light_on ? case_light_brightness : 0, n10ct = INVERT_CASE_LIGHT ? 255 - i : i;

    #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)

      leds.set_color(
        MakeLEDColor(case_light_color.r, case_light_color.g, case_light_color.b, case_light_color.w, n10ct),
        false
      );

    #else // !CASE_LIGHT_USE_NEOPIXEL

      SET_OUTPUT(CASE_LIGHT_PIN);
      if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
        analogWrite(CASE_LIGHT_PIN, n10ct);
      else {
        const bool s = case_light_on ? !INVERT_CASE_LIGHT : INVERT_CASE_LIGHT;
        WRITE(CASE_LIGHT_PIN, s ? HIGH : LOW);
      }

    #endif // !CASE_LIGHT_USE_NEOPIXEL
  }
#endif // HAS_CASE_LIGHT

/**
 * M355: Turn case light on/off and set brightness
 *
 *   P<byte>  Set case light brightness (PWM pin required - ignored otherwise)
 *
 *   S<bool>  Set case light on/off
 *
 *   When S turns on the light on a PWM pin then the current brightness level is used/restored
 *
 *   M355 P200 S0 turns off the light & sets the brightness level
 *   M355 S1 turns on the light with a brightness of 200 (assuming a PWM pin)
 */
inline void gcode_M355() {
  #if HAS_CASE_LIGHT
    uint8_t args = 0;
    if (parser.seenval('P')) ++args, case_light_brightness = parser.value_byte();
    if (parser.seenval('S')) ++args, case_light_on = parser.value_bool();
    if (args) update_case_light();

    // always report case light status
    SERIAL_ECHO_START();
    if (!case_light_on) {
      SERIAL_ECHOLNPGM("Case light: off");
    }
    else {
      if (!USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)) SERIAL_ECHOLNPGM("Case light: on");
      else SERIAL_ECHOLNPAIR("Case light: ", (int)case_light_brightness);
    }

  #else
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_M355_NONE);
  #endif // HAS_CASE_LIGHT
}

#if ENABLED(MIXING_EXTRUDER)

  /**
   * M163: Set a single mix factor for a mixing extruder
   *       This is called "weight" by some systems.
   *
   *   S[index]   The channel index to set
   *   P[float]   The mix value
   *
   */
  inline void gcode_M163() {
    const int mix_index = parser.intval('S');
    if (mix_index < MIXING_STEPPERS) {
      float mix_value = parser.floatval('P');
      NOLESS(mix_value, 0.0);
      mixing_factor[mix_index] = RECIPROCAL(mix_value);
    }
  }

  #if MIXING_VIRTUAL_TOOLS > 1

    /**
     * M164: Store the current mix factors as a virtual tool.
     *
     *   S[index]   The virtual tool to store
     *
     */
    inline void gcode_M164() {
      const int tool_index = parser.intval('S');
      if (tool_index < MIXING_VIRTUAL_TOOLS) {
        normalize_mix();
        for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
          mixing_virtual_tool_mix[tool_index][i] = mixing_factor[i];
      }
    }

  #endif

  #if ENABLED(DIRECT_MIXING_IN_G1)
    /**
     * M165: Set multiple mix factors for a mixing extruder.
     *       Factors that are left out will be set to 0.
     *       All factors together must add up to 1.0.
     *
     *   A[factor] Mix factor for extruder stepper 1
     *   B[factor] Mix factor for extruder stepper 2
     *   C[factor] Mix factor for extruder stepper 3
     *   D[factor] Mix factor for extruder stepper 4
     *   H[factor] Mix factor for extruder stepper 5
     *   I[factor] Mix factor for extruder stepper 6
     *
     */
    inline void gcode_M165() { gcode_get_mix(); }
  #endif

#endif // MIXING_EXTRUDER

/**
 * M999: Restart after being stopped
 *
 * Default behaviour is to flush the serial buffer and request
 * a resend to the host starting on the last N line received.
 *
 * Sending "M999 S1" will resume printing without flushing the
 * existing command buffer.
 *
 */
inline void gcode_M999() {
  Running = true;
  lcd_reset_alert_level();

  if (parser.boolval('S')) return;

  // gcode_LastN = Stopped_gcode_LastN;
  FlushSerialRequestResend();
}

#if ENABLED(SWITCHING_EXTRUDER)
  #if EXTRUDERS > 3
    #define REQ_ANGLES 4
    #define _SERVO_NR (e < 2 ? SWITCHING_EXTRUDER_SERVO_NR : SWITCHING_EXTRUDER_E23_SERVO_NR)
  #else
    #define REQ_ANGLES 2
    #define _SERVO_NR SWITCHING_EXTRUDER_SERVO_NR
  #endif
  inline void move_extruder_servo(const uint8_t e) {
    constexpr int16_t angles[] = SWITCHING_EXTRUDER_SERVO_ANGLES;
    static_assert(COUNT(angles) == REQ_ANGLES, "SWITCHING_EXTRUDER_SERVO_ANGLES needs " STRINGIFY(REQ_ANGLES) " angles.");
    stepper.synchronize();
    #if EXTRUDERS & 1
      if (e < EXTRUDERS - 1)
    #endif
    {
      MOVE_SERVO(_SERVO_NR, angles[e]);
      safe_delay(500);
    }
  }
#endif // SWITCHING_EXTRUDER

#if ENABLED(SWITCHING_NOZZLE)
  inline void move_nozzle_servo(const uint8_t e) {
    const int16_t angles[2] = SWITCHING_NOZZLE_SERVO_ANGLES;
    stepper.synchronize();
    MOVE_SERVO(SWITCHING_NOZZLE_SERVO_NR, angles[e]);
    safe_delay(500);
  }
#endif

inline void invalid_extruder_error(const uint8_t e) {
  SERIAL_ECHO_START();
  SERIAL_CHAR('T');
  SERIAL_ECHO_F(e, DEC);
  SERIAL_CHAR(' ');
  SERIAL_ECHOLNPGM(MSG_INVALID_EXTRUDER);
}

#if ENABLED(PARKING_EXTRUDER)

  #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
    #define PE_MAGNET_ON_STATE !PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #else
    #define PE_MAGNET_ON_STATE PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #endif

  void pe_set_magnet(const uint8_t extruder_num, const uint8_t state) {
    switch (extruder_num) {
      case 1: OUT_WRITE(SOL1_PIN, state); break;
      default: OUT_WRITE(SOL0_PIN, state); break;
    }
    #if PARKING_EXTRUDER_SOLENOIDS_DELAY > 0
      dwell(PARKING_EXTRUDER_SOLENOIDS_DELAY);
    #endif
  }

  inline void pe_activate_magnet(const uint8_t extruder_num) { pe_set_magnet(extruder_num, PE_MAGNET_ON_STATE); }
  inline void pe_deactivate_magnet(const uint8_t extruder_num) { pe_set_magnet(extruder_num, !PE_MAGNET_ON_STATE); }

#endif // PARKING_EXTRUDER

#if HAS_FANMUX

  void fanmux_switch(const uint8_t e) {
    WRITE(FANMUX0_PIN, TEST(e, 0) ? HIGH : LOW);
    #if PIN_EXISTS(FANMUX1)
      WRITE(FANMUX1_PIN, TEST(e, 1) ? HIGH : LOW);
      #if PIN_EXISTS(FANMUX2)
        WRITE(FANMUX2, TEST(e, 2) ? HIGH : LOW);
      #endif
    #endif
  }

  FORCE_INLINE void fanmux_init(void) {
    SET_OUTPUT(FANMUX0_PIN);
    #if PIN_EXISTS(FANMUX1)
      SET_OUTPUT(FANMUX1_PIN);
      #if PIN_EXISTS(FANMUX2)
        SET_OUTPUT(FANMUX2_PIN);
      #endif
    #endif
    fanmux_switch(0);
  }

#endif // HAS_FANMUX

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, const float fr_mm_s/*=0.0*/, bool no_move/*=false*/) {
  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1

    if (tmp_extruder >= MIXING_VIRTUAL_TOOLS)
      return invalid_extruder_error(tmp_extruder);

    // T0-Tnnn: Switch virtual tool by changing the mix
    for (uint8_t j = 0; j < MIXING_STEPPERS; j++)
      mixing_factor[j] = mixing_virtual_tool_mix[tmp_extruder][j];

  #else // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1

    if (tmp_extruder >= EXTRUDERS)
      return invalid_extruder_error(tmp_extruder);

    #if HOTENDS > 1

      const float old_feedrate_mm_s = fr_mm_s > 0.0 ? fr_mm_s : feedrate_mm_s;

      feedrate_mm_s = fr_mm_s > 0.0 ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;

      if (tmp_extruder != active_extruder) {
        if (!no_move && axis_unhomed_error()) {
          no_move = true;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("No move on toolchange");
          #endif
        }

        // Save current position to destination, for use later
        set_destination_from_current();

        #if ENABLED(DUAL_X_CARRIAGE)

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPGM("Dual X Carriage Mode ");
              switch (dual_x_carriage_mode) {
                case DXC_FULL_CONTROL_MODE: SERIAL_ECHOLNPGM("DXC_FULL_CONTROL_MODE"); break;
                case DXC_AUTO_PARK_MODE: SERIAL_ECHOLNPGM("DXC_AUTO_PARK_MODE"); break;
                case DXC_DUPLICATION_MODE: SERIAL_ECHOLNPGM("DXC_DUPLICATION_MODE"); break;
              }
            }
          #endif

          const float xhome = x_home_pos(active_extruder);
          if (dual_x_carriage_mode == DXC_AUTO_PARK_MODE
              && IsRunning()
              && (delayed_move_time || current_position[X_AXIS] != xhome)
          ) {
            float raised_z = current_position[Z_AXIS] + TOOLCHANGE_PARK_ZLIFT;
            #if ENABLED(MAX_SOFTWARE_ENDSTOPS)
              NOMORE(raised_z, soft_endstop_max[Z_AXIS]);
            #endif
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOLNPAIR("Raise to ", raised_z);
                SERIAL_ECHOLNPAIR("MoveX to ", xhome);
                SERIAL_ECHOLNPAIR("Lower to ", current_position[Z_AXIS]);
              }
            #endif
            // Park old head: 1) raise 2) move to park position 3) lower
            for (uint8_t i = 0; i < 3; i++)
              planner.buffer_line(
                i == 0 ? current_position[X_AXIS] : xhome,
                current_position[Y_AXIS],
                i == 2 ? current_position[Z_AXIS] : raised_z,
                current_position[E_AXIS],
                planner.max_feedrate_mm_s[i == 1 ? X_AXIS : Z_AXIS],
                active_extruder
              );
            stepper.synchronize();
          }

          // Apply Y & Z extruder offset (X offset is used as home pos with Dual X)
          current_position[Y_AXIS] -= hotend_offset[Y_AXIS][active_extruder] - hotend_offset[Y_AXIS][tmp_extruder];
          current_position[Z_AXIS] -= hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];

          // Activate the new extruder ahead of calling set_axis_is_at_home!
          active_extruder = tmp_extruder;

          // This function resets the max/min values - the current position may be overwritten below.
          set_axis_is_at_home(X_AXIS);

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("New Extruder", current_position);
          #endif

          // Only when auto-parking are carriages safe to move
          if (dual_x_carriage_mode != DXC_AUTO_PARK_MODE) no_move = true;

          switch (dual_x_carriage_mode) {
            case DXC_FULL_CONTROL_MODE:
              // New current position is the position of the activated extruder
              current_position[X_AXIS] = inactive_extruder_x_pos;
              // Save the inactive extruder's position (from the old current_position)
              inactive_extruder_x_pos = destination[X_AXIS];
              break;
            case DXC_AUTO_PARK_MODE:
              // record raised toolhead position for use by unpark
              COPY(raised_parked_position, current_position);
              raised_parked_position[Z_AXIS] += TOOLCHANGE_UNPARK_ZLIFT;
              #if ENABLED(MAX_SOFTWARE_ENDSTOPS)
                NOMORE(raised_parked_position[Z_AXIS], soft_endstop_max[Z_AXIS]);
              #endif
              active_extruder_parked = true;
              delayed_move_time = 0;
              break;
            case DXC_DUPLICATION_MODE:
              // If the new extruder is the left one, set it "parked"
              // This triggers the second extruder to move into the duplication position
              active_extruder_parked = (active_extruder == 0);

              if (active_extruder_parked)
                current_position[X_AXIS] = inactive_extruder_x_pos;
              else
                current_position[X_AXIS] = destination[X_AXIS] + duplicate_extruder_x_offset;
              inactive_extruder_x_pos = destination[X_AXIS];
              extruder_duplication_enabled = false;
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                if (DEBUGGING(LEVELING)) {
                  SERIAL_ECHOLNPAIR("Set inactive_extruder_x_pos=", inactive_extruder_x_pos);
                  SERIAL_ECHOLNPGM("Clear extruder_duplication_enabled");
                }
              #endif
              break;
          }

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOLNPAIR("Active extruder parked: ", active_extruder_parked ? "yes" : "no");
              DEBUG_POS("New extruder (parked)", current_position);
            }
          #endif

          // No extra case for HAS_ABL in DUAL_X_CARRIAGE. Does that mean they don't work together?

        #else // !DUAL_X_CARRIAGE

          #if ENABLED(PARKING_EXTRUDER) // Dual Parking extruder
            const float z_diff = hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder];
            float z_raise = PARKING_EXTRUDER_SECURITY_RAISE;
            if (!no_move) {

              const float parkingposx[] = PARKING_EXTRUDER_PARKING_X,
                          midpos = (parkingposx[0] + parkingposx[1]) * 0.5 + hotend_offset[X_AXIS][active_extruder],
                          grabpos = parkingposx[tmp_extruder] + hotend_offset[X_AXIS][active_extruder]
                                    + (tmp_extruder == 0 ? -(PARKING_EXTRUDER_GRAB_DISTANCE) : PARKING_EXTRUDER_GRAB_DISTANCE);
              /**
               *  Steps:
               *    1. Raise Z-Axis to give enough clearance
               *    2. Move to park position of old extruder
               *    3. Disengage magnetic field, wait for delay
               *    4. Move near new extruder
               *    5. Engage magnetic field for new extruder
               *    6. Move to parking incl. offset of new extruder
               *    7. Lower Z-Axis
               */

              // STEP 1
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("Starting Autopark");
                if (DEBUGGING(LEVELING)) DEBUG_POS("current position:", current_position);
              #endif
              current_position[Z_AXIS] += z_raise;
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(1) Raise Z-Axis ");
                if (DEBUGGING(LEVELING)) DEBUG_POS("Moving to Raised Z-Position", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
              stepper.synchronize();

              // STEP 2
              current_position[X_AXIS] = parkingposx[active_extruder] + hotend_offset[X_AXIS][active_extruder];
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPAIR("(2) Park extruder ", active_extruder);
                if (DEBUGGING(LEVELING)) DEBUG_POS("Moving ParkPos", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              stepper.synchronize();

              // STEP 3
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(3) Disengage magnet ");
              #endif
              pe_deactivate_magnet(active_extruder);

              // STEP 4
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(4) Move to position near new extruder");
              #endif
              current_position[X_AXIS] += (active_extruder == 0 ? 10 : -10); // move 10mm away from parked extruder

              #if ENABLED(DEBUG_LEVELING_FEATURE)
                if (DEBUGGING(LEVELING)) DEBUG_POS("Moving away from parked extruder", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              stepper.synchronize();

              // STEP 5
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(5) Engage magnetic field");
              #endif

              #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
                pe_activate_magnet(active_extruder); //just save power for inverted magnets
              #endif
              pe_activate_magnet(tmp_extruder);

              // STEP 6
              current_position[X_AXIS] = grabpos + (tmp_extruder == 0 ? (+10) : (-10));
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              current_position[X_AXIS] = grabpos;
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPAIR("(6) Unpark extruder ", tmp_extruder);
                if (DEBUGGING(LEVELING)) DEBUG_POS("Move UnparkPos", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS]/2, active_extruder);
              stepper.synchronize();

              // Step 7
              current_position[X_AXIS] = midpos - hotend_offset[X_AXIS][tmp_extruder];
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("(7) Move midway between hotends");
                if (DEBUGGING(LEVELING)) DEBUG_POS("Move midway to new extruder", current_position);
              #endif
              planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[X_AXIS], active_extruder);
              stepper.synchronize();
              #if ENABLED(DEBUG_LEVELING_FEATURE)
                SERIAL_ECHOLNPGM("Autopark done.");
              #endif
            }
            else { // nomove == true
              // Only engage magnetic field for new extruder
              pe_activate_magnet(tmp_extruder);
              #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
                pe_activate_magnet(active_extruder); // Just save power for inverted magnets
              #endif
            }
            current_position[Z_AXIS] -= hotend_offset[Z_AXIS][tmp_extruder] - hotend_offset[Z_AXIS][active_extruder]; // Apply Zoffset

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) DEBUG_POS("Applying Z-offset", current_position);
            #endif

          #endif // dualParking extruder

          #if ENABLED(SWITCHING_NOZZLE)
            #define DONT_SWITCH (SWITCHING_EXTRUDER_SERVO_NR == SWITCHING_NOZZLE_SERVO_NR)
            // <0 if the new nozzle is higher, >0 if lower. A bigger raise when lower.
            const float z_diff = hotend_offset[Z_AXIS][active_extruder] - hotend_offset[Z_AXIS][tmp_extruder],
                        z_raise = 0.3 + (z_diff > 0.0 ? z_diff : 0.0);

            // Always raise by some amount (destination copied from current_position earlier)
            current_position[Z_AXIS] += z_raise;
            planner.buffer_line_kinematic(current_position, planner.max_feedrate_mm_s[Z_AXIS], active_extruder);
            move_nozzle_servo(tmp_extruder);
          #endif

          /**
           * Set current_position to the position of the new nozzle.
           * Offsets are based on linear distance, so we need to get
           * the resulting position in coordinate space.
           *
           * - With grid or 3-point leveling, offset XYZ by a tilted vector
           * - With mesh leveling, update Z for the new position
           * - Otherwise, just use the raw linear distance
           *
           * Software endstops are altered here too. Consider a case where:
           *   E0 at X=0 ... E1 at X=10
           * When we switch to E1 now X=10, but E1 can't move left.
           * To express this we apply the change in XY to the software endstops.
           * E1 can move farther right than E0, so the right limit is extended.
           *
           * Note that we don't adjust the Z software endstops. Why not?
           * Consider a case where Z=0 (here) and switching to E1 makes Z=1
           * because the bed is 1mm lower at the new position. As long as
           * the first nozzle is out of the way, the carriage should be
           * allowed to move 1mm lower. This technically "breaks" the
           * Z software endstop. But this is technically correct (and
           * there is no viable alternative).
           */
          #if ABL_PLANAR
            // Offset extruder, make sure to apply the bed level rotation matrix
            vector_3 tmp_offset_vec = vector_3(hotend_offset[X_AXIS][tmp_extruder],
                                               hotend_offset[Y_AXIS][tmp_extruder],
                                               0),
                     act_offset_vec = vector_3(hotend_offset[X_AXIS][active_extruder],
                                               hotend_offset[Y_AXIS][active_extruder],
                                               0),
                     offset_vec = tmp_offset_vec - act_offset_vec;

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                tmp_offset_vec.debug(PSTR("tmp_offset_vec"));
                act_offset_vec.debug(PSTR("act_offset_vec"));
                offset_vec.debug(PSTR("offset_vec (BEFORE)"));
              }
            #endif

            offset_vec.apply_rotation(planner.bed_level_matrix.transpose(planner.bed_level_matrix));

            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) offset_vec.debug(PSTR("offset_vec (AFTER)"));
            #endif

            // Adjustments to the current position
            const float xydiff[2] = { offset_vec.x, offset_vec.y };
            current_position[Z_AXIS] += offset_vec.z;

          #else // !ABL_PLANAR

            const float xydiff[2] = {
              hotend_offset[X_AXIS][tmp_extruder] - hotend_offset[X_AXIS][active_extruder],
              hotend_offset[Y_AXIS][tmp_extruder] - hotend_offset[Y_AXIS][active_extruder]
            };

            #if ENABLED(MESH_BED_LEVELING)

              if (planner.leveling_active) {
                #if ENABLED(DEBUG_LEVELING_FEATURE)
                  if (DEBUGGING(LEVELING)) SERIAL_ECHOPAIR("Z before MBL: ", current_position[Z_AXIS]);
                #endif
                float x2 = current_position[X_AXIS] + xydiff[X_AXIS],
                      y2 = current_position[Y_AXIS] + xydiff[Y_AXIS],
                      z1 = current_position[Z_AXIS], z2 = z1;
                planner.apply_leveling(current_position[X_AXIS], current_position[Y_AXIS], z1);
                planner.apply_leveling(x2, y2, z2);
                current_position[Z_AXIS] += z2 - z1;
                #if ENABLED(DEBUG_LEVELING_FEATURE)
                  if (DEBUGGING(LEVELING))
                    SERIAL_ECHOLNPAIR(" after: ", current_position[Z_AXIS]);
                #endif
              }

            #endif // MESH_BED_LEVELING

          #endif // !HAS_ABL

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPAIR("Offset Tool XY by { ", xydiff[X_AXIS]);
              SERIAL_ECHOPAIR(", ", xydiff[Y_AXIS]);
              SERIAL_ECHOLNPGM(" }");
            }
          #endif

          // The newly-selected extruder XY is actually at...
          current_position[X_AXIS] += xydiff[X_AXIS];
          current_position[Y_AXIS] += xydiff[Y_AXIS];

          // Set the new active extruder
          active_extruder = tmp_extruder;

        #endif // !DUAL_X_CARRIAGE

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("Sync After Toolchange", current_position);
        #endif

        // Tell the planner the new "current position"
        SYNC_PLAN_POSITION_KINEMATIC();

        // Move to the "old position" (move the extruder into place)
        #if ENABLED(SWITCHING_NOZZLE)
          destination[Z_AXIS] += z_diff;  // Include the Z restore with the "move back"
        #endif
        if (!no_move && IsRunning()) {
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) DEBUG_POS("Move back", destination);
          #endif
          // Move back to the original (or tweaked) position
          do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS]);
        }
        #if ENABLED(SWITCHING_NOZZLE)
          else {
            // Move back down. (Including when the new tool is higher.)
            do_blocking_move_to_z(destination[Z_AXIS], planner.max_feedrate_mm_s[Z_AXIS]);
          }
        #endif
      } // (tmp_extruder != active_extruder)

      stepper.synchronize();

      #if ENABLED(EXT_SOLENOID) && !ENABLED(PARKING_EXTRUDER)
        disable_all_solenoids();
        enable_solenoid_on_active_extruder();
      #endif // EXT_SOLENOID

      feedrate_mm_s = old_feedrate_mm_s;

    #else // HOTENDS <= 1

      UNUSED(fr_mm_s);
      UNUSED(no_move);

      #if ENABLED(MK2_MULTIPLEXER)
        if (tmp_extruder >= E_STEPPERS)
          return invalid_extruder_error(tmp_extruder);

        select_multiplexed_stepper(tmp_extruder);
      #endif

      // Set the new active extruder
      active_extruder = tmp_extruder;

    #endif // HOTENDS <= 1

    #if ENABLED(SWITCHING_EXTRUDER) && !DONT_SWITCH
      stepper.synchronize();
      move_extruder_servo(active_extruder);
    #endif

    #if HAS_FANMUX
      fanmux_switch(active_extruder);
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ACTIVE_EXTRUDER, (int)active_extruder);

  #endif // !MIXING_EXTRUDER || MIXING_VIRTUAL_TOOLS <= 1
}

/**
 * T0-T3: Switch tool, usually switching extruders
 *
 *   F[units/min] Set the movement feedrate
 *   S1           Don't move the tool in XY after change
 */
inline void gcode_T(const uint8_t tmp_extruder) {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> gcode_T(", tmp_extruder);
      SERIAL_CHAR(')');
      SERIAL_EOL();
      DEBUG_POS("BEFORE", current_position);
    }
  #endif

  #if HOTENDS == 1 || (ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1)

    tool_change(tmp_extruder);

  #elif HOTENDS > 1

    tool_change(
      tmp_extruder,
      MMM_TO_MMS(parser.linearval('F')),
      (tmp_extruder == active_extruder) || parser.boolval('S')
    );

  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      DEBUG_POS("AFTER", current_position);
      SERIAL_ECHOLNPGM("<<< gcode_T");
    }
  #endif
}

/**
 * Process the parsed command and dispatch it to its handler
 */
void process_parsed_command() {
  KEEPALIVE_STATE(IN_HANDLER);

  // Handle a known G, M, or T
  switch (parser.command_letter) {
    case 'G': switch (parser.codenum) {

      case 0: case 1: gcode_G0_G1(                                // G0: Fast Move, G1: Linear Move
                        #if IS_SCARA
                          parser.codenum == 0
                        #endif
                      ); break;

      #if ENABLED(ARC_SUPPORT) && DISABLED(SCARA)
        case 2: case 3: gcode_G2_G3(parser.codenum == 2); break;  // G2: CW ARC, G3: CCW ARC
      #endif

      case 4: gcode_G4(); break;                                  // G4: Dwell

      #if ENABLED(BEZIER_CURVE_SUPPORT)
        case 5: gcode_G5(); break;                                // G5: Cubic B_spline
      #endif

      #if ENABLED(FWRETRACT)
        case 10: gcode_G10(); break;                              // G10: Retract
        case 11: gcode_G11(); break;                              // G11: Prime
      #endif

      #if ENABLED(NOZZLE_CLEAN_FEATURE)
        case 12: gcode_G12(); break;                              // G12: Clean Nozzle
      #endif

      #if ENABLED(CNC_WORKSPACE_PLANES)
        case 17: gcode_G17(); break;                              // G17: Select Plane XY
        case 18: gcode_G18(); break;                              // G18: Select Plane ZX
        case 19: gcode_G19(); break;                              // G19: Select Plane YZ
      #endif

      #if ENABLED(INCH_MODE_SUPPORT)
        case 20: gcode_G20(); break;                              // G20: Inch Units
        case 21: gcode_G21(); break;                              // G21: Millimeter Units
      #endif

      #if ENABLED(G26_MESH_VALIDATION)
        case 26: gcode_G26(); break;                              // G26: Mesh Validation Pattern
      #endif

      #if ENABLED(NOZZLE_PARK_FEATURE)
        case 27: gcode_G27(); break;                              // G27: Park Nozzle
      #endif

      case 28: gcode_G28(false); break;                           // G28: Home one or more axes

      #if HAS_LEVELING
        case 29: gcode_G29(); break;                              // G29: Detailed Z probe
      #endif

      #if HAS_BED_PROBE
        case 30: gcode_G30(); break;                              // G30: Single Z probe
      #endif

      #if ENABLED(Z_PROBE_SLED)
        case 31: gcode_G31(); break;                              // G31: Dock sled
        case 32: gcode_G32(); break;                              // G32: Undock sled
      #endif

      #if ENABLED(DELTA_AUTO_CALIBRATION)
        case 33: gcode_G33(); break;                              // G33: Delta Auto-Calibration
      #endif

      #if ENABLED(G38_PROBE_TARGET)
        case 38:
          if (parser.subcode == 2 || parser.subcode == 3)
            gcode_G38(parser.subcode == 2);                       // G38.2, G38.3: Probe towards object
          break;
      #endif

      #if HAS_MESH
        case 42: gcode_G42(); break;                              // G42: Move to mesh point
      #endif

      case 90: relative_mode = false; break;                      // G90: Absolute coordinates
      case 91: relative_mode = true; break;                       // G91: Relative coordinates

      case 92: gcode_G92(); break;                                // G92: Set Position

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800: parser.debug(); break;                          // G800: GCode Parser Test for G
      #endif
    }
    break;

    case 'M': switch (parser.codenum) {
      #if HAS_RESUME_CONTINUE
        case 0: case 1: gcode_M0_M1(); break;                     // M0: Unconditional stop, M1: Conditional stop
      #endif

      #if ENABLED(SPINDLE_LASER_ENABLE)
        case 3: gcode_M3_M4(true); break;                         // M3: Laser/CW-Spindle Power
        case 4: gcode_M3_M4(false); break;                        // M4: Laser/CCW-Spindle Power
        case 5: gcode_M5(); break;                                // M5: Laser/Spindle OFF
      #endif

      case 17: gcode_M17(); break;                                // M17: Enable all steppers

      #if ENABLED(SDSUPPORT)
        case 20: gcode_M20(); break;                              // M20: List SD Card
        case 21: gcode_M21(); break;                              // M21: Init SD Card
        case 22: gcode_M22(); break;                              // M22: Release SD Card
        case 23: gcode_M23(); break;                              // M23: Select File
        case 24: gcode_M24(); break;                              // M24: Start SD Print
        case 25: gcode_M25(); break;                              // M25: Pause SD Print
        case 26: gcode_M26(); break;                              // M26: Set SD Index
        case 27: gcode_M27(); break;                              // M27: Get SD Status
        case 28: gcode_M28(); break;                              // M28: Start SD Write
        case 29: gcode_M29(); break;                              // M29: Stop SD Write
        case 30: gcode_M30(); break;                              // M30: Delete File
        case 32: gcode_M32(); break;                              // M32: Select file, Start SD Print
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          case 33: gcode_M33(); break;                            // M33: Report longname path
        #endif
        #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
          case 34: gcode_M34(); break;                            // M34: Set SD card sorting options
        #endif
        case 928: gcode_M928(); break;                            // M928: Start SD write
      #endif // SDSUPPORT

      case 31: gcode_M31(); break;                                // M31: Report print job elapsed time

      case 42: gcode_M42(); break;                                // M42: Change pin state
      #if ENABLED(PINS_DEBUGGING)
        case 43: gcode_M43(); break;                              // M43: Read/monitor pin and endstop states
      #endif

      #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
        case 48: gcode_M48(); break;                              // M48: Z probe repeatability test
      #endif
      #if ENABLED(G26_MESH_VALIDATION)
        case 49: gcode_M49(); break;                              // M49: Toggle the G26 Debug Flag
      #endif

      #if ENABLED(ULTRA_LCD) && ENABLED(LCD_SET_PROGRESS_MANUALLY)
        case 73: gcode_M73(); break;                              // M73: Set Print Progress %
      #endif
      case 75: gcode_M75(); break;                                // M75: Start Print Job Timer
      case 76: gcode_M76(); break;                                // M76: Pause Print Job Timer
      case 77: gcode_M77(); break;                                // M77: Stop Print Job Timer
      #if ENABLED(PRINTCOUNTER)
        case 78: gcode_M78(); break;                              // M78: Report Print Statistics
      #endif

      #if ENABLED(M100_FREE_MEMORY_WATCHER)
        case 100: gcode_M100(); break;                            // M100: Free Memory Report
      #endif

      case 104: gcode_M104(); break;                              // M104: Set Hotend Temperature
      case 110: gcode_M110(); break;                              // M110: Set Current Line Number
      case 111: gcode_M111(); break;                              // M111: Set Debug Flags

      #if DISABLED(EMERGENCY_PARSER)
        case 108: gcode_M108(); break;                            // M108: Cancel Waiting
        case 112: gcode_M112(); break;                            // M112: Emergency Stop
        case 410: gcode_M410(); break;                            // M410: Quickstop. Abort all planned moves
      #endif

      #if ENABLED(HOST_KEEPALIVE_FEATURE)
        case 113: gcode_M113(); break;                            // M113: Set Host Keepalive Interval
      #endif

      case 140: gcode_M140(); break;                              // M140: Set Bed Temperature

      case 105: gcode_M105(); KEEPALIVE_STATE(NOT_BUSY); return;  // M105: Report Temperatures (and say "ok")

      #if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)
        case 155: gcode_M155(); break;                            // M155: Set Temperature Auto-report Interval
      #endif

      case 109: gcode_M109(); break;                              // M109: Set Hotend Temperature. Wait for target.

      #if HAS_TEMP_BED
        case 190: gcode_M190(); break;                            // M190: Set Bed Temperature. Wait for target.
      #endif

      #if FAN_COUNT > 0
        case 106: gcode_M106(); break;                            // M106: Set Fan Speed
        case 107: gcode_M107(); break;                            // M107: Fan Off
      #endif

      #if ENABLED(PARK_HEAD_ON_PAUSE)
        case 125: gcode_M125(); break;                            // M125: Park (for Filament Change)
      #endif

      #if ENABLED(BARICUDA)
        #if HAS_HEATER_1
          case 126: gcode_M126(); break;                          // M126: Valve 1 Open
          case 127: gcode_M127(); break;                          // M127: Valve 1 Closed
        #endif
        #if HAS_HEATER_2
          case 128: gcode_M128(); break;                          // M128: Valve 2 Open
          case 129: gcode_M129(); break;                          // M129: Valve 2 Closed
        #endif
      #endif

      #if HAS_POWER_SWITCH
        case 80: gcode_M80(); break;                              // M80: Turn on Power Supply
      #endif
      case 81: gcode_M81(); break;                                // M81: Turn off Power and Power Supply

      case 82: gcode_M82(); break;                                // M82: Disable Relative E-Axis
      case 83: gcode_M83(); break;                                // M83: Set Relative E-Axis
      case 18: case 84: gcode_M18_M84(); break;                   // M18/M84: Disable Steppers / Set Timeout
      case 85: gcode_M85(); break;                                // M85: Set inactivity stepper shutdown timeout
      case 92: gcode_M92(); break;                                // M92: Set steps-per-unit
      case 114: gcode_M114(); break;                              // M114: Report Current Position
      case 115: gcode_M115(); break;                              // M115: Capabilities Report
      case 117: gcode_M117(); break;                              // M117: Set LCD message text
      case 118: gcode_M118(); break;                              // M118: Print a message in the host console
      case 119: gcode_M119(); break;                              // M119: Report Endstop states
      case 120: gcode_M120(); break;                              // M120: Enable Endstops
      case 121: gcode_M121(); break;                              // M121: Disable Endstops

      #if ENABLED(ULTIPANEL)
        case 145: gcode_M145(); break;                            // M145: Set material heatup parameters
      #endif

      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        case 149: gcode_M149(); break;                            // M149: Set Temperature Units, C F K
      #endif

      #if HAS_COLOR_LEDS
        case 150: gcode_M150(); break;                            // M150: Set Status LED Color
      #endif

      #if ENABLED(MIXING_EXTRUDER)
        case 163: gcode_M163(); break;                            // M163: Set Mixing Component
        #if MIXING_VIRTUAL_TOOLS > 1
          case 164: gcode_M164(); break;                          // M164: Save Current Mix
        #endif
        #if ENABLED(DIRECT_MIXING_IN_G1)
          case 165: gcode_M165(); break;                          // M165: Set Multiple Mixing Components
        #endif
      #endif

      #if DISABLED(NO_VOLUMETRICS)
        case 200: gcode_M200(); break;                            // M200: Set Filament Diameter, Volumetric Extrusion
      #endif

      case 201: gcode_M201(); break;                              // M201: Set Max Printing Acceleration (units/sec^2)
      #if 0
        case 202: gcode_M202(); break;                            // M202: Not used for Sprinter/grbl gen6
      #endif
      case 203: gcode_M203(); break;                              // M203: Set Max Feedrate (units/sec)
      case 204: gcode_M204(); break;                              // M204: Set Acceleration
      case 205: gcode_M205(); break;                              // M205: Set Advanced settings

      #if HAS_M206_COMMAND
        case 206: gcode_M206(); break;                            // M206: Set Home Offsets
        case 428: gcode_M428(); break;                            // M428: Set Home Offsets based on current position
      #endif

      #if ENABLED(FWRETRACT)
        case 207: gcode_M207(); break;                            // M207: Set Retract Length, Feedrate, Z lift
        case 208: gcode_M208(); break;                            // M208: Set Additional Prime Length and Feedrate
        case 209:
          if (MIN_AUTORETRACT <= MAX_AUTORETRACT) gcode_M209();   // M209: Turn Auto-Retract on/off
          break;
      #endif

      case 211: gcode_M211(); break;                              // M211: Enable/Disable/Report Software Endstops

      #if HOTENDS > 1
        case 218: gcode_M218(); break;                            // M218: Set Tool Offset
      #endif

      case 220: gcode_M220(); break;                              // M220: Set Feedrate Percentage
      case 221: gcode_M221(); break;                              // M221: Set Flow Percentage
      case 226: gcode_M226(); break;                              // M226: Wait for Pin State

      #if defined(CHDK) || HAS_PHOTOGRAPH
        case 240: gcode_M240(); break;                            // M240: Trigger Camera
      #endif

      #if HAS_LCD_CONTRAST
        case 250: gcode_M250(); break;                            // M250: Set LCD Contrast
      #endif

      #if ENABLED(EXPERIMENTAL_I2CBUS)
        case 260: gcode_M260(); break;                            // M260: Send Data to i2c slave
        case 261: gcode_M261(); break;                            // M261: Request Data from i2c slave
      #endif

      #if HAS_SERVOS
        case 280: gcode_M280(); break;                            // M280: Set Servo Position
      #endif

      #if ENABLED(BABYSTEPPING)
        case 290: gcode_M290(); break;                            // M290: Babystepping
      #endif

      #if HAS_BUZZER
        case 300: gcode_M300(); break;                            // M300: Add Tone/Buzz to Queue
      #endif

      #if ENABLED(PIDTEMP)
        case 301: gcode_M301(); break;                            // M301: Set Hotend PID parameters
      #endif

      #if ENABLED(PREVENT_COLD_EXTRUSION)
        case 302: gcode_M302(); break;                            // M302: Set Minimum Extrusion Temp
      #endif

      case 303: gcode_M303(); break;                              // M303: PID Autotune

      #if ENABLED(PIDTEMPBED)
        case 304: gcode_M304(); break;                            // M304: Set Bed PID parameters
      #endif

      #if HAS_MICROSTEPS
        case 350: gcode_M350(); break;                            // M350: Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
        case 351: gcode_M351(); break;                            // M351: Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
      #endif

      case 355: gcode_M355(); break;                              // M355: Set Case Light brightness

      #if ENABLED(MORGAN_SCARA)
        case 360: if (gcode_M360()) return; break;                // M360: SCARA Theta pos1
        case 361: if (gcode_M361()) return; break;                // M361: SCARA Theta pos2
        case 362: if (gcode_M362()) return; break;                // M362: SCARA Psi pos1
        case 363: if (gcode_M363()) return; break;                // M363: SCARA Psi pos2
        case 364: if (gcode_M364()) return; break;                // M364: SCARA Psi pos3 (90 deg to Theta)
      #endif

      case 400: gcode_M400(); break;                              // M400: Synchronize. Wait for moves to finish.

      #if HAS_BED_PROBE
        case 401: gcode_M401(); break;                            // M401: Deploy Probe
        case 402: gcode_M402(); break;                            // M402: Stow Probe
      #endif

      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        case 404: gcode_M404(); break;                            // M404: Set/Report Nominal Filament Width
        case 405: gcode_M405(); break;                            // M405: Enable Filament Width Sensor
        case 406: gcode_M406(); break;                            // M406: Disable Filament Width Sensor
        case 407: gcode_M407(); break;                            // M407: Report Measured Filament Width
      #endif

      #if HAS_LEVELING
        case 420: gcode_M420(); break;                            // M420: Set Bed Leveling Enabled / Fade
      #endif

      #if HAS_MESH
        case 421: gcode_M421(); break;                            // M421: Set a Mesh Z value
      #endif

      case 500: gcode_M500(); break;                              // M500: Store Settings in EEPROM
      case 501: gcode_M501(); break;                              // M501: Read Settings from EEPROM
      case 502: gcode_M502(); break;                              // M502: Revert Settings to defaults
      #if DISABLED(DISABLE_M503)
        case 503: gcode_M503(); break;                            // M503: Report Settings (in SRAM)
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        case 504: gcode_M504(); break;                            // M504: Validate EEPROM
      #endif

      #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
        case 540: gcode_M540(); break;                            // M540: Set Abort on Endstop Hit for SD Printing
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case 600: gcode_M600(); break;                            // M600: Pause for Filament Change
        case 603: gcode_M603(); break;                            // M603: Configure Filament Change
      #endif

      #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
        case 605: gcode_M605(); break;                            // M605: Set Dual X Carriage movement mode
      #endif

      #if ENABLED(DELTA)
        case 665: gcode_M665(); break;                            // M665: Delta Configuration
      #endif
      #if ENABLED(DELTA) || ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
        case 666: gcode_M666(); break;                            // M666: DELTA/Dual Endstop Adjustment
      #endif

      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        case 701: gcode_M701(); break;                            // M701: Load Filament
        case 702: gcode_M702(); break;                            // M702: Unload Filament
      #endif

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800: parser.debug(); break;                          // M800: GCode Parser Test for M
      #endif

      #if HAS_BED_PROBE
        case 851: gcode_M851(); break;                            // M851: Set Z Probe Z Offset
      #endif

      #if ENABLED(SKEW_CORRECTION_GCODE)
        case 852: gcode_M852(); break;                            // M852: Set Skew factors
      #endif

      #if ENABLED(I2C_POSITION_ENCODERS)
        case 860: gcode_M860(); break;                            // M860: Report encoder module position
        case 861: gcode_M861(); break;                            // M861: Report encoder module status
        case 862: gcode_M862(); break;                            // M862: Perform axis test
        case 863: gcode_M863(); break;                            // M863: Calibrate steps/mm
        case 864: gcode_M864(); break;                            // M864: Change module address
        case 865: gcode_M865(); break;                            // M865: Check module firmware version
        case 866: gcode_M866(); break;                            // M866: Report axis error count
        case 867: gcode_M867(); break;                            // M867: Toggle error correction
        case 868: gcode_M868(); break;                            // M868: Set error correction threshold
        case 869: gcode_M869(); break;                            // M869: Report axis error
      #endif

      #if ENABLED(LIN_ADVANCE)
        case 900: gcode_M900(); break;                            // M900: Set Linear Advance K factor
      #endif

      case 907: gcode_M907(); break;                              // M907: Set Digital Trimpot Motor Current using axis codes.

      #if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)
        case 908: gcode_M908(); break;                            // M908: Direct Control Digital Trimpot
        #if ENABLED(DAC_STEPPER_CURRENT)
          case 909: gcode_M909(); break;                          // M909: Print Digipot/DAC current value (As with Printrbot RevF)
          case 910: gcode_M910(); break;                          // M910: Commit Digipot/DAC value to External EEPROM (As with Printrbot RevF)
        #endif
      #endif

      #if ENABLED(HAVE_TMC2130) || ENABLED(HAVE_TMC2208)
        #if ENABLED(TMC_DEBUG)
          case 122: gcode_M122(); break;                          // M122: Debug TMC steppers
        #endif
        case 906: gcode_M906(); break;                            // M906: Set motor current in milliamps using axis codes X, Y, Z, E
        case 911: gcode_M911(); break;                            // M911: Report TMC prewarn triggered flags
        case 912: gcode_M912(); break;                            // M911: Clear TMC prewarn triggered flags
        #if ENABLED(HYBRID_THRESHOLD)
          case 913: gcode_M913(); break;                          // M913: Set HYBRID_THRESHOLD speed.
        #endif
        #if ENABLED(SENSORLESS_HOMING)
          case 914: gcode_M914(); break;                          // M914: Set SENSORLESS_HOMING sensitivity.
        #endif
        #if ENABLED(TMC_Z_CALIBRATION)
          case 915: gcode_M915(); break;                          // M915: TMC Z axis calibration routine
        #endif
      #endif

      case 999: gcode_M999(); break;                              // M999: Restart after being Stopped
    }
    break;

    case 'T': gcode_T(parser.codenum); break;                     // T: Tool Select

    default: parser.unknown_command_error();
  }

  KEEPALIVE_STATE(NOT_BUSY);
  ok_to_send();
}

void process_next_command() {
  char * const current_command = command_queue[cmd_queue_index_r];

  if (DEBUGGING(ECHO)) {
    SERIAL_ECHO_START();
    SERIAL_ECHOLN(current_command);
    #if ENABLED(M100_FREE_MEMORY_WATCHER)
      SERIAL_ECHOPAIR("slot:", cmd_queue_index_r);
      M100_dump_routine("   Command Queue:", (const char*)command_queue, (const char*)(command_queue + sizeof(command_queue)));
    #endif
  }

  // Parse the next command in the queue
  parser.parse(current_command);
  process_parsed_command();
}

/**
 * Send a "Resend: nnn" message to the host to
 * indicate that a command needs to be re-sent.
 */
void FlushSerialRequestResend() {
  //char command_queue[cmd_queue_index_r][100]="Resend:";
  SERIAL_FLUSH();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ok_to_send();
}

/**
 * Send an "ok" message to the host, indicating
 * that a command was successfully processed.
 *
 * If ADVANCED_OK is enabled also include:
 *   N<int>  Line number of the command, if any
 *   P<int>  Planner space remaining
 *   B<int>  Block queue space remaining
 */
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
    SERIAL_PROTOCOLPGM(" P"); SERIAL_PROTOCOL(int(BLOCK_BUFFER_SIZE - planner.movesplanned() - 1));
    SERIAL_PROTOCOLPGM(" B"); SERIAL_PROTOCOL(BUFSIZE - commands_in_queue);
  #endif
  SERIAL_EOL();
}

#if HAS_SOFTWARE_ENDSTOPS

  /**
   * Constrain the given coordinates to the software endstops.
   *
   * For DELTA/SCARA the XY constraint is based on the smallest
   * radius within the set software endstops.
   */
  void clamp_to_software_endstops(float target[XYZ]) {
    if (!soft_endstops_enabled) return;
    #if IS_KINEMATIC
      const float dist_2 = HYPOT2(target[X_AXIS], target[Y_AXIS]);
      if (dist_2 > soft_endstop_radius_2) {
        const float ratio = soft_endstop_radius / SQRT(dist_2); // 200 / 300 = 0.66
        target[X_AXIS] *= ratio;
        target[Y_AXIS] *= ratio;
      }
    #else
      #if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
        NOLESS(target[X_AXIS], soft_endstop_min[X_AXIS]);
      #endif
      #if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
        NOLESS(target[Y_AXIS], soft_endstop_min[Y_AXIS]);
      #endif
      #if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
        NOMORE(target[X_AXIS], soft_endstop_max[X_AXIS]);
      #endif
      #if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
        NOMORE(target[Y_AXIS], soft_endstop_max[Y_AXIS]);
      #endif
    #endif
    #if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
      NOLESS(target[Z_AXIS], soft_endstop_min[Z_AXIS]);
    #endif
    #if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
      NOMORE(target[Z_AXIS], soft_endstop_max[Z_AXIS]);
    #endif
  }

#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

  // Get the Z adjustment for non-linear bed leveling
  float bilinear_z_offset(const float raw[XYZ]) {

    static float z1, d2, z3, d4, L, D, ratio_x, ratio_y,
                 last_x = -999.999, last_y = -999.999;

    // Whole units for the grid line indices. Constrained within bounds.
    static int8_t gridx, gridy, nextx, nexty,
                  last_gridx = -99, last_gridy = -99;

    // XY relative to the probed area
    const float rx = raw[X_AXIS] - bilinear_start[X_AXIS],
                ry = raw[Y_AXIS] - bilinear_start[Y_AXIS];

    #if ENABLED(EXTRAPOLATE_BEYOND_GRID)
      // Keep using the last grid box
      #define FAR_EDGE_OR_BOX 2
    #else
      // Just use the grid far edge
      #define FAR_EDGE_OR_BOX 1
    #endif

    if (last_x != rx) {
      last_x = rx;
      ratio_x = rx * ABL_BG_FACTOR(X_AXIS);
      const float gx = constrain(FLOOR(ratio_x), 0, ABL_BG_POINTS_X - FAR_EDGE_OR_BOX);
      ratio_x -= gx;      // Subtract whole to get the ratio within the grid box

      #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
        // Beyond the grid maintain height at grid edges
        NOLESS(ratio_x, 0); // Never < 0.0. (> 1.0 is ok when nextx==gridx.)
      #endif

      gridx = gx;
      nextx = min(gridx + 1, ABL_BG_POINTS_X - 1);
    }

    if (last_y != ry || last_gridx != gridx) {

      if (last_y != ry) {
        last_y = ry;
        ratio_y = ry * ABL_BG_FACTOR(Y_AXIS);
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

      // Bilinear interpolate. Needed since ry or gridx has changed.
                  L = z1 + d2 * ratio_y;   // Linear interp. LF -> LB
      const float R = z3 + d4 * ratio_y;   // Linear interp. RF -> RB

      D = R - L;
    }

    const float offset = L + ratio_x * D;   // the offset almost always changes

    /*
    static float last_offset = 0;
    if (FABS(last_offset - offset) > 0.2) {
      SERIAL_ECHOPGM("Sudden Shift at ");
      SERIAL_ECHOPAIR("x=", rx);
      SERIAL_ECHOPAIR(" / ", bilinear_grid_spacing[X_AXIS]);
      SERIAL_ECHOLNPAIR(" -> gridx=", gridx);
      SERIAL_ECHOPAIR(" y=", ry);
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
  void recalc_delta_settings() {
    const float trt[ABC] = DELTA_RADIUS_TRIM_TOWER,
                drt[ABC] = DELTA_DIAGONAL_ROD_TRIM_TOWER;
    delta_tower[A_AXIS][X_AXIS] = cos(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (delta_radius + trt[A_AXIS]); // front left tower
    delta_tower[A_AXIS][Y_AXIS] = sin(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (delta_radius + trt[A_AXIS]);
    delta_tower[B_AXIS][X_AXIS] = cos(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (delta_radius + trt[B_AXIS]); // front right tower
    delta_tower[B_AXIS][Y_AXIS] = sin(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (delta_radius + trt[B_AXIS]);
    delta_tower[C_AXIS][X_AXIS] = cos(RADIANS( 90 + delta_tower_angle_trim[C_AXIS])) * (delta_radius + trt[C_AXIS]); // back middle tower
    delta_tower[C_AXIS][Y_AXIS] = sin(RADIANS( 90 + delta_tower_angle_trim[C_AXIS])) * (delta_radius + trt[C_AXIS]);
    delta_diagonal_rod_2_tower[A_AXIS] = sq(delta_diagonal_rod + drt[A_AXIS]);
    delta_diagonal_rod_2_tower[B_AXIS] = sq(delta_diagonal_rod + drt[B_AXIS]);
    delta_diagonal_rod_2_tower[C_AXIS] = sq(delta_diagonal_rod + drt[C_AXIS]);
    update_software_endstops(Z_AXIS);
    axis_homed[X_AXIS] = axis_homed[Y_AXIS] = axis_homed[Z_AXIS] = false;
  }

  #if ENABLED(DELTA_FAST_SQRT)
    /**
     * Fast inverse sqrt from Quake III Arena
     * See: https://en.wikipedia.org/wiki/Fast_inverse_square_root
     */
    float Q_rsqrt(const float number) {
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

  #endif

  /**
   * Delta Inverse Kinematics
   *
   * Calculate the tower positions for a given machine
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

  #define DELTA_DEBUG() do { \
      SERIAL_ECHOPAIR("cartesian X:", raw[X_AXIS]); \
      SERIAL_ECHOPAIR(" Y:", raw[Y_AXIS]);          \
      SERIAL_ECHOLNPAIR(" Z:", raw[Z_AXIS]);        \
      SERIAL_ECHOPAIR("delta A:", delta[A_AXIS]);   \
      SERIAL_ECHOPAIR(" B:", delta[B_AXIS]);        \
      SERIAL_ECHOLNPAIR(" C:", delta[C_AXIS]);      \
    }while(0)

  void inverse_kinematics(const float raw[XYZ]) {
    DELTA_IK(raw);
    // DELTA_DEBUG();
  }

  /**
   * Calculate the highest Z position where the
   * effector has the full range of XY motion.
   */
  float delta_safe_distance_from_top() {
    float cartesian[XYZ] = { 0, 0, 0 };
    inverse_kinematics(cartesian);
    float distance = delta[A_AXIS];
    cartesian[Y_AXIS] = DELTA_PRINTABLE_RADIUS;
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
    const float p12[] = {
      delta_tower[B_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS],
      delta_tower[B_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS],
      z2 - z1
    },

    // Get the Magnitude of vector.
    d = SQRT(sq(p12[0]) + sq(p12[1]) + sq(p12[2])),

    // Create unit vector by dividing by magnitude.
    ex[3] = { p12[0] / d, p12[1] / d, p12[2] / d },

    // Get the vector from the origin of the new system to the third point.
    p13[3] = {
      delta_tower[C_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS],
      delta_tower[C_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS],
      z3 - z1
    },

    // Use the dot product to find the component of this vector on the X axis.
    i = ex[0] * p13[0] + ex[1] * p13[1] + ex[2] * p13[2],

    // Create a vector along the x axis that represents the x component of p13.
    iex[] = { ex[0] * i, ex[1] * i, ex[2] * i };

    // Subtract the X component from the original vector leaving only Y. We use the
    // variable that will be the unit vector after we scale it.
    float ey[3] = { p13[0] - iex[0], p13[1] - iex[1], p13[2] - iex[2] };

    // The magnitude of Y component
    const float j = SQRT(sq(ey[0]) + sq(ey[1]) + sq(ey[2]));

    // Convert to a unit vector
    ey[0] /= j; ey[1] /= j;  ey[2] /= j;

    // The cross product of the unit x and y is the unit z
    // float[] ez = vectorCrossProd(ex, ey);
    const float ez[3] = {
      ex[1] * ey[2] - ex[2] * ey[1],
      ex[2] * ey[0] - ex[0] * ey[2],
      ex[0] * ey[1] - ex[1] * ey[0]
    },
    // We now have the d, i and j values defined in Wikipedia.
    // Plug them into the equations defined in Wikipedia for Xnew, Ynew and Znew
    Xnew = (delta_diagonal_rod_2_tower[A_AXIS] - delta_diagonal_rod_2_tower[B_AXIS] + sq(d)) / (d * 2),
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
 * unapply_leveling to obtain machine coordinates suitable
 * for current_position, etc.
 */
void get_cartesian_from_steppers() {
  #if ENABLED(DELTA)
    forward_kinematics_DELTA(
      stepper.get_axis_position_mm(A_AXIS),
      stepper.get_axis_position_mm(B_AXIS),
      stepper.get_axis_position_mm(C_AXIS)
    );
  #else
    #if IS_SCARA
      forward_kinematics_SCARA(
        stepper.get_axis_position_degrees(A_AXIS),
        stepper.get_axis_position_degrees(B_AXIS)
      );
    #else
      cartes[X_AXIS] = stepper.get_axis_position_mm(X_AXIS);
      cartes[Y_AXIS] = stepper.get_axis_position_mm(Y_AXIS);
    #endif
    cartes[Z_AXIS] = stepper.get_axis_position_mm(Z_AXIS);
  #endif
}

/**
 * Set the current_position for an axis based on
 * the stepper positions, removing any leveling that
 * may have been applied.
 *
 * To prevent small shifts in axis position always call
 * SYNC_PLAN_POSITION_KINEMATIC after updating axes with this.
 *
 * To keep hosts in sync, always call report_current_position
 * after updating the current_position.
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

#if IS_CARTESIAN
#if ENABLED(SEGMENT_LEVELED_MOVES)

  /**
   * Prepare a segmented move on a CARTESIAN setup.
   *
   * This calls planner.buffer_line several times, adding
   * small incremental moves. This allows the planner to
   * apply more detailed bed leveling to the full move.
   */
  inline void segmented_line_to_destination(const float &fr_mm_s, const float segment_size=LEVELED_SEGMENT_LENGTH) {

    const float xdiff = destination[X_AXIS] - current_position[X_AXIS],
                ydiff = destination[Y_AXIS] - current_position[Y_AXIS];

    // If the move is only in Z/E don't split up the move
    if (!xdiff && !ydiff) {
      planner.buffer_line_kinematic(destination, fr_mm_s, active_extruder);
      return;
    }

    // Remaining cartesian distances
    const float zdiff = destination[Z_AXIS] - current_position[Z_AXIS],
                ediff = destination[E_AXIS] - current_position[E_AXIS];

    // Get the linear distance in XYZ
    // If the move is very short, check the E move distance
    // No E move either? Game over.
    float cartesian_mm = SQRT(sq(xdiff) + sq(ydiff) + sq(zdiff));
    if (UNEAR_ZERO(cartesian_mm)) cartesian_mm = FABS(ediff);
    if (UNEAR_ZERO(cartesian_mm)) return;

    // The length divided by the segment size
    // At least one segment is required
    uint16_t segments = cartesian_mm / segment_size;
    NOLESS(segments, 1);

    // The approximate length of each segment
    const float inv_segments = 1.0 / float(segments),
                cartesian_segment_mm = cartesian_mm * inv_segments,
                segment_distance[XYZE] = {
                  xdiff * inv_segments,
                  ydiff * inv_segments,
                  zdiff * inv_segments,
                  ediff * inv_segments
                };

    // SERIAL_ECHOPAIR("mm=", cartesian_mm);
    // SERIAL_ECHOLNPAIR(" segments=", segments);
    // SERIAL_ECHOLNPAIR(" segment_mm=", cartesian_segment_mm);

    // Get the raw current position as starting point
    float raw[XYZE];
    COPY(raw, current_position);

    // Calculate and execute the segments
    while (--segments) {
      static millis_t next_idle_ms = millis() + 200UL;
      thermalManager.manage_heater();  // This returns immediately if not really needed.
      if (ELAPSED(millis(), next_idle_ms)) {
        next_idle_ms = millis() + 200UL;
        idle();
      }
      LOOP_XYZE(i) raw[i] += segment_distance[i];
      planner.buffer_line_kinematic(raw, fr_mm_s, active_extruder, cartesian_segment_mm);
    }

    // Since segment_distance is only approximate,
    // the final move must be to the exact destination.
    planner.buffer_line_kinematic(destination, fr_mm_s, active_extruder, cartesian_segment_mm);
  }

#elif ENABLED(MESH_BED_LEVELING)

  /**
   * Prepare a mesh-leveled linear move in a Cartesian setup,
   * splitting the move where it crosses mesh borders.
   */
  void mesh_line_to_destination(const float fr_mm_s, uint8_t x_splits=0xFF, uint8_t y_splits=0xFF) {
    // Get current and destination cells for this line
    int cx1 = mbl.cell_index_x(current_position[X_AXIS]),
        cy1 = mbl.cell_index_y(current_position[Y_AXIS]),
        cx2 = mbl.cell_index_x(destination[X_AXIS]),
        cy2 = mbl.cell_index_y(destination[Y_AXIS]);
    NOMORE(cx1, GRID_MAX_POINTS_X - 2);
    NOMORE(cy1, GRID_MAX_POINTS_Y - 2);
    NOMORE(cx2, GRID_MAX_POINTS_X - 2);
    NOMORE(cy2, GRID_MAX_POINTS_Y - 2);

    // Start and end in the same cell? No split needed.
    if (cx1 == cx2 && cy1 == cy2) {
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    #define MBL_SEGMENT_END(A) (current_position[A ##_AXIS] + (destination[A ##_AXIS] - current_position[A ##_AXIS]) * normalized_dist)

    float normalized_dist, end[XYZE];
    const int8_t gcx = max(cx1, cx2), gcy = max(cy1, cy2);

    // Crosses on the X and not already split on this X?
    // The x_splits flags are insurance against rounding errors.
    if (cx2 != cx1 && TEST(x_splits, gcx)) {
      // Split on the X grid line
      CBI(x_splits, gcx);
      COPY(end, destination);
      destination[X_AXIS] = mbl.index_to_xpos[gcx];
      normalized_dist = (destination[X_AXIS] - current_position[X_AXIS]) / (end[X_AXIS] - current_position[X_AXIS]);
      destination[Y_AXIS] = MBL_SEGMENT_END(Y);
    }
    // Crosses on the Y and not already split on this Y?
    else if (cy2 != cy1 && TEST(y_splits, gcy)) {
      // Split on the Y grid line
      CBI(y_splits, gcy);
      COPY(end, destination);
      destination[Y_AXIS] = mbl.index_to_ypos[gcy];
      normalized_dist = (destination[Y_AXIS] - current_position[Y_AXIS]) / (end[Y_AXIS] - current_position[Y_AXIS]);
      destination[X_AXIS] = MBL_SEGMENT_END(X);
    }
    else {
      // Must already have been split on these border(s)
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    destination[Z_AXIS] = MBL_SEGMENT_END(Z);
    destination[E_AXIS] = MBL_SEGMENT_END(E);

    // Do the split and look for more borders
    mesh_line_to_destination(fr_mm_s, x_splits, y_splits);

    // Restore destination from stack
    COPY(destination, end);
    mesh_line_to_destination(fr_mm_s, x_splits, y_splits);
  }

#elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

  #define CELL_INDEX(A,V) ((V - bilinear_start[A##_AXIS]) * ABL_BG_FACTOR(A##_AXIS))

  /**
   * Prepare a bilinear-leveled linear move on Cartesian,
   * splitting the move where it crosses grid borders.
   */
  void bilinear_line_to_destination(const float fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF) {
    // Get current and destination cells for this line
    int cx1 = CELL_INDEX(X, current_position[X_AXIS]),
        cy1 = CELL_INDEX(Y, current_position[Y_AXIS]),
        cx2 = CELL_INDEX(X, destination[X_AXIS]),
        cy2 = CELL_INDEX(Y, destination[Y_AXIS]);
    cx1 = constrain(cx1, 0, ABL_BG_POINTS_X - 2);
    cy1 = constrain(cy1, 0, ABL_BG_POINTS_Y - 2);
    cx2 = constrain(cx2, 0, ABL_BG_POINTS_X - 2);
    cy2 = constrain(cy2, 0, ABL_BG_POINTS_Y - 2);

    // Start and end in the same cell? No split needed.
    if (cx1 == cx2 && cy1 == cy2) {
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    #define LINE_SEGMENT_END(A) (current_position[A ##_AXIS] + (destination[A ##_AXIS] - current_position[A ##_AXIS]) * normalized_dist)

    float normalized_dist, end[XYZE];
    const int8_t gcx = max(cx1, cx2), gcy = max(cy1, cy2);

    // Crosses on the X and not already split on this X?
    // The x_splits flags are insurance against rounding errors.
    if (cx2 != cx1 && TEST(x_splits, gcx)) {
      // Split on the X grid line
      CBI(x_splits, gcx);
      COPY(end, destination);
      destination[X_AXIS] = bilinear_start[X_AXIS] + ABL_BG_SPACING(X_AXIS) * gcx;
      normalized_dist = (destination[X_AXIS] - current_position[X_AXIS]) / (end[X_AXIS] - current_position[X_AXIS]);
      destination[Y_AXIS] = LINE_SEGMENT_END(Y);
    }
    // Crosses on the Y and not already split on this Y?
    else if (cy2 != cy1 && TEST(y_splits, gcy)) {
      // Split on the Y grid line
      CBI(y_splits, gcy);
      COPY(end, destination);
      destination[Y_AXIS] = bilinear_start[Y_AXIS] + ABL_BG_SPACING(Y_AXIS) * gcy;
      normalized_dist = (destination[Y_AXIS] - current_position[Y_AXIS]) / (end[Y_AXIS] - current_position[Y_AXIS]);
      destination[X_AXIS] = LINE_SEGMENT_END(X);
    }
    else {
      // Must already have been split on these border(s)
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    destination[Z_AXIS] = LINE_SEGMENT_END(Z);
    destination[E_AXIS] = LINE_SEGMENT_END(E);

    // Do the split and look for more borders
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);

    // Restore destination from stack
    COPY(destination, end);
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);
  }

#endif // AUTO_BED_LEVELING_BILINEAR
#endif // IS_CARTESIAN

#if !UBL_SEGMENTED
#if IS_KINEMATIC

  /**
   * Prepare a linear move in a DELTA or SCARA setup.
   *
   * This calls planner.buffer_line several times, adding
   * small incremental moves for DELTA or SCARA.
   *
   * For Unified Bed Leveling (Delta or Segmented Cartesian)
   * the ubl.prepare_segmented_line_to method replaces this.
   */
  inline bool prepare_kinematic_move_to(const float (&rtarget)[XYZE]) {

    // Get the top feedrate of the move in the XY plane
    const float _feedrate_mm_s = MMS_SCALED(feedrate_mm_s);

    const float xdiff = rtarget[X_AXIS] - current_position[X_AXIS],
                ydiff = rtarget[Y_AXIS] - current_position[Y_AXIS];

    // If the move is only in Z/E don't split up the move
    if (!xdiff && !ydiff) {
      planner.buffer_line_kinematic(rtarget, _feedrate_mm_s, active_extruder);
      return false; // caller will update current_position
    }

    // Fail if attempting move outside printable radius
    if (!position_is_reachable(rtarget[X_AXIS], rtarget[Y_AXIS])) return true;

    // Remaining cartesian distances
    const float zdiff = rtarget[Z_AXIS] - current_position[Z_AXIS],
                ediff = rtarget[E_AXIS] - current_position[E_AXIS];

    // Get the linear distance in XYZ
    // If the move is very short, check the E move distance
    // No E move either? Game over.
    float cartesian_mm = SQRT(sq(xdiff) + sq(ydiff) + sq(zdiff));
    if (UNEAR_ZERO(cartesian_mm)) cartesian_mm = FABS(ediff);
    if (UNEAR_ZERO(cartesian_mm)) return true;

    // Minimum number of seconds to move the given distance
    const float seconds = cartesian_mm / _feedrate_mm_s;

    // The number of segments-per-second times the duration
    // gives the number of segments
    uint16_t segments = delta_segments_per_second * seconds;

    // For SCARA minimum segment size is 0.25mm
    #if IS_SCARA
      NOMORE(segments, cartesian_mm * 4);
    #endif

    // At least one segment is required
    NOLESS(segments, 1);

    // The approximate length of each segment
    const float inv_segments = 1.0 / float(segments),
                cartesian_segment_mm = cartesian_mm * inv_segments,
                segment_distance[XYZE] = {
                  xdiff * inv_segments,
                  ydiff * inv_segments,
                  zdiff * inv_segments,
                  ediff * inv_segments
                };

    // SERIAL_ECHOPAIR("mm=", cartesian_mm);
    // SERIAL_ECHOPAIR(" seconds=", seconds);
    // SERIAL_ECHOLNPAIR(" segments=", segments);
    // SERIAL_ECHOLNPAIR(" segment_mm=", cartesian_segment_mm);

    // Get the current position as starting point
    float raw[XYZE];
    COPY(raw, current_position);


    // Calculate and execute the segments
    while (--segments) {

      static millis_t next_idle_ms = millis() + 200UL;
      thermalManager.manage_heater();  // This returns immediately if not really needed.
      if (ELAPSED(millis(), next_idle_ms)) {
        next_idle_ms = millis() + 200UL;
        idle();
      }

      LOOP_XYZE(i) raw[i] += segment_distance[i];
      #if ENABLED(DELTA)
        DELTA_IK(raw); // Delta can inline its kinematics
      #else
        inverse_kinematics(raw);
      #endif

      ADJUST_DELTA(raw); // Adjust Z if bed leveling is enabled

      planner.buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], raw[E_AXIS], _feedrate_mm_s, active_extruder, cartesian_segment_mm);
    }

    // Ensure last segment arrives at target location.
    planner.buffer_line_kinematic(rtarget, _feedrate_mm_s, active_extruder, cartesian_segment_mm);

    return false; // caller will update current_position
  }

#else // !IS_KINEMATIC

  /**
   * Prepare a linear move in a Cartesian setup.
   *
   * When a mesh-based leveling system is active, moves are segmented
   * according to the configuration of the leveling system.
   *
   * Returns true if current_position[] was set to destination[]
   */
  inline bool prepare_move_to_destination_cartesian() {
    #if HAS_MESH
      if (planner.leveling_active && planner.leveling_active_at_z(destination[Z_AXIS])) {
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          ubl.line_to_destination_cartesian(MMS_SCALED(feedrate_mm_s), active_extruder);  // UBL's motion routine needs to know about
          return true;                                                                    // all moves, including Z-only moves.
        #elif ENABLED(SEGMENT_LEVELED_MOVES)
          segmented_line_to_destination(MMS_SCALED(feedrate_mm_s));
          return false; // caller will update current_position
        #else
          /**
           * For MBL and ABL-BILINEAR only segment moves when X or Y are involved.
           * Otherwise fall through to do a direct single move.
           */
          if (current_position[X_AXIS] != destination[X_AXIS] || current_position[Y_AXIS] != destination[Y_AXIS]) {
            #if ENABLED(MESH_BED_LEVELING)
              mesh_line_to_destination(MMS_SCALED(feedrate_mm_s));
            #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
              bilinear_line_to_destination(MMS_SCALED(feedrate_mm_s));
            #endif
            return true;
          }
        #endif
      }
    #endif // HAS_MESH

    buffer_line_to_destination(MMS_SCALED(feedrate_mm_s));
    return false; // caller will update current_position
  }

#endif // !IS_KINEMATIC
#endif // !UBL_SEGMENTED

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * Unpark the carriage, if needed
   */
  inline bool dual_x_carriage_unpark() {
    if (active_extruder_parked)
      switch (dual_x_carriage_mode) {

        case DXC_FULL_CONTROL_MODE: break;

        case DXC_AUTO_PARK_MODE:
          if (current_position[E_AXIS] == destination[E_AXIS]) {
            // This is a travel move (with no extrusion)
            // Skip it, but keep track of the current position
            // (so it can be used as the start of the next non-travel move)
            if (delayed_move_time != 0xFFFFFFFFUL) {
              set_current_from_destination();
              NOLESS(raised_parked_position[Z_AXIS], destination[Z_AXIS]);
              delayed_move_time = millis();
              return true;
            }
          }
          // unpark extruder: 1) raise, 2) move into starting XY position, 3) lower
          for (uint8_t i = 0; i < 3; i++)
            planner.buffer_line(
              i == 0 ? raised_parked_position[X_AXIS] : current_position[X_AXIS],
              i == 0 ? raised_parked_position[Y_AXIS] : current_position[Y_AXIS],
              i == 2 ? current_position[Z_AXIS] : raised_parked_position[Z_AXIS],
              current_position[E_AXIS],
              i == 1 ? PLANNER_XY_FEEDRATE() : planner.max_feedrate_mm_s[Z_AXIS],
              active_extruder
            );
          delayed_move_time = 0;
          active_extruder_parked = false;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Clear active_extruder_parked");
          #endif
          break;

        case DXC_DUPLICATION_MODE:
          if (active_extruder == 0) {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("Set planner X", inactive_extruder_x_pos);
                SERIAL_ECHOLNPAIR(" ... Line to X", current_position[X_AXIS] + duplicate_extruder_x_offset);
              }
            #endif
            // move duplicate extruder into correct duplication position.
            planner.set_position_mm(
              inactive_extruder_x_pos,
              current_position[Y_AXIS],
              current_position[Z_AXIS],
              current_position[E_AXIS]
            );
            planner.buffer_line(
              current_position[X_AXIS] + duplicate_extruder_x_offset,
              current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS],
              planner.max_feedrate_mm_s[X_AXIS], 1
            );
            SYNC_PLAN_POSITION_KINEMATIC();
            stepper.synchronize();
            extruder_duplication_enabled = true;
            active_extruder_parked = false;
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Set extruder_duplication_enabled\nClear active_extruder_parked");
            #endif
          }
          else {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Active extruder not 0");
            #endif
          }
          break;
      }
    return false;
  }

#endif // DUAL_X_CARRIAGE

/**
 * Prepare a single move and get ready for the next one
 *
 * This may result in several calls to planner.buffer_line to
 * do smaller moves for DELTA, SCARA, mesh moves, etc.
 *
 * Make sure current_position[E] and destination[E] are good
 * before calling or cold/lengthy extrusion may get missed.
 */
void prepare_move_to_destination() {
  clamp_to_software_endstops(destination);
  refresh_cmd_timeout();

  #if ENABLED(PREVENT_COLD_EXTRUSION) || ENABLED(PREVENT_LENGTHY_EXTRUDE)

    if (!DEBUGGING(DRYRUN)) {
      if (destination[E_AXIS] != current_position[E_AXIS]) {
        #if ENABLED(PREVENT_COLD_EXTRUSION)
          if (thermalManager.tooColdToExtrude(active_extruder)) {
            current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM(MSG_ERR_COLD_EXTRUDE_STOP);
          }
        #endif // PREVENT_COLD_EXTRUSION
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          if (FABS(destination[E_AXIS] - current_position[E_AXIS]) * planner.e_factor[active_extruder] > (EXTRUDE_MAXLENGTH)) {
            current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM(MSG_ERR_LONG_EXTRUDE_STOP);
          }
        #endif // PREVENT_LENGTHY_EXTRUDE
      }
    }

  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    if (dual_x_carriage_unpark()) return;
  #endif

  if (
    #if UBL_SEGMENTED
      ubl.prepare_segmented_line_to(destination, MMS_SCALED(feedrate_mm_s))
    #elif IS_KINEMATIC
      prepare_kinematic_move_to(destination)
    #else
      prepare_move_to_destination_cartesian()
    #endif
  ) return;

  set_current_from_destination();
}

#if ENABLED(ARC_SUPPORT)

  #if N_ARC_CORRECTION < 1
    #undef N_ARC_CORRECTION
    #define N_ARC_CORRECTION 1
  #endif

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
    const float (&cart)[XYZE], // Destination position
    const float (&offset)[2], // Center of rotation relative to current_position
    const bool clockwise      // Clockwise?
  ) {
    #if ENABLED(CNC_WORKSPACE_PLANES)
      AxisEnum p_axis, q_axis, l_axis;
      switch (workspace_plane) {
        default:
        case PLANE_XY: p_axis = X_AXIS; q_axis = Y_AXIS; l_axis = Z_AXIS; break;
        case PLANE_ZX: p_axis = Z_AXIS; q_axis = X_AXIS; l_axis = Y_AXIS; break;
        case PLANE_YZ: p_axis = Y_AXIS; q_axis = Z_AXIS; l_axis = X_AXIS; break;
      }
    #else
      constexpr AxisEnum p_axis = X_AXIS, q_axis = Y_AXIS, l_axis = Z_AXIS;
    #endif

    // Radius vector from center to current location
    float r_P = -offset[0], r_Q = -offset[1];

    const float radius = HYPOT(r_P, r_Q),
                center_P = current_position[p_axis] - r_P,
                center_Q = current_position[q_axis] - r_Q,
                rt_X = cart[p_axis] - center_P,
                rt_Y = cart[q_axis] - center_Q,
                linear_travel = cart[l_axis] - current_position[l_axis],
                extruder_travel = cart[E_AXIS] - current_position[E_AXIS];

    // CCW angle of rotation between position and target from the circle center. Only one atan2() trig computation required.
    float angular_travel = ATAN2(r_P * rt_Y - r_Q * rt_X, r_P * rt_X + r_Q * rt_Y);
    if (angular_travel < 0) angular_travel += RADIANS(360);
    if (clockwise) angular_travel -= RADIANS(360);

    // Make a circle if the angular rotation is 0 and the target is current position
    if (angular_travel == 0 && current_position[p_axis] == cart[p_axis] && current_position[q_axis] == cart[q_axis])
      angular_travel = RADIANS(360);

    const float flat_mm = radius * angular_travel,
                mm_of_travel = linear_travel ? HYPOT(flat_mm, linear_travel) : FABS(flat_mm);
    if (mm_of_travel < 0.001) return;

    uint16_t segments = FLOOR(mm_of_travel / (MM_PER_ARC_SEGMENT));
    NOLESS(segments, 1);

    /**
     * Vector rotation by transformation matrix: r is the original vector, r_T is the rotated vector,
     * and phi is the angle of rotation. Based on the solution approach by Jens Geisler.
     *     r_T = [cos(phi) -sin(phi);
     *            sin(phi)  cos(phi)] * r ;
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
    float raw[XYZE];
    const float theta_per_segment = angular_travel / segments,
                linear_per_segment = linear_travel / segments,
                extruder_per_segment = extruder_travel / segments,
                sin_T = theta_per_segment,
                cos_T = 1 - 0.5 * sq(theta_per_segment); // Small angle approximation

    // Initialize the linear axis
    raw[l_axis] = current_position[l_axis];

    // Initialize the extruder axis
    raw[E_AXIS] = current_position[E_AXIS];

    const float fr_mm_s = MMS_SCALED(feedrate_mm_s);

    millis_t next_idle_ms = millis() + 200UL;

    #if N_ARC_CORRECTION > 1
      int8_t arc_recalc_count = N_ARC_CORRECTION;
    #endif

    for (uint16_t i = 1; i < segments; i++) { // Iterate (segments-1) times

      thermalManager.manage_heater();
      if (ELAPSED(millis(), next_idle_ms)) {
        next_idle_ms = millis() + 200UL;
        idle();
      }

      #if N_ARC_CORRECTION > 1
        if (--arc_recalc_count) {
          // Apply vector rotation matrix to previous r_P / 1
          const float r_new_Y = r_P * sin_T + r_Q * cos_T;
          r_P = r_P * cos_T - r_Q * sin_T;
          r_Q = r_new_Y;
        }
        else
      #endif
      {
        #if N_ARC_CORRECTION > 1
          arc_recalc_count = N_ARC_CORRECTION;
        #endif

        // Arc correction to radius vector. Computed only every N_ARC_CORRECTION increments.
        // Compute exact location by applying transformation matrix from initial radius vector(=-offset).
        // To reduce stuttering, the sin and cos could be computed at different times.
        // For now, compute both at the same time.
        const float cos_Ti = cos(i * theta_per_segment), sin_Ti = sin(i * theta_per_segment);
        r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti;
        r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti;
      }

      // Update raw location
      raw[p_axis] = center_P + r_P;
      raw[q_axis] = center_Q + r_Q;
      raw[l_axis] += linear_per_segment;
      raw[E_AXIS] += extruder_per_segment;

      clamp_to_software_endstops(raw);

      planner.buffer_line_kinematic(raw, fr_mm_s, active_extruder);
    }

    // Ensure last segment arrives at target location.
    planner.buffer_line_kinematic(cart, fr_mm_s, active_extruder);

    // As far as the parser is concerned, the position is now == target. In reality the
    // motion control system might still be processing the action and the real tool position
    // in any intermediate location.
    set_current_from_destination();
  } // plan_arc

#endif // ARC_SUPPORT

#if ENABLED(BEZIER_CURVE_SUPPORT)

  void plan_cubic_move(const float (&offset)[4]) {
    cubic_b_spline(current_position, destination, offset, MMS_SCALED(feedrate_mm_s), active_extruder);

    // As far as the parser is concerned, the position is now == destination. In reality the
    // motion control system might still be processing the action and the real tool position
    // in any intermediate location.
    set_current_from_destination();
  }

#endif // BEZIER_CURVE_SUPPORT

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
      const uint8_t speed = (lastMotorOn && PENDING(ms, lastMotorOn + (CONTROLLERFAN_SECS) * 1000UL)) ? CONTROLLERFAN_SPEED : 0;
      controllerFanSpeed = speed;

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
  void inverse_kinematics(const float raw[XYZ]) {

    static float C2, S2, SK1, SK2, THETA, PSI;

    float sx = raw[X_AXIS] - SCARA_OFFSET_X,  // Translate SCARA to standard X Y
          sy = raw[Y_AXIS] - SCARA_OFFSET_Y;  // With scaling factor.

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
    delta[C_AXIS] = raw[Z_AXIS];

    /*
      DEBUG_POS("SCARA IK", raw);
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

void enable_all_steppers() {
  #if ENABLED(AUTO_POWER_CONTROL)
    powerManager.power_on();
  #endif
  enable_X();
  enable_Y();
  enable_Z();
  enable_E0();
  enable_E1();
  enable_E2();
  enable_E3();
  enable_E4();
}

void disable_e_stepper(const uint8_t e) {
  switch (e) {
    case 0: disable_E0(); break;
    case 1: disable_E1(); break;
    case 2: disable_E2(); break;
    case 3: disable_E3(); break;
    case 4: disable_E4(); break;
  }
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

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

  FORCE_INLINE bool check_filament_runout() {

    if (IS_SD_PRINTING || print_job_timer.isRunning()) {

      #if NUM_RUNOUT_SENSORS < 2
        // A single sensor applying to all extruders
        return READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING;
      #else
        // Read the sensor for the active extruder
        switch (active_extruder) {
          case 0: return READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING;
          case 1: return READ(FIL_RUNOUT2_PIN) == FIL_RUNOUT_INVERTING;
          #if NUM_RUNOUT_SENSORS > 2
            case 2: return READ(FIL_RUNOUT3_PIN) == FIL_RUNOUT_INVERTING;
            #if NUM_RUNOUT_SENSORS > 3
              case 3: return READ(FIL_RUNOUT4_PIN) == FIL_RUNOUT_INVERTING;
              #if NUM_RUNOUT_SENSORS > 4
                case 4: return READ(FIL_RUNOUT5_PIN) == FIL_RUNOUT_INVERTING;
              #endif
            #endif
          #endif
        }
      #endif

    }
    return false;
  }

#endif // FILAMENT_RUNOUT_SENSOR

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
    if (check_filament_runout()) handle_filament_runout();
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
    #define MOVE_AWAY_TEST !did_pause_print
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
    #if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(ULTIPANEL)  // Only needed with an LCD
      if (ubl.lcd_map_control) ubl.lcd_map_control = defer_return_to_status = false;
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

  #if ENABLED(AUTO_POWER_CONTROL)
    powerManager.check();
  #endif

  #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
    if (thermalManager.degHotend(active_extruder) > EXTRUDER_RUNOUT_MINTEMP
      && ELAPSED(ms, previous_cmd_ms + (EXTRUDER_RUNOUT_SECONDS) * 1000UL)
      && !planner.blocks_queued()
    ) {
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

      previous_cmd_ms = ms; // refresh_cmd_timeout()

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
      set_destination_from_current();
      prepare_move_to_destination();
    }
  #endif

  #if ENABLED(TEMP_STAT_LEDS)
    handle_status_leds();
  #endif

  #if ENABLED(MONITOR_DRIVER_STATUS)
    monitor_tmc_driver();
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
    static millis_t i2cpem_next_update_ms;
    if (planner.blocks_queued() && ELAPSED(millis(), i2cpem_next_update_ms)) {
      I2CPEM.update();
      i2cpem_next_update_ms = millis() + I2CPE_MIN_UPD_TIME_MS;
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

  #if ENABLED(DISABLE_JTAG)
    // Disable JTAG on AT90USB chips to free up pins for IO
    MCUCR = 0x80;
    MCUCR = 0x80;
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    setup_filament_runout_pins();
  #endif

  setup_killpin();

  setup_powerhold();

  #if HAS_STEPPER_RESET
    disableStepperDrivers();
  #endif

  MYSERIAL0.begin(BAUDRATE);
  SERIAL_PROTOCOLLNPGM("start");
  SERIAL_ECHO_START();

  #if ENABLED(HAVE_TMC2130)
    tmc_init_cs_pins();
  #endif
  #if ENABLED(HAVE_TMC2208)
    tmc2208_serial_begin();
  #endif

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = MCUSR;
  if (mcu &  1) SERIAL_ECHOLNPGM(MSG_POWERUP);
  if (mcu &  2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
  if (mcu &  4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
  if (mcu &  8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
  if (mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
  MCUSR = 0;

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

  // Send "ok" after commands by default
  for (int8_t i = 0; i < BUFSIZE; i++) send_ok[i] = true;

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

  #if ENABLED(USE_WATCHDOG)
    watchdog_init();
  #endif

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
    #if ENABLED(SPINDLE_LASER_PWM)
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

  #if HAS_COLOR_LEDS
    leds.setup();
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
  LCD_MESSAGEPGM(WELCOME_MSG);

  #if ENABLED(SHOW_BOOTSCREEN)
    lcd_bootscreen();
  #endif

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    // Virtual Tools 0, 1, 2, 3 = Filament 1, 2, 3, 4, etc.
    for (uint8_t t = 0; t < MIXING_VIRTUAL_TOOLS && t < MIXING_STEPPERS; t++)
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        mixing_virtual_tool_mix[t][i] = (t == i) ? 1.0 : 0.0;

    // Remaining virtual tools are 100% filament 1
    #if MIXING_STEPPERS < MIXING_VIRTUAL_TOOLS
      for (uint8_t t = MIXING_STEPPERS; t < MIXING_VIRTUAL_TOOLS; t++)
        for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
          mixing_virtual_tool_mix[t][i] = (i == 0) ? 1.0 : 0.0;
    #endif

    // Initialize mixing to tool 0 color
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
      mixing_factor[i] = mixing_virtual_tool_mix[0][i];
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
  #if ENABLED(MKS_12864OLED) || ENABLED(MKS_12864OLED_SSD1306)
    SET_OUTPUT(LCD_PINS_DC);
    OUT_WRITE(LCD_PINS_RS, LOW);
    delay(1000);
    WRITE(LCD_PINS_RS, HIGH);
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

          #ifndef USBCON
            #if ENABLED(SERIAL_STATS_DROPPED_RX)
              SERIAL_ECHOLNPAIR("Dropped bytes: ", customizedSerial.dropped());
            #endif

            #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
              SERIAL_ECHOLNPAIR("Max RX Queue Size: ", customizedSerial.rxMaxEnqueued());
            #endif
          #endif // !USBCON

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

    // The queue may be reset by a command handler or by code invoked by idle() within a handler
    if (commands_in_queue) {
      --commands_in_queue;
      if (++cmd_queue_index_r >= BUFSIZE) cmd_queue_index_r = 0;
    }
  }
  endstops.report_state();
  idle();
}
