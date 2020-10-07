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

/* DGUS implementation written by Sebastiaan Dammann in 2020 for Marlin */

#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"

#include "../../../../ultralcd.h"
#include "../../../ui_api.h"

#include "PageHandlers.h"

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 10;
#endif
using namespace ExtUI;

const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

// ----- Which variables to auto-update on which screens
const uint16_t VPList_None[] PROGMEM = {
  0x0000
};

const uint16_t VPList_Main[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif
  0x0000
};

const uint16_t VPList_File1[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif


  0x0000
};

const uint16_t VPList_Control[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif

  VP_LED_TOGGLE,

  0x0000
};

const uint16_t VPList_Temp[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif

  VP_LED_TOGGLE,

  0x0000
};

const uint16_t VPList_PrintPausingError[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif


  0x0000
};

const uint16_t VPList_PrintScreen[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  /*VP_XPos, VP_YPos,*/ VP_ZPos,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif


  0x0000
};

// Toggle button handler
void DGUSCrealityDisplay_HandleToggleButton(DGUS_VP_Variable &var, void *val_ptr) {
  switch (*(uint16_t*)var.memadr) {
    case ICON_TOGGLE_ON:
      *((bool*)var.memadr) = true;
      break;

    case ICON_TOGGLE_OFF:
      *((bool*)var.memadr) = true;
      break;
  }
}

void DGUSCrealityDisplay_SendToggleButton(DGUS_VP_Variable &var) {
  dgusdisplay.WriteVariable(var.VP, *(bool*)var.memadr ? ICON_TOGGLE_ON : ICON_TOGGLE_OFF);
}

// -- Mapping from screen to variable list
const struct VPMapping VPMap[] PROGMEM = {
  { DGUSLCD_SCREEN_BOOT, VPList_None },
  { DGUSLCD_SCREEN_MAIN, VPList_Main },

  { DGUSLCD_SCREEN_SDFILELIST_1, VPList_File1 },
  { DGUSLCD_SCREEN_SDFILELIST_2, VPList_File1 },
  { DGUSLCD_SCREEN_SDFILELIST_3, VPList_File1 },
  { DGUSLCD_SCREEN_SDFILELIST_4, VPList_File1 },
  { DGUSLCD_SCREEN_SDFILELIST_5, VPList_File1 },

  { DGUSLCD_SCREEN_FILAMENTRUNOUT1, VPList_PrintPausingError },
  { DGUSLCD_SCREEN_FILAMENTRUNOUT2, VPList_PrintPausingError },

  { DGUSLCD_SCREEN_PRINT_FINISH, VPList_PrintScreen },
  { DGUSLCD_SCREEN_PRINT_RUNNING, VPList_PrintScreen },
  { DGUSLCD_SCREEN_PRINT_PAUSED, VPList_PrintScreen },

  { DGUSLCD_SCREEN_TUNING, VPList_PrintScreen },
  { DGUSLCD_SCREEN_PREPARE, VPList_PrintScreen },

  { DGUSLCD_SCREEN_MOVE1MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE10MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE01MM, VPList_PrintScreen },

  { DGUSLCD_SCREEN_FEED, VPList_PrintScreen },
  { DGUSLCD_SCREEN_CONTROL, VPList_Control },

  { DGUSLCD_SCREEN_TEMP, VPList_Temp },
  { DGUSLCD_SCREEN_TEMP_PLA, VPList_PrintScreen },
  { DGUSLCD_SCREEN_TEMP_ABS, VPList_PrintScreen },

  { DGUSLCD_SCREEN_INFO, VPList_PrintScreen },
  { DGUSLCD_SCREEN_ZOFFSET_LEVEL, VPList_PrintScreen },
  { DGUSLCD_SCREEN_LEVELING, VPList_PrintScreen },

  { DGUSLCD_SCREEN_POWER_LOSS, VPList_None },
  { DGUSLCD_SCREEN_THERMAL_RUNAWAY, VPList_None },
  { DGUSLCD_SCREEN_HEATING_FAILED, VPList_None },
  { DGUSLCD_SCREEN_THERMISTOR_ERROR, VPList_None },

  { DGUSLCD_SCREEN_AUTOHOME, VPList_None },

  { DGUSLCD_SCREEN_DIALOG_PAUSE, VPList_None },
  { DGUSLCD_SCREEN_DIALOG_STOP, VPList_None },

  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

// Helper to define a DGUS_VP_Variable for common use cases.
#define VPHELPER(VPADR, VPADRVAR, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=sizeof(VPADRVAR), \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable when the sizeo of the var cannot be determined automaticalyl (eg. a string)
#define VPHELPER_STR(VPADR, VPADRVAR, STRLEN, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=STRLEN, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // TODO:

  #if HOTENDS >= 1
    VPHELPER(VP_T_E0_Is, &thermalManager.temp_hotend[0].celsius, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_E0_Set, &thermalManager.temp_hotend[0].target, ScreenHandler.HandleTemperatureChanged, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  #if HAS_HEATED_BED
    VPHELPER(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_Bed_Set, &thermalManager.temp_bed.target, ScreenHandler.HandleTemperatureChanged, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  // Feedrate
  VPHELPER(VP_Feedrate_Percentage, &feedrate_percentage, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay ),

  // Position Data
  //VPHELPER(VP_XPos, &current_position.x, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  //VPHELPER(VP_YPos, &current_position.y, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  VPHELPER(VP_ZPos, &current_position.z, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<2>),

  VPHELPER(VP_CONFIRMED, nullptr, ScreenHandler.ScreenConfirmedOK, nullptr),

  // Creality has the same button ID mapped all over the place, so let the generic handler figure it out
  VPHELPER(VP_BUTTON_MAINENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_ADJUSTENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_PREPAREENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),

  // Creality toggle buttons
  VPHELPER(VP_BUTTON_PREPAREENTERKEY, &LEDStatus, DGUSCrealityDisplay_HandleToggleButton, DGUSCrealityDisplay_SendToggleButton),

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_ORIGIN
