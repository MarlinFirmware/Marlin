#include <stdint.h>

/**
  * @brief  ILI9341 Registers
  */

#define ILI9341_SWRESET            0x01   /* Software Reset */
#define ILI9341_LCD_ID             0xD3
#define ILI9341_SLEEP_IN           0x10
#define ILI9341_SLEEP_OUT          0x11
#define ILI9341_PARTIAL_DISPLAY    0x12
#define ILI9341_DISPLAY_INVERSION  0x21
#define ILI9341_DISPLAY_OFF        0x28
#define ILI9341_DISPLAY_ON         0x29
#define ILI9341_WRITE_RAM          0x2C
#define ILI9341_READ_RAM           0x2E
#define ILI9341_CASET              0x2A
#define ILI9341_RASET              0x2B
#define ILI9341_VSCRDEF            0x33 /* Vertical Scroll Definition */
#define ILI9341_VSCSAD             0x37 /* Vertical Scroll Start Address of RAM */
#define ILI9341_TEARING_EFFECT     0x35
#define ILI9341_NORMAL_DISPLAY     0x36
#define ILI9341_IDLE_MODE_OFF      0x38
#define ILI9341_IDLE_MODE_ON       0x39
#define ILI9341_COLOR_MODE         0x3A

#define ILI9341_ID4                0xD3  // real ID register for 9341


//#define ST7789V_PORCH_CTRL         0xB2
#define ILI9341_BLKING_PORCH_CTRL  0xB5

#define ST7789V_GATE_CTRL          0xB7

//#define ST7789V_VCOM_SET           0xBB
#define ILI9341_VCOM_CTRL1          0xC5
#define ILI9341_VCOM_CTRL2          0xC7

#define ST7789V_LCM_CTRL           0xC0
#define ST7789V_VDV_VRH_EN         0xC2
#define ST7789V_VDV_SET            0xC4
#define ST7789V_VCOMH_OFFSET_SET   0xC5


//#define ST7789V_FR_CTRL            0xC6
#define ILI9341_FR_CTRL           0xB1

//#define ST7789V_POWER_CTRL         0xD0
#define ILI9341_POWER_CTRL1         0xC0
#define ILI9341_POWER_CTRL2         0xC1

#define ST7789V_PV_GAMMA_CTRL      0xE0
#define ST7789V_NV_GAMMA_CTRL      0xE1

void ILI9341_Init(void);
void ILI9341_DisplayOn(void);
void ILI9341_WriteRam(void);
void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);
void ILI9341_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax = 319, uint16_t Ymax = 239);

/*
  ??? 0xEF, 3, 0x03, 0x80, 0x02,
  ??? 0xCF, 3, 0x00, 0xC1, 0x30,
  ??? 0xED, 4, 0x64, 0x03, 0x12, 0x81,
  ??? 0xE8, 3, 0x85, 0x00, 0x78,
  ??? 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  ??? 0xF7, 1, 0x20,
  ??? 0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  */
