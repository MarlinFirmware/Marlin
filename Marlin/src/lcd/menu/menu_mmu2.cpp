/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(MMU_MENUS)

#if HAS_PRUSA_MMU3
  #include "../../feature/mmu3/mmu3.h"
  #include "../../feature/mmu3/mmu3_reporting.h"
  #include "../../feature/mmu3/SpoolJoin.h"
#else
  #include "../../feature/mmu/mmu2.h"
#endif

#include "menu_mmu2.h"
#include "menu_item.h"

#include "../../module/temperature.h"
#include "../../module/motion.h"

//
// Load Filament
//

//
// Actions that need a hot nozzle: Load to Nozzle, Unload
//
// A cold nozzle cannot extrude, so the MMU would feed the filament and Marlin would drop the
// extruder moves (MMU3), or the action would be refused and retried forever (MMU2 Unload).
// Offer to preheat first, wait for the temperature (a click or M108 stops), then do the action.
//

enum MMUHotAction : uint8_t { MMU_HOT_LOAD_TO_NOZZLE, MMU_HOT_UNLOAD, MMU_HOT_UNLOAD_THEN_LOAD };  // THEN_LOAD: mmu_hot_tool is the slot, or EXTRUDERS for all
static uint8_t mmu_hot_action, mmu_hot_tool;
static bool mmu_hot_back_to_pause;  // Started from the Filament Change menu: return to it when done

// There is no queue for menu actions, so while one runs (its wait loops keep the menus alive)
// ignore a second one rather than send the MMU a command on top of the running command.
static bool mmu_action_busy;

struct MMUActionGuard {
  const bool ok;
  MMUActionGuard() : ok(!mmu_action_busy) { if (ok) mmu_action_busy = true; else MarlinUI::completion_feedback(false); }
  ~MMUActionGuard() { if (ok) mmu_action_busy = false; }
};

static void _mmu2_back_to_pause() {
  if (mmu_hot_back_to_pause) ui.goto_screen(menu_mmu2_pause); // An error screen may have replaced the menu
}

static void _mmu2_load_to_nozzle_now(const uint8_t tool) {
  MMUActionGuard busy; if (!busy.ok) return;
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_LOADING_FILAMENT), int(tool + 1));
  TERN(HAS_PRUSA_MMU3, mmu3, mmu2).load_to_nozzle(tool);
  ui.reset_status();
}

static void _mmu2_unload_now() {
  MMUActionGuard busy; if (!busy.ok) return;
  ui.reset_status();
  ui.return_to_status();
  // The message needs a slot number. Without a known tool, show a plain message instead of "%i".
  const uint8_t tool = uint8_t(TERN(HAS_PRUSA_MMU3, mmu3, mmu2).get_current_tool());
  if (mmu_hot_action == MMU_HOT_UNLOAD_THEN_LOAD && mmu_hot_tool < EXTRUDERS)
    ui.status_printf(0, GET_TEXT_F(MSG_MMU2_UNLOAD_THEN_LOAD), int(mmu_hot_tool + 1));  // Say that the Load follows
  else if (tool < EXTRUDERS)
    ui.status_printf(0, GET_TEXT_F(MSG_MMU2_UNLOADING_FILAMENT), int(tool + 1));
  else
    LCD_MESSAGE(MSG_FILAMENTUNLOAD);
  while (!TERN(HAS_PRUSA_MMU3, mmu3, mmu2).unload()) {
    safe_delay(50);
    TERN(HAS_PRUSA_MMU3, MMU3::marlin_idle(true), marlin.idle());
  }
  ui.reset_status();
}

void _mmu2_load_to_feeder(const uint8_t tool);
void action_mmu2_load_all();

static void _mmu2_run_hot_action() {
  switch (mmu_hot_action) {
    case MMU_HOT_LOAD_TO_NOZZLE: _mmu2_load_to_nozzle_now(mmu_hot_tool); break;
    case MMU_HOT_UNLOAD:         _mmu2_unload_now();                    break;
    case MMU_HOT_UNLOAD_THEN_LOAD: {
      const uint8_t tool = mmu_hot_tool;      // The Load below changes the state, so keep a copy
      _mmu2_unload_now();                     // Runs until the Unload is done, and releases the action guard
      #if HAS_PRUSA_MMU3
        // The FINDA state comes with the MMU's next status reply, so it may still show the filament for a moment
        // after the Unload ends. Wait for it to clear, or the Load below would stop on the same alert again.
        const millis_t wait_end = millis() + 3000UL;
        while (mmu3.findaDetectsFilament() && PENDING(millis(), wait_end)) { safe_delay(50); MMU3::marlin_idle(true); }
      #endif
      if (tool < EXTRUDERS) _mmu2_load_to_feeder(tool); else action_mmu2_load_all();
    } break;
  }
}

