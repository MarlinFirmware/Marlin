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

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../ultralcd.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../module/printcounter.h"
#include "../../gcode/queue.h"
#include "../../sd/cardreader.h"
#if HAS_BUZZER
  #include "../../libs/buzzer.h"
#endif

#if ENABLED(EEPROM_SETTINGS)
  #include "../../module/configuration_store.h"
#endif

#if WATCH_HOTENDS || WATCH_BED
  #include "../../module/temperature.h"
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../module/probe.h"
#endif

#if EITHER(ENABLE_LEVELING_FADE_HEIGHT, AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

////////////////////////////////////////////
///////////// Global Variables /////////////
////////////////////////////////////////////

// Menu Navigation
int8_t encoderTopLine, encoderLine, screen_items;

typedef struct {
  screenFunc_t menu_function;
  uint32_t encoder_position;
  int8_t top_line, items;
} menuPosition;
menuPosition screen_history[6];
uint8_t screen_history_depth = 0;
bool screen_changed;

// Value Editing
PGM_P MenuItemBase::editLabel;
void* MenuItemBase::editValue;
int32_t MenuItemBase::minEditValue, MenuItemBase::maxEditValue;
screenFunc_t MenuItemBase::callbackFunc;
bool MenuItemBase::liveEdit;

// Prevent recursion into screen handlers
bool no_reentry = false;

////////////////////////////////////////////
//////// Menu Navigation & History /////////
////////////////////////////////////////////

void MarlinUI::return_to_status() { goto_screen(status_screen); }

void MarlinUI::save_previous_screen() {
  if (screen_history_depth < COUNT(screen_history))
    screen_history[screen_history_depth++] = { currentScreen, encoderPosition, encoderTopLine, screen_items };
}

void MarlinUI::goto_previous_screen() {
  if (screen_history_depth > 0) {
    menuPosition &sh = screen_history[--screen_history_depth];
    goto_screen(sh.menu_function, sh.encoder_position, sh.top_line, sh.items);
  }
  else
    return_to_status();
}

////////////////////////////////////////////
/////////// Common Menu Actions ////////////
////////////////////////////////////////////

void MenuItem_gcode::action(PGM_P const pgcode) { queue.inject_P(pgcode); }

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
  if (int16_t(ui.encoderPosition) < 0) ui.encoderPosition = 0;
  if (int16_t(ui.encoderPosition) > maxEditValue) ui.encoderPosition = maxEditValue;
  if (ui.should_draw())
    draw_edit_screen(editLabel, strfunc(ui.encoderPosition + minEditValue));
  if (ui.lcd_clicked || (liveEdit && ui.should_draw())) {
    if (editValue != nullptr) loadfunc(editValue, ui.encoderPosition + minEditValue);
    if (callbackFunc && (liveEdit || ui.lcd_clicked)) (*callbackFunc)();
    if (ui.use_click()) ui.goto_previous_screen();
  }
}

