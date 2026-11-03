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

/**
 * feature/mmu/chameleon.cpp - 3D Chameleon Mk4 filament changer
 *
 * Button commands (hold time = pulses * CHAMELEON_PULSE_MS):
 *   1-4 pulses : Select T0-T3. A previously loaded filament is cut (Clippy servo),
 *                then a 2nd press unloads it and a 3rd press loads the new one.
 *   7 pulses   : Home the selector (no filament movement).
 */

#include "../../inc/MarlinConfig.h"

#if HAS_CHAMELEON

#include "chameleon.h"
#include "../../MarlinCore.h"
#include "../../gcode/gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/temperature.h"
#include "../../lcd/marlinui.h"

#if HAS_FILAMENT_SENSOR
  #include "../runout.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

Chameleon chameleon;

int8_t Chameleon::extruder = CHAMELEON_NO_TOOL;

// Button geometry
static constexpr AxisEnum button_axis = AxisEnum(CHAMELEON_BUTTON_AXIS);
static constexpr float axis_min[] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS },
                       axis_max[] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };

static_assert(CHAMELEON_BUTTON_AXIS >= X_AXIS && CHAMELEON_BUTTON_AXIS <= Z_AXIS, "CHAMELEON_BUTTON_AXIS must be X_AXIS, Y_AXIS, or Z_AXIS.");
static_assert(CHAMELEON_BUTTON_POS >= axis_min[button_axis] && CHAMELEON_BUTTON_POS <= axis_max[button_axis],
              "CHAMELEON_BUTTON_POS must be within the MIN_POS and MAX_POS of CHAMELEON_BUTTON_AXIS.");

// The standby position is CHAMELEON_PRESS_MM short of the button, toward the middle of the axis.
static constexpr float button_pressed_pos = CHAMELEON_BUTTON_POS,
                       button_standby_pos = button_pressed_pos + (button_pressed_pos > (axis_min[button_axis] + axis_max[button_axis]) * 0.5f ? -(CHAMELEON_PRESS_MM) : (CHAMELEON_PRESS_MM));

// Number of pulses for each command
static constexpr uint8_t CHAMELEON_HOME_PULSES = 7;

#ifdef CHAMELEON_TIP_SEQUENCE
  struct TipStep {
    int16_t temp;     // (°C) Wait for this hotend temperature before the move. 0 for no change.
    float   extrude;  // (mm) Relative extruder move
    float   feedrate; // (mm/min)
  };
#endif

// Run G-code, if any, and wait for it to finish
static void run_gcode(FSTR_P const fgcode) {
  gcode.process_subcommands_now(fgcode);
  planner.synchronize();
}

// A relative extruder move at the given feedrate (mm/min)
static void e_move(const float length, const float feedrate) {
  DEBUG_ECHOLNPGM("Chameleon E", length, " F", feedrate);
  motion.unscaled_e_move(length, MMM_TO_MMS(feedrate));
}

// Move the button axis to its pressed or standby position
void Chameleon::button(const bool pressed) {
  motion.position[button_axis] = pressed ? button_pressed_pos : button_standby_pos;
  motion.goto_current_position(MMM_TO_MMS(CHAMELEON_PRESS_FEEDRATE));
  planner.synchronize();
}

// Hold for a time while keeping the machine alive (same as G4)
void Chameleon::hold(const uint16_t ms) {
  DEBUG_ECHOLNPGM("Chameleon hold ", ms, "ms");
  gcode.dwell(ms);
}

// Press the button for the number of pulses and release
void Chameleon::pulses(const uint8_t count) {
  button(true);
  hold(uint16_t(count) * (CHAMELEON_PULSE_MS));
  button(false);
}

// Go to the standby position with the button axis ready to press
void Chameleon::go_standby() {
  #ifdef CHAMELEON_TOOLCHANGE_Z
    if (button_axis != Z_AXIS && motion.position.z < (CHAMELEON_TOOLCHANGE_Z)) {
      motion.position.z = CHAMELEON_TOOLCHANGE_Z;
      motion.goto_current_position(MMM_TO_MMS(CHAMELEON_TRAVEL_FEEDRATE));
    }
  #endif
  #ifdef CHAMELEON_TOOLCHANGE_X
    if (button_axis != X_AXIS) motion.position.x = CHAMELEON_TOOLCHANGE_X;
  #endif
  #ifdef CHAMELEON_TOOLCHANGE_Y
    if (button_axis != Y_AXIS) motion.position.y = CHAMELEON_TOOLCHANGE_Y;
  #endif
  motion.position[button_axis] = button_standby_pos;
  motion.goto_current_position(MMM_TO_MMS(CHAMELEON_TRAVEL_FEEDRATE));
  planner.synchronize();
}

/**
 * Change filaments the 3D Chameleon Mk4 way:
 *
 *  - Go to the standby position, 'CHAMELEON_PRESS_MM' short of the button.
 *  - If a filament is loaded, retract, cut (optionally) and shape the tip, then
 *    retract out of the extruder.
 *  - Press: Select the tool (and cut if needed). Home the selector first if the state is unknown.
 *  - Press: Unload the old filament from the Chameleon, if any.
 *  - Press: Load the new filament, helping with the extruder as it arrives.
 *  - Heat up if needed and push the filament the rest of the way to the nozzle.
 *  - Return to the position before the tool-change.
 */
