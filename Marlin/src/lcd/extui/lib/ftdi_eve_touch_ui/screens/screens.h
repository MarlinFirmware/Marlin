/*************
 * screens.h *
 *************/

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

#pragma once

#include "../ftdi_eve_lib/ftdi_eve_lib.h"
#include "../language/language.h"
#include "../theme/theme.h"
#include "string_format.h"

#ifndef BED_LEVELING_COMMANDS
  #define BED_LEVELING_COMMANDS "G29"
#endif

extern tiny_timer_t refresh_timer;

/********************************* DL CACHE SLOTS ******************************/

// In order to reduce SPI traffic, we cache display lists (DL) in RAMG. This
// is done using the CLCD::DLCache class, which takes a unique ID for each
// cache location. These IDs are defined here:

enum {
  STATUS_SCREEN_CACHE,
  MENU_SCREEN_CACHE,
  TUNE_SCREEN_CACHE,
  ALERT_BOX_CACHE,
  SPINNER_CACHE,
  ADVANCED_SETTINGS_SCREEN_CACHE,
  MOVE_AXIS_SCREEN_CACHE,
  TEMPERATURE_SCREEN_CACHE,
  STEPS_SCREEN_CACHE,
  MAX_FEEDRATE_SCREEN_CACHE,
  MAX_VELOCITY_SCREEN_CACHE,
  MAX_ACCELERATION_SCREEN_CACHE,
  DEFAULT_ACCELERATION_SCREEN_CACHE,
  #if HAS_LEVELING
    LEVELING_SCREEN_CACHE,
    #if HAS_BED_PROBE
      ZOFFSET_SCREEN_CACHE,
    #endif
    #if HAS_MESH
      BED_MESH_SCREEN_CACHE,
    #endif
  #endif
  #if ENABLED(BABYSTEPPING)
    ADJUST_OFFSETS_SCREEN_CACHE,
  #endif
  #if HAS_TRINAMIC_CONFIG
    STEPPER_CURRENT_SCREEN_CACHE,
    STEPPER_BUMP_SENSITIVITY_SCREEN_CACHE,
  #endif
  #if HAS_MULTI_HOTEND
    NOZZLE_OFFSET_SCREEN_CACHE,
  #endif
  #if ENABLED(BACKLASH_GCODE)
    BACKLASH_COMPENSATION_SCREEN_CACHE,
  #endif
  #if HAS_JUNCTION_DEVIATION
    JUNC_DEV_SCREEN_CACHE,
  #else
    JERK_SCREEN_CACHE,
  #endif
  #if ENABLED(CASE_LIGHT_ENABLE)
    CASE_LIGHT_SCREEN_CACHE,
  #endif
  #if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
    FILAMENT_MENU_CACHE,
  #endif
  #if ENABLED(LIN_ADVANCE)
    LINEAR_ADVANCE_SCREEN_CACHE,
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    FILAMENT_RUNOUT_SCREEN_CACHE,
  #endif
  #if ENABLED(TOUCH_UI_LULZBOT_BIO)
    PRINTING_SCREEN_CACHE,
  #endif
  #if ENABLED(TOUCH_UI_COCOA_PRESS)
    PREHEAT_MENU_CACHE,
    PREHEAT_TIMER_SCREEN_CACHE,
    UNLOAD_CARTRIDGE_SCREEN_CACHE,
    LOAD_CHOCOLATE_SCREEN_CACHE,
    MOVE_XYZ_SCREEN_CACHE,
    MOVE_E_SCREEN_CACHE,
  #endif
  #if ENABLED(SDSUPPORT)
    FILES_SCREEN_CACHE,
  #endif
  #if ENABLED(CUSTOM_USER_MENUS)
    CUSTOM_USER_MENUS_SCREEN_CACHE,
  #endif
  CHANGE_FILAMENT_SCREEN_CACHE,
  INTERFACE_SETTINGS_SCREEN_CACHE,
  INTERFACE_SOUNDS_SCREEN_CACHE,
  LOCK_SCREEN_CACHE,
  DISPLAY_TIMINGS_SCREEN_CACHE
};

// To save MCU RAM, the status message is "baked" in to the status screen
// cache, so we reserve a large chunk of memory for the DL cache

