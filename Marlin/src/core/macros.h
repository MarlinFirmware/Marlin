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

#if !defined(__has_include)
  #define __has_include(...) 1
#endif

#define ABCE 4
#define XYZE 4
#define ABC  3
#define XYZ  3
#define XY   2

#define _AXIS(A) (A##_AXIS)

#define _XSTOP_  0x01
#define _YSTOP_  0x02
#define _ZSTOP_  0x03
#define _ISTOP_  0x04
#define _JSTOP_  0x05
#define _KSTOP_  0x06
#define _XMIN_   0x11
#define _YMIN_   0x12
#define _ZMIN_   0x13
#define _IMIN_   0x14
#define _JMIN_   0x15
#define _KMIN_   0x16
#define _XMAX_   0x21
#define _YMAX_   0x22
#define _ZMAX_   0x23
#define _IMAX_   0x24
#define _JMAX_   0x25
#define _KMAX_   0x26
#define _XDIAG_  0x31
#define _YDIAG_  0x32
#define _ZDIAG_  0x33
#define _IDIAG_  0x34
#define _JDIAG_  0x35
#define _KDIAG_  0x36
#define _E0DIAG_ 0xE0
#define _E1DIAG_ 0xE1
#define _E2DIAG_ 0xE2
#define _E3DIAG_ 0xE3
#define _E4DIAG_ 0xE4
#define _E5DIAG_ 0xE5
#define _E6DIAG_ 0xE6
#define _E7DIAG_ 0xE7

#define _FORCE_INLINE_ __attribute__((__always_inline__)) __inline__
#define  FORCE_INLINE  __attribute__((always_inline)) inline
#define NO_INLINE      __attribute__((noinline))
#define _UNUSED      __attribute__((unused))
#define _O0          __attribute__((optimize("O0")))
#define _Os          __attribute__((optimize("Os")))
#define _O1          __attribute__((optimize("O1")))
#define _O2          __attribute__((optimize("O2")))
#define _O3          __attribute__((optimize("O3")))

#define IS_CONSTEXPR(...) __builtin_constant_p(__VA_ARGS__) // Only valid solution with C++14. Should use std::is_constant_evaluated() in C++20 instead

#ifndef UNUSED
  #define UNUSED(x) ((void)(x))
#endif

// Clock speed factors
#if !defined(CYCLES_PER_MICROSECOND) && !defined(__STM32F1__)
  #define CYCLES_PER_MICROSECOND (F_CPU / 1000000UL) // 16 or 20 on AVR
#endif

// Nanoseconds per cycle
#define NANOSECONDS_PER_CYCLE (1000000000.0 / F_CPU)

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
  #define SBI(A,B) (A |= _BV(B))
#endif
#ifndef CBI
  #define CBI(A,B) (A &= ~_BV(B))
#endif
#define TBI(N,B) (N ^= _BV(B))
#define _BV32(b) (1UL << (b))
#define TEST32(n,b) !!((n)&_BV32(b))
#define SBI32(n,b) (n |= _BV32(b))
#define CBI32(n,b) (n &= ~_BV32(b))
#define TBI32(N,B) (N ^= _BV32(B))

#define cu(x)      ({__typeof__(x) _x = (x); (_x)*(_x)*(_x);})
#define RADIANS(d) ((d)*float(M_PI)/180.0f)
#define DEGREES(r) ((r)*180.0f/float(M_PI))
#define HYPOT2(x,y) (sq(x)+sq(y))
#define NORMSQ(x,y,z) (sq(x)+sq(y)+sq(z))

#define CIRCLE_AREA(R) (float(M_PI) * sq(float(R)))
#define CIRCLE_CIRC(R) (2 * float(M_PI) * float(R))

#define SIGN(a) ({__typeof__(a) _a = (a); (_a>0)-(_a<0);})
#define IS_POWER_OF_2(x) ((x) && !((x) & ((x) - 1)))

// Macros to constrain values
#ifdef __cplusplus

  // C++11 solution that is standards compliant.
  template <class V, class N> static constexpr void NOLESS(V& v, const N n) {
    if (n > v) v = n;
  }
  template <class V, class N> static constexpr void NOMORE(V& v, const N n) {
    if (n < v) v = n;
  }
  template <class V, class N1, class N2> static constexpr void LIMIT(V& v, const N1 n1, const N2 n2) {
    if (n1 > v) v = n1;
    else if (n2 < v) v = n2;
  }

