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

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../ultralcd.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../module/printcounter.h"
#include "../../gcode/queue.h"
#include "../../sd/cardreader.h"
#include "../../libs/buzzer.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../module/configuration_store.h"
#endif

#if WATCH_HOTENDS || WATCH_THE_BED || ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../module/temperature.h"
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../module/probe.h"
#endif

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT) || ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

////////////////////////////////////////////
///////////// Global Variables /////////////
////////////////////////////////////////////

// Menu Navigation
int8_t encoderTopLine;
typedef struct {
  screenFunc_t menu_function;
  uint32_t encoder_position;
} menuPosition;
menuPosition screen_history[6];
uint8_t screen_history_depth = 0;
bool screen_changed;

// Value Editing
PGM_P editLabel;
void *editValue;
int32_t minEditValue, maxEditValue;
screenFunc_t callbackFunc;
bool liveEdit;

// Prevent recursion into screen handlers
bool no_reentry = false;

////////////////////////////////////////////
//////// Menu Navigation & History /////////
////////////////////////////////////////////

void MarlinUI::return_to_status() { goto_screen(status_screen); }

void MarlinUI::save_previous_screen() {
  if (screen_history_depth < COUNT(screen_history)) {
    screen_history[screen_history_depth].menu_function = currentScreen;
    screen_history[screen_history_depth].encoder_position = encoderPosition;
    ++screen_history_depth;
  }
}

void MarlinUI::goto_previous_screen() {
  if (screen_history_depth > 0) {
    --screen_history_depth;
    goto_screen(
      screen_history[screen_history_depth].menu_function,
      screen_history[screen_history_depth].encoder_position
    );
  }
  else
    return_to_status();
}

////////////////////////////////////////////
/////////// Common Menu Actions ////////////
////////////////////////////////////////////

void MenuItem_gcode::action(PGM_P pgcode) { enqueue_and_echo_commands_P(pgcode); }

////////////////////////////////////////////
/////////// Menu Editing Actions ///////////
////////////////////////////////////////////

/**
 * Functions for editing single values
 *
 * The "DEFINE_MENU_EDIT_ITEM" macro generates the functions needed to edit a numerical value.
 *
 * The prerequisite is that in the header the type was already declared:
 *
 *   DECLARE_MENU_EDIT_TYPE(int16_t, int3, i16tostr3, 1)
 *
 * For example, DEFINE_MENU_EDIT_ITEM(int3) expands into these functions:
 *
 *   bool MenuItem_int3::_edit();
 *   void MenuItem_int3::edit(); // edit int16_t (interactively)
 *   void MenuItem_int3::action_edit(PGM_P const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue, const screenFunc_t callback = null, const bool live = false);
 *
 * You can then use one of the menu macros to present the edit interface:
 *   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
 *
 * This expands into a more primitive menu item:
 *   MENU_ITEM_VARIANT(int3, _edit, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
 *
 * ...which calls:
 *       MenuItem_int3::action_edit(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
 */
void MenuItemBase::edit(strfunc_t strfunc, loadfunc_t loadfunc) {
  ui.encoder_direction_normal();
  if ((int32_t)ui.encoderPosition < 0) ui.encoderPosition = 0;
  if ((int32_t)ui.encoderPosition > maxEditValue) ui.encoderPosition = maxEditValue;
  if (ui.should_draw())
    draw_edit_screen(editLabel, strfunc(ui.encoderPosition + minEditValue));
  if (ui.lcd_clicked || (liveEdit && ui.should_draw())) {
    if (editValue != NULL) loadfunc(editValue, ui.encoderPosition + minEditValue);
    if (callbackFunc && (liveEdit || ui.lcd_clicked)) (*callbackFunc)();
    if (ui.use_click()) ui.goto_previous_screen();
  }
}

void MenuItemBase::init(PGM_P const el, void * const ev, const int32_t minv, const int32_t maxv, const uint32_t ep, const screenFunc_t cs, const screenFunc_t cb, const bool le) {
  ui.save_previous_screen();
  ui.refresh();
  editLabel = el;
  editValue = ev;
  minEditValue = minv;
  maxEditValue = maxv;
  ui.encoderPosition = ep;
  ui.currentScreen = cs;
  callbackFunc = cb;
  liveEdit = le;
}

#define DEFINE_MENU_EDIT_ITEM(NAME) template class TMenuItem<MenuItemInfo_##NAME>

DEFINE_MENU_EDIT_ITEM(int3);        // 123, -12   right-justified
DEFINE_MENU_EDIT_ITEM(int4);        // 1234, -123 right-justified
DEFINE_MENU_EDIT_ITEM(int8);        // 123, -12   right-justified
DEFINE_MENU_EDIT_ITEM(uint8);       // 123        right-justified
DEFINE_MENU_EDIT_ITEM(uint16_3);    // 123, -12   right-justified
DEFINE_MENU_EDIT_ITEM(uint16_4);    // 1234, -123 right-justified
DEFINE_MENU_EDIT_ITEM(float3);      // 123        right-justified
DEFINE_MENU_EDIT_ITEM(float52);     // 123.45
DEFINE_MENU_EDIT_ITEM(float43);     // 1.234
DEFINE_MENU_EDIT_ITEM(float5);      // 12345      right-justified
DEFINE_MENU_EDIT_ITEM(float51);     // +1234.5
DEFINE_MENU_EDIT_ITEM(float52sign); // +123.45
DEFINE_MENU_EDIT_ITEM(float62);     // 1234.56    right-justified
DEFINE_MENU_EDIT_ITEM(long5);       // 12345      right-justified

