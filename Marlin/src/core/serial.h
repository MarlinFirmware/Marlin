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

#include "../inc/MarlinConfig.h"

#if HAS_ETHERNET
  #include "../feature/ethernet.h"
#endif

/**
 * Define debug bit-masks
 */
enum MarlinDebugFlags : uint8_t {
  MARLIN_DEBUG_NONE          = 0,
  MARLIN_DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  MARLIN_DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  MARLIN_DEBUG_ERRORS        = _BV(2), ///< Not implemented
  MARLIN_DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  MARLIN_DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    MARLIN_DEBUG_LEVELING    = _BV(5), ///< Print detailed output for homing and leveling
    MARLIN_DEBUG_MESH_ADJUST = _BV(6), ///< UBL bed leveling
  #else
    MARLIN_DEBUG_LEVELING    = 0,
    MARLIN_DEBUG_MESH_ADJUST = 0,
  #endif
  MARLIN_DEBUG_ALL           = 0xFF
};

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (MARLIN_DEBUG_## F))

#define SERIAL_BOTH 0x7F
#if HAS_MULTI_SERIAL
  extern int8_t serial_port_index;
  #define _PORT_REDIRECT(n,p)   REMEMBER(n,serial_port_index,p)
  #define _PORT_RESTORE(n)      RESTORE(n)

  #ifdef SERIAL_CATCHALL
    #define SERIAL_OUT(WHAT, V...) (void)CAT(MYSERIAL,SERIAL_CATCHALL).WHAT(V)
  #else
    #define SERIAL_OUT(WHAT, V...) do{ \
      const bool port2_open = TERN1(HAS_ETHERNET, ethernet.have_telnet_client); \
      if ( serial_port_index == 0 || serial_port_index == SERIAL_BOTH)                (void)MYSERIAL0.WHAT(V); \
      if ((serial_port_index == 1 || serial_port_index == SERIAL_BOTH) && port2_open) (void)MYSERIAL1.WHAT(V); \
    }while(0)
  #endif

  #define SERIAL_ASSERT(P)      if(serial_port_index!=(P)){ debugger(); }
#else
  #define _PORT_REDIRECT(n,p)   NOOP
  #define _PORT_RESTORE(n)      NOOP
  #define SERIAL_OUT(WHAT, V...) (void)MYSERIAL0.WHAT(V)
  #define SERIAL_ASSERT(P)      NOOP
#endif

#define PORT_REDIRECT(p)        _PORT_REDIRECT(1,p)
#define PORT_RESTORE()          _PORT_RESTORE(1)

#define SERIAL_ECHO(x)          SERIAL_OUT(print, x)
#define SERIAL_ECHO_F(V...)     SERIAL_OUT(print, V)
#define SERIAL_ECHOLN(x)        SERIAL_OUT(println, x)
#define SERIAL_PRINT(x,b)       SERIAL_OUT(print, x, b)
#define SERIAL_PRINTLN(x,b)     SERIAL_OUT(println, x, b)
#define SERIAL_PRINTF(V...)     SERIAL_OUT(printf, V)
#define SERIAL_FLUSH()          SERIAL_OUT(flush)

#ifdef ARDUINO_ARCH_STM32
  #define SERIAL_FLUSHTX()      SERIAL_OUT(flush)
#elif TX_BUFFER_SIZE > 0
  #define SERIAL_FLUSHTX()      SERIAL_OUT(flushTX)
#else
  #define SERIAL_FLUSHTX()
#endif

// Print up to 10 chars from a list
#define __CHAR_N(N,V...)  _CHAR_##N(V)
#define _CHAR_N(N,V...)   __CHAR_N(N,V)
#define _CHAR_1(c)        SERIAL_OUT(write, c)
#define _CHAR_2(a,b)      do{ _CHAR_1(a); _CHAR_1(b); }while(0)
#define _CHAR_3(a,V...)   do{ _CHAR_1(a); _CHAR_2(V); }while(0)
#define _CHAR_4(a,V...)   do{ _CHAR_1(a); _CHAR_3(V); }while(0)
#define _CHAR_5(a,V...)   do{ _CHAR_1(a); _CHAR_4(V); }while(0)
#define _CHAR_6(a,V...)   do{ _CHAR_1(a); _CHAR_5(V); }while(0)
#define _CHAR_7(a,V...)   do{ _CHAR_1(a); _CHAR_6(V); }while(0)
#define _CHAR_8(a,V...)   do{ _CHAR_1(a); _CHAR_7(V); }while(0)
#define _CHAR_9(a,V...)   do{ _CHAR_1(a); _CHAR_8(V); }while(0)
#define _CHAR_10(a,V...)  do{ _CHAR_1(a); _CHAR_9(V); }while(0)

