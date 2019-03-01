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
#pragma once

#include "../../inc/MarlinConfig.h"

#define OVERSAMPLENR 16
#define OV(N) int16_t((N) * (OVERSAMPLENR))

#define ANY_THERMISTOR_IS(n) (THERMISTORHEATER_0 == n || THERMISTORHEATER_1 == n || THERMISTORHEATER_2 == n || THERMISTORHEATER_3 == n || THERMISTORHEATER_4 == n || THERMISTORBED == n || THERMISTORCHAMBER == n)

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
#if ANY_THERMISTOR_IS(20) // PT100 with INA826 amp on Ultimaker v2.0 electronics
  #include "thermistor_20.h"
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
#if ANY_THERMISTOR_IS(110) // Pt100 with 1k0 pullup
  #include "thermistor_110.h"
#endif
#if ANY_THERMISTOR_IS(147) // Pt100 with 4k7 pullup
  #include "thermistor_147.h"
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

#define _TT_NAME(_N) temptable_ ## _N
#define TT_NAME(_N) _TT_NAME(_N)

#if THERMISTORHEATER_0
  #define HEATER_0_TEMPTABLE TT_NAME(THERMISTORHEATER_0)
  #define HEATER_0_TEMPTABLE_LEN COUNT(HEATER_0_TEMPTABLE)
#elif defined(HEATER_0_USES_THERMISTOR)
  #error "No heater 0 thermistor table specified"
#else
  #define HEATER_0_TEMPTABLE NULL
  #define HEATER_0_TEMPTABLE_LEN 0
#endif

#if THERMISTORHEATER_1
  #define HEATER_1_TEMPTABLE TT_NAME(THERMISTORHEATER_1)
  #define HEATER_1_TEMPTABLE_LEN COUNT(HEATER_1_TEMPTABLE)
#elif defined(HEATER_1_USES_THERMISTOR)
  #error "No heater 1 thermistor table specified"
#else
  #define HEATER_1_TEMPTABLE NULL
  #define HEATER_1_TEMPTABLE_LEN 0
#endif

#if THERMISTORHEATER_2
  #define HEATER_2_TEMPTABLE TT_NAME(THERMISTORHEATER_2)
  #define HEATER_2_TEMPTABLE_LEN COUNT(HEATER_2_TEMPTABLE)
#elif defined(HEATER_2_USES_THERMISTOR)
  #error "No heater 2 thermistor table specified"
#else
  #define HEATER_2_TEMPTABLE NULL
  #define HEATER_2_TEMPTABLE_LEN 0
#endif

#if THERMISTORHEATER_3
  #define HEATER_3_TEMPTABLE TT_NAME(THERMISTORHEATER_3)
  #define HEATER_3_TEMPTABLE_LEN COUNT(HEATER_3_TEMPTABLE)
#elif defined(HEATER_3_USES_THERMISTOR)
  #error "No heater 3 thermistor table specified"
#else
  #define HEATER_3_TEMPTABLE NULL
  #define HEATER_3_TEMPTABLE_LEN 0
#endif

#if THERMISTORHEATER_4
  #define HEATER_4_TEMPTABLE TT_NAME(THERMISTORHEATER_4)
  #define HEATER_4_TEMPTABLE_LEN COUNT(HEATER_4_TEMPTABLE)
#elif defined(HEATER_4_USES_THERMISTOR)
  #error "No heater 4 thermistor table specified"
#else
  #define HEATER_4_TEMPTABLE NULL
  #define HEATER_4_TEMPTABLE_LEN 0
#endif

#ifdef THERMISTORBED
  #define BEDTEMPTABLE TT_NAME(THERMISTORBED)
  #define BEDTEMPTABLE_LEN COUNT(BEDTEMPTABLE)
#elif defined(HEATER_BED_USES_THERMISTOR)
  #error "No bed thermistor table specified"
#else
  #define BEDTEMPTABLE_LEN 0
#endif

#ifdef THERMISTORCHAMBER
  #define CHAMBERTEMPTABLE TT_NAME(THERMISTORCHAMBER)
  #define CHAMBERTEMPTABLE_LEN COUNT(CHAMBERTEMPTABLE)
#elif defined(HEATER_CHAMBER_USES_THERMISTOR)
  #error "No chamber thermistor table specified"
#else
  #define CHAMBERTEMPTABLE_LEN 0
#endif

// The SCAN_THERMISTOR_TABLE macro needs alteration?
static_assert(HEATER_0_TEMPTABLE_LEN < 256 && HEATER_1_TEMPTABLE_LEN < 256 && HEATER_2_TEMPTABLE_LEN < 256 && HEATER_3_TEMPTABLE_LEN < 256 && HEATER_4_TEMPTABLE_LEN < 256 && BEDTEMPTABLE_LEN < 256 && CHAMBERTEMPTABLE_LEN < 256,
  "Temperature conversion tables over 255 entries need special consideration."
);

// Set the high and low raw values for the heaters
// For thermistors the highest temperature results in the lowest ADC value
// For thermocouples the highest temperature results in the highest ADC value
#ifndef HEATER_0_RAW_HI_TEMP
  #ifdef HEATER_0_USES_THERMISTOR
    #define HEATER_0_RAW_HI_TEMP 0
    #define HEATER_0_RAW_LO_TEMP 16383
  #else
    #define HEATER_0_RAW_HI_TEMP 16383
    #define HEATER_0_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_1_RAW_HI_TEMP
  #ifdef HEATER_1_USES_THERMISTOR
    #define HEATER_1_RAW_HI_TEMP 0
    #define HEATER_1_RAW_LO_TEMP 16383
  #else
    #define HEATER_1_RAW_HI_TEMP 16383
    #define HEATER_1_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_2_RAW_HI_TEMP
  #ifdef HEATER_2_USES_THERMISTOR
    #define HEATER_2_RAW_HI_TEMP 0
    #define HEATER_2_RAW_LO_TEMP 16383
  #else
    #define HEATER_2_RAW_HI_TEMP 16383
    #define HEATER_2_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_3_RAW_HI_TEMP
  #ifdef HEATER_3_USES_THERMISTOR
    #define HEATER_3_RAW_HI_TEMP 0
    #define HEATER_3_RAW_LO_TEMP 16383
  #else
    #define HEATER_3_RAW_HI_TEMP 16383
    #define HEATER_3_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_4_RAW_HI_TEMP
  #ifdef HEATER_4_USES_THERMISTOR
    #define HEATER_4_RAW_HI_TEMP 0
    #define HEATER_4_RAW_LO_TEMP 16383
  #else
    #define HEATER_4_RAW_HI_TEMP 16383
    #define HEATER_4_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_BED_RAW_HI_TEMP
  #ifdef HEATER_BED_USES_THERMISTOR
    #define HEATER_BED_RAW_HI_TEMP 0
    #define HEATER_BED_RAW_LO_TEMP 16383
  #else
    #define HEATER_BED_RAW_HI_TEMP 16383
    #define HEATER_BED_RAW_LO_TEMP 0
  #endif
#endif
#ifndef HEATER_CHAMBER_RAW_HI_TEMP
  #ifdef HEATER_CHAMBER_USES_THERMISTOR
    #define HEATER_CHAMBER_RAW_HI_TEMP 0
    #define HEATER_CHAMBER_RAW_LO_TEMP 16383
  #else
    #define HEATER_CHAMBER_RAW_HI_TEMP 16383
    #define HEATER_CHAMBER_RAW_LO_TEMP 0
  #endif
#endif
