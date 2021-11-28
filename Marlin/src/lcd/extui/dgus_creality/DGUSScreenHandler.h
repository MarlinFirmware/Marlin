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

#include "DGUSDisplay.h"
#include "DGUSVPVariable.h"
#include "../../../module/motion.h"
#include "../../../inc/MarlinConfig.h"

#if HAS_COLOR_LEDS
  #include "../../../feature/leds/leds.h"

  #if ENABLED(PRINTER_EVENT_LEDS)
    #define HAS_COLOR_LEDS_PREFERENCES 1
  #endif
#endif

enum DGUSLCD_Screens : uint8_t;

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
  LEDColor LastLEDColor;
  #endif
};

class DGUSScreenHandler {
public:
  DGUSScreenHandler() = default;

  static bool loop();

  static void Init();
  static void DefaultSettings();
  static void LoadSettings(const char* buff);
  static void StoreSettings(char* buff);
  static void SetTouchScreenConfiguration();
  static void KillScreenCalled();

  static void OnPowerlossResume();

  static void RequestSaveSettings();

  /// Send all 4 strings that are displayed on the infoscreen, confirmation screen and kill screen
  /// The bools specifing whether the strings are in RAM or FLASH.
  static void sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  static void HandleUserConfirmationPopUp(uint16_t ConfirmVP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  static void HandleDevelopmentTestButton(DGUS_VP_Variable &var, void *val_ptr);

  /// "M117" Message -- msg is a RAM ptr.
  static void setstatusmessage(const char* msg);
  /// The same for messages from Flash
  static void setstatusmessagePGM(PGM_P const msg);

  // Callback for VP "Display wants to change screen on idle printer"
  static void ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "Screen has been changed"
  static void ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "All Heaters Off"
  static void HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change this temperature"
  static void HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleFanSpeedChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change Flowrate"
  static void HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for manual extrude.
  static void HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for motor lock and unlook
  static void HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(POWER_LOSS_RECOVERY)
    static void TogglePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);

    // Hook for power loss recovery.
    static void HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  // Version sanity check
  static void HandleScreenVersion(DGUS_VP_Variable &var, void *val_ptr);

  // Hook for settings
  static void HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleFadeHeight(DGUS_VP_Variable &var, void *val_ptr);

  // Hook for move to position
  static void HandlePositionChange(DGUS_VP_Variable &var, void *val_ptr);

  static void HandleToggleTouchScreenMute(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleToggleTouchScreenStandbySetting(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleTouchScreenBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleTouchScreenStandbyBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleTouchScreenStandbyTimeSetting(DGUS_VP_Variable &var, void *val_ptr);

  #if HAS_PROBE_SETTINGS
  static void HandleToggleProbeHeaters(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleToggleProbeTemperatureStabilization(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleToggleProbePreheatTemp(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  #if HAS_PID_HEATING
    // Hook for "Change this temperature PID para"
    static void HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for PID autotune
    static void HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
    // Hook for "Change probe offset z"
    template<int decimals>
    static void HandleZoffsetChange(DGUS_VP_Variable &var, void *val_ptr) {
      HandleLiveAdjustZ(var, val_ptr, cpow(10.f, decimals));
    }
  #endif

  #if HAS_MESH
    static void OnMeshLevelingStart();

    static void OnMeshLevelingUpdate(const int8_t x, const int8_t y, const float z);

    static void InitMeshValues();

    static void ResetMeshValues();

    static void UpdateMeshValue(const int8_t x, const int8_t y, const float z);

    static void HandleMeshPoint(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  // LED
  #if HAS_COLOR_LEDS
    static void HandleLED(DGUS_VP_Variable &var, void *val_ptr);
    static void SendLEDToDisplay(DGUS_VP_Variable &var);
  #endif

  // Hook for live z adjust action
  static void HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr, const_float_t scalingFactor);
  static float GetCurrentLifeAdjustZ();

  // Hook for heater control
  static void HandleHeaterControl(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(DGUS_PREHEAT_UI)
    // Hook for preheat
    static void HandlePreheat(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
    // Hook for filament load and unload filament option
    static void HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for filament load and unload
    static void HandleFilamentLoadUnload(DGUS_VP_Variable &var);
  #endif

  #if ENABLED(SDSUPPORT)
    // Callback for VP "Display wants to change screen when there is a SD card"
    static void ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr);
    /// Scroll buttons on the file listing screen.
    static void DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    /// File touched.
    static void DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr);
    /// start print after confirmation received.
    static void DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr);
    /// Send a single filename to the display.
    static void DGUSLCD_SD_SendFilename(DGUS_VP_Variable &var);
    /// Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    /// Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    /// Marlin informed us about a bad SD Card.
    static void SDCardError();

    static void SetPrintingFromHost();
  #endif

  static void HandleLEDToggle();

  static void HandleFanToggle();

  static void FilamentRunout();

  static void OnFactoryReset();

#if HAS_BUZZER
  static void Buzzer(const uint16_t frequency, const uint16_t duration);
#endif

  static void OnHomingStart();
  static void OnHomingComplete();
  static void OnPrintFinished();

  // OK Button the Confirm screen.
  static void ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr);

  // Update data after went to new screen (by display or by GotoScreen)
  // remember: store the last-displayed screen, so it can get returned to.
  // (e.g for pop up messages)
  static void UpdateNewScreen(DGUSLCD_Screens newscreen, bool save_current_screen=true);

  // Recall the remembered screen.
  static void PopToOldScreen();
  static void OnBackButton(DGUS_VP_Variable &var, void *val_ptr);

  // Make the display show the screen and update all VPs in it.
  static void GotoScreen(DGUSLCD_Screens screen, bool save_current_screen = true);

  static void UpdateScreenVPData();

  // Helpers to convert and transfer data to the display.
  static void DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendScrollingStringToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var);
  static void DGUSLCD_SendScrollingStringToDisplayPGM(DGUS_VP_Variable &var);
  static void DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeWithRemainingToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeRemainingToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(PRINTCOUNTER)
    static void DGUSLCD_SendPrintAccTimeToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendPrintsTotalToDisplay(DGUS_VP_Variable &var);
  #endif
  #if HAS_FAN
    static void DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendFanSpeedToDisplay(DGUS_VP_Variable &var);
  #endif
  static void DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(DGUS_UI_WAITING)
    static void DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var);
  #endif

  static void DGUSLCD_SendAboutFirmwareWebsite(DGUS_VP_Variable &var);
  static void DGUSLCD_SendAboutFirmwareVersion(DGUS_VP_Variable &var);
  static void DGUSLCD_SendAboutPrintSize(DGUS_VP_Variable &var);

  /// Send a value from 0..100 to a variable with a range from 0..255
  static void DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr);

