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
#pragma once

#include "../ultralcd.h"
#include "../../libs/numtostr.h"
#include "../../inc/MarlinConfig.h"

#include "limits.h"

extern int8_t encoderLine, encoderTopLine, screen_items;
extern bool screen_changed;

#if HOTENDS
  constexpr int16_t heater_maxtemp[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP);
#endif

void scroll_screen(const uint8_t limit, const bool is_menu);
bool printer_busy();

////////////////////////////////////////////
////////// Menu Item Numeric Types /////////
////////////////////////////////////////////

#define DECLARE_MENU_EDIT_TYPE(TYPE, NAME, STRFUNC, SCALE) \
  struct MenuEditItemInfo_##NAME { \
    typedef TYPE type_t; \
    static constexpr float scale = SCALE; \
    static inline char* strfunc(const float value) { return STRFUNC((TYPE) value); } \
  };

DECLARE_MENU_EDIT_TYPE(uint8_t,  percent,     ui8tostr4pct, 100.0/255);   // 100%       right-justified
DECLARE_MENU_EDIT_TYPE(int16_t,  int3,        i16tostr3,       1     );   // 123, -12   right-justified
DECLARE_MENU_EDIT_TYPE(int16_t,  int4,        i16tostr4sign,   1     );   // 1234, -123 right-justified
DECLARE_MENU_EDIT_TYPE(int8_t,   int8,        i8tostr3,        1     );   // 123, -12   right-justified
DECLARE_MENU_EDIT_TYPE(uint8_t,  uint8,       ui8tostr3,       1     );   // 123        right-justified
DECLARE_MENU_EDIT_TYPE(uint16_t, uint16_3,    ui16tostr3,      1     );   // 123        right-justified
DECLARE_MENU_EDIT_TYPE(uint16_t, uint16_4,    ui16tostr4,      0.1   );   // 1234       right-justified
DECLARE_MENU_EDIT_TYPE(uint16_t, uint16_5,    ui16tostr5,      0.01  );   // 12345      right-justified
DECLARE_MENU_EDIT_TYPE(float,    float3,      ftostr3,         1     );   // 123        right-justified
DECLARE_MENU_EDIT_TYPE(float,    float52,     ftostr42_52,   100     );   // _2.34, 12.34, -2.34 or 123.45, -23.45
DECLARE_MENU_EDIT_TYPE(float,    float43,     ftostr43sign, 1000     );   // 1.234
DECLARE_MENU_EDIT_TYPE(float,    float5,      ftostr5rj,       1     );   // 12345      right-justified
DECLARE_MENU_EDIT_TYPE(float,    float5_25,   ftostr5rj,       0.04f );   // 12345      right-justified (25 increment)
DECLARE_MENU_EDIT_TYPE(float,    float51,     ftostr51rj,     10     );   // 1234.5     right-justified
DECLARE_MENU_EDIT_TYPE(float,    float51sign, ftostr51sign,   10     );   // +1234.5
DECLARE_MENU_EDIT_TYPE(float,    float52sign, ftostr52sign,  100     );   // +123.45
DECLARE_MENU_EDIT_TYPE(uint32_t, long5,       ftostr5rj,       0.01f );   // 12345      right-justified
DECLARE_MENU_EDIT_TYPE(uint32_t, long5_25,    ftostr5rj,       0.04f );   // 12345      right-justified (25 increment)

////////////////////////////////////////////
///////// Menu Item Draw Functions /////////
////////////////////////////////////////////

typedef void (*selectFunc_t)();
void draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string, PGM_P const suff);
void do_select_screen(PGM_P const yes, PGM_P const no, selectFunc_t yesFunc, selectFunc_t noFunc, PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr);
inline void do_select_screen_yn(selectFunc_t yesFunc, selectFunc_t noFunc, PGM_P const pref, const char * const string=nullptr, PGM_P const suff=nullptr) {
  do_select_screen(GET_TEXT(MSG_YES), GET_TEXT(MSG_NO), yesFunc, noFunc, pref, string, suff);
}

#define SS_LEFT    0x00
#define SS_CENTER  0x01
#define SS_INVERT  0x02
#define SS_DEFAULT SS_CENTER

