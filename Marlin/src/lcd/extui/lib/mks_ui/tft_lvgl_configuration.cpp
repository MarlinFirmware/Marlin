/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * @file tft_lvgl_configuration.cpp
 * @date    2020-02-21
 */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#if ENABLED(TFT_LVGL_UI_SPI)
  #include "SPI_TFT.h"
#endif

#include "tft_lvgl_configuration.h"
#include "draw_ready_print.h"

#include "pic_manager.h"
#include "mks_hardware_test.h"
#include "draw_ui.h"
#include "SPIFlashStorage.h"
#include <lvgl.h>

#include "../../../../MarlinCore.h"
#include "../../../../inc/MarlinConfig.h"

#include HAL_PATH(../../HAL, tft/xpt2046.h)
#include "../../../ultralcd.h"
XPT2046 touch;

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#include <SPI.h>

#ifndef TFT_WIDTH
  #define TFT_WIDTH  480
#endif
#ifndef TFT_HEIGHT
  #define TFT_HEIGHT 320
#endif

#if HAS_SPI_FLASH_FONT
  extern void init_gb2312_font();
#endif

static lv_disp_buf_t disp_buf;
lv_group_t*  g;
#if ENABLED(SDSUPPORT)
  extern void UpdateAssets();
#endif
uint16_t DeviceCode = 0x9488;
extern uint8_t sel_id;

#define SetCs
#define ClrCs

#define HDP  799 // Horizontal Display Period
#define HT  1000 // Horizontal Total
#define HPS   51 // LLINE Pulse Start Position
#define LPS    3 // Horizontal Display Period Start Position
#define HPW    8 // LLINE Pulse Width

#define VDP  479 // Vertical Display Period
#define VT   530 // Vertical Total
#define VPS   24 // LFRAME Pulse Start Position
#define FPS   23 // Vertical Display Period Start Positio
#define VPW    3 // LFRAME Pulse Width

#define MAX_HZ_POSX HDP+1
#define MAX_HZ_POSY VDP+1

extern uint8_t gcode_preview_over, flash_preview_begin, default_preview_flg;

uint8_t bmp_public_buf[17 * 1024];

void SysTick_Callback() {
  lv_tick_inc(1);
  print_time_count();
  #if ENABLED(USE_WIFI_FUNCTION)
    if (tips_disp.timer == TIPS_TIMER_START) {
      tips_disp.timer_count++;
    }
  #endif
  if (uiCfg.filament_loading_time_flg == 1) {
    uiCfg.filament_loading_time_cnt++;
    uiCfg.filament_rate = (uint32_t)(((uiCfg.filament_loading_time_cnt / (uiCfg.filament_loading_time * 1000.0)) * 100.0) + 0.5);
    if (uiCfg.filament_loading_time_cnt >= (uiCfg.filament_loading_time * 1000)) {
      uiCfg.filament_loading_time_cnt  = 0;
      uiCfg.filament_loading_time_flg  = 0;
      uiCfg.filament_loading_completed = 1;
    }
  }
  if (uiCfg.filament_unloading_time_flg == 1) {
    uiCfg.filament_unloading_time_cnt++;
    uiCfg.filament_rate = (uint32_t)(((uiCfg.filament_unloading_time_cnt / (uiCfg.filament_unloading_time * 1000.0)) * 100.0) + 0.5);
    if (uiCfg.filament_unloading_time_cnt >= (uiCfg.filament_unloading_time * 1000)) {
      uiCfg.filament_unloading_time_cnt  = 0;
      uiCfg.filament_unloading_time_flg  = 0;
      uiCfg.filament_unloading_completed = 1;
      uiCfg.filament_rate                = 100;
    }
  }
}

