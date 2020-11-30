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
#pragma once

#include "../../inc/MarlinConfig.h"

#define THERMISTOR_TABLE_ADC_RESOLUTION 10
#define THERMISTOR_TABLE_SCALE (HAL_ADC_RANGE / _BV(THERMISTOR_TABLE_ADC_RESOLUTION))
#if ENABLED(HAL_ADC_FILTERED)
  #define OVERSAMPLENR 1
#else
  #define OVERSAMPLENR 16
#endif
#define MAX_RAW_THERMISTOR_VALUE (HAL_ADC_RANGE * (OVERSAMPLENR) - 1)

// Currently Marlin stores all oversampled ADC values as int16_t, make sure the HAL settings do not overflow 15bit
#if MAX_RAW_THERMISTOR_VALUE > ((1 << 15) - 1)
  #error "MAX_RAW_THERMISTOR_VALUE is too large for int16_t. Reduce OVERSAMPLENR or HAL_ADC_RESOLUTION."
#endif

#define OV_SCALE(N) (N)
#define OV(N) int16_t(OV_SCALE(N) * (OVERSAMPLENR) * (THERMISTOR_TABLE_SCALE))

#define ANY_THERMISTOR_IS(n) (THERMISTOR_HEATER_0 == n || THERMISTOR_HEATER_1 == n || THERMISTOR_HEATER_2 == n || THERMISTOR_HEATER_3 == n || THERMISTOR_HEATER_4 == n || THERMISTOR_HEATER_5 == n || THERMISTOR_HEATER_6 == n || THERMISTOR_HEATER_7 == n || THERMISTORBED == n || THERMISTORCHAMBER == n || THERMISTORPROBE == n)

typedef struct { int16_t value, celsius; } temp_entry_t;

// Pt1000 and Pt100 handling
//
// Rt=R0*(1+a*T+b*T*T) [for T>0]
// a=3.9083E-3, b=-5.775E-7
#define PtA 3.9083E-3
#define PtB -5.775E-7
#define PtRt(T,R0) ((R0) * (1.0 + (PtA) * (T) + (PtB) * (T) * (T)))
#define PtAdVal(T,R0,Rup) (short)(1024 / (Rup / PtRt(T, R0) + 1))
#define PtLine(T,R0,Rup) { OV(PtAdVal(T, R0, Rup)), T }

#if ANY_THERMISTOR_IS(1) // beta25 = 4092 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "EPCOS"
  #include "thermistor_1.h"
#endif
#if ANY_THERMISTOR_IS(2) // 4338 K, R25 = 200 kOhm, Pull-up = 4.7 kOhm, "ATC Semitec 204GT-2"
  #include "thermistor_2.h"
#endif
#if ANY_THERMISTOR_IS(3) // beta25 = 4120 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Mendel-parts"
  #include "thermistor_3.h"
#endif
#if ANY_THERMISTOR_IS(4) // beta25 = 3950 K, R25 = 10 kOhm, Pull-up = 4.7 kOhm, "Generic"
  #include "thermistor_4.h"
#endif
#if ANY_THERMISTOR_IS(5) // beta25 = 4267 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "ParCan, ATC 104GT-2"
  #include "thermistor_5.h"
#endif
#if ANY_THERMISTOR_IS(501) // 100K Zonestar thermistor
  #include "thermistor_501.h"
#endif
#if ANY_THERMISTOR_IS(502) // Unknown thermistor used by the Zonestar Průša P802M hot bed
  #include "thermistor_502.h"
#endif
#if ANY_THERMISTOR_IS(503) // Zonestar (Z8XM2) Heated Bed thermistor
  #include "thermistor_503.h"
#endif
#if ANY_THERMISTOR_IS(512) // 100k thermistor in RPW-Ultra hotend, Pull-up = 4.7 kOhm, "unknown model"
  #include "thermistor_512.h"
#endif
#if ANY_THERMISTOR_IS(6) // beta25 = 4092 K, R25 = 100 kOhm, Pull-up = 8.2 kOhm, "EPCOS ?"
  #include "thermistor_6.h"
#endif
#if ANY_THERMISTOR_IS(7) // beta25 = 3974 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Honeywell 135-104LAG-J01"
  #include "thermistor_7.h"
#endif
#if ANY_THERMISTOR_IS(71) // beta25 = 3974 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Honeywell 135-104LAF-J01"
  #include "thermistor_71.h"
#endif
#if ANY_THERMISTOR_IS(8) // beta25 = 3950 K, R25 = 100 kOhm, Pull-up = 10 kOhm, "Vishay E3104FHT"
  #include "thermistor_8.h"
