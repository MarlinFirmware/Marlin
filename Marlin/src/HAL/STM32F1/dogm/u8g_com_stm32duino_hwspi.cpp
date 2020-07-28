/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef __STM32F1__

#include "../../../inc/MarlinConfig.h"

#if ENABLED(HAS_GRAPHICAL_LCD) && DISABLED(FORCE_SOFT_SPI)

#include "../HAL.h"
#include <U8glib.h>
#include <SPI.h>

#define SPI_TFT_CS_H  OUT_WRITE(SPI_TFT_CS_PIN, HIGH)
#define SPI_TFT_CS_L  OUT_WRITE(SPI_TFT_CS_PIN, LOW)

#define SPI_TFT_DC_H  OUT_WRITE(SPI_TFT_DC_PIN, HIGH)
#define SPI_TFT_DC_L  OUT_WRITE(SPI_TFT_DC_PIN, LOW)

#define SPI_TFT_RST_H OUT_WRITE(SPI_TFT_RST_PIN, HIGH)
#define SPI_TFT_RST_L OUT_WRITE(SPI_TFT_RST_PIN, LOW)

#define SPI_TFT_BLK_H OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH)
#define SPI_TFT_BLK_L OUT_WRITE(LCD_BACKLIGHT_PIN, LOW)

void LCD_IO_Init(uint8_t cs, uint8_t rs);
void LCD_IO_WriteData(uint16_t RegValue);
void LCD_IO_WriteReg(uint16_t Reg);
uint16_t LCD_IO_ReadData(uint16_t RegValue);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
#ifdef LCD_USE_DMA_FSMC
  void LCD_IO_WriteMultiple(uint16_t data, uint32_t count);
  void LCD_IO_WriteSequence(uint16_t *data, uint16_t length);
#endif

void LCD_WR_REG(uint8_t cmd) {
  SPI_TFT_CS_L;
  SPI_TFT_DC_L;
  SPI.send(cmd);
  SPI_TFT_CS_H;
}
void LCD_WR_DATA(uint8_t data) {
  SPI_TFT_CS_L;
  SPI_TFT_DC_H;
  SPI.send(data);
  SPI_TFT_CS_H;
}

uint32_t lcd_id1, lcd_id2, foo, foo1;
void LCD_init() {
  // if (msg == U8G_DEV_MSG_PAGE_NEXT) {
       lcd_id1 = LCD_IO_ReadData(0x0000);
       lcd_id2 = 0;
      int LCD_READ_ID = 0x04;
      if (lcd_id1 == 0 || lcd_id1 == 0xff)
        lcd_id2 = (LCD_READ_ID << 24) | LCD_IO_ReadData(LCD_READ_ID, 3);
       foo = LCD_IO_ReadData(LCD_READ_ID, 3);
       foo1 = LCD_IO_ReadData(LCD_READ_ID);

      // lcd_id1 = LCD_IO_ReadData(0x0000);
      // lcd_id2 = 0;
      // LCD_READ_ID = 0xD3;
      // if (lcd_id1 == 0)
      //   lcd_id2 = (LCD_READ_ID << 24) | LCD_IO_ReadData(LCD_READ_ID, 3);
      // foo = LCD_IO_ReadData(LCD_READ_ID, 3);
      // foo1 = LCD_IO_ReadData(LCD_READ_ID);
      // SERIAL_ECHOLNPAIR("0xD3 ==> lcd_id1: ", lcd_id1, ", lcd_id2: ", lcd_id2, ", foo: ", foo, ", foo1: ", foo1);
  // }

  SPI_TFT_RST_H;
  delay(150);
  SPI_TFT_RST_L;
  delay(150);
  SPI_TFT_RST_H;

  delay(120);
  LCD_WR_REG(0x11);
  delay(120);

  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0xC3);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x96);

  LCD_WR_REG(0x36);
  LCD_WR_DATA(0x28);

  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x55);

  LCD_WR_REG(0xB4);
  LCD_WR_DATA(0x01);
  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0xC6);
  LCD_WR_REG(0xE8);
  LCD_WR_DATA(0x40);
  LCD_WR_DATA(0x8A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x29);
  LCD_WR_DATA(0x19);
  LCD_WR_DATA(0xA5);
  LCD_WR_DATA(0x33);
  LCD_WR_REG(0xC1);
  LCD_WR_DATA(0x06);
  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0xA7);
  LCD_WR_REG(0xC5);
  LCD_WR_DATA(0x18);
  LCD_WR_REG(0xE0);     // Positive Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x15);
  LCD_WR_DATA(0x2F);
  LCD_WR_DATA(0x54);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3C);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xE1);     // Negative Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x2D);
  LCD_WR_DATA(0x43);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3B);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x3C);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x69);
  delay(120);     // Delay 120ms
  LCD_WR_REG(0x29);     // Display ON

  // LCD_clear(0x0000);    //
  // LCD_Draw_Logo();
  SPI_TFT_BLK_H;
  // delay(2000);
}