#define STATUS_SCREEN_DL_SIZE        4096
#define ALERT_BOX_DL_SIZE            3072
#define SPINNER_DL_SIZE              3072
#define FILE_SCREEN_DL_SIZE          4160
#define PRINTING_SCREEN_DL_SIZE      2048

/************************* MENU SCREEN DECLARATIONS *************************/

class BaseScreen : public UIScreen {
  protected:
    #if LCD_TIMEOUT_TO_STATUS > 0
      static uint32_t last_interaction;
    #endif

    static bool buttonIsPressed(uint8_t tag);

  public:
    static bool buttonStyleCallback(CommandProcessor &, uint8_t, uint8_t &, uint16_t &, bool);

    static void reset_menu_timeout();

    static void onEntry();
    static void onIdle();
};

class BootScreen : public BaseScreen, public UncachedScreen {
  private:
    static void showSplashScreen();
  public:
    static void onRedraw(draw_mode_t);
    static void onIdle();
};

class AboutScreen : public BaseScreen, public UncachedScreen {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

#if ENABLED(PRINTCOUNTER)
  class StatisticsScreen : public BaseScreen, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };
#endif

class KillScreen {
  // The KillScreen is behaves differently than the
  // others, so we do not bother extending UIScreen.
  public:
    static void show(const char*);
};

class DialogBoxBaseClass : public BaseScreen {
  protected:
    template<typename T> static void drawMessage(const T, int16_t font = 0);
    static void drawYesNoButtons(uint8_t default_btn = 0);
    static void drawOkayButton();
    static void drawSpinner();
    static void drawButton(const progmem_str);

    static void onRedraw(draw_mode_t) {};
  public:
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};

class AlertDialogBox : public DialogBoxBaseClass, public CachedScreen<ALERT_BOX_CACHE,ALERT_BOX_DL_SIZE> {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    template<typename T> static void show(T);
    template<typename T> static void showError(T);
    static void hide();
};

class RestoreFailsafeDialogBox : public DialogBoxBaseClass, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

class SaveSettingsDialogBox : public DialogBoxBaseClass, public UncachedScreen {
  private:
    static bool needs_save;

  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);

    static void promptToSaveSettings();
    static void settingsChanged() {needs_save = true;}
};

class ConfirmStartPrintDialogBox : public DialogBoxBaseClass, public UncachedScreen {
  private:
    inline static const char *getShortFilename() {return getFilename(false);}
    inline static const char *getLongFilename()  {return getFilename(true);}

    static const char *getFilename(bool longName);
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t);

    static void show(uint8_t file_index);
};

class ConfirmAbortPrintDialogBox : public DialogBoxBaseClass, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

#if ENABLED(CALIBRATION_GCODE)
class ConfirmAutoCalibrationDialogBox : public DialogBoxBaseClass, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};
#endif

class ConfirmUserRequestAlertBox : public AlertDialogBox {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t);
    static void hide();
    static void show(const char*);
};

#if ENABLED(CUSTOM_USER_MENUS)
  class CustomUserMenus : public BaseScreen, public CachedScreen<CUSTOM_USER_MENUS_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };
#endif

class SpinnerDialogBox : public DialogBoxBaseClass, public CachedScreen<SPINNER_CACHE,SPINNER_DL_SIZE> {
  public:
    static void onRedraw(draw_mode_t);
    static void onIdle();

    static void show(const progmem_str);
    static void hide();
    static void enqueueAndWait_P(const progmem_str commands);
    static void enqueueAndWait_P(const progmem_str message, const progmem_str commands);
};

