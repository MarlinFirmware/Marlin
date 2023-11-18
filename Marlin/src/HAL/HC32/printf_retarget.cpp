#ifdef ARDUINO_ARCH_HC32
#ifdef REDIRECT_PRINTF_TO_SERIAL
#if !defined(__GNUC__)
  #error "only GCC is supported"
#endif

#include "../../inc/MarlinConfig.h"

/**
 * @brief implementation of _write that redirects everything to the host serial(s)
 * @param file file descriptor. don't care
 * @param ptr pointer to the data to write
 * @param len length of the data to write
 * @return number of bytes written
 */
extern "C" int _write(int file, char *ptr, int len) {
  // SERIAL_ECHO_START(); // echo:
  for (int i = 0; i < len; i++) {
    SERIAL_CHAR(ptr[i]);
  }

  return len;
}

/**
 * @brief implementation of _isatty that always returns 1
 * @param file file descriptor. don't care
 * @return everything is a tty. there are no files to be had
 */
extern "C" int _isatty(int file) {
  return 1;
}

#endif // REDIRECT_PRINTF_TO_SERIAL
#endif // ARDUINO_ARCH_HC32