void draw_edit_screen(PGM_P const pstr, const char* const value=nullptr);
void draw_menu_item(const bool sel, const uint8_t row, PGM_P const pstr, const char pre_char, const char post_char);
void draw_menu_item_static(const uint8_t row, PGM_P const pstr, const uint8_t style=SS_DEFAULT, const char * const valstr=nullptr);
void _draw_menu_item_edit(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm);
FORCE_INLINE void draw_menu_item_back(const bool sel, const uint8_t row, PGM_P const pstr) { draw_menu_item(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0]); }
FORCE_INLINE void draw_menu_item_edit(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data) { _draw_menu_item_edit(sel, row, pstr, data, false); }
FORCE_INLINE void draw_menu_item_edit_P(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data) { _draw_menu_item_edit(sel, row, pstr, data, true); }
#define draw_menu_item_submenu(sel, row, pstr, data)  draw_menu_item(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define draw_menu_item_gcode(sel, row, pstr, gcode)   draw_menu_item(sel, row, pstr, '>', ' ')
#define draw_menu_item_function(sel, row, pstr, data) draw_menu_item(sel, row, pstr, '>', ' ')
#define DRAW_MENU_ITEM_SETTING_EDIT_GENERIC(VAL)      draw_menu_item_edit(sel, row, pstr, VAL)
#define DRAW_BOOL_SETTING(sel, row, pstr, data)       draw_menu_item_edit_P(sel, row, pstr, (*(data))?GET_TEXT(MSG_LCD_ON):GET_TEXT(MSG_LCD_OFF))

#if ENABLED(SDSUPPORT)
  class CardReader;
  void draw_sd_menu_item(const bool sel, const uint8_t row, PGM_P const pstr, CardReader &theCard, const bool isDir);
  FORCE_INLINE void draw_menu_item_sdfile(const bool sel, const uint8_t row, PGM_P const pstr, CardReader &theCard) { draw_sd_menu_item(sel, row, pstr, theCard, false); }
  FORCE_INLINE void draw_menu_item_sdfolder(const bool sel, const uint8_t row, PGM_P const pstr, CardReader &theCard) { draw_sd_menu_item(sel, row, pstr, theCard, true); }
#endif

#if HAS_GRAPHICAL_LCD && EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)
  void _lcd_zoffset_overlay_gfx(const float zvalue);
#endif

////////////////////////////////////////////
/////// Edit Setting Draw Functions ////////
////////////////////////////////////////////

#define _DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(TYPE, NAME, STRFUNC) \
  FORCE_INLINE void draw_menu_item_##NAME (const bool sel, const uint8_t row, PGM_P const pstr, TYPE * const data, ...) { \
    DRAW_MENU_ITEM_SETTING_EDIT_GENERIC(STRFUNC(*(data))); \
  } \
  FORCE_INLINE void draw_menu_item_accessor_##NAME (const bool sel, const uint8_t row, PGM_P const pstr, PGM_P const, TYPE (*pget)(), void (*)(TYPE), ...) { \
    DRAW_MENU_ITEM_SETTING_EDIT_GENERIC(STRFUNC(pget())); \
  } \
  typedef void NAME##_void
#define DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(NAME) _DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(MenuEditItemInfo_##NAME::type_t, NAME, MenuEditItemInfo_##NAME::strfunc)

DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(percent);          // 100%       right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(int3);             // 123, -12   right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(int4);             // 1234, -123 right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(int8);             // 123, -12   right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(uint8);            // 123        right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(uint16_3);         // 123        right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(uint16_4);         // 1234       right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(uint16_5);         // 12345      right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float3);           // 123        right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float52);          // _2.34, 12.34, -2.34 or 123.45, -23.45
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float43);          // 1.234
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float5);           // 12345      right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float5_25);        // 12345      right-justified (25 increment)
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float51);          // _1234.5    right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float51sign);      // +1234.5
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(float52sign);      // +123.45
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(long5);            // 12345      right-justified
DEFINE_DRAW_MENU_ITEM_SETTING_EDIT(long5_25);         // 12345      right-justified (25 increment)

#define draw_menu_item_bool(sel, row, pstr, data, ...)           DRAW_BOOL_SETTING(sel, row, pstr, data)
#define draw_menu_item_accessor_bool(sel, row, pstr, pget, pset) DRAW_BOOL_SETTING(sel, row, pstr, data)

////////////////////////////////////////////
/////////////// Menu Actions ///////////////
////////////////////////////////////////////