#if DISABLED(TFT_LVGL_UI_SPI)

  extern void LCD_IO_Init(uint8_t cs, uint8_t rs);
  extern void LCD_IO_WriteData(uint16_t RegValue);
  extern void LCD_IO_WriteReg(uint16_t Reg);

  extern void LCD_IO_WriteMultiple(uint16_t color, uint32_t count);
  void tft_set_cursor(uint16_t x, uint16_t y) {
    LCD_IO_WriteReg(0x002A);
    LCD_IO_WriteData(x >> 8);
    LCD_IO_WriteData(x & 0x00FF);
    LCD_IO_WriteData(x >> 8);
    LCD_IO_WriteData(x & 0x00FF);
    //ILI9488_WriteData(0x01);
    //ILI9488_WriteData(0xDF);
    LCD_IO_WriteReg(0x002B);
    LCD_IO_WriteData(y >> 8);
    LCD_IO_WriteData(y & 0x00FF);
    LCD_IO_WriteData(y >> 8);
    LCD_IO_WriteData(y & 0x00FF);
    //ILI9488_WriteData(0x01);
    //ILI9488_WriteData(0x3F);
  }

  void LCD_WriteRAM_Prepare(void) {
    #if 0
      switch (DeviceCode) {
        case 0x9325: case 0x9328: case 0x8989: {
          ClrCs
          LCD->LCD_REG = R34;
          SetCs
        } break;
        default: LCD_WrtReg(0x002C);
      }
    #else
      LCD_IO_WriteReg(0x002C);
    #endif
  }

  void tft_set_point(uint16_t x, uint16_t y, uint16_t point) {
    //if (DeviceCode == 0x9488) {
    if (x > (TFT_WIDTH) || y > (TFT_HEIGHT)) return;
    //}
    tft_set_cursor(x, y);

    LCD_WriteRAM_Prepare();
    //LCD_WriteRAM(point);
    LCD_IO_WriteData(point);
  }

  void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue) {
    /* Write 16-bit Index, then Write Reg */
    ClrCs
    LCD_IO_WriteReg(LCD_Reg);
    /* Write 16-bit Reg */
    LCD_IO_WriteData(LCD_RegValue);
    SetCs
  }

  void LCD_setWindowArea(uint16_t StartX, uint16_t StartY, uint16_t width, uint16_t heigh) {
    uint16_t s_h, s_l, e_h, e_l;
    uint16_t xEnd, yEnd;
    xEnd = StartX + width;
    yEnd = StartY + heigh - 1;
    if (DeviceCode == 0x8989) {
      /*LCD_WriteReg(0x0044, (StartX & 0xFF) | (xEnd << 8));
      LCD_WriteReg(0x0045, StartY);
      LCD_WriteReg(0x0046, yEnd);*/
      LCD_WriteReg(0x0044, (StartY & 0xFF) | (yEnd << 8));
      LCD_WriteReg(0x0045, StartX);
      LCD_WriteReg(0x0046, xEnd);
    }
    else if (DeviceCode == 0x9488) {
      s_h = (StartX >> 8) & 0x00ff;
      s_l = StartX & 0x00ff;
      e_h = ((StartX + width - 1) >> 8) & 0x00ff;
      e_l = (StartX + width - 1) & 0x00ff;

      LCD_IO_WriteReg(0x002A);
      LCD_IO_WriteData(s_h);
      LCD_IO_WriteData(s_l);
      LCD_IO_WriteData(e_h);
      LCD_IO_WriteData(e_l);

      s_h = (StartY >> 8) & 0x00ff;
      s_l = StartY & 0x00ff;
      e_h = ((StartY + heigh - 1) >> 8) & 0x00ff;
      e_l = (StartY + heigh - 1) & 0x00ff;

      LCD_IO_WriteReg(0x002B);
      LCD_IO_WriteData(s_h);
      LCD_IO_WriteData(s_l);
      LCD_IO_WriteData(e_h);
      LCD_IO_WriteData(e_l);
    }
    else if ((DeviceCode == 0x9325) || (DeviceCode == 0x9328) || (DeviceCode == 0x1505)) {
      /* LCD_WriteReg(0x0050, StartX);
       LCD_WriteReg(0x0052, StartY);
       LCD_WriteReg(0x0051, xEnd);
       LCD_WriteReg(0x0053, yEnd);*/
      LCD_WriteReg(0x0050, StartY);   // Specify the start/end positions of the window address in the horizontal direction by an address unit
      LCD_WriteReg(0x0051, yEnd);     // Specify the start positions of the window address in the vertical direction by an address unit
      LCD_WriteReg(0x0052, (TFT_HEIGHT) - xEnd);
      LCD_WriteReg(0x0053, (TFT_HEIGHT) - StartX - 1); // Specify the end positions of the window address in the vertical direction by an address unit

    }
    else {
      s_h = (StartX >> 8) & 0xFF;
      s_l = StartX & 0xFF;
      e_h = ((StartX + width - 1) >> 8) & 0xFF;
      e_l = (StartX + width - 1) & 0xFF;

      LCD_IO_WriteReg(0x2A);
      LCD_IO_WriteData(s_h);
      LCD_IO_WriteData(s_l);
      LCD_IO_WriteData(e_h);
      LCD_IO_WriteData(e_l);

      s_h = (StartY >> 8) & 0xFF;
      s_l = StartY & 0xFF;
      e_h = ((StartY + heigh - 1) >> 8) & 0xFF;
      e_l = (StartY + heigh - 1) & 0xFF;

      LCD_IO_WriteReg(0x2B);
      LCD_IO_WriteData(s_h);
      LCD_IO_WriteData(s_l);
      LCD_IO_WriteData(e_h);
      LCD_IO_WriteData(e_l);
    }
  }

  void LCD_Clear(uint16_t Color) {
    uint32_t index = 0;
    unsigned int count;

    if (DeviceCode == 0x9488) {
      tft_set_cursor(0, 0);
      LCD_setWindowArea(0, 0, TFT_WIDTH, TFT_HEIGHT);
      LCD_WriteRAM_Prepare();
      #ifdef LCD_USE_DMA_FSMC
        LCD_IO_WriteMultiple(Color, (TFT_WIDTH) * (TFT_HEIGHT));
      #else
        //index = (TFT_HEIGHT) / 2 * (TFT_WIDTH);
        for (index = 0; index < (TFT_HEIGHT) * (TFT_WIDTH); index++)
          LCD_IO_WriteData(Color);
      #endif
      //LCD_IO_WriteMultiple(Color, (TFT_WIDTH) * (TFT_HEIGHT));
      //while(index --) LCD_IO_WriteData(Color);
    }
    else if (DeviceCode == 0x5761) {
      LCD_IO_WriteReg(0x002a);
      LCD_IO_WriteData(0);
      LCD_IO_WriteData(0);
      LCD_IO_WriteData(HDP >> 8);
      LCD_IO_WriteData(HDP & 0x00ff);
      LCD_IO_WriteReg(0x002b);
      LCD_IO_WriteData(0);
      LCD_IO_WriteData(0);
      LCD_IO_WriteData(VDP >> 8);
      LCD_IO_WriteData(VDP & 0x00ff);
      LCD_IO_WriteReg(0x002c);
      LCD_IO_WriteReg(0x002c);
      for (count = 0; count < (HDP + 1) * (VDP + 1); count++)
        LCD_IO_WriteData(Color);
    }
    else {
      tft_set_cursor(0, 0);
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      for (index = 0; index < 76800; index++)
        LCD_IO_WriteData(Color);
    }
  }

  #include HAL_PATH(../../HAL, tft/tft_fsmc.h)
  extern TFT_IO tftio;
  void fsmc_tft_init() {
    uint16_t i;

    TERN_(HAS_LCD_CONTRAST, refresh_contrast());

    #ifdef LCD_USE_DMA_FSMC
      dma_init(FSMC_DMA_DEV);
      dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
      dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);
    #endif

    LCD_IO_Init(FSMC_CS_PIN, FSMC_RS_PIN);

    _delay_ms(5);

    DeviceCode = tftio.GetID() & 0xFFFF;
    // Chitu and others
    if (DeviceCode == 0x8066) DeviceCode = 0x9488;

    if (DeviceCode == 0x9488) {
      LCD_IO_WriteReg(0x00E0);
      LCD_IO_WriteData(0x0000);
      LCD_IO_WriteData(0x0007);
      LCD_IO_WriteData(0x000f);
      LCD_IO_WriteData(0x000D);
      LCD_IO_WriteData(0x001B);
      LCD_IO_WriteData(0x000A);
      LCD_IO_WriteData(0x003c);
      LCD_IO_WriteData(0x0078);
      LCD_IO_WriteData(0x004A);
      LCD_IO_WriteData(0x0007);
      LCD_IO_WriteData(0x000E);
      LCD_IO_WriteData(0x0009);
      LCD_IO_WriteData(0x001B);
      LCD_IO_WriteData(0x001e);
      LCD_IO_WriteData(0x000f);

      LCD_IO_WriteReg(0x00E1);
      LCD_IO_WriteData(0x0000);
      LCD_IO_WriteData(0x0022);
      LCD_IO_WriteData(0x0024);
      LCD_IO_WriteData(0x0006);
      LCD_IO_WriteData(0x0012);
      LCD_IO_WriteData(0x0007);
      LCD_IO_WriteData(0x0036);
      LCD_IO_WriteData(0x0047);
      LCD_IO_WriteData(0x0047);
      LCD_IO_WriteData(0x0006);
      LCD_IO_WriteData(0x000a);
      LCD_IO_WriteData(0x0007);
      LCD_IO_WriteData(0x0030);
      LCD_IO_WriteData(0x0037);
      LCD_IO_WriteData(0x000f);

      LCD_IO_WriteReg(0x00C0);
      LCD_IO_WriteData(0x0010);
      LCD_IO_WriteData(0x0010);

      LCD_IO_WriteReg(0x00C1);
      LCD_IO_WriteData(0x0041);

      LCD_IO_WriteReg(0x00C5);
      LCD_IO_WriteData(0x0000);
      LCD_IO_WriteData(0x0022);
      LCD_IO_WriteData(0x0080);

      LCD_IO_WriteReg(0x0036);
      LCD_IO_WriteData(TERN(GRAPHICAL_TFT_ROTATE_180, 0xE8, 0x0068));

      LCD_IO_WriteReg(0x003A); //Interface Mode Control
      LCD_IO_WriteData(0x0055);

      LCD_IO_WriteReg(0x00B0);  //Interface Mode Control
      LCD_IO_WriteData(0x0000);
      LCD_IO_WriteReg(0x00B1);   //Frame rate 70HZ
      LCD_IO_WriteData(0x00B0);
      LCD_IO_WriteData(0x0011);
      LCD_IO_WriteReg(0x00B4);
      LCD_IO_WriteData(0x0002);
      LCD_IO_WriteReg(0x00B6); //RGB/MCU Interface Control
      LCD_IO_WriteData(0x0002);
      LCD_IO_WriteData(0x0042);

      LCD_IO_WriteReg(0x00B7);
      LCD_IO_WriteData(0x00C6);

      //WriteComm(0xBE);
      //WriteData(0x00);
      //WriteData(0x04);

      LCD_IO_WriteReg(0x00E9);
      LCD_IO_WriteData(0x0000);

      LCD_IO_WriteReg(0x00F7);
      LCD_IO_WriteData(0x00A9);
      LCD_IO_WriteData(0x0051);
      LCD_IO_WriteData(0x002C);
      LCD_IO_WriteData(0x0082);

      LCD_IO_WriteReg(0x0011);
      for (i = 0; i < 65535; i++) { /* do nothing */ }
      LCD_IO_WriteReg(0x0029);

      LCD_setWindowArea(0, 0, TFT_WIDTH, TFT_HEIGHT);

      OUT_WRITE(LCD_BACKLIGHT_PIN, LOW);
      LCD_Clear(0x0000);

      TERN_(HAS_LOGO_IN_FLASH, lcd_draw_logo());

      OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
      delay(2000);
    }
  }

  extern void LCD_IO_WriteSequence(uint16_t *data, uint16_t length);

  void lcd_draw_logo() {
    LCD_setWindowArea(0, 0, TFT_WIDTH, TFT_HEIGHT);
    LCD_WriteRAM_Prepare();

    for (uint16_t i = 0; i < (TFT_HEIGHT); i ++) {
      Pic_Logo_Read((uint8_t *)"", (uint8_t *)bmp_public_buf, (TFT_WIDTH) * 2);
      #ifdef LCD_USE_DMA_FSMC
        LCD_IO_WriteSequence((uint16_t *)bmp_public_buf, TFT_WIDTH);
      #else
        int index = 0;,x_off = 0;
        for (x_off = 0; x_off < TFT_WIDTH; x_off++) {
          LCD_IO_WriteData((uint16_t)bmp_public_buf[index]);
          index += 2;
        }
      #endif
    }
  }