#else

  #define NOLESS(v, n) \
    do{ \
      __typeof__(v) _n = (n); \
      if (_n > v) v = _n; \
    }while(0)

  #define NOMORE(v, n) \
    do{ \
      __typeof__(v) _n = (n); \
      if (_n < v) v = _n; \
    }while(0)

  #define LIMIT(v, n1, n2) \
    do{ \
      __typeof__(v) _n1 = (n1); \
      __typeof__(v) _n2 = (n2); \
      if (_n1 > v) v = _n1; \
      else if (_n2 < v) v = _n2; \
    }while(0)

#endif

// Macros to chain up to 40 conditions
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
#define _DO_13(W,C,A,V...) (_##W##_1(A) C _DO_12(W,C,V))
#define _DO_14(W,C,A,V...) (_##W##_1(A) C _DO_13(W,C,V))
#define _DO_15(W,C,A,V...) (_##W##_1(A) C _DO_14(W,C,V))
#define _DO_16(W,C,A,V...) (_##W##_1(A) C _DO_15(W,C,V))
#define _DO_17(W,C,A,V...) (_##W##_1(A) C _DO_16(W,C,V))
#define _DO_18(W,C,A,V...) (_##W##_1(A) C _DO_17(W,C,V))
#define _DO_19(W,C,A,V...) (_##W##_1(A) C _DO_18(W,C,V))
#define _DO_20(W,C,A,V...) (_##W##_1(A) C _DO_19(W,C,V))
#define _DO_21(W,C,A,V...) (_##W##_1(A) C _DO_20(W,C,V))
#define _DO_22(W,C,A,V...) (_##W##_1(A) C _DO_21(W,C,V))
#define _DO_23(W,C,A,V...) (_##W##_1(A) C _DO_22(W,C,V))
#define _DO_24(W,C,A,V...) (_##W##_1(A) C _DO_23(W,C,V))
#define _DO_25(W,C,A,V...) (_##W##_1(A) C _DO_24(W,C,V))
#define _DO_26(W,C,A,V...) (_##W##_1(A) C _DO_25(W,C,V))
#define _DO_27(W,C,A,V...) (_##W##_1(A) C _DO_26(W,C,V))
#define _DO_28(W,C,A,V...) (_##W##_1(A) C _DO_27(W,C,V))
#define _DO_29(W,C,A,V...) (_##W##_1(A) C _DO_28(W,C,V))
#define _DO_30(W,C,A,V...) (_##W##_1(A) C _DO_29(W,C,V))
#define _DO_31(W,C,A,V...) (_##W##_1(A) C _DO_30(W,C,V))
#define _DO_32(W,C,A,V...) (_##W##_1(A) C _DO_31(W,C,V))
#define _DO_33(W,C,A,V...) (_##W##_1(A) C _DO_32(W,C,V))
#define _DO_34(W,C,A,V...) (_##W##_1(A) C _DO_33(W,C,V))
#define _DO_35(W,C,A,V...) (_##W##_1(A) C _DO_34(W,C,V))
#define _DO_36(W,C,A,V...) (_##W##_1(A) C _DO_35(W,C,V))
#define _DO_37(W,C,A,V...) (_##W##_1(A) C _DO_36(W,C,V))
#define _DO_38(W,C,A,V...) (_##W##_1(A) C _DO_37(W,C,V))
#define _DO_39(W,C,A,V...) (_##W##_1(A) C _DO_38(W,C,V))
#define _DO_40(W,C,A,V...) (_##W##_1(A) C _DO_39(W,C,V))
#define __DO_N(W,C,N,V...) _DO_##N(W,C,V)
#define _DO_N(W,C,N,V...)  __DO_N(W,C,N,V)
#define DO(W,C,V...)       (_DO_N(W,C,NUM_ARGS(V),V))

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
#define COUNT_ENABLED(V...) DO(ENA,+,V)

#define TERN(O,A,B)         _TERN(_ENA_1(O),B,A)    // OPTION ? 'A' : 'B'
#define TERN0(O,A)          _TERN(_ENA_1(O),0,A)    // OPTION ? 'A' : '0'
#define TERN1(O,A)          _TERN(_ENA_1(O),1,A)    // OPTION ? 'A' : '1'
#define TERN_(O,A)          _TERN(_ENA_1(O),,A)     // OPTION ? 'A' : '<nul>'
#define _TERN(E,V...)       __TERN(_CAT(T_,E),V)    // Prepend 'T_' to get 'T_0' or 'T_1'
#define __TERN(T,V...)      ___TERN(_CAT(_NO,T),V)  // Prepend '_NO' to get '_NOT_0' or '_NOT_1'
#define ___TERN(P,V...)     THIRD(P,V)              // If first argument has a comma, A. Else B.

