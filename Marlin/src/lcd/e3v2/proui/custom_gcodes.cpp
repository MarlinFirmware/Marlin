/**
 * Custom G-code implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 1.1.0
 * Date: 2022/05/22
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../../../MarlinCore.h" // for wait_for_user
#include "../../../core/types.h"
#include "../../../gcode/gcode.h"
#include "../../../libs/buzzer.h"
#include "../../marlinui.h"

#include "dwin.h"
#include "dwinui.h"
#include "dwin_lcd.h"
#include "custom_gcodes.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

//=============================================================================
// Extended G-CODES 
//=============================================================================

void CError() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM(" This G-code is not implemented in firmware");
}

// Mark the G-code file as a Configuration file
void C10() {
  HMI_flag.config_flag = true;
  TERN_(POWER_LOSS_RECOVERY, if (card.isPrinting()) recovery.cancel());
  ui.reset_alert_level();
  if (checkkey == PrintProcess) Goto_Main_Menu();
}

// C11 Set color for UI element E
void C11() {
  const int16_t E = parser.seenval('E') ? parser.value_byte() : 0; // UI element
  if (E) {
    HMI_value.Color[0] = parser.seenval('R') ? parser.value_byte() : 0;
    HMI_value.Color[1] = parser.seenval('G') ? parser.value_byte() : 0;
    HMI_value.Color[2] = parser.seenval('B') ? parser.value_byte() : 0;
    DWIN_ApplyColor(E);
  } else DWIN_RedrawScreen();
}

// Cancel a Wait for User without an Emergecy Parser
void C108() { 
  #if DEBUG_DWIN
    SERIAL_ECHOLNPGM(F("wait_for_user was "), wait_for_user);
    SERIAL_ECHOLNPGM(F("checkkey was "), checkkey);
  #endif
  wait_for_user = false;
  DONE_BUZZ(true);
}

// lock/unlock screen
#if HAS_LOCKSCREEN
  void C510() {
    if (parser.seenval('U') && parser.value_int()) DWIN_UnLockScreen();
    else DWIN_LockScreen();
  }
#endif

#if DEBUG_DWIN
  #include "../../../module/planner.h"
  void C997() {
    #if ENABLED(POWER_LOSS_RECOVERY)
      if (IS_SD_PRINTING() && recovery.enabled) {
        planner.synchronize();
        recovery.save(true);
      }
    #endif
    DWIN_RebootScreen();
    SERIAL_ECHOLNPGM("Simulating a printer freeze");
    while (1) {};
  }
#endif

// Special Creality DWIN GCodes
void custom_gcode(const int16_t codenum) {
  switch(codenum) {
    case 10: C10(); break;             // Mark the G-code file as a Configuration file
    case 11: C11(); break;             // Set color for UI element E
    case 108: C108(); break;           // Cancel a Wait for User without an Emergecy Parser
    #if HAS_LOCKSCREEN
      case 510: C510(); break;         // lock screen
    #endif
    #if DEBUG_DWIN
      case 997: C997(); break;         // Simulate a printer freeze
    #endif
    #if ProUIex
      #if HAS_MESH
        case 29: ProEx.C29(); break;        // Set probing area and mesh leveling settings
      #endif
      case 100: ProEx.C100(); break;        // Change Physical minimums
      case 101: ProEx.C101(); break;        // Change Physical maximums
      case 102: ProEx.C102(); break;        // Change Bed size
      case 104: ProEx.C104(); break;        // Set extruder max temperature (limited by maxtemp in thermistor table)
      case 115: ProEx.C115(); break;        // ProUI Info
      #if ENABLED(NOZZLE_PARK_FEATURE)
        case 125: ProEx.C125(); break;      // Set park position
      #endif
      #if HAS_FILAMENT_SENSOR
        case 412: ProEx.C412(); break;      // Set runout sensor active mode
      #endif
      case 562: ProEx.C562(); break;        // Invert Extruder
      case 851: ProEx.C851(); break;        // If has a probe set z feed rate and multiprobe, if not, set manual z-offset
      #if HAS_TOOLBAR
        case 810: ProEx.C810(); break;      // Config toolbar
      #endif
    #endif
    default: CError(); break;
  }
}

void custom_gcode_report(const bool forReplay/*=true*/) {
  #if ProUIex
    #if HAS_MESH
      ProEx.C29_report(forReplay);
    #endif
    ProEx.C100_report(forReplay);
    ProEx.C101_report(forReplay);
    ProEx.C102_report(forReplay);
    ProEx.C104_report(forReplay);
    #if ENABLED(NOZZLE_PARK_FEATURE)
      ProEx.C125_report(forReplay);
    #endif
    #if HAS_FILAMENT_SENSOR
      ProEx.C412_report(forReplay);
    #endif
      ProEx.C562_report(forReplay);
    #if HAS_BED_PROBE
      ProEx.C851_report(forReplay);
    #endif
  #endif
}

#endif