#endif // !TFT_LVGL_UI_SPI

void tft_lvgl_init() {

  //uint16_t test_id=0;
  W25QXX.init(SPI_QUARTER_SPEED);
  //test_id=W25QXX.W25QXX_ReadID();

  gCfgItems_init();
  ui_cfg_init();
  disp_language_init();

  //init tft first!
  #if ENABLED(TFT_LVGL_UI_SPI)
    SPI_TFT.spi_init(SPI_FULL_SPEED);
    SPI_TFT.LCD_init();
  #else
    fsmc_tft_init();
  #endif

  //spi_flash_read_test();
  #if ENABLED(SDSUPPORT)
    UpdateAssets();
  #endif
  mks_test_get();

  touch.Init();

  lv_init();

  lv_disp_buf_init(&disp_buf, bmp_public_buf, NULL, LV_HOR_RES_MAX * 18); /*Initialize the display buffer*/

  lv_disp_drv_t disp_drv;     /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);    /*Basic initialization*/
  disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
  disp_drv.buffer = &disp_buf;    /*Assign the buffer to the display*/
  lv_disp_drv_register(&disp_drv);  /*Finally register the driver*/

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);     /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;  /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);   /*Finally register the driver*/

  #if HAS_ROTARY_ENCODER
    g = lv_group_create();
    lv_indev_drv_t enc_drv;
    lv_indev_drv_init(&enc_drv);
    enc_drv.type = LV_INDEV_TYPE_ENCODER;
    enc_drv.read_cb = my_mousewheel_read;
    lv_indev_t * enc_indev = lv_indev_drv_register(&enc_drv);
    lv_indev_set_group(enc_indev, g);
  #endif

  lv_fs_drv_t spi_flash_drv;
  lv_fs_drv_init(&spi_flash_drv);
  spi_flash_drv.letter = 'F';
  spi_flash_drv.open_cb = spi_flash_open_cb;
  spi_flash_drv.close_cb = spi_flash_close_cb;
  spi_flash_drv.read_cb = spi_flash_read_cb;
  spi_flash_drv.seek_cb = spi_flash_seek_cb;
  spi_flash_drv.tell_cb = spi_flash_tell_cb;
  lv_fs_drv_register(&spi_flash_drv);

  lv_fs_drv_t sd_drv;
  lv_fs_drv_init(&sd_drv);
  sd_drv.letter = 'S';
  sd_drv.open_cb = sd_open_cb;
  sd_drv.close_cb = sd_close_cb;
  sd_drv.read_cb = sd_read_cb;
  sd_drv.seek_cb = sd_seek_cb;
  sd_drv.tell_cb = sd_tell_cb;
  lv_fs_drv_register(&sd_drv);

  systick_attach_callback(SysTick_Callback);

  #if HAS_SPI_FLASH_FONT
    init_gb2312_font();
  #endif

  tft_style_init();

  filament_pin_setup();

  lv_encoder_pin_init();

  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.load();
    if (recovery.valid()) {
      if (gCfgItems.from_flash_pic == 1)
        flash_preview_begin = 1;
      else
        default_preview_flg = 1;

      uiCfg.print_state = REPRINTING;

      ZERO(public_buf_m);
      strncpy(public_buf_m, recovery.info.sd_filename, sizeof(public_buf_m));
      card.printLongPath(public_buf_m);

      strncpy(list_file.long_name[sel_id], card.longFilename, sizeof(list_file.long_name[sel_id]));

      lv_draw_printing();
    }
    else
  #endif
    lv_draw_ready_print();

  if (mks_test_flag == 0x1E)
    mks_gpio_test();
}

