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
 * gcode.h - Temporary container for all gcode handlers
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
 *        OR, with 'S<seconds>' set the SD status auto-report interval. (Requires AUTO_REPORT_SD_STATUS)
 *        OR, with 'C' get the current filename.
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
 * M122 - Debug stepper (Requires HAVE_TMC2130 or HAVE_TMC2208)
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
 * M218 - Set/get a tool offset: "M218 T<index> X<offset> Y<offset>". (Requires 2 or more extruders)
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
 * M605 - Set Dual X-Carriage movement mode: "M605 S<mode> [X<x_offset>] [R<temp_offset>]". (Requires DUAL_X_CARRIAGE)
 * M665 - Set delta configurations: "M665 L<diagonal rod> R<delta radius> S<segments/s> A<rod A trim mm> B<rod B trim mm> C<rod C trim mm> I<tower A trim angle> J<tower B trim angle> K<tower C trim angle>" (Requires DELTA)
 * M666 - Set/get offsets for delta (Requires DELTA) or dual endstops (Requires [XYZ]_DUAL_ENDSTOPS).
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

#ifndef _GCODE_H_
#define _GCODE_H_

#include "../inc/MarlinConfig.h"
#include "parser.h"

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "../feature/I2CPositionEncoder.h"
#endif

class GcodeSuite {
public:

  GcodeSuite() {}

  static uint8_t target_extruder;

  static bool axis_relative_modes[];

  #if ENABLED(CNC_WORKSPACE_PLANES)
    /**
     * Workspace planes only apply to G2/G3 moves
     * (and "canned cycles" - not a current feature)
     */
    enum WorkspacePlane : char { PLANE_XY, PLANE_ZX, PLANE_YZ };
    static WorkspacePlane workspace_plane;
  #endif

  #define MAX_COORDINATE_SYSTEMS 9
  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    static int8_t active_coordinate_system;
    static float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ];
    static bool select_coordinate_system(const int8_t _new);
  #endif

  static millis_t previous_move_ms;
  FORCE_INLINE static void reset_stepper_timeout() { previous_move_ms = millis(); }

  static bool get_target_extruder_from_command();
  static void get_destination_from_command();
  static void process_parsed_command();
  static void process_next_command();

  FORCE_INLINE static void home_all_axes() { G28(true); }

  /**
   * Multi-stepper support for M92, M201, M203
   */
  #if ENABLED(DISTINCT_E_FACTORS)
    #define GET_TARGET_EXTRUDER() if (gcode.get_target_extruder_from_command()) return
    #define TARGET_EXTRUDER gcode.target_extruder
  #else
    #define GET_TARGET_EXTRUDER() NOOP
    #define TARGET_EXTRUDER 0
  #endif

  #if ENABLED(HOST_KEEPALIVE_FEATURE)
    /**
     * States for managing Marlin and host communication
     * Marlin sends messages if blocked or busy
     */
    enum MarlinBusyState : char {
      NOT_BUSY,           // Not in a handler
      IN_HANDLER,         // Processing a GCode
      IN_PROCESS,         // Known to be blocking command input (as in G29)
      PAUSED_FOR_USER,    // Blocking pending any input
      PAUSED_FOR_INPUT    // Blocking pending text input (concept)
    };

    static MarlinBusyState busy_state;
    static uint8_t host_keepalive_interval;

    static void host_keepalive();

    #define KEEPALIVE_STATE(n) gcode.busy_state = gcode.n
  #else
    #define KEEPALIVE_STATE(n) NOOP
  #endif

  static void dwell(millis_t time);

