/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

/**
 * pins/lcd/TFTGLCD.h
 * TFTGLCD Panel — SPI CS pin default (EXP2 pin 3).
 * I2C mode needs no board pins.
 *
 * Boards may override TFTGLCD_CS before this file is included by defining it inline
 * in the board file (e.g. non-standard wiring with EXP1_03_PIN or EXP2_05_PIN).
 *
 * Board files with SD detection on the TFTGLCD connector must retain an inline
 * IS_TFTGLCD_PANEL block to define SD_DETECT_PIN (board-specific).
 */

#if ENABLED(TFTGLCD_PANEL_SPI)
  #ifndef TFTGLCD_CS
    #define TFTGLCD_CS              EXP2_03_PIN
  #endif
#endif
