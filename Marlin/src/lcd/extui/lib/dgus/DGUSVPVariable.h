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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <stdint.h>

/**
 * DGUSVPVariable.h
 *
 *  Created on: Feb 9, 2019
 *      Author: tobi
 */

struct DGUS_VP_Variable {
  uint16_t VP;
  void*    memadr;  // If nullptr, the value cannot be uploaded to the display.
  uint8_t  size;

  // Callback that will be called if the display modified the value.
  // nullptr makes it readonly for the display.
  void (*set_by_display_handler)(DGUS_VP_Variable &var, void *val_ptr);
  void (*send_to_display_handler)(DGUS_VP_Variable &var);

  template<typename T>
  DGUS_VP_Variable& operator =(T &o) {
    *(T*)memadr = o;  // warning this is not typesafe.
    // TODO: Call out the display or mark as dirty for the next update.
    return *this;
  }
};
