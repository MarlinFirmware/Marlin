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
#pragma once

#include "../marlinui.h"
#include "../../libs/numtostr.h"
#include "../../inc/MarlinConfig.h"

#include "limits.h"

extern int8_t encoderLine, encoderTopLine, screen_items;

void scroll_screen(const uint8_t limit, const bool is_menu);
bool printer_busy();

typedef void (*selectFunc_t)();

#define SS_LEFT    0x00
#define SS_CENTER  0x01
#define SS_INVERT  0x02
#define SS_DEFAULT SS_CENTER

#if EITHER(HAS_MARLINUI_U8GLIB, IS_DWIN_MARLINUI) && EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)
  void _lcd_zoffset_overlay_gfx(const_float_t zvalue);
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET) && Z_PROBE_OFFSET_RANGE_MIN >= -9 && Z_PROBE_OFFSET_RANGE_MAX <= 9
  #define BABYSTEP_TO_STR(N) ftostr43sign(N)
#elif ENABLED(BABYSTEPPING)
  #define BABYSTEP_TO_STR(N) ftostr53sign(N)
#endif

////////////////////////////////////////////
///////////// Base Menu Items //////////////
////////////////////////////////////////////

class MenuItemBase {
  public:
    // Index to interject in the item label and/or for use by its action.
    static int8_t itemIndex;

    // An optional pointer for use in display or by the action
    static PGM_P itemString;

    // Store the index of the item ahead of use by indexed items
    FORCE_INLINE static void init(const int8_t ind=0, PGM_P const pstr=nullptr) { itemIndex = ind; itemString = pstr; }

    // Draw an item either selected (pre_char) or not (space) with post_char
    static void _draw(const bool sel, const uint8_t row, PGM_P const pstr, const char pre_char, const char post_char);

    // Draw an item either selected ('>') or not (space) with post_char
    FORCE_INLINE static void _draw(const bool sel, const uint8_t row, PGM_P const pstr, const char post_char) {
      _draw(sel, row, pstr, '>', post_char);
    }
};

// STATIC_ITEM(LABEL,...)
class MenuItem_static : public MenuItemBase {
  public:
    static void draw(const uint8_t row, PGM_P const pstr, const uint8_t style=SS_DEFAULT, const char * const vstr=nullptr);
};

// BACK_ITEM(LABEL)
class MenuItem_back : public MenuItemBase {
  public:
    FORCE_INLINE static void draw(const bool sel, const uint8_t row, PGM_P const pstr) {
      _draw(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0]);
    }
    // Back Item action goes back one step in history
    FORCE_INLINE static void action(PGM_P const=nullptr) { ui.go_back(); }
};

// CONFIRM_ITEM(LABEL,Y,N,FY,FN,...),
// YESNO_ITEM(LABEL,FY,FN,...)
class MenuItem_confirm : public MenuItemBase {
  public:
    FORCE_INLINE static void draw(const bool sel, const uint8_t row, PGM_P const pstr, ...) {
      _draw(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0]);
    }
    // Implemented for HD44780 and DOGM
    // Draw the prompt, buttons, and state
    static void draw_select_screen(
      PGM_P const yes,            // Right option label
      PGM_P const no,             // Left option label
      const bool yesno,           // Is "yes" selected?
      PGM_P const pref,           // Prompt prefix
      const char * const string,  // Prompt runtime string
      PGM_P const suff            // Prompt suffix
    );
    static void select_screen(
      PGM_P const yes, PGM_P const no,
      selectFunc_t yesFunc, selectFunc_t noFunc,
      PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr
    );
    static inline void select_screen(
      PGM_P const yes, PGM_P const no,
      selectFunc_t yesFunc, selectFunc_t noFunc,
      PGM_P const pref, FSTR_P const string, PGM_P const suff=nullptr
    ) {
      char str[strlen_P((PGM_P)string) + 1];
      strcpy_P(str, (PGM_P)string);
      select_screen(yes, no, yesFunc, noFunc, pref, str, suff);
    }
    // Shortcut for prompt with "NO"/ "YES" labels
    FORCE_INLINE static void confirm_screen(selectFunc_t yesFunc, selectFunc_t noFunc, PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr) {
      select_screen(GET_TEXT(MSG_YES), GET_TEXT(MSG_NO), yesFunc, noFunc, pref, string, suff);
    }
};

////////////////////////////////////////////
///////////// Edit Menu Items //////////////
////////////////////////////////////////////

