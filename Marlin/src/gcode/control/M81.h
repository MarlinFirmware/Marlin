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

/**
 * M81: Turn off Power, including Power Supply, if there is one.
 *
 *      This code should ALWAYS be available for EMERGENCY SHUTDOWN!
 */
void gcode_M81() {
  thermalManager.disable_all_heaters();
  stepper.finish_and_disable();

  #if FAN_COUNT > 0
    for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
    #if ENABLED(PROBING_FANS_OFF)
      fans_paused = false;
      ZERO(paused_fanSpeeds);
    #endif
  #endif

  safe_delay(1000); // Wait 1 second before switching off

  #if HAS_SUICIDE
    stepper.synchronize();
    suicide();
  #elif HAS_POWER_SWITCH
    OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
    powersupply_on = false;
  #endif

  #if ENABLED(ULTIPANEL)
    LCD_MESSAGEPGM(MACHINE_NAME " " MSG_OFF ".");
  #endif
}
