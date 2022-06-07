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
 * HAL_LPC1768/include/i2c_util.c
 */

#ifdef TARGET_LPC1768

#include "i2c_util.h"

#define U8G_I2C_OPT_FAST 16  // from u8g.h

#ifdef __cplusplus
  extern "C" {
#endif

void configure_i2c(const uint8_t clock_option) {
  /**
   * Init I2C pin connect
   */
  PINSEL_CFG_Type PinCfg;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  #if I2C_MASTER_ID == 0
    PinCfg.Funcnum = 1;
    PinCfg.Pinnum = 27;     // SDA0 / D57  AUX-1 ... SCL0 / D58  AUX-1
  #elif I2C_MASTER_ID == 1
    PinCfg.Funcnum = 3;
    PinCfg.Pinnum = 0;      // SDA1 / D20 SCA ... SCL1 / D21 SCL
  #elif I2C_MASTER_ID == 2
    PinCfg.Funcnum = 2;
    PinCfg.Pinnum = 10;     // SDA2 / D38  X_ENABLE_PIN ... SCL2 / D55  X_DIR_PIN
  #endif
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum += 1;
  PINSEL_ConfigPin(&PinCfg);

  // Initialize I2C peripheral
  I2C_Init(I2CDEV_M, (clock_option & U8G_I2C_OPT_FAST) ? 400000: 100000);  // LCD data rates

  // Enable Master I2C operation
  I2C_Cmd(I2CDEV_M, I2C_MASTER_MODE, ENABLE);
}

//////////////////////////////////////////////////////////////////////////////////////
// These two routines are exact copies of the lpc17xx_i2c.c routines.  Couldn't link to
// to the lpc17xx_i2c.c routines so had to copy them into this file & rename them.

uint32_t _I2C_Start(LPC_I2C_TypeDef *I2Cx) {
  // Reset STA, STO, SI
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC|I2C_I2CONCLR_STOC|I2C_I2CONCLR_STAC;

  // Enter to Master Transmitter mode
  I2Cx->I2CONSET = I2C_I2CONSET_STA;

  // Wait for complete
  while (!(I2Cx->I2CONSET & I2C_I2CONSET_SI));
  I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
  return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
}

void _I2C_Stop(LPC_I2C_TypeDef *I2Cx) {
  /* Make sure start bit is not active */
  if (I2Cx->I2CONSET & I2C_I2CONSET_STA)
    I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;

  I2Cx->I2CONSET = I2C_I2CONSET_STO|I2C_I2CONSET_AA;
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
}

#ifdef __cplusplus
  }
#endif

#endif // TARGET_LPC1768