#define SERIAL_CHAR(V...) _CHAR_N(NUM_ARGS(V),V)

// Print up to 12 pairs of values. Odd elements auto-wrapped in PSTR().
#define __SEP_N(N,V...)   _SEP_##N(V)
#define _SEP_N(N,V...)    __SEP_N(N,V)
#define _SEP_1(PRE)       SERIAL_ECHOPGM(PRE)
#define _SEP_2(PRE,V)     serial_echopair_PGM(PSTR(PRE),V)
#define _SEP_3(a,b,c)     do{ _SEP_2(a,b); SERIAL_ECHOPGM(c); }while(0)
#define _SEP_4(a,b,V...)  do{ _SEP_2(a,b); _SEP_2(V); }while(0)
#define _SEP_5(a,b,V...)  do{ _SEP_2(a,b); _SEP_3(V); }while(0)
#define _SEP_6(a,b,V...)  do{ _SEP_2(a,b); _SEP_4(V); }while(0)
#define _SEP_7(a,b,V...)  do{ _SEP_2(a,b); _SEP_5(V); }while(0)
#define _SEP_8(a,b,V...)  do{ _SEP_2(a,b); _SEP_6(V); }while(0)
#define _SEP_9(a,b,V...)  do{ _SEP_2(a,b); _SEP_7(V); }while(0)
#define _SEP_10(a,b,V...) do{ _SEP_2(a,b); _SEP_8(V); }while(0)
#define _SEP_11(a,b,V...) do{ _SEP_2(a,b); _SEP_9(V); }while(0)
#define _SEP_12(a,b,V...) do{ _SEP_2(a,b); _SEP_10(V); }while(0)
#define _SEP_13(a,b,V...) do{ _SEP_2(a,b); _SEP_11(V); }while(0)
#define _SEP_14(a,b,V...) do{ _SEP_2(a,b); _SEP_12(V); }while(0)
#define _SEP_15(a,b,V...) do{ _SEP_2(a,b); _SEP_13(V); }while(0)
#define _SEP_16(a,b,V...) do{ _SEP_2(a,b); _SEP_14(V); }while(0)
#define _SEP_17(a,b,V...) do{ _SEP_2(a,b); _SEP_15(V); }while(0)
#define _SEP_18(a,b,V...) do{ _SEP_2(a,b); _SEP_16(V); }while(0)
#define _SEP_19(a,b,V...) do{ _SEP_2(a,b); _SEP_17(V); }while(0)
#define _SEP_20(a,b,V...) do{ _SEP_2(a,b); _SEP_18(V); }while(0)
#define _SEP_21(a,b,V...) do{ _SEP_2(a,b); _SEP_19(V); }while(0)
#define _SEP_22(a,b,V...) do{ _SEP_2(a,b); _SEP_20(V); }while(0)
#define _SEP_23(a,b,V...) do{ _SEP_2(a,b); _SEP_21(V); }while(0)
#define _SEP_24(a,b,V...) do{ _SEP_2(a,b); _SEP_22(V); }while(0)

#define SERIAL_ECHOPAIR(V...) _SEP_N(NUM_ARGS(V),V)

