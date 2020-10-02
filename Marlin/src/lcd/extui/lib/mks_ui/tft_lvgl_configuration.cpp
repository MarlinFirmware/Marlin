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
#include "W25Qxx.h"
#include "pic_manager.h"
#include "mks_hardware_test.h"
#include "draw_ui.h"
#include <lvgl.h>

#include "../../../../MarlinCore.h"
#include "../../../../feature/touch/xpt2046.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#include <SPI.h>

#if HAS_SPI_FLASH_FONT
  extern void init_gb2312_font();
#endif

static lv_disp_buf_t disp_buf;
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

void SysTick_Callback() {
  lv_tick_inc(1);
  print_time_count();
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
  if ((DeviceCode == 0x9325) || (DeviceCode == 0x9328) || (DeviceCode == 0x8989)) {
    ClrCs
    LCD->LCD_REG = R34;
    SetCs
  }
  else {
    LCD_WrtReg(0x002C);
  }
  #else
    LCD_IO_WriteReg(0x002C);
  #endif
}

void tft_set_point(uint16_t x, uint16_t y, uint16_t point) {
  //if (DeviceCode == 0x9488) {
  if ((x > 480) || (y > 320)) return;
  //}
  //**if ( (x>320)||(y>240) ) return;
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

void ili9320_SetWindows(uint16_t StartX, uint16_t StartY, uint16_t width, uint16_t heigh) {
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
    LCD_WriteReg(0x0050, StartY);      //Specify the start/end positions of the window address in the horizontal direction by an address unit
    LCD_WriteReg(0x0051, yEnd);      //Specify the start positions of the window address in the vertical direction by an address unit
    LCD_WriteReg(0x0052, 320 - xEnd);
    LCD_WriteReg(0x0053, 320 - StartX - 1);      //Specify the end positions of the window address in the vertical direction by an address unit

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
    ili9320_SetWindows(0, 0, 480, 320);
    LCD_WriteRAM_Prepare();
    #ifdef LCD_USE_DMA_FSMC
      LCD_IO_WriteMultiple(Color, LCD_FULL_PIXEL_WIDTH * LCD_FULL_PIXEL_HEIGHT);
    #else
    //index = (160*480);
    for (index = 0; index < 320 * 480; index++)
      LCD_IO_WriteData(Color);
    #endif
    //LCD_IO_WriteMultiple(Color, (480*320));
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

extern uint16_t ILI9488_ReadRAM();


void init_tft() {
  uint16_t i;
  //************* Start Initial Sequence **********//

  //start lcd pins and dma
  #if PIN_EXISTS(LCD_BACKLIGHT)
    OUT_WRITE(LCD_BACKLIGHT_PIN, DISABLED(DELAYED_BACKLIGHT_INIT)); // Illuminate after reset or right away
  #endif

  #if PIN_EXISTS(LCD_RESET)
    // Perform a clean hardware reset with needed delays
    OUT_WRITE(LCD_RESET_PIN, LOW);
    _delay_ms(5);
    WRITE(LCD_RESET_PIN, HIGH);
    _delay_ms(5);
  #endif

  #if PIN_EXISTS(LCD_BACKLIGHT) && ENABLED(DELAYED_BACKLIGHT_INIT)
    WRITE(LCD_BACKLIGHT_PIN, HIGH);
  #endif

  TERN_(HAS_LCD_CONTRAST, refresh_contrast());

  #ifdef LCD_USE_DMA_FSMC
    dma_init(FSMC_DMA_DEV);
    dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);
  #endif

  LCD_IO_Init(FSMC_CS_PIN, FSMC_RS_PIN);

  _delay_ms(5);

  LCD_IO_WriteReg(0x00D3);
  DeviceCode = ILI9488_ReadRAM(); //dummy read
  DeviceCode = ILI9488_ReadRAM();
  DeviceCode = ILI9488_ReadRAM();
  DeviceCode <<= 8;
  DeviceCode |= ILI9488_ReadRAM();

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
    //ILI9488_WriteData(0x0068);
    //if (gCfgItems.overturn_180 != 0xEE) {
    LCD_IO_WriteData(0x0068);
    //}
    //else {
    //ILI9488_WriteData(0x00A8);
    //}

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
    for (i = 0; i < 65535; i++);
    LCD_IO_WriteReg(0x0029);

    ili9320_SetWindows(0, 0, 480, 320);
    LCD_Clear(0x0000);

    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
  }
}

