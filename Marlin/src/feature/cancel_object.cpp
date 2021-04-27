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
#include "../inc/MarlinConfig.h"

#if ENABLED(CANCEL_OBJECTS)

#include "cancel_object.h"
#include "../gcode/gcode.h"
#include "../lcd/marlinui.h"

CancelObject cancelable;

int8_t CancelObject::object_count, // = 0
       CancelObject::active_object = -1;
uint32_t CancelObject::canceled; // = 0x0000
bool CancelObject::skipping; // = false

void CancelObject::set_active_object(const int8_t obj) {
  active_object = obj;
  if (WITHIN(obj, 0, 31)) {
    if (obj >= object_count) object_count = obj + 1;
    skipping = TEST(canceled, obj);
  }
  else
    skipping = false;

  #if HAS_STATUS_MESSAGE
    if (active_object >= 0)
      ui.status_printf_P(0, PSTR(S_FMT " %i"), GET_TEXT(MSG_PRINTING_OBJECT), int(active_object));
    else
      ui.reset_status();
  #endif
}

void CancelObject::cancel_object(const int8_t obj) {
  if (WITHIN(obj, 0, 31)) {
    SBI(canceled, obj);
    if (obj == active_object) skipping = true;
  }
}

void CancelObject::uncancel_object(const int8_t obj) {
  if (WITHIN(obj, 0, 31)) {
    CBI(canceled, obj);
    if (obj == active_object) skipping = false;
  }
}

void CancelObject::report() {
  if (active_object >= 0)
    SERIAL_ECHO_MSG("Active Object: ", active_object);

  if (canceled) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Canceled:");
    for (int i = 0; i < object_count; i++)
      if (TEST(canceled, i)) { SERIAL_CHAR(' '); SERIAL_ECHO(i); }
    SERIAL_EOL();
  }
}

#endif // CANCEL_OBJECTS