// Temperature to preheat to: the first preheat preset, or a PLA-like temperature without presets
static celsius_t _mmu2_preheat_temp() {
  celsius_t temp = TERN(HAS_PREHEAT, ui.material_preset[0].hotend_temp, 200);
  NOLESS(temp, thermalManager.extrude_min_temp + 5);
  return temp;
}

// Heat the nozzle, with progress on the Status Screen, then run the action
static void _mmu2_heat_and_run() {
  const uint8_t e = motion.extruder;
  const celsius_t temp = _mmu2_preheat_temp(),
                  old_target = thermalManager.degTargetHotend(e);

  ui.reset_status();
  ui.return_to_status();
  LCD_MESSAGE(MSG_HEATING);

  thermalManager.setTargetHotend(temp, e);
  marlin.heatup_start();    // A click or M108 ends the wait
  while (marlin.is_heating() && !thermalManager.hotendReachedTarget(e)) {
    marlin.idle();
    if (ui.use_click()) marlin.heatup_done();
  }
  const bool reached = marlin.is_heating();   // Still set if the temperature was reached
  marlin.heatup_done();

  if (reached)
    _mmu2_run_hot_action();
  else
    thermalManager.setTargetHotend(old_target, e);  // Stopped: leave the target as it was

  ui.reset_status();
  _mmu2_back_to_pause();
}

// Start an action that needs a hot nozzle, asking to preheat first if the nozzle is too cold
static void _mmu2_hot_action(const MMUHotAction action, const uint8_t tool=0, const bool back_to_pause=false) {
  if (mmu_action_busy) return MarlinUI::completion_feedback(false);
  mmu_hot_action = action;
  mmu_hot_tool = tool;
  mmu_hot_back_to_pause = back_to_pause;

  if (!thermalManager.tooColdToExtrude(motion.extruder)) {
    _mmu2_run_hot_action();
    _mmu2_back_to_pause();
    return;
  }

  static MString<8> temp_str;
  temp_str.set(' ').append(_mmu2_preheat_temp());

  ui.push_current_screen();
  ui.goto_screen([]{
    MenuItem_confirm::select_screen(
      GET_TEXT_F(MSG_BUTTON_PROCEED), GET_TEXT_F(MSG_BUTTON_STOP),
      _mmu2_heat_and_run, nullptr,
      GET_TEXT_F(MSG_PREHEAT_HOTEND), temp_str, F("?")
    );
  });
}

inline void action_mmu2_load_to_nozzle(const uint8_t tool) {
  _mmu2_hot_action(MMU_HOT_LOAD_TO_NOZZLE, tool);
}

#if HAS_PRUSA_MMU3

  // The MMU refuses to load while its FINDA sees filament, so say so on a screen the user must answer.
  // (A status message is easily missed, and a Load from the Filament Change menu returns to that menu at once.)
  static bool mmu_already_loaded_from_pause;  // Opened from the Filament Change menu: Unload returns to it
  static uint8_t mmu_already_loaded_slot;     // The slot that was asked for, or EXTRUDERS for all

  // Unload, then do the Load that was asked for
  static void _mmu2_unload_from_alert() {
    _mmu2_hot_action(MMU_HOT_UNLOAD_THEN_LOAD, mmu_already_loaded_slot, mmu_already_loaded_from_pause);
  }

  static void menu_mmu2_already_loaded() {
    MenuItem_confirm::select_screen(
      GET_TEXT_F(MSG_MMU2_UNLOAD_FILAMENT), GET_TEXT_F(MSG_BUTTON_BACK),
      _mmu2_unload_from_alert, nullptr,   // Back returns to the screen that was open
      GET_TEXT_F(MSG_TITLE_FILAMENT_ALREADY_LOADED)
    );
  }

  // Returns true if the alert was shown and the Load must not be sent.
  static bool _mmu2_already_loaded(const uint8_t slot, const bool from_pause=false) {
    if (!mmu3.findaDetectsFilament()) return false;
    MarlinUI::completion_feedback(false);
    SERIAL_ECHO_MSG("MMU3: ", GET_TEXT_F(MSG_DESC_FILAMENT_ALREADY_LOADED));
    mmu_already_loaded_slot = slot;
    mmu_already_loaded_from_pause = from_pause;
    ui.push_current_screen();
    ui.goto_screen(menu_mmu2_already_loaded);
    return true;
  }

