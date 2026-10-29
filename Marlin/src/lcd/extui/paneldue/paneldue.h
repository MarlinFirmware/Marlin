/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/paneldue/paneldue.h
 *
 * PanelDue touch controller by Duet3D (PanelDueFirmware 3.2 and newer)
 *
 * The PanelDue talks to the printer as if it were running RepRapFirmware:
 *  - Each line is sent as "N<line> <command>*<checksum>" with a CRC-16 or XOR checksum.
 *  - The printer state is polled with M409 (Object Model queries) and replies are JSON.
 *  - Files are listed with M20 S2 and queried with M36. Paths have a "0:" drive prefix.
 *  - Some buttons send RepRapFirmware-specific G-code, which is translated here.
 *
 * All PanelDue traffic happens on LCD_SERIAL and is handled in the idle loop, so
 * the panel stays responsive while Marlin is busy (homing, heating, printing).
 */

#include "../../../inc/MarlinConfig.h"

class PanelDue {
public:
  static void startup();
  static void idle();

  // Messages and events to push to the panel
  static void status_message(const char * const msg);
  static void user_confirm(const char * const msg);
  static void pause_option(const bool show);
  static void beep(const uint16_t frequency, const uint16_t duration);
  static void media_changed();
  static void killed(FSTR_P const error, FSTR_P const component);
};

extern PanelDue paneldue;
