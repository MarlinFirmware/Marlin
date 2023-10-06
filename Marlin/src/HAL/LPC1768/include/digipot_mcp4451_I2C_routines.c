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
 * digipot_mcp4451_I2C_routines.c
 * Adapted from https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/master_8c_source.html
 */

#ifdef TARGET_LPC1768

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DIGIPOT_MCP4451) && MB(MKS_SBASE)

#ifdef __cplusplus
  extern "C" {
#endif

#include "digipot_mcp4451_I2C_routines.h"

uint8_t digipot_mcp4451_start(uint8_t sla) {  // send slave address and write bit
  // Sometimes TX data ACK or NAK status is returned.  That mean the start state didn't
  // happen which means only the value of the slave address was send.  Keep looping until
  // the slave address and write bit are actually sent.
  do {
    _I2C_Stop(I2CDEV_M); // output stop state on I2C bus
    _I2C_Start(I2CDEV_M); // output start state on I2C bus
    while ((I2C_status != I2C_I2STAT_M_TX_START)
        && (I2C_status != I2C_I2STAT_M_TX_RESTART)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_NACK));  //wait for start to be asserted

    LPC_I2C1->I2CONCLR = I2C_I2CONCLR_STAC; // clear start state before tansmitting slave address
    LPC_I2C1->I2DAT = (sla << 1) & I2C_I2DAT_BITMASK; // transmit slave address & write bit
    LPC_I2C1->I2CONSET = I2C_I2CONSET_AA;
    LPC_I2C1->I2CONCLR = I2C_I2CONCLR_SIC;
    while ((I2C_status != I2C_I2STAT_M_TX_SLAW_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_SLAW_NACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_ACK)
        && (I2C_status != I2C_I2STAT_M_TX_DAT_NACK)) { /* wait for slaw to finish */ }
  } while ( (I2C_status == I2C_I2STAT_M_TX_DAT_ACK) || (I2C_status == I2C_I2STAT_M_TX_DAT_NACK));
  return 1;
}

uint8_t digipot_mcp4451_send_byte(uint8_t data) {
  LPC_I2C1->I2DAT = data & I2C_I2DAT_BITMASK; // transmit data
  LPC_I2C1->I2CONSET = I2C_I2CONSET_AA;
  LPC_I2C1->I2CONCLR = I2C_I2CONCLR_SIC;
  while (I2C_status != I2C_I2STAT_M_TX_DAT_ACK && I2C_status != I2C_I2STAT_M_TX_DAT_NACK);  // wait for xmit to finish
  return 1;
}

#ifdef __cplusplus
  }
#endif

#endif // DIGIPOT_MCP4451 && MKS_SBASE
#endif // TARGET_LPC1768
