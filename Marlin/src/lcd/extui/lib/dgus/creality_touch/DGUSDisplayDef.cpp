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

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 10;
#endif
using namespace ExtUI;

const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

const struct VPMapping VPMap[] PROGMEM = {
  // TODO
};

// Helper to define a DGUS_VP_Variable for common use cases.
#define VPHELPER(VPADR, VPADRVAR, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=sizeof(VPADRVAR), \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable when the sizeo of the var cannot be determined automaticalyl (eg. a string)
#define VPHELPER_STR(VPADR, VPADRVAR, STRLEN, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=STRLEN, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // TODO:

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_ORIGIN
