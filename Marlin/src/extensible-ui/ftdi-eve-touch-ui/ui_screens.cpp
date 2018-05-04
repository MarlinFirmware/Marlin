/******************
 * ui_screens.cpp *
 ******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../../Marlin.h"
#include "../ui_config.h"

#if defined(FTDI_EVE_TOUCHSCREEN)

#include "../../lcd/ultralcd.h"

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_timings.h"


#include "../ui_api.h"
#include "ui_sounds.h"
#include "ui_bitmaps.h"
#include "ui_screens.h"

using namespace FTDI;

#define DISPLAY_UPDATE_INTERVAL    1000
#define TOUCH_REPEATS_PER_SECOND      4
#define DEBOUNCE_PERIOD             100

#define N_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

/**************************** GRID LAYOUT MACROS **************************/

/* The grid layout macros allow buttons to be arranged on a grid so
 * that their locations become independent of the display size. The
 * layout model is similar to that of HTML TABLEs.
 *
 * These macros are meant to be evaluated into constants at compile
 * time, so resolution independence can be as efficient as using
 * hard-coded coordinates.
 */

// Margin defines the margin (in pixels) on each side of a button in
// the layout

#if defined(LCD_800x480)
#define MARGIN_L         5
#define MARGIN_R         5
#define MARGIN_T         5
#define MARGIN_B         5
#else
#define MARGIN_L         3
#define MARGIN_R         3
#define MARGIN_T         3
#define MARGIN_B         3
#endif

// EDGE_R adds some black space on the right edge of the display
// This shifts some of the screens left to visually center them.

#define EDGE_R           0

// GRID_X and GRID_Y computes the positions of the divisions on
// the layout grid.
#if defined(USE_PORTRAIT_ORIENTATION)
#define GRID_X(x)        ((x)*(Vsize-EDGE_R)/GRID_COLS)
#define GRID_Y(y)        ((y)*Hsize/GRID_ROWS)
#else
#define GRID_X(x)        ((x)*(Hsize-EDGE_R)/GRID_COLS)
#define GRID_Y(y)        ((y)*Vsize/GRID_ROWS)
#endif

// BTN_X, BTN_Y, BTN_W and BTN_X returns the top-left and width
// and height of a button, taking into account the button margins.

#define BTN_X(x)         (GRID_X(x-1) + MARGIN_L)
#define BTN_Y(y)         (GRID_Y(y-1) + MARGIN_T)
#define BTN_W(w)         (GRID_X(w)   - MARGIN_L - MARGIN_R)
#define BTN_H(h)         (GRID_Y(h)   - MARGIN_T - MARGIN_B)

// Abbreviations for common phrases, to allow a button to be
// defined in one line of source.
#define BTN_POS(x,y)     BTN_X(x), BTN_Y(y)
#define BTN_SIZE(w,h)    BTN_W(w), BTN_H(h)
#define BTN              cmd.draw_button
#define BTX              cmd.draw_button_text
#define BTI              cmd.draw_button_icon
#define BTN_TAG(t)       cmd.set_tag(t);
#define RGB(rgb)         cmd.set_foreground_color(rgb);
#define THEME(color)     cmd.set_foreground_color(Theme::color);
#define BTN_ENABLED(en)  if(en) { \
    cmd.set_color(Theme::btn_rgb_enabled); \
    cmd.set_foreground_color(Theme::btn_fg_enabled); \
  } else { \
    cmd.set_color(Theme::btn_rgb_disabled); \
    cmd.set_foreground_color(Theme::btn_fg_disabled); \
    cmd.set_tag(0); \
  }

#define FONT_SML         Theme::font_small
#define FONT_MED         Theme::font_medium
#define FONT_LRG         Theme::font_large

#define MENU_BTN_STYLE   Theme::font_medium, OPT_3D

#define EXEC_GCODE(cmd)  Marlin_LCD_API::runGCode(cmd)

/****************************** SCREEN STATIC DATA *************************/

// To save RAM, store state information related to a particular screen
// in a union. The values should be initialized in the onEntry method.

static union {
  struct {uint8_t increment;}          ValueAdjusters;
  struct {uint8_t page, selected_tag;} FilesScreen;
} screen_data;

/******************************* MENU SCREEN TABLE ******************************/

SCREEN_TABLE {
  DECL_SCREEN(BootScreen),
  DECL_SCREEN(AboutScreen),
  DECL_SCREEN(AlertBoxScreen),
  DECL_SCREEN(RestoreFailsafeScreen),
  DECL_SCREEN(ConfirmAbortPrint),
  DECL_SCREEN(CalibrationScreen),
  DECL_SCREEN(StatusScreen),
  DECL_SCREEN(MenuScreen),
  DECL_SCREEN(TuneScreen),
  DECL_SCREEN(MoveAxisScreen),
  DECL_SCREEN(AdvancedSettingsScreen),
  DECL_SCREEN(StepsScreen),
  DECL_SCREEN(ZOffsetScreen),
  DECL_SCREEN(FeedrateScreen),
  DECL_SCREEN(TemperatureScreen),
  DECL_SCREEN(CalibrationRegistersScreen),
  DECL_SCREEN(FilesScreen),
};

SCREEN_TABLE_POST

/************************************ MENU THEME ********************************/

namespace Theme {
  const uint32_t theme_darkest = 0x2A2F0A;
  const uint32_t theme_dark    = 0x495212;
  const uint32_t theme_light   = 0x8C9D22;

  const uint32_t background    = theme_dark;
  const uint32_t back_btn      = theme_light;

  const uint32_t x_axis        = 0x500000;
  const uint32_t y_axis        = 0x005000;
  const uint32_t z_axis        = 0x000050;
  const uint32_t e_axis        = 0x000000;
  const uint32_t feedrate      = 0x000000;

  const uint32_t toggle_on     = theme_light;
  const uint32_t toggle_off    = theme_darkest;

  // Disabled vs enabled buttons
  const uint32_t btn_rgb_enabled  = 0xFFFFFF;
  const uint32_t btn_fg_enabled   = theme_darkest;
  const uint32_t btn_rgb_disabled = theme_dark;
  const uint32_t btn_fg_disabled  = theme_dark;

  // Files screens

  const uint32_t files_selected = theme_light;

  // Adjustment Screens

  const uint32_t incr_btn      = theme_darkest;

  // Status screen
  const uint32_t status_bg     = 0x707070;
  const uint32_t status_dark   = 0x404040;
  const uint32_t stop_btn      = 0xF02020;
  const uint32_t prnt_btn      = 0x20D020;
  const uint32_t progress      = 0x404040;
  const uint32_t status_msg    = 0x404040;
  const uint32_t fan_speed     = 0x6060D0;
  const uint32_t temp          = 0xD04000;
  const uint32_t axis_label    = 0x404040;

  // Calibration Registers Screen
  const uint32_t transformA    = 0x3010D0;
  const uint32_t transformB    = 0x4010D0;
  const uint32_t transformC    = 0x5010D0;
  const uint32_t transformD    = 0x6010D0;
  const uint32_t transformE    = 0x7010D0;
  const uint32_t transformF    = 0x8010D0;
  const uint32_t transformVal  = 0x104010;

#if defined(LCD_800x480)
  #if defined(USE_PORTRAIT_ORIENTATION)
    const int16_t  font_small    = 29;
    const int16_t  font_medium   = 30;
    const int16_t  font_large    = 30;
  #else
    const int16_t  font_small    = 30;
    const int16_t  font_medium   = 30;
    const int16_t  font_large    = 31;
  #endif
  const float      icon_scale    = 1.0;
#else
  #if defined(USE_PORTRAIT_ORIENTATION)
  const int16_t  font_small    = 27;
  const int16_t  font_medium   = 28;
  const int16_t  font_large    = 28;
  const float    icon_scale    = 0.7;
  #else
  const int16_t  font_small    = 27;
  const int16_t  font_medium   = 28;
  const int16_t  font_large    = 30;
  const float    icon_scale    = 0.6;
  #endif
#endif

  const effect_t press_sound   = CHACK;
  const effect_t repeat_sound  = CHACK;
  const effect_t unpress_sound = POP;
};

/******************************** BOOT SCREEN ****************************/

void BootScreen::onRedraw(draw_mode_t what) {
  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  CLCD::turn_on_backlight();
  CLCD::SoundPlayer::set_volume(255);
}

void BootScreen::onIdle() {
  if(CLCD::is_touching()) {
    // If the user is touching the screen at startup, then
    // assume the user wants to re-calibrate the screen.
    // This gives the user the ability to recover a
    // miscalibration that has been stored to EEPROM.
    GOTO_SCREEN(CalibrationScreen);
  } else {
    GOTO_SCREEN(StatusScreen);
  }
}

/******************************** ABOUT SCREEN ****************************/

CLCD::SoundPlayer sound;

void AboutScreen::onEntry() {
  UIScreen::onEntry();
  sound.play(chimes);
}

void AboutScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;
  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  #define GRID_COLS 4
  #define GRID_ROWS 8

  BTX( BTN_POS(1,2), BTN_SIZE(4,1), F("Color LCD Interface"),          FONT_LRG);
  BTN_TAG(2)
  BTX( BTN_POS(1,3), BTN_SIZE(4,1), F("(c) 2018 Aleph Objects, Inc."), FONT_LRG);

  BTX( BTN_POS(1,5), BTN_SIZE(4,1), getFirmwareName(), FONT_LRG);

  BTN_TAG(1) BTN_ENABLED(1) BTN( BTN_POS(2,7), BTN_SIZE(2,1), F("Okay"), MENU_BTN_STYLE);
}

bool AboutScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:        GOTO_PREVIOUS();                         return true;
    case 2:        GOTO_SCREEN(CalibrationRegistersScreen); return true;
  }
}

/**************************** GENERIC DIALOG BOX SCREEN ****************************/

void DialogBoxBaseClass::onEntry() {
  repaintBackground();
  UIScreen::onEntry();
}

void DialogBoxBaseClass::drawDialog(const progmem_str lines[], size_t n_lines, progmem_str btn1, progmem_str btn2 ) {
  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  #define GRID_COLS 2
  #define GRID_ROWS 8

  for(uint8_t line = 0; line < n_lines; line++) {
    BTX( BTN_POS(1,3-n_lines/2+line), BTN_SIZE(2,1), lines[line], FONT_LRG);
  }

  if(btn1 && btn2) {
    BTN_TAG(1) BTN_ENABLED(1) BTN( BTN_POS(1,8), BTN_SIZE(1,1), btn1, MENU_BTN_STYLE);
    BTN_TAG(2) BTN_ENABLED(1) BTN( BTN_POS(2,8), BTN_SIZE(1,1), btn2, MENU_BTN_STYLE);
  } else if(btn1) {
    BTN_TAG(1) BTN_ENABLED(1) BTN( BTN_POS(1,8), BTN_SIZE(2,1), btn1, MENU_BTN_STYLE);
  } else if(btn2) {
    BTN_TAG(2) BTN_ENABLED(1) BTN( BTN_POS(1,8), BTN_SIZE(2,1), btn2, MENU_BTN_STYLE);
  }
}

bool DialogBoxBaseClass::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1: GOTO_PREVIOUS(); return true;
    case 2: GOTO_PREVIOUS(); return true;
  }
}

/****************************** ALERT BOX SCREEN *****************************/

void AlertBoxScreen::onEntry() {
  UIScreen::onEntry();
}

void AlertBoxScreen::onRedraw(draw_mode_t what) {
  DialogBoxBaseClass::onRedraw(what);
}

void AlertBoxScreen::show(const progmem_str line1, const progmem_str line2, const progmem_str line3) {
  CLCD::CommandFifo cmd;
  cmd.cmd(CMD_DLSTART);

  progmem_str lines[] = {line1, line2, line3};
  drawDialog(lines, line3 ? 3 : line2 ? 2 : 1, F("Okay"), 0);

  if(!storeBackground()) {
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLLN("Unable to set the confirmation message, not enough DL cache space");
    #else
      #if defined(UI_FRAMEWORK_DEBUG)
        Serial.print(F("Unable to set the confirmation message, not enough DL cache space"));
      #endif
    #endif
  }

  sound.play(c_maj_arpeggio);
  GOTO_SCREEN(AlertBoxScreen);
}

/**************************** RESTORE FAILSAFE SCREEN ***************************/

void RestoreFailsafeScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    progmem_str lines[] = {
      F("Are you sure?"),
      F("Customizations will be lost.")
    };
    drawDialog(lines, N_ELEMENTS(lines), F("Yes"), F("No"));
  }
}

bool RestoreFailsafeScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:
      EXEC_GCODE(F("M502\nM500"));
      AlertBoxScreen::show(F("Factory settings restored."));
      // Remove RestoreFailsafeScreen from the stack
      // so the alert box doesn't return to it.
      current_screen.forget();
      return true;
    default:
      return DialogBoxBaseClass::onTouchStart(tag);
  }
}

/**************************** RESTORE FAILSAFE SCREEN ***************************/

void ConfirmAbortPrint::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    progmem_str lines[] = {
      F("Are you sure you want"),
      F("to stop the print?")
    };
    drawDialog(lines, N_ELEMENTS(lines), F("Yes"), F("No"));
  }
}

bool ConfirmAbortPrint::onTouchStart(uint8_t tag) {
  using namespace Marlin_LCD_API;

  switch(tag) {
    case 1:
      #if defined (SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLLN("Abort confirmed");
      #else
        #if defined(UI_FRAMEWORK_DEBUG)
          Serial.print(F("Abort confirmed"));
        #endif
      #endif
      GOTO_PREVIOUS();
      stopPrint();
      return true;
    default:
      return DialogBoxBaseClass::onTouchStart(tag);
  }
}

/************************************ KILL SCREEN *******************************/

// The kill screen is an oddball that happens after Marlin has killed the events
// loop. So we only have a show() method rather than onRedraw(). The KillScreen
// should not be used as a model for other UI screens as it is an exception.

void KillScreen::show(progmem_str message) {
  CLCD::CommandFifo cmd;

  cmd.cmd(CMD_DLSTART);
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  #define GRID_COLS 4
  #define GRID_ROWS 8

  BTX( BTN_POS(1,2), BTN_SIZE(4,1), message,                     FONT_LRG);

  BTX( BTN_POS(1,3), BTN_SIZE(4,1), F("PRINTER HALTED"),         FONT_LRG);

  BTX( BTN_POS(1,6), BTN_SIZE(4,1), F("Please reset"),           FONT_LRG);

  cmd.cmd(DL_DISPLAY);
  cmd.cmd(CMD_SWAP);
  cmd.execute();

  sound.play(sad_trombone);

  // Marlin won't call the idle function anymore,
  // so we have to loop here to play the sounds.
  while(sound.has_more_notes()) {
    sound.onIdle();
  }
}

/*********************************** STATUS SCREEN ******************************/
#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_ROWS 9
  #define GRID_COLS 3
#else
  #define GRID_ROWS 8
  #define GRID_COLS 3
#endif

void StatusScreen::static_axis_position() {
  CLCD::CommandFifo cmd;

  BTN_TAG(6)
  #if defined(USE_PORTRAIT_ORIENTATION)
    THEME(axis_label) BTN( BTN_POS(1,5), BTN_SIZE(2,1), F(""),  FONT_LRG, OPT_FLAT);
    THEME(axis_label) BTN( BTN_POS(1,6), BTN_SIZE(2,1), F(""),  FONT_LRG, OPT_FLAT);
    THEME(axis_label) BTN( BTN_POS(1,7), BTN_SIZE(2,1), F(""),  FONT_LRG, OPT_FLAT);
                      BTX( BTN_POS(1,5), BTN_SIZE(1,1), F("X"), FONT_SML);
                      BTX( BTN_POS(1,6), BTN_SIZE(1,1), F("Y"), FONT_SML);
                      BTX( BTN_POS(1,7), BTN_SIZE(1,1), F("Z"), FONT_SML);
    THEME(x_axis)     BTN( BTN_POS(2,5), BTN_SIZE(2,1), F(""),  FONT_MED, OPT_FLAT);
    THEME(y_axis)     BTN( BTN_POS(2,6), BTN_SIZE(2,1), F(""),  FONT_MED, OPT_FLAT);
    THEME(z_axis)     BTN( BTN_POS(2,7), BTN_SIZE(2,1), F(""),  FONT_MED, OPT_FLAT);
  #else
    THEME(axis_label) BTN( BTN_POS(1,5), BTN_SIZE(1,2), F(""),  FONT_LRG, OPT_FLAT);
    THEME(axis_label) BTN( BTN_POS(2,5), BTN_SIZE(1,2), F(""),  FONT_LRG, OPT_FLAT);
    THEME(axis_label) BTN( BTN_POS(3,5), BTN_SIZE(1,2), F(""),  FONT_LRG, OPT_FLAT);
                      BTX( BTN_POS(1,5), BTN_SIZE(1,1), F("X"), FONT_SML);
                      BTX( BTN_POS(2,5), BTN_SIZE(1,1), F("Y"), FONT_SML);
                      BTX( BTN_POS(3,5), BTN_SIZE(1,1), F("Z"), FONT_SML);
    THEME(x_axis)     BTN( BTN_POS(1,6), BTN_SIZE(1,1), F(""),  FONT_MED, OPT_FLAT);
    THEME(y_axis)     BTN( BTN_POS(2,6), BTN_SIZE(1,1), F(""),  FONT_MED, OPT_FLAT);
    THEME(z_axis)     BTN( BTN_POS(3,6), BTN_SIZE(1,1), F(""),  FONT_MED, OPT_FLAT);
  #endif
}

