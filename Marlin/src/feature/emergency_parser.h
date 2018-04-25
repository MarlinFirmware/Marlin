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
 * emergency_parser.h - Intercept special commands directly in the serial stream
 */

#ifndef _EMERGENCY_PARSER_H_
#define _EMERGENCY_PARSER_H_

class EmergencyParser {

  // Currently looking for: M108, M112, M410
  enum State : char {
    EP_RESET,
    EP_N,
    EP_M,
    EP_M1,
    EP_M10,
    EP_M108,
    EP_M11,
    EP_M112,
    EP_M4,
    EP_M41,
    EP_M410,
    EP_IGNORE // to '\n'
  };

public:

  static EmergencyParser::State state;
  static bool killed_by_M112;

  EmergencyParser() {}

  static void update(const uint8_t c);

};

extern EmergencyParser emergency_parser;

#endif // _EMERGENCY_PARSER_H_
