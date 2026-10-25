/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * lcd/extui/dgus/creality/DGUSScreenHandler.h
 *
 * Screen handler for the Creality CR-6 touch screen, ported from the
 * CR6Community firmware (DGUS implementation by Sebastiaan Dammann).
 */

#include "../DGUSScreenHandlerBase.h"
#include "../../../../module/motion.h"
#include "../../../../module/settings.h"

#if HAS_COLOR_LEDS
  #include "../../../../feature/leds/leds.h"
  #if ENABLED(PRINTER_EVENT_LEDS)
    #define HAS_COLOR_LEDS_PREFERENCES 1
  #endif
#endif

//#define DEBUG_DGUS_SCREEN_HANDLER
#define DEBUG_OUT ENABLED(DEBUG_DGUS_SCREEN_HANDLER)
#include "../../../../core/debug_out.h"

enum DGUS_ScreenID : uint8_t;

// Touch screen settings stored in the ExtUI area of the EEPROM
struct creality_dwin_settings_t {
  size_t settings_size;
  uint8_t settings_version;

  bool led_state;
  bool display_standby;
  bool display_sound;
  int16_t standby_screen_brightness;
  int16_t screen_brightness;
  int16_t standby_time_seconds;

  #if HAS_COLOR_LEDS_PREFERENCES
    LED1Color_t last_led_color;
  #endif
};

class DGUSScreenHandlerCreality : public DGUSScreenHandler {
public:
  DGUSScreenHandlerCreality() = default;

  static bool loop();

  // Touch screen settings
  static void defaultSettings();
  static void loadSettings(const char *buff);
  static void storeSettings(char *buff);
  static void requestSaveSettings() { // Saved after EEPROM_AUTOSAVE_AFTER, if enabled
    #ifdef EEPROM_AUTOSAVE_AFTER
      setStatusMessage(F("Saving settings - please wait"));
    #endif
    settings.request_autosave();
  }
  static void setTouchScreenConfiguration();

  static void killScreenCalled();

  #if ENABLED(POWER_LOSS_RECOVERY)
    static void onPowerLossResume();
    static void togglePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
    static void handlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  static void handleUserConfirmationPopUp(uint16_t confirmVP, PGM_P const line1, PGM_P const line2, PGM_P const line3, PGM_P const line4, bool l1inflash, bool l2inflash, bool l3inflash, bool l4inflash);

  static void handleDevelopmentTestButton(DGUS_VP_Variable &var, void *val_ptr);

  // Status messages use a static field for short messages and a scrolling field for long ones
  static void setStatusMessage(const char *msg, const bool forceScrolling=false);
  static void setStatusMessage_P(PGM_P const msg);
  static void setStatusMessage(FSTR_P const fmsg) { setStatusMessage_P(FTOP(fmsg)); }

