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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#if !defined(__has_include)
  #define __has_include(...) 1
#endif

#define ABCE 4
#define XYZE 4
#define ABC  3
#define XYZ  3
#define XY   2

#define _AXIS(A) (A##_AXIS)

#define _XMIN_   100
#define _YMIN_   200
#define _ZMIN_   300
#define _XMAX_   101
#define _YMAX_   201
#define _ZMAX_   301
#define _XDIAG_  102
#define _YDIAG_  202
#define _ZDIAG_  302
#define _E0DIAG_ 400
#define _E1DIAG_ 401
#define _E2DIAG_ 402
#define _E3DIAG_ 403
#define _E4DIAG_ 404
#define _E5DIAG_ 405
#define _E6DIAG_ 406
#define _E7DIAG_ 407

#define _FORCE_INLINE_ __attribute__((__always_inline__)) __inline__
#define  FORCE_INLINE  __attribute__((always_inline)) inline
#define _UNUSED      __attribute__((unused))
#define _O0          __attribute__((optimize("O0")))
#define _Os          __attribute__((optimize("Os")))
#define _O1          __attribute__((optimize("O1")))
#define _O2          __attribute__((optimize("O2")))
#define _O3          __attribute__((optimize("O3")))

#ifndef UNUSED
  #if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)
    #define UNUSED(X) (void)X
  #else
    #define UNUSED(x) ((void)(x))
  #endif
#endif

// Clock speed factors
#if !defined(CYCLES_PER_MICROSECOND) && !defined(__STM32F1__)
  #define CYCLES_PER_MICROSECOND (F_CPU / 1000000UL) // 16 or 20 on AVR
#endif

// Nanoseconds per cycle
#define NANOSECONDS_PER_CYCLE (1000000000.0 / F_CPU)

// Macros to make sprintf_P read from PROGMEM (AVR extension)
#ifdef __AVR__
  #define S_FMT "%S"
#else
  #define S_FMT "%s"
#endif

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define A(CODE) " " CODE "\n\t"
#define L(CODE) CODE ":\n\t"

// Macros for bit masks
#undef _BV
#define _BV(n) (1<<(n))
#define TEST(n,b) (!!((n)&_BV(b)))
#define SET_BIT_TO(N,B,TF) do{ if (TF) SBI(N,B); else CBI(N,B); }while(0)

#ifndef SBI
  #define SBI(A,B) (A |= (1 << (B)))
#endif

#ifndef CBI
  #define CBI(A,B) (A &= ~(1 << (B)))
#endif

#define _BV32(b) (1UL << (b))
#define TEST32(n,b) !!((n)&_BV32(b))
#define SBI32(n,b) (n |= _BV32(b))
#define CBI32(n,b) (n &= ~_BV32(b))

#define cu(x)      ((x)*(x)*(x))
#define RADIANS(d) ((d)*float(M_PI)/180.0f)
#define DEGREES(r) ((r)*180.0f/float(M_PI))
#define HYPOT2(x,y) (sq(x)+sq(y))

#define CIRCLE_AREA(R) (float(M_PI) * sq(float(R)))
#define CIRCLE_CIRC(R) (2 * float(M_PI) * float(R))

#define SIGN(a) ((a>0)-(a<0))
#define IS_POWER_OF_2(x) ((x) && !((x) & ((x) - 1)))

// Macros to constrain values
#ifdef __cplusplus

  // C++11 solution that is standards compliant.
  template <class V, class N> static inline constexpr void NOLESS(V& v, const N n) {
    if (n > v) v = n;
  }
  template <class V, class N> static inline constexpr void NOMORE(V& v, const N n) {
    if (n < v) v = n;
  }
  template <class V, class N1, class N2> static inline constexpr void LIMIT(V& v, const N1 n1, const N2 n2) {
    if (n1 > v) v = n1;
    else if (n2 < v) v = n2;
  }

#else

  // Using GCC extensions, but Travis GCC version does not like it and gives
  //  "error: statement-expressions are not allowed outside functions nor in template-argument lists"
  #define NOLESS(v, n) \
    do{ \
      __typeof__(n) _n = (n); \
      if (_n > v) v = _n; \
    }while(0)

  #define NOMORE(v, n) \
    do{ \
      __typeof__(n) _n = (n); \
      if (_n < v) v = _n; \
    }while(0)

  #define LIMIT(v, n1, n2) \
    do{ \
      __typeof__(n1) _n1 = (n1); \
      __typeof__(n2) _n2 = (n2); \
      if (_n1 > v) v = _n1; \
      else if (_n2 < v) v = _n2; \
    }while(0)

