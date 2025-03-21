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
 * Based on u8g_dev_ssd1306_128x64.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2015, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * These routines are meant for two wire I2C interfaces.
 *
 * Three and four wire I2C interfaces have an A0 line.  That line is
 * used to switch between command and data modes.
 *
 * The two wire LCDs use an instruction byte to signal if data or
 * command info is to follow.  The command stream needs the instruction
 * byte between eack command byte.  The data stream needs one at the
 * beginning.
 */

#include "../../../inc/MarlinConfigPre.h"

#if HAS_U8GLIB_I2C_OLED

#include "HAL_LCD_com_defines.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

#define CMD_COL_ADR(N)        (0x10 | ((N) >> 4)), ((N) & 0xF)
#define CMD_PAGE_ADR(N)       (0x20), (N)
#define CMD_COLUMN_RANGE(N,O) (0x21), (N), (O)
#define CMD_PAGE_RANGE(N,O)   (0x22), (N), (O)
#define CMD_SCROLL(N)         ((N) ? 0x2F : 0x2E)
#define CMD_START_LINE(N)     (0x40 | (N))
#define CMD_CONTRAST(N)       (0x81), (N)
#define CMD_CHARGE_PUMP(N)    (0x8D), ((N) ? 0x14 : 0x10)
#define CMD_ADC_REVERSE(N)    ((N) ? 0xA1 : 0xA0)
#define CMD_ALL_PIX(N)        ((N) ? 0xA5 : 0xA4)
#define CMD_INVERTED(N)       ((N) ? 0xA7 : 0xA6)
#define CMD_MUX_RATIO(N)      (0xA8), (N)
#define CMD_ON(N)             ((N) ? 0xAF : 0xAE)
#define CMD_OUT_MODE(N)       ((N) ? 0xC8 : 0xC0)
#define CMD_DISP_OFFS(N)      (0xD3), (N)
#define CMD_OSC_FREQ(R,F)     (0xD5), ((F) << 4 | (R))
#define CMD_CHARGE_PER(P,D)   (0xD9), ((D) << 4 | (P))
#define CMD_COM_CONFIG(N)     (0xDA), ((N) ? 0x12 : 0x02)
#define CMD_VCOM_DESEL(N)     (0xDB), (N)
#define CMD_NOOP()            (0xE3)

uint8_t u8g_WriteEscSeqP_2_wire(u8g_t *u8g, u8g_dev_t *dev, const uint8_t *esc_seq);
uint8_t u8g_Write_Init_Sequence_2_wire(u8g_t *u8g, u8g_dev_t *dev, uint32_t length, const uint8_t *init_seq);

// SH1106 is compatible with SSD1306, but is 132x64. Display 128x64 centered within the 132x64.

static const uint8_t u8g_dev_sh1106_128x64_data_start_2_wire[] PROGMEM = {
  CMD_COL_ADR(2),           // Column 2 to center 128 pixels in 132 pixels
  U8G_ESC_END               // End of sequence
};

static const uint8_t u8g_dev_sh1106_128x64_init_seq_2_wire[] PROGMEM = {
  U8G_ESC_ADR(0),           // Initiate command mode
  CMD_ON(0),                // Display off, sleep mode
  CMD_MUX_RATIO(0x3F),      // Mux ratio
  CMD_DISP_OFFS(0),         // Display offset
  CMD_START_LINE(0),        // Start line
  CMD_ADC_REVERSE(1),       // Segment remap A0/A1
  CMD_OUT_MODE(1),          // 0: scan dir normal, 1: reverse
  CMD_COM_CONFIG(1),        // COM pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
  CMD_CONTRAST(0xCF),       // Set contrast control
  CMD_PAGE_ADR(0x02),       // page addressing mode
  CMD_COLUMN_RANGE(2, 129), // Set column range 2 .. 129
  CMD_PAGE_RANGE(0, 7),     // Set page range 0 .. 7
  CMD_CHARGE_PER(0x1, 0xF), // Pre-charge period
  CMD_VCOM_DESEL(0x40),     // Vcomh deselect level
  CMD_ALL_PIX(0),           // Output RAM to display
  CMD_INVERTED(0),          // Normal display mode
  CMD_OSC_FREQ(0, 8),       // Clock divide ratio (0:1) and oscillator frequency (8)
  CMD_CHARGE_PUMP(1),       // Charge pump setting
  CMD_SCROLL(0),            // Deactivate scroll
  CMD_ON(1),                // Display ON
  U8G_ESC_END               // End of sequence
};

