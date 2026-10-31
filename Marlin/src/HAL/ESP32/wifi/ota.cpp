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

#ifdef ARDUINO_ARCH_ESP32

#include <WiFi.h>

#undef ENABLED
#undef DISABLED

#include "../../../inc/MarlinConfigPre.h"

#if ALL(WIFISUPPORT, OTASUPPORT)

#include "../../../inc/MarlinConfig.h"

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#if ENABLED(USE_WATCHDOG)
  #include <esp_task_wdt.h>
#endif

void OTA_init() {
  ArduinoOTA
    .onStart([]() {
      HAL_timer_pause(MF_TIMER_STEP);
      HAL_timer_pause(MF_TIMER_TEMP);

      // The whole update runs in the Marlin loop task, which can't refresh the watchdog meanwhile.
      // A successful update reboots; a failed one restarts the timers and watches the task again in onError.
      TERN_(USE_WATCHDOG, esp_task_wdt_delete(nullptr));

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      SERIAL_ECHOLNPGM("OTA update: ", ArduinoOTA.getCommand() == U_FLASH ? "firmware" : "filesystem");
    })
    .onEnd([]() {
      SERIAL_ECHOLNPGM("OTA update done");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      static uint8_t last_pc = 255;
      const uint8_t pc = progress / (total / 100);
      if (pc && pc != last_pc && pc % 10 == 0) { last_pc = pc; SERIAL_ECHOLNPGM("OTA update ", pc, "%"); }
    })
    .onError([](ota_error_t error) {
      // The update failed, so Marlin carries on. Without the timers the temperature ISR
      // never runs, so the watchdog is never refreshed again and heater PWM is frozen.
      HAL_timer_resume(MF_TIMER_STEP);
      HAL_timer_resume(MF_TIMER_TEMP);
      TERN_(USE_WATCHDOG, esp_task_wdt_add(nullptr));
      const char *str = "unknown";
      switch (error) {
        case OTA_AUTH_ERROR:    str = "Auth Failed";    break;
        case OTA_BEGIN_ERROR:   str = "Begin Failed";   break;
        case OTA_CONNECT_ERROR: str = "Connect Failed"; break;
        case OTA_RECEIVE_ERROR: str = "Receive Failed"; break;
        case OTA_END_ERROR:     str = "End Failed";     break;
      }
      SERIAL_ECHO_MSG("OTA update failed: ", str);
    });

  ArduinoOTA.begin();
}

void OTA_handle() {
  ArduinoOTA.handle();
}

#endif // WIFISUPPORT && OTASUPPORT
#endif // ARDUINO_ARCH_ESP32