#endif

// Macros to chain up to 12 conditions
#define _DO_1(W,C,A)       (_##W##_1(A))
#define _DO_2(W,C,A,B)     (_##W##_1(A) C _##W##_1(B))
#define _DO_3(W,C,A,V...)  (_##W##_1(A) C _DO_2(W,C,V))
#define _DO_4(W,C,A,V...)  (_##W##_1(A) C _DO_3(W,C,V))
#define _DO_5(W,C,A,V...)  (_##W##_1(A) C _DO_4(W,C,V))
#define _DO_6(W,C,A,V...)  (_##W##_1(A) C _DO_5(W,C,V))
#define _DO_7(W,C,A,V...)  (_##W##_1(A) C _DO_6(W,C,V))
#define _DO_8(W,C,A,V...)  (_##W##_1(A) C _DO_7(W,C,V))
#define _DO_9(W,C,A,V...)  (_##W##_1(A) C _DO_8(W,C,V))
#define _DO_10(W,C,A,V...) (_##W##_1(A) C _DO_9(W,C,V))
#define _DO_11(W,C,A,V...) (_##W##_1(A) C _DO_10(W,C,V))
#define _DO_12(W,C,A,V...) (_##W##_1(A) C _DO_11(W,C,V))
#define __DO_N(W,C,N,V...) _DO_##N(W,C,V)
#define _DO_N(W,C,N,V...)  __DO_N(W,C,N,V)
#define DO(W,C,V...)       _DO_N(W,C,NUM_ARGS(V),V)

// Macros to support option testing
#define _CAT(a,V...) a##V
#define CAT(a,V...) _CAT(a,V)

#define _ISENA_     ~,1
#define _ISENA_1    ~,1
#define _ISENA_0x1  ~,1
#define _ISENA_true ~,1
#define _ISENA(V...)        IS_PROBE(V)

#define _ENA_1(O)           _ISENA(CAT(_IS,CAT(ENA_, O)))
#define _DIS_1(O)           NOT(_ENA_1(O))
#define ENABLED(V...)       DO(ENA,&&,V)
#define DISABLED(V...)      DO(DIS,&&,V)

#define TERN(O,A,B)         _TERN(_ENA_1(O),B,A)    // OPTION converted to '0' or '1'
#define TERN0(O,A)          _TERN(_ENA_1(O),0,A)    // OPTION converted to A or '0'
#define TERN1(O,A)          _TERN(_ENA_1(O),1,A)    // OPTION converted to A or '1'
#define TERN_(O,A)          _TERN(_ENA_1(O),,A)     // OPTION converted to A or '<nul>'
#define _TERN(E,V...)       __TERN(_CAT(T_,E),V)    // Prepend 'T_' to get 'T_0' or 'T_1'
#define __TERN(T,V...)      ___TERN(_CAT(_NO,T),V)  // Prepend '_NO' to get '_NOT_0' or '_NOT_1'
#define ___TERN(P,V...)     THIRD(P,V)              // If first argument has a comma, A. Else B.

#define ANY(V...)          !DISABLED(V)
#define NONE(V...)          DISABLED(V)
#define ALL(V...)           ENABLED(V)
#define BOTH(V1,V2)         ALL(V1,V2)
#define EITHER(V1,V2)       ANY(V1,V2)

// Macros to support pins/buttons exist testing
#define PIN_EXISTS(PN)      (defined(PN##_PIN) && PN##_PIN >= 0)
#define _PINEX_1            PIN_EXISTS
#define PINS_EXIST(V...)    DO(PINEX,&&,V)
#define ANY_PIN(V...)       DO(PINEX,||,V)

#define BUTTON_EXISTS(BN)   (defined(BTN_##BN) && BTN_##BN >= 0)
#define _BTNEX_1            BUTTON_EXISTS
#define BUTTONS_EXIST(V...) DO(BTNEX,&&,V)
#define ANY_BUTTON(V...)    DO(BTNEX,||,V)

