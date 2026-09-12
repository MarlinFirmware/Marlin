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

#include "../inc/MarlinConfig.h"

#if ENABLED(CONTINUE_PRINT_FROM_Z)

#include "continue_from_z.h"

#include "../sd/cardreader.h"
#include "../gcode/gcode.h"
#include "../gcode/queue.h"
#include "../module/motion.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/printcounter.h"
#include "../MarlinCore.h"
#include "../lcd/marlinui.h"

#define DEBUG_OUT ENABLED(START_PRINT_FROM_Z_DEBUG)
#include "../core/debug_out.h"

namespace ContinueFromZ {

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

  // Find the value of LETTER<number> in 'line'. Returns true and writes value
  // to 'out' if found. Stops at ';'.
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

  // Read one line from card.file into 'buf' (NUL-terminated, no CR/LF).
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
  // 'z_changed' is set true if Z moved. 'extruded' is set true if this move
  // increases the extruder counter (a real extrusion, not a retract or pure
  // travel). G2/G3 default to extruding when E is present.
  void apply_motion(ScanState &s, const char *line, bool &z_changed, bool &extruded) {
    z_changed = false;
    extruded  = false;
    float v;
    if (get_param(line, 'F', v)) s.feedrate_mm_m = to_mm(s, v);

    if (get_param(line, 'X', v)) s.x = s.rel_xyz ? s.x + to_mm(s, v) : to_mm(s, v);
    if (get_param(line, 'Y', v)) s.y = s.rel_xyz ? s.y + to_mm(s, v) : to_mm(s, v);
    if (get_param(line, 'E', v)) {
      const float new_e = s.rel_e ? s.e + to_mm(s, v) : to_mm(s, v);
      if (new_e > s.e) extruded = true;
      s.e = new_e;
    }

    if (get_param(line, 'Z', v)) {
      const float new_z = s.rel_xyz ? s.z + to_mm(s, v) : to_mm(s, v);
      z_changed = (new_z != s.z);
      s.z = new_z;
    }
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
      if (get_param(line, 'P', v)) {
        p = (uint8_t)v;
        if (p >= FAN_COUNT) return;
      }
      if (off) s.fan_speed[p] = 0;
      else if (get_param(line, 'S', v)) s.fan_speed[p] = (uint8_t)constrain((int)v, 0, 255);
    #else
      UNUSED(s); UNUSED(line); UNUSED(off);
    #endif
  }

