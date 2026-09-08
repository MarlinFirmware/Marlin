/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(MKS_WIFI_MODULE)

#include "../../gcode.h"
#include "../../../feature/mks_wifi/wifi_module.h"
#include "../../../feature/mks_wifi/wifi_upload.h"
#include "../../../MarlinCore.h"
#include "../../../sd/cardreader.h"

static void set_wifi_str(uint8_t * const dst, const size_t dstsize, const char * const src) {
  memset(dst, '\0', dstsize);
  strlcpy((char *)dst, src, dstsize);
}

/**
 * M587: Set or report the MKS WiFi module network settings
 *
 * Parameters:
 *   S"<ssid>"  Network to join, or to host in access point mode
 *   P"<pass>"  Network password. Use P"" for an open network
 *   A<bool>    1 to host an access point, 0 to join a network
 *   U          Update the module's own firmware from 'MksWifi.bin' on the
 *              media, the same thing that runs at startup when the file is
 *              present. Takes up to a minute and blocks while it runs.
 *
 * With GCODE_QUOTED_STRINGS disabled only one string can be given per command,
 * unquoted: 'M587 SMyNetwork' then 'M587 PMyPassword'.
 *
 * Settings are applied to the module at once and saved with M500.
 * With no parameters, report the current settings.
 */
void GcodeSuite::M587() {

  // Flash the module from MksWifi.bin, as done at startup when it's present
  if (parser.seen('U')) {
    if (marlin.printingIsActive())
      SERIAL_ECHOLNPGM("Not while printing.");
    else {
      if (!card.isMounted()) card.mount();
      if (!card.isMounted())
        SERIAL_ECHOLNPGM("No media.");
      else if (!card.fileExists(ESP_FIRMWARE_FILE))
        SERIAL_ECHOLNPGM("No " ESP_FIRMWARE_FILE " on the media.");
      else
        mks_wifi_firmware_update(true);
    }
    return;
  }

  bool didset = false;

  if (parser.seenval('A')) {
    mks_wifi.mode = parser.value_bool() ? AP_MODEL : STA_MODEL;
    didset = true;
  }

  if (char * const ssid = parser.stringval('S')) {
    set_wifi_str(mks_wifi.ssid, sizeof(mks_wifi.ssid), ssid);
    didset = true;
  }
  else if (TERN(GCODE_QUOTED_STRINGS, false, parser.seen('S'))) {
    set_wifi_str(mks_wifi.ssid, sizeof(mks_wifi.ssid), parser.string_arg[0] == 'S' ? &parser.string_arg[1] : parser.string_arg);
    didset = true;
  }

  if (char * const pwd = parser.stringval('P')) {
    set_wifi_str(mks_wifi.key, sizeof(mks_wifi.key), pwd);
    didset = true;
  }
  else if (TERN(GCODE_QUOTED_STRINGS, false, parser.seen('P'))) {
    set_wifi_str(mks_wifi.key, sizeof(mks_wifi.key), parser.string_arg[0] == 'P' ? &parser.string_arg[1] : parser.string_arg);
    didset = true;
  }

  if (didset)
    mks_wifi_apply_settings();
  else
    M587_report(false);
}

/**
 * The password is deliberately not reported, so it doesn't end up in logs or
 * in a shared M503 dump. Set it again with M587 P.
 */
void GcodeSuite::M587_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  const bool is_ap = (mks_wifi.mode == AP_MODEL);

  report_heading(forReplay, F("MKS WiFi module"));
  report_echo_start(forReplay);
  SERIAL_ECHOLNPGM("  M587 S\"", (char *)mks_wifi.ssid, "\" A", int(is_ap),
                   "  ; ", is_ap ? "Access point" : "Join network", ", password not shown");

  // Live state from the module. Not part of an M503 replay, since it isn't a setting.
  if (!forReplay) {
    const char *state;
    switch (wifi_link_state) {
      case WIFI_CONNECTED:  state = "connected";      break;
      case WIFI_EXCEPTION:  state = "error";          break;
      case WIFI_NOT_CONFIG: state = "not configured"; break;
      default:              state = "connecting";     break;
    }
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("  Link: ", state);
    if (wifi_link_state == WIFI_CONNECTED && ipPara.ip_addr[0])
      SERIAL_ECHOPGM("  IP: ", ipPara.ip_addr);
    SERIAL_EOL();
  }
}

#endif // MKS_WIFI_MODULE
