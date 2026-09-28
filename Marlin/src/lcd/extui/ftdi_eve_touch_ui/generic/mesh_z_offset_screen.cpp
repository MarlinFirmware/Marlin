/****************************
 * mesh_z_offset_screen.cpp *
 ****************************/

/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_MESH_Z_OFFSET_SCREEN

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_COLS 13
#define GRID_ROWS (9+EXTRUDERS)

void MeshZOffsetScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  CommandProcessor cmd;

  w.precision(2, BaseNumericAdjustmentScreen::DEFAULT_MIDRANGE).units(GET_TEXT_F(MSG_UNITS_MM));

  w.heading(                  GET_TEXT_F(MSG_MESH_Z_OFFSET));
  w.color(z_axis).adjuster(4, GET_TEXT_F(MSG_MESH_Z_OFFSET), getMeshZOffset_mm());
  w.increments();
  if (what & FOREGROUND) {
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(6).colors(action_btn).button(BTN_POS(1,GRID_ROWS), BTN_SIZE(GRID_COLS,1), GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool MeshZOffsetScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 4: UI_DECREMENT(MeshZOffset_mm); break;
    case 5: UI_INCREMENT(MeshZOffset_mm); break;
    case 6: GOTO_SCREEN(SaveSettingsDialogBox); break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // FTDI_MESH_Z_OFFSET_SCREEN