uint8_t u8g_dev_sh1106_128x64_2x_2_wire_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_Write_Init_Sequence_2_wire(u8g, dev, COUNT(u8g_dev_sh1106_128x64_init_seq_2_wire), u8g_dev_sh1106_128x64_init_seq_2_wire);
      break;
    case U8G_DEV_MSG_STOP: break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_SetAddress(u8g, dev, 0);                      // Instruction mode
      u8g_WriteEscSeqP_2_wire(u8g, dev, u8g_dev_sh1106_128x64_data_start_2_wire);
      u8g_WriteByte(u8g, dev, 0xB0 | (pb->p.page*2));   // Select current page
      u8g_SetAddress(u8g, dev, 1);                      // Data mode
      u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)pb->buf);
      u8g_SetChipSelect(u8g, dev, 0);
      u8g_SetAddress(u8g, dev, 0);                      // Instruction mode
      u8g_WriteEscSeqP_2_wire(u8g, dev, u8g_dev_sh1106_128x64_data_start_2_wire);
      u8g_WriteByte(u8g, dev, 0xB0 | (pb->p.page*2+1)); // Select current page
      u8g_SetAddress(u8g, dev, 1);                      // Data mode
      u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)(pb->buf)+pb->width);
      u8g_SetChipSelect(u8g, dev, 0);
    } break;
    case U8G_DEV_MSG_SLEEP_ON:
    case U8G_DEV_MSG_SLEEP_OFF: return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_sh1106_128x64_2x_i2c_2_wire_buf[WIDTH * 2] U8G_NOCOMMON;
u8g_pb_t u8g_dev_sh1106_128x64_2x_i2c_2_wire_pb = { { 16, HEIGHT, 0, 0, 0 }, WIDTH, u8g_dev_sh1106_128x64_2x_i2c_2_wire_buf };
u8g_dev_t u8g_dev_sh1106_128x64_2x_i2c_2_wire = { u8g_dev_sh1106_128x64_2x_2_wire_fn, &u8g_dev_sh1106_128x64_2x_i2c_2_wire_pb, U8G_COM_SSD_I2C_HAL };

/////////////////////////////////////////////////////////////////////////////////////////////

static const uint8_t u8g_dev_ssd1306_128x64_data_start_2_wire[] PROGMEM = {
  CMD_COL_ADR(0),           // Column 0
  U8G_ESC_END               // End of sequence
};

static const uint8_t u8g_dev_ssd1306_128x64_init_seq_2_wire[] PROGMEM = {
  CMD_ON(0),                // Display OFF, sleep mode
  CMD_MUX_RATIO(0x3F),      // Mux ratio
  CMD_DISP_OFFS(0),         // Display offset
  CMD_START_LINE(0),        // Start line
  CMD_ADC_REVERSE(1),       // Segment remap A0/A1
  CMD_OUT_MODE(1),          // 0: scan dir normal, 1: reverse
  CMD_COM_CONFIG(1),        // COM pin HW config, sequential COM pin config (bit 4), disable left/right remap (bit 5)
  CMD_CONTRAST(0xCF),       // Set contrast control
  CMD_PAGE_ADR(2),          // Page addressing mode
  CMD_COLUMN_RANGE(0, 127), // Set column range 0 .. 127
  CMD_PAGE_RANGE(0, 7),     // Set page range from 0 .. 7
  CMD_CHARGE_PER(0x1, 0xF), // Pre-charge period
  CMD_VCOM_DESEL(0x40),     // Vcomh deselect level
  CMD_ALL_PIX(0),           // Send RAM to display
  CMD_INVERTED(0),          // Normal display mode
  CMD_OSC_FREQ(0, 8),       // Clock divide ratio (0:1) and oscillator frequency (8)
  CMD_CHARGE_PUMP(1),       // Charge pump setting
  CMD_SCROLL(0),            // Deactivate scroll
  CMD_ON(1),                // Display ON
  U8G_ESC_END               // End of sequence
};

