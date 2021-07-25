/*********************
 * status_screen.cpp *
 *********************/

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
#include "../screens.h"
#include "../screen_data.h"

#ifdef FTDI_STATUS_SCREEN

#include "../archim2-flash/flash_storage.h"

using namespace FTDI;
using namespace Theme;

#if ENABLED(TOUCH_UI_PORTRAIT)
  #define GRID_ROWS 16
#else
  #define GRID_ROWS 16
#endif

void StatusScreen::draw_axis_position(draw_mode_t what) {
  CommandProcessor cmd;

  #define GRID_COLS 3

  #if ENABLED(TOUCH_UI_PORTRAIT)
    #define X_LBL_POS  BTN_POS(1, 9), BTN_SIZE(1,2)
    #define Y_LBL_POS  BTN_POS(1,11), BTN_SIZE(1,2)
    #define Z_LBL_POS  BTN_POS(1,13), BTN_SIZE(1,2)
    #define X_VAL_POS  BTN_POS(2, 9), BTN_SIZE(2,2)
    #define Y_VAL_POS  BTN_POS(2,11), BTN_SIZE(2,2)
    #define Z_VAL_POS  BTN_POS(2,13), BTN_SIZE(2,2)
  #else
    #define X_LBL_POS  BTN_POS(1, 9), BTN_SIZE(1,2)
    #define Y_LBL_POS  BTN_POS(2, 9), BTN_SIZE(1,2)
    #define Z_LBL_POS  BTN_POS(3, 9), BTN_SIZE(1,2)
    #define X_VAL_POS  BTN_POS(1,11), BTN_SIZE(1,2)
    #define Y_VAL_POS  BTN_POS(2,11), BTN_SIZE(1,2)
    #define Z_VAL_POS  BTN_POS(3,11), BTN_SIZE(1,2)
  #endif

  #define _UNION_POS(x1,y1,w1,h1,x2,y2,w2,h2) x1,y1,max(x1+w1,x2+w2)-x1,max(y1+h1,y2+h2)-y1
  #define UNION_POS(p1, p2) _UNION_POS(p1, p2)

  if (what & BACKGROUND) {
    cmd.tag(6)
       .fgcolor(Theme::axis_label)
       .font(Theme::font_large)
                               .button(UNION_POS(X_LBL_POS, X_VAL_POS), F(""), OPT_FLAT)
                               .button(UNION_POS(Y_LBL_POS, Y_VAL_POS), F(""), OPT_FLAT)
                               .button(UNION_POS(Z_LBL_POS, Z_VAL_POS), F(""), OPT_FLAT)
       .font(Theme::font_medium)
       .fgcolor(Theme::x_axis) .button(X_VAL_POS, F(""), OPT_FLAT)
       .fgcolor(Theme::y_axis) .button(Y_VAL_POS, F(""), OPT_FLAT)
       .fgcolor(Theme::z_axis) .button(Z_VAL_POS, F(""), OPT_FLAT)
       .font(Theme::font_small)
                               .text  ( X_LBL_POS, GET_TEXT_F(MSG_AXIS_X))
                               .text  ( Y_LBL_POS, GET_TEXT_F(MSG_AXIS_Y))
                               .text  ( Z_LBL_POS, GET_TEXT_F(MSG_AXIS_Z))
       .colors(normal_btn);
  }

  if (what & FOREGROUND) {
    using namespace ExtUI;
    char x_str[15];
    char y_str[15];
    char z_str[15];

    if (isAxisPositionKnown(X))
      format_position(x_str, getAxisPosition_mm(X));
    else
      strcpy_P(x_str, PSTR("?"));

    if (isAxisPositionKnown(Y))
      format_position(y_str, getAxisPosition_mm(Y));
    else
      strcpy_P(y_str, PSTR("?"));

    if (isAxisPositionKnown(Z))
      format_position(z_str, getAxisPosition_mm(Z), 2);
    else
      strcpy_P(z_str, PSTR("?"));

    cmd.tag(6)
       .font(Theme::font_medium)
       .text(X_VAL_POS, x_str)
       .text(Y_VAL_POS, y_str)
       .text(Z_VAL_POS, z_str);
  }

  #undef GRID_COLS
}

#if ENABLED(TOUCH_UI_PORTRAIT)
  #define GRID_COLS 8
#else
  #define GRID_COLS 12
#endif

