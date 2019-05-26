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
  * Marlin release version identifier
  */
#ifndef SHORT_BUILD_VERSION
  #define SHORT_BUILD_VERSION "bugfix-2.0.x"
#endif

/**
  * Verbose version identifier which should contain a reference to the location
  * from where the binary was downloaded or the source code was compiled.
  */
#ifndef DETAILED_BUILD_VERSION
  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION " (Github)"
#endif

/**
  * The STRING_DISTRIBUTION_DATE represents when the binary file was built,
  * here we define this default string as the date where the latest release
  * version was tagged.
  */
#ifndef STRING_DISTRIBUTION_DATE
  #define STRING_DISTRIBUTION_DATE "2018-01-20"
#endif

/**
  * Required minimum Configuration.h and Configuration_adv.h file versions.
  *
  * You must increment this version number for every significant change such as,
  * but not limited to: ADD, DELETE RENAME OR REPURPOSE any directive/option on
  * the configuration files.
  */
#ifndef REQUIRED_CONFIGURATION_H_VERSION
  #define REQUIRED_CONFIGURATION_H_VERSION 020000
#endif
#ifndef REQUIRED_CONFIGURATION_ADV_H_VERSION
  #define REQUIRED_CONFIGURATION_ADV_H_VERSION 020000
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
  * Defines a generic printer name to be output to the LCD after booting Marlin.
  */
#ifndef MACHINE_NAME
  #define MACHINE_NAME "3D Printer"
#endif

/**
  * The SOURCE_CODE_URL is the location where users will find the Marlin Source
  * Code which is installed on the device. In most cases —unless the manufacturer
  * has a distinct Github fork— the Source Code URL should just be the main
  * Marlin repository.
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
  * The WEBSITE_URL is the location where users can get more information such as
  * documentation about a specific Marlin release.
  */
#ifndef WEBSITE_URL
  #define WEBSITE_URL "http://marlinfw.org"
#endif
