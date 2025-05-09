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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(LIN_ADVANCE)

#include "../../gcode.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"

#if ENABLED(ADVANCE_K_EXTRA)
  float other_extruder_advance_K[EXTRUDERS];
  uint8_t lin_adv_slot = 0;
#endif

/**
 * M900: Get or Set Linear Advance K-factor
 *  T<tool>     Which tool to address
 *  K<factor>   Set current advance K factor (aka Slot 0).
 *
 * With ADVANCE_K_EXTRA:
 *  S<0/1>      Activate slot 0 or 1.
 *  L<factor>   Set secondary advance K factor (Slot 1).
 */
void GcodeSuite::M900() {

  auto echo_value_oor = [](const char ltr, const bool ten=true) {
    SERIAL_CHAR('?', ltr);
    SERIAL_ECHOPGM(" value out of range");
    if (ten) SERIAL_ECHOPGM(" (0-10)");
    SERIAL_ECHOLNPGM(".");
  };

  #if EXTRUDERS < 2
    constexpr uint8_t tool_index = 0;
    UNUSED(tool_index);
  #else
    const uint8_t tool_index = parser.intval('T', active_extruder);
    if (tool_index >= EXTRUDERS) {
      echo_value_oor('T', false);
      return;
    }
  #endif

  const float oldK = planner.get_advance_k(tool_index);
  float newK = oldK;

  #if ENABLED(SMOOTH_LIN_ADVANCE)
    const float oldU = stepper.get_advance_tau(tool_index);
    float newU = oldU;
  #endif

  #if ENABLED(ADVANCE_K_EXTRA)

    float &lref = other_extruder_advance_K[tool_index];

    const bool old_slot = TEST(lin_adv_slot, tool_index), // Each tool uses 1 bit to store its current slot (0 or 1)
               new_slot = parser.boolval('S', old_slot);  // The new slot (0 or 1) to set for the tool (default = no change)

    // If a new slot is being selected swap the current and
    // saved K values. Do here so K/L will apply correctly.
    if (new_slot != old_slot) {                       // Not the same slot?
      SET_BIT_TO(lin_adv_slot, tool_index, new_slot); // Update the slot for the tool
      newK = lref;                                    // Get the backup K value (to apply below)
      lref = oldK;                                    // Back up the active K value
    }

    // Set the main K value. Apply if the main slot is active.
    if (parser.seenval('K')) {
      const float K = parser.value_float();
      if (!WITHIN(K, 0, 10)) echo_value_oor('K');
      else if (new_slot)        lref = K;             // S1 Knn (set main K in its backup slot)
      else                      newK = K;             // S0 Knn (use main K now)
    }

    // Set the extra K value. Apply if the extra slot is active.
    if (parser.seenval('L')) {
      const float L = parser.value_float();
      if (!WITHIN(L, 0, 10)) echo_value_oor('L');
      else if (!new_slot)       lref = L;             // S0 Lnn (set extra K in its backup slot)
      else                      newK = L;             // S1 Lnn (use extra K now)
    }

  #else

    if (parser.seenval('K')) {
      const float K = parser.value_float();
      if (WITHIN(K, 0, 10))
        newK = K;
      else
        echo_value_oor('K');
    }

  #endif

  #if ENABLED(SMOOTH_LIN_ADVANCE)
    if (parser.seenval('U')) {
      const float tau = parser.value_float();
      if (WITHIN(tau, 0.0f, 0.5f))
        newU = tau;
      else
        echo_value_oor('U');
    }
  #endif

  if (newK != oldK || TERN0(SMOOTH_LIN_ADVANCE, newU != oldU)) {
    planner.synchronize();
    if (newK != oldK) planner.set_advance_k(newK, tool_index);
    #if ENABLED(SMOOTH_LIN_ADVANCE)
      if (newU != oldU) stepper.set_advance_tau(newU, tool_index);
    #endif
  }

  if (!parser.seen_any()) {

    #if ENABLED(ADVANCE_K_EXTRA)

      #if DISABLED(DISTINCT_E_FACTORS)
        SERIAL_ECHOLNPGM("Advance S", new_slot, " K", newK, "(S", !new_slot, " K", lref, ")");
      #else
        EXTRUDER_LOOP() {
          const bool slot = TEST(lin_adv_slot, e);
          SERIAL_ECHOLNPGM("Advance T", e, " S", slot, " K", planner.get_advance_k(e),
                           "(S", !slot, " K", other_extruder_advance_K[e], ")");
        }
      #endif

    #else // !ADVANCE_K_EXTRA

      SERIAL_ECHO_START();
      #if DISABLED(DISTINCT_E_FACTORS)
        SERIAL_ECHOPGM("Advance K=", planner.get_advance_k());
        #if ENABLED(SMOOTH_LIN_ADVANCE)
          SERIAL_ECHOPGM(" TAU=", stepper.get_advance_tau());
        #endif
        SERIAL_EOL();
      #else
        SERIAL_ECHOPGM("Advance K");
        EXTRUDER_LOOP() SERIAL_ECHO(C(' '), C('0' + e), C(':'), planner.get_advance_k(e));
        SERIAL_EOL();
        #if ENABLED(SMOOTH_LIN_ADVANCE)
          SERIAL_ECHOPGM("Advance TAU");
          EXTRUDER_LOOP() SERIAL_ECHO(C(' '), C('0' + e), C(':'), stepper.get_advance_tau(e));
          SERIAL_EOL();
        #endif
      #endif

    #endif // !ADVANCE_K_EXTRA
  }

}

void GcodeSuite::M900_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading(forReplay, F(STR_LINEAR_ADVANCE));
  DISTINCT_E_LOOP() {
    report_echo_start(forReplay);
    SERIAL_ECHOPGM(
      #if ENABLED(DISTINCT_E_FACTORS)
        "  M900 T", e, " K"
      #else
        "  M900 K"
      #endif
    );
    SERIAL_ECHO(planner.get_advance_k(e));
    #if ENABLED(SMOOTH_LIN_ADVANCE)
      SERIAL_ECHOPGM(" U", stepper.get_advance_tau(e));
    #endif
    SERIAL_EOL();
  }
}

#endif // LIN_ADVANCE
