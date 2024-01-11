/**
 * Bed Level Tools for Pro UI
 * Extended by: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 5.1.0
 * Date: 2023/08/22
 *
 * Based on the original work of: Henri-J-Norden
 * https://github.com/Jyers/Marlin/pull/126
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

#include "../../inc/MarlinConfigPre.h"

#define UBL_Z_OFFSET_MIN -3.0
#define UBL_Z_OFFSET_MAX  3.0

class BedLevelTools {
public:
  static bool goto_mesh_value;
  static uint8_t mesh_x, mesh_y;
  static uint8_t tilt_grid;

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    static void manualValueUpdate(const uint8_t mesh_x, const uint8_t mesh_y, bool undefined=false);
    static bool createPlaneFromMesh();
  #else
    static void manualValueUpdate(const uint8_t mesh_x, const uint8_t mesh_y);
  #endif
  static void manualMove(const uint8_t mesh_x, const uint8_t mesh_y, bool zmove=false);
  static void moveToXYZ();
  static void moveToXY();
  static void moveToZ();
  static void probeXY();
  static void meshReset();
  static bool meshValidate();
};

extern BedLevelTools bedLevelTools;