void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
  #if ENABLED(TFT_LVGL_UI_SPI)
    uint16_t i, width, height;

    width = area->x2 - area->x1 + 1;
    height = area->y2 - area->y1 + 1;

    SPI_TFT.SetWindows((uint16_t)area->x1, (uint16_t)area->y1, width, height);
    for (i = 0; i < height; i++) {
      SPI_TFT.tftio.WriteSequence((uint16_t*)(color_p + width * i), width);
    }
    lv_disp_flush_ready(disp);       /* Indicate you are ready with the flushing*/

    W25QXX.init(SPI_QUARTER_SPEED);

  #else // !TFT_LVGL_UI_SPI

    #if 1
      uint16_t i, width, height;
      //uint16_t clr_temp;
      width = area->x2 - area->x1 + 1;
      height = area->y2 - area->y1 + 1;
      LCD_setWindowArea((uint16_t)area->x1, (uint16_t)area->y1, width, height);
      LCD_WriteRAM_Prepare();
      for (i = 0; i < width * height - 2; i++) {
        //clr_temp = (uint16_t)(((uint16_t)color_p->ch.red << 11)
                              //| ((uint16_t)color_p->ch.green << 5)
                              //| ((uint16_t)color_p->ch.blue));
        LCD_IO_WriteData(color_p->full);
        color_p++;
      }

      lv_disp_flush_ready(disp);       /* Indicate you are ready with the flushing*/
    #endif

  #endif // !TFT_LVGL_UI_SPI
}

