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

#if ENABLED(SPI_GRAPHICAL_TFT) && DISABLED(FORCE_SOFT_SPI)

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
#ifdef LCD_USE_DMA_SPI
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

void spi1Init(uint8_t spiRate) {
  SPI_TFT_CS_H;

  /**
   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
   */
  uint8_t clock;
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
}

void LCD_IO_Init(uint8_t cs, uint8_t rs) {
  spi1Init(SPI_FULL_SPEED);
}

void LCD_IO_WriteData(uint16_t RegValue) {
  LCD_WR_DATA(RegValue);
}

void LCD_IO_WriteReg(uint16_t Reg) {
  LCD_WR_REG(Reg);
}

uint16_t LCD_IO_ReadData(uint16_t RegValue) {
  uint16_t d = 0;
  SPI_TFT_CS_L;

  SPI_TFT_DC_L;
  SPI.send(RegValue);
  SPI_TFT_DC_H;

  SPI.read((uint8_t*)&d, 1); //dummy read
  SPI.read((uint8_t*)&d, 1);

  SPI_TFT_CS_H;
  return d >> 7;
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) {
  uint32_t data = 0;
  uint8_t d = 0;
  SPI_TFT_CS_L;

  SPI_TFT_DC_L;
  SPI.send(RegValue);
  SPI_TFT_DC_H;

  SPI.read((uint8_t*)&d, 1); //dummy read
  SPI.read((uint8_t*)&d, 1);
  data = d;
  while (--ReadSize) {
    data <<= 8;
    SPI.read((uint8_t*)&d, 1);
    data |= (d & 0xFF);
  }

  SPI_TFT_CS_H;
  return uint32_t(data >> 7);
}

#ifdef LCD_USE_DMA_SPI
  void LCD_IO_WriteMultiple(uint16_t data, uint32_t count) {
    if (SPI.getDataSize() == DATA_SIZE_8BIT) {
      count *= 2;
    }
    while (count > 0) {
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(&data, 1, true);
      SPI_TFT_CS_H;
      count--;
    }
  }

  void LCD_IO_WriteSequence(uint16_t *data, uint16_t length) {
    if (SPI.getDataSize() == DATA_SIZE_8BIT) {
      length *= 2;
    }
    SPI_TFT_CS_L;
    SPI_TFT_DC_H;
    SPI.dmaSend(data, length, true);
    SPI_TFT_CS_H;
  }

  void LCD_IO_WriteSequence_Async(uint16_t *data, uint16_t length) {
    if (SPI.getDataSize() == DATA_SIZE_8BIT) {
      length *= 2;
    }
    SPI_TFT_CS_L;
    SPI_TFT_DC_H;
    SPI.dmaSendAsync(data, length, true);
    SPI_TFT_CS_H;
  }

  void LCD_IO_WaitSequence_Async() {
    SPI_TFT_CS_L;
    SPI_TFT_DC_H;
    SPI.dmaSendAsync(NULL, 0, true);
    SPI_TFT_CS_H;
  }
#endif

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
        spi1Init(SPI_EIGHTH_SPEED);
        *((uint32_t *)arg_ptr) = (LCD_READ_ID << 24) | LCD_IO_ReadData(LCD_READ_ID, 3);
        spi1Init(SPI_FULL_SPEED);
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

#endif // SPI_GRAPHICAL_TFT && !FORCE_SOFT_SPI
#endif // STM32F1
