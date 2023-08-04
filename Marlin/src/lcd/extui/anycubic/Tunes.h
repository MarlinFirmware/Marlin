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

/**
 * lcd/extui/anycubic/Tunes.h
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

// Note duration divisors
enum { l_T1=1, l_T2 =2, l_T3=3, l_T4 =4, l_T8=8, l_T16=16 };

// Note Frequency
enum {
n_P =   0,  // silence or pause
n_C0=  16, n_CS0=  17, n_D0=  18, n_DS0=  19, n_E0=  21, n_F0=  22, n_FS0=  23, n_G0=  25, n_GS0=  26, n_A0=  28, n_AS0=  29, n_B0=  31,
n_C1=  33, n_CS1=  35, n_D1=  37, n_DS1=  39, n_E1=  41, n_F1=  44, n_FS1=  46, n_G1=  49, n_GS1=  52, n_A1=  55, n_AS1=  58, n_B1=  62,
n_C2=  65, n_CS2=  69, n_D2=  73, n_DS2=  78, n_E2=  82, n_F2=  87, n_FS2=  93, n_G2=  98, n_GS2= 104, n_A2= 110, n_AS2= 117, n_B2= 123,
n_C3= 131, n_CS3= 139, n_D3= 147, n_DS3= 156, n_E3= 165, n_F3= 175, n_FS3= 185, n_G3= 196, n_GS3= 208, n_A3= 220, n_AS3= 233, n_B3= 247,
n_C4= 262, n_CS4= 277, n_D4= 294, n_DS4= 311, n_E4= 330, n_F4= 349, n_FS4= 370, n_G4= 392, n_GS4= 415, n_A4= 440, n_AS4= 466, n_B4= 494,
n_C5= 523, n_CS5= 554, n_D5= 587, n_DS5= 622, n_E5= 659, n_F5= 698, n_FS5= 740, n_G5= 784, n_GS5= 831, n_A5= 880, n_AS5= 932, n_B5= 988,
n_C6=1047, n_CS6=1109, n_D6=1175, n_DS6=1245, n_E6=1319, n_F6=1397, n_FS6=1480, n_G6=1568, n_GS6=1661, n_A6=1760, n_AS6=1865, n_B6=1976,
n_C7=2093, n_CS7=2217, n_D7=2349, n_DS7=2489, n_E7=2637, n_F7=2794, n_FS7=2960, n_G7=3136, n_GS7=3322, n_A7=3520, n_AS7=3729, n_B7=3951,
n_C8=4186, n_CS8=4435, n_D8=4699, n_DS8=4978, n_E8=5274, n_F8=5587, n_FS8=5920, n_G8=6272, n_GS8=6645, n_A8=7040, n_AS8=7459, n_B8=7902,
n_END=10000 // end of tune marker
};

namespace Anycubic {

  void playTune(const uint16_t *tune, const uint8_t speed=1);

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
    n_E7,l_T8, n_P,l_T8, n_D7,l_T4, n_P,l_T4,
    n_END
  };

  const uint16_t GB_PowerOn[] = {
    500,
    n_C6,l_T4, n_P,l_T16, n_C7,l_T2, n_P,l_T8,
    n_END
  };

  const uint16_t HeaterTimeout[] = {
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
