/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
    case PAUSE_MODE_PAUSE_PRINT:
    default:
      return PSTR("M600 B0 T%d");
  }
  return PSTR(MSG_FILAMENTCHANGE);
}

static void _change_filament_temp(const uint16_t temperature) {
  char cmd[11];
  sprintf_P(cmd, _change_filament_temp_command(), _change_filament_temp_extruder);
  thermalManager.setTargetHotend(temperature, _change_filament_temp_extruder);
  lcd_enqueue_command(cmd);
}
inline void _lcd_change_filament_temp_1_func()    { _change_filament_temp(ui.preheat_hotend_temp[0]); }
inline void _lcd_change_filament_temp_2_func()    { _change_filament_temp(ui.preheat_hotend_temp[1]); }
inline void _lcd_change_filament_temp_custom_cb() { _change_filament_temp(thermalManager.temp_hotend[_change_filament_temp_extruder].target); }

static PGM_P change_filament_header(const PauseMode mode) {
  switch (mode) {
    case PAUSE_MODE_LOAD_FILAMENT:
      return PSTR(MSG_FILAMENTLOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT:
      return PSTR(MSG_FILAMENTUNLOAD);
    default: break;
  }
  return PSTR(MSG_FILAMENTCHANGE);
}

void _menu_temp_filament_op(const PauseMode mode, const int8_t extruder) {
  _change_filament_temp_mode = mode;
  _change_filament_temp_extruder = extruder;
  START_MENU();
  if (LCD_HEIGHT >= 4) STATIC_ITEM_P(change_filament_header(mode), true, true);
  MENU_BACK(MSG_BACK);
  MENU_ITEM(function, MSG_PREHEAT_1, _lcd_change_filament_temp_1_func);
  MENU_ITEM(function, MSG_PREHEAT_2, _lcd_change_filament_temp_2_func);
  uint16_t max_temp;
  switch (extruder) {
    default: max_temp = HEATER_0_MAXTEMP;
    #if HOTENDS > 1
      case 1: max_temp = HEATER_1_MAXTEMP; break;
      #if HOTENDS > 2
        case 2: max_temp = HEATER_2_MAXTEMP; break;
        #if HOTENDS > 3
          case 3: max_temp = HEATER_3_MAXTEMP; break;
          #if HOTENDS > 4
            case 4: max_temp = HEATER_4_MAXTEMP; break;
            #if HOTENDS > 5
              case 5: max_temp = HEATER_5_MAXTEMP; break;
            #endif
          #endif
        #endif
      #endif
    #endif
  }
  MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_PREHEAT_CUSTOM, &thermalManager.temp_hotend[_change_filament_temp_extruder].target, EXTRUDE_MINTEMP, max_temp - 15, _lcd_change_filament_temp_custom_cb);
  END_MENU();
}
#if E_STEPPERS
  void menu_temp_e0_filament_change()  { _menu_temp_filament_op(PAUSE_MODE_PAUSE_PRINT, 0); }
  void menu_temp_e0_filament_load()    { _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 0); }
  void menu_temp_e0_filament_unload()  { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 0); }
  #if E_STEPPERS > 1
    void menu_temp_e1_filament_change()  { _menu_temp_filament_op(PAUSE_MODE_PAUSE_PRINT, 1); }
    void menu_temp_e1_filament_load()    { _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 1); }
    void menu_temp_e1_filament_unload()  { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 1); }
    #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
      void menu_unload_filament_all_temp() { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, -1); }
    #endif
    #if E_STEPPERS > 2
      void menu_temp_e2_filament_change()  { _menu_temp_filament_op(PAUSE_MODE_PAUSE_PRINT, 2); }
      void menu_temp_e2_filament_load()    { _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 2); }
      void menu_temp_e2_filament_unload()  { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 2); }
      #if E_STEPPERS > 3
        void menu_temp_e3_filament_change()  { _menu_temp_filament_op(PAUSE_MODE_PAUSE_PRINT, 3); }
        void menu_temp_e3_filament_load()    { _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 3); }
        void menu_temp_e3_filament_unload()  { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 3); }
        #if E_STEPPERS > 4
          void menu_temp_e4_filament_change()  { _menu_temp_filament_op(PAUSE_MODE_PAUSE_PRINT, 4); }
          void menu_temp_e4_filament_load()    { _menu_temp_filament_op(PAUSE_MODE_LOAD_FILAMENT, 4); }
          void menu_temp_e4_filament_unload()  { _menu_temp_filament_op(PAUSE_MODE_UNLOAD_FILAMENT, 4); }
        #endif // E_STEPPERS > 4
      #endif // E_STEPPERS > 3
    #endif // E_STEPPERS > 2
  #endif // E_STEPPERS > 1
