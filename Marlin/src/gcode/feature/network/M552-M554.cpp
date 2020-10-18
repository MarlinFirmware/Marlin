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

#if ENABLED(ETHERNET_SUPPORT)

#include "../../../feature/ethernet.h"
#include "../../../core/serial.h"
#include "../../gcode.h"

void MAC_report() {
  uint8_t mac[6];

  Ethernet.MACAddress(mac);

  SERIAL_ECHO("MAC: ");
  SERIAL_PRINTF("%02X:",mac[0]);
  SERIAL_PRINTF("%02X:",mac[1]);
  SERIAL_PRINTF("%02X:",mac[2]);
  SERIAL_PRINTF("%02X:",mac[3]);
  SERIAL_PRINTF("%02X:",mac[4]);
  SERIAL_PRINTF("%02X\n",mac[5]);
}

void M552_report() {
  IPAddress thisip;
  if (Ethernet.linkStatus() == LinkON)
    thisip = Ethernet.localIP();        // display current value if link is active
  else
    thisip = ip;                        // otherwise show the EEPROM value

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

//
// M552: Set IP address, enable/disable network interface
//      Pnnn IP address, 0.0.0.0 means acquire an IP address using DHCP
//      Snnn (optional) -1 = reset network interface, 0 = disable networking, 1 = enable networking

  void GcodeSuite::M552() {
    if (parser.seenval('P')) ip.fromString(parser.value_string());
    M552_report();

    if (parser.seenval('S')) switch (parser.value_int()) {
      case -1:
        if (telnetClient) telnetClient.stop();
        ethernet_init();
        break;
      case 0:
        ethernet_hardware_enabled = false;
        break;
      case 1:
        ethernet_hardware_enabled = true;
        break;
      default:
        break;
    }
    SERIAL_ECHO("  Ethernet hardware ");
    if (ethernet_hardware_enabled)
      SERIAL_ECHOLN("enabled");
    else {
      SERIAL_ECHOLN("disabled");
      SERIAL_ECHOLN("Send M552 S1 to enable");
    }

    SERIAL_ECHO("  Ethernet port ");
    if (have_telnet_client)
      SERIAL_ECHOLN("enabled");
    else
      SERIAL_ECHOLN("disabled");
  }

//
// M553: Set Netmask
//      Pnnn Netmask
//

  void GcodeSuite::M553() {
    if (parser.seenval('P')) subnet.fromString(parser.value_string());
    M553_report();
  }

//
// M554: Set Gateway
//      Pnnn Gatewway
//

  void GcodeSuite::M554() {
    if (parser.seenval('P')) gateway.fromString(parser.value_string());
    M554_report();
  }


#endif // ETHERNET_SUPPORT
