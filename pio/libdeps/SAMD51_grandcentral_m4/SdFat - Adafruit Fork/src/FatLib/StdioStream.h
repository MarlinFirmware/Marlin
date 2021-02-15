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
#ifndef StdioStream_h
#define StdioStream_h
/**
 * \file
 * \brief StdioStream class
 */
#include <limits.h>
#include "FatFile.h"
//------------------------------------------------------------------------------
/** Total size of stream buffer. The entire buffer is used for output.
  * During input UNGETC_BUF_SIZE of this space is reserved for ungetc.
  */
const uint8_t STREAM_BUF_SIZE = 64;
/** Amount of buffer allocated for ungetc during input. */
const uint8_t UNGETC_BUF_SIZE = 2;
//------------------------------------------------------------------------------
// Get rid of any macros defined in <stdio.h>.
#include <stdio.h>
#undef clearerr
#undef fclose
#undef feof
#undef ferror
#undef fflush
#undef fgetc
#undef fgetpos
#undef fgets
#undef fopen
#undef fprintf
#undef fputc
#undef fputs
#undef fread
#undef freopen
#undef fscanf
#undef fseek
#undef fsetpos
#undef ftell
#undef fwrite
#undef getc
#undef getchar
#undef gets
#undef perror
//#undef printf  // NOLINT
#undef putc
#undef putchar
#undef puts
#undef remove
#undef rename
#undef rewind
#undef scanf
#undef setbuf
#undef setvbuf
//#undef sprintf  // NOLINT
#undef sscanf
#undef tmpfile
#undef tmpnam
#undef ungetc
#undef vfprintf
#undef vprintf
#undef vsprintf

// make sure needed macros are defined
#ifndef EOF
/** End-of-file return value. */
#define EOF (-1)
#endif  // EOF
#ifndef NULL
/** Null pointer */
#define NULL 0
#endif  // NULL
#ifndef SEEK_CUR
/** Seek relative to current position. */
#define SEEK_CUR 1
#endif  // SEEK_CUR
#ifndef  SEEK_END
/** Seek relative to end-of-file. */
#define SEEK_END 2
#endif  // SEEK_END
#ifndef SEEK_SET
/** Seek relative to start-of-file. */
#define SEEK_SET 0
#endif  // SEEK_SET
//------------------------------------------------------------------------------
/** \class StdioStream
 * \brief StdioStream implements a minimal stdio stream.
 *
 * StdioStream does not support subdirectories or long file names.
 */
