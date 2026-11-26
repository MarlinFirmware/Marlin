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

//
// MKS WiFi Module status
//
// Read-only status under "About Printer". The LVGL UI has its own WiFi
// screens; this lets TFT_COLOR_UI and TFT_CLASSIC_UI see whether the module is
// connected and what address it got. MarlinUI has no text entry, so the
// network itself is set with M587 (or WIFI_SSID / WIFI_PWD at build time).
//

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, LCD_INFO_MENU, MKS_WIFI_MODULE)

#include "menu_item.h"
#include "../../feature/mks_wifi/wifi_module.h"

void menu_info_mks_wifi() {
  // The module reports its state and address in every status packet
  const bool is_up = (wifi_link_state == WIFI_CONNECTED);

  FSTR_P state;
  switch (wifi_link_state) {
    case WIFI_CONNECTED:        state = GET_TEXT_F(MSG_MKS_WIFI_CONNECTED); break;
    case WIFI_WAIT_TRANS_START:
    case WIFI_TRANS_FILE:       state = GET_TEXT_F(MSG_MKS_WIFI_BUSY);      break;
    case WIFI_EXCEPTION:        state = GET_TEXT_F(MSG_MKS_WIFI_EXCEPTION); break;
    case WIFI_NOT_CONFIG:       state = GET_TEXT_F(MSG_MKS_WIFI_OFFLINE);   break;
    default:                    state = GET_TEXT_F(MSG_MKS_WIFI_CONNECTING); break;
  }

  if (ui.use_click()) return ui.go_back();

  START_SCREEN();

  // Link state, as one translated line (static items take a plain char* value)
  STATIC_ITEM_F(state, SS_LEFT);

  // Network it is joined to, or hosting
  STATIC_ITEM(MSG_MKS_WIFI_SSID, SS_LEFT, mks_wifi.ssid[0] ? (const char *)mks_wifi.ssid : "---");

  // Address the module reported. Stale unless the link is up.
  STATIC_ITEM(MSG_MKS_WIFI_IP, SS_LEFT, is_up && ipPara.ip_addr[0] ? ipPara.ip_addr : "---");

  // Joining a network, or being one
  STATIC_ITEM_F(mks_wifi.mode == AP_MODEL ? GET_TEXT_F(MSG_MKS_WIFI_MODE_AP) : GET_TEXT_F(MSG_MKS_WIFI_MODE_STA), SS_LEFT);

  END_SCREEN();
}

#endif // HAS_MARLINUI_MENU && LCD_INFO_MENU && MKS_WIFI_MODULE
