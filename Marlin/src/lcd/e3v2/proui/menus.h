/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Menu functions for ProUI
 * Author: Miguel A. Risco-Castillo
 * Version: 1.2.1
 * Date: 2022/02/25
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

#include "dwinui.h"

typedef struct {
  int32_t MaxValue     = 0;        // Auxiliar max integer/scaled float value
  int32_t MinValue     = 0;        // Auxiliar min integer/scaled float value
  int8_t dp            = 0;        // Auxiliar decimal places
  int32_t Value        = 0;        // Auxiliar integer / scaled float value
  int16_t *P_Int       = nullptr;  // Auxiliar pointer to 16 bit integer variable
  float *P_Float       = nullptr;  // Auxiliar pointer to float variable
  void (*Apply)()      = nullptr;  // Auxiliar apply function
  void (*LiveUpdate)() = nullptr;  // Auxiliar live update function
} MenuData_t;

extern MenuData_t MenuData;

// Menuitem Drawing functions =================================================
void Draw_Title(TitleClass* title);
void Draw_Menu(MenuClass* menu);
void Draw_Menu_Cursor(const int8_t line);
void Erase_Menu_Cursor(const int8_t line);
void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false);
void Draw_Chkb_Line(const uint8_t line, const bool checked);
void Draw_Menu_IntValue(uint16_t bcolor, const uint8_t line, uint8_t iNum, const int32_t value=0);
void onDrawMenuItem(MenuItemClass* menuitem, int8_t line);
void onDrawSubMenu(MenuItemClass* menuitem, int8_t line);
void onDrawIntMenu(MenuItemClass* menuitem, int8_t line, int32_t value);
void onDrawPIntMenu(MenuItemClass* menuitem, int8_t line);
void onDrawPInt8Menu(MenuItemClass* menuitem, int8_t line);
void onDrawPInt32Menu(MenuItemClass* menuitem, int8_t line);
void onDrawFloatMenu(MenuItemClass* menuitem, int8_t line, uint8_t dp, const float value);
void onDrawPFloatMenu(MenuItemClass* menuitem, int8_t line);
void onDrawPFloat2Menu(MenuItemClass* menuitem, int8_t line);
void onDrawChkbMenu(MenuItemClass* menuitem, int8_t line, bool checked);

// On click functions =========================================================
void SetOnClick(uint8_t process, const int32_t lo, const int32_t hi, uint8_t dp, const int32_t val, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetValueOnClick(uint8_t process, const int32_t lo, const int32_t hi, const int32_t val, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetValueOnClick(uint8_t process, const float lo, const float hi, uint8_t dp, const float val, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetIntOnClick(const int32_t lo, const int32_t hi, const int32_t val, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetPIntOnClick(const int32_t lo, const int32_t hi, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetFloatOnClick(const float lo, const float hi, uint8_t dp, const float val, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);
void SetPFloatOnClick(const float lo, const float hi, uint8_t dp, void (*Apply)() = nullptr, void (*LiveUpdate)() = nullptr);

// HMI user control functions =================================================
void HMI_Menu();
void HMI_SetInt();
void HMI_SetPInt();
void HMI_SetIntNoDraw();
void HMI_SetFloat();
void HMI_SetPFloat();