#endif
#if ANY_THERMISTOR_IS(9) // beta25 = 3960 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "GE Sensing AL03006-58.2K-97-G1"
  #include "thermistor_9.h"
#endif
#if ANY_THERMISTOR_IS(10) // beta25 = 3960 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "RS 198-961"
  #include "thermistor_10.h"
#endif
#if ANY_THERMISTOR_IS(11) // beta25 = 3950 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "QU-BD silicone bed, QWG-104F-3950"
  #include "thermistor_11.h"
#endif
#if ANY_THERMISTOR_IS(13) // beta25 = 4100 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Hisens"
  #include "thermistor_13.h"
#endif
#if ANY_THERMISTOR_IS(15) // JGAurora A5 thermistor calibration
  #include "thermistor_15.h"
#endif
#if ANY_THERMISTOR_IS(17) // Dagoma NTC 100k white thermistor
  #include "thermistor_17.h"
#endif
#if ANY_THERMISTOR_IS(18) // ATC Semitec 204GT-2 (4.7k pullup) Dagoma.Fr - MKS_Base_DKU001327
  #include "thermistor_18.h"
#endif
#if ANY_THERMISTOR_IS(20) // Pt100 with INA826 amp on Ultimaker v2.0 electronics
  #include "thermistor_20.h"
#endif
#if ANY_THERMISTOR_IS(21) // Pt100 with INA826 amp with 3.3v excitation based on "Pt100 with INA826 amp on Ultimaker v2.0 electronics"
  #include "thermistor_21.h"
#endif
#if ANY_THERMISTOR_IS(22) // Thermistor in a Rostock 301 hot end, calibrated with a multimeter
  #include "thermistor_22.h"
#endif
#if ANY_THERMISTOR_IS(23) // By AluOne #12622. Formerly 22 above. May need calibration/checking.
  #include "thermistor_23.h"
#endif
#if ANY_THERMISTOR_IS(30) // Kis3d Silicone mat 24V 200W/300W with 6mm Precision cast plate (EN AW 5083)
  #include "thermistor_30.h"
#endif
#if ANY_THERMISTOR_IS(51) // beta25 = 4092 K, R25 = 100 kOhm, Pull-up = 1 kOhm, "EPCOS"
  #include "thermistor_51.h"
#endif
#if ANY_THERMISTOR_IS(52) // beta25 = 4338 K, R25 = 200 kOhm, Pull-up = 1 kOhm, "ATC Semitec 204GT-2"
  #include "thermistor_52.h"
#endif
#if ANY_THERMISTOR_IS(55) // beta25 = 4267 K, R25 = 100 kOhm, Pull-up = 1 kOhm, "ATC Semitec 104GT-2 (Used on ParCan)"
  #include "thermistor_55.h"
#endif
#if ANY_THERMISTOR_IS(60) // beta25 = 3950 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Maker's Tool Works Kapton Bed"
  #include "thermistor_60.h"
#endif
#if ANY_THERMISTOR_IS(61) // beta25 = 3950 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Formbot 350°C Thermistor"
  #include "thermistor_61.h"
#endif
#if ANY_THERMISTOR_IS(66) // beta25 = 4500 K, R25 = 2.5 MOhm, Pull-up = 4.7 kOhm, "DyzeDesign 500 °C Thermistor"
  #include "thermistor_66.h"
#endif
#if ANY_THERMISTOR_IS(67) // R25 = 500 KOhm, beta25 = 3800 K, 4.7 kOhm pull-up, SliceEngineering 450 °C Thermistor
  #include "thermistor_67.h"
#endif
#if ANY_THERMISTOR_IS(12) // beta25 = 4700 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Personal calibration for Makibox hot bed"
  #include "thermistor_12.h"
#endif
#if ANY_THERMISTOR_IS(70) // beta25 = 4100 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "Hephestos 2, bqh2 stock thermistor"
  #include "thermistor_70.h"
#endif
#if ANY_THERMISTOR_IS(75) // beta25 = 4100 K, R25 = 100 kOhm, Pull-up = 4.7 kOhm, "MGB18-104F39050L32 thermistor"
  #include "thermistor_75.h"
#endif
#if ANY_THERMISTOR_IS(99) // 100k bed thermistor with a 10K pull-up resistor (on some Wanhao i3 models)
  #include "thermistor_99.h"
#endif
#if ANY_THERMISTOR_IS(110) // Pt100 with 1k0 pullup
  #include "thermistor_110.h"
#endif
#if ANY_THERMISTOR_IS(147) // Pt100 with 4k7 pullup
  #include "thermistor_147.h"
