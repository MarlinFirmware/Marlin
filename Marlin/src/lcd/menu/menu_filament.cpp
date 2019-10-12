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

//
// Filament Change Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(ADVANCED_PAUSE_FEATURE)

#include "menu.h"
#include "../../module/temperature.h"
#include "../../feature/pause.h"
#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

//
// Change Filament > Change/Unload/Load Filament
//
static PauseMode _change_filament_temp_mode; // =PAUSE_MODE_PAUSE_PRINT
static int8_t _change_filament_temp_extruder; // =0

inline PGM_P _change_filament_temp_command() {
  switch (_change_filament_temp_mode) {
    case PAUSE_MODE_LOAD_FILAMENT:
      return PSTR("M701 T%d");
    case PAUSE_MODE_UNLOAD_FILAMENT:
      return _change_filament_temp_extruder >= 0 ? PSTR("M702 T%d") : PSTR("M702 ;%d");
    case PAUSE_MODE_CHANGE_FILAMENT:
    case PAUSE_MODE_PAUSE_PRINT:
    default:
      return PSTR("M600 B0 T%d");
  }
  return GET_TEXT(MSG_FILAMENTCHANGE);
}

// Initiate Filament Load/Unload/Change at the specified temperature
static void _change_filament_temp(const uint16_t temperature) {
  char cmd[11];
  sprintf_P(cmd, _change_filament_temp_command(), _change_filament_temp_extruder);
  thermalManager.setTargetHotend(temperature, _change_filament_temp_extruder);
  lcd_enqueue_one_now(cmd);
}

//
// Menu to choose the temperature and start Filament Change
//

