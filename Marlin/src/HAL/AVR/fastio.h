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
 * Fast I/O Routines for AVR
 * Use direct port manipulation to save scads of processor time.
 * Contributed by Triffid_Hunter and modified by Kliment, thinkyhead, Bob-the-Kuhn, et.al.
 */

#include <avr/io.h>

#if defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1286P__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB646P__) || defined(__AVR_AT90USB647__)
  #define AVR_AT90USB1286_FAMILY 1
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
  #define AVR_ATmega1284_FAMILY 1
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define AVR_ATmega2560_FAMILY 1
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define AVR_ATmega2561_FAMILY 1
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #define AVR_ATmega328_FAMILY 1
#endif

/**
 * Include Ports and Functions
 */
#if AVR_ATmega328_FAMILY
  #include "fastio/fastio_168.h"
#elif AVR_ATmega1284_FAMILY
  #include "fastio/fastio_644.h"
#elif AVR_ATmega2560_FAMILY
  #include "fastio/fastio_1280.h"
#elif AVR_AT90USB1286_FAMILY
  #include "fastio/fastio_AT90USB.h"
#elif AVR_ATmega2561_FAMILY
  #include "fastio/fastio_1281.h"
#else
  #error "No FastIO definition for the selected AVR Board."
#endif

/**
 * Magic I/O routines
 *
 * Now you can simply SET_OUTPUT(PIN); WRITE(PIN, HIGH); WRITE(PIN, LOW);
 *
 * Why double up on these macros? see https://gcc.gnu.org/onlinedocs/cpp/Stringification.html
 */

#define _READ(IO)             TEST(DIO ## IO ## _RPORT, DIO ## IO ## _PIN)

#define _WRITE_NC(IO,V) do{ \
  if (V) SBI(DIO ## IO ## _WPORT, DIO ## IO ## _PIN); \
  else   CBI(DIO ## IO ## _WPORT, DIO ## IO ## _PIN); \
}while(0)

#define _WRITE_C(IO,V) do{ \
  uint8_t port_bits = DIO ## IO ## _WPORT;                  /* Get a mask from the current port bits */ \
  if (V) port_bits = ~port_bits;                            /* For setting bits, invert the mask */ \
  DIO ## IO ## _RPORT = port_bits & _BV(DIO ## IO ## _PIN); /* Atomically toggle the output port bits */ \
}while(0)

#define _WRITE(IO,V)          do{ if (&(DIO ## IO ## _RPORT) < (uint8_t*)0x100) _WRITE_NC(IO,V); else _WRITE_C(IO,V); }while(0)

#define _TOGGLE(IO)           (DIO ## IO ## _RPORT = _BV(DIO ## IO ## _PIN))

#define _SET_INPUT(IO)        CBI(DIO ## IO ## _DDR, DIO ## IO ## _PIN)
#define _SET_OUTPUT(IO)       SBI(DIO ## IO ## _DDR, DIO ## IO ## _PIN)

#define _IS_INPUT(IO)         !TEST(DIO ## IO ## _DDR, DIO ## IO ## _PIN)
#define _IS_OUTPUT(IO)        TEST(DIO ## IO ## _DDR, DIO ## IO ## _PIN)

// digitalRead/Write wrappers
#ifdef FASTIO_EXT_START
  void extDigitalWrite(const int8_t pin, const uint8_t state);
  uint8_t extDigitalRead(const int8_t pin);
#else
  #define extDigitalWrite(IO,V) digitalWrite(IO,V)
  #define extDigitalRead(IO)    digitalRead(IO)
#endif

#define READ(IO)              _READ(IO)
#define WRITE(IO,V)           _WRITE(IO,V)
#define TOGGLE(IO)            _TOGGLE(IO)

#define SET_INPUT(IO)         _SET_INPUT(IO)
#define SET_INPUT_PULLUP(IO)  do{ _SET_INPUT(IO); _WRITE(IO, HIGH); }while(0)
#define SET_INPUT_PULLDOWN    SET_INPUT
#define SET_OUTPUT(IO)        _SET_OUTPUT(IO)
#define SET_PWM               SET_OUTPUT

#define IS_INPUT(IO)          _IS_INPUT(IO)
#define IS_OUTPUT(IO)         _IS_OUTPUT(IO)

#define OUT_WRITE(IO,V)       do{ SET_OUTPUT(IO); WRITE(IO,V); }while(0)

/**
 * Timer and Interrupt Control
 */