  template<typename T>
  static void DGUSLCD_SetValueDirectly(DGUS_VP_Variable &var, void *val_ptr) {
    if (!var.memadr) return;
    union { unsigned char tmp[sizeof(T)]; T t; } x;
    unsigned char *ptr = (unsigned char*)val_ptr;
    LOOP_L_N(i, sizeof(T)) x.tmp[i] = ptr[sizeof(T) - i - 1];
    *(T*)var.memadr = x.t;
  }

  template<DGUSLCD_Screens TPage>
  static void DGUSLCD_NavigateToPage(DGUS_VP_Variable &var, void *val_ptr) {
    GotoScreen(TPage);
  }

  template<DGUSLCD_Screens TPage, typename Handler>
  static void DGUSLCD_NavigateToPage(DGUS_VP_Variable &var, void *val_ptr) {
    GotoScreen(TPage);
    Handler::Init();
  }

  /// Send a float value to the display.
  /// Display will get a 4-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsLongValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      double d = static_cast<double>(*(float *)var.memadr);
      d *= cpow(10, decimals);

      // Round - truncated values look like skipped numbers
      static_assert(sizeof(long) == 4, "Assuming long is 4 bytes");
      long roundedValue = static_cast<long>(round(d));
      dgusdisplay.WriteVariable(var.VP, roundedValue);
    }
  }

  // Receive a float from the display - Display will send a 2-byte integer scaled to the number of digits
  template<unsigned int decimals>
  static void DGUSLCD_SetFloatAsIntFromDisplay(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) {
      uint16_t value_raw = swap16(*(uint16_t*)val_ptr);

      float value = static_cast<float>(static_cast<double>(value_raw) /cpow(10, decimals));
      *(float *)var.memadr = value;
    }
  }

  // Receive a float from the display - Display will send a 4-byte integer scaled to the number of digits
  template<unsigned int decimals>
  static void DGUSLCD_SetFloatAsLongFromDisplay(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) {
      uint32_t value_raw = uInt32Value(*(uint32_t*)val_ptr);

      float value = static_cast<float>(static_cast<double>(value_raw) /cpow(10, decimals));
      *(float *)var.memadr = value;
    }
  }

   static void DGUSLCD_SendULongToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      // Round - truncated values look like skipped numbers
      long roundedValue = *(uint32_t *) var.memadr;
      dgusdisplay.WriteVariable(var.VP, roundedValue);
    }
  }

  static void DGUSLCD_ReceiveULongFromDisplay(DGUS_VP_Variable &var, void* val_ptr) {
    if (var.memadr) {
      // Round - truncated values look like skipped numbers
      uint32_t incomingValue = *(uint32_t *) val_ptr;

      *(uint32_t*)var.memadr = uInt32Value(incomingValue); 
    }
  }

  // Toggle a boolean at the specified memory address
  static void DGUSLCD_ToggleBoolean(DGUS_VP_Variable &var, void *val_ptr) {
    if (var.memadr) {
      SERIAL_ECHOLNPAIR("Toggle boolean - ", var.VP);

      bool* val = (bool *)var.memadr;
      *val = !*val;
    }
  }

  // Send an icon to the display, depending on whether it is true or false
  template<unsigned int value_if_true, unsigned int value_if_false>
  static void DGUSLCD_SendIconValue(DGUS_VP_Variable &var) {
    if (var.memadr) {
      bool value = *(bool *)var.memadr;
      uint16_t valueToSend = value ? value_if_true : value_if_false;
      dgusdisplay.WriteVariable(var.VP, valueToSend);
    }
  }

  /// Send a float value to the display.
  /// Display will get a 2-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsIntValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      double d = static_cast<double>(*(float *)var.memadr);
      d *= cpow(10, decimals);

      // Round - truncated values look like skipped numbers
      int16_t roundedValue = static_cast<int16_t>(round(d));
      dgusdisplay.WriteVariable(var.VP, roundedValue);
    }
  }

  // Send the current Z-offset to the display, scaled to a number of digits
  template<unsigned int decimals>
  static void DGUSLCD_SendZOffsetToDisplay(DGUS_VP_Variable &var) {
    float currentOffset = GetCurrentLifeAdjustZ() * cpow(10.f, decimals);

    // Round - truncated values look like skipped numbers
    int16_t roundedValue = static_cast<int16_t>(round(currentOffset));
    dgusdisplay.WriteVariable(var.VP, roundedValue);
  }

  template<AxisEnum Axis>
  static void SendAxisTrustValue(DGUS_VP_Variable &var) {
    bool trust = axis_is_trusted(Axis);

    uint16_t color = trust ? 0xFFFF /*White*/ : 0XF800 /*Red*/;
    dgusdisplay.SetVariableDisplayColor(var.VP, color);
  }

  /// Force an update of all VP on the current screen.
  static inline void ForceCompleteUpdate() { update_ptr = 0; ScreenComplete = false; }
  /// Has all VPs sent to the screen
  static inline bool IsScreenComplete() { return ScreenComplete; }

  static inline DGUSLCD_Screens getCurrentScreen() { return current_screen; }

  static bool HandlePendingUserConfirmation();

  static void SetSynchronousOperationStart();
  static void SetSynchronousOperationFinish();
  static bool HasCurrentSynchronousOperation() { return HasSynchronousOperation; }
  static void SendBusyState(DGUS_VP_Variable &var);

  static void SetViewMeshLevelState();

  static bool fwretract_available;

