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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_ETHERNET

#include "../../../feature/ethernet.h"
#include "../../../core/serial.h"
#include "../../gcode.h"

/**
 * M552: Set IP address, enable/disable network interface
 *
 *  S0   : disable networking
 *  S1   : enable networking
 *  S-1  : reset network interface
 *
 *  Pnnn : Set IP address, 0.0.0.0 means acquire an IP address using DHCP
 */
void GcodeSuite::M552() {
  const bool seenP = parser.seenval('P');
  if (seenP) ethernet.ip.fromString(parser.value_string());

  const bool seenS = parser.seenval('S');
  if (seenS) {
    switch (parser.value_int()) {
      case -1:
        if (ethernet.telnetClient) ethernet.telnetClient.stop();
        ethernet.init();
        break;
      case 0: ethernet.hardware_enabled = false; break;
      case 1: ethernet.hardware_enabled = true; break;
      default: break;
    }
  }
  const bool nopar = !seenS && !seenP;
  if (nopar || seenS) ethernet.ETH0_report();
  if (nopar || seenP) M552_report();
}

void GcodeSuite::M552_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);
  ethernet.ip_report(552, F("ip address"), Ethernet.linkStatus() == LinkON ? Ethernet.localIP() : ethernet.ip, forReplay);
}

/**
 * M553 Pnnn - Set netmask
 */
void GcodeSuite::M553() {
  if (parser.seenval('P'))
    ethernet.subnet.fromString(parser.value_string());
  else
    M553_report();
}

void GcodeSuite::M553_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);
  ethernet.ip_report(553, F("subnet mask"), Ethernet.linkStatus() == LinkON ? Ethernet.subnetMask() : ethernet.subnet, forReplay);
}

/**
 * M554 Pnnn - Set Gateway
 */
void GcodeSuite::M554() {
  if (parser.seenval('P'))
    ethernet.gateway.fromString(parser.value_string());
  else
    M554_report();
}

void GcodeSuite::M554_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);
  ethernet.ip_report(554, F("gateway"), Ethernet.linkStatus() == LinkON ? Ethernet.gatewayIP() : ethernet.gateway, forReplay);
}

#endif // HAS_ETHERNET