#define _OPTITEM(A...)      A,
#define OPTITEM(O,A...)     TERN_(O,DEFER4(_OPTITEM)(A))
#define _OPTARG(A...)       , A
#define OPTARG(O,A...)      TERN_(O,DEFER4(_OPTARG)(A))
#define _OPTCODE(A)         A;
#define OPTCODE(O,A)        TERN_(O,DEFER4(_OPTCODE)(A))

// Macros to avoid 'f + 0.0' which is not always optimized away. Minus included for symmetry.
// Compiler flags -fno-signed-zeros -ffinite-math-only also cover 'f * 1.0', 'f - f', etc.
#define PLUS_TERN0(O,A)     _TERN(_ENA_1(O),,+ (A)) // OPTION ? '+ (A)' : '<nul>'
#define MINUS_TERN0(O,A)    _TERN(_ENA_1(O),,- (A)) // OPTION ? '- (A)' : '<nul>'
#define SUM_TERN(O,B,A)     ((B) PLUS_TERN0(O,A))   // ((B) (OPTION ? '+ (A)' : '<nul>'))
#define DIFF_TERN(O,B,A)    ((B) MINUS_TERN0(O,A))  // ((B) (OPTION ? '- (A)' : '<nul>'))

#define IF_ENABLED          TERN_
#define IF_DISABLED(O,A)    TERN(O,,A)

#define ANY(V...)          !DISABLED(V)
#define NONE(V...)          DISABLED(V)
#define ALL(V...)           ENABLED(V)
#define BOTH(V1,V2)         ALL(V1,V2)
#define EITHER(V1,V2)       ANY(V1,V2)
#define MANY(V...)          (COUNT_ENABLED(V) > 1)

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
#define ISEOL(C)            ((C) == '\n' || (C) == '\r')
#define NUMERIC(a)          WITHIN(a, '0', '9')
#define DECIMAL(a)          (NUMERIC(a) || a == '.')
#define HEXCHR(a)           (NUMERIC(a) ? (a) - '0' : WITHIN(a, 'a', 'f') ? ((a) - 'a' + 10)  : WITHIN(a, 'A', 'F') ? ((a) - 'A' + 10) : -1)
#define NUMERIC_SIGNED(a)   (NUMERIC(a) || (a) == '-' || (a) == '+')
#define DECIMAL_SIGNED(a)   (DECIMAL(a) || (a) == '-' || (a) == '+')
#define COUNT(a)            (sizeof(a)/sizeof(*a))
#define ZERO(a)             memset(a,0,sizeof(a))
#define COPY(a,b) do{ \
    static_assert(sizeof(a[0]) == sizeof(b[0]), "COPY: '" STRINGIFY(a) "' and '" STRINGIFY(b) "' types (sizes) don't match!"); \
    memcpy(&a[0],&b[0],_MIN(sizeof(a),sizeof(b))); \
  }while(0)

#define CODE_16( A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,...) A; B; C; D; E; F; G; H; I; J; K; L; M; N; O; P
#define CODE_15( A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,...) A; B; C; D; E; F; G; H; I; J; K; L; M; N; O
#define CODE_14( A,B,C,D,E,F,G,H,I,J,K,L,M,N,...) A; B; C; D; E; F; G; H; I; J; K; L; M; N
#define CODE_13( A,B,C,D,E,F,G,H,I,J,K,L,M,...) A; B; C; D; E; F; G; H; I; J; K; L; M
#define CODE_12( A,B,C,D,E,F,G,H,I,J,K,L,...) A; B; C; D; E; F; G; H; I; J; K; L
#define CODE_11( A,B,C,D,E,F,G,H,I,J,K,...) A; B; C; D; E; F; G; H; I; J; K
#define CODE_10( A,B,C,D,E,F,G,H,I,J,...) A; B; C; D; E; F; G; H; I; J
#define CODE_9( A,B,C,D,E,F,G,H,I,...) A; B; C; D; E; F; G; H; I
#define CODE_8( A,B,C,D,E,F,G,H,...) A; B; C; D; E; F; G; H
#define CODE_7( A,B,C,D,E,F,G,...) A; B; C; D; E; F; G
#define CODE_6( A,B,C,D,E,F,...) A; B; C; D; E; F
#define CODE_5( A,B,C,D,E,...) A; B; C; D; E
#define CODE_4( A,B,C,D,...) A; B; C; D
#define CODE_3( A,B,C,...) A; B; C
#define CODE_2( A,B,...) A; B
#define CODE_1( A,...) A
#define CODE_0(...)
#define _CODE_N(N,V...) CODE_##N(V)
#define CODE_N(N,V...) _CODE_N(N,V)

