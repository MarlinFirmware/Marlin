/*
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
  Copyright (c) 2009 Michael Margolis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 * Defines for 16 bit timers used with  Servo library
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */

/**
 * AVR Only definitions
 * --------------------
 */

// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
//#define _useTimer1
#define _useTimer3
#define _useTimer4
//typedef enum { _timer5, _timer1, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t ;
typedef enum { _timer5, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega32U4__)
//#define _useTimer1
#define _useTimer3
//typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;
typedef enum { _timer3, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
//#define _useTimer1
//typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;
typedef enum { _timer3, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__) ||defined(__AVR_ATmega2561__)
#define _useTimer3
//#define _useTimer1
//typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;
typedef enum { _timer3, _Nbr_16timers } timer16_Sequence_t ;

#else  // everything else
//#define _useTimer1
//typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;
typedef enum { _Nbr_16timers } timer16_Sequence_t ;
#endif