#define WITHIN(N,L,H)       ((N) >= (L) && (N) <= (H))
#define NUMERIC(a)          WITHIN(a, '0', '9')
#define DECIMAL(a)          (NUMERIC(a) || a == '.')
#define NUMERIC_SIGNED(a)   (NUMERIC(a) || (a) == '-' || (a) == '+')
#define DECIMAL_SIGNED(a)   (DECIMAL(a) || (a) == '-' || (a) == '+')
#define COUNT(a)            (sizeof(a)/sizeof(*a))
#define ZERO(a)             memset(a,0,sizeof(a))
#define COPY(a,b) do{ \
    static_assert(sizeof(a[0]) == sizeof(b[0]), "COPY: '" STRINGIFY(a) "' and '" STRINGIFY(b) "' types (sizes) don't match!"); \
    memcpy(&a[0],&b[0],_MIN(sizeof(a),sizeof(b))); \
  }while(0)

// Macros for initializing arrays
#define LIST_16(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P
#define LIST_15(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O
#define LIST_14(A,B,C,D,E,F,G,H,I,J,K,L,M,N,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N
#define LIST_13(A,B,C,D,E,F,G,H,I,J,K,L,M,...) A,B,C,D,E,F,G,H,I,J,K,L,M
#define LIST_12(A,B,C,D,E,F,G,H,I,J,K,L,...) A,B,C,D,E,F,G,H,I,J,K,L
#define LIST_11(A,B,C,D,E,F,G,H,I,J,K,...) A,B,C,D,E,F,G,H,I,J,K
#define LIST_10(A,B,C,D,E,F,G,H,I,J,...) A,B,C,D,E,F,G,H,I,J
#define LIST_9( A,B,C,D,E,F,G,H,I,...) A,B,C,D,E,F,G,H,I
#define LIST_8( A,B,C,D,E,F,G,H,...) A,B,C,D,E,F,G,H
#define LIST_7( A,B,C,D,E,F,G,...) A,B,C,D,E,F,G
#define LIST_6( A,B,C,D,E,F,...) A,B,C,D,E,F
#define LIST_5( A,B,C,D,E,...) A,B,C,D,E
#define LIST_4( A,B,C,D,...) A,B,C,D
#define LIST_3( A,B,C,...) A,B,C
#define LIST_2( A,B,...) A,B
#define LIST_1( A,...) A

#define _LIST_N(N,V...) LIST_##N(V)
#define LIST_N(N,V...) _LIST_N(N,V)
#define ARRAY_N(N,V...) { _LIST_N(N,V) }

#define _JOIN_1(O)         (O)
#define JOIN_N(N,C,V...)   (DO(JOIN,C,LIST_N(N,V)))

#define LOOP_S_LE_N(VAR, S, N) for (uint8_t VAR=(S); VAR<=(N); VAR++)
#define LOOP_S_L_N(VAR, S, N) for (uint8_t VAR=(S); VAR<(N); VAR++)
#define LOOP_LE_N(VAR, N) LOOP_S_LE_N(VAR, 0, N)
#define LOOP_L_N(VAR, N) LOOP_S_L_N(VAR, 0, N)

#define NOOP (void(0))

#define CEILING(x,y) (((x) + (y) - 1) / (y))

#undef ABS
#ifdef __cplusplus
  template <class T> static inline constexpr const T ABS(const T v) { return v >= 0 ? v : -v; }
#else
  #define ABS(a) ({__typeof__(a) _a = (a); _a >= 0 ? _a : -_a;})
#endif

#define UNEAR_ZERO(x) ((x) < 0.000001f)
#define NEAR_ZERO(x) WITHIN(x, -0.000001f, 0.000001f)
#define NEAR(x,y) NEAR_ZERO((x)-(y))

#define RECIPROCAL(x) (NEAR_ZERO(x) ? 0 : (1 / float(x)))
#define FIXFLOAT(f) (f + (f < 0 ? -0.00005f : 0.00005f))

//
// Maths macros that can be overridden by HAL
//
#define ACOS(x)     acosf(x)
#define ATAN2(y, x) atan2f(y, x)
#define POW(x, y)   powf(x, y)
#define SQRT(x)     sqrtf(x)
#define RSQRT(x)    (1.0f / sqrtf(x))
#define CEIL(x)     ceilf(x)
#define FLOOR(x)    floorf(x)
#define LROUND(x)   lroundf(x)
#define FMOD(x, y)  fmodf(x, y)
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

#define I2C_ADDRESS(A) (TERN(TARGET_LPC1768, (A) << 1, A))

// Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
#define _NUM_ARGS(_,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
#define NUM_ARGS(V...) _NUM_ARGS(0,V,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#ifdef __cplusplus

  #ifndef _MINMAX_H_
  #define _MINMAX_H_

    extern "C++" {

      // C++11 solution that is standards compliant. Return type is deduced automatically
      template <class L, class R> static inline constexpr auto _MIN(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs < rhs ? lhs : rhs;
      }
      template <class L, class R> static inline constexpr auto _MAX(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs > rhs ? lhs : rhs;
      }
      template<class T, class ... Ts> static inline constexpr const T _MIN(T V, Ts... Vs) { return _MIN(V, _MIN(Vs...)); }
      template<class T, class ... Ts> static inline constexpr const T _MAX(T V, Ts... Vs) { return _MAX(V, _MAX(Vs...)); }

    }

  #endif

#else

  #define MIN_2(a,b)      ((a)<(b)?(a):(b))
  #define MIN_3(a,V...)   MIN_2(a,MIN_2(V))
  #define MIN_4(a,V...)   MIN_2(a,MIN_3(V))
  #define MIN_5(a,V...)   MIN_2(a,MIN_4(V))
  #define MIN_6(a,V...)   MIN_2(a,MIN_5(V))
  #define MIN_7(a,V...)   MIN_2(a,MIN_6(V))
  #define MIN_8(a,V...)   MIN_2(a,MIN_7(V))
  #define MIN_9(a,V...)   MIN_2(a,MIN_8(V))
  #define MIN_10(a,V...)  MIN_2(a,MIN_9(V))
  #define __MIN_N(N,V...) MIN_##N(V)
  #define _MIN_N(N,V...)  __MIN_N(N,V)
  #define _MIN(V...)      _MIN_N(NUM_ARGS(V), V)

  #define MAX_2(a,b)      ((a)>(b)?(a):(b))
  #define MAX_3(a,V...)   MAX_2(a,MAX_2(V))
  #define MAX_4(a,V...)   MAX_2(a,MAX_3(V))
  #define MAX_5(a,V...)   MAX_2(a,MAX_4(V))
  #define MAX_6(a,V...)   MAX_2(a,MAX_5(V))
  #define MAX_7(a,V...)   MAX_2(a,MAX_6(V))
  #define MAX_8(a,V...)   MAX_2(a,MAX_7(V))
  #define MAX_9(a,V...)   MAX_2(a,MAX_8(V))
  #define MAX_10(a,V...)  MAX_2(a,MAX_9(V))
  #define __MAX_N(N,V...) MAX_##N(V)
  #define _MAX_N(N,V...)  __MAX_N(N,V)
  #define _MAX(V...)      _MAX_N(NUM_ARGS(V), V)

#endif

// Macros for adding
#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INCREMENT_(n) INC_##n
#define INCREMENT(n) INCREMENT_(n)

#define ADD0(N)  N
#define ADD1(N)  INCREMENT_(N)
#define ADD2(N)  ADD1(ADD1(N))
#define ADD3(N)  ADD1(ADD2(N))
#define ADD4(N)  ADD2(ADD2(N))
#define ADD5(N)  ADD2(ADD3(N))
#define ADD6(N)  ADD3(ADD3(N))
#define ADD7(N)  ADD3(ADD4(N))
#define ADD8(N)  ADD4(ADD4(N))
#define ADD9(N)  ADD4(ADD5(N))
#define ADD10(N) ADD5(ADD5(N))

// Macros for subtracting
#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DECREMENT_(n) DEC_##n
#define DECREMENT(n) DECREMENT_(n)

#define SUB0(N)  N
#define SUB1(N)  DECREMENT_(N)
#define SUB2(N)  SUB1(SUB1(N))
#define SUB3(N)  SUB1(SUB2(N))
#define SUB4(N)  SUB2(SUB2(N))
#define SUB5(N)  SUB2(SUB3(N))
#define SUB6(N)  SUB3(SUB3(N))
#define SUB7(N)  SUB3(SUB4(N))
#define SUB8(N)  SUB4(SUB4(N))
#define SUB9(N)  SUB4(SUB5(N))
#define SUB10(N) SUB5(SUB5(N))

//
// Primitives supporting precompiler REPEAT
//
#define FIRST(a,...)     a
#define SECOND(a,b,...)  b
#define THIRD(a,b,c,...) c

// Defer expansion
#define EMPTY()
#define DEFER(M)  M EMPTY()
#define DEFER2(M) M EMPTY EMPTY()()
#define DEFER3(M) M EMPTY EMPTY EMPTY()()()
#define DEFER4(M) M EMPTY EMPTY EMPTY EMPTY()()()()

