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

#include "../Marlin.h" // for idle()

uint8_t GcodeSuite::target_extruder;
millis_t GcodeSuite::previous_cmd_ms;

bool GcodeSuite::axis_relative_modes[] = AXIS_RELATIVE_MODES;

#if ENABLED(HOST_KEEPALIVE_FEATURE)
  GcodeSuite::MarlinBusyState GcodeSuite::busy_state = NOT_BUSY;
  uint8_t GcodeSuite::host_keepalive_interval = DEFAULT_KEEPALIVE_INTERVAL;
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
    if (parser.seen(axis_codes[i]))
      destination[i] = parser.value_axis_units((AxisEnum)i) + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
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
  refresh_cmd_timeout();
  time += previous_cmd_ms;
  while (PENDING(millis(), time)) idle();
}

//
// Placeholders for non-migrated codes
//
extern void gcode_G5();
extern void gcode_G12();
extern void gcode_G17();
extern void gcode_G18();
extern void gcode_G19();
extern void gcode_G20();
extern void gcode_G21();
extern void gcode_G27();
extern void gcode_G30();
extern void gcode_G31();
extern void gcode_G32();
extern void gcode_G38(bool is_38_2);
extern void gcode_G42();
extern void gcode_G92();
extern void gcode_M0_M1();
extern void gcode_M3_M4(bool is_M3);
extern void gcode_M5();
extern void gcode_M17();
extern void gcode_M18_M84();
extern void gcode_M20();
extern void gcode_M21();
extern void gcode_M22();
extern void gcode_M23();
extern void gcode_M24();
extern void gcode_M25();
extern void gcode_M26();
extern void gcode_M27();
extern void gcode_M28();
extern void gcode_M29();
extern void gcode_M30();
extern void gcode_M31();
extern void gcode_M32();
extern void gcode_M33();
extern void gcode_M34();
extern void gcode_M42();
extern void gcode_M43();
extern void gcode_M48();
extern void gcode_M75();
extern void gcode_M76();
extern void gcode_M77();
extern void gcode_M78();
extern void gcode_M80();
extern void gcode_M81();
extern void gcode_M82();
extern void gcode_M83();
extern void gcode_M85();
extern void gcode_M92();
extern void gcode_M100();
extern void gcode_M105();
extern void gcode_M106();
extern void gcode_M107();
extern void gcode_M108();
extern void gcode_M110();
extern void gcode_M111();
extern void gcode_M112();
extern void gcode_M113();
extern void gcode_M114();
extern void gcode_M115();
extern void gcode_M117();
extern void gcode_M118();
extern void gcode_M119();
extern void gcode_M120();
extern void gcode_M121();
extern void gcode_M125();
extern void gcode_M126();
extern void gcode_M127();
extern void gcode_M128();
extern void gcode_M129();
extern void gcode_M140();
extern void gcode_M145();
extern void gcode_M149();
extern void gcode_M150();
extern void gcode_M155();
extern void gcode_M163();
extern void gcode_M164();
extern void gcode_M165();
extern void gcode_M190();
extern void gcode_M201();
extern void gcode_M203();
extern void gcode_M204();
extern void gcode_M205();
extern void gcode_M206();
extern void gcode_M211();
extern void gcode_M220();
extern void gcode_M226();
extern void gcode_M240();
extern void gcode_M250();
extern void gcode_M260();
extern void gcode_M261();
extern void gcode_M280();
extern void gcode_M300();
extern void gcode_M301();
extern void gcode_M302();
extern void gcode_M304();
extern void gcode_M350();
extern void gcode_M351();
extern void gcode_M355();
extern bool gcode_M360();
extern bool gcode_M361();
extern bool gcode_M362();
extern bool gcode_M363();
extern bool gcode_M364();
extern void gcode_M380();
extern void gcode_M381();
extern void gcode_M400();
extern void gcode_M401();
extern void gcode_M402();
extern void gcode_M410();
extern void gcode_M428();
extern void gcode_M500();
extern void gcode_M501();
extern void gcode_M502();
extern void gcode_M503();
extern void gcode_M540();
extern void gcode_M600();
extern void gcode_M605();
extern void gcode_M665();
extern void gcode_M666();
extern void gcode_M702();
extern void gcode_M900();
extern void gcode_M906();
extern void gcode_M911();
extern void gcode_M912();
extern void gcode_M913();
extern void gcode_M914();
extern void gcode_M907();
extern void gcode_M908();
extern void gcode_M909();
extern void gcode_M910();
extern void gcode_M928();
extern void gcode_M999();
extern void gcode_T(uint8_t tmp_extruder);

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  extern void M100_dump_routine(const char * const title, const char *start, const char *end);
#endif

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

  KEEPALIVE_STATE(IN_HANDLER);

  // Parse the next command in the queue
  parser.parse(current_command);

  // Handle a known G, M, or T
  switch (parser.command_letter) {
    case 'G': switch (parser.codenum) {

      // G0, G1
      case 0:
      case 1:
        #if IS_SCARA
          G0_G1(parser.codenum == 0);
        #else
          G0_G1();
        #endif
        break;

      // G2, G3
      #if ENABLED(ARC_SUPPORT) && DISABLED(SCARA)
        case 2: // G2: CW ARC
        case 3: // G3: CCW ARC
          G2_G3(parser.codenum == 2);
          break;
      #endif

      // G4 Dwell
      case 4:
        G4();
        break;

      #if ENABLED(BEZIER_CURVE_SUPPORT)
        case 5: // G5: Cubic B_spline
          gcode_G5();
          break;
      #endif // BEZIER_CURVE_SUPPORT

      #if ENABLED(FWRETRACT)
        case 10: // G10: retract
          G10();
          break;
        case 11: // G11: retract_recover
          G11();
          break;
      #endif // FWRETRACT

      #if ENABLED(NOZZLE_CLEAN_FEATURE)
        case 12:
          gcode_G12(); // G12: Nozzle Clean
          break;
      #endif // NOZZLE_CLEAN_FEATURE

      #if ENABLED(CNC_WORKSPACE_PLANES)
        case 17: // G17: Select Plane XY
          gcode_G17();
          break;
        case 18: // G18: Select Plane ZX
          gcode_G18();
          break;
        case 19: // G19: Select Plane YZ
          gcode_G19();
          break;
      #endif // CNC_WORKSPACE_PLANES

      #if ENABLED(INCH_MODE_SUPPORT)
        case 20: // G20: Inch Mode
          gcode_G20();
          break;

        case 21: // G21: MM Mode
          gcode_G21();
          break;
      #endif // INCH_MODE_SUPPORT

      #if ENABLED(UBL_G26_MESH_VALIDATION)
        case 26: // G26: Mesh Validation Pattern generation
          G26();
          break;
      #endif // AUTO_BED_LEVELING_UBL

      #if ENABLED(NOZZLE_PARK_FEATURE)
        case 27: // G27: Nozzle Park
          gcode_G27();
          break;
      #endif // NOZZLE_PARK_FEATURE

      case 28: // G28: Home all axes, one at a time
        G28(false);
        break;

      #if HAS_LEVELING
        case 29: // G29 Detailed Z probe, probes the bed at 3 or more points,
                 // or provides access to the UBL System if enabled.
          G29();
          break;
      #endif // HAS_LEVELING

      #if HAS_BED_PROBE

        case 30: // G30 Single Z probe
          gcode_G30();
          break;

        #if ENABLED(Z_PROBE_SLED)

            case 31: // G31: dock the sled
              gcode_G31();
              break;

            case 32: // G32: undock the sled
              gcode_G32();
              break;

        #endif // Z_PROBE_SLED

      #endif // HAS_BED_PROBE

      #if ENABLED(DELTA_AUTO_CALIBRATION)
        case 33: // G33: Delta Auto-Calibration
          G33();
          break;
      #endif // DELTA_AUTO_CALIBRATION

      #if ENABLED(G38_PROBE_TARGET)
        case 38: // G38.2 & G38.3
          if (parser.subcode == 2 || parser.subcode == 3)
            gcode_G38(parser.subcode == 2);
          break;
      #endif

      case 90: // G90
        relative_mode = false;
        break;
      case 91: // G91
        relative_mode = true;
        break;

      case 92: // G92
        gcode_G92();
        break;

      #if HAS_MESH
        case 42:
          gcode_G42();
          break;
      #endif

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800:
          parser.debug(); // GCode Parser Test for G
          break;
      #endif
    }
    break;

    case 'M': switch (parser.codenum) {
      #if HAS_RESUME_CONTINUE
        case 0: // M0: Unconditional stop - Wait for user button press on LCD
        case 1: // M1: Conditional stop - Wait for user button press on LCD
          gcode_M0_M1();
          break;
      #endif // ULTIPANEL

      #if ENABLED(SPINDLE_LASER_ENABLE)
        case 3:
          gcode_M3_M4(true);   // M3: turn spindle/laser on, set laser/spindle power/speed, set rotation direction CW
          break;               // synchronizes with movement commands
        case 4:
          gcode_M3_M4(false);  // M4: turn spindle/laser on, set laser/spindle power/speed, set rotation direction CCW
          break;               // synchronizes with movement commands
        case 5:
          gcode_M5();     // M5 - turn spindle/laser off
          break;          // synchronizes with movement commands
      #endif
      case 17: // M17: Enable all stepper motors
        gcode_M17();
        break;

      #if ENABLED(SDSUPPORT)
        case 20: // M20: list SD card
          gcode_M20(); break;
        case 21: // M21: init SD card
          gcode_M21(); break;
        case 22: // M22: release SD card
          gcode_M22(); break;
        case 23: // M23: Select file
          gcode_M23(); break;
        case 24: // M24: Start SD print
          gcode_M24(); break;
        case 25: // M25: Pause SD print
          gcode_M25(); break;
        case 26: // M26: Set SD index
          gcode_M26(); break;
        case 27: // M27: Get SD status
          gcode_M27(); break;
        case 28: // M28: Start SD write
          gcode_M28(); break;
        case 29: // M29: Stop SD write
          gcode_M29(); break;
        case 30: // M30 <filename> Delete File
          gcode_M30(); break;
        case 32: // M32: Select file and start SD print
          gcode_M32(); break;

        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          case 33: // M33: Get the long full path to a file or folder
            gcode_M33(); break;
        #endif

        #if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)
          case 34: // M34: Set SD card sorting options
            gcode_M34(); break;
        #endif // SDCARD_SORT_ALPHA && SDSORT_GCODE

        case 928: // M928: Start SD write
          gcode_M928(); break;
      #endif // SDSUPPORT

      case 31: // M31: Report time since the start of SD print or last M109
        gcode_M31(); break;

      case 42: // M42: Change pin state
        gcode_M42(); break;

      #if ENABLED(PINS_DEBUGGING)
        case 43: // M43: Read pin state
          gcode_M43(); break;
      #endif


      #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
        case 48: // M48: Z probe repeatability test
          gcode_M48();
          break;
      #endif // Z_MIN_PROBE_REPEATABILITY_TEST

      #if ENABLED(UBL_G26_MESH_VALIDATION)
        case 49: M49(); break;    // M49: Turn on or off G26 debug flag for verbose output
      #endif

      case 75: // M75: Start print timer
        gcode_M75(); break;
      case 76: // M76: Pause print timer
        gcode_M76(); break;
      case 77: // M77: Stop print timer
        gcode_M77(); break;

      #if ENABLED(PRINTCOUNTER)
        case 78: // M78: Show print statistics
          gcode_M78(); break;
      #endif

      #if ENABLED(M100_FREE_MEMORY_WATCHER)
        case 100: // M100: Free Memory Report
          gcode_M100();
          break;
      #endif

      case 104: M104(); break;    // M104: Set hot end temperature
      case 109: M109(); break;    // M109: Wait for hotend temperature to reach target

      case 110: // M110: Set Current Line Number
        gcode_M110();
        break;

      case 111: // M111: Set debug level
        gcode_M111();
        break;

      #if DISABLED(EMERGENCY_PARSER)

        case 108: // M108: Cancel Waiting
          gcode_M108();
          break;

        case 112: // M112: Emergency Stop
          gcode_M112();
          break;

        case 410: // M410 quickstop - Abort all the planned moves.
          gcode_M410();
          break;

      #endif

      #if ENABLED(HOST_KEEPALIVE_FEATURE)
        case 113: // M113: Set Host Keepalive interval
          gcode_M113();
          break;
      #endif

      case 140: // M140: Set bed temperature
        gcode_M140();
        break;


      case 105: // M105: Report current temperature
        gcode_M105();
        KEEPALIVE_STATE(NOT_BUSY);
        return; // "ok" already printed

      #if ENABLED(AUTO_REPORT_TEMPERATURES) && (HAS_TEMP_HOTEND || HAS_TEMP_BED)
        case 155: // M155: Set temperature auto-report interval
          gcode_M155();
          break;
      #endif

      #if HAS_TEMP_BED
        case 190: // M190: Wait for bed temperature to reach target
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

      #if ENABLED(PARK_HEAD_ON_PAUSE)
        case 125: // M125: Store current position and move to filament change position
          gcode_M125(); break;
      #endif

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

      case 82: // M82: Set E axis normal mode (same as other axes)
        gcode_M82();
        break;
      case 83: // M83: Set E axis relative mode
        gcode_M83();
        break;
      case 18: // M18 => M84
      case 84: // M84: Disable all steppers or set timeout
        gcode_M18_M84();
        break;
      case 85: // M85: Set inactivity stepper shutdown timeout
        gcode_M85();
        break;
      case 92: // M92: Set the steps-per-unit for one or more axes
        gcode_M92();
        break;
      case 114: // M114: Report current position
        gcode_M114();
        break;
      case 115: // M115: Report capabilities
        gcode_M115();
        break;
      case 117: // M117: Set LCD message text, if possible
        gcode_M117();
        break;
      case 118: // M118: Display a message in the host console
        gcode_M118();
        break;
      case 119: // M119: Report endstop states
        gcode_M119();
        break;
      case 120: // M120: Enable endstops
        gcode_M120();
        break;
      case 121: // M121: Disable endstops
        gcode_M121();
        break;

      #if ENABLED(ULTIPANEL)

        case 145: // M145: Set material heatup parameters
          gcode_M145();
          break;

      #endif

      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        case 149: // M149: Set temperature units
          gcode_M149();
          break;
      #endif

      #if HAS_COLOR_LEDS

        case 150: // M150: Set Status LED Color
          gcode_M150();
          break;

      #endif // HAS_COLOR_LEDS

      #if ENABLED(MIXING_EXTRUDER)
        case 163: // M163: Set a component weight for mixing extruder
          gcode_M163();
          break;
        #if MIXING_VIRTUAL_TOOLS > 1
          case 164: // M164: Save current mix as a virtual extruder
            gcode_M164();
            break;
        #endif
        #if ENABLED(DIRECT_MIXING_IN_G1)
          case 165: // M165: Set multiple mix weights
            gcode_M165();
            break;
        #endif
      #endif

      case 200: // M200: Set filament diameter, E to cubic units
        M200();
        break;
      case 201: // M201: Set max acceleration for print moves (units/s^2)
        gcode_M201();
        break;
      #if 0 // Not used for Sprinter/grbl gen6
        case 202: // M202
          gcode_M202();
          break;
      #endif
      case 203: // M203: Set max feedrate (units/sec)
        gcode_M203();
        break;
      case 204: // M204: Set acceleration
        gcode_M204();
        break;
      case 205: // M205: Set advanced settings
        gcode_M205();
        break;

      #if HAS_M206_COMMAND
        case 206: // M206: Set home offsets
          gcode_M206();
          break;
      #endif

      #if ENABLED(DELTA)
        case 665: // M665: Set delta configurations
          gcode_M665();
          break;
      #endif

      #if ENABLED(DELTA) || ENABLED(Z_DUAL_ENDSTOPS)
        case 666: // M666: Set delta or dual endstop adjustment
          gcode_M666();
          break;
      #endif

      #if ENABLED(FWRETRACT)
        case 207: M207(); break;  // M207: Set Retract Length, Feedrate, and Z lift
        case 208: M208(); break;  // M208: Set Recover (unretract) Additional Length and Feedrate
        case 209: if (MIN_AUTORETRACT <= MAX_AUTORETRACT) M209(); break;  // M209: Turn Automatic Retract Detection on/off
      #endif

      case 211: // M211: Enable, Disable, and/or Report software endstops
        gcode_M211();
        break;

      #if HOTENDS > 1
        case 218: // M218: Set a tool offset
          M218();
          break;
      #endif

      case 220: // M220: Set Feedrate Percentage: S<percent> ("FR" on your LCD)
        gcode_M220();
        break;

      case 221: // M221: Set Flow Percentage
        M221();
        break;

      case 226: // M226: Wait until a pin reaches a state
        gcode_M226();
        break;

      #if HAS_SERVOS
        case 280: // M280: Set servo position absolute
          gcode_M280();
          break;
      #endif // HAS_SERVOS

      #if HAS_BUZZER
        case 300: // M300: Play beep tone
          gcode_M300();
          break;
      #endif // HAS_BUZZER

      #if ENABLED(PIDTEMP)
        case 301: // M301: Set hotend PID parameters
          gcode_M301();
          break;
      #endif // PIDTEMP

      #if ENABLED(PIDTEMPBED)
        case 304: // M304: Set bed PID parameters
          gcode_M304();
          break;
      #endif // PIDTEMPBED

      #if defined(CHDK) || HAS_PHOTOGRAPH
        case 240: // M240: Trigger a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
          gcode_M240();
          break;
      #endif // CHDK || PHOTOGRAPH_PIN

      #if HAS_LCD_CONTRAST
        case 250: // M250: Set LCD contrast
          gcode_M250();
          break;
      #endif // HAS_LCD_CONTRAST

      #if ENABLED(EXPERIMENTAL_I2CBUS)

        case 260: // M260: Send data to an i2c slave
          gcode_M260();
          break;

        case 261: // M261: Request data from an i2c slave
          gcode_M261();
          break;

      #endif // EXPERIMENTAL_I2CBUS

      #if ENABLED(PREVENT_COLD_EXTRUSION)
        case 302: // M302: Allow cold extrudes (set the minimum extrude temperature)
          gcode_M302();
          break;
      #endif // PREVENT_COLD_EXTRUSION

      case 303: // M303: PID autotune
        M303();
        break;

      #if ENABLED(MORGAN_SCARA)
        case 360:  // M360: SCARA Theta pos1
          if (gcode_M360()) return;
          break;
        case 361:  // M361: SCARA Theta pos2
          if (gcode_M361()) return;
          break;
        case 362:  // M362: SCARA Psi pos1
          if (gcode_M362()) return;
          break;
        case 363:  // M363: SCARA Psi pos2
          if (gcode_M363()) return;
          break;
        case 364:  // M364: SCARA Psi pos3 (90 deg to Theta)
          if (gcode_M364()) return;
          break;
      #endif // SCARA

      #if ENABLED(EXT_SOLENOID)
        case 380: // M380: Activate solenoid on active extruder
          gcode_M380();
          break;
        case 381: // M381: Disable all solenoids
          gcode_M381();
          break;
      #endif

      case 400: // M400: Finish all moves
        gcode_M400();
        break;

      #if HAS_BED_PROBE
        case 401: // M401: Deploy probe
          gcode_M401();
          break;
        case 402: // M402: Stow probe
          gcode_M402();
          break;
      #endif // HAS_BED_PROBE

      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        case 404:  // M404: Enter the nominal filament width (3mm, 1.75mm ) N<3.0> or display nominal filament width
          M404();
          break;
        case 405:  // M405: Turn on filament sensor for control
          M405();
          break;
        case 406:  // M406: Turn off filament sensor for control
          M406();
          break;
        case 407:   // M407: Display measured filament diameter
          M407();
          break;
      #endif // FILAMENT_WIDTH_SENSOR

      #if HAS_LEVELING
        case 420: // M420: Enable/Disable Bed Leveling
          M420();
          break;
      #endif

      #if HAS_MESH
        case 421: // M421: Set a Mesh Bed Leveling Z coordinate
          M421();
          break;
      #endif

      #if HAS_M206_COMMAND
        case 428: // M428: Apply current_position to home_offset
          gcode_M428();
          break;
      #endif

      case 500: // M500: Store settings in EEPROM
        gcode_M500();
        break;
      case 501: // M501: Read settings from EEPROM
        gcode_M501();
        break;
      case 502: // M502: Revert to default settings
        gcode_M502();
        break;

      #if DISABLED(DISABLE_M503)
        case 503: // M503: print settings currently in memory
          gcode_M503();
          break;
      #endif

      #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
        case 540: // M540: Set abort on endstop hit for SD printing
          gcode_M540();
          break;
      #endif

      #if HAS_BED_PROBE
        case 851: // M851: Set Z Probe Z Offset
          M851();
          break;
      #endif // HAS_BED_PROBE

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case 600: // M600: Pause for filament change
          gcode_M600();
          break;
      #endif // ADVANCED_PAUSE_FEATURE

      #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
        case 605: // M605: Set Dual X Carriage movement mode
          gcode_M605();
          break;
      #endif // DUAL_X_CARRIAGE

      #if ENABLED(MK2_MULTIPLEXER)
        case 702: // M702: Unload all extruders
          gcode_M702();
          break;
      #endif

      #if ENABLED(LIN_ADVANCE)
        case 900: // M900: Set advance K factor.
          gcode_M900();
          break;
      #endif

      #if ENABLED(HAVE_TMC2130)
        case 906: // M906: Set motor current in milliamps using axis codes X, Y, Z, E
          gcode_M906();
          break;
      #endif

      case 907: // M907: Set digital trimpot motor current using axis codes.
        gcode_M907();
        break;

      #if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)

        case 908: // M908: Control digital trimpot directly.
          gcode_M908();
          break;

        #if ENABLED(DAC_STEPPER_CURRENT) // As with Printrbot RevF

          case 909: // M909: Print digipot/DAC current value
            gcode_M909();
            break;

          case 910: // M910: Commit digipot/DAC value to external EEPROM
            gcode_M910();
            break;

        #endif

      #endif // HAS_DIGIPOTSS || DAC_STEPPER_CURRENT

      #if ENABLED(HAVE_TMC2130)
        case 911: // M911: Report TMC2130 prewarn triggered flags
          gcode_M911();
          break;

        case 912: // M911: Clear TMC2130 prewarn triggered flags
          gcode_M912();
          break;

        #if ENABLED(HYBRID_THRESHOLD)
          case 913: // M913: Set HYBRID_THRESHOLD speed.
            gcode_M913();
            break;
        #endif

        #if ENABLED(SENSORLESS_HOMING)
          case 914: // M914: Set SENSORLESS_HOMING sensitivity.
            gcode_M914();
            break;
        #endif
      #endif

      #if HAS_MICROSTEPS

        case 350: // M350: Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
          gcode_M350();
          break;

        case 351: // M351: Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
          gcode_M351();
          break;

      #endif // HAS_MICROSTEPS

      case 355: // M355 set case light brightness
        gcode_M355();
        break;

      #if ENABLED(DEBUG_GCODE_PARSER)
        case 800:
          parser.debug(); // GCode Parser Test for M
          break;
      #endif

      #if ENABLED(I2C_POSITION_ENCODERS)
        case 860: M860(); break; // M860: Report encoder module position
        case 861: M861(); break; // M861: Report encoder module status
        case 862: M862(); break; // M862: Perform axis test
        case 863: M863(); break; // M863: Calibrate steps/mm
        case 864: M864(); break; // M864: Change module address
        case 865: M865(); break; // M865: Check module firmware version
        case 866: M866(); break; // M866: Report axis error count
        case 867: M867(); break; // M867: Toggle error correction
        case 868: M868(); break; // M868: Set error correction threshold
        case 869: M869(); break; // M869: Report axis error
      #endif

      case 999: // M999: Restart after being Stopped
        gcode_M999();
        break;
    }
    break;

    case 'T':
      gcode_T(parser.codenum);
      break;

    default: parser.unknown_command_error();
  }

  KEEPALIVE_STATE(NOT_BUSY);

  ok_to_send();
}

#if ENABLED(HOST_KEEPALIVE_FEATURE)

  /**
   * Output a "busy" message at regular intervals
   * while the machine is not accepting commands.
   */
  void GcodeSuite::host_keepalive() {
    const millis_t ms = millis();
    static millis_t next_busy_signal_ms = 0;
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
