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
#pragma once

#include <stdint.h>

#include "../../inc/MarlinConfig.h"
#include "../scaled_tft.h"

#define UPSCALE0(M) ((M) * (GRAPHICAL_TFT_UPSCALE))
#define UPSCALE(A,M) (UPSCALE0(M) + (A))

#define BUTTON_DRAW_WIDTH  32
#define BUTTON_DRAW_HEIGHT 20

#define BUTTON_WIDTH  UPSCALE0(BUTTON_DRAW_WIDTH)
#define BUTTON_HEIGHT UPSCALE0(BUTTON_DRAW_HEIGHT)

// calc the space between buttons
#define BUTTON_SPACING (((TFT_WIDTH) - (BUTTON_WIDTH * 4)) / 5)

#define BUTTOND_X_LO BUTTON_SPACING
#define BUTTOND_X_HI BUTTOND_X_LO + BUTTON_WIDTH - 1

#define BUTTONA_X_LO BUTTOND_X_HI + BUTTON_SPACING
#define BUTTONA_X_HI BUTTONA_X_LO + BUTTON_WIDTH - 1

#define BUTTONB_X_LO BUTTONA_X_HI + BUTTON_SPACING
#define BUTTONB_X_HI BUTTONB_X_LO + BUTTON_WIDTH - 1

#define BUTTONC_X_LO BUTTONB_X_HI + BUTTON_SPACING
#define BUTTONC_X_HI BUTTONC_X_LO + BUTTON_WIDTH - 1

#define BUTTON_Y_HI (TFT_HEIGHT) - (BUTTON_SPACING / 2)
#define BUTTON_Y_LO BUTTON_Y_HI - BUTTON_HEIGHT

class TouchButtons {
public:
  static void init();
  static uint8_t read_buttons();
};

extern TouchButtons touch;