#else
  static bool _mmu2_already_loaded(const uint8_t, const bool=false) { return false; }
#endif

// An MMU command (e.g. a Load) was running but making no progress toward its goal
// (say, filament that can't reach the FINDA). The caller has already stopped the
// command, so Retry re-sends it on the clean MMU and Abort ends the operation.
#if HAS_WIRED_LCD
  static bool load_timeout_retry = true; // Retry is the default choice
  static bool load_timeout_done;

  static void menu_load_timeout() {
    MenuItem_confirm::select_screen(
      GET_TEXT_F(MSG_BTN_RETRY), GET_TEXT_F(MSG_MMU2_LOAD_ERROR_ABORT),
      []{ load_timeout_retry = true;  load_timeout_done = true; ui.return_to_status(); },
      []{ load_timeout_retry = false; load_timeout_done = true; ui.return_to_status(); },
      GET_TEXT_F(MSG_MMU2_LOAD_ERROR_TITLE)
    );
  }

  bool mmu3_load_timeout_select() {
    load_timeout_retry = true;   // Retry is the default
    load_timeout_done = false;
    MarlinUI::completion_feedback();
    ui.defer_status_screen();
    ui.goto_screen(menu_load_timeout);
    while (!load_timeout_done) marlin.idle();
    return load_timeout_retry;
  }
#endif

void _mmu2_load_to_feeder(const uint8_t tool) {
  MMUActionGuard busy; if (!busy.ok) return;
  if (_mmu2_already_loaded(tool)) return;
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_LOADING_FILAMENT), int(tool + 1));
  TERN(HAS_PRUSA_MMU3, mmu3, mmu2).load_to_feeder(tool);
  ui.reset_status();
}

void action_mmu2_load_all() {
  if (_mmu2_already_loaded(EXTRUDERS)) return;
  EXTRUDER_LOOP() _mmu2_load_to_feeder(e);
}

void menu_mmu2_load_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU2_ALL, action_mmu2_load_all);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_load_to_feeder(MenuItemBase::itemIndex); });
  END_MENU();
}

void menu_mmu2_load_to_nozzle() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ action_mmu2_load_to_nozzle(MenuItemBase::itemIndex); });
  END_MENU();
}

//
// Eject Filament
//

void _mmu2_eject_filament(uint8_t index, const bool return_to_pause=false) {
  MMUActionGuard busy; if (!busy.ok) return;
  TERN_(HAS_PRUSA_MMU3, mmu3.set_return_to_pause_after_eject(return_to_pause));
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_EJECTING_FILAMENT), int(index + 1));
  if (TERN(HAS_PRUSA_MMU3, mmu3, mmu2).eject_filament(index, true)) {
    ui.reset_status();
    if (return_to_pause) {
      ui.goto_screen(menu_mmu2_pause);
      TERN_(HAS_PRUSA_MMU3, mmu3.set_return_to_pause_after_eject(false));
    }
  }
}

void _mmu2_cut_filament(uint8_t index) {
  MMUActionGuard busy; if (!busy.ok) return;
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_CUTTING_FILAMENT), int(index + 1));
  if (TERN0(HAS_PRUSA_MMU3, mmu3.cut_filament(index, true)))
    ui.reset_status();
}

void action_mmu2_unload_filament() {
  _mmu2_hot_action(MMU_HOT_UNLOAD);
}

void menu_mmu2_eject_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{
    _mmu2_eject_filament(MenuItemBase::itemIndex, false);
  });
  END_MENU();
}

// Cutter

#if HAS_PRUSA_MMU3

void menu_mmu3_cutter_set_mode(uint8_t mode) { mmu3.cutter_mode = mode; }
void menu_mmu3_cutter_disable() { menu_mmu3_cutter_set_mode(0); }
void menu_mmu3_cutter_enable() { menu_mmu3_cutter_set_mode(1); }
void menu_mmu3_cutter_always() { menu_mmu3_cutter_set_mode(2); }

void menu_mmu3_cutter() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_DISABLE, menu_mmu3_cutter_disable);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_ENABLE, menu_mmu3_cutter_enable);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_ALWAYS, menu_mmu3_cutter_always);
  END_MENU();
}

