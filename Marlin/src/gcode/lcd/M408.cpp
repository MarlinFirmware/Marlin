/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(PANELDUE)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/printcounter.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../lcd/marlinui.h"

#if ENABLED(BABYSTEPPING)
  #include "../../feature/babystep.h"
#endif

// Print a JSON key (comma-prefixed after first field; we always start with '{')
// Usage: serial_json_key("foo") => ,"foo":
inline void serial_json_key(FSTR_P const fkey) {
  SERIAL_ECHO(C(','), C('"'), fkey, F("\":"));
}

// Print a named float array, e.g.  ,"pos":[1.234,5.678,0.000]
inline void serial_json_float_array(FSTR_P const fname, const float *vals, const uint8_t n, const uint8_t dp=2) {
  serial_json_key(fname);
  SERIAL_CHAR('[');
  for (uint8_t i = 0; i < n; i++) {
    if (i) SERIAL_CHAR(',');
    SERIAL_ECHO(p_float_t(vals[i], dp));
  }
  SERIAL_CHAR(']');
}

// Print a named integer array, e.g.  ,"homed":[1,1,0]
inline void serial_json_int_array(FSTR_P const fname, const int *vals, const uint8_t n) {
  serial_json_key(fname);
  SERIAL_CHAR('[');
  for (uint8_t i = 0; i < n; i++) {
    if (i) SERIAL_CHAR(',');
    SERIAL_ECHO(vals[i]);
  }
  SERIAL_CHAR(']');
}

/**
 * M408: Report machine state in JSON format for PanelDue
 *
 *  S<type> - Response type (0=short, 1=long, 2=+print times, 3=+static config)
 *  R<seq>  - Last received sequence number (not used, ignored)
 *
 * The JSON fields follow the RepRapFirmware "legacy status response" protocol
 * as consumed by PanelDue firmware. Unsupported fields are omitted or set to
 * safe sentinel values.
 *
 * Always sent (type 0+):
 *   status, heaters, active, standby, hstat,
 *   pos, machine, extr, sfactor, efactor, tool,
 *   probe, fanPercent, fanRPM, homed, babystep, msgBox.mode
 *
 * Type 1+ (reserved, currently same as 0)
 *
 * Type 2+: fraction_printed, timesLeft
 *
 * Type 3+: geometry, axes, totalAxes, axisNames, volumes, numTools,
 *           myName, firmwareName
 */
