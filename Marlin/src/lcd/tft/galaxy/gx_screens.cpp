/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * NOVIN3DP Galaxy UI - screens
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(TFT_COLOR_UI, NOVIN3DP_GALAXY_UI)

#include "gx_core.h"
#include "gx_thumb.h"
#include "gx_home_icons.h"
#include "../ui_common.h"
#include "../../marlinui.h"
#include "../../menu/menu.h"
#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"
#include "../../../module/temperature.h"
#include "../../../module/planner.h"
#include "../../../module/motion.h"
#include "../../../module/probe.h"
#include "../../../module/settings.h"
#include "../../../module/printcounter.h"
#include "../../../sd/cardreader.h"

#if HAS_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif
#if ENABLED(BABYSTEPPING)
  #include "../../../feature/babystep.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif
#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

// Marlin menus reused as-is (restyled by gx_ui.cpp)
void menu_advanced_settings();
#if ALL(HAS_MARLINUI_MENU, HAS_TRINAMIC_CONFIG)
  #define GX_HAS_TMC 1
#endif
#if GX_HAS_TMC
  #include "../../../module/stepper/trinamic.h"
  void menu_tmc();
  void menu_tmc_current();
  #if ENABLED(HYBRID_THRESHOLD)
    void menu_tmc_hybrid_thrs();
  #endif
  #if ENABLED(SENSORLESS_HOMING)
    void menu_tmc_homing_thrs();
  #endif
  #if HAS_STEALTHCHOP
    void menu_tmc_step_mode();
  #endif
#endif
#if DISABLED(SLIM_LCD_MENUS)
  void menu_advanced_velocity();
  void menu_advanced_acceleration();
  #if ENABLED(CLASSIC_JERK)
    void menu_advanced_jerk();
  #endif
  #if ENABLED(SHAPING_MENU)
    void menu_advanced_input_shaping();
  #endif
#endif
#if ENABLED(EDITABLE_STEPS_PER_UNIT)
  void menu_advanced_steps_per_mm();
#endif
#if ENABLED(LCD_BED_TRAMMING)
  void _lcd_bed_tramming();
#endif

//
// Shared state
//
static bool gx_beep_on = true;
static float gx_move_dist = 10;
static float gx_zstep = 0.05f;
static uint8_t gx_mesh_cell = 0;
static int16_t gx_file_page = 0;
static char gx_sel_dos[13], gx_sel_long[48];
static uint32_t gx_sel_size = 0;
static bool gx_pid_bed = false;
static celsius_t gx_pid_temp = 210;
static uint8_t gx_pid_cycles = 8;
static bool gx_aborted = false;
static bool gx_was_printing = false, gx_show_done = false;
static millis_t gx_start_deadline = 0;   // Print requested: it must start before this time
// Brightness in percent, kept in ui.brightness so M500 saves it
static uint8_t gx_brightness() {
  return TERN(HAS_LCD_BRIGHTNESS, uint8_t((uint16_t(ui.brightness) * 100 + 127) / 255), 100);
}
static void gx_set_brightness(const uint8_t pct) {
  TERN_(HAS_LCD_BRIGHTNESS, ui.set_brightness(uint8_t(uint16_t(constrain(pct, 10, 100)) * 255 / 100)));
  UNUSED(pct);
}

static void gx_screen_begin() {
  #if ENABLED(TOUCH_SCREEN)
    if (gx_full()) touch.clear();
  #endif
}
static void gx_screen_end() { gx_full_redraw = false; }

#define GX_SCREEN(NAME) static void NAME(); static void NAME##_draw(); static void NAME() { \
  if (!ui.should_draw()) return; ui.defer_status_screen(); gx_screen_begin(); NAME##_draw(); gx_screen_end(); } \
  static void NAME##_draw()

void gx_go(void (*screen)()) {
  ui.push_current_screen();
  ui.goto_screen(screen);
}
void gx_back() { ui.goto_previous_screen(); }

static void gx_status_msg(const char *msg) { ui.set_status(msg); gx_invalidate(); }

static char gx_buf[48];
static const char* gx_cat(const char *a, const char *b, const char *c=nullptr) {
  strlcpy(gx_buf, a, sizeof(gx_buf));
  if (b) strlcat(gx_buf, b, sizeof(gx_buf));
  if (c) strlcat(gx_buf, c, sizeof(gx_buf));
  return gx_buf;
}

static const char* gx_strip_gcode(const char *name) {
  static char n[48];
  strlcpy(n, name, sizeof(n));
  const size_t l = strlen(n);
  if (l > 6 && (strcasecmp(n + l - 6, ".gcode") == 0)) n[l - 6] = '\0';
  return n;
}

//////////////////////////////////////////////////////////////////////////////
// Values edited with the numeric keypad
//////////////////////////////////////////////////////////////////////////////

enum GxValue : uint8_t {
  V_NOZZLE, V_BED, V_FAN, V_SPEED, V_FLOW,
  V_P0N, V_P0B, V_P0F, V_P1N, V_P1B, V_P1F,
  V_PID_T, V_PID_C, V_PROBE_X, V_PROBE_Y, V_FADE,
  V_LA_K, V_UNLOAD, V_LOAD, V_RUNOUT_D,
  V_BRIGHT, V_SLEEP,
  V_MFR_X, V_MFR_Y, V_MFR_Z, V_MFR_E,
  V_COUNT
};

static float v_get(const uint8_t v);
static void v_set(const uint8_t v, const float f);
static void v_info(const uint8_t v, const char* &label, const char* &unit, float &mn, float &mx, uint8_t &dec);

static float v_get(const uint8_t v) {
  switch (v) {
    #if HAS_HOTEND
      case V_NOZZLE: return thermalManager.degTargetHotend(0);
    #endif
    #if HAS_HEATED_BED
      case V_BED: return thermalManager.degTargetBed();
    #endif
    #if HAS_FAN
      case V_FAN: return fans[0].speed_pct();
    #endif
    case V_SPEED: return motion.feedrate_percentage;
    #if HAS_EXTRUDERS
      case V_FLOW: return planner.flow_percentage[0];
    #endif
    #if HAS_PREHEAT
      case V_P0N: return ui.material_preset[0].hotend_temp;
      case V_P0B: return TERN0(HAS_HEATED_BED, ui.material_preset[0].bed_temp);
      case V_P0F: return TERN0(HAS_FAN, ui.material_preset[0].fan_speed * 100 / 255);
      #if PREHEAT_COUNT > 1
        case V_P1N: return ui.material_preset[1].hotend_temp;
        case V_P1B: return TERN0(HAS_HEATED_BED, ui.material_preset[1].bed_temp);
        case V_P1F: return TERN0(HAS_FAN, ui.material_preset[1].fan_speed * 100 / 255);
      #endif
    #endif
    case V_PID_T: return gx_pid_temp;
    case V_PID_C: return gx_pid_cycles;
    #if HAS_BED_PROBE
      case V_PROBE_X: return probe.offset.x;
      case V_PROBE_Y: return probe.offset.y;
    #endif
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      case V_FADE: return planner.z_fade_height;
    #endif
    #if ENABLED(LIN_ADVANCE)
      case V_LA_K: return planner.extruder_advance_K[0];
    #endif
    #if ENABLED(CONFIGURE_FILAMENT_CHANGE)
      case V_UNLOAD: return fc_settings[0].unload_length;
      case V_LOAD: return fc_settings[0].load_length;
    #endif
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      case V_RUNOUT_D: return runout.runout_distance();
    #endif
    case V_BRIGHT: return gx_brightness();
    case V_MFR_X: case V_MFR_Y: case V_MFR_Z: case V_MFR_E:
      return MMM_TO_MMS(gx_manual_feedrate_mm_m[v == V_MFR_E ? E_AXIS : AxisEnum(v - V_MFR_X)]);
    #if HAS_DISPLAY_SLEEP
      case V_SLEEP: return ui.sleep_timeout_minutes;
    #endif
    default: return 0;
  }
}

static void v_set(const uint8_t v, const float f) {
  switch (v) {
    #if HAS_HOTEND
      case V_NOZZLE: thermalManager.setTargetHotend(celsius_t(f), 0); TERN_(WATCH_HOTENDS, thermalManager.start_watching_hotend(0)); break;
    #endif
    #if HAS_HEATED_BED
      case V_BED: thermalManager.setTargetBed(celsius_t(f)); TERN_(WATCH_BED, thermalManager.start_watching_bed()); break;
    #endif
    #if HAS_FAN
      case V_FAN: thermalManager.set_fan_speed(0, uint8_t(f * 255 / 100 + 0.5f)); break;
    #endif
    case V_SPEED: motion.feedrate_percentage = int16_t(f); break;
    #if HAS_EXTRUDERS
      case V_FLOW: planner.flow_percentage[0] = int16_t(f); planner.refresh_e_factor(0); break;
    #endif
    #if HAS_PREHEAT
      case V_P0N: ui.material_preset[0].hotend_temp = celsius_t(f); break;
      case V_P0B: TERN_(HAS_HEATED_BED, ui.material_preset[0].bed_temp = celsius_t(f)); break;
      case V_P0F: TERN_(HAS_FAN, ui.material_preset[0].fan_speed = uint16_t(f * 255 / 100 + 0.5f)); break;
      #if PREHEAT_COUNT > 1
        case V_P1N: ui.material_preset[1].hotend_temp = celsius_t(f); break;
        case V_P1B: TERN_(HAS_HEATED_BED, ui.material_preset[1].bed_temp = celsius_t(f)); break;
        case V_P1F: TERN_(HAS_FAN, ui.material_preset[1].fan_speed = uint16_t(f * 255 / 100 + 0.5f)); break;
      #endif
    #endif
    case V_PID_T: gx_pid_temp = celsius_t(f); break;
    case V_PID_C: gx_pid_cycles = uint8_t(f); break;
    #if HAS_BED_PROBE
      case V_PROBE_X: probe.offset.x = f; break;
      case V_PROBE_Y: probe.offset.y = f; break;
    #endif
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      case V_FADE: set_z_fade_height(f); break;
    #endif
    #if ENABLED(LIN_ADVANCE)
      case V_LA_K: planner.extruder_advance_K[0] = f; break;
    #endif
    #if ENABLED(CONFIGURE_FILAMENT_CHANGE)
      case V_UNLOAD: fc_settings[0].unload_length = f; break;
      case V_LOAD: fc_settings[0].load_length = f; break;
    #endif
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      case V_RUNOUT_D: runout.set_runout_distance(f); break;
    #endif
    case V_BRIGHT: gx_set_brightness(uint8_t(f)); break;
    case V_MFR_X: case V_MFR_Y: case V_MFR_Z: case V_MFR_E:
      gx_manual_feedrate_mm_m[v == V_MFR_E ? E_AXIS : AxisEnum(v - V_MFR_X)] = MMS_TO_MMM(f); break;
    #if ALL(HAS_DISPLAY_SLEEP, EDITABLE_DISPLAY_TIMEOUT)
      case V_SLEEP: ui.sleep_timeout_minutes = uint8_t(f); TERN_(TOUCH_SCREEN, touch.wakeUp()); break;
    #endif
    default: break;
  }
}

static void v_info(const uint8_t v, const char* &label, const char* &unit, float &mn, float &mx, uint8_t &dec) {
  unit = "\xC2\xB0" "C"; mn = 0; dec = 0; mx = 999;
  switch (v) {
    case V_NOZZLE: label = "NOZZLE TEMP"; mx = TERN(HAS_HOTEND, thermalManager.hotend_max_target(0), 0); break;
    case V_BED:    label = "BED TEMP"; mx = TERN(HAS_HEATED_BED, BED_MAX_TARGET, 0); break;
    case V_FAN:    label = "FAN SPEED"; unit = "%"; mx = 100; break;
    case V_SPEED:  label = "PRINT SPEED"; unit = "%"; mn = SPEED_EDIT_MIN; mx = SPEED_EDIT_MAX; break;
    case V_FLOW:   label = "FLOW"; unit = "%"; mn = FLOW_EDIT_MIN; mx = FLOW_EDIT_MAX; break;
    case V_P0N:    label = "PLA NOZZLE"; mx = TERN(HAS_HOTEND, thermalManager.hotend_max_target(0), 0); break;
    case V_P0B:    label = "PLA BED"; mx = TERN(HAS_HEATED_BED, BED_MAX_TARGET, 0); break;
    case V_P0F:    label = "PLA FAN"; unit = "%"; mx = 100; break;
    case V_P1N:    label = "ABS NOZZLE"; mx = TERN(HAS_HOTEND, thermalManager.hotend_max_target(0), 0); break;
    case V_P1B:    label = "ABS BED"; mx = TERN(HAS_HEATED_BED, BED_MAX_TARGET, 0); break;
    case V_P1F:    label = "ABS FAN"; unit = "%"; mx = 100; break;
    case V_PID_T:  label = "PID TARGET"; mx = gx_pid_bed ? TERN(HAS_HEATED_BED, BED_MAX_TARGET, 0) : TERN(HAS_HOTEND, thermalManager.hotend_max_target(0), 0); break;
    case V_PID_C:  label = "PID CYCLES"; unit = ""; mn = 3; mx = 20; break;
    case V_PROBE_X: label = "PROBE X OFFSET"; unit = "mm"; mn = -100; mx = 100; dec = 1; break;
    case V_PROBE_Y: label = "PROBE Y OFFSET"; unit = "mm"; mn = -100; mx = 100; dec = 1; break;
    case V_FADE:   label = "FADE HEIGHT"; unit = "mm"; mx = 100; dec = 1; break;
    case V_LA_K:   label = "LINEAR ADVANCE K"; unit = ""; mx = 10; dec = 3; break;
    case V_UNLOAD: label = "UNLOAD LENGTH"; unit = "mm"; mx = 800; break;
    case V_LOAD:   label = "LOAD LENGTH"; unit = "mm"; mx = 800; break;
    case V_RUNOUT_D: label = "RUNOUT DISTANCE"; unit = "mm"; mx = 100; dec = 1; break;
    case V_BRIGHT: label = "BRIGHTNESS"; unit = "%"; mn = 10; mx = 100; break;
    case V_SLEEP:  label = "SCREEN SLEEP"; unit = "min"; mx = 99; break;
    case V_MFR_X:  label = "X MOVE SPEED"; unit = "mm/s"; mn = 1; mx = 500; break;
    case V_MFR_Y:  label = "Y MOVE SPEED"; unit = "mm/s"; mn = 1; mx = 500; break;
    case V_MFR_Z:  label = "Z MOVE SPEED"; unit = "mm/s"; mn = 0.1f; mx = 50; dec = 1; break;
    case V_MFR_E:  label = "E MOVE SPEED"; unit = "mm/s"; mn = 0.1f; mx = 50; dec = 1; break;
    default: label = ""; break;
  }
}

