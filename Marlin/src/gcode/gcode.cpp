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
 * gcode.cpp - Temporary container for all gcode handlers
 *             Most will migrate to classes, by feature.
 */

#include "gcode.h"
GcodeSuite gcode;

#include "parser.h"
#include "queue.h"
#include "../module/motion.h"

#if ENABLED(PRINTCOUNTER)
  #include "../module/printcounter.h"
#endif

#if ENABLED(DIRECT_MIXING_IN_G1)
  #include "../feature/mixing.h"
#endif

#include "../Marlin.h" // for idle() and suspend_auto_report

uint8_t GcodeSuite::target_extruder;
millis_t GcodeSuite::previous_move_ms;

bool GcodeSuite::axis_relative_modes[] = AXIS_RELATIVE_MODES;

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  GcodeSuite::MarlinBusyState GcodeSuite::busy_state = NOT_BUSY;
  uint8_t GcodeSuite::host_keepalive_interval = DEFAULT_KEEPALIVE_INTERVAL;
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)
  GcodeSuite::WorkspacePlane GcodeSuite::workspace_plane = PLANE_XY;
#endif

#if ENABLED(CNC_COORDINATE_SYSTEMS)
  int8_t GcodeSuite::active_coordinate_system = -1; // machine space
  float GcodeSuite::coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ];
#endif

#if HAS_LEVELING && ENABLED(G29_RETRY_AND_RECOVER)
  #include "../feature/bedlevel/bedlevel.h"
  #include "../module/planner.h"
#endif

/**
 * Set target_extruder from the T parameter or the active_extruder
 *
 * Returns TRUE if the target is invalid
 */
bool GcodeSuite::get_target_extruder_from_command() {
  if (parser.seenval('T')) {
    const int8_t e = parser.value_byte();
    if (e >= EXTRUDERS) {
      SERIAL_ECHO_START();
      SERIAL_CHAR('M');
      SERIAL_ECHO(parser.codenum);
      SERIAL_ECHOLNPAIR(" " MSG_INVALID_EXTRUDER " ", e);
      return true;
    }
    target_extruder = e;
  }
  else
    target_extruder = active_extruder;

  return false;
}

/**
 * Set XYZE destination and feedrate from the current GCode command
 *
 *  - Set destination from included axis codes
 *  - Set to current for missing axis codes
 *  - Set the feedrate, if included
 */
