/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#if ENABLED(WIFI_CUSTOM_COMMAND)
  extern bool wifi_custom_command(char * const command_ptr);
#endif

#include "parser.h"
#include "queue.h"
#include "../module/motion.h"

#if ENABLED(PRINTCOUNTER)
  #include "../module/printcounter.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../feature/host_actions.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../sd/cardreader.h"
  #include "../feature/power_loss_recovery.h"
#endif

#if ENABLED(CANCEL_OBJECTS)
  #include "../feature/cancel_object.h"
#endif

#include "../MarlinCore.h" // for idle() and suspend_auto_report

millis_t GcodeSuite::previous_move_ms;

// Relative motion mode for each logical axis
static constexpr xyze_bool_t ar_init = AXIS_RELATIVE_MODES;
uint8_t GcodeSuite::axis_relative = (
    (ar_init.x ? _BV(REL_X) : 0)
  | (ar_init.y ? _BV(REL_Y) : 0)
  | (ar_init.z ? _BV(REL_Z) : 0)
  | (ar_init.e ? _BV(REL_E) : 0)
);

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  GcodeSuite::MarlinBusyState GcodeSuite::busy_state = NOT_BUSY;
  uint8_t GcodeSuite::host_keepalive_interval = DEFAULT_KEEPALIVE_INTERVAL;
#endif

#if ENABLED(CNC_WORKSPACE_PLANES)
  GcodeSuite::WorkspacePlane GcodeSuite::workspace_plane = PLANE_XY;
#endif

#if ENABLED(CNC_COORDINATE_SYSTEMS)
  int8_t GcodeSuite::active_coordinate_system = -1; // machine space
  xyz_pos_t GcodeSuite::coordinate_system[MAX_COORDINATE_SYSTEMS];
#endif

/**
 * Get the target extruder from the T parameter or the active_extruder
 * Return -1 if the T parameter is out of range
 */
int8_t GcodeSuite::get_target_extruder_from_command() {
  if (parser.seenval('T')) {
    const int8_t e = parser.value_byte();
    if (e < EXTRUDERS) return e;
    SERIAL_ECHO_START();
    SERIAL_CHAR('M'); SERIAL_ECHO(parser.codenum);
    SERIAL_ECHOLNPAIR(" " MSG_INVALID_EXTRUDER " ", int(e));
    return -1;
  }
  return active_extruder;
}

/**
 * Get the target e stepper from the T parameter
 * Return -1 if the T parameter is out of range or unspecified
 */
int8_t GcodeSuite::get_target_e_stepper_from_command() {
  const int8_t e = parser.intval('T', -1);
  if (WITHIN(e, 0, E_STEPPERS - 1)) return e;

  SERIAL_ECHO_START();
  SERIAL_CHAR('M'); SERIAL_ECHO(parser.codenum);
  if (e == -1)
    SERIAL_ECHOLNPGM(" " MSG_E_STEPPER_NOT_SPECIFIED);
  else
    SERIAL_ECHOLNPAIR(" " MSG_INVALID_E_STEPPER " ", int(e));
  return -1;
}

/**
 * Set XYZE destination and feedrate from the current GCode command
 *
 *  - Set destination from included axis codes
 *  - Set to current for missing axis codes
 *  - Set the feedrate, if included
 */
