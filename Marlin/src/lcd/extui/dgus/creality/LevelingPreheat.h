/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * lcd/extui/dgus/creality/LevelingPreheat.h
 *
 * Heat the bed and nozzle before homing with the probe or leveling, and wait for
 * the heat soak. Marlin's own preheat only starts at G29, after G28 has already
 * homed on a cold bed, and the bed and nozzle change shape and size as they warm up.
 * The temperatures are those of the Leveling Settings screen (PREHEAT_BEFORE_LEVELING)
 * and the soak time is LEVELING_HEAT_SOAK_TIME. This waits from the screen's loop(), not
 * inside a G-code, so the printer stays responsive and the heat-up can be canceled.
 */

#include "../../../../inc/MarlinConfig.h" // HAS_HEATED_BED needs the full config

#if ALL(PREHEAT_BEFORE_LEVELING, HAS_HEATED_BED)
  #define CREALITY_LEVEL_PREHEAT 1
#endif

#if CREALITY_LEVEL_PREHEAT

#include "../../../../core/types.h"

class LevelingPreheat {
public:
  // Heat up and soak if needed, then inject the commands. If everything is already at
  // temperature (and so already soaked) the commands are injected at once.
  // With popup, the wait is shown on the Confirm page (for screens with no message field).
  static void startThen(FSTR_P const cmds, const bool popup=false);

  // Cancel on the wait page. Returns true if that is what the Cancel button was for.
  static bool handleCancel();

  // Called from the screen's loop()
  static void loop();

  // Stop waiting. The heaters are left alone.
  static void cancel();

  static bool isActive() { return state != IDLE; }

private:
  enum State : uint8_t { IDLE, HEATING, SOAKING };
  static State state;
  static FSTR_P commands;
  static bool use_popup;
  static void message(const char * const text);
  static void finish();
  static millis_t soak_end_ms, next_message_ms;
};

#endif // CREALITY_LEVEL_PREHEAT

