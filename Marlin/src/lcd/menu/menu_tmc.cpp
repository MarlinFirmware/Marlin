/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// TMC Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_TRINAMIC && HAS_LCD_MENU

#include "menu.h"
#include "../../module/stepper/indirection.h"
#include "../../feature/tmc_util.h"

#define TMC_EDIT_STORED_I_RMS(ST,MSG) EDIT_ITEM_P(uint16_4, PSTR(MSG), &stepper##ST.val_mA, 100, 3000, [](){ stepper##ST.refresh_stepper_current(); })

void menu_tmc_current() {
  START_MENU();
  BACK_ITEM(MSG_TMC_DRIVERS);
  #if AXIS_IS_TMC(X)
    TMC_EDIT_STORED_I_RMS(X, MSG_X);
  #endif
  #if AXIS_IS_TMC(Y)
    TMC_EDIT_STORED_I_RMS(Y, MSG_Y);
  #endif
  #if AXIS_IS_TMC(Z)
    TMC_EDIT_STORED_I_RMS(Z, MSG_Z);
  #endif
  #if AXIS_IS_TMC(X2)
    TMC_EDIT_STORED_I_RMS(X2, MSG_X2);
  #endif
  #if AXIS_IS_TMC(Y2)
    TMC_EDIT_STORED_I_RMS(Y2, MSG_Y2);
  #endif
  #if AXIS_IS_TMC(Z2)
    TMC_EDIT_STORED_I_RMS(Z2, MSG_Z2);
  #endif
  #if AXIS_IS_TMC(Z3)
    TMC_EDIT_STORED_I_RMS(Z3, MSG_Z3);
  #endif
  #if AXIS_IS_TMC(E0)
    TMC_EDIT_STORED_I_RMS(E0, LCD_STR_E0);
  #endif
  #if AXIS_IS_TMC(E1)
    TMC_EDIT_STORED_I_RMS(E1, LCD_STR_E1);
  #endif
  #if AXIS_IS_TMC(E2)
    TMC_EDIT_STORED_I_RMS(E2, LCD_STR_E2);
  #endif
  #if AXIS_IS_TMC(E3)
    TMC_EDIT_STORED_I_RMS(E3, LCD_STR_E3);
  #endif
  #if AXIS_IS_TMC(E4)
    TMC_EDIT_STORED_I_RMS(E4, LCD_STR_E4);
  #endif
  #if AXIS_IS_TMC(E5)
    TMC_EDIT_STORED_I_RMS(E5, LCD_STR_E5);
  #endif
  END_MENU();
}

#if ENABLED(HYBRID_THRESHOLD)

  #define TMC_EDIT_STORED_HYBRID_THRS(ST, MSG) EDIT_ITEM_P(uint8, PSTR(MSG), &stepper##ST.stored.hybrid_thrs, 0, 255, [](){ stepper##ST.refresh_hybrid_thrs(); });

  void menu_tmc_hybrid_thrs() {
    START_MENU();
    BACK_ITEM(MSG_TMC_DRIVERS);
    #if AXIS_HAS_STEALTHCHOP(X)
      TMC_EDIT_STORED_HYBRID_THRS(X, MSG_X);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      TMC_EDIT_STORED_HYBRID_THRS(Y, MSG_Y);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      TMC_EDIT_STORED_HYBRID_THRS(Z, MSG_Z);
    #endif
    #if AXIS_HAS_STEALTHCHOP(X2)
      TMC_EDIT_STORED_HYBRID_THRS(X2, MSG_X2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y2)
      TMC_EDIT_STORED_HYBRID_THRS(Y2, MSG_Y2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z2)
      TMC_EDIT_STORED_HYBRID_THRS(Z2, MSG_Z2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z3)
      TMC_EDIT_STORED_HYBRID_THRS(Z3, MSG_Z3);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E0)
      TMC_EDIT_STORED_HYBRID_THRS(E0, LCD_STR_E0);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E1)
      TMC_EDIT_STORED_HYBRID_THRS(E1, LCD_STR_E1);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E2)
      TMC_EDIT_STORED_HYBRID_THRS(E2, LCD_STR_E2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E3)
      TMC_EDIT_STORED_HYBRID_THRS(E3, LCD_STR_E3);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E4)
      TMC_EDIT_STORED_HYBRID_THRS(E4, LCD_STR_E4);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E5)
      TMC_EDIT_STORED_HYBRID_THRS(E5, LCD_STR_E5);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(SENSORLESS_HOMING)

  #define TMC_EDIT_STORED_SGT(ST) EDIT_ITEM_P(int4, PSTR(MSG_##ST), &stepper##ST.stored.homing_thrs, stepper##ST.sgt_min, stepper##ST.sgt_max, [](){ stepper##ST.refresh_homing_thrs(); });

  void menu_tmc_homing_thrs() {
    START_MENU();
    BACK_ITEM(MSG_TMC_DRIVERS);
    #if X_SENSORLESS
      TMC_EDIT_STORED_SGT(X);
    #endif
    #if X2_SENSORLESS
      TMC_EDIT_STORED_SGT(X2);
    #endif
    #if Y_SENSORLESS
      TMC_EDIT_STORED_SGT(Y);
    #endif
    #if Z_SENSORLESS
      TMC_EDIT_STORED_SGT(Z);
    #endif
    END_MENU();
  }

