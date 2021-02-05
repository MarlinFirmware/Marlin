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
#pragma once

#include <HardwareSerial.h>
#include <libmaple/usart.h>
#include <WString.h>

#include "../../inc/MarlinConfigPre.h"
#include "../../core/serial_hook.h"

#if HAS_TFT_LVGL_UI
  extern "C" { extern char public_buf_m[100]; }
#endif

// Increase priority of serial interrupts, to reduce overflow errors
#define UART_IRQ_PRIO 1

struct MarlinSerial : public HardwareSerial {
  MarlinSerial(struct usart_dev *usart_device, uint8 tx_pin, uint8 rx_pin) : HardwareSerial(usart_device, tx_pin, rx_pin) { }

  #ifdef UART_IRQ_PRIO
    // Shadow the parent methods to set IRQ priority after begin()
    void begin(uint32 baud) {
      MarlinSerial::begin(baud, SERIAL_8N1);
    }

    void begin(uint32 baud, uint8_t config) {
      HardwareSerial::begin(baud, config);
      nvic_irq_set_priority(c_dev()->irq_num, UART_IRQ_PRIO);
    }
  #endif

  #if HAS_TFT_LVGL_UI
    // Hook the serial write method to capture the output of GCode command sent via LCD
    uint32_t current_wpos;
    void (*line_callback)(void *, const char * msg);
    void *user_pointer;

    void set_hook(void (*hook)(void *, const char *), void * that) { line_callback = hook; user_pointer = that; current_wpos = 0; }

    size_t write(uint8_t c) {
      if (line_callback) {
        if (c == '\n' || current_wpos == sizeof(public_buf_m) - 1) { // End of line, probably end of command anyway
          public_buf_m[current_wpos] = 0;
          line_callback(user_pointer, public_buf_m);
          current_wpos = 0;
        }
        else
          public_buf_m[current_wpos++] = c;
      }
      return HardwareSerial::write(c);
    }
  #endif
};

typedef Serial0Type<MarlinSerial> MSerialT;

extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;
#if EITHER(STM32_HIGH_DENSITY, STM32_XL_DENSITY)
  extern MSerialT MSerial4;
  extern MSerialT MSerial5;
#endif
