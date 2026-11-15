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
 * feature/canbus/klipper_can.h - Talk to CAN nodes running Klipper (or Katapult) firmware
 *
 * Most CAN toolhead boards (BTT EBB / SB, Mellow FLY SHT / SB, MKS THR, LDO ...) ship with
 * Klipper or the Katapult bootloader. This is the host side of Klipper's CAN admin protocol
 * (node discovery and node ID assignment) and its message block framing, as described in
 * Klipper's docs/CANBUS_protocol.md and docs/Protocol.md and implemented by klippy/serialhdl.py
 * and src/generic/canserial.c. Clock sync, the data dictionary and the command set build on it.
 * Message encodings are in klipper_proto.h.
 */

#include "../../inc/MarlinConfigPre.h"
#include "../../core/millis_t.h"
#include "../../HAL/shared/canbus.h"
#include "klipper_proto.h"

#define KLIPPER_CAN_UUID_LEN 6

class KlipperCAN {
public:
  static constexpr uint16_t ADMIN_ID      = 0x3F0;   // Host -> all nodes admin requests
  static constexpr uint16_t ADMIN_RESP_ID = 0x3F1;   // Node -> host admin replies
  static constexpr uint8_t  NODEID_FIRST  = 4;       // Klipper assigns node IDs from 4 up

  // Application that answered a query (byte 7 of the reply)
  enum NodeApp : uint8_t { APP_KLIPPER = 0x01, APP_KATAPULT = 0x11 };

  typedef struct {
    uint8_t uuid[KLIPPER_CAN_UUID_LEN];
    uint8_t app;
  } node_t;

  // Host -> node and node -> host CAN identifiers for an assigned node ID
  static constexpr uint16_t tx_id(const uint8_t nodeid) { return 0x100 + nodeid * 2; }
  static constexpr uint16_t rx_id(const uint8_t nodeid) { return tx_id(nodeid) + 1; }

  // Ask unassigned nodes to report their UUID. Return the number found (up to 'max').
  static uint8_t query_unassigned(node_t nodes[], const uint8_t max, const millis_t wait_ms=500);

  // Assign a node ID to the node with the given UUID
  static bool set_nodeid(const uint8_t (&uuid)[KLIPPER_CAN_UUID_LEN], const uint8_t nodeid);

  // Read the node's data dictionary (zlib-compressed JSON) without storing it.
  // Return its size in bytes, or -1 on failure. 'adler' gets the Adler-32 of the data.
  static int32_t read_identify(const uint8_t nodeid, uint32_t &adler);

private:
  static uint8_t seq;  // Sequence number of the next block to send
  static uint8_t rx_buf[KlipperProto::MESSAGE_MAX * 2], rx_have; // Bytes received from the node, not yet used

  static bool send_block(const uint8_t nodeid, const uint8_t *payload, const uint8_t len);
  static uint8_t receive_block(const uint8_t nodeid, uint8_t (&block)[KlipperProto::MESSAGE_MAX], const millis_t timeout_ms);
};