static const char* v_str(const uint8_t v) {
  const char *l, *u; float mn, mx; uint8_t d;
  v_info(v, l, u, mn, mx, d);
  return gx_ftoa(v_get(v), d);
}
static const char* v_unit(const uint8_t v) {
  const char *l, *u; float mn, mx; uint8_t d;
  v_info(v, l, u, mn, mx, d);
  return u;
}

//////////////////////////////////////////////////////////////////////////////
// Numeric keypad
//////////////////////////////////////////////////////////////////////////////

static struct {
  uint8_t value;
  char buf[12];
  bool fresh;
} kp;

static void gx_screen_keypad();

static void gx_edit(const uint8_t v) {
  kp.value = v;
  strlcpy(kp.buf, v_str(v), sizeof(kp.buf));
  kp.fresh = true;
  gx_go(gx_screen_keypad);
}

static void kp_key(const char k) {
  const char *l, *u; float mn, mx; uint8_t d;
  v_info(kp.value, l, u, mn, mx, d);
  size_t n = strlen(kp.buf);
  const float step = d == 0 ? 1.0f : (d == 1 ? 0.1f : (d == 2 ? 0.01f : 0.001f));
  switch (k) {
    case 'k': {   // OK
      const float f = constrain(atof(kp.buf), mn, mx);
      v_set(kp.value, f);
      gx_back();
      return;
    }
    case 'b':
      if (kp.fresh) { kp.buf[0] = '\0'; kp.fresh = false; }
      else if (n) kp.buf[n - 1] = '\0';
      break;
    case 'm': case 'p': case 'M': case 'P': {
      const float dv = (k == 'm' ? -step : k == 'p' ? step : k == 'M' ? -10 * step : 10 * step);
      strlcpy(kp.buf, gx_ftoa(constrain(atof(kp.buf) + dv, mn, mx), d), sizeof(kp.buf));
      kp.fresh = false;
    } break;
    case 's':     // Toggle sign
      if (kp.fresh) { strcpy(kp.buf, "-"); kp.fresh = false; }
      else if (kp.buf[0] == '-') memmove(kp.buf, kp.buf + 1, n);
      else if (n < sizeof(kp.buf) - 1) { memmove(kp.buf + 1, kp.buf, n + 1); kp.buf[0] = '-'; }
      break;
    case '.':
      if (!d) break;
      if (kp.fresh) { strcpy(kp.buf, "0"); kp.fresh = false; n = 1; }
      if (!strchr(kp.buf, '.') && n < sizeof(kp.buf) - 1) strcat(kp.buf, ".");
      break;
    default:
      if (k < '0' || k > '9') break;
      if (kp.fresh) { kp.buf[0] = '\0'; kp.fresh = false; n = 0; }
      if (n == 1 && kp.buf[0] == '0') n = 0;
      if (n < 8) { kp.buf[n] = k; kp.buf[n + 1] = '\0'; }
      break;
  }
  gx_invalidate();
}

GX_SCREEN(gx_screen_keypad) {
  const char *l, *u; float mn, mx; uint8_t d;
  v_info(kp.value, l, u, mn, mx, d);
  if (gx_full()) gx_header(l);

  // Value (narrower when a sign key is needed)
  const bool can_neg = mn < 0;
  const uint16_t vw0 = can_neg ? 162 : 220;
  gx_canvas(10, 60, vw0, 84);
  gx_panel(0, 0, vw0, 84, GX_C_ACC);
  const uint16_t vw = gx_text(16, 10, kp.buf[0] ? kp.buf : "0", GX_C_ACC, GXF_L);
  gx_text(24 + vw, 16, u, GX_C_DIM);
  char mm[32];
  strlcpy(mm, "min ", sizeof(mm));
  strlcat(mm, gx_ftoa(mn, d), sizeof(mm));
  strlcat(mm, "  max ", sizeof(mm));
  strlcat(mm, gx_ftoa(mx, d), sizeof(mm));
  gx_text(16, 52, mm, GX_C_DIM);

  if (!gx_full()) return;

  if (can_neg) gx_button(178, 60, 52, 84, GX_NO_ICON, "+/-", GXA(GA_KEY, 's'));

  // Step buttons: -10x, +10x, -1x, +1x of the smallest step
  static const char inc_key[4] = { 'M', 'P', 'm', 'p' };
  const float step = d == 0 ? 1.0f : (d == 1 ? 0.1f : (d == 2 ? 0.01f : 0.001f));
  for (uint8_t i = 0; i < 4; i++) {
    char t[12];
    strlcpy(t, (i & 1) ? "+" : "-", sizeof(t));
    strlcat(t, gx_ftoa(i < 2 ? 10 * step : step, d), sizeof(t));
    gx_button(10 + (i & 1) * 113, 152 + (i >> 1) * 52, 107, 46, GX_NO_ICON, t, GXA(GA_KEY, inc_key[i]) | GX_REPEAT);
  }

  // Keypad
  static const char keys[] = "789456123.0b";
  for (uint8_t i = 0; i < 12; i++) {
    const uint16_t kx = 240 + (i % 3) * 78, ky = 60 + (i / 3) * 50;
    const char k = keys[i];
    if (k == 'b') gx_button(kx, ky, 72, 44, GXI_BACKSPACE, nullptr, GXA(GA_KEY, 'b'));
    else { const char s[2] = { k, 0 }; gx_button(kx, ky, 72, 44, GX_NO_ICON, s, GXA(GA_KEY, k), GXB_NORMAL, GXF_L); }
  }

  gx_button(10, 264, 220, 48, GXI_CLOSE, "CANCEL", GXA(GA_BACK, 0));
  gx_button(240, 264, 230, 48, GXI_CHECK, "OK", GXA(GA_KEY, 'k'), GXB_PRIMARY);
}

//////////////////////////////////////////////////////////////////////////////
// Confirm dialog
//////////////////////////////////////////////////////////////////////////////

enum GxCommand : uint8_t;
static struct {
  const char *title, *line1, *line2, *yes;
  GxIcon icon;
  uint16_t color;
  uint8_t cmd;
  bool danger;
} cf;

GX_SCREEN(gx_screen_confirm) {
  if (!gx_full()) return;
  gx_dialog(cf.title, cf.line1, cf.line2, cf.icon, cf.color, cf.yes, GXA(GA_CMD, cf.cmd), "CANCEL", GXA(GA_BACK, 0), cf.danger);
}

static void gx_confirm(const char *title, const char *l1, const char *l2, const GxIcon icon, const uint16_t color, const char *yes, const uint8_t cmd, const bool danger=false) {
  cf.title = title; cf.line1 = l1; cf.line2 = l2; cf.icon = icon; cf.color = color; cf.yes = yes; cf.cmd = cmd; cf.danger = danger;
  gx_go(gx_screen_confirm);
}

//////////////////////////////////////////////////////////////////////////////
// Commands
//////////////////////////////////////////////////////////////////////////////

enum GxCommand : uint8_t {
  C_HOME, C_MOTORS_OFF, C_PREHEAT0, C_PREHEAT1, C_COOLDOWN,
  C_PAUSE_RESUME, C_STOP_ASK, C_STOP_YES, C_DONE_OK, C_REPRINT,
  C_LOAD, C_UNLOAD, C_PURGE, C_CHANGE,
  C_AUTOLEVEL, C_LEVEL_TOGGLE, C_MESH_M, C_MESH_P,
  C_DEPLOY, C_STOW, C_SELFTEST, C_ALARM,
  C_ZUP, C_ZDOWN, C_ZSTEP0, C_ZSTEP1, C_ZSTEP2, C_ZHOME, C_SAVE,
  C_SAVE_ASK, C_LOAD_ASK, C_LOAD_YES, C_RESTORE_ASK, C_RESTORE_YES,
  C_RECOVERY, C_BEEP, C_RUNOUT,
  C_VOL_USB, C_VOL_SD, C_EJECT, C_REFRESH, C_PRINT,
  C_XM, C_XP, C_YM, C_YP, C_ZM, C_ZP, C_EM, C_EP,
  C_DIST0, C_DIST1, C_DIST2, C_DIST3,
  C_PID_HOT, C_PID_BED, C_PID_START,
  C_HOME_XY, C_HOME_Z
};

// Cold extrusion is blocked only with PREVENT_COLD_EXTRUSION (and not after M302)
static bool gx_too_cold() { return thermalManager.tooColdToExtrude(0); }

static void gx_move(const AxisEnum axis, const int8_t dir) {
  #if HAS_EXTRUDERS
    if (axis == E_AXIS && gx_too_cold()) { gx_status_msg("Nozzle too cold"); return; }
  #endif
  if (ui.manual_move.processing) return;
  float mn, mx;
  motion.soft_endstop.get_manual_axis_limits(axis, mn, mx);
  ui.manual_move.apply_diff(axis, gx_move_dist * dir, mn, mx);
  ui.manual_move.soon(axis);
}

static void gx_zoffset_change(const float d) {
  #if HAS_BED_PROBE
    const float nv = constrain(probe.offset.z + d, PROBE_OFFSET_ZMIN, PROBE_OFFSET_ZMAX), diff = nv - probe.offset.z;
    if (!diff) return;
    #if ENABLED(BABYSTEPPING)
      if (BABYSTEP_ALLOWED()) { babystep.add_mm(Z_AXIS, diff); probe.offset.z = nv; return; }
    #endif
    probe.offset.z = nv;
    if (motion.all_axes_trusted() && !marlin.printer_busy() && !planner.has_blocks_queued()) {
      motion.position.z += diff;
      motion.goto_current_position(MMM_TO_MMS(300));
    }
  #else
    UNUSED(d);
  #endif
}

static void gx_mesh_change(const float d) {
  #if HAS_MESH
    const uint8_t i = gx_mesh_cell % (GRID_MAX_POINTS_X), j = gx_mesh_cell / (GRID_MAX_POINTS_X);
    if (j >= GRID_MAX_POINTS_Y) return;
    float &z = bedlevel.z_values[i][j];
    if (isnan(z)) z = 0;
    z += d;
    TERN_(ABL_BILINEAR_SUBDIVISION, bedlevel.refresh_bed_level());
  #else
    UNUSED(d);
  #endif
}

static void gx_screen_files();
static void gx_screen_mesh();

static void gx_open_volume(const bool usb) {
  #if ENABLED(MULTI_VOLUME)
    card.changeMedia(usb ? (DiskIODriver*)&card.media_driver_usbFlash : (DiskIODriver*)&card.media_driver_sdcard);
  #else
    UNUSED(usb);
  #endif
  card.mount();
  if (card.isMounted()) { gx_file_page = 0; gx_go(gx_screen_files); }
  else gx_status_msg(usb ? "No USB drive found" : "No SD card found");
}