void StatusScreen::draw_temperature(draw_mode_t what) {
  using namespace Theme;

  #define TEMP_RECT_1 BTN_POS(1,1), BTN_SIZE(4,4)
  #define TEMP_RECT_2 BTN_POS(1,1), BTN_SIZE(8,2)
  #define NOZ_1_POS   BTN_POS(1,1), BTN_SIZE(4,2)
  #define NOZ_2_POS   BTN_POS(5,1), BTN_SIZE(4,2)
  #define BED_POS     BTN_POS(1,3), BTN_SIZE(4,2)
  #define FAN_POS     BTN_POS(5,3), BTN_SIZE(4,2)

  #define _ICON_POS(x,y,w,h) x, y, w/4, h
  #define _TEXT_POS(x,y,w,h) x + w/4, y, w - w/4, h
  #define ICON_POS(pos) _ICON_POS(pos)
  #define TEXT_POS(pos) _TEXT_POS(pos)

  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.font(Theme::font_small)
       .tag(5)
       .fgcolor(temp)     .button(TEMP_RECT_1, F(""), OPT_FLAT)
                          .button(TEMP_RECT_2, F(""), OPT_FLAT)
       .fgcolor(fan_speed).button(FAN_POS,     F(""), OPT_FLAT)
       .tag(0);

    // Draw Extruder Bitmap on Extruder Temperature Button

    cmd.tag(5)
       .cmd (BITMAP_SOURCE(Extruder_Icon_Info))
       .cmd (BITMAP_LAYOUT(Extruder_Icon_Info))
       .cmd (BITMAP_SIZE  (Extruder_Icon_Info))
       .icon(ICON_POS(NOZ_1_POS), Extruder_Icon_Info, icon_scale)
       .icon(ICON_POS(NOZ_2_POS), Extruder_Icon_Info, icon_scale);

    // Draw Bed Heat Bitmap on Bed Heat Button
    cmd.cmd (BITMAP_SOURCE(Bed_Heat_Icon_Info))
       .cmd (BITMAP_LAYOUT(Bed_Heat_Icon_Info))
       .cmd (BITMAP_SIZE  (Bed_Heat_Icon_Info))
       .icon(ICON_POS(BED_POS), Bed_Heat_Icon_Info, icon_scale);

    // Draw Fan Percent Bitmap on Bed Heat Button

    cmd.cmd (BITMAP_SOURCE(Fan_Icon_Info))
       .cmd (BITMAP_LAYOUT(Fan_Icon_Info))
       .cmd (BITMAP_SIZE  (Fan_Icon_Info))
       .icon(ICON_POS(FAN_POS), Fan_Icon_Info, icon_scale);

    TERN_(TOUCH_UI_USE_UTF8, load_utf8_bitmaps(cmd)); // Restore font bitmap handles
  }

  if (what & FOREGROUND) {
    using namespace ExtUI;
    char e0_str[20], e1_str[20], bed_str[20], fan_str[20];

    sprintf_P(fan_str, PSTR("%-3d %%"), int8_t(getActualFan_percent(FAN0)));

    if (isHeaterIdle(BED))
      format_temp_and_idle(bed_str, getActualTemp_celsius(BED));
    else
      format_temp_and_temp(bed_str, getActualTemp_celsius(BED), getTargetTemp_celsius(BED));

    if (isHeaterIdle(H0))
      format_temp_and_idle(e0_str, getActualTemp_celsius(H0));
    else
      format_temp_and_temp(e0_str, getActualTemp_celsius(H0), getTargetTemp_celsius(H0));

    #if HAS_MULTI_EXTRUDER
      if (isHeaterIdle(H1))
        format_temp_and_idle(e1_str, getActualTemp_celsius(H1));
      else
        format_temp_and_temp(e1_str, getActualTemp_celsius(H1), getTargetTemp_celsius(H1));
    #else
      strcpy_P(e1_str, PSTR("-"));
    #endif

    cmd.tag(5)
       .font(font_medium)
       .text(TEXT_POS(NOZ_1_POS), e0_str)
       .text(TEXT_POS(NOZ_2_POS), e1_str)
       .text(TEXT_POS(BED_POS), bed_str)
       .text(TEXT_POS(FAN_POS), fan_str);
  }
}

void StatusScreen::_format_time(char *outstr, uint32_t time) {
  const uint8_t hrs = time / 3600,
                min = (time / 60) % 60,
                sec = time % 60;
  if (hrs)
    sprintf_P(outstr, PSTR("%02d:%02d"), hrs, min);
  else
    sprintf_P(outstr, PSTR("%02d:%02ds"), min, sec);
}