void GcodeSuite::get_destination_from_command() {
  xyze_bool_t seen = { false, false, false, false };

  #if ENABLED(CANCEL_OBJECTS)
    const bool &skip_move = cancelable.skipping;
  #else
    constexpr bool skip_move = false;
  #endif

  // Get new XYZ position, whether absolute or relative
  LOOP_XYZ(i) {
    if ( (seen[i] = parser.seenval(axis_codes[i])) ) {
      const float v = parser.value_axis_units((AxisEnum)i);
      if (skip_move)
        destination[i] = current_position[i];
      else
        destination[i] = axis_is_relative(AxisEnum(i)) ? current_position[i] + v : LOGICAL_TO_NATIVE(v, i);
    }
    else
      destination[i] = current_position[i];
  }

  // Get new E position, whether absolute or relative
  if ( (seen.e = parser.seenval('E')) ) {
    const float v = parser.value_axis_units(E_AXIS);
    destination.e = axis_is_relative(E_AXIS) ? current_position.e + v : v;
  }
  else
    destination.e = current_position.e;

  #if ENABLED(POWER_LOSS_RECOVERY) && !PIN_EXISTS(POWER_LOSS)
    // Only update power loss recovery on moves with E
    if (recovery.enabled && IS_SD_PRINTING() && seen.e && (seen.x || seen.y))
      recovery.save();
  #endif

  if (parser.linearval('F') > 0)
    feedrate_mm_s = parser.value_feedrate();

  #if ENABLED(PRINTCOUNTER)
    if (!DEBUGGING(DRYRUN) && !skip_move)
      print_job_timer.incFilamentUsed(destination.e - current_position.e);
  #endif

  // Get ABCDHI mixing factors
  #if BOTH(MIXING_EXTRUDER, DIRECT_MIXING_IN_G1)
    M165();
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

  #ifndef G29_MAX_RETRIES
    #define G29_MAX_RETRIES 0
  #endif

  void GcodeSuite::G29_with_retry() {
    uint8_t retries = G29_MAX_RETRIES;
    while (G29()) { // G29 should return true for failed probes ONLY
      if (retries--) event_probe_recover();
      else {
        event_probe_failure();
        return;
      }
    }

    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_action_prompt_end();
    #endif

    #ifdef G29_SUCCESS_COMMANDS
      process_subcommands_now_P(PSTR(G29_SUCCESS_COMMANDS));
    #endif
  }

#endif // HAS_LEVELING && G29_RETRY_AND_RECOVER

//
// Placeholders for non-migrated codes
//
#if ENABLED(M100_FREE_MEMORY_WATCHER)
  extern void M100_dump_routine(PGM_P const title, const char * const start, const char * const end);
#endif

/**
 * Process the parsed command and dispatch it to its handler
 */
void GcodeSuite::process_parsed_command(const bool no_ok/*=false*/) {
  KEEPALIVE_STATE(IN_HANDLER);

  // Handle a known G, M, or T
  switch (parser.command_letter) {
    case 'G': switch (parser.codenum) {

      case 0: case 1: G0_G1(                                      // G0: Fast Move, G1: Linear Move
                        #if IS_SCARA || defined(G0_FEEDRATE)
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
      #else
        case 21: NOOP; break;                                     // No error on unknown G21
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

      #if ENABLED(Z_STEPPER_AUTO_ALIGN)
        case 34: G34(); break;                                    // G34: Z Stepper automatic alignment using probe
      #endif

      #if ENABLED(G38_PROBE_TARGET)
        case 38:                                                  // G38.2, G38.3: Probe towards target
          if (WITHIN(parser.subcode, 2,
            #if ENABLED(G38_PROBE_AWAY)
              5
            #else
              3
            #endif
          )) G38(parser.subcode);                                 // G38.4, G38.5: Probe away from target
          break;
      #endif

      #if ENABLED(CNC_COORDINATE_SYSTEMS)
        case 53: G53(); break;
        case 54: G54(); break;
        case 55: G55(); break;
        case 56: G56(); break;
        case 57: G57(); break;
        case 58: G58(); break;
        case 59: G59(); break;
      #endif

      #if ENABLED(GCODE_MOTION_MODES)
        case 80: G80(); break;                                    // G80: Reset the current motion mode
      #endif

      case 90: set_relative_mode(false); break;                   // G90: Absolute Mode
      case 91: set_relative_mode(true);  break;                   // G91: Relative Mode

      case 92: G92(); break;                                      // G92: Set current axis position(s)

      #if HAS_MESH
        case 42: G42(); break;                                    // G42: Coordinated move to a mesh point
      #endif

      #if ENABLED(CALIBRATION_GCODE)
        case 425: G425(); break;                                  // G425: Perform calibration with calibration cube
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

      #if HAS_CUTTER
        case 3: M3_M4(false); break;                              // M3: Turn ON Laser | Spindle (clockwise), set Power | Speed
        case 4: M3_M4(true ); break;                              // M4: Turn ON Laser | Spindle (counter-clockwise), set Power | Speed
        case 5: M5(); break;                                      // M5: Turn OFF Laser | Spindle
      #endif

      #if ENABLED(COOLANT_CONTROL)
        #if ENABLED(COOLANT_MIST)
          case 7: M7(); break;                                    // M7: Mist coolant ON
        #endif
        #if ENABLED(COOLANT_FLOOD)
          case 8: M8(); break;                                    // M8: Flood coolant ON
        #endif
        case 9: M9(); break;                                      // M9: Coolant OFF
      #endif

      #if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
        case 12: M12(); break;                                    // M12: Synchronize and optionally force a CLC set
      #endif

      #if ENABLED(EXPECTED_PRINTER_CHECK)
        case 16: M16(); break;                                    // M16: Expected printer check
      #endif

      case 17: M17(); break;                                      // M17: Enable all stepper motors

      #if ENABLED(SDSUPPORT)
        case 20: M20(); break;                                    // M20: List SD card
        case 21: M21(); break;                                    // M21: Init SD card
        case 22: M22(); break;                                    // M22: Release SD card
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

        #if BOTH(SDCARD_SORT_ALPHA, SDSORT_GCODE)
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

      #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
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

      #if EXTRUDERS
        case 104: M104(); break;                                  // M104: Set hot end temperature
        case 109: M109(); break;                                  // M109: Wait for hotend temperature to reach target
      #endif

      case 105: M105(); return;                                   // M105: Report Temperatures (and say "ok")

      #if FAN_COUNT > 0
        case 106: M106(); break;                                  // M106: Fan On
        case 107: M107(); break;                                  // M107: Fan Off
      #endif

      case 110: M110(); break;                                    // M110: Set Current Line Number
      case 111: M111(); break;                                    // M111: Set debug level

      #if DISABLED(EMERGENCY_PARSER)
        case 108: M108(); break;                                  // M108: Cancel Waiting
        case 112: M112(); break;                                  // M112: Full Shutdown
        case 410: M410(); break;                                  // M410: Quickstop - Abort all the planned moves.
        #if ENABLED(HOST_PROMPT_SUPPORT)
          case 876: M876(); break;                                // M876: Handle Host prompt responses
        #endif
      #else
        case 108: case 112: case 410:
        #if ENABLED(HOST_PROMPT_SUPPORT)
          case 876:
        #endif
        break;
      #endif

      #if ENABLED(HOST_KEEPALIVE_FEATURE)
        case 113: M113(); break;                                  // M113: Set Host Keepalive interval
      #endif

      #if HAS_HEATED_BED
        case 140: M140(); break;                                  // M140: Set bed temperature
        case 190: M190(); break;                                  // M190: Wait for bed temperature to reach target
      #endif

      #if HAS_HEATED_CHAMBER
        case 141: M141(); break;                                  // M141: Set chamber temperature
        case 191: M191(); break;                                  // M191: Wait for chamber temperature to reach target
      #endif

      #if ENABLED(AUTO_REPORT_TEMPERATURES) && HAS_TEMP_SENSOR
        case 155: M155(); break;                                  // M155: Set temperature auto-report interval
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

      #if ENABLED(PSU_CONTROL)
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

      #if HOTENDS && HAS_LCD_MENU
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
        case 164: M164(); break;                                  // M164: Save current mix as a virtual extruder
        #if ENABLED(DIRECT_MIXING_IN_G1)
          case 165: M165(); break;                                // M165: Set multiple mix weights
        #endif
        #if ENABLED(GRADIENT_MIX)
          case 166: M166(); break;                                // M166: Set Gradient Mix
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

      #if ANY(DELTA, X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS, Z_DUAL_ENDSTOPS)
        case 666: M666(); break;                                  // M666: Set delta or dual endstop adjustment
      #endif

      #if ENABLED(FWRETRACT)
        case 207: M207(); break;                                  // M207: Set Retract Length, Feedrate, and Z lift
        case 208: M208(); break;                                  // M208: Set Recover (unretract) Additional Length and Feedrate
        #if ENABLED(FWRETRACT_AUTORETRACT)
          case 209:
            if (MIN_AUTORETRACT <= MAX_AUTORETRACT) M209();       // M209: Turn Automatic Retract Detection on/off
            break;
        #endif
      #endif

      #if HAS_SOFTWARE_ENDSTOPS
        case 211: M211(); break;                                  // M211: Enable, Disable, and/or Report software endstops
      #endif

      #if EXTRUDERS > 1
        case 217: M217(); break;                                  // M217: Set filament swap parameters
      #endif

      #if HAS_HOTEND_OFFSET
        case 218: M218(); break;                                  // M218: Set a tool offset
      #endif

      case 220: M220(); break;                                    // M220: Set Feedrate Percentage: S<percent> ("FR" on your LCD)

      #if EXTRUDERS
        case 221: M221(); break;                                  // M221: Set Flow Percentage
      #endif

      case 226: M226(); break;                                    // M226: Wait until a pin reaches a state

      #if HAS_SERVOS
        case 280: M280(); break;                                  // M280: Set servo position absolute
        #if ENABLED(EDITABLE_SERVO_ANGLES)
          case 281: M281(); break;                                // M281: Set servo angles
        #endif
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

      #if ENABLED(PHOTO_GCODE)
        case 240: M240(); break;                                  // M240: Trigger a camera
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

      #if HAS_PID_HEATING
        case 303: M303(); break;                                  // M303: PID autotune
      #endif

      #if HAS_USER_THERMISTORS
        case 305: M305(); break;                                  // M305: Set user thermistor parameters
      #endif

      #if ENABLED(MORGAN_SCARA)
        case 360: if (M360()) return; break;                      // M360: SCARA Theta pos1
        case 361: if (M361()) return; break;                      // M361: SCARA Theta pos2
        case 362: if (M362()) return; break;                      // M362: SCARA Psi pos1
        case 363: if (M363()) return; break;                      // M363: SCARA Psi pos2
        case 364: if (M364()) return; break;                      // M364: SCARA Psi pos3 (90 deg to Theta)
      #endif

      #if EITHER(EXT_SOLENOID, MANUAL_SOLENOID_CONTROL)
        case 380: M380(); break;                                  // M380: Activate solenoid on active (or specified) extruder
        case 381: M381(); break;                                  // M381: Disable all solenoids or, if MANUAL_SOLENOID_CONTROL, active (or specified) solenoid
      #endif

      case 400: M400(); break;                                    // M400: Finish all moves

      #if HAS_BED_PROBE
        case 401: M401(); break;                                  // M401: Deploy probe
        case 402: M402(); break;                                  // M402: Stow probe
      #endif

      #if ENABLED(PRUSA_MMU2)
        case 403: M403(); break;
      #endif

      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        case 404: M404(); break;                                  // M404: Enter the nominal filament width (3mm, 1.75mm ) N<3.0> or display nominal filament width
        case 405: M405(); break;                                  // M405: Turn on filament sensor for control
        case 406: M406(); break;                                  // M406: Turn off filament sensor for control
        case 407: M407(); break;                                  // M407: Display measured filament diameter
      #endif

      #if HAS_FILAMENT_SENSOR
        case 412: M412(); break;                                  // M412: Enable/Disable filament runout detection
      #endif

      #if HAS_LEVELING
        case 420: M420(); break;                                  // M420: Enable/Disable Bed Leveling
      #endif

      #if HAS_MESH
        case 421: M421(); break;                                  // M421: Set a Mesh Bed Leveling Z coordinate
      #endif

      #if ENABLED(BACKLASH_GCODE)
        case 425: M425(); break;                                  // M425: Tune backlash compensation
      #endif

      #if HAS_M206_COMMAND
        case 428: M428(); break;                                  // M428: Apply current_position to home_offset
      #endif

      #if ENABLED(CANCEL_OBJECTS)
        case 486: M486(); break;                                  // M486: Identify and cancel objects
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

      #if ENABLED(SDSUPPORT)
        case 524: M524(); break;                                   // M524: Abort the current SD print job
      #endif

      #if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)
        case 540: M540(); break;                                  // M540: Set abort on endstop hit for SD printing
      #endif

      #if ENABLED(BAUD_RATE_GCODE)
        case 575: M575(); break;                                  // M575: Set serial baudrate
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

      #if HAS_DUPLICATION_MODE
        case 605: M605(); break;                                  // M605: Set Dual X Carriage movement mode
      #endif

      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        case 701: M701(); break;                                  // M701: Load Filament
        case 702: M702(); break;                                  // M702: Unload Filament
      #endif

      #if ENABLED(MAX7219_GCODE)
        case 7219: M7219(); break;                                // M7219: Set LEDs, columns, and rows
      #endif

      #if ENABLED(GCODE_MACROS)
        case 810: case 811: case 812: case 813: case 814:
        case 815: case 816: case 817: case 818: case 819:
        M810_819(); break;                                        // M810-M819: Define/execute G-code macro
      #endif

      #if ENABLED(LIN_ADVANCE)
        case 900: M900(); break;                                  // M900: Set advance K factor.
      #endif

      #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM || EITHER(DIGIPOT_I2C, DAC_STEPPER_CURRENT)
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
        case 122: M122(); break;                                  // M122: Report driver configuration and status
        case 906: M906(); break;                                  // M906: Set motor current in milliamps using axis codes X, Y, Z, E
        #if HAS_STEALTHCHOP
          case 569: M569(); break;                                // M569: Enable stealthChop on an axis.
        #endif
        #if ENABLED(MONITOR_DRIVER_STATUS)
          case 911: M911(); break;                                // M911: Report TMC2130 prewarn triggered flags
          case 912: M912(); break;                                // M912: Clear TMC2130 prewarn triggered flags
        #endif
        #if ENABLED(HYBRID_THRESHOLD)
          case 913: M913(); break;                                // M913: Set HYBRID_THRESHOLD speed.
        #endif
        #if USE_SENSORLESS
          case 914: M914(); break;                                // M914: Set StallGuard sensitivity.
        #endif
      #endif

      #if HAS_DRIVER(L6470)
        case 122: M122(); break;                                   // M122: Report status
        case 906: M906(); break;                                   // M906: Set or get motor drive level
        case 916: M916(); break;                                   // M916: L6470 tuning: Increase drive level until thermal warning
        case 917: M917(); break;                                   // M917: L6470 tuning: Find minimum current thresholds
        case 918: M918(); break;                                   // M918: L6470 tuning: Increase speed until max or error
      #endif

      #if HAS_MICROSTEPS
        case 350: M350(); break;                                  // M350: Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
        case 351: M351(); break;                                  // M351: Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
      #endif

      #if HAS_CASE_LIGHT
        case 355: M355(); break;                                  // M355: Set case light brightness
      #endif

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

      #if ENABLED(MAGNETIC_PARKING_EXTRUDER)
        case 951: M951(); break;                                  // M951: Set Magnetic Parking Extruder parameters
      #endif

      #if ENABLED(Z_STEPPER_AUTO_ALIGN)
        case 422: M422(); break;                                  // M422: Set Z Stepper automatic alignment position using probe
      #endif

      #if ENABLED(PLATFORM_M997_SUPPORT)
        case 997: M997(); break;                                  // M997: Perform in-application firmware update
      #endif

      case 999: M999(); break;                                    // M999: Restart after being Stopped

      #if ENABLED(POWER_LOSS_RECOVERY)
        case 413: M413(); break;                                  // M413: Enable/disable/query Power-Loss Recovery
        case 1000: M1000(); break;                                // M1000: Resume from power-loss
      #endif

      default: parser.unknown_command_error(); break;
    }
    break;

    case 'T': T(parser.codenum); break;                           // Tn: Tool Change

    default:
      #if ENABLED(WIFI_CUSTOM_COMMAND)
        if (wifi_custom_command(parser.command_ptr)) break;
      #endif
      parser.unknown_command_error();
  }

  if (!no_ok) queue.ok_to_send();
}

/**
 * Process a single command and dispatch it to its handler
 * This is called from the main loop()
 */
void GcodeSuite::process_next_command() {
  char * const current_command = queue.command_buffer[queue.index_r];

  PORT_REDIRECT(queue.port[queue.index_r]);

  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.queue_index_r = queue.index_r;
  #endif

  if (DEBUGGING(ECHO)) {
    SERIAL_ECHO_START();
    SERIAL_ECHOLN(current_command);
    #if ENABLED(M100_FREE_MEMORY_DUMPER)
      SERIAL_ECHOPAIR("slot:", queue.index_r);
      M100_dump_routine(PSTR("   Command Queue:"), &queue.command_buffer[0][0], &queue.command_buffer[BUFSIZE - 1][MAX_CMD_SIZE - 1]);
    #endif
  }

  // Parse the next command in the queue
  parser.parse(current_command);
  process_parsed_command();
}

/**
 * Run a series of commands, bypassing the command queue to allow
 * G-code "macros" to be called from within other G-code handlers.
 */

void GcodeSuite::process_subcommands_now_P(PGM_P pgcode) {
  char * const saved_cmd = parser.command_ptr;        // Save the parser state
  for (;;) {
    PGM_P const delim = strchr_P(pgcode, '\n');       // Get address of next newline
    const size_t len = delim ? delim - pgcode : strlen_P(pgcode); // Get the command length
    char cmd[len + 1];                                // Allocate a stack buffer
    strncpy_P(cmd, pgcode, len);                      // Copy the command to the stack
    cmd[len] = '\0';                                  // End with a nul
    parser.parse(cmd);                                // Parse the command
    process_parsed_command(true);                     // Process it
    if (!delim) break;                                // Last command?
    pgcode = delim + 1;                               // Get the next command
  }
  parser.parse(saved_cmd);                            // Restore the parser state
}

void GcodeSuite::process_subcommands_now(char * gcode) {
  char * const saved_cmd = parser.command_ptr;        // Save the parser state
  for (;;) {
    char * const delim = strchr(gcode, '\n');         // Get address of next newline
    if (delim) *delim = '\0';                         // Replace with nul
    parser.parse(gcode);                              // Parse the current command
    process_parsed_command(true);                     // Process it
    if (!delim) break;                                // Last command?
    gcode = delim + 1;                                // Get the next command
  }
  parser.parse(saved_cmd);                            // Restore the parser state
}

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
          SERIAL_ECHO_MSG(MSG_BUSY_PROCESSING);
          break;
        case PAUSED_FOR_USER:
          SERIAL_ECHO_MSG(MSG_BUSY_PAUSED_FOR_USER);
          break;
        case PAUSED_FOR_INPUT:
          SERIAL_ECHO_MSG(MSG_BUSY_PAUSED_FOR_INPUT);
          break;
        default:
          break;
      }
    }
    next_busy_signal_ms = ms + host_keepalive_interval * 1000UL;
  }

#endif // HOST_KEEPALIVE_FEATURE