static void gx_command(const uint8_t c) {
  switch (c) {
    case C_HOME:       queue.inject_P(G28_STR); ui.set_status(GET_TEXT(MSG_LEVEL_BED_HOMING)); break;
    case C_HOME_XY:    queue.inject(F("G28 X Y")); ui.set_status("Homing X Y"); break;
    case C_HOME_Z:     queue.inject(F("G28 Z")); ui.set_status("Homing Z"); break;
    case C_MOTORS_OFF: queue.inject(F("M84")); break;
    #if HAS_PREHEAT
      case C_PREHEAT0: ui.preheat_all(0); break;
      #if PREHEAT_COUNT > 1
        case C_PREHEAT1: ui.preheat_all(1); break;
      #endif
    #endif
    case C_COOLDOWN:   thermalManager.cooldown(); break;
    case C_PAUSE_RESUME:
      if (marlin.printingIsPaused()) ui.resume_print(); else ui.pause_print();
      break;
    case C_STOP_ASK:   gx_confirm("STOP PRINT?", "The current print will be aborted.", "Heaters turn off.", GXI_STOP, GX_C_BAD, "STOP", C_STOP_YES, true); return;
    case C_STOP_YES:   gx_aborted = true; ui.abort_print(); ui.return_to_status(); return;
    case C_DONE_OK:    gx_show_done = false; ui.return_to_status(); gx_invalidate(); break;
    #if HAS_MEDIA
      case C_REPRINT:  // Print the last job again (same file, same folder)
        if (!gx_thumb_has_job() || card.isFileOpen()) break;
        if (!gx_job_file_ok()) { gx_alert("File not found. Insert the media."); break; }
        gx_show_done = false;
        gx_aborted = false;
        gx_start_deadline = millis() + 5000UL;
        card.openAndPrintFile(gx_job_dosname());
        ui.return_to_status();
        break;
    #endif

    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      case C_LOAD:   if (gx_too_cold()) { gx_status_msg("Heat the nozzle first"); return; } queue.inject(F("M701")); break;
      case C_UNLOAD: if (gx_too_cold()) { gx_status_msg("Heat the nozzle first"); return; } queue.inject(F("M702")); break;
    #endif
    case C_PURGE:  // Extrude 30mm relative, then restore the E mode the host / G-code had set
      if (gx_too_cold()) { gx_status_msg("Heat the nozzle first"); return; }
      queue.inject(gcode.axis_is_relative(E_AXIS) ? F("G1 E30 F180") : F("M83\nG1 E30 F180\nM82"));
      break;
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      case C_CHANGE: queue.inject(F("M600")); break;
    #endif

    #if HAS_LEVELING
      case C_AUTOLEVEL: queue.inject(F("G28\nG29")); ui.set_status("Probing the bed..."); gx_go(gx_screen_mesh); return;
      case C_LEVEL_TOGGLE: set_bed_leveling_enabled(!planner.leveling_active); break;
    #endif
    case C_MESH_M: gx_mesh_change(-0.025f); break;
    case C_MESH_P: gx_mesh_change(+0.025f); break;

    #if HAS_BED_PROBE
      case C_DEPLOY:   queue.inject(F("M401")); break;
      case C_STOW:     queue.inject(F("M402")); break;
    #endif
    #if ENABLED(BLTOUCH)
      case C_SELFTEST: queue.inject(F("M280 P0 S120")); break;
      case C_ALARM:    queue.inject(F("M280 P0 S160")); break;
    #endif

    case C_ZUP:    gx_zoffset_change(+gx_zstep); break;
    case C_ZDOWN:  gx_zoffset_change(-gx_zstep); break;
    case C_ZSTEP0: gx_zstep = 0.01f; break;
    case C_ZSTEP1: gx_zstep = 0.05f; break;
    case C_ZSTEP2: gx_zstep = 0.10f; break;
    case C_ZHOME:  queue.inject(F("G28\nG1 Z0 F600")); break;

    case C_SAVE:        settings.save(); ui.set_status(GET_TEXT(MSG_STORE_EEPROM)); break;
    case C_SAVE_ASK:    gx_confirm("STORE SETTINGS?", "Save the current settings", "to EEPROM (M500).", GXI_STORE, GX_C_ACC, "STORE", C_SAVE); return;
    case C_LOAD_ASK:    gx_confirm("LOAD SETTINGS?", "Read the saved settings", "from EEPROM (M501).", GXI_LOAD_SET, GX_C_ACC, "LOAD", C_LOAD_YES); return;
    case C_LOAD_YES:    settings.load(); gx_back(); return;
    case C_RESTORE_ASK: gx_confirm("RESTORE DEFAULTS?", "Firmware defaults (M502).", "Store afterwards to keep them.", GXI_RESTORE, GX_C_HOT, "RESTORE", C_RESTORE_YES); return;
    case C_RESTORE_YES: settings.reset(); gx_back(); return;

    #if ENABLED(POWER_LOSS_RECOVERY)
      case C_RECOVERY: recovery.enable(!recovery.enabled); break;
    #endif
    case C_BEEP: gx_beep_on = !gx_beep_on; break;
    #if HAS_FILAMENT_SENSOR
      case C_RUNOUT: runout.enabled = !runout.enabled; break;
    #endif

    #if HAS_MEDIA
      case C_VOL_USB: gx_open_volume(true); return;
      case C_VOL_SD:  gx_open_volume(false); return;
      case C_EJECT:   card.release(); gx_status_msg("Media released"); return;
      case C_REFRESH: card.mount(); gx_status_msg(card.isMounted() ? "Media ready" : "No media"); return;
      case C_PRINT: {
        // Make sure the file can be opened before leaving the file screen
        MediaFile dir = card.getWorkDir(), f;
        if (!card.isMounted() || !f.open(&dir, gx_sel_dos, O_READ)) { gx_alert("Can't open the file. Check the media."); return; }
        f.close();
        gx_thumb_set_job(gx_sel_dos, gx_sel_long);
        gx_aborted = false;
        gx_start_deadline = millis() + 5000UL;
        card.openAndPrintFile(gx_sel_dos);
        ui.return_to_status();
      } return;
    #endif

    case C_XM: gx_move(X_AXIS, -1); break;
    case C_XP: gx_move(X_AXIS, +1); break;
    case C_YM: gx_move(Y_AXIS, -1); break;
    case C_YP: gx_move(Y_AXIS, +1); break;
    case C_ZM: gx_move(Z_AXIS, -1); break;
    case C_ZP: gx_move(Z_AXIS, +1); break;
    #if HAS_EXTRUDERS
      case C_EM: gx_move(E_AXIS, -1); break;
      case C_EP: gx_move(E_AXIS, +1); break;
    #endif
    case C_DIST0: gx_move_dist = 0.1f; break;
    case C_DIST1: gx_move_dist = 1; break;
    case C_DIST2: gx_move_dist = 10; break;
    case C_DIST3: gx_move_dist = 100; break;

    case C_PID_HOT: gx_pid_bed = false; gx_pid_temp = 210; break;
    case C_PID_BED: gx_pid_bed = true;  gx_pid_temp = 60;  break;
    case C_PID_START: {
      char cmd[32];
      sprintf_P(cmd, PSTR("M303 E%i S%i C%u U1"), gx_pid_bed ? -1 : 0, int(gx_pid_temp), unsigned(gx_pid_cycles));
      queue.inject(cmd);
      ui.set_status("PID autotune started");
    } break;
    default: break;
  }
  gx_invalidate();
}

//////////////////////////////////////////////////////////////////////////////
// Status: home / printing / finished
//////////////////////////////////////////////////////////////////////////////

static void gx_screen_prepare();
static void gx_screen_settings();
static void gx_screen_storage();
static void gx_screen_tune();

enum GxMode : uint8_t { GM_NONE, GM_IDLE, GM_PRINT, GM_DONE };
static GxMode gx_mode = GM_NONE;

static void status_message_line(const uint16_t y) {
  gx_canvas(0, y, GX_W, 26);
  if (ui.has_status()) gx_text_center(10, 4, GX_W - 20, ui.status_message, GX_C_DIM);
}

static void home_draw(const bool full) {
  if (full) {
    gx_canvas(0, 0, GX_W, 56);
    tft.add_image((GX_W - 200) / 2, 18, imgNOVINBIG, COLOR_WHITE);
    static const char * const labels[3] = { "prepare", "settings", "print" };
    for (uint8_t i = 0; i < 3; i++) {
      const uint16_t x = 24 + i * 152;
      gx_canvas(x, 92, 128, 172);
      gx_pal_image(24, 18, gx_home_icons[i]);
      gx_text_center(0, 124, 128, labels[i], GX_C_TEXT, GXF_S);
      gx_touch_add(x, 92, 128, 172, GXA(GA_GO, i));
    }
  }
  status_message_line(60);
  gx_footer_temps();
}

static uint8_t gx_progress_pct() {
  uint8_t p = TERN(HAS_PRINT_PROGRESS, ui.get_progress_percent(), 0);
  #if HAS_MEDIA
    if (!p && card.isFileOpen()) p = card.percentDone();   // No M73 in the file: use the read position
  #endif
  return p;
}

static void job_thumb_panel(const uint16_t x, const uint16_t y) {
  gx_canvas(x, y, 160, 160);
  gx_panel(0, 0, 160, 160);
  #if HAS_MEDIA
    if (gx_thumb_has_job()) gx_thumb(5, 5, 150, gx_job_dosname(), true);
    else gx_icon(40, 40, GXI_FILE, 80, GX_C_DIM);
  #endif
}

static void stat_row(const uint16_t x, const uint16_t y, const uint16_t w, const GxIcon icon, const char *label, const char *value, const uint16_t vcolor=GX_C_TEXT) {
  gx_canvas(x, y, w, 32);
  gx_icon(0, 4, icon, 24, GX_C_DIM);
  const int16_t vy = gx_vcenter(32, GXF_S);
  gx_text(34, vy, label, GX_C_DIM);
  gx_text_right(w, vy, value, vcolor);
  gx_bar(0, 31, w, 1, GX_C_LINE);
}

static void print_draw(const bool full) {
  const uint8_t pct = gx_progress_pct();
  const bool paused = marlin.printingIsPaused();
  if (full) {
    gx_header("PRINTING", false);
    job_thumb_panel(10, 58);
    gx_canvas(182, 60, 288, 28);
    gx_icon(0, 2, GXI_FILE, 24);
    #if HAS_MEDIA
      gx_text(32, gx_vcenter(28, GXF_S), gx_strip_gcode(gx_job_name()), GX_C_TEXT, GXF_S, 254);
    #endif
  }
  char p[8]; sprintf_P(p, PSTR("%u%%"), pct);
  gx_header_info(p, GX_C_ACC, GXF_L);
  gx_progress(182, 94, 288, 14, pct, paused ? GX_C_HOT : GX_C_ACC);

  const uint32_t el = print_job_timer.duration();
  stat_row(182, 116, 288, GXI_CLOCK, "Elapsed", gx_time(el));
  uint32_t rem = 0;
  #if ENABLED(SHOW_REMAINING_TIME)
    rem = ui.get_remaining_time();
  #endif
  if (!rem && pct > 0) rem = el * (100 - pct) / pct;
  stat_row(182, 150, 288, GXI_HOURGLASS, "Remaining", pct ? gx_time(rem) : "--:--:--");
  stat_row(182, 184, 288, GXI_LAYERS, paused ? "Paused" : "Z height", gx_ftoa(motion.position.z, 2), paused ? GX_C_HOT : GX_C_TEXT);

  gx_button(10, 226, 148, 44, GXI_TUNE, "TUNE", full ? GXA(GA_GO, 3) : 0);
  gx_button(166, 226, 148, 44, paused ? GXI_PLAY : GXI_PAUSE, paused ? "RESUME" : "PAUSE", full ? GXA(GA_CMD, C_PAUSE_RESUME) : 0, paused ? GXB_PRIMARY : GXB_NORMAL);
  gx_button(322, 226, 148, 44, GXI_STOP, "STOP", full ? GXA(GA_CMD, C_STOP_ASK) : 0, GXB_DANGER);
  gx_footer_temps();
}

static void done_draw(const bool full) {
  if (!full) return;
  job_thumb_panel(20, 30);
  gx_canvas(196, 30, 270, 48);
  gx_icon(0, 4, GXI_CHECK_C, 32, GX_C_OK);
  gx_text(42, gx_vcenter(40, GXF_L), "COMPLETE", GX_C_OK, GXF_L);
  gx_canvas(196, 80, 270, 28);
  #if HAS_MEDIA
    gx_text(0, gx_vcenter(28, GXF_S), gx_strip_gcode(gx_job_name()), GX_C_TEXT, GXF_S, 270);
  #endif
  stat_row(196, 112, 270, GXI_CLOCK, "Print time", gx_time(print_job_timer.duration()));
  stat_row(196, 146, 270, GXI_LAYERS, "Height", gx_ftoa(motion.position.z, 2));
  #if ENABLED(PRINTCOUNTER)
    stat_row(196, 180, 270, GXI_STATS, "Total prints", gx_itoa(print_job_timer.getStats().totalPrints));
  #endif
  #if HAS_MEDIA
    gx_button(20, 222, 212, 46, GXI_PLAY, "REPRINT", gx_thumb_has_job() ? GXA(GA_CMD, C_REPRINT) : 0);
  #endif
  gx_button(248, 222, 212, 46, GXI_HOME, "HOME", GXA(GA_CMD, C_DONE_OK), GXB_PRIMARY);
  gx_footer_temps();
}

void gx_status_screen() {
  const bool printing = marlin.printingIsActive() || marlin.printingIsPaused();
  // A requested print that never started (M23 failed): tell the user
  if (gx_start_deadline) {
    if (printing) gx_start_deadline = 0;
    else if (ELAPSED(millis(), gx_start_deadline)) { gx_start_deadline = 0; ui.set_status("Print did not start: file could not be opened"); gx_invalidate(); }
  }
  #if ENABLED(PRINTCOUNTER)
    // Only a print that really finished counts: also catches M524 / host aborts
    static uint16_t finished_at_start = 0;
    if (printing && !gx_was_printing) finished_at_start = print_job_timer.getStats().finishedPrints;
  #endif
  if (printing) { gx_was_printing = true; gx_show_done = false; }
  else if (gx_was_printing) {
    gx_was_printing = false;
    gx_show_done = !gx_aborted && TERN1(PRINTCOUNTER, print_job_timer.getStats().finishedPrints != finished_at_start);
    gx_aborted = false;
  }

  const GxMode mode = printing ? GM_PRINT : (gx_show_done ? GM_DONE : GM_IDLE);
  // Print start switches heaters/motors on: their surge can corrupt the first frames.
  // Repaint the whole screen once, 1s after print start / print end.
  static millis_t heal_at = 0;
  if (mode != gx_mode) {
    if (mode != GM_IDLE) heal_at = millis() + 1000UL;
    gx_mode = mode; gx_clear_screen();
  }
  else if (heal_at && ELAPSED(millis(), heal_at)) { heal_at = 0; gx_clear_screen(); }

  const bool full = gx_full();
  gx_screen_begin();
  switch (mode) {
    case GM_PRINT: print_draw(full); break;
    case GM_DONE:  done_draw(full);  break;
    default:       home_draw(full);  break;
  }
  gx_screen_end();
}

//////////////////////////////////////////////////////////////////////////////
// Tune
//////////////////////////////////////////////////////////////////////////////

static void gx_screen_zoffset();

