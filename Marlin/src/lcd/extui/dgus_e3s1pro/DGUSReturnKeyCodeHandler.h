/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "definition/DGUS_VP.h"

namespace DGUSReturnKeyCodeHandler {
  void Command_MenuSelect(DGUS_VP &vp, void *data);
  void Command_Adjust(DGUS_VP &vp, void *data);
  void Command_CheckKO(DGUS_VP &vp, void *data);
  void Command_StopPause(DGUS_VP &vp, void *data);
  void Command_CheckOK(DGUS_VP &vp, void *data);
  void Command_PresetControl(DGUS_VP &vp, void *data);
  void Control_TemperatureCommand(DGUS_VP &vp, void *data);
  void Command_SettingsMenu(DGUS_VP &vp, void *data);
  void Command_Leveling(DGUS_VP &vp, void *data);
  void Command_AxisControl(DGUS_VP &vp, void *data);
  void Command_FilamentIO(DGUS_VP &vp, void *data);
  void Command_PowerLoss(DGUS_VP &vp, void *data);
  void Command_AdvancedSettings(DGUS_VP &vp, void *data);
  void Command_FilelistControl(DGUS_VP &vp, void *data);
  void Command_LaserControl(DGUS_VP &vp, void *data);
}