void menu_mmu3_cut_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_cut_filament(MenuItemBase::itemIndex); });
  END_MENU();
}

// SpoolJoin
void spool_join_status() { spooljoin.initStatus(); }

// Fail Stats Menu
void menu_mmu3_fail_stats_last_print() {
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH], buffer2[LCD_WIDTH];

  // had to cast the uint8_t values to uint16_t before formatting them.
  const uint16_t fail_num = MMU3::operation_statistics.fail_num;
  const uint16_t load_fail_num = MMU3::operation_statistics.load_fail_num;

  sprintf_P(buffer1, PSTR("%hu"), fail_num);
  sprintf_P(buffer2, PSTR("%hu"), load_fail_num);

  START_SCREEN();
  STATIC_ITEM_F(marlin.printJobOngoing() ? GET_TEXT_F(MSG_MMU_CURRENT_PRINT_FAILURES) :  GET_TEXT_F(MSG_MMU_LAST_PRINT_FAILURES), SS_INVERT);
  #ifndef __AVR__
    // TODO: I couldn't make this work on AVR
    PSTRING_ITEM(MSG_MMU_FAILS, buffer1, SS_FULL);
    PSTRING_ITEM(MSG_MMU_LOAD_FAILS, buffer2, SS_FULL);
  #endif
  END_SCREEN();
}

void menu_mmu3_fail_stas_total() {
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH], buffer2[LCD_WIDTH], buffer3[LCD_WIDTH];

  sprintf_P(buffer1, PSTR("%hu"), MMU3::operation_statistics.fail_total_num);
  sprintf_P(buffer2, PSTR("%hu"), MMU3::operation_statistics.load_fail_total_num);
  sprintf_P(buffer3, PSTR("%hu"), mmu3.tmcFailures());

  START_SCREEN();
  STATIC_ITEM(MSG_MMU_TOTAL_FAILURES, SS_INVERT);
  #ifndef __AVR__
    // TODO: I couldn't make this work on AVR
    PSTRING_ITEM(MSG_MMU_FAILS, buffer1, SS_FULL);
    PSTRING_ITEM(MSG_MMU_LOAD_FAILS, buffer2, SS_FULL);
    PSTRING_ITEM(MSG_MMU_POWER_FAILS, buffer3, SS_FULL);
  #endif
  END_SCREEN();
}

#if ENABLED(MARLIN_DEV_MODE)
  void menu_mmu3_dev_increment_fail_stat() {
    MMU3::operation_statistics.increment_mmu_fails();
  }

  void menu_mmu3_dev_increment_load_fail_stat() {
    MMU3::operation_statistics.increment_load_fails();
  }
#endif

static void mmu3_reset_fail_stats() {
  bool result = MMU3::operation_statistics.reset_fail_stats();
  ui.go_back();
  MarlinUI::completion_feedback(result);
}

static void mmu3_reset_stats() {
  bool result = MMU3::operation_statistics.reset_stats();
  ui.go_back();
  MarlinUI::completion_feedback(result);
}

void menu_mmu3_toolchange_stat_total() {
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH];
  sprintf_P(buffer1, PSTR("%u"), MMU3::operation_statistics.tool_change_counter);

  char buffer2[LCD_WIDTH];
  sprintf_P(buffer2, PSTR("%" PRIu32 ""), MMU3::operation_statistics.tool_change_total_counter);

  START_SCREEN();
  STATIC_ITEM(MSG_MMU_MATERIAL_CHANGES, SS_INVERT);
  #ifndef __AVR__
    // TODO: I couldn't make this work on AVR
    if (marlin.printJobOngoing())
      PSTRING_ITEM(MSG_MMU_CURRENT_PRINT, buffer1, SS_FULL);
    else
      PSTRING_ITEM(MSG_MMU_LAST_PRINT, buffer1, SS_FULL);
    PSTRING_ITEM(MSG_MMU_TOTAL, buffer2, SS_FULL);
  #endif
  END_SCREEN();
}