#define TICK_CYCLE 1

unsigned int getTickDiff(unsigned int curTick, unsigned int lastTick) {
  return TICK_CYCLE * (lastTick <= curTick ? (curTick - lastTick) : (0xFFFFFFFF - lastTick + curTick));
}

static bool get_point(int16_t *x, int16_t *y) {
  bool is_touched = touch.getRawPoint(x, y);

  if (is_touched) {
    *x = int16_t((int32_t(*x) * XPT2046_X_CALIBRATION) >> 16) + XPT2046_X_OFFSET;
    *y = int16_t((int32_t(*y) * XPT2046_Y_CALIBRATION) >> 16) + XPT2046_Y_OFFSET;
  }

  #if ENABLED(GRAPHICAL_TFT_ROTATE_180)
    *x = int16_t((TFT_WIDTH) - (int)(*x));
    *y = int16_t((TFT_HEIGHT) - (int)(*y));
  #endif

  return is_touched;
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  static int16_t last_x = 0, last_y = 0;
  static uint8_t last_touch_state = LV_INDEV_STATE_REL;
  static int32_t touch_time1 = 0;
  uint32_t tmpTime, diffTime = 0;

  tmpTime = millis();
  diffTime = getTickDiff(tmpTime, touch_time1);
  /*Save the state and save the pressed coordinate*/
  //data->state = TOUCH_PressValid(last_x, last_y) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
  //if (data->state == LV_INDEV_STATE_PR)  ADS7843_Rd_Addata((u16 *)&last_x, (u16 *)&last_y);
  //touchpad_get_xy(&last_x, &last_y);
  /*Save the pressed coordinates and the state*/
  if (diffTime > 20) {
    if (get_point(&last_x, &last_y)) {

      if (last_touch_state == LV_INDEV_STATE_PR) return false;
      data->state = LV_INDEV_STATE_PR;

      // Set the coordinates (if released use the last-pressed coordinates)

      data->point.x = last_x;
      data->point.y = last_y;

      last_x = last_y = 0;
      last_touch_state = LV_INDEV_STATE_PR;
    }
    else {
      if (last_touch_state == LV_INDEV_STATE_PR)
        data->state = LV_INDEV_STATE_REL;
      last_touch_state = LV_INDEV_STATE_REL;
    }

    touch_time1 = tmpTime;
  }

  return false; // Return `false` since no data is buffering or left to read
}