#endif // E_STEPPERS

/**
 *
 * "Change Filament" submenu
 *
 */
#if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
  void menu_change_filament() {
    START_MENU();
    MENU_BACK(MSG_MAIN);

    // Change filament
    #if E_STEPPERS == 1
      PGM_P msg0 = PSTR(MSG_FILAMENTCHANGE);
      if (thermalManager.targetTooColdToExtrude(active_extruder))
        MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_change);
      else
        MENU_ITEM_P(gcode, msg0, PSTR("M600 B0"));
    #else
      PGM_P msg0 = PSTR(MSG_FILAMENTCHANGE " " MSG_E1);
      PGM_P msg1 = PSTR(MSG_FILAMENTCHANGE " " MSG_E2);
      if (thermalManager.targetTooColdToExtrude(0))
        MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_change);
      else
        MENU_ITEM_P(gcode, msg0, PSTR("M600 B0 T0"));
      if (thermalManager.targetTooColdToExtrude(1))
        MENU_ITEM_P(submenu, msg1, menu_temp_e1_filament_change);
      else
        MENU_ITEM_P(gcode, msg1, PSTR("M600 B0 T1"));
      #if E_STEPPERS > 2
        PGM_P msg2 = PSTR(MSG_FILAMENTCHANGE " " MSG_E3);
        if (thermalManager.targetTooColdToExtrude(2))
          MENU_ITEM_P(submenu, msg2, menu_temp_e2_filament_change);
        else
          MENU_ITEM_P(gcode, msg2, PSTR("M600 B0 T2"));
        #if E_STEPPERS > 3
          PGM_P msg3 = PSTR(MSG_FILAMENTCHANGE " " MSG_E4);
          if (thermalManager.targetTooColdToExtrude(3))
            MENU_ITEM_P(submenu, msg3, menu_temp_e3_filament_change);
          else
            MENU_ITEM_P(gcode, msg3, PSTR("M600 B0 T3"));
          #if E_STEPPERS > 4
            PGM_P msg4 = PSTR(MSG_FILAMENTCHANGE " " MSG_E5);
            if (thermalManager.targetTooColdToExtrude(4))
              MENU_ITEM_P(submenu, msg4, menu_temp_e4_filament_change);
            else
              MENU_ITEM_P(gcode, msg4, PSTR("M600 B0 T4"));
            #if E_STEPPERS > 5
              PGM_P msg5 = PSTR(MSG_FILAMENTCHANGE " " MSG_E6);
              if (thermalManager.targetTooColdToExtrude(5))
                MENU_ITEM_P(submenu, msg5, menu_temp_e5_filament_change);
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
          PGM_P msg0 = PSTR(MSG_FILAMENTLOAD);
          if (thermalManager.targetTooColdToExtrude(active_extruder))
            MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_load);
          else
            MENU_ITEM_P(gcode, msg0, PSTR("M701"));
        #else
          PGM_P msg0 = PSTR(MSG_FILAMENTLOAD " " MSG_E1);
          PGM_P msg1 = PSTR(MSG_FILAMENTLOAD " " MSG_E2);
          if (thermalManager.targetTooColdToExtrude(0))
            MENU_ITEM_P(submenu, msg0, menu_temp_e0_filament_load);
          else
            MENU_ITEM_P(gcode, msg0, PSTR("M701 T0"));
          if (thermalManager.targetTooColdToExtrude(1))
            MENU_ITEM_P(submenu, msg1, menu_temp_e1_filament_load);
          else
            MENU_ITEM_P(gcode, msg1, PSTR("M701 T1"));
          #if E_STEPPERS > 2
            PGM_P msg2 = PSTR(MSG_FILAMENTLOAD " " MSG_E3);
            if (thermalManager.targetTooColdToExtrude(2))
              MENU_ITEM_P(submenu, msg2, menu_temp_e2_filament_load);
            else
              MENU_ITEM_P(gcode, msg2, PSTR("M701 T2"));
            #if E_STEPPERS > 3
              PGM_P msg3 = PSTR(MSG_FILAMENTLOAD " " MSG_E4);
              if (thermalManager.targetTooColdToExtrude(3))
                MENU_ITEM_P(submenu, msg3, menu_temp_e3_filament_load);
              else
                MENU_ITEM_P(gcode, msg3, PSTR("M701 T3"));
              #if E_STEPPERS > 4
                PGM_P msg4 = PSTR(MSG_FILAMENTLOAD " " MSG_E5);
                if (thermalManager.targetTooColdToExtrude(4))
                  MENU_ITEM_P(submenu, msg4, menu_temp_e4_filament_load);
                else
                  MENU_ITEM_P(gcode, msg4, PSTR("M701 T4"));
                #if E_STEPPERS > 5
                  PGM_P msg5 = PSTR(MSG_FILAMENTLOAD " " MSG_E6);
                  if (thermalManager.targetTooColdToExtrude(5))
                    MENU_ITEM_P(submenu, msg5, menu_temp_e5_filament_load);
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
            MENU_ITEM(gcode, MSG_FILAMENTUNLOAD, PSTR("M702"));
          else
            MENU_ITEM(submenu, MSG_FILAMENTUNLOAD, menu_temp_e0_filament_unload);
        #else
          #if ENABLED(FILAMENT_UNLOAD_ALL_EXTRUDERS)
            if (thermalManager.targetHotEnoughToExtrude(0)
              #if E_STEPPERS > 1
                && thermalManager.targetHotEnoughToExtrude(1)
                #if E_STEPPERS > 2
                  && thermalManager.targetHotEnoughToExtrude(2)
                  #if E_STEPPERS > 3
                    && thermalManager.targetHotEnoughToExtrude(3)
                    #if E_STEPPERS > 4
                      && thermalManager.targetHotEnoughToExtrude(4)
                      #if E_STEPPERS > 5
                        && thermalManager.targetHotEnoughToExtrude(5)
                      #endif // E_STEPPERS > 5
                    #endif // E_STEPPERS > 4
                  #endif // E_STEPPERS > 3
                #endif // E_STEPPERS > 2
              #endif // E_STEPPERS > 1
            )
              MENU_ITEM(gcode, MSG_FILAMENTUNLOAD_ALL, PSTR("M702"));
          else
            MENU_ITEM(submenu, MSG_FILAMENTUNLOAD_ALL, menu_unload_filament_all_temp);
          #endif
          if (thermalManager.targetHotEnoughToExtrude(0))
            MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E1, PSTR("M702 T0"));
          else
            MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E1, menu_temp_e0_filament_unload);
          if (thermalManager.targetHotEnoughToExtrude(1))
            MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E2, PSTR("M702 T1"));
          else
            MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E2, menu_temp_e1_filament_unload);
          #if E_STEPPERS > 2
            if (thermalManager.targetHotEnoughToExtrude(2))
              MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E3, PSTR("M702 T2"));
            else
              MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E3, menu_temp_e2_filament_unload);
            #if E_STEPPERS > 3
              if (thermalManager.targetHotEnoughToExtrude(3))
                MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E4, PSTR("M702 T3"));
              else
                MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E4, menu_temp_e3_filament_unload);
              #if E_STEPPERS > 4
                if (thermalManager.targetHotEnoughToExtrude(4))
                  MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E5, PSTR("M702 T4"));
                else
                  MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E5, menu_temp_e4_filament_unload);
                #if E_STEPPERS > 5
                  if (thermalManager.targetHotEnoughToExtrude(5))
                    MENU_ITEM(gcode, MSG_FILAMENTUNLOAD " " MSG_E6, PSTR("M702 T5"));
                  else
                    MENU_ITEM(submenu, MSG_FILAMENTUNLOAD " " MSG_E6, menu_temp_e5_filament_unload);
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
    case PAUSE_MODE_LOAD_FILAMENT:
      return PSTR(MSG_FILAMENT_CHANGE_HEADER_LOAD);
    case PAUSE_MODE_UNLOAD_FILAMENT:
      return PSTR(MSG_FILAMENT_CHANGE_HEADER_UNLOAD);
    default: break;
  }
  return PSTR(MSG_FILAMENT_CHANGE_HEADER_PAUSE);
}

