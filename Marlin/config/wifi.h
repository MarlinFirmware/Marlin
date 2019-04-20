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

/**
 * WiFi Support (Espressif ESP32 WiFi)
 */
//#define WIFISUPPORT
#if ENABLED(WIFISUPPORT)
  #define WIFI_SSID "Wifi SSID"
  #define WIFI_PWD  "Wifi Password"
  //#define WEBSUPPORT        // Start a webserver with auto-discovery
  //#define OTASUPPORT        // Support over-the-air firmware updates
#endif
