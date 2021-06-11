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
#elif HAL_ADC_RESOLUTION > 10
  #define OVERSAMPLENR (20 - HAL_ADC_RESOLUTION)
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

#define ANY_THERMISTOR_IS(n) (TEMP_SENSOR_0_THERMISTOR_ID == n || TEMP_SENSOR_1_THERMISTOR_ID == n || TEMP_SENSOR_2_THERMISTOR_ID == n || TEMP_SENSOR_3_THERMISTOR_ID == n || TEMP_SENSOR_4_THERMISTOR_ID == n || TEMP_SENSOR_5_THERMISTOR_ID == n || TEMP_SENSOR_6_THERMISTOR_ID == n || TEMP_SENSOR_7_THERMISTOR_ID == n || TEMP_SENSOR_BED_THERMISTOR_ID == n || TEMP_SENSOR_CHAMBER_THERMISTOR_ID == n || TEMP_SENSOR_COOLER_THERMISTOR_ID == n || TEMP_SENSOR_PROBE_THERMISTOR_ID == n)

typedef struct { int16_t value; celsius_t celsius; } temp_entry_t;

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


#if TEMP_SENSOR_0_THERMISTOR_ID
  #define TEMPTABLE_0 TT_NAME(TEMP_SENSOR_0_THERMISTOR_ID)
  #define TEMPTABLE_0_LEN COUNT(TEMPTABLE_0)
#elif TEMP_SENSOR_0_IS_THERMISTOR
  #error "No heater 0 thermistor table specified"
#else
  #define TEMPTABLE_0 nullptr
  #define TEMPTABLE_0_LEN 0
#endif

#if TEMP_SENSOR_1_THERMISTOR_ID
  #define TEMPTABLE_1 TT_NAME(TEMP_SENSOR_1_THERMISTOR_ID)
  #define TEMPTABLE_1_LEN COUNT(TEMPTABLE_1)
#elif TEMP_SENSOR_1_IS_THERMISTOR
  #error "No heater 1 thermistor table specified"
#else
  #define TEMPTABLE_1 nullptr
  #define TEMPTABLE_1_LEN 0
#endif

#if TEMP_SENSOR_2_THERMISTOR_ID
  #define TEMPTABLE_2 TT_NAME(TEMP_SENSOR_2_THERMISTOR_ID)
  #define TEMPTABLE_2_LEN COUNT(TEMPTABLE_2)
#elif TEMP_SENSOR_2_IS_THERMISTOR
  #error "No heater 2 thermistor table specified"
#else
  #define TEMPTABLE_2 nullptr
  #define TEMPTABLE_2_LEN 0
#endif

#if TEMP_SENSOR_3_THERMISTOR_ID
  #define TEMPTABLE_3 TT_NAME(TEMP_SENSOR_3_THERMISTOR_ID)
  #define TEMPTABLE_3_LEN COUNT(TEMPTABLE_3)
#elif TEMP_SENSOR_3_IS_THERMISTOR
  #error "No heater 3 thermistor table specified"
#else
  #define TEMPTABLE_3 nullptr
  #define TEMPTABLE_3_LEN 0
#endif

#if TEMP_SENSOR_4_THERMISTOR_ID
  #define TEMPTABLE_4 TT_NAME(TEMP_SENSOR_4_THERMISTOR_ID)
  #define TEMPTABLE_4_LEN COUNT(TEMPTABLE_4)
#elif TEMP_SENSOR_4_IS_THERMISTOR
  #error "No heater 4 thermistor table specified"
#else
  #define TEMPTABLE_4 nullptr
  #define TEMPTABLE_4_LEN 0
#endif

#if TEMP_SENSOR_5_THERMISTOR_ID
  #define TEMPTABLE_5 TT_NAME(TEMP_SENSOR_5_THERMISTOR_ID)
  #define TEMPTABLE_5_LEN COUNT(TEMPTABLE_5)
#elif TEMP_SENSOR_5_IS_THERMISTOR
  #error "No heater 5 thermistor table specified"
#else
  #define TEMPTABLE_5 nullptr
  #define TEMPTABLE_5_LEN 0
#endif

#if TEMP_SENSOR_6_THERMISTOR_ID
  #define TEMPTABLE_6 TT_NAME(TEMP_SENSOR_6_THERMISTOR_ID)
  #define TEMPTABLE_6_LEN COUNT(TEMPTABLE_6)
#elif TEMP_SENSOR_6_IS_THERMISTOR
  #error "No heater 6 thermistor table specified"
#else
  #define TEMPTABLE_6 nullptr
  #define TEMPTABLE_6_LEN 0
#endif

#if TEMP_SENSOR_7_THERMISTOR_ID
  #define TEMPTABLE_7 TT_NAME(TEMP_SENSOR_7_THERMISTOR_ID)
  #define TEMPTABLE_7_LEN COUNT(TEMPTABLE_7)
#elif TEMP_SENSOR_7_IS_THERMISTOR
  #error "No heater 7 thermistor table specified"
