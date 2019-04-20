/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * To use TMC2130, TMC2160, TMC2660, TMC5130, TMC5160 stepper drivers in SPI mode
 * connect your SPI pins to the hardware SPI interface on your board and define
 * the required CS pins in your `pins_MYBOARD.h` file. (e.g., RAMPS 1.4 uses AUX3
 * pins `X_CS_PIN 53`, `Y_CS_PIN 49`, etc.).
 * You may also use software SPI if you wish to use general purpose IO pins.
 *
 * To use TMC2208 stepper UART-configurable stepper drivers connect #_SERIAL_TX_PIN
 * to the driver side PDN_UART pin with a 1K resistor.
 * To use the reading capabilities, also connect #_SERIAL_RX_PIN to PDN_UART without
 * a resistor.
 * The drivers can also be used with hardware serial.
 *
 * TMCStepper library is required to use TMC stepper drivers.
 * https://github.com/teemuatlut/TMCStepper
 */
#if HAS_TRINAMIC

  #define HOLD_MULTIPLIER    0.5  // Scales down the holding current from run current
  #define INTERPOLATE       true  // Interpolate X/Y/Z_MICROSTEPS to 256

  #if AXIS_IS_TMC(X)
    #define X_CURRENT     800  // (mA) RMS current. Multiply by 1.414 for peak current.
    #define X_MICROSTEPS   16  // 0..256
    #define X_RSENSE     0.11
  #endif

  #if AXIS_IS_TMC(X2)
    #define X2_CURRENT    800
    #define X2_MICROSTEPS  16
    #define X2_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(Y)
    #define Y_CURRENT     800
    #define Y_MICROSTEPS   16
    #define Y_RSENSE     0.11
  #endif

  #if AXIS_IS_TMC(Y2)
    #define Y2_CURRENT    800
    #define Y2_MICROSTEPS  16
    #define Y2_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(Z)
    #define Z_CURRENT     800
    #define Z_MICROSTEPS   16
    #define Z_RSENSE     0.11
  #endif

  #if AXIS_IS_TMC(Z2)
    #define Z2_CURRENT    800
    #define Z2_MICROSTEPS  16
    #define Z2_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(Z3)
    #define Z3_CURRENT    800
    #define Z3_MICROSTEPS  16
    #define Z3_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E0)
    #define E0_CURRENT    800
    #define E0_MICROSTEPS  16
    #define E0_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E1)
    #define E1_CURRENT    800
    #define E1_MICROSTEPS  16
    #define E1_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E2)
    #define E2_CURRENT    800
    #define E2_MICROSTEPS  16
    #define E2_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E3)
    #define E3_CURRENT    800
    #define E3_MICROSTEPS  16
    #define E3_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E4)
    #define E4_CURRENT    800
    #define E4_MICROSTEPS  16
    #define E4_RSENSE    0.11
  #endif

  #if AXIS_IS_TMC(E5)
    #define E5_CURRENT    800
    #define E5_MICROSTEPS  16
    #define E5_RSENSE    0.11
  #endif

  /**
   * Override default SPI pins for TMC2130, TMC2160, TMC2660, TMC5130 and TMC5160 drivers here.
   * The default pins can be found in your board's pins file.
   */
  //#define X_CS_PIN          -1
  //#define Y_CS_PIN          -1
  //#define Z_CS_PIN          -1
  //#define X2_CS_PIN         -1
  //#define Y2_CS_PIN         -1
  //#define Z2_CS_PIN         -1
  //#define Z3_CS_PIN         -1
  //#define E0_CS_PIN         -1
  //#define E1_CS_PIN         -1
  //#define E2_CS_PIN         -1
  //#define E3_CS_PIN         -1
  //#define E4_CS_PIN         -1
  //#define E5_CS_PIN         -1

  /**
   * Use software SPI for TMC2130.
   * Software option for SPI driven drivers (TMC2130, TMC2160, TMC2660, TMC5130 and TMC5160).
   * The default SW SPI pins are defined the respective pins files,
   * but you can override or define them here.
   */
  //#define TMC_USE_SW_SPI
  //#define TMC_SW_MOSI       -1
  //#define TMC_SW_MISO       -1
  //#define TMC_SW_SCK        -1

  /**
   * Software enable
   *
   * Use for drivers that do not use a dedicated enable pin, but rather handle the same
   * function through a communication line such as SPI or UART.
   */
  //#define SOFTWARE_DRIVER_ENABLE

  /**
   * TMC2130, TMC2160, TMC2208, TMC5130 and TMC5160 only
   * Use Trinamic's ultra quiet stepping mode.
   * When disabled, Marlin will use spreadCycle stepping mode.
   */
  #define STEALTHCHOP_XY
  #define STEALTHCHOP_Z
  #define STEALTHCHOP_E

  /**
   * Optimize spreadCycle chopper parameters by using predefined parameter sets
   * or with the help of an example included in the library.
   * Provided parameter sets are
   * CHOPPER_DEFAULT_12V
   * CHOPPER_DEFAULT_19V
   * CHOPPER_DEFAULT_24V
   * CHOPPER_DEFAULT_36V
   * CHOPPER_PRUSAMK3_24V // Imported parameters from the official Prusa firmware for MK3 (24V)
   * CHOPPER_MARLIN_119   // Old defaults from Marlin v1.1.9
   *
   * Define you own with
   * { <off_time[1..15]>, <hysteresis_end[-3..12]>, hysteresis_start[1..8] }
   */
  #define CHOPPER_TIMING CHOPPER_DEFAULT_12V

  /**
   * Monitor Trinamic drivers for error conditions,
   * like overtemperature and short to ground. TMC2208 requires hardware serial.
   * In the case of overtemperature Marlin can decrease the driver current until error condition clears.
   * Other detected conditions can be used to stop the current print.
   * Relevant g-codes:
   * M906 - Set or get motor current in milliamps using axis codes X, Y, Z, E. Report values if no axis codes given.
   * M911 - Report stepper driver overtemperature pre-warn condition.
   * M912 - Clear stepper driver overtemperature pre-warn condition flag.
   * M122 - Report driver parameters (Requires TMC_DEBUG)
   */
  //#define MONITOR_DRIVER_STATUS

  #if ENABLED(MONITOR_DRIVER_STATUS)
    #define CURRENT_STEP_DOWN     50  // [mA]
    #define REPORT_CURRENT_CHANGE
    #define STOP_ON_ERROR
  #endif

  /**
   * TMC2130, TMC2160, TMC2208, TMC5130 and TMC5160 only
   * The driver will switch to spreadCycle when stepper speed is over HYBRID_THRESHOLD.
   * This mode allows for faster movements at the expense of higher noise levels.
   * STEALTHCHOP_(XY|Z|E) must be enabled to use HYBRID_THRESHOLD.
   * M913 X/Y/Z/E to live tune the setting
   */
  //#define HYBRID_THRESHOLD

  #define X_HYBRID_THRESHOLD     100  // [mm/s]
  #define X2_HYBRID_THRESHOLD    100
  #define Y_HYBRID_THRESHOLD     100
  #define Y2_HYBRID_THRESHOLD    100
  #define Z_HYBRID_THRESHOLD       3
  #define Z2_HYBRID_THRESHOLD      3
  #define Z3_HYBRID_THRESHOLD      3
  #define E0_HYBRID_THRESHOLD     30
  #define E1_HYBRID_THRESHOLD     30
  #define E2_HYBRID_THRESHOLD     30
  #define E3_HYBRID_THRESHOLD     30
  #define E4_HYBRID_THRESHOLD     30
  #define E5_HYBRID_THRESHOLD     30

  /**
   * TMC2130, TMC2160, TMC2660, TMC5130, and TMC5160 only
   * Use StallGuard2 to sense an obstacle and trigger an endstop.
   * Connect the stepper driver's DIAG1 pin to the X/Y endstop pin.
   * X, Y, and Z homing will always be done in spreadCycle mode.
   *
   * X/Y/Z_STALL_SENSITIVITY is used for tuning the trigger sensitivity.
   * Higher values make the system LESS sensitive.
   * Lower value make the system MORE sensitive.
   * Too low values can lead to false positives, while too high values will collide the axis without triggering.
   * It is advised to set X/Y/Z_HOME_BUMP_MM to 0.
   * M914 X/Y/Z to live tune the setting
   */
  //#define SENSORLESS_HOMING // TMC2130 only

  /**
   * Use StallGuard2 to probe the bed with the nozzle.
   *
   * CAUTION: This could cause damage to machines that use a lead screw or threaded rod
   *          to move the Z axis. Take extreme care when attempting to enable this feature.
   */
  //#define SENSORLESS_PROBING // TMC2130 only

  #if EITHER(SENSORLESS_HOMING, SENSORLESS_PROBING)
    #define X_STALL_SENSITIVITY  8
    #define Y_STALL_SENSITIVITY  8
    //#define Z_STALL_SENSITIVITY  8
  #endif

  /**
   * Enable M122 debugging command for TMC stepper drivers.
   * M122 S0/1 will enable continous reporting.
   */
  //#define TMC_DEBUG

  /**
   * You can set your own advanced settings by filling in predefined functions.
   * A list of available functions can be found on the library github page
   * https://github.com/teemuatlut/TMCStepper
   *
   * Example:
   * #define TMC_ADV() { \
   *   stepperX.diag0_temp_prewarn(1); \
   *   stepperY.interpolate(0); \
   * }
   */
  #define TMC_ADV() {  }

#endif // HAS_TRINAMIC