uint8_t u8g_dev_ssd1306_128x64_2x_2_wire_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_Write_Init_Sequence_2_wire(u8g, dev, COUNT(u8g_dev_ssd1306_128x64_init_seq_2_wire), u8g_dev_ssd1306_128x64_init_seq_2_wire);
      break;
    case U8G_DEV_MSG_STOP: break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_SetAddress(u8g, dev, 0);                      // Instruction mode
      u8g_WriteEscSeqP_2_wire(u8g, dev, u8g_dev_ssd1306_128x64_data_start_2_wire);
      u8g_WriteByte(u8g, dev, 0xB0 | (pb->p.page*2));   // Select current page
      u8g_SetAddress(u8g, dev, 1);                      // Data mode
      u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)pb->buf);
      u8g_SetChipSelect(u8g, dev, 0);
      u8g_SetAddress(u8g, dev, 0);                      // Instruction mode
      u8g_WriteEscSeqP_2_wire(u8g, dev, u8g_dev_ssd1306_128x64_data_start_2_wire);
      u8g_WriteByte(u8g, dev, 0xB0 | (pb->p.page*2+1)); // Select current page
      u8g_SetAddress(u8g, dev, 1);                      // Data mode
      u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)(pb->buf)+pb->width);
      u8g_SetChipSelect(u8g, dev, 0);
    } break;
    case U8G_DEV_MSG_SLEEP_ON:
    case U8G_DEV_MSG_SLEEP_OFF: return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1306_128x64_2x_i2c_2_wire_buf[WIDTH * 2] U8G_NOCOMMON;
u8g_pb_t u8g_dev_ssd1306_128x64_2x_i2c_2_wire_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1306_128x64_2x_i2c_2_wire_buf };
u8g_dev_t u8g_dev_ssd1306_128x64_2x_i2c_2_wire = { u8g_dev_ssd1306_128x64_2x_2_wire_fn, &u8g_dev_ssd1306_128x64_2x_i2c_2_wire_pb, U8G_COM_SSD_I2C_HAL };

/////////////////////////////////////////////////////////////////////////////////////////////

// This routine adds the instruction byte in between the command bytes.
// This makes the init sequences a lot easier to read.

#define I2C_CMD_MODE    0x80

uint8_t u8g_WriteEscSeqP_2_wire(u8g_t *u8g, u8g_dev_t *dev, const uint8_t *esc_seq) {
  uint8_t is_escape = 0;
  for (;;) {
    uint8_t value = u8g_pgm_read(esc_seq);
    if (is_escape == 0) {
      if (value != 0xFF) {
        if (u8g_WriteByte(u8g, dev, value) == 0) return 0;
        if (u8g_WriteByte(u8g, dev, I2C_CMD_MODE) == 0) return 0;
      }
      else {
        is_escape = 1;
      }
    }
    else {
      if (value == 0xFF) {
        if (u8g_WriteByte(u8g, dev, value) == 0) return 0;
        if (u8g_WriteByte(u8g, dev, I2C_CMD_MODE) == 0) return 0;
      }
      else if (value == 0xFE) {
        break;
      }
      else if (value >= 0xF0) {
        // Not yet used, do nothing
      }
      else if (value >= 0xE0) {
        u8g_SetAddress(u8g, dev, value & 0x0F);
      }
      else if (value >= 0xD0) {
        u8g_SetChipSelect(u8g, dev, value & 0x0F);
      }
      else if (value >= 0xC0) {
        u8g_SetResetLow(u8g, dev);
        value &= 0x0F;
        value <<= 4;
        value += 2;
        u8g_Delay(value);
        u8g_SetResetHigh(u8g, dev);
        u8g_Delay(value);
      }
      else if (value >= 0xBE) {                       // Not yet implemented
        //u8g_SetVCC(u8g, dev, value & 0x01);
      }
      else if (value <= 0x7F) {
        u8g_Delay(value);
      }
      is_escape = 0;
    }
    esc_seq++;
  }
  return 1;
}

uint8_t u8g_Write_Init_Sequence_2_wire(u8g_t *u8g, u8g_dev_t *dev, uint32_t length, const uint8_t *init_seq) {
  u8g_SetAddress(u8g, dev, 0);  // Instruction mode
  u8g_WriteSequence(u8g, dev, length, (uint8_t*)init_seq);
  return 1;
}

#endif // HAS_U8GLIB_I2C_OLED
