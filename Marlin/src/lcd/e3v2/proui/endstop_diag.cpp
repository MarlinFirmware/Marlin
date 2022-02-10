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
 * DWIN End Stops diagnostic page
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 1.0.2
 * Date: 2021/11/06
 *
 * Based on the original code provided by Creality under GPL
 */

#include "../../../inc/MarlinConfigPre.h"
#include "dwin_defines.h"

#if BOTH(DWIN_CREALITY_LCD_ENHANCED, HAS_ESDIAG)

#include "endstop_diag.h"

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin_popup.h"
#include "dwin.h"

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

ESDiagClass ESDiag;

void draw_es_label(FSTR_P const flabel=nullptr) {
  DWINUI::cursor.x = 40;
  if (flabel) DWINUI::Draw_String(F(flabel));
  DWINUI::Draw_String(F(": "));
  DWINUI::MoveBy(0, 25);
}

void draw_es_state(const bool is_hit) {
  const uint8_t LM = 130;
  DWINUI::cursor.x = LM;
  DWIN_Draw_Rectangle(1, HMI_data.PopupBg_color, LM, DWINUI::cursor.y, LM + 100, DWINUI::cursor.y + 20);
  is_hit ? DWINUI::Draw_String(RGB(31,31,16), F(STR_ENDSTOP_HIT)) : DWINUI::Draw_String(RGB(16,63,16), F(STR_ENDSTOP_OPEN));
  DWINUI::MoveBy(0, 25);
}

void ESDiagClass::Draw() {
  Title.ShowCaption(F("End-stops Diagnostic"));
  DWINUI::ClearMenuArea();
  Draw_Popup_Bkgd();
  DWINUI::Draw_Icon(ICON_Continue_E, 86, 250);
  DWINUI::cursor.y = 80;
  #define ES_LABEL(S) draw_es_label(F(STR_##S))
  #if HAS_X_MIN
    ES_LABEL(X_MIN);
  #endif
  #if HAS_Y_MIN
    ES_LABEL(Y_MIN);
  #endif
  #if HAS_Z_MIN
    ES_LABEL(Z_MIN);
  #endif
  #if HAS_FILAMENT_SENSOR
    draw_es_label(F(STR_FILAMENT));
  #endif
  Update();
}

void ESDiagClass::Update() {
  DWINUI::cursor.y = 80;
  #define ES_REPORT(S) draw_es_state(READ(S##_PIN) != S##_ENDSTOP_INVERTING)
  #if HAS_X_MIN
    ES_REPORT(X_MIN);
  #endif
  #if HAS_Y_MIN
    ES_REPORT(Y_MIN);
  #endif
  #if HAS_Z_MIN
    ES_REPORT(Z_MIN);
  #endif
  #if HAS_FILAMENT_SENSOR
    draw_es_state(READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE);
  #endif
  DWIN_UpdateLCD();
}

#endif // DWIN_CREALITY_LCD_ENHANCED && HAS_ESDIAG
