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
#include "MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "SdFatUtil.h"
#include "serial.h"

/**
 * Amount of free RAM
 * \return The number of free bytes.
 */
#ifdef __arm__
extern "C" char* sbrk(int incr);
int SdFatUtil::FreeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else  // __arm__
extern char* __brkval;
extern char __bss_end;
/**
 * Amount of free RAM
 * \return The number of free bytes.
 */
int SdFatUtil::FreeRam() {
  char top;
  return __brkval ? &top - __brkval : &top - &__bss_end;
}
#endif  // __arm

/**
 * %Print a string in flash memory.
 *
 * \param[in] pr Print object for output.
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::print_P(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) MYSERIAL.write(c);
}

/**
 * %Print a string in flash memory followed by a CR/LF.
 *
 * \param[in] pr Print object for output.
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::println_P(PGM_P str) { print_P(str); MYSERIAL.println(); }

/**
 * %Print a string in flash memory to Serial.
 *
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::SerialPrint_P(PGM_P str) { print_P(str); }

/**
 * %Print a string in flash memory to Serial followed by a CR/LF.
 *
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::SerialPrintln_P(PGM_P str) { println_P(str); }

#endif // SDSUPPORT
