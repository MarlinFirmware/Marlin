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

//
// Backlash Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_MARLINUI_MENU, BACKLASH_GCODE)

#include "menu_item.h"

#include "../../feature/backlash.h"

void menu_backlash() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  editable.uint8 = backlash.get_correction_uint8();
  EDIT_ITEM_FAST(percent, MSG_BACKLASH_CORRECTION, &editable.uint8, backlash.all_off, backlash.all_on, []{ backlash.set_correction_uint8(editable.uint8); });

  #if DISABLED(CORE_BACKLASH) || EITHER(MARKFORGED_XY, MARKFORGED_YX)
    #define _CAN_CALI AXIS_CAN_CALIBRATE
  #else
    #define _CAN_CALI(A) true
  #endif

  #define EDIT_BACKLASH_DISTANCE(N) do { \
    editable.decimal = backlash.get_distance_mm(_AXIS(N)); \
    EDIT_ITEM_FAST(float43, MSG_BACKLASH_##N, &editable.decimal, 0.0f, 9.9f, []{ backlash.set_distance_mm(_AXIS(N), editable.decimal); }); \
  } while (0);

  if (_CAN_CALI(A)) EDIT_BACKLASH_DISTANCE(A);
  #if HAS_Y_AXIS && _CAN_CALI(B)
    EDIT_BACKLASH_DISTANCE(B);
  #endif
  #if HAS_Z_AXIS && _CAN_CALI(C)
    EDIT_BACKLASH_DISTANCE(C);
  #endif
  #if HAS_I_AXIS && _CAN_CALI(I)
    EDIT_BACKLASH_DISTANCE(I);
  #endif
  #if HAS_J_AXIS && _CAN_CALI(J)
    EDIT_BACKLASH_DISTANCE(J);
  #endif
  #if HAS_K_AXIS && _CAN_CALI(K)
    EDIT_BACKLASH_DISTANCE(K);
  #endif
  #if HAS_U_AXIS && _CAN_CALI(U)
    EDIT_BACKLASH_DISTANCE(U);
  #endif
  #if HAS_V_AXIS && _CAN_CALI(V)
    EDIT_BACKLASH_DISTANCE(V);
  #endif
  #if HAS_W_AXIS && _CAN_CALI(W)
    EDIT_BACKLASH_DISTANCE(W);
  #endif

  #ifdef BACKLASH_SMOOTHING_MM
    editable.decimal = backlash.get_smoothing_mm();
    EDIT_ITEM_FAST(float43, MSG_BACKLASH_SMOOTHING, &editable.decimal, 0.0f, 9.9f, []{ backlash.set_smoothing_mm(editable.decimal); });
  #endif

  END_MENU();
}

#endif // HAS_MARLINUI_MENU && BACKLASH_GCODE
