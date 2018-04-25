/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * emergency_parser.cpp - Intercept special commands directly in the serial stream
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)

#include "emergency_parser.h"

extern volatile bool wait_for_user, wait_for_heatup;
void quickstop_stepper();

EmergencyParser::State EmergencyParser::state = EP_RESET;
bool EmergencyParser::killed_by_M112; // = false

EmergencyParser emergency_parser;

void EmergencyParser::update(const uint8_t c) {

  switch (state) {
    case EP_RESET:
      switch (c) {
        case ' ': break;
        case 'N': state = EP_N;      break;
        case 'M': state = EP_M;      break;
        default: state  = EP_IGNORE;
      }
      break;

    case EP_N:
      switch (c) {
        case '0': case '1': case '2':
        case '3': case '4': case '5':
        case '6': case '7': case '8':
        case '9': case '-': case ' ':   break;
        case 'M': state = EP_M;      break;
        default:  state = EP_IGNORE;
      }
      break;

    case EP_M:
      switch (c) {
        case ' ': break;
        case '1': state = EP_M1;     break;
        case '4': state = EP_M4;     break;
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

    case EP_M10:
      state = (c == '8') ? EP_M108 : EP_IGNORE;
      break;

    case EP_M11:
      state = (c == '2') ? EP_M112 : EP_IGNORE;
      break;

    case EP_M4:
      state = (c == '1') ? EP_M41 : EP_IGNORE;
      break;

    case EP_M41:
      state = (c == '0') ? EP_M410 : EP_IGNORE;
      break;

    case EP_IGNORE:
      if (c == '\n') state = EP_RESET;
      break;

    default:
      if (c == '\n') {
        switch (state) {
          case EP_M108:
            wait_for_user = wait_for_heatup = false;
            break;
          case EP_M112:
            killed_by_M112 = true;
            break;
          case EP_M410:
            quickstop_stepper();
            break;
          default:
            break;
        }
        state = EP_RESET;
      }
  }
}

#endif // EMERGENCY_PARSER
