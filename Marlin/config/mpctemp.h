/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

/**
 * Model Predictive Control for hotend
 *
 * Use a physical model of the hotend to control temperature. When configured correctly this gives
 * better responsiveness and stability than PID and removes the need for PID_EXTRUSION_SCALING
 * and PID_FAN_SCALING. Enable MPC_AUTOTUNE and use M306 T to autotune the model.
 */
#if ENABLED(MPCTEMP)
  #define MPC_AUTOTUNE                                // Include a method to do MPC auto-tuning (~6.3K bytes of flash)
  //#define MPC_EDIT_MENU                             // Add MPC editing to the "Advanced Settings" menu. (~1.3K bytes of flash)
  //#define MPC_AUTOTUNE_MENU                         // Add MPC auto-tuning to the "Advanced Settings" menu. (~350 bytes of flash)

  #define MPC_MAX 255                                 // (0..255) Current to nozzle while MPC is active.
  #define MPC_HEATER_POWER { 40.0f }                  // (W) Heat cartridge powers.

  #define MPC_INCLUDE_FAN                             // Model the fan speed?

  // Measured physical constants from M306
  #define MPC_BLOCK_HEAT_CAPACITY { 16.7f }           // (J/K) Heat block heat capacities.
  #define MPC_SENSOR_RESPONSIVENESS { 0.22f }         // (K/s per ∆K) Rate of change of sensor temperature from heat block.
  #define MPC_AMBIENT_XFER_COEFF { 0.068f }           // (W/K) Heat transfer coefficients from heat block to room air with fan off.
  #if ENABLED(MPC_INCLUDE_FAN)
    #define MPC_AMBIENT_XFER_COEFF_FAN255 { 0.097f }  // (W/K) Heat transfer coefficients from heat block to room air with fan on full.
  #endif

  // For one fan and multiple hotends MPC needs to know how to apply the fan cooling effect.
  #if ENABLED(MPC_INCLUDE_FAN)
    //#define MPC_FAN_0_ALL_HOTENDS
    //#define MPC_FAN_0_ACTIVE_HOTEND
  #endif

  // Filament Heat Capacity (joules/kelvin/mm)
  // Set at runtime with M306 H<value>
  #define FILAMENT_HEAT_CAPACITY_PERMM { 5.6e-3f }    // 0.0056 J/K/mm for 1.75mm PLA (0.0149 J/K/mm for 2.85mm PLA).
                                                      // 0.0036 J/K/mm for 1.75mm PETG (0.0094 J/K/mm for 2.85mm PETG).
                                                      // 0.00515 J/K/mm for 1.75mm ABS (0.0137 J/K/mm for 2.85mm ABS).
                                                      // 0.00522 J/K/mm for 1.75mm Nylon (0.0138 J/K/mm for 2.85mm Nylon).

  // Advanced options
  #define MPC_SMOOTHING_FACTOR 0.5f                   // (0.0...1.0) Noisy temperature sensors may need a lower value for stabilization.
  #define MPC_MIN_AMBIENT_CHANGE 1.0f                 // (K/s) Modeled ambient temperature rate of change, when correcting model inaccuracies.
  #define MPC_STEADYSTATE 0.5f                        // (K/s) Temperature change rate for steady state logic to be enforced.

  #define MPC_TUNING_POS { X_CENTER, Y_CENTER, 1.0f } // (mm) M306 Autotuning position, ideally bed center at first layer height.
  #define MPC_TUNING_END_Z 10.0f                      // (mm) M306 Autotuning final Z position.
#endif