// Portions from STATIC_ITEM...
#define HOTEND_STATUS_ITEM() do { \
  if (_menuLineNr == _thisItemNr) { \
    if (ui.should_draw()) { \
      draw_menu_item_static(_lcdLineNr, PSTR(MSG_FILAMENT_CHANGE_NOZZLE), false, true); \
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

void lcd_pause_resume_print() {
  pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
}

void lcd_pause_extrude_more() {
  pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
}

void menu_pause_option() {
  START_MENU();
  #if LCD_HEIGHT > 2
    STATIC_ITEM(MSG_FILAMENT_CHANGE_OPTION_HEADER, true, false);
  #endif
  MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_PURGE, lcd_pause_extrude_more);
  #if HAS_FILAMENT_SENSOR
    if (runout.filament_ran_out)
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_RUNOUT_SENSOR, &runout.enabled, runout.reset);
    else
  #endif
      MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_RESUME, lcd_pause_resume_print);
  END_MENU();
}

//
// ADVANCED_PAUSE_FEATURE message screens
//

void _lcd_pause_message(PGM_P const msg1, PGM_P const msg2=NULL, PGM_P const msg3=NULL) {
  START_SCREEN();
  STATIC_ITEM_P(pause_header(), true, true);
  STATIC_ITEM_P(msg1);
  if (msg2) STATIC_ITEM_P(msg2);
  if (msg3) STATIC_ITEM_P(msg3);
  if ((!!msg2) + (!!msg3) + 2 < LCD_HEIGHT - 1) STATIC_ITEM(" ");
  HOTEND_STATUS_ITEM();
  END_SCREEN();
}