class MenuItem_back {
  public:
    static inline void action(PGM_P const=nullptr) {
      ui.goto_previous_screen(
        #if ENABLED(TURBO_BACK_MENU_ITEM)
          true
        #endif
      );
    }
};

class MenuItem_submenu {
  public:
    static inline void action(PGM_P const, const screenFunc_t func) { ui.save_previous_screen(); ui.goto_screen(func); }
};

class MenuItem_gcode {
  public:
    static void action(PGM_P const, const char * const pgcode);
};

class MenuItem_function {
  public:
    static inline void action(PGM_P const, const menuAction_t func) { (*func)(); };
};

////////////////////////////////////////////
/////////// Menu Editing Actions ///////////
////////////////////////////////////////////

//
// The Menu Edit shadow value
// Only one edit value is needed at a time
//

typedef union {
  bool     state;
  float    decimal;
  int8_t   int8;
  int16_t  int16;
  int32_t  int32;
  uint8_t  uint8;
  uint16_t uint16;
  uint32_t uint32;
} chimera_t;

extern chimera_t editable;

// Edit items use long integer encoder units
class MenuEditItemBase {
  private:
    static PGM_P editLabel;
    static void *editValue;
    static int32_t minEditValue, maxEditValue;  // Encoder value range
    static screenFunc_t callbackFunc;
    static bool liveEdit;
  protected:
    typedef char* (*strfunc_t)(const int32_t);
    typedef void (*loadfunc_t)(void *, const int32_t);
    static void init(PGM_P const el, void * const ev, const int32_t minv, const int32_t maxv, const uint16_t ep, const screenFunc_t cs, const screenFunc_t cb, const bool le);
    static void edit(strfunc_t, loadfunc_t);
};

template<typename NAME>
class TMenuEditItem : MenuEditItemBase {
  private:
    typedef typename NAME::type_t type_t;
    static inline float unscale(const float value)    { return value * (1.0f / NAME::scale);  }
    static inline float scale(const float value)      { return value * NAME::scale;           }
    static void load(void *ptr, const int32_t value)  { *((type_t*)ptr) = unscale(value);     }
    static char* to_string(const int32_t value)       { return NAME::strfunc(unscale(value)); }
  public:
    static void action(
      PGM_P const pstr,                     // Edit label
      type_t * const ptr,                   // Value pointer
      const type_t minValue,                // Value range
      const type_t maxValue,
      const screenFunc_t callback=nullptr,  // Value update callback
      const bool live=false                 // Callback during editing
    ) {
      // Make sure minv and maxv fit within int32_t
      const int32_t minv = _MAX(scale(minValue), INT32_MIN),
                    maxv = _MIN(scale(maxValue), INT32_MAX);
      init(pstr, ptr, minv, maxv - minv, scale(*ptr) - minv, edit, callback, live);
    }
    static void edit() { MenuEditItemBase::edit(to_string, load); }
};

#define DECLARE_MENU_EDIT_ITEM(NAME) typedef TMenuEditItem<MenuEditItemInfo_##NAME> MenuItem_##NAME;

DECLARE_MENU_EDIT_ITEM(percent);
DECLARE_MENU_EDIT_ITEM(int3);
DECLARE_MENU_EDIT_ITEM(int4);
DECLARE_MENU_EDIT_ITEM(int8);
DECLARE_MENU_EDIT_ITEM(uint8);
DECLARE_MENU_EDIT_ITEM(uint16_3);
DECLARE_MENU_EDIT_ITEM(uint16_4);
DECLARE_MENU_EDIT_ITEM(uint16_5);
DECLARE_MENU_EDIT_ITEM(float3);
DECLARE_MENU_EDIT_ITEM(float52);
DECLARE_MENU_EDIT_ITEM(float43);
DECLARE_MENU_EDIT_ITEM(float5);
DECLARE_MENU_EDIT_ITEM(float5_25);
DECLARE_MENU_EDIT_ITEM(float51);
DECLARE_MENU_EDIT_ITEM(float51sign);
DECLARE_MENU_EDIT_ITEM(float52sign);
DECLARE_MENU_EDIT_ITEM(long5);
DECLARE_MENU_EDIT_ITEM(long5_25);

class MenuItem_bool {
  public:
    static void action(PGM_P const pstr, bool* ptr, const screenFunc_t callbackFunc=nullptr);
};