void StatusScreen::dynamic_axis_position() {
  using namespace Marlin_LCD_API;
  CLCD::CommandFifo cmd;

  char x_str[15];
  char y_str[15];
  char z_str[15];

  dtostrf(getAxisPosition_mm(X), 5, 1, x_str);
  dtostrf(getAxisPosition_mm(Y), 5, 1, y_str);
  dtostrf(getAxisPosition_mm(Z), 5, 1, z_str);

  strcat_P(x_str, PSTR(" mm"));
  strcat_P(y_str, PSTR(" mm"));
  strcat_P(z_str, PSTR(" mm"));

  BTN_TAG(6)
  #if defined(USE_PORTRAIT_ORIENTATION)
    BTX( BTN_POS(2,5), BTN_SIZE(2,1), x_str, FONT_MED);
    BTX( BTN_POS(2,6), BTN_SIZE(2,1), y_str, FONT_MED);
    BTX( BTN_POS(2,7), BTN_SIZE(2,1), z_str, FONT_MED);
  #else
    BTX( BTN_POS(1,6), BTN_SIZE(1,1), x_str, FONT_MED);
    BTX( BTN_POS(2,6), BTN_SIZE(1,1), y_str, FONT_MED);
    BTX( BTN_POS(3,6), BTN_SIZE(1,1), z_str, FONT_MED);
  #endif

  //#define MARGIN_T 5
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS 8
#else
  #define GRID_COLS 12
#endif

void StatusScreen::static_temperature() {
  CLCD::CommandFifo cmd;

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN_TAG(5)
    THEME(temp)      BTN( BTN_POS(1,1), BTN_SIZE(4,2), F(""), FONT_SML, OPT_FLAT);
    THEME(temp)      BTN( BTN_POS(1,1), BTN_SIZE(8,1), F(""), FONT_SML, OPT_FLAT);
    THEME(fan_speed) BTN( BTN_POS(5,2), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    BTN_TAG(0)
    THEME(progress)  BTN( BTN_POS(1,3), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    THEME(progress)  BTN( BTN_POS(5,3), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
  #else
    BTN_TAG(5)
    THEME(temp)      BTN( BTN_POS(1,1), BTN_SIZE(4,2), F(""), FONT_SML, OPT_FLAT);
    THEME(temp)      BTN( BTN_POS(1,1), BTN_SIZE(8,1), F(""), FONT_SML, OPT_FLAT);
    THEME(fan_speed) BTN( BTN_POS(5,2), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    BTN_TAG(0)
    THEME(progress)  BTN( BTN_POS(9,1), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    THEME(progress)  BTN( BTN_POS(9,2), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
  #endif

  // Draw Extruder Bitmap on Extruder Temperature Button

  cmd.bitmap_source(Extruder_Icon_Info);
  cmd.bitmap_layout(Extruder_Icon_Info);
  cmd.bitmap_size  (Extruder_Icon_Info);

  BTN_TAG(5)
  BTI(BTN_POS(1,1), BTN_SIZE(1,1),  Extruder_Icon_Info, Theme::icon_scale);
  BTI(BTN_POS(5,1), BTN_SIZE(1,1),  Extruder_Icon_Info, Theme::icon_scale);

  // Draw Bed Heat Bitmap on Bed Heat Button
  cmd.bitmap_source(Bed_Heat_Icon_Info);
  cmd.bitmap_layout(Bed_Heat_Icon_Info);
  cmd.bitmap_size  (Bed_Heat_Icon_Info);

  BTI(BTN_POS(1,2), BTN_SIZE(1,1), Bed_Heat_Icon_Info, Theme::icon_scale);

  // Draw Fan Percent Bitmap on Bed Heat Button

  cmd.bitmap_source(Fan_Icon_Info);
  cmd.bitmap_layout(Fan_Icon_Info);
  cmd.bitmap_size  (Fan_Icon_Info);

  BTI(BTN_POS(5,2), BTN_SIZE(1,1), Fan_Icon_Info, Theme::icon_scale);
}

#define ROUND(val) uint16_t((val)+0.5)

void StatusScreen::dynamic_temperature() {
  using namespace Marlin_LCD_API;

  CLCD::CommandFifo cmd;

  char e0_str[15];
  char e1_str[15];
  char bed_str[15];
  char fan_str[15];

  sprintf_P(
    fan_str,
    PSTR("%-3d %%"),
    int8_t(getFan_percent(0))
  );

  sprintf_P(
    bed_str,
    PSTR("%-3d / %-3d  " ),
    ROUND(getActualTemp_celsius(0)),
    ROUND(getTargetTemp_celsius(0))
  );

  sprintf_P(
    e0_str,
    PSTR("%-3d / %-3d C"),
    ROUND(getActualTemp_celsius(1)),
    ROUND(getTargetTemp_celsius(1))
  );

  #if EXTRUDERS == 2
    sprintf_P(
      e1_str,
      PSTR("%-3d / %-3d C"),
      ROUND(getActualTemp_celsius(2)),
      ROUND(getTargetTemp_celsius(2))
    );
  #else
    strcpy_P(
      e1_str,
      PSTR("-")
    );
  #endif

  BTN_TAG(5)
  BTX( BTN_POS(2,1), BTN_SIZE(3,1), e0_str,  FONT_MED);
  BTX( BTN_POS(6,1), BTN_SIZE(3,1), e1_str,  FONT_MED);
  BTX( BTN_POS(2,2), BTN_SIZE(3,1), bed_str, FONT_MED);
  BTX( BTN_POS(6,2), BTN_SIZE(3,1), fan_str, FONT_MED);
}

void StatusScreen::static_progress() {
  CLCD::CommandFifo cmd;

  BTN_TAG(0)
  #if defined(USE_PORTRAIT_ORIENTATION)
    THEME(progress)  BTN( BTN_POS(1,3), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    THEME(progress)  BTN( BTN_POS(5,3), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
  #else
    THEME(progress)  BTN( BTN_POS(9,1), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
    THEME(progress)  BTN( BTN_POS(9,2), BTN_SIZE(4,1), F(""), FONT_SML, OPT_FLAT);
  #endif
}

void StatusScreen::dynamic_progress() {
  using namespace Marlin_LCD_API;
  CLCD::CommandFifo cmd;

  const uint32_t elapsed = getProgress_seconds_elapsed();
  const uint8_t hrs = elapsed/3600;
  const uint8_t min = (elapsed/60)%60;

  char time_str[10];
  char progress_str[10];

  sprintf_P(time_str,     PSTR(" %02d : %02d"), hrs, min);
  sprintf_P(progress_str, PSTR("%-3d %%"),      getProgress_percent() );

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN_TAG(0)
    BTX( BTN_POS(1,3), BTN_SIZE(4,1), time_str,     FONT_MED);
    BTX( BTN_POS(5,3), BTN_SIZE(4,1), progress_str, FONT_MED);
  #else
    BTN_TAG(0)
    BTX( BTN_POS(9,1), BTN_SIZE(4,1), time_str,     FONT_MED);
    BTX( BTN_POS(9,2), BTN_SIZE(4,1), progress_str, FONT_MED);
  #endif
}

#define GRID_COLS 4

void StatusScreen::static_media_button() {
  using namespace Marlin_LCD_API;
  CLCD::CommandFifo cmd;

  if(isMediaInserted()) {
    BTN_TAG(3)
    BTN_ENABLED(1)
  } else {
    BTN_TAG(0)
    cmd.set_color(Theme::status_bg);
    cmd.set_foreground_color(Theme::status_bg);
  }

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN( BTN_POS(1,9), BTN_SIZE(2,1), F(""),      MENU_BTN_STYLE);
  #else
    BTN( BTN_POS(3,7), BTN_SIZE(1,2), F(""),      MENU_BTN_STYLE);
  #endif

  if(!isMediaInserted()) {
    cmd.set_color(Theme::status_dark);
  }

  // Draw Thumb Drive Bitmap on USB Button

  cmd.bitmap_source(TD_Icon_Info);
  cmd.bitmap_layout(TD_Icon_Info);
  cmd.bitmap_size  (TD_Icon_Info);

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTI(BTN_POS(1,9), BTN_SIZE(2,1), TD_Icon_Info, Theme::icon_scale);
  #else
    BTI(BTN_POS(3,7), BTN_SIZE(1,2), TD_Icon_Info, Theme::icon_scale);
  #endif
}

void StatusScreen::static_interaction_buttons() {
  CLCD::CommandFifo cmd;

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN_TAG(4) BTN_ENABLED(1) BTN( BTN_POS(3,9), BTN_SIZE(2,1), F("MENU"),  MENU_BTN_STYLE);
    #else
    BTN_TAG(4) BTN_ENABLED(1) BTN( BTN_POS(4,7), BTN_SIZE(1,2), F("MENU"),  MENU_BTN_STYLE);
  #endif
}

void StatusScreen::dynamic_interaction_buttons() {
  using namespace Marlin_LCD_API;
  CLCD::CommandFifo cmd;

  if(isPrintingFromMedia()) {
    BTN_TAG(1)
    THEME(stop_btn)
  } else {
    BTN_TAG(0)
    cmd.set_color(Theme::status_bg);
    cmd.set_foreground_color(Theme::status_bg);
  }

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN( BTN_POS(1,8), BTN_SIZE(4,1), F("STOP"),  MENU_BTN_STYLE);
  #else
    BTN( BTN_POS(1,7), BTN_SIZE(2,2), F("STOP"),  MENU_BTN_STYLE);
  #endif
}

#define GRID_COLS 1

void StatusScreen::static_status_message(const char * const message) {
    CLCD::CommandFifo cmd;

    BTN_TAG(0)
    #if defined(USE_PORTRAIT_ORIENTATION)
      THEME(status_msg) BTN( BTN_POS(1,4), BTN_SIZE(1,1), message, FONT_LRG, OPT_FLAT);
    #else
      THEME(status_msg) BTN( BTN_POS(1,3), BTN_SIZE(1,2), message, FONT_LRG, OPT_FLAT);
    #endif
}

void StatusScreen::setStatusMessage(const char * const message) {
  CLCD::CommandFifo cmd;
  cmd.cmd(CMD_DLSTART);

  cmd.set_clear_color(Theme::status_bg);
  cmd.clear(1,1,1);

  static_temperature();
  static_progress();
  static_axis_position();
  static_media_button();
  static_interaction_buttons();
  static_status_message(message);

  if(!storeBackground()) {
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLLNPAIR("Unable to set the status message, not enough DL cache space: ",message);
    #else
      #if defined(UI_FRAMEWORK_DEBUG)
        Serial.print(F("Unable to set the status message, not enough DL cache space: "));
        Serial.println(message);
      #endif
    #endif
  }

  if(current_screen.getType() == current_screen.lookupScreen(StatusScreen::onRedraw)) {
    current_screen.onRefresh();
  }
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS 8
#else
  #define GRID_COLS 12
#endif

void StatusScreen::onStartup() {
  // Load the bitmaps for the status screen

  CLCD::flash_write_rgb332_bitmap(TD_Icon_Info.RAMG_addr,       TD_Icon,       sizeof(TD_Icon));
  CLCD::flash_write_rgb332_bitmap(Extruder_Icon_Info.RAMG_addr, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::flash_write_rgb332_bitmap(Bed_Heat_Icon_Info.RAMG_addr, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::flash_write_rgb332_bitmap(Fan_Icon_Info.RAMG_addr,      Fan_Icon,      sizeof(Fan_Icon));
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if(what & FOREGROUND) {
    dynamic_temperature();
    dynamic_progress();
    dynamic_axis_position();
    dynamic_interaction_buttons();
  }
}

void StatusScreen::onEntry() {
  onRefresh();
}

void StatusScreen::onIdle() {
  onRefresh();
}

bool StatusScreen::onTouchStart(uint8_t tag) {
  using namespace Marlin_LCD_API;

  switch(tag) {
    case 1:
      #if defined(UI_FRAMEWORK_DEBUG)
        #if defined (SERIAL_PROTOCOLLNPGM)
          SERIAL_PROTOCOLLNPGM("Aborting print");
        #endif
      #endif
      GOTO_SCREEN(ConfirmAbortPrint);
      break;
    case 3:  GOTO_SCREEN(FilesScreen);       break;
    case 4:
      if(isPrinting()) {
        GOTO_SCREEN(TuneScreen);
      } else {
        GOTO_SCREEN(MenuScreen);
      }
      break;
    case 5:  GOTO_SCREEN(TemperatureScreen); break;
    case 6:
      if(!isPrinting()) {
        GOTO_SCREEN(MoveAxisScreen);
      }
      break;
  }
  return true;
}

/************************************ MENU SCREEN *******************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_ROWS 7
  #define GRID_COLS 2
#else
  #define GRID_ROWS 5
  #define GRID_COLS 2
#endif

void MenuScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CLCD::CommandFifo cmd;
    cmd.set_clear_color(Theme::background);
    cmd.clear(1,1,1);

    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(2) BTN_ENABLED(1)  BTN( BTN_POS(1,1), BTN_SIZE(1,1), F("Auto Home"),          MENU_BTN_STYLE);
      BTN_TAG(3) BTN_ENABLED(1)  BTN( BTN_POS(2,1), BTN_SIZE(1,1), F("Level X Axis"),       MENU_BTN_STYLE);
      BTN_TAG(4) BTN_ENABLED(1)  BTN( BTN_POS(1,2), BTN_SIZE(1,1), F("Move Axis"),          MENU_BTN_STYLE);
      BTN_TAG(5) BTN_ENABLED(1)  BTN( BTN_POS(2,2), BTN_SIZE(1,1), F("Motors Off"),         MENU_BTN_STYLE);
      BTN_TAG(6) BTN_ENABLED(1)  BTN( BTN_POS(1,3), BTN_SIZE(2,1), F("Temperature"),        MENU_BTN_STYLE);
      BTN_TAG(7) BTN_ENABLED(0)  BTN( BTN_POS(1,4), BTN_SIZE(2,1), F("Change Filament"),    MENU_BTN_STYLE);
      BTN_TAG(8) BTN_ENABLED(1)  BTN( BTN_POS(1,5), BTN_SIZE(2,1), F("Advanced Settings"),  MENU_BTN_STYLE);
      BTN_TAG(9) BTN_ENABLED(1)  BTN( BTN_POS(1,6), BTN_SIZE(2,1), F("About Firmware"),     MENU_BTN_STYLE);
    #else
      BTN_TAG(2) BTN_ENABLED(1)  BTN( BTN_POS(1,1), BTN_SIZE(1,1), F("Auto Home"),          MENU_BTN_STYLE);
      BTN_TAG(3) BTN_ENABLED(1)  BTN( BTN_POS(2,1), BTN_SIZE(1,1), F("Level X Axis"),       MENU_BTN_STYLE);
      BTN_TAG(4) BTN_ENABLED(1)  BTN( BTN_POS(1,2), BTN_SIZE(1,1), F("Move Axis"),          MENU_BTN_STYLE);
      BTN_TAG(5) BTN_ENABLED(1)  BTN( BTN_POS(2,2), BTN_SIZE(1,1), F("Motors Off"),         MENU_BTN_STYLE);
      BTN_TAG(6) BTN_ENABLED(1)  BTN( BTN_POS(1,3), BTN_SIZE(1,1), F("Temperature"),        MENU_BTN_STYLE);
      BTN_TAG(7) BTN_ENABLED(0)  BTN( BTN_POS(2,3), BTN_SIZE(1,1), F("Change Filament"),    MENU_BTN_STYLE);
      BTN_TAG(8) BTN_ENABLED(1)  BTN( BTN_POS(1,4), BTN_SIZE(1,1), F("Advanced Settings"),  MENU_BTN_STYLE);
      BTN_TAG(9) BTN_ENABLED(1)  BTN( BTN_POS(2,4), BTN_SIZE(1,1), F("About Firmware"),     MENU_BTN_STYLE);
    #endif

    #if defined(USE_PORTRAIT_ORIENTATION)
      #define MARGIN_T 15
      BTN_TAG(1) THEME(back_btn) BTN( BTN_POS(1,7), BTN_SIZE(2,1), F("Back"),               MENU_BTN_STYLE);
    #else
      BTN_TAG(1) THEME(back_btn) BTN( BTN_POS(1,5), BTN_SIZE(2,1), F("Back"),               MENU_BTN_STYLE);
    #endif

    #define MARGIN_T 5
  }
}

bool MenuScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:  GOTO_PREVIOUS();                                    break;
    case 2:  EXEC_GCODE(F("G28"));                               break;
    case 3:  EXEC_GCODE(F(LULZBOT_MENU_AXIS_LEVELING_COMMANDS)); break;
    case 4:  GOTO_SCREEN(MoveAxisScreen);                        break;
    case 5:  EXEC_GCODE(F("M84"));                               break;
    case 6:  GOTO_SCREEN(TemperatureScreen);                     break;
    case 8:  GOTO_SCREEN(AdvancedSettingsScreen);                break;
    case 9:  GOTO_SCREEN(AboutScreen);                           break;
    default:
      return false;
  }
  return true;
}

/************************************ TUNE SCREEN *******************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_ROWS 5
  #define GRID_COLS 2
#else
  #define GRID_ROWS 3
  #define GRID_COLS 2
#endif

void TuneScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CLCD::CommandFifo cmd;
    cmd.set_clear_color(Theme::background);
    cmd.clear(1,1,1);

    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(2) BTN_ENABLED(1)  BTN( BTN_POS(1,1), BTN_SIZE(2,1), F("Temperature"),        MENU_BTN_STYLE);
      BTN_TAG(3) BTN_ENABLED(0)  BTN( BTN_POS(1,2), BTN_SIZE(2,1), F("Change Filament"),    MENU_BTN_STYLE);
      BTN_TAG(4) BTN_ENABLED(1)  BTN( BTN_POS(1,3), BTN_SIZE(2,1), F("Z Offset"),           MENU_BTN_STYLE);
      BTN_TAG(5) BTN_ENABLED(1)  BTN( BTN_POS(1,4), BTN_SIZE(2,1), F("Print Speed"),        MENU_BTN_STYLE);
    #else
      BTN_TAG(2) BTN_ENABLED(1)  BTN( BTN_POS(1,1), BTN_SIZE(1,1), F("Temperature"),        MENU_BTN_STYLE);
      BTN_TAG(3) BTN_ENABLED(0)  BTN( BTN_POS(1,2), BTN_SIZE(1,1), F("Change Filament"),    MENU_BTN_STYLE);
      BTN_TAG(4) BTN_ENABLED(1)  BTN( BTN_POS(2,1), BTN_SIZE(1,1), F("Z Offset"),           MENU_BTN_STYLE);
      BTN_TAG(5) BTN_ENABLED(1)  BTN( BTN_POS(2,2), BTN_SIZE(2,1), F("Print Speed"),        MENU_BTN_STYLE);
    #endif

    #if defined(USE_PORTRAIT_ORIENTATION)
      #define MARGIN_T 15
      BTN_TAG(1) THEME(back_btn) BTN( BTN_POS(1,5), BTN_SIZE(2,1), F("Back"),               MENU_BTN_STYLE);
    #else
      BTN_TAG(1) THEME(back_btn) BTN( BTN_POS(1,3), BTN_SIZE(2,1), F("Back"),               MENU_BTN_STYLE);
    #endif

    #define MARGIN_T 5
  }
}

bool TuneScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:  GOTO_PREVIOUS();                    break;
    case 2:  GOTO_SCREEN(TemperatureScreen);     break;
    case 4:  GOTO_SCREEN(ZOffsetScreen);         break;
    case 5:  GOTO_SCREEN(FeedrateScreen);        break;
    default:
      return false;
  }
  return true;
}

/******************************* CONFIGURATION SCREEN ****************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_ROWS 7
  #define GRID_COLS 2
#else
  #define GRID_ROWS 4
  #define GRID_COLS 2
#endif

void AdvancedSettingsScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CLCD::CommandFifo cmd;
    cmd.set_clear_color(Theme::background);
    cmd.clear(1,1,1);

    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(4) BTN_ENABLED(1)   BTN( BTN_POS(1,1), BTN_SIZE(1,2), F("Z Offset "),                 MENU_BTN_STYLE);
      BTN_TAG(5) BTN_ENABLED(1)   BTN( BTN_POS(1,3), BTN_SIZE(1,1), F("Steps/mm"),                  MENU_BTN_STYLE);

      BTN_TAG(6)  BTN_ENABLED(0)  BTN( BTN_POS(2,1), BTN_SIZE(1,1), F("Velocity "),                MENU_BTN_STYLE);
      BTN_TAG(7)  BTN_ENABLED(0)  BTN( BTN_POS(2,2), BTN_SIZE(1,1), F("Acceleration"),             MENU_BTN_STYLE);
      BTN_TAG(8)  BTN_ENABLED(0)  BTN( BTN_POS(2,3), BTN_SIZE(1,1), F("Jerk"),                     MENU_BTN_STYLE);
      BTN_TAG(9)  BTN_ENABLED(1)  BTN( BTN_POS(1,4), BTN_SIZE(2,1), F("Recalibrate Screen"),       MENU_BTN_STYLE);
      BTN_TAG(10) BTN_ENABLED(1)  BTN( BTN_POS(1,5), BTN_SIZE(2,1), F("Restore Factory Settings"), MENU_BTN_STYLE);
      BTN_TAG(2)  BTN_ENABLED(1)  BTN( BTN_POS(1,6), BTN_SIZE(2,1), F("Save As Default"),          MENU_BTN_STYLE);
      BTN_TAG(1)  THEME(back_btn) BTN( BTN_POS(1,7), BTN_SIZE(2,1), F("Back"),                     MENU_BTN_STYLE);
    #else
      BTN_TAG(4)  BTN_ENABLED(1)  BTN( BTN_POS(1,1), BTN_SIZE(1,1), F("Z Offset "),                MENU_BTN_STYLE);
      BTN_TAG(5)  BTN_ENABLED(1)  BTN( BTN_POS(1,2), BTN_SIZE(1,1), F("Steps/mm"),                 MENU_BTN_STYLE);

      BTN_TAG(6)  BTN_ENABLED(0)  BTN( BTN_POS(2,1), BTN_SIZE(1,1), F("Velocity "),                MENU_BTN_STYLE);
      BTN_TAG(7)  BTN_ENABLED(0)  BTN( BTN_POS(2,2), BTN_SIZE(1,1), F("Acceleration"),             MENU_BTN_STYLE);
      BTN_TAG(8)  BTN_ENABLED(0)  BTN( BTN_POS(2,3), BTN_SIZE(1,1), F("Jerk"),                     MENU_BTN_STYLE);
      BTN_TAG(10) BTN_ENABLED(1)  BTN( BTN_POS(1,3), BTN_SIZE(1,1), F("Restore Failsafe"),         MENU_BTN_STYLE);

      BTN_TAG(2)  BTN_ENABLED(1)  BTN( BTN_POS(1,4), BTN_SIZE(1,1), F("Save"),                     MENU_BTN_STYLE);
      BTN_TAG(1)  THEME(back_btn) BTN( BTN_POS(2,4), BTN_SIZE(1,1), F("Back"),                     MENU_BTN_STYLE);
    #endif
  }
}

bool AdvancedSettingsScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:  GOTO_PREVIOUS();                    break;
    case 2:
      EXEC_GCODE(F("M500"));
      AlertBoxScreen::show(F("Settings saved!"));
      break;
    case 4:  GOTO_SCREEN(ZOffsetScreen);         break;
    case 5:  GOTO_SCREEN(StepsScreen);           break;
    case 9:  GOTO_SCREEN(CalibrationScreen);     break;
    case 10: GOTO_SCREEN(RestoreFailsafeScreen); break;
    default:
      return false;
  }
  return true;
}

/******************************** CALIBRATION SCREEN ****************************/

#define GRID_COLS 4
#define GRID_ROWS 16

void CalibrationScreen::onEntry() {
  // Clear the display
  CLCD::CommandFifo cmd;
  cmd.cmd(CMD_DLSTART);
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);
  cmd.cmd(DL_DISPLAY);
  cmd.cmd(CMD_SWAP);
  cmd.execute();

  // Wait for the touch to release before starting,
  // as otherwise the first calibration point could
  // be misinterpreted.
  while(CLCD::is_touching()) {
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPGM)
        SERIAL_PROTOCOLLNPGM("Waiting for touch release");
      #endif
    #endif
  }
  UIScreen::onEntry();
}

void CalibrationScreen::onRedraw(draw_mode_t what) {
  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  #if defined(USE_PORTRAIT_ORIENTATION)
  BTX( BTN_POS(1,8), BTN_SIZE(4,1), F("Touch the dots"), FONT_LRG);
  BTX( BTN_POS(1,9), BTN_SIZE(4,1), F("to calibrate"), FONT_LRG);
  #else
    #if defined(LCD_800x480)
      BTX( BTN_POS(1,1), BTN_SIZE(4,16), F("Touch the dots to calibrate"), FONT_LRG);
    #else
      BTX( BTN_POS(1,1), BTN_SIZE(4,16), F("Touch the dots to calibrate"), FONT_MED);
    #endif
  #endif

  cmd.cmd(CMD_CALIBRATE);
}

void CalibrationScreen::onIdle() {
  if(CLCD::CommandFifo::is_idle()) {
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPGM)
        SERIAL_PROTOCOLLNPGM("Calibration finished");
      #endif
    #endif
    GOTO_SCREEN(StatusScreen);
  }
  #if defined(UI_FRAMEWORK_DEBUG)
    else {
      #if defined (SERIAL_PROTOCOLLNPGM)
        SERIAL_PROTOCOLLNPGM("Waiting for calibration to finish.");
      #endif
    }
  #endif
}

/***************************** CALIBRATION REGISTERS SCREEN ****************************/

#define MARGIN_T 5
#define GRID_ROWS 7
#define GRID_COLS 2

void CalibrationRegistersScreen::onRedraw(draw_mode_t what) {
  const uint32_t T_Transform_A = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_A);
  const uint32_t T_Transform_B = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_B);
  const uint32_t T_Transform_C = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_C);
  const uint32_t T_Transform_D = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_D);
  const uint32_t T_Transform_E = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_E);
  const uint32_t T_Transform_F = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_F);
  char b[20];

  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  BTN_TAG(0)
  THEME(transformA) BTN( BTN_POS(1,1), BTN_SIZE(1,1), F("TOUCH TRANSFORM_A"), 28, OPT_3D);
  THEME(transformB) BTN( BTN_POS(1,2), BTN_SIZE(1,1), F("TOUCH TRANSFORM_B"), 28, OPT_3D);
  THEME(transformC) BTN( BTN_POS(1,3), BTN_SIZE(1,1), F("TOUCH TRANSFORM_C"), 28, OPT_3D);
  THEME(transformD) BTN( BTN_POS(1,4), BTN_SIZE(1,1), F("TOUCH TRANSFORM_D"), 28, OPT_3D);
  THEME(transformE) BTN( BTN_POS(1,5), BTN_SIZE(1,1), F("TOUCH TRANSFORM_E"), 28, OPT_3D);
  THEME(transformF) BTN( BTN_POS(1,6), BTN_SIZE(1,1), F("TOUCH TRANSFORM_F"), 28, OPT_3D);

  THEME(transformVal) BTN( BTN_POS(2,1), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);
  THEME(transformVal) BTN( BTN_POS(2,2), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);
  THEME(transformVal) BTN( BTN_POS(2,3), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);
  THEME(transformVal) BTN( BTN_POS(2,4), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);
  THEME(transformVal) BTN( BTN_POS(2,5), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);
  THEME(transformVal) BTN( BTN_POS(2,6), BTN_SIZE(1,1), F(""), 28, OPT_FLAT);

  sprintf_P(b, PSTR("0x%08lX"), T_Transform_A); BTX( BTN_POS(2,1), BTN_SIZE(1,1), b, 28);
  sprintf_P(b, PSTR("0x%08lX"), T_Transform_B); BTX( BTN_POS(2,2), BTN_SIZE(1,1), b, 28);
  sprintf_P(b, PSTR("0x%08lX"), T_Transform_C); BTX( BTN_POS(2,3), BTN_SIZE(1,1), b, 28);
  sprintf_P(b, PSTR("0x%08lX"), T_Transform_D); BTX( BTN_POS(2,4), BTN_SIZE(1,1), b, 28);
  sprintf_P(b, PSTR("0x%08lX"), T_Transform_E); BTX( BTN_POS(2,5), BTN_SIZE(1,1), b, 28);
  sprintf_P(b, PSTR("0x%08lX"), T_Transform_F); BTX( BTN_POS(2,6), BTN_SIZE(1,1), b, 28);

  #define GRID_COLS 3

  BTN_TAG(1) THEME(back_btn) BTN( BTN_POS(3,7), BTN_SIZE(1,1), F("Back"), MENU_BTN_STYLE);

  sound.play(js_bach_joy);
}

bool CalibrationRegistersScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:        GOTO_PREVIOUS();                 break;
    default:
      return false;
  }
  return true;
}

/*************************** GENERIC VALUE ADJUSTMENT SCREEN ******************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS  6
  #define GRID_ROWS 10
#else
  #define GRID_COLS  9
  #define GRID_ROWS  6
#endif

ValueAdjusters::widgets_t::widgets_t(draw_mode_t what) : _what(what) {
  if(what & BACKGROUND) {
    CLCD::CommandFifo cmd;
    cmd.set_clear_color(Theme::background);
    cmd.clear(1,1,1);

    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(1) THEME(back_btn)  BTN( BTN_POS(1,10), BTN_SIZE(6,1), F("Back"),           MENU_BTN_STYLE);
    #else
      BTN_TAG(1) THEME(back_btn)  BTN( BTN_POS(8,6),  BTN_SIZE(2,1), F("Back"),           MENU_BTN_STYLE);
    #endif
  }
  _line = 1;
}

void ValueAdjusters::widgets_t::color(uint32_t color) {
  _color = color;
}

void ValueAdjusters::widgets_t::units(const char *units) {
  _units = units;
}

void ValueAdjusters::widgets_t::precision(uint8_t decimals) {
  _decimals = decimals;
}

void ValueAdjusters::widgets_t::heading(char *label) {
  CLCD::CommandFifo cmd;

  if(_what & BACKGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(0) THEME(background) BTN( BTN_POS(1, _line),  BTN_SIZE(6,1), (progmem_str) label, FONT_MED, OPT_FLAT);
    #else
      BTN_TAG(0) THEME(background) BTN( BTN_POS(3, _line),  BTN_SIZE(4,1), (progmem_str) label, FONT_MED, OPT_FLAT);
    #endif
  }

  _line++;
}

void ValueAdjusters::widgets_t::_draw_increment_btn(uint8_t line, const uint8_t tag) {
  CLCD::CommandFifo  cmd;
  const char        *label = PSTR("?");
  uint8_t            pos;

  switch(tag) {
    case 240: label = PSTR(   ".001"); pos = _decimals - 3; break;
    case 241: label = PSTR(   ".01" ); pos = _decimals - 2; break;
    case 242: label = PSTR(  "0.1"  ); pos = _decimals - 1; break;
    case 243: label = PSTR(  "1"    ); pos = _decimals + 0; break;
    case 244: label = PSTR( "10"    ); pos = _decimals + 1; break;
    case 245: label = PSTR("100"    ); pos = _decimals + 2; break;
    default:
      #if defined(UI_FRAMEWORK_DEBUG)
        #if defined(SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLLNPAIR("Unknown tag for increment btn: ", tag);
        #else
        Serial.print(F("Unknown tag for increment btn:"));
        Serial.println(tag);
        #endif
      #endif
      ;
  }

  BTN_TAG(tag)
  switch(pos) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      case 0: BTN( BTN_POS(3,_line), BTN_SIZE(1,1), progmem_str(label), FONT_SML, OPT_3D); break;
      case 1: BTN( BTN_POS(4,_line), BTN_SIZE(1,1), progmem_str(label), FONT_SML, OPT_3D); break;
      case 2: BTN( BTN_POS(5,_line), BTN_SIZE(1,1), progmem_str(label), FONT_SML, OPT_3D); break;
    #else
      case 0: BTN( BTN_POS(8,2), BTN_SIZE(2,1), progmem_str(label), FONT_MED, OPT_3D); break;
      case 1: BTN( BTN_POS(8,3), BTN_SIZE(2,1), progmem_str(label), FONT_MED, OPT_3D); break;
      case 2: BTN( BTN_POS(8,4), BTN_SIZE(2,1), progmem_str(label), FONT_MED, OPT_3D); break;
    #endif
  }
}

void ValueAdjusters::widgets_t::increments() {
  CLCD::CommandFifo cmd;

  if(_what & BACKGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      BTN_TAG(0) THEME(background) BTN( BTN_POS(1, _line), BTN_SIZE(2,1), F("Increment:"),  FONT_SML, OPT_FLAT);
    #else
      BTN_TAG(0) THEME(background) BTN( BTN_POS(8,1),      BTN_SIZE(2,1), F("Increment"),   FONT_MED, OPT_FLAT);
    #endif

    // Draw all the buttons in the off state.
    THEME(toggle_off);
    _draw_increment_btn(_line+1, 243 - _decimals);
    _draw_increment_btn(_line+1, 244 - _decimals);
    _draw_increment_btn(_line+1, 245 - _decimals);

    screen_data.ValueAdjusters.increment = 243 - _decimals;
  }

  if(_what & FOREGROUND) {
    CLCD::CommandFifo cmd;

    THEME(toggle_on);
    _draw_increment_btn(_line+1, screen_data.ValueAdjusters.increment);
  }

  _line += 2;
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #if defined(LCD_800x480)
    #define EDGE_R 20
  #else
    #define EDGE_R 10
  #endif
#else
  #if defined(LCD_800x480)
    #define EDGE_R  40
  #else
    #define EDGE_R  20
  #endif
#endif

void ValueAdjusters::widgets_t::adjuster(uint8_t tag, const char *label,float value) {
  CLCD::CommandFifo cmd;

  if(_what & BACKGROUND) {
    progmem_str   str  = (progmem_str) label;
    BTN_TAG( 0     ) RGB(_color)       BTN( BTN_POS(3,_line), BTN_SIZE(2,1), F(""),  FONT_SML, OPT_FLAT);
    BTN_TAG( 0     ) THEME(background) BTN( BTN_POS(1,_line), BTN_SIZE(2,1), str,    FONT_SML, OPT_FLAT);
    BTN_TAG(tag    ) BTN_ENABLED(1)    BTN( BTN_POS(5,_line), BTN_SIZE(1,1), F("-"), FONT_MED, OPT_3D);
    BTN_TAG(tag + 1) BTN_ENABLED(1)    BTN( BTN_POS(6,_line), BTN_SIZE(1,1), F("+"), FONT_MED, OPT_3D);
  }

  if(_what & FOREGROUND) {
    char b[32];

    dtostrf(value, 5, _decimals, b);
    strcat_P(b, PSTR(" "));
    strcat_P(b, (const char*) _units);

    BTN_TAG(0)
    BTX( BTN_POS(3,_line), BTN_SIZE(2,1), b, FONT_SML);
  }

  _line++;
}

void ValueAdjusters::onEntry() {
  screen_data.ValueAdjusters.increment = 242;
  UIScreen::onEntry();
}

bool ValueAdjusters::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:           GOTO_PREVIOUS();                             return true;
    case 240 ... 245: screen_data.ValueAdjusters.increment = tag;  break;
    default:          return current_screen.onTouchHeld(tag);
  }
  current_screen.onRefresh();
  return true;
}

float ValueAdjusters::getIncrement() {
  switch(screen_data.ValueAdjusters.increment) {
    case 240: return   0.001;
    case 241: return   0.01;
    case 242: return   0.1;
    case 243: return   1.0;
    case 244: return  10.0;
    case 245: return 100.0;
  }
}

#define EDGE_R 0

/******************************** MOVE AXIS SCREEN ******************************/

void MoveAxisScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  widgets_t w(what);
  w.precision(1);
  w.units(PSTR("mm"));

  w.heading(                                PSTR("Move Axis"));
  w.color(Theme::x_axis);   w.adjuster(  2, PSTR("X:"),  getAxisPosition_mm(X));
  w.color(Theme::y_axis);   w.adjuster(  4, PSTR("Y:"),  getAxisPosition_mm(Y));
  w.color(Theme::z_axis);   w.adjuster(  6, PSTR("Z:"),  getAxisPosition_mm(Z));
  #if EXTRUDERS == 1
    w.color(Theme::e_axis); w.adjuster(  8, PSTR("E0:"), getAxisPosition_mm(E0));
  #else
    w.color(Theme::e_axis); w.adjuster(  8, PSTR("E0:"), getAxisPosition_mm(E0));
    w.color(Theme::x_axis); w.adjuster( 10, PSTR("E1:"), getAxisPosition_mm(E1));
  #endif
  w.increments();
}

bool MoveAxisScreen::onTouchHeld(uint8_t tag) {
  using namespace Marlin_LCD_API;

  // We don't want to stack up moves, so wait until the
  // machine is idle before sending another.
  if(isMoving()) {
    return false;
  }

  float inc = getIncrement();
  axis_t axis;
  const float feedrate_mm_s = inc * TOUCH_REPEATS_PER_SECOND;

  switch(tag) {
    case  2: axis = X;  inc *= -1;  break;
    case  3: axis = X;  inc *=  1;  break;
    case  4: axis = Y;  inc *= -1;  break;
    case  5: axis = Y;  inc *=  1;  break;
    case  6: axis = Z;  inc *= -1;  break;
    case  7: axis = Z;  inc *=  1;  break;
    case  8: axis = E0; inc *= -1;  break;
    case  9: axis = E0; inc *=  1;  break;
    case 10: axis = E1; inc *= -1;  break;
    case 11: axis = E1; inc *=  1;  break;
    default:
      return false;
  }
  setAxisPosition_mm(axis, getAxisPosition_mm(axis) + inc, feedrate_mm_s);
  onRefresh();
  return true;
}

/******************************* TEMPERATURE SCREEN ******************************/

void TemperatureScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  widgets_t w(what);
  w.precision(0);
  w.color(Theme::temp);
  w.units(PSTR("C"));

  w.heading(       PSTR("Temperature:"));
  #if EXTRUDERS == 1
    w.adjuster(   2, PSTR("Nozzle:"),       getTargetTemp_celsius(1));
  #else
    w.adjuster(   2, PSTR("Nozzle 1:"),     getTargetTemp_celsius(1));
    w.adjuster(   4, PSTR("Nozzle 2:"),     getTargetTemp_celsius(2));
  #endif
  w.adjuster(    20, PSTR("Bed:"),          getTargetTemp_celsius(0));

  w.color(Theme::fan_speed);
  w.units(PSTR("%"));

  w.adjuster(    10, PSTR("Fan Speed:"),    getFan_percent(0));
  w.increments();
}