// Force define expansion
#define EVAL(V...)     EVAL16(V)
#define EVAL1024(V...) EVAL512(EVAL512(V))
#define EVAL512(V...)  EVAL256(EVAL256(V))
#define EVAL256(V...)  EVAL128(EVAL128(V))
#define EVAL128(V...)  EVAL64(EVAL64(V))
#define EVAL64(V...)   EVAL32(EVAL32(V))
#define EVAL32(V...)   EVAL16(EVAL16(V))
#define EVAL16(V...)   EVAL8(EVAL8(V))
#define EVAL8(V...)    EVAL4(EVAL4(V))
#define EVAL4(V...)    EVAL2(EVAL2(V))
#define EVAL2(V...)    EVAL1(EVAL1(V))
#define EVAL1(V...)    V

#define IS_PROBE(V...) SECOND(V, 0)     // Get the second item passed, or 0
#define PROBE() ~, 1                    // Second item will be 1 if this is passed
#define _NOT_0 PROBE()
#define NOT(x) IS_PROBE(_CAT(_NOT_, x)) // NOT('0') gets '1'. Anything else gets '0'.
#define _BOOL(x) NOT(NOT(x))            // NOT('0') gets '0'. Anything else gets '1'.

#define IF_ELSE(TF) _IF_ELSE(_BOOL(TF))
#define _IF_ELSE(TF) _CAT(_IF_, TF)

#define _IF_1(V...) V _IF_1_ELSE
#define _IF_0(...)    _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(V...) V

#define HAS_ARGS(V...) _BOOL(FIRST(_END_OF_ARGUMENTS_ V)())
#define _END_OF_ARGUMENTS_() 0

//
// REPEAT core macros. Recurse N times with ascending I.
//

// Call OP(I) N times with ascending counter.
#define _REPEAT(_RPT_I,_RPT_N,_RPT_OP)                        \
  _RPT_OP(_RPT_I)                                             \
  IF_ELSE(SUB1(_RPT_N))                                       \
    ( DEFER2(__REPEAT)()(ADD1(_RPT_I),SUB1(_RPT_N),_RPT_OP) ) \
    ( /* Do nothing */ )
#define __REPEAT() _REPEAT

// Call OP(I, ...) N times with ascending counter.
#define _REPEAT2(_RPT_I,_RPT_N,_RPT_OP,V...)                     \
  _RPT_OP(_RPT_I,V)                                              \
  IF_ELSE(SUB1(_RPT_N))                                          \
    ( DEFER2(__REPEAT2)()(ADD1(_RPT_I),SUB1(_RPT_N),_RPT_OP,V) ) \
    ( /* Do nothing */ )
#define __REPEAT2() _REPEAT2

// Repeat a macro passing S...N-1.
#define REPEAT_S(S,N,OP)        EVAL(_REPEAT(S,SUB##S(N),OP))
#define REPEAT(N,OP)            REPEAT_S(0,N,OP)

// Repeat a macro passing 0...N-1 plus additional arguments.
#define REPEAT2_S(S,N,OP,V...)  EVAL(_REPEAT2(S,SUB##S(N),OP,V))
#define REPEAT2(N,OP,V...)      REPEAT2_S(0,N,OP,V)

// Use RREPEAT macros with REPEAT macros for nesting
#define _RREPEAT(_RPT_I,_RPT_N,_RPT_OP)                           \
  _RPT_OP(_RPT_I)                                                 \
  IF_ELSE(SUB1(_RPT_N))                                           \
    ( DEFER2(__RREPEAT)()(ADD1(_RPT_I),SUB1(_RPT_N),_RPT_OP) )    \
    ( /* Do nothing */ )
#define __RREPEAT() _RREPEAT
#define _RREPEAT2(_RPT_I,_RPT_N,_RPT_OP,V...)                     \
  _RPT_OP(_RPT_I,V)                                               \
  IF_ELSE(SUB1(_RPT_N))                                           \
    ( DEFER2(__RREPEAT2)()(ADD1(_RPT_I),SUB1(_RPT_N),_RPT_OP,V) ) \
    ( /* Do nothing */ )
#define __RREPEAT2() _RREPEAT2
#define RREPEAT_S(S,N,OP)        EVAL1024(_RREPEAT(S,SUB##S(N),OP))
#define RREPEAT(N,OP)            RREPEAT_S(0,N,OP)
#define RREPEAT2_S(S,N,OP,V...)  EVAL1024(_RREPEAT2(S,SUB##S(N),OP,V))
#define RREPEAT2(N,OP,V...)      RREPEAT2_S(0,N,OP,V)
