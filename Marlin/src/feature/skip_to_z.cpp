/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(START_PRINT_FROM_Z)

#include "skip_to_z.h"
#include "powerloss.h"

#include "../sd/cardreader.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../MarlinCore.h"
#include "../lcd/marlinui.h"

#if ENABLED(START_PRINT_FROM_Z_DEBUG)
  #define DEBUG_OUT 1
#else
  #define DEBUG_OUT 0
#endif
#include "../core/debug_out.h"

namespace SkipToZ {

float target_z = 0.0f;

namespace {

  constexpr uint16_t LINE_BUF_SIZE = 128;

  struct ScanState {
    float x, y, z, e;       // simulated logical position (mm)
    float feedrate_mm_m;    // last feedrate seen (mm/min)
    bool  rel_xyz;          // false = G90 (default), true = G91
    bool  rel_e;            // false = M82 (default), true = M83
    bool  inches;           // G20 active (else G21)
    uint8_t active_tool;
    celsius_t hotend_target[HOTENDS];
    #if HAS_HEATED_BED
      celsius_t bed_target;
    #endif
    #if HAS_FAN
      uint8_t fan_speed[FAN_COUNT];
    #endif
  };

  // ----------------- line-level parsing helpers -----------------

  // Match LETTER as a parameter at position p (case-insensitive).
  // Valid if p is at line start, or previous char is a separator.
  inline bool is_param_at(const char *base, const char *p, char letter) {
    if (((*p) | 0x20) != (letter | 0x20)) return false;
    if (p == base) return true;
    const char prev = *(p - 1);
    return prev == ' ' || prev == '\t';
  }

  // Find the value of LETTER<number> in `line`. Returns true and writes value
  // to `out` if found. Stops at ';'.
  bool get_param(const char *line, const char letter, float &out) {
    for (const char *p = line; *p && *p != ';'; ++p) {
      if (is_param_at(line, p, letter)) {
        char *end;
        const float v = strtod(p + 1, &end);
        if (end != p + 1) { out = v; return true; }
      }
    }
    return false;
  }

  // Read one line from card.file into `buf` (NUL-terminated, no CR/LF).
  // Returns line length, or -1 on EOF.
  int16_t read_line(char *buf) {
    uint16_t n = 0;
    bool any = false;
    while (true) {
      const int16_t b = card.get();
      if (b < 0) { buf[n] = 0; return any ? (int16_t)n : -1; }
      any = true;
      const char c = (char)b;
      if (c == '\n') break;
      if (c == '\r') continue;
      if (n + 1 < LINE_BUF_SIZE) buf[n++] = c;
    }
    buf[n] = 0;
    return (int16_t)n;
  }

  // ----------------- state simulation -----------------

  inline float to_mm(const ScanState &s, const float v) {
    return s.inches ? v * 25.4f : v;
  }

  // Apply a motion command (G0/G1/G2/G3). Updates state from line params.
  // Returns true if Z changed.
  bool apply_motion(ScanState &s, const char *line) {
    float v;
    if (get_param(line, 'F', v)) s.feedrate_mm_m = to_mm(s, v);

    if (get_param(line, 'X', v)) s.x = s.rel_xyz ? s.x + to_mm(s, v) : to_mm(s, v);
    if (get_param(line, 'Y', v)) s.y = s.rel_xyz ? s.y + to_mm(s, v) : to_mm(s, v);
    if (get_param(line, 'E', v)) s.e = s.rel_e   ? s.e + to_mm(s, v) : to_mm(s, v);

    bool z_changed = false;
    if (get_param(line, 'Z', v)) {
      const float new_z = s.rel_xyz ? s.z + to_mm(s, v) : to_mm(s, v);
      z_changed = (new_z != s.z);
      s.z = new_z;
    }
    return z_changed;
  }

  // G92 X.. Y.. Z.. E.. — set the simulated logical position.
  bool apply_g92(ScanState &s, const char *line) {
    float v;
    bool z_changed = false;
    if (get_param(line, 'X', v)) s.x = to_mm(s, v);
    if (get_param(line, 'Y', v)) s.y = to_mm(s, v);
    if (get_param(line, 'E', v)) s.e = to_mm(s, v);
    if (get_param(line, 'Z', v)) { s.z = to_mm(s, v); z_changed = true; }
    return z_changed;
  }