#else
  #define TEMPTABLE_7 nullptr
  #define TEMPTABLE_7_LEN 0
#endif

#ifdef TEMP_SENSOR_BED_THERMISTOR_ID
  #define TEMPTABLE_BED TT_NAME(TEMP_SENSOR_BED_THERMISTOR_ID)
  #define TEMPTABLE_BED_LEN COUNT(TEMPTABLE_BED)
#elif TEMP_SENSOR_BED_IS_THERMISTOR
  #error "No bed thermistor table specified"
#else
  #define TEMPTABLE_BED_LEN 0
#endif

#ifdef TEMP_SENSOR_CHAMBER_THERMISTOR_ID
  #define TEMPTABLE_CHAMBER TT_NAME(TEMP_SENSOR_CHAMBER_THERMISTOR_ID)
  #define TEMPTABLE_CHAMBER_LEN COUNT(TEMPTABLE_CHAMBER)
#elif TEMP_SENSOR_CHAMBER_IS_THERMISTOR
  #error "No chamber thermistor table specified"
#else
  #define TEMPTABLE_CHAMBER_LEN 0
#endif

#ifdef TEMP_SENSOR_COOLER_THERMISTOR_ID
  #define TEMPTABLE_COOLER TT_NAME(TEMP_SENSOR_COOLER_THERMISTOR_ID)
  #define TEMPTABLE_COOLER_LEN COUNT(TEMPTABLE_COOLER)
#elif TEMP_SENSOR_COOLER_IS_THERMISTOR
  #error "No cooler thermistor table specified"
#else
  #define TEMPTABLE_COOLER_LEN 0
#endif
#ifdef TEMP_SENSOR_PROBE_THERMISTOR_ID
  #define TEMPTABLE_PROBE TT_NAME(TEMP_SENSOR_PROBE_THERMISTOR_ID)
  #define TEMPTABLE_PROBE_LEN COUNT(TEMPTABLE_PROBE)
#elif TEMP_SENSOR_PROBE_IS_THERMISTOR
  #error "No probe thermistor table specified"
#else
  #define TEMPTABLE_PROBE_LEN 0
#endif

// The SCAN_THERMISTOR_TABLE macro needs alteration?
static_assert(
     TEMPTABLE_0_LEN < 256 && TEMPTABLE_1_LEN < 256
  && TEMPTABLE_2_LEN < 256 && TEMPTABLE_3_LEN < 256
  && TEMPTABLE_4_LEN < 256 && TEMPTABLE_5_LEN < 256
  && TEMPTABLE_6_LEN < 256 && TEMPTABLE_7_LEN < 256
  && TEMPTABLE_BED_LEN < 256 && TEMPTABLE_CHAMBER_LEN < 256
  && TEMPTABLE_COOLER_LEN < 256 && TEMPTABLE_PROBE_LEN < 256,
  "Temperature conversion tables over 255 entries need special consideration."
);

// Set the high and low raw values for the heaters
// For thermistors the highest temperature results in the lowest ADC value
// For thermocouples the highest temperature results in the highest ADC value

#define __TT_REV(N)   REVERSE_TEMP_SENSOR_RANGE_##N
#define _TT_REV(N)    __TT_REV(N)
#define TT_REV(N)     _TT_REV(TEMP_SENSOR_##N##_THERMISTOR_ID)
#define _TT_REVRAW(N) !TEMP_SENSOR_##N##_IS_THERMISTOR
#define TT_REVRAW(N)  (TT_REV(N) || _TT_REVRAW(N))

#ifdef TEMPTABLE_0
  #if TT_REV(0)
    #define TEMP_SENSOR_0_MINTEMP_IND 0
    #define TEMP_SENSOR_0_MAXTEMP_IND TEMPTABLE_0_LEN - 1
  #else
    #define TEMP_SENSOR_0_MINTEMP_IND TEMPTABLE_0_LEN - 1
    #define TEMP_SENSOR_0_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_1
  #if TT_REV(1)
    #define TEMP_SENSOR_1_MINTEMP_IND 0
    #define TEMP_SENSOR_1_MAXTEMP_IND TEMPTABLE_1_LEN - 1
  #else
    #define TEMP_SENSOR_1_MINTEMP_IND TEMPTABLE_1_LEN - 1
    #define TEMP_SENSOR_1_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_2
  #if TT_REV(2)
    #define TEMP_SENSOR_2_MINTEMP_IND 0
    #define TEMP_SENSOR_2_MAXTEMP_IND TEMPTABLE_2_LEN - 1
  #else
    #define TEMP_SENSOR_2_MINTEMP_IND TEMPTABLE_2_LEN - 1
    #define TEMP_SENSOR_2_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_3
  #if TT_REV(3)
    #define TEMP_SENSOR_3_MINTEMP_IND 0
    #define TEMP_SENSOR_3_MAXTEMP_IND TEMPTABLE_3_LEN - 1
  #else
    #define TEMP_SENSOR_3_MINTEMP_IND TEMPTABLE_3_LEN - 1
    #define TEMP_SENSOR_3_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_4
  #if TT_REV(4)
    #define TEMP_SENSOR_4_MINTEMP_IND 0
    #define TEMP_SENSOR_4_MAXTEMP_IND TEMPTABLE_4_LEN - 1
  #else
    #define TEMP_SENSOR_4_MINTEMP_IND TEMPTABLE_4_LEN - 1
    #define TEMP_SENSOR_4_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_5
  #if TT_REV(5)
    #define TEMP_SENSOR_5_MINTEMP_IND 0
    #define TEMP_SENSOR_5_MAXTEMP_IND TEMPTABLE_5_LEN - 1
  #else
    #define TEMP_SENSOR_5_MINTEMP_IND TEMPTABLE_5_LEN - 1
    #define TEMP_SENSOR_5_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_6
  #if TT_REV(6)
    #define TEMP_SENSOR_6_MINTEMP_IND 0
    #define TEMP_SENSOR_6_MAXTEMP_IND TEMPTABLE_6_LEN - 1
  #else
    #define TEMP_SENSOR_6_MINTEMP_IND TEMPTABLE_6_LEN - 1
    #define TEMP_SENSOR_6_MAXTEMP_IND 0
  #endif
