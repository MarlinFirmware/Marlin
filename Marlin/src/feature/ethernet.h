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

#include "../inc/MarlinConfig.h"

#ifdef ETHERNET_SUPPORT

#ifdef __IMXRT1062__
    #include <NativeEthernet.h>
#endif

// MAC_ADDRESS must be unique on your network.
//#define MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD
// Teensy 4.1 uses internal MAC Address

extern IPAddress ip, myDns, gateway, subnet;

extern bool ethernet_hardware_enabled, have_telnet_client;

extern EthernetClient telnetClient;

void ethernet_init();
void ethernet_check();

#endif  // ETHERNET_SUPPORT