void LCD_IO_Init(uint8_t cs, uint8_t rs)
{
  static bool init = true;

  SPI_TFT_CS_H;

  /**
   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
   */
  uint8_t clock;
  uint8_t spiRate = SPI_FULL_SPEED;
  switch (spiRate) {
    case SPI_FULL_SPEED:    clock = SPI_CLOCK_DIV4;  break;
    case SPI_HALF_SPEED:    clock = SPI_CLOCK_DIV4; break;
    case SPI_QUARTER_SPEED: clock = SPI_CLOCK_DIV8; break;
    case SPI_EIGHTH_SPEED:  clock = SPI_CLOCK_DIV16; break;
    case SPI_SPEED_5:       clock = SPI_CLOCK_DIV32; break;
    case SPI_SPEED_6:       clock = SPI_CLOCK_DIV64; break;
    default:                clock = SPI_CLOCK_DIV2;        // Default from the SPI library
  }
  SPI.setModule(1);
  SPI.begin();
  SPI.setClockDivider(clock);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  if (init) {
    init = false;
    LCD_init();
  }
}

void LCD_IO_WriteData(uint16_t RegValue)
{
  // LCD_WR_DATA(RegValue >> 8);
  LCD_WR_DATA(RegValue);
}

void LCD_IO_WriteReg(uint16_t Reg)
{
  // LCD_WR_REG(Reg >> 8);
  LCD_WR_REG(Reg);
}

uint16_t LCD_IO_ReadData(uint16_t RegValue)
{
  uint16_t d = 0;

  SPI_TFT_CS_L;
  SPI_TFT_DC_L;
  d = SPI.transfer(RegValue);
  // d = SPI.transfer(0xff);
  // SPI.read((uint8_t*)&d, 1);
  SPI_TFT_CS_H;

  // SPI_TFT_CS_L;
  // SPI.transfer(RegValue);
  // SPI_TFT_CS_H;
  // LCD_WR_REG(RegValue);
  // SPI_TFT_CS_L;
  // // d = SPI.read();
  // SPI.dmaTransfer(NULL, &d, 1);
  // SPI_TFT_CS_H;
  // SPI_TFT_CS_L;
  // SPI_TFT_DC_L;
  // SPI.dmaTransfer(NULL, &d, 1);
  // SPI_TFT_CS_H;
  return d;
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  uint32_t data = 0;
  SPI_TFT_CS_L;
  SPI_TFT_DC_L;
  SPI.transfer(RegValue);
  // data <<= 8;
  // // SPI_TFT_DC_H;
  // data |= SPI.transfer(0xFF) & 0xFF;
  // data <<= 8;
  // data |= SPI.transfer(0xFF) & 0xFF;
  // data <<= 8;
  // data |= SPI.transfer(0xFF) & 0xFF;
  // uint16_t d = 0;
  // SPI.read((uint8_t*)&d, 1);
  // data = d & 0xff;
  // while (--ReadSize) {
  //   data <<= 8;
  //   SPI.read((uint8_t*)&d, 1);
  //   data |= (d & 0xFF);
  // }
  SPI.read((uint8_t*)&data, 4);
  SPI_TFT_CS_H;

  // LCD_WR_REG(RegValue);
  // SPI_TFT_CS_L;
  // SPI.dmaTransfer(NULL, &data, ReadSize);
  // // data = SPI.read() & 0x00FF;
  // // while (--ReadSize) {
  // //   data <<= 8;
  // //   data |= (SPI.read() & 0x00FF);
  // // }
  // SPI_TFT_CS_H;
  // SPI_TFT_CS_L;
  // SPI_TFT_DC_L;
  // SPI.dmaTransfer(NULL, &data, ReadSize);
  // SPI_TFT_CS_H;
  return uint32_t(data);
}

  void LCD_IO_WriteMultiple(uint16_t data, uint32_t count)
  {
    count *= 2;
    while (count > 0) {
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(&data, 1, true);
      SPI_TFT_CS_H;
      count--;
    }
  }

  void LCD_IO_WriteSequence(uint16_t *data, uint16_t length)
  {
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(data, length * 2, true);
      SPI_TFT_CS_H;
  }

  void LCD_IO_WriteSequence_Async(uint16_t *data, uint16_t length) {
    SPI_TFT_CS_L;
    SPI_TFT_DC_H;
    SPI.dmaSendAsync(data, length * 2, true);
    SPI_TFT_CS_H;
  }

  void LCD_IO_WaitSequence_Async() {
    SPI_TFT_CS_L;
    SPI_TFT_DC_H;
    SPI.dmaSendAsync(NULL, 0, true);
    SPI_TFT_CS_H;
  }

static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT

#ifndef LCD_READ_ID
  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
#endif

uint8_t u8g_com_stm32duino_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  static uint8_t isCommand;

  LCD_IO_Init(-1, -1);

  switch (msg) {
    case U8G_COM_MSG_STOP: break;
    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      u8g_Delay(50);

      if (arg_ptr) {
        *((uint32_t *)arg_ptr) = LCD_IO_ReadData(0x0000);
        if (*((uint32_t *)arg_ptr) == 0)
          *((uint32_t *)arg_ptr) = (LCD_READ_ID << 24) | LCD_IO_ReadData(LCD_READ_ID, 3);
      }
      isCommand = 0;
      break;

    case U8G_COM_MSG_ADDRESS: // define cmd (arg_val = 0) or data mode (arg_val = 1)
      isCommand = arg_val == 0 ? 1 : 0;
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      if (isCommand)
        LCD_IO_WriteReg(arg_val);
      else
        LCD_IO_WriteData((uint16_t)arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      for (uint8_t i = 0; i < arg_val; i += 2)
        LCD_IO_WriteData(*(uint16_t *)(((uint32_t)arg_ptr) + i));
      break;

  }
  return 1;
}

#endif // HAS_GRAPHICAL_LCD
#endif // STM32F1
