#include "fsmc.h"
#include "st7789v.h"
#include "ili9328.h"
#include "ili9341.h"

#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F

void lcdInit(void);
void lcdSetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax = 319, uint16_t Ymax = 239);
void lcdWriteRam(void);
void lcdPrint(uint16_t x, uint16_t y, char *text);
void lcdFill(uint16_t color, uint32_t count);
void lcdClear(uint16_t color = BLACK);
