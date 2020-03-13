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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#include "../../../../MarlinCore.h"

#if HAS_BED_PROBE
  #include "../../../../module/probe.h"
#endif

#include "../../ui_api.h"

#include "DGUSVPVariable.h"
#include "DGUSDisplay.h"

// compile-time x^y
constexpr float cpow(const float x, const int y) { return y == 0 ? 1.0 : x * cpow(x, y - 1); }

class DGUSScreenHandler {
public:
  DGUSScreenHandler() = default;

  static void Init();
  static bool Loop();

  static void PrinterKilled(PGM_P error, PGM_P component);
  static void UserConfirmRequired(const char * const msg);

  #if ENABLED(SDSUPPORT)
    /// Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    /// Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    /// Marlin informed us about a bad SD Card.
    static void SDCardError();
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    static void PowerLossResume();
  #endif

  #if HAS_PID_HEATING
    static void PidTuning(const ExtUI::result_t rst);
  #endif

  /// Send all 4 strings that are displayed on the infoscreen, confirmation screen and kill screen
  /// The bools specifing whether the strings are in RAM or FLASH.
  static void SendInfoScreen(const char* l1, bool l1inflash, const char* l2, bool l2inflash, const char* l3, bool l3inflash, const char* l4, bool l4inflash);

  static void HandleUserConfirmationPopUp(uint16_t VP, const char* l1, bool l1inflash, const char* l2, bool l2inflash, const char* l3, bool l3inflash, const char* l4, bool l4inflash);

  /// "M117" Message -- msg is a RAM ptr.
  static void SetStatusMessage(const char* msg);
  /// The same for messages from Flash
  static void SetStatusMessagePGM(PGM_P const msg);
  // Callback for VP "Display wants to change screen on idle printer"
  static void ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "Screen has been changed"
  static void ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "All Heaters Off"
  static void HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change this temperature"
  static void HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change Flowrate"
  static void HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    // Hook for manual move option
    static void HandleManualMoveOption(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  // Hook for manual move.
  static void HandleManualMove(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for manual extrude.
  static void HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for motor lock and unlook
  static void HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(POWER_LOSS_RECOVERY)
    // Hook for power loss recovery.
    static void HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  // Hook for settings
  static void HandleSettings(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr);
  #if HAS_PID_HEATING
    // Hook for "Change this temperature PID para"
    static void HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for PID autotune
    static void HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if HAS_BED_PROBE
    // Hook for "Change probe offset z"
    static void HandleProbeOffsetZChanged(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if ENABLED(BABYSTEPPING)
    // Hook for live z adjust action
    static void HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if FAN_COUNT > 0
    // Hook for fan control
    static void HandleFanControl(DGUS_VP_Variable &var, void *val_ptr);
  #endif
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
    static void SD_ScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    /// File touched.
    static void SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr);
    /// start print after confirmation received.
    static void SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the pause, resume or abort button.
    static void SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User confirmed the abort action
    static void SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the tune button
    static void SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr);
    /// Send a single filename to the display.
    static void SD_SendFilename(DGUS_VP_Variable &var);
  #endif

  // OK Button the Confirm screen.
  static void ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr);

  // Update data after went to new screen (by display or by GotoScreen)
  // remember: store the last-displayed screen, so it can get returned to.
  // (e.g for pop up messages)
  static void UpdateNewScreen(DGUS_Screen newscreen, bool popup=false);

  // Recall the remembered screen.
  static void PopToOldScreen();

  // Make the display show the screen and update all VPs in it.
  static void GotoScreen(DGUS_Screen screen, bool ispopup = false);

  static void UpdateScreenVPData();

  // Helpers to convert and transfer data to the display.
  static void SendWordValueToDisplay(DGUS_VP_Variable &var);
  static void SendStringToDisplay(DGUS_VP_Variable &var);
  static void SendStringToDisplayPGM(DGUS_VP_Variable &var);
  static void SendTemperaturePID(DGUS_VP_Variable &var);
  static void SendPercentageToDisplay(DGUS_VP_Variable &var);
  static void SendPrintProgressToDisplay(DGUS_VP_Variable &var);
  static void SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(PRINTCOUNTER)
    static void SendPrintAccTimeToDisplay(DGUS_VP_Variable &var);
    static void SendPrintsTotalToDisplay(DGUS_VP_Variable &var);
  #endif
  #if FAN_COUNT > 0
    static void SendFanStatusToDisplay(DGUS_VP_Variable &var);
  #endif
  static void SendHeaterStatusToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(DGUS_UI_WAITING)
    static void SendWaitingStatusToDisplay(DGUS_VP_Variable &var);
  #endif

  /// Send a value from 0..100 to a variable with a range from 0..255
  static void PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr);

  template<typename T>
  static void SetValueDirectly(DGUS_VP_Variable &var, void *val_ptr) {
    if (!var.memadr) return;
    union { unsigned char tmp[sizeof(T)]; T t; } x;
    unsigned char *ptr = (unsigned char*)val_ptr;
    for (uint8_t i = 0; i < sizeof(T); i++) x.tmp[i] = ptr[sizeof(T) - i - 1];
    *(T*)var.memadr = x.t;
  }

  /// Send a float value to the display.
  /// Display will get a 4-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void SendFloatAsLongValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      f *= cpow(10, decimals);
      union { long l; char lb[4]; } endian;

      char tmp[4];
      endian.l = f;
      tmp[0] = endian.lb[3];
      tmp[1] = endian.lb[2];
      tmp[2] = endian.lb[1];
      tmp[3] = endian.lb[0];
      dgus_display.WriteVariable(var.VP, tmp, 4);
    }
  }

  /// Send a float value to the display.
  /// Display will get a 2-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void SendFloatAsIntValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      DEBUG_ECHOLNPAIR_F(" >> ", f, 6);
      f *= cpow(10, decimals);
      union { int16_t i; char lb[2]; } endian;

      char tmp[2];
      endian.i = f;
      tmp[0] = endian.lb[1];
      tmp[1] = endian.lb[0];
      dgus_display.WriteVariable(var.VP, tmp, 2);
    }
  }

  /// Force an update of all VP on the current screen.
  static inline void ForceCompleteUpdate() { update_index = 0; screen_complete = false; }
  /// Has all VPs sent to the screen
  static inline bool IsScreenComplete() { return screen_complete; }

  static inline DGUS_Screen GetCurrentScreen() { return current_screen; }

  static inline void SetupConfirmAction( void (*f)()) { confirm_action_cb = f; }

private:
  static DGUS_Screen current_screen;  ///< currently on screen
  static constexpr uint8_t NUM_PAST_SCREENS = 4;
  static DGUS_Screen past_screens[NUM_PAST_SCREENS]; ///< LIFO with past screens for the "back" button.

  static uint8_t update_index;    ///< Last sent entry in the VPList for the actual screen.
  static uint16_t skip_vp;       ///< When updating the screen data, skip this one, because the user is interacting with it.
  static bool screen_complete;   ///< All VPs sent to screen?

  static uint16_t confirm_vp;    ///< context for confirm screen (VP that will be emulated-sent on "OK").

  #if ENABLED(SDSUPPORT)
    static int16_t top_file;    ///< file on top of file chooser
    static int16_t file_to_print; ///< touched file to be confirmed
  #endif

  static void (*confirm_action_cb)();
};

extern DGUSScreenHandler dgus_screen_handler;
