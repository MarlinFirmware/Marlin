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

/**
 * lcd/extui/lib/Tunes.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

/**************************************************************************
 * Notes definition from https://pages.mtu.edu/~suits/NoteFreqCalcs.html  *
 *                                                                        *
 * The format of a tune is:                                               *
 * {<whole note time>,<note1>,<length1>, <note2>,<length2>, ... <END>}    *
 *                                                                        *
 * 1) The first value is the length of a whole note in milliseconds       *
 * 2) Then a sequence of pitch and duration pairs                         *
 * 3) Finally the END marker so your tunes can be any length up to        *
 *    MAX_TUNE_LEN                                                        *
 *************************************************************************/

#include <stdint.h>

#define MAX_TUNE_LENGTH 128

// Special notes!
#define n_P    0       // silence or pause
#define n_END  10000   // end of tune marker

// Note duration divisors
#define l_T1   1
#define l_T2   2
#define l_T3   3
#define l_T4   4
#define l_T8   8
#define l_T16 16

// Note Frequency
#define n_C0     16
#define n_CS0    17
#define n_D0     18
#define n_DS0    19
#define n_E0     21
#define n_F0     22
#define n_FS0    23
#define n_G0     25
#define n_GS0    26
#define n_A0     28
#define n_AS0    29
#define n_B0     31
#define n_C1     33
#define n_CS1    35
#define n_D1     37
#define n_DS1    39
#define n_E1     41
#define n_F1     44
#define n_FS1    46
#define n_G1     49
#define n_GS1    52
#define n_A1     55
#define n_AS1    58
#define n_B1     62
#define n_C2     65
#define n_CS2    69
#define n_D2     73
#define n_DS2    78
#define n_E2     82
#define n_F2     87
#define n_FS2    93
#define n_G2     98
#define n_GS2   104
#define n_A2    110
#define n_AS2   117
#define n_B2    123
#define n_C3    131
#define n_CS3   139
#define n_D3    147
#define n_DS3   156
#define n_E3    165
#define n_F3    175
#define n_FS3   185
#define n_G3    196
#define n_GS3   208
#define n_A3    220
#define n_AS3   233
#define n_B3    247
#define n_C4    262
#define n_CS4   277
#define n_D4    294
#define n_DS4   311
#define n_E4    330
#define n_F4    349
#define n_FS4   370
#define n_G4    392
#define n_GS4   415
#define n_A4    440
#define n_AS4   466
#define n_B4    494
#define n_C5    523
#define n_CS5   554
#define n_D5    587
#define n_DS5   622
#define n_E5    659
#define n_F5    698
#define n_FS5   740
#define n_G5    784
#define n_GS5   831
#define n_A5    880
#define n_AS5   932
#define n_B5    988
#define n_C6   1047
#define n_CS6  1109
#define n_D6   1175
#define n_DS6  1245
#define n_E6   1319
#define n_F6   1397
#define n_FS6  1480
#define n_G6   1568
#define n_GS6  1661
#define n_A6   1760
#define n_AS6  1865
#define n_B6   1976
#define n_C7   2093
#define n_CS7  2217
#define n_D7   2349
#define n_DS7  2489
#define n_E7   2637
#define n_F7   2794
#define n_FS7  2960
#define n_G7   3136
#define n_GS7  3322
#define n_A7   3520
#define n_AS7  3729
#define n_B7   3951
#define n_C8   4186
#define n_CS8  4435
#define n_D8   4699
#define n_DS8  4978
#define n_E8   5274
#define n_F8   5587
#define n_FS8  5920
#define n_G8   6272
#define n_GS8  6645
#define n_A8   7040
#define n_AS8  7459
#define n_B8   7902

namespace Anycubic {

  void PlayTune(uint8_t beeperPin, const uint16_t *tune, uint8_t speed);

  // Only uncomment the tunes you are using to save memory
  // This will help you write tunes!
  // https://www.apronus.com/music/flashpiano.htm

  const uint16_t SOS[] = {
    250,
    n_G6,l_T3, n_P,l_T3, n_G6,l_T3, n_P,l_T3, n_G6,l_T3, n_P,l_T1,
    n_G6,l_T1, n_P,l_T3, n_G6,l_T1, n_P,l_T3, n_G6,l_T1, n_P,l_T1,
    n_G6,l_T3, n_P,l_T3, n_G6,l_T3, n_P,l_T3, n_G6,l_T3, n_P,l_T1,
    n_END
  };

  const uint16_t BeepBeep[] = {
    500,
    n_C7,l_T8, n_P,l_T16, n_C7,l_T8, n_P,l_T8,
    n_END
  };

  const uint16_t BeepBeepBeeep[] = {
    1000,
    n_G7,l_T4, n_P,l_T16, n_G7,l_T4, n_P,l_T8, n_G7,l_T2,
    n_END
  };

  const uint16_t Anycubic_PowerOn[] = {
    1000,
    n_F7,l_T8, n_P,l_T8, n_C7,l_T8, n_P,l_T8, n_D7,l_T8, n_P,l_T8,
    n_E7,l_T8, n_P,l_T8, n_D7,l_T4, n_P,l_T4, n_G7,l_T4, n_P,l_T4,
    n_A7,l_T2, n_P,l_T1,
    n_END
  };

  const uint16_t GB_PowerOn[] = {
    500,
    n_C6,l_T4, n_P,l_T16, n_C7,l_T2, n_P,l_T8,
    n_END
  };

  const uint16_t Heater_Timedout[] = {
    1000,
    n_C6,l_T1,
    n_END
  };

  const uint16_t FilamentOut[] = {
    1000,
    n_AS7,l_T4, n_P,l_T16, n_FS7,l_T2,
    n_END
  };

}