void lcd_pause_pausing_message() {
  _lcd_pause_message(PSTR(MSG_PAUSE_PRINT_INIT_1)
    #ifdef MSG_PAUSE_PRINT_INIT_2
      , PSTR(MSG_PAUSE_PRINT_INIT_2)
      #ifdef MSG_PAUSE_PRINT_INIT_3
        , PSTR(MSG_PAUSE_PRINT_INIT_3)
      #endif
    #endif
  );
}

void lcd_pause_changing_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_INIT_1)
    #ifdef MSG_FILAMENT_CHANGE_INIT_2
      , PSTR(MSG_FILAMENT_CHANGE_INIT_2)
      #ifdef MSG_FILAMENT_CHANGE_INIT_3
        , PSTR(MSG_FILAMENT_CHANGE_INIT_3)
      #endif
    #endif
  );
}

void lcd_pause_unload_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_UNLOAD_1)
    #ifdef MSG_FILAMENT_CHANGE_UNLOAD_2
      , PSTR(MSG_FILAMENT_CHANGE_UNLOAD_2)
      #ifdef MSG_FILAMENT_CHANGE_UNLOAD_3
        , PSTR(MSG_FILAMENT_CHANGE_UNLOAD_3)
      #endif
    #endif
  );
}

void lcd_pause_heating_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_HEATING_1)
    #ifdef MSG_FILAMENT_CHANGE_HEATING_2
      , PSTR(MSG_FILAMENT_CHANGE_HEATING_2)
      #ifdef MSG_FILAMENT_CHANGE_HEATING_3
        , PSTR(MSG_FILAMENT_CHANGE_HEATING_3)
      #endif
    #endif
  );
}

