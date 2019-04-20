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
 * L6470 Stepper Driver options
 *
 * Arduino-L6470 library (0.7.0 or higher) is required for this stepper driver.
 * https://github.com/ameyer/Arduino-L6470
 *
 * Requires the following to be defined in your pins_YOUR_BOARD file
 *     L6470_CHAIN_SCK_PIN
 *     L6470_CHAIN_MISO_PIN
 *     L6470_CHAIN_MOSI_PIN
 *     L6470_CHAIN_SS_PIN
 *     L6470_RESET_CHAIN_PIN  (optional)
 */
#if HAS_DRIVER(L6470)

  //#define L6470_CHITCHAT        // Display additional status info

  #if AXIS_DRIVER_TYPE_X(L6470)
    #define X_MICROSTEPS     128  // Number of microsteps (VALID: 1, 2, 4, 8, 16, 32, 128)
    #define X_OVERCURRENT   2000  // (mA) Current where the driver detects an over current (VALID: 375 x (1 - 16) - 6A max - rounds down)
    #define X_STALLCURRENT  1500  // (mA) Current where the driver detects a stall (VALID: 31.25 * (1-128) -  4A max - rounds down)
    #define X_MAX_VOLTAGE    127  // 0-255, Maximum effective voltage seen by stepper
    #define X_CHAIN_POS        0  // Position in SPI chain, 0=Not in chain, 1=Nearest MOSI
  #endif

  #if AXIS_DRIVER_TYPE_X2(L6470)
    #define X2_MICROSTEPS      128
    #define X2_OVERCURRENT    2000
    #define X2_STALLCURRENT   1500
    #define X2_MAX_VOLTAGE     127
    #define X2_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_Y(L6470)
    #define Y_MICROSTEPS       128
    #define Y_OVERCURRENT     2000
    #define Y_STALLCURRENT    1500
    #define Y_MAX_VOLTAGE      127
    #define Y_CHAIN_POS          0
  #endif

  #if AXIS_DRIVER_TYPE_Y2(L6470)
    #define Y2_MICROSTEPS      128
    #define Y2_OVERCURRENT    2000
    #define Y2_STALLCURRENT   1500
    #define Y2_MAX_VOLTAGE     127
    #define Y2_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_Z(L6470)
    #define Z_MICROSTEPS       128
    #define Z_OVERCURRENT     2000
    #define Z_STALLCURRENT    1500
    #define Z_MAX_VOLTAGE      127
    #define Z_CHAIN_POS          0
  #endif

  #if AXIS_DRIVER_TYPE_Z2(L6470)
    #define Z2_MICROSTEPS      128
    #define Z2_OVERCURRENT    2000
    #define Z2_STALLCURRENT   1500
    #define Z2_MAX_VOLTAGE     127
    #define Z2_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_Z3(L6470)
    #define Z3_MICROSTEPS      128
    #define Z3_OVERCURRENT    2000
    #define Z3_STALLCURRENT   1500
    #define Z3_MAX_VOLTAGE     127
    #define Z3_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E0(L6470)
    #define E0_MICROSTEPS      128
    #define E0_OVERCURRENT    2000
    #define E0_STALLCURRENT   1500
    #define E0_MAX_VOLTAGE     127
    #define E0_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E1(L6470)
    #define E1_MICROSTEPS      128
    #define E1_OVERCURRENT    2000
    #define E1_STALLCURRENT   1500
    #define E1_MAX_VOLTAGE     127
    #define E1_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E2(L6470)
    #define E2_MICROSTEPS      128
    #define E2_OVERCURRENT    2000
    #define E2_STALLCURRENT   1500
    #define E2_MAX_VOLTAGE     127
    #define E2_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E3(L6470)
    #define E3_MICROSTEPS      128
    #define E3_OVERCURRENT    2000
    #define E3_STALLCURRENT   1500
    #define E3_MAX_VOLTAGE     127
    #define E3_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E4(L6470)
    #define E4_MICROSTEPS      128
    #define E4_OVERCURRENT    2000
    #define E4_STALLCURRENT   1500
    #define E4_MAX_VOLTAGE     127
    #define E4_CHAIN_POS         0
  #endif

  #if AXIS_DRIVER_TYPE_E5(L6470)
    #define E5_MICROSTEPS      128
    #define E5_OVERCURRENT    2000
    #define E5_STALLCURRENT   1500
    #define E5_MAX_VOLTAGE     127
    #define E5_CHAIN_POS         0
  #endif

  /**
   * Monitor L6470 drivers for error conditions like over temperature and over current.
   * In the case of over temperature Marlin can decrease the drive until the error condition clears.
   * Other detected conditions can be used to stop the current print.
   * Relevant g-codes:
   * M906 - I1/2/3/4/5  Set or get motor drive level using axis codes X, Y, Z, E. Report values if no axis codes given.
   *         I not present or I0 or I1 - X, Y, Z or E0
   *         I2 - X2, Y2, Z2 or E1
   *         I3 - Z3 or E3
   *         I4 - E4
   *         I5 - E5
   * M916 - Increase drive level until get thermal warning
   * M917 - Find minimum current thresholds
   * M918 - Increase speed until max or error
   * M122 S0/1 - Report driver parameters
   */
  //#define MONITOR_L6470_DRIVER_STATUS

  #if ENABLED(MONITOR_L6470_DRIVER_STATUS)
    #define KVAL_HOLD_STEP_DOWN     1
    //#define L6470_STOP_ON_ERROR
  #endif

#endif // L6470
