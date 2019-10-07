/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../core/macros.h" // for ENABLED

/**
 * This file is the standard Marlin version identifier file.
 * Use -DUSE_AUTOMATIC_VERSIONING=1 and a custom _Version.h
 * to override these values.
 */

#if ENABLED(USE_AUTOMATIC_VERSIONING)

  #include "_Version.h"

#else

  /**
   * Marlin release version identifier
   */
  #define SHORT_BUILD_VERSION "bugfix-2.0.x"

  /**
   * Verbose version identifier which should contain a reference to the location
   * from where the binary was downloaded or the source code was compiled.
   */
  #define DETAILED_BUILD_VERSION SHORT_BUILD_VERSION " (Github)"

  /**
   * The STRING_DISTRIBUTION_DATE represents when the binary file was built,
   * here we define this default string as the date where the latest release
   * version was tagged.
   */
  #define STRING_DISTRIBUTION_DATE "2019-10-05"

  /**
   * Required minimum Configuration.h and Configuration_adv.h file versions.
   *
   * You must increment this version number for every significant change such as,
   * but not limited to: ADD, DELETE RENAME OR REPURPOSE any directive/option on
   * the configuration files.
   */
  #define REQUIRED_CONFIGURATION_H_VERSION 020000
  #define REQUIRED_CONFIGURATION_ADV_H_VERSION 020000

  /**
   * The protocol for communication to the host. Protocol indicates communication
   * standards such as the use of ASCII, "echo:" and "error:" line prefixes, etc.
   * (Other behaviors are given by the firmware version and capabilities report.)
   */
  #define PROTOCOL_VERSION "1.0"

  /**
   * Defines a generic printer name to be output to the LCD after booting Marlin.
   */
  #define MACHINE_NAME "3D Printer"

  /**
   * The SOURCE_CODE_URL is the location where users will find the Marlin Source
   * Code which is installed on the device. In most cases —unless the manufacturer
   * has a distinct Github fork— the Source Code URL should just be the main
   * Marlin repository.
   */
  #define SOURCE_CODE_URL "https://github.com/MarlinFirmware/Marlin"

  /**
   * Default generic printer UUID.
   */
  #define DEFAULT_MACHINE_UUID "cede2a2f-41a2-4748-9b12-c55c62f367ff"

  /**
   * The WEBSITE_URL is the location where users can get more information such as
   * documentation about a specific Marlin release. Displayed in the Info Menu.
   */
  #define WEBSITE_URL "http://marlinfw.org"

#endif // USE_AUTOMATIC_VERSIONING
