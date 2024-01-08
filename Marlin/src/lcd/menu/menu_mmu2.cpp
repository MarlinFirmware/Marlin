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

#if ALL(HAS_MARLINUI_MENU, MMU2_MENUS)

#include "../../MarlinCore.h"

#if HAS_PRUSA_MMU3
  #include "src/feature/mmu3/mmu2.h"
  #include "src/feature/mmu3/mmu2_reporting.h"
  #include "src/feature/mmu3/SpoolJoin.h"
#else
  #include "../../feature/mmu/mmu2.h"
#endif

#include "menu_mmu2.h"
#include "menu_item.h"

//
// Load Filament
//

inline void action_mmu2_load_to_nozzle(const uint8_t tool) {
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_LOADING_FILAMENT), int(tool + 1));
  #if HAS_PRUSA_MMU3
    MMU2::mmu2.load_filament_to_nozzle(tool);
  #else
    mmu2.load_to_nozzle(tool);
  #endif
  ui.reset_status();
}

void _mmu2_load_to_feeder(const uint8_t tool) {
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_LOADING_FILAMENT), int(tool + 1));

  #if HAS_PRUSA_MMU3
    MMU2::mmu2.load_filament(tool);
  #else
    mmu2.load_to_feeder(tool);
  #endif
  ui.reset_status();
}

void action_mmu2_load_all() {
  EXTRUDER_LOOP() _mmu2_load_to_feeder(e);
  ui.return_to_status();
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

void _mmu2_eject_filament(uint8_t index) {
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_EJECTING_FILAMENT), int(index + 1));
  #if HAS_PRUSA_MMU3
    if (MMU2::mmu2.eject_filament(index, true)) ui.reset_status();
  #else
    if (mmu2.eject_filament(index, true)) ui.reset_status();
  #endif
}

void _mmu2_cut_filament(uint8_t index) {
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf(0, GET_TEXT_F(MSG_MMU2_CUTTING_FILAMENT), int(index + 1));
  #if HAS_PRUSA_MMU3
    if (MMU2::mmu2.cut_filament(index, true)) ui.reset_status();
  #endif
}

void action_mmu2_unload_filament() {
  ui.reset_status();
  ui.return_to_status();
  LCD_MESSAGE(MSG_MMU2_UNLOADING_FILAMENT);
  #if HAS_PRUSA_MMU3
    MMU2::marlin_idle(false);
    if (MMU2::mmu2.unload()) ui.reset_status();
  #else
    idle();
    if (mmu2.unload()) ui.reset_status();
  #endif
}

void menu_mmu2_eject_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_eject_filament(MenuItemBase::itemIndex); });
  END_MENU();
}

// Cutter

#if HAS_PRUSA_MMU3
void menu_mmu2_cutter_set_mode(uint8_t mode){MMU2::mmu2.cutter_mode = mode;}
void menu_mmu2_cutter_disable(){menu_mmu2_cutter_set_mode(0);}
void menu_mmu2_cutter_enable(){menu_mmu2_cutter_set_mode(1);}
void menu_mmu2_cutter_always(){menu_mmu2_cutter_set_mode(2);}

void menu_mmu2_cutter() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_DISABLE, menu_mmu2_cutter_disable);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_ENABLE, menu_mmu2_cutter_enable);
  ACTION_ITEM(MSG_MMU_CUTTER_MODE_ALWAYS, menu_mmu2_cutter_always);
  END_MENU();
}

void menu_mmu2_cut_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  EXTRUDER_LOOP() ACTION_ITEM_N(e, MSG_MMU2_FILAMENT_N, []{ _mmu2_cut_filament(MenuItemBase::itemIndex); });
  END_MENU();
}

// SpoolJoin
void spool_join_status() {SpoolJoin::spooljoin.initSpoolJoinStatus();}


