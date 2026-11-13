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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(CAN_BUS)

#include "../../gcode.h"
#include "../../../feature/canbus/klipper_can.h"
#include "../../../libs/hex_print.h"

static void print_uuid(const uint8_t (&uuid)[KLIPPER_CAN_UUID_LEN]) {
  for (uint8_t i = 0; i < KLIPPER_CAN_UUID_LEN; ++i) print_hex_byte(uuid[i]);
}

/**
 * M960: CAN bus status and node discovery
 *
 *   M960     Report the bus state and frame / error counters
 *   M960 Q   List CAN nodes running Klipper or Katapult that have no node ID yet
 *   M960 I   Assign a node ID to each unassigned Klipper node and read its data dictionary.
 *            A node keeps its ID until it is reset, so it won't appear in a later query.
 */
void GcodeSuite::M960() {
  if (!CANBus::bitrate()) {
    SERIAL_ECHOLNPGM("CAN bus not started");
    return;
  }

  if (parser.seen_test('Q') || parser.seen_test('I')) {
    KlipperCAN::node_t nodes[8];
    const uint8_t count = KlipperCAN::query_unassigned(nodes, COUNT(nodes));
    const bool identify = parser.seen_test('I');
    uint8_t nodeid = KlipperCAN::NODEID_FIRST;
    for (uint8_t i = 0; i < count; ++i) {
      const KlipperCAN::node_t &n = nodes[i];
      SERIAL_ECHOPGM("CAN node uuid=");
      print_uuid(n.uuid);
      SERIAL_ECHOPGM(" app=");
      switch (n.app) {
        case KlipperCAN::APP_KLIPPER:  SERIAL_ECHOPGM("Klipper"); break;
        case KlipperCAN::APP_KATAPULT: SERIAL_ECHOPGM("Katapult"); break;
        default: SERIAL_ECHOPGM("Unknown"); break;
      }
      if (identify && n.app == KlipperCAN::APP_KLIPPER) {
        uint32_t adler = 0;
        int32_t size = -1;
        if (KlipperCAN::set_nodeid(n.uuid, nodeid)) size = KlipperCAN::read_identify(nodeid, adler);
        SERIAL_ECHOPGM(" nodeid=", nodeid);
        if (size < 0)
          SERIAL_ECHOPGM(" identify failed");
        else {
          SERIAL_ECHOPGM(" dictionary=", size, " bytes adler32=");
          print_hex_long(adler);
        }
        ++nodeid;
      }
      SERIAL_EOL();
    }
    SERIAL_ECHOLNPGM("CAN nodes found: ", count);
    return;
  }

  can_stats_t s;
  CANBus::get_stats(s);
  SERIAL_ECHOLNPGM(
    "CAN bitrate:", CANBus::bitrate(),
    " state:", s.bus_off ? F("bus-off") : s.error_passive ? F("error-passive") : F("active"),
    " rx:", s.rx_frames, " tx:", s.tx_frames,
    " rx_overflow:", s.rx_overflow, " tx_full:", s.tx_full,
    " rx_errors:", s.rx_errors, " tx_errors:", s.tx_errors
  );
}

#endif // CAN_BUS