// The Menu Edit shadow value
typedef union {
  bool      state;
  float     decimal;
  int8_t    int8;
  int16_t   int16;
  int32_t   int32;
  uint8_t   uint8;
  uint16_t  uint16;
  uint32_t  uint32;
  celsius_t celsius;
} chimera_t;
extern chimera_t editable;

// Base class for Menu Edit Items
class MenuEditItemBase : public MenuItemBase {
  private:
    // These values are statically constructed by init() via action()
    // The action() method acts like the instantiator. The entire lifespan
    // of a menu item is within its declaration, so all these values decompose
    // into behavior and unused items get optimized out.
    static PGM_P editLabel;
    static void *editValue;
    static int32_t minEditValue, maxEditValue;  // Encoder value range
    static screenFunc_t callbackFunc;
    static bool liveEdit;
  protected:
    typedef const char* (*strfunc_t)(const int32_t);
    typedef void (*loadfunc_t)(void *, const int32_t);
    static void goto_edit_screen(
      PGM_P const el,         // Edit label
      void * const ev,        // Edit value pointer
      const int32_t minv,     // Encoder minimum
      const int32_t maxv,     // Encoder maximum
      const uint16_t ep,      // Initial encoder value
      const screenFunc_t cs,  // MenuItem_type::draw_edit_screen => MenuEditItemBase::edit()
      const screenFunc_t cb,  // Callback after edit
      const bool le           // Flag to call cb() during editing
    );
    static void edit_screen(strfunc_t, loadfunc_t); // Edit value handler
  public:
    // Implemented for HD44780 and DOGM
    // Draw the current item at specified row with edit data
    static void draw(const bool sel, const uint8_t row, PGM_P const pstr, const char * const inStr, const bool pgm=false);

    // Implemented for HD44780 and DOGM
    // This low-level method is good to draw from anywhere
    static void draw_edit_screen(PGM_P const pstr, const char * const value);

    // This method is for the current menu item
    static inline void draw_edit_screen(const char * const value) { draw_edit_screen(editLabel, value); }
};

#if ENABLED(SDSUPPORT)
  class CardReader;
  class MenuItem_sdbase {
    public:
      // Implemented for HD44780 and DOGM
      static void draw(const bool sel, const uint8_t row, PGM_P const pstr, CardReader &theCard, const bool isDir);
  };
#endif

////////////////////////////////////////////
/////////////// Menu Screens ///////////////
////////////////////////////////////////////

void menu_main();
void menu_move();

#if ENABLED(SDSUPPORT)
  void menu_media();
#endif

////////////////////////////////////////////
//////// Menu Item Helper Functions ////////
////////////////////////////////////////////

void lcd_move_z();
void _lcd_draw_homing();

#define HAS_LINE_TO_Z ANY(DELTA, PROBE_MANUALLY, MESH_BED_LEVELING, LEVEL_BED_CORNERS)

#if HAS_LINE_TO_Z
  void line_to_z(const_float_t z);
#endif

#if ENABLED(PROBE_OFFSET_WIZARD)
  void goto_probe_offset_wizard();
#endif

#if ENABLED(LCD_BED_LEVELING) || (HAS_LEVELING && DISABLED(SLIM_LCD_MENUS))
  void _lcd_toggle_bed_leveling();
#endif

#if ENABLED(BABYSTEPPING)
  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    void lcd_babystep_zoffset();
  #else
    void lcd_babystep_z();
  #endif

  #if ENABLED(BABYSTEP_MILLIMETER_UNITS)
    #define BABYSTEP_SIZE_X int32_t((BABYSTEP_MULTIPLICATOR_XY) * planner.settings.axis_steps_per_mm[X_AXIS])
    #define BABYSTEP_SIZE_Y int32_t((BABYSTEP_MULTIPLICATOR_XY) * planner.settings.axis_steps_per_mm[Y_AXIS])
    #define BABYSTEP_SIZE_Z int32_t((BABYSTEP_MULTIPLICATOR_Z)  * planner.settings.axis_steps_per_mm[Z_AXIS])
  #else
    #define BABYSTEP_SIZE_X BABYSTEP_MULTIPLICATOR_XY
    #define BABYSTEP_SIZE_Y BABYSTEP_MULTIPLICATOR_XY
    #define BABYSTEP_SIZE_Z BABYSTEP_MULTIPLICATOR_Z
  #endif

#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  void touch_screen_calibration();
#endif

extern uint8_t screen_history_depth;
inline void clear_menu_history() { screen_history_depth = 0; }

#define STICKY_SCREEN(S) []{ ui.defer_status_screen(); ui.goto_screen(S); }
