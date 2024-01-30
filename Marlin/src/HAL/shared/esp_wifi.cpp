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

#include "../../inc/MarlinConfig.h"

#if ENABLED(WIFISUPPORT)

#include "Delay.h"

void esp_wifi_init(void) {                          // init ESP01 WIFI module pins
  #if PIN_EXISTS(ESP_WIFI_MODULE_GPIO0)
    OUT_WRITE(ESP_WIFI_MODULE_GPIO0_PIN, HIGH);
  #endif
  #if PIN_EXISTS(ESP_WIFI_MODULE_GPIO2)
    OUT_WRITE(ESP_WIFI_MODULE_GPIO2_PIN, HIGH);
  #endif
  #if PIN_EXISTS(ESP_WIFI_MODULE_RESET)
    delay(1);  // power up delay (0.1mS minimum)
    OUT_WRITE(ESP_WIFI_MODULE_RESET_PIN, LOW);
    delay(1);
    OUT_WRITE(ESP_WIFI_MODULE_RESET_PIN, HIGH);
  #endif
  #if PIN_EXISTS(ESP_WIFI_MODULE_ENABLE)
    delay(1);  // delay after reset released (0.1mS minimum)
    OUT_WRITE(ESP_WIFI_MODULE_ENABLE_PIN, HIGH);
  #endif
}

#endif // WIFISUPPORT