// Waveform Generation Modes
enum WaveGenMode : uint8_t {
  WGM_NORMAL,          //  0
  WGM_PWM_PC_8,        //  1
  WGM_PWM_PC_9,        //  2
  WGM_PWM_PC_10,       //  3
  WGM_CTC_OCRnA,       //  4  COM OCnx
  WGM_FAST_PWM_8,      //  5
  WGM_FAST_PWM_9,      //  6
  WGM_FAST_PWM_10,     //  7
  WGM_PWM_PC_FC_ICRn,  //  8
  WGM_PWM_PC_FC_OCRnA, //  9  COM OCnA
  WGM_PWM_PC_ICRn,     // 10
  WGM_PWM_PC_OCRnA,    // 11  COM OCnA
  WGM_CTC_ICRn,        // 12  COM OCnx
  WGM_reserved,        // 13
  WGM_FAST_PWM_ICRn,   // 14  COM OCnA
  WGM_FAST_PWM_OCRnA   // 15  COM OCnA
};

// Wavefore Generation Modes (Timer 2 only)
enum WaveGenMode2 : uint8_t {
  WGM2_NORMAL,         // 0
  WGM2_PWM_PC,         // 1
  WGM2_CTC_OCR2A,      // 2
  WGM2_FAST_PWM,       // 3
  WGM2_reserved_1,     // 4
  WGM2_PWM_PC_OCR2A,   // 5
  WGM2_reserved_2,     // 6
  WGM2_FAST_PWM_OCR2A, // 7
};

// Compare Modes
enum CompareMode : uint8_t {
  COM_NORMAL,          //  0
  COM_TOGGLE,          //  1  Non-PWM: OCnx ... Both PWM (WGM 9,11,14,15): OCnA only ... else NORMAL
  COM_CLEAR_SET,       //  2  Non-PWM: OCnx ... Fast PWM: OCnx/Bottom ... PF-FC: OCnx Up/Down
  COM_SET_CLEAR        //  3  Non-PWM: OCnx ... Fast PWM: OCnx/Bottom ... PF-FC: OCnx Up/Down
};

// Clock Sources
enum ClockSource : uint8_t {
  CS_NONE,             //  0
  CS_PRESCALER_1,      //  1
  CS_PRESCALER_8,      //  2
  CS_PRESCALER_64,     //  3
  CS_PRESCALER_256,    //  4
  CS_PRESCALER_1024,   //  5
  CS_EXT_FALLING,      //  6
  CS_EXT_RISING        //  7
};

// Clock Sources (Timer 2 only)
enum ClockSource2 : uint8_t {
  CS2_NONE,            //  0
  CS2_PRESCALER_1,     //  1
  CS2_PRESCALER_8,     //  2
  CS2_PRESCALER_32,    //  3
  CS2_PRESCALER_64,    //  4
  CS2_PRESCALER_128,   //  5
  CS2_PRESCALER_256,   //  6
  CS2_PRESCALER_1024   //  7
};

// Get interrupt bits in an orderly way
// Ex: cs = GET_CS(0); coma1 = GET_COM(A,1);
#define GET_WGM(T)   (((TCCR##T##A >> WGM##T##0) & 0x3) | ((TCCR##T##B >> WGM##T##2 << 2) & 0xC))
#define GET_CS(T)    ((TCCR##T##B >> CS##T##0) & 0x7)
#define GET_COM(T,Q) ((TCCR##T##Q >> COM##T##Q##0) & 0x3)
#define GET_COMA(T)  GET_COM(T,A)
#define GET_COMB(T)  GET_COM(T,B)
#define GET_COMC(T)  GET_COM(T,C)
#define GET_ICNC(T)  (!!(TCCR##T##B & _BV(ICNC##T)))
#define GET_ICES(T)  (!!(TCCR##T##B & _BV(ICES##T)))
#define GET_FOC(T,Q) (!!(TCCR##T##C & _BV(FOC##T##Q)))
#define GET_FOCA(T)  GET_FOC(T,A)
#define GET_FOCB(T)  GET_FOC(T,B)
#define GET_FOCC(T)  GET_FOC(T,C)

// Set Wave Generation Mode bits
// Ex: SET_WGM(5,CTC_ICRn);
#define _SET_WGM(T,V) do{ \
    TCCR##T##A = (TCCR##T##A & ~(0x3 << WGM##T##0)) | (( int(V)       & 0x3) << WGM##T##0); \
    TCCR##T##B = (TCCR##T##B & ~(0x3 << WGM##T##2)) | (((int(V) >> 2) & 0x3) << WGM##T##2); \
  }while(0)