// Print up to 12 pairs of values. Odd elements must be PSTR pointers.
#define __SEP_N_P(N,V...)   _SEP_##N##_P(V)
#define _SEP_N_P(N,V...)    __SEP_N_P(N,V)
#define _SEP_1_P(PRE)       serialprintPGM(PRE)
#define _SEP_2_P(PRE,V)     serial_echopair_PGM(PRE,V)
#define _SEP_3_P(a,b,c)     do{ _SEP_2_P(a,b); serialprintPGM(c); }while(0)
#define _SEP_4_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_2_P(V); }while(0)
#define _SEP_5_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_3_P(V); }while(0)
#define _SEP_6_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_4_P(V); }while(0)
#define _SEP_7_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_5_P(V); }while(0)
#define _SEP_8_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_6_P(V); }while(0)
#define _SEP_9_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_7_P(V); }while(0)
#define _SEP_10_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_8_P(V); }while(0)
#define _SEP_11_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_9_P(V); }while(0)
#define _SEP_12_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_10_P(V); }while(0)
#define _SEP_13_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_11_P(V); }while(0)
#define _SEP_14_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_12_P(V); }while(0)
#define _SEP_15_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_13_P(V); }while(0)
#define _SEP_16_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_14_P(V); }while(0)
#define _SEP_17_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_15_P(V); }while(0)
#define _SEP_18_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_16_P(V); }while(0)
#define _SEP_19_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_17_P(V); }while(0)
#define _SEP_20_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_18_P(V); }while(0)
#define _SEP_21_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_19_P(V); }while(0)
#define _SEP_22_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_20_P(V); }while(0)
#define _SEP_23_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_21_P(V); }while(0)
#define _SEP_24_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_22_P(V); }while(0)

#define SERIAL_ECHOPAIR_P(V...) _SEP_N_P(NUM_ARGS(V),V)

// Print up to 12 pairs of values followed by newline
#define __SELP_N(N,V...)   _SELP_##N(V)
#define _SELP_N(N,V...)    __SELP_N(N,V)
#define _SELP_1(PRE)       SERIAL_ECHOLNPGM(PRE)
#define _SELP_2(PRE,V)     do{ serial_echopair_PGM(PSTR(PRE),V); SERIAL_EOL(); }while(0)
#define _SELP_3(a,b,c)     do{ _SEP_2(a,b); SERIAL_ECHOLNPGM(c); }while(0)
#define _SELP_4(a,b,V...)  do{ _SEP_2(a,b); _SELP_2(V); }while(0)
#define _SELP_5(a,b,V...)  do{ _SEP_2(a,b); _SELP_3(V); }while(0)
#define _SELP_6(a,b,V...)  do{ _SEP_2(a,b); _SELP_4(V); }while(0)
#define _SELP_7(a,b,V...)  do{ _SEP_2(a,b); _SELP_5(V); }while(0)
#define _SELP_8(a,b,V...)  do{ _SEP_2(a,b); _SELP_6(V); }while(0)
#define _SELP_9(a,b,V...)  do{ _SEP_2(a,b); _SELP_7(V); }while(0)
#define _SELP_10(a,b,V...) do{ _SEP_2(a,b); _SELP_8(V); }while(0)
#define _SELP_11(a,b,V...) do{ _SEP_2(a,b); _SELP_9(V); }while(0)
#define _SELP_12(a,b,V...) do{ _SEP_2(a,b); _SELP_10(V); }while(0)
#define _SELP_13(a,b,V...) do{ _SEP_2(a,b); _SELP_11(V); }while(0)
#define _SELP_14(a,b,V...) do{ _SEP_2(a,b); _SELP_12(V); }while(0)
#define _SELP_15(a,b,V...) do{ _SEP_2(a,b); _SELP_13(V); }while(0)
#define _SELP_16(a,b,V...) do{ _SEP_2(a,b); _SELP_14(V); }while(0)
#define _SELP_17(a,b,V...) do{ _SEP_2(a,b); _SELP_15(V); }while(0)
#define _SELP_18(a,b,V...) do{ _SEP_2(a,b); _SELP_16(V); }while(0)
#define _SELP_19(a,b,V...) do{ _SEP_2(a,b); _SELP_17(V); }while(0)
#define _SELP_20(a,b,V...) do{ _SEP_2(a,b); _SELP_18(V); }while(0)
#define _SELP_21(a,b,V...) do{ _SEP_2(a,b); _SELP_19(V); }while(0)
#define _SELP_22(a,b,V...) do{ _SEP_2(a,b); _SELP_20(V); }while(0)
#define _SELP_23(a,b,V...) do{ _SEP_2(a,b); _SELP_21(V); }while(0)
#define _SELP_24(a,b,V...) do{ _SEP_2(a,b); _SELP_22(V); }while(0)
#define _SELP_25(a,b,V...) do{ _SEP_2(a,b); _SELP_23(V); }while(0)
#define _SELP_26(a,b,V...) do{ _SEP_2(a,b); _SELP_24(V); }while(0)
#define _SELP_27(a,b,V...) do{ _SEP_2(a,b); _SELP_25(V); }while(0)
#define _SELP_28(a,b,V...) do{ _SEP_2(a,b); _SELP_26(V); }while(0)
#define _SELP_29(a,b,V...) do{ _SEP_2(a,b); _SELP_27(V); }while(0)
#define _SELP_30(a,b,V...) do{ _SEP_2(a,b); _SELP_28(V); }while(0) // Eat two args, pass the rest up