#define GANG_16(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,...) A B C D E F G H I J K L M N O P
#define GANG_15(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,...) A B C D E F G H I J K L M N O
#define GANG_14(A,B,C,D,E,F,G,H,I,J,K,L,M,N,...) A B C D E F G H I J K L M N
#define GANG_13(A,B,C,D,E,F,G,H,I,J,K,L,M...) A B C D E F G H I J K L M
#define GANG_12(A,B,C,D,E,F,G,H,I,J,K,L...) A B C D E F G H I J K L
#define GANG_11(A,B,C,D,E,F,G,H,I,J,K,...) A B C D E F G H I J K
#define GANG_10(A,B,C,D,E,F,G,H,I,J,...) A B C D E F G H I J
#define GANG_9( A,B,C,D,E,F,G,H,I,...) A B C D E F G H I
#define GANG_8( A,B,C,D,E,F,G,H,...) A B C D E F G H
#define GANG_7( A,B,C,D,E,F,G,...) A B C D E F G
#define GANG_6( A,B,C,D,E,F,...) A B C D E F
#define GANG_5( A,B,C,D,E,...) A B C D E
#define GANG_4( A,B,C,D,...) A B C D
#define GANG_3( A,B,C,...) A B C
#define GANG_2( A,B,...) A B
#define GANG_1( A,...) A
#define GANG_0(...)
#define _GANG_N(N,V...) GANG_##N(V)
#define GANG_N(N,V...) _GANG_N(N,V)
#define GANG_N_1(N,K) _GANG_N(N,K,K,K,K,K,K,K,K,K,K,K,K,K,K,K,K)

// Macros for initializing arrays
#define LIST_20(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T
#define LIST_19(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S
#define LIST_18(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R
#define LIST_17(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,...) A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q
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
#define LIST_0(...)

#define _LIST_N(N,V...) LIST_##N(V)
#define LIST_N(N,V...) _LIST_N(N,V)
#define LIST_N_1(N,K) _LIST_N(N,K,K,K,K,K,K,K,K,K,K,K,K,K,K,K,K)
#define ARRAY_N(N,V...) { _LIST_N(N,V) }
#define ARRAY_N_1(N,K)  { LIST_N_1(N,K) }

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
  template <class T> static constexpr const T ABS(const T v) { return v >= 0 ? v : -v; }
#else
  #define ABS(a) ({__typeof__(a) _a = (a); _a >= 0 ? _a : -_a;})
#endif

#define UNEAR_ZERO(x) ((x) < 0.000001f)
#define NEAR_ZERO(x) WITHIN(x, -0.000001f, 0.000001f)
#define NEAR(x,y) NEAR_ZERO((x)-(y))

#define RECIPROCAL(x) (NEAR_ZERO(x) ? 0 : (1 / float(x)))
#define FIXFLOAT(f)  ({__typeof__(f) _f = (f); _f + (_f < 0 ? -0.0000005f : 0.0000005f);})

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
#define TRUNC(x)    truncf(x)
#define LROUND(x)   lroundf(x)
#define FMOD(x, y)  fmodf(x, y)
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

// Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
#define _NUM_ARGS(_,n,m,l,k,j,i,h,g,f,e,d,c,b,a,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
#define NUM_ARGS(V...) _NUM_ARGS(0,V,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

// Use TWO_ARGS(__VA_ARGS__) to get whether there are 1, 2, or >2 arguments
#define _TWO_ARGS(_,n,m,l,k,j,i,h,g,f,e,d,c,b,a,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
#define TWO_ARGS(V...) _TWO_ARGS(0,V,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,0)