#endif // !TFT_LVGL_UI_SPI

extern uint8_t bmp_public_buf[17 * 1024];

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
    init_tft();
  #endif

  #if ENABLED(SDSUPPORT)
    UpdateAssets();
  #endif
  mks_test_get();

  //spi_flash_read_test();

  TERN_(TOUCH_BUTTONS, touch.init());

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

  systick_attach_callback(SysTick_Callback);

  #if HAS_SPI_FLASH_FONT
    init_gb2312_font();
  #endif

  tft_style_init();

  filament_pin_setup();

  #if ENABLED(POWER_LOSS_RECOVERY)
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
    uint16_t clr_temp;
    uint8_t tbuf[480 * 2];

    SPI_TFT.spi_init(SPI_FULL_SPEED);

    width = area->x2 - area->x1 + 1;
    height = area->y2 - area->y1 + 1;

    for (int j = 0; j < height; j++) {
      SPI_TFT.SetCursor(0, 0);
      SPI_TFT.SetWindows((uint16_t)area->x1, (uint16_t)area->y1 + j, width, 1);
      SPI_TFT.LCD_WriteRAM_Prepare();

      for (i = 0; i < width * 2;) {
        clr_temp = (uint16_t)(((uint16_t)color_p->ch.red << 11)
                              | ((uint16_t)color_p->ch.green << 5)
                              | ((uint16_t)color_p->ch.blue));

        tbuf[i] = clr_temp >> 8;
        tbuf[i + 1] = clr_temp;
        i += 2;
        color_p++;
      }
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(tbuf, width * 2, true);
      SPI_TFT_CS_H;
    }

    lv_disp_flush_ready(disp);       /* Indicate you are ready with the flushing*/

    W25QXX.init(SPI_QUARTER_SPEED);

  #else // !TFT_LVGL_UI_SPI

    #if 1
      uint16_t i, width, height;
      uint16_t clr_temp;
      width = area->x2 - area->x1 + 1;
      height = area->y2 - area->y1 + 1;
      ili9320_SetWindows((uint16_t)area->x1, (uint16_t)area->y1, width, height);
      LCD_WriteRAM_Prepare();
      for (i = 0; i < width * height - 2; i++) {
        clr_temp = (uint16_t)(((uint16_t)color_p->ch.red << 11)
                              | ((uint16_t)color_p->ch.green << 5)
                              | ((uint16_t)color_p->ch.blue));
        LCD_IO_WriteData(clr_temp);
        color_p++;
      }

      lv_disp_flush_ready(disp);       /* Indicate you are ready with the flushing*/
    #endif

  #endif // !TFT_LVGL_UI_SPI
}

#define TICK_CYCLE 1

static int32_t touch_time1 = 0;

unsigned int getTickDiff(unsigned int curTick, unsigned int lastTick) {
  return TICK_CYCLE * (lastTick <= curTick ? (curTick - lastTick) : (0xFFFFFFFF - lastTick + curTick));
}

#if ENABLED(TFT_LVGL_UI_SPI)

  #ifndef USE_XPT2046
    #define USE_XPT2046       1
    #define XPT2046_XY_SWAP   1
    #define XPT2046_X_INV     1
    #define XPT2046_Y_INV     0
  #endif

  #if USE_XPT2046
    #define XPT2046_HOR_RES 480
    #define XPT2046_VER_RES 320
    #define XPT2046_X_MIN   201
    #define XPT2046_Y_MIN   164
    #define XPT2046_X_MAX  3919
    #define XPT2046_Y_MAX  3776
    #define XPT2046_AVG       4
    #define XPT2046_INV       1
  #endif