void menu_mmu3_statistics() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  #if ENABLED(MARLIN_DEV_MODE)
    ACTION_ITEM(MSG_MMU_DEV_INCREMENT_FAILS, menu_mmu3_dev_increment_fail_stat);
    ACTION_ITEM(MSG_MMU_DEV_INCREMENT_LOAD_FAILS, menu_mmu3_dev_increment_load_fail_stat);
  #endif

  SUBMENU_F(marlin.printJobOngoing() ? GET_TEXT_F(MSG_MMU_CURRENT_PRINT_FAILURES) : GET_TEXT_F(MSG_MMU_LAST_PRINT_FAILURES), menu_mmu3_fail_stats_last_print);
  SUBMENU(MSG_MMU_TOTAL_FAILURES, menu_mmu3_fail_stas_total);
  SUBMENU(MSG_MMU_MATERIAL_CHANGES, menu_mmu3_toolchange_stat_total);
  CONFIRM_ITEM(MSG_MMU_RESET_FAIL_STATS,
    MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
    mmu3_reset_fail_stats, nullptr,
    GET_TEXT_F(MSG_MMU_RESET_FAIL_STATS), (const char *)nullptr, F("?")
  );
  CONFIRM_ITEM(MSG_MMU_RESET_STATS,
    MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
    mmu3_reset_stats, nullptr,
    GET_TEXT_F(MSG_MMU_RESET_STATS), (const char *)nullptr, F("?")
  );
  END_MENU();
}

#endif // HAS_PRUSA_MMU3

//
// MMU2 Menu
//

void action_mmu2_reset() {
  #if HAS_PRUSA_MMU3
    #if PIN_EXISTS(MMU_RST)
      mmu3.reset(MMU3::MMU3::ResetForm::ResetPin);
    #else
      mmu3.reset(MMU3::MMU3::ResetForm::Software);
    #endif
  #else
    mmu2.init();
  #endif
  ui.reset_status();
}

void menu_mmu2() {
  const bool busy = marlin.printJobOngoing(); // printingIsActive()

  START_MENU();
  BACK_ITEM(MSG_MAIN_MENU);

  // MMU2/MMU3 Commands
  if (!busy && TERN1(HAS_PRUSA_MMU3, mmu3.mmu_hw_enabled)) {
    SUBMENU(MSG_MMU2_LOAD_FILAMENT, menu_mmu2_load_filament);
    SUBMENU(MSG_MMU2_LOAD_TO_NOZZLE, menu_mmu2_load_to_nozzle);
    SUBMENU(MSG_MMU2_EJECT_FILAMENT, menu_mmu2_eject_filament);
    ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, action_mmu2_unload_filament);
  }

  #if HAS_PRUSA_MMU3
    // MMU3 Enable/Disable
    #ifndef __AVR__
      editable.state = mmu3.mmu_hw_enabled;
      EDIT_ITEM_F(bool, F("MMU"), &mmu3.mmu_hw_enabled, []{
        if (editable.state)
          mmu3.stop();
        else
          mmu3.start();
      });
    #endif

    // SpoolJoin Enable/Disable
    EDIT_ITEM(bool, MSG_MMU_SPOOL_JOIN, &spooljoin.enabled, spool_join_status);

    // Cutter Enable/Disable
    bool cutter_enabled = mmu3.cutter_mode != 0;
    editable.state = cutter_enabled;
    EDIT_ITEM(bool, MSG_MMU_CUTTER, &cutter_enabled, []{
      menu_mmu3_cutter_set_mode((uint8_t)!editable.state);
    });
    if (!busy && MMU3::cutter_enabled() && mmu3.mmu_hw_enabled) {
      SUBMENU(MSG_MMU2_CUT_FILAMENT, menu_mmu3_cut_filament);
    }

    // Statistics
    SUBMENU(MSG_MMU_STATISTICS, menu_mmu3_statistics);
  #endif

  if (TERN1(HAS_PRUSA_MMU3, mmu3.mmu_hw_enabled)) {
    ACTION_ITEM(MSG_MMU2_RESET, action_mmu2_reset);
  }

  END_MENU();
}

//
// T* Choose Filament
//

int8_t feeder_index;
bool wait_for_mmu_menu;

inline void action_mmu2_chosen(const uint8_t index) {
  feeder_index = index;
  wait_for_mmu_menu = false;
}

void menu_mmu2_choose_filament() {
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_MMU2_CHOOSE_FILAMENT_HEADER, SS_DEFAULT|SS_INVERT);
  #endif
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ action_mmu2_chosen(MenuItemBase::itemIndex); });
  END_MENU();
}

//
// MMU2 / MMU3 Filament Runout
//
// Load and Load to Nozzle use the slot that was in use when the menu opened, so a runout
// is recovered with the same slot. If that slot is not known they ask for one, because the
// tool in use is unknown after an Unload and a slot taken from it would be invalid for the MMU.
//

static uint8_t pause_slot = 0xFF; // Slot in use when the Filament Change menu opened

