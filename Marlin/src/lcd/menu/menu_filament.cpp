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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Filament Change Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, ADVANCED_PAUSE_FEATURE)

#include "menu.h"
#include "../../module/temperature.h"
#include "../../feature/pause.h"
#include "../../gcode/queue.h"
#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

//
// Change Filament > Change/Unload/Load Filament
//
static PauseMode _change_filament_mode; // = PAUSE_MODE_PAUSE_PRINT
static int8_t _change_filament_extruder; // = 0

inline PGM_P _change_filament_command() {
  switch (_change_filament_mode) {
    case PAUSE_MODE_LOAD_FILAMENT:    return PSTR("M701 T%d");
    case PAUSE_MODE_UNLOAD_FILAMENT:  return _change_filament_extruder >= 0
                                           ? PSTR("M702 T%d") : PSTR("M702 ;%d");
    case PAUSE_MODE_CHANGE_FILAMENT:
    case PAUSE_MODE_PAUSE_PRINT:
    default: break;
  }
  return PSTR("M600 B0 T%d");
}

// Initiate Filament Load/Unload/Change at the specified temperature
static void _change_filament_with_temp(const uint16_t celsius) {
  char cmd[11];
  sprintf_P(cmd, _change_filament_command(), _change_filament_extruder);
  thermalManager.setTargetHotend(celsius, _change_filament_extruder);
  queue.inject(cmd);
}

static void _change_filament_with_preset() {
  _change_filament_with_temp(ui.material_preset[MenuItemBase::itemIndex].hotend_temp);
}

static void _change_filament_with_custom() {
  _change_filament_with_temp(thermalManager.temp_hotend[MenuItemBase::itemIndex].target);
}

//
// Menu to choose the temperature and start Filament Change
//

