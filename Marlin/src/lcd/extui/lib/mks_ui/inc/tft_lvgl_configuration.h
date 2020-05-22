/**
 * @file tft_lvgl_configuration.h
 * @date    2020-02-21
 * */

#ifndef TFT_LVGL_CONFIGURATION_H
#define TFT_LVGL_CONFIGURATION_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "lvgl.h"

void tft_lvgl_init();
void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void LCD_Clear(uint16_t  Color);
void tft_set_point(uint16_t x,uint16_t y,uint16_t point);
void ili9320_SetWindows(uint16_t StartX,uint16_t StartY,uint16_t width,uint16_t heigh);
void LCD_WriteRAM_Prepare(void);


//#ifdef __cplusplus
//} /* extern "C" */
//#endif

#endif