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

#include "../inc/MarlinConfig.h"

#if ENABLED(MIXING_EXTRUDER)

//#define MIXER_NORMALIZER_DEBUG

#include "mixing.h"

Mixer mixer;

#ifdef MIXER_NORMALIZER_DEBUG
  #include "../core/serial.h"
#endif

// Used up to Planner level
uint_fast8_t  Mixer::selected_vtool = 0;
float         Mixer::collector[MIXING_STEPPERS]; // mix proportion. 0.0 = off, otherwise <= COLOR_A_MASK.
mixer_comp_t  Mixer::color[NR_MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];

// Used in Stepper
int_fast8_t   Mixer::runner = 0;
mixer_comp_t  Mixer::s_color[MIXING_STEPPERS];
mixer_accu_t  Mixer::accu[MIXING_STEPPERS] = { 0 };

#if EITHER(HAS_DUAL_MIXING, GRADIENT_MIX)
  mixer_perc_t Mixer::mix[MIXING_STEPPERS];
#endif

void Mixer::normalize(const uint8_t tool_index) {
  float cmax = 0;
  #ifdef MIXER_NORMALIZER_DEBUG
    float csum = 0;
  #endif
  MIXER_STEPPER_LOOP(i) {
    const float v = collector[i];
    NOLESS(cmax, v);
    #ifdef MIXER_NORMALIZER_DEBUG
      csum += v;
    #endif
  }
  #ifdef MIXER_NORMALIZER_DEBUG
    SERIAL_ECHOPGM("Mixer: Old relation : [ ");
    MIXER_STEPPER_LOOP(i) {
      SERIAL_ECHO_F(collector[i] / csum, 3);
      SERIAL_CHAR(' ');
    }
    SERIAL_ECHOLNPGM("]");
  #endif

  // Scale all values so their maximum is COLOR_A_MASK
  const float scale = float(COLOR_A_MASK) / cmax;
  MIXER_STEPPER_LOOP(i) color[tool_index][i] = collector[i] * scale;

  #ifdef MIXER_NORMALIZER_DEBUG
    csum = 0;
    SERIAL_ECHOPGM("Mixer: Normalize to : [ ");
    MIXER_STEPPER_LOOP(i) {
      SERIAL_ECHO(uint16_t(color[tool_index][i]));
      SERIAL_CHAR(' ');
      csum += color[tool_index][i];
    }
    SERIAL_ECHOLNPGM("]");
    SERIAL_ECHOPGM("Mixer: New relation : [ ");
    MIXER_STEPPER_LOOP(i) {
      SERIAL_ECHO_F(uint16_t(color[tool_index][i]) / csum, 3);
      SERIAL_CHAR(' ');
    }
    SERIAL_ECHOLNPGM("]");
  #endif

  TERN_(GRADIENT_MIX, refresh_gradient());
}

void Mixer::reset_vtools() {
  // Virtual Tools 0, 1, 2, 3 = Filament 1, 2, 3, 4, etc.
  // Every virtual tool gets a pure filament
  LOOP_L_N(t, _MIN(MIXING_VIRTUAL_TOOLS, MIXING_STEPPERS))
    MIXER_STEPPER_LOOP(i)
      color[t][i] = (t == i) ? COLOR_A_MASK : 0;

  // Remaining virtual tools are 100% filament 1
  #if MIXING_VIRTUAL_TOOLS > MIXING_STEPPERS
    LOOP_S_L_N(t, MIXING_STEPPERS, MIXING_VIRTUAL_TOOLS)
      MIXER_STEPPER_LOOP(i)
        color[t][i] = (i == 0) ? COLOR_A_MASK : 0;
  #endif
}

// called at boot
void Mixer::init() {

  reset_vtools();

  #if HAS_MIXER_SYNC_CHANNEL
    // AUTORETRACT_TOOL gets the same amount of all filaments
    MIXER_STEPPER_LOOP(i)
      color[MIXER_AUTORETRACT_TOOL][i] = COLOR_A_MASK;
  #endif

  ZERO(collector);

  #if EITHER(HAS_DUAL_MIXING, GRADIENT_MIX)
    update_mix_from_vtool();
  #endif

  TERN_(GRADIENT_MIX, update_gradient_for_planner_z());
}

void Mixer::refresh_collector(const float proportion/*=1.0*/, const uint8_t t/*=selected_vtool*/, float (&c)[MIXING_STEPPERS]/*=collector*/) {
  float csum = 0, cmax = 0;
  MIXER_STEPPER_LOOP(i) {
    const float v = color[t][i];
    cmax = _MAX(cmax, v);
    csum += v;
  }
  //SERIAL_ECHOPAIR("Mixer::refresh_collector(", proportion, ", ", t, ") cmax=", cmax, "  csum=", csum, "  color");
  const float inv_prop = proportion / csum;
  MIXER_STEPPER_LOOP(i) {
    c[i] = color[t][i] * inv_prop;
    //SERIAL_ECHOPAIR(" [", t, "][", i, "] = ", color[t][i], " (", c[i], ")  ");
  }
  //SERIAL_EOL();
}

#if ENABLED(GRADIENT_MIX)

  #include "../module/motion.h"
  #include "../module/planner.h"

  gradient_t Mixer::gradient = {
    false,    // enabled
    {0},      // color (array)
    0, 0,     // start_z, end_z
    0, 1,     // start_vtool, end_vtool
    {0}, {0}  // start_mix[], end_mix[]
    #if ENABLED(GRADIENT_VTOOL)
      , -1    // vtool_index
    #endif
  };

  float Mixer::prev_z; // = 0

  void Mixer::update_gradient_for_z(const float z) {
    if (z == prev_z) return;
    prev_z = z;

    const float slice = gradient.end_z - gradient.start_z;

    float pct = (z - gradient.start_z) / slice;
    NOLESS(pct, 0.0f); NOMORE(pct, 1.0f);

    MIXER_STEPPER_LOOP(i) {
      const mixer_perc_t sm = gradient.start_mix[i];
      mix[i] = sm + (gradient.end_mix[i] - sm) * pct;
    }

    copy_mix_to_color(gradient.color);
  }

  void Mixer::update_gradient_for_planner_z() {
    #if ENABLED(DELTA)
      get_cartesian_from_steppers();
      update_gradient_for_z(cartes.z);
    #else
      update_gradient_for_z(planner.get_axis_position_mm(Z_AXIS));
    #endif
  }

#endif // GRADIENT_MIX

#endif // MIXING_EXTRUDER
