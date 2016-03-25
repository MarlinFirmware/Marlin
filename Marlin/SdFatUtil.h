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
 * Arduino SdFat Library
 * Copyright (C) 2008 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */
#include "Marlin.h"
#if ENABLED(SDSUPPORT)

#ifndef SdFatUtil_h
#define SdFatUtil_h
/**
 * \file
 * \brief Useful utility functions.
 */
#include "Marlin.h"
#include "MarlinSerial.h"
/** Store and print a string in flash memory.*/
#define PgmPrint(x) SerialPrint_P(PSTR(x))
/** Store and print a string in flash memory followed by a CR/LF.*/
#define PgmPrintln(x) SerialPrintln_P(PSTR(x))

namespace SdFatUtil {
  int FreeRam();
  void print_P(PGM_P str);
  void println_P(PGM_P str);
  void SerialPrint_P(PGM_P str);
  void SerialPrintln_P(PGM_P str);
}

using namespace SdFatUtil;  // NOLINT
#endif  //#define SdFatUtil_h


#endif