#else

  #ifndef USE_XPT2046
    #define USE_XPT2046       1
    #ifndef XPT2046_XY_SWAP
      #define XPT2046_XY_SWAP 1
    #endif
    #ifndef XPT2046_X_INV
      #define XPT2046_X_INV   0
    #endif
    #ifndef XPT2046_Y_INV
      #define XPT2046_Y_INV   1
    #endif
  #endif

  #if USE_XPT2046
    #ifndef XPT2046_HOR_RES
      #define XPT2046_HOR_RES 480
    #endif
    #ifndef XPT2046_VER_RES
      #define XPT2046_VER_RES 320
    #endif
    #ifndef XPT2046_X_MIN
      #define XPT2046_X_MIN   201
    #endif
    #ifndef XPT2046_Y_MIN
      #define XPT2046_Y_MIN   164
    #endif
    #ifndef XPT2046_X_MAX
      #define XPT2046_X_MAX  3919
    #endif
    #ifndef XPT2046_Y_MAX
      #define XPT2046_Y_MAX  3776
    #endif
    #ifndef XPT2046_AVG
      #define XPT2046_AVG       4
    #endif
    #ifndef XPT2046_INV
      #define XPT2046_INV       0
    #endif
  #endif

#endif

static void xpt2046_corr(uint16_t *x, uint16_t *y) {
  #if XPT2046_XY_SWAP
    int16_t swap_tmp;
    swap_tmp = *x;
    *x = *y;
    *y = swap_tmp;
  #endif
  if ((*x) > XPT2046_X_MIN) (*x) -= XPT2046_X_MIN; else (*x) = 0;
  if ((*y) > XPT2046_Y_MIN) (*y) -= XPT2046_Y_MIN; else (*y) = 0;
  (*x) = uint32_t(uint32_t(*x) * XPT2046_HOR_RES) / (XPT2046_X_MAX - XPT2046_X_MIN);
  (*y) = uint32_t(uint32_t(*y) * XPT2046_VER_RES) / (XPT2046_Y_MAX - XPT2046_Y_MIN);
  #if XPT2046_X_INV
    (*x) = XPT2046_HOR_RES - (*x);
  #endif
  #if XPT2046_Y_INV
    (*y) = XPT2046_VER_RES - (*y);
  #endif
}

#define times 4
#define CHX   0x90
#define CHY   0xD0

int SPI2_ReadWrite2Bytes(void) {
  #define SPI_READ_WRITE_BYTE(B) TERN(TFT_LVGL_UI_SPI, SPI_TFT.spi_read_write_byte, W25QXX.spi_flash_read_write_byte)(B)
  const uint16_t t1 = SPI_READ_WRITE_BYTE(0xFF),
                 t2 = SPI_READ_WRITE_BYTE(0xFF);
  return (((t1 << 8) | t2) >> 3) & 0x0FFF;
}