#endif
#if ANY_THERMISTOR_IS(201) // Pt100 with LMV324 Overlord
  #include "thermistor_201.h"
#endif
#if ANY_THERMISTOR_IS(202) // 200K thermistor in Copymaker3D hotend
  #include "thermistor_202.h"
#endif
#if ANY_THERMISTOR_IS(331) // Like table 1, but with 3V3 as input voltage for MEGA
  #include "thermistor_331.h"
#endif
#if ANY_THERMISTOR_IS(332) // Like table 1, but with 3V3 as input voltage for DUE
  #include "thermistor_332.h"
#endif
#if ANY_THERMISTOR_IS(666) // beta25 = UNK, R25 = 200K, Pull-up = 10 kOhm, "Unidentified 200K NTC thermistor (Einstart S)"
  #include "thermistor_666.h"
#endif
#if ANY_THERMISTOR_IS(1010) // Pt1000 with 1k0 pullup
  #include "thermistor_1010.h"
#endif
#if ANY_THERMISTOR_IS(1047) // Pt1000 with 4k7 pullup
  #include "thermistor_1047.h"
#endif
#if ANY_THERMISTOR_IS(998) // User-defined table 1
  #include "thermistor_998.h"
#endif
#if ANY_THERMISTOR_IS(999) // User-defined table 2
  #include "thermistor_999.h"
#endif
#if ANY_THERMISTOR_IS(1000) // Custom
  const temp_entry_t temptable_1000[] PROGMEM = { { 0, 0 } };
#endif

#define _TT_NAME(_N) temptable_ ## _N
#define TT_NAME(_N) _TT_NAME(_N)


#if THERMISTOR_HEATER_0
  #define HEATER_0_TEMPTABLE TT_NAME(THERMISTOR_HEATER_0)
  #define HEATER_0_TEMPTABLE_LEN COUNT(HEATER_0_TEMPTABLE)
#elif HEATER_0_USES_THERMISTOR
  #error "No heater 0 thermistor table specified"
#else
  #define HEATER_0_TEMPTABLE nullptr
  #define HEATER_0_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_1
  #define HEATER_1_TEMPTABLE TT_NAME(THERMISTOR_HEATER_1)
  #define HEATER_1_TEMPTABLE_LEN COUNT(HEATER_1_TEMPTABLE)
#elif HEATER_1_USES_THERMISTOR
  #error "No heater 1 thermistor table specified"
#else
  #define HEATER_1_TEMPTABLE nullptr
  #define HEATER_1_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_2
  #define HEATER_2_TEMPTABLE TT_NAME(THERMISTOR_HEATER_2)
  #define HEATER_2_TEMPTABLE_LEN COUNT(HEATER_2_TEMPTABLE)
#elif HEATER_2_USES_THERMISTOR
  #error "No heater 2 thermistor table specified"
#else
  #define HEATER_2_TEMPTABLE nullptr
  #define HEATER_2_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_3
  #define HEATER_3_TEMPTABLE TT_NAME(THERMISTOR_HEATER_3)
  #define HEATER_3_TEMPTABLE_LEN COUNT(HEATER_3_TEMPTABLE)
#elif HEATER_3_USES_THERMISTOR
  #error "No heater 3 thermistor table specified"
#else
  #define HEATER_3_TEMPTABLE nullptr
  #define HEATER_3_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_4
  #define HEATER_4_TEMPTABLE TT_NAME(THERMISTOR_HEATER_4)
  #define HEATER_4_TEMPTABLE_LEN COUNT(HEATER_4_TEMPTABLE)
#elif HEATER_4_USES_THERMISTOR
  #error "No heater 4 thermistor table specified"
#else
  #define HEATER_4_TEMPTABLE nullptr
  #define HEATER_4_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_5
  #define HEATER_5_TEMPTABLE TT_NAME(THERMISTOR_HEATER_5)
  #define HEATER_5_TEMPTABLE_LEN COUNT(HEATER_5_TEMPTABLE)
#elif HEATER_5_USES_THERMISTOR
  #error "No heater 5 thermistor table specified"
#else
  #define HEATER_5_TEMPTABLE nullptr
  #define HEATER_5_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_6
  #define HEATER_6_TEMPTABLE TT_NAME(THERMISTOR_HEATER_6)
  #define HEATER_6_TEMPTABLE_LEN COUNT(HEATER_6_TEMPTABLE)
#elif HEATER_6_USES_THERMISTOR
  #error "No heater 6 thermistor table specified"
#else
  #define HEATER_6_TEMPTABLE nullptr
  #define HEATER_6_TEMPTABLE_LEN 0
#endif

