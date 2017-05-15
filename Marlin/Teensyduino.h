/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
//  some of the pin mapping functions of the Teensduino extension to the Arduino IDE
//  do not function the same as the other Arduino extensions
//

#ifndef _TEENSY_
  #define _TEENSY_

  #define TEENSYDUINO_IDE

  #undef NUM_DIGITAL_PINS
  #define  NUM_DIGITAL_PINS 48  //Teensy says 46 but FASTIO is 48

  //digitalPinToTimer(pin) function works like Arduino but Timers are not defined
  #define TIMER0B 1
  #define TIMER1A 7
  #define TIMER1B 8
  #define TIMER1C 9
  #define TIMER2A 6
  #define TIMER2B 2
  #define TIMER3A 5
  #define TIMER3B 4
  #define TIMER3C 3

  #if ENABLED(PINS_DEBUGGING)
    // "digitalPinToPort" function just returns the pin number so need to create our own.
    // Can't use the name "digitalPinToPort" for our own because it interferes with the
    // FAST_PWM_FAN function if we do

    #define PA 1
    #define PB 2
    #define PC 3
    #define PD 4
    #define PE 5
    #define PF 6


    const uint8_t PROGMEM digital_pin_to_port_PGM_Teensy[] = {
      PD, // 0  - PD0 - INT0 - PWM
      PD, // 1  - PD1 - INT1 - PWM
      PD, // 2  - PD2 - INT2 - RX
      PD, // 3  - PD3 - INT3 - TX
      PD, // 4  - PD4
      PD, // 5  - PD5
      PD, // 6  - PD6
      PD, // 7  - PD7
      PE, // 8  - PE0
      PE, // 9  - PE1
      PC, // 10 - PC0
      PC, // 11 - PC1
      PC, // 12 - PC2
      PC, // 13 - PC3
      PC, // 14 - PC4 - PWM
      PC, // 15 - PC5 - PWM
      PC, // 16 - PC6 - PWM
      PC, // 17 - PC7
      PE, // 18 - PE6 - INT6
      PE, // 19 - PE7 - INT7
      PB, // 20 - PB0
      PB, // 21 - PB1
      PB, // 22 - PB2
      PB, // 23 - PB3
      PB, // 24 - PB4 - PWM
      PB, // 25 - PB5 - PWM
      PB, // 26 - PB6 - PWM
      PB, // 27 - PB7 - PWM
      PA, // 28 - PA0
      PA, // 29 - PA1
      PA, // 30 - PA2
      PA, // 31 - PA3
      PA, // 32 - PA4
      PA, // 33 - PA5
      PA, // 34 - PA6
      PA, // 35 - PA7
      PE, // 36 - PE4 - INT4
      PE, // 37 - PE5 - INT5
      PF, // 38 - PF0 - A0
      PF, // 39 - PF1 - A1
      PF, // 40 - PF2 - A2
      PF, // 41 - PF3 - A3
      PF, // 42 - PF4 - A4
      PF, // 43 - PF5 - A5
      PF, // 44 - PF6 - A6
      PF, // 45 - PF7 - A7
      PE, // 46 - PE2 (not defined in teensyduino)
      PE, // 47 - PE3 (not defined in teensyduino)
    };

    #define digitalPinToPort_Teensy(P) ( pgm_read_byte( digital_pin_to_port_PGM_Teensy + (P) ) )

    // digitalPinToBitMask(pin) is OK

    #define digitalRead_mod(p)  digitalRead(p)   // Teensyduino's version of digitalRead doesn't
                                                 // disable the PWMs so we can use it as is

    // portModeRegister(pin) is OK

    /**
     * The Teensyduino type IDEs use two different pin maps.  FAST_IO standard is used for all
     * READ() & WRITE() functions while the Teensy (Arduino) pin map is used for all the
     * digitalRead() & digitalWrite() functions.  Because 80% of the pin numbers in the
     * pins_YOUR_BOARD.h files are FASTIO it is less confusing to use the FAST_IO numbers as the
     * index to find & display pin names.  The M43 routines must use the Teensy pin numbers
     * to fetch the data for each pin so a translation index is needed.
     */
    const uint8_t PROGMEM FAST_IO_Teensy_PGM[] = {
    // Teensy (Arduino)      FASTIO
      28,                   //  0
      29,                   //  1
      30,                   //  2
      31,                   //  3
      32,                   //  4
      33,                   //  5
      34,                   //  6
      35,                   //  7
      20,                   //  8
      21,                   //  9
      22,                   //  10
      23,                   //  11
      24,                   //  12
      25,                   //  13
      26,                   //  14
      27,                   //  15
      10,                   //  16
      11,                   //  17
      12,                   //  18
      13,                   //  19
      14,                   //  20
      15,                   //  21
      16,                   //  22
      17,                   //  23
      0,                    //  24
      1,                    //  25
      2,                    //  26
      3,                    //  27
      4,                    //  28
      5,                    //  29
      6,                    //  30
      7,                    //  31
      8,                    //  32
      9,                    //  33
      46,                   //  34    not in usual Teensy pin map
      47,                   //  35    not in usual Teensy pin map
      36,                   //  36
      37,                   //  37
      18,                   //  38
      19,                   //  39
      38,                   //  40
      39,                   //  41
      40,                   //  42
      41,                   //  43
      42,                   //  44
      43,                   //  45
      44,                   //  46
      45                    //  47
    };

    #define FAST_to_Teensy(pin)  ((uint8_t) pgm_read_byte(FAST_IO_Teensy_PGM + (pin)))

    const uint8_t PROGMEM Teensy_to_FAST_PGM[] = {
    // FASTIO          Teensy (Arduino)
      24,                   //  0
      25,                   //  1
      26,                   //  2
      27,                   //  3
      28,                   //  4
      29,                   //  5
      30,                   //  6
      31,                   //  7
      32,                   //  8
      33,                   //  9
      16,                   //  10
      17,                   //  11
      18,                   //  12
      19,                   //  13
      20,                   //  14
      21,                   //  15
      22,                   //  16
      23,                   //  17
      38,                   //  18
      39,                   //  19
       8,                   //  20
       9,                   //  21
      10,                   //  22
      11,                   //  23
      12,                   //  24
      13,                   //  25
      14,                   //  26
      15,                   //  27
       0,                   //  28
       1,                   //  29
       2,                   //  30
       3,                   //  31
       4,                   //  32
       5,                   //  33
       6,                   //  34
       7,                   //  35
      36,                   //  36
      37,                   //  37
      40,                   //  38
      41,                   //  39
      42,                   //  40
      43,                   //  41
      44,                   //  42
      45,                   //  43
      46,                   //  44
      47,                   //  45
      34,                   //  46     not in usual Teensy pin map
      35                    //  47     not in usual Teensy pin map
  };

    #define Teensy_to_FAST(pin)  ((uint8_t) pgm_read_byte(Teensy_to_FAST_PGM + (pin)))

    // macros to translate Teensy (Arduino) pin numbers to FAST_IO pin numbers
          // Teensy (Arduino)  FASTIO
    #define TEENSY_TO_FAST_0   24
    #define TEENSY_TO_FAST_1   25
    #define TEENSY_TO_FAST_2   26
    #define TEENSY_TO_FAST_3   27
    #define TEENSY_TO_FAST_4   28
    #define TEENSY_TO_FAST_5   29
    #define TEENSY_TO_FAST_6   30
    #define TEENSY_TO_FAST_7   31
    #define TEENSY_TO_FAST_8   32
    #define TEENSY_TO_FAST_9   33
    #define TEENSY_TO_FAST_00  24
    #define TEENSY_TO_FAST_01  25
    #define TEENSY_TO_FAST_02  26
    #define TEENSY_TO_FAST_03  27
    #define TEENSY_TO_FAST_04  28
    #define TEENSY_TO_FAST_05  29
    #define TEENSY_TO_FAST_06  30
    #define TEENSY_TO_FAST_07  31
    #define TEENSY_TO_FAST_08  32
    #define TEENSY_TO_FAST_09  33
    #define TEENSY_TO_FAST_10  16
    #define TEENSY_TO_FAST_11  17
    #define TEENSY_TO_FAST_12  18
    #define TEENSY_TO_FAST_13  19
    #define TEENSY_TO_FAST_14  20
    #define TEENSY_TO_FAST_15  21
    #define TEENSY_TO_FAST_16  22
    #define TEENSY_TO_FAST_17  23
    #define TEENSY_TO_FAST_18  38
    #define TEENSY_TO_FAST_19  39
    #define TEENSY_TO_FAST_20  8
    #define TEENSY_TO_FAST_21  9
    #define TEENSY_TO_FAST_22  10
    #define TEENSY_TO_FAST_23  11
    #define TEENSY_TO_FAST_24  12
    #define TEENSY_TO_FAST_25  13
    #define TEENSY_TO_FAST_26  14
    #define TEENSY_TO_FAST_27  15
    #define TEENSY_TO_FAST_28  0
    #define TEENSY_TO_FAST_29  1
    #define TEENSY_TO_FAST_30  2
    #define TEENSY_TO_FAST_31  3
    #define TEENSY_TO_FAST_32  4
    #define TEENSY_TO_FAST_33  5
    #define TEENSY_TO_FAST_34  6
    #define TEENSY_TO_FAST_35  7
    #define TEENSY_TO_FAST_36  36
    #define TEENSY_TO_FAST_37  37
    #define TEENSY_TO_FAST_38  40
    #define TEENSY_TO_FAST_39  41
    #define TEENSY_TO_FAST_40  42
    #define TEENSY_TO_FAST_41  43
    #define TEENSY_TO_FAST_42  44
    #define TEENSY_TO_FAST_43  45
    #define TEENSY_TO_FAST_44  46
    #define TEENSY_TO_FAST_45  47
    #define TEENSY_TO_FAST_46  34
    #define TEENSY_TO_FAST_47  35

    #define _TEENSY_TO_FAST(a, ...) a ## __VA_ARGS__
    #define TEENSY_TO_FAST(c) _TEENSY_TO_FAST(TEENSY_TO_FAST_, c)

  #endif
#endif
