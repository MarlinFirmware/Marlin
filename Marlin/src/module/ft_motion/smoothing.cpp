/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(FTM_SMOOTHING)

#include "smoothing.h"

// Set smoothing time and recalculate alpha and delay.
void AxisSmoothing::set_smoothing_time(const float s_time) {
  if (s_time > 0.001f) {
    alpha = 1.0f - expf(-(FTM_TS) * (FTM_SMOOTHING_ORDER) / s_time );
    delay_samples = s_time * FTM_FS;
  }
  else {
    alpha = 0.0f;
    delay_samples = 0;
  }
}

#endif // FTM_SMOOTHING
