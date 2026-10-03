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

#if ENABLED(FIRST_LAYER_CALIBRATION)

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif
#include "../../module/temperature.h"
#include "../../feature/babystep.h"
#include "../../feature/first_layer_cal.h"
#include "../../lcd/marlinui.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(NOZZLE_PARK_FEATURE)
  #include "../../libs/nozzle.h"
#endif

#if HAS_MARLINUI_MENU
  #include "../../lcd/menu/menu_item.h"
#endif

#if ENABLED(FWRETRACT)
  #include "../../feature/fwretract.h"
#endif

// Runout switch can tell whether filament is loaded. Motion sensor can't.
#if HAS_FILAMENT_SENSOR && (DISABLED(FILAMENT_MOTION_SENSOR) || ENABLED(FILAMENT_SWITCH_AND_MOTION))
  #define FLC_FILAMENT_CHECK 1
  #include "../../feature/runout.h"
#endif

// Default to Mesh Validation settings when G26 is enabled
#ifndef FLC_LAYER_HEIGHT
  #ifdef MESH_TEST_LAYER_HEIGHT
    #define FLC_LAYER_HEIGHT MESH_TEST_LAYER_HEIGHT
  #else
    #define FLC_LAYER_HEIGHT 0.2
  #endif
#endif
#ifndef FLC_LINE_WIDTH
  #ifdef MESH_TEST_NOZZLE_SIZE
    #define FLC_LINE_WIDTH ((MESH_TEST_NOZZLE_SIZE) + 0.02)
  #else
    #define FLC_LINE_WIDTH 0.42
  #endif
#endif
#ifndef FLC_ROWS
  #define FLC_ROWS 6
#endif
#ifndef FLC_MARGIN
  #define FLC_MARGIN 10
#endif

#define FLC_RETRACT_LENGTH   2.0f   // (mm) Retract after printing, unless FWRETRACT sets length
#define FLC_FEEDRATE      1000      // (mm/min) Print feedrate

static bool flc_canceled;

#if HAS_MARLINUI_MENU

  static void flc_babystep_screen() {
    TERN(MESH_BED_LEVELING, lcd_babystep_z(), ui.goto_screen(lcd_babystep_zoffset));
  }

  // Clicking out of babystep screen asks whether to stop
  static void flc_stop_screen() {
    MenuItem_confirm::select_screen(
        GET_TEXT_F(MSG_BUTTON_STOP), GET_TEXT_F(MSG_BACK)
      , []{ flc_canceled = true; ui.return_to_status(); }
      , flc_babystep_screen
      , GET_TEXT_F(MSG_STOP_PRINT), (const char *)nullptr, F("?")
    );
  }

  #if ENABLED(EEPROM_SETTINGS)
    // Offer to save result
    static void flc_save_screen() {
      MenuItem_confirm::select_screen(
          GET_TEXT_F(MSG_BUTTON_SAVE), GET_TEXT_F(MSG_BUTTON_CANCEL)
        , []{ ui.store_settings(); ui.return_to_status(); }
        , ui.return_to_status
        , GET_TEXT_F(TERN(MESH_BED_LEVELING, MSG_MESH_Z_OFFSET, MSG_ZPROBE_ZOFFSET))
        , BABYSTEP_TO_STR(TERN(MESH_BED_LEVELING, bedlevel.z_offset, probe.offset.z)), F("?")
      );
    }
  #endif

#endif

// Stop on M108 or when confirmed on LCD
static bool flc_stop_requested() {
  if (!marlin.wait_for_heatup) flc_canceled = true;
  #if HAS_MARLINUI_MENU
    if (!flc_canceled && ui.on_status_screen()) ui.goto_screen(flc_stop_screen);
  #endif
  return flc_canceled;
}

static void flc_heaters_off() {
  TERN_(HAS_HOTEND, thermalManager.setTargetHotend(0, motion.extruder));
  TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(0));
}

// Stop before printing, with heaters off
static void flc_abort(const bool canceled=true) {
  TERN_(HAS_MARLINUI_MENU, ui.release());
  if (canceled) LCD_MESSAGE(MSG_FLC_CANCELED);
  flc_heaters_off();
}

