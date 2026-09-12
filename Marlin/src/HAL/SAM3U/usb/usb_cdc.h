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
#pragma once

/**
 * USB CDC-ACM device for Atmel SAM3U (AT91SAM3U4E)
 *
 * The 4pi's only host connection is native USB - there is no USB-serial chip
 * on the board - so this is what Marlin normally talks to the host over.
 *
 * The SAM3U's device controller is UDPHS, which is a different peripheral from
 * the Arduino Due's UOTGHS; HAL/DUE/usb/ is not reusable. This driver was
 * written against the SAM3U datasheet (section 38) using the working UDPHS
 * stack in the original 4pi/Sprinter firmware
 * (referance material/4pi-firmware/at91lib/usb/) as the reference for the
 * hardware sequences, but it is purpose-built for CDC rather than a port of
 * that stack's generic framework.
 *
 * See usb_cdc.cpp for the design notes, in particular why this enumerates as
 * a full-speed device.
 */

#include <stdint.h>
#include <stddef.h>

// Bring up the controller and attach to the bus. Called from MarlinHAL::init().
void usb_cdc_init();

// True once the host has configured us and opened the port (DTR asserted)
bool usb_cdc_connected();

// Bytes waiting in the receive buffer
uint16_t usb_cdc_available();

// Next byte, or -1 if none. Does not block.
int usb_cdc_read();

// Next byte without consuming it, or -1 if none
int usb_cdc_peek();

// Discard everything buffered for reading
void usb_cdc_rx_flush();

// Queue one byte. Blocks only if the transmit buffer is full and the host is
// still reading; drops the byte if nothing is listening.
void usb_cdc_write(const uint8_t c);

// Push any partially-filled packet to the host
void usb_cdc_flush();

// Called from MarlinHAL::idletask() to move buffered data onto the bus
void usb_cdc_task();