private:
  static void HandleScreenVersionMismatchLEDFlash();

  static DGUSLCD_Screens current_screen;  ///< currently on screen
  static constexpr uint8_t NUM_PAST_SCREENS = 4;
  static DGUSLCD_Screens past_screens[NUM_PAST_SCREENS]; ///< LIFO with past screens for the "back" button.

  static uint8_t update_ptr;    ///< Last sent entry in the VPList for the actual screen.
  static uint16_t skipVP;       ///< When updating the screen data, skip this one, because the user is interacting with it.
  static bool ScreenComplete;   ///< All VPs sent to screen?

  static uint16_t ConfirmVP;    ///< context for confirm screen (VP that will be emulated-sent on "OK").

  static uint8_t MeshLevelIndex;
  static uint8_t MeshLevelIconIndex;
  static bool SaveSettingsRequested;
  static bool HasScreenVersionMismatch;
  static bool HasSynchronousOperation;

  #if ENABLED(SDSUPPORT)
    static int16_t top_file;    ///< file on top of file chooser
    static int16_t file_to_print; ///< touched file to be confirmed
  #endif

private:
  FORCE_INLINE static DGUSLCD_Screens GetPreviousScreen() {
    return past_screens[0];
  }

public: // Needed for VP auto-upload
  static bool HasRGBSettings;
  static creality_dwin_settings_t Settings;
};

extern DGUSScreenHandler ScreenHandler;

struct DGUSSynchronousOperation {
  private:
    bool is_running;

  public:
    DGUSSynchronousOperation() : is_running(false) {}

    // Don't allow this to be created on the stack
    void* operator new (std::size_t size) = delete;

    void start() { 
      is_running = true; 
      ScreenHandler.SetSynchronousOperationStart();
    }

    void done() {
      is_running = false; 
      ScreenHandler.SetSynchronousOperationFinish();
    }

    ~DGUSSynchronousOperation() { 
      if (is_running) { 
        ScreenHandler.SetSynchronousOperationFinish();
      }
    }
};
