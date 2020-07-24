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

/**
 * PCA9533 LED controller driver (MightyBoard, FlashForge Creator Pro, etc.)
 *  by @grauerfuchs - 1 Apr 2020
 */
#include "../../inc/MarlinConfig.h"

#if ENABLED(PCA9533)

#include "pca9533.h"
#include <Wire.h>

void PCA9533_init() {
  Wire.begin();
  PCA9533_reset();
}

static void PCA9533_writeAllRegisters(uint8_t psc0, uint8_t pwm0, uint8_t psc1, uint8_t pwm1, uint8_t ls0){
  uint8_t data[6] = { PCA9533_REG_PSC0 | PCA9533_REGM_AI, psc0, pwm0, psc1, pwm1, ls0 };
  Wire.beginTransmission(PCA9533_Addr >> 1);
  Wire.write(data, 6);
  Wire.endTransmission();
  delayMicroseconds(1);
}

static void PCA9533_writeRegister(uint8_t reg, uint8_t val){
  uint8_t data[2] = { reg, val };
  Wire.beginTransmission(PCA9533_Addr >> 1);
  Wire.write(data, 2);
  Wire.endTransmission();
  delayMicroseconds(1);
}

// Reset (clear) all registers
void PCA9533_reset() {
  PCA9533_writeAllRegisters(0, 0, 0, 0, 0);
}

// Turn all LEDs off
void PCA9533_setOff() {
  PCA9533_writeRegister(PCA9533_REG_SEL, 0);
}

void PCA9533_setColor(uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t r_pwm0 = 0; // Register data - PWM value
  uint8_t r_pwm1 = 0; // Register data - PWM value

  uint8_t op_g = 0, op_r = 0, op_b = 0; // Opcodes - Green, Red, Blue

  // Light theory! GREEN takes priority because
  // it's the most visible to the human eye.
       if (green ==   0)  op_g = PCA9533_LED_OP_OFF;
  else if (green == 255)  op_g = PCA9533_LED_OP_ON;
  else { r_pwm0 = green;  op_g = PCA9533_LED_OP_PWM0; }

  // RED
       if (red ==   0)    op_r = PCA9533_LED_OP_OFF;
  else if (red == 255)    op_r = PCA9533_LED_OP_ON;
  else if (r_pwm0 == 0 || r_pwm0 == red) {
         r_pwm0 = red;    op_r = PCA9533_LED_OP_PWM0;
  }
  else {
         r_pwm1 = red;    op_r = PCA9533_LED_OP_PWM1;
  }

  // BLUE
       if (blue ==   0)   op_b = PCA9533_LED_OP_OFF;
  else if (blue == 255)   op_b = PCA9533_LED_OP_ON;
  else if (r_pwm0 == 0 || r_pwm0 == blue) {
         r_pwm0 = blue;   op_b = PCA9533_LED_OP_PWM0;
  }
  else if (r_pwm1 == 0 || r_pwm1 == blue) {
         r_pwm1 = blue;   op_b = PCA9533_LED_OP_PWM1;
  }
  else {
    /**
     * Conflict. 3 values are requested but only 2 channels exist.
     * G is on channel 0 and R is on channel 1, so work from there.
     * Find the closest match, average the values, then use the free channel.
     */
    uint8_t dgb = ABS(green - blue),
            dgr = ABS(green - red),
            dbr = ABS(blue - red);
    if (dgb < dgr && dgb < dbr) {         // Mix with G on channel 0.
      op_b = PCA9533_LED_OP_PWM0;
      r_pwm0 = uint8_t(((uint16_t)green + (uint16_t)blue) / 2);
    }
    else if (dbr <= dgr && dbr <= dgb) {  // Mix with R on channel 1.
      op_b = PCA9533_LED_OP_PWM1;
      r_pwm1 = uint8_t(((uint16_t)red + (uint16_t)blue) / 2);
    }
    else {                                // Mix R+G on 0 and put B on 1.
      op_r = PCA9533_LED_OP_PWM0;
      r_pwm0 = uint8_t(((uint16_t)green + (uint16_t)red) / 2);
      op_b = PCA9533_LED_OP_PWM1;
      r_pwm1 = blue;
    }
  }

  // Write the changes to the hardware
  PCA9533_writeAllRegisters(0, r_pwm0, 0, r_pwm1,
    (op_g << PCA9533_LED_OFS_GRN) | (op_r << PCA9533_LED_OFS_RED) | (op_b << PCA9533_LED_OFS_BLU)
  );
}

#endif // PCA9533
