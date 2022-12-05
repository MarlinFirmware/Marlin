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

/**
 * e_parser.cpp - Intercept special commands directly in the serial stream
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)

#include "e_parser.h"

// Static data members
bool EmergencyParser::killed_by_M112, // = false
     EmergencyParser::quickstop_by_M410,
     #if ENABLED(SDSUPPORT)
       EmergencyParser::sd_abort_by_M524,
     #endif
     EmergencyParser::enabled;

#if ENABLED(HOST_PROMPT_SUPPORT)
  uint8_t EmergencyParser::M876_reason; // = 0
#endif

// Global instance
EmergencyParser emergency_parser;

#endif // EMERGENCY_PARSER
