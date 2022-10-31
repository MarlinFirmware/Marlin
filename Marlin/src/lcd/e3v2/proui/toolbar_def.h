/**
 * ToolBar for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 1.3.1
 * Date: 2022/08/05
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "../../../inc/MarlinConfig.h"

#include "dwin.h"
#include "toolbar.h"

TBItem_t TBItemA[] = {
  {ICON_Homing, GET_TEXT_F(MSG_AUTO_HOME), AutoHome},
  #if HAS_BED_PROBE
    {(ICON_SetHome - 1), GET_TEXT_F(MSG_TRAMMING_WIZARD), Trammingwizard},
    {ICON_SetZOffset, GET_TEXT_F(MSG_PROBE_WIZARD), Draw_ZOffsetWiz_Menu},
    {ICON_Level, GET_TEXT_F(MSG_AUTO_MESH), AutoLev},
  #else
    {ICON_MoveZ0, F("Home Z and disable"), HomeZandDisable},
  #endif
  {ICON_CloseMotor, GET_TEXT_F(MSG_DISABLE_STEPPERS), DisableMotors},
  {ICON_Cool, GET_TEXT_F(MSG_COOLDOWN), DoCoolDown},
  #if HAS_PREHEAT
    #define _TBPREHEAT(N) {ICON_Preheat##N, GET_TEXT_F(MSG_PREHEAT_##N), DoPreheat##N},
    REPEAT_1(PREHEAT_COUNT, _TBPREHEAT)
  #endif
  {ICON_Box, GET_TEXT_F(MSG_BRIGHTNESS_OFF), TurnOffBacklight},
  {ICON_Reboot, GET_TEXT_F(MSG_RESET_PRINTER), RebootPrinter},
  {ICON_WriteEEPROM, GET_TEXT_F(MSG_STORE_EEPROM), WriteEeprom}
};
