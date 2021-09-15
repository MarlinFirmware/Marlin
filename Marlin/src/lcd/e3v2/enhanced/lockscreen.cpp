/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.6.3
 * Date: 2021/09/08
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD_ENHANCED)

#include "../../../core/types.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"
#include "lockscreen.h"

LockScreenClass LockScreen;

void LockScreenClass::Init() {
  Lock_Pos = 0;
  unlocked = false;
  Draw();
}

void LockScreenClass::Draw() {
  Title.SetCaption(PSTR("Lock Screen"));
  DWINUI::ClearMenuArea();
  DWINUI::Draw_Icon(ICON_LOGO, 71, 120);  // CREALITY logo
  DWINUI::Draw_CenteredString(Color_White, 180, F("Printer is Locked,"));
  DWINUI::Draw_CenteredString(Color_White, 200, F("Scroll to unlock."));
  DWINUI::Draw_CenteredString(Color_White, 240, F("-> | <-"));
  DWIN_Draw_Box(1, HMI_data.Barfill_Color, 0, 260, DWIN_WIDTH, 20);
  DWIN_Draw_VLine(Color_Yellow, Lock_Pos * DWIN_WIDTH / 255, 260, 20);
  DWIN_UpdateLCD();
}

void LockScreenClass::onEncoderState(ENCODER_DiffState encoder_diffState) {
  if (encoder_diffState == ENCODER_DIFF_CW) {
    Lock_Pos += 8;
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    Lock_Pos -= 8;
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    unlocked = (Lock_Pos == 128);
  }
  DWIN_Draw_Box(1, HMI_data.Barfill_Color, 0, 260, DWIN_WIDTH, 20);
  DWIN_Draw_VLine(Color_Yellow, Lock_Pos * DWIN_WIDTH / 255, 260, 20);
  DWIN_UpdateLCD();
}

bool LockScreenClass::isUnlocked() { return unlocked; }

#endif // DWIN_CREALITY_LCD_ENHANCED