void lcd_pause_heat_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_HEAT_1)
    #ifdef MSG_FILAMENT_CHANGE_HEAT_2
      , PSTR(MSG_FILAMENT_CHANGE_HEAT_2)
      #ifdef MSG_FILAMENT_CHANGE_HEAT_3
        , PSTR(MSG_FILAMENT_CHANGE_HEAT_3)
      #endif
    #endif
  );
}

void lcd_pause_insert_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_INSERT_1)
    #ifdef MSG_FILAMENT_CHANGE_INSERT_2
      , PSTR(MSG_FILAMENT_CHANGE_INSERT_2)
      #ifdef MSG_FILAMENT_CHANGE_INSERT_3
        , PSTR(MSG_FILAMENT_CHANGE_INSERT_3)
      #endif
    #endif
  );
}

void lcd_pause_load_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_LOAD_1)
    #ifdef MSG_FILAMENT_CHANGE_LOAD_2
      , PSTR(MSG_FILAMENT_CHANGE_LOAD_2)
      #ifdef MSG_FILAMENT_CHANGE_LOAD_3
        , PSTR(MSG_FILAMENT_CHANGE_LOAD_3)
      #endif
    #endif
  );
}

void lcd_pause_waiting_message() {
  _lcd_pause_message(PSTR(MSG_ADVANCED_PAUSE_WAITING_1)
    #ifdef MSG_ADVANCED_PAUSE_WAITING_2
      , PSTR(MSG_ADVANCED_PAUSE_WAITING_2)
      #ifdef MSG_ADVANCED_PAUSE_WAITING_3
        , PSTR(MSG_ADVANCED_PAUSE_WAITING_3)
      #endif
    #endif
  );
}

void lcd_pause_resume_message() {
  _lcd_pause_message(PSTR(MSG_FILAMENT_CHANGE_RESUME_1)
    #ifdef MSG_FILAMENT_CHANGE_RESUME_2
      , PSTR(MSG_FILAMENT_CHANGE_RESUME_2)
      #ifdef MSG_FILAMENT_CHANGE_RESUME_3
        , PSTR(MSG_FILAMENT_CHANGE_RESUME_3)
      #endif
    #endif
  );
}

void lcd_pause_purge_message() {
  _lcd_pause_message(
    #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
      PSTR(MSG_FILAMENT_CHANGE_CONT_PURGE_1)
      #ifdef MSG_FILAMENT_CHANGE_CONT_PURGE_2
        , PSTR(MSG_FILAMENT_CHANGE_CONT_PURGE_2)
        #ifdef MSG_FILAMENT_CHANGE_CONT_PURGE_3
          , PSTR(MSG_FILAMENT_CHANGE_CONT_PURGE_3)
        #endif
      #endif
    #else
      PSTR(MSG_FILAMENT_CHANGE_PURGE_1)
      #ifdef MSG_FILAMENT_CHANGE_PURGE_2
        , PSTR(MSG_FILAMENT_CHANGE_PURGE_2)
        #ifdef MSG_FILAMENT_CHANGE_PURGE_3
          , PSTR(MSG_FILAMENT_CHANGE_PURGE_3)
        #endif
      #endif
    #endif
  );
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
  return NULL;
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
    ui.defer_status_screen(true);
    ui.goto_screen(next_screen);
  }
  else
    ui.return_to_status();
}

#endif // HAS_LCD_MENU && ADVANCED_PAUSE_FEATURE