void MenuItemBase::init(PGM_P const el, void * const ev, const int32_t minv, const int32_t maxv, const uint16_t ep, const screenFunc_t cs, const screenFunc_t cb, const bool le) {
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

DEFINE_MENU_EDIT_ITEM(percent);     // 100%       right-justified
DEFINE_MENU_EDIT_ITEM(int3);        // 123, -12   right-justified
DEFINE_MENU_EDIT_ITEM(int4);        // 1234, -123 right-justified
DEFINE_MENU_EDIT_ITEM(int8);        // 123, -12   right-justified
DEFINE_MENU_EDIT_ITEM(uint8);       // 123        right-justified
DEFINE_MENU_EDIT_ITEM(uint16_3);    // 123        right-justified
DEFINE_MENU_EDIT_ITEM(uint16_4);    // 1234       right-justified
DEFINE_MENU_EDIT_ITEM(uint16_5);    // 12345      right-justified
DEFINE_MENU_EDIT_ITEM(float3);      // 123        right-justified
DEFINE_MENU_EDIT_ITEM(float52);     // _2.34, 12.34, -2.34 or 123.45, -23.45
DEFINE_MENU_EDIT_ITEM(float43);     // 1.234
DEFINE_MENU_EDIT_ITEM(float5);      // 12345      right-justified
DEFINE_MENU_EDIT_ITEM(float5_25);   // 12345      right-justified (25 increment)
DEFINE_MENU_EDIT_ITEM(float51);     // 1234.5     right-justified
DEFINE_MENU_EDIT_ITEM(float51sign); // +1234.5
DEFINE_MENU_EDIT_ITEM(float52sign); // +123.45
DEFINE_MENU_EDIT_ITEM(long5);       // 12345      right-justified
DEFINE_MENU_EDIT_ITEM(long5_25);    // 12345      right-justified (25 increment)

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
void MarlinUI::goto_screen(screenFunc_t screen, const uint16_t encoder/*=0*/, const uint8_t top/*=0*/, const uint8_t items/*=0*/) {
  if (currentScreen != screen) {

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      // Shadow for editing the fade height
      lcd_z_fade_height = planner.z_fade_height;
    #endif

    #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
      progress_reset();
    #endif

    #if BOTH(DOUBLECLICK_FOR_Z_BABYSTEPPING, BABYSTEPPING)
      static millis_t doubleclick_expire_ms = 0;
      // Going to menu_main from status screen? Remember first click time.
      // Going back to status screen within a very short time? Go to Z babystepping.
      if (screen == menu_main) {
        if (on_status_screen())
          doubleclick_expire_ms = millis() + DOUBLECLICK_MAX_INTERVAL;
      }
      else if (screen == status_screen && currentScreen == menu_main && PENDING(millis(), doubleclick_expire_ms)) {

        #if ENABLED(BABYSTEP_WITHOUT_HOMING)
          constexpr bool can_babystep = true;
        #else
          const bool can_babystep = all_axes_known();
        #endif
        #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
          constexpr bool should_babystep = true;
        #else
          const bool should_babystep = printer_busy();
        #endif

        if (should_babystep && can_babystep) {
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
    encoderTopLine = top;
    screen_items = items;
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
      if (true
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          && !ubl.lcd_map_control
        #endif
      ) set_custom_characters(screen == status_screen ? CHARSET_INFO : CHARSET_MENU);
    #endif

    refresh(LCDVIEW_CALL_REDRAW_NEXT);
    screen_changed = true;
    #if HAS_GRAPHICAL_LCD
      drawing_screen = false;
    #endif

    #if HAS_LCD_MENU
      encoder_direction_normal();
    #endif

    set_selection(false);
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
void MarlinUI::synchronize(PGM_P const msg/*=nullptr*/) {
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
void scroll_screen(const uint8_t limit, const bool is_menu) {
  ui.encoder_direction_menus();
  ENCODER_RATE_MULTIPLY(false);
  if (ui.encoderPosition > 0x8000) ui.encoderPosition = 0;
  if (ui.first_page) {
    encoderLine = ui.encoderPosition / (ENCODER_STEPS_PER_MENU_ITEM);
    screen_changed = false;
  }
  if (screen_items > 0 && encoderLine >= screen_items - limit) {
    encoderLine = _MAX(0, screen_items - limit);
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

#if HAS_BUZZER
  void MarlinUI::completion_feedback(const bool good/*=true*/) {
    if (good) {
      BUZZ(100, 659);
      BUZZ(100, 698);
    }
    else BUZZ(20, 440);
  }
#endif

#if HAS_LINE_TO_Z

  void line_to_z(const float &z) {
    current_position[Z_AXIS] = z;
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[Z_AXIS]), active_extruder);
  }

#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)

  #include "../../feature/babystep.h"

  void lcd_babystep_zoffset() {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    ui.defer_status_screen();
    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
      const bool do_probe = (active_extruder == 0);
    #else
      constexpr bool do_probe = true;
    #endif
    if (ui.encoderPosition) {
      const int16_t babystep_increment = int16_t(ui.encoderPosition) * (BABYSTEP_MULTIPLICATOR);
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

        babystep.add_steps(Z_AXIS, babystep_increment);

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
          draw_edit_screen(PSTR(MSG_Z_OFFSET), ftostr43sign(hotend_offset[Z_AXIS][active_extruder]));
        else
      #endif
          draw_edit_screen(PSTR(MSG_ZPROBE_ZOFFSET), ftostr43sign(zprobe_zoffset));

      #if ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY)
        if (do_probe) _lcd_zoffset_overlay_gfx(zprobe_zoffset);
      #endif
    }
  }

#endif // BABYSTEP_ZPROBE_OFFSET

#if ANY(AUTO_BED_LEVELING_UBL, PID_AUTOTUNE_MENU, ADVANCED_PAUSE_FEATURE)

  void lcd_enqueue_one_now(const char * const cmd) {
    no_reentry = true;
    queue.enqueue_one_now(cmd);
    no_reentry = false;
  }

  void lcd_enqueue_one_now_P(PGM_P const cmd) {
    no_reentry = true;
    queue.enqueue_now_P(cmd);
    no_reentry = false;
  }

#endif

#if ENABLED(EEPROM_SETTINGS)
  void lcd_store_settings() {
    const bool saved = settings.save();
    #if HAS_BUZZER
      ui.completion_feedback(saved);
    #endif
  }
  void lcd_load_settings() {
    const bool loaded = settings.load();
    #if HAS_BUZZER
      ui.completion_feedback(loaded);
    #endif
  }
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

//
// Selection screen presents a prompt and two options
//
bool MarlinUI::selection; // = false
bool MarlinUI::update_selection() {
  encoder_direction_select();
  if (encoderPosition) {
    selection = int16_t(encoderPosition) > 0;
    encoderPosition = 0;
  }
  return selection;
}
void do_select_screen(PGM_P const yes, PGM_P const no, selectFunc_t yesFunc, selectFunc_t noFunc, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
  const bool ui_selection = ui.update_selection(), got_click = ui.use_click();
  if (got_click || ui.should_draw()) {
    draw_select_screen(yes, no, ui_selection, pref, string, suff);
    if (got_click) { ui_selection ? yesFunc() : noFunc(); }
  }
}

#endif // HAS_LCD_MENU
