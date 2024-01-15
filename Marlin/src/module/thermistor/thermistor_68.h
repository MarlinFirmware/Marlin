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
#pragma once

#define REVERSE_TEMP_SENSOR_RANGE_68 1

// PT100 amplifier board from Dyze Design
constexpr temp_entry_t temptable_68[] PROGMEM = {
  { OV(273), 0   },
  { OV(294), 20  },
  { OV(315), 40  },
  { OV(336), 60  },
  { OV(356), 80  },
  { OV(376), 100 },
  { OV(396), 120 },
  { OV(416), 140 },
  { OV(436), 160 },
  { OV(455), 180 },
  { OV(474), 200 },
  { OV(494), 220 },
  { OV(513), 240 },
  { OV(531), 260 },
  { OV(550), 280 },
  { OV(568), 300 },
  { OV(587), 320 },
  { OV(605), 340 },
  { OV(623), 360 },
  { OV(641), 380 },
  { OV(658), 400 },
  { OV(676), 420 },
  { OV(693), 440 },
  { OV(710), 460 },
  { OV(727), 480 },
  { OV(744), 500 }
};
