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
 * sd/SdFile.cpp
 *
 * Arduino SdFat Library
 * Copyright (c) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "SdFile.h"

/**
 *  Create a file object and open it in the current working directory.
 *
 * \param[in] path A path with a valid 8.3 DOS name for a file to be opened.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of open flags. see SdBaseFile::open(SdBaseFile*, const char*, uint8_t).
 */
SdFile::SdFile(const char *path, uint8_t oflag) : SdBaseFile(path, oflag) { }

/**
 * Write data to an open file.
 *
 * \note Data is moved to the cache but may not be written to the
 * storage device until sync() is called.
 *
 * \param[in] buf Pointer to the location of the data to be written.
 *
 * \param[in] nbyte Number of bytes to write.
 *
 * \return For success write() returns the number of bytes written, always
 * \a nbyte.  If an error occurs, write() returns -1.  Possible errors
 * include write() is called before a file has been opened, write is called
 * for a read-only file, device is full, a corrupt file system or an I/O error.
 */
int16_t SdFile::write(const void* buf, uint16_t nbyte) { return SdBaseFile::write(buf, nbyte); }

/**
 * Write a byte to a file. Required by the Arduino Print class.
 * \param[in] b the byte to be written.
 * Use writeError to check for errors.
 */
#if ARDUINO >= 100
  size_t SdFile::write(uint8_t b) { return SdBaseFile::write(&b, 1); }
#else
  void SdFile::write(uint8_t b) { SdBaseFile::write(&b, 1); }
#endif

/**
 * Write a string to a file. Used by the Arduino Print class.
 * \param[in] str Pointer to the string.
 * Use writeError to check for errors.
 */
void SdFile::write(const char *str) { SdBaseFile::write(str, strlen(str)); }

/**
 * Write a PROGMEM string to a file.
 * \param[in] str Pointer to the PROGMEM string.
 * Use writeError to check for errors.
 */
void SdFile::write_P(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) write(c);
}

/**
 * Write a PROGMEM string followed by CR/LF to a file.
 * \param[in] str Pointer to the PROGMEM string.
 * Use writeError to check for errors.
 */
void SdFile::writeln_P(PGM_P str) {
  write_P(str);
  write_P(PSTR("\r\n"));
}

#endif // SDSUPPORT