////////////////////////////////////////////
//////////// Menu System Macros ////////////
////////////////////////////////////////////

/**
 * SCREEN_OR_MENU_LOOP generates init code for a screen or menu
 *
 *   encoderTopLine is the top menu line to display
 *   _lcdLineNr is the index of the LCD line (e.g., 0-3)
 *   _menuLineNr is the menu item to draw and process
 *   _thisItemNr is the index of each MENU_ITEM or STATIC_ITEM
 */
#define SCREEN_OR_MENU_LOOP() \
  int8_t _menuLineNr = encoderTopLine, _thisItemNr; \
  for (int8_t _lcdLineNr = 0; _lcdLineNr < LCD_HEIGHT; _lcdLineNr++, _menuLineNr++) { \
    _thisItemNr = 0

/**
 * START_SCREEN  Opening code for a screen having only static items.
 *               Do simplified scrolling of the entire screen.
 *
 * START_MENU    Opening code for a screen with menu items.
 *               Scroll as-needed to keep the selected line in view.
 */
#define START_SCREEN() \
  scroll_screen(LCD_HEIGHT, false); \
  bool _skipStatic = false; \
  SCREEN_OR_MENU_LOOP()

#define START_MENU() \
  scroll_screen(1, true); \
  bool _skipStatic = true; \
  SCREEN_OR_MENU_LOOP()

#define END_SCREEN() \
  } \
  screen_items = _thisItemNr

#define END_MENU() \
  } \
  screen_items = _thisItemNr; \
  UNUSED(_skipStatic)

#if ENABLED(ENCODER_RATE_MULTIPLIER)
  #define ENCODER_RATE_MULTIPLY(F) (ui.encoderRateMultiplierEnabled = F)
  #define _MENU_ITEM_MULTIPLIER_CHECK(USE_MULTIPLIER) do{ if (USE_MULTIPLIER) ui.enable_encoder_multiplier(true); }while(0)
  //#define ENCODER_RATE_MULTIPLIER_DEBUG  // If defined, output the encoder steps per second value
#else
  #define ENCODER_RATE_MULTIPLY(F) NOOP
  #define _MENU_ITEM_MULTIPLIER_CHECK(USE_MULTIPLIER)
#endif

/**
 * MENU_ITEM generates draw & handler code for a menu item, potentially calling:
 *
 *   draw_menu_item_<type>(sel, row, label, arg3...)
 *   MenuItem_<type>::action(arg3...)
 *
 * Examples:
 *   BACK_ITEM(MSG_WATCH)
 *   MENU_ITEM(back, MSG_WATCH)
 *     draw_menu_item_back(sel, row, GET_TEXT(MSG_WATCH))
 *     MenuItem_back::action()
 *
 *   ACTION_ITEM(MSG_PAUSE_PRINT, lcd_sdcard_pause)
 *   MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause)
 *     draw_menu_item_function(sel, row, GET_TEXT(MSG_PAUSE_PRINT), lcd_sdcard_pause)
 *     MenuItem_function::action(lcd_sdcard_pause)
 *
 *   EDIT_ITEM(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
 *     draw_menu_item_int3(sel, row, GET_TEXT(MSG_SPEED), &feedrate_percentage, 10, 999)
 *     MenuItem_int3::action(GET_TEXT(MSG_SPEED), &feedrate_percentage, 10, 999)
 *
 */
#define _MENU_ITEM_P(TYPE, USE_MULTIPLIER, PLABEL, V...) do {  \
    _skipStatic = false;                                          \
    if (_menuLineNr == _thisItemNr) {                             \
      PGM_P const plabel = PLABEL;                                \
      if (encoderLine == _thisItemNr && ui.use_click()) {         \
        _MENU_ITEM_MULTIPLIER_CHECK(USE_MULTIPLIER);              \
        MenuItem_##TYPE ::action(plabel, ##V);                    \
        if (screen_changed) return;                               \
      }                                                           \
      if (ui.should_draw())                                       \
        draw_menu_item_ ## TYPE                                   \
          (encoderLine == _thisItemNr, _lcdLineNr, plabel, ##V);  \
    }                                                             \
  ++_thisItemNr;                                                  \
}while(0)

