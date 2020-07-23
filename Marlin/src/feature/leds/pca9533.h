/*
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

/*
 * Driver for the PCA9533 LED controller found on the MightyBoard
 * used by FlashForge Creator Pro, MakerBot, etc.
 * Written 2020 APR 01 by grauerfuchs
 */
#include <Arduino.h>

#define ENABLE_I2C_PULLUPS

// Chip address (for Wire)
#define PCA9533_Addr        0xC4

// Control registers
#define PCA9533_REG_READ    0x00
#define PCA9533_REG_PSC0    0x01
#define PCA9533_REG_PWM0    0x02
#define PCA9533_REG_PSC1    0x03
#define PCA9533_REG_PWM1    0x04
#define PCA9533_REG_SEL     0x05
#define PCA9533_REGM_AI     0x10

// LED selector operation
#define PCA9533_LED_OP_OFF  0B00
#define PCA9533_LED_OP_ON   0B01
#define PCA9533_LED_OP_PWM0 0B10
#define PCA9533_LED_OP_PWM1 0B11

// Select register bit offsets for LED colors
#define PCA9533_LED_OFS_RED 0
#define PCA9533_LED_OFS_GRN 2
#define PCA9533_LED_OFS_BLU 4

void PCA9533_init();
void PCA9533_reset();
void PCA9533_setColor(uint8_t red, uint8_t green, uint8_t blue);
void PCA9533_setOff();
