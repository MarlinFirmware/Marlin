#include "../../../inc/MarlinConfig.h"

#if defined(ARDUINO_ARCH_STM32F1) && ENABLED(TOUCH_CALIBRATION)

#include "xpt2046.h"
#include "lcd.h"

#ifndef HEATER_BED_PIN
#define HEATER_BED_PIN PA8
#define HEATER_0_PIN   PD3
#define FAN_PIN       PA15
#define LED_PIN        PC2
#endif

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
// __attribute__(( constructor (101))) void premain() {
//    init();
//}
#ifdef LONGER3D
  #include "../../longer/W25Qxx.h"
 // #include "../../longer/stm32f103_flash.h"
  #include "../../longer/LGT_MACRO.h"
  #include  "../../longer/display_image.h"
 // extern void Write_Flash_Data(uint32_t WriteAddress, int16_t data[], uint32_t num);
  extern  W25Qxx W25QxxFlash;
  extern  SAVEDATA flashsave;
#endif
uint16_t color = BLACK, bgColor =WHITE; //BLACK;
uint16_t reg00, lcdId = 0;

/*static*/int16_t xCalibration, yCalibration, xOffset, yOffset;
static uint32_t backlightTimeout;
static uint32_t ledTimeout;
static char text[41];
static char controller[8];

static void drawCross(uint16_t x, uint16_t y, uint16_t color) {
  lcdSetWindow(x - 15, y, x + 15, y); lcdFill(color, 31);
  lcdSetWindow(x, y - 15, x, y + 15); lcdFill(color, 31);
}

