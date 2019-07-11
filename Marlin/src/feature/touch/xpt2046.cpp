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

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"

static uint32_t timeout = 0;

extern int8_t encoderDiff;

void touch_xpt2046_swspi_init(void)
{
  SET_INPUT(TOUCH_INT); // Pendrive interrupt pin, used as polling in getInTouch()
  SET_INPUT(TOUCH_MISO);
  SET_OUTPUT(TOUCH_MOSI);

  OUT_WRITE(TOUCH_SCK,0);
  OUT_WRITE(TOUCH_CS, 1);
  // This is a dummy read needed to enable pendrive status pin
  getInTouch(XPT2046_X);
}

uint8_t xpt2046_read_buttons()
{
  uint16_t x, y;
  int16_t tsoffsets[4] = { 0, 0, 0, 0 };

  if (timeout > millis())
    return 0;

  timeout = millis() + 250;

  if (tsoffsets[0] + tsoffsets[1] == 0) {
    // Not yet set, so use defines as fallback...
    tsoffsets[0] = XPT2046_X_CALIBRATION;
    tsoffsets[1] = XPT2046_X_OFFSET;
    tsoffsets[2] = XPT2046_Y_CALIBRATION;
    tsoffsets[3] = XPT2046_Y_OFFSET;
  }

  // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

  if (READ(TOUCH_INT)) { return 0; } // if TOUCH_INT is high, no fingers are pressing on the touch screen > exit
  x = (uint16_t)((((uint32_t)(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1];
  y = (uint16_t)((((uint32_t)(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
  if (READ(TOUCH_INT)) { return 0; } // The fingers still need to be on the TS to have a valid read.

  if (y < 185 || y > 224) { return 0; }
  if (x >  20 && x <  99) encoderDiff = - ENCODER_STEPS_PER_MENU_ITEM * ENCODER_PULSES_PER_STEP;
  if (x > 120 && x < 199) encoderDiff = ENCODER_STEPS_PER_MENU_ITEM * ENCODER_PULSES_PER_STEP;
  if (x > 220 && x < 299) { return EN_C; }

  return 0;
}

uint16_t getInTouch(uint8_t coordinate)
{
  uint16_t data[3], delta[3];

  coordinate |= XPT2046_CONTROL | XPT2046_DFR_MODE;

  OUT_WRITE(TOUCH_CS, LOW);

  for (uint16_t i = 0; i < 3 ; i++)
  {
    for (uint16_t j=0x80; j>0x00; j>>=1) {
      WRITE(TOUCH_SCK, LOW);
      WRITE(TOUCH_MOSI, bool(coordinate & j));
      WRITE(TOUCH_SCK, HIGH);
    }

    data[i] = 0;
    for (uint32_t j=0x8000; j!=0x0000; j>>=1) {
      WRITE(TOUCH_SCK, LOW);
      if (READ(TOUCH_MISO)) data[i] = (data[i] | j);
      WRITE(TOUCH_SCK, HIGH);
    }
    WRITE(TOUCH_SCK, LOW);
    data[i] >>= 4;
  }

  WRITE(TOUCH_CS, HIGH);

  delta[0] = data[0] > data[1] ? data[0] - data[1] : data[1] - data[0];
  delta[1] = data[0] > data[2] ? data[0] - data[2] : data[2] - data[0];
  delta[2] = data[1] > data[2] ? data[1] - data[2] : data[2] - data[1];

  if (delta[0] <= delta[1] && delta[0] <= delta[2])
    return (data[0] + data[1]) >> 1;
  if (delta[1] <= delta[2])
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

#endif // TOUCH_BUTTONS
