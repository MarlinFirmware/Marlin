/**
 * Professional Firmware UI extensions menus
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 1.1.0
 * Date: 2023/11/2
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

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, PROUI_EX)

#include "menu_item.h"
#include "../../MarlinCore.h"
#include "menu_prouiex.h"

#if HAS_MESH

  void menu_mesh_settings() {
    START_MENU();
    BACK_ITEM(MSG_PROUIEX_SET);
    EDIT_ITEM(uint8, MSG_MESH_POINTS_X, &meshSet.grid_max_points_x, GRID_MIN, GRID_LIMIT, proUIEx.applyMeshLimits);
    EDIT_ITEM(uint8, MSG_MESH_POINTS_Y, &meshSet.grid_max_points_y, GRID_MIN, GRID_LIMIT, proUIEx.applyMeshLimits);
    EDIT_ITEM(uint16_3, MSG_MESH_MIN_X, &meshSet.mesh_min_x, MIN_MESH_INSET, MAX_MESH_INSET, proUIEx.applyMeshLimits);
    EDIT_ITEM(uint16_3, MSG_MESH_MAX_X, &meshSet.mesh_max_x, MIN_MESH_INSET, MAX_MESH_INSET, proUIEx.applyMeshLimits);
    EDIT_ITEM(uint16_3, MSG_MESH_MIN_Y, &meshSet.mesh_min_y, MIN_MESH_INSET, MAX_MESH_INSET, proUIEx.applyMeshLimits);
    EDIT_ITEM(uint16_3, MSG_MESH_MAX_Y, &meshSet.mesh_max_y, MIN_MESH_INSET, MAX_MESH_INSET, proUIEx.applyMeshLimits);
    ACTION_ITEM(MSG_MESH_AMAX, proUIEx.maxMeshArea);
    ACTION_ITEM(MSG_MESH_CENTER, proUIEx.centerMeshArea);
    END_MENU();
  }

#endif // HAS_MESH

void menu_park_position() {
  START_MENU();
  BACK_ITEM(MSG_PROUIEX_SET);
  EDIT_ITEM(int4, MSG_PARK_XPOSITION, &PRO_data.Park_point.x, X_MIN_POS, X_MAX_POS);
  EDIT_ITEM(int4, MSG_PARK_YPOSITION, &PRO_data.Park_point.y, Y_MIN_POS, Y_MAX_POS);
  EDIT_ITEM(int4, MSG_PARK_ZRAISE, &PRO_data.Park_point.z, Z_MIN_POS, 50);
  END_MENU();
}

void menu_advanced_prouiexset() {
  START_MENU();
  BACK_ITEM(MSG_ADVANCED_SETTINGS);
  EDIT_ITEM(int4, MSG_PHY_XMINPOS, &PRO_data.x_min_pos, -100, 100, proUIEx.applyPhySet);
  EDIT_ITEM(int4, MSG_PHY_YMINPOS, &PRO_data.y_min_pos, -100, 100, proUIEx.applyPhySet);
  EDIT_ITEM(int4, MSG_PHY_XMAXPOS, &PRO_data.x_max_pos, X_BED_MIN, 999, proUIEx.applyPhySet);
  EDIT_ITEM(int4, MSG_PHY_YMAXPOS, &PRO_data.y_max_pos, Y_BED_MIN, 999, proUIEx.applyPhySet);
  EDIT_ITEM(int4, MSG_PHY_ZMAXPOS, &PRO_data.z_max_pos, 100, 999, proUIEx.applyPhySet);
  EDIT_ITEM(uint16_3, MSG_PHY_XBEDSIZE, &PRO_data.x_bed_size, X_BED_MIN, X_MAX_POS, proUIEx.applyPhySet);
  EDIT_ITEM(uint16_3, MSG_PHY_YBEDSIZE, &PRO_data.y_bed_size, Y_BED_MIN, Y_MAX_POS, proUIEx.applyPhySet);
  #if ENABLED(NOZZLE_PARK_FEATURE)
    SUBMENU(MSG_FILAMENT_PARK_ENABLED, menu_park_position);
  #endif
  #if HAS_MESH
    SUBMENU(MSG_MESH_LEVELING, menu_mesh_settings);
  #endif
  END_MENU();
}

#endif