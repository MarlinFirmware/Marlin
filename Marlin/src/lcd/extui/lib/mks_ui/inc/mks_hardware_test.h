#ifndef MKS_HARDWARE_TEST_H
#define MKS_HARDWARE_TEST_H

#include "lvgl.h"

void Test_GPIO();
void disp_char_1624(uint16_t x,uint16_t y,uint8_t c,uint16_t charColor,uint16_t bkColor);
void disp_string(uint16_t x,uint16_t y,const char * string,uint16_t charColor,uint16_t bkColor);
void mks_test();
void disp_pic_update();
void disp_font_update();

#endif