  void apply_temp_hotend(ScanState &s, const char *line) {
    float v;
    uint8_t idx = s.active_tool;
    if (get_param(line, 'T', v)) idx = (uint8_t)v;
    if (idx >= HOTENDS) return;
    if (get_param(line, 'S', v)) s.hotend_target[idx] = (celsius_t)v;
  }

  void apply_temp_bed(ScanState &s, const char *line) {
    #if HAS_HEATED_BED
      float v;
      if (get_param(line, 'S', v)) s.bed_target = (celsius_t)v;
    #else
      UNUSED(s); UNUSED(line);
    #endif
  }

  void apply_fan(ScanState &s, const char *line, const bool off) {
    #if HAS_FAN
      float v;
      uint8_t p = 0;
      if (get_param(line, 'P', v)) p = (uint8_t)v;
      if (p >= FAN_COUNT) return;
      if (off) s.fan_speed[p] = 0;
      else if (get_param(line, 'S', v)) s.fan_speed[p] = (uint8_t)constrain((int)v, 0, 255);
    #else
      UNUSED(s); UNUSED(line); UNUSED(off);
    #endif
  }

  // Locate the first G/M/T command number in the line. Skips leading
  // whitespace and line numbers (`N123`). Returns the prefix letter
  // ('G'/'M'/'T') or 0 if none. `num` receives the integer code.
  // `args` is set to the remainder of the line (from the first whitespace
  // after the command, or end if none). Comments are stripped here.
  char parse_cmd(char *line, int16_t &num, const char *&args) {
    // Strip ';' comment by terminating early.
    for (char *p = line; *p; ++p) if (*p == ';') { *p = 0; break; }

    char *p = line;
    while (*p == ' ' || *p == '\t') ++p;
    // Skip line number `N<digits>`.
    if ((*p | 0x20) == 'n' && (p[1] >= '0' && p[1] <= '9')) {
      ++p; while (*p >= '0' && *p <= '9') ++p;
      while (*p == ' ' || *p == '\t') ++p;
    }
    if (*p == 0) return 0;
    const char up = *p & 0xDF;
    if (up != 'G' && up != 'M' && up != 'T') return 0;
    char *end;
    num = (int16_t)strtol(p + 1, &end, 10);
    if (end == p + 1) return 0;
    args = end;
    return up;
  }

  // Periodically pet the system during the scan (watchdog, heaters, UI).
  inline void scan_idle(uint32_t &last_ms) {
    const uint32_t now = millis();
    if (now - last_ms >= 100) {
      last_ms = now;
      idle_no_sleep();
    }
  }