bool TemperatureScreen::onTouchHeld(uint8_t tag) {
  using namespace Marlin_LCD_API;

  switch(tag) {
    case 20: setTargetTemp_celsius(0, getTargetTemp_celsius(0) - getIncrement()); break;
    case 21: setTargetTemp_celsius(0, getTargetTemp_celsius(0) + getIncrement()); break;
    case  2: setTargetTemp_celsius(1, getTargetTemp_celsius(1) - getIncrement()); break;
    case  3: setTargetTemp_celsius(1, getTargetTemp_celsius(1) + getIncrement()); break;
    case  4: setTargetTemp_celsius(2, getTargetTemp_celsius(2) - getIncrement()); break;
    case  5: setTargetTemp_celsius(2, getTargetTemp_celsius(2) + getIncrement()); break;
    case 10: setFan_percent(       0, getFan_percent(0)        - getIncrement()); break;
    case 11: setFan_percent(       0, getFan_percent(0)        + getIncrement()); break;
    default:
      return false;
  }
  onRefresh();
  return true;
}

/******************************* STEPS SCREEN ******************************/

void StepsScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  widgets_t w(what);
  w.precision(0);
  w.units(PSTR(""));

  w.heading(                               PSTR("Steps/mm"));
  w.color(Theme::x_axis); w.adjuster(   2, PSTR("X:"),  getAxisSteps_per_mm(X) );
  w.color(Theme::y_axis); w.adjuster(   4, PSTR("Y:"),  getAxisSteps_per_mm(Y) );
  w.color(Theme::z_axis); w.adjuster(   6, PSTR("Z:"),  getAxisSteps_per_mm(Z) );
  #if EXTRUDERS == 1
    w.color(Theme::e_axis); w.adjuster( 6, PSTR("E:"),  getAxisSteps_per_mm(E0) );
  #else
    w.color(Theme::e_axis); w.adjuster( 8, PSTR("E0:"), getAxisSteps_per_mm(E0) );
    w.color(Theme::e_axis); w.adjuster(10, PSTR("E1:"), getAxisSteps_per_mm(E1) );
  #endif
  w.increments();
}