#endif
#ifdef TEMPTABLE_7
  #if TT_REV(7)
    #define TEMP_SENSOR_7_MINTEMP_IND 0
    #define TEMP_SENSOR_7_MAXTEMP_IND TEMPTABLE_7_LEN - 1
  #else
    #define TEMP_SENSOR_7_MINTEMP_IND TEMPTABLE_7_LEN - 1
    #define TEMP_SENSOR_7_MAXTEMP_IND 0
  #endif
#endif

#ifndef TEMP_SENSOR_0_RAW_HI_TEMP
  #if TT_REVRAW(0)
    #define TEMP_SENSOR_0_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_0_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_0_RAW_HI_TEMP 0
    #define TEMP_SENSOR_0_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_1_RAW_HI_TEMP
  #if TT_REVRAW(1)
    #define TEMP_SENSOR_1_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_1_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_1_RAW_HI_TEMP 0
    #define TEMP_SENSOR_1_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_2_RAW_HI_TEMP
  #if TT_REVRAW(2)
    #define TEMP_SENSOR_2_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_2_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_2_RAW_HI_TEMP 0
    #define TEMP_SENSOR_2_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_3_RAW_HI_TEMP
  #if TT_REVRAW(3)
    #define TEMP_SENSOR_3_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_3_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_3_RAW_HI_TEMP 0
    #define TEMP_SENSOR_3_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_4_RAW_HI_TEMP
  #if TT_REVRAW(4)
    #define TEMP_SENSOR_4_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_4_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_4_RAW_HI_TEMP 0
    #define TEMP_SENSOR_4_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_5_RAW_HI_TEMP
  #if TT_REVRAW(5)
    #define TEMP_SENSOR_5_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_5_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_5_RAW_HI_TEMP 0
    #define TEMP_SENSOR_5_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_6_RAW_HI_TEMP
  #if TT_REVRAW(6)
    #define TEMP_SENSOR_6_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_6_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_6_RAW_HI_TEMP 0
    #define TEMP_SENSOR_6_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_7_RAW_HI_TEMP
  #if TT_REVRAW(7)
    #define TEMP_SENSOR_7_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_7_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_7_RAW_HI_TEMP 0
    #define TEMP_SENSOR_7_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_BED_RAW_HI_TEMP
  #if TT_REVRAW(BED)
    #define TEMP_SENSOR_BED_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_BED_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_BED_RAW_HI_TEMP 0
    #define TEMP_SENSOR_BED_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_CHAMBER_RAW_HI_TEMP
  #if TT_REVRAW(CHAMBER)
    #define TEMP_SENSOR_CHAMBER_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_CHAMBER_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_CHAMBER_RAW_HI_TEMP 0
    #define TEMP_SENSOR_CHAMBER_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_COOLER_RAW_HI_TEMP
  #if TT_REVRAW(COOLER)
    #define TEMP_SENSOR_COOLER_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_COOLER_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_COOLER_RAW_HI_TEMP 0
    #define TEMP_SENSOR_COOLER_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif
#ifndef TEMP_SENSOR_PROBE_RAW_HI_TEMP
  #if TT_REVRAW(PROBE)
    #define TEMP_SENSOR_PROBE_RAW_HI_TEMP MAX_RAW_THERMISTOR_VALUE
    #define TEMP_SENSOR_PROBE_RAW_LO_TEMP 0
  #else
    #define TEMP_SENSOR_PROBE_RAW_HI_TEMP 0
    #define TEMP_SENSOR_PROBE_RAW_LO_TEMP MAX_RAW_THERMISTOR_VALUE
  #endif
#endif

#undef __TT_REV
#undef _TT_REV
#undef TT_REV
#undef _TT_REVRAW
#undef TT_REVRAW
