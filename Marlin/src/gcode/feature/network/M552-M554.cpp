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

void ETH0_report() {
  SERIAL_ECHO_TERNARY(ethernet_hardware_enabled, "  Ethernet port ", "en", "dis", "abled.\n");
  if (!ethernet_hardware_enabled) SERIAL_ECHOLNPGM("Send 'M552 S1' to enable.");
  SERIAL_ECHO_TERNARY(have_telnet_client, "  Ethernet client ", "en", "dis", "abled.\n");
}

void MAC_report() {
  uint8_t mac[6];
  if (ethernet_hardware_enabled) {
    Ethernet.MACAddress(mac);
    SERIAL_ECHOPGM("MAC: ");
    LOOP_LE_N(i, 5) SERIAL_PRINTF("%02X:", mac[i]);
    SERIAL_PRINTF("%02X", mac[5]);
  }
  SERIAL_EOL();
}

void M552_report() {
  IPAddress thisip;
  if (Ethernet.linkStatus() == LinkON)
    thisip = Ethernet.localIP();  // Display current value if link is active
  else
    thisip = ip;                  // Otherwise show the EEPROM value

  SERIAL_ECHOLNPAIR("ip address: ", thisip[0], ".", thisip[1], ".", thisip[2], ".", thisip[3]);
}
void M553_report() {
  IPAddress thisip;
  if (Ethernet.linkStatus() == LinkON)
    thisip = Ethernet.subnetMask();
  else
    thisip = subnet;

  SERIAL_ECHOLNPAIR("subnet mask: ", thisip[0], ".", thisip[1], ".", thisip[2], ".", thisip[3]);
}
void M554_report() {
  IPAddress thisip;
  if (Ethernet.linkStatus() == LinkON)
    thisip = Ethernet.gatewayIP();
  else
    thisip = gateway;

  SERIAL_ECHOLNPAIR("gateway address: ", thisip[0], ".", thisip[1], ".", thisip[2], ".", thisip[3]);
}

/**
 * M552: Set IP address, enable/disable network interface
 *
 *  Pnnn : Set IP address, 0.0.0.0 means acquire an IP address using DHCP
 *  S0   : disable networking
 *  S1   : enable networking
 *  S-1  : reset network interface
 */
void GcodeSuite::M552() {
  if (parser.seenval('P')) ip.fromString(parser.value_string());
  M552_report();
  if (parser.seenval('S')) switch (parser.value_int()) {
    case -1:
      if (telnetClient) telnetClient.stop();
      ethernet_init();
      break;
    case 0: ethernet_hardware_enabled = false; break;
    case 1: ethernet_hardware_enabled = true; break;
    default: break;
  }
  ETH0_report();
}

/**
 * M553 Pnnn - Set netmask
 */
void GcodeSuite::M553() {
  if (parser.seenval('P')) subnet.fromString(parser.value_string());
  M553_report();
}

/**
 * M554 Pnnn - Set Gateway
 */
void GcodeSuite::M554() {
  if (parser.seenval('P')) gateway.fromString(parser.value_string());
  M554_report();
}

#endif // HAS_ETHERNET