void GcodeSuite::M408() {
  const uint8_t type = parser.byteval('S', 0);

  // ---- status character
  // PanelDue legacy protocol:
  //   'I' idle, 'P' printing, 'S' stopped/halted, 'A' paused,
  //   'C' running config file, 'B' busy (homing, probing, etc.)
  char status_ch = 'I';
  #if HAS_MEDIA
    if (card.isStillPrinting())
      status_ch = card.isPaused() ? 'A' : 'P';
  #endif

  SERIAL_ECHO(C('{'), F("\"status\":\""), C(status_ch), C('"'));

  // ---- heaters: [bed, e0, e1, ...]
  {
    float vals[1 + HOTENDS];
    vals[0] = TERN0(HAS_HEATED_BED, thermalManager.degBed());
    HOTEND_LOOP() vals[e + 1] = thermalManager.degHotend(e);
    serial_json_float_array(F("heaters"), vals, 1 + HOTENDS, 1);
  }

  // ---- active: [bed target, e0 target, ...]
  {
    float vals[1 + HOTENDS];
    vals[0] = TERN0(HAS_HEATED_BED, thermalManager.degTargetBed());
    HOTEND_LOOP() vals[e + 1] = thermalManager.degTargetHotend(e);
    serial_json_float_array(F("active"), vals, 1 + HOTENDS, 1);

    // ---- standby: Marlin has no tool standby temps; mirror active
    serial_json_float_array(F("standby"), vals, 1 + HOTENDS, 1);
  }

  // ---- hstat: heater status 0=off 1=standby 2=active 3=fault
  {
    serial_json_key(F("hstat"));
    SERIAL_CHAR('[', TERN_(HAS_HEATED_BED, thermalManager.degTargetBed() > 0 ? '2' :) '0');
    HOTEND_LOOP() {
      SERIAL_CHAR(',', thermalManager.degTargetHotend(e) > 0 ? '2' : '0');
    }
    SERIAL_CHAR(']');
  }

  // ---- pos: logical (workspace) tool coordinates
  {
    float vals[NUM_AXES];
    LOOP_NUM_AXES(i) vals[i] = motion.position[i];
    serial_json_float_array(F("pos"), vals, NUM_AXES, 3);
  }

  // ---- machine: native (no workspace offset) coordinates
  {
    float vals[NUM_AXES];
    LOOP_NUM_AXES(i)
      vals[i] = motion.logical_to_native(motion.position[i], AxisEnum(i));
    serial_json_float_array(F("machine"), vals, NUM_AXES, 3);
  }

  // ---- extr: extruder positions
  {
    float vals[EXTRUDERS];
    EXTRUDER_LOOP() vals[e] = motion.position.e;
    serial_json_float_array(F("extr"), vals, EXTRUDERS, 3);
  }

  // ---- sfactor: feedrate override %
  serial_json_key(F("sfactor"));
  SERIAL_ECHO(motion.feedrate_percentage);

  // ---- efactor: flow override % per extruder
  {
    float vals[EXTRUDERS];
    EXTRUDER_LOOP() vals[e] = planner.flow_percentage[e];
    serial_json_float_array(F("efactor"), vals, EXTRUDERS, 1);
  }

  // ---- tool: active extruder index
  serial_json_key(F("tool"));
  SERIAL_ECHO(motion.extruder);

  // ---- probe: last Z probe reading (raw ADC string)
  serial_json_key(F("probe"));
  SERIAL_ECHOPGM("\"0\"");

  // ---- fanPercent: [virtual print fan, fan0, fan1, ...]
  // PanelDue 1.13+ expects the virtual/mapped print cooling fan first,
  // then all physical fans. Marlin tracks fan speed in fan_speed[].
  #if HAS_FAN
    {
      // First element: mapped print cooling fan (fan 0 in Marlin)
      const uint8_t nfans = FAN_COUNT;
      float vals[nfans + 1];
      vals[0] = thermalManager.fan_speed[0] * 100.0f / 255.0f;
      for (uint8_t i = 0; i < nfans; i++)
        vals[i + 1] = thermalManager.fan_speed[i] * 100.0f / 255.0f;
      serial_json_float_array(F("fanPercent"), vals, nfans + 1, 1);
    }

    // ---- fanRPM: tachometer readings (Marlin has no tach; send 0s)
    {
      int vals[FAN_COUNT] = {};
      serial_json_int_array(F("fanRPM"), vals, FAN_COUNT);
    }
  #endif

  // ---- homed: 1=homed 0=not homed per axis
  {
    int vals[NUM_AXES];
    LOOP_NUM_AXES(i) vals[i] = motion.axis_was_homed(AxisEnum(i)) ? 1 : 0;
    serial_json_int_array(F("homed"), vals, NUM_AXES);
  }

  // ---- babystep: total Z babystep offset in mm
  #if ALL(BABYSTEPPING, BABYSTEP_DISPLAY_TOTAL)
    serial_json_key(F("babystep"));
    SERIAL_ECHO(p_float_t(planner.mm_per_step[Z_AXIS] * babystep.axis_total[BS_TOTAL_IND(Z_AXIS)], 3));
  #else
    serial_json_key(F("babystep"));
    SERIAL_ECHOPGM("0.000");
  #endif

  // ---- msgBox: PanelDue dialog support (not implemented)
  // Send mode -1 to tell PanelDue there is no active message box.
  serial_json_key(F("msgBox.mode"));
  SERIAL_ECHOPGM("-1");

  //
  // Type >= 2: add print progress and estimated times
  //
  if (type >= 2) {
    #if HAS_MEDIA
      if (card.isStillPrinting()) {
        serial_json_key(F("fraction_printed"));
        SERIAL_ECHO(p_float_t(0.01f * ui.get_progress_percent(), 4));

        // timesLeft: [file-based, filament-based, layer-based] in seconds
        // Marlin does not parse slicer metadata, so we send zeros.
        serial_json_key(F("timesLeft"));
        SERIAL_ECHOPGM("[0.0,0.0,0.0]");
      }
    #endif
  }

  //
  // Type == 3: add static machine configuration
  //
  if (type == 3) {
    serial_json_key(F("geometry"));
    SERIAL_ECHOPGM("\""
      TERN_(IS_FULL_CARTESIAN, "cartesian")
      TERN_(IS_SCARA,          "scara")
      TERN_(DELTA,             "delta")
      TERN_(COREXY,            "corexy")
      TERN_(COREXZ,            "corexz")
      TERN_(COREYZ,            "coreyz")
      TERN_(COREYX,            "coreyx")
      TERN_(COREZX,            "corezx")
      TERN_(COREZY,            "corezy")
    "\"");

    // axes: number of visible/linear axes (X Y Z only for PanelDue)
    serial_json_key(F("axes"));
    SERIAL_ECHO(_MIN(NUM_AXES, 3));     // PanelDue only shows X/Y/Z

    // totalAxes: all configured axes
    serial_json_key(F("totalAxes"));
    SERIAL_ECHO(NUM_AXES);

    // axisNames: e.g. "XYZ" or "XYZI"
    serial_json_key(F("axisNames"));
    SERIAL_CHAR('"');
    LOOP_NUM_AXES(i) SERIAL_CHAR(AXIS_CHAR(i));
    SERIAL_CHAR('"');

    // volumes: number of SD card slots
    serial_json_key(F("volumes"));
    SERIAL_CHAR(TERN(SDSUPPORT, '1', '0'));

    // numTools: extruder count
    serial_json_key(F("numTools"));
    SERIAL_ECHO(EXTRUDERS);

    // myName: machine name
    serial_json_key(F("myName"));
    SERIAL_ECHOPGM("\"" MACHINE_NAME "\"");

    // firmwareName
    serial_json_key(F("firmwareName"));
    SERIAL_ECHOPGM("\"Marlin\"");
  }

  SERIAL_CHAR('}');
  SERIAL_EOL();
}

#endif // PANELDUE
