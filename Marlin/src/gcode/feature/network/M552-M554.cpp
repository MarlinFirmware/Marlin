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

void M552_report() {
  SERIAL_ECHO("ip address: ");
  if (Ethernet.linkStatus() == LinkON) {
    switch (serial_port_index) {
      case 0: Ethernet.localIP().printTo(MYSERIAL0);
        break;
      case 1: 
        #ifdef SERIAL_PORT_2
          Ethernet.localIP().printTo(MYSERIAL1);
        #else
          if (have_telnet_client) Ethernet.localIP().printTo(telnetClient);
        #endif
        break;
    }
  }
  else {
    switch (serial_port_index) {
      case 0: ip.printTo(MYSERIAL0);
        break;
      case 1:
        #ifdef SERIAL_PORT_2
          ip.printTo(MYSERIAL1);
        #else
          if (have_telnet_client) ip.printTo(telnetClient);
        #endif
        break;
    }
  }
  if (!ip) SERIAL_ECHO(" (DHCP)");
  SERIAL_EOL();
}
void M553_report() {
  SERIAL_ECHO("subnet mask: ");
  if (Ethernet.linkStatus() == LinkON) {
    switch (serial_port_index) {
      case 0: Ethernet.subnetMask().printTo(MYSERIAL0);
        break;
      case 1:
        #ifdef SERIAL_PORT_2
          Ethernet.subnetMask().printTo(MYSERIAL1);
        #else
          if (have_telnet_client) Ethernet.subnetMask().printTo(telnetClient);
        #endif
        break;
    }
  }
  else {
    switch (serial_port_index) {
      case 0: subnet.printTo(MYSERIAL0);
        break;
      case 1:
        #ifdef SERIAL_PORT_2
          subnet.printTo(MYSERIAL1);
        #else
          if (have_telnet_client) subnet.printTo(telnetClient);
        #endif
        break;
    }
  }
  SERIAL_EOL();
}
void M554_report() {
  SERIAL_ECHO("gateway address: ");
  if (Ethernet.linkStatus() == LinkON) {
    switch (serial_port_index) {
      case 0: Ethernet.gatewayIP().printTo(MYSERIAL0);
        break;
      case 1:
        #ifdef SERIAL_PORT_2
          Ethernet.gatewayIP().printTo(MYSERIAL1);
        #else
          if (have_telnet_client) Ethernet.gatewayIP().printTo(telnetClient);
        #endif
        break;
    }
  }
  else {
    switch (serial_port_index) {
      case 0: gateway.printTo(MYSERIAL0);
        break;
      case 1:
        #ifdef SERIAL_PORT_2
          gateway.printTo(MYSERIAL1);
        #else
          if (have_telnet_client) gateway.printTo(telnetClient);
        #endif
        break;
    }
  }
  SERIAL_EOL();
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
