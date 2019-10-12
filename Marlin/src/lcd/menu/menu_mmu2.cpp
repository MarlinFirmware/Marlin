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

#include "../../inc/MarlinConfig.h"

#if HAS_LCD_MENU && ENABLED(MMU2_MENUS)

#include "../../feature/prusa_MMU2/mmu2.h"
#include "menu_mmu2.h"
#include "menu.h"

uint8_t currentTool;
bool mmuMenuWait;

//
// Load Filament
//

void _mmu2_load_filamentToNozzle(uint8_t index) {
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf_P(0,  GET_TEXT(MSG_MMU2_LOADING_FILAMENT), int(index + 1));
  if (mmu2.load_filament_to_nozzle(index)) ui.reset_status();
}

inline void action_mmu2_load_filament_to_nozzle(const uint8_t tool) {
  _mmu2_load_filamentToNozzle(tool);
  ui.return_to_status();
}

void _mmu2_load_filament(uint8_t index) {
  ui.return_to_status();
  ui.status_printf_P(0, GET_TEXT(MSG_MMU2_LOADING_FILAMENT), int(index + 1));
  mmu2.load_filament(index);
  ui.reset_status();
}
void action_mmu2_load_all() {
  for (uint8_t i = 0; i < EXTRUDERS; i++)
    _mmu2_load_filament(i);
  ui.return_to_status();
}

void menu_mmu2_load_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU2_ALL, action_mmu2_load_all);
  ACTION_ITEM(MSG_MMU2_FILAMENT0, [](){ _mmu2_load_filament(0); });
  ACTION_ITEM(MSG_MMU2_FILAMENT1, [](){ _mmu2_load_filament(1); });
  ACTION_ITEM(MSG_MMU2_FILAMENT2, [](){ _mmu2_load_filament(2); });
  ACTION_ITEM(MSG_MMU2_FILAMENT3, [](){ _mmu2_load_filament(3); });
  ACTION_ITEM(MSG_MMU2_FILAMENT4, [](){ _mmu2_load_filament(4); });
  END_MENU();
}

void menu_mmu2_load_to_nozzle() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU2_FILAMENT0, [](){ action_mmu2_load_filament_to_nozzle(0); });
  ACTION_ITEM(MSG_MMU2_FILAMENT1, [](){ action_mmu2_load_filament_to_nozzle(1); });
  ACTION_ITEM(MSG_MMU2_FILAMENT2, [](){ action_mmu2_load_filament_to_nozzle(2); });
  ACTION_ITEM(MSG_MMU2_FILAMENT3, [](){ action_mmu2_load_filament_to_nozzle(3); });
  ACTION_ITEM(MSG_MMU2_FILAMENT4, [](){ action_mmu2_load_filament_to_nozzle(4); });
  END_MENU();
}

//
// Eject Filament
//

void _mmu2_eject_filament(uint8_t index) {
  ui.reset_status();
  ui.return_to_status();
  ui.status_printf_P(0, GET_TEXT(MSG_MMU2_EJECTING_FILAMENT), int(index + 1));
  if (mmu2.eject_filament(index, true)) ui.reset_status();
}

void action_mmu2_unload_filament() {
  ui.reset_status();
  ui.return_to_status();
  LCD_MESSAGEPGM(MSG_MMU2_UNLOADING_FILAMENT);
  idle();
  if (mmu2.unload()) ui.reset_status();
}

void menu_mmu2_eject_filament() {
  START_MENU();
  BACK_ITEM(MSG_MMU2_MENU);
  ACTION_ITEM(MSG_MMU2_FILAMENT0, [](){ _mmu2_eject_filament(0); });
  ACTION_ITEM(MSG_MMU2_FILAMENT1, [](){ _mmu2_eject_filament(1); });
  ACTION_ITEM(MSG_MMU2_FILAMENT2, [](){ _mmu2_eject_filament(2); });
  ACTION_ITEM(MSG_MMU2_FILAMENT3, [](){ _mmu2_eject_filament(3); });
  ACTION_ITEM(MSG_MMU2_FILAMENT4, [](){ _mmu2_eject_filament(4); });
  END_MENU();
}

//
// MMU2 Menu
//

void action_mmu2_reset() {
  mmu2.init();
  ui.reset_status();
}

void menu_mmu2() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  SUBMENU(MSG_MMU2_LOAD_FILAMENT, menu_mmu2_load_filament);
  SUBMENU(MSG_MMU2_LOAD_TO_NOZZLE, menu_mmu2_load_to_nozzle);
  SUBMENU(MSG_MMU2_EJECT_FILAMENT, menu_mmu2_eject_filament);
  ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, action_mmu2_unload_filament);
  ACTION_ITEM(MSG_MMU2_RESET, action_mmu2_reset);
  END_MENU();
}

//
// T* Choose Filament
//

inline void action_mmu2_choose(const uint8_t tool) {
  currentTool = tool;
  mmuMenuWait = false;
}

void menu_mmu2_choose_filament() {
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_MMU2_CHOOSE_FILAMENT_HEADER, SS_CENTER|SS_INVERT);
  #endif
  ACTION_ITEM(MSG_MMU2_FILAMENT0, [](){ action_mmu2_choose(0); });
  ACTION_ITEM(MSG_MMU2_FILAMENT1, [](){ action_mmu2_choose(1); });
  ACTION_ITEM(MSG_MMU2_FILAMENT2, [](){ action_mmu2_choose(2); });
  ACTION_ITEM(MSG_MMU2_FILAMENT3, [](){ action_mmu2_choose(3); });
  ACTION_ITEM(MSG_MMU2_FILAMENT4, [](){ action_mmu2_choose(4); });
  END_MENU();
}

//
// MMU2 Filament Runout
//

void menu_mmu2_pause() {
  currentTool = mmu2.get_current_tool();
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_FILAMENT_CHANGE_HEADER, SS_CENTER|SS_INVERT);
  #endif
  ACTION_ITEM(MSG_MMU2_RESUME, [](){ mmuMenuWait = false; });
  ACTION_ITEM(MSG_MMU2_UNLOAD_FILAMENT, [](){ mmu2.unload(); });
  ACTION_ITEM(MSG_MMU2_LOAD_FILAMENT, [](){ mmu2.load_filament(currentTool); });
  ACTION_ITEM(MSG_MMU2_LOAD_TO_NOZZLE, [](){ mmu2.load_filament_to_nozzle(currentTool); });
  END_MENU();
}

void mmu2_M600() {
  ui.defer_status_screen();
  ui.goto_screen(menu_mmu2_pause);
  mmuMenuWait = true;
  while (mmuMenuWait) idle();
}

uint8_t mmu2_choose_filament() {
  ui.defer_status_screen();
  ui.goto_screen(menu_mmu2_choose_filament);
  mmuMenuWait = true;
  while (mmuMenuWait) idle();
  ui.return_to_status();
  return currentTool;
}

#endif //  HAS_LCD_MENU && ENABLED(PRUSA_MMU2_MENUS)
