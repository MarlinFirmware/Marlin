/****************
 * ui_screens.h *
 ****************/

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

#ifndef _UI_SCREENS_
#define _UI_SCREENS_

#include "../ui_framework.h"

typedef const __FlashStringHelper *progmem_str;

/********************************* DL CACHE SLOTS ******************************/

// In order to reduce SPI traffic, we cache display lists (DL) in RAMG. This
// is done using the CLCD::DLCache class, which takes a unique ID for each
// cache location. These IDs are defined here:

enum {
  STATUS_SCREEN_CACHE,
  MENU_SCREEN_CACHE,
  TUNE_SCREEN_CACHE,
  DIALOG_BOX_CACHE,
  ADVANCED_SETTINGS_SCREEN_CACHE,
  MOVE_AXIS_SCREEN_CACHE,
  TEMPERATURE_SCREEN_CACHE,
  STEPS_SCREEN_CACHE,
  ZOFFSET_SCREEN_CACHE,
  FEEDRATE_SCREEN_CACHE,
  FILES_SCREEN_CACHE
};

// To save MCU RAM, the status message is "baked" in to the status screen
// cache, so we reserve a large chunk of memory for the DL cache

#define STATUS_SCREEN_DL_SIZE        2048
#define DIALOG_BOX_DL_SIZE           3072

/************************** REFRESH METHOD SHIMS ***************************/

class UncachedScreen {
  public:
    static void onRefresh(){
      CLCD::CommandFifo cmd;
      cmd.cmd(CMD_DLSTART);

      current_screen.onRedraw(BOTH);

      cmd.cmd(DL_DISPLAY);
      cmd.cmd(CMD_SWAP);
      cmd.execute();
    }
};

template<uint8_t DL_SLOT,uint32_t DL_SIZE = 0>
class CachedScreen {
  protected:
    static bool storeBackground(){
      CLCD::DLCache dlcache(DL_SLOT);
      dlcache.store(DL_SIZE);
    }

    static void repaintBackground(){
      CLCD::DLCache dlcache(DL_SLOT);
      CLCD::CommandFifo cmd;

      cmd.cmd(CMD_DLSTART);
      current_screen.onRedraw(BACKGROUND);

      dlcache.store(DL_SIZE);
    }

  public:
    static void onRefresh(){
      CLCD::DLCache dlcache(DL_SLOT);
      CLCD::CommandFifo cmd;

      cmd.cmd(CMD_DLSTART);

      if(dlcache.has_data()) {
        dlcache.append();
      } else {
        current_screen.onRedraw(BACKGROUND);
        dlcache.store(DL_SIZE);
      }

      current_screen.onRedraw(FOREGROUND);

      cmd.cmd(DL_DISPLAY);
      cmd.cmd(CMD_SWAP);
      cmd.execute();
    }
};

/************************* MENU SCREEN DECLARATIONS *************************/

class BootScreen : public UIScreen, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t what);
    static void onIdle();
};

class AboutScreen : public UIScreen, public UncachedScreen {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class KillScreen {
  // The KillScreen is behaves differently than the
  // others, so we do not bother extending UIScreen.
  public:
    static void show(progmem_str msg);
};

class DialogBoxBaseClass : public UIScreen, public CachedScreen<DIALOG_BOX_CACHE,DIALOG_BOX_DL_SIZE>  {
  protected:
    static void drawDialog(const progmem_str lines[], size_t n_lines, progmem_str btn1, progmem_str btn2);
    static void onRedraw(draw_mode_t what) {};
  public:
    static void onEntry();
    static bool onTouchStart(uint8_t tag);
};

class AlertBoxScreen : public DialogBoxBaseClass {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t what);
    static void show(const progmem_str line1, const progmem_str line2 = 0, const progmem_str line3 = 0);
};

class RestoreFailsafeScreen : public DialogBoxBaseClass {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class ConfirmAbortPrint : public DialogBoxBaseClass {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class StatusScreen : public UIScreen, public CachedScreen<STATUS_SCREEN_CACHE,STATUS_SCREEN_DL_SIZE> {
  private:
    static void static_axis_position();
    static void static_temperature();
    static void static_progress();
    static void static_media_button();
    static void static_interaction_buttons();
    static void static_status_message(const char * const message);

    static void dynamic_axis_position();
    static void dynamic_temperature();
    static void dynamic_progress();
    static void dynamic_interaction_buttons();

  public:
    static void setStatusMessage(const char * message);
    static void onRedraw(draw_mode_t what);
    static void onStartup();
    static void onEntry();
    static void onIdle();
    static bool onTouchStart(uint8_t tag);
};

class MenuScreen : public UIScreen, public CachedScreen<MENU_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class TuneScreen : public UIScreen, public CachedScreen<TUNE_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class CalibrationScreen : public UIScreen, public UncachedScreen {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t what);
    static void onIdle();
};

class CalibrationRegistersScreen : public UIScreen, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class AdvancedSettingsScreen : public UIScreen, public CachedScreen<ADVANCED_SETTINGS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

class ValueAdjusters : public UIScreen {
  protected:

    class widgets_t {
      private:
        draw_mode_t _what;
        uint8_t     _line;
        uint32_t    _color;
        uint8_t     _decimals;
        const char *_units;

      protected:
        void _draw_increment_btn(uint8_t line, const uint8_t tag);

      public:
        widgets_t(draw_mode_t what);
        void color(uint32_t color);
        void units(const char *units);
        void precision(uint8_t decimals);

        void heading(char *label);
        void adjuster(uint8_t tag, const char *label,float value=0);
        void increments();
    };

    static float getIncrement();

  public:
    static void onEntry();
    static bool onTouchStart(uint8_t tag);
};

class MoveAxisScreen : public ValueAdjusters, public CachedScreen<MOVE_AXIS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchHeld(uint8_t tag);
};

class StepsScreen : public ValueAdjusters, public CachedScreen<STEPS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchHeld(uint8_t tag);
};

class ZOffsetScreen : public ValueAdjusters, public CachedScreen<ZOFFSET_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchHeld(uint8_t tag);
};

class FeedrateScreen : public ValueAdjusters, public CachedScreen<FEEDRATE_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchHeld(uint8_t tag);
};

class TemperatureScreen : public ValueAdjusters, public CachedScreen<TEMPERATURE_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t what);
    static bool onTouchHeld(uint8_t tag);
};

class FilesScreen : public UIScreen, public CachedScreen<FILES_SCREEN_CACHE> {
  private:
    static const char *getSelectedShortFilename();
    static uint8_t getTagForIndex(uint16_t index);
    static uint16_t getIndexForTag(uint8_t tag);
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
};

//void lcd_init();
void lcd_update();
//bool lcd_hasstatus();
//void lcd_reset_status();
//void lcd_setstatus(const char * const message, const bool persist = false);
//void lcd_setstatusPGM(const char * const message, int8_t level = 0);
void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...);
//void lcd_setalertstatusPGM(const char * const message);
void lcd_buttons_update();
//void lcd_reset_alert_level();
//void lcd_refresh();

namespace Marlin_LCD_API {
  void onPrinterKilled(const char* lcd_msg);
  void onCardInserted();
  void onCardRemoved();
  void onPlayTone(const uint16_t frequency, const uint16_t duration);
}

#endif // _UI_SCREENS_