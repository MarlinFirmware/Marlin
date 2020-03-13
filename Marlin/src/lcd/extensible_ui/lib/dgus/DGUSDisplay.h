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
#include "DGUSVPVariable.h"

enum DGUSLCD_Screens : uint8_t;

#define DEBUG_OUT ENABLED(DEBUG_DGUSLCD)
#include "../../../../core/debug_out.h"

typedef enum : uint8_t {
  DGUS_IDLE,           //< waiting for DGUS_HEADER1.
  DGUS_HEADER1_SEEN,   //< DGUS_HEADER1 received
  DGUS_HEADER2_SEEN,   //< DGUS_HEADER2 received
  DGUS_WAIT_TELEGRAM,  //< LEN received, Waiting for to receive all bytes.
} rx_datagram_state_t;

// Low-Level access to the display.
class DGUSDisplay {
public:

  DGUSDisplay() = default;

  static void InitDisplay();

  // Variable access.
  static void WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr=false);
  static void WriteVariablePGM(uint16_t adr, const void* values, uint8_t valueslen, bool isstr=false);
  template<typename T>
  static void WriteVariable(uint16_t adr, T value) {
    WriteVariable(adr, static_cast<const void*>(&value), sizeof(T));
  }

  // Until now I did not need to actively read from the display. That's why there is no ReadVariable
  // (I extensively use the auto upload of the display)

  // Force display into another screen.
  // (And trigger update of containing VPs)
  // (to implement a pop up message, which may not be nested)
  static void RequestScreen(DGUSLCD_Screens screen);

  // Periodic tasks, eg. Rx-Queue handling.
  static void loop();

public:
  // Helper for users of this class to estimate if an interaction would be blocking.
  static size_t GetFreeTxBuffer();

  // Checks two things: Can we confirm the presence of the display and has we initiliazed it.
  // (both boils down that the display answered to our chatting)
  static inline bool isInitialized() { return Initialized; }

private:
  static void WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen);
  static void WritePGM(const char str[], uint8_t len);
  static void ProcessRx();

  static rx_datagram_state_t rx_datagram_state;
  static uint8_t rx_datagram_len;
  static bool Initialized, no_reentrance;
};

extern DGUSDisplay dgusdisplay;

// compile-time x^y
constexpr float cpow(const float x, const int y) { return y == 0 ? 1.0 : x * cpow(x, y - 1); }

class DGUSScreenVariableHandler {
public:
  DGUSScreenVariableHandler() = default;

  static bool loop();

  /// Send all 4 strings that are displayed on the infoscreen, confirmation screen and kill screen
  /// The bools specifing whether the strings are in RAM or FLASH.
  static void sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  static void HandleUserConfirmationPopUp(uint16_t ConfirmVP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

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
    static void DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    /// File touched.
    static void DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr);
    /// start print after confirmation received.
    static void DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the pause, resume or abort button.
    static void DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User confirmed the abort action
    static void DGUSLCD_SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the tune button
    static void DGUSLCD_SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr);
    /// Send a single filename to the display.
    static void DGUSLCD_SD_SendFilename(DGUS_VP_Variable &var);
    /// Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    /// Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    /// Marlin informed us about a bad SD Card.
    static void SDCardError();
  #endif

  // OK Button the Confirm screen.
  static void ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr);

  // Update data after went to new screen (by display or by GotoScreen)
  // remember: store the last-displayed screen, so it can get returned to.
  // (e.g for pop up messages)
  static void UpdateNewScreen(DGUSLCD_Screens newscreen, bool popup=false);

  // Recall the remembered screen.
  static void PopToOldScreen();

  // Make the display show the screen and update all VPs in it.
  static void GotoScreen(DGUSLCD_Screens screen, bool ispopup = false);

  static void UpdateScreenVPData();

  // Helpers to convert and transfer data to the display.
  static void DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var);
  static void DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(PRINTCOUNTER)
    static void DGUSLCD_SendPrintAccTimeToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendPrintsTotalToDisplay(DGUS_VP_Variable &var);
  #endif
  #if FAN_COUNT > 0
    static void DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var);
  #endif
  static void DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(DGUS_UI_WAITING)
    static void DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var);
  #endif

  /// Send a value from 0..100 to a variable with a range from 0..255
  static void DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr);

  template<typename T>
  static void DGUSLCD_SetValueDirectly(DGUS_VP_Variable &var, void *val_ptr) {
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
  static void DGUSLCD_SendFloatAsLongValueToDisplay(DGUS_VP_Variable &var) {
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
      dgusdisplay.WriteVariable(var.VP, tmp, 4);
    }
  }

  /// Send a float value to the display.
  /// Display will get a 2-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsIntValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      DEBUG_ECHOLNPAIR_F(" >> ", f, 6);
      f *= cpow(10, decimals);
      union { int16_t i; char lb[2]; } endian;

      char tmp[2];
      endian.i = f;
      tmp[0] = endian.lb[1];
      tmp[1] = endian.lb[0];
      dgusdisplay.WriteVariable(var.VP, tmp, 2);
    }
  }

  /// Force an update of all VP on the current screen.
  static inline void ForceCompleteUpdate() { update_ptr = 0; ScreenComplete = false; }
  /// Has all VPs sent to the screen
  static inline bool IsScreenComplete() { return ScreenComplete; }

  static inline DGUSLCD_Screens getCurrentScreen() { return current_screen; }

  static inline void SetupConfirmAction( void (*f)()) { confirm_action_cb = f; }

private:
  static DGUSLCD_Screens current_screen;  ///< currently on screen
  static constexpr uint8_t NUM_PAST_SCREENS = 4;
  static DGUSLCD_Screens past_screens[NUM_PAST_SCREENS]; ///< LIFO with past screens for the "back" button.

  static uint8_t update_ptr;    ///< Last sent entry in the VPList for the actual screen.
  static uint16_t skipVP;       ///< When updating the screen data, skip this one, because the user is interacting with it.
  static bool ScreenComplete;   ///< All VPs sent to screen?

  static uint16_t ConfirmVP;    ///< context for confirm screen (VP that will be emulated-sent on "OK").

  #if ENABLED(SDSUPPORT)
    static int16_t top_file;    ///< file on top of file chooser
    static int16_t file_to_print; ///< touched file to be confirmed
  #endif

  static void (*confirm_action_cb)();
};

extern DGUSScreenVariableHandler ScreenHandler;

/// Find the flash address of a DGUS_VP_Variable for the VP.
extern const DGUS_VP_Variable* DGUSLCD_FindVPVar(const uint16_t vp);

/// Helper to populate a DGUS_VP_Variable for a given VP. Return false if not found.
extern bool populate_VPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy);