// Extrude line from current position using 'e' mm of filament
static void flc_line_to(const flc_segment_t &seg, const feedRate_t fr_mm_s) {
  motion.destination = motion.position;
  motion.destination.x = seg.x;
  motion.destination.y = seg.y;
  motion.destination.e += seg.e;
  motion.prepare_internal_move_to_destination(fr_mm_s);

  // Keep planner queue short so stop request takes effect quickly
  while (planner.movesplanned() > 1 && !flc_stop_requested()) marlin.idle();
}

static void flc_retract(const float len) {
  motion.destination = motion.position;
  motion.destination.e -= len;
  motion.prepare_internal_move_to_destination(planner.settings.max_feedrate_mm_s[E_AXIS] * 0.666f);
}

// Use M200 filament diameter when set, otherwise configured diameter
static float flc_filament_diameter() {
  #if HAS_VOLUMETRIC_EXTRUSION
    const float d = planner.filament_size[motion.extruder];
    if (d > 0) return d;
  #endif
  return DEFAULT_NOMINAL_FILAMENT_DIA;
}

/**
 * M1005: First Layer Calibration
 *
 * Calibrate nozzle-to-bed distance.
 * Prints zig-zag pattern across bed ending in solid patch.
 * Babystep Z while pattern prints to adjust Probe Z Offset.
 * With MESH_BED_LEVELING babysteps are added to Mesh Z Offset after printing.
 * Won't start without filament when runout switch is enabled.
 * Click LCD while heating, click out of babystep screen, or send M108 to stop early.
 * Result is not saved. LCD offers to save when done, or use M500.
 *
 *  H<temp>    Hotend temperature. If omitted with no target set, use first preheat preset.
 *  B<temp>    Bed temperature. If omitted with no target set, use first preheat preset.
 *  R          Reset Probe Z Offset to configured default, or Mesh Z Offset to 0, before printing,
 *             but only if that raises nozzle
 *  K<bool>    Keep heaters on after printing (default: FLC_KEEP_HEATERS_ON)
 *  A<bool>    Run G29 first if leveling is not valid (default: 1). Ignored with MESH_BED_LEVELING.
 *  O          Home only if needed. Otherwise always home so nozzle height matches current Z offset.
 *  L<linear>  Layer height
 *  W<linear>  Line width
 *  I<linear>  Margin from bed edges or printable radius
 *  P<count>   Number of rows
 *  F<rate>    Print feedrate
 *  D<linear>  Filament diameter (default: M200 diameter or DEFAULT_NOMINAL_FILAMENT_DIA)
 */