void MenuItem_bool::action_edit(PGM_P pstr, bool *ptr, screenFunc_t callback) {
  UNUSED(pstr); *ptr ^= true; ui.refresh();
  if (callback) (*callback)();
}

////////////////////////////////////////////
///////////////// Menu Tree ////////////////
////////////////////////////////////////////

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float lcd_z_fade_height;
  void _lcd_set_z_fade_height() { set_z_fade_height(lcd_z_fade_height); }
#endif

bool printer_busy() {
  return planner.movesplanned() || IS_SD_PRINTING() || print_job_timer.isRunning();
}

/**
 * General function to go directly to a screen
 */
void MarlinUI::goto_screen(screenFunc_t screen, const uint32_t encoder/*=0*/) {
  if (currentScreen != screen) {

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      // Shadow for editing the fade height
      lcd_z_fade_height = planner.z_fade_height;
    #endif

    #if ENABLED(DOUBLECLICK_FOR_Z_BABYSTEPPING) && ENABLED(BABYSTEPPING)
      static millis_t doubleclick_expire_ms = 0;
      // Going to menu_main from status screen? Remember first click time.
      // Going back to status screen within a very short time? Go to Z babystepping.
      if (screen == menu_main) {
        if (on_status_screen())
          doubleclick_expire_ms = millis() + DOUBLECLICK_MAX_INTERVAL;
      }
      else if (screen == status_screen && currentScreen == menu_main && PENDING(millis(), doubleclick_expire_ms)) {
        if (printer_busy()) {
          screen =
            #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
              lcd_babystep_zoffset
            #else
              lcd_babystep_z
            #endif
          ;
        }
        #if ENABLED(MOVE_Z_WHEN_IDLE)
          else {
            move_menu_scale = MOVE_Z_IDLE_MULTIPLICATOR;
            screen = lcd_move_z;
          }
        #endif
      }
    #endif

    currentScreen = screen;
    encoderPosition = encoder;
    if (screen == status_screen) {
      defer_status_screen(false);
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.lcd_map_control = false;
      #endif
      screen_history_depth = 0;
    }

    clear_lcd();

    // Re-initialize custom characters that may be re-used
    #if HAS_CHARACTER_LCD
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!ubl.lcd_map_control)
      #endif
          set_custom_characters(screen == status_screen ? CHARSET_INFO : CHARSET_MENU);
    #endif

    refresh(LCDVIEW_CALL_REDRAW_NEXT);
    screen_changed = true;
    #if HAS_GRAPHICAL_LCD
      drawing_screen = false;
    #endif
  }
}

////////////////////////////////////////////
///////////// Manual Movement //////////////
////////////////////////////////////////////

//
// Display the synchronize screen until moves are
// finished, and don't return to the caller until
// done. ** This blocks the command queue! **
//
static PGM_P sync_message;

void MarlinUI::_synchronize() {
  if (should_draw()) draw_menu_item_static(LCD_HEIGHT >= 4 ? 1 : 0, sync_message);
  if (no_reentry) return;
  // Make this the current handler till all moves are done
  no_reentry = true;
  const screenFunc_t old_screen = currentScreen;
  goto_screen(_synchronize);
  planner.synchronize(); // idle() is called until moves complete
  no_reentry = false;
  goto_screen(old_screen);
}

// Display the synchronize screen with a custom message
// ** This blocks the command queue! **
void MarlinUI::synchronize(PGM_P const msg/*=NULL*/) {
  static const char moving[] PROGMEM = MSG_MOVING;
  sync_message = msg ? msg : moving;
  _synchronize();
}

/**
 * Scrolling for menus and other line-based screens
 *
 *   encoderLine is the position based on the encoder
 *   encoderTopLine is the top menu line to display
 *   _lcdLineNr is the index of the LCD line (e.g., 0-3)
 *   _menuLineNr is the menu item to draw and process
 *   _thisItemNr is the index of each MENU_ITEM or STATIC_ITEM
 *   screen_items is the total number of items in the menu (after one call)
 */