#if THERMISTOR_HEATER_7
  #define HEATER_7_TEMPTABLE TT_NAME(THERMISTOR_HEATER_7)
  #define HEATER_7_TEMPTABLE_LEN COUNT(HEATER_7_TEMPTABLE)
#elif HEATER_7_USES_THERMISTOR
  #error "No heater 7 thermistor table specified"
#else
  #define HEATER_7_TEMPTABLE nullptr
  #define HEATER_7_TEMPTABLE_LEN 0
#endif

#ifdef THERMISTORBED
  #define BED_TEMPTABLE TT_NAME(THERMISTORBED)
  #define BED_TEMPTABLE_LEN COUNT(BED_TEMPTABLE)
#elif HEATER_BED_USES_THERMISTOR
  #error "No bed thermistor table specified"
#else
  #define BED_TEMPTABLE_LEN 0
#endif

#ifdef THERMISTORCHAMBER
  #define CHAMBER_TEMPTABLE TT_NAME(THERMISTORCHAMBER)
  #define CHAMBER_TEMPTABLE_LEN COUNT(CHAMBER_TEMPTABLE)
#elif HEATER_CHAMBER_USES_THERMISTOR
  #error "No chamber thermistor table specified"
#else
  #define CHAMBER_TEMPTABLE_LEN 0
#endif

#ifdef THERMISTORPROBE
  #define PROBE_TEMPTABLE TT_NAME(THERMISTORPROBE)
  #define PROBE_TEMPTABLE_LEN COUNT(PROBE_TEMPTABLE)
#elif HEATER_PROBE_USES_THERMISTOR
  #error "No probe thermistor table specified"
#else
  #define PROBE_TEMPTABLE_LEN 0
#endif

// The SCAN_THERMISTOR_TABLE macro needs alteration?
static_assert(
     HEATER_0_TEMPTABLE_LEN < 256 && HEATER_1_TEMPTABLE_LEN < 256
  && HEATER_2_TEMPTABLE_LEN < 256 && HEATER_3_TEMPTABLE_LEN < 256
  && HEATER_4_TEMPTABLE_LEN < 256 && HEATER_5_TEMPTABLE_LEN < 256
  && HEATER_6_TEMPTABLE_LEN < 256 && HEATER_7_TEMPTABLE_LEN < 256
  &&      BED_TEMPTABLE_LEN < 256 &&  CHAMBER_TEMPTABLE_LEN < 256
  &&    PROBE_TEMPTABLE_LEN < 256,
  "Temperature conversion tables over 255 entries need special consideration."
);

// Set the high and low raw values for the heaters
// For thermistors the highest temperature results in the lowest ADC value
// For thermocouples the highest temperature results in the highest ADC value

#define _TT_REV(N) REVERSE_TEMP_SENSOR_RANGE_##N
#define TT_REV(N) _TT_REV(N)

#ifdef HEATER_0_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_0)
    #define HEATER_0_SENSOR_MINTEMP_IND 0
    #define HEATER_0_SENSOR_MAXTEMP_IND HEATER_0_TEMPTABLE_LEN - 1
  #else
    #define HEATER_0_SENSOR_MINTEMP_IND HEATER_0_TEMPTABLE_LEN - 1
    #define HEATER_0_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_1_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_1)
    #define HEATER_1_SENSOR_MINTEMP_IND 0
    #define HEATER_1_SENSOR_MAXTEMP_IND HEATER_1_TEMPTABLE_LEN - 1
  #else
    #define HEATER_1_SENSOR_MINTEMP_IND HEATER_1_TEMPTABLE_LEN - 1
    #define HEATER_1_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_2_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_2)
    #define HEATER_2_SENSOR_MINTEMP_IND 0
    #define HEATER_2_SENSOR_MAXTEMP_IND HEATER_2_TEMPTABLE_LEN - 1
  #else
    #define HEATER_2_SENSOR_MINTEMP_IND HEATER_2_TEMPTABLE_LEN - 1
    #define HEATER_2_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_3_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_3)
    #define HEATER_3_SENSOR_MINTEMP_IND 0
    #define HEATER_3_SENSOR_MAXTEMP_IND HEATER_3_TEMPTABLE_LEN - 1
  #else
    #define HEATER_3_SENSOR_MINTEMP_IND HEATER_3_TEMPTABLE_LEN - 1
    #define HEATER_3_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_4_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_4)
    #define HEATER_4_SENSOR_MINTEMP_IND 0
    #define HEATER_4_SENSOR_MAXTEMP_IND HEATER_4_TEMPTABLE_LEN - 1
  #else
    #define HEATER_4_SENSOR_MINTEMP_IND HEATER_4_TEMPTABLE_LEN - 1
    #define HEATER_4_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_5_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_5)
    #define HEATER_5_SENSOR_MINTEMP_IND 0
    #define HEATER_5_SENSOR_MAXTEMP_IND HEATER_5_TEMPTABLE_LEN - 1
  #else
    #define HEATER_5_SENSOR_MINTEMP_IND HEATER_5_TEMPTABLE_LEN - 1
    #define HEATER_5_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_6_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_6)
    #define HEATER_6_SENSOR_MINTEMP_IND 0
    #define HEATER_6_SENSOR_MAXTEMP_IND HEATER_6_TEMPTABLE_LEN - 1
  #else
    #define HEATER_6_SENSOR_MINTEMP_IND HEATER_6_TEMPTABLE_LEN - 1
    #define HEATER_6_SENSOR_MAXTEMP_IND 0
  #endif