// Used to print static text with no visible cursor.
// Parameters: label [, bool center [, bool invert [, char *value] ] ]
#define STATIC_ITEM_P(PLABEL, V...) do{                   \
  if (_menuLineNr == _thisItemNr) {                       \
    if (_skipStatic && encoderLine <= _thisItemNr) {      \
      ui.encoderPosition += ENCODER_STEPS_PER_MENU_ITEM;  \
      ++encoderLine;                                      \
    }                                                     \
    if (ui.should_draw())                                 \
      draw_menu_item_static(_lcdLineNr, PLABEL, ##V);     \
  }                                                       \
  ++_thisItemNr;                                          \
} while(0)

#define MENU_ITEM_ADDON_START(X) do{ \
  if (ui.should_draw() && _menuLineNr == _thisItemNr - 1) { \
    SETCURSOR(X, _lcdLineNr)

#define MENU_ITEM_ADDON_END() } }while(0)

#define STATIC_ITEM(LABEL, V...) STATIC_ITEM_P(GET_TEXT(LABEL), ##V)

#define MENU_ITEM_P(TYPE, PLABEL, V...)       _MENU_ITEM_P(TYPE, false, PLABEL, ##V)
#define MENU_ITEM(TYPE, LABEL, V...)           MENU_ITEM_P(TYPE, GET_TEXT(LABEL), ##V)

#define EDIT_ITEM_P(TYPE, PLABEL, V...)        MENU_ITEM_P(TYPE, PLABEL, ##V)
#define EDIT_ITEM(TYPE, LABEL, V...)           EDIT_ITEM_P(TYPE, GET_TEXT(LABEL), ##V)

#define EDIT_ITEM_FAST_P(TYPE, PLABEL, V...)  _MENU_ITEM_P(TYPE, true, PLABEL, ##V)
#define EDIT_ITEM_FAST(TYPE, LABEL, V...) EDIT_ITEM_FAST_P(TYPE, GET_TEXT(LABEL), ##V)

#define ACTION_ITEM_P(PLABEL, ACTION)          MENU_ITEM_P(function, PLABEL, ACTION)
#define ACTION_ITEM(LABEL, ACTION)           ACTION_ITEM_P(GET_TEXT(LABEL), ACTION)

#define GCODES_ITEM_P(PLABEL, GCODES)          MENU_ITEM_P(gcode, PLABEL, GCODES)
#define GCODES_ITEM(LABEL, GCODES)           GCODES_ITEM_P(GET_TEXT(LABEL), GCODES)

#define SUBMENU_P(PLABEL, DEST)                MENU_ITEM_P(submenu, PLABEL, DEST)
#define SUBMENU(LABEL, DEST)                     SUBMENU_P(GET_TEXT(LABEL), DEST)

#define BACK_ITEM(LABEL)                         MENU_ITEM(back, LABEL)
#define SKIP_ITEM() (_thisItemNr++)

////////////////////////////////////////////
/////////////// Menu Screens ///////////////
////////////////////////////////////////////

void menu_main();
void menu_move();

#if ENABLED(SDSUPPORT)
  void menu_media();
#endif

// First Fan Speed title in "Tune" and "Control>Temperature" menus
#if FAN_COUNT > 0 && HAS_FAN0
  #if FAN_COUNT > 1
    #define FAN_SPEED_1_SUFFIX " 1"
  #else
    #define FAN_SPEED_1_SUFFIX ""
  #endif
#endif

////////////////////////////////////////////
//////// Menu Item Helper Functions ////////
////////////////////////////////////////////

void lcd_move_z();
void _lcd_draw_homing();

#define HAS_LINE_TO_Z ANY(DELTA, PROBE_MANUALLY, MESH_BED_LEVELING, LEVEL_BED_CORNERS)

#if HAS_LINE_TO_Z
  void line_to_z(const float &z);
#endif

#if ANY(AUTO_BED_LEVELING_UBL, PID_AUTOTUNE_MENU, ADVANCED_PAUSE_FEATURE)
  void lcd_enqueue_one_now(const char * const cmd);
  void lcd_enqueue_one_now_P(PGM_P const cmd);
#endif

#if ENABLED(LEVEL_BED_CORNERS)
  void _lcd_level_bed_corners();
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
#endif

#if ENABLED(EEPROM_SETTINGS)
  void lcd_store_settings();
  void lcd_load_settings();
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  void menu_job_recovery();
#endif