private:

  static void G0_G1(
    #if IS_SCARA
      bool fast_move=false
    #endif
  );

  #if ENABLED(ARC_SUPPORT)
    static void G2_G3(const bool clockwise);
  #endif

  static void G4();

  #if ENABLED(BEZIER_CURVE_SUPPORT)
    static void G5();
  #endif

  #if ENABLED(FWRETRACT)
    static void G10();
    static void G11();
  #endif

  #if ENABLED(NOZZLE_CLEAN_FEATURE)
    static void G12();
  #endif

  #if ENABLED(CNC_WORKSPACE_PLANES)
    static void G17();
    static void G18();
    static void G19();
  #endif

  #if ENABLED(INCH_MODE_SUPPORT)
    static void G20();
    static void G21();
  #endif

  #if ENABLED(G26_MESH_VALIDATION)
    static void G26();
  #endif

  #if ENABLED(NOZZLE_PARK_FEATURE)
    static void G27();
  #endif

  static void G28(const bool always_home_all);

  #if HAS_LEVELING
    static void G29();
  #endif

  #if HAS_BED_PROBE
    static void G30();
    #if ENABLED(Z_PROBE_SLED)
      static void G31();
      static void G32();
    #endif
  #endif

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    static void G33();
  #endif

  #if ENABLED(G38_PROBE_TARGET)
    static void G38(const bool is_38_2);
  #endif

  #if HAS_MESH
    static void G42();
  #endif

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    static void G53();
    static void G54();
    static void G55();
    static void G56();
    static void G57();
    static void G58();
    static void G59();
  #endif

  static void G92();

  #if HAS_RESUME_CONTINUE
    static void M0_M1();
  #endif

  #if ENABLED(SPINDLE_LASER_ENABLE)
    static void M3_M4(bool is_M3);
    static void M5();
  #endif

  static void M17();

  static void M18_M84();

  #if ENABLED(SDSUPPORT)
    static void M20();
    static void M21();
    static void M22();
    static void M23();
    static void M24();
    static void M25();
    static void M26();
    static void M27();
    static void M28();
    static void M29();
    static void M30();
  #endif

  static void M31();

  #if ENABLED(SDSUPPORT)
    static void M32();
    #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
      static void M33();
    #endif
    #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
      static void M34();
    #endif
  #endif

  static void M42();

  #if ENABLED(PINS_DEBUGGING)
    static void M43();
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    static void M48();
  #endif

  #if ENABLED(G26_MESH_VALIDATION)
    static void M49();
  #endif

  #if ENABLED(ULTRA_LCD) && ENABLED(LCD_SET_PROGRESS_MANUALLY)
    static void M73();
  #endif

  static void M75();
  static void M76();
  static void M77();

  #if ENABLED(PRINTCOUNTER)
    static void M78();
  #endif

  #if HAS_POWER_SWITCH
    static void M80();
  #endif

  static void M81();
  static void M82();
  static void M83();
  static void M85();
  static void M92();

  #if ENABLED(M100_FREE_MEMORY_WATCHER)
    static void M100();
  #endif

  static void M104();
  static void M105();
  static void M106();
  static void M107();

  #if DISABLED(EMERGENCY_PARSER)
    static void M108();
    static void M112();
    static void M410();
  #endif

  static void M109();

  static void M110();
  static void M111();

  #if ENABLED(HOST_KEEPALIVE_FEATURE)
    static void M113();
  #endif

  static void M114();
  static void M115();
  static void M117();
  static void M118();
  static void M119();
  static void M120();
  static void M121();

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    static void M125();
  #endif

  #if ENABLED(BARICUDA)
    #if HAS_HEATER_1
      static void M126();
      static void M127();
    #endif
    #if HAS_HEATER_2
      static void M128();
      static void M129();
    #endif
  #endif

  #if HAS_HEATER_BED && HAS_TEMP_BED
    static void M140();
    static void M190();
  #endif

  #if ENABLED(ULTIPANEL)
    static void M145();
  #endif

  #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
    static void M149();
  #endif

  #if HAS_COLOR_LEDS
    static void M150();
  #endif

  #if ENABLED(AUTO_REPORT_TEMPERATURES) && HAS_TEMP_SENSOR
    static void M155();
  #endif

  #if ENABLED(MIXING_EXTRUDER)
    static void M163();
    #if MIXING_VIRTUAL_TOOLS > 1
      static void M164();
    #endif
    #if ENABLED(DIRECT_MIXING_IN_G1)
      static void M165();
    #endif
  #endif

  static void M200();
  static void M201();

  #if 0
    static void M202(); // Not used for Sprinter/grbl gen6
  #endif

  static void M203();
  static void M204();
  static void M205();

  #if HAS_M206_COMMAND
    static void M206();
  #endif

  #if ENABLED(FWRETRACT)
    static void M207();
    static void M208();
    static void M209();
  #endif

  static void M211();

  #if HOTENDS > 1
    static void M218();
  #endif

  static void M220();
  static void M221();
  static void M226();

  #if defined(CHDK) || HAS_PHOTOGRAPH
    static void M240();
  #endif

  #if HAS_LCD_CONTRAST
    static void M250();
  #endif

  #if ENABLED(EXPERIMENTAL_I2CBUS)
    static void M260();
    static void M261();
  #endif

  #if HAS_SERVOS
    static void M280();
  #endif

  #if ENABLED(BABYSTEPPING)
    static void M290();
  #endif

  #if HAS_BUZZER
    static void M300();
  #endif

  #if ENABLED(PIDTEMP)
    static void M301();
  #endif

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    static void M302();
  #endif

  static void M303();

  #if ENABLED(PIDTEMPBED)
    static void M304();
  #endif

  #if HAS_MICROSTEPS
    static void M350();
    static void M351();
  #endif

  static void M355();

  #if ENABLED(MORGAN_SCARA)
    static bool M360();
    static bool M361();
    static bool M362();
    static bool M363();
    static bool M364();
  #endif

  #if ENABLED(EXT_SOLENOID)
    static void M380();
    static void M381();
  #endif

  static void M400();

  #if HAS_BED_PROBE
    static void M401();
    static void M402();
  #endif

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    static void M404();
    static void M405();
    static void M406();
    static void M407();
  #endif

  #if HAS_LEVELING
    static void M420();
    static void M421();
  #endif

  #if HAS_M206_COMMAND
    static void M428();
  #endif

  static void M500();
  static void M501();
  static void M502();
  #if DISABLED(DISABLE_M503)
    static void M503();
  #endif
  #if ENABLED(EEPROM_SETTINGS)
    static void M504();
  #endif

  #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
    static void M540();
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    static void M600();
    static void M603();
  #endif

  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
    static void M605();
  #endif

  #if IS_KINEMATIC
    static void M665();
  #endif

  #if ENABLED(DELTA) || ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
    static void M666();
  #endif

  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    static void M701();
    static void M702();
  #endif

  #if HAS_BED_PROBE
    static void M851();
  #endif

  #if ENABLED(SKEW_CORRECTION_GCODE)
    static void M852();
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    FORCE_INLINE static void M860() { I2CPEM.M860(); }
    FORCE_INLINE static void M861() { I2CPEM.M861(); }
    FORCE_INLINE static void M862() { I2CPEM.M862(); }
    FORCE_INLINE static void M863() { I2CPEM.M863(); }
    FORCE_INLINE static void M864() { I2CPEM.M864(); }
    FORCE_INLINE static void M865() { I2CPEM.M865(); }
    FORCE_INLINE static void M866() { I2CPEM.M866(); }
    FORCE_INLINE static void M867() { I2CPEM.M867(); }
    FORCE_INLINE static void M868() { I2CPEM.M868(); }
    FORCE_INLINE static void M869() { I2CPEM.M869(); }
  #endif

  #if ENABLED(LIN_ADVANCE)
    static void M900();
  #endif

  #if HAS_TRINAMIC
    #if ENABLED(TMC_DEBUG)
      static void M122();
    #endif
    static void M906();
    static void M911();
    static void M912();
    #if ENABLED(HYBRID_THRESHOLD)
      static void M913();
    #endif
    #if ENABLED(SENSORLESS_HOMING)
      static void M914();
    #endif
    #if ENABLED(TMC_Z_CALIBRATION)
      static void M915();
    #endif
  #endif

  #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM || ENABLED(DIGIPOT_I2C) || ENABLED(DAC_STEPPER_CURRENT)
    static void M907();
    #if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)
      static void M908();
      #if ENABLED(DAC_STEPPER_CURRENT)
        static void M909();
        static void M910();
      #endif
    #endif
  #endif

  #if ENABLED(SDSUPPORT)
    static void M928();
  #endif

  static void M999();

  static void T(const uint8_t tmp_extruder);

};

extern GcodeSuite gcode;

#endif // _GCODE_H_
