/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifndef __DELTA_AUTO_CAL_H__
#define __DELTA_AUTO_CAL_H__

constexpr uint8_t _7P_STEP = 1,              // 7-point step - to change number of calibration points
                  _4P_STEP = _7P_STEP * 2,   // 4-point step
                  NPP      = _7P_STEP * 6;   // number of calibration points on the radius
enum CalEnum {                               // the 7 main calibration points - add definitions if needed
  CEN      = 0,
  __A      = 1,
  _AB      = __A + _7P_STEP,
  __B      = _AB + _7P_STEP,
  _BC      = __B + _7P_STEP,
  __C      = _BC + _7P_STEP,
  _CA      = __C + _7P_STEP,
};

extern float cal_ref;

#define LOOP_CAL_PT(VAR, S, N) for (uint8_t VAR=S; VAR<=NPP; VAR+=N)
#define F_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR<NPP+0.9999; VAR+=N)
#define I_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR>CEN+0.9999; VAR-=N)
#define LOOP_CAL_ALL(VAR) LOOP_CAL_PT(VAR, CEN, 1)
#define LOOP_CAL_RAD(VAR) LOOP_CAL_PT(VAR, __A, _7P_STEP)
#define LOOP_CAL_ACT(VAR, _4P, _OP) LOOP_CAL_PT(VAR, _OP ? _AB : __A, _4P ? _4P_STEP : _7P_STEP)

void refresh_auto_cal_ref(const float z_shift);

#endif // __DELTA_AUTO_CAL_H__