inline PGM_P change_filament_header(const PauseMode mode) {
  switch (mode) {
    case PAUSE_MODE_LOAD_FILAMENT:   return GET_TEXT(MSG_FILAMENTLOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT: return GET_TEXT(MSG_FILAMENTUNLOAD);
    default: break;
  }
  return GET_TEXT(MSG_FILAMENTCHANGE);
}

void _menu_temp_filament_op(const PauseMode mode, const int8_t extruder) {
  _change_filament_mode = mode;
  _change_filament_extruder = extruder;
  START_MENU();
  if (LCD_HEIGHT >= 4) STATIC_ITEM_P(change_filament_header(mode), SS_CENTER|SS_INVERT);
  BACK_ITEM(MSG_BACK);
  #if PREHEAT_COUNT
    LOOP_L_N(m, PREHEAT_COUNT)
      ACTION_ITEM_N_S(m, ui.get_preheat_label(m), MSG_PREHEAT_M, _change_filament_with_preset);
  #endif
  EDIT_ITEM_FAST_N(int3, extruder, MSG_PREHEAT_CUSTOM, &thermalManager.temp_hotend[extruder].target,
    EXTRUDE_MINTEMP, thermalManager.heater_maxtemp[extruder] - HOTEND_OVERSHOOT,
    _change_filament_with_custom
  );
  END_MENU();
}

/**
 *
 * "Change Filament" submenu
 *
 */
#if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)

  void menu_change_filament() {
    // Say "filament change" when no print is active
    editable.int8 = printingIsPaused() ? PAUSE_MODE_PAUSE_PRINT : PAUSE_MODE_CHANGE_FILAMENT;

    #if E_STEPPERS > 1 && ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
      bool too_cold = false;
      for (uint8_t s = 0; !too_cold && s < E_STEPPERS; s++)
        too_cold = thermalManager.targetTooColdToExtrude(s);
    #endif

    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      const bool is_busy = printer_busy();
    #endif

    START_MENU();
    BACK_ITEM(MSG_MAIN);

    // Change filament
    #if E_STEPPERS == 1
      PGM_P const msg = GET_TEXT(MSG_FILAMENTCHANGE);
      if (thermalManager.targetTooColdToExtrude(active_extruder))
        SUBMENU_P(msg, []{ _menu_temp_filament_op(PAUSE_MODE_CHANGE_FILAMENT, 0); });
      else
        GCODES_ITEM_P(msg, PSTR("M600 B0"));
    #else
      PGM_P const msg = GET_TEXT(MSG_FILAMENTCHANGE_E);
      LOOP_L_N(s, E_STEPPERS) {
        if (thermalManager.targetTooColdToExtrude(s))
          SUBMENU_N_P(s, msg, []{ _menu_temp_filament_op(PAUSE_MODE_CHANGE_FILAMENT, MenuItemBase::itemIndex); });
        else {
          ACTION_ITEM_N_P(s, msg, []{
            char cmd[13];
            sprintf_P(cmd, PSTR("M600 B0 T%i"), int(MenuItemBase::itemIndex));
            queue.inject(cmd);
          });
        }
      }
    #endif

    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      if (!is_busy) {
        // Load filament
        #if E_STEPPERS == 1
          PGM_P const msg_load = GET_TEXT(MSG_FILAMENTLOAD);
          if (thermalManager.targetTooColdToExtrude(active_extruder))
            SUBMENU_P(msg_load, []{ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 0); });
          else
            GCODES_ITEM_P(msg_load, PSTR("M701"));
        #else
          PGM_P const msg_load = GET_TEXT(MSG_FILAMENTLOAD_E);
          LOOP_L_N(s, E_STEPPERS) {
            if (thermalManager.targetTooColdToExtrude(s))
              SUBMENU_N_P(s, msg_load, []{ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, MenuItemBase::itemIndex); });
            else {
              ACTION_ITEM_N_P(s, msg_load, []{
                char cmd[12];
                sprintf_P(cmd, PSTR("M701 T%i"), int(MenuItemBase::itemIndex));
                queue.inject(cmd);
              });
            }
          }
        #endif

        // Unload filament
        #if E_STEPPERS == 1
          PGM_P const msg_unload = GET_TEXT(MSG_FILAMENTUNLOAD);
          if (thermalManager.targetTooColdToExtrude(active_extruder))
            SUBMENU_P(msg_unload, []{ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 0); });
          else
            GCODES_ITEM_P(msg_unload, PSTR("M702"));
        #else
          #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
            if (too_cold)
              SUBMENU(MSG_FILAMENTUNLOAD_ALL, []{ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, -1); });
            else
              GCODES_ITEM(MSG_FILAMENTUNLOAD_ALL, PSTR("M702"));
          #endif
          PGM_P const msg_unload = GET_TEXT(MSG_FILAMENTUNLOAD_E);
          LOOP_L_N(s, E_STEPPERS) {
            if (thermalManager.targetTooColdToExtrude(s))
              SUBMENU_N_P(s, msg_unload, []{ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, MenuItemBase::itemIndex); });
            else {
              ACTION_ITEM_N_P(s, msg_unload, []{
                char cmd[12];
                sprintf_P(cmd, PSTR("M702 T%i"), int(MenuItemBase::itemIndex));
                queue.inject(cmd);
              });
            }
          }
        #endif
      } // !printer_busy
    #endif

    END_MENU();
  }
#endif

static uint8_t hotend_status_extruder = 0;

static PGM_P pause_header() {
  switch (pause_mode) {
    case PAUSE_MODE_CHANGE_FILAMENT:  return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER);
    case PAUSE_MODE_LOAD_FILAMENT:    return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_LOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT:  return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_UNLOAD);
    default: break;
  }
  return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_PAUSE);
}

// Portions from STATIC_ITEM...
#define HOTEND_STATUS_ITEM() do { \
  if (_menuLineNr == _thisItemNr) { \
    if (ui.should_draw()) { \
      MenuItem_static::draw(_lcdLineNr, GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE), SS_INVERT); \
      ui.draw_hotend_status(_lcdLineNr, hotend_status_extruder); \
    } \
    if (_skipStatic && encoderLine <= _thisItemNr) { \
      ui.encoderPosition += ENCODER_STEPS_PER_MENU_ITEM; \
      ++encoderLine; \
    } \
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT); \
  } \
  ++_thisItemNr; \
}while(0)

void menu_pause_option() {
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_FILAMENT_CHANGE_OPTION_HEADER);
  #endif
  ACTION_ITEM(MSG_FILAMENT_CHANGE_OPTION_PURGE, []{ pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE; });

  #if HAS_FILAMENT_SENSOR
    const bool still_out = runout.filament_ran_out;
    if (still_out)
      EDIT_ITEM(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
  #else
    constexpr bool still_out = false;
  #endif

  if (!still_out)
    ACTION_ITEM(MSG_FILAMENT_CHANGE_OPTION_RESUME, []{ pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT; });

  END_MENU();
}