void Chameleon::tool_change(const uint8_t new_tool) {

  if (int8_t(new_tool) == extruder) return;

  if (motion.homing_needed_error()) return;

  // The filament must be pushed into the hot end, so there must be a target temperature.
  if (thermalManager.targetTooColdToExtrude(0)) {
    ui.set_status(GET_TEXT_F(MSG_HOTEND_TOO_COLD));
    SERIAL_ECHO_START();
    SERIAL_ECHOLN(GET_EN_TEXT_F(MSG_HOTEND_TOO_COLD));
    return;
  }

  planner.synchronize();

  const bool unloading = extruder != CHAMELEON_NO_TOOL;
  const xyze_pos_t resume = motion.position;
  const celsius_t target_temp = thermalManager.degTargetHotend(0);

  DEBUG_ECHOLNPGM("Chameleon T", extruder, " > T", new_tool);

  ui.status_printf(0, unloading ? GET_TEXT_F(MSG_MMU2_UNLOADING_FILAMENT) : GET_TEXT_F(MSG_MMU2_LOADING_FILAMENT), int((unloading ? extruder : new_tool) + 1));

  // The filament will be moving in and out. Don't treat that as a runout.
  #if HAS_FILAMENT_SENSOR
    const bool was_runout_enabled = runout.enabled;
    runout.enabled = false;
  #endif

  // Tip shaping is done at temperatures that are too cold to extrude
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    const bool was_cold_extrude_allowed = thermalManager.allow_cold_extrude;
    thermalManager.allow_cold_extrude = true;
  #endif

  stepper.enable_extruder();

  go_standby();

  #ifdef CHAMELEON_START_GCODE
    run_gcode(F(CHAMELEON_START_GCODE));
  #endif

  // Back the old filament out of the extruder
  if (unloading) {

    e_move(-(CHAMELEON_CUT_RETRACT_MM), CHAMELEON_SLOW_FEEDRATE);

    #ifdef CHAMELEON_CUT_GCODE
      run_gcode(F(CHAMELEON_CUT_GCODE));
    #endif

    #ifdef CHAMELEON_TIP_SEQUENCE
    {
      static constexpr TipStep tip_sequence[] PROGMEM = { CHAMELEON_TIP_SEQUENCE };
      for (uint8_t i = 0; i < COUNT(tip_sequence); ++i) {
        const int16_t temp = int16_t(pgm_read_word(&tip_sequence[i].temp));
        if (temp) {
          thermalManager.setTargetHotend(temp, 0);
          thermalManager.wait_for_hotend(0, false);
        }
        e_move(pgm_read_float(&tip_sequence[i].extrude), pgm_read_float(&tip_sequence[i].feedrate));
      }
      thermalManager.setTargetHotend(target_temp, 0);   // Heat back up while retracting
    }
    #endif

    e_move(-(CHAMELEON_EXTRUDER_TO_HOTEND_MM), CHAMELEON_FAST_FEEDRATE);
    e_move(CHAMELEON_RELEASE_MM, CHAMELEON_FAST_FEEDRATE);
  }

  // Select the new tool, homing the selector first if its state is unknown
  #if ENABLED(CHAMELEON_HOME_FIRST)
    if (!unloading) {
      pulses(CHAMELEON_HOME_PULSES);
      hold(CHAMELEON_HOME_MS);
    }
  #endif

  pulses(new_tool + 1);
  hold(CHAMELEON_CUT_MS);       // Wait for the cutter and the selector

  // Unload the old filament from the Chameleon
  if (unloading) {
    button(true);
    hold(CHAMELEON_UNLOAD_MS);
    button(false);
    hold(CHAMELEON_SETTLE_MS);
  }

  // Load the new filament. Run the extruder to catch it as the Chameleon lets go.
  {
    constexpr float assist_mm = CHAMELEON_LOAD_ASSIST_MM,
                    assist_ms = assist_mm * 60000.0f / (CHAMELEON_ASSIST_FEEDRATE);
    static_assert(CHAMELEON_LOAD_MS >= assist_ms, "CHAMELEON_LOAD_MS is too short for CHAMELEON_LOAD_ASSIST_MM at CHAMELEON_ASSIST_FEEDRATE.");
    button(true);
    hold(uint16_t(CHAMELEON_LOAD_MS - assist_ms));
    e_move(assist_mm, CHAMELEON_ASSIST_FEEDRATE);
    button(false);
    hold(CHAMELEON_SETTLE_MS);
  }

  // Wait for the nozzle to get hot enough, then push filament the rest of the way
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    thermalManager.allow_cold_extrude = was_cold_extrude_allowed;
  #endif
  while (!thermalManager.wait_for_hotend(0, false)) safe_delay(100);

  e_move((CHAMELEON_EXTRUDER_TO_HOTEND_MM) - (CHAMELEON_LOAD_ASSIST_MM), CHAMELEON_FAST_FEEDRATE);

  #ifdef CHAMELEON_END_GCODE
    run_gcode(F(CHAMELEON_END_GCODE));
  #endif

  // The extruder position is whatever it was before the tool-change
  motion.position.e = resume.e;
  motion.sync_plan_position_e();

  // Return to the original position. If Z was raised, lower it last.
  LOOP_NUM_AXES(i) if (i != Z_AXIS) motion.position[i] = resume[i];
  motion.goto_current_position(MMM_TO_MMS(CHAMELEON_TRAVEL_FEEDRATE));
  motion.position.z = resume.z;
  motion.goto_current_position(MMM_TO_MMS(CHAMELEON_TRAVEL_FEEDRATE));
  planner.synchronize();

  // The one physical extruder is always E0
  extruder = new_tool;
  motion.extruder = 0;

  #if HAS_FILAMENT_SENSOR
    runout.enabled = was_runout_enabled;
    runout.reset();
  #endif

  SERIAL_ECHO_MSG(STR_ACTIVE_EXTRUDER, new_tool);

  ui.reset_status();
}

#endif // HAS_CHAMELEON