uint16_t x_addata[times], y_addata[times];
void XPT2046_Rd_Addata(uint16_t *X_Addata, uint16_t *Y_Addata) {
  uint16_t i, j, k;

  TERN(TFT_LVGL_UI_SPI, SPI_TFT.spi_init, W25QXX.init)(SPI_SPEED_6);

  for (i = 0; i < times; i++) {
    #if ENABLED(TFT_LVGL_UI_SPI)
      OUT_WRITE(TOUCH_CS_PIN, LOW);
      SPI_TFT.spi_read_write_byte(CHX);
      y_addata[i] = SPI2_ReadWrite2Bytes();
      WRITE(TOUCH_CS_PIN, HIGH);

      OUT_WRITE(TOUCH_CS_PIN, LOW);
      SPI_TFT.spi_read_write_byte(CHY);
      x_addata[i] = SPI2_ReadWrite2Bytes();
      WRITE(TOUCH_CS_PIN, HIGH);
    #else // #if ENABLED(TOUCH_BUTTONS)
      OUT_WRITE(TOUCH_CS_PIN, LOW);
      W25QXX.spi_flash_read_write_byte(CHX);
      y_addata[i] = SPI2_ReadWrite2Bytes();
      WRITE(TOUCH_CS_PIN, HIGH);

      OUT_WRITE(TOUCH_CS_PIN, LOW);
      W25QXX.spi_flash_read_write_byte(CHY);
      x_addata[i] = SPI2_ReadWrite2Bytes();
      WRITE(TOUCH_CS_PIN, HIGH);
    #endif

  }
  TERN(TFT_LVGL_UI_SPI,,W25QXX.init(SPI_QUARTER_SPEED));

  for (i = 0; i < times; i++)
    for (j = i + 1; j < times; j++)
      if (x_addata[j] > x_addata[i]) {
        k = x_addata[j];
        x_addata[j] = x_addata[i];
        x_addata[i] = k;
      }
  if (x_addata[times / 2 - 1] - x_addata[times / 2] > 50) {
    *X_Addata = *Y_Addata = 0;
    return;
  }

  *X_Addata = (x_addata[times / 2 - 1] + x_addata[times / 2]) / 2;

  for (i = 0; i < times; i++)
    for (j = i + 1; j < times; j++)
      if (y_addata[j] > y_addata[i]) {
        k = y_addata[j];
        y_addata[j] = y_addata[i];
        y_addata[i] = k;
      }

  if (y_addata[times / 2 - 1] - y_addata[times / 2] > 50) {
    *X_Addata = *Y_Addata = 0;
    return;
  }

  *Y_Addata = (y_addata[times / 2 - 1] + y_addata[times / 2]) / 2;
}

#define ADC_VALID_OFFSET  10

uint8_t TOUCH_PressValid(uint16_t _usX, uint16_t _usY) {
  if ( (_usX <= ADC_VALID_OFFSET)
    || (_usY <= ADC_VALID_OFFSET)
    || (_usX >= 4095 - ADC_VALID_OFFSET)
    || (_usY >= 4095 - ADC_VALID_OFFSET)
  ) return 0;
  return 1;
}

static lv_coord_t last_x = 0, last_y = 0;
bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  uint32_t tmpTime, diffTime = 0;

  tmpTime = millis();
  diffTime = getTickDiff(tmpTime, touch_time1);
  /*Save the state and save the pressed coordinate*/
  //data->state = TOUCH_PressValid(last_x, last_y) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
  //if (data->state == LV_INDEV_STATE_PR)  ADS7843_Rd_Addata((u16 *)&last_x, (u16 *)&last_y);
  //touchpad_get_xy(&last_x, &last_y);
  /*Save the pressed coordinates and the state*/
  if (diffTime > 10) {
    //use marlin touch code if enabled
    #if ENABLED(TOUCH_BUTTONS)
      touch.getTouchPoint(reinterpret_cast<uint16_t&>(last_x), reinterpret_cast<uint16_t&>(last_y));
    #else
      XPT2046_Rd_Addata((uint16_t *)&last_x, (uint16_t *)&last_y);
    #endif
    if (TOUCH_PressValid(last_x, last_y)) {

      data->state = LV_INDEV_STATE_PR;

      /* Set the coordinates (if released use the last pressed coordinates) */

      // SERIAL_ECHOLNPAIR("antes X: ", last_x, ", y: ", last_y);
      xpt2046_corr((uint16_t *)&last_x, (uint16_t *)&last_y);
      // SERIAL_ECHOLNPAIR("X: ", last_x, ", y: ", last_y);
      data->point.x = last_x;
      data->point.y = last_y;

      last_x = 0;
      last_y = 0;
    }
    else {
      data->state = LV_INDEV_STATE_REL;
    }
    touch_time1 = tmpTime;
  }

  return false; /*Return `false` because we are not buffering and no more data to read*/
}

#endif // HAS_TFT_LVGL_UI