GX_SCREEN(gx_screen_tune) {
  gx_set_refresh(1000);
  if (gx_full()) gx_header("TUNE");
  char p[8]; sprintf_P(p, PSTR("%u%%"), gx_progress_pct());
  gx_header_info(p, GX_C_ACC, GXF_L);
  constexpr uint16_t W = 226, H = 42;
  gx_row(10, 58, W, H, GXI_SPEED, "Speed", v_str(V_SPEED), "%", GXA(GA_EDIT, V_SPEED), GXT_EDIT);
  gx_row(244, 58, W, H, GXI_FLOW, "Flow", v_str(V_FLOW), "%", GXA(GA_EDIT, V_FLOW), GXT_EDIT);
  gx_row(10, 106, W, H, GXI_NOZZLE, "Nozzle", v_str(V_NOZZLE), v_unit(V_NOZZLE), GXA(GA_GO, 27), GXT_CHEVRON, 0, GX_C_HOT);
  gx_row(244, 106, W, H, GXI_BED, "Bed", v_str(V_BED), v_unit(V_BED), GXA(GA_GO, 28), GXT_CHEVRON);
  gx_row(10, 154, W, H, GXI_FAN, "Fan", v_str(V_FAN), "%", GXA(GA_GO, 29), GXT_CHEVRON);
  gx_row(244, 154, W, H, GXI_BABYSTEP, "Z offset", TERN(HAS_BED_PROBE, gx_ftoa(probe.offset.z, 2), "-"), "mm", GXA(GA_GO, 11), GXT_CHEVRON);
  const bool paused = marlin.printingIsPaused();
  gx_button(10, 204, W, 42, paused ? GXI_PLAY : GXI_PAUSE, paused ? "RESUME" : "PAUSE", gx_full() ? GXA(GA_CMD, C_PAUSE_RESUME) : 0, paused ? GXB_PRIMARY : GXB_NORMAL);
  gx_button(244, 204, W, 42, GXI_STOP, "STOP PRINT", gx_full() ? GXA(GA_CMD, C_STOP_ASK) : 0, GXB_DANGER);
  gx_progress(10, 254, 460, 12, gx_progress_pct());
  gx_footer_temps();
}

//////////////////////////////////////////////////////////////////////////////
// Prepare / Move / Temperature / Filament
//////////////////////////////////////////////////////////////////////////////

static void gx_screen_move();
static void gx_screen_temp();
static void gx_screen_filament();
static void gx_screen_level();

static void tiles4x2(const GxIcon *icons, const char * const *labels, const uint16_t *actions, const char * const *subs=nullptr, const bool *on=nullptr) {
  constexpr uint16_t TW = 109, TH = 101;
  for (uint8_t i = 0; i < 8; i++)
    gx_tile(10 + (i % 4) * (TW + 8), 58 + (i / 4) * (TH + 8), TW, TH, icons[i], 48, labels[i], actions[i], GX_C_ACC, subs ? subs[i] : nullptr, on ? on[i] : false);
}

GX_SCREEN(gx_screen_prepare) {
  gx_set_refresh(1000);
  if (gx_full()) {
    gx_header("PREPARE");
    static const GxIcon icons[8] = { GXI_AUTOHOME, GXI_AXES, GXI_STEPPERS_OFF, GXI_SPOOL, GXI_PREHEAT, GXI_PREHEAT, GXI_SNOW, GXI_LEVELING };
    static const char * const labels[8] = { "AUTO HOME", "MOVE", "MOTORS OFF", "FILAMENT", "PREHEAT ABS", "PREHEAT PLA", "COOLDOWN", "LEVELING" };
    const uint16_t actions[8] = { GXA(GA_CMD, C_HOME), GXA(GA_GO, 4), GXA(GA_CMD, C_MOTORS_OFF), GXA(GA_GO, 6), GXA(GA_CMD, C_PREHEAT1), GXA(GA_CMD, C_PREHEAT0), GXA(GA_CMD, C_COOLDOWN), GXA(GA_GO, 7) };
    tiles4x2(icons, labels, actions);
  }
  char pos[40];
  sprintf_P(pos, PSTR("X%s Y%s Z%s"), gx_ftoa(motion.position.x, 1), gx_ftoa(motion.position.y, 1), gx_ftoa(motion.position.z, 1));
  gx_header_info(pos);
  gx_footer_status();
}

//
// Round XY pad: four ring segments (Y+, X+, Y-, X-) split by diagonal gaps, with a
// round Home XY hub in the middle. Rendered per pixel into the canvas strip.
//
typedef struct __attribute__((__packed__)) {
  uint8_t r_out, r_in, r_hub, gap;
} dpad_t;

static void dpad_render(uint16_t *buffer, uint16_t cw, uint16_t startLine, uint16_t endLine, int16_t cx, int16_t cy, const uint8_t *payload) {
  dpad_t p; memcpy(&p, payload, sizeof(p));
  const int32_t ro2 = int32_t(p.r_out) * p.r_out, ro_in2 = int32_t(p.r_out - 1) * (p.r_out - 1),
                ri2 = int32_t(p.r_in) * p.r_in,   ri_out2 = int32_t(p.r_in + 1) * (p.r_in + 1),
                rh2 = int32_t(p.r_hub) * p.r_hub, rh_in2 = int32_t(p.r_hub - 2) * (p.r_hub - 2);
  const uint16_t line_c = ENDIAN_COLOR(GX_C_LINE), acc_c = ENDIAN_COLOR(GX_C_ACC);
  const int16_t y0 = _MAX(int16_t(startLine), int16_t(cy - p.r_out)), y1 = _MIN(int16_t(endLine), int16_t(cy + p.r_out + 1));
  const int16_t x0 = _MAX(int16_t(0), int16_t(cx - p.r_out)), x1 = _MIN(int16_t(cw), int16_t(cx + p.r_out + 1));
  for (int16_t line = y0; line < y1; line++) {
    const int32_t dy = line - cy;
    uint16_t *row = buffer + (line - startLine) * cw;
    for (int16_t px = x0; px < x1; px++) {
      const int32_t dx = px - cx, d2 = dx * dx + dy * dy;
      if (d2 <= rh2) {                                     // Hub
        row[px] = d2 > rh_in2 ? acc_c : ENDIAN_COLOR(gx_half(ENDIAN_COLOR(row[px]), GX_C_PANEL));
      }
      else if (d2 >= ri2 && d2 <= ro2) {                   // Ring segments
        const int32_t diag = ABS(ABS(dx) - ABS(dy));       // ~ distance to the diagonal * sqrt(2)
        if (diag < p.gap) continue;
        const bool border = d2 > ro_in2 || d2 < ri_out2 || diag < p.gap + 2;
        row[px] = border ? line_c : ENDIAN_COLOR(gx_half(ENDIAN_COLOR(row[px]), GX_C_PANEL));
      }
    }
  }
}

GX_SCREEN(gx_screen_move) {
  gx_set_refresh(500);

  // XY pad: center (116, 164) on screen, outer radius 103
  constexpr uint16_t PX = 12, PY = 60, PS = 208, R = 103, RI = 50, RH = 40;
  constexpr int16_t CX = PX + PS / 2, CY = PY + PS / 2;

  if (gx_full()) {
    gx_header("MOVE");

    gx_canvas(PX, PY, PS, PS);
    const dpad_t pad = { R, RI, RH, 8 };
    tft.add_custom(PS / 2, PS / 2, (void *)dpad_render, &pad, sizeof(pad));
    const uint16_t fh = gx_font_height(GXF_L), mid = (R + RI) / 2;
    gx_text_center(PS / 2 - 40, PS / 2 - mid - fh / 2, 80, "Y+", GX_C_TEXT, GXF_L);
    gx_text_center(PS / 2 - 40, PS / 2 + mid - fh / 2, 80, "Y-", GX_C_TEXT, GXF_L);
    gx_text_center(PS / 2 - mid - 30, PS / 2 - fh / 2, 60, "X-", GX_C_TEXT, GXF_L);
    gx_text_center(PS / 2 + mid - 30, PS / 2 - fh / 2, 60, "X+", GX_C_TEXT, GXF_L);
    gx_icon(PS / 2 - 12, PS / 2 - 24, GXI_HOME, 24);
    gx_text_center(PS / 2 - 30, PS / 2 + 2, 60, "XY", GX_C_ACC);

    gx_touch_add(CX - 60, CY - R, 120, R - RH, GXA(GA_CMD, C_YP));
    gx_touch_add(CX - 60, CY + RH, 120, R - RH, GXA(GA_CMD, C_YM));
    gx_touch_add(CX - R, CY - 60, R - RH, 120, GXA(GA_CMD, C_XM));
    gx_touch_add(CX + RH, CY - 60, R - RH, 120, GXA(GA_CMD, C_XP));
    gx_touch_add(CX - RH, CY - RH, 2 * RH, 2 * RH, GXA(GA_CMD, C_HOME_XY));

    // Z column
    gx_button(234, 104, 64, 54, GXI_CHEV_U, "Z", GXA(GA_CMD, C_ZP), GXB_NORMAL, GXF_L);
    gx_canvas(234, 164, 64, 42);
    gx_icon(8, 9, GXI_HOME, 24);
    gx_text(36, gx_vcenter(42, GXF_S), "Z", GX_C_ACC);
    gx_touch_add(234, 164, 64, 42, GXA(GA_CMD, C_HOME_Z));
    gx_button(234, 212, 64, 54, GXI_CHEV_D, "Z", GXA(GA_CMD, C_ZM), GXB_NORMAL, GXF_L);

    // E column
    #if HAS_EXTRUDERS
      gx_button(306, 104, 64, 54, GXI_CHEV_U, "E", GXA(GA_CMD, C_EM), GXB_NORMAL, GXF_L);   // Up = retract
      gx_canvas(306, 164, 64, 42);
      gx_panel(0, 0, 64, 42);
      gx_icon(20, 9, GXI_EXTRUDER, 24, GX_C_DIM);
      gx_button(306, 212, 64, 54, GXI_CHEV_D, "E", GXA(GA_CMD, C_EP), GXB_NORMAL, GXF_L);   // Down = extrude
    #endif

    gx_button(378, 222, 92, 44, GXI_STEPPERS_OFF, "OFF", GXA(GA_CMD, C_MOTORS_OFF));
  }

  // Distance selector (segmented)
  gx_canvas(234, 58, 236, 40);
  gx_panel(0, 0, 236, 40, GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, 19);
  static const char * const dl[4] = { "0.1", "1", "10", "100" };
  const float dv[4] = { 0.1f, 1, 10, 100 };
  for (uint8_t i = 0; i < 4; i++) {
    const uint16_t sx = 3 + i * 58;
    const bool sel = gx_move_dist == dv[i];
    if (sel) gx_panel(sx, 3, 56, 34, GX_C_ACC, GXP_BLEND | GXP_BORDER, GX_C_ACC, 16);
    gx_text_center(sx, gx_vcenter(40, GXF_S), 56, dl[i], sel ? GX_C_TEXT : GX_C_DIM);
    gx_touch_add(234 + sx, 58, 56, 40, GXA(GA_CMD, C_DIST0 + i));
  }

  // Position
  gx_canvas(378, 104, 92, 112);
  gx_panel(0, 0, 92, 112);
  static const char * const names[4] = { "X", "Y", "Z", "E" };
  const float vals[4] = { motion.position.x, motion.position.y, motion.position.z, TERN(HAS_EXTRUDERS, motion.position.e, 0) };
  for (uint8_t i = 0; i < 4; i++) {
    const uint16_t y = 6 + i * 26;
    const bool unhomed = i < 3 && motion.axis_should_home(AxisEnum(i));
    gx_text(8, y, names[i], GX_C_DIM);
    gx_text_right(86, y, gx_ftoa(vals[i], 1), unhomed ? GX_C_HOT : GX_C_TEXT);
  }
  gx_footer_status();
}

static void temp_card(const uint8_t col, const GxIcon icon, const char *label, const char *cur, const char *tgt, const uint16_t color, const uint8_t value) {
  const uint16_t x = 10 + col * 156;
  gx_canvas(x, 58, 148, 168);
  gx_panel(0, 0, 148, 168);
  gx_icon(58, 8, icon, 32, color);
  gx_text_center(0, 44, 148, label, GX_C_DIM);
  gx_text_center(0, 66, 148, cur, color, GXF_L);
  gx_text_center(0, 100, 148, tgt, GX_C_DIM);
  gx_panel(16, 128, 116, 32);
  gx_icon(30, 134, GXI_EDIT, 20);
  gx_text(58, 128 + gx_vcenter(32, GXF_S), "EDIT", GX_C_TEXT);
  gx_touch_add(x, 58, 148, 168, GXA(GA_GO, value == V_NOZZLE ? 27 : value == V_BED ? 28 : 29));
}

GX_SCREEN(gx_screen_temp) {
  gx_set_refresh(1000);
  if (gx_full()) gx_header("TEMPERATURE");
  char a[12], b[20];
  #if HAS_HOTEND
    const celsius_t nc = thermalManager.wholeDegHotend(0), nt = thermalManager.degTargetHotend(0);
    sprintf_P(a, PSTR("%i\xC2\xB0"), int(nc)); sprintf_P(b, PSTR("Target %i\xC2\xB0"), int(nt));
    temp_card(0, GXI_NOZZLE, "NOZZLE", a, b, gx_heat_color(nc), V_NOZZLE);
  #endif
  #if HAS_HEATED_BED
    const celsius_t bc = thermalManager.wholeDegBed(), bt = thermalManager.degTargetBed();
    sprintf_P(a, PSTR("%i\xC2\xB0"), int(bc)); sprintf_P(b, PSTR("Target %i\xC2\xB0"), int(bt));
    temp_card(1, GXI_BED, "BED", a, b, gx_heat_color(bc), V_BED);
  #endif
  #if HAS_FAN
    sprintf_P(a, PSTR("%u%%"), unsigned(fans[0].speed_pct()));
    temp_card(2, GXI_FAN, "FAN", a, "Speed", GX_C_ACC, V_FAN);
  #endif
  if (gx_full()) {
    gx_button(10, 232, 148, 38, GXI_PREHEAT, "PLA", GXA(GA_CMD, C_PREHEAT0));
    gx_button(166, 232, 148, 38, GXI_PREHEAT, "ABS", GXA(GA_CMD, C_PREHEAT1));
    gx_button(322, 232, 148, 38, GXI_SNOW, "COOLDOWN", GXA(GA_CMD, C_COOLDOWN));
  }
  gx_footer_status();
}

