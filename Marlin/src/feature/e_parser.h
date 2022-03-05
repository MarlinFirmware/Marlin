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

/**
 * e_parser.h - Intercept special commands directly in the serial stream
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "host_actions.h"
#endif

// External references
extern bool wait_for_user, wait_for_heatup;

#if ENABLED(REALTIME_REPORTING_COMMANDS)
  // From motion.h, which cannot be included here
  void report_current_position_moving();
  void quickpause_stepper();
  void quickresume_stepper();
#endif

#if ENABLED(SOFT_RESET_VIA_SERIAL)
  void HAL_reboot();
#endif

class EmergencyParser {

public:

  // Currently looking for: M108, M112, M410, M876 S[0-9], S000, P000, R000
  enum State : uint8_t {
    EP_RESET,
    EP_N,
    EP_M,
    EP_M1,
    EP_M10, EP_M108,
    EP_M11, EP_M112,
    EP_M4, EP_M41, EP_M410,
    #if ENABLED(HOST_PROMPT_SUPPORT)
      EP_M8, EP_M87, EP_M876, EP_M876S, EP_M876SN,
    #endif
    #if ENABLED(REALTIME_REPORTING_COMMANDS)
      EP_S, EP_S0, EP_S00, EP_GRBL_STATUS,
      EP_R, EP_R0, EP_R00, EP_GRBL_RESUME,
      EP_P, EP_P0, EP_P00, EP_GRBL_PAUSE,
    #endif
    #if ENABLED(SOFT_RESET_VIA_SERIAL)
      EP_ctrl,
      EP_K, EP_KI, EP_KIL, EP_KILL,
    #endif
    EP_IGNORE // to '\n'
  };

  static bool killed_by_M112;
  static bool quickstop_by_M410;

  #if ENABLED(HOST_PROMPT_SUPPORT)
    static uint8_t M876_reason;
  #endif

  EmergencyParser() { enable(); }

  FORCE_INLINE static void enable()  { enabled = true; }
  FORCE_INLINE static void disable() { enabled = false; }

  FORCE_INLINE static void update(State &state, const uint8_t c) {
    switch (state) {
      case EP_RESET:
        switch (c) {
          case ' ': case '\n': case '\r': break;
          case 'N': state = EP_N; break;
          case 'M': state = EP_M; break;
          #if ENABLED(REALTIME_REPORTING_COMMANDS)
            case 'S': state = EP_S; break;
            case 'P': state = EP_P; break;
            case 'R': state = EP_R; break;
          #endif
          #if ENABLED(SOFT_RESET_VIA_SERIAL)
            case '^': state = EP_ctrl; break;
            case 'K': state = EP_K; break;
          #endif
          default: state = EP_IGNORE;
        }
        break;

      case EP_N:
        switch (c) {
          case '0' ... '9':
          case '-': case ' ':     break;
          case 'M': state = EP_M; break;
          #if ENABLED(REALTIME_REPORTING_COMMANDS)
            case 'S': state = EP_S; break;
            case 'P': state = EP_P; break;
            case 'R': state = EP_R; break;
          #endif
          default: state = EP_IGNORE;
        }
        break;

      #if ENABLED(REALTIME_REPORTING_COMMANDS)
        case EP_S:   state = (c == '0') ? EP_S0          : EP_IGNORE; break;
        case EP_S0:  state = (c == '0') ? EP_S00         : EP_IGNORE; break;
        case EP_S00: state = (c == '0') ? EP_GRBL_STATUS : EP_IGNORE; break;

        case EP_R:   state = (c == '0') ? EP_R0          : EP_IGNORE; break;
        case EP_R0:  state = (c == '0') ? EP_R00         : EP_IGNORE; break;
        case EP_R00: state = (c == '0') ? EP_GRBL_RESUME : EP_IGNORE; break;

        case EP_P:   state = (c == '0') ? EP_P0          : EP_IGNORE; break;
        case EP_P0:  state = (c == '0') ? EP_P00         : EP_IGNORE; break;
        case EP_P00: state = (c == '0') ? EP_GRBL_PAUSE  : EP_IGNORE; break;
      #endif

      #if ENABLED(SOFT_RESET_VIA_SERIAL)
        case EP_ctrl: state = (c == 'X') ? EP_KILL : EP_IGNORE; break;
        case EP_K:    state = (c == 'I') ? EP_KI   : EP_IGNORE; break;
        case EP_KI:   state = (c == 'L') ? EP_KIL  : EP_IGNORE; break;
        case EP_KIL:  state = (c == 'L') ? EP_KILL : EP_IGNORE; break;
      #endif

      case EP_M:
        switch (c) {
          case ' ': break;
          case '1': state = EP_M1;     break;
          case '4': state = EP_M4;     break;
          #if ENABLED(HOST_PROMPT_SUPPORT)
            case '8': state = EP_M8;     break;
          #endif
          default: state  = EP_IGNORE;
        }
        break;

      case EP_M1:
        switch (c) {
          case '0': state = EP_M10;    break;
          case '1': state = EP_M11;    break;
          default: state  = EP_IGNORE;
        }
        break;

      case EP_M10: state = (c == '8') ? EP_M108 : EP_IGNORE; break;
      case EP_M11: state = (c == '2') ? EP_M112 : EP_IGNORE; break;
      case EP_M4:  state = (c == '1') ? EP_M41  : EP_IGNORE; break;
      case EP_M41: state = (c == '0') ? EP_M410 : EP_IGNORE; break;

      #if ENABLED(HOST_PROMPT_SUPPORT)

        case EP_M8:  state = (c == '7') ? EP_M87  : EP_IGNORE; break;
        case EP_M87: state = (c == '6') ? EP_M876 : EP_IGNORE; break;

        case EP_M876:
          switch (c) {
            case ' ': break;
            case 'S': state = EP_M876S; break;
            default: state = EP_IGNORE; break;
          }
          break;

        case EP_M876S:
          switch (c) {
            case ' ': break;
            case '0' ... '9':
              state = EP_M876SN;
              M876_reason = uint8_t(c - '0');
              break;
          }
          break;

      #endif

      case EP_IGNORE:
        if (ISEOL(c)) state = EP_RESET;
        break;

      default:
        if (ISEOL(c)) {
          if (enabled) switch (state) {
            case EP_M108: wait_for_user = wait_for_heatup = false; break;
            case EP_M112: killed_by_M112 = true; break;
            case EP_M410: quickstop_by_M410 = true; break;
            #if ENABLED(HOST_PROMPT_SUPPORT)
              case EP_M876SN: hostui.handle_response(M876_reason); break;
            #endif
            #if ENABLED(REALTIME_REPORTING_COMMANDS)
              case EP_GRBL_STATUS: report_current_position_moving(); break;
              case EP_GRBL_PAUSE: quickpause_stepper(); break;
              case EP_GRBL_RESUME: quickresume_stepper(); break;
            #endif
            #if ENABLED(SOFT_RESET_VIA_SERIAL)
              case EP_KILL: HAL_reboot(); break;
            #endif
            default: break;
          }
          state = EP_RESET;
        }
    }
  }

private:
  static bool enabled;
};

extern EmergencyParser emergency_parser;