bool StepsScreen::onTouchHeld(uint8_t tag) {
  using namespace Marlin_LCD_API;

  float inc = getIncrement();
  axis_t axis;

  switch(tag) {
    case  2:  axis = X;  inc *= -1;  break;
    case  3:  axis = X;  inc *=  1;  break;
    case  4:  axis = Y;  inc *= -1;  break;
    case  5:  axis = Y;  inc *=  1;  break;
    case  6:  axis = Z;  inc *= -1;  break;
    case  7:  axis = Z;  inc *=  1;  break;
    case  8:  axis = E0; inc *= -1;  break;
    case  9:  axis = E0; inc *=  1;  break;
    #if EXTRUDERS == 2
    case 10:  axis = E1; inc *= -1;  break;
    case 11:  axis = E1; inc *=  1;  break;
    #endif
    default:
      return false;
  }

  setAxisSteps_per_mm(axis, getAxisSteps_per_mm(axis) + inc);
  onRefresh();
  return true;
}

/***************************** Z-OFFSET SCREEN ***************************/

void ZOffsetScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  widgets_t w(what);
  w.precision(3);
  w.units(PSTR("mm"));

  w.heading(     PSTR("Z Offset"));
  w.adjuster(4,  PSTR("Z Offset:"), getZOffset_mm());
  w.increments();
}