GX_SCREEN(gx_screen_filament) {
  gx_set_refresh(1000);
  if (gx_full()) {
    gx_header("FILAMENT");
    static const GxIcon icons[4] = { GXI_LOAD, GXI_UNLOAD, GXI_PURGE, GXI_CHANGE };
    static const char * const labels[4] = { "LOAD", "UNLOAD", "PURGE", "CHANGE" };
    static const uint8_t cmds[4] = { C_LOAD, C_UNLOAD, C_PURGE, C_CHANGE };
    for (uint8_t i = 0; i < 4; i++)
      gx_tile(200 + (i & 1) * 139, 58 + (i >> 1) * 109, 131, 101, icons[i], 48, labels[i], GXA(GA_CMD, cmds[i]));
  }
  #if HAS_HOTEND
    const celsius_t nc = thermalManager.wholeDegHotend(0), nt = thermalManager.degTargetHotend(0);
    const bool cold = gx_too_cold();
    char a[12], b[20];
    sprintf_P(a, PSTR("%i\xC2\xB0"), int(nc)); sprintf_P(b, PSTR("Target %i\xC2\xB0"), int(nt));
    gx_canvas(10, 58, 182, 210);
    gx_panel(0, 0, 182, 210);
    gx_icon(67, 10, GXI_NOZZLE, 48, cold ? GX_C_HOT : GX_C_OK);
    gx_text_center(0, 64, 182, "NOZZLE", GX_C_DIM);
    gx_text_center(0, 86, 182, a, cold ? GX_C_HOT : GX_C_ACC, GXF_L);
    gx_text_center(0, 120, 182, b, GX_C_DIM);
    gx_touch_add(10, 58, 182, 150, GXA(GA_GO, 27));
    if (gx_full()) {
      gx_button(18, 222, 80, 38, GX_NO_ICON, "PLA", GXA(GA_CMD, C_PREHEAT0));
      gx_button(104, 222, 80, 38, GX_NO_ICON, "ABS", GXA(GA_CMD, C_PREHEAT1));
    }
    if (ui.has_status()) gx_footer_status();
    else gx_footer_msg(cold ? "Heat the nozzle to extrude" : "Ready to extrude", cold ? GXI_WARNING : GXI_CHECK_C, cold ? GX_C_HOT : GX_C_OK);
  #endif
}

//////////////////////////////////////////////////////////////////////////////
// Leveling / Mesh / Z offset / BLTouch
//////////////////////////////////////////////////////////////////////////////

static void gx_screen_bltouch();
static void gx_screen_meshedit();

GX_SCREEN(gx_screen_level) {
  if (!gx_full()) return;
  gx_header("LEVELING");
  const bool lev = TERN0(HAS_LEVELING, planner.leveling_active);
  char fade[12] = "-";
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    strlcpy(fade, gx_cat(gx_ftoa(planner.z_fade_height, 1), " mm"), sizeof(fade));
  #endif
  static const GxIcon icons[8] = { GXI_LEVELING, GXI_MESH, GXI_MESH_EDIT, GXI_CHECK_C, GXI_LAYERS, GXI_TRAMMING, GXI_ZOFFSET, GXI_PROBE };
  static const char * const labels[8] = { "AUTO LEVEL", "MESH VIEW", "EDIT MESH", "LEVELING", "FADE", "TRAMMING", "Z OFFSET", "BLTOUCH" };
  const char * const subs[8] = { nullptr, nullptr, nullptr, lev ? "ON" : "OFF", fade, nullptr, nullptr, nullptr };
  const bool on[8] = { false, false, false, lev, false, false, false, false };
  const uint16_t actions[8] = { GXA(GA_CMD, C_AUTOLEVEL), GXA(GA_GO, 8), GXA(GA_GO, 9), GXA(GA_CMD, C_LEVEL_TOGGLE), GXA(GA_EDIT, V_FADE), GXA(GA_GO, 10), GXA(GA_GO, 11), GXA(GA_GO, 12) };
  tiles4x2(icons, labels, actions, subs, on);
  char info[24];
  sprintf_P(info, PSTR("%ux%u %s"), unsigned(TERN(HAS_MESH, GRID_MAX_POINTS_X, 0)), unsigned(TERN(HAS_MESH, GRID_MAX_POINTS_Y, 0)), lev ? "ON" : "OFF");
  gx_header_info(info);
  gx_footer_temps();
}

#if HAS_MESH
  static uint16_t mesh_color(const float v, const float mn, const float mx) {
    const float t = mx > mn ? (v - mn) / (mx - mn) : 0.5f;
    const uint16_t A = 0x2AFF /* #2A5CFF */, B = GX_C_ACC, C = GX_C_HOT;
    return t < 0.5f ? gx_blend(A, B, uint8_t(t * 2 * 255)) : gx_blend(B, C, uint8_t((t - 0.5f) * 2 * 255));
  }

  static void mesh_grid(const uint16_t x, const uint16_t y, const uint16_t size, const bool edit) {
    constexpr uint8_t NX = GRID_MAX_POINTS_X, NY = GRID_MAX_POINTS_Y;
    float mn = 999, mx = -999;
    for (uint8_t i = 0; i < NX; i++) for (uint8_t j = 0; j < NY; j++) {
      const float v = bedlevel.z_values[i][j];
      if (!isnan(v)) { NOMORE(mn, v); NOLESS(mx, v); }
    }
    const uint16_t cw = (size - (NX - 1) * 2) / NX, ch = (size - (NY - 1) * 2) / NY;
    gx_canvas(x, y, size, size);
    for (uint8_t j = 0; j < NY; j++) for (uint8_t i = 0; i < NX; i++) {
      const uint16_t cx = i * (cw + 2), cy = (NY - 1 - j) * (ch + 2);   // front row at the bottom
      const float v = bedlevel.z_values[i][j];
      const bool sel = edit && gx_mesh_cell == j * NX + i;
      if (isnan(v)) {
        gx_panel(cx, cy, cw, ch, sel ? GX_C_TEXT : GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, 3);
        gx_text_center(cx, cy + gx_vcenter(ch, GXF_S), cw, "--", GX_C_DIM);
      }
      else {
        gx_panel(cx, cy, cw, ch, sel ? GX_C_TEXT : mesh_color(v, mn, mx), GXP_SOLID | GXP_BORDER, mesh_color(v, mn, mx), 3);
        gx_text_center(cx, cy + gx_vcenter(ch, GXF_S), cw, gx_ftoa(v, 2), GX_C_BG);
      }
      if (edit) gx_touch_add(x + cx, y + cy, cw, ch, GXA(GA_CELL, j * NX + i));
    }
    if (!edit) {
      // Min / max / range
      gx_canvas(240, 60, 230, 120);
      static const char * const l[3] = { "MIN", "MAX", "RANGE" };
      const bool any = mx >= mn;
      const float vals[3] = { mn, mx, mx - mn };
      for (uint8_t k = 0; k < 3; k++) {
        gx_panel(k * 78, 0, 72, 56);
        gx_text_center(k * 78, 6, 72, l[k], GX_C_DIM);
        gx_text_center(k * 78, 28, 72, any ? gx_ftoa(vals[k], 2) : "--", GX_C_TEXT);
      }
      gx_panel(0, 72, 230, 10, GX_C_LINE, GXP_BORDER, 0, 4);
      for (uint16_t px = 2; px < 228; px += 6) gx_bar(px, 74, _MIN(uint16_t(6), uint16_t(228 - px)), 6, mesh_color(px + 3, 2, 227));  // 38 bars, not 226
      gx_text(0, 90, "low", GX_C_DIM);
      gx_text_right(230, 90, "high", GX_C_DIM);
    }
  }
#endif

GX_SCREEN(gx_screen_mesh) {
  gx_set_refresh(1000);
  if (gx_full()) {
    gx_header("MESH VIEW");
    gx_button(240, 216, 110, 50, GXI_LEVELING, "PROBE", GXA(GA_CMD, C_AUTOLEVEL));
    gx_button(358, 216, 112, 50, GXI_STORE, "SAVE", GXA(GA_CMD, C_SAVE_ASK), GXB_PRIMARY);
  }
  #if HAS_MESH
    mesh_grid(16, 58, 210, false);
  #endif
  gx_footer_status();
}

GX_SCREEN(gx_screen_meshedit) {
  if (gx_full()) gx_header("EDIT MESH");
  #if HAS_MESH
    mesh_grid(16, 58, 210, true);
    const uint8_t i = gx_mesh_cell % (GRID_MAX_POINTS_X), j = gx_mesh_cell / (GRID_MAX_POINTS_X);
    const float v = j < GRID_MAX_POINTS_Y ? bedlevel.z_values[i][j] : NAN;
    gx_canvas(240, 60, 230, 100);
    char pt[16]; sprintf_P(pt, PSTR("POINT %u, %u"), i, j);
    gx_text(0, 0, pt, GX_C_DIM);
    gx_text(0, 30, isnan(v) ? "--" : gx_ftoa(v, 3), GX_C_ACC, GXF_L);
    gx_text(0, 72, "step 0.025 mm", GX_C_DIM);
    if (gx_full()) {
      gx_button(240, 166, 111, 50, GXI_MINUS, nullptr, GXA(GA_CMD, C_MESH_M) | GX_REPEAT);
      gx_button(359, 166, 111, 50, GXI_PLUS, nullptr, GXA(GA_CMD, C_MESH_P) | GX_REPEAT);
      gx_button(240, 224, 230, 44, GXI_CHECK, "DONE", GXA(GA_BACK, 0), GXB_PRIMARY);
    }
  #endif
  gx_footer_msg("Tap a point, then adjust it. Save to keep.");
}

GX_SCREEN(gx_screen_zoffset) {
  if (gx_full()) {
    gx_header("Z OFFSET");
    gx_canvas(10, 64, 150, 150);
    gx_icon(35, 6, GXI_ZOFFSET, 80);
    gx_text_center(0, 104, 150, "PROBE Z OFFSET", GX_C_DIM);
    gx_button(170, 148, 146, 64, GXI_CHEV_U, "Z +", GXA(GA_CMD, C_ZUP) | GX_REPEAT);
    gx_button(324, 148, 146, 64, GXI_CHEV_D, "Z -", GXA(GA_CMD, C_ZDOWN) | GX_REPEAT);
    gx_button(170, 222, 146, 46, GXI_AUTOHOME, "HOME", GXA(GA_CMD, C_ZHOME));
    gx_button(324, 222, 146, 46, GXI_STORE, "SAVE", GXA(GA_CMD, C_SAVE), GXB_PRIMARY);
  }
  gx_canvas(170, 58, 300, 42);
  const char *val = TERN(HAS_BED_PROBE, gx_ftoa(probe.offset.z, 2), "-");
  const uint16_t vw = gx_text_width(val, GXF_L) + 8 + gx_text_width("mm");
  const int16_t sx = (300 - vw) / 2;
  const uint16_t w1 = gx_text(sx, gx_vcenter(42, GXF_L), val, GX_C_ACC, GXF_L);
  gx_text(sx + w1 + 8, gx_vcenter(42, GXF_S) + 4, "mm", GX_C_DIM);
  static const char * const sl[3] = { "0.01", "0.05", "0.10" };
  const float sv[3] = { 0.01f, 0.05f, 0.10f };
  for (uint8_t k = 0; k < 3; k++)
    gx_button(170 + k * 102, 104, 96, 36, GX_NO_ICON, sl[k], gx_full() ? GXA(GA_CMD, C_ZSTEP0 + k) : 0, fabsf(gx_zstep - sv[k]) < 0.001f ? GXB_SELECTED : GXB_NORMAL);
  gx_footer_msg("Paper test: a light drag on the paper is right");
}

GX_SCREEN(gx_screen_bltouch) {
  if (!gx_full()) return;
  gx_header("BLTOUCH");
  static const GxIcon icons[6] = { GXI_DEPLOY, GXI_STOW, GXI_CHECK_C, GXI_RESTORE, GXI_PROBE, GXI_ZOFFSET };
  static const char * const labels[6] = { "DEPLOY", "STOW", "SELF-TEST", "RESET ALARM", "OFFSETS", "Z OFFSET" };
  const uint16_t actions[6] = { GXA(GA_CMD, C_DEPLOY), GXA(GA_CMD, C_STOW), GXA(GA_CMD, C_SELFTEST), GXA(GA_CMD, C_ALARM), GXA(GA_GO, 15), GXA(GA_GO, 11) };
  for (uint8_t i = 0; i < 6; i++)
    gx_tile(10 + (i % 3) * 156, 58 + (i / 3) * 109, 148, 101, icons[i], 48, labels[i], actions[i]);
  gx_footer_status();
}

//////////////////////////////////////////////////////////////////////////////
// Settings
//////////////////////////////////////////////////////////////////////////////

static void gx_screen_settemp();
static void gx_screen_pid();
static void gx_screen_filset();
static void gx_screen_probeset();
static void gx_screen_about();

