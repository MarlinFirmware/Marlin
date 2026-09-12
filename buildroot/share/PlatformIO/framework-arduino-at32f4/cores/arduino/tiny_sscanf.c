/******************************************************************************
 * @file    tiny_sscanf.c
 * @author  MCD Application Team
 * @version V1.1.4
 * @date    08-January-2018
 * @brief   Tiny implementation of sscanf
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/*  $OpenBSD: sscanf.c,v 1.12 2005/08/08 08:05:36 espie Exp $ */
/*-
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/*
 * This code is derived from
 *     https://github.com/rapid7/metasploit-payloads/, in c/meterpreter/source/bionic/libc/stdio/sscanf.c
 * It has been derived in order to optimize code size. In this context
 * all the formats are not supported. Current supported formats are
 *    %hx, %hhx, %ul, %d,...
 * when TINY_SSCANF is defined
 *
 * When TINY_NO_OX is defined, this is not possible to sscanf("%x") of "0xab",
 * only "ab" is possible
 *
 * When TINY_SPACE_NOT_SPECIALCASE is defined, "space" is not a special character.
 * That means that we expect a single space, and not any of ispace() character
 * (space, tabs,...)
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tiny_sscanf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TINY_SSCANF
#define TINY_NO_OX
#define TINY_SPACE_NOT_SPECIALCASE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

#ifdef FLOATING_POINT
#include "floatio.h"
#endif

#define  BUF    513  /* Maximum length of numeric string. */

/*
 * Flags used during conversion.
 */
#define  LONG    0x00001  /* l: long or double */
#define  SHORT    0x00004  /* h: short */
#define  SHORTSHORT  0x00008  /* hh: 8 bit integer */
#define  UNSIGNED  0x00800  /* %[oupxX] conversions */
#ifdef TINY_SSCANF
#else
#define  LONGDBL    0x00002  /* L: long double; unimplemented */
#define LLONG      0x00010  /* ll: long long (+ deprecated q: quad) */
#define  POINTER    0x00020  /* p: void * (as hex) */
#define  SIZEINT    0x00040  /* z: (signed) size_t */
#define  MAXINT    0x00080  /* j: intmax_t */
#define  PTRINT    0x00100  /* t: ptrdiff_t */
#define  NOSKIP    0x00200  /* [ or c: do not skip blanks */
#define  SUPPRESS  0x00400  /* *: suppress assignment */
#endif

/*
 * The following are used in numeric conversions only:
 * SIGNOK, HAVESIGN, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, HAVESIGN, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */
#define  SIGNOK    0x01000  /* +/- is (still) legal */
#define  HAVESIGN  0x02000  /* sign detected */
#define  NDIGITS    0x04000  /* no digits detected */

#define  DPTOK    0x08000  /* (float) decimal point is still legal */
#define  EXPOK    0x10000  /* (float) exponent (e+3, etc) still legal */

#ifdef TINY_NO_OX
#else
#define  PFXOK    0x08000  /* 0x prefix is (still) legal */
#define  NZDIGITS  0x10000  /* no zero digits detected */
#endif

/*
 * Conversion types.
 */
#define  CT_INT    3  /* integer, i.e., strtoimax or strtoumax */
#define  CT_FLOAT  4  /* floating, i.e., strtod */

#ifdef TINY_SSCANF
#else
#define  CT_CHAR    0  /* %c conversion */
#define  CT_CCL    1  /* %[...] conversion */
#define  CT_STRING  2  /* %s conversion */
#endif

#define u_char unsigned char
#define u_long unsigned long

#ifdef TINY_SSCANF
#else
static u_char *__sccl(char *, u_char *);
#endif

#define VFSCANF tiny_vfscanf

#if !defined(VFSCANF)
#define VFSCANF  vfscanf
#endif


#define __srefill(_x) 1
#define ungetc(_c, _fp) do { (_c), fp_p--; fp_r++; } while (0)
  
/*
 * vfscanf
 */
