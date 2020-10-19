/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_CALIBRATION)

#include "../../lcd/touch/xpt2046.h"
#include "../../module/temperature.h"
#include "../../lcd/ultralcd.h" // ui.xxx
#include "calibration.h"
#include "lcd.h"

TouchCalibration::TouchCalibration() {
  memset(&results[0], 0, sizeof(results));
}

void TouchCalibration::init_calibration(const uint8_t init_state) {
  calibration_state = init_state;
  ui.defer_status_screen();
  do_calibration(results);
  exit_calibration();
}

void TouchCalibration::exit_calibration() {
  ui.init();
  ui.goto_previous_screen_no_defer();
}

//#define SERIAL_DEBUG
#define TEXT_COLOR DARKCYAN

// variables used in lcd.cpp
uint16_t color = WHITE, bgColor = BLACK;
uint16_t reg00, lcdId = 0;

static void drawCross(uint16_t x, uint16_t y, uint16_t color) {
  lcdSetWindow(x - 15, y, x + 15, y); lcdFill(color, 31);
  lcdSetWindow(x, y - 15, x, y + 15); lcdFill(color, 31);
}

int16_t TouchCalibration::do_calibration(int16_t results[4]) {
  volatile uint32_t data;
  size_t length;
  char text[41];

  int16_t xCalibration, yCalibration, xOffset, yOffset;
  uint16_t x[4] = {0,0,0,0};
  uint16_t y[4] = {0,0,0,0};

  lcdBacklightOff();
  lcdReset();

  // Turn off mosfets
  #if HAS_HEATED_BED
    thermalManager.setTargetBed(0);
  #endif
  thermalManager.setTargetHotend(0, 0);
  #if FAN_COUNT > 0
    for (uint8_t fan=0; fan < FAN_COUNT; fan++)
      thermalManager.set_fan_speed(fan, 0);
  #endif

  #ifdef SERIAL_DEBUG
    //Serial1.begin(BAUDRATE);
    Serial1.println("\nCalibration");
    Serial1.flush();
  #endif

  #if ENABLED(LCD_USE_DMA_FSMC)
    dma_init(FSMC_DMA_DEV);
    dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);
  #endif

  lcdBacklightOn();

  //TOUCH_LCD_IO_Init();
  //delay(100);
  //lcdReset();

  reg00 = TOUCH_LCD_IO_ReadReg(0x00);
  if (reg00 == 0) {
    // read ID1 register to get LCD controller ID, MOST of the time located in register 0x04
    data = TOUCH_LCD_IO_ReadData(0x04, 3);
    lcdId = (uint16_t)(data & 0xFFFF);
    #ifdef SERIAL_DEBUG
      Serial1.print("DisplayID ID1 Reg : ");
      Serial1.println((data >> 16) & 0xFF, HEX);
    #endif
  } else {
    lcdId = reg00;
  }

  //If ID1 is 0, it means we need to check alternate registers, like 0xD3 in the case of ILI9341
  if (lcdId == 0) {
    #ifdef SERIAL_DEBUG
      Serial1.println("Cannot identify screen controller with reg 0x04, Trying alt. ID4 register");
    #endif
    reg00 = TOUCH_LCD_IO_ReadReg(0x00);
    if (reg00 == 0) {
      // reading ID4 register (0xD3)  to get ILI9341 identifier instead of register ID (0x04)
      data = TOUCH_LCD_IO_ReadData(0xD3, 3);
      lcdId = (uint16_t)(data & 0xFFFF);
      #ifdef SERIAL_DEBUG
        Serial1.print("DisplayID ID4 reg : ");
        Serial1.println((data >> 16) & 0xFF, HEX);
      #endif
    } else {
      lcdId = reg00;
    }
  }

  char controller[8];

  switch(lcdId) {
  case 0x1505: sprintf(controller, "R61505U"); break;
  case 0x8552: sprintf(controller, "ST7789V"); break;
  case 0x8989: sprintf(controller, "SSD1289"); break;
  case 0x9325: sprintf(controller, "ILI9325"); break;
  case 0x9328: sprintf(controller, "ILI9328"); break;
  case 0x9341: sprintf(controller, "ILI9341"); break;
  case 0x0404: sprintf(controller, "NONE");    break;
  default:     sprintf(controller, "Unknown");
  }

  #ifdef SERIAL_DEBUG
    Serial1.print("LCD Controller: ");
    Serial1.print(controller);
    Serial1.print(" ");
    Serial1.println(data & 0xFFFF, HEX);
    Serial1.flush();
  #endif

  if (lcdId == 0x0404) return -1;

  lcdInit();

  color = TEXT_COLOR;
  bgColor = BLACK;
  lcdClear(bgColor);

  /**
   *  Calibrate touch screen
   */
  touch.init();

  #ifdef SERIAL_DEBUG
    Serial1.println("\nRaw ADC data:");
  #endif
  for (uint8_t i = 0; i < 4;) {
    lcdClear(bgColor);

    /**
     * Test coordinates and colors.
     * Draw RGB squares in bottom center area of the screen.
     */
    lcdSetWindow(130, 200, 189, 239);
    for (uint8_t j = 0 ; j < 20; j++) {
      lcdFill(RED, 20);
      lcdFill(GREEN, 20);
      lcdFill(BLUE, 20);
    }

    length = sprintf(text, "ControllerID:  %04X", lcdId);
    lcdPrint(160 - length * 4, 28, text);
    length = sprintf(text, "Controller: %s", controller);
    lcdPrint(160 - length * 4, 46, text);

    length = sprintf(text, "Touch calibration");
    lcdPrint(92, 88, text);

    switch (i) {
      case 0:
        drawCross( 20,  20, WHITE);
        length = sprintf(text, "Top Left");
        break;
      case 1:
        drawCross( 20, 219, WHITE);
        length = sprintf(text, "Bottom Left");
        break;
      case 2:
        drawCross(299,  20, WHITE);
        length = sprintf(text, "Top Right");
        break;
      case 3:
        drawCross(299, 219, WHITE);
        length = sprintf(text, "Bottom Right");
        break;
    }

    lcdPrint(160 - length * 4, 108, text);

    touch.waitForTouch(x[i], y[i]);

    length = sprintf(text, "X:0x%03X Y:0x%03X", x[i], y[i]);
    lcdPrint(160 - length * 4, 148, text);

    if ((x[i] < 409 || x[i] > 1637) && (y[i] < 409 || y[i] > 1637)) {
      switch (i) {
        case 0: // Top Left
          i++;
          touch.waitForRelease();
          delay(300);
          continue;
        case 1: // Bottom Left
          if (((x[0] < 409 && x[1] < 409) || (x[0] > 1637 && x[1] > 1637)) && ((y[0] < 409 && y[1] > 1637) || (y[0] > 1637 && y[1] < 409))) {
            i++;
            touch.waitForRelease();
            delay(300);
            continue;
          }
          break;
        case 2: // Top Right
          if (((x[0] < 409 && x[2] > 1637) || (x[0] > 1637 && x[2] < 409)) && ((y[0] < 409 && y[2] < 409) || (y[0] > 1637 && y[2] > 1637))) {
            i++;
            touch.waitForRelease();
            delay(300);
            continue;
          }
          break;
        case 3: // Bottom Right
          if (((x[0] < 409 && x[3] > 1637) || (x[0] > 1637 && x[3] < 409)) && ((y[0] < 409 && y[3] > 1637) || (y[0] > 1637 && y[3] < 409))) {
            i++;
            touch.waitForRelease();
            delay(300);
            continue;
          }
          break;
        }
    }
    delay(1500);
    lcdClear(RED);
    touch.waitForRelease();
    delay(500);
  }

  lcdClear(bgColor);

  length = sprintf(text, "ControllerID:  %04X", lcdId);
  lcdPrint(160 - length * 4, 28, text);
  length = sprintf(text, "Controller: %s", controller);
  lcdPrint(160 - length * 4, 46, text);

  length = sprintf(text, "Touch calibration completed");
  lcdPrint(160 - length * 4, 88, text);

  // 36569088L == ((int32_t)(299 - 20)) << 17
  xCalibration = (int16_t)(36569088L / ((int32_t)x[3] + (int32_t)x[2] - (int32_t)x[1] - (int32_t)x[0]));
  // 26083328L == ((int32_t)(219 - 20)) << 17
  yCalibration = (int16_t)(26083328L / ((int32_t)y[3] - (int32_t)y[2] + (int32_t)y[1] - (int32_t)y[0]));

  xOffset = (int16_t)(20 - ((((int32_t)(x[0] + x[1])) * (int32_t)xCalibration) >> 17));
  yOffset = (int16_t)(20 - ((((int32_t)(y[0] + y[2])) * (int32_t)yCalibration) >> 17));

  color = TEXT_COLOR;
  sprintf(text, "X_CALIBRATION:");
  lcdPrint(76, 108, text);
  sprintf(text, "%6d", xCalibration);
  color = xCalibration >= 0 ? GREEN : RED;
  lcdPrint(196, 108, text);

  color = TEXT_COLOR;
  sprintf(text, "Y_CALIBRATION:");
  lcdPrint(76, 124, text);
  sprintf(text, "%6d", yCalibration);
  color = yCalibration >= 0 ? GREEN : RED;
  lcdPrint(196, 124, text);

  color = TEXT_COLOR;
  sprintf(text, "X_OFFSET:");
  lcdPrint(76, 140, text);
  sprintf(text, "%6d", xOffset);
  color = xOffset >= 0 ? GREEN : RED;
  lcdPrint(196, 140, text);

  color = TEXT_COLOR;
  sprintf(text, "Y_OFFSET:");
  lcdPrint(76, 156, text);
  sprintf(text, "%6d", yOffset);
  color = yOffset >= 0 ? GREEN : RED;
  lcdPrint(196, 156, text);

  color = WHITE;
  length = sprintf(text, "Touch the screen to exit.");
  lcdPrint(160 - length * 4, 200, text);

  x[0] = (uint16_t)((((int32_t)x[0] * (int32_t)xCalibration) >> 16) + xOffset);
  x[1] = (uint16_t)((((int32_t)x[1] * (int32_t)xCalibration) >> 16) + xOffset);
  x[2] = (uint16_t)((((int32_t)x[2] * (int32_t)xCalibration) >> 16) + xOffset);
  x[3] = (uint16_t)((((int32_t)x[3] * (int32_t)xCalibration) >> 16) + xOffset);
  y[0] = (uint16_t)((((int32_t)y[0] * (int32_t)yCalibration) >> 16) + yOffset);
  y[1] = (uint16_t)((((int32_t)y[1] * (int32_t)yCalibration) >> 16) + yOffset);
  y[2] = (uint16_t)((((int32_t)y[2] * (int32_t)yCalibration) >> 16) + yOffset);
  y[3] = (uint16_t)((((int32_t)y[3] * (int32_t)yCalibration) >> 16) + yOffset);

  #ifdef SERIAL_DEBUG
    Serial1.println("\nCalibrated coordinates:");
    Serial1.print("X: "); Serial1.print(x[0]); Serial1.print("   Y: "); Serial1.println(y[0]);
    Serial1.print("X: "); Serial1.print(x[1]); Serial1.print("   Y: "); Serial1.println(y[1]);
    Serial1.print("X: "); Serial1.print(x[2]); Serial1.print("   Y: "); Serial1.println(y[2]);
    Serial1.print("X: "); Serial1.print(x[3]); Serial1.print("   Y: "); Serial1.println(y[3]);
    Serial1.flush();
  #endif

  while (!touch.isTouched()) {};

  results[0] = xCalibration;
  results[1] = xOffset;
  results[2] = yCalibration;
  results[3] = yOffset;

  return 0; // EOK
}

#endif // TOUCH_CALIBRATION
