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
#ifdef ARDUINO_ARCH_SAM

/**
 * MarlinSerial_Due.cpp - Hardware serial library for Arduino DUE
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 * Based on MarlinSerial for AVR, copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 */

#include "../../inc/MarlinConfig.h"

#if HAS_USB_SERIAL

#include "MarlinSerialUSB.h"

// Imports from Atmel USB Stack/CDC implementation
extern "C" {
  bool usb_task_cdc_isenabled();
  bool usb_task_cdc_dtr_active();
  bool udi_cdc_is_rx_ready();
  int udi_cdc_getc();
  bool udi_cdc_is_tx_ready();
  int udi_cdc_putc(int value);
};

// Pending character
static int pending_char = -1;

// Public Methods
void MarlinSerialUSB::begin(const long) {}

void MarlinSerialUSB::end() {}

int MarlinSerialUSB::peek() {
  if (pending_char >= 0)
    return pending_char;

  // If USB CDC not enumerated or not configured on the PC side
  if (!usb_task_cdc_isenabled())
    return -1;

  // If no bytes sent from the PC
  if (!udi_cdc_is_rx_ready())
    return -1;

  pending_char = udi_cdc_getc();

  TERN_(EMERGENCY_PARSER, emergency_parser.update(static_cast<MSerialT1*>(this)->emergency_state, (char)pending_char));

  return pending_char;
}

int MarlinSerialUSB::read() {
  if (pending_char >= 0) {
    int ret = pending_char;
    pending_char = -1;
    return ret;
  }

  // If USB CDC not enumerated or not configured on the PC side
  if (!usb_task_cdc_isenabled())
    return -1;

  // If no bytes sent from the PC
  if (!udi_cdc_is_rx_ready())
    return -1;

  int c = udi_cdc_getc();

  TERN_(EMERGENCY_PARSER, emergency_parser.update(static_cast<MSerialT1*>(this)->emergency_state, (char)c));

  return c;
}

int MarlinSerialUSB::available() {
  if (pending_char > 0) return pending_char;
  return pending_char == 0 ||
    // or USB CDC enumerated and configured on the PC side and some bytes where sent to us */
    (usb_task_cdc_isenabled() && udi_cdc_is_rx_ready());
}

void MarlinSerialUSB::flush() { }

size_t MarlinSerialUSB::write(const uint8_t c) {

  /* Do not even bother sending anything if USB CDC is not enumerated
     or not configured on the PC side or there is no program on the PC
     listening to our messages */
  if (!usb_task_cdc_isenabled() || !usb_task_cdc_dtr_active())
    return 0;

  /* Wait until the PC has read the pending to be sent data */
  while (usb_task_cdc_isenabled() &&
         usb_task_cdc_dtr_active() &&
        !udi_cdc_is_tx_ready()) {
  };

  /* Do not even bother sending anything if USB CDC is not enumerated
     or not configured on the PC side or there is no program on the PC
     listening to our messages at this point */
  if (!usb_task_cdc_isenabled() || !usb_task_cdc_dtr_active())
    return 0;

  // Fifo full
  //  udi_cdc_signal_overrun();
  udi_cdc_putc(c);
  return 1;
}

// Preinstantiate
#if SERIAL_PORT == -1
  MSerialT1 customizedSerial1(TERN0(EMERGENCY_PARSER, true));
#endif
#if SERIAL_PORT_2 == -1
  MSerialT2 customizedSerial2(TERN0(EMERGENCY_PARSER, true));
#endif
#if SERIAL_PORT_3 == -1
  MSerialT3 customizedSerial3(TERN0(EMERGENCY_PARSER, true));
#endif

#endif // HAS_USB_SERIAL
#endif // ARDUINO_ARCH_SAM