static inline int
VFSCANF(const char *str, const char *fmt0, __va_list ap)
{
  u_char *fmt = (u_char *)fmt0;
  int c;    /* character from format, or conversion */
  size_t width;  /* field width, or 0 */
  char *p;  /* points into all kinds of strings */
  int flags;  /* flags as defined above */
  int nassigned;    /* number of fields assigned */
  int nread;    /* number of characters consumed from fp */
  int base;    /* base argument to strtoimax/strtouimax */
  char buf[BUF];    /* buffer for numeric conversions */
  const char *fp_p;
  int fp_r;
  uintmax_t value;
  int sign_minus;


#ifdef TINY_SSCANF
#else
  int n;    /* handy integer */
  char *p0;  /* saves original value of p when necessary */
  char ccltab[256];  /* character class table for %[...] */
#endif
  
  /* `basefix' is used to avoid `if' tests in the integer scanner */
#ifdef TINY_SSCANF
  /* basefix[] can be removed as we do not support %i */
#else  
  static short basefix[17] =
    { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
#endif
    
   fp_p = str;
  fp_r = strlen(str);

  nassigned = 0;
  nread = 0;
  base = 0;    /* XXX just to keep gcc happy */
  for (;;) {
    c = *fmt++;
    if (c == 0)
      return (nassigned);
#ifdef TINY_SPACE_NOT_SPECIALCASE
#else
    if (isspace(c)) {
      while ((fp_r > 0 || __srefill(fp) == 0) &&
          isspace(*fp_p))
        nread++, fp_r--, fp_p++;
      continue;
    }
#endif
    if (c != '%')
      goto literal;
    width = 0;
    flags = 0;
    /*
     * switch on the format.  continue if done;
     * break once format type is derived.
     */
again:    c = *fmt++;
    switch (c) {
    case '%':
literal:
      if (fp_r <= 0 && __srefill(fp))
        goto input_failure;
      if (*fp_p != c)
        goto match_failure;
      fp_r--, fp_p++;
      nread++;
      continue;

#ifdef TINY_SSCANF
#else
    case '*':
      flags |= SUPPRESS;
      goto again;
    case 'j':
      flags |= MAXINT;
      goto again;
    case 'L':
      flags |= LONGDBL;
      goto again;
#endif
    case 'h':
      if (*fmt == 'h') {
        fmt++;
        flags |= SHORTSHORT;
      } else {
        flags |= SHORT;
      }
      goto again;
    case 'l':
#ifdef TINY_SSCANF
      /* %ll not supported */
      flags |= LONG;
      goto again;
#else
      if (*fmt == 'l') {
        fmt++;
        flags |= LLONG;
      } else {
        flags |= LONG;
      }
      goto again;
#endif

#ifdef TINY_SSCANF
#else
    case 'q':
      flags |= LLONG;    /* deprecated */
      goto again;
    case 't':
      flags |= PTRINT;
      goto again;
    case 'z':
      flags |= SIZEINT;
      goto again;
#endif
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      width = width * 10 + c - '0';
      goto again;

    /*
     * Conversions.
     * Those marked `compat' are for 4.[123]BSD compatibility.
     *
     * (According to ANSI, E and X formats are supposed
     * to the same as e and x.  Sorry about that.)
     */
    case 'D':  /* compat */
      flags |= LONG;
      /* FALLTHROUGH */
    case 'd':
      c = CT_INT;
      base = 10;
      break;

#ifdef TINY_SSCANF
    /*
     * We do not support %i to remove potential base=8 in the following
     * Hence basefix can be removed
     */
#else
    case 'i':
      c = CT_INT;
      base = 0;
      break;
#endif
    
#ifdef TINY_SSCANF
#else
    case 'O':  /* compat */
      flags |= LONG;
      /* FALLTHROUGH */
    case 'o':
      c = CT_INT;
      flags |= UNSIGNED;
      base = 8;
      break;
#endif
    
    case 'u':
      c = CT_INT;
      flags |= UNSIGNED;
      base = 10;
      break;

    case 'X':
    case 'x':
#ifdef TINY_NO_OX 
#else
      flags |= PFXOK;  /* enable 0x prefixing */
#endif
      c = CT_INT;
      flags |= UNSIGNED;
      base = 16;
      break;

#ifdef FLOATING_POINT
    case 'E':
    case 'G':
    case 'e': 
    case 'f': 
    case 'g':
      c = CT_FLOAT;
      break;
#endif

#ifdef TINY_SSCANF
#else
    case 's':
      c = CT_STRING;
      break;

    case '[':
      fmt = __sccl(ccltab, fmt);
      flags |= NOSKIP;
      c = CT_CCL;
      break;

    case 'c':
      flags |= NOSKIP;
      c = CT_CHAR;
      break;

    case 'p':  /* pointer format is like hex */
      flags |= POINTER | PFXOK;
      c = CT_INT;
      flags |= UNSIGNED;
      base = 16;
      break;

    case 'n':
      if (flags & SUPPRESS)
        continue;
      if (flags & SHORTSHORT)
        *va_arg(ap, char *) = nread;
      else if (flags & SHORT)
        *va_arg(ap, short *) = nread;
      else if (flags & LONG)
        *va_arg(ap, long *) = nread;
      else if (flags & SIZEINT)
        *va_arg(ap, size_t *) = nread;
      else if (flags & PTRINT)
        *va_arg(ap, ptrdiff_t *) = nread;
      else if (flags & LLONG)
        *va_arg(ap, long long *) = nread;
      else if (flags & MAXINT)
        *va_arg(ap, intmax_t *) = nread;
      else
        *va_arg(ap, int *) = nread;
      continue;
#endif

    /*
     * Disgusting backwards compatibility hacks.  XXX
     */
    case '\0':  /* compat */
      return (EOF);

    default:  /* compat */
#ifdef TINY_SSCANF
#else
      if (isupper(c))
        flags |= LONG;
      c = CT_INT;
      base = 10;
#endif
      break;
    }

    /*
     * We have a conversion that requires input.
     */
    if (fp_r <= 0 && __srefill(fp))
      goto input_failure;

    
    /*
     * Consume leading white space, except for formats
     * that suppress this.
     */
#ifdef TINY_SSCANF
#else
    if ((flags & NOSKIP) == 0) {
      while (isspace(*fp_p)) {
        nread++;
        if (--fp_r > 0)
          fp_p++;
        else if (__srefill(fp))
          goto input_failure;
      }
      /*
       * Note that there is at least one character in
       * the buffer, so conversions that do not set NOSKIP
       * ca no longer result in an input failure.
       */
    }
#endif
    
    /*
     * Do the conversion.
     */
    switch (c) {
#ifdef TINY_SSCANF
#else
    case CT_CHAR:
      /* scan arbitrary characters (sets NOSKIP) */
      if (width == 0)
        width = 1;
      if (flags & SUPPRESS) {
        size_t sum = 0;
        for (;;) {
          if ((n = fp_r) < (int)width) {
            sum += n;
            width -= n;
            fp_p += n;
            if (__srefill(fp)) {
              if (sum == 0)
                  goto input_failure;
              break;
            }
          } else {
            sum += width;
            fp_r -= width;
            fp_p += width;
            break;
          }
        }
        nread += sum;
      } else {
        size_t r = fread((void *)va_arg(ap, char *), 1,
            width, fp);

        if (r == 0)
          goto input_failure;
        nread += r;
        nassigned++;
      }
      break;
#endif
      
#ifdef TINY_SSCANF
#else
    case CT_CCL:
      /* scan a (nonempty) character class (sets NOSKIP) */
      if (width == 0)
        width = (size_t)~0;  /* `infinity' */
      /* take only those things in the class */
      if (flags & SUPPRESS) {
        n = 0;
        while (ccltab[*fp_p]) {
          n++, fp_r--, fp_p++;
          if (--width == 0)
            break;
          if (fp_r <= 0 && __srefill(fp)) {
            if (n == 0)
              goto input_failure;
            break;
          }
        }
        if (n == 0)
          goto match_failure;
      } else {
        p0 = p = va_arg(ap, char *);
        while (ccltab[*fp_p]) {
          fp_r--;
          *p++ = *fp_p++;
          if (--width == 0)
            break;
          if (fp_r <= 0 && __srefill(fp)) {
            if (p == p0)
              goto input_failure;
            break;
          }
        }
        n = p - p0;
        if (n == 0)
          goto match_failure;
        *p = '\0';
        nassigned++;
      }
      nread += n;
      break;
#endif
      
#ifdef TINY_SSCANF
#else
    case CT_STRING:
      /* like CCL, but zero-length string OK, & no NOSKIP */
      if (width == 0)
        width = (size_t)~0;
      if (flags & SUPPRESS) {
        n = 0;
        while (!isspace(*fp_p)) {
          n++, fp_r--, fp_p++;
          if (--width == 0)
            break;
          if (fp_r <= 0 && __srefill(fp))
            break;
        }
        nread += n;
      } else {
        p0 = p = va_arg(ap, char *);
        while (!isspace(*fp_p)) {
          fp_r--;
          *p++ = *fp_p++;
          if (--width == 0)
            break;
          if (fp_r <= 0 && __srefill(fp))
            break;
        }
        *p = '\0';
        nread += p - p0;
        nassigned++;
      }
      continue;
#endif
      
    case CT_INT:
      /* scan an integer as if by strtoimax/strtoumax */
#ifdef hardway
      if (width == 0 || width > sizeof(buf) - 1)
        width = sizeof(buf) - 1;
#else
      /* size_t is unsigned, hence this optimisation */
      if (--width > sizeof(buf) - 2)
        width = sizeof(buf) - 2;
      width++;
#endif

#ifdef TINY_NO_OX
      flags |= SIGNOK | NDIGITS;
#else
      flags |= SIGNOK | NDIGITS | NZDIGITS;
#endif
      
      sign_minus = 0;
      value = 0;
      for (p = buf; width; width--) {
        c = *fp_p;
        /*
         * Switch on the character; `goto ok'
         * if we accept it as a part of number.
         */
        switch (c) {

        /*
         * The digit 0 is always legal, but is
         * special.  For %i conversions, if no
         * digits (zero or nonzero) have been
         * scanned (only signs), we will have
         * base==0.  In that case, we should set
         * it to 8 and enable 0x prefixing.
         * Also, if we have not scanned zero digits
         * before this, do not turn off prefixing
         * (someone else will turn it off if we
         * have scanned any nonzero digits).
         */
        case '0':
#ifdef TINY_NO_OX 
          /* FALLTHROUGH */
#else        
#ifdef TINY_SSCANF
#else
          if (base == 0) {
            base = 8;
            flags |= PFXOK;
          }
#endif 
          if (!(flags & NDIGITS)) {
            value = value * base;
          }
          
          if (flags & NZDIGITS)
              flags &= ~(SIGNOK|NZDIGITS|NDIGITS);
          else
              flags &= ~(SIGNOK|PFXOK|NDIGITS);
          goto ok;
#endif
          
#ifdef TINY_SSCANF
        /* we only support base 10 and 16 */
        case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
#ifdef TINY_NO_OX 
          flags &= ~(SIGNOK | NDIGITS);
#else
          flags &= ~(SIGNOK | PFXOK | NDIGITS);
#endif
          value = value * base + c - '0';
          goto ok;
#else
        /* 1 through 7 always legal */
        case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
          base = basefix[base];
          flags &= ~(SIGNOK | PFXOK | NDIGITS);
          value = value * base + c - '0';
          goto ok;

        /* digits 8 and 9 ok iff decimal or hex */
        case '8': case '9':
          base = basefix[base];
          if (base <= 8)
            break;  /* not legal here */
          flags &= ~(SIGNOK | PFXOK | NDIGITS);
          value = value * base + c - '0';
          goto ok;
#endif
          
        /* letters ok iff hex */
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
          /* no need to fix base here */
          if (base <= 10)
            break;  /* not legal here */
#ifdef TINY_NO_OX 
          flags &= ~(SIGNOK | NDIGITS);
#else
          flags &= ~(SIGNOK | PFXOK | NDIGITS);
#endif
          value = value * base + c - 'A' + 10;
          goto ok;          
        
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
          /* no need to fix base here */
          if (base <= 10)
            break;  /* not legal here */
#ifdef TINY_NO_OX 
          flags &= ~(SIGNOK | NDIGITS);
#else
          flags &= ~(SIGNOK | PFXOK | NDIGITS);
#endif
          value = value * base + c - 'a' + 10;
          goto ok;

        /* sign ok only as first character */
        case '-':
          if (!(flags & HAVESIGN)) {
            sign_minus = 1;
          }
          /* FALLTHROUGH */
        case '+':
          if (flags & SIGNOK) {
            flags &= ~SIGNOK;
            flags |= HAVESIGN;
            goto ok;
          }
          break;

        /*
         * x ok iff flag still set and 2nd char (or
         * 3rd char if we have a sign).
         */
#ifdef TINY_NO_OX
#else
        case 'x': case 'X':
          if ((flags & PFXOK) && p ==
              buf + 1 + !!(flags & HAVESIGN)) {
            base = 16;  /* if %i */
            flags &= ~PFXOK;
            goto ok;
          }
          break;
#endif
        }

        /*
         * If we got here, c is not a legal character
         * for a number.  Stop accumulating digits.
         */
        break;
    ok:
        /*
         * c is legal: store it and look at the next.
         */
        *p++ = c;
        if (--fp_r > 0)
          fp_p++;
        else if (__srefill(fp))
          break;    /* EOF */
      }
      /*
       * If we had only a sign, it is no good; push
       * back the sign.  If the number ends in `x',
       * it was [sign] '0' 'x', so push back the x
       * and treat it as [sign] '0'.
       */
      if (flags & NDIGITS) {
        if (p > buf)
        {
          --c;
          --p;
          ungetc(c++, fp);
          /* There is a dummy post-increment to 
             avoid an unused value warning */
        }
        goto match_failure;
      }
#ifdef TINY_NO_OX 
#else
      c = ((u_char *)p)[-1];
      if (c == 'x' || c == 'X') {
        --p;
        ungetc(c, fp);
      }
#endif
      
#ifdef TINY_SSCANF
      {
#else
      if ((flags & SUPPRESS) == 0) {
#endif

        *p = '\0';
        if (sign_minus)
          value = -value;

#ifdef TINY_SSCANF
#else
        if (flags & POINTER)
          *va_arg(ap, void **) =
              (void *)(uintptr_t)value;
        else if (flags & MAXINT)
          *va_arg(ap, intmax_t *) = value;
        else if (flags & LLONG)
          *va_arg(ap, long long *) = value;
        else if (flags & SIZEINT)
          *va_arg(ap, size_t *) = value;
        else if (flags & PTRINT)
          *va_arg(ap, ptrdiff_t *) = value;
        else 
#endif
        if (flags & LONG)
          *va_arg(ap, long *) = value;
        else if (flags & SHORT)
          *va_arg(ap, short *) = value;
        else if (flags & SHORTSHORT)
          *va_arg(ap, char *) = value;
        else
          *va_arg(ap, int *) = value;
        nassigned++;
      }
      nread += p - buf;
      break;

#ifdef FLOATING_POINT
    case CT_FLOAT:
      /* scan a floating point number as if by strtod */
#ifdef hardway
      if (width == 0 || width > sizeof(buf) - 1)
        width = sizeof(buf) - 1;
#else
      /* size_t is unsigned, hence this optimisation */
      if (--width > sizeof(buf) - 2)
        width = sizeof(buf) - 2;
      width++;
#endif
      flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
      for (p = buf; width; width--) {
        c = *fp->_p;
        /*
         * This code mimicks the integer conversion
         * code, but is much simpler.
         */
        switch (c) {

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
          flags &= ~(SIGNOK | NDIGITS);
          goto fok;

        case '+': case '-':
          if (flags & SIGNOK) {
            flags &= ~SIGNOK;
            goto fok;
          }
          break;
        case '.':
          if (flags & DPTOK) {
            flags &= ~(SIGNOK | DPTOK);
            goto fok;
          }
          break;
        case 'e': case 'E':
          /* no exponent without some digits */
          if ((flags&(NDIGITS|EXPOK)) == EXPOK) {
            flags =
                (flags & ~(EXPOK|DPTOK)) |
                SIGNOK | NDIGITS;
            goto fok;
          }
          break;
        }
        break;
    fok:
        *p++ = c;
        if (--fp->_r > 0)
          fp->_p++;
        else if (__srefill(fp))
          break;  /* EOF */
      }
      /*
       * If no digits, might be missing exponent digits
       * (just give back the exponent) or might be missing
       * regular digits, but had sign and/or decimal point.
       */
      if (flags & NDIGITS) {
        if (flags & EXPOK) {
          /* no digits at all */
          while (p > buf)
            ungetc(*(u_char *)--p, fp);
          goto match_failure;
        }
        /* just a bad exponent (e and maybe sign) */
        c = *(u_char *)--p;
        if (c != 'e' && c != 'E') {
          (void) ungetc(c, fp);/* sign */
          c = *(u_char *)--p;
        }
        (void) ungetc(c, fp);
      }
      if ((flags & SUPPRESS) == 0) {
        double res;

        *p = '\0';
        res = strtod(buf, (char **) NULL);
        if (flags & LONGDBL)
          *va_arg(ap, long double *) = res;
        else if (flags & LONG)
          *va_arg(ap, double *) = res;
        else
          *va_arg(ap, float *) = res;
        nassigned++;
      }
      nread += p - buf;
      break;
#endif /* FLOATING_POINT */
    }
  }
input_failure:
  return (nassigned ? nassigned : -1);
match_failure:
  return (nassigned);
}

#ifdef TINY_SSCANF
#else
/*
 * Fill in the given table from the scanset at the given format
 * (just after `[').  Return a pointer to the character past the
 * closing `]'.  The table has a 1 wherever characters should be
 * considered part of the scanset.
 */
static u_char *
__sccl(char *tab, u_char *fmt)
{
  int c, n, v;

  /* first `clear' the whole table */
  c = *fmt++;    /* first char hat => negated scanset */
  if (c == '^') {
    v = 1;    /* default => accept */
    c = *fmt++;  /* get new first char */
  } else
    v = 0;    /* default => reject */
  /* should probably use memset here */
  for (n = 0; n < 256; n++)
    tab[n] = v;
  if (c == 0)
    return (fmt - 1);/* format ended before closing ] */

  /*
   * Now set the entries corresponding to the actual scanset
   * to the opposite of the above.
   *
   * The first character may be ']' (or '-') without being special;
   * the last character may be '-'.
   */
  v = 1 - v;
  for (;;) {
    tab[c] = v;    /* take character c */
doswitch:
    n = *fmt++;    /* and examine the next */
    switch (n) {

    case 0:      /* format ended too soon */
      return (fmt - 1);

    case '-':
      /*
       * A scanset of the form
       *  [01+-]
       * is defined as `the digit 0, the digit 1,
       * the character +, the character -', but
       * the effect of a scanset such as
       *  [a-zA-Z0-9]
       * is implementation defined.  The V7 Unix
       * scanf treats `a-z' as `the letters a through
       * z', but treats `a-a' as `the letter a, the
       * character -, and the letter a'.
       *
       * For compatibility, the `-' is not considerd
       * to define a range if the character following
       * it is either a close bracket (required by ANSI)
       * or is not numerically greater than the character
       * we just stored in the table (c).
       */
      n = *fmt;
      if (n == ']' || n < c) {
        c = '-';
        break;  /* resume the for(;;) */
      }
      fmt++;
      do {    /* fill in the range */
        tab[++c] = v;
      } while (c < n);
#if 1  /* XXX another disgusting compatibility hack */
      /*
       * Alas, the V7 Unix scanf also treats formats
       * such as [a-c-e] as `the letters a through e'.
       * This too is permitted by the standard....
       */
      goto doswitch;
#else
      c = *fmt++;
      if (c == 0)
        return (fmt - 1);
      if (c == ']')
        return (fmt);
#endif
      break;

    case ']':    /* end of scanset */
      return (fmt);

    default:    /* just another character */
      c = n;
      break;
    }
  }
  /* NOTREACHED */
}
#endif

int
tiny_sscanf(const char *str, const char *fmt, ...)
{
  int ret;
  va_list ap;
  
  va_start(ap, fmt);
  ret = tiny_vfscanf(str, fmt, ap);
  va_end(ap);
  return (ret);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
