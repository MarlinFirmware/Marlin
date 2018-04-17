/**
 * @file    fontutils.cpp
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(ULTRA_LCD)
#include "ultralcd.h"
#include "../Marlin.h"
#endif

#include "fontutils.h"

uint8_t read_byte_ram(uint8_t * str) { return *str; }
uint8_t read_byte_rom(uint8_t * str) { return pgm_read_byte(str); }

#if DEBUG
  #ifdef ARDUINO
    #include <Arduino.h>
    #include <stdarg.h>

    void serial_printf_P(const char *format, ...) {
      static char buff[128];
      va_list args;
      va_start(args,format);
      vsnprintf_P(buff,sizeof(buff),format,args);
      va_end(args);
      buff[sizeof(buff)/sizeof(buff[0])-1]='\0';

      //Serial.print(buff);
      SERIAL_ECHO(buff); SERIAL_EOL;
    }
  #endif
#endif


#ifdef __WIN32__                // or whatever
  #define PRIiSZ "ld"
  #define PRIuSZ "Iu"
#else
  #define PRIiSZ "zd"
  #define PRIuSZ "zu"
#endif
#define PRIiOFF "lld"
#define PRIuOFF "llu"


#define DBGMSG(a,b, ...) TRACE( #__VA_ARGS__ )

//typedef int (* pf_bsearch_cb_comp_t)(void *userdata, size_t idx, void * data_pin); /*"data_list[idx] - *data_pin"*/
/**
 * @brief 折半方式查找记录
 *
 * @param userdata : 用户数据指针
 * @param num_data : 数据个数
 * @param cb_comp : 比较两个数据的回调函数
 * @param data_pinpoint : 所要查找的 匹配数据指针
 * @param ret_idx : 查找到的位置;如果没有找到，则返回如添加该记录时其所在的位置。
 *
 * @return 找到则返回0，否则返回<0
 *
 * 折半方式查找记录, psl->marr 中指向的数据已经以先小后大方式排好序
 */
/**
 * @brief Using binary search to find the position by data_pin
 *
 * @param userdata : User's data
 * @param num_data : the item number of the sorted data
 * @param cb_comp : the callback function to compare the user's data and pin
 * @param data_pin : The reference data to be found
 * @param ret_idx : the position of the required data; If failed, then it is the failed position, which is the insert position if possible.
 *
 * @return 0 on found, <0 on failed(fail position is saved by ret_idx)
 *
 * Using binary search to find the position by data_pin. The user's data should be sorted.
 */
int pf_bsearch_r(void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx) {
  int retcomp;

  FU_ASSERT(NULL != ret_idx);
  /* 查找合适的位置 */
  if (num_data < 1) {
    *ret_idx = 0;
    DBGMSG (PFDBG_CATLOG_PF, PFDBG_LEVEL_ERROR, "num_data(%" PRIuSZ ") < 1", num_data);
    return -1;
  }

  /* 折半查找 */
  /* 为了不出现负数，以免缩小索引的所表示的数据范围
   * (负数表明减少一位二进制位的使用)，
   * 内部 ileft 和 iright使用从1开始的下标，
   *   即1表示C语言中的0, 2表示语言中的1，以此类推。
   * 对外还是使用以 0 为开始的下标
   */
  size_t i = 0, ileft = 1, iright = num_data;
  bool flg_found = false;
  for (; ileft <= iright;) {
    i = (ileft + iright) / 2 - 1;
    /* cb_comp should return the *userdata[i] - *data_pinpoint */
    retcomp = cb_comp (userdata, i, data_pinpoint);
    if (retcomp > 0)
      iright = i;
    else if (retcomp < 0)
      ileft = i + 2;
    else {
      /* found ! */
      flg_found = true;
      break;
    }
  }

  if (flg_found) {
    *ret_idx = i;
    return 0;
  }
  if (iright <= i)
    *ret_idx = i;
  else if (ileft >= i + 2)
    *ret_idx = i + 1;
  //DBGMSG (PFDBG_CATLOG_PF, PFDBG_LEVEL_DEBUG, "not found! num_data=%" PRIuSZ "; ileft=%" PRIuSZ ", iright=%" PRIuSZ ", i=%" PRIuSZ "", num_data, ileft, iright, i);
  return -1;
}

/**
 * @brief 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 *
 * @param pstart : 存储 UTF-8 字符的指针
 * @param cb_read_byte : 读取字符的函数；用于8位MCU ROM
 * @param pval : 需要返回的 Unicode 字符存放地址指针
 *
 * @return 成功返回下个 UTF-8 字符的位置
 *
 * 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 */