bool ZOffsetScreen::onTouchHeld(uint8_t tag) {
  using namespace Marlin_LCD_API;

  switch(tag) {
    case 4:  incrementZOffset_mm(-getIncrement()); break;
    case 5:  incrementZOffset_mm( getIncrement()); break;
    default:
      return false;
  }
  onRefresh();
  return true;
}

/***************************** FEEDRATE SCREEN ***************************/

void FeedrateScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("%"));

  w.heading(PSTR("Print Speed"));
  w.adjuster(4,  PSTR("Speed"), getFeedRate_percent());
  w.increments();
}

bool FeedrateScreen::onTouchHeld(uint8_t tag) {
  using namespace Marlin_LCD_API;

  float inc = getIncrement();
  switch(tag) {
    case 4:  setFeedrate_percent(getFeedRate_percent() - inc); break;
    case 5:  setFeedrate_percent(getFeedRate_percent() + inc); break;
    default:
      return false;
  }
  onRefresh();
  return true;
}

/***************************** FILES SCREEN ***************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS  6
  #define GRID_ROWS 14
#else
  #define GRID_COLS  3
  #define GRID_ROWS  6
#endif

const uint16_t filesPerPage = GRID_ROWS - 4;

void FilesScreen::onEntry() {
  screen_data.FilesScreen.page            = 0;
  screen_data.FilesScreen.selected_tag    = 0xFF;
  UIScreen::onEntry();
}

const char *FilesScreen::getSelectedShortFilename() {
  using namespace Marlin_LCD_API;

  Media_Iterator iterator(getIndexForTag(screen_data.FilesScreen.selected_tag));
  return iterator.shortFilename();
}

uint8_t FilesScreen::getTagForIndex(uint16_t fileIndex) {
  return fileIndex + 1;
}

uint16_t FilesScreen::getIndexForTag(uint8_t tag) {
  return tag - 1;
}

void FilesScreen::onRedraw(draw_mode_t what) {
  using namespace Marlin_LCD_API;

  CLCD::CommandFifo cmd;
  cmd.set_clear_color(Theme::background);
  cmd.clear(1,1,1);

  #define MARGIN_T 0
  #define MARGIN_B 0

  bool dirSelected = false;

  Media_Iterator iterator(screen_data.FilesScreen.page * filesPerPage);
  if(iterator.count()) {
    do {
      const uint16_t tag = getTagForIndex(iterator.value());
      const bool isDir   = iterator.isDirectory();

      BTN_TAG(tag)
      if(screen_data.FilesScreen.selected_tag == tag) {
        RGB(Theme::files_selected)
        dirSelected = isDir;
      } else {
        RGB(Theme::background)
      }
      BTN( BTN_POS(1,tag+2), BTN_SIZE(6,1), F(""),               FONT_SML, OPT_FLAT);
      BTX( BTN_POS(1,tag+2), BTN_SIZE(6,1), iterator.filename(), FONT_LRG, OPT_CENTERY);
      if(isDir) {
        BTX( BTN_POS(1,tag+2), BTN_SIZE(6,1), F("> "),           FONT_LRG, OPT_CENTERY | OPT_RIGHTX);
      }

      iterator.next();
    } while(iterator.hasMore());
  }

  #define MARGIN_T 5
  #define MARGIN_B 5

  const uint16_t pageCount = iterator.count() / filesPerPage + 1;
  const bool prevEnabled   = screen_data.FilesScreen.page > 0;
  const bool nextEnabled   = screen_data.FilesScreen.page < (pageCount - 1);
  const bool itemSelected  = screen_data.FilesScreen.selected_tag != 0xFF;
  const uint8_t backTag    = isAtRootDir() ? 240 : 245;

  char page_str[15];
  sprintf_P(page_str, PSTR("Page %d of %d"), screen_data.FilesScreen.page + 1, pageCount);

  #if defined(USE_PORTRAIT_ORIENTATION)
    BTN_TAG(0)
    BTX( BTN_POS(1,1), BTN_SIZE(6,1), page_str, FONT_LRG, OPT_CENTER);

    if(prevEnabled)  {BTN_TAG(241); BTN( BTN_POS(1,1),  BTN_SIZE(1,2), F("<"), MENU_BTN_STYLE);}
    if(nextEnabled)  {BTN_TAG(242); BTN( BTN_POS(6,1),  BTN_SIZE(1,2), F(">"), MENU_BTN_STYLE);}

    #define MARGIN_T 15
    BTN_TAG(backTag) THEME(back_btn) BTN( BTN_POS(5,13), BTN_SIZE(2,2), F("Back"), MENU_BTN_STYLE);

    BTN_ENABLED(itemSelected)
    if(dirSelected) {
      BTN_TAG(244); BTN( BTN_POS(1,13), BTN_SIZE(4,2), F("Open"),  MENU_BTN_STYLE);
    } else {
      BTN_TAG(243); BTN( BTN_POS(1,13), BTN_SIZE(4,2), F("Print"), MENU_BTN_STYLE);
    }
  #else
    BTN_TAG(backTag) THEME(back_btn) BTN( BTN_POS(1,4), BTN_SIZE(1,1), F("Back"), MENU_BTN_STYLE);
  #endif

  #define MARGIN_T 5
}

bool FilesScreen::onTouchStart(uint8_t tag) {
  using namespace Marlin_LCD_API;

  switch(tag) {
    case 240: GOTO_PREVIOUS();                  return true;
    case 241: screen_data.FilesScreen.page--;   break;
    case 242: screen_data.FilesScreen.page++;   break;
    case 243:
      printFromSDCard(getSelectedShortFilename());
      lcd_setstatusPGM(PSTR("Print Starting"), 0);
      GOTO_SCREEN(StatusScreen);
      sound.play(start_print);
      return true;
    case 244:
      changeDir(getSelectedShortFilename());
      break;
    case 245:
      upDir();
      break;
    default:
      if(tag < 240) {
        if(screen_data.FilesScreen.selected_tag != tag) {
          screen_data.FilesScreen.selected_tag = tag;
        } else {
          // Double clicked.
        }
      }
      break;
  }
  onRefresh();
  return true;
}

/******************************** MAIN EVENT HANDLER *******************************/

