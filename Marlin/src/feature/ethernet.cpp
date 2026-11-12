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

#include "../inc/MarlinConfigPre.h"

#if HAS_ETHERNET

#include "ethernet.h"
#include "../core/serial.h"

#define DEBUG_OUT ENABLED(DEBUG_ETHERNET)
#include "../core/debug_out.h"

bool MarlinEthernet::hardware_enabled, // = false
     MarlinEthernet::have_telnet_client; // = false

IPAddress MarlinEthernet::ip,
          MarlinEthernet::myDns,
          MarlinEthernet::gateway,
          MarlinEthernet::subnet;

EthernetClient  MarlinEthernet::telnetClient;  // connected client

MarlinEthernet ethernet;

EthernetServer server(23);    // telnet server

enum linkStates { UNLINKED, LINKING, LINKED, CONNECTING, CONNECTED, NO_HARDWARE } linkState;

#ifdef __IMXRT1062__

  static void teensyMAC(uint8_t * const mac) {
    const uint32_t m1 = HW_OCOTP_MAC1, m2 = HW_OCOTP_MAC0;
    mac[0] = m1 >> 8;
    mac[1] = m1 >> 0;
    mac[2] = m2 >> 24;
    mac[3] = m2 >> 16;
    mac[4] = m2 >> 8;
    mac[5] = m2 >> 0;
  }

#else

  byte mac[] = MAC_ADDRESS;

#endif

void ethernet_cable_error() { SERIAL_ERROR_MSG("Ethernet cable is not connected."); }

void MarlinEthernet::init() {
  if (!hardware_enabled) return;

  SERIAL_ECHO_MSG("Starting network...");

  // Init the Ethernet device
  #ifdef __IMXRT1062__
    uint8_t mac[6];
    teensyMAC(mac);
  #endif

  if (!ip) {
    Ethernet.begin(mac); // use DHCP
  }
  else {
    if (!gateway) {
      gateway = ip;
      gateway[3] = 1;
      myDns = gateway;
      subnet = IPAddress(255,255,255,0);
    }
    if (!myDns) myDns = gateway;
    if (!subnet) subnet = IPAddress(255,255,255,0);
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    SERIAL_ERROR_MSG("No Ethernet hardware found.");
    linkState = NO_HARDWARE;
    return;
  }

  linkState = UNLINKED;

  if (Ethernet.linkStatus() == LinkOFF)
    ethernet_cable_error();
}

void MarlinEthernet::check() {
  if (!hardware_enabled) return;

  switch (linkState) {
    case NO_HARDWARE:
      break;

    case UNLINKED:
      if (Ethernet.linkStatus() == LinkOFF) break;

      SERIAL_ECHOLNPGM("Ethernet cable connected");
      server.begin();
      linkState = LINKING;
      break;

    case LINKING:
      if (!Ethernet.localIP()) break;

      SERIAL_ECHOPGM("Successfully started telnet server with IP ");
      MYSERIAL1.println(Ethernet.localIP());

      linkState = LINKED;
      break;

    case LINKED:
      if (Ethernet.linkStatus() == LinkOFF) {
        ethernet_cable_error();
        linkState = UNLINKED;
        break;
      }
      telnetClient = server.accept();
      if (telnetClient) linkState = CONNECTING;
      break;

    case CONNECTING:
      telnetClient.println("Marlin " SHORT_BUILD_VERSION);
      #ifdef STRING_DISTRIBUTION_DATE
        telnetClient.println(
          " Last Updated: " STRING_DISTRIBUTION_DATE
          " | Author: " STRING_CONFIG_H_AUTHOR
        );
      #endif
      telnetClient.println(" Compiled: " __DATE__);

      SERIAL_ECHOLNPGM("Client connected");
      have_telnet_client = true;
      linkState = CONNECTED;
      break;

    case CONNECTED:
      if (telnetClient && !telnetClient.connected()) {
        SERIAL_ECHOLNPGM("Client disconnected");
        telnetClient.stop();
        have_telnet_client = false;
        linkState = LINKED;
      }
      if (Ethernet.linkStatus() == LinkOFF) {
        ethernet_cable_error();
        if (telnetClient) telnetClient.stop();
        linkState = UNLINKED;
      }
      break;

    default: break;
  }
}

void say_ethernet() { SERIAL_ECHOPGM("  Ethernet "); }

void MarlinEthernet::ETH0_report(const bool forReplay/*=true*/) {
  say_ethernet();
  SERIAL_ECHO_TERNARY(ethernet.hardware_enabled, "port ", "en", "dis", "abled.\n");
  if (ethernet.hardware_enabled) {
    say_ethernet();
    SERIAL_ECHO_TERNARY(ethernet.have_telnet_client, "client ", "en", "dis", "abled.\n");
  }
  else
    SERIAL_ECHOLNPGM("Send 'M552 S1' to enable.");
}

void MarlinEthernet::MAC_report(const bool forReplay/*=true*/) {
  if (!forReplay) SERIAL_ECHO_START();
  SERIAL_ECHOPGM("MAC: ");
  if (ethernet.hardware_enabled) {
    uint8_t mac[6];
    Ethernet.MACAddress(mac);
    for (uint8_t i = 0; i < 6; ++i) {
      if (mac[i] < 0x10) SERIAL_CHAR('0');
      SERIAL_PRINT(mac[i], PrintBase::Hex);
      if (i < 5) SERIAL_CHAR(':');
    }
  }
  else
    SERIAL_ECHOPGM("Disabled");
  SERIAL_EOL();
}

// Display current values when the link is active,
// otherwise show the stored values
void MarlinEthernet::ip_report(const uint16_t cmd, FSTR_P const post, const IPAddress &ipo, const bool forReplay/*=true*/) {
  if (!forReplay) SERIAL_ECHO_START();
  SERIAL_ECHO(F("  M"), cmd, C(' '));
  for (uint8_t i = 0; i < 4; ++i) {
    SERIAL_ECHO(ipo[i]);
    if (i < 3) SERIAL_CHAR('.');
  }
  SERIAL_ECHOLN(F(" ; "), post);
}

#endif // HAS_ETHERNET
