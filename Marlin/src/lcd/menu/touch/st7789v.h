#include <stdint.h>

/**
  * @brief  ST7789V Registers
  */
#define ST7789V_LCD_ID             0x04
#define ST7789V_SLEEP_IN           0x10
#define ST7789V_SLEEP_OUT          0x11
#define ST7789V_PARTIAL_DISPLAY    0x12
#define ST7789V_DISPLAY_INVERSION  0x21
#define ST7789V_DISPLAY_ON         0x29
#define ST7789V_WRITE_RAM          0x2C
#define ST7789V_READ_RAM           0x2E
#define ST7789V_CASET              0x2A
#define ST7789V_RASET              0x2B
#define ST7789V_VSCRDEF            0x33 /* Vertical Scroll Definition */
#define ST7789V_VSCSAD             0x37 /* Vertical Scroll Start Address of RAM */
#define ST7789V_TEARING_EFFECT     0x35
#define ST7789V_NORMAL_DISPLAY     0x36
#define ST7789V_IDLE_MODE_OFF      0x38
#define ST7789V_IDLE_MODE_ON       0x39
#define ST7789V_COLOR_MODE         0x3A
#define ST7789V_PORCH_CTRL         0xB2
#define ST7789V_GATE_CTRL          0xB7
#define ST7789V_VCOM_SET           0xBB
#define ST7789V_DISPLAY_OFF        0xBD
#define ST7789V_LCM_CTRL           0xC0
#define ST7789V_VDV_VRH_EN         0xC2
#define ST7789V_VDV_SET            0xC4
#define ST7789V_VCOMH_OFFSET_SET   0xC5
#define ST7789V_FR_CTRL            0xC6
#define ST7789V_POWER_CTRL         0xD0
#define ST7789V_PV_GAMMA_CTRL      0xE0
#define ST7789V_NV_GAMMA_CTRL      0xE1

#define ST7789V_SWRESET            0x01   /* Software Reset */


void st7789v_Init(void);
void st7789v_DisplayOn(void);
void st7789v_WriteRam(void);
void st7789v_SetCursor(uint16_t Xpos, uint16_t Ypos);
void st7789v_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax = 319, uint16_t Ymax = 239);

