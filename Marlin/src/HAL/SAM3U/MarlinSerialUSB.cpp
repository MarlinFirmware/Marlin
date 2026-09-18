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
 * Marlin serial port over native USB (CDC) for SAM3U. See MarlinSerialUSB.h.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"

#if SAM3U_SERIAL_IN_USE(-1)

#include "MarlinSerialUSB.h"
#include "usb/usb_cdc.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

// The device is brought up in MarlinHAL::init(); the baud rate is meaningless
// over CDC, so this only has to exist.
void MarlinSerialUSB::begin(const long) {}

void MarlinSerialUSB::end() {}

int MarlinSerialUSB::peek() { return usb_cdc_peek(); }

int MarlinSerialUSB::read() {
  const int c = usb_cdc_read();

  #if ENABLED(EMERGENCY_PARSER)
    // The CDC receive path has no interrupt hook of its own that sees whole
    // lines, so feed the parser here. M108/M112/M410 still get through while
    // the planner is busy because reads happen from the idle task.
    if (c >= 0) {
      static EmergencyParser::State emergency_state;  // = EP_RESET
      emergency_parser.update(emergency_state, uint8_t(c));
    }
  #endif

  return c;
}

int MarlinSerialUSB::available() { return usb_cdc_available(); }

void MarlinSerialUSB::flush() { usb_cdc_flush(); }

size_t MarlinSerialUSB::write(const uint8_t c) {
  usb_cdc_write(c);
  return 1;
}

// Preinstantiate whichever slots asked for USB
#if SERIAL_PORT == -1
  MSerialUSB1 customizedSerial1(TERN0(EMERGENCY_PARSER, true));
#endif
#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 == -1
  MSerialUSB2 customizedSerial2(TERN0(EMERGENCY_PARSER, true));
#endif
#if defined(SERIAL_PORT_3) && SERIAL_PORT_3 == -1
  MSerialUSB3 customizedSerial3(TERN0(EMERGENCY_PARSER, true));
#endif

#endif // SAM3U_SERIAL_IN_USE(-1)
#endif // __SAM3U4E__