#define SET_WGM(T,V) _SET_WGM(T,WGM_##V)

// Set Clock Select bits
// Ex: SET_CS3(PRESCALER_64);
#ifdef TCCR2
  #define HAS_TCCR2 1
#endif
#define _SET_CS(T,V) (TCCR##T##B = (TCCR##T##B & ~(0x7 << CS##T##0)) | ((int(V) & 0x7) << CS##T##0))
#define _SET_CS0(V) _SET_CS(0,V)
#define _SET_CS1(V) _SET_CS(1,V)
#define _SET_CS3(V) _SET_CS(3,V)
#define _SET_CS4(V) _SET_CS(4,V)
#define _SET_CS5(V) _SET_CS(5,V)
#define SET_CS0(V) _SET_CS0(CS_##V)
#define SET_CS1(V) _SET_CS1(CS_##V)

#if HAS_TCCR2
  #define _SET_CS2(V) (TCCR2 = (TCCR2 & ~(0x7 << CS20)) | (int(V) << CS20))
  #define SET_CS2(V) _SET_CS2(CS2_##V)
#else
  #define _SET_CS2(V) _SET_CS(2,V)
  #define SET_CS2(V) _SET_CS2(CS_##V)
#endif

#define SET_CS3(V) _SET_CS3(CS_##V)
#define SET_CS4(V) _SET_CS4(CS_##V)
#define SET_CS5(V) _SET_CS5(CS_##V)
#define SET_CS(T,V) SET_CS##T(V)

// Set Compare Mode bits
// Ex: SET_COMS(4,CLEAR_SET,CLEAR_SET,CLEAR_SET);
#define _SET_COM(T,Q,V) (TCCR##T##Q = (TCCR##T##Q & ~(0x3 << COM##T##Q##0)) | (int(V) << COM##T##Q##0))
#define SET_COM(T,Q,V) _SET_COM(T,Q,COM_##V)
#define SET_COMA(T,V) SET_COM(T,A,V)
#define SET_COMB(T,V) SET_COM(T,B,V)
#define SET_COMC(T,V) SET_COM(T,C,V)
#define SET_COMS(T,V1,V2,V3) do{ SET_COMA(T,V1); SET_COMB(T,V2); SET_COMC(T,V3); }while(0)

// Set Noise Canceler bit
// Ex: SET_ICNC(2,1)
#define SET_ICNC(T,V) (TCCR##T##B = (V) ? TCCR##T##B | _BV(ICNC##T) : TCCR##T##B & ~_BV(ICNC##T))

// Set Input Capture Edge Select bit
// Ex: SET_ICES(5,0)
#define SET_ICES(T,V) (TCCR##T##B = (V) ? TCCR##T##B | _BV(ICES##T) : TCCR##T##B & ~_BV(ICES##T))

// Set Force Output Compare bit
// Ex: SET_FOC(3,A,1)
#define SET_FOC(T,Q,V) (TCCR##T##C = (V) ? TCCR##T##C | _BV(FOC##T##Q) : TCCR##T##C & ~_BV(FOC##T##Q))
#define SET_FOCA(T,V) SET_FOC(T,A,V)
#define SET_FOCB(T,V) SET_FOC(T,B,V)
#define SET_FOCC(T,V) SET_FOC(T,C,V)

#if 0

/**
 * PWM availability macros
 */

// Determine which hardware PWMs are already in use
#define _PWM_CHK_FAN_B(P) (P == E0_AUTO_FAN_PIN || P == E1_AUTO_FAN_PIN || P == E2_AUTO_FAN_PIN || P == E3_AUTO_FAN_PIN || P == E4_AUTO_FAN_PIN || P == E5_AUTO_FAN_PIN || P == E6_AUTO_FAN_PIN || P == E7_AUTO_FAN_PIN || P == CHAMBER_AUTO_FAN_PIN || P == COOLER_AUTO_FAN_PIN)
#if PIN_EXISTS(CONTROLLER_FAN)
  #define PWM_CHK_FAN_B(P) (_PWM_CHK_FAN_B(P) || P == CONTROLLER_FAN_PIN)
#else
  #define PWM_CHK_FAN_B(P) _PWM_CHK_FAN_B(P)
#endif