void GcodeSuite::get_destination_from_command() {
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

/**
 * Dwell waits immediately. It does not synchronize. Use M400 instead of G4
 */
void GcodeSuite::dwell(millis_t time) {
  time += millis();
  while (PENDING(millis(), time)) idle();
}

/**
 * When G29_RETRY_AND_RECOVER is enabled, call G29() in
 * a loop with recovery and retry handling.
 */
#if HAS_LEVELING && ENABLED(G29_RETRY_AND_RECOVER)

  void GcodeSuite::G29_with_retry() {
    set_bed_leveling_enabled(false);
    for (uint8_t i = G29_MAX_RETRIES; i--;) {
      G29();
      if (planner.leveling_active) break;
      #ifdef G29_ACTION_ON_RECOVER
        SERIAL_ECHOLNPGM("//action:" G29_ACTION_ON_RECOVER);
      #endif
      #ifdef G29_RECOVER_COMMANDS
        process_subcommands_now_P(PSTR(G29_RECOVER_COMMANDS));
      #endif
    }
    if (planner.leveling_active) {
      #ifdef G29_SUCCESS_COMMANDS
        process_subcommands_now_P(PSTR(G29_SUCCESS_COMMANDS));
      #endif
    }
    else {
      #ifdef G29_FAILURE_COMMANDS
        process_subcommands_now_P(PSTR(G29_FAILURE_COMMANDS));
      #endif
      #ifdef G29_ACTION_ON_FAILURE
        SERIAL_ECHOLNPGM("//action:" G29_ACTION_ON_FAILURE);
      #endif
      #if ENABLED(G29_HALT_ON_FAILURE)
        kill(PSTR(MSG_ERR_PROBING_FAILED));
      #endif
    }
  }

#endif // HAS_LEVELING && G29_RETRY_AND_RECOVER

//
// Placeholders for non-migrated codes
//
#if ENABLED(M100_FREE_MEMORY_WATCHER)
  extern void M100_dump_routine(const char * const title, const char *start, const char *end);
#endif

/**
 * Process the parsed command and dispatch it to its handler
 */
void GcodeSuite::process_parsed_command(
  #if ENABLED(USE_EXECUTE_COMMANDS_IMMEDIATE)
    const bool no_ok
  #endif
) {
  KEEPALIVE_STATE(IN_HANDLER);

  // Handle a known G, M, or T
  switch (parser.command_letter) {
    case 'G': switch (parser.codenum) {

      case 0: case 1: G0_G1(                                      // G0: Fast Move, G1: Linear Move
                        #if IS_SCARA
                          parser.codenum == 0
                        #endif
                      );
                      break;

      #if ENABLED(ARC_SUPPORT) && DISABLED(SCARA)
        case 2: case 3: G2_G3(parser.codenum == 2); break;        // G2: CW ARC, G3: CCW ARC
      #endif

      case 4: G4(); break;                                        // G4: Dwell

      #if ENABLED(BEZIER_CURVE_SUPPORT)
        case 5: G5(); break;                                      // G5: Cubic B_spline
      #endif

      #if ENABLED(FWRETRACT)
        case 10: G10(); break;                                    // G10: Retract / Swap Retract
        case 11: G11(); break;                                    // G11: Recover / Swap Recover
      #endif

      #if ENABLED(NOZZLE_CLEAN_FEATURE)
        case 12: G12(); break;                                    // G12: Nozzle Clean
      #endif

      #if ENABLED(CNC_WORKSPACE_PLANES)
        case 17: G17(); break;                                    // G17: Select Plane XY
        case 18: G18(); break;                                    // G18: Select Plane ZX
        case 19: G19(); break;                                    // G19: Select Plane YZ
      #endif

      #if ENABLED(INCH_MODE_SUPPORT)
        case 20: G20(); break;                                    // G20: Inch Mode
        case 21: G21(); break;                                    // G21: MM Mode
      #endif

      #if ENABLED(G26_MESH_VALIDATION)
        case 26: G26(); break;                                    // G26: Mesh Validation Pattern generation
      #endif

      #if ENABLED(NOZZLE_PARK_FEATURE)
        case 27: G27(); break;                                    // G27: Nozzle Park
      #endif

      case 28: G28(false); break;                                 // G28: Home all axes, one at a time

      #if HAS_LEVELING
        case 29:                                                  // G29: Bed leveling calibration
          #if ENABLED(G29_RETRY_AND_RECOVER)
            G29_with_retry();
          #else
            G29();
          #endif
          break;
      #endif // HAS_LEVELING

      #if HAS_BED_PROBE
        case 30: G30(); break;                                    // G30: Single Z probe
        #if ENABLED(Z_PROBE_SLED)
          case 31: G31(); break;                                  // G31: dock the sled
          case 32: G32(); break;                                  // G32: undock the sled
        #endif
      #endif

      #if ENABLED(DELTA_AUTO_CALIBRATION)
        case 33: G33(); break;                                    // G33: Delta Auto-Calibration
      #endif

      #if ENABLED(G38_PROBE_TARGET)
        case 38:                                                  // G38.2 & G38.3
          if (parser.subcode == 2 || parser.subcode == 3)
            G38(parser.subcode == 2);
          break;
      #endif

      case 90: relative_mode = false; break;                      // G90: Relative Mode
      case 91: relative_mode = true; break;                       // G91: Absolute Mode

      case 92: G92(); break;                                      // G92: Set current axis position(s)

      #if HAS_MESH
        case 42: G42(); break;                                    // G42: Coordinated move to a mesh point
      #endif

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800: parser.debug(); break;                          // G800: GCode Parser Test for G
      #endif

      default: parser.unknown_command_error(); break;
    }
    break;

    case 'M': switch (parser.codenum) {
      #if HAS_RESUME_CONTINUE
        case 0:                                                   // M0: Unconditional stop - Wait for user button press on LCD
        case 1: M0_M1(); break;                                   // M1: Conditional stop - Wait for user button press on LCD
      #endif

      #if ENABLED(SPINDLE_LASER_ENABLE)
        case 3: M3_M4(true ); break;                              // M3: turn spindle/laser on, set laser/spindle power/speed, set rotation direction CW
        case 4: M3_M4(false); break;                              // M4: turn spindle/laser on, set laser/spindle power/speed, set rotation direction CCW
        case 5: M5(); break;                                      // M5 - turn spindle/laser off
      #endif

      case 17: M17(); break;                                      // M17: Enable all stepper motors

      #if ENABLED(SDSUPPORT)
        case 20: M20(); break;                                    // M20: list SD card
        case 21: M21(); break;                                    // M21: init SD card
        case 22: M22(); break;                                    // M22: release SD card
        case 23: M23(); break;                                    // M23: Select file
        case 24: M24(); break;                                    // M24: Start SD print
        case 25: M25(); break;                                    // M25: Pause SD print
        case 26: M26(); break;                                    // M26: Set SD index
        case 27: M27(); break;                                    // M27: Get SD status
        case 28: M28(); break;                                    // M28: Start SD write
        case 29: M29(); break;                                    // M29: Stop SD write
        case 30: M30(); break;                                    // M30 <filename> Delete File
        case 32: M32(); break;                                    // M32: Select file and start SD print

        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          case 33: M33(); break;                                  // M33: Get the long full path to a file or folder
        #endif

        #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
          case 34: M34(); break;                                  // M34: Set SD card sorting options
        #endif

        case 928: M928(); break;                                  // M928: Start SD write
      #endif // SDSUPPORT

      case 31: M31(); break;                                      // M31: Report time since the start of SD print or last M109
      case 42: M42(); break;                                      // M42: Change pin state

      #if ENABLED(PINS_DEBUGGING)
        case 43: M43(); break;                                    // M43: Read pin state
      #endif

      #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
        case 48: M48(); break;                                    // M48: Z probe repeatability test
      #endif

      #if ENABLED(G26_MESH_VALIDATION)
        case 49: M49(); break;                                    // M49: Turn on or off G26 debug flag for verbose output
      #endif

      #if ENABLED(ULTRA_LCD) && ENABLED(LCD_SET_PROGRESS_MANUALLY)
        case 73: M73(); break;                                    // M73: Set progress percentage (for display on LCD)
      #endif

      case 75: M75(); break;                                      // M75: Start print timer
      case 76: M76(); break;                                      // M76: Pause print timer
      case 77: M77(); break;                                      // M77: Stop print timer

      #if ENABLED(PRINTCOUNTER)
        case 78: M78(); break;                                    // M78: Show print statistics
      #endif

      #if ENABLED(M100_FREE_MEMORY_WATCHER)
        case 100: M100(); break;                                  // M100: Free Memory Report
      #endif

      case 104: M104(); break;                                    // M104: Set hot end temperature
      case 109: M109(); break;                                    // M109: Wait for hotend temperature to reach target
      case 110: M110(); break;                                    // M110: Set Current Line Number
      case 111: M111(); break;                                    // M111: Set debug level

      #if DISABLED(EMERGENCY_PARSER)
        case 108: M108(); break;                                  // M108: Cancel Waiting
        case 112: M112(); break;                                  // M112: Emergency Stop
        case 410: M410(); break;                                  // M410: Quickstop - Abort all the planned moves.
      #endif

      #if ENABLED(HOST_KEEPALIVE_FEATURE)
        case 113: M113(); break;                                  // M113: Set Host Keepalive interval
      #endif

      #if HAS_HEATED_BED
        case 140: M140(); break;                                  // M140: Set bed temperature
        case 190: M190(); break;                                  // M190: Wait for bed temperature to reach target
      #endif

      case 105: M105(); KEEPALIVE_STATE(NOT_BUSY); return;        // M105: Report Temperatures (and say "ok")

      #if ENABLED(AUTO_REPORT_TEMPERATURES) && HAS_TEMP_SENSOR
        case 155: M155(); break;                                  // M155: Set temperature auto-report interval
      #endif

      #if FAN_COUNT > 0
        case 106: M106(); break;                                  // M106: Fan On
        case 107: M107(); break;                                  // M107: Fan Off
      #endif

      #if ENABLED(PARK_HEAD_ON_PAUSE)
        case 125: M125(); break;                                  // M125: Store current position and move to filament change position
      #endif

      #if ENABLED(BARICUDA)
        // PWM for HEATER_1_PIN
        #if HAS_HEATER_1
          case 126: M126(); break;                                // M126: valve open
          case 127: M127(); break;                                // M127: valve closed
        #endif

        // PWM for HEATER_2_PIN
        #if HAS_HEATER_2
          case 128: M128(); break;                                // M128: valve open
          case 129: M129(); break;                                // M129: valve closed
        #endif
      #endif // BARICUDA

      #if HAS_POWER_SWITCH
        case 80: M80(); break;                                    // M80: Turn on Power Supply
      #endif
      case 81: M81(); break;                                      // M81: Turn off Power, including Power Supply, if possible

      case 82: M82(); break;                                      // M82: Set E axis normal mode (same as other axes)
      case 83: M83(); break;                                      // M83: Set E axis relative mode
      case 18: case 84: M18_M84(); break;                         // M18/M84: Disable Steppers / Set Timeout
      case 85: M85(); break;                                      // M85: Set inactivity stepper shutdown timeout
      case 92: M92(); break;                                      // M92: Set the steps-per-unit for one or more axes
      case 114: M114(); break;                                    // M114: Report current position
      case 115: M115(); break;                                    // M115: Report capabilities
      case 117: M117(); break;                                    // M117: Set LCD message text, if possible
      case 118: M118(); break;                                    // M118: Display a message in the host console
      case 119: M119(); break;                                    // M119: Report endstop states
      case 120: M120(); break;                                    // M120: Enable endstops
      case 121: M121(); break;                                    // M121: Disable endstops

      #if ENABLED(ULTIPANEL)
        case 145: M145(); break;                                  // M145: Set material heatup parameters
      #endif

      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        case 149: M149(); break;                                  // M149: Set temperature units
      #endif

      #if HAS_COLOR_LEDS
        case 150: M150(); break;                                  // M150: Set Status LED Color
      #endif

      #if ENABLED(MIXING_EXTRUDER)
        case 163: M163(); break;                                  // M163: Set a component weight for mixing extruder
        #if MIXING_VIRTUAL_TOOLS > 1
          case 164: M164(); break;                                // M164: Save current mix as a virtual extruder
        #endif
        #if ENABLED(DIRECT_MIXING_IN_G1)
          case 165: M165(); break;                                // M165: Set multiple mix weights
        #endif
      #endif

      #if DISABLED(NO_VOLUMETRICS)
        case 200: M200(); break;                                  // M200: Set filament diameter, E to cubic units
      #endif

      case 201: M201(); break;                                    // M201: Set max acceleration for print moves (units/s^2)

      #if 0
        case 202: M202(); break;                                  // M202: Not used for Sprinter/grbl gen6
      #endif

      case 203: M203(); break;                                    // M203: Set max feedrate (units/sec)
      case 204: M204(); break;                                    // M204: Set acceleration
      case 205: M205(); break;                                    // M205: Set advanced settings

      #if HAS_M206_COMMAND
        case 206: M206(); break;                                  // M206: Set home offsets
      #endif

      #if ENABLED(DELTA)
        case 665: M665(); break;                                  // M665: Set delta configurations
      #endif

      #if ENABLED(DELTA) || ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
        case 666: M666(); break;                                  // M666: Set delta or dual endstop adjustment
      #endif

      #if ENABLED(FWRETRACT)
        case 207: M207(); break;                                  // M207: Set Retract Length, Feedrate, and Z lift
        case 208: M208(); break;                                  // M208: Set Recover (unretract) Additional Length and Feedrate
        case 209:
          if (MIN_AUTORETRACT <= MAX_AUTORETRACT) M209();         // M209: Turn Automatic Retract Detection on/off
          break;
      #endif

      case 211: M211(); break;                                    // M211: Enable, Disable, and/or Report software endstops

      #if HOTENDS > 1
        case 218: M218(); break;                                  // M218: Set a tool offset
      #endif

      case 220: M220(); break;                                    // M220: Set Feedrate Percentage: S<percent> ("FR" on your LCD)
      case 221: M221(); break;                                    // M221: Set Flow Percentage
      case 226: M226(); break;                                    // M226: Wait until a pin reaches a state

      #if HAS_SERVOS
        case 280: M280(); break;                                  // M280: Set servo position absolute
      #endif

      #if ENABLED(BABYSTEPPING)
        case 290: M290(); break;                                  // M290: Babystepping
      #endif

      #if HAS_BUZZER
        case 300: M300(); break;                                  // M300: Play beep tone
      #endif

      #if ENABLED(PIDTEMP)
        case 301: M301(); break;                                  // M301: Set hotend PID parameters
      #endif

      #if ENABLED(PIDTEMPBED)
        case 304: M304(); break;                                  // M304: Set bed PID parameters
      #endif

      #if defined(CHDK) || HAS_PHOTOGRAPH
        case 240: M240(); break;                                  // M240: Trigger a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
      #endif

      #if HAS_LCD_CONTRAST
        case 250: M250(); break;                                  // M250: Set LCD contrast
      #endif

      #if ENABLED(EXPERIMENTAL_I2CBUS)
        case 260: M260(); break;                                  // M260: Send data to an i2c slave
        case 261: M261(); break;                                  // M261: Request data from an i2c slave
      #endif

      #if ENABLED(PREVENT_COLD_EXTRUSION)
        case 302: M302(); break;                                  // M302: Allow cold extrudes (set the minimum extrude temperature)
      #endif

      case 303: M303(); break;                                    // M303: PID autotune

      #if ENABLED(MORGAN_SCARA)
        case 360: if (M360()) return; break;                      // M360: SCARA Theta pos1
        case 361: if (M361()) return; break;                      // M361: SCARA Theta pos2
        case 362: if (M362()) return; break;                      // M362: SCARA Psi pos1
        case 363: if (M363()) return; break;                      // M363: SCARA Psi pos2
        case 364: if (M364()) return; break;                      // M364: SCARA Psi pos3 (90 deg to Theta)
      #endif

      #if ENABLED(EXT_SOLENOID)
        case 380: M380(); break;                                  // M380: Activate solenoid on active extruder
        case 381: M381(); break;                                  // M381: Disable all solenoids
      #endif

      case 400: M400(); break;                                    // M400: Finish all moves

      #if HAS_BED_PROBE
        case 401: M401(); break;                                  // M401: Deploy probe
        case 402: M402(); break;                                  // M402: Stow probe
      #endif

      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        case 404: M404(); break;                                  // M404: Enter the nominal filament width (3mm, 1.75mm ) N<3.0> or display nominal filament width
        case 405: M405(); break;                                  // M405: Turn on filament sensor for control
        case 406: M406(); break;                                  // M406: Turn off filament sensor for control
        case 407: M407(); break;                                  // M407: Display measured filament diameter
      #endif

      #if HAS_LEVELING
        case 420: M420(); break;                                  // M420: Enable/Disable Bed Leveling
      #endif

      #if HAS_MESH
        case 421: M421(); break;                                  // M421: Set a Mesh Bed Leveling Z coordinate
      #endif

      #if HAS_M206_COMMAND
        case 428: M428(); break;                                  // M428: Apply current_position to home_offset
      #endif

      case 500: M500(); break;                                    // M500: Store settings in EEPROM
      case 501: M501(); break;                                    // M501: Read settings from EEPROM
      case 502: M502(); break;                                    // M502: Revert to default settings
      #if DISABLED(DISABLE_M503)
        case 503: M503(); break;                                  // M503: print settings currently in memory
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        case 504: M504(); break;                                  // M504: Validate EEPROM contents
      #endif

      #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
        case 540: M540(); break;                                  // M540: Set abort on endstop hit for SD printing
      #endif

      #if HAS_BED_PROBE
        case 851: M851(); break;                                  // M851: Set Z Probe Z Offset
      #endif

      #if ENABLED(SKEW_CORRECTION_GCODE)
        case 852: M852(); break;                                  // M852: Set Skew factors
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case 600: M600(); break;                                  // M600: Pause for Filament Change
        case 603: M603(); break;                                  // M603: Configure Filament Change
      #endif

      #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
        case 605: M605(); break;                                  // M605: Set Dual X Carriage movement mode
      #endif

      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        case 701: M701(); break;                                  // M701: Load Filament
        case 702: M702(); break;                                  // M702: Unload Filament
      #endif

      #if ENABLED(MAX7219_GCODE)
        case 7219: M7219(); break;                                // M7219: Set LEDs, columns, and rows
      #endif

      #if ENABLED(LIN_ADVANCE)
        case 900: M900(); break;                                  // M900: Set advance K factor.
      #endif

      #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM || ENABLED(DIGIPOT_I2C) || ENABLED(DAC_STEPPER_CURRENT)
        case 907: M907(); break;                                  // M907: Set digital trimpot motor current using axis codes.
        #if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)
          case 908: M908(); break;                                // M908: Control digital trimpot directly.
          #if ENABLED(DAC_STEPPER_CURRENT)
            case 909: M909(); break;                              // M909: Print digipot/DAC current value
            case 910: M910(); break;                              // M910: Commit digipot/DAC value to external EEPROM
          #endif
        #endif
      #endif

      #if HAS_TRINAMIC
        #if ENABLED(TMC_DEBUG)
          case 122: M122(); break;
        #endif
        case 906: M906(); break;                                  // M906: Set motor current in milliamps using axis codes X, Y, Z, E
        case 911: M911(); break;                                  // M911: Report TMC2130 prewarn triggered flags
        case 912: M912(); break;                                  // M912: Clear TMC2130 prewarn triggered flags
        #if ENABLED(HYBRID_THRESHOLD)
          case 913: M913(); break;                                // M913: Set HYBRID_THRESHOLD speed.
        #endif
        #if ENABLED(SENSORLESS_HOMING)
          case 914: M914(); break;                                // M914: Set SENSORLESS_HOMING sensitivity.
        #endif
        #if ENABLED(TMC_Z_CALIBRATION)
          case 915: M915(); break;                                // M915: TMC Z axis calibration.
        #endif
      #endif

      #if HAS_MICROSTEPS
        case 350: M350(); break;                                  // M350: Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
        case 351: M351(); break;                                  // M351: Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
      #endif

      case 355: M355(); break;                                    // M355: Set case light brightness

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800: parser.debug(); break;                          // M800: GCode Parser Test for M
      #endif

      #if ENABLED(I2C_POSITION_ENCODERS)
        case 860: M860(); break;                                  // M860: Report encoder module position
        case 861: M861(); break;                                  // M861: Report encoder module status
        case 862: M862(); break;                                  // M862: Perform axis test
        case 863: M863(); break;                                  // M863: Calibrate steps/mm
        case 864: M864(); break;                                  // M864: Change module address
        case 865: M865(); break;                                  // M865: Check module firmware version
        case 866: M866(); break;                                  // M866: Report axis error count
        case 867: M867(); break;                                  // M867: Toggle error correction
        case 868: M868(); break;                                  // M868: Set error correction threshold
        case 869: M869(); break;                                  // M869: Report axis error
      #endif

      case 999: M999(); break;                                    // M999: Restart after being Stopped

      default: parser.unknown_command_error(); break;
    }
    break;

    case 'T': T(parser.codenum); break;                           // Tn: Tool Change

    default: parser.unknown_command_error();
  }

  KEEPALIVE_STATE(NOT_BUSY);

  #if ENABLED(USE_EXECUTE_COMMANDS_IMMEDIATE)
    if (!no_ok)
  #endif
      ok_to_send();
}

/**
 * Process a single command and dispatch it to its handler
 * This is called from the main loop()
 */
void GcodeSuite::process_next_command() {
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

#if ENABLED(USE_EXECUTE_COMMANDS_IMMEDIATE)
  /**
   * Run a series of commands, bypassing the command queue to allow
   * G-code "macros" to be called from within other G-code handlers.
   */
  void GcodeSuite::process_subcommands_now_P(const char *pgcode) {
    // Save the parser state
    char * const saved_cmd = parser.command_ptr;

    // Process individual commands in string
    while (pgm_read_byte_near(pgcode)) {
      // Break up string at '\n' delimiters
      const char *delim = strchr_P(pgcode, '\n');
      size_t len = delim ? delim - pgcode : strlen_P(pgcode);
      char cmd[len + 1];
      strncpy_P(cmd, pgcode, len);
      cmd[len] = '\0';
      pgcode += len;
      if (delim) pgcode++;

      // Parse the next command in the string
      parser.parse(cmd);
      process_parsed_command(true);
    }

    // Restore the parser state
    parser.parse(saved_cmd);
  }
#endif

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  /**
   * Output a "busy" message at regular intervals
   * while the machine is not accepting commands.
   */
  void GcodeSuite::host_keepalive() {
    const millis_t ms = millis();
    static millis_t next_busy_signal_ms = 0;
    if (!suspend_auto_report && host_keepalive_interval && busy_state != NOT_BUSY) {
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