GX_SCREEN(gx_screen_settings) {
  if (!gx_full()) return;
  gx_header("SETTINGS");
  const bool rec = TERN0(POWER_LOSS_RECOVERY, recovery.enabled);
  static const GxIcon icons[11] = { GXI_THERMO, GXI_MOTION, GXI_SPOOL, GXI_PROBE, GXI_LEVELING, GXI_BOLT, GXI_DISPLAY, GXI_STORE, GXI_GEAR, GXI_EDIT, GXI_INFO };
  const char * const labels[11] = { "Temperature", "Motion", "Filament", "Probe", "Leveling", rec ? "Recovery on" : "Recovery off", "Display", "EEPROM", "More", "G-code", "About" };
  const uint16_t actions[11] = { GXA(GA_GO, 13), GXA(GA_GO, 16), GXA(GA_GO, 14), GXA(GA_GO, 15), GXA(GA_GO, 7), GXA(GA_CMD, C_RECOVERY), GXA(GA_GO, 26),
                                 GXA(GA_GO, 30), GXA(GA_GO, 31), GXA(GA_GO, 32), GXA(GA_GO, 17) };
  constexpr uint16_t TW = 109, TH = 64;
  for (uint8_t i = 0; i < 11; i++) {
    const bool on = i == 5 && rec;
    gx_tile(10 + (i % 4) * (TW + 8), 58 + (i / 4) * (TH + 6), TW, TH, icons[i], 24, labels[i], actions[i], i == 5 && !on ? GX_C_DIM : GX_C_ACC, nullptr, on);
  }
  gx_footer_temps();
}

GX_SCREEN(gx_screen_settemp) {
  if (!gx_full()) return;
  gx_header("PREHEAT");
  gx_canvas(10, 56, 460, 22);
  gx_text_center(0, 2, 226, "PLA", GX_C_DIM);
  gx_text_center(234, 2, 226, "ABS", GX_C_DIM);
  constexpr uint16_t W = 226, H = 42;
  gx_row(10, 80, W, H, GXI_NOZZLE, "Nozzle", v_str(V_P0N), v_unit(V_P0N), GXA(GA_EDIT, V_P0N), GXT_EDIT);
  gx_row(244, 80, W, H, GXI_NOZZLE, "Nozzle", v_str(V_P1N), v_unit(V_P1N), GXA(GA_EDIT, V_P1N), GXT_EDIT);
  gx_row(10, 128, W, H, GXI_BED, "Bed", v_str(V_P0B), v_unit(V_P0B), GXA(GA_EDIT, V_P0B), GXT_EDIT);
  gx_row(244, 128, W, H, GXI_BED, "Bed", v_str(V_P1B), v_unit(V_P1B), GXA(GA_EDIT, V_P1B), GXT_EDIT);
  gx_row(10, 176, W, H, GXI_FAN, "Fan", v_str(V_P0F), "%", GXA(GA_EDIT, V_P0F), GXT_EDIT);
  gx_row(244, 176, W, H, GXI_FAN, "Fan", v_str(V_P1F), "%", GXA(GA_EDIT, V_P1F), GXT_EDIT);
  gx_button(10, 226, W, 44, GXI_PID, "PID HOTEND", GXA(GA_GO, 19));
  gx_button(244, 226, W, 44, GXI_PID, "PID BED", GXA(GA_GO, 20));
  gx_footer_msg("Store settings to keep the presets");
}

GX_SCREEN(gx_screen_pid) {
  gx_set_refresh(1000);
  if (gx_full()) {
    gx_header("PID TUNE");
    gx_button(10, 58, 110, 36, GX_NO_ICON, "HOTEND", GXA(GA_CMD, C_PID_HOT), gx_pid_bed ? GXB_NORMAL : GXB_SELECTED);
    gx_button(126, 58, 110, 36, GX_NO_ICON, "BED", GXA(GA_CMD, C_PID_BED), gx_pid_bed ? GXB_SELECTED : GXB_NORMAL);
    gx_row(10, 102, 226, 42, gx_pid_bed ? GXI_BED : GXI_NOZZLE, "Target", v_str(V_PID_T), v_unit(V_PID_T), GXA(GA_EDIT, V_PID_T), GXT_EDIT);
    gx_row(10, 150, 226, 42, GXI_REFRESH, "Cycles", v_str(V_PID_C), nullptr, GXA(GA_EDIT, V_PID_C), GXT_EDIT);
    gx_button(10, 204, 110, 46, GXI_PLAY, "START", GXA(GA_CMD, C_PID_START), GXB_PRIMARY);
    gx_button(126, 204, 110, 46, GXI_STORE, "SAVE", GXA(GA_CMD, C_SAVE_ASK));
  }
  gx_canvas(244, 58, 226, 192);
  gx_panel(0, 0, 226, 192);
  gx_icon(97, 12, gx_pid_bed ? GXI_BED : GXI_NOZZLE, 32, GX_C_HOT);
  char t[16];
  #if HAS_HEATED_BED
    if (gx_pid_bed) sprintf_P(t, PSTR("%i\xC2\xB0"), int(thermalManager.wholeDegBed())); else
  #endif
  sprintf_P(t, PSTR("%i\xC2\xB0"), int(TERN0(HAS_HOTEND, thermalManager.wholeDegHotend(0))));
  gx_text_center(0, 56, 226, t, GX_C_HOT, GXF_L);
  gx_text_center(8, 100, 210, "Results are applied", GX_C_DIM);
  gx_text_center(8, 124, 210, "automatically (U1).", GX_C_DIM);
  gx_text_center(8, 154, 210, "Then press SAVE.", GX_C_DIM);
  gx_footer_status();
}

GX_SCREEN(gx_screen_filset) {
  if (!gx_full()) return;
  gx_header("FILAMENT SETUP");
  constexpr uint16_t W = 226, H = 46;
  gx_row(10, 58, W, H, GXI_LA, "K factor", TERN(LIN_ADVANCE, v_str(V_LA_K), "-"), nullptr, TERN(LIN_ADVANCE, GXA(GA_EDIT, V_LA_K), 0), TERN(LIN_ADVANCE, GXT_EDIT, GXT_NONE));
  #if ENABLED(CONFIGURE_FILAMENT_CHANGE)
    gx_row(244, 58, W, H, GXI_UNLOAD, "Unload", v_str(V_UNLOAD), "mm", GXA(GA_EDIT, V_UNLOAD), GXT_EDIT);
    gx_row(10, 110, W, H, GXI_LOAD, "Fast load", v_str(V_LOAD), "mm", GXA(GA_EDIT, V_LOAD), GXT_EDIT);
  #endif
  #if HAS_FILAMENT_SENSOR
    gx_row(244, 110, W, H, GXI_RUNOUT, "Sensor", nullptr, nullptr, GXA(GA_CMD, C_RUNOUT), runout.enabled ? GXT_ON : GXT_OFF);
  #endif
  #if HAS_FILAMENT_RUNOUT_DISTANCE
    gx_row(10, 162, W, H, GXI_RULER, "Distance", v_str(V_RUNOUT_D), "mm", GXA(GA_EDIT, V_RUNOUT_D), GXT_EDIT);
  #endif
  gx_footer_msg("Store settings to keep the changes");
}

GX_SCREEN(gx_screen_probeset) {
  if (!gx_full()) return;
  gx_header("PROBE");
  constexpr uint16_t W = 226, H = 46;
  #if HAS_BED_PROBE
    gx_row(10, 58, W, H, GX_NO_ICON, "X offset", v_str(V_PROBE_X), "mm", GXA(GA_EDIT, V_PROBE_X), GXT_EDIT, 'X');
    gx_row(244, 58, W, H, GX_NO_ICON, "Y offset", v_str(V_PROBE_Y), "mm", GXA(GA_EDIT, V_PROBE_Y), GXT_EDIT, 'Y');
    gx_row(10, 110, W, H, GX_NO_ICON, "Z offset", gx_ftoa(probe.offset.z, 2), "mm", GXA(GA_GO, 11), GXT_CHEVRON, 'Z');
  #endif
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    gx_row(244, 110, W, H, GXI_LAYERS, "Fade", v_str(V_FADE), "mm", GXA(GA_EDIT, V_FADE), GXT_EDIT);
  #endif
  gx_tile(10, 164, 148, 104, GXI_PROBE, 48, "BLTOUCH", GXA(GA_GO, 12));
  gx_tile(166, 164, 148, 104, GXI_ZOFFSET, 48, "Z OFFSET", GXA(GA_GO, 11));
  gx_tile(322, 164, 148, 104, GXI_TRAMMING, 48, "TRAMMING", GXA(GA_GO, 10));
  gx_footer_status();
}

// Motion: summary of the motion settings, each row opens Marlin's own editor list
GX_SCREEN(gx_screen_motion) {
  if (!gx_full()) return;
  gx_header("MOTION");
  constexpr uint16_t W = 460, H = 40, Y0 = 58, DY = 42;
  char v[48];
  uint8_t r = 0;
  #if ENABLED(EDITABLE_STEPS_PER_UNIT)
    snprintf_P(v, sizeof(v), PSTR("X%s Y%s Z%s E%s"), gx_ftoa(planner.settings.axis_steps_per_mm[X_AXIS], 1), gx_ftoa(planner.settings.axis_steps_per_mm[Y_AXIS], 1),
               gx_ftoa(planner.settings.axis_steps_per_mm[Z_AXIS], 0), gx_ftoa(planner.settings.axis_steps_per_mm[E_AXIS], 1));
    gx_row(10, Y0 + DY * r++, W, H, GXI_RULER, "Steps/mm", v, nullptr, GXA(GA_GO, 21), GXT_CHEVRON);
  #endif
  #if DISABLED(SLIM_LCD_MENUS)
    snprintf_P(v, sizeof(v), PSTR("X%s Y%s Z%s E%s"), gx_ftoa(planner.settings.max_feedrate_mm_s[X_AXIS], 0), gx_ftoa(planner.settings.max_feedrate_mm_s[Y_AXIS], 0),
               gx_ftoa(planner.settings.max_feedrate_mm_s[Z_AXIS], 0), gx_ftoa(planner.settings.max_feedrate_mm_s[E_AXIS], 0));
    gx_row(10, Y0 + DY * r++, W, H, GXI_SPEED, "Max speed", v, "mm/s", GXA(GA_GO, 22), GXT_CHEVRON);
    snprintf_P(v, sizeof(v), PSTR("P%s R%s T%s"), gx_ftoa(planner.settings.acceleration, 0), gx_ftoa(planner.settings.retract_acceleration, 0), gx_ftoa(planner.settings.travel_acceleration, 0));
    gx_row(10, Y0 + DY * r++, W, H, GXI_ACCEL, "Acceleration", v, nullptr, GXA(GA_GO, 23), GXT_CHEVRON);
    #if ENABLED(CLASSIC_JERK)
      snprintf_P(v, sizeof(v), PSTR("X%s Y%s Z%s"), gx_ftoa(planner.max_jerk.x, 1), gx_ftoa(planner.max_jerk.y, 1), gx_ftoa(planner.max_jerk.z, 2));
      gx_row(10, Y0 + DY * r++, W, H, GXI_JERK, "Jerk", v, nullptr, GXA(GA_GO, 24), GXT_CHEVRON);
    #endif
    #if ENABLED(SHAPING_MENU)
      gx_row(10, Y0 + DY * r++, W, H, GXI_PID, "Input shaping", nullptr, nullptr, GXA(GA_GO, 25), GXT_CHEVRON);
    #endif
  #endif
  UNUSED(r); UNUSED(v);
  gx_footer_msg("Store settings to keep the changes");
}

// Display: brightness, screen sleep, touch beep, touch calibration
GX_SCREEN(gx_screen_display) {
  if (!gx_full()) return;
  gx_header("DISPLAY");
  constexpr uint16_t W = 460, H = 42;
  uint16_t y = 58;
  #if PIN_EXISTS(TFT_BACKLIGHT)
    if (PWM_PIN(TFT_BACKLIGHT_PIN)) { gx_row(10, y, W, H, GXI_DISPLAY, "Brightness", v_str(V_BRIGHT), "%", GXA(GA_EDIT, V_BRIGHT), GXT_EDIT); y += 48; }
  #endif
  #if HAS_DISPLAY_SLEEP
    gx_row(10, y, W, H, GXI_MOON, "Screen sleep after", ui.sleep_timeout_minutes ? v_str(V_SLEEP) : "Never", ui.sleep_timeout_minutes ? "min" : nullptr,
           TERN(EDITABLE_DISPLAY_TIMEOUT, GXA(GA_EDIT, V_SLEEP), 0), TERN(EDITABLE_DISPLAY_TIMEOUT, GXT_EDIT, GXT_NONE));
    y += 48;
  #endif
  gx_row(10, y, W, H, GXI_BELL, "Touch beep", nullptr, nullptr, GXA(GA_CMD, C_BEEP), gx_beep_on ? GXT_ON : GXT_OFF);
  y += 48;
  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    gx_button(10, y + 6, W, 46, GXI_TARGET, "TOUCH CALIBRATION", GXA(GA_GO, 18));
  #endif
  gx_footer_msg("Sleep time is kept by Store settings");
}

static void kv_row(const uint16_t y, const char *k, const char *v) {
  gx_text(12, y, k, GX_C_DIM);
  gx_text_right(214, y, v, GX_C_TEXT);
}

GX_SCREEN(gx_screen_about) {
  if (!gx_full()) return;
  gx_header("ABOUT");
  gx_canvas(10, 58, 226, 210);
  gx_panel(0, 0, 226, 210);
  gx_text(12, 8, "PRINTER", GX_C_DIM);
  kv_row(40, "Firmware", "Marlin " SHORT_BUILD_VERSION);
  kv_row(72, "Machine", MACHINE_NAME);
  char bed[24]; sprintf_P(bed, PSTR("%ux%u mm"), unsigned(X_BED_SIZE), unsigned(Y_BED_SIZE));
  kv_row(104, "Bed", bed);
  kv_row(136, "Board", "Robin Nano V3");
  kv_row(168, "Display", "TS35 480x320");
  #if ENABLED(PRINTCOUNTER)
    const printStatistics st = print_job_timer.getStats();
    gx_canvas(244, 58, 226, 210);
    gx_panel(0, 0, 226, 210);
    gx_text(12, 8, "STATISTICS", GX_C_DIM);
    kv_row(40, "Prints", gx_itoa(st.totalPrints));
    kv_row(72, "Completed", gx_itoa(st.finishedPrints));
    kv_row(104, "Print time", gx_time(st.printTime));
    kv_row(136, "Longest", gx_time(st.longestPrint));
    kv_row(168, "Filament", gx_cat(gx_ftoa(st.filamentUsed / 1000.0f, 1), " m"));
  #endif
  gx_footer_temps();
}

