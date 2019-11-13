/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// adapted from  I2C/master/master.c example
//   https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/master_8c_source.html

#ifdef TARGET_LPC1768

extern int millis();

#ifdef __cplusplus
  extern "C" {
#endif

#include <lpc17xx_i2c.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg_default.h>

#include "../../../core/millis_t.h"


//////////////////////////////////////////////////////////////////////////////////////

// These two routines are exact copies of the lpc17xx_i2c.c routines.  Couldn't link to
// to the lpc17xx_i2c.c routines so had to copy them into this file & rename them.

static uint32_t _I2C_Start (LPC_I2C_TypeDef *I2Cx) {
  // Reset STA, STO, SI
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC|I2C_I2CONCLR_STOC|I2C_I2CONCLR_STAC;

  // Enter to Master Transmitter mode
  I2Cx->I2CONSET = I2C_I2CONSET_STA;

  // Wait for complete
  while (!(I2Cx->I2CONSET & I2C_I2CONSET_SI));
  I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
  return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
}

static void _I2C_Stop (LPC_I2C_TypeDef *I2Cx) {
  /* Make sure start bit is not active */
  if (I2Cx->I2CONSET & I2C_I2CONSET_STA)
    I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;

  I2Cx->I2CONSET = I2C_I2CONSET_STO|I2C_I2CONSET_AA;
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
}

//////////////////////////////////////////////////////////////////////////////////////

#define U8G_I2C_OPT_FAST 16  // from u8g.h

#define USEDI2CDEV_M            1

#define I2CDEV_S_ADDR   0x78  // from SSD1306  //actual address is 0x3C - shift left 1 with LSB set to 0 to indicate write

#define BUFFER_SIZE                     0x1  // only do single byte transfers with LCDs

#if (USEDI2CDEV_M == 0)
  #define I2CDEV_M LPC_I2C0
#elif (USEDI2CDEV_M == 1)
  #define I2CDEV_M LPC_I2C1
#elif (USEDI2CDEV_M == 2)
  #define I2CDEV_M LPC_I2C2
#else
  #error "Master I2C device not defined!"
#endif

PINSEL_CFG_Type PinCfg;
I2C_M_SETUP_Type transferMCfg;

#define I2C_status (LPC_I2C1->I2STAT & I2C_STAT_CODE_BITMASK)

uint8_t u8g_i2c_start(uint8_t sla) {  // send slave address and write bit
  // Sometimes TX data ACK or NAK status is returned.  That mean the start state didn't
  // happen which means only the value of the slave address was send.  Keep looping until
  // the slave address and write bit are actually sent.
  do{
    _I2C_Stop(I2CDEV_M); // output stop state on I2C bus
    _I2C_Start(I2CDEV_M); // output start state on I2C bus
    while ((I2C_status != I2C_I2STAT_M_TX_START)
        && (I2C_status != I2C_I2STAT_M_TX_RESTART)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_NACK));  //wait for start to be asserted

    LPC_I2C1->I2CONCLR = I2C_I2CONCLR_STAC; // clear start state before tansmitting slave address
    LPC_I2C1->I2DAT = I2CDEV_S_ADDR & I2C_I2DAT_BITMASK; // transmit slave address & write bit
    LPC_I2C1->I2CONSET = I2C_I2CONSET_AA;
    LPC_I2C1->I2CONCLR = I2C_I2CONCLR_SIC;
    while ((I2C_status != I2C_I2STAT_M_TX_SLAW_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_SLAW_NACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_NACK));  //wait for slaw to finish
  }while ( (I2C_status == I2C_I2STAT_M_TX_DAT_ACK) ||  (I2C_status == I2C_I2STAT_M_TX_DAT_NACK));
  return 1;
}

void u8g_i2c_init(uint8_t clock_option) {

  /**
   * Init I2C pin connect
   */
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  #if ((USEDI2CDEV_M == 0))
    PinCfg.Funcnum = 1;
    PinCfg.Pinnum = 27;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg); // SDA0 / D57  AUX-1
    PinCfg.Pinnum = 28;
    PINSEL_ConfigPin(&PinCfg); // SCL0 / D58  AUX-1
  #endif
  #if ((USEDI2CDEV_M == 1))
    PinCfg.Funcnum = 3;
    PinCfg.Pinnum = 0;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);  // SDA1 / D20 SCA
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);  // SCL1 / D21 SCL
  #endif
  #if ((USEDI2CDEV_M == 2))
    PinCfg.Funcnum = 2;
    PinCfg.Pinnum = 10;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg); // SDA2 / D38  X_ENABLE_PIN
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg); // SCL2 / D55  X_DIR_PIN
  #endif
  // Initialize I2C peripheral
  I2C_Init(I2CDEV_M, (clock_option & U8G_I2C_OPT_FAST) ? 400000: 100000);  // LCD data rates

  // Enable Master I2C operation
  I2C_Cmd(I2CDEV_M, I2C_MASTER_MODE, ENABLE);

  u8g_i2c_start(0); // send slave address and write bit
}

uint8_t u8g_i2c_send_byte(uint8_t data) {
  #define I2C_TIMEOUT 3
  LPC_I2C1->I2DAT = data & I2C_I2DAT_BITMASK; // transmit data
  LPC_I2C1->I2CONSET = I2C_I2CONSET_AA;
  LPC_I2C1->I2CONCLR = I2C_I2CONCLR_SIC;
  const millis_t timeout = millis() + I2C_TIMEOUT;
  while ((I2C_status != I2C_I2STAT_M_TX_DAT_ACK) && (I2C_status != I2C_I2STAT_M_TX_DAT_NACK) && PENDING(millis(), timeout));  // wait for xmit to finish
  // had hangs with SH1106 so added time out - have seen temporary screen corruption when this happens
  return 1;
}

void u8g_i2c_stop() {
}


#ifdef __cplusplus
  }
#endif

#endif // TARGET_LPC1768
