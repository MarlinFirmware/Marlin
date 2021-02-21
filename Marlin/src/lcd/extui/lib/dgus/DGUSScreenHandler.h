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
#pragma once

/**
 * lcd/extui/lib/dgus/DGUSScreenHandler.h
 */

#include "../../../../inc/MarlinConfigPre.h"

#include "../../ui_api.h"

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  typedef struct  {
    ExtUI::extruder_t extruder; // which extruder to operate
    uint8_t action; // load or unload
    bool heated; // heating done ?
    float purge_length; // the length to extrude before unload, prevent filament jam
  } filament_data_t;

  extern filament_data_t filament_data;

#endif

// endianness swap
inline uint16_t swap16(const uint16_t value) { return (value & 0xFFU) << 8U | (value >> 8U); }

#if ENABLED(DGUS_LCD_UI_ORIGIN)
  #include "origin/DGUSScreenHandler.h"
#elif ENABLED(DGUS_LCD_UI_MKS)
  #include "mks/DGUSScreenHandler.h"
#elif ENABLED(DGUS_LCD_UI_FYSETC)
  #include "fysetc/DGUSScreenHandler.h"
#elif ENABLED(DGUS_LCD_UI_HIPRECY)
  #include "hiprecy/DGUSScreenHandler.h"
#endif

extern DGUSScreenHandler ScreenHandler;