#ifdef __cplusplus

  #ifndef _MINMAX_H_
  #define _MINMAX_H_

    extern "C++" {

      // C++11 solution that is standards compliant. Return type is deduced automatically
      template <class L, class R> static constexpr auto _MIN(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs < rhs ? lhs : rhs;
      }
      template <class L, class R> static constexpr auto _MAX(const L lhs, const R rhs) -> decltype(lhs + rhs) {
        return lhs > rhs ? lhs : rhs;
      }
      template<class T, class ... Ts> static constexpr const T _MIN(T V, Ts... Vs) { return _MIN(V, _MIN(Vs...)); }
      template<class T, class ... Ts> static constexpr const T _MAX(T V, Ts... Vs) { return _MAX(V, _MAX(Vs...)); }

    }

  #endif

  // Allow manipulating enumeration value like flags without ugly cast everywhere
  #define ENUM_FLAGS(T) \
    FORCE_INLINE constexpr T operator&(T x, T y) { return static_cast<T>(static_cast<int>(x) & static_cast<int>(y)); } \
    FORCE_INLINE constexpr T operator|(T x, T y) { return static_cast<T>(static_cast<int>(x) | static_cast<int>(y)); } \
    FORCE_INLINE constexpr T operator^(T x, T y) { return static_cast<T>(static_cast<int>(x) ^ static_cast<int>(y)); } \
    FORCE_INLINE constexpr T operator~(T x)      { return static_cast<T>(~static_cast<int>(x)); } \
    FORCE_INLINE T & operator&=(T &x, T y) { return x &= y; } \
    FORCE_INLINE T & operator|=(T &x, T y) { return x |= y; } \
    FORCE_INLINE T & operator^=(T &x, T y) { return x ^= y; }

  // C++11 solution that is standard compliant. <type_traits> is not available on all platform
  namespace Private {
    template<bool, typename _Tp = void> struct enable_if { };
    template<typename _Tp>              struct enable_if<true, _Tp> { typedef _Tp type; };

    template<typename T, typename U> struct is_same { enum { value = false }; };
    template<typename T> struct is_same<T, T> { enum { value = true }; };

    template <typename T, typename ... Args> struct first_type_of { typedef T type; };
    template <typename T> struct first_type_of<T> { typedef T type; };
  }
  // C++11 solution using SFINAE to detect the existence of a member in a class at compile time.
  // It creates a HasMember<Type> structure containing 'value' set to true if the member exists
  #define HAS_MEMBER_IMPL(Member) \
    namespace Private { \
      template <typename Type, typename Yes=char, typename No=long> struct HasMember_ ## Member { \
        template <typename C> static Yes& test( decltype(&C::Member) ) ; \
        template <typename C> static No& test(...); \
        enum { value = sizeof(test<Type>(0)) == sizeof(Yes) }; }; \
    }

  // Call the method if it exists, but do nothing if it does not. The method is detected at compile time.
  // If the method exists, this is inlined and does not cost anything. Else, an "empty" wrapper is created, returning a default value
  #define CALL_IF_EXISTS_IMPL(Return, Method, ...) \
    HAS_MEMBER_IMPL(Method) \
    namespace Private { \
      template <typename T, typename ... Args> FORCE_INLINE typename enable_if<HasMember_ ## Method <T>::value, Return>::type Call_ ## Method(T * t, Args... a) { return static_cast<Return>(t->Method(a...)); } \
                                                      _UNUSED static                                                  Return  Call_ ## Method(...) { return __VA_ARGS__; } \
    }
  #define CALL_IF_EXISTS(Return, That, Method, ...) \
    static_cast<Return>(Private::Call_ ## Method(That, ##__VA_ARGS__))

  // Compile-time string manipulation
  namespace CompileTimeString {
    // Simple compile-time parser to find the position of the end of a string
    constexpr const char* findStringEnd(const char *str) {
      return *str ? findStringEnd(str + 1) : str;
    }

    // Check whether a string contains a specific character
    constexpr bool contains(const char *str, const char ch) {
      return *str == ch ? true : (*str ? contains(str + 1, ch) : false);
    }
    // Find the last position of the specific character (should be called with findStringEnd)
    constexpr const char* findLastPos(const char *str, const char ch) {
      return *str == ch ? (str + 1) : findLastPos(str - 1, ch);
    }
    // Compile-time evaluation of the last part of a file path
    // Typically used to shorten the path to file in compiled strings
    // CompileTimeString::baseName(__FILE__) returns "macros.h" and not /path/to/Marlin/src/core/macros.h
    constexpr const char* baseName(const char *str) {
      return contains(str, '/') ? findLastPos(findStringEnd(str), '/') : str;
    }

    // Find the first occurrence of a character in a string (or return the last position in the string)
    constexpr const char* findFirst(const char *str, const char ch) {
      return *str == ch || *str == 0 ? (str + 1) : findFirst(str + 1, ch);
    }
    // Compute the string length at compile time
    constexpr unsigned stringLen(const char *str) {
      return *str == 0 ? 0 : 1 + stringLen(str + 1);
    }
  }

  #define ONLY_FILENAME CompileTimeString::baseName(__FILE__)
  /** Get the templated type name. This does not depends on RTTI, but on the preprocessor, so it should be quite safe to use even on old compilers.
      WARNING: DO NOT RENAME THIS FUNCTION (or change the text inside the function to match what the preprocessor will generate)
      The name is chosen very short since the binary will store "const char* gtn(T*) [with T = YourTypeHere]" so avoid long function name here */
  template <typename T>
  inline const char* gtn(T*) {
    // It works on GCC by instantiating __PRETTY_FUNCTION__ and parsing the result. So the syntax here is very limited to GCC output
    constexpr unsigned verboseChatLen = sizeof("const char* gtn(T*) [with T = ") - 1;
    static char templateType[sizeof(__PRETTY_FUNCTION__) - verboseChatLen] = {};
    __builtin_memcpy(templateType, __PRETTY_FUNCTION__ + verboseChatLen, sizeof(__PRETTY_FUNCTION__) - verboseChatLen - 2);
    return templateType;
  }

#else

  #define __MIN_N(N,V...) MIN_##N(V)
  #define _MIN_N(N,V...)  __MIN_N(N,V)
  #define _MIN_N_REF()    _MIN_N
  #define _MIN(V...)      EVAL(_MIN_N(TWO_ARGS(V),V))
  #define MIN_2(a,b)      ((a)<(b)?(a):(b))
  #define MIN_3(a,V...)   MIN_2(a,DEFER2(_MIN_N_REF)()(TWO_ARGS(V),V))

  #define __MAX_N(N,V...) MAX_##N(V)
  #define _MAX_N(N,V...)  __MAX_N(N,V)
  #define _MAX_N_REF()    _MAX_N
  #define _MAX(V...)      EVAL(_MAX_N(TWO_ARGS(V),V))
  #define MAX_2(a,b)      ((a)>(b)?(a):(b))
  #define MAX_3(a,V...)   MAX_2(a,DEFER2(_MAX_N_REF)()(TWO_ARGS(V),V))

#endif

// Macros for adding
#define INC_0   1
#define INC_1   2
#define INC_2   3
#define INC_3   4
#define INC_4   5
#define INC_5   6
#define INC_6   7
#define INC_7   8
#define INC_8   9
#define INC_9  10
#define INC_10 11
#define INC_11 12
#define INC_12 13
#define INC_13 14
#define INC_14 15
#define INC_15 16
#define INC_16 17
#define INC_17 18
#define INC_18 19
#define INC_19 20
#define INC_20 21
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
#define SUM(A,B) _CAT(ADD,A)(B)
#define DOUBLE_(n) ADD##n(n)
#define DOUBLE(n) DOUBLE_(n)

// Macros for subtracting
#define DEC_0   0
#define DEC_1   0
#define DEC_2   1
#define DEC_3   2
#define DEC_4   3
#define DEC_5   4
#define DEC_6   5
#define DEC_7   6
#define DEC_8   7
#define DEC_9   8
#define DEC_10  9
#define DEC_11 10
#define DEC_12 11
#define DEC_13 12
#define DEC_14 13
#define DEC_15 14
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


// Simple Inline IF Macros, friendly to use in other macro definitions
#define IF(O, A, B) ((O) ? (A) : (B))
#define IF_0(O, A) IF(O, A, 0)
#define IF_1(O, A) IF(O, A, 1)

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
#define REPEAT_1(N,OP)          REPEAT_S(1,INCREMENT(N),OP)

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

// See https://github.com/swansontec/map-macro
#define MAP_OUT
#define MAP_END(...)
#define MAP_GET_END() 0, MAP_END
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0 (test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1 (MAP_GET_END test, next)
#define MAP0(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP1) (f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP0) (f, peek, __VA_ARGS__)
#define MAP(f, ...) EVAL512 (MAP1 (f, __VA_ARGS__, (), 0))
