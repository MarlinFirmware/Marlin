/*

  u8g_clip.c
  
  procedures for clipping
  taken over from procs in u8g_pb.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  Notes
  
  This is one of the most critical parts of u8glib. It must be fast, but still reliable.
  Based on the intersection program (see tools folder), there is minimized version of
  the condition for the intersaction test:
    minimized version
    ---1----0 1             b1 <= a2 && b1 > b2
    -----1--0 1             b2 >= a1 && b1 > b2
    ---1-1--- 1             b1 <= a2 && b2 >= a1
  It includes the assumption, that a1 <= a2 is always true (correct, because
  a1, a2 are the page dimensions.

  The direct implementation of the above result is done in:
  uint8_t u8g_is_intersection_boolean(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1)
  However, this is slower than a decision tree version:  
  static uint8_t u8g_is_intersection_decision_tree(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1) 
  Also suprising is, that the macro implementation is slower than the inlined version.
  
  The decision tree is based on the expansion of the truth table.
  
*/

#include "u8g.h"

#ifdef __GNUC__
#define U8G_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#else
#define U8G_ALWAYS_INLINE
#endif 

/*
  intersection assumptions:
    a1 <= a2 is always true    
    
    minimized version
    ---1----0 1             b1 <= a2 && b1 > b2
    -----1--0 1             b2 >= a1 && b1 > b2
    ---1-1--- 1             b1 <= a2 && b2 >= a1
  */

#ifdef OLD_CODE_WHICH_IS_TOO_SLOW
static uint8_t u8g_is_intersection_boolean(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1)
{
  uint8_t c1, c2, c3, tmp;
  c1 = v0 <= a1;
  c2 = v1 >= a0;
  c3 = v0 > v1;
  
  tmp = c1;
  c1 &= c2;
  c2 &= c3;
  c3 &= tmp;
  c1 |= c2;
  c1 |= c3;
  return c1 & 1;
}
#endif

#define U8G_IS_INTERSECTION_MACRO(a0,a1,v0,v1) ((uint8_t)( (v0) <= (a1) ) ? ( ( (v1) >= (a0) ) ? ( 1 ) : ( (v0) > (v1) ) ) : ( ( (v1) >= (a0) ) ? ( (v0) > (v1) ) : ( 0 ) ))

//static uint8_t u8g_is_intersection_decision_tree(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1) U8G_ALWAYS_INLINE;
static uint8_t U8G_ALWAYS_INLINE u8g_is_intersection_decision_tree(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1) 
{
  /* surprisingly the macro leads to larger code */
  /* return U8G_IS_INTERSECTION_MACRO(a0,a1,v0,v1); */
  if ( v0 <= a1 )
  {
    if ( v1 >= a0 )
    {
      return 1;
    }
    else
    {
      if ( v0 > v1 )
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
  }
  else
  {
    if ( v1 >= a0 )
    {
      if ( v0 > v1 )
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
    else
    {
      return 0;
    }
  }
}


uint8_t u8g_IsBBXIntersection(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
  register u8g_uint_t tmp;
  tmp = y;
  tmp += h;
  tmp--;
  if ( u8g_is_intersection_decision_tree(u8g->current_page.y0, u8g->current_page.y1, y, tmp) == 0 )
    return 0; 
  
  tmp = x;
  tmp += w;
  tmp--;
  return u8g_is_intersection_decision_tree(u8g->current_page.x0, u8g->current_page.x1, x, tmp);
}


