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

/**
 * DWIN by Creality3D
 * Rewrite and Extui Port by Jacob Myers
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../feature/pause.h"
#endif

#include "creality_dwin.h"
#include "../marlinui.h"

uint8_t MarlinUI::brightness = DEFAULT_LCD_BRIGHTNESS;
bool MarlinUI::backlight = true;

void MarlinUI::set_brightness(const uint8_t value) {
  if (value == 0) {
    backlight = false;
    DWIN_Backlight_SetLuminance(0);
  }
  else {
    backlight = true;
    brightness = constrain(value, MIN_LCD_BRIGHTNESS, MAX_LCD_BRIGHTNESS);
    DWIN_Backlight_SetLuminance(brightness);
  }
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    switch(message) {
      case PAUSE_MESSAGE_INSERT:
        CrealityDWIN.Confirm_Handler(FilInsert);
        break;
      case PAUSE_MESSAGE_OPTION:
        CrealityDWIN.Popup_Handler(PurgeMore);
        break;
      case PAUSE_MESSAGE_HEAT:
        CrealityDWIN.Confirm_Handler(HeaterTime);
        break;
      case PAUSE_MESSAGE_WAITING:
        CrealityDWIN.Draw_Print_Screen();
        break;
      default:
        break;
    }
  }
#endif

bool MarlinUI::get_blink() {
  static uint8_t blink = 0;
  static millis_t next_blink_ms = 0;
  millis_t ms = millis();
  if (ELAPSED(ms, next_blink_ms)) {
    blink ^= 0xFF;
    next_blink_ms = ms + 1000 - 250;
  }
  return blink != 0;
}

void MarlinUI::update() {
  CrealityDWIN.Update();
}

void MarlinUI::init() {
  delay(800);
  SERIAL_ECHOPGM("\nDWIN handshake ");
  if (DWIN_Handshake()) SERIAL_ECHOLNPGM("ok."); else SERIAL_ECHOLNPGM("error.");
  DWIN_Frame_SetDir(1); // Orientation 90°
  DWIN_UpdateLCD();     // Show bootscreen (first image)
  Encoder_Configuration();
  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }
  DWIN_JPG_CacheTo1(Language_English);
  CrealityDWIN.Redraw_Screen();
}

void MarlinUI::kill_screen(PGM_P const error, PGM_P const component) {
  CrealityDWIN.Draw_Popup((char*)"Printer Kill Reason:", error, (char*)"Restart Required", Wait, ICON_BLTouch);
}

#endif