#if NONE(TOUCH_UI_LULZBOT_BIO, TOUCH_UI_COCOA_PRESS)
class StatusScreen : public BaseScreen, public CachedScreen<STATUS_SCREEN_CACHE,STATUS_SCREEN_DL_SIZE> {
  private:
    static void draw_axis_position(draw_mode_t);
    static void draw_temperature(draw_mode_t);
    static void draw_progress(draw_mode_t);
    static void draw_interaction_buttons(draw_mode_t);
    static void draw_status_message(draw_mode_t, const char * const);
    static void _format_time(char *outstr, uint32_t time);
  public:
    static void loadBitmaps();
    static void setStatusMessage(const char *);
    static void setStatusMessage(progmem_str);
    static void onRedraw(draw_mode_t);
    static void onStartup();
    static void onEntry();
    static void onIdle();
    static bool onTouchEnd(uint8_t tag);
};
#else
  class StatusScreen : public BaseScreen, public CachedScreen<STATUS_SCREEN_CACHE> {
    private:
      static float increment;
      static bool  jog_xy;
      static bool  fine_motion;

      static void draw_progress(draw_mode_t what);
      static void draw_temperature(draw_mode_t what);
      static void draw_syringe(draw_mode_t what);
      static void draw_arrows(draw_mode_t what);
      static void draw_overlay_icons(draw_mode_t what);
      static void draw_fine_motion(draw_mode_t what);
      static void draw_buttons(draw_mode_t what);
    public:
      static void loadBitmaps();
      static void unlockMotors();

      static void setStatusMessage(const char *);
      static void setStatusMessage(progmem_str);

      static void onRedraw(draw_mode_t);

      static bool onTouchStart(uint8_t tag);
      static bool onTouchHeld(uint8_t tag);
      static bool onTouchEnd(uint8_t tag);
      static void onIdle();

  };
#endif

#if ENABLED(TOUCH_UI_LULZBOT_BIO)
  class BioPrintingDialogBox : public BaseScreen, public CachedScreen<PRINTING_SCREEN_CACHE,PRINTING_SCREEN_DL_SIZE> {
    private:
      static void draw_status_message(draw_mode_t, const char * const);
      static void draw_progress(draw_mode_t);
      static void draw_time_remaining(draw_mode_t);
      static void draw_interaction_buttons(draw_mode_t);
    public:
      static void onRedraw(draw_mode_t);

      static void show();

      static void setStatusMessage(const char *);
      static void setStatusMessage(progmem_str);

      static void onIdle();
      static bool onTouchEnd(uint8_t tag);
  };

  class BioConfirmHomeXYZ : public DialogBoxBaseClass, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };

  class BioConfirmHomeE : public DialogBoxBaseClass, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };
#endif

class MainMenu : public BaseScreen, public CachedScreen<MENU_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

class TuneMenu : public BaseScreen, public CachedScreen<TUNE_SCREEN_CACHE> {
  private:
    static void pausePrint();
    static void resumePrint();
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

class TouchCalibrationScreen : public BaseScreen, public UncachedScreen {
  public:
    static void onRefresh();
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static void onIdle();
};

class TouchRegistersScreen : public BaseScreen, public UncachedScreen {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

class AdvancedSettingsMenu : public BaseScreen, public CachedScreen<ADVANCED_SETTINGS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

class ChangeFilamentScreen : public BaseScreen, public CachedScreen<CHANGE_FILAMENT_SCREEN_CACHE> {
  private:
    static uint8_t getSoftenTemp();
    static ExtUI::extruder_t getExtruder();
    static void drawTempGradient(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    static uint32_t getTempColor(uint32_t temp);
    static void doPurge();
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
    static void onIdle();
};

class BaseNumericAdjustmentScreen : public BaseScreen {
  public:
    enum precision_default_t {
      DEFAULT_LOWEST,
      DEFAULT_MIDRANGE,
      DEFAULT_HIGHEST
    };

  protected:
    class widgets_t {
      private:
        draw_mode_t _what;
        uint8_t     _line;
        uint32_t    _color;
        uint8_t     _decimals;
        progmem_str _units;
        enum style_t {
          BTN_NORMAL,
          BTN_ACTION,
          BTN_TOGGLE,
          BTN_DISABLED,
          TEXT_AREA,
          TEXT_LABEL
        } _style;

      protected:
        void _draw_increment_btn(CommandProcessor &, uint8_t line, const uint8_t tag);
        void _button(CommandProcessor &, uint8_t tag, int16_t x, int16_t y, int16_t w, int16_t h, progmem_str, bool enabled = true, bool highlight = false);
        void _button_style(CommandProcessor &cmd, style_t style);
      public:
        widgets_t(draw_mode_t);

        widgets_t &color(uint32_t color)       {_color = color; return *this;}
        widgets_t &units(progmem_str units)    {_units = units; return *this;}
        widgets_t &draw_mode(draw_mode_t what) {_what  = what;  return *this;}
        widgets_t &precision(uint8_t decimals, precision_default_t = DEFAULT_HIGHEST);