int16_t enc_diff = 0;
lv_indev_state_t state = LV_INDEV_STATE_REL;

bool my_mousewheel_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
  (void) indev_drv;   /*Unused*/

  data->state = state;
  data->enc_diff = enc_diff;
  enc_diff = 0;

  return false;       /*No more data to read so return false*/
}

extern uint8_t currentFlashPage;

//spi_flash
uint32_t pic_read_base_addr = 0, pic_read_addr_offset = 0;
lv_fs_res_t spi_flash_open_cb (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode) {
  static char last_path_name[30];
  if (strcasecmp(last_path_name,path) != 0) {
    pic_read_base_addr = lv_get_pic_addr((uint8_t *)path);
    ZERO(last_path_name);
    strcpy(last_path_name,path);
  }
  else {
    W25QXX.init(SPI_QUARTER_SPEED);
    currentFlashPage = 0;
  }
  pic_read_addr_offset = pic_read_base_addr;
  return LV_FS_RES_OK;
}

lv_fs_res_t spi_flash_close_cb (lv_fs_drv_t * drv, void * file_p) {
  lv_fs_res_t res = LV_FS_RES_OK;
  /* Add your code here*/
  pic_read_addr_offset = pic_read_base_addr;
  return res;
}

lv_fs_res_t spi_flash_read_cb (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br) {
  lv_pic_test((uint8_t *)buf, pic_read_addr_offset, btr);
  *br = btr;
  return LV_FS_RES_OK;
}

