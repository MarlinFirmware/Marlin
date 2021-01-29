/******************************
 * change_filament_screen.cpp *
 ******************************/

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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"
#include "screen_data.h"

using namespace ExtUI;
using namespace FTDI;
using namespace Theme;

#ifdef TOUCH_UI_PORTRAIT
  #define GRID_COLS 2
  #define GRID_ROWS 11
  #define E_TEMP_POS           BTN_POS(2,7),  BTN_SIZE(1,1)
  #define E_TEMP_LBL_POS       BTN_POS(1,7),  BTN_SIZE(1,1)
  #define UNLD_LABL_POS        BTN_POS(1,8),  BTN_SIZE(1,1)
  #define LOAD_LABL_POS        BTN_POS(2,8),  BTN_SIZE(1,1)
  #define UNLD_MOMN_POS        BTN_POS(1,9),  BTN_SIZE(1,1)
  #define LOAD_MOMN_POS        BTN_POS(2,9),  BTN_SIZE(1,1)
  #define UNLD_CONT_POS        BTN_POS(1,10), BTN_SIZE(1,1)
  #define LOAD_CONT_POS        BTN_POS(2,10), BTN_SIZE(1,1)
  #define BACK_POS             BTN_POS(1,11), BTN_SIZE(2,1)
#else
  #define GRID_COLS 4
  #define GRID_ROWS 6
  #define E_TEMP_POS           BTN_POS(3,2),  BTN_SIZE(2,1)
  #define E_TEMP_LBL_POS       BTN_POS(3,1),  BTN_SIZE(2,1)
  #define UNLD_LABL_POS        BTN_POS(3,3),  BTN_SIZE(1,1)
  #define LOAD_LABL_POS        BTN_POS(4,3),  BTN_SIZE(1,1)
  #define UNLD_MOMN_POS        BTN_POS(3,4),  BTN_SIZE(1,1)
  #define LOAD_MOMN_POS        BTN_POS(4,4),  BTN_SIZE(1,1)
  #define UNLD_CONT_POS        BTN_POS(3,5),  BTN_SIZE(1,1)
  #define LOAD_CONT_POS        BTN_POS(4,5),  BTN_SIZE(1,1)
  #define BACK_POS             BTN_POS(3,6),  BTN_SIZE(2,1)
#endif
#define REMOVAL_TEMP_LBL_POS   BTN_POS(1,3),  BTN_SIZE(2,1)
#define GRADIENT_POS           BTN_POS(1,4),  BTN_SIZE(1,3)
#define LOW_TEMP_POS           BTN_POS(2,6),  BTN_SIZE(1,1)
#define MED_TEMP_POS           BTN_POS(2,5),  BTN_SIZE(1,1)
#define HIG_TEMP_POS           BTN_POS(2,4),  BTN_SIZE(1,1)
#define HEATING_LBL_POS        BTN_POS(1,6),  BTN_SIZE(1,1)
#define CAUTION_LBL_POS        BTN_POS(1,4),  BTN_SIZE(1,1)
#define HOT_LBL_POS            BTN_POS(1,6),  BTN_SIZE(1,1)
#define E_SEL_LBL_POS          BTN_POS(1,1),  BTN_SIZE(2,1)
#define E1_SEL_POS             BTN_POS(1,2),  BTN_SIZE(1,1)
#define E2_SEL_POS             BTN_POS(2,2),  BTN_SIZE(1,1)

#define COOL_TEMP  40
#define LOW_TEMP  180
#define MED_TEMP  200
#define HIGH_TEMP 220

/****************** COLOR SCALE ***********************/

uint32_t getWarmColor(uint16_t temp, uint16_t cool, uint16_t low, uint16_t med, uint16_t high) {
  rgb_t R0, R1, mix;

  float t;
  if (temp < cool) {
    R0 = cool_rgb;
    R1 = low_rgb;
    t  = 0;
  }
  else if (temp < low) {
    R0 = cool_rgb;
    R1 = low_rgb;
    t = (float(temp)-cool)/(low-cool);
  }
  else if (temp < med) {
    R0 = low_rgb;
    R1 = med_rgb;
    t = (float(temp)-low)/(med-low);
  }
  else if (temp < high) {
    R0 = med_rgb;
    R1 = high_rgb;
    t = (float(temp)-med)/(high-med);
  }
  else if (temp >= high) {
    R0 = med_rgb;
    R1 = high_rgb;
    t = 1;
  }
  rgb_t::lerp(t, R0, R1, mix);
  return mix;
}

