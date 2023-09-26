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
 * Temperature Sensors:
 *
 * NORMAL IS 4.7kΩ PULLUP! Hotend sensors can use 1kΩ pullup with correct resistor and table.
 *
 * ================================================================
 *  Analog Thermistors - 4.7kΩ pullup - Normal
 * ================================================================
 *     1 : 100kΩ EPCOS - Best choice for EPCOS thermistors
 *   331 : 100kΩ Same as #1, but 3.3V scaled for MEGA
 *   332 : 100kΩ Same as #1, but 3.3V scaled for DUE
 *     2 : 200kΩ ATC Semitec 204GT-2
 *   202 : 200kΩ Copymaster 3D
 *     3 : ???Ω  Mendel-parts thermistor
 *     4 : 10kΩ  Generic Thermistor !! DO NOT use for a hotend - it gives bad resolution at high temp. !!
 *     5 : 100kΩ ATC Semitec 104GT-2/104NT-4-R025H42G - Used in ParCan, J-Head, and E3D, SliceEngineering 300°C
 *   501 : 100kΩ Zonestar - Tronxy X3A
 *   502 : 100kΩ Zonestar - used by hot bed in Zonestar Průša P802M
 *   503 : 100kΩ Zonestar (Z8XM2) Heated Bed thermistor
 *   504 : 100kΩ Zonestar P802QR2 (Part# QWG-104F-B3950) Hotend Thermistor
 *   505 : 100kΩ Zonestar P802QR2 (Part# QWG-104F-3950) Bed Thermistor
 *   512 : 100kΩ RPW-Ultra hotend
 *     6 : 100kΩ EPCOS - Not as accurate as table #1 (created using a fluke thermocouple)
 *     7 : 100kΩ Honeywell 135-104LAG-J01
 *    71 : 100kΩ Honeywell 135-104LAF-J01
 *     8 : 100kΩ Vishay 0603 SMD NTCS0603E3104FXT
 *     9 : 100kΩ GE Sensing AL03006-58.2K-97-G1
 *    10 : 100kΩ RS PRO 198-961
 *    11 : 100kΩ Keenovo AC silicone mats, most Wanhao i3 machines - beta 3950, 1%
 *    12 : 100kΩ Vishay 0603 SMD NTCS0603E3104FXT (#8) - calibrated for Makibox hot bed
 *    13 : 100kΩ Hisens up to 300°C - for "Simple ONE" & "All In ONE" hotend - beta 3950, 1%
 *    14 : 100kΩ  (R25), 4092K (beta25), 4.7kΩ pull-up, bed thermistor as used in Ender-5 S1
 *    15 : 100kΩ Calibrated for JGAurora A5 hotend
 *    17 : 100kΩ Dagoma NTC white thermistor
 *    18 : 200kΩ ATC Semitec 204GT-2 Dagoma.Fr - MKS_Base_DKU001327
 *    22 : 100kΩ GTM32 Pro vB - hotend - 4.7kΩ pullup to 3.3V and 220Ω to analog input
 *    23 : 100kΩ GTM32 Pro vB - bed - 4.7kΩ pullup to 3.3v and 220Ω to analog input
 *    30 : 100kΩ Kis3d Silicone heating mat 200W/300W with 6mm precision cast plate (EN AW 5083) NTC100K - beta 3950
 *    60 : 100kΩ Maker's Tool Works Kapton Bed Thermistor - beta 3950
 *    61 : 100kΩ Formbot/Vivedino 350°C Thermistor - beta 3950
 *    66 : 4.7MΩ Dyze Design / Trianglelab T-D500 500°C High Temperature Thermistor
 *    67 : 500kΩ SliceEngineering 450°C Thermistor
 *    68 : PT100 Smplifier board from Dyze Design
 *    70 : 100kΩ bq Hephestos 2
 *    75 : 100kΩ Generic Silicon Heat Pad with NTC100K MGB18-104F39050L32
 *   666 : 200kΩ Einstart S custom thermistor with 10k pullup.
 *  2000 : 100kΩ Ultimachine Rambo TDK NTCG104LH104KT1 NTC100K motherboard Thermistor
 *
 * ================================================================
 *  Analog Thermistors - 1kΩ pullup
 *   Atypical, and requires changing out the 4.7kΩ pullup for 1kΩ.
 *   (but gives greater accuracy and more stable PID)
 * ================================================================
 *    51 : 100kΩ EPCOS (1kΩ pullup)
 *    52 : 200kΩ ATC Semitec 204GT-2 (1kΩ pullup)
 *    55 : 100kΩ ATC Semitec 104GT-2 - Used in ParCan & J-Head (1kΩ pullup)
 *
 * ================================================================
 *  Analog Thermistors - 10kΩ pullup - Atypical
 * ================================================================
 *    99 : 100kΩ Found on some Wanhao i3 machines with a 10kΩ pull-up resistor
 *
 * ================================================================
 *  Analog RTDs (Pt100/Pt1000)
 * ================================================================
 *   110 : Pt100  with 1kΩ pullup (atypical)
 *   147 : Pt100  with 4.7kΩ pullup
 *  1010 : Pt1000 with 1kΩ pullup (atypical)
 *  1022 : Pt1000 with 2.2kΩ pullup
 *  1047 : Pt1000 with 4.7kΩ pullup (E3D)
 *    20 : Pt100  with circuit in the Ultimainboard V2.x with mainboard ADC reference voltage = INA826 amplifier-board supply voltage.
 *                NOTE: (1) Must use an ADC input with no pullup. (2) Some INA826 amplifiers are unreliable at 3.3V so consider using sensor 147, 110, or 21.
 *    21 : Pt100  with circuit in the Ultimainboard V2.x with 3.3v ADC reference voltage (STM32, LPC176x....) and 5V INA826 amplifier board supply.
 *                NOTE: ADC pins are not 5V tolerant. Not recommended because it's possible to damage the CPU by going over 500°C.
 *   201 : Pt100  with circuit in Overlord, similar to Ultimainboard V2.x
 *
 * ================================================================
 *  SPI RTD/Thermocouple Boards
 * ================================================================
 *    -5 : MAX31865 with Pt100/Pt1000, 2, 3, or 4-wire  (only for sensors 0-1)
 *                  NOTE: You must uncomment/set the MAX31865_*_OHMS_n defines below.
 *    -3 : MAX31855 with Thermocouple, -200°C to +700°C (only for sensors 0-1)
 *    -2 : MAX6675  with Thermocouple, 0°C to +700°C    (only for sensors 0-1)
 *
 *  NOTE: Ensure TEMP_n_CS_PIN is set in your pins file for each TEMP_SENSOR_n using an SPI Thermocouple. By default,
 *        Hardware SPI on the default serial bus is used. If you have also set TEMP_n_SCK_PIN and TEMP_n_MISO_PIN,
 *        Software SPI will be used on those ports instead. You can force Hardware SPI on the default bus in the
 *        Configuration_adv.h file. At this time, separate Hardware SPI buses for sensors are not supported.
 *
 * ================================================================
 *  Analog Thermocouple Boards
 * ================================================================
 *    -4 : AD8495 with Thermocouple
 *    -1 : AD595  with Thermocouple
 *
 * ================================================================
 *  SoC internal sensor
 * ================================================================
 *   100 : SoC internal sensor
 *
 * ================================================================
 *  Custom/Dummy/Other Thermal Sensors
 * ================================================================
 *     0 : not used
 *  1000 : Custom - Specify parameters in Configuration_adv.h
 *
 *   !!! Use these for Testing or Development purposes. NEVER for production machine. !!!
 *   998 : Dummy Table that ALWAYS reads 25°C or the temperature defined below.
 *   999 : Dummy Table that ALWAYS reads 100°C or the temperature defined below.
 */
#define TEMP_SENSOR_0 1
#define TEMP_SENSOR_1 0
#define TEMP_SENSOR_2 0
#define TEMP_SENSOR_3 0
#define TEMP_SENSOR_4 0
#define TEMP_SENSOR_5 0
#define TEMP_SENSOR_6 0
#define TEMP_SENSOR_7 0
#define TEMP_SENSOR_BED 1
#define TEMP_SENSOR_PROBE 0
#define TEMP_SENSOR_CHAMBER 0
#define TEMP_SENSOR_COOLER 0
#define TEMP_SENSOR_BOARD 0
#define TEMP_SENSOR_SOC 0
#define TEMP_SENSOR_REDUNDANT 0

// Dummy thermistor constant temperature readings, for use with 998 and 999
#define DUMMY_THERMISTOR_998_VALUE  25
#define DUMMY_THERMISTOR_999_VALUE 100

// Resistor values when using MAX31865 sensors (-5) on TEMP_SENSOR_0 / 1
#if TEMP_SENSOR_IS_MAX_TC(0)
  #define MAX31865_SENSOR_OHMS_0      100 // (Ω) Typically 100 or 1000 (PT100 or PT1000)
  #define MAX31865_CALIBRATION_OHMS_0 430 // (Ω) Typically 430 for Adafruit PT100; 4300 for Adafruit PT1000
#endif
#if TEMP_SENSOR_IS_MAX_TC(1)
  #define MAX31865_SENSOR_OHMS_1      100
  #define MAX31865_CALIBRATION_OHMS_1 430
#endif
#if TEMP_SENSOR_IS_MAX_TC(2)
  #define MAX31865_SENSOR_OHMS_2      100
  #define MAX31865_CALIBRATION_OHMS_2 430
#endif

#if HAS_E_TEMP_SENSOR
  #define TEMP_RESIDENCY_TIME         10  // (seconds) Time to wait for hotend to "settle" in M109
  #define TEMP_WINDOW                  1  // (°C) Temperature proximity for the "temperature reached" timer
  #define TEMP_HYSTERESIS              3  // (°C) Temperature proximity considered "close enough" to the target
#endif

#if TEMP_SENSOR_BED
  #define TEMP_BED_RESIDENCY_TIME     10  // (seconds) Time to wait for bed to "settle" in M190
  #define TEMP_BED_WINDOW              1  // (°C) Temperature proximity for the "temperature reached" timer
  #define TEMP_BED_HYSTERESIS          3  // (°C) Temperature proximity considered "close enough" to the target
#endif

#if TEMP_SENSOR_CHAMBER
  #define TEMP_CHAMBER_RESIDENCY_TIME 10  // (seconds) Time to wait for chamber to "settle" in M191
  #define TEMP_CHAMBER_WINDOW          1  // (°C) Temperature proximity for the "temperature reached" timer
  #define TEMP_CHAMBER_HYSTERESIS      3  // (°C) Temperature proximity considered "close enough" to the target
#endif

/**
 * Redundant Temperature Sensor (TEMP_SENSOR_REDUNDANT)
 *
 * Use a temp sensor as a redundant sensor for another reading. Select an unused temperature sensor, and another
 * sensor you'd like it to be redundant for. If the two thermistors differ by TEMP_SENSOR_REDUNDANT_MAX_DIFF (°C),
 * the print will be aborted. Whichever sensor is selected will have its normal functions disabled; i.e. selecting
 * the Bed sensor (-1) will disable bed heating/monitoring.
 *
 * For selecting source/target use: COOLER, PROBE, BOARD, CHAMBER, BED, E0, E1, E2, E3, E4, E5, E6, E7
 */
#if TEMP_SENSOR_REDUNDANT
  #define TEMP_SENSOR_REDUNDANT_SOURCE    E1  // The sensor that will provide the redundant reading.
  #define TEMP_SENSOR_REDUNDANT_TARGET    E0  // The sensor that we are providing a redundant reading for.
  #define TEMP_SENSOR_REDUNDANT_MAX_DIFF  10  // (°C) Temperature difference that will trigger a print abort.
#endif

// Below this temperature the heater will be switched off
// because it probably indicates a broken thermistor wire.
#define HEATER_0_MINTEMP   5
#define HEATER_1_MINTEMP   5
#define HEATER_2_MINTEMP   5
#define HEATER_3_MINTEMP   5
#define HEATER_4_MINTEMP   5
#define HEATER_5_MINTEMP   5
#define HEATER_6_MINTEMP   5
#define HEATER_7_MINTEMP   5
#define BED_MINTEMP        5
#define CHAMBER_MINTEMP    5

// Above this temperature the heater will be switched off.
// This can protect components from overheating, but NOT from shorts and failures.
// (Use MINTEMP for thermistor short/failure protection.)
#define HEATER_0_MAXTEMP 275
#define HEATER_1_MAXTEMP 275
#define HEATER_2_MAXTEMP 275
#define HEATER_3_MAXTEMP 275
#define HEATER_4_MAXTEMP 275
#define HEATER_5_MAXTEMP 275
#define HEATER_6_MAXTEMP 275
#define HEATER_7_MAXTEMP 275
#define BED_MAXTEMP      150
#define CHAMBER_MAXTEMP  60

// Control heater 0 and heater 1 in parallel.
//#define HEATERS_PARALLEL

/**
 * Thermal Overshoot
 * During heatup (and printing) the temperature can often "overshoot" the target by many degrees
 * (especially before PID tuning). Setting the target temperature too close to MAXTEMP guarantees
 * a MAXTEMP shutdown! Use these values to forbid temperatures being set too close to MAXTEMP.
 */
#define HOTEND_OVERSHOOT 15   // (°C) Forbid temperatures over MAXTEMP - OVERSHOOT
#define BED_OVERSHOOT    10   // (°C) Forbid temperatures over MAXTEMP - OVERSHOOT
#define COOLER_OVERSHOOT  2   // (°C) Forbid temperatures closer than OVERSHOOT

//===========================================================================
//============================= PID Settings ================================
//===========================================================================

/**
 * Temperature Control
 *
 *  (NONE) : Bang-bang heating
 * PIDTEMP : PID temperature control (~4.1K)
 * MPCTEMP : Predictive Model temperature control. (~1.8K without auto-tune)
 */
#define PIDTEMP           // See the PID Tuning Guide at https://reprap.org/wiki/PID_Tuning
//#define MPCTEMP         // ** EXPERIMENTAL ** See https://marlinfw.org/docs/features/model_predictive_control.html

#define PID_MAX  255      // Limit hotend current while PID is active (see PID_FUNCTIONAL_RANGE below); 255=full current
#define PID_K1     0.95   // Smoothing factor within any PID loop

#if ENABLED(PIDTEMP)
  //#define PID_DEBUG             // Print PID debug data to the serial port. Use 'M303 D' to toggle activation.
  //#define PID_PARAMS_PER_HOTEND // Use separate PID parameters for each extruder (useful for mismatched extruders)
                                  // Set/get with G-code: M301 E[extruder number, 0-2]

  #if ENABLED(PID_PARAMS_PER_HOTEND)
    // Specify up to one value per hotend here, according to your setup.
    // If there are fewer values, the last one applies to the remaining hotends.
    #define DEFAULT_Kp_LIST {  22.20,  22.20 }
    #define DEFAULT_Ki_LIST {   1.08,   1.08 }
    #define DEFAULT_Kd_LIST { 114.00, 114.00 }
  #else
    #define DEFAULT_Kp  22.20
    #define DEFAULT_Ki   1.08
    #define DEFAULT_Kd 114.00
  #endif
#else
  #define BANG_MAX 255    // Limit hotend current while in bang-bang mode; 255=full current
#endif

//===========================================================================
//====================== PID > Bed Temperature Control ======================
//===========================================================================

/**
 * Max Bed Power
 * Applies to all forms of bed control (PID, bang-bang, and bang-bang with hysteresis).
 * When set to any value below 255, enables a form of PWM to the bed that acts like a divider
 * so don't use it unless you are OK with PWM on your bed. (See the comment on enabling PIDTEMPBED)
 */
#define MAX_BED_POWER 255 // limits duty cycle to bed; 255=full current

/**
 * PID Bed Heating
 *
 * The PID frequency will be the same as the extruder PWM.
 * If PID_dT is the default, and correct for the hardware/configuration, that means 7.689Hz,
 * which is fine for driving a square wave into a resistive load and does not significantly
 * impact FET heating. This also works fine on a Fotek SSR-10DA Solid State Relay into a 250W
 * heater. If your configuration is significantly different than this and you don't understand
 * the issues involved, don't use bed PID until someone else verifies that your hardware works.
 *
 * With this option disabled, bang-bang will be used. BED_LIMIT_SWITCHING enables hysteresis.
 */
//#define PIDTEMPBED

#if ENABLED(PIDTEMPBED)
  //#define MIN_BED_POWER 0
  //#define PID_BED_DEBUG // Print Bed PID debug data to the serial port.

  // 120V 250W silicone heater into 4mm borosilicate (MendelMax 1.5+)
  // from FOPDT model - kp=.39 Tp=405 Tdead=66, Tc set to 79.2, aggressive factor of .15 (vs .1, 1, 10)
  #define DEFAULT_bedKp 10.00
  #define DEFAULT_bedKi .023
  #define DEFAULT_bedKd 305.4

  // FIND YOUR OWN: "M303 E-1 C8 S90" to run autotune on the bed at 90 degreesC for 8 cycles.
#else
  #define BED_CHECK_INTERVAL 5000   // (ms) Interval between checks in bang-bang control
  //#define BED_LIMIT_SWITCHING     // Keep the bed temperature within BED_HYSTERESIS of the target
  #if ENABLED(BED_LIMIT_SWITCHING)
    #define BED_HYSTERESIS 2        // (°C) Only set the relevant heater state when ABS(T-target) > BED_HYSTERESIS
  #endif
#endif

//===========================================================================
//==================== PID > Chamber Temperature Control ====================
//===========================================================================

/**
 * PID Chamber Heating
 *
 * If this option is enabled set PID constants below.
 * If this option is disabled, bang-bang will be used and CHAMBER_LIMIT_SWITCHING will enable
 * hysteresis.
 *
 * The PID frequency will be the same as the extruder PWM.
 * If PID_dT is the default, and correct for the hardware/configuration, that means 7.689Hz,
 * which is fine for driving a square wave into a resistive load and does not significantly
 * impact FET heating. This also works fine on a Fotek SSR-10DA Solid State Relay into a 200W
 * heater. If your configuration is significantly different than this and you don't understand
 * the issues involved, don't use chamber PID until someone else verifies that your hardware works.
 * @section chamber temp
 */
//#define PIDTEMPCHAMBER
//#define CHAMBER_LIMIT_SWITCHING

/**
 * Max Chamber Power
 * Applies to all forms of chamber control (PID, bang-bang, and bang-bang with hysteresis).
 * When set to any value below 255, enables a form of PWM to the chamber heater that acts like a divider
 * so don't use it unless you are OK with PWM on your heater. (See the comment on enabling PIDTEMPCHAMBER)
 */
#define MAX_CHAMBER_POWER 255 // limits duty cycle to chamber heater; 255=full current

#if ENABLED(PIDTEMPCHAMBER)
  #define MIN_CHAMBER_POWER 0
  //#define PID_CHAMBER_DEBUG // Print Chamber PID debug data to the serial port.

  // Lasko "MyHeat Personal Heater" (200w) modified with a Fotek SSR-10DA to control only the heating element
  // and placed inside the small Creality printer enclosure tent.
  //
  #define DEFAULT_chamberKp 37.04
  #define DEFAULT_chamberKi 1.40
  #define DEFAULT_chamberKd 655.17
  // M309 P37.04 I1.04 D655.17

  // FIND YOUR OWN: "M303 E-2 C8 S50" to run autotune on the chamber at 50 degreesC for 8 cycles.
#endif // PIDTEMPCHAMBER

#if ANY(PIDTEMP, PIDTEMPBED, PIDTEMPCHAMBER)
  //#define PID_OPENLOOP          // Puts PID in open loop. M104/M140 sets the output power from 0 to PID_MAX
  //#define SLOW_PWM_HEATERS      // PWM with very low frequency (roughly 0.125Hz=8s) and minimum state time of approximately 1s useful for heaters driven by a relay
  #define PID_FUNCTIONAL_RANGE 10 // If the temperature difference between the target temperature and the actual temperature
                                  // is more than PID_FUNCTIONAL_RANGE then the PID will be shut off and the heater will be set to min/max.

  //#define PID_EDIT_MENU         // Add PID editing to the "Advanced Settings" menu. (~700 bytes of flash)
  //#define PID_AUTOTUNE_MENU     // Add PID auto-tuning to the "Advanced Settings" menu. (~250 bytes of flash)
#endif

// Calibration for AD595 / AD8495 sensor to adjust temperature measurements.
// The final temperature is calculated as (measuredTemp * GAIN) + OFFSET.
#define TEMP_SENSOR_AD595_OFFSET  0.0
#define TEMP_SENSOR_AD595_GAIN    1.0
#define TEMP_SENSOR_AD8495_OFFSET 0.0
#define TEMP_SENSOR_AD8495_GAIN   1.0

/**
 * High Temperature Thermistor Support
 *
 * Thermistors able to support high temperature tend to have a hard time getting
 * good readings at room and lower temperatures. This means TEMP_SENSOR_X_RAW_LO_TEMP
 * will probably be caught when the heating element first turns on during the
 * preheating process, which will trigger a MINTEMP error as a safety measure
 * and force stop everything.
 * To circumvent this limitation, we allow for a preheat time (during which,
 * MINTEMP error won't be triggered) and add a min_temp buffer to handle
 * aberrant readings.
 *
 * If you want to enable this feature for your hotend thermistor(s)
 * uncomment and set values > 0 in the constants below
 */

/**
 * Thermocouple sensors are quite sensitive to noise.  Any noise induced in
 * the sensor wires, such as by stepper motor wires run in parallel to them,
 * may result in the thermocouple sensor reporting spurious errors.  This
 * value is the number of errors which can occur in a row before the error
 * is reported.  This allows us to ignore intermittent error conditions while
 * still detecting an actual failure, which should result in a continuous
 * stream of errors from the sensor.
 *
 * Set this value to 0 to fail on the first error to occur.
 */
#define THERMOCOUPLE_MAX_ERRORS 15

// The number of consecutive low temperature errors that can occur
// before a MINTEMP error is triggered. (Shouldn't be more than 10.)
//#define MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED 0