lv_fs_res_t spi_flash_seek_cb(lv_fs_drv_t * drv, void * file_p, uint32_t pos) {
  #if HAS_SPI_FLASH_COMPRESSION
    if (pos == 4) {
      uint8_t bmp_header[4];
      SPIFlash.beginRead(pic_read_base_addr);
      SPIFlash.readData(bmp_header, 4);
      currentFlashPage = 1;
    }
    pic_read_addr_offset = pic_read_base_addr;
  #else
    pic_read_addr_offset = pic_read_base_addr + pos;
  #endif
  return LV_FS_RES_OK;
}

lv_fs_res_t spi_flash_tell_cb(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
  *pos_p = pic_read_addr_offset - pic_read_base_addr;
  return LV_FS_RES_OK;
}

//sd
char *cur_namefff;
uint32_t sd_read_base_addr = 0,sd_read_addr_offset = 0;
lv_fs_res_t sd_open_cb (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode) {
  //cur_namefff = strrchr(path, '/');
  char name_buf[100];
  ZERO(name_buf);
  strcat(name_buf,"/");
  strcat(name_buf,path);
  char *temp = strstr(name_buf,".bin");
  if (temp) { strcpy(temp,".GCO"); }
  sd_read_base_addr = lv_open_gcode_file((char *)name_buf);
  sd_read_addr_offset = sd_read_base_addr;
  if (sd_read_addr_offset == 0) return LV_FS_RES_NOT_EX;
  return LV_FS_RES_OK;
}

lv_fs_res_t sd_close_cb (lv_fs_drv_t * drv, void * file_p) {
  /* Add your code here*/
  lv_close_gcode_file();
  return LV_FS_RES_OK;
}

lv_fs_res_t sd_read_cb (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br) {
  if (btr == 200) {
    lv_gcode_file_read((uint8_t *)buf);
    //pic_read_addr_offset += 208;
    *br = 200;
  }
  else if (btr == 4) {
    uint8_t header_pic[4] = { 0x04, 0x90, 0x81, 0x0C };
    memcpy(buf, header_pic, 4);
    //pic_read_addr_offset += 4;
    *br = 4;
  }
  return LV_FS_RES_OK;
}