void ChangeFilamentScreen::drawTempGradient(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  CommandProcessor cmd;
  cmd.cmd(SCISSOR_XY   (x, y))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y,     high_rgb, x, y+h/2, med_rgb)
     .cmd(SCISSOR_XY   (x, y+h/2))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y+h/2, med_rgb,  x, y+h, low_rgb)
     .cmd(SCISSOR_XY   ())
     .cmd(SCISSOR_SIZE ());
}

void ChangeFilamentScreen::onEntry() {
  BaseScreen::onEntry();
  screen_data.ChangeFilament.e_tag = ExtUI::getActiveTool() + 10;
  screen_data.ChangeFilament.t_tag = 0;
  screen_data.ChangeFilament.repeat_tag = 0;
  screen_data.ChangeFilament.saved_extruder = getActiveTool();
  #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
    screen_data.ChangeFilament.need_purge = true;
  #endif
}

void ChangeFilamentScreen::onExit() {
  setActiveTool(screen_data.ChangeFilament.saved_extruder, true);
}

void ChangeFilamentScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(TERN(TOUCH_UI_PORTRAIT, font_large, font_medium))
       .text(E_SEL_LBL_POS, GET_TEXT_F(MSG_EXTRUDER_SELECTION))
       .text(E_TEMP_LBL_POS, GET_TEXT_F(MSG_CURRENT_TEMPERATURE))
       .text(REMOVAL_TEMP_LBL_POS, GET_TEXT_F(MSG_REMOVAL_TEMPERATURE));
    drawTempGradient(GRADIENT_POS);
  }

  if (what & FOREGROUND) {
    char str[15];
    const extruder_t e = getExtruder();

    if (isHeaterIdle(e))
      format_temp_and_idle(str, getActualTemp_celsius(e));
    else
      format_temp_and_temp(str, getActualTemp_celsius(e), getTargetTemp_celsius(e));

    const rgb_t tcol = getWarmColor(getActualTemp_celsius(e), COOL_TEMP, LOW_TEMP, MED_TEMP, HIGH_TEMP);
    cmd.cmd(COLOR_RGB(tcol))
       .tag(15)
       .rectangle(E_TEMP_POS)
       .cmd(COLOR_RGB(tcol.luminance() > 128 ? 0x000000 : 0xFFFFFF))
       .font(font_medium)
       .text(E_TEMP_POS, str)
       .colors(normal_btn);

    const bool t_ok = getActualTemp_celsius(e) > getSoftenTemp() - 10;

    if (screen_data.ChangeFilament.t_tag && !t_ok) {
      cmd.text(HEATING_LBL_POS, GET_TEXT_F(MSG_HEATING));
    } else if (getActualTemp_celsius(e) > 100) {
      cmd.cmd(COLOR_RGB(0xFF0000))
         .text(CAUTION_LBL_POS, GET_TEXT_F(MSG_CAUTION))
         .colors(normal_btn)
         .text(HOT_LBL_POS, GET_TEXT_F(MSG_HOT));
    }

    #define TOG_STYLE(A) colors(A ? action_btn : normal_btn)

    const bool tog2  = screen_data.ChangeFilament.t_tag == 2;
    const bool tog3  = screen_data.ChangeFilament.t_tag == 3;
    const bool tog4  = screen_data.ChangeFilament.t_tag == 4;
    const bool tog10 = screen_data.ChangeFilament.e_tag == 10;
    #if HAS_MULTI_HOTEND
      const bool tog11 = screen_data.ChangeFilament.e_tag == 11;
    #endif

    cmd.TOG_STYLE(tog10)
       .tag(10).button (E1_SEL_POS, F("1"))
    #if HOTENDS < 2
       .enabled(false)
    #else
       .TOG_STYLE(tog11)
    #endif
       .tag(11).button (E2_SEL_POS, F("2"));

    if (!t_ok) reset_menu_timeout();

    const bool tog7 = screen_data.ChangeFilament.repeat_tag == 7;
    const bool tog8 = screen_data.ChangeFilament.repeat_tag == 8;

    {
      char str[30];
      format_temp(str, LOW_TEMP);
      cmd.tag(2) .TOG_STYLE(tog2).button (LOW_TEMP_POS, str);

      format_temp(str, MED_TEMP);
      cmd.tag(3) .TOG_STYLE(tog3).button (MED_TEMP_POS, str);

      format_temp(str, HIGH_TEMP);
      cmd.tag(4) .TOG_STYLE(tog4).button (HIG_TEMP_POS, str);
    }

    cmd.cmd(COLOR_RGB(t_ok ? bg_text_enabled : bg_text_disabled))
       .tag(0)                              .text   (UNLD_LABL_POS, GET_TEXT_F(MSG_UNLOAD_FILAMENT))
                                            .text   (LOAD_LABL_POS, GET_TEXT_F(MSG_LOAD_FILAMENT))
       .colors(normal_btn)
       .tag(5)                .enabled(t_ok).button (UNLD_MOMN_POS, GET_TEXT_F(MSG_MOMENTARY))
       .tag(6)                .enabled(t_ok).button (LOAD_MOMN_POS, GET_TEXT_F(MSG_MOMENTARY))
       .tag(7).TOG_STYLE(tog7).enabled(t_ok).button (UNLD_CONT_POS, GET_TEXT_F(MSG_CONTINUOUS))
       .tag(8).TOG_STYLE(tog8).enabled(t_ok).button (LOAD_CONT_POS, GET_TEXT_F(MSG_CONTINUOUS))
       .tag(1).colors(action_btn)           .button (BACK_POS, GET_TEXT_F(MSG_BACK));
  }
}