        void heading           (progmem_str label);
        void adjuster_sram_val (uint8_t tag,  progmem_str label, const char *value,  bool is_enabled = true);
        void adjuster          (uint8_t tag,  progmem_str label, const char *value,  bool is_enabled = true);
        void adjuster          (uint8_t tag,  progmem_str label, float value=0,      bool is_enabled = true);
        void button            (uint8_t tag,  progmem_str label,                     bool is_enabled = true);
        void text_field        (uint8_t tag,  progmem_str label, const char *value,  bool is_enabled = true);
        void two_buttons       (uint8_t tag1, progmem_str label1,
                                uint8_t tag2, progmem_str label2,                    bool is_enabled = true);
        void toggle            (uint8_t tag,  progmem_str label,                     bool value, bool is_enabled = true);
        void home_buttons      (uint8_t tag);
        void increments        ();
    };

    static float getIncrement();

  public:
    static void onEntry();
    static bool onTouchEnd(uint8_t tag);
};

class BaseMoveAxisScreen : public BaseNumericAdjustmentScreen {
  private:
    static float getManualFeedrate(uint8_t axis, float increment_mm);
  public:
    static void setManualFeedrate(ExtUI::axis_t, float increment_mm);
    static void setManualFeedrate(ExtUI::extruder_t, float increment_mm);

    static void onEntry();
    static bool onTouchHeld(uint8_t tag);
};

class MoveAxisScreen : public BaseMoveAxisScreen, public CachedScreen<MOVE_AXIS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static void onIdle();
};

class StepsScreen : public BaseNumericAdjustmentScreen, public CachedScreen<STEPS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

#if HAS_TRINAMIC_CONFIG
  class StepperCurrentScreen : public BaseNumericAdjustmentScreen, public CachedScreen<STEPPER_CURRENT_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };

  class StepperBumpSensitivityScreen : public BaseNumericAdjustmentScreen, public CachedScreen<STEPPER_BUMP_SENSITIVITY_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

#if HAS_MULTI_HOTEND
  class NozzleOffsetScreen : public BaseNumericAdjustmentScreen, public CachedScreen<NOZZLE_OFFSET_SCREEN_CACHE> {
    public:
      static void onEntry();
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

#if HAS_LEVELING

  class LevelingMenu : public BaseScreen, public CachedScreen<LEVELING_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };

  #if HAS_BED_PROBE
    class ZOffsetScreen : public BaseNumericAdjustmentScreen, public CachedScreen<ZOFFSET_SCREEN_CACHE> {
      public:
        static void onRedraw(draw_mode_t);
        static bool onTouchHeld(uint8_t tag);
    };
  #endif

  #if HAS_MESH

    class BedMeshScreen : public BaseScreen, public CachedScreen<BED_MESH_SCREEN_CACHE> {
      private:
        enum MeshOpts {
          USE_POINTS    = 0x01,
          USE_COLORS    = 0x02,
          USE_TAGS      = 0x04,
          USE_HIGHLIGHT = 0x08,
          USE_AUTOSCALE = 0x10
        };

        static uint8_t pointToTag(uint8_t x, uint8_t y);
        static bool tagToPoint(uint8_t tag, uint8_t &x, uint8_t &y);
        static float getHightlightedValue();
        static void drawHighlightedPointValue();
        static void drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, ExtUI::bed_mesh_t data, uint8_t opts, float autoscale_max = 0.1);

      public:
        static void onMeshUpdate(const int8_t x, const int8_t y, const float val);
        static void onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t);
        static void onEntry();
        static void onRedraw(draw_mode_t);
        static bool onTouchStart(uint8_t tag);
        static bool onTouchEnd(uint8_t tag);

        static void startMeshProbe();
    };

  #endif // HAS_MESH

#endif // HAS_LEVELING

#if ENABLED(BABYSTEPPING)
  class NudgeNozzleScreen : public BaseNumericAdjustmentScreen, public CachedScreen<ADJUST_OFFSETS_SCREEN_CACHE> {
    public:
      static void onEntry();
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
      static bool onTouchHeld(uint8_t tag);
      static void onIdle();
  };
#endif

#if ENABLED(BACKLASH_GCODE)
  class BacklashCompensationScreen : public BaseNumericAdjustmentScreen, public CachedScreen<BACKLASH_COMPENSATION_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

class FeedratePercentScreen : public BaseNumericAdjustmentScreen, public CachedScreen<MAX_FEEDRATE_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

class MaxVelocityScreen : public BaseNumericAdjustmentScreen, public CachedScreen<MAX_VELOCITY_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

class MaxAccelerationScreen : public BaseNumericAdjustmentScreen, public CachedScreen<MAX_ACCELERATION_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

class DefaultAccelerationScreen : public BaseNumericAdjustmentScreen, public CachedScreen<DEFAULT_ACCELERATION_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

#if HAS_JUNCTION_DEVIATION
  class JunctionDeviationScreen : public BaseNumericAdjustmentScreen, public CachedScreen<JUNC_DEV_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#else
  class JerkScreen : public BaseNumericAdjustmentScreen, public CachedScreen<JERK_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

#if ENABLED(CASE_LIGHT_ENABLE)
  class CaseLightScreen : public BaseNumericAdjustmentScreen, public CachedScreen<CASE_LIGHT_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

#if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
  class FilamentMenu : public BaseNumericAdjustmentScreen, public CachedScreen<FILAMENT_MENU_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  class FilamentRunoutScreen : public BaseNumericAdjustmentScreen, public CachedScreen<FILAMENT_RUNOUT_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

#if ENABLED(LIN_ADVANCE)
  class LinearAdvanceScreen : public BaseNumericAdjustmentScreen, public CachedScreen<LINEAR_ADVANCE_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchHeld(uint8_t tag);
  };
#endif

class TemperatureScreen : public BaseNumericAdjustmentScreen, public CachedScreen<TEMPERATURE_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

class InterfaceSoundsScreen : public BaseScreen, public CachedScreen<INTERFACE_SOUNDS_SCREEN_CACHE> {
  public:
    enum event_t {
      PRINTING_STARTED  = 0,
      PRINTING_FINISHED = 1,
      PRINTING_FAILED   = 2,

      NUM_EVENTS
    };

  private:
    friend class InterfaceSettingsScreen;

    static uint8_t event_sounds[NUM_EVENTS];

    static const char* getSoundSelection(event_t);
    static void toggleSoundSelection(event_t);
    static void setSoundSelection(event_t, const FTDI::SoundPlayer::sound_t*);

  public:
    static void playEventSound(event_t, FTDI::play_mode_t = FTDI::PLAY_ASYNCHRONOUS);

    static void defaultSettings();

    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};

class InterfaceSettingsScreen : public BaseScreen, public CachedScreen<INTERFACE_SETTINGS_SCREEN_CACHE> {
  private:
    struct persistent_data_t {
      uint32_t touch_transform_a;
      uint32_t touch_transform_b;
      uint32_t touch_transform_c;
      uint32_t touch_transform_d;
      uint32_t touch_transform_e;
      uint32_t touch_transform_f;
      uint16_t passcode;
      uint8_t  display_brightness;
      int8_t   display_h_offset_adj;
      int8_t   display_v_offset_adj;
      uint8_t  sound_volume;
      uint8_t  bit_flags;
      uint8_t  event_sounds[InterfaceSoundsScreen::NUM_EVENTS];
    };

  public:
    #ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
      static bool backupEEPROM();
    #endif

    static void saveSettings(char *);
    static void loadSettings(const char *);
    static void defaultSettings();
    static void failSafeSettings();

    static void onStartup();
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};

class LockScreen : public BaseScreen, public CachedScreen<LOCK_SCREEN_CACHE> {
  private:
    friend InterfaceSettingsScreen;

    static uint16_t passcode;

    static char & message_style();
    static uint16_t compute_checksum();
    static void onPasscodeEntered();
  public:
    static bool is_enabled();
    static void check_passcode();
    static void enable();
    static void disable();

    static void set_hash(uint16_t pass) {passcode = pass;};
    static uint16_t get_hash() {return passcode;};

    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};

