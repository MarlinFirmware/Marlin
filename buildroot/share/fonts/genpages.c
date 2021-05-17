/**
 * @file    genpages.c
 * @brief   generate required font page files
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright Yunhui Fu (2015)
 */

#include <stdio.h>
#include <stdint.h>    /* uint8_t */
#include <stdlib.h>    /* size_t */
#include <string.h>
#include <assert.h>
#include "getline.h"

wchar_t get_val_utf82uni(uint8_t *pstart) {
  size_t cntleft;
  wchar_t retval = 0;

  if (0 == (0x80 & *pstart)) return *pstart;

  if (((*pstart & 0xE0) ^ 0xC0) == 0) {
    cntleft = 1;
    retval = *pstart & ~0xE0;
  }
  else if (((*pstart & 0xF0) ^ 0xE0) == 0) {
    cntleft = 2;
    retval = *pstart & ~0xF0;
  }
  else if (((*pstart & 0xF8) ^ 0xF0) == 0) {
    cntleft = 3;
    retval = *pstart & ~0xF8;
  }
  else if (((*pstart & 0xFC) ^ 0xF8) == 0) {
    cntleft = 4;
    retval = *pstart & ~0xFC;
  }
  else if (((*pstart & 0xFE) ^ 0xFC) == 0) {
    cntleft = 5;
    retval = *pstart & ~0xFE;
  }
  else {
    /* encoding error */
    cntleft = 0;
    retval = 0;
  }
  pstart++;
  for (; cntleft > 0; cntleft --) {
    retval <<= 6;
    retval |= *pstart & 0x3F;
    pstart++;
  }
  return retval;
}

/**
 * @brief 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 *
 * @param pstart : 存储 UTF-8 字符的指针
 * @param pval : 需要返回的 Unicode 字符存放地址指针
 *
 * @return 成功返回下个 UTF-8 字符的位置
 *
 * 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 */
uint8_t* get_utf8_value(uint8_t *pstart, wchar_t *pval) {
  uint32_t val = 0;
  uint8_t *p = pstart;
  /*size_t maxlen = strlen(pstart);*/

  assert(NULL != pstart);

  #define NEXT_6_BITS() do{ val <<= 6; p++; val |= (*p & 0x3F); }while(0)

  if (0 == (0x80 & *p)) {
    val = (size_t)*p;
    p++;
  }
  else if (0xC0 == (0xE0 & *p)) {
    val = *p & 0x1F;
    NEXT_6_BITS();
    p++;
    assert((wchar_t)val == get_val_utf82uni(pstart));
  }
  else if (0xE0 == (0xF0 & *p)) {
    val = *p & 0x0F;
    NEXT_6_BITS();
    NEXT_6_BITS();
    p++;
    assert((wchar_t)val == get_val_utf82uni(pstart));
  }
  else if (0xF0 == (0xF8 & *p)) {
    val = *p & 0x07;
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    p++;
    assert((wchar_t)val == get_val_utf82uni(pstart));
  }
  else if (0xF8 == (0xFC & *p)) {
    val = *p & 0x03;
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    p++;
    assert((wchar_t)val == get_val_utf82uni(pstart));
  }
  else if (0xFC == (0xFE & *p)) {
    val = *p & 0x01;
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    NEXT_6_BITS();
    p++;
    assert((wchar_t)val == get_val_utf82uni(pstart));
  }
  else if (0x80 == (0xC0 & *p)) {
    /* error? */
    for (; 0x80 == (0xC0 & *p); p++);
  }
  else {
    /* error */
    for (; ((0xFE & *p) > 0xFC); p++);
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

void usage(char* progname) {
  fprintf(stderr, "usage: %s\n", progname);
  fprintf(stderr, "   read data from stdin\n");
}

void utf8_parse(const char* msg, unsigned int len) {
  uint8_t *pend = NULL;
  uint8_t *p;
  uint8_t *pre;
  wchar_t val;
  int page;

  pend = (uint8_t *)msg + len;
  for (pre = (uint8_t *)msg; pre < pend;) {
    val = 0;
    p = get_utf8_value(pre, &val);
    if (NULL == p) break;
    page = val / 128;
    if (val >= 256) {
      fprintf(stdout, "%d %d ", page, (val % 128));
      for (; pre < p; pre++) fprintf(stdout, "%c", *pre);
      fprintf(stdout, "\n");
    }
    pre = p;
  }
}

int load_file(FILE *fp) {
  char * buffer = NULL;
  size_t szbuf = 0;

  szbuf = 10000;
  buffer = (char*)malloc(szbuf);
  if (NULL == buffer) return -1;
  //pos = ftell (fp);
  while (getline( &buffer, &szbuf, fp ) > 0)
    utf8_parse((const char*)buffer, (unsigned int)strlen ((char *)buffer));

  free(buffer);
  return 0;
}

int main(int argc, char * argv[]) {
  if (argc > 1) {
    usage(argv[0]);
    exit(1);
  }
  load_file(stdin);
}