inline PGM_P change_filament_header(const PauseMode mode) {
  switch (mode) {
    case PAUSE_MODE_LOAD_FILAMENT:
      return GET_TEXT(MSG_FILAMENTLOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT:
      return GET_TEXT(MSG_FILAMENTUNLOAD);
    default: break;
  }
  return GET_TEXT(MSG_FILAMENTCHANGE);
}

void _menu_temp_filament_op(const PauseMode mode, const int8_t extruder) {
  _change_filament_temp_mode = mode;
  _change_filament_temp_extruder = extruder;
  START_MENU();
  if (LCD_HEIGHT >= 4) STATIC_ITEM_P(change_filament_header(mode), SS_CENTER|SS_INVERT);
  BACK_ITEM(MSG_BACK);
  ACTION_ITEM(MSG_PREHEAT_1, [](){ _change_filament_temp(ui.preheat_hotend_temp[0]); });
  ACTION_ITEM(MSG_PREHEAT_2, [](){ _change_filament_temp(ui.preheat_hotend_temp[1]); });
  EDIT_ITEM_FAST(int3, MSG_PREHEAT_CUSTOM, &thermalManager.temp_hotend[_change_filament_temp_extruder].target, EXTRUDE_MINTEMP, heater_maxtemp[extruder] - 15, [](){
    _change_filament_temp(thermalManager.temp_hotend[_change_filament_temp_extruder].target);
  });
  END_MENU();
}

/**
 *
 * "Change Filament" submenu
 *
 */
#if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
  void menu_change_filament() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);

    // Say "filament change" when no print is active
    editable.int8 = printingIsPaused() ? PAUSE_MODE_PAUSE_PRINT : PAUSE_MODE_CHANGE_FILAMENT;

    // Change filament
    #if E_STEPPERS == 1
      PGM_P const msg0 = GET_TEXT(MSG_FILAMENTCHANGE);
      if (thermalManager.targetTooColdToExtrude(active_extruder))
        MENU_ITEM_P(submenu, msg0, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 0); });
      else
        MENU_ITEM_P(gcode, msg0, PSTR("M600 B0"));
    #else
      PGM_P const msg0 = GET_TEXT(MSG_FILAMENTCHANGE_E0);
      PGM_P const msg1 = GET_TEXT(MSG_FILAMENTCHANGE_E1);
      if (thermalManager.targetTooColdToExtrude(0))
        MENU_ITEM_P(submenu, msg0, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 0); });
      else
        MENU_ITEM_P(gcode, msg0, PSTR("M600 B0 T0"));
      if (thermalManager.targetTooColdToExtrude(1))
        MENU_ITEM_P(submenu, msg1, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 1); });
      else
        MENU_ITEM_P(gcode, msg1, PSTR("M600 B0 T1"));
      #if E_STEPPERS > 2
        PGM_P const msg2 = GET_TEXT(MSG_FILAMENTCHANGE_E2);
        if (thermalManager.targetTooColdToExtrude(2))
          MENU_ITEM_P(submenu, msg2, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 2); });
        else
          MENU_ITEM_P(gcode, msg2, PSTR("M600 B0 T2"));
        #if E_STEPPERS > 3
          PGM_P const msg3 = GET_TEXT(MSG_FILAMENTCHANGE_E3);
          if (thermalManager.targetTooColdToExtrude(3))
            MENU_ITEM_P(submenu, msg3, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 3); });
          else
            MENU_ITEM_P(gcode, msg3, PSTR("M600 B0 T3"));
          #if E_STEPPERS > 4
            PGM_P const msg4 = GET_TEXT(MSG_FILAMENTCHANGE_E4);
            if (thermalManager.targetTooColdToExtrude(4))
              MENU_ITEM_P(submenu, msg4, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 4); });
            else
              MENU_ITEM_P(gcode, msg4, PSTR("M600 B0 T4"));
            #if E_STEPPERS > 5
              PGM_P const msg5 = GET_TEXT(MSG_FILAMENTCHANGE_E5);
              if (thermalManager.targetTooColdToExtrude(5))
                MENU_ITEM_P(submenu, msg5, [](){ _menu_temp_filament_op(PauseMode(editable.int8), 5); });
              else
                MENU_ITEM_P(gcode, msg5, PSTR("M600 B0 T5"));
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #endif // E_STEPPERS == 1

    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      if (!printer_busy()) {
        // Load filament
        #if E_STEPPERS == 1
          PGM_P const msg0 = GET_TEXT(MSG_FILAMENTLOAD);
          if (thermalManager.targetTooColdToExtrude(active_extruder))
            MENU_ITEM_P(submenu, msg0, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 0); });
          else
            MENU_ITEM_P(gcode, msg0, PSTR("M701"));
        #else
          PGM_P const msg0 = GET_TEXT(MSG_FILAMENTLOAD_E0);
          PGM_P const msg1 = GET_TEXT(MSG_FILAMENTLOAD_E1);
          if (thermalManager.targetTooColdToExtrude(0))
            MENU_ITEM_P(submenu, msg0, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 0); });
          else
            MENU_ITEM_P(gcode, msg0, PSTR("M701 T0"));
          if (thermalManager.targetTooColdToExtrude(1))
            MENU_ITEM_P(submenu, msg1, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 1); });
          else
            MENU_ITEM_P(gcode, msg1, PSTR("M701 T1"));
          #if E_STEPPERS > 2
            PGM_P const msg2 = GET_TEXT(MSG_FILAMENTLOAD_E2);
            if (thermalManager.targetTooColdToExtrude(2))
              MENU_ITEM_P(submenu, msg2, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 2); });
            else
              MENU_ITEM_P(gcode, msg2, PSTR("M701 T2"));
            #if E_STEPPERS > 3
              PGM_P const msg3 = GET_TEXT(MSG_FILAMENTLOAD_E3);
              if (thermalManager.targetTooColdToExtrude(3))
                MENU_ITEM_P(submenu, msg3, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 3); });
              else
                MENU_ITEM_P(gcode, msg3, PSTR("M701 T3"));
              #if E_STEPPERS > 4
                PGM_P const msg4 = GET_TEXT(MSG_FILAMENTLOAD_E4);
                if (thermalManager.targetTooColdToExtrude(4))
                  MENU_ITEM_P(submenu, msg4, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 4); });
                else
                  MENU_ITEM_P(gcode, msg4, PSTR("M701 T4"));
                #if E_STEPPERS > 5
                  PGM_P const msg5 = GET_TEXT(MSG_FILAMENTLOAD_E5);
                  if (thermalManager.targetTooColdToExtrude(5))
                    MENU_ITEM_P(submenu, msg5, [](){ _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 5); });
                  else
                    MENU_ITEM_P(gcode, msg5, PSTR("M701 T5"));
                #endif // E_STEPPERS > 5
              #endif // E_STEPPERS > 4
            #endif // E_STEPPERS > 3
          #endif // E_STEPPERS > 2
        #endif // E_STEPPERS == 1

        // Unload filament
        #if E_STEPPERS == 1
          if (thermalManager.targetHotEnoughToExtrude(active_extruder))
            GCODES_ITEM(MSG_FILAMENTUNLOAD, PSTR("M702"));
          else
            SUBMENU(MSG_FILAMENTUNLOAD, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 0); });
        #else
          #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
            if (JOIN_N(E_STEPPERS, &&,
              thermalManager.targetHotEnoughToExtrude(0),
              thermalManager.targetHotEnoughToExtrude(1),
              thermalManager.targetHotEnoughToExtrude(2),
              thermalManager.targetHotEnoughToExtrude(3),
              thermalManager.targetHotEnoughToExtrude(4),
              thermalManager.targetHotEnoughToExtrude(5))
            ) GCODES_ITEM(MSG_FILAMENTUNLOAD_ALL, PSTR("M702"));
          else
            SUBMENU(MSG_FILAMENTUNLOAD_ALL, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, -1); });
          #endif
          if (thermalManager.targetHotEnoughToExtrude(0))
            GCODES_ITEM(MSG_FILAMENTUNLOAD_E0, PSTR("M702 T0"));
          else
            SUBMENU(MSG_FILAMENTUNLOAD_E0, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 0); });
          if (thermalManager.targetHotEnoughToExtrude(1))
            GCODES_ITEM(MSG_FILAMENTUNLOAD_E1, PSTR("M702 T1"));
          else
            SUBMENU(MSG_FILAMENTUNLOAD_E1, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 1); });
          #if E_STEPPERS > 2
            if (thermalManager.targetHotEnoughToExtrude(2))
              GCODES_ITEM(MSG_FILAMENTUNLOAD_E2, PSTR("M702 T2"));
            else
              SUBMENU(MSG_FILAMENTUNLOAD_E2, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 2); });
            #if E_STEPPERS > 3
              if (thermalManager.targetHotEnoughToExtrude(3))
                GCODES_ITEM(MSG_FILAMENTUNLOAD_E3, PSTR("M702 T3"));
              else
                SUBMENU(MSG_FILAMENTUNLOAD_E3, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 3); });
              #if E_STEPPERS > 4
                if (thermalManager.targetHotEnoughToExtrude(4))
                  GCODES_ITEM(MSG_FILAMENTUNLOAD_E4, PSTR("M702 T4"));
                else
                  SUBMENU(MSG_FILAMENTUNLOAD_E4, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 4); });
                #if E_STEPPERS > 5
                  if (thermalManager.targetHotEnoughToExtrude(5))
                    GCODES_ITEM(MSG_FILAMENTUNLOAD_E5, PSTR("M702 T5"));
                  else
                    SUBMENU(MSG_FILAMENTUNLOAD_E5, [](){ _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 5); });
                #endif // E_STEPPERS > 5
              #endif // E_STEPPERS > 4
            #endif // E_STEPPERS > 3
          #endif // E_STEPPERS > 2
        #endif // E_STEPPERS == 1
      }
    #endif

    END_MENU();
  }