//
// ADVANCED_PAUSE_FEATURE message screens
//
// Warning: msg must have three null bytes to delimit lines!
//
void _lcd_pause_message(PGM_P const msg) {
  PGM_P const msg1 = msg;
  PGM_P const msg2 = msg1 + strlen_P(msg1) + 1;
  PGM_P const msg3 = msg2 + strlen_P(msg2) + 1;
  const bool has2 = msg2[0], has3 = msg3[0],
             skip1 = !has2 && (LCD_HEIGHT) >= 5;

  START_SCREEN();
  STATIC_ITEM_P(pause_header(), SS_CENTER|SS_INVERT);           // 1: Header
  if (skip1) SKIP_ITEM();                                       // Move a single-line message down
  STATIC_ITEM_P(msg1);                                          // 2: Message Line 1
  if (has2) STATIC_ITEM_P(msg2);                                // 3: Message Line 2
  if (has3 && (LCD_HEIGHT) >= 5) STATIC_ITEM_P(msg3);           // 4: Message Line 3 (if LCD has 5 lines)
  if (skip1 + 1 + has2 + has3 < (LCD_HEIGHT) - 2) SKIP_ITEM();  // Push Hotend Status down, if needed
  HOTEND_STATUS_ITEM();                                         // 5: Hotend Status
  END_SCREEN();
}

void lcd_pause_parking_message()  { _lcd_pause_message(GET_TEXT(MSG_PAUSE_PRINT_PARKING));     }
void lcd_pause_changing_message() { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_INIT));    }
void lcd_pause_unload_message()   { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_UNLOAD));  }
void lcd_pause_heating_message()  { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_HEATING)); }
void lcd_pause_heat_message()     { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_HEAT));    }
void lcd_pause_insert_message()   { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_INSERT));  }
void lcd_pause_load_message()     { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_LOAD));    }
void lcd_pause_waiting_message()  { _lcd_pause_message(GET_TEXT(MSG_ADVANCED_PAUSE_WAITING));  }
void lcd_pause_resume_message()   { _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_RESUME));  }

void lcd_pause_purge_message() {
  #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
    _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_CONT_PURGE));
  #else
    _lcd_pause_message(GET_TEXT(MSG_FILAMENT_CHANGE_PURGE));
  #endif
}

FORCE_INLINE screenFunc_t ap_message_screen(const PauseMessage message) {
  switch (message) {
    case PAUSE_MESSAGE_PARKING:  return lcd_pause_parking_message;
    case PAUSE_MESSAGE_CHANGING: return lcd_pause_changing_message;
    case PAUSE_MESSAGE_UNLOAD:   return lcd_pause_unload_message;
    case PAUSE_MESSAGE_WAITING:  return lcd_pause_waiting_message;
    case PAUSE_MESSAGE_INSERT:   return lcd_pause_insert_message;
    case PAUSE_MESSAGE_LOAD:     return lcd_pause_load_message;
    case PAUSE_MESSAGE_PURGE:    return lcd_pause_purge_message;
    case PAUSE_MESSAGE_RESUME:   return lcd_pause_resume_message;
    case PAUSE_MESSAGE_HEAT:     return lcd_pause_heat_message;
    case PAUSE_MESSAGE_HEATING:  return lcd_pause_heating_message;
    case PAUSE_MESSAGE_OPTION:   pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
                                 return menu_pause_option;
    case PAUSE_MESSAGE_STATUS:
    default: break;
  }
  return nullptr;
}

void lcd_pause_show_message(
  const PauseMessage message,
  const PauseMode mode/*=PAUSE_MODE_SAME*/,
  const uint8_t extruder/*=active_extruder*/
) {
  if (mode != PAUSE_MODE_SAME) pause_mode = mode;
  hotend_status_extruder = extruder;
  const screenFunc_t next_screen = ap_message_screen(message);
  if (next_screen) {
    ui.defer_status_screen();
    ui.goto_screen(next_screen);
  }
  else
    ui.return_to_status();
}

#endif // HAS_LCD_MENU && ADVANCED_PAUSE_FEATURE