#endif

#if HAS_STEALTHCHOP

  #define TMC_EDIT_STEP_MODE(ST, MSG) EDIT_ITEM_P(bool, PSTR(MSG), &stepper##ST.stored.stealthChop_enabled, [](){ stepper##ST.refresh_stepping_mode(); })

  void menu_tmc_step_mode() {
    START_MENU();
    STATIC_ITEM(MSG_TMC_STEALTH_ENABLED);
    BACK_ITEM(MSG_TMC_DRIVERS);
    #if AXIS_HAS_STEALTHCHOP(X)
      TMC_EDIT_STEP_MODE(X, MSG_X);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      TMC_EDIT_STEP_MODE(Y, MSG_Y);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      TMC_EDIT_STEP_MODE(Z, MSG_Z);
    #endif
    #if AXIS_HAS_STEALTHCHOP(X2)
      TMC_EDIT_STEP_MODE(X2, MSG_X2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y2)
      TMC_EDIT_STEP_MODE(Y2, MSG_Y2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z2)
      TMC_EDIT_STEP_MODE(Z2, MSG_Z2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z3)
      TMC_EDIT_STEP_MODE(Z3, MSG_Z3);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E0)
      TMC_EDIT_STEP_MODE(E0, LCD_STR_E0);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E1)
      TMC_EDIT_STEP_MODE(E1, LCD_STR_E1);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E2)
      TMC_EDIT_STEP_MODE(E2, LCD_STR_E2);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E3)
      TMC_EDIT_STEP_MODE(E3, LCD_STR_E3);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E4)
      TMC_EDIT_STEP_MODE(E4, LCD_STR_E4);
    #endif
    #if AXIS_HAS_STEALTHCHOP(E5)
      TMC_EDIT_STEP_MODE(E5, LCD_STR_E5);
    #endif
    END_MENU();
  }

#endif

void menu_tmc() {
  START_MENU();
  BACK_ITEM(MSG_CONTROL);
  SUBMENU(MSG_TMC_CURRENT, menu_tmc_current);
  #if ENABLED(HYBRID_THRESHOLD)
    SUBMENU(MSG_TMC_HYBRID_THRS, menu_tmc_hybrid_thrs);
  #endif
  #if ENABLED(SENSORLESS_HOMING)
    SUBMENU(MSG_TMC_HOMING_THRS, menu_tmc_homing_thrs);
  #endif
  #if HAS_STEALTHCHOP
    SUBMENU(MSG_TMC_STEPPING_MODE, menu_tmc_step_mode);
  #endif
  END_MENU();
}

#endif // HAS_TRINAMIC