//////////////////////////////////////////////////////////////////////////////
// Storage / files / file details
//////////////////////////////////////////////////////////////////////////////

#if HAS_MEDIA

  static void gx_screen_details();

  static bool gx_on_usb() {
    #if ENABLED(MULTI_VOLUME)
      return card.diskIODriver() == (DiskIODriver*)&card.media_driver_usbFlash;
    #else
      return true;
    #endif
  }

  GX_SCREEN(gx_screen_storage) {
    if (gx_full()) {
      gx_header("STORAGE");
      gx_button(10, 224, 226, 44, GXI_REFRESH, "REFRESH", GXA(GA_CMD, C_REFRESH));
      gx_button(244, 224, 226, 44, GXI_EJECT, "EJECT", GXA(GA_CMD, C_EJECT));
    }
    const bool mounted = card.isMounted(), usb = gx_on_usb();
    for (uint8_t v = 0; v < 2; v++) {
      const bool is_usb = v == 0, active = mounted && usb == is_usb;
      const uint16_t x = 10 + v * 234;
      gx_canvas(x, 58, 226, 158);
      gx_panel(0, 0, 226, 158, active ? GX_C_ACC : GX_C_LINE);
      gx_icon(81, 14, is_usb ? GXI_USB : GXI_SD, 64);
      gx_text_center(0, 86, 226, is_usb ? "USB drive" : "SD card", GX_C_TEXT, GXF_L);
      gx_text_center(0, 122, 226, active ? "Ready" : "Tap to open", active ? GX_C_OK : GX_C_DIM);
      gx_touch_add(x, 58, 226, 158, GXA(GA_CMD, is_usb ? C_VOL_USB : C_VOL_SD));
    }
    gx_footer_status();
  }

  // Slot 0 is ".." inside a folder
  static int16_t files_offset() { return card.flag.workDirIsRoot ? 0 : 1; }
  static int16_t files_total() { return card.get_num_items() + files_offset(); }

  GX_SCREEN(gx_screen_files) {
    if (!gx_full()) return;
    const int16_t total = files_total(), pages = _MAX(int16_t(1), int16_t((total + 7) / 8));
    NOMORE(gx_file_page, pages - 1);
    gx_header(gx_on_usb() ? "USB" : "SD CARD");
    char info[16]; sprintf_P(info, PSTR("%i / %i"), gx_file_page + 1, pages);
    gx_canvas(240, 8, 116, 36);
    gx_text_right(110, gx_vcenter(36, GXF_S), info, GX_C_DIM);
    gx_button(362, 8, 50, 36, GXI_CHEV_U, nullptr, GXA(GA_PAGE, 0));
    gx_button(420, 8, 50, 36, GXI_CHEV_D, nullptr, GXA(GA_PAGE, 1));

    constexpr uint16_t TW = 109, TH = 101;
    for (uint8_t s = 0; s < 8; s++) {
      const int16_t idx = gx_file_page * 8 + s;
      if (idx >= total) break;
      const uint16_t x = 10 + (s % 4) * (TW + 8), y = 58 + (s / 4) * (TH + 8);
      gx_canvas(x, y, TW, TH);
      gx_panel(0, 0, TW, TH);
      const char *label;
      if (idx < files_offset()) {
        gx_icon((TW - 48) / 2, 12, GXI_FOLDER_UP, 48, GX_C_DIM);
        label = "..";
      }
      else {
        card.selectFileByIndexSorted(idx - files_offset());
        if (card.flag.filenameIsDir) gx_icon((TW - 48) / 2, 12, GXI_FOLDER, 48, GX_C_HOT);
        else gx_thumb((TW - 48) / 2, 12, 48, card.filename);
        label = gx_strip_gcode(card.longest_filename());
      }
      gx_text_center(4, 68, TW - 8, label, GX_C_TEXT);
      gx_touch_add(x, y, TW, TH, GXA(GA_FILE, s));
    }
    char msg[32]; sprintf_P(msg, PSTR("%i items"), int(card.get_num_items()));
    gx_footer_msg(msg, GXI_FILE);
  }

  static void gx_file_tap(const uint8_t slot) {
    const int16_t idx = gx_file_page * 8 + slot;
    if (idx >= files_total()) return;
    if (idx < files_offset()) { card.cdup(); gx_file_page = 0; gx_invalidate(); return; }
    card.selectFileByIndexSorted(idx - files_offset());
    if (card.flag.filenameIsDir) { card.cd(card.filename); gx_file_page = 0; gx_invalidate(); return; }
    strlcpy(gx_sel_dos, card.filename, sizeof(gx_sel_dos));
    strlcpy(gx_sel_long, card.longest_filename(), sizeof(gx_sel_long));
    gx_sel_size = 0;
    MediaFile f;
    if (f.open(&card.getWorkDir(), gx_sel_dos, O_READ)) { gx_sel_size = f.fileSize(); f.close(); }
    gx_go(gx_screen_details);
  }

  GX_SCREEN(gx_screen_details) {
    if (!gx_full()) return;
    gx_header("PRINT FILE");
    gx_canvas(10, 64, 160, 160);
    gx_panel(0, 0, 160, 160);
    gx_thumb(5, 5, 150, gx_sel_dos);
    // Name on up to two lines
    gx_canvas(182, 60, 288, 64);
    const char *name = gx_strip_gcode(gx_sel_long);
    char l1[48]; strlcpy(l1, name, sizeof(l1));
    uint8_t cut = strlen(l1);
    while (cut > 1 && gx_text_width(l1, GXF_S) > 284) {
      l1[--cut] = '\0';
      while (cut > 1 && (uint8_t(name[cut]) & 0xC0) == 0x80) l1[--cut] = '\0';   // Never split a UTF-8 character
    }
    if (cut < strlen(name)) {                                                     // Prefer to break at a space
      for (uint8_t i = cut; i > cut / 2; i--) if (l1[i - 1] == ' ') { cut = i; l1[cut] = '\0'; break; }
    }
    gx_text(0, 4, l1, GX_C_ACC);
    if (cut < strlen(name)) gx_text(0, 34, name + cut, GX_C_ACC, GXF_S, 284);
    char sz[16];
    if (gx_sel_size >= 1048576UL) sprintf_P(sz, PSTR("%s MB"), gx_ftoa(gx_sel_size / 1048576.0f, 1));
    else sprintf_P(sz, PSTR("%u KB"), unsigned(gx_sel_size / 1024));
    stat_row(182, 132, 288, GXI_FILE, "Size", sz);
    stat_row(182, 166, 288, gx_on_usb() ? GXI_USB : GXI_SD, "Storage", gx_on_usb() ? "USB" : "SD card");
    gx_button(182, 222, 136, 46, GXI_CLOSE, "CANCEL", GXA(GA_BACK, 0));
    gx_button(326, 222, 144, 46, GXI_PLAY, "PRINT", GXA(GA_CMD, C_PRINT), GXB_PRIMARY);
    gx_footer_temps();
  }

#else
  static void gx_screen_storage() {}
#endif // HAS_MEDIA

//////////////////////////////////////////////////////////////////////////////
// Touch dispatch
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Nozzle / bed / fan: one screen each, with a slider instead of the keypad
//////////////////////////////////////////////////////////////////////////////

static uint8_t gx_heat_target = 0;   // 0 = nozzle, 1 = bed, 2 = fan

static int16_t heat_max(const uint8_t t) {
  switch (t) {
    case 0: return TERN(HAS_HOTEND, thermalManager.hotend_max_target(0), 0);
    case 1: return TERN(HAS_HEATED_BED, BED_MAX_TARGET, 0);
    default: return 100;
  }
}
static int16_t heat_get(const uint8_t t) { return int16_t(v_get(t == 0 ? V_NOZZLE : t == 1 ? V_BED : V_FAN)); }
static void heat_set(const uint8_t t, const int16_t v) { v_set(t == 0 ? V_NOZZLE : t == 1 ? V_BED : V_FAN, constrain(v, 0, heat_max(t))); }

void gx_slider(const uint8_t id, const uint16_t permille) {
  const int16_t mx = heat_max(id);
  heat_set(id, int16_t((uint32_t(permille) * mx + 500) / 1000));
}

// op: 0 = -10, 1 = -1, 2 = +1, 3 = +10, 4 = off, 5 = preset 1 (PLA) / 50%, 6 = preset 2 (ABS) / 100%
static void gx_adjust(const uint8_t t, const uint8_t op) {
  static const int8_t steps[4] = { -10, -1, 1, 10 };
  if (op < 4) heat_set(t, heat_get(t) + steps[op]);
  else if (op == 4) heat_set(t, 0);
  else if (t == 2) heat_set(t, op == 5 ? 50 : 100);
  else {
    #if HAS_PREHEAT
      const uint8_t m = op - 5;
      if (m < PREHEAT_COUNT) { if (t == 0) ui.preheat_hotend(m); else TERN_(HAS_HEATED_BED, ui.preheat_bed(m)); }
    #endif
  }
  gx_invalidate();
}

constexpr uint16_t SL_X = 24, SL_W = 432, SL_Y = 132, SL_H = 52;   // Slider touch area (screen)

GX_SCREEN(gx_screen_heat) {
  gx_set_refresh(1000);
  const uint8_t t = gx_heat_target;
  const bool fan = t == 2;
  static const char * const titles[3] = { "NOZZLE", "HEATED BED", "FAN" };
  static const GxIcon icons[3] = { GXI_NOZZLE, GXI_BED, GXI_FAN };

  // Current / target values
  int16_t cur = 0;
  #if HAS_HOTEND
    if (t == 0) cur = thermalManager.wholeDegHotend(0);
  #endif
  #if HAS_HEATED_BED
    if (t == 1) cur = thermalManager.wholeDegBed();
  #endif
  const int16_t tgt = heat_get(t), mx = heat_max(t);
  const uint16_t col = fan ? GX_C_ACC : gx_heat_color(cur);

  if (gx_full()) {
    gx_header(titles[t]);
    // Fine steps
    static const char * const sl[4] = { "-10", "-1", "+1", "+10" };
    for (uint8_t i = 0; i < 4; i++)
      gx_button(10 + i * 117, 190, 109, 36, GX_NO_ICON, sl[i], GXA(GA_ADJ, (t << 4) | i) | GX_REPEAT, GXB_NORMAL, GXF_L);
    // Presets + off
    if (fan) {
      gx_button(10, 232, 148, 38, GXI_FAN, "OFF", GXA(GA_ADJ, (t << 4) | 4));
      gx_button(166, 232, 148, 38, GXI_FAN, "50%", GXA(GA_ADJ, (t << 4) | 5));
      gx_button(322, 232, 148, 38, GXI_FAN, "100%", GXA(GA_ADJ, (t << 4) | 6));
    }
    else {
      gx_button(10, 232, 148, 38, GXI_PREHEAT, "PLA", GXA(GA_ADJ, (t << 4) | 5));
      gx_button(166, 232, 148, 38, GXI_PREHEAT, "ABS", GXA(GA_ADJ, (t << 4) | 6));
      gx_button(322, 232, 148, 38, GXI_SNOW, "COOLDOWN", GXA(GA_ADJ, (t << 4) | 4), GXB_PRIMARY);
    }
    #if ENABLED(TOUCH_SCREEN)
      touch.add_control(GX_SLIDER, SL_X, SL_Y, SL_W, SL_H, t);
    #endif
  }

  // Info panel
  gx_canvas(10, 58, 460, 66);
  gx_panel(0, 0, 460, 66);
  gx_icon(12, 9, icons[t], 48, col);
  char a[16], b[24];
  if (fan) { sprintf_P(a, PSTR("%i%%"), tgt); strcpy_P(b, PSTR("Fan speed")); }
  else { sprintf_P(a, PSTR("%i\xC2\xB0"), cur); sprintf_P(b, PSTR("Target %i\xC2\xB0" "C"), tgt); }
  gx_text(76, gx_vcenter(66, GXF_L), a, col, GXF_L);
  gx_text_right(448, gx_vcenter(66, GXF_S), b, fan ? GX_C_DIM : (tgt ? GX_C_ACC : GX_C_DIM));

  // Slider
  gx_canvas(10, SL_Y, 460, SL_H);
  const uint16_t tx = SL_X - 10, kx = tx + uint32_t(SL_W) * _MAX(int16_t(0), tgt) / _MAX(mx, int16_t(1));
  gx_panel(tx, 21, SL_W, 10, GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, 5);
  if (kx > tx + 4) gx_panel(tx + 1, 22, kx - tx, 8, col, GXP_SOLID, col, 4);
  gx_panel(kx - 13, 13, 26, 26, GX_C_TEXT, GXP_SOLID | GXP_BORDER, col, 12);
  gx_text(tx, 36, "0", GX_C_DIM);
  gx_text_right(tx + SL_W, 36, gx_itoa(mx), GX_C_DIM);

  gx_footer_temps();
}

static void gx_tramming() {
  #if ENABLED(LCD_BED_TRAMMING)
    gx_go(_lcd_bed_tramming);
  #endif
}

//////////////////////////////////////////////////////////////////////////////
// EEPROM: store / load / restore
//////////////////////////////////////////////////////////////////////////////

GX_SCREEN(gx_screen_eeprom) {
  if (!gx_full()) return;
  gx_header("EEPROM");
  constexpr uint16_t TW = 146, TH = 120;
  gx_tile(10,               70, TW, TH, GXI_STORE,    48, "Store",   GXA(GA_CMD, C_SAVE_ASK),    GX_C_ACC, "Save (M500)");
  gx_tile(10 + (TW + 11),   70, TW, TH, GXI_LOAD_SET, 48, "Load",    GXA(GA_CMD, C_LOAD_ASK),    GX_C_ACC, "Read (M501)");
  gx_tile(10 + 2*(TW + 11), 70, TW, TH, GXI_RESTORE,  48, "Restore", GXA(GA_CMD, C_RESTORE_ASK), GX_C_HOT, "Defaults (M502)");
  gx_footer_temps();
}