// Fail Stats Menu
void menu_mmu2_fail_stats_last_print(){
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH];
  char buffer2[LCD_WIDTH];

  // had to cast the uint8_t values to uint16_t before formatting them.
  uint16_t fail_num = MMU2::operation_statistics.fail_num;
  uint16_t load_fail_num = MMU2::operation_statistics.load_fail_num;

  sprintf_P(buffer1, PSTR("%hu"), fail_num);
  sprintf_P(buffer2, PSTR("%hu"), load_fail_num);

  START_SCREEN();
  STATIC_ITEM(MSG_MMU_LAST_PRINT_FAILURES, SS_INVERT);
  #ifndef __AVR__
  // TODO: I couldn't make this work on AVR
  PSTRING_ITEM(MSG_MMU_FAILS, buffer1, SS_FULL);
  PSTRING_ITEM(MSG_MMU_LOAD_FAILS, buffer2, SS_FULL);
  #endif
  END_SCREEN();
}

void menu_mmu2_fail_stas_total(){
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH];
  char buffer2[LCD_WIDTH];
  char buffer3[LCD_WIDTH];

  sprintf_P(buffer1, PSTR("%hu"), MMU2::operation_statistics.fail_total_num);
  sprintf_P(buffer2, PSTR("%hu"), MMU2::operation_statistics.load_fail_total_num);
  sprintf_P(buffer3, PSTR("%hu"), MMU2::mmu2.TMCFailures());

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

#ifdef MARLIN_DEV_MODE
void menu_mmu2_dev_increment_fail_stat(){
  MMU2::operation_statistics.increment_mmu_fails();
}

void menu_mmu2_dev_increment_load_fail_stat(){
  MMU2::operation_statistics.increment_load_fails();
}
#endif

static void mmu2_reset_stats(){
  bool result = MMU2::operation_statistics.reset_stats();
  ui.go_back();
  MarlinUI::completion_feedback(result);
}

void menu_mmu2_toolchange_stat_total(){
  if (ui.use_click()) return ui.go_back();
  char buffer1[LCD_WIDTH];
  sprintf_P(buffer1, PSTR("%u"), MMU2::operation_statistics.tool_change_counter);

  char buffer2[LCD_WIDTH];
  sprintf_P(buffer2, PSTR("%u"), MMU2::operation_statistics.tool_change_total_counter);

  START_SCREEN();
  STATIC_ITEM(MSG_MMU_MATERIAL_CHANGES, SS_INVERT);
  #ifndef __AVR__
  // TODO: I couldn't make this work on AVR
  PSTRING_ITEM(MSG_MMU_LAST_PRINT, buffer1, SS_FULL);
  PSTRING_ITEM(MSG_MMU_TOTAL, buffer2, SS_FULL);
  #endif
  END_SCREEN();
}

void menu_mmu2_statistics() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  #ifdef MARLIN_DEV_MODE
    ACTION_ITEM(MSG_MMU_DEV_INCREMENT_FAILS, menu_mmu2_dev_increment_fail_stat);
    ACTION_ITEM(MSG_MMU_DEV_INCREMENT_LOAD_FAILS, menu_mmu2_dev_increment_load_fail_stat);
  #endif
  SUBMENU(MSG_MMU_LAST_PRINT, menu_mmu2_fail_stats_last_print);
  SUBMENU(MSG_MMU_TOTAL, menu_mmu2_fail_stas_total);
  SUBMENU(MSG_MMU_MATERIAL_CHANGES, menu_mmu2_toolchange_stat_total);
  CONFIRM_ITEM(MSG_MMU_RESET_STATS,
    MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
    mmu2_reset_stats, nullptr,
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
    #ifdef MMU2_RST_PIN
      MMU2::mmu2.Reset(MMU2::MMU2::ResetForm::Hardware);
    #else
      MMU2::mmu2.Reset(MMU2::MMU2::ResetForm::Software);
    #endif
  #else
    mmu2.init();
  #endif
  ui.reset_status();
}

