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

/**
 * gcode/temp/M140_M190.cpp
 *
 * Bed target temperature control
 */

#include "../../inc/MarlinConfig.h"

#if HAS_HEATED_BED

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../lcd/marlinui.h"

/**
 * M140 - Set Bed Temperature target and return immediately
 * M190 - Set Bed Temperature target and wait
 *
 * Parameters
 *  S<target> : The target temperature in current units. Wait for heating only.
 *  A<index>  : (BED_ZONES) Zone index (0-based). Sets the target for that one zone only.
 *  I<index>  : (No BED_ZONES) Preset index (if material presets are defined).
 *  K<mask>   : (BED_ZONES) Bitmask of zones to target. Bit 0 = zone 0, bit 1 = zone 1, etc.
 *              Overrides the persistent active zone mask for this command only.
 *
 * M190 Parameters
 *  R<target> : The target temperature in current units. Wait for heating and cooling.
 *
 * Examples
 *  M140 S60        : Set target to 60° for all active zones and return right away.
 *  M190 R40        : Set target to 40°. Wait until all active zones reach 40°.
 *  M140 S80 A2     : (BED_ZONES) Set zone 2 only to 80° and return.
 *  M190 S60 K5     : (BED_ZONES) Set zones 0 and 2 (0b0101) to 60° and wait.
 *  M140 S0  K15    : (BED_ZONES) Turn off all 4 zones (0b1111).
 *
 * With PRINTJOB_TIMER_AUTOSTART turning on heaters will start the print job timer
 *  (used by printingIsActive, etc.) and turning off heaters will stop the timer.
 *
 * With BED_ANNEALING_GCODE:
 *
 * M190 Parameters
 *     T<seconds>: Cooldown time, for more gradual cooling. Use with R parameter.
 *                 M190 R T - Cool the bed down over a given period of time.
 *
 * Examples
 *  M190 R70 T600: Cool down to 70°C over a period of ten minutes.
 *
 */
void GcodeSuite::M140_M190(const bool isM190) {

  if (DEBUGGING(DRYRUN)) return;

  bool got_temp = false;
  celsius_t temp = 0;

  // Accept 'I' for preheat preset index when BED_ZONES is not active
  #if HAS_PREHEAT && !HAS_BED_ZONES
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].bed_temp;
    }
  #endif

  // Get the temperature from 'S' or 'R'
  bool no_wait_for_cooling = false;
  if (!got_temp) {
    no_wait_for_cooling = parser.seenval('S');
    got_temp = no_wait_for_cooling || (isM190 && parser.seenval('R'));
    if (got_temp) temp = parser.value_celsius();
  }

  if (!got_temp) return;

  // Resolve zone mask: K<bitmask> overrides, A<index> selects one area/zone,
  // otherwise use the persistent active mask.
  #if HAS_BED_ZONES
    uint16_t zmask;
    if (parser.seenval('K'))
      zmask = (uint16_t)parser.value_ulong() & ((1U << BED_ZONES_COUNT) - 1U);
    else if (parser.seenval('A'))
      zmask = 1U << (_MIN((uint8_t)parser.value_byte(), (uint8_t)(BED_ZONES_COUNT - 1)));
    else
      zmask = thermalManager.bed_zone_mask;
  #endif

  #if ENABLED(BED_ANNEALING_GCODE)
    const bool anneal = isM190 && !no_wait_for_cooling && parser.seenval('T');
    const millis_t anneal_ms = anneal ? parser.value_millis_from_seconds() : 0UL;
  #else
    constexpr bool anneal = false;
  #endif

  if (!anneal) {
    #if HAS_BED_ZONES
      thermalManager.setTargetBed(temp, zmask);
    #else
      thermalManager.setTargetBed(temp);
    #endif
    thermalManager.isHeatingBed() ? LCD_MESSAGE(MSG_BED_HEATING) : LCD_MESSAGE(MSG_BED_COOLING);
  }

  // With PRINTJOB_TIMER_AUTOSTART, M190 can start the timer, and M140 can stop it
  TERN_(PRINTJOB_TIMER_AUTOSTART, thermalManager.auto_job_check_timer(isM190, !isM190));

  if (isM190) {
    #if ENABLED(BED_ANNEALING_GCODE)
      if (anneal) {
        LCD_MESSAGE(MSG_BED_ANNEALING);
        const millis_t wait_ms = anneal_ms / (thermalManager.degBed() - temp);
        // Loop from current temp down to the target
        for (celsius_t cool_temp = thermalManager.degBed() - 1; cool_temp >= temp; --cool_temp) {
          #if HAS_BED_ZONES
            thermalManager.setTargetBed(cool_temp, zmask);
          #else
            thermalManager.setTargetBed(cool_temp);
          #endif
          dwell(wait_ms);   // Wait while going to the next degree
        }
        return;
      }
    #endif

    thermalManager.wait_for_bed(no_wait_for_cooling);

    #if ENABLED(REMAINING_TIME_AUTOPRIME)
      if (card.isStillPrinting()) {
        print_job_timer.primeRemainingTimeEstimate(card.getIndex(), card.getFileSize());
        //SERIAL_ECHOLN(F("M190 - Prime Remaining Time Estimate: "), print_job_timer.duration(), C(' '), card.getIndex(), C(' '), card.getFileSize() - card.getIndex());
      }
    #endif
  }
  else {
    ui.set_status_reset_fn([]{
      const celsius_t c = thermalManager.degTargetBed();
      return c < 30 || thermalManager.degBedNear(c);
    });
  }
}

#endif // HAS_HEATED_BED