void lcd_init() {
  using namespace Marlin_LCD_API;

  CLCD::init();
  CLCD::DLCache::init();
  initMedia();

  lcd_setstatusPGM(PSTR(WELCOME_MSG));

  current_screen.start();
}

void lcd_update() {
  using namespace Marlin_LCD_API;

  enum {
    UNPRESSED       = 0xFF, //255
    IGNORE_UNPRESS  = 0xFE, //254
    DEBOUNCING      = 0xFD  //253
  };
  static uint8_t pressed_state  = UNPRESSED;

  static tiny_interval_t touch_timer;
  static tiny_interval_t refresh_timer;

  sound.onIdle();

  if(refresh_timer.elapsed()) {
    refresh_timer.wait_for(DISPLAY_UPDATE_INTERVAL);
    current_screen.onIdle();
  }

  checkMedia();

  // If the LCD is processing commands, don't check
  // for tags since they may be changing and could
  // cause spurious events.
  if(!CLCD::CommandFifo::is_idle()) {
    return;
  }

  const uint8_t tag = CLCD::get_tag();

  switch(pressed_state) {
    case UNPRESSED:
      if(tag != 0) {
        #if defined(UI_FRAMEWORK_DEBUG)
          #if defined (SERIAL_PROTOCOLLNPAIR)
            SERIAL_PROTOCOLLNPAIR("Touch start: ", tag);
          #else
            Serial.print(F("Touch start: "));
            Serial.println(tag);
          #endif
        #endif

        // When the user taps on a button, activate the onTouchStart handler
        const uint8_t lastScreen = current_screen.getScreen();

        if(current_screen.onTouchStart(tag)) {
          touch_timer.wait_for(1000 / TOUCH_REPEATS_PER_SECOND);
          sound.play(Theme::press_sound);
        }

        if(lastScreen != current_screen.getScreen()) {
          // In the case in which a touch event triggered a new screen to be
          // drawn, we don't issue a touchEnd since it would be sent to the
          // wrong screen.
          pressed_state = IGNORE_UNPRESS;
          #if defined(UI_FRAMEWORK_DEBUG)
            #if defined (SERIAL_PROTOCOLLNPAIR)
              SERIAL_PROTOCOLLNPAIR("Ignoring press", tag);
            #else
              Serial.print(F("Ignoring press"));
              Serial.println(tag);
            #endif
          #endif
        } else {
          pressed_state = tag;
        }
      }
      break;
    case DEBOUNCING:
      if(tag == 0) {
        if(touch_timer.elapsed()) {
          pressed_state = UNPRESSED;
          sound.play(Theme::unpress_sound);
        }
      } else {
        pressed_state = IGNORE_UNPRESS;
      }
      break;
    case IGNORE_UNPRESS:
      if(tag == 0) {
        // Ignore subsequent presses for a while to avoid bouncing
        touch_timer.wait_for(DEBOUNCE_PERIOD);
        pressed_state = DEBOUNCING;
      }
      break;
    default: // PRESSED
      if(tag == pressed_state) {
        // The user is holding down a button.
        if(touch_timer.elapsed() && current_screen.onTouchHeld(tag)) {
          sound.play(Theme::repeat_sound);
          touch_timer.wait_for(1000 / TOUCH_REPEATS_PER_SECOND);
        }
      }
      else if(tag == 0) {
        #if defined(UI_FRAMEWORK_DEBUG)
          #if defined (SERIAL_PROTOCOLLNPAIR)
            SERIAL_PROTOCOLLNPAIR("Touch end: ", pressed_state);
          #else
            Serial.print(F("Touch end: "));
            Serial.println(pressed_state);
          #endif
        #endif

        current_screen.onTouchEnd(pressed_state);
        // Ignore subsequent presses for a while to avoid bouncing
        touch_timer.wait_for(DEBOUNCE_PERIOD);
        pressed_state = DEBOUNCING;
      }
      break;
  }
}

