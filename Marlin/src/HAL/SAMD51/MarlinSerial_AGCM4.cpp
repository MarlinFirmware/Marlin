/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
#ifdef ADAFRUIT_GRAND_CENTRAL_M4

/**
 * Framework doesn't define some serials to save sercom resources
 * hence if these are used I need to define them
 */

#include "../../inc/MarlinConfig.h"

#if ANY_SERIAL_IS(1)
  UartT Serial2(false, &sercom4, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);
  void SERCOM4_0_Handler() { Serial2.IrqHandler(); }
  void SERCOM4_1_Handler() { Serial2.IrqHandler(); }
  void SERCOM4_2_Handler() { Serial2.IrqHandler(); }
  void SERCOM4_3_Handler() { Serial2.IrqHandler(); }
#endif

#if ANY_SERIAL_IS(2)
  UartT Serial3(false, &sercom1, PIN_SERIAL3_RX, PIN_SERIAL3_TX, PAD_SERIAL3_RX, PAD_SERIAL3_TX);
  void SERCOM1_0_Handler() { Serial3.IrqHandler(); }
  void SERCOM1_1_Handler() { Serial3.IrqHandler(); }
  void SERCOM1_2_Handler() { Serial3.IrqHandler(); }
  void SERCOM1_3_Handler() { Serial3.IrqHandler(); }
#endif

#if ANY_SERIAL_IS(3)
  UartT Serial4(false, &sercom5, PIN_SERIAL4_RX, PIN_SERIAL4_TX, PAD_SERIAL4_RX, PAD_SERIAL4_TX);
  void SERCOM5_0_Handler() { Serial4.IrqHandler(); }
  void SERCOM5_1_Handler() { Serial4.IrqHandler(); }
  void SERCOM5_2_Handler() { Serial4.IrqHandler(); }
  void SERCOM5_3_Handler() { Serial4.IrqHandler(); }
#endif

#endif // ADAFRUIT_GRAND_CENTRAL_M4