uint8_t ChangeFilamentScreen::getSoftenTemp() {
  switch (screen_data.ChangeFilament.t_tag) {
    case 2:  return LOW_TEMP;
    case 3:  return MED_TEMP;
    case 4:  return HIGH_TEMP;
    default: return EXTRUDE_MINTEMP;
  }
}

ExtUI::extruder_t ChangeFilamentScreen::getExtruder() {
  switch (screen_data.ChangeFilament.e_tag) {
    case 13: return ExtUI::E3;
    case 12: return ExtUI::E2;
    case 11: return ExtUI::E1;
    default: return ExtUI::E0;
  }
}

void ChangeFilamentScreen::doPurge() {
  #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
    constexpr float purge_distance_mm = FILAMENT_UNLOAD_PURGE_LENGTH;
    if (screen_data.ChangeFilament.need_purge) {
      screen_data.ChangeFilament.need_purge = false;
      MoveAxisScreen::setManualFeedrate(getExtruder(), purge_distance_mm);
      ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(getExtruder()) + purge_distance_mm, getExtruder());
    }
  #endif
}

bool ChangeFilamentScreen::onTouchStart(uint8_t tag) {
  // Make the Momentary and Continuous buttons slightly more responsive
  switch (tag) {
    case 5: case 6: case 7: case 8:
      #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
        if (tag == 5 || tag == 7) doPurge();
      #endif
      return ChangeFilamentScreen::onTouchHeld(tag);
    default:
      return false;
  }
}

bool ChangeFilamentScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    case 2:
    case 3:
    case 4:
      // Change temperature
      screen_data.ChangeFilament.t_tag = tag;
      setTargetTemp_celsius(getSoftenTemp(), getExtruder());
      break;
    case 7:
      screen_data.ChangeFilament.repeat_tag = (screen_data.ChangeFilament.repeat_tag == 7) ? 0 : 7;
      break;
    case 8:
      screen_data.ChangeFilament.repeat_tag = (screen_data.ChangeFilament.repeat_tag == 8) ? 0 : 8;
      break;
    case 10:
    case 11:
      // Change extruder
      screen_data.ChangeFilament.e_tag      = tag;
      screen_data.ChangeFilament.t_tag      = 0;
      screen_data.ChangeFilament.repeat_tag = 0;
      #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
        screen_data.ChangeFilament.need_purge = true;
      #endif
      setActiveTool(getExtruder(), true);
      break;
    case 15: GOTO_SCREEN(TemperatureScreen); break;
  }
  return true;
}

bool ChangeFilamentScreen::onTouchHeld(uint8_t tag) {
  if (ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  constexpr float increment = 1;
  #define UI_INCREMENT_AXIS(axis) UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) UI_DECREMENT(AxisPosition_mm, axis);
  switch (tag) {
    case 5: case 7: UI_DECREMENT_AXIS(getExtruder()); break;
    case 6: case 8: UI_INCREMENT_AXIS(getExtruder()); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return false;
}

void ChangeFilamentScreen::onIdle() {
  reset_menu_timeout();
  if (screen_data.ChangeFilament.repeat_tag) onTouchHeld(screen_data.ChangeFilament.repeat_tag);
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

#endif // TOUCH_UI_FTDI_EVE