#define SERIAL_ECHOLNPAIR(V...) _SELP_N(NUM_ARGS(V),V)

// Print up to 12 pairs of values followed by newline
#define __SELP_N_P(N,V...)   _SELP_##N##_P(V)
#define _SELP_N_P(N,V...)    __SELP_N_P(N,V)
#define _SELP_1_P(PRE)       serialprintPGM(PRE)
#define _SELP_2_P(PRE,V)     do{ serial_echopair_PGM(PRE,V); SERIAL_EOL(); }while(0)
#define _SELP_3_P(a,b,c)     do{ _SEP_2_P(a,b); serialprintPGM(c); }while(0)
#define _SELP_4_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_2_P(V); }while(0)
#define _SELP_5_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_3_P(V); }while(0)
#define _SELP_6_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_4_P(V); }while(0)
#define _SELP_7_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_5_P(V); }while(0)
#define _SELP_8_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_6_P(V); }while(0)
#define _SELP_9_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_7_P(V); }while(0)
#define _SELP_10_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_8_P(V); }while(0)
#define _SELP_11_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_9_P(V); }while(0)
#define _SELP_12_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_10_P(V); }while(0)
#define _SELP_13_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_11_P(V); }while(0)
#define _SELP_14_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_12_P(V); }while(0)
#define _SELP_15_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_13_P(V); }while(0)
#define _SELP_16_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_14_P(V); }while(0)
#define _SELP_17_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_15_P(V); }while(0)
#define _SELP_18_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_16_P(V); }while(0)
#define _SELP_19_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_17_P(V); }while(0)
#define _SELP_20_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_18_P(V); }while(0)
#define _SELP_21_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_19_P(V); }while(0)
#define _SELP_22_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_20_P(V); }while(0)
#define _SELP_23_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_21_P(V); }while(0)
#define _SELP_24_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_22_P(V); }while(0)
#define _SELP_25_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_23_P(V); }while(0)
#define _SELP_26_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_24_P(V); }while(0)
#define _SELP_27_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_25_P(V); }while(0)
#define _SELP_28_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_26_P(V); }while(0)
#define _SELP_29_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_27_P(V); }while(0)
#define _SELP_30_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_28_P(V); }while(0) // Eat two args, pass the rest up

#define SERIAL_ECHOLNPAIR_P(V...) _SELP_N_P(NUM_ARGS(V),V)

// Print up to 20 comma-separated pairs of values
#define __SLST_N(N,V...)   _SLST_##N(V)
#define _SLST_N(N,V...)    __SLST_N(N,V)
#define _SLST_1(a)         SERIAL_ECHO(a)
#define _SLST_2(a,b)       do{ SERIAL_ECHO(a); SERIAL_ECHOPAIR(", ",b);     }while(0)
#define _SLST_3(a,b,c)     do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_1(c);  }while(0)
#define _SLST_4(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_2(V);  }while(0)
#define _SLST_5(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_3(V);  }while(0)
#define _SLST_6(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_4(V);  }while(0)
#define _SLST_7(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_5(V);  }while(0)
#define _SLST_8(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_6(V);  }while(0)
#define _SLST_9(a,b,V...)  do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_7(V);  }while(0)
#define _SLST_10(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_8(V);  }while(0)
#define _SLST_11(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_9(V);  }while(0)
#define _SLST_12(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_10(V); }while(0)
#define _SLST_13(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_11(V); }while(0)
#define _SLST_14(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_12(V); }while(0)
#define _SLST_15(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_13(V); }while(0)
#define _SLST_16(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_14(V); }while(0)
#define _SLST_17(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_15(V); }while(0)
#define _SLST_18(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_16(V); }while(0)
#define _SLST_19(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_17(V); }while(0)
#define _SLST_20(a,b,V...) do{ SERIAL_ECHO(a); _SEP_2(", ",b); _SLST_18(V); }while(0) // Eat two args, pass the rest up