  static void handleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr);
  #if HAS_FAN
    static void handleFanSpeedChanged(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  // Version sanity check
  static void handleScreenVersion(DGUS_VP_Variable &var, void *val_ptr);

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    static void handleFadeHeight(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  // Move to position
  static void handlePositionChange(DGUS_VP_Variable &var, void *val_ptr);

  // Values whose screen format differs from their storage
  static void handleMinSegmentTime(DGUS_VP_Variable &var, void *val_ptr);
  static void sendMinSegmentTime(DGUS_VP_Variable &var);
  #if ENABLED(LIN_ADVANCE)
    static void handleLinearAdvance(DGUS_VP_Variable &var, void *val_ptr);
    static void sendLinearAdvance(DGUS_VP_Variable &var);
  #endif

  static void handleToggleTouchScreenMute(DGUS_VP_Variable &var, void *val_ptr);
  static void handleToggleTouchScreenStandbySetting(DGUS_VP_Variable &var, void *val_ptr);
  static void handleTouchScreenBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr);
  static void handleTouchScreenStandbyBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr);
  static void handleTouchScreenStandbyTimeSetting(DGUS_VP_Variable &var, void *val_ptr);

  #if ANY(HAS_BED_PROBE, BABYSTEPPING)
    // Change probe offset z / babystep z
    template<int decimals>
    static void handleZoffsetChange(DGUS_VP_Variable &var, void *val_ptr) {
      handleLiveAdjustZ(var, val_ptr, cpow(10.0f, decimals));
    }
  #endif

  // Live Z adjust
  static void handleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr, const float scalingFactor);
  static float getCurrentLiveAdjustZ();

  #if HAS_MESH
    static void onLevelingStart();
    static void onLevelingUpdate(const int8_t x, const int8_t y, const float z);
    static void initMeshValues();
    static void resetMeshValues();
    static void updateMeshValue(const int8_t x, const int8_t y, const float z);
    static void handleMeshPoint(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  #if HAS_COLOR_LEDS
    static void handleLED(DGUS_VP_Variable &var, void *val_ptr);
    static void sendLEDToDisplay(DGUS_VP_Variable &var);
  #endif

  #if HAS_MEDIA
    static void sdScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    static void sdCardError();
    static void setPrintingFromHost();
  #endif

  static void handleLEDToggle();
  static void handleFanToggle();
  static void filamentRunout();
  static void onFactoryReset();

  #if HAS_SOUND
    static void buzzer(const uint16_t frequency, const uint16_t duration=0);
  #else
    static void buzzer(const uint16_t, const uint16_t=0) {}
  #endif

  static void onHomingStart();
  static void onHomingDone();
  static void onPrintDone();

  // Navigation is driven by the firmware. The display doesn't report its current screen.
  static void updateNewScreen(const DGUS_ScreenID screenID, const bool popup=false);
  static void popToOldScreen();
  static void onBackButton(DGUS_VP_Variable &var, void *val_ptr);
  static void gotoScreen(const DGUS_ScreenID screenID, const bool popup=false);
  static void updateScreenVPData();

  // Helpers to convert and transfer data to the display
  static void sendScrollingStringToDisplay(DGUS_VP_Variable &var);
  static void sendScrollingStringToDisplayPGM(DGUS_VP_Variable &var);
  static void sendPrintTimeToDisplay(DGUS_VP_Variable &var);
  static void sendPrintTimeWithRemainingToDisplay(DGUS_VP_Variable &var);
  static void sendPrintTimeRemainingToDisplay(DGUS_VP_Variable &var);
  #if HAS_FAN
    static void sendFanStatusToDisplay(DGUS_VP_Variable &var);
    static void sendFanSpeedToDisplay(DGUS_VP_Variable &var);
  #endif

  static void sendAboutFirmwareWebsite(DGUS_VP_Variable &var);
  static void sendAboutFirmwareVersion(DGUS_VP_Variable &var);
  static void sendAboutPrintSize(DGUS_VP_Variable &var);

  template<DGUS_ScreenID TPage>
  static void navigateToPage(DGUS_VP_Variable &var, void *val_ptr) {
    gotoScreen(TPage);
  }

  template<DGUS_ScreenID TPage, typename Handler>
  static void navigateToPage(DGUS_VP_Variable &var, void *val_ptr) {
    gotoScreen(TPage);
    Handler::init();
  }

  // Receive a float from the display as a 2-byte integer scaled to the number of digits
  template<unsigned int decimals>
  static void setFloatAsIntFromDisplay(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) *(float *)var.memadr = float(BE16_P(val_ptr)) / cpow(10, decimals);
  }

  // Receive a float from the display as a 4-byte integer scaled to the number of digits
  template<unsigned int decimals>
  static void setFloatAsLongFromDisplay(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) *(float *)var.memadr = float(uint32_t(BE32_P(val_ptr))) / cpow(10, decimals);
  }

  static void sendULongToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) dgus.writeVariable(var.VP, long(*(uint32_t *)var.memadr));
  }

  static void receiveULongFromDisplay(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) *(uint32_t *)var.memadr = BE32_P(val_ptr);
  }

  // Toggle a boolean at the specified memory address
  static void toggleBoolean(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) {
      bool * const val = (bool *)var.memadr;
      *val = !*val;
    }
  }

  // Send an icon to the display, depending on whether it is true or false
  template<unsigned int value_if_true, unsigned int value_if_false>
  static void sendIconValue(DGUS_VP_Variable &var) {
    if (var.memadr) {
      const bool value = *(bool *)var.memadr;
      const uint16_t valueToSend = value ? value_if_true : value_if_false;
      dgus.writeVariable(var.VP, valueToSend);
    }
  }

  // Send the current Z offset to the display, scaled to a number of digits
  template<unsigned int decimals>
  static void sendZOffsetToDisplay(DGUS_VP_Variable &var) {
    const float currentOffset = getCurrentLiveAdjustZ() * cpow(10.0f, decimals);
    dgus.writeVariable(var.VP, int16_t(LROUND(currentOffset)));
  }

  // Show untrusted axis positions in red
  template<AxisEnum Axis>
  static void sendAxisTrustValue(DGUS_VP_Variable &var) {
    const bool trust = motion.axis_is_trusted(Axis);
    const uint16_t color = trust ? 0xFFFF /*White*/ : 0xF800 /*Red*/;
    dgus.setVariableDisplayColor(var.VP, color);
  }

  static bool handlePendingUserConfirmation();

  // Synchronous operations (homing, leveling...) disable the back button and show a throbber
  static void setSynchronousOperationStart();
  static void setSynchronousOperationFinish();
  static bool hasCurrentSynchronousOperation() { return hasSynchronousOperation; }

  // A print job is running or paused. Unlike ExtUI::isPrinting(), this is not fooled by
  // injected commands or by a job timer started by a heating command (e.g., M109 for a filament change).
  static bool isPrintJobActive() { return ExtUI::isPrintingFromMedia() || printJobScreens; }
  static void setPrintJobScreens(const bool b) { printJobScreens = b; }
  static void sendBusyState(DGUS_VP_Variable &var);

  static void setViewMeshLevelState();

  // Call a function from loop() once all queued commands and moves are done
  typedef void (*queue_done_func_t)();
  static void runWhenQueueDone(const queue_done_func_t func) { queueDoneFunc = func; }