  // Locate the first G/M/T command number in the line. Skips leading
  // whitespace and line numbers ('N123'). Returns the prefix letter
  // ('G'/'M'/'T') or 0 if none. 'num' receives the integer code.
  // 'args' is set to the remainder of the line (from the first whitespace
  // after the command, or end if none). Comments are stripped here.
  char parse_cmd(char *line, int16_t &num, const char *&args) {
    // Strip ';' comment by terminating early.
    for (char *p = line; *p; ++p) if (*p == ';') { *p = 0; break; }

    char *p = line;
    while (*p == ' ' || *p == '\t') ++p;
    // Skip line number 'N<digits>'.
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
  inline void scan_idle(millis_t &last_ms) {
    const millis_t now = millis();
    if (millis_t(now - last_ms) < 10UL) return;
    last_ms = now;
    marlin.idle_no_sleep();
  }

  // Periodic progress report to LCD status line and serial host.
  // Throttled to ~1 s to avoid flooding the host on slow links.
  inline void report_progress(millis_t &last_ms, const float skip_to_z, const float current_z) {
    const millis_t now = millis();
    if (millis_t(now - last_ms) < 1000UL) return;
    last_ms = now;
    const uint8_t pct = card.percentDone();
    // LCD status (e.g. "Scan Z 1.2/12.0 56%")
    ui.set_status(&MString<22>.setf(F("Scan Z %s/%s %u%%"), w_float_t(current_z, 1, 1), w_float_t(skip_to_z, 1, 1), pct));
    // Serial line for host log
    SERIAL_ECHOLNPGM("ContinueFromZ: ", pct, "% z=", current_z, "/", skip_to_z, " sdpos=", card.getIndex());
  }

  // Build a G-code prep sequence and enqueue it,
  // then start the SD print from byte offset 'sdpos'.
  void do_resume(const ScanState &s, const uint32_t sdpos) {
    ui.set_status(GET_TEXT_F(MSG_PRINTING));

    // Start heating bed first (non-blocking).
    #if HAS_HEATED_BED
      if (s.bed_target > 0)
        queue.enqueue_one_now(TS(F("M140 S"), s.bed_target));
    #endif

    // Start heating hotends (non-blocking).
    #if HAS_HOTEND
      HOTEND_LOOP()
        if (s.hotend_target[e] > 0)
          queue.enqueue_one_now(TS(F("M104 T"), e, F(" S"), s.hotend_target[e]));
    #endif

    // Wait for bed.
    #if HAS_HEATED_BED
      if (s.bed_target > 0)
        queue.enqueue_one_now(TS(F("M190 S"), s.bed_target));
    #endif

    // Switch to active tool (also waits for/applies its temp).
    #if HAS_MULTI_EXTRUDER
      if (s.active_tool != motion.extruder)
        queue.enqueue_one_now(TS(F("T"), s.active_tool, F(" S0")));
    #endif

    // Wait for active hotend.
    #if HAS_HOTEND
      if (s.hotend_target[s.active_tool] > 0)
        queue.enqueue_one_now(TS(F("M109 T"), s.active_tool, F(" S"), s.hotend_target[s.active_tool]));
    #endif

    // Units / absolute mode for the lift move.
    queue.enqueue_one_now(s.inches ? F("G20") : F("G21"));
    queue.enqueue_one_now(F("G90"));
    TERN_(HAS_EXTRUDERS, queue.enqueue_one_now(F("M82")));

    // Home XY.
    queue.enqueue_one_now(F("G28 X Y"));

    // Restore extruder counter to scanned value.
    TERN_(HAS_EXTRUDERS, queue.enqueue_one_now(TS(F("G92 E"), w_float_t(s.e, 1, 5))));

    // Move to scanned Z (absolute).
    queue.enqueue_one_now(TS(F("G1 Z"), w_float_t(s.z, 1, 3), F(" F300")));

    // Restore relative modes (if any) so the resumed file picks up correctly.
    if (s.rel_xyz) queue.enqueue_one_now(F("G91"));
    if (TERN0(HAS_EXTRUDERS, s.rel_e)) queue.enqueue_one_now(F("M83"));

    // Set fans.
    #if HAS_FAN
      FANS_LOOP(i)
        if (s.fan_speed[i] > 0)
          queue.enqueue_one_now(TS(F("M106 P"), i, F(" S"), s.fan_speed[i]));
    #endif

    // Resume SD print from the byte offset where the scan stopped.
    card.setIndex(sdpos);
    card.startOrResumeFilePrinting();
    print_job_timer.start();
    ui.reset_status();
  }

} // namespace (anonymous)

Status prepare(const float skip_to_z) {
  if (skip_to_z <= 0.0f) return ERR_NO_FILE;
  if (!card.isFileOpen()) return ERR_NO_FILE;

  // Rewind file to start of scan.
  card.setIndex(0);

  ScanState st = {};
  st.feedrate_mm_m = 1500.0f;
  // Defaults follow Marlin's startup convention (absolute XYZ + absolute E).
  st.rel_xyz = false;
  st.rel_e   = false;
  st.inches  = false;
  st.active_tool = motion.extruder;

  char buf[LINE_BUF_SIZE];
  uint32_t last_idle_ms = millis();
  millis_t last_progress_ms = 0;    // Force first report immediately
  uint32_t lines_seen = 0;

  // Track the most recent line that established the current Z. When we later
  // see an extrusion move at Z >= target, we resume FROM that line so the
  // printer travels to (X,Y,Z) before extruding. This rejects parking moves
  // such as 'G1 Z15 F500' that have no extrusion at that height.
  ScanState  z_setup_state{};       // State BEFORE the Z-establishing line ran
  uint32_t   z_setup_sdpos = 0;     // Byte offset of that line's first byte
  bool       have_z_setup  = false;

  ui.set_status(GET_TEXT_F(MSG_SKIP_TO_Z_SCAN));

  for (;;) {
    scan_idle(last_idle_ms);
    report_progress(last_progress_ms, skip_to_z, st.z);
    if (card.eof()) {
      DEBUG_ECHOLN(F("ContinueFromZ: "), F("EOF without hitting target Z"));
      return ERR_NOT_FOUND;
    }

    const uint32_t line_start_sdpos = card.getIndex();
    const int16_t len = read_line(buf);
    if (len < 0) return ERR_NOT_FOUND;
    lines_seen++;

    if (len == 0) continue;

    int16_t num = 0;
    const char *args = nullptr;
    const char prefix = parse_cmd(buf, num, args);
    if (prefix == 0) continue;

    // Snapshot state BEFORE applying this line so we can rewind if this
    // turns out to be the Z-establishing line we'll resume from.
    const ScanState pre = st;

    bool z_changed = false,
         extruded  = false;

    if (prefix == 'G') {
      switch (num) {
        case 0: case 1: case 2: case 3:
          apply_motion(st, args, z_changed, extruded);
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
        case 140: case 190: apply_temp_bed(st, args); break;
        case 106: apply_fan(st, args, /*off=*/false); break;
        case 107: apply_fan(st, args, /*off=*/true); break;
        default: break;
      }
    }
    else if (prefix == 'T') {
      if (num >= 0 && num < EXTRUDERS) st.active_tool = (uint8_t)num;
    }

    // Remember every line that changes Z as the latest "Z-establishing" line.
    if (z_changed) {
      z_setup_state = pre;
      z_setup_sdpos = line_start_sdpos;
      have_z_setup  = true;
      DEBUG_ECHOLN(F("ContinueFromZ: "), F("Z move @ line "), lines_seen, F(" z="), st.z, F(" sdpos="), line_start_sdpos);
    }

    // Trigger only when we actually extrude at Z >= target.
    if (extruded && st.z >= skip_to_z) {
      // Prefer resuming from the Z-establishing line (so we travel to X,Y,Z
      // properly before extruding). If none was recorded — e.g. file uses Z
      // only at startup — fall back to start of the extrusion line itself.
      const uint32_t sdpos = have_z_setup ? z_setup_sdpos : line_start_sdpos;
      const ScanState &resume_state = have_z_setup ? z_setup_state : pre;
      DEBUG_ECHOLNPGM(F("ContinueFromZ: "), F("extrusion @ line ", lines_seen, F(" z="), st.z, F(" resume sdpos="), sdpos);
      // We want the printer at the layer Z (current Z), but with E counter
      // restored to the value it had just before the Z-setup line.
      ScanState rs = resume_state;
      rs.z = st.z;
      do_resume(rs, sdpos);
      return OK;
    }

  } // for loop
}

} // namespace ContinueFromZ

#endif // CONTINUE_PRINT_FROM_Z
