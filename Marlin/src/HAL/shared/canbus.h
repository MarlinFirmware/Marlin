/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * HAL/shared/canbus.h - CAN bus interface implemented by each HAL that supports CAN_BUS
 *
 * Classic CAN 2.0 frames (up to 8 data bytes) with 11-bit or 29-bit identifiers.
 * Received frames are queued (by interrupt where the HAL can, otherwise by poll())
 * and consumed with read(). Sending never blocks: send() returns false when the
 * hardware transmit queue is full.
 */

#include <stdint.h>

#define CAN_FRAME_EXT       (1UL << 31)   // Flag on can_frame_t::id for a 29-bit (extended) identifier
#define CAN_STD_ID_MASK     0x7FFUL
#define CAN_EXT_ID_MASK     0x1FFFFFFFUL

#ifndef CAN_IRQ_PRIO
  #define CAN_IRQ_PRIO      5             // Receive interrupt priority, below the stepper ISR
#endif

#ifndef CAN_RX_QUEUE_SIZE
  #define CAN_RX_QUEUE_SIZE 16            // Received frames buffered between read() calls (power of 2)
#endif

typedef struct {
  uint32_t id;      // Identifier, with CAN_FRAME_EXT set for an extended identifier
  uint8_t len;      // Data length, 0-8
  uint8_t data[8];
} can_frame_t;

typedef struct {
  uint32_t rx_frames, tx_frames;  // Frames received / sent
  uint32_t rx_overflow;           // Frames lost because the receive queue was full
  uint32_t tx_full;               // send() calls refused because the hardware queue was full
  uint16_t rx_errors, tx_errors;  // Error counters reported by the CAN controller
  bool bus_off, error_passive;    // Controller error state
} can_stats_t;

class CANBus {
public:
  static bool begin(const uint32_t bitrate);  // Start the CAN peripheral on CAN_RX_PIN / CAN_TX_PIN. False on failure.
  static bool send(const can_frame_t &frame); // Queue a frame for transmit. False if the queue is full or the bus is down.
  static bool read(can_frame_t &frame);       // Get the oldest received frame. False if none is waiting.
  static void poll();                         // Move received frames into the queue on HALs without a receive interrupt
  static void get_stats(can_stats_t &stats);
  static uint32_t bitrate();                  // The bitrate in use, or 0 if not started
};
