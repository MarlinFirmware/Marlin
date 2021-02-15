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
#ifndef iostream_h
#define iostream_h
/**
 * \file
 * \brief \ref iostream class
 */
#include "istream.h"
#include "ostream.h"
/** Skip white space
 * \param[in] is the Stream
 * \return The stream
 */
inline istream& ws(istream& is) {
  is.skipWhite();
  return is;
}
/** insert endline
 * \param[in] os The Stream
 * \return The stream
 */
inline ostream& endl(ostream& os) {
  os.put('\n');
#if ENDL_CALLS_FLUSH
  os.flush();
#endif  // ENDL_CALLS_FLUSH
  return os;
}
/** flush manipulator
 * \param[in] os The stream
 * \return The stream
 */
inline ostream& flush(ostream& os) {
  os.flush();
  return os;
}
/**
 * \struct setfill
 * \brief type for setfill manipulator
 */
struct setfill {
  /** fill character */
  char c;
  /** constructor
   *
   * \param[in] arg new fill character
   */
  explicit setfill(char arg) : c(arg) {}
};
/** setfill manipulator
 * \param[in] os the stream
 * \param[in] arg set setfill object
 * \return the stream
 */
inline ostream &operator<< (ostream &os, const setfill &arg) {
  os.fill(arg.c);
  return os;
}
/** setfill manipulator
 * \param[in] obj the stream
 * \param[in] arg set setfill object
 * \return the stream
 */
inline istream &operator>>(istream &obj, const setfill &arg) {
  obj.fill(arg.c);
  return obj;
}
//------------------------------------------------------------------------------
/** \struct setprecision
 * \brief type for setprecision manipulator
 */
struct setprecision {
  /** precision */
  unsigned int p;
  /** constructor
   * \param[in] arg new precision
   */
  explicit setprecision(unsigned int arg) : p(arg) {}
};
/** setprecision manipulator
 * \param[in] os the stream
 * \param[in] arg set setprecision object
 * \return the stream
 */
inline ostream &operator<< (ostream &os, const setprecision &arg) {
  os.precision(arg.p);
  return os;
}
/** setprecision manipulator
 * \param[in] is the stream
 * \param[in] arg set setprecision object
 * \return the stream
 */
inline istream &operator>>(istream &is, const setprecision &arg) {
  is.precision(arg.p);
  return is;
}
//------------------------------------------------------------------------------
/** \struct setw
 * \brief type for setw manipulator
 */
struct setw {
  /** width */
  unsigned w;
  /** constructor
   * \param[in] arg new width
   */
  explicit setw(unsigned arg) : w(arg) {}
};
/** setw manipulator
 * \param[in] os the stream
 * \param[in] arg set setw object
 * \return the stream
 */
inline ostream &operator<< (ostream &os, const setw &arg) {
  os.width(arg.w);
  return os;
}
/** setw manipulator
 * \param[in] is the stream
 * \param[in] arg set setw object
 * \return the stream
 */
inline istream &operator>>(istream &is, const setw &arg) {
  is.width(arg.w);
  return is;
}
//==============================================================================
/**
 * \class iostream
 * \brief Input/Output stream
 */
class iostream : public istream, public ostream {
};
#endif  // iostream_h
