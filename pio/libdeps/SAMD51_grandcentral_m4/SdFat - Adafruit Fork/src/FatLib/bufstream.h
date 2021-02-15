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
#ifndef bufstream_h
#define bufstream_h
/**
 * \file
 * \brief \ref ibufstream and \ref obufstream classes
 */
#include <string.h>
#include "iostream.h"
//==============================================================================
/**
 * \class ibufstream
 * \brief parse a char string
 */
class ibufstream : public istream {
 public:
  /** Constructor */
  ibufstream() : m_buf(0), m_len(0) {}
  /** Constructor
   * \param[in] str pointer to string to be parsed
   * Warning: The string will not be copied so must stay in scope.
   */
  explicit ibufstream(const char* str) {
    init(str);
  }
  /** Initialize an ibufstream
   * \param[in] str pointer to string to be parsed
   * Warning: The string will not be copied so must stay in scope.
   */
  void init(const char* str) {
    m_buf = str;
    m_len = strlen(m_buf);
    m_pos = 0;
    clear();
  }

 protected:
  /// @cond SHOW_PROTECTED
  int16_t getch() {
    if (m_pos < m_len) {
      return m_buf[m_pos++];
    }
    setstate(eofbit);
    return -1;
  }
  void getpos(FatPos_t *pos) {
    pos->position = m_pos;
  }
  bool seekoff(off_type off, seekdir way) {
    (void)off;
    (void)way;
    return false;
  }
  bool seekpos(pos_type pos) {
    if (pos < m_len) {
      m_pos = pos;
      return true;
    }
    return false;
  }
  void setpos(FatPos_t *pos) {
    m_pos = pos->position;
  }
  pos_type tellpos() {
    return m_pos;
  }
  /// @endcond
 private:
  const char* m_buf;
  size_t m_len;
  size_t m_pos;
};
//==============================================================================
/**
 * \class obufstream
 * \brief format a char string
 */
class obufstream : public ostream {
 public:
  /** constructor */
  obufstream() : m_in(0) {}
  /** Constructor
   * \param[in] buf buffer for formatted string
   * \param[in] size buffer size
   */
  obufstream(char *buf, size_t size) {
    init(buf, size);
  }
  /** Initialize an obufstream
   * \param[in] buf buffer for formatted string
   * \param[in] size buffer size
   */
  void init(char *buf, size_t size) {
    m_buf = buf;
    buf[0] = '\0';
    m_size = size;
    m_in = 0;
  }
  /** \return a pointer to the buffer */
  char* buf() {
    return m_buf;
  }
  /** \return the length of the formatted string */
  size_t length() {
    return m_in;
  }

 protected:
  /// @cond SHOW_PROTECTED
  void putch(char c) {
    if (m_in >= (m_size - 1)) {
      setstate(badbit);
      return;
    }
    m_buf[m_in++] = c;
    m_buf[m_in] = '\0';
  }
  void putstr(const char *str) {
    while (*str) {
      putch(*str++);
    }
  }
  bool seekoff(off_type off, seekdir way) {
    (void)off;
    (void)way;
    return false;
  }
  bool seekpos(pos_type pos) {
    if (pos > m_in) {
      return false;
    }
    m_in = pos;
    m_buf[m_in] = '\0';
    return true;
  }
  bool sync() {
    return true;
  }

  pos_type tellpos() {
    return m_in;
  }
  /// @endcond
 private:
  char *m_buf;
  size_t m_size;
  size_t m_in;
};
#endif  // bufstream_h