#if ANY_PIN(FAN, FAN1, FAN2, FAN3, FAN4, FAN5, FAN6, FAN7)
  #if PIN_EXISTS(FAN7)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN || P == FAN3_PIN || P == FAN4_PIN || P == FAN5_PIN || P == FAN6_PIN || P == FAN7_PIN)
  #elif PIN_EXISTS(FAN6)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN || P == FAN3_PIN || P == FAN4_PIN || P == FAN5_PIN || P == FAN6_PIN)
  #elif PIN_EXISTS(FAN5)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN || P == FAN3_PIN || P == FAN4_PIN || P == FAN5_PIN)
  #elif PIN_EXISTS(FAN4)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN || P == FAN3_PIN || P == FAN4_PIN)
  #elif PIN_EXISTS(FAN3)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN || P == FAN3_PIN)
  #elif PIN_EXISTS(FAN2)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN || P == FAN2_PIN)
  #elif PIN_EXISTS(FAN1)
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN || P == FAN1_PIN)
  #else
    #define PWM_CHK_FAN_A(P) (P == FAN0_PIN)
  #endif
#else
  #define PWM_CHK_FAN_A(P) false
#endif

#if HAS_MOTOR_CURRENT_PWM
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
    #define PWM_CHK_MOTOR_CURRENT(P) (P == MOTOR_CURRENT_PWM_E || P == MOTOR_CURRENT_PWM_Z || P == MOTOR_CURRENT_PWM_XY)
  #elif PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
    #define PWM_CHK_MOTOR_CURRENT(P) (P == MOTOR_CURRENT_PWM_E || P == MOTOR_CURRENT_PWM_Z)
  #else
    #define PWM_CHK_MOTOR_CURRENT(P) (P == MOTOR_CURRENT_PWM_E)
  #endif
#else
  #define PWM_CHK_MOTOR_CURRENT(P) false
#endif

#ifdef NUM_SERVOS
  #if AVR_ATmega2560_FAMILY
    #define PWM_CHK_SERVO(P) (P == 5 || (NUM_SERVOS > 12 && P == 6) || (NUM_SERVOS > 24 && P == 46))  // PWMS 3A, 4A & 5A
  #elif AVR_ATmega2561_FAMILY
    #define PWM_CHK_SERVO(P)   (P == 5)  // PWM3A
  #elif AVR_ATmega1284_FAMILY
    #define PWM_CHK_SERVO(P)   false
  #elif AVR_AT90USB1286_FAMILY
    #define PWM_CHK_SERVO(P)   (P == 16) // PWM3A
  #elif AVR_ATmega328_FAMILY
    #define PWM_CHK_SERVO(P)   false
  #endif
#else
  #define PWM_CHK_SERVO(P) false
#endif

#if ENABLED(BARICUDA)
  #if HAS_HEATER_1 && HAS_HEATER_2
    #define PWM_CHK_HEATER(P) (P == HEATER_1_PIN || P == HEATER_2_PIN)
  #elif HAS_HEATER_1
    #define PWM_CHK_HEATER(P) (P == HEATER_1_PIN)
  #endif
#else
    #define PWM_CHK_HEATER(P) false
#endif

#define PWM_CHK(P) (PWM_CHK_HEATER(P) || PWM_CHK_SERVO(P) || PWM_CHK_MOTOR_CURRENT(P) || PWM_CHK_FAN_A(P) || PWM_CHK_FAN_B(P))

#endif // PWM_CHK is not used in Marlin

// define which hardware PWMs are available for the current CPU
// all timer 1 PWMS deleted from this list because they are never available
#if AVR_ATmega2560_FAMILY
  #define PWM_PIN(P)  ((P >= 2 && P <= 10) || P == 13 || P == 44 || P == 45 || P == 46)
#elif AVR_ATmega2561_FAMILY
  #define PWM_PIN(P)  ((P >= 2 && P <= 6) || P == 9)
#elif AVR_ATmega1284_FAMILY
  #define PWM_PIN(P)  (P == 3 || P == 4 || P == 14 || P == 15)
#elif AVR_AT90USB1286_FAMILY
  #define PWM_PIN(P)  (P == 0 || P == 1 || P == 14 || P == 15 || P == 16 || P == 24)
#elif AVR_ATmega328_FAMILY
  #define PWM_PIN(P)  (P == 3 || P == 5 || P == 6 || P == 11)
#else
  #error "unknown CPU"
#endif
