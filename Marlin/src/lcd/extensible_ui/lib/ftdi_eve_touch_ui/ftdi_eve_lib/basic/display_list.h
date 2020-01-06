/******************
 * display_list.h *
 *****************/

/**********************************************************************************
 * Adapted from:                                                                  *
 *     https://github.com/RudolphRiedel/FT800-FT813                               *
 *     By Rudolph Riedel                                                          *
 *                                                                                *
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2017                                                             *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 *                                                                                *
 **********************************************************************************/

#pragma once

namespace FTDI {
  /* FT8xx graphics engine specific macros useful for static display list generation */
  inline uint32_t ALPHA_FUNC(uint8_t func, uint8_t ref)        {return DL::ALPHA_FUNC|((func&7UL)<<8)|(ref&255UL);}
  inline uint32_t BEGIN(begin_t prim)                          {return DL::BEGIN|(prim&15UL);}

  inline uint32_t BITMAP_SOURCE(uint32_t ram_g_addr)           {return DL::BITMAP_SOURCE|(ram_g_addr);}
  inline uint32_t BITMAP_HANDLE(uint8_t handle)                {return DL::BITMAP_HANDLE|(handle&31UL);}
  inline uint32_t BITMAP_LAYOUT(uint8_t format, uint16_t linestride, uint16_t height)
                                                               {return DL::BITMAP_LAYOUT|((format&31UL)<<19)|((linestride&1023UL)<<9)|(height&511UL);}

