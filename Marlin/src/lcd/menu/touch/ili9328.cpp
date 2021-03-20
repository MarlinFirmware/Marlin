#include "../../../inc/MarlinConfig.h"

#if defined(__STM32F1__) && ENABLED(TOUCH_CALIBRATION)

#include "ili9328.h"

#include "fsmc.h"

void ili9328_Init(void) {
/*
  LCD_IO_WriteReg(0x0001); LCD_IO_WriteData(0x0000);  // Driver Output Control 1 // Vertical
  LCD_IO_WriteReg(0x0001); LCD_IO_WriteData(0x0100);  // Driver Output Control 1 // Horizontal

  LCD_IO_WriteReg(0x0060); LCD_IO_WriteData(0x2700);  // Driver Output Control 2 // Vertical
  LCD_IO_WriteReg(0x0060); LCD_IO_WriteData(0xA700);  // Driver Output Control 2 // Horizontal

  LCD_IO_WriteReg(0x0003); LCD_IO_WriteData(0x1038); // Horizontal


  LCD_IO_WriteReg(0x0060);
  LCD_IO_WriteData(0x2700);
  LCD_IO_WriteReg(0x0003);
  LCD_IO_WriteData(0x1038);
*/

//LCD_IO_WriteReg(0x0002);
//LCD_IO_WriteData(0x0700);
//LCD_IO_WriteReg(0x0002); LCD_IO_WriteData(0x0400);  // LCD Driving Control


//LCD_IO_WriteReg(0x00E5); LCD_IO_WriteData(0x78F0);  // unknown


//LCD_IO_WriteReg(0x0001); LCD_IO_WriteData(0x0000);  // Driver Output Control 1
LCD_IO_WriteReg(0x0001); LCD_IO_WriteData(0x0100);  // Driver Output Control 1 // Landscape
// 0000 - bottom left (?)
// 0080 - bottom left (junk)
// 0100 - top left (junk)

LCD_IO_WriteReg(0x0002); LCD_IO_WriteData(0x0400);  // LCD Driving Control

//LCD_IO_WriteReg(0x0003); LCD_IO_WriteData(0x1090);  // Entry Mode
LCD_IO_WriteReg(0x0003); LCD_IO_WriteData(0x1038);  // Entry Mode // Landscape

LCD_IO_WriteReg(0x0004); LCD_IO_WriteData(0x0000);  // Resize Control
LCD_IO_WriteReg(0x0008); LCD_IO_WriteData(0x0202);  // Display Control 2
LCD_IO_WriteReg(0x0009); LCD_IO_WriteData(0x0000);  // Display Control 3
LCD_IO_WriteReg(0x000A); LCD_IO_WriteData(0x0000);  // Display Control 4
LCD_IO_WriteReg(0x000C); LCD_IO_WriteData(0x0000);  // RGB Display Interface Control 1
LCD_IO_WriteReg(0x000D); LCD_IO_WriteData(0x0000);  // Frame Maker Position
LCD_IO_WriteReg(0x000F); LCD_IO_WriteData(0x0000);  // RGB Display Interface Control 2
LCD_IO_WriteReg(0x0010); LCD_IO_WriteData(0x0000);  // Power Control 1
LCD_IO_WriteReg(0x0011); LCD_IO_WriteData(0x0007);  // Power Control 2
LCD_IO_WriteReg(0x0012); LCD_IO_WriteData(0x0000);  // Power Control 3
LCD_IO_WriteReg(0x0013); LCD_IO_WriteData(0x0000);  // Power Control 4
LCD_IO_WriteReg(0x0007); LCD_IO_WriteData(0x0001);  // Display Control 1
LCD_Delay(200);
LCD_IO_WriteReg(0x0010); LCD_IO_WriteData(0x1690);  // Power Control 1
LCD_IO_WriteReg(0x0011); LCD_IO_WriteData(0x0227);  // Power Control 2
LCD_Delay(50);
LCD_IO_WriteReg(0x0012); LCD_IO_WriteData(0x008C);  // Power Control 3
LCD_Delay(50);
LCD_IO_WriteReg(0x0013); LCD_IO_WriteData(0x1500);  // Power Control 4
LCD_IO_WriteReg(0x0029); LCD_IO_WriteData(0x0004);  // Power Control 7
LCD_IO_WriteReg(0x002B); LCD_IO_WriteData(0x000D);  // Frame Rate and Color Control
LCD_Delay(50);


/*
LCD_IO_WriteReg(0x0030); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x0031); LCD_IO_WriteData(0x0607);
LCD_IO_WriteReg(0x0032); LCD_IO_WriteData(0x0305);
LCD_IO_WriteReg(0x0035); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x0036); LCD_IO_WriteData(0x1604);
LCD_IO_WriteReg(0x0037); LCD_IO_WriteData(0x0204);
LCD_IO_WriteReg(0x0038); LCD_IO_WriteData(0x0001);
LCD_IO_WriteReg(0x0039); LCD_IO_WriteData(0x0707);
LCD_IO_WriteReg(0x003C); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x003D); LCD_IO_WriteData(0x000F);
*/

LCD_IO_WriteReg(0x0050); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x0051); LCD_IO_WriteData(0x00EF);
LCD_IO_WriteReg(0x0052); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x0053); LCD_IO_WriteData(0x013F);
LCD_IO_WriteReg(0x0020); LCD_IO_WriteData(0x0000);
LCD_IO_WriteReg(0x0021); LCD_IO_WriteData(0x0000);


