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

/**
 * Filament Carbonization Prevention
 * Prevent nozzle jam/clog as result from carbonized filament (which happens if the nozzle stay hot too long without extrusion) 
 * 
 * Logic:
 * Check if extruder have not moved during FCP_TIMEOUT(minutes), check if temperature is above FCP_MIN_TRIGGER(celsius)
 * if both conditions are true, then set nozzle and bed to their respective FCP_NOZZLE_TARGET and FCP_BED_TARGET
*/

#include "../inc/MarlinConfig.h"

#if ENABLED(FCP_ENABLE)

#include "../module/stepper/indirection.h"
#include "../module/temperature.h"
#include "../module/motion.h"
#include "../gcode/queue.h"
#include "../gcode/gcode.h"
#include "../gcode/parser.h"
static float old_position = 0;
unsigned long interval = FCP_TIMEOUT * 60000; // timeout in minutes to milisseconds
static unsigned long previousMillis = 0;

//for some reason beyond my understanding, the ELAPSED macro did not work duo overflows, so i used another method to
//compare time, if someone can make it work would be nice

void fcp_update(){
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= interval)    {
        if (old_position == (current_position.e))        {
            if ((thermalManager.degHotend(0)) >= FCP_MIN_TRIGGER || (thermalManager.degHotend(1)) >= FCP_MIN_TRIGGER)            {
                SERIAL_ECHOPGM("WARNING: FCP WATCHDOG TRIGGERED\n");
                gcode.process_subcommands_now_P(PSTR("M117 FCP WATCHDOG TRIG"));

                thermalManager.setTargetBed(FCP_BED_TARGET);
                thermalManager.setTargetHotend(FCP_NOZZLE_TARGET, 0);
                thermalManager.setTargetHotend(FCP_NOZZLE_TARGET, 1);
            }
        }
        old_position = (current_position.e);
        previousMillis = millis();
        //SERIAL_ECHOPGM("FCP WATCHDOG MONITORING\n");
    }
}

#endif // ENABLED(FCP_ENABLE)
