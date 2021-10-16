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
 * power.cpp - power control
 */

#include "../inc/MarlinConfig.h"

#include "power.h"
#include "../module/stepper.h"
#include "../MarlinCore.h"

#if ENABLED(PS_OFF_SOUND)
  #include "../libs/buzzer.h"
#endif

#if defined(PSU_POWERUP_GCODE) || defined(PSU_POWEROFF_GCODE)
  #include "../gcode/gcode.h"
#endif

#if EITHER(PSU_CONTROL, AUTO_POWER_CONTROL)

Power powerManager;
bool Power::psu_on;

#if ENABLED(AUTO_POWER_CONTROL)
  #include "../module/temperature.h"

  #if BOTH(USE_CONTROLLER_FAN, AUTO_POWER_CONTROLLERFAN)
    #include "controllerfan.h"
  #endif

  millis_t Power::lastPowerOn;
#endif

/**
 * Initialize pins & state for the power manager.
 *
 */
void Power::init() {
  psu_on = ENABLED(PSU_DEFAULT_OFF);              // Set opposite state to get full power_off/on
  TERN(PSU_DEFAULT_OFF, power_off(), power_on());
}

/**
 * Power on if the power is currently off.
 * Restores stepper drivers and processes any PSU_POWERUP_GCODE.
 *
 */
void Power::power_on() {
  #if ENABLED(AUTO_POWER_CONTROL)
    const millis_t now = millis();
    lastPowerOn = now + !now;
  #endif

  if (psu_on) return;

  OUT_WRITE(PS_ON_PIN, PSU_ACTIVE_STATE);
  psu_on = true;
  safe_delay(PSU_POWERUP_DELAY);
  restore_stepper_drivers();
  TERN_(HAS_TRINAMIC_CONFIG, safe_delay(PSU_POWERUP_DELAY));

  #ifdef PSU_POWERUP_GCODE
    GcodeSuite::process_subcommands_now_P(PSTR(PSU_POWERUP_GCODE));
  #endif
}

/**
 * Power off if the power is currently on.
 * Processes any PSU_POWEROFF_GCODE and makes a PS_OFF_SOUND if enabled.
 *
 */
void Power::power_off() {
  if (!psu_on) return;

  #ifdef PSU_POWEROFF_GCODE
    GcodeSuite::process_subcommands_now_P(PSTR(PSU_POWEROFF_GCODE));
  #endif

  #if ENABLED(PS_OFF_SOUND)
    BUZZ(1000, 659);
  #endif

  OUT_WRITE(PS_ON_PIN, !PSU_ACTIVE_STATE);
  psu_on = false;
}


#if ENABLED(AUTO_POWER_CONTROL)

  #ifndef POWER_TIMEOUT
    #define POWER_TIMEOUT 0
  #endif

  /**
   * Check all conditions that would signal power needing to be on.
   *
   * @returns bool  if power is needed
   */
  bool Power::is_power_needed() {

    // If any of the stepper drivers are enabled...
    if (stepper.axis_enabled.bits) return true;

    if (printJobOngoing() || printingIsPaused()) return true;

    #if ENABLED(AUTO_POWER_FANS)
      FANS_LOOP(i) if (thermalManager.fan_speed[i]) return true;
    #endif

    #if ENABLED(AUTO_POWER_E_FANS)
      HOTEND_LOOP() if (thermalManager.autofan_speed[e]) return true;
    #endif

    #if BOTH(USE_CONTROLLER_FAN, AUTO_POWER_CONTROLLERFAN)
      if (controllerFan.state()) return true;
    #endif

    if (TERN0(AUTO_POWER_CHAMBER_FAN, thermalManager.chamberfan_speed))
      return true;

    if (TERN0(AUTO_POWER_COOLER_FAN, thermalManager.coolerfan_speed))
      return true;

    #if HAS_HOTEND
      HOTEND_LOOP() if (thermalManager.degTargetHotend(e) > 0 || thermalManager.temp_hotend[e].soft_pwm_amount > 0) return true;
    #endif

    if (TERN0(HAS_HEATED_BED, thermalManager.degTargetBed() > 0 || thermalManager.temp_bed.soft_pwm_amount > 0)) return true;

    #if HAS_HOTEND && AUTO_POWER_E_TEMP
      HOTEND_LOOP() if (thermalManager.degHotend(e) >= (AUTO_POWER_E_TEMP)) return true;
    #endif

    #if HAS_HEATED_CHAMBER && AUTO_POWER_CHAMBER_TEMP
      if (thermalManager.degChamber() >= (AUTO_POWER_CHAMBER_TEMP)) return true;
    #endif

    #if HAS_COOLER && AUTO_POWER_COOLER_TEMP
      if (thermalManager.degCooler() >= (AUTO_POWER_COOLER_TEMP)) return true;
    #endif

    return false;
  }

  /**
   * Check if we should power off automatically (POWER_TIMEOUT elapsed, !is_power_needed).
   *
   * @param pause  pause the 'timer'
   */
  void Power::check(const bool pause) {
    static millis_t nextPowerCheck = 0;
    const millis_t now = millis();
    #if POWER_TIMEOUT > 0
      static bool _pause = false;
      if (pause != _pause) {
        lastPowerOn = now + !now;
        _pause = pause;
      }
      if (pause) return;
    #endif
    if (ELAPSED(now, nextPowerCheck)) {
      nextPowerCheck = now + 2500UL;
      if (is_power_needed())
        power_on();
      else if (!lastPowerOn || (POWER_TIMEOUT > 0 && ELAPSED(now, lastPowerOn + SEC_TO_MS(POWER_TIMEOUT))))
        power_off();
    }
  }

  #if POWER_OFF_DELAY > 0

    /**
     * Power off with a delay. Power off is triggered by check() after the delay.
     *
     */
    void Power::power_off_soon() {
      lastPowerOn = millis() - SEC_TO_MS(POWER_TIMEOUT) + SEC_TO_MS(POWER_OFF_DELAY);
    }

  #endif

#endif // AUTO_POWER_CONTROL

#endif // PSU_CONTROL || AUTO_POWER_CONTROL
