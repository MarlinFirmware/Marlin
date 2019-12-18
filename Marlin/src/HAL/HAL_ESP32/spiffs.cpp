/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(WEBSUPPORT)

#include "../../core/serial.h"

#include <FS.h>
#include <SPIFFS.h>

bool spiffs_initialized;

void spiffs_init() {
  if (SPIFFS.begin(true))  // formatOnFail = true
    spiffs_initialized = true;
  else
    SERIAL_ERROR_MSG("SPIFFS mount failed");
}

#endif // WEBSUPPORT
#endif // ARDUINO_ARCH_ESP32