void StatusScreen::draw_progress(draw_mode_t what) {
  using namespace ExtUI;
  using namespace Theme;

  CommandProcessor cmd;

  #undef GRID_COLS
  #if ENABLED(TOUCH_UI_PORTRAIT)
    #define GRID_COLS 3
    #define PROGRESSZONE_POS BTN_POS(1,5), BTN_SIZE(3,2)
    #define TIME_POS_X       BTN_X(1)
    #define TIME_POS_W       BTN_W(1)
    #define REMAINING_POS_X  BTN_X(2)
    #define REMAINING_POS_W  BTN_W(1)
    #define PROGRESS_POS_X   BTN_X(3)
    #define PROGRESS_POS_W   BTN_W(1)
    #define PROGRESSZONE_FIRSTLINE_Y BTN_Y(5)
    #define PROGRESSBAR_POS  BTN_POS(1,6), BTN_SIZE(3,1)
  #else
    #define GRID_COLS 6
    #define PROGRESSZONE_POS BTN_POS(5,1), BTN_SIZE(2,4)
    #if ENABLED(SHOW_REMAINING_TIME)
      #define TIME_POS       BTN_POS(5,1), BTN_SIZE(1,2)
      #define REMAINING_POS  BTN_POS(6,1), BTN_SIZE(1,2)
    #else
      #define TIME_POS       BTN_POS(5,1), BTN_SIZE(2,2)
    #endif
    #define PROGRESS_POS     BTN_POS(5,3), BTN_SIZE(2,2)
    #define PROGRESSBAR_POS  BTN_POS(5,2), BTN_SIZE(2,2)
  #endif

  if (what & BACKGROUND) {
    cmd.tag(0).font(font_medium)
       .fgcolor(progress).button(PROGRESSZONE_POS, F(""), OPT_FLAT);
  }

  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    char elapsed_str[10];
    _format_time(elapsed_str, elapsed);

    #if ENABLED(SHOW_REMAINING_TIME)
      const uint32_t remaining = getProgress_seconds_remaining();
      char remaining_str[10];
      _format_time(remaining_str, remaining);
    #endif

    const uint16_t current_progress = TERN(HAS_PRINT_PROGRESS_PERMYRIAD, getProgress_permyriad(), getProgress_percent() * 100);
    constexpr uint16_t progress_range = 10000U;

    const bool show_progress_bar = current_progress > 0 && current_progress < progress_range + 1;
    if (show_progress_bar) {
      cmd.tag(0).font(font_medium)
        .bgcolor(progress)
        .progress(PROGRESSBAR_POS, current_progress, progress_range, OPT_FLAT);
    }

    char progress_str[10];
    sprintf_P(progress_str,
      #if ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
        PSTR("%3d.%02d%%"), uint8_t(current_progress / 100), current_progress % 100
      #else
        PSTR("%3d%%"), uint8_t(current_progress / 100)
      #endif
    );

    #if ENABLED(TOUCH_UI_PORTRAIT)
      const uint16_t texts_pos_h = show_progress_bar ? (BTN_H(1)) : (BTN_H(2));
      cmd.font(font_medium)
         .tag(7).text(TIME_POS_X, PROGRESSZONE_FIRSTLINE_Y, TIME_POS_W, texts_pos_h, elapsed_str)
         #if ENABLED(SHOW_REMAINING_TIME)
           .text(REMAINING_POS_X, PROGRESSZONE_FIRSTLINE_Y, REMAINING_POS_W, texts_pos_h, remaining_str)
         #endif
         .text(PROGRESS_POS_X, PROGRESSZONE_FIRSTLINE_Y, PROGRESS_POS_W, texts_pos_h, progress_str);
    #else
      cmd.font(font_medium)
         .tag(7).text(TIME_POS, elapsed_str)
         #if ENABLED(SHOW_REMAINING_TIME)
           .text(REMAINING_POS, remaining_str)
         #endif
         .text(PROGRESS_POS, progress_str);
    #endif
  }

  #undef GRID_COLS
}