LCD_IO_WriteReg(0x0060); LCD_IO_WriteData(0x2700);  // Driver Output Control 2 // Landscape

LCD_IO_WriteReg(0x0061); LCD_IO_WriteData(0x0001);  // Base Image Display Control
LCD_IO_WriteReg(0x006A); LCD_IO_WriteData(0x0000);  // Vertical Scroll Control
LCD_IO_WriteReg(0x0080); LCD_IO_WriteData(0x0000);  // Partial Image 1 Display Position
LCD_IO_WriteReg(0x0081); LCD_IO_WriteData(0x0000);  // Partial Image 1 Area (Start Line)
LCD_IO_WriteReg(0x0082); LCD_IO_WriteData(0x0000);  // Partial Image 1 Area (End Line)
LCD_IO_WriteReg(0x0083); LCD_IO_WriteData(0x0000);  // Partial Image 2 Display Position
LCD_IO_WriteReg(0x0084); LCD_IO_WriteData(0x0000);  // Partial Image 2 Area (Start Line)
LCD_IO_WriteReg(0x0085); LCD_IO_WriteData(0x0000);  // Partial Image 2 Area (End Line)

LCD_IO_WriteReg(0x0090); LCD_IO_WriteData(0x0010);  // Panel Interface Control 1
LCD_IO_WriteReg(0x0092); LCD_IO_WriteData(0x0600);  // Panel Interface Control 2
LCD_IO_WriteReg(0x0007); LCD_IO_WriteData(0x0133);  // Display Control 1

LCD_IO_WriteReg(ILI9328_WRITE_RAM);
}

void ili9328_DisplayOn(void) {}

void ili9328_WriteRam(void) {
  LCD_IO_WriteReg(ILI9328_WRITE_RAM);
}

void ili9328_SetCursor(uint16_t Xpos, uint16_t Ypos) {
  LCD_IO_WriteReg(0x0020);
  LCD_IO_WriteData(Ypos);
  LCD_IO_WriteReg(0x0021);
  LCD_IO_WriteData(Xpos);
  LCD_IO_WriteReg(ILI9328_WRITE_RAM);

}

void ili9328_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax, uint16_t Ymax) {
  LCD_IO_WriteReg(0x0050);
  LCD_IO_WriteData(Ymin);
  LCD_IO_WriteReg(0x0051);
  LCD_IO_WriteData(Ymax);
  LCD_IO_WriteReg(0x0052);
  LCD_IO_WriteData(Xmin);
  LCD_IO_WriteReg(0x0053);
  LCD_IO_WriteData(XMax);

  ili9328_SetCursor(Xmin, Ymin);
}

#endif