class StdioStream : private FatFile {
 public:
  /** Constructor
   *
   */
  StdioStream() {
    m_w = m_r = 0;
    m_p = m_buf;
    m_status = 0;
  }
  //----------------------------------------------------------------------------
  /** Clear the stream's end-of-file and error indicators. */
  void clearerr() {
    m_status &= ~(S_ERR | S_EOF);
  }
  //----------------------------------------------------------------------------
  /** Close a stream.
   *
   * A successful call to the fclose function causes the stream  to be
   * flushed and the associated file to be closed. Any unwritten buffered
   * data is written to the file; any unread buffered data is discarded.
   * Whether or not the call succeeds, the stream is disassociated from
   * the file.
   *
   * \return zero if the stream was successfully closed, or EOF if any any
   * errors are detected.
   */
  int fclose();
  //----------------------------------------------------------------------------
  /** Test the stream's end-of-file indicator.
   * \return non-zero if and only if the end-of-file indicator is set.
   */
  int feof() {
    return (m_status & S_EOF) != 0;
  }
  //----------------------------------------------------------------------------
  /** Test the stream's error indicator.
   * \return return non-zero if and only if the error indicator is set.
   */
  int ferror() {
    return (m_status & S_ERR) != 0;
  }
  //----------------------------------------------------------------------------
  /** Flush the stream.
   *
   * If stream is an output stream or an update stream in which the most
   * recent operation was not input, any unwritten data is written to the
   * file; otherwise the call is an error since any buffered input data
   * would be lost.
   *
   * \return sets the error indicator for the stream and returns EOF if an
   * error occurs, otherwise it returns zero.
   */
  int fflush();
  //----------------------------------------------------------------------------
  /** Get a byte from the stream.
   *
   * \return If the end-of-file indicator for the stream is set, or if the
   * stream is at end-of-file, the end-of-file indicator for the stream is
   * set and the fgetc function returns EOF. Otherwise, the fgetc function
   * returns the next character from the input stream.
   */
  int fgetc() {
    return m_r-- == 0 ? fillGet() : *m_p++;
  }
  //----------------------------------------------------------------------------
  /** Get a string from a stream.
   *
   * The fgets function reads at most one less than the number of
   * characters specified by num from the stream into the array pointed
   * to by str. No additional characters are read after a new-line
   * character (which is retained) or after end-of-file. A null character
   * is written immediately after the last character read into the array.
   *
   * \param[out] str Pointer to an array of where the string is copied.
   *
   * \param[in] num Maximum number of characters including the null
   * character.
   *
   * \param[out] len If len is not null and fgets is successful, the
   * length of the string is returned.
   *
   * \return str if successful. If end-of-file is encountered and no
   * characters have been read into the array, the contents of the array
   * remain unchanged and a null pointer is returned. If a read error
   * occurs during the operation, the array contents are indeterminate
   * and a null pointer is returned.
   */
  char* fgets(char* str, size_t num, size_t* len = 0);
  //----------------------------------------------------------------------------
  /** Open a stream.
   *
   * Open a file and associates the stream with it.
   *
   * \param[in] path file to be opened.
   *
   * \param[in] mode a string that indicates the open mode.
   *
   * <table>
   * <tr>
   * <td>"r" or "rb"</td>
   * <td>Open a file for reading. The file must exist.</td>
   * </tr>
   * <tr>
   * <td>"w" or "wb"</td>
   * <td>Truncate an existing to zero length or create an empty file
   * for writing.</td>
   * </tr>
   * <tr>
   * <td>"wx" or "wbx"</td>
   * <td>Create a file for writing. Fails if the file already exists.</td>
   * </tr>
   * <tr>
   * <td>"a" or "ab"</td>
   * <td>Append; open or create file for writing at end-of-file.</td>
   * </tr>
   * <tr>
   * <td>"r+" or "rb+" or "r+b"</td>
   * <td>Open a file for update (reading and writing).</td>
   * </tr>
   * <tr>
   * <td>"w+" or "w+b" or "wb+"</td>
   * <td>Truncate an existing to zero length or create a file for update.</td>
   * </tr>
   * <tr>
   * <td>"w+x" or "w+bx" or "wb+x"</td>
   * <td>Create a file for update. Fails if the file already exists.</td>
   * </tr>
   * <tr>
   * <td>"a+" or "a+b" or "ab+"</td>
   * <td>Append; open or create a file for update, writing at end-of-file.</td>
   * </tr>
   * </table>
   * The character 'b' shall have no effect, but is allowed for ISO C
   * standard conformance.
   *
   * Opening a file with append mode causes all subsequent writes to the
   * file to be forced to the then current end-of-file, regardless of
   * intervening calls to the fseek function.
   *
   * When a file is opened with update mode, both input and output may be
   * performed on the associated stream. However, output shall not be
   * directly followed by input without an intervening call to the fflush
   * function or to a file positioning function (fseek, or rewind), and
   * input shall not be directly followed by output without an intervening
   * call to a file positioning function, unless the input operation
   * encounters end-of-file.
   *
   * \return true for success or false for failure.
   */
  bool fopen(const char* path, const char * mode);
  //----------------------------------------------------------------------------
  /** Write a byte to a stream.
   *
   * \param[in] c the byte to be written  (converted to an unsigned char).
   *
   * \return Upon successful completion, fputc() returns the value it
   * has written. Otherwise, it returns EOF and sets the error indicator for
   * the stream.
   */
  int fputc(int c) {
    return m_w-- == 0 ? flushPut(c) : *m_p++ = c;
  }
  //----------------------------------------------------------------------------
  /** Write a string to a stream.
   *
   * \param[in] str a pointer to the string to be written.
   *
   * \return for success, fputs() returns a non-negative
   * number. Otherwise, it returns EOF and sets the error indicator for
   * the stream.
   */
  int fputs(const char* str);
  //----------------------------------------------------------------------------
  /** Binary input.
   *
   * Reads an array of up to count elements, each one with a size of size
   * bytes.
   * \param[out] ptr pointer to area of at least (size*count) bytes where
   * the data will be stored.
   *
   * \param[in] size the size, in bytes, of each element to be read.
   *
   * \param[in] count the number of elements to be read.
   *
   * \return number of elements successfully read, which may be less than
   * count if a read error or end-of-file is encountered. If size or count
   * is zero, fread returns zero and the contents of the array and the
   * state of the stream remain unchanged.
   */
  size_t fread(void* ptr, size_t size, size_t count);
  //----------------------------------------------------------------------------
  /** Set the file position for the stream.
   *
   * \param[in] offset number of offset from the origin.
   *
   * \param[in] origin position used as reference for the offset. It is
   * specified by one of the following constants.
   *
   * SEEK_SET	- Beginning of file.
   *
   * SEEK_CUR	- Current position of the file pointer.
   *
   * SEEK_END	- End of file.
   *
   * \return zero for success. Otherwise, it returns non-zero and sets the
   * error indicator for the stream.
   */
  int fseek(int32_t offset, int origin);
  //----------------------------------------------------------------------------
  /** Get the current position in a stream.
   *
   * \return If successful, ftell return the current value of the position
   * indicator. On failure, ftell returns −1L.
   */
  int32_t ftell();
  //----------------------------------------------------------------------------
  /** Binary output.
   *
   * Writes an array of up to count elements, each one with a size of size
   * bytes.
   * \param[in] ptr pointer to (size*count) bytes of data to be written.
   *
   * \param[in] size the size, in bytes, of each element to be written.
   *
   * \param[in] count the number of elements to be written.
   *
   * \return number of elements successfully written. if this number is
   * less than count, an error has occurred.  If size or count is zero,
   * fwrite returns zero.
   */
  size_t fwrite(const void * ptr, size_t size, size_t count);
  //----------------------------------------------------------------------------
  /** Get a byte from the stream.
   *
   * getc and fgetc are equivalent but getc is in-line so it is faster but
   * require more flash memory.
   *
   * \return If the end-of-file indicator for the stream is set, or if the
   * stream is at end-of-file, the end-of-file indicator for the stream is
   * set and the fgetc function returns EOF. Otherwise, the fgetc function
   * returns the next character from the input stream.
   */
  inline __attribute__((always_inline))
  int getc() {
    return m_r-- == 0 ? fillGet() : *m_p++;
  }
  //----------------------------------------------------------------------------
  /** Write a byte to a stream.
   *
   * putc and fputc are equivalent but putc is in-line so it is faster but
   * require more flash memory.
   *
   * \param[in] c the byte to be written  (converted to an unsigned char).
   *
   * \return Upon successful completion, fputc() returns the value it
   * has written. Otherwise, it returns EOF and sets the error indicator for
   * the stream.
   */
  inline __attribute__((always_inline))
  int putc(int c) {
    return m_w-- == 0 ? flushPut(c) : *m_p++ = c;
  }
  //----------------------------------------------------------------------------
  /** Write a CR/LF.
   *
   * \return two, the number of bytes written, for success or -1 for failure.
   */
  inline __attribute__((always_inline))
  int putCRLF() {
    if (m_w < 2) {
      if (!flushBuf()) {
        return -1;
      }
    }
    *m_p++ = '\r';
    *m_p++ = '\n';
    m_w -= 2;
    return 2;
  }
  //----------------------------------------------------------------------------
  /** Write a character.
   * \param[in] c the character to write.
   * \return the number of bytes written.
   */
  size_t print(char c) {
    return putc(c) < 0 ? 0 : 1;
  }
  //----------------------------------------------------------------------------
  /** Write a string.
   *
   * \param[in] str the string to be written.
   *
   * \return the number of bytes written.
   */
  size_t print(const char* str) {
    int n = fputs(str);
    return n < 0 ? 0 : n;
  }
  //----------------------------------------------------------------------------
#if (defined(ARDUINO) && ENABLE_ARDUINO_FEATURES) || defined(DOXYGEN)
  /** Print a string stored in flash memory.
   *
   * \param[in] str the string to print.
   *
   * \return the number of bytes written.
   */
  size_t print(const __FlashStringHelper *str);
#endif  // (defined(ARDUINO) && ENABLE_ARDUINO_FEATURES) || defined(DOXYGEN)
  //----------------------------------------------------------------------------
  /** Print a floating point number.
   *
   * \param[in] prec Number of digits after decimal point.
   *
   * \param[in] val the number to be printed.
   *
   * \return the number of bytes written.
   */
  size_t print(double val, uint8_t prec = 2) {
    return print(static_cast<float>(val), prec);
  }
  //----------------------------------------------------------------------------
  /** Print a floating point number.
   *
   * \param[in] prec Number of digits after decimal point.
   *
   * \param[in] val the number to be printed.
   *
   * \return the number of bytes written.
   */
  size_t print(float val, uint8_t prec = 2) {
    int n = printDec(val, prec);
    return n > 0 ? n : 0;
  }
  //----------------------------------------------------------------------------
  /**  Print a number.
   *
   * \param[in] val the number to be printed.
   *
   * \return the number of bytes written.
   */
  template <typename T>
  size_t print(T val) {
    int n = printDec(val);
    return n > 0 ? n : 0;
  }
  //----------------------------------------------------------------------------
  /** Write a CR/LF.
   *
   * \return two, the number of bytes written, for success or zero for failure.
   */
  size_t println() {
    return putCRLF() > 0 ? 2 : 0;
  }
  //----------------------------------------------------------------------------
  /** Print a floating point number followed by CR/LF.
   *
   * \param[in] val the number to be printed.
   *
   * \param[in] prec Number of digits after decimal point.
   *
   * \return the number of bytes written.
   */
  size_t println(double val, uint8_t prec = 2) {
    return println(static_cast<float>(val), prec);
  }
  //----------------------------------------------------------------------------
  /** Print a floating point number followed by CR/LF.
   *
   * \param[in] val the number to be printed.
   *
   * \param[in] prec Number of digits after decimal point.
   *
   * \return the number of bytes written.
   */
  size_t println(float val, uint8_t prec = 2) {
    int n = printDec(val, prec);
    return n > 0 && putCRLF() > 0 ? n + 2 : 0;
  }
  //----------------------------------------------------------------------------
  /** Print an item followed by CR/LF
   *
   * \param[in] val the item to be printed.
   *
   * \return the number of bytes written.
   */
  template <typename T>
  size_t println(T val) {
    int n = print(val);
    return putCRLF() > 0 ? n + 2 : 0;
  }
  //----------------------------------------------------------------------------
  /** Print a char as a number.
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(char n) {
    if (CHAR_MIN == 0) {
      return printDec((unsigned char)n);
    } else {
      return printDec((signed char)n);
    }
  }
  //----------------------------------------------------------------------------
  /** print a signed 8-bit integer
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(signed char n);
  //----------------------------------------------------------------------------
  /** Print an unsigned 8-bit number.
   * \param[in] n number to be print.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(unsigned char n) {
    return printDec((uint16_t)n);
  }
  //----------------------------------------------------------------------------
  /** Print a int16_t
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(int16_t n);
  //----------------------------------------------------------------------------
  /** print a uint16_t.
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(uint16_t n);
  //----------------------------------------------------------------------------
  /** Print a signed 32-bit integer.
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(int32_t n);
  //----------------------------------------------------------------------------
  /** Write an unsigned 32-bit number.
   * \param[in] n number to be printed.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(uint32_t n);
  //----------------------------------------------------------------------------
  /** Print a double.
   * \param[in] value The number to be printed.
   * \param[in] prec Number of digits after decimal point.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(double value, uint8_t prec) {
    return printDec(static_cast<float>(value), prec);
  }
  //----------------------------------------------------------------------------
  /** Print a float.
   * \param[in] value The number to be printed.
   * \param[in] prec Number of digits after decimal point.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printDec(float value, uint8_t prec);
  //----------------------------------------------------------------------------
  /** Print a number followed by a field terminator.
   * \param[in] value The number to be printed.
   * \param[in] term The field terminator.
   * \param[in] prec Number of digits after decimal point.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printField(double value, char term, uint8_t prec = 2) {
    return printField(static_cast<float>(value), term, prec) > 0;
  }
  //----------------------------------------------------------------------------
  /** Print a number followed by a field terminator.
   * \param[in] value The number to be printed.
   * \param[in] term The field terminator.
   * \param[in] prec Number of digits after decimal point.
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printField(float value, char term, uint8_t prec = 2) {
    int rtn = printDec(value, prec);
    return rtn < 0 || putc(term) < 0 ? -1 : rtn + 1;
  }
  //----------------------------------------------------------------------------
  /** Print a number followed by a field terminator.
   * \param[in] value The number to be printed.
   * \param[in] term The field terminator.
   * \return The number of bytes written or -1 if an error occurs.
   */
  template <typename T>
  int printField(T value, char term) {
    int rtn = printDec(value);
    return rtn < 0 || putc(term) < 0 ? -1 : rtn + 1;
  }
  //----------------------------------------------------------------------------
  /** Print HEX
   * \param[in] n number to be printed as HEX.
   *
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printHex(uint32_t n);
  //----------------------------------------------------------------------------
  /** Print HEX with CRLF
   * \param[in] n number to be printed as HEX.
   *
   * \return The number of bytes written or -1 if an error occurs.
   */
  int printHexln(uint32_t n) {
    int rtn = printHex(n);
    return  rtn < 0 || putCRLF() != 2 ? -1 : rtn + 2;
  }
  //----------------------------------------------------------------------------
  /** Set position of a stream to the beginning.
   *
   * The rewind function sets the file position to the beginning of the
   * file. It is equivalent to fseek(0L, SEEK_SET) except that the error
   * indicator for the stream is also cleared.
   *
   * \return true for success or false for failure.
   */
  bool rewind();
  //----------------------------------------------------------------------------
  /** Push a byte back into an input stream.
   *
   * \param[in] c the byte (converted to an unsigned char) to be pushed back.
   *
   * One character of push-back is guaranteed. If the ungetc function is
   * called too many times without an intervening read or file positioning
   * operation on that stream, the operation may fail.
   *
   * A successful intervening call to a file positioning function (fseek,
   * fsetpos, or rewind) discards any pushed-back characters for the stream.
   *
   * \return Upon successful completion, ungetc() returns the byte pushed
   * back after conversion. Otherwise it returns EOF.
   */
  int ungetc(int c);
  //============================================================================
 private:
  bool fillBuf();
  int fillGet();
  bool flushBuf();
  int flushPut(uint8_t c);
  char* fmtSpace(uint8_t len);
  int write(const void* buf, size_t count);
  //----------------------------------------------------------------------------
  // S_SRD and S_WR are never simultaneously asserted
  static const uint8_t S_SRD = 0x01;  // OK to read
  static const uint8_t S_SWR = 0x02;  // OK to write
  static const uint8_t S_SRW = 0x04;  // open for reading & writing
  static const uint8_t S_EOF = 0x10;  // found EOF
  static const uint8_t S_ERR = 0x20;  // found error
  //----------------------------------------------------------------------------
  uint8_t  m_status;
  uint8_t* m_p;
  uint8_t  m_r;
  uint8_t  m_w;
  uint8_t  m_buf[STREAM_BUF_SIZE];
};
//------------------------------------------------------------------------------
#endif  // StdioStream_h
