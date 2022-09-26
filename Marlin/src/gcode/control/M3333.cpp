/**
 * Refill control feature
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

#include "../gcode.h"
#include "../../MarlinCore.h"   // for pin_is_protected
#include "../../inc/MarlinConfig.h"

/**
* M3333: Turn on Refill Control Pin
* S State on or off (1 on 0 off)
*/

void GcodeSuite::M3333()
{
    const pin_t pin = REFILL_CONTROL_PIN;
    pinMode(pin,OUTPUT);

    if (!parser.seenval('S')) return; 
    const int on_off =  parser.intval('S',0); 
    if (on_off == 1){
        extDigitalWrite(pin, HIGH);
    } else if (on_off == 0){
        extDigitalWrite(pin,LOW);
    } else {
        return;
    } 
}