void StatusScreen::draw_interaction_buttons(draw_mode_t what) {
  #define GRID_COLS 4
  if (what & FOREGROUND) {
    using namespace ExtUI;

  #if ENABLED(TOUCH_UI_PORTRAIT)
    #define MEDIA_BTN_POS  BTN_POS(1,15), BTN_SIZE(2,2)
    #define MENU_BTN_POS   BTN_POS(3,15), BTN_SIZE(2,2)
  #else
    #define MEDIA_BTN_POS  BTN_POS(1,13), BTN_SIZE(2,4)
    #define MENU_BTN_POS   BTN_POS(3,13), BTN_SIZE(2,4)
  #endif

    const bool has_media = isMediaInserted() && !isPrintingFromMedia();

    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .colors(has_media ? action_btn : normal_btn)
       .enabled(has_media && !isPrinting())
       .tag(3).button(MEDIA_BTN_POS, isPrinting() ? GET_TEXT_F(MSG_PRINTING) : GET_TEXT_F(MSG_BUTTON_MEDIA))
       .colors(!has_media ? action_btn : normal_btn)
       .tag(4).button(MENU_BTN_POS, GET_TEXT_F(MSG_BUTTON_MENU));
  }
  #undef  GRID_COLS
}

void StatusScreen::draw_status_message(draw_mode_t what, const char *message) {
  #define GRID_COLS 1

  #if ENABLED(TOUCH_UI_PORTRAIT)
    #define STATUS_POS  BTN_POS(1,7), BTN_SIZE(1,2)
  #else
    #define STATUS_POS  BTN_POS(1,5), BTN_SIZE(1,4)
  #endif

  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::status_msg)
       .tag(0)
       .button(STATUS_POS, F(""), OPT_FLAT);

    draw_text_box(cmd, STATUS_POS, message, OPT_CENTER, font_large);
  }
  #undef  GRID_COLS
}

void StatusScreen::setStatusMessage(progmem_str message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage((const char *) buff);
}

void StatusScreen::setStatusMessage(const char *message) {
  if (CommandProcessor::is_processing()) {
    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHO_MSG("Cannot update status message, command processor busy");
    #endif
    return;
  }

  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::bg_color))
     .cmd(CLEAR(true,true,true));

  draw_temperature(BACKGROUND);
  draw_status_message(BACKGROUND, message);
  draw_interaction_buttons(BACKGROUND);
  draw_progress(BACKGROUND);
  draw_axis_position(BACKGROUND);

  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_MSG("New status message: ", message);
  #endif

  if (AT_SCREEN(StatusScreen)) {
    current_screen.onRefresh();
  }
}

void StatusScreen::loadBitmaps() {
  // Load the bitmaps for the status screen
  using namespace Theme;
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_pgm(base + TD_Icon_Info.RAMG_offset,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_pgm(base + Extruder_Icon_Info.RAMG_offset, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_pgm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_pgm(base + Fan_Icon_Info.RAMG_offset,      Fan_Icon,      sizeof(Fan_Icon));

  // Load fonts for internationalization
  #if ENABLED(TOUCH_UI_USE_UTF8)
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void StatusScreen::onStartup() {
  UIFlashStorage::initialize();
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    draw_temperature(FOREGROUND);
    draw_progress(FOREGROUND);
    draw_axis_position(FOREGROUND);
    draw_interaction_buttons(FOREGROUND);
  }
}

void StatusScreen::onEntry() {
  BaseScreen::onEntry();
  onRefresh();
}

void StatusScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch (tag) {
    #if ENABLED(SDSUPPORT)
      case 3: GOTO_SCREEN(FilesScreen); break;
    #endif
    case 4:
      if (isPrinting()) {
        GOTO_SCREEN(TuneMenu);
      }
      else {
        GOTO_SCREEN(MainMenu);
      }
      break;
    case 5:  GOTO_SCREEN(TemperatureScreen); break;
    case 6:
      if (isPrinting()) {
        #if ENABLED(BABYSTEPPING)
          GOTO_SCREEN(NudgeNozzleScreen);
        #elif HAS_BED_PROBE
          GOTO_SCREEN(ZOffsetScreen);
        #else
          return false;
        #endif
      }
      else {
        GOTO_SCREEN(MoveAxisScreen);
      }
      break;
    case 7:  GOTO_SCREEN(FeedratePercentScreen); break;
    default:
      return true;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
}

void StatusScreen::onMediaInserted() {
  if (AT_SCREEN(StatusScreen))
    setStatusMessage(GET_TEXT_F(MSG_MEDIA_INSERTED));
}

void StatusScreen::onMediaRemoved() {
  if (AT_SCREEN(StatusScreen) || ExtUI::isPrintingFromMedia())
    setStatusMessage(GET_TEXT_F(MSG_MEDIA_REMOVED));
}

#endif // FTDI_STATUS_SCREEN
