/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
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
 */

#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(WEBSUPPORT)

#include "../../core/serial.h"

#include "FS.h"
#include "SPIFFS.h"
#include "wifi.h"

AsyncEventSource events("/events"); // event source (Server-Sent events)

void onNotFound(AsyncWebServerRequest *request){
  request->send(404);
}

void web_init() {
  server.addHandler(&events);       // attach AsyncEventSource
  if (SPIFFS.begin()) {
    server.serveStatic("/", SPIFFS, "/www").setDefaultFile("index.html");
    server.onNotFound(onNotFound);
  }
  else
    SERIAL_ECHO_MSG("SPIFFS Mount Failed");
}

#endif // WEBSUPPORT
#endif // ARDUINO_ARCH_ESP32