static void _mmu2_pause_load(const uint8_t slot) {
  pause_slot = slot;
  if (_mmu2_already_loaded(slot, true)) return;   // Alert; Back returns to the screen that was open
  _mmu2_load_to_feeder(slot);   // Shows what is happening on the Status Screen
  ui.goto_screen(menu_mmu2_pause);
}

static void _mmu2_pause_load_to_nozzle(const uint8_t slot) {
  pause_slot = slot;
  _mmu2_hot_action(MMU_HOT_LOAD_TO_NOZZLE, slot, true);   // Preheats if needed, then returns to the menu
}

static void menu_mmu2_pause_load() {
  START_MENU();
  BACK_ITEM(MSG_FILAMENT_CHANGE_HEADER);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_pause_load(MenuItemBase::itemIndex); });
  END_MENU();
}

static void menu_mmu2_pause_load_to_nozzle() {
  START_MENU();
  BACK_ITEM(MSG_FILAMENT_CHANGE_HEADER);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_pause_load_to_nozzle(MenuItemBase::itemIndex); });
  END_MENU();
}

void menu_mmu2_pause() {
  ui.defer_status_screen(); // A return_to_status() (error screen, Eject) clears the flag set by mmu2_M600()
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, SS_DEFAULT|SS_INVERT);
  #endif
  ACTION_ITEM(MSG_MMU2_RESUME,          []{ wait_for_mmu_menu = false; });
  ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, []{ _mmu2_hot_action(MMU_HOT_UNLOAD, 0, true); });
  ACTION_ITEM(MSG_MMU2_EJECT_FILAMENT, []{
    if (pause_slot < EXTRUDERS) _mmu2_eject_filament(pause_slot, true);
    else ui.goto_screen(menu_mmu2_eject_filament);
  });
  ACTION_ITEM(MSG_MMU2_LOAD_FILAMENT, []{
    if (pause_slot < EXTRUDERS) _mmu2_pause_load(pause_slot); else ui.goto_screen(menu_mmu2_pause_load);
  });
  ACTION_ITEM(MSG_MMU2_LOAD_TO_NOZZLE, []{
    if (pause_slot < EXTRUDERS) _mmu2_pause_load_to_nozzle(pause_slot); else ui.goto_screen(menu_mmu2_pause_load_to_nozzle);
  });
  END_MENU();
}

void mmu2_M600(const bool automatic/*=false*/) {
  // Disable automatic switching if MMU3 is not enabled or spool join is disabled
  #if HAS_PRUSA_MMU3
    if (automatic && spooljoin.enabled) {
      uint8_t slot;
      slot = spooljoin.nextSlot();
      mmu3.load_to_nozzle(slot);
      return;
    }
  #else
    UNUSED(automatic);
  #endif
  pause_slot = TERN(HAS_PRUSA_MMU3, mmu3, mmu2).get_current_tool(); // Unknown (255) if no tool is selected

  // Hotend Idle Timeout lowers the targets while this menu waits, and nothing else puts them back.
  // Remember them so the print can resume at the temperatures it paused with.
  #if ENABLED(HOTEND_IDLE_TIMEOUT)
    celsius_t saved_hotend_target[HOTENDS];
    HOTEND_LOOP() saved_hotend_target[e] = thermalManager.degTargetHotend(e);
    #if HAS_HEATED_BED
      const celsius_t saved_bed_target = thermalManager.degTargetBed();
    #endif
  #endif

  ui.defer_status_screen();
  ui.goto_screen(menu_mmu2_pause);
  wait_for_mmu_menu = true;
  while (wait_for_mmu_menu) marlin.idle();

  // Raise (never lower) the targets again. resume_print() then waits for the hotend to reach its target.
  #if ENABLED(HOTEND_IDLE_TIMEOUT)
    HOTEND_LOOP() {
      if (thermalManager.degTargetHotend(e) < saved_hotend_target[e])
        thermalManager.setTargetHotend(saved_hotend_target[e], e);
    }
    #if HAS_HEATED_BED
      if (thermalManager.degTargetBed() < saved_bed_target)
        thermalManager.setTargetBed(saved_bed_target);
    #endif
  #endif
}

uint8_t mmu2_choose_filament() {
  ui.defer_status_screen();
  ui.goto_screen(menu_mmu2_choose_filament);
  wait_for_mmu_menu = true;
  while (wait_for_mmu_menu) marlin.idle();
  ui.return_to_status();
  return feeder_index;
}

#endif // MMU_MENUS