lv_fs_res_t sd_seek_cb(lv_fs_drv_t * drv, void * file_p, uint32_t pos) {
  sd_read_addr_offset = sd_read_base_addr + (pos - 4) / 200 * 409;
  lv_gcode_file_seek(sd_read_addr_offset);
  return LV_FS_RES_OK;
}

lv_fs_res_t sd_tell_cb(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
  if (sd_read_addr_offset) *pos_p = 0;
  else *pos_p = (sd_read_addr_offset - sd_read_base_addr) / 409 * 200 + 4;
  return LV_FS_RES_OK;
}

void lv_encoder_pin_init() {
  #if 1 // HAS_DIGITAL_BUTTONS

    #if BUTTON_EXISTS(EN1)
      SET_INPUT_PULLUP(BTN_EN1);
    #endif
    #if BUTTON_EXISTS(EN2)
      SET_INPUT_PULLUP(BTN_EN2);
    #endif
    #if BUTTON_EXISTS(ENC)
      SET_INPUT_PULLUP(BTN_ENC);
    #endif

    #if BUTTON_EXISTS(BACK)
      SET_INPUT_PULLUP(BTN_BACK);
    #endif

    #if BUTTON_EXISTS(UP)
      SET_INPUT(BTN_UP);
    #endif
    #if BUTTON_EXISTS(DWN)
      SET_INPUT(BTN_DWN);
    #endif
    #if BUTTON_EXISTS(LFT)
      SET_INPUT(BTN_LFT);
    #endif
    #if BUTTON_EXISTS(RT)
      SET_INPUT(BTN_RT);
    #endif

  #endif // HAS_DIGITAL_BUTTONS
}

#if 1 // HAS_ENCODER_ACTION

  //static const int8_t encoderDirection = 1;
  //static int16_t enc_Direction;
  void lv_update_encoder() {
    static uint8_t buttons;
    static uint32_t encoder_time1;
    uint32_t tmpTime, diffTime = 0;
    tmpTime = millis();
    diffTime = getTickDiff(tmpTime, encoder_time1);
    if (diffTime > 50) {

      #if ANY_BUTTON(EN1, EN2, ENC, BACK)

        uint8_t newbutton = 0;

        #if BUTTON_EXISTS(EN1)
          if (BUTTON_PRESSED(EN1)) newbutton |= EN_A;
        #endif
        #if BUTTON_EXISTS(EN2)
          if (BUTTON_PRESSED(EN2)) newbutton |= EN_B;
        #endif
        #if BUTTON_EXISTS(ENC)
          if (BUTTON_PRESSED(ENC)) newbutton |= EN_C;
        #endif
        #if BUTTON_EXISTS(BACK)
          if (BUTTON_PRESSED(BACK)) newbutton |= EN_D;
        #endif

      #else

        constexpr uint8_t newbutton = 0;

      #endif

      buttons = newbutton;

      #if HAS_ENCODER_WHEEL
        static uint8_t lastEncoderBits;

        #define encrot0 0
        #define encrot1 1
        #define encrot2 2

        // Manage encoder rotation
        //#define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: enc_Direction += encoderDirection; break; case _E2: enc_Direction -= encoderDirection; }

        uint8_t enc = 0;
        if (buttons & EN_A) enc |= B01;
        if (buttons & EN_B) enc |= B10;
        if (enc != lastEncoderBits) {
          switch (enc) {
            case encrot1:
              if (lastEncoderBits == encrot0) {
                enc_diff--;
                encoder_time1 = tmpTime;
              }
              break;
            case encrot2:
              if (lastEncoderBits == encrot0) {
                enc_diff++;
                encoder_time1 = tmpTime;
              }
              break;
          }
          lastEncoderBits = enc;
        }
        static uint8_t last_button_state = LV_INDEV_STATE_REL;
        const uint8_t enc_c = (buttons & EN_C) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        if (enc_c != last_button_state) {
          state = enc_c ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

          last_button_state = enc_c;
        }

      #endif // HAS_ENCODER_WHEEL

    } // next_button_update_ms
  }

#endif // HAS_ENCODER_ACTION

#endif // HAS_TFT_LVGL_UI