int8_t encoderLine, screen_items;
void scroll_screen(const uint8_t limit, const bool is_menu) {
  ui.encoder_direction_menus();
  ENCODER_RATE_MULTIPLY(false);
  if (ui.encoderPosition > 0x8000) ui.encoderPosition = 0;
  if (ui.first_page) {
    encoderLine = ui.encoderPosition / (ENCODER_STEPS_PER_MENU_ITEM);
    screen_changed = false;
  }
  if (screen_items > 0 && encoderLine >= screen_items - limit) {
    encoderLine = MAX(0, screen_items - limit);
    ui.encoderPosition = encoderLine * (ENCODER_STEPS_PER_MENU_ITEM);
  }
  if (is_menu) {
    NOMORE(encoderTopLine, encoderLine);
    if (encoderLine >= encoderTopLine + LCD_HEIGHT)
      encoderTopLine = encoderLine - LCD_HEIGHT + 1;
  }
  else
    encoderTopLine = encoderLine;
}

void MarlinUI::completion_feedback(const bool good/*=true*/) {
  if (good) {
    BUZZ(100, 659);
    BUZZ(100, 698);
  }
  else BUZZ(20, 440);
}

#if HAS_LINE_TO_Z

  void line_to_z(const float &z) {
    current_position[Z_AXIS] = z;
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[Z_AXIS]), active_extruder);
  }

#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)

  void lcd_babystep_zoffset() {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    ui.defer_status_screen(true);
    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
      const bool do_probe = (active_extruder == 0);
    #else
      constexpr bool do_probe = true;
    #endif
    ui.encoder_direction_normal();
    if (ui.encoderPosition) {
      const int16_t babystep_increment = (int32_t)ui.encoderPosition * (BABYSTEP_MULTIPLICATOR);
      ui.encoderPosition = 0;

      const float diff = planner.steps_to_mm[Z_AXIS] * babystep_increment,
                  new_probe_offset = zprobe_zoffset + diff,
                  new_offs =
                    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
                      do_probe ? new_probe_offset : hotend_offset[Z_AXIS][active_extruder] - diff
                    #else
                      new_probe_offset
                    #endif
                  ;
      if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {

        thermalManager.babystep_axis(Z_AXIS, babystep_increment);

        if (do_probe) zprobe_zoffset = new_offs;
        #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
          else hotend_offset[Z_AXIS][active_extruder] = new_offs;
        #endif

        ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
      }
    }
    if (ui.should_draw()) {
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        if (!do_probe)
          draw_edit_screen(PSTR(MSG_IDEX_Z_OFFSET), ftostr43sign(hotend_offset[Z_AXIS][active_extruder]));
        else
      #endif
          draw_edit_screen(PSTR(MSG_ZPROBE_ZOFFSET), ftostr43sign(zprobe_zoffset));

      #if ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY)
        if (do_probe) _lcd_zoffset_overlay_gfx(zprobe_zoffset);
      #endif
    }
  }

#endif // BABYSTEP_ZPROBE_OFFSET

/**
 * Watch temperature callbacks
 */
#if HAS_TEMP_HOTEND
  #if WATCH_HOTENDS
    #define _WATCH_FUNC(N) thermalManager.start_watching_heater(N)
  #else
    #define _WATCH_FUNC(N) NOOP
  #endif
  void watch_temp_callback_E0() { _WATCH_FUNC(0); }
  #if HOTENDS > 1
    void watch_temp_callback_E1() { _WATCH_FUNC(1); }
    #if HOTENDS > 2
      void watch_temp_callback_E2() { _WATCH_FUNC(2); }
      #if HOTENDS > 3
        void watch_temp_callback_E3() { _WATCH_FUNC(3); }
        #if HOTENDS > 4
          void watch_temp_callback_E4() { _WATCH_FUNC(4); }
          #if HOTENDS > 5
            void watch_temp_callback_E5() { _WATCH_FUNC(5); }
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1
#endif // HAS_TEMP_HOTEND

void watch_temp_callback_bed() {
  #if WATCH_THE_BED
    thermalManager.start_watching_bed();
  #endif
}

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(PID_AUTOTUNE_MENU) || ENABLED(ADVANCED_PAUSE_FEATURE)

  void lcd_enqueue_command(const char * const cmd) {
    no_reentry = true;
    enqueue_and_echo_command_now(cmd);
    no_reentry = false;
  }

  void lcd_enqueue_commands_P(PGM_P const cmd) {
    no_reentry = true;
    enqueue_and_echo_commands_now_P(cmd);
    no_reentry = false;
  }

#endif

#if ENABLED(EEPROM_SETTINGS)
  void lcd_store_settings()   { ui.completion_feedback(settings.save()); }
  void lcd_load_settings()    { ui.completion_feedback(settings.load()); }
#endif

void _lcd_draw_homing() {
  constexpr uint8_t line = (LCD_HEIGHT - 1) / 2;
  if (ui.should_draw()) draw_menu_item_static(line, PSTR(MSG_LEVEL_BED_HOMING));
  ui.refresh(LCDVIEW_CALL_NO_REDRAW);
}

#if ENABLED(LCD_BED_LEVELING) || (HAS_LEVELING && DISABLED(SLIM_LCD_MENUS))
  #include "../../feature/bedlevel/bedlevel.h"
  void _lcd_toggle_bed_leveling() { set_bed_leveling_enabled(!planner.leveling_active); }
#endif

#endif // HAS_LCD_MENU
