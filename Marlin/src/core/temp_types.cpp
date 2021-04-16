/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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

#include "temp_types.h"

kelvin_t::kelvin_t(celsius_t c) : f((float)c + 273.15f) {}
fahrenheit_t::fahrenheit_t(celsius_t c) : f((float)c * 0.5555555556f + 32) {}

#if ENABLED(FIXED_POINT_CELSIUS)
  celsius_t minCValue = celsius_t::_minValue;
  celsius_t maxCValue = celsius_t::_maxValue;
#elif ENABLED(FLOATING_POINT_CELSIUS)
  celsius_t minCValue = -273.15f;
  celsius_t maxCValue = 32767.0f;
#else
  celsius_t minCValue = -274;
  celsius_t maxCValue = 32767;
#endif
