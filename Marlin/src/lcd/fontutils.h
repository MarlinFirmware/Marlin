/**
 * @file    fontutils.h
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#ifndef _FONT_UTILS_H
#define _FONT_UTILS_H

#define DEBUG 0

#ifdef ARDUINO
  #include <Arduino.h>
#else // ARDUINO
  #include <stdint.h>
  #include <stdio.h>
  #include <stdlib.h>
#endif // ARDUINO

#ifndef pgm_read_word_near // __AVR__
  #include <stdint.h>
  #include <string.h>
  #include <assert.h>
  //#define pgm_read_word_near(a) *((uint16_t *)(a))
  #define pgm_read_word_near(a) (*(a))
  #define pgm_read_byte_near(a) *((uint8_t *)(a))
  #define pgm_read_byte pgm_read_byte_near
#elif defined(__AVR__)
  #include <avr/pgmspace.h>
#endif

#ifndef PROGMEM
  #define PROGMEM
  #define strlen_P strlen
  #define memcpy_P memcpy
  #define vsnprintf_P vsnprintf
#endif // PROGMEM

#ifdef __cplusplus
extern "C" {
#endif

// read a byte from ROM or RAM
typedef uint8_t (* read_byte_cb_t)(uint8_t * str);

//inline uint8_t read_byte_ram(uint8_t * str) { return *str; }
//inline uint8_t read_byte_rom(uint8_t * str) { return pgm_read_byte(str); }
uint8_t read_byte_ram(uint8_t * str);
uint8_t read_byte_rom(uint8_t * str);

#ifdef __cplusplus
}
#endif

#include <stddef.h> // wchar_t
#include <stdint.h> // uint32_t

#ifdef ARDUINO

  // there's overflow of the wchar_t due to the 2-byte size in Arduino
  // sizeof(wchar_t)=2; sizeof(size_t)=2; sizeof(uint32_t)=4;
  // sizeof(int)=2; sizeof(long)=4; sizeof(unsigned)=2;
  //#undef wchar_t
  #define wchar_t uint32_t
  //typedef uint32_t wchar_t;

#else

  #include <sys/types.h> // ssize_t
  #include <assert.h>
  // x86_64
  // sizeof(wchar_t)=4; sizeof(size_t)=8; sizeof(uint32_t)=4;
  // sizeof(int)=4; sizeof(long)=8; sizeof(unsigned)=4;
  //#define wchar_t uint32_t
  #define wchar_t size_t

  #ifndef PRIu32
    #define PRIu32 "lu"
  #endif
  #ifndef PRIX32
    #define PRIX32 "lX"
  #endif

#endif

#define UNUSED_VARIABLE(a) ((void)(a))

#ifndef MIN
  #define MIN(a,b) (((a)>(b))?(b):(a))
#endif

#ifndef NUM_ARRAY
  #define NUM_ARRAY(a) (sizeof(a)/sizeof((a)[0]))
#endif // NUM_ARRAY


#ifdef __cplusplus
extern "C" {
#endif

//#define pixel_len_t u8g_uint_t
#define pixel_len_t uint16_t
//#define pixel_len_t uint8_t
//typedef uint16_t pixel_len_t;
#define PIXEL_LEN_NOLIMIT ((pixel_len_t)(-1))

typedef int (* pf_bsearch_cb_comp_t)(void *userdata, size_t idx, void * data_pin); /*"data_list[idx] - *data_pin"*/
int pf_bsearch_r(void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx);

//wchar_t get_val_utf82uni(uint8_t *pstart);
//uint8_t * get_utf8_value(uint8_t *pstart, wchar_t *pval);
uint8_t * get_utf8_value_cb(uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval);

uint8_t utf8_strlen(const char *pstart);
uint8_t utf8_strlen_P(const char *pstart);

char * utf8_strncpy(char * destination, const char * source, size_t num);
char * utf8_strncpy_P(char * destination, const char * source, size_t num);
int my_strlen_P(const char *pstart);

#if 0 // DEBUG
#if 0 //defined(ARDUINO)
#if defined(__AVR__)
#define TRACE(fmt, ...) {static const PROGMEM char CONSTSTR[] = "%d %d " fmt " {ln:%d;}\n"; serial_printf_P(CONSTSTR, millis(), ##__VA_ARGS__, __LINE__);  }
#else
#define TRACE(fmt, ...) {static const PROGMEM char CONSTSTR[] = "%d " fmt " {ln:%d, fn:" __FILE__ "}\n"; serial_printf_P(CONSTSTR, millis(), ##__VA_ARGS__, __LINE__);  }
#endif
#define FU_ASSERT(a) if (!(a)) {TRACE("Assert: " # a ); }

#ifdef __cplusplus
extern "C" {
#endif
void serial_printf_P(const char *format, ...);
#ifdef __cplusplus
}
#endif

#else // ARDUINO
#include <stdio.h>
#define FU_ASSERT(a) if (!(a)) {printf("Assert: " # a); exit(1);}
#define TRACE(fmt, ...) fprintf(stdout, "[%s()] " fmt " {ln:%d, fn:" __FILE__ "}\n", __func__, ##__VA_ARGS__, __LINE__)
//#else
//#define FU_ASSERT(a)
//#define TRACE(...)
#endif // ARDUINO

#else // DEBUG
  #define TRACE(fmt, ...)
  #define FU_ASSERT(a)
#endif // DEBUG


#ifdef __cplusplus
}
#endif

#endif // _FONT_UTILS_H
