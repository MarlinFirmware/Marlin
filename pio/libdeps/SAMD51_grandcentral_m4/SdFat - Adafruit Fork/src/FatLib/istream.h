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
#ifndef istream_h
#define istream_h
/**
 * \file
 * \brief \ref istream class
 */
#include "ios.h"

/**
 * \class istream
 * \brief Input Stream
 */
class istream : public virtual ios {
 public:
  istream() {}
  /** call manipulator
   * \param[in] pf function to call
   * \return the stream
   */
  istream& operator>>(istream& (*pf)(istream& str)) {
    return pf(*this);
  }
  /** call manipulator
   * \param[in] pf function to call
   * \return the stream
   */
  istream& operator>>(ios_base& (*pf)(ios_base& str)) {
    pf(*this);
    return *this;
  }
  /** call manipulator
   * \param[in] pf function to call
   * \return the stream
   */
  istream& operator>>(ios& (*pf)(ios& str)) {
    pf(*this);
    return *this;
  }
  /**
   * Extract a character string
   * \param[out] str location to store the string.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(char *str) {
    getStr(str);
    return *this;
  }
  /**
   * Extract a character
   * \param[out] ch location to store the character.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(char& ch) {
    getChar(&ch);
    return *this;
  }
  /**
   * Extract a character string
   * \param[out] str location to store the string.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(signed char *str) {
    getStr(reinterpret_cast<char*>(str));
    return *this;
  }
  /**
   * Extract a character
   * \param[out] ch location to store the character.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(signed char& ch) {
    getChar(reinterpret_cast<char*>(&ch));
    return *this;
  }
  /**
   * Extract a character string
   * \param[out] str location to store the string.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(unsigned char *str) {
    getStr(reinterpret_cast<char*>(str));
    return *this;
  }
  /**
   * Extract a character
   * \param[out] ch location to store the character.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(unsigned char& ch) {
    getChar(reinterpret_cast<char*>(&ch));
    return *this;
  }
  /**
   * Extract a value of type bool.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>>(bool& arg) {
    getBool(&arg);
    return *this;
  }
  /**
   * Extract a value of type short.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(short& arg) {  // NOLINT
    getNumber(&arg);
    return *this;
  }
  /**
   * Extract a value of type unsigned short.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(unsigned short& arg) {  // NOLINT
    getNumber(&arg);
    return *this;
  }
  /**
   * Extract a value of type int.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(int& arg) {
    getNumber(&arg);
    return *this;
  }
  /**
   * Extract a value of type unsigned int.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(unsigned int& arg) {
    getNumber(&arg);
    return *this;
  }
  /**
   * Extract a value of type long.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(long& arg) {  // NOLINT
    getNumber(&arg);
    return *this;
  }
  /**
   * Extract a value of type unsigned long.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>>(unsigned long& arg) {  // NOLINT
    getNumber(&arg);
    return *this;
  }
  /**
  * Extract a value of type double.
  * \param[out] arg location to store the value.
  * \return Is always *this.  Failure is indicated by the state of *this.
  */
  istream &operator>> (double& arg) {
    getDouble(&arg);
    return *this;
  }
  /**
   * Extract a value of type float.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream &operator>> (float& arg) {
    double v;
    getDouble(&v);
    arg = v;
    return *this;
  }
  /**
   * Extract a value of type void*.
   * \param[out] arg location to store the value.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& operator>> (void*& arg) {
    uint32_t val;
    getNumber(&val);
    arg = reinterpret_cast<void*>(val);
    return *this;
  }
  /**
   * \return The number of characters extracted by the last unformatted
   * input function.
   */
  streamsize gcount() const {
    return m_gcount;
  }
  /**
   * Extract a character if one is available.
   *
   * \return The character or -1 if a failure occurs.  A failure is indicated
   * by the stream state.
   */
  int get();
  /**
   * Extract a character if one is available.
   *
   * \param[out] ch location to receive the extracted character.
   *
   * \return always returns *this. A failure is indicated by the stream state.
   */
  istream& get(char& ch);
  /**
   * Extract characters.
   *
   * \param[out] str Location to receive extracted characters.
   * \param[in] n Size of str.
   * \param[in] delim Delimiter
   *
   * Characters are extracted until extraction fails, n is less than 1,
   * n-1 characters are extracted, or the next character equals
   * \a delim (delim is not extracted). If no characters are extracted
   * failbit is set.  If end-of-file occurs the eofbit is set.
   *
   * \return always returns *this. A failure is indicated by the stream state.
   */
  istream& get(char *str, streamsize n, char delim = '\n');
  /**
   * Extract characters
   *
   * \param[out] str Location to receive extracted characters.
   * \param[in] n Size of str.
   * \param[in] delim Delimiter
   *
   * Characters are extracted until extraction fails,
   * the next character equals \a delim (delim is extracted), or n-1
   * characters are extracted.
   *
   * The failbit is set if no characters are extracted or n-1 characters
   * are extracted.  If end-of-file occurs the eofbit is set.
   *
   * \return always returns *this. A failure is indicated by the stream state.
   */
  istream& getline(char *str, streamsize n, char delim = '\n');
  /**
   * Extract characters and discard them.
   *
   * \param[in] n maximum number of characters to ignore.
   * \param[in] delim Delimiter.
   *
   * Characters are extracted until extraction fails, \a n characters
   * are extracted, or the next input character equals \a delim
   * (the delimiter is extracted).  If end-of-file occurs the eofbit is set.
   *
   * Failures are indicated by the state of the stream.
   *
   * \return *this
   *
   */
  istream& ignore(streamsize n = 1, int delim = -1);
  /**
   * Return the next available character without consuming it.
   *
   * \return The character if the stream state is good else -1;
   *
   */
  int peek();
// istream& read(char *str, streamsize count);
// streamsize readsome(char *str, streamsize count);
  /**
   * \return the stream position
   */
  pos_type tellg() {
    return tellpos();
  }
  /**
   * Set the stream position
   * \param[in] pos The absolute position in which to move the read pointer.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& seekg(pos_type pos) {
    if (!seekpos(pos)) {
      setstate(failbit);
    }
    return *this;
  }
  /**
   * Set the stream position.
   *
   * \param[in] off An offset to move the read pointer relative to way.
   * \a off is a signed 32-bit int so the offset is limited to +- 2GB.
   * \param[in] way One of ios::beg, ios::cur, or ios::end.
   * \return Is always *this.  Failure is indicated by the state of *this.
   */
  istream& seekg(off_type off, seekdir way) {
    if (!seekoff(off, way)) {
      setstate(failbit);
    }
    return *this;
  }
  void skipWhite();