void GcodeSuite::M1005() {
  const float h = parser.linearval('L', FLC_LAYER_HEIGHT),
              w = parser.linearval('W', FLC_LINE_WIDTH),
              inset = parser.linearval('I', FLC_MARGIN),
              fil_dia = parser.linearval('D', flc_filament_diameter());
  const uint8_t rows = parser.byteval('P', FLC_ROWS);

  if (h <= 0 || w < h || rows < 2 || fil_dia <= 0 || inset < 0) {
    SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Bad M1005 parameter."));
    return;
  }

  const feedRate_t fr_mm_s = parser.feedrateval('F', MMM_TO_MMS(FLC_FEEDRATE));

  //
  // Cover bed within travel limits.
  // Rows run along X from back to front.
  //
  FLCPattern pattern;
  #if IS_KINEMATIC
    const bool fits = pattern.init_round(X_CENTER, Y_CENTER, (PRINTABLE_RADIUS) - inset, rows, h, w, fil_dia);
  #else
    const float x_min = _MAX(X_MIN_BED, X_MIN_POS) + inset, x_max = _MIN(X_MAX_BED, X_MAX_POS) - inset,
                y_front = _MAX(Y_MIN_BED, Y_MIN_POS) + inset, y_back = _MIN(Y_MAX_BED, Y_MAX_POS) - inset;
    const bool fits = pattern.init_rect(x_min, x_max, y_front, y_back, rows, h, w, fil_dia);
  #endif

  if (!fits) {
    SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Print area too small."));
    LCD_MESSAGE(MSG_FLC_BED_TOO_SMALL);
    return;
  }

  #if ENABLED(MESH_BED_LEVELING)
    // Manual leveling can't be run from here
    if (!leveling_is_valid()) {
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Mesh Bed Leveling required."));
      LCD_MESSAGE(MSG_UBL_MESH_INVALID);
      return;
    }
  #endif

  #if FLC_FILAMENT_CHECK
    if (runout.enabled && TEST(FilamentSensorBase::poll_runout_states(), TERN0(MULTI_FILAMENT_SENSOR, motion.extruder))) {
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No filament."));
      LCD_MESSAGE(MSG_FLC_LOAD_FILAMENT);
      return;
    }
  #endif

  //
  // Heat, home, and level
  //
  LCD_MESSAGE(MSG_FIRST_LAYER_CAL);

  const bool keep_heaters_on = parser.boolval('K', ENABLED(FLC_KEEP_HEATERS_ON)),
             do_level = TERN0(HAS_LEVELING, DISABLED(MESH_BED_LEVELING) && parser.boolval('A', true) && !leveling_is_valid());

  // Probe with cold nozzle so it can't ooze on bed. Otherwise heat nozzle along with bed.
  #if HAS_HOTEND
    celsius_t hotend_target = parser.seenval('H') ? parser.value_int() : thermalManager.degTargetHotend(motion.extruder); // Marlin always sends itself Celsius
    #if HAS_PREHEAT
      if (!hotend_target && !parser.seen('H')) hotend_target = ui.material_preset[0].hotend_temp;
    #endif
    if (!do_level) thermalManager.setTargetHotend(hotend_target, motion.extruder);
  #endif
  #if HAS_HEATED_BED
    if (parser.seenval('B'))
      thermalManager.setTargetBed(parser.value_int());
    #if HAS_PREHEAT
      else if (!thermalManager.degTargetBed())
        thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
    #endif
    if (thermalManager.degTargetBed()) {
      thermalManager.isHeatingBed() ? LCD_MESSAGE(MSG_BED_HEATING) : LCD_MESSAGE(MSG_BED_COOLING);
      TERN_(HAS_MARLINUI_MENU, ui.capture());  // Click cancels, like G26
      if (!thermalManager.wait_for_bed(false OPTARG(HEATUP_CLICK_CAN_CANCEL, true))) return flc_abort();
      TERN_(HAS_MARLINUI_MENU, ui.release());
    }
  #endif

  if (!parser.seen_test('O') || motion.axes_should_home()) {
    LCD_MESSAGE(MSG_LEVEL_BED_HOMING);
    home_all_axes(true);
  }
  if (motion.homing_needed_error()) return flc_abort(false);

  #if HAS_LEVELING
    if (do_level) process_subcommands_now(F(TERN(AUTO_BED_LEVELING_UBL, "G29P1\nG29P3\nG29P3", "G29")));
    set_bed_leveling_enabled(true);
  #endif

  if (parser.seen_test('R')) {
    #if ENABLED(MESH_BED_LEVELING)
      // Mesh Z Offset applies to next move. Only clear offset that lowers nozzle.
      if (bedlevel.z_offset < 0) {
        const bool was_active = planner.leveling_active;
        set_bed_leveling_enabled(false);
        bedlevel.z_offset = 0;
        set_bed_leveling_enabled(was_active);
      }
      else if (bedlevel.z_offset > 0)
        SERIAL_ECHOLNPGM("Mesh Z Offset not reset. That would lower nozzle.");
    #else
      // Babystep to default offset like LCD does, so nozzle and offset stay in step
      constexpr float dpo[] = NOZZLE_TO_PROBE_OFFSET;
      const float diff = dpo[Z_AXIS] - probe.offset.z;
      if (diff > 0) {
        babystep.add_mm(Z_AXIS, diff);
        probe.offset.z = dpo[Z_AXIS];
      }
      else if (diff < 0)
        SERIAL_ECHOLNPGM("Probe Z Offset not reset. Default would lower nozzle.");
    #endif
  }

  #if HAS_HOTEND
    thermalManager.setTargetHotend(hotend_target, motion.extruder);
    if (hotend_target) {
      thermalManager.isHeatingHotend(motion.extruder) ? LCD_MESSAGE(MSG_HEATING) : LCD_MESSAGE(MSG_COOLING);
      TERN_(HAS_MARLINUI_MENU, ui.capture());
      if (!thermalManager.wait_for_hotend(motion.extruder, false OPTARG(HEATUP_CLICK_CAN_CANCEL, true))) return flc_abort();
      TERN_(HAS_MARLINUI_MENU, ui.release());
    }
  #endif

  //
  // Print
  //
  LCD_MESSAGE(MSG_FIRST_LAYER_CAL);

  #if HAS_VOLUMETRIC_EXTRUSION
    const bool volumetric_was_enabled = parser.volumetric_enabled;
    parser.volumetric_enabled = false;
    planner.calculate_volumetric_multipliers();
  #endif
  motion.remember_feedrate_scaling_off();

  motion.do_z_clearance(Z_CLEARANCE_BETWEEN_PROBES);
  motion.blocking_move_xy(pattern.start_x(), pattern.start_y());
  motion.blocking_move_z(h);

  flc_canceled = false;
  marlin.wait_for_heatup = true;  // M108 clears this to stop
  TERN_(HAS_MARLINUI_MENU, flc_babystep_screen());

  flc_segment_t seg;
  while (!flc_canceled && pattern.next(seg)) flc_line_to(seg, fr_mm_s);

  //
  // Finish
  //
  marlin.wait_for_heatup = false;
  const bool completed = !flc_canceled;
  if (flc_canceled) motion.quickstop_stepper();
  flc_retract(TERN(FWRETRACT, fwretract.settings.retract_length, FLC_RETRACT_LENGTH));
  planner.synchronize();

  #if ENABLED(MESH_BED_LEVELING)
    // Move babysteps since homing into Mesh Z Offset. Nozzle stays where it is.
    while (babystep.has_steps()) marlin.idle();
    bedlevel.z_offset += babystep.axis_total[BS_TOTAL_IND(Z_AXIS)] * planner.mm_per_step[Z_AXIS];
    babystep.reset_total(Z_AXIS);
    motion.sync_plan_position();
  #endif

  #if ENABLED(NOZZLE_PARK_FEATURE)
    nozzle.park(2);
  #else
    motion.do_z_clearance_by(Z_CLEARANCE_BETWEEN_PROBES);
    #if !IS_KINEMATIC
      motion.blocking_move_xy(x_min, y_back); // Move nozzle away so print is visible
    #endif
  #endif

  if (!keep_heaters_on) flc_heaters_off();

  motion.restore_feedrate_and_scaling();
  #if HAS_VOLUMETRIC_EXTRUSION
    parser.volumetric_enabled = volumetric_was_enabled;
    planner.calculate_volumetric_multipliers();
  #endif

  if (completed) {
    ui.reset_status();
    ui.completion_feedback();
  }
  else
    LCD_MESSAGE(MSG_FLC_CANCELED);

  #if HAS_MARLINUI_MENU
    ui.defer_status_screen(false);
    if (TERN0(EEPROM_SETTINGS, completed))
      TERN_(EEPROM_SETTINGS, ui.goto_screen(flc_save_screen));
    else
      ui.return_to_status();
  #endif

  if (completed) {
    #if ENABLED(MESH_BED_LEVELING)
      SERIAL_ECHOLNPGM("Mesh Z Offset ", bedlevel.z_offset, ". Use M500 to save.");
    #else
      SERIAL_ECHOLNPGM(STR_PROBE_OFFSET " " STR_Z, probe.offset.z, ". Use M500 to save.");
    #endif
  }
}

#endif // FIRST_LAYER_CALIBRATION
