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
#ifndef ArduinoStream_h
#define ArduinoStream_h
/**
 * \file
 * \brief ArduinoInStream and ArduinoOutStream classes
 */
#include "FatLibConfig.h"
#if ENABLE_ARDUINO_FEATURES
#include "bufstream.h"
//==============================================================================
/**
 * \class ArduinoInStream
 * \brief Input stream for Arduino Stream objects
 */
class ArduinoInStream : public ibufstream {
 public:
  /**
   * Constructor
   * \param[in] hws hardware stream
   * \param[in] buf buffer for input line
   * \param[in] size size of input buffer
   */
  ArduinoInStream(Stream &hws, char* buf, size_t size) {
    m_hw = &hws;
    m_line = buf;
    m_size = size;
  }
  /** read a line. */
  void readline() {
    size_t i = 0;
    uint32_t t;
    m_line[0] = '\0';
    while (!m_hw->available()) {
      yield();
    }

    while (1) {
      t = millis();
      while (!m_hw->available()) {
        if ((millis() - t) > 10) {
          goto done;
        }
      }
      if (i >= (m_size - 1)) {
        setstate(failbit);
        return;
      }
      m_line[i++] = m_hw->read();
      m_line[i] = '\0';
    }
done:
    init(m_line);
  }

 protected:
  /** Internal - do not use.
   * \param[in] off
   * \param[in] way
   * \return true/false.
   */
  bool seekoff(off_type off, seekdir way) {
    (void)off;
    (void)way;
    return false;
  }
  /** Internal - do not use.
   * \param[in] pos
   * \return true/false.
   */
  bool seekpos(pos_type pos) {
    (void)pos;
    return false;
  }

 private:
  char *m_line;
  size_t m_size;
  Stream* m_hw;
};
//==============================================================================
/**
 * \class ArduinoOutStream
 * \brief Output stream for Arduino Print objects
 */
class ArduinoOutStream : public ostream {
 public:
  /** constructor
   *
   * \param[in] pr Print object for this ArduinoOutStream.
   */
  explicit ArduinoOutStream(Print& pr) : m_pr(&pr) {}

 protected:
  /// @cond SHOW_PROTECTED
  /**
   * Internal do not use
   * \param[in] c
   */
  void putch(char c) {
    if (c == '\n') {
      m_pr->write('\r');
    }
    m_pr->write(c);
  }
  void putstr(const char* str) {
    m_pr->write(str);
  }
  bool seekoff(off_type off, seekdir way) {
    (void)off;
    (void)way;
    return false;
  }
  bool seekpos(pos_type pos) {
    (void)pos;
    return false;
  }
  bool sync() {
    return true;
  }
  pos_type tellpos() {
    return 0;
  }
  /// @endcond
 private:
  ArduinoOutStream() {}
  Print* m_pr;
};
#endif  // ENABLE_ARDUINO_FEATURES
#endif  // ArduinoStream_h