#define SERIAL_ECHOLIST(pre,V...)   do{ SERIAL_ECHOPGM(pre); _SLST_N(NUM_ARGS(V),V); }while(0)
#define SERIAL_ECHOLIST_N(N,V...)   _SLST_N(N,LIST_N(N,V))

#define SERIAL_ECHOPGM_P(P)         (serialprintPGM(P))
#define SERIAL_ECHOLNPGM_P(P)       (serialprintPGM(P "\n"))

#define SERIAL_ECHOPGM(S)           (serialprintPGM(PSTR(S)))
#define SERIAL_ECHOLNPGM(S)         (serialprintPGM(PSTR(S "\n")))

#define SERIAL_ECHOPAIR_F_P(P,V...) do{ serialprintPGM(P); SERIAL_ECHO_F(V); }while(0)
#define SERIAL_ECHOLNPAIR_F_P(V...) do{ SERIAL_ECHOPAIR_F_P(V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHOPAIR_F(S,V...)   SERIAL_ECHOPAIR_F_P(PSTR(S),V)
#define SERIAL_ECHOLNPAIR_F(V...)   do{ SERIAL_ECHOPAIR_F(V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()         serial_echo_start()
#define SERIAL_ERROR_START()        serial_error_start()
#define SERIAL_EOL()                SERIAL_CHAR('\n')

#define SERIAL_ECHO_MSG(V...)       do{ SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR(V); }while(0)
#define SERIAL_ERROR_MSG(V...)      do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPAIR(V); }while(0)

#define SERIAL_ECHO_SP(C)           serial_spaces(C)

#define SERIAL_ECHO_TERNARY(TF, PRE, ON, OFF, POST) serial_ternary(TF, PSTR(PRE), PSTR(ON), PSTR(OFF), PSTR(POST))

#if SERIAL_FLOAT_PRECISION
  #define SERIAL_DECIMAL(V) SERIAL_PRINT(V, SERIAL_FLOAT_PRECISION)
#else
  #define SERIAL_DECIMAL(V) SERIAL_ECHO(V)
#endif

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
void serial_echopair_PGM(PGM_P const s_P, const char *v);
void serial_echopair_PGM(PGM_P const s_P, char v);
void serial_echopair_PGM(PGM_P const s_P, int v);
void serial_echopair_PGM(PGM_P const s_P, unsigned int v);
void serial_echopair_PGM(PGM_P const s_P, long v);
void serial_echopair_PGM(PGM_P const s_P, unsigned long v);
void serial_echopair_PGM(PGM_P const s_P, float v);
void serial_echopair_PGM(PGM_P const s_P, double v);
inline void serial_echopair_PGM(PGM_P const s_P, uint8_t v) { serial_echopair_PGM(s_P, (int)v); }
inline void serial_echopair_PGM(PGM_P const s_P, bool v)    { serial_echopair_PGM(s_P, (int)v); }
inline void serial_echopair_PGM(PGM_P const s_P, void *v)   { serial_echopair_PGM(s_P, (uintptr_t)v); }

void serialprintPGM(PGM_P str);
void serial_echo_start();
void serial_error_start();
void serial_ternary(const bool onoff, PGM_P const pre, PGM_P const on, PGM_P const off, PGM_P const post=nullptr);
void serialprint_onoff(const bool onoff);
void serialprintln_onoff(const bool onoff);
void serialprint_truefalse(const bool tf);
void serial_spaces(uint8_t count);

void print_bin(const uint16_t val);
void print_xyz(const float &x, const float &y, const float &z, PGM_P const prefix=nullptr, PGM_P const suffix=nullptr);

inline void print_xyz(const xyz_pos_t &xyz, PGM_P const prefix=nullptr, PGM_P const suffix=nullptr) {
  print_xyz(xyz.x, xyz.y, xyz.z, prefix, suffix);
}

#define SERIAL_POS(SUFFIX,VAR) do { print_xyz(VAR, PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n")); }while(0)
#define SERIAL_XYZ(PREFIX,V...) do { print_xyz(V, PSTR(PREFIX), nullptr); }while(0)
