/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../core/macros.h" // for ENABLED

/**
 * Release version. Leave the Marlin version or apply a custom scheme.
 */
#ifndef SHORT_BUILD_VERSION
  #define SHORT_BUILD_VERSION "bugfix-2.0.x"
#endif

/**
 * Verbose version identifier containing a unique identifier, such as the
 * vendor name, download location, GitHub account, etc.
 */
#ifndef DETAILED_BUILD_VERSION
  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION " (GitHub)"
#endif

/**
 * Distribution date of this release.
 */
#ifndef STRING_DISTRIBUTION_DATE
  #define STRING_DISTRIBUTION_DATE "2018-01-20"
#endif

/**
 * Minimum Configuration.h and Configuration_adv.h file versions.
 * Set based on the release version number. Used to catch an attempt to use
 * older configurations. Override these if using a custom versioning scheme
 * to alert users to major changes.
 */
#define MARLIN_HEX_VERSION 020000
#ifndef REQUIRED_CONFIGURATION_H_VERSION
  #define REQUIRED_CONFIGURATION_H_VERSION MARLIN_HEX_VERSION
#endif
#ifndef REQUIRED_CONFIGURATION_ADV_H_VERSION
  #define REQUIRED_CONFIGURATION_ADV_H_VERSION MARLIN_HEX_VERSION
#endif

/**
 * The protocol for communication to the host. Protocol indicates communication
 * standards such as the use of ASCII, "echo:" and "error:" line prefixes, etc.
 * (Other behaviors are given by the firmware version and capabilities report.)
 */
#ifndef PROTOCOL_VERSION
  #define PROTOCOL_VERSION "1.0"
#endif

/**
 * Define a generic printer name to be output to the LCD after booting Marlin.
 */
#ifndef MACHINE_NAME
  #define MACHINE_NAME "3D Printer"
#endif

/**
 * Website where users can find Marlin source code for the binary installed on the
 * device. Override this if you provide public source code download. (GPLv3 requires
 * providing the source code to your customers.)
 */
#ifndef SOURCE_CODE_URL
  #define SOURCE_CODE_URL "https://github.com/MarlinFirmware/Marlin"
#endif

/**
 * Default generic printer UUID.
 */
#ifndef DEFAULT_MACHINE_UUID
  #define DEFAULT_MACHINE_UUID "cede2a2f-41a2-4748-9b12-c55c62f367ff"
#endif

/**
 * Website where users can find information about the 3D Printer.
 * A permanent link to a vendor website is recommended.
 */
#ifndef WEBSITE_URL
  #define WEBSITE_URL "http://marlinfw.org"
#endif