bool lcd_hasstatus() { return true; }

void lcd_reset_status() {
}

void lcd_setstatus(const char * const message, const bool persist /* = false */) {
  StatusScreen::setStatusMessage(message);
}

void lcd_setstatusPGM(const char * const message, int8_t level /* = 0 */) {
  char buff[64];
  strncpy_P(buff, message, sizeof(buff));
  StatusScreen::setStatusMessage(buff);
}

void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) {
  char buff[64];
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(buff, sizeof(buff), fmt, args);
  va_end(args);
  StatusScreen::setStatusMessage(buff);
}

void lcd_setalertstatusPGM(const char * const message) {
  lcd_setstatusPGM(message, 1);
  GOTO_SCREEN(StatusScreen);
}

void lcd_buttons_update() {}
void lcd_reset_alert_level() {}
void lcd_refresh() {current_screen.onIdle();}

namespace Marlin_LCD_API {
  void onPrinterKilled(const char* lcd_msg) {
    KillScreen::show(progmem_str(lcd_msg));
  }

  void onCardInserted() {
    lcd_setstatusPGM(PSTR(MSG_SD_INSERTED), 0);
    sound.play(card_inserted);
  }

  void onCardRemoved() {
    lcd_setstatusPGM(PSTR(MSG_SD_REMOVED), 0);
    sound.play(card_removed);
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    sound.play_tone(frequency, duration);
  }
}

#endif // FTDI_EVE_TOUCHSCREEN