#if ENABLED(SDSUPPORT)

  class FilesScreen : public BaseScreen, public CachedScreen<FILES_SCREEN_CACHE, FILE_SCREEN_DL_SIZE> {
    private:
      #if ENABLED(TOUCH_UI_PORTRAIT)
        static constexpr uint8_t header_h       = 2;
        static constexpr uint8_t footer_h       = 2;
        static constexpr uint8_t files_per_page = 11;
      #else
        static constexpr uint8_t header_h       = 1;
        static constexpr uint8_t footer_h       = 1;
        static constexpr uint8_t files_per_page = 6;
      #endif

      static uint8_t  getTagForLine(uint8_t line) {return line + 2;}
      static uint8_t  getLineForTag(uint8_t tag)  {return  tag - 2;}
      static uint16_t getFileForTag(uint8_t tag);
      static uint16_t getSelectedFileIndex();

      inline static const char *getSelectedShortFilename() {return getSelectedFilename(false);}
      inline static const char *getSelectedLongFilename()  {return getSelectedFilename(true);}
      static const char *getSelectedFilename(bool longName);

      static void drawFileButton(const char* filename, uint8_t tag, bool is_dir, bool is_highlighted);
      static void drawFileList();
      static void drawHeader();
      static void drawFooter();
      static void drawSelectedFile();

      static void gotoPage(uint8_t);
    public:
      static void onEntry();
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
      static void onIdle();
  };

#endif // SDSUPPORT

class EndstopStatesScreen : public BaseScreen, public UncachedScreen {
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};

class DisplayTuningScreen : public BaseNumericAdjustmentScreen, public CachedScreen<DISPLAY_TIMINGS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};

#if ENABLED(TOUCH_UI_DEVELOPER_MENU)

  class DeveloperMenu : public BaseScreen, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };

  class ConfirmEraseFlashDialogBox : public DialogBoxBaseClass, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };

  class WidgetsScreen : public BaseScreen, public UncachedScreen {
    public:
      static void onEntry();
      static void onRedraw(draw_mode_t);
      static bool onTouchStart(uint8_t tag);
      static void onIdle();
  };

  class StressTestScreen : public BaseScreen, public UncachedScreen {
    private:
      static void drawDots(uint16_t x, uint16_t y, uint16_t h, uint16_t v);
      static bool watchDogTestNow();
      static void recursiveLockup();
      static void iterativeLockup();
      static void runTestOnBootup(bool enable);

    public:
      static void startupCheck();

      static void onEntry();
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
      static void onIdle();
  };

#endif // TOUCH_UI_DEVELOPER_MENU

class MediaPlayerScreen : public BaseScreen, public UncachedScreen {
  private:
    typedef int16_t media_streamer_func_t(void *obj, void *buff, size_t bytes);

  public:
    static bool playCardMedia();
    static bool playBootMedia();

    static void onEntry();
    static void onRedraw(draw_mode_t);

    static void playStream(void *obj, media_streamer_func_t*);
};

#if NUM_LANGUAGES > 1
  class LanguageMenu : public BaseScreen, public UncachedScreen {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };
#endif

#if ENABLED(TOUCH_UI_COCOA_PRESS)

  class PreheatMenu : public BaseScreen, public CachedScreen<PREHEAT_MENU_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
  };

  class PreheatTimerScreen : public BaseScreen, public CachedScreen<PREHEAT_TIMER_SCREEN_CACHE> {
    private:
      static uint16_t secondsRemaining();

      static void draw_message(draw_mode_t);
      static void draw_time_remaining(draw_mode_t);
      static void draw_interaction_buttons(draw_mode_t);
      static void draw_adjuster(draw_mode_t, uint8_t tag, progmem_str label, float value, int16_t x, int16_t y, int16_t w, int16_t h);
    public:
      static void onRedraw(draw_mode_t);

      static void onEntry();
      static void onIdle();
      static bool onTouchHeld(uint8_t tag);
      static bool onTouchEnd(uint8_t tag);
  };

  class UnloadCartridgeScreen : public BaseScreen, public CachedScreen<UNLOAD_CARTRIDGE_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
      static bool onTouchHeld(uint8_t tag);
  };

  class LoadChocolateScreen : public BaseScreen, public CachedScreen<LOAD_CHOCOLATE_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static bool onTouchEnd(uint8_t tag);
      static bool onTouchHeld(uint8_t tag);
  };

  class MoveXYZScreen : public BaseMoveAxisScreen, public CachedScreen<MOVE_XYZ_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static void onIdle();
  };

  class MoveEScreen : public BaseMoveAxisScreen, public CachedScreen<MOVE_E_SCREEN_CACHE> {
    public:
      static void onRedraw(draw_mode_t);
      static void onIdle();
  };

#endif // TOUCH_UI_COCOA_PRESS
