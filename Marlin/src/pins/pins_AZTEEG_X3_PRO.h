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
 * AZTEEG_X3_PRO (Arduino Mega) pin assignments
 */

#if HOTENDS > 5 || E_STEPPERS > 5
  #error "Azteeg X3 Pro supports up to 5 hotends / E-steppers. Comment out this line to continue."
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #define CASE_LIGHT_PIN 44     // must define it here or else RAMPS will define it
#endif


#define BOARD_NAME "Azteeg X3 Pro"

#ifndef __AVR_ATmega2560__
  #error "Oops! Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

//DIGIPOT slave addresses
#define DIGIPOT_I2C_ADDRESS_A 0x2C  // unshifted slave address for first DIGIPOT 0x2C (0x58 <- 0x2C << 1)
#define DIGIPOT_I2C_ADDRESS_B 0x2E  // unshifted slave address for second DIGIPOT 0x2E (0x5C <- 0x2E << 1)

//
// Servos
//
// Tested this pin with bed leveling on a Delta with 1 servo.
// Physical wire attachment on EXT1: GND, 5V, D47.
//
#undef SERVO0_PIN
#define SERVO0_PIN         47

//
// Limit Switches
//
// Swap the MIN and MAX endstop pins because the X3 Pro comes with only
// MIN endstop pin headers soldered onto the board.
//
#if ENABLED(DELTA)
  #undef X_MIN_PIN
  #undef X_MAX_PIN
  #undef Y_MIN_PIN
  #undef Y_MAX_PIN
  #undef Z_MIN_PIN
  #undef Z_MAX_PIN

  #define X_MIN_PIN         2
  #define X_MAX_PIN         3
  #define Y_MIN_PIN        15
  #define Y_MAX_PIN        14
  #define Z_MIN_PIN        19
  #define Z_MAX_PIN        18
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN   12 /// exp1: d12 pin assigned to zprobe
#endif

//
// Endstops
//

#define X_MIN_PIN		3
#define X_MAX_PIN		2
#define Y_MIN_PIN		14
#define Y_MAX_PIN		15
#define Z_MIN_PIN		18
#define Z_MAX_PIN		19

//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define X2_STEP_PIN        23
#define X2_DIR_PIN         25
#define X2_ENABLE_PIN      40

#define Y2_STEP_PIN        27
#define Y2_DIR_PIN         29
#define Y2_ENABLE_PIN      41

#define Z2_STEP_PIN        43
#define Z2_DIR_PIN         37
#define Z2_ENABLE_PIN      42

//Dummy stepper
#define E2_STEP_PIN			35
#define E2_DIR_PIN			32
#define E2_ENABLE_PIN		31


//
// Temperature Sensors
//
#define TEMP_0_PIN		   3	// T0 TEMP - ANALOG
#define	TEMP_1_PIN		   9	// T1 TEMP - ANALOG
#define TEMP_2_PIN         13   // CHAMBER TEMP - ANALOG
#define TEMP_BED_PIN	   14	// BED TEMP - ANALOG

//
// Heaters / Fans
//
#define HEATER_0_PIN	   10	//T0 HE HEATER
#define HEATER_1_PIN       9	//T1 HE HEATER
#define HEATER_2_PIN	   4	//CHAMBER HEATER SSR
#define HEATER_BED_PIN	   8	//BED HEATER SSR

#define FAN_PIN             16	//T0 PART COOLING
#define FAN1_PIN			17	//T1 PART COOLING
#define FAN2_PIN			5	//CHAMBER FAN

//Filament runout - Dyze Sentinel
#define FIL_RUNOUT_PIN      22

//
// LCD / Controller
//
#undef BEEPER_PIN
#define BEEPER_PIN         33

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN    49   // For easy adapter board
  #undef BEEPER_PIN
  #define  BEEPER_PIN      12   // 33 isn't physically available to the LCD display
#else
  #define STAT_LED_RED_PIN  32
  #define STAT_LED_BLUE_PIN 35
#endif

//
// Misc. Functions
//
#if ENABLED(CASE_LIGHT_ENABLE)  && PIN_EXISTS(CASE_LIGHT) && defined(DOGLCD_A0) && DOGLCD_A0 == CASE_LIGHT_PIN
  #undef DOGLCD_A0            // Steal pin 44 for the case light; if you have a Viki2 and have connected it
  #define DOGLCD_A0        57 // following the Panucatt wiring diagram, you may need to tweak these pin assignments
                              // as the wiring diagram uses pin 44 for DOGLCD_A0
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#undef SPINDLE_LASER_PWM_PIN    // Definitions in pins_RAMPS.h are no good with the AzteegX3pro board
#undef SPINDLE_LASER_ENABLE_PIN
#undef SPINDLE_DIR_PIN

#if ENABLED(SPINDLE_LASER_ENABLE)   // use EXP2 header
  #if ENABLED(VIKI2) || ENABLED(miniVIKI)
    #undef BTN_EN2
    #define BTN_EN2             31  // need 7 for the spindle speed PWM
  #endif
  #define SPINDLE_LASER_PWM_PIN     7  // must have a hardware PWM
  #define SPINDLE_LASER_ENABLE_PIN 20  // Pin should have a pullup!
  #define SPINDLE_DIR_PIN          21
#endif