private:
  static void handleScreenVersionMismatchLEDFlash();

  static DGUS_ScreenID getPreviousScreen() { return past_screenIDs[0]; }

  static int8_t meshLevelIndex;
  static uint8_t meshLevelIconIndex;
  static bool hasScreenVersionMismatch;
  static bool hasSynchronousOperation;
  static bool printJobScreens;     // The print screens were shown for the current job timer run
  static queue_done_func_t queueDoneFunc;

public: // Needed for VP auto-upload
  static bool hasRGBSettings;
  static creality_dwin_settings_t lcd_settings;
};

typedef DGUSScreenHandlerCreality DGUSScreenHandlerClass;

// Mark an operation as busy while an instance is in scope, or between start() and done()
struct DGUSSynchronousOperation {
  private:
    bool is_running;

  public:
    DGUSSynchronousOperation() : is_running(false) {}

    // Don't allow this to be created on the heap
    void* operator new(size_t size) = delete;

    void start() {
      is_running = true;
      DGUSScreenHandlerCreality::setSynchronousOperationStart();
    }

    void done() {
      is_running = false;
      DGUSScreenHandlerCreality::setSynchronousOperationFinish();
    }

    ~DGUSSynchronousOperation() {
      if (is_running) DGUSScreenHandlerCreality::setSynchronousOperationFinish();
    }
};

#if ENABLED(POWER_LOSS_RECOVERY)
  #define PLR_SCREEN_RECOVER DGUS_SCREEN_SDPRINTMANIPULATION
  #define PLR_SCREEN_CANCEL DGUS_SCREEN_MAIN
#endif