  inline uint32_t BITMAP_SIZE(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
                                                               {return DL::BITMAP_SIZE|((filter&1UL)<<20)|((wrapx&1UL)<<19)|((wrapy&1UL)<<18)|((width&511UL)<<9)|(height&511UL);}
  #if FTDI_API_LEVEL >= 810
  inline uint32_t BITMAP_LAYOUT_H(uint8_t linestride, uint8_t height)
                                                               {return DL::BITMAP_LAYOUT_H|((linestride&3UL)<<2)|(height&3UL);}
  inline uint32_t BITMAP_SIZE_H(uint8_t width, uint8_t height)
                                                               {return DL::BITMAP_SIZE_H|((width&3UL)<<2)|(height&3UL);}
  #endif
  inline uint32_t BITMAP_TRANSFORM_A(uint16_t a)               {return DL::BITMAP_TRANSFORM_A|(a&131071UL);}
  inline uint32_t BITMAP_TRANSFORM_B(uint16_t b)               {return DL::BITMAP_TRANSFORM_B|(b&131071UL);}
  inline uint32_t BITMAP_TRANSFORM_C(uint32_t c)               {return DL::BITMAP_TRANSFORM_C|(c&16777215UL);}
  inline uint32_t BITMAP_TRANSFORM_D(uint16_t d)               {return DL::BITMAP_TRANSFORM_D|(d&131071UL);}
  inline uint32_t BITMAP_TRANSFORM_E(uint16_t e)               {return DL::BITMAP_TRANSFORM_E|(e&131071UL);}
  inline uint32_t BITMAP_TRANSFORM_F(uint32_t f)               {return DL::BITMAP_TRANSFORM_F|(f&16777215UL);}
  inline uint32_t BLEND_FUNC(uint8_t src,uint8_t dst)          {return DL::BLEND_FUNC|((src&7UL)<<3)|(dst&7UL);}
  inline uint32_t CALL(uint16_t dest)                          {return DL::CALL|(dest&65535UL);}
  inline uint32_t CELL(uint8_t cell)                           {return DL::CELL|(cell&127UL);}
  inline uint32_t CLEAR(bool c,bool s,bool t)                  {return DL::CLEAR|((c?1UL:0UL)<<2)|((s?1UL:0UL)<<1)|(t?1UL:0UL);}
  inline uint32_t CLEAR_COLOR_A(uint8_t alpha)                 {return DL::CLEAR_COLOR_A|(alpha&255UL);}
  inline uint32_t CLEAR_COLOR_RGB(uint8_t red, uint8_t green, uint8_t blue)
                                                               {return DL::CLEAR_COLOR_RGB|((red&255UL)<<16)|((green&255UL)<<8)|(blue&255UL);}
  inline uint32_t CLEAR_COLOR_RGB(uint32_t rgb)                {return DL::CLEAR_COLOR_RGB|rgb;}
  inline uint32_t CLEAR_STENCIL(uint8_t s)                     {return DL::CLEAR_STENCIL|(s&255UL);}
  inline uint32_t CLEAR_TAG(uint8_t s)                         {return DL::CLEAR_TAG|(s&255UL);}
  inline uint32_t COLOR_A(uint8_t alpha)                       {return DL::COLOR_A|(alpha&255UL);}
  inline uint32_t COLOR_MASK(bool r, bool g, bool b, bool a)   {return DL::COLOR_MASK|((r?1UL:0UL)<<3)|((g?1UL:0UL)<<2)|((b?1UL:0UL)<<1)|(a?1UL:0UL);}
  inline uint32_t COLOR_RGB(uint8_t red,uint8_t green,uint8_t blue)
                                                               {return DL::COLOR_RGB|((red&255UL)<<16)|((green&255UL)<<8)|(blue&255UL);}
  inline uint32_t COLOR_RGB(uint32_t rgb)                      {return DL::COLOR_RGB|rgb;}
  /* inline uint32_t DISPLAY()                                 {return (0UL<<24)) */
  inline uint32_t END()                                        {return DL::END;}
  inline uint32_t JUMP(uint16_t dest)                          {return DL::JUMP|(dest&65535UL);}
  inline uint32_t LINE_WIDTH(uint16_t width)                   {return DL::LINE_WIDTH|(width&4095UL);}
  inline uint32_t MACRO(uint8_t m)                             {return DL::MACRO|(m&1UL);}
  inline uint32_t POINT_SIZE(uint16_t size)                    {return DL::POINT_SIZE|(size&8191UL);}
  inline uint32_t RESTORE_CONTEXT()                            {return DL::RESTORE_CONTEXT;}
  inline uint32_t RETURN ()                                    {return DL::RETURN;}
  inline uint32_t SAVE_CONTEXT()                               {return DL::SAVE_CONTEXT;}
  inline uint32_t SCISSOR_XY(uint16_t x,uint16_t y) {
    return DL::SCISSOR_XY |
      (FTDI::ftdi_chip >= 810
        ? ((x&2047UL)<<11)|(y&2047UL)
        : ((x& 511UL)<<10)|(y&511UL));
  }
  inline uint32_t SCISSOR_SIZE(uint16_t w,uint16_t h) {
    return DL::SCISSOR_SIZE |
      (FTDI::ftdi_chip >= 810
        ? ((w&4095UL)<<12)|(h&4095UL)
        : ((w&1023UL)<<10)|(h&1023UL));
  }
  inline uint32_t SCISSOR_XY()                                 {return DL::SCISSOR_XY;}
  inline uint32_t SCISSOR_SIZE() {
    return DL::SCISSOR_SIZE |
      (FTDI::ftdi_chip >= 810
        ? (2048UL<<12)|(2048UL)
        : ( 512UL<<10)|( 512UL));
  }
  inline uint32_t STENCIL_FUNC(uint16_t func, uint8_t ref, uint8_t mask)
                                                               {return DL::STENCIL_FUNC|((func&7UL)<<16)|((ref&255UL)<<8)|(mask&255UL);}
  inline uint32_t STENCIL_MASK(uint8_t mask)                   {return DL::STENCIL_MASK|(mask&255UL);}
  inline uint32_t STENCIL_OP(uint8_t sfail, uint8_t spass)     {return DL::STENCIL_OP|(((sfail)&7UL)<<3)|(spass&7UL);}
  inline uint32_t TAG(uint8_t s)                               {return DL::TAG|(s&255UL);}
  inline uint32_t TAG_MASK(bool mask)                          {return DL::TAG_MASK|(mask?1:0);}
  inline uint32_t VERTEX2F(uint16_t x, uint16_t y)             {return DL::VERTEX2F|((x&32767UL)<<15)|(y&32767UL);}
  inline uint32_t VERTEX2II(uint16_t x,uint16_t y, uint8_t handle = 0, uint8_t cell = 0)
                                                               {return DL::VERTEX2II|((x&511UL)<<21)|((y&511UL)<<12)|((handle&31UL)<<7)|(cell&127UL);}

  #if FTDI_API_LEVEL >= 810
  inline uint32_t VERTEX_FORMAT(uint8_t frac)                  {return DL::VERTEX_FORMAT|(frac&7UL);}
  inline uint32_t VERTEX_TRANSLATE_X(int32_t x)                {return DL::VERTEX_TRANSLATE_X|(x&131071UL);}
  inline uint32_t VERTEX_TRANSLATE_Y(int32_t y)                {return DL::VERTEX_TRANSLATE_Y|(y&131071UL);}
  #endif
}