#endif
#ifdef HEATER_7_TEMPTABLE
  #if TT_REV(THERMISTOR_HEATER_7)
    #define HEATER_7_SENSOR_MINTEMP_IND 0
    #define HEATER_7_SENSOR_MAXTEMP_IND HEATER_7_TEMPTABLE_LEN - 1
  #else
    #define HEATER_7_SENSOR_MINTEMP_IND HEATER_7_TEMPTABLE_LEN - 1
    #define HEATER_7_SENSOR_MAXTEMP_IND 0
  #endif
#endif

#ifndef HEATER_0_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_0) || !HEATER_0_USES_THERMISTOR
    #define HEATER_0_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_0_RAW_LO_TEMP 0
  #else
    #define HEATER_0_RAW_HI_TEMP 0
    #define HEATER_0_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_1_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_1) || !HEATER_1_USES_THERMISTOR
    #define HEATER_1_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_1_RAW_LO_TEMP 0
  #else
    #define HEATER_1_RAW_HI_TEMP 0
    #define HEATER_1_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_2_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_2) || !HEATER_2_USES_THERMISTOR
    #define HEATER_2_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_2_RAW_LO_TEMP 0
  #else
    #define HEATER_2_RAW_HI_TEMP 0
    #define HEATER_2_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_3_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_3) || !HEATER_3_USES_THERMISTOR
    #define HEATER_3_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_3_RAW_LO_TEMP 0
  #else
    #define HEATER_3_RAW_HI_TEMP 0
    #define HEATER_3_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_4_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_4) || !HEATER_4_USES_THERMISTOR
    #define HEATER_4_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_4_RAW_LO_TEMP 0
  #else
    #define HEATER_4_RAW_HI_TEMP 0
    #define HEATER_4_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_5_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_5) || !HEATER_5_USES_THERMISTOR
    #define HEATER_5_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_5_RAW_LO_TEMP 0
  #else
    #define HEATER_5_RAW_HI_TEMP 0
    #define HEATER_5_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_6_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_6) || !HEATER_6_USES_THERMISTOR
    #define HEATER_6_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_6_RAW_LO_TEMP 0
  #else
    #define HEATER_6_RAW_HI_TEMP 0
    #define HEATER_6_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_7_RAW_HI_TEMP
  #if TT_REV(THERMISTOR_HEATER_7) || !HEATER_7_USES_THERMISTOR
    #define HEATER_7_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_7_RAW_LO_TEMP 0
  #else
    #define HEATER_7_RAW_HI_TEMP 0
    #define HEATER_7_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_BED_RAW_HI_TEMP
  #if TT_REV(THERMISTORBED) || !HEATER_BED_USES_THERMISTOR
    #define HEATER_BED_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_BED_RAW_LO_TEMP 0
  #else
    #define HEATER_BED_RAW_HI_TEMP 0
    #define HEATER_BED_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_CHAMBER_RAW_HI_TEMP
  #if TT_REV(THERMISTORCHAMBER) || !HEATER_CHAMBER_USES_THERMISTOR
    #define HEATER_CHAMBER_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_CHAMBER_RAW_LO_TEMP 0
  #else
    #define HEATER_CHAMBER_RAW_HI_TEMP 0
    #define HEATER_CHAMBER_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef HEATER_PROBE_RAW_HI_TEMP
  #if TT_REV(THERMISTORPROBE) || !HEATER_PROBE_USES_THERMISTOR
    #define HEATER_PROBE_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define HEATER_PROBE_RAW_LO_TEMP 0
  #else
    #define HEATER_PROBE_RAW_HI_TEMP 0
    #define HEATER_PROBE_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif

#undef _TT_REV
#undef TT_REV