#endif

static uint8_t hotend_status_extruder = 0;

static PGM_P pause_header() {
  switch (pause_mode) {
    case PAUSE_MODE_CHANGE_FILAMENT:
      return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER);
    case PAUSE_MODE_LOAD_FILAMENT:
      return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_LOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT:
      return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_UNLOAD);
    default: break;
  }
  return GET_TEXT(MSG_FILAMENT_CHANGE_HEADER_PAUSE);
}

// Portions from STATIC_ITEM...
#define HOTEND_STATUS_ITEM() do { \
  if (_menuLineNr == _thisItemNr) { \
    if (ui.should_draw()) { \
      draw_menu_item_static(_lcdLineNr, GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE), SS_INVERT); \
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
  ACTION_ITEM(MSG_FILAMENT_CHANGE_OPTION_PURGE, [](){ pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE; });
  #if HAS_FILAMENT_SENSOR
    if (runout.filament_ran_out)
      EDIT_ITEM(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
    else
  #endif
      ACTION_ITEM(MSG_FILAMENT_CHANGE_OPTION_RESUME, [](){ pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT; });
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

void lcd_pause_pausing_message()  { _lcd_pause_message(GET_TEXT(MSG_PAUSE_PRINT_INIT));        }
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
    case PAUSE_MESSAGE_PAUSING:  return lcd_pause_pausing_message;
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