uint8_t* get_utf8_value_cb(uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval) {
  uint32_t val = 0;
  uint8_t *p = pstart;

  FU_ASSERT(NULL != pstart);
  FU_ASSERT(NULL != cb_read_byte);

  uint8_t valcur = cb_read_byte(p);
  if (0 == (0x80 & valcur)) {
    val = valcur;
    p++;
  }
  else if (0xC0 == (0xE0 & valcur)) {
    val = valcur & 0x1F;
    val <<= 6;
    p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    p++;
  }
  else if (0xE0 == (0xF0 & valcur)) {
    val = valcur & 0x0F;
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    p++;
  }
  else if (0xF0 == (0xF8 & valcur)) {
    val = valcur & 0x07;
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    p++;
  }
  else if (0xF8 == (0xFC & valcur)) {
    val = valcur & 0x03;
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    p++;
  }
  else if (0xFC == (0xFE & valcur)) {
    val = valcur & 0x01;
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    val <<= 6; p++;
    valcur = cb_read_byte(p);
    val |= (valcur & 0x3F);
    p++;
  }
  else if (0x80 == (0xC0 & valcur)) {
    /* error? */
    TRACE("ERR 1");
    for (; 0x80 == (0xC0 & valcur); ) { p++; valcur = cb_read_byte(p); }
  }
  else {
    /* error */
    TRACE("ERR 2");
    for (; ((0xFE & valcur) > 0xFC); ) { p++; valcur = cb_read_byte(p); }
  }
  /*
    if (val == 0) {
      p = NULL;
  */
  /*
    }
    else if (pstart + maxlen < p) {
      p = pstart;
      if (pval) *pval = 0;
    }
  */

  if (pval) *pval = val;

  return p;
}

// uint8_t * get_utf8_value_cb (uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval);
int utf8_strlen_cb(const char *pstart, read_byte_cb_t cb_read_byte) {
  wchar_t ch;
  uint8_t *pnext;
  int cnt = 0;

  for (pnext = (uint8_t *)pstart; ; ) {
    pnext = get_utf8_value_cb(pnext, cb_read_byte, &ch);
    if (pnext == NULL || ch == 0) break;
    cnt++;
    TRACE("cnt=%d, ch=0x%X", cnt, (int)ch);
  }
  return cnt;
}

int
my_strlen_P(const char *pstart)
{
  const char *p;
  FU_ASSERT(NULL != pstart);
  p = pstart;
  while (p && pgm_read_byte(p) != '\0') p ++;
  return (p - pstart);
}

uint8_t utf8_strlen(const char *pstart)   { return utf8_strlen_cb(pstart, read_byte_ram); }
uint8_t utf8_strlen_P(const char *pstart) { return utf8_strlen_cb(pstart, read_byte_rom); }

char* utf8_strncpy_cb( char * destination, const char *source, size_t num, int len_src, read_byte_cb_t cb_read_byte) {
  uint8_t *p = (uint8_t *)source;
  uint8_t *d = (uint8_t *)destination;

  FU_ASSERT(NULL != destination);
  FU_ASSERT(NULL != source);
  FU_ASSERT(NULL != cb_read_byte);

  uint8_t *pend = p + len_src;

  while (p < pend) {
    uint8_t valcur = cb_read_byte(p);
    size_t len = 0;
    if (0 == (0x80 & valcur))
      len = 1;
    else if (0xC0 == (0xE0 & valcur))
      len = 2;
    else if (0xE0 == (0xF0 & valcur))
      len = 3;
    else if (0xF0 == (0xF8 & valcur))
      len = 4;
    else if (0xF8 == (0xFC & valcur))
      len = 5;
    else if (0xFC == (0xFE & valcur))
      len = 6;
    else if (0x80 == (0xC0 & valcur)) {
      /* error? */
      for (; 0x80 == (0xC0 & valcur) && (p < pend); ) { p++; valcur = cb_read_byte(p); }
    }
    else {
      /* error */
      for (; ((0xFE & valcur) > 0xFC) && (p < pend); ) { p++; valcur = cb_read_byte(p); }
    }
    if (len < num) {
      for (size_t i = 0; i < len; i++) {
        valcur = cb_read_byte(p);
        *d = valcur;
        d++;
        p++;
      }
    }
    else
      break;
  }
  *d = 0;
  return destination;
}

char* utf8_strncpy(char * destination, const char * source, size_t num) {
  return utf8_strncpy_cb(destination, source, num, strlen(source), read_byte_ram);
}

char* utf8_strncpy_P(char * destination, const char * source, size_t num) {
  return utf8_strncpy_cb(destination, source, num, my_strlen_P(source), read_byte_rom);
}
