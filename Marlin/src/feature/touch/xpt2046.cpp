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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"

XPT2046 touch;

#include "../../inc/MarlinConfig.h"

extern int8_t encoderDiff;

void XPT2046::swspi_init(void) {
  SET_INPUT(TOUCH_INT); // Pendrive interrupt pin, used as polling in getInTouch()
  SET_INPUT(TOUCH_MISO);
  SET_OUTPUT(TOUCH_MOSI);

  OUT_WRITE(TOUCH_SCK,0);
  OUT_WRITE(TOUCH_CS, 1);

  // This dummy read is needed to enable pendrive status pin
  getInTouch(XPT2046_X);
}

#include "../../lcd/ultralcd.h" // For EN_C bit mask

uint8_t XPT2046::read_buttons() {
  int16_t tsoffsets[4] = { 0 };

  static uint32_t timeout = 0;
  if (PENDING(millis(), timeout)) return 0;
  timeout = millis() + 250;

  if (tsoffsets[0] + tsoffsets[1] == 0) {
    // Not yet set, so use defines as fallback...
    tsoffsets[0] = XPT2046_X_CALIBRATION;
    tsoffsets[1] = XPT2046_X_OFFSET;
    tsoffsets[2] = XPT2046_Y_CALIBRATION;
    tsoffsets[3] = XPT2046_Y_OFFSET;
  }

  // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

  if (READ(TOUCH_INT)) return 0; // If HIGH there are no screen presses.
  const uint16_t x = uint16_t(((uint32_t(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1],
                 y = uint16_t(((uint32_t(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
  if (READ(TOUCH_INT)) return 0; // Fingers must still be on the TS for a valid read.

  if (y < 185 || y > 224) return 0;

       if (WITHIN(x,  21,  98)) encoderDiff = -(ENCODER_STEPS_PER_MENU_ITEM) * ENCODER_PULSES_PER_STEP;
  else if (WITHIN(x, 121, 198)) encoderDiff =   ENCODER_STEPS_PER_MENU_ITEM  * ENCODER_PULSES_PER_STEP;
  else if (WITHIN(x, 221, 298)) return EN_C;

  return 0;
}

uint16_t XPT2046::getInTouch(uint8_t coordinate) {
  uint16_t data[3], delta[3];

  coordinate |= XPT2046_CONTROL | XPT2046_DFR_MODE;

  OUT_WRITE(TOUCH_CS, LOW);

  for (uint16_t i = 0; i < 3 ; i++) {
    for (uint8_t j = 0x80; j; j >>= 1) {
      WRITE(TOUCH_SCK, LOW);
      WRITE(TOUCH_MOSI, bool(coordinate & j));
      WRITE(TOUCH_SCK, HIGH);
    }

    data[i] = 0;
    for (uint16_t j = 0x8000; j; j >>= 1) {
      WRITE(TOUCH_SCK, LOW);
      if (READ(TOUCH_MISO)) data[i] |= j;
      WRITE(TOUCH_SCK, HIGH);
    }
    WRITE(TOUCH_SCK, LOW);
    data[i] >>= 4;
  }

  WRITE(TOUCH_CS, HIGH);

  delta[0] = _MAX(data[0], data[1]) - _MIN(data[0], data[1]);
  delta[1] = _MAX(data[0], data[2]) - _MIN(data[0], data[2]);
  delta[2] = _MAX(data[1], data[2]) - _MIN(data[1], data[2]);

  if (delta[0] <= delta[1] && delta[0] <= delta[2])
    return (data[0] + data[1]) >> 1;

  if (delta[1] <= delta[2])
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

#endif // TOUCH_BUTTONS