  // Dump simulated state into PrintJobRecovery::info and mark it valid.
  void commit_to_recovery(const ScanState &s, const uint32_t sdpos) {
    job_recovery_info_t &info = recovery.info;
    memset(&info, 0, sizeof(info));

    info.current_position.x = s.x;
    info.current_position.y = s.y;
    info.current_position.z = s.z;
    TERN_(HAS_EXTRUDERS, info.current_position.e = s.e);

    info.feedrate = s.feedrate_mm_m > 0 ? (uint16_t)s.feedrate_mm_m : 1500;
    info.zraise = POWER_LOSS_ZRAISE;
    info.flag.raised = false;

    #if HAS_HOME_OFFSET
      info.home_offset = home_offset;
    #endif
    #if HAS_POSITION_SHIFT
      info.position_shift = position_shift;
    #endif
    #if HAS_MULTI_EXTRUDER
      info.active_extruder = s.active_tool;
    #endif

    #if DISABLED(NO_VOLUMETRICS)
      EXTRUDER_LOOP() info.filament_size[e] = planner.filament_size[e];
      info.flag.volumetric_enabled = parser.volumetric_enabled;
    #endif

    #if HAS_HOTEND
      HOTEND_LOOP() info.target_temperature[e] = s.hotend_target[e];
    #endif
    #if HAS_HEATED_BED
      info.target_temperature_bed = s.bed_target;
    #endif
    #if HAS_FAN
      FANS_LOOP(i) info.fan_speed[i] = s.fan_speed[i];
    #endif

    #if HAS_LEVELING
      info.fade = TERN0(ENABLE_LEVELING_FADE_HEIGHT, planner.z_fade_height);
      info.flag.leveling = planner.leveling_active;
    #endif

    // Filename and SD position.
    card.getAbsFilenameInCWD(info.sd_filename);
    info.sdpos = sdpos;

    info.print_job_elapsed = 0;

    // Relative axis mode flags (bitmask using REL_X/REL_Y/REL_Z/REL_E shifts).
    relative_t rel = 0;
    if (s.rel_xyz) rel |= _BV(REL_X) | _BV(REL_Y) | _BV(REL_Z);
    #if HAS_EXTRUDERS
      if (s.rel_e) rel |= _BV(REL_E);
    #endif
    info.axis_relative = rel;

    info.flag.dryrun = false;
    info.flag.allow_cold_extrusion = TERN0(PREVENT_COLD_EXTRUSION, thermalManager.allow_cold_extrude);

    // Mark valid so recovery.valid()/resume() will accept it.
    info.valid_head = info.valid_foot = 0xC0;
  }

} // namespace (anonymous)

Status prepare(const float skip_to) {
  if (skip_to <= 0.0f) return ERR_NO_FILE;
  if (!card.isFileOpen()) return ERR_NO_FILE;

  // Rewind file to start of scan.
  card.setIndex(0);

  ScanState st = {};
  st.feedrate_mm_m = 1500.0f;
  // Defaults follow Marlin's startup convention (absolute XYZ + absolute E).
  st.rel_xyz = false;
  st.rel_e   = false;
  st.inches  = false;
  st.active_tool = active_extruder;

  char buf[LINE_BUF_SIZE];
  uint32_t last_idle_ms = millis();
  uint32_t lines_seen = 0;

  ui.set_status(GET_TEXT_F(MSG_SKIP_TO_Z_SCAN));

  while (true) {
    scan_idle(last_idle_ms);
    if (card.eof()) {
      DEBUG_ECHOLNPGM("SkipToZ: EOF without hitting target Z");
      return ERR_NOT_FOUND;
    }

    const int16_t len = read_line(buf);
    if (len < 0) return ERR_NOT_FOUND;
    lines_seen++;

    if (len == 0) continue;

    int16_t num = 0;
    const char *args = nullptr;
    const char prefix = parse_cmd(buf, num, args);
    if (prefix == 0) continue;

    bool z_changed = false;

    if (prefix == 'G') {
      switch (num) {
        case 0: case 1: case 2: case 3:
          z_changed = apply_motion(st, args);
          break;
        case 20: st.inches = true;  break;
        case 21: st.inches = false; break;
        case 90: st.rel_xyz = false; st.rel_e = false; break;
        case 91: st.rel_xyz = true;  st.rel_e = true;  break;
        case 92:
          z_changed = apply_g92(st, args);
          break;
        default: break;
      }
    }
    else if (prefix == 'M') {
      switch (num) {
        case 82: st.rel_e = false; break;
        case 83: st.rel_e = true;  break;
        case 104: case 109: apply_temp_hotend(st, args); break;
        case 140: case 190: apply_temp_bed(st, args);    break;
        case 106: apply_fan(st, args, /*off=*/false); break;
        case 107: apply_fan(st, args, /*off=*/true);  break;
        default: break;
      }
    }
    else if (prefix == 'T') {
      if (num >= 0 && num < EXTRUDERS) st.active_tool = (uint8_t)num;
    }

    if (z_changed && st.z >= skip_to) {
      const uint32_t sdpos_next = card.getIndex(); // byte offset of NEXT line
      DEBUG_ECHOLNPGM("SkipToZ: hit target at line ", lines_seen, " sdpos=", sdpos_next, " z=", st.z);
      commit_to_recovery(st, sdpos_next);
      return OK;
    }
  }
}

} // namespace SkipToZ

#endif // START_PRINT_FROM_Z