//////////////////////////////////////////////////////////////////////////////
// More: TMC drivers and advanced settings
//////////////////////////////////////////////////////////////////////////////

GX_SCREEN(gx_screen_movespeed) {
  if (!gx_full()) return;
  gx_header("MOVE SPEED");
  constexpr uint16_t W = 460, H = 42;
  static const char * const names[4] = { "X axis", "Y axis", "Z axis", "Extruder" };
  static const GxIcon icons[4] = { GXI_AXES, GXI_AXES, GXI_ZOFFSET, GXI_EXTRUDER };
  for (uint8_t i = 0; i < 4; i++)
    gx_row(10, 58 + i * 48, W, H, icons[i], names[i], v_str(V_MFR_X + i), "mm/s", GXA(GA_EDIT, V_MFR_X + i), GXT_EDIT);
  gx_footer_msg("Speeds of the Move screen. Keep with EEPROM > Store");
}

#if ALL(GX_HAS_TMC, HAS_STEALTHCHOP)

  // Every driver with StealthChop: label, mode flag and the refresh call
  struct gx_stepmode_t { const char *name; bool *stealth; void (*refresh)(); };
  #define GX_SM(ST, NAME) { NAME, &stepper##ST.stored.stealthChop_enabled, []{ stepper##ST.refresh_stepping_mode(); } },
  static const gx_stepmode_t gx_stepmodes[] = {
    #if X_HAS_STEALTHCHOP
      GX_SM(X, "X")
    #endif
    #if X2_HAS_STEALTHCHOP
      GX_SM(X2, "X2")
    #endif
    #if Y_HAS_STEALTHCHOP
      GX_SM(Y, "Y")
    #endif
    #if Y2_HAS_STEALTHCHOP
      GX_SM(Y2, "Y2")
    #endif
    #if Z_HAS_STEALTHCHOP
      GX_SM(Z, "Z")
    #endif
    #if Z2_HAS_STEALTHCHOP
      GX_SM(Z2, "Z2")
    #endif
    #if Z3_HAS_STEALTHCHOP
      GX_SM(Z3, "Z3")
    #endif
    #if Z4_HAS_STEALTHCHOP
      GX_SM(Z4, "Z4")
    #endif
    #if E0_HAS_STEALTHCHOP
      GX_SM(E0, "E0")
    #endif
    #if E1_HAS_STEALTHCHOP
      GX_SM(E1, "E1")
    #endif
    #if E2_HAS_STEALTHCHOP
      GX_SM(E2, "E2")
    #endif
    #if E3_HAS_STEALTHCHOP
      GX_SM(E3, "E3")
    #endif
  };
  #undef GX_SM
  constexpr uint8_t gx_stepmode_count = COUNT(gx_stepmodes);

  static void gx_set_stepmode(const uint8_t arg) {
    const uint8_t i = arg >> 1;
    if (i >= gx_stepmode_count) return;
    *gx_stepmodes[i].stealth = arg & 1;
    gx_stepmodes[i].refresh();
    gx_invalidate();
  }

  GX_SCREEN(gx_screen_stepmode) {
    if (gx_full()) gx_header("STEP MODE");
    // Rows of two choices; up to 4 drivers per screen height
    const uint8_t n = _MIN(gx_stepmode_count, uint8_t(4));
    constexpr uint16_t RH = 46;
    for (uint8_t i = 0; i < n; i++) {
      const uint16_t y = 58 + i * (RH + 6);
      const bool st = *gx_stepmodes[i].stealth;
      gx_canvas(10, y, 90, RH);
      gx_panel(0, 0, 90, RH);
      char l[12]; strlcpy(l, gx_stepmodes[i].name, sizeof(l)); strlcat(l, " axis", sizeof(l));
      gx_text_center(0, gx_vcenter(RH, GXF_S), 90, l, GX_C_TEXT);
      gx_button(108, y, 176, RH, GXI_MOON, "Quiet",  GXA(GA_STEP, (i << 1) | 1), st ? GXB_SELECTED : GXB_NORMAL);
      gx_button(292, y, 178, RH, GXI_BOLT, "Strong", GXA(GA_STEP, (i << 1) | 0), st ? GXB_NORMAL : GXB_SELECTED);
    }
    if (gx_full()) gx_footer_msg("Quiet: StealthChop, silent   Strong: SpreadCycle, more torque");
  }

#endif

GX_SCREEN(gx_screen_more) {
  if (!gx_full()) return;
  gx_header("MORE");
  struct { GxIcon icon; const char *label; uint8_t go; bool on; } t[6] = {
    { GXI_BOLT,     "TMC current", 34, ENABLED(GX_HAS_TMC) },
    { GXI_SPEED,    "Hybrid thr.", 35, ALL(GX_HAS_TMC, HYBRID_THRESHOLD) },
    { GXI_MOON,     "Step mode",   36, ALL(GX_HAS_TMC, HAS_STEALTHCHOP) },
    { GXI_TARGET,   "Sensorless",  37, ALL(GX_HAS_TMC, SENSORLESS_HOMING) },
    { GXI_SPEED,    "Move speed",  39, true },
    { GXI_GEAR,     "Advanced",    38, true }
  };
  constexpr uint16_t TW = 109, TH = 96;
  for (uint8_t i = 0; i < 6; i++)
    gx_tile(10 + (i % 4) * (TW + 8), 62 + (i / 4) * (TH + 8), TW, TH, t[i].icon, 32, t[i].label,
            t[i].on ? GXA(GA_GO, t[i].go) : 0, t[i].on ? GX_C_ACC : GX_C_LINE);
  gx_footer_temps();
}

//////////////////////////////////////////////////////////////////////////////
// G-code: type a command and send it
//////////////////////////////////////////////////////////////////////////////

static char gx_gcode[40] = "";

#define GX_TXT_BS    0x08
#define GX_TXT_CLR   0x01
#define GX_TXT_SEND  0x0D

static void gx_text_key(const char k) {
  size_t n = strlen(gx_gcode);
  switch (k) {
    case GX_TXT_BS:  if (n) gx_gcode[n - 1] = '\0'; break;
    case GX_TXT_CLR: gx_gcode[0] = '\0'; break;
    case GX_TXT_SEND:
      if (n) {
        queue.enqueue_one_now(gx_gcode);
        char m[48]; strlcpy(m, "Sent: ", sizeof(m)); strlcat(m, gx_gcode, sizeof(m));
        ui.set_status(m);
        gx_gcode[0] = '\0';
      }
      break;
    default:
      if (n < sizeof(gx_gcode) - 1 && !(k == ' ' && (!n || gx_gcode[n - 1] == ' '))) { gx_gcode[n] = k; gx_gcode[n + 1] = '\0'; }
      break;
  }
  gx_invalidate();
}

GX_SCREEN(gx_screen_gcode) {
  if (gx_full()) gx_header("G-CODE");

  // Command line and the last status message
  gx_canvas(10, 56, 460, 40);
  gx_panel(0, 0, 460, 40, GX_C_ACC);
  if (gx_gcode[0]) gx_text(12, gx_vcenter(40, GXF_L), gx_gcode, GX_C_TEXT, GXF_L, 436);
  else gx_text(12, gx_vcenter(40, GXF_S), "Type a command, e.g. G28", GX_C_DIM);
  gx_canvas(10, 98, 460, 20);
  if (ui.has_status()) gx_text(4, 2, ui.status_message, GX_C_DIM, GXF_S, 452);

  if (!gx_full()) return;

  // 4 rows of 11 keys
  static const char rows[4][12] = { "1234567890.", "GMTSEFXYZP-", "ABCDHIJKLNO", "QRUVW:*;=/!" };
  constexpr uint16_t KW = 38, KH = 36, GAP = 4, KX = 10, KY = 122;
  for (uint8_t r = 0; r < 4; r++)
    for (uint8_t c = 0; c < 11; c++) {
      const char k = rows[r][c], lbl[2] = { k, 0 };
      gx_button(KX + c * (KW + GAP), KY + r * (KH + GAP), KW, KH, GX_NO_ICON, lbl, GXA(GA_TXT, k));
    }

  // Bottom row: clear, space, backspace, send
  constexpr uint16_t BY = KY + 4 * (KH + GAP);
  gx_button(10,  BY, 92,  36, GXI_CLOSE,     "CLEAR", GXA(GA_TXT, GX_TXT_CLR));
  gx_button(108, BY, 150, 36, GX_NO_ICON,    "SPACE", GXA(GA_TXT, ' '));
  gx_button(264, BY, 72,  36, GXI_BACKSPACE, nullptr, GXA(GA_TXT, GX_TXT_BS) | GX_REPEAT);
  gx_button(342, BY, 128, 36, GXI_PLAY,      "SEND",  GXA(GA_TXT, GX_TXT_SEND), GXB_PRIMARY);
}

static void gx_open(const uint8_t id) {
  switch (id) {
    case 0:  gx_go(gx_screen_prepare); break;
    case 1:  gx_go(gx_screen_settings); break;
    case 2:  TERN_(HAS_MEDIA, gx_go(gx_screen_storage)); break;
    case 3:  gx_go(gx_screen_tune); break;
    case 4:  gx_go(gx_screen_move); break;
    case 5:  gx_go(gx_screen_temp); break;
    case 6:  gx_go(gx_screen_filament); break;
    case 7:  gx_go(gx_screen_level); break;
    case 8:  gx_go(gx_screen_mesh); break;
    case 9:  gx_go(gx_screen_meshedit); break;
    case 10: gx_tramming(); break;
    case 11: gx_go(gx_screen_zoffset); break;
    case 12: gx_go(gx_screen_bltouch); break;
    case 13: gx_go(gx_screen_settemp); break;
    case 14: gx_go(gx_screen_filset); break;
    case 15: gx_go(gx_screen_probeset); break;
    case 16: gx_go(gx_screen_motion); break;
    case 26: gx_go(gx_screen_display); break;
    case 27: gx_heat_target = 0; gx_go(gx_screen_heat); break;
    case 28: gx_heat_target = 1; gx_go(gx_screen_heat); break;
    case 29: gx_heat_target = 2; gx_go(gx_screen_heat); break;
    #if ENABLED(EDITABLE_STEPS_PER_UNIT)
      case 21: gx_go(menu_advanced_steps_per_mm); break;
    #endif
    #if DISABLED(SLIM_LCD_MENUS)
      case 22: gx_go(menu_advanced_velocity); break;
      case 23: gx_go(menu_advanced_acceleration); break;
      #if ENABLED(CLASSIC_JERK)
        case 24: gx_go(menu_advanced_jerk); break;
      #endif
      #if ENABLED(SHAPING_MENU)
        case 25: gx_go(menu_advanced_input_shaping); break;
      #endif
    #endif
    case 17: gx_go(gx_screen_about); break;
    case 30: gx_go(gx_screen_eeprom); break;
    case 31: gx_go(gx_screen_more); break;
    case 32: gx_go(gx_screen_gcode); break;
    #if GX_HAS_TMC
      case 33: gx_go(menu_tmc); break;
      case 34: gx_go(menu_tmc_current); break;
      #if ENABLED(HYBRID_THRESHOLD)
        case 35: gx_go(menu_tmc_hybrid_thrs); break;
      #endif
      #if HAS_STEALTHCHOP
        case 36: gx_go(gx_screen_stepmode); break;
      #endif
      #if ENABLED(SENSORLESS_HOMING)
        case 37: gx_go(menu_tmc_homing_thrs); break;
      #endif
    #endif
    case 38: gx_go(menu_advanced_settings); break;
    case 39: gx_go(gx_screen_movespeed); break;
    case 18: TERN_(TOUCH_SCREEN_CALIBRATION, gx_go(touch_screen_calibration)); break;
    case 19: gx_pid_bed = false; gx_pid_temp = 210; gx_go(gx_screen_pid); break;
    case 20: gx_pid_bed = true;  gx_pid_temp = 60;  gx_go(gx_screen_pid); break;
    default: break;
  }
}

void gx_touch(const uint16_t action) {
  const uint8_t cmd = action >> 8, arg = action & 0xFF;
  switch (cmd) {
    case GA_BACK:  gx_back(); break;
    case GA_TEMPS: gx_go(gx_screen_temp); break;
    case GA_GO:    gx_open(arg); break;
    case GA_EDIT:  gx_edit(arg); break;
    case GA_CMD: {
      const bool from_confirm = ui.currentScreen == gx_screen_confirm;
      gx_command(arg);
      if (from_confirm && ui.currentScreen == gx_screen_confirm) gx_back();   // Close the dialog after "yes"
    } break;
    case GA_KEY:   kp_key(char(arg)); break;
    case GA_TXT:   gx_text_key(char(arg)); break;
    #if ALL(GX_HAS_TMC, HAS_STEALTHCHOP)
      case GA_STEP: gx_set_stepmode(arg); break;
    #endif
    #if HAS_MEDIA
      case GA_FILE: gx_file_tap(arg); break;
      case GA_PAGE:
        if (arg) { if ((gx_file_page + 1) * 8 < files_total()) gx_file_page++; }
        else if (gx_file_page > 0) gx_file_page--;
        gx_invalidate();
        break;
    #endif
    case GA_CELL:  gx_mesh_cell = arg; gx_invalidate(); break;
    case GA_ADJ:   gx_adjust(arg >> 4, arg & 0x0F); break;
    default: break;
  }
}

bool gx_beep_enabled() { return gx_beep_on; }

#endif // TFT_COLOR_UI && NOVIN3DP_GALAXY_UI