int16_t do_calibration(int16_t results[4]) {
  volatile uint32_t data;
  uint32_t i, j;
  uint16_t length;

  uint16_t x[4] = {0,0,0,0};
  uint16_t y[4] = {0,0,0,0};

  pinMode(LED_PIN, OUTPUT); // initialize LED digital pin as an output on Longer3D LK1/LK2 boards
  digitalWrite(LED_PIN, HIGH);

  TOUCH_LCD_BacklightOff();
  TOUCH_LCD_Reset();

  // Set all heaters inactive, as well as Alfawise fan
  pinMode(HEATER_BED_PIN, OUTPUT);
  digitalWrite(HEATER_BED_PIN, LOW);
  pinMode(HEATER_0_PIN, OUTPUT);
  digitalWrite(HEATER_0_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  Serial1.begin(250000);
  Serial1.println("\nSTM32F103VET6");
  Serial1.flush();

#if ENABLED(LCD_USE_DMA_FSMC)
  dma_init(FSMC_DMA_DEV);
  dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
  dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_HIGH);
#endif

  TOUCH_LCD_BacklightOn();

  //TOUCH_LCD_IO_Init();
  //TOUCH_LCD_Delay(100);
  //TOUCH_LCD_Reset();

  Serial1.print("DisplayID ID1 Reg : ");
  reg00 = TOUCH_LCD_IO_ReadReg(0x00);
  if (reg00 == 0) {
    // read ID1 register to get LCD controller ID, MOST of the time located in register 0x04
    data = TOUCH_LCD_IO_ReadData(0x04, 3);
    lcdId = (uint16_t)(data & 0xFFFF);
    Serial1.print((data >> 16) & 0xFF, HEX);
    Serial1.print(" ");
  } else {
    lcdId = reg00;
  }

  //If ID1 is 0, it means we need to check alternate registers, like 0xD3 in the case of ILI9341
  if (lcdId == 0) {
    Serial1.println("");
    Serial1.println("Cannot identify screen controller with reg 0x04 .Trying alternate ID register");
    Serial1.print("DisplayID ID4 reg  : ");
    reg00 = TOUCH_LCD_IO_ReadReg(0x00);
    if (reg00 == 0) {
      // reading ID4 register (0xD3)  to get ILI9341 identifier instead of register ID (0x04)
      data = TOUCH_LCD_IO_ReadData(0xD3, 3);
      lcdId = (uint16_t)(data & 0xFFFF);
      Serial1.print((data >> 16) & 0xFF, HEX);
      Serial1.print(" ");
    } else {
      lcdId = reg00;
    }
  }

  Serial1.print((data >> 8) & 0xFF, HEX);
  Serial1.print(" ");
  Serial1.println(data & 0xFF, HEX);
  Serial1.flush();

  switch(lcdId) {
  case 0x1505: Serial1.println("LCD Controller: R61505U"); sprintf(controller, "R61505U"); break;
  case 0x8552: Serial1.println("LCD Controller: ST7789V"); sprintf(controller, "ST7789V"); break;
  case 0x8989: Serial1.println("LCD Controller: SSD1289"); sprintf(controller, "SSD1289"); break;
  case 0x9325: Serial1.println("LCD Controller: ILI9325"); sprintf(controller, "ILI9325"); break;
  case 0x9328: Serial1.println("LCD Controller: ILI9328"); sprintf(controller, "ILI9328"); break;
  case 0x9341: Serial1.println("LCD Controller: ILI9341"); sprintf(controller, "ILI9341"); break;
  case 0x0404: Serial1.println("No LCD Controller detected"); break;
  default:
    Serial1.print("LCD Controller: Unknown (0x");
    Serial1.print(data & 0xFFFF, HEX);
    Serial1.println(")");
    sprintf(controller, "Unknown");
    break;
  }

  Serial1.flush();
  if (lcdId == 0x0404) return -1;

  lcdInit();

  #ifndef LONGER3D
  color = YELLOW;
  bgColor = BLACK;
  #else
     color = YELLOW;
  bgColor = BLACK;
  #endif
  lcdClear(bgColor);

  /**
   *  Calibrate touch screen
   */
  initTouch_sw_SPI();

  Serial1.println("\nRaw ADC data:");
  for (i = 0; i < 4;) {
    lcdClear(bgColor);

    /**
     * Test coordinates and colors inversion.
     * Draw RED and GREEN squares in top left area of the screen.
     */
    lcdSetWindow(40, 20, 99, 69);
    // #ifndef LONGER3D
    // for (j = 0 ; j < 30; j++) {
    //   lcdFill(RED, 20);
    //   lcdFill(GREEN, 20);
    //   lcdFill(BLUE, 20);
    // }
    // #endif
    #ifdef  LONGER3D 
    length = sprintf(text, TXT_TFT_CONTROLLER_ID, lcdId);
    LCD_ShowString(160 - length * 4, 48, text);
    length = sprintf(text, TXT_TFT_CONTROLLER, controller);
    LCD_ShowString(160 - length * 4, 66, text);

    length = sprintf(text, TXT_TOUCH_CALIBRATION);
    LCD_ShowString(92, 88, text);

    switch (i) {
      case 0:
        drawCross( 20 , 20, 0xFFFF);
        length = sprintf(text, TXT_TOP_LEFT);
        break;
      case 1:
        drawCross( 20, 219, 0xFFFF);
        length = sprintf(text, TXT_BOTTOM_LEFT);
        break;
      case 2:
        drawCross(299,  20, 0xFFFF);
        length = sprintf(text, TXT_TOP_RIGHT);
        break;
      case 3:
        drawCross(299, 219, 0xFFFF);
        length = sprintf(text, TXT_BOTTOM_RIGHT);
        break;
    }

    LCD_ShowString(160 - length * 4, 108, text);
    #else
      length = sprintf(text, "ControllerID:  %04X", lcdId);
      lcdPrint(160 - length * 4, 48, text);
      length = sprintf(text,  "Controller: %s", controller);
      lcdPrint(160 - length * 4, 66, text);

      length = sprintf(text, "Touch calibration");
      lcdPrint(92, 88, text);

    switch (i) {
      case 0:
        drawCross( 20 , 20, 0xFFFF);
        length = sprintf(text, "Top Left");
        break;
      case 1:
        drawCross( 20, 219, 0xFFFF);
        length = sprintf(text,  "Bottom Left");
        break;
      case 2:
        drawCross(299,  20, 0xFFFF);
        length = sprintf(text,  "Top Right");
        break;
      case 3:
        drawCross(299, 219, 0xFFFF);
        length = sprintf(text,  "Bottom Right");
        break;
    }

    lcdPrint(160 - length * 4, 108, text);
    #endif
    waitForTouch(x + i, y + i);

    if ((x[i] < 409 || x[i] > 1637) && (y[i] < 409 || y[i] > 1637)) {
      switch (i) {
        case 0: // Top Left
          i++;
          waitForRelease();
          delay(300);
          continue;
        case 1: // Bottom Left
          if (((x[0] < 409 && x[1] < 409) || (x[0] > 1637 && x[1] > 1637)) && ((y[0] < 409 && y[1] > 1637) || (y[0] > 1637 && y[1] < 409))) {
            i++;
            waitForRelease();
            delay(300);
            continue;
          }
          break;
        case 2: // Top Right
          if (((x[0] < 409 && x[2] > 1637) || (x[0] > 1637 && x[2] < 409)) && ((y[0] < 409 && y[2] < 409) || (y[0] > 1637 && y[2] > 1637))) {
            i++;
            waitForRelease();
            delay(300);
            continue;
          }
          break;
        case 3: // Bottom Right
          if (((x[0] < 409 && x[3] > 1637) || (x[0] > 1637 && x[3] < 409)) && ((y[0] < 409 && y[3] > 1637) || (y[0] > 1637 && y[3] < 409))) {
            i++;
            waitForRelease();
            delay(300);
            continue;
          }
          break;
        }
    }
    delay(1500);
    lcdClear(RED);
    waitForRelease();
    delay(500);
   }

  lcdClear(bgColor);
// #ifndef LONGER3D
//   lcdSetWindow(40, 20, 79, 59);
//   for (j = 0 ; j < 20; j++) {
//     lcdFill(RED, 20);
//     lcdFill(GREEN, 20);
//   }
// #endif
#ifdef LONGER3D
  length = sprintf(text, TXT_TFT_CONTROLLER_ID, lcdId);
  LCD_ShowString(160 - length * 4, 48, text);
  length = sprintf(text,TXT_TFT_CONTROLLER, controller);
  LCD_ShowString(160 - length * 4, 66, text);
  color=GREEN;   //longer
  length = sprintf(text, TXT_CALI_COMPLETED);
  LCD_ShowString(160 - length * 4, 88, text);
   color=YELLOW;   //longer
  // 36569088L == ((int32_t)(299 - 20)) << 17
  xCalibration = (int16_t)(36569088L / ((int32_t)x[3] + (int32_t)x[2] - (int32_t)x[1] - (int32_t)x[0]));
  // 26083328L == ((int32_t)(219 - 20)) << 17
  yCalibration = (int16_t)(26083328L / ((int32_t)y[3] - (int32_t)y[2] + (int32_t)y[1] - (int32_t)y[0]));

  xOffset = (int16_t)(20 - ((((int32_t)(x[0] + x[1])) * (int32_t)xCalibration) >> 17));
  yOffset = (int16_t)(20 - ((((int32_t)(y[0] + y[2])) * (int32_t)yCalibration) >> 17));

  sprintf(text, TXT_X_CALIBRATION);
  LCD_ShowString(76, 108, text);
  sprintf(text, "%6d", xCalibration);
  color = xCalibration >= 0 ? GREEN : RED;
  LCD_ShowString(196, 108, text);
  color = YELLOW;

  sprintf(text, TXT_Y_CALIBRATION);
  LCD_ShowString(76, 124, text);
  sprintf(text, "%6d", yCalibration);
  color = yCalibration >= 0 ? GREEN : RED;
  LCD_ShowString(196, 124, text);
  color = YELLOW;

  sprintf(text, TXT_X_OFFSET);
  LCD_ShowString(76, 140, text);
  sprintf(text, "%6d", xOffset);
  color = xOffset >= 0 ? GREEN : RED;
  LCD_ShowString(196, 140, text);
  color = YELLOW;

  sprintf(text, TXT_Y_OFFSET);
  LCD_ShowString(76, 156, text);
  sprintf(text, "%6d", yOffset);
  color = yOffset >= 0 ? GREEN : RED;
  LCD_ShowString(196, 156, text);
//  #ifdef LONGER3D
   color = RED;
  length = sprintf(text, TXT_PROMPT_INFO1);
  LCD_ShowString(160 - length * 4, 180, text);
  length = sprintf(text, TXT_PROMPT_INFO2);
  LCD_ShowString(160 - length * 4, 198, text);
  color = WHITE;
 // #endif
#else
  // lcdSetWindow(40, 20, 79, 59);
  // for (j = 0 ; j < 20; j++) {
  //   lcdFill(RED, 20);
  //   lcdFill(GREEN, 20);
  // }
  length = sprintf(text, "ControllerID:  %04X", lcdId);
  lcdPrint(160 - length * 4, 48, text);
  length = sprintf(text,"Controller: %s", controller);
  lcdPrint(160 - length * 4, 66, text);
  color=GREEN;   //longer
  length = sprintf(text, "Touch calibration completed");
  lcdPrint(160 - length * 4, 88, text);
   color=YELLOW;   //longer
  // 36569088L == ((int32_t)(299 - 20)) << 17
  xCalibration = (int16_t)(36569088L / ((int32_t)x[3] + (int32_t)x[2] - (int32_t)x[1] - (int32_t)x[0]));
  // 26083328L == ((int32_t)(219 - 20)) << 17
  yCalibration = (int16_t)(26083328L / ((int32_t)y[3] - (int32_t)y[2] + (int32_t)y[1] - (int32_t)y[0]));

  xOffset = (int16_t)(20 - ((((int32_t)(x[0] + x[1])) * (int32_t)xCalibration) >> 17));
  yOffset = (int16_t)(20 - ((((int32_t)(y[0] + y[2])) * (int32_t)yCalibration) >> 17));

  sprintf(text, "X_CALIBRATION:");
  lcdPrint(76, 108, text);
  sprintf(text, "%6d", xCalibration);
  color = xCalibration >= 0 ? GREEN : RED;
  lcdPrint(196, 108, text);
  color = YELLOW;

  sprintf(text, "Y_CALIBRATION:");
  lcdPrint(76, 124, text);
  sprintf(text, "%6d", yCalibration);
  color = yCalibration >= 0 ? GREEN : RED;
  lcdPrint(196, 124, text);
  color = YELLOW;

  sprintf(text,  "X_OFFSET:");
  lcdPrint(76, 140, text);
  sprintf(text, "%6d", xOffset);
  color = xOffset >= 0 ? GREEN : RED;
  lcdPrint(196, 140, text);
  color = YELLOW;

  sprintf(text, "Y_OFFSET:");
  lcdPrint(76, 156, text);
  sprintf(text, "%6d", yOffset);
  color = yOffset >= 0 ? GREEN : RED;
  lcdPrint(196, 156, text);
#endif

  x[0] = (uint16_t)((((int32_t)x[0] * (int32_t)xCalibration) >> 16) + xOffset);
  x[1] = (uint16_t)((((int32_t)x[1] * (int32_t)xCalibration) >> 16) + xOffset);
  x[2] = (uint16_t)((((int32_t)x[2] * (int32_t)xCalibration) >> 16) + xOffset);
  x[3] = (uint16_t)((((int32_t)x[3] * (int32_t)xCalibration) >> 16) + xOffset);
  y[0] = (uint16_t)((((int32_t)y[0] * (int32_t)yCalibration) >> 16) + yOffset);
  y[1] = (uint16_t)((((int32_t)y[1] * (int32_t)yCalibration) >> 16) + yOffset);
  y[2] = (uint16_t)((((int32_t)y[2] * (int32_t)yCalibration) >> 16) + yOffset);
  y[3] = (uint16_t)((((int32_t)y[3] * (int32_t)yCalibration) >> 16) + yOffset);

  Serial1.println("\nCalibrated coordinates:");
  Serial1.print("X: "); Serial1.print(x[0]); Serial1.print("   Y: "); Serial1.println(y[0]);
  Serial1.print("X: "); Serial1.print(x[1]); Serial1.print("   Y: "); Serial1.println(y[1]);
  Serial1.print("X: "); Serial1.print(x[2]); Serial1.print("   Y: "); Serial1.println(y[2]);
  Serial1.print("X: "); Serial1.print(x[3]); Serial1.print("   Y: "); Serial1.println(y[3]);
  Serial1.flush();

  while (!isTouched()) {};

  backlightTimeout = millis() + 60000;

  results[0] = xCalibration;
  results[1] = xOffset;
  results[2] = yCalibration;
  results[3] = yOffset;
  #ifdef LONGER3D
//   for(int i=0;i<4;i++)
//     flashsave.s_result[i]=results[i];
//   flashsave.checksflag=FW_VERSION;
//  //Write_Flash_Data(0,(int16_t*)results,4);
//  //Write_Flash_Data(0,(int16_t*)&flashsave,22);
//   W25QxxFlash.W25QXX_Write((uint8_t*)&flashsave,SAVE_BASE_ADDR,sizeof(SAVEDATA));
// memset(ss,0,sizeof(ss));
//  Read_Flash_Data(0,(int16_t*)&flashsave,22);
//  Serial1.print("ss=");
//  Serial1.println(flashsave.checksflag);
   Serial1.print("xCalibration:");
   Serial1.print(results[0]);
   Serial1.println();
   Serial1.print("yCalibration:");
   Serial1.print(results[2]);
   Serial1.println();
   Serial1.print("xOffset:");
   Serial1.print(results[1]);
   Serial1.println();
   Serial1.print("yOffset:");
   Serial1.print(results[3]);
   bgColor=WHITE;
  #endif
  return 0; // EOK
}

void loop_calibration() {
  uint16_t x, y;

  if (ledTimeout < millis()) {
    digitalWrite(LED_PIN, LOW);
    ledTimeout = millis() + 2000;
  } else if (ledTimeout < millis() + 1000) {
    digitalWrite(LED_PIN, HIGH);
  }

  if (backlightTimeout < millis()) {
    TOUCH_LCD_BacklightOff();
  }

  if (!getTouchPoint(&x, &y)) return;

  if (backlightTimeout < millis()) {
    TOUCH_LCD_BacklightOn();
    waitForRelease();
    delay(200);
    backlightTimeout = millis() + 30000;
    return;
  } else {
    backlightTimeout = millis() + 30000;
  }

  x = (uint16_t)((((int32_t)x * (int32_t)xCalibration) >> 16) + xOffset);
  y = (uint16_t)((((int32_t)y * (int32_t)yCalibration) >> 16) + yOffset);

  lcdSetWindow(x, y);
  TOUCH_LCD_IO_WriteData(color);
}

#endif // TOUCH_CALIBRATION