 protected:
  /// @cond SHOW_PROTECTED
  /**
  * Internal - do not use
  * \return
  */
  virtual int16_t getch() = 0;
  /**
   * Internal - do not use
   * \param[out] pos
   * \return
   */
  int16_t getch(FatPos_t* pos) {
    getpos(pos);
    return getch();
  }
  /**
   * Internal - do not use
   * \param[out] pos
   */
  virtual void getpos(FatPos_t* pos) = 0;
  /**
   * Internal - do not use
   * \param[in] pos
   */
  virtual bool seekoff(off_type off, seekdir way) = 0;
  virtual bool seekpos(pos_type pos) = 0;
  virtual void setpos(FatPos_t* pos) = 0;
  virtual pos_type tellpos() = 0;

  /// @endcond
 private:
  void getBool(bool *b);
  void getChar(char* ch);
  bool getDouble(double* value);
  template <typename T>  void getNumber(T* value);
  bool getNumber(uint32_t posMax, uint32_t negMax, uint32_t* num);
  void getStr(char *str);
  int16_t readSkip();

  size_t m_gcount;
};
//------------------------------------------------------------------------------
template <typename T>
void istream::getNumber(T* value) {
  uint32_t tmp;
  if ((T)-1 < 0) {
    // number is signed, max positive value
    uint32_t const m = ((uint32_t)-1) >> (33 - sizeof(T) * 8);
    // max absolute value of negative number is m + 1.
    if (getNumber(m, m + 1, &tmp)) {
      *value = (T)tmp;
    }
  } else {
    // max unsigned value for T
    uint32_t const m = (T)-1;
    if (getNumber(m, m, &tmp)) {
      *value = (T)tmp;
    }
  }
}
#endif  // istream_h
