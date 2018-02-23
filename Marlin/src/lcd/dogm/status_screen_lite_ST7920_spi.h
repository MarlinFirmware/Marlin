/**
 * Lightweight Status Screen for the RepRapDiscount Full
 * Graphics Smart Controller (ST7920-based 128x64 LCD)
 *
 * (c) 2017 Aleph Objects, Inc.
 *
 * The code in this page is free software: you can
 * redistribute it and/or modify it under the terms of the GNU
 * General Public License (GNU GPL) as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.  The code is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
 *
 */

#include "status_screen_lite_ST7920_class.h"

void ST7920_Lite_Status_Screen::cs() {
  ST7920_CS();
  current_bits.synced = false;
}

void ST7920_Lite_Status_Screen::ncs() {
  ST7920_NCS();
  current_bits.synced = false;
}

void ST7920_Lite_Status_Screen::sync_cmd() {
  ST7920_SET_CMD();
}

void ST7920_Lite_Status_Screen::sync_dat() {
  ST7920_SET_DAT();
}

void ST7920_Lite_Status_Screen::write_byte(const uint8_t data) {
  ST7920_WRITE_BYTE(data);
}