void menu_mmu2() {
  const bool busy = printingIsActive()
    #if HAS_MEDIA
      , card_detected = card.isMounted()
      , card_open = card_detected && card.isFileOpen()
    #endif
  ;

  START_MENU();
  BACK_ITEM(MSG_MAIN_MENU);
  if (!busy){
    SUBMENU(MSG_MMU2_LOAD_FILAMENT, menu_mmu2_load_filament);
    SUBMENU(MSG_MMU2_LOAD_TO_NOZZLE, menu_mmu2_load_to_nozzle);
    SUBMENU(MSG_MMU2_EJECT_FILAMENT, menu_mmu2_eject_filament);
    ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, action_mmu2_unload_filament);
  }

  #if HAS_PRUSA_MMU3
    // SUBMENU(MSG_MMU_CUTTER_MODE, menu_mmu2_cutter);
    bool cutter_enabled = MMU2::mmu2.cutter_mode != 0;
    editable.state = cutter_enabled;
    EDIT_ITEM(bool, MSG_MMU_CUTTER, &cutter_enabled, []{
      menu_mmu2_cutter_set_mode((uint8_t)!editable.state);
    });
    if (!busy && MMU2::cutter_enabled()){
      SUBMENU(MSG_MMU2_CUT_FILAMENT, menu_mmu2_cut_filament);
    }
    EDIT_ITEM(bool, MSG_MMU_SPOOL_JOIN, &SpoolJoin::spooljoin.enabled, spool_join_status);

    SUBMENU(MSG_MMU_STATISTICS, menu_mmu2_statistics);
  #endif

  ACTION_ITEM(MSG_MMU2_RESET, action_mmu2_reset);
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
// MMU2 Filament Runout
//

void menu_mmu2_pause() {
  #if HAS_PRUSA_MMU3
    feeder_index = MMU2::mmu2.get_current_tool();
  #else
    feeder_index = mmu2.get_current_tool();
  #endif
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, SS_DEFAULT|SS_INVERT);
  #endif
  ACTION_ITEM(MSG_MMU2_RESUME,          []{ wait_for_mmu_menu = false; });
  #if HAS_PRUSA_MMU3
    ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, []{ MMU2::mmu2.unload(); });
    ACTION_ITEM(MSG_MMU2_LOAD_FILAMENT,   []{ MMU2::mmu2.load_filament(feeder_index); });
    ACTION_ITEM(MSG_MMU2_LOAD_TO_NOZZLE,  []{ MMU2::mmu2.load_filament_to_nozzle(feeder_index); });
  #else
    ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, []{ mmu2.unload(); });
    ACTION_ITEM(MSG_MMU2_LOAD_FILAMENT,   []{ mmu2.load_to_feeder(feeder_index); });
    ACTION_ITEM(MSG_MMU2_LOAD_TO_NOZZLE,  []{ mmu2.load_to_nozzle(feeder_index); });
  #endif
  END_MENU();
}

void mmu2_M600(bool automatic) {
  // Disable automatic switching if MMU3 is not enabled or spool join is disabled
  #if HAS_PRUSA_MMU3
    if(automatic && SpoolJoin::spooljoin.enabled){
      uint8_t slot;
      slot = SpoolJoin::spooljoin.nextSlot();
      MMU2::mmu2.load_filament_to_nozzle(slot);
    } else {
  #endif
      ui.defer_status_screen();
      ui.goto_screen(menu_mmu2_pause);
      wait_for_mmu_menu = true;
      while (wait_for_mmu_menu) idle();
  #if HAS_PRUSA_MMU3
    }
  #endif
}

uint8_t mmu2_choose_filament() {
  ui.defer_status_screen();
  ui.goto_screen(menu_mmu2_choose_filament);
  wait_for_mmu_menu = true;
  while (wait_for_mmu_menu) idle();
  ui.return_to_status();
  return feeder_index;
}

#endif // HAS_MARLINUI_MENU && MMU2_MENUS
