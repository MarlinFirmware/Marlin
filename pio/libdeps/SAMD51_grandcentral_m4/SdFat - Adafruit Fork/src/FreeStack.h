/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef FreeStack_h
#define FreeStack_h
/**
 * \file
 * \brief FreeStack() function.
 */
#if defined(__AVR__) || defined(DOXYGEN)
/** boundary between stack and heap. */
extern char *__brkval;
/** End of bss section.*/
extern char __bss_end;
/** Amount of free stack space.
 * \return The number of free bytes.
 */
static int FreeStack() {
  char* sp = reinterpret_cast<char*>(SP);
  return __brkval ? sp - __brkval : sp - &__bss_end;
//  char top;
//  return __brkval ? &top - __brkval : &top - &__bss_end;
}
#elif defined(PLATFORM_ID)  // Particle board
static int FreeStack() {
  return System.freeMemory();
}
#elif defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else
#warning FreeStack is not defined for this system.
static int FreeStack() {
  return 0;
}
#endif
#endif  // FreeStack_h
