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

/**
 * lcd/extui/paneldue/paneldue.cpp
 *
 * PanelDue serial protocol, modeled on RepRapFirmware 3.x
 * as consumed by PanelDueFirmware 3.2 and newer.
 *
 * Panel => Printer
 *   - "N<n> <cmd>*<crc>" lines. A 5-digit checksum is CRC-16/XMODEM, 1-3 digits is XOR.
 *     Bad lines are dropped silently. Line numbers are not checked.
 *   - M409 K"<key>" F"<flags>" : Object Model query, answered with JSON.
 *   - M20 S2 P<dir>            : File listing, answered with JSON.
 *   - M36 <file>               : File info, answered with JSON.
 *   - Other commands are translated from RepRapFirmware dialect and run.
 *
 * Printer => Panel
 *   - One JSON object per line. The panel ignores anything that isn't JSON.
 *   - {"key":"<key>","flags":"<flags>","result":{...}} for M409
 *   - {"seq":<n>,"resp":"<text>"} for console messages
 *   - {"beep_freq":<hz>,"beep_length":<ms>} for tones
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(PANELDUE)

#include "paneldue.h"
#include "../ui_api.h"
#include "../../marlinui.h"

#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"

#if HAS_MEDIA
  #include "../../../sd/cardreader.h"
#endif
#if ENABLED(BABYSTEP_DISPLAY_TOTAL)
  #include "../../../feature/babystep.h"
#endif
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

#ifndef LCD_BAUDRATE
  #define LCD_BAUDRATE 57600 // PanelDue default
#endif

#ifndef PANELDUE_TX_BUFSIZE
  #define PANELDUE_TX_BUFSIZE TERN(__AVR__, 128, 1024) // JSON output buffer. Larger replies block briefly.
#endif
#ifndef PANELDUE_LINE_SIZE
  #define PANELDUE_LINE_SIZE 160                       // Longest line accepted from the panel
#endif
#ifndef PANELDUE_FIFO_SIZE
  #define PANELDUE_FIFO_SIZE 192                       // Commands waiting to be injected
#endif

//#define PANELDUE_DEBUG
#if ENABLED(PANELDUE_DEBUG)
  #define PD_DEBUG(V...) SERIAL_ECHOLN(F("PanelDue: "), V)
#else
  #define PD_DEBUG(...) NOOP
#endif

PanelDue paneldue;

#define PD_UPPER(C) (WITHIN(C, 'a', 'z') ? char((C) - ('a' - 'A')) : (C))

//
// Output to the panel
//
// Replies are written to a buffer that drains in the idle loop so a poll
// response doesn't block Marlin for the whole transmission time. When the
// buffer is full the oldest data is written out with a blocking write.
//
static char tx_buf[PANELDUE_TX_BUFSIZE];
static uint16_t tx_head, tx_tail;               // Data waiting to be sent is tx_buf[tx_tail..tx_head)

#if defined(LCD_SERIAL_TX_BUFFER_FREE)
  #define PD_TX_FREE() LCD_SERIAL_TX_BUFFER_FREE()
#elif defined(SERIAL_GET_TX_BUFFER_FREE)
  #define PD_TX_FREE() SERIAL_GET_TX_BUFFER_FREE()
#endif

static void tx_flush_blocking() {
  while (tx_tail < tx_head) LCD_SERIAL.write(tx_buf[tx_tail++]);
  tx_head = tx_tail = 0;
}

static void tx_drain() {
  #ifdef PD_TX_FREE
    while (tx_tail < tx_head && PD_TX_FREE() > 0) LCD_SERIAL.write(tx_buf[tx_tail++]);
    if (tx_tail >= tx_head) tx_head = tx_tail = 0;
  #else
    tx_flush_blocking();
  #endif
}

static void o_char(const char c) {
  if (tx_head >= sizeof(tx_buf)) tx_flush_blocking();
  tx_buf[tx_head++] = c;
}
static void o_str(const char *s) { while (*s) o_char(*s++); }
static void o_pstr(PGM_P p) { while (const char c = pgm_read_byte(p++)) o_char(c); }
static void o_fstr(FSTR_P const f) { o_pstr(FTOP(f)); }
static void o_bool(const bool b) { o_fstr(b ? F("true") : F("false")); }

static void o_uint(uint32_t v) {
  char b[11], *p = &b[10];
  *p = '\0';
  do { *--p = '0' + (v % 10); v /= 10; } while (v);
  o_str(p);
}

static void o_int(const int32_t v) {
  if (v < 0) { o_char('-'); o_uint(uint32_t(-v)); }
  else o_uint(uint32_t(v));
}

static void o_float(float f, const uint8_t dp) {
  if (isnan(f) || isinf(f)) { o_fstr(F("null")); return; }
  if (f < 0) { o_char('-'); f = -f; }
  uint32_t scale = 1;
  for (uint8_t i = 0; i < dp; ++i) scale *= 10;
  const uint32_t v = uint32_t(f * scale + 0.5f);
  o_uint(v / scale);
  if (dp) {
    o_char('.');
    uint32_t frac = v % scale;
    for (uint32_t d = scale / 10; d; d /= 10) { o_char('0' + frac / d); frac %= d; }
  }
}

// A quoted JSON string with escapes. Control characters become spaces.
static void o_jstr(const char *s) {
  o_char('"');
  if (s) for (; *s; ++s) {
    const char c = *s;
    if (c == '"' || c == '\\') { o_char('\\'); o_char(c); }
    else o_char((uint8_t)c < ' ' ? ' ' : c);
  }
  o_char('"');
}

// A key with a leading comma (or not) and the colon, e.g. ,"key":
static void o_key(FSTR_P const k, const bool comma=true) {
  if (comma) o_char(',');
  o_char('"'); o_fstr(k); o_fstr(F("\":"));
}

static void o_end() { o_fstr(F("}\n")); }

//
// State reported to the panel
//
static uint16_t seq_state,    // Bumped when the message box changes so the panel re-reads "state"
                seq_volumes,  // Bumped when media is mounted or removed
                resp_seq;     // Sequence number for console messages

static bool mbox_active,      // A message box is being shown
            mbox_choices;     // The message box offers "Purge more" / "Continue"
static int8_t mbox_mode;      // RRF M291 mode
static uint16_t mbox_seq;
static char mbox_msg[96];
static bool mbox_waited;      // Marlin has waited for the user since the box was shown

static char status_msg[MAX_MESSAGE_BYTES + 1];
static bool status_pending;
static uint16_t beep_freq, beep_ms;

static void mbox_clear() {
  if (!mbox_active) return;
  mbox_active = mbox_choices = mbox_waited = false;
  mbox_mode = -1;
  seq_state++;
}

static bool waiting_for_user() {
  return ExtUI::awaitingUserConfirm()
    || TERN0(ADVANCED_PAUSE_FEATURE, (mbox_choices && pause_menu_response == PAUSE_RESPONSE_WAIT_FOR));
}

// Update the message box mode to match what Marlin is waiting for
static void mbox_update() {
  if (!mbox_active) return;
  const bool waiting = waiting_for_user();
  if (waiting) mbox_waited = true;
  else if (mbox_waited) { mbox_clear(); return; }     // Confirmed some other way

  const int8_t mode = mbox_choices ? 4 : waiting ? 2 : 1; // Choices, OK, or Close button
  if (mode != mbox_mode) {
    mbox_mode = mode;
    mbox_seq++;
    seq_state++;
  }
}

//
// Status string for state.status
//
static FSTR_P printer_status() {
  if (ExtUI::isPrintingPaused() || ExtUI::isPrintingFromMediaPaused()) return F("paused");
  if (ExtUI::isPrintingFromMedia() || marlin.printJobOngoing()) return F("processing");
  if (ExtUI::isMoving() || queue.has_commands_queued()) return F("busy");
  return F("idle");
}

//
// Heater numbering in the Object Model
//  0      : Bed (null if none)
//  1..n   : Hotends
//  n+1    : Chamber
//
#define HEATER_E(N)    ((N) + 1)
#define HEATER_CHAMBER (HOTENDS + 1)

// The hotend used by a tool
#if HAS_MULTI_HOTEND
  #define TOOL_HOTEND(E) (E)
#else
  #define TOOL_HOTEND(E) 0
#endif

static void o_heater(const celsius_float_t current, const celsius_t target) {
  o_key(F("active"), false); o_int(target);
  o_key(F("current")); o_float(current, 1);
  o_key(F("standby")); o_char('0');
  o_key(F("state")); o_fstr(target > 0 ? F("\"active\"") : F("\"off\""));
}

static void o_heaters() {
  o_key(F("heaters"), false); o_char('[');
  #if HAS_HEATED_BED
    o_char('{'); o_heater(thermalManager.degBed(), thermalManager.degTargetBed()); o_char('}');
  #else
    o_fstr(F("null"));
  #endif
  HOTEND_LOOP() {
    o_fstr(F(",{")); o_heater(thermalManager.degHotend(e), thermalManager.degTargetHotend(e)); o_char('}');
  }
  #if HAS_HEATED_CHAMBER
    o_fstr(F(",{")); o_heater(thermalManager.degChamber(), thermalManager.degTargetChamber()); o_char('}');
  #endif
  o_char(']');
}

//
// M409 K"heat"
//
static void om_heat(const bool live) {
  o_char('{');
  if (!live) {
    o_key(F("bedHeaters"), false); o_fstr(TERN(HAS_HEATED_BED, F("[0]"), F("[-1]")));
    o_key(F("chamberHeaters"));
    #if HAS_HEATED_CHAMBER
      o_char('['); o_int(HEATER_CHAMBER); o_char(']');
    #else
      o_fstr(F("[-1]"));
    #endif
    o_char(',');
  }
  o_heaters();
  o_char('}');
}

//
// M409 K"tools"
//
static void om_tools(const bool live) {
  o_char('[');
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      if (e) o_char(',');
      o_char('{');
      #if HAS_HOTEND
        const celsius_t target = thermalManager.degTargetHotend(TOOL_HOTEND(e));
      #else
        constexpr celsius_t target = 0;
      #endif
      o_key(F("active"), false); o_char('['); o_int(target); o_char(']');
      if (!live) {
        o_key(F("extruders")); o_char('['); o_uint(e); o_char(']');
        o_key(F("fans")); o_fstr(TERN(HAS_FAN, F("[0]"), F("[]")));
        o_key(F("heaters")); o_char('[');
        TERN_(HAS_HOTEND, o_uint(HEATER_E(TOOL_HOTEND(e))));
        o_char(']');
        o_key(F("number")); o_uint(e);
      }
      o_key(F("standby")); o_fstr(F("[0]"));
      o_key(F("state")); o_fstr(e == motion.extruder ? F("\"active\"") : F("\"off\""));
      o_char('}');
    }
  #endif
  o_char(']');
}

//
// M409 K"fans"
//
static void om_fans() {
  o_char('[');
  #if HAS_FAN
    for (uint8_t f = 0; f < FAN_COUNT; ++f) {
      if (f) o_char(',');
      o_char('{');
      o_key(F("requestedValue"), false);
      o_float(ExtUI::getTargetFan_percent(ExtUI::fan_t(f)) * 0.01f, 2);
      o_char('}');
    }
  #endif
  o_char(']');
}

//
// M409 K"move"
//
static void om_move(const bool live) {
  o_char('{');
  o_key(F("axes"), false); o_char('[');
  LOOP_NUM_AXES(a) {
    const AxisEnum axis = AxisEnum(a);
    if (a) o_char(',');
    o_char('{');
    o_key(F("babystep"), false);
    #if ENABLED(BABYSTEP_DISPLAY_TOTAL)
      if (axis == Z_AXIS)
        o_float(planner.mm_per_step[Z_AXIS] * babystep.axis_total[BS_TOTAL_IND(Z_AXIS)], 3);
      else
    #endif
        o_char('0');
    o_key(F("homed")); o_bool(motion.axis_was_homed(axis));
    if (!live) {
      o_key(F("letter")); o_char('"'); o_char(AXIS_CHAR(a)); o_char('"');
    }
    o_key(F("machinePosition")); o_float(motion.position[a], 3);
    if (!live) {
      o_key(F("max")); o_float(motion.base_max_pos(axis), 1);
      o_key(F("min")); o_float(motion.base_min_pos(axis), 1);
    }
    o_key(F("userPosition")); o_float(motion.native_to_logical(motion.position[a], axis), 3);
    if (!live) { o_key(F("visible")); o_bool(true); }
    o_char('}');
  }
  o_char(']');

  o_key(F("extruders")); o_char('[');
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      if (e) o_char(',');
      o_char('{'); o_key(F("factor"), false); o_float(planner.flow_percentage[e] * 0.01f, 2); o_char('}');
    }
  #endif
  o_char(']');

  if (!live) {
    // Kinematics names as used by RepRapFirmware. The panel only checks for "delta".
    #if ENABLED(DELTA)
      #define PD_KINEMATICS "delta"
    #elif IS_SCARA
      #define PD_KINEMATICS "scara"
    #elif ENABLED(POLAR)
      #define PD_KINEMATICS "polar"
    #elif ANY(COREXY, COREYX)
      #define PD_KINEMATICS "coreXY"
    #elif ANY(COREXZ, COREZX)
      #define PD_KINEMATICS "coreXZ"
    #elif ANY(COREYZ, COREZY)
      #define PD_KINEMATICS "coreYZ"
    #elif ANY(MARKFORGED_XY, MARKFORGED_YX)
      #define PD_KINEMATICS "markForged"
    #elif ENABLED(POLARGRAPH)
      #define PD_KINEMATICS "polargraph"
    #else
      #define PD_KINEMATICS "cartesian"
    #endif
    o_key(F("kinematics")); o_fstr(F("{\"name\":\"" PD_KINEMATICS "\"}"));
    o_key(F("workplaceNumber")); o_char('0');
  }
  o_key(F("speedFactor")); o_float(motion.feedrate_percentage * 0.01f, 2);
  o_char('}');
}

//
// M409 K"job"
// The panel computes progress as filePosition / file.size so
// report the progress in parts per 10000 for all print sources.
//
static void om_job() {
  const bool printing = ExtUI::isPrintingFromMedia() || marlin.printJobOngoing();
  o_char('{');
  o_key(F("duration"), false);
  if (printing) o_uint(ExtUI::getProgress_seconds_elapsed()); else o_fstr(F("null"));
  o_key(F("file")); o_char('{');
  o_key(F("fileName"), false);
  #if HAS_MEDIA
    if (ExtUI::isPrintingFromMedia()) o_jstr(card.longest_filename()); else
  #endif
      o_fstr(printing ? F("\"\"") : F("null"));
  o_key(F("size")); o_fstr(printing ? F("10000") : F("0"));
  o_char('}');
  o_key(F("filePosition"));
  #if HAS_PRINT_PROGRESS_PERMYRIAD
    o_uint(printing ? ui.get_progress_permyriad() : 0);
  #else
    o_uint(printing ? ui.get_progress_percent() * 100UL : 0);
  #endif
  o_key(F("lastFileName")); o_fstr(F("null"));
  o_key(F("timesLeft")); o_char('{');
  o_key(F("file"), false);
  #if ENABLED(SHOW_REMAINING_TIME)
    const uint32_t remain = printing ? ui.get_remaining_time() : 0;
    if (remain) o_uint(remain); else o_fstr(F("null"));
  #else
    o_fstr(F("null"));
  #endif
  o_key(F("filament")); o_fstr(F("null"));
  o_key(F("slicer")); o_fstr(F("null"));
  o_char('}');
  o_key(F("warmUpDuration")); o_fstr(F("null"));
  o_char('}');
}

//
// M409 K"state"
//
static void om_state(const bool live) {
  o_char('{');
  o_key(F("currentTool"), false); o_int(TERN(HAS_EXTRUDERS, int(motion.extruder), -1));
  if (!live) {
    o_key(F("messageBox"));
    if (!mbox_active)
      o_fstr(F("null"));
    else {
      o_char('{');
      o_key(F("axisControls"), false); o_char('0');
      o_key(F("message")); o_jstr(mbox_msg);
      o_key(F("mode")); o_int(mbox_mode);
      o_key(F("seq")); o_uint(mbox_seq);
      o_key(F("timeout")); o_char('0');
      o_key(F("title")); o_jstr(MACHINE_NAME);
      if (mbox_choices) {
        o_key(F("cancelButton")); o_bool(false);
        o_key(F("choices")); o_fstr(F("[\"Purge more\",\"Continue\"]"));
        o_key(F("default")); o_char('1');
      }
      o_char('}');
    }
  }
  o_key(F("status")); o_char('"'); o_fstr(printer_status()); o_char('"');
  o_key(F("upTime")); o_uint(millis() / 1000UL);
  o_char('}');
}

//
// M409 K"seqs"
//
static void om_seqs() {
  o_char('{');
  o_key(F("state"), false); o_uint(seq_state);
  o_key(F("volumes")); o_uint(seq_volumes);
  o_char('}');
}

static void om_boards() {
  o_fstr(F("[{\"firmwareName\":\"Marlin\",\"firmwareVersion\":"));
  o_jstr(SHORT_BUILD_VERSION);
  o_key(F("name")); o_jstr(BOARD_INFO_NAME);
  o_fstr(F("}]"));
}

static void om_network() {
  o_char('{');
  o_key(F("interfaces"), false); o_fstr(F("[]"));
  o_key(F("name")); o_jstr(MACHINE_NAME);
  o_char('}');
}

static void om_volumes() {
  o_char('[');
  #if HAS_MEDIA
    o_char('{'); o_key(F("mounted"), false); o_bool(card.isMounted()); o_char('}');
  #endif
  o_char(']');
}

//
// The live response: frequently changing values from all keys
//
static void om_live() {
  o_char('{');
  o_key(F("fans"), false); om_fans();
  o_key(F("heat"));  om_heat(true);
  o_key(F("job"));   om_job();
  o_key(F("move"));  om_move(true);
  o_key(F("seqs"));  om_seqs();
  o_key(F("state")); om_state(true);
  o_key(F("tools")); om_tools(true);
  o_char('}');
}

//
// Command parsing helpers
//

// Find the start of a parameter (outside of quotes or braces). Return nullptr if not found.
static const char* find_param(const char *s, const char letter) {
  bool quote = false;
  int8_t brace = 0;
  for (const char *p = s; *p; ++p) {
    const char c = *p;
    if (c == '"') quote = !quote;
    else if (!quote) {
      if (c == '{') brace++;
      else if (c == '}') brace--;
      else if (!brace && PD_UPPER(c) == letter && (p == s || p[-1] == ' ')) return p + 1;
    }
  }
  return nullptr;
}

static float param_float(const char *s, const char letter, const float dflt=0) {
  const char * const p = find_param(s, letter);
  return p ? atof(p) : dflt;
}

static int32_t param_int(const char *s, const char letter, const int32_t dflt=0) {
  const char * const p = find_param(s, letter);
  return p ? atol(p) : dflt;
}

// Copy a string argument, removing quotes. Stop at a space unless quoted.
static void get_string(const char *p, char * const out, const size_t size) {
  size_t i = 0;
  if (p) {
    while (*p == ' ') p++;
    const bool quoted = (*p == '"');
    if (quoted) p++;
    for (; *p && i < size - 1; ++p) {
      if (quoted ? *p == '"' : *p == ' ') break;
      out[i++] = *p;
    }
  }
  out[i] = '\0';
}

// Get a G-code string from a parameter or the remainder of the line
static void get_path_arg(const char *args, const char letter, char * const out, const size_t size) {
  const char *p = letter ? find_param(args, letter) : nullptr;
  get_string(p ?: args, out, size);
}

#if HAS_MEDIA

  // Convert "0:/dir/file" or "dir/file" to "/dir/file". Return false for other drives.
  static bool to_card_path(const char *in, char * const out, const size_t size) {
    if (NUMERIC(in[0]) && in[1] == ':') {
      if (in[0] != '0') return false;
      in += 2;
    }
    size_t i = 0;
    if (*in != '/') out[i++] = '/';
    while (*in && i < size - 1) out[i++] = *in++;
    out[i] = '\0';
    return true;
  }

  //
  // M20 S2 P<dir> - List a folder in JSON
  //
  static void m20_json(const char * const args) {
    char dir[MAXPATHNAMELENGTH], path[MAXPATHNAMELENGTH + 1];
    get_path_arg(args, 'P', dir, sizeof(dir));
    if (!dir[0]) strcpy_P(dir, PSTR("0:/"));

    o_fstr(F("{\"dir\":")); o_jstr(dir);
    o_key(F("first")); o_char('0');
    o_key(F("files")); o_char('[');

    uint8_t err = 0;
    if (!card.isMounted())
      err = 1;
    else if (!to_card_path(dir, path, sizeof(path) - 1))
      err = 2;
    else {
      // Dive into the folder, which must end with a slash
      const size_t len = strlen(path);
      if (path[len - 1] != '/') { path[len] = '/'; path[len + 1] = '\0'; }
      MediaFile *dirPtr;
      const char * const fname = card.diveToFile(false, dirPtr, path);
      if (!fname)
        err = 2;
      else {
        char dosname[FILENAME_LENGTH];
        bool isDir, first = true;
        dirPtr->rewind();
        while (card.nextVisibleItem(*dirPtr, dosname, isDir)) {
          if (!first) o_char(',');
          first = false;
          o_char('"');
          if (isDir) o_char('*');
          o_str(dosname);
          o_char('"');
        }
      }
    }

    o_char(']');
    o_key(F("next")); o_char('0');
    o_key(F("err")); o_uint(err);
    o_end();
  }

  //
  // M36 <file> - Return file information in JSON
  //
  static void m36_json(const char * const args) {
    char fname[MAXPATHNAMELENGTH], path[MAXPATHNAMELENGTH];
    get_path_arg(args, 0, fname, sizeof(fname));

    bool ok = false;
    uint32_t size = 0;
    if (card.isMounted() && to_card_path(fname, path, sizeof(path))) {
      MediaFile *dirPtr;
      const char * const name = card.diveToFile(false, dirPtr, path);
      MediaFile f;
      if (name && *name && f.open(dirPtr, name, O_READ)) {
        size = f.fileSize();
        f.close();
        ok = true;
      }
    }

    o_char('{');
    o_key(F("err"), false); o_char(ok ? '0' : '1');
    o_key(F("fileName")); o_jstr(fname);
    if (ok) {
      o_key(F("size")); o_uint(size);
      o_key(F("filament")); o_fstr(F("[]"));
    }
    o_end();
  }

#endif // HAS_MEDIA

//
// M409 K"<key>" F"<flags>" - Report part of the Object Model
//
static void m409(const char * const args) {
  char key[24], flags[12];
  get_path_arg(args, 'K', key, sizeof(key));
  if (!find_param(args, 'K')) key[0] = '\0';
  get_path_arg(args, 'F', flags, sizeof(flags));
  if (!find_param(args, 'F')) flags[0] = '\0';

  o_fstr(F("{\"key\":")); o_jstr(key);
  o_key(F("flags")); o_jstr(flags);
  o_key(F("result"));

  #define KEY_IS(S) !strcmp_P(key, PSTR(S))
       if (!key[0])           om_live();
  else if (KEY_IS("boards"))  om_boards();
  else if (KEY_IS("fans"))    om_fans();
  else if (KEY_IS("heat"))    om_heat(false);
  else if (KEY_IS("job"))     om_job();
  else if (KEY_IS("move"))    om_move(false);
  else if (KEY_IS("network")) om_network();
  else if (KEY_IS("seqs"))    om_seqs();
  else if (KEY_IS("spindles")) o_fstr(F("[]"));
  else if (KEY_IS("state"))   om_state(false);
  else if (KEY_IS("tools"))   om_tools(false);
  else if (KEY_IS("volumes")) om_volumes();
  else                        o_fstr(F("null"));
  #undef KEY_IS

  o_end();
}

//
// Commands to run are kept in a FIFO and injected when the queue is free
//
static char fifo[PANELDUE_FIFO_SIZE];
static uint16_t fifo_len;

static void enqueue(const char * const cmd) {
  const size_t len = strlen(cmd);
  if (!len) return;
  if (fifo_len + len + 1 > sizeof(fifo)) { PD_DEBUG(F("Dropped "), cmd); return; }
  memcpy(&fifo[fifo_len], cmd, len);
  fifo_len += len;
  fifo[fifo_len++] = '\n';
}
static void enqueue(FSTR_P const fcmd) {
  char cmd[8];
  PGM_P p = FTOP(fcmd);
  uint8_t i = 0;
  while (i < sizeof(cmd) - 1 && (cmd[i] = pgm_read_byte(p++))) i++;
  cmd[i] = '\0';
  enqueue(cmd);
}

// Hand off as many whole commands as fit into the injection buffer
static void fifo_pump() {
  if (!fifo_len || queue.injected_commands[0]) return;

  constexpr uint16_t maxlen = sizeof(queue.injected_commands) - 1;
  uint16_t take = 0;
  for (uint16_t i = 0; i < fifo_len && i <= maxlen; ++i)
    if (fifo[i] == '\n') take = i + 1;

  if (take) {
    // Inject one or more commands
    char cmds[maxlen + 1];
    memcpy(cmds, fifo, take - 1);
    cmds[take - 1] = '\0';
    queue.inject(cmds);
  }
  else {
    // A long command goes to the main queue
    char * const nl = (char*)memchr(fifo, '\n', fifo_len);
    *nl = '\0';
    const bool ok = queue.enqueue_one(fifo);
    *nl = '\n';
    if (!ok) return;
    take = nl - fifo + 1;
  }

  fifo_len -= take;
  memmove(fifo, &fifo[take], fifo_len);
}

//
// Emulate M120 / M121 (push / pop) as used by the extrude buttons
//
static relative_t pushed_rel;
static bool pushed, pushed_xyz, pushed_e;

static void pop_modes() {
  if (!pushed) return;
  pushed = false;
  const bool xyz_rel = TEST(pushed_rel, REL_X);
  if (pushed_xyz) enqueue(xyz_rel ? F("G91") : F("G90"));
  #if HAS_EXTRUDERS
    if (pushed_xyz || pushed_e) {
      if (TEST(pushed_rel, E_MODE_REL)) enqueue(F("M83"));
      else if (TEST(pushed_rel, E_MODE_ABS)) enqueue(F("M82"));
      else if (!pushed_xyz) enqueue(xyz_rel ? F("G91") : F("G90")); // E follows G90 / G91
    }
  #endif
}

//
// Handle a single command from the panel, translating from RepRapFirmware dialect
//
static void handle_command(char * const cmd) {
  const char letter = PD_UPPER(cmd[0]);
  const char *p = cmd + 1;
  const int code = atoi(p);
  while (NUMERIC(*p)) p++;
  int sub = -1;
  if (*p == '.') { sub = atoi(++p); while (NUMERIC(*p)) p++; }
  while (*p == ' ') p++;
  const char * const args = p;

  PD_DEBUG(cmd);

  if (letter == 'M') switch (code) {

    case 409: m409(args); return;                     // Object Model query

    #if HAS_MEDIA
      case 20:                                        // M20 S2: JSON file list
        if (param_int(args, 'S') == 2) { m20_json(args); return; }
        break;

      case 36:                                        // M36: JSON file info
        if (sub < 0) m36_json(args);                  // (M36.1 thumbnails are not supported)
        return;

      case 32: case 98: {                             // M32: Print a file. M98: Run a macro.
        char fname[MAXPATHNAMELENGTH], path[MAXPATHNAMELENGTH];
        get_path_arg(args, code == 98 ? 'P' : 0, fname, sizeof(fname));
        if (!ExtUI::isPrinting() && to_card_path(fname, path, sizeof(path)))
          ExtUI::printFile(path);
      } return;

      case 30: {                                      // M30: Delete a file
        char fname[MAXPATHNAMELENGTH], cmd30[MAXPATHNAMELENGTH + 5] = "M30 ";
        get_path_arg(args, 0, fname, sizeof(fname));
        if (to_card_path(fname, &cmd30[4], sizeof(cmd30) - 4)) enqueue(cmd30);
      } return;

      case 21:                                        // M21 P<n>: Only card 0
        if (param_int(args, 'P') > 0) return;
        break;
    #endif

    case 0: case 1:                                   // M0: Cancel a paused print
      if (ExtUI::isPrinting()) ExtUI::stopPrint();
      return;

    case 24: ExtUI::resumePrint(); return;            // M24: Resume
    case 25: ExtUI::pausePrint(); return;             // M25: Pause

    case 112: marlin.kill(FPSTR(M112_KILL_STR), nullptr, true); return; // M112: Emergency stop

    case 999: return;                                 // M999: Nothing to reset

    case 120:                                         // M120: Push state
      pushed_rel = gcode.axis_relative;
      pushed = true; pushed_xyz = pushed_e = false;
      return;

    case 121: pop_modes(); return;                    // M121: Pop state

    case 82: case 83:                                 // M82 / M83
      if (pushed) pushed_e = true;
      break;

    case 104: case 109: case 140: case 141: case 190: case 191:
      if (find_param(args, 'R') && !find_param(args, 'S')) return; // Standby temperatures are not supported
      #if HAS_HEATED_BED
        if (code == 140 && find_param(args, 'S')) {   // M140 P<bed> S<temp>
          ExtUI::setTargetTemp_celsius(param_float(args, 'S'), ExtUI::BED);
          return;
        }
      #endif
      #if HAS_HEATED_CHAMBER
        if (code == 141 && find_param(args, 'S')) {   // M141 P<chamber> S<temp>
          ExtUI::setTargetTemp_celsius(param_float(args, 'S'), ExtUI::CHAMBER);
          return;
        }
      #endif
      break;

    case 106: {                                       // M106 S<0-256>: Fan speed
      #if HAS_FAN
        if (!find_param(args, 'P')) {
          ExtUI::setTargetFan_percent(param_float(args, 'S', 255) * 100.0f / 256.0f, ExtUI::FAN0);
          return;
        }
      #endif
    } break;

    case 220:                                         // M220 S<percent>: Speed factor
      if (find_param(args, 'S')) { ExtUI::setFeedrate_percent(param_float(args, 'S')); return; }
      break;

    case 221:                                         // M221 D<extruder> S<percent>: Extrusion factor
      #if HAS_EXTRUDERS
        if (find_param(args, 'S')) {
          const int8_t e = param_int(args, 'D', param_int(args, 'T', motion.extruder));
          if (WITHIN(e, 0, EXTRUDERS - 1)) ExtUI::setFlow_percent(param_float(args, 'S'), ExtUI::extruder_t(e));
          return;
        }
      #endif
      break;

    case 292: {                                       // M292: Message box response
      if (!mbox_active) return;
      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        if (mbox_choices) {
          // R{0} = Purge more, R{1} = Continue
          const char * const r = find_param(args, 'R');
          const int choice = r ? atoi(r[0] == '{' ? r + 1 : r) : 1;
          ExtUI::setPauseMenuResponse(choice == 0 ? PAUSE_RESPONSE_EXTRUDE_MORE : PAUSE_RESPONSE_RESUME_PRINT);
        }
        else
      #endif
          ExtUI::setUserConfirmed();
      mbox_clear();
    } return;

    case 37: case 144: case 568: return;              // Simulation, bed standby, tool settings: Not supported

    default: break;
  }
  else if (letter == 'G') switch (code) {

    case 10:                                          // G10 P<tool> S<temp> R<standby>: Tool temperatures
      if (find_param(args, 'P') && (find_param(args, 'S') || find_param(args, 'R'))) {
        #if HAS_HOTEND
          const int t = param_int(args, 'P');
          if (find_param(args, 'S') && WITHIN(t, 0, EXTRUDERS - 1))
            ExtUI::setTargetTemp_celsius(param_float(args, 'S'), ExtUI::extruder_t(t));
        #endif
        return;
      }
      break;

    case 90: case 91:                                 // G90 / G91
      if (pushed) pushed_xyz = pushed_e = true;
      break;

    case 32:                                          // G32: Bed calibration
      #if ENABLED(DELTA_AUTO_CALIBRATION)
        enqueue(F("G33"));
      #elif ANY(AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR)
        if (!ExtUI::isMachineHomed()) enqueue(F("G28"));
        enqueue(F("G29"));
      #endif
      return;

    default: break;
  }
  else if (letter == 'T') {
    if (*(cmd + 1) == '-') return;                    // T-1: Deselect tool is not supported
  }

  enqueue(cmd);
}

// Commands with string arguments that should not be split up
static bool is_string_command(const char * const cmd) {
  if (PD_UPPER(cmd[0]) != 'M') return false;
  switch (atoi(cmd + 1)) {
    case 20: case 23: case 28: case 30: case 32: case 33: case 36: case 37:
    case 98: case 117: case 118: case 292: case 409: case 928: return true;
  }
  return false;
}

// G-code commands start with G or M followed by a digit
static bool is_command_start(const char * const p) {
  const char c = PD_UPPER(p[0]);
  return (c == 'G' || c == 'M') && NUMERIC(p[1]);
}

//
// Process a line from the panel
//
static void process_line(char *line) {
  // Validate and remove the checksum
  char * const star = strrchr(line, '*');
  if (star) {
    uint32_t sum = 0;
    uint8_t digits = 0;
    for (const char *d = star + 1; NUMERIC(*d); ++d, ++digits) sum = sum * 10 + (*d - '0');
    const size_t len = star - line;
    uint32_t calc;
    if (digits == 5) {
      uint16_t crc = 0;                               // CRC-16/XMODEM, as in RRFLibraries CRC16
      for (size_t i = 0; i < len; ++i) {
        crc ^= uint16_t(uint8_t(line[i])) << 8;
        for (uint8_t b = 0; b < 8; ++b) crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
      }
      calc = crc;
    }
    else {
      uint8_t x = 0;
      for (size_t i = 0; i < len; ++i) x ^= line[i];
      calc = x;
    }
    if (!WITHIN(digits, 1, 5) || calc != sum) { PD_DEBUG(F("Bad checksum: "), line); return; }
    *star = '\0';
  }

  // Skip the line number, which is not checked
  while (*line == ' ') line++;
  if (PD_UPPER(*line) == 'N' && NUMERIC(line[1])) {
    line++;
    while (NUMERIC(*line)) line++;
    while (*line == ' ') line++;
  }

  // Remove a comment that isn't inside quotes
  {
    bool quote = false;
    for (char *c = line; *c; ++c) {
      if (*c == '"') quote = !quote;
      else if (*c == ';' && !quote) { *c = '\0'; break; }
    }
  }

  // Trim trailing spaces
  for (char *end = line + strlen(line); end > line && end[-1] == ' ';) *--end = '\0';
  if (!*line) return;

  // Jog lines look like "G91 G1 X10 F6000 G90". Treat as push / G91 / move / pop.
  const bool is_jog = !strncmp_P(line, PSTR("G91 "), 4);
  if (is_jog) {
    pushed_rel = gcode.axis_relative;
    pushed = true; pushed_xyz = pushed_e = false;
  }

  // Split lines with multiple commands, e.g., "M120 M83 G1 E5 F300 M121"
  if (is_string_command(line)) { handle_command(line); return; }

  char *start = line;
  for (;;) {
    char *next = start;
    for (;;) {
      next = strchr(next, ' ');
      if (!next || is_command_start(next + 1)) break;
      next++;
    }
    if (next) *next = '\0';
    // The final G90 of a jog line is replaced by the pop
    if (!(is_jog && !next && !strcmp_P(start, PSTR("G90")))) handle_command(start);
    if (!next) break;
    start = next + 1;
  }

  if (is_jog) pop_modes();
}

//
// Push messages, sent when there's no pending request
//
static void send_pushes() {
  if (status_pending) {
    status_pending = false;
    o_fstr(F("{\"seq\":")); o_uint(++resp_seq);
    o_key(F("resp")); o_jstr(status_msg);
    o_end();
  }
  if (beep_freq) {
    o_fstr(F("{\"beep_freq\":")); o_uint(beep_freq);
    o_key(F("beep_length")); o_uint(beep_ms);
    o_end();
    beep_freq = 0;
  }
}

//
// Public interface
//

void PanelDue::startup() {
  LCD_SERIAL.begin(LCD_BAUDRATE);
  mbox_mode = -1;
}

void PanelDue::idle() {
  static char line[PANELDUE_LINE_SIZE];
  static uint8_t line_len;
  static bool overflow;

  // Read and process lines from the panel
  while (LCD_SERIAL.available()) {
    const int c = LCD_SERIAL.read();
    if (c < 0) break;
    if (c == '\n' || c == '\r') {
      line[line_len] = '\0';
      if (overflow) PD_DEBUG(F("Line too long"));
      else if (line_len) process_line(line);
      line_len = 0;
      overflow = false;
    }
    else if (line_len < sizeof(line) - 1)
      line[line_len++] = c;
    else
      overflow = true;
  }

  mbox_update();
  send_pushes();
  fifo_pump();
  tx_drain();
}

void PanelDue::status_message(const char * const msg) {
  if (!msg || !*msg || !strcmp(msg, status_msg)) return;
  strlcpy(status_msg, msg, sizeof(status_msg));
  status_pending = true;
}

void PanelDue::user_confirm(const char * const msg) {
  if (!msg || !*msg) { mbox_clear(); return; }
  strlcpy(mbox_msg, msg, sizeof(mbox_msg));
  mbox_active = true;
  mbox_waited = false;
  mbox_mode = -1;                                     // Set in mbox_update
  mbox_update();
}

void PanelDue::pause_option(const bool show) {
  if (mbox_choices != show) { mbox_choices = show; mbox_mode = -1; }
}

void PanelDue::beep(const uint16_t frequency, const uint16_t duration) {
  beep_freq = frequency;
  beep_ms = duration;
}

void PanelDue::media_changed() { seq_volumes++; }

void PanelDue::killed(FSTR_P const error, FSTR_P const component) {
  // Marlin halts after this, so send the message right away
  tx_flush_blocking();
  o_fstr(F("{\"message\":\""));
  o_fstr(error);
  if (component && pgm_read_byte(FTOP(component))) { o_fstr(F(": ")); o_fstr(component); }
  o_fstr(F("\"}\n"));
  tx_flush_blocking();
  LCD_SERIAL.flush();
}

#endif // PANELDUE
