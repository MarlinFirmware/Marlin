/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef TARGET_LPC5528

#include "MarlinSerial.h"

#include "../../inc/MarlinConfig.h"

#ifdef USBCON
  DefaultSerial1 USBSerial(false, UsbSerial);
#endif

#if USING_HW_SERIAL1
  MarlinSerial _MSerial1(USART1);
  MSerialT MSerial1(true, _MSerial1);
  extern "C" void FLEXCOMM1_IRQHandler() { _MSerial1.IRQHandler(); }
#endif

#if USING_HW_SERIAL2
  MarlinSerial _MSerial2(USART2);
  MSerialT MSerial2(true, _MSerial2);
  extern "C" void FLEXCOMM2_IRQHandler() { _MSerial2.IRQHandler(); }
#endif

#if USING_HW_SERIAL3
  MarlinSerial _MSerial3(USART3);
  MSerialT MSerial3(true, _MSerial3);
  extern "C" void FLEXCOMM3_IRQHandler() { _MSerial3.IRQHandler(); }
#endif

#if USING_HW_SERIAL4
  MarlinSerial _MSerial4(USART4);
  MSerialT MSerial4(true, _MSerial4);
  extern "C" void FLEXCOMM4_IRQHandler() { _MSerial4.IRQHandler(); }
#endif

#if USING_HW_SERIAL5
  MarlinSerial _MSerial5(USART5);
  MSerialT MSerial5(true, _MSerial5);
  extern "C" void FLEXCOMM5_IRQHandler() { _MSerial5.IRQHandler(); }
#endif

#if USING_HW_SERIAL6
  MarlinSerial _MSerial6(USART6);
  MSerialT MSerial6(true, _MSerial6);
  extern "C" void FLEXCOMM6_IRQHandler() { _MSerial6.IRQHandler(); }
#endif

#if USING_HW_SERIAL7
  MarlinSerial _MSerial7(USART7);
  MSerialT MSerial7(true, _MSerial7);
  extern "C" void FLEXCOMM7_IRQHandler() { _MSerial7.IRQHandler(); }
#endif

void MarlinSerial::begin(uint32_t baud) {
  HardwareSerial::begin(baud);
}

#if ENABLED(EMERGENCY_PARSER)

  bool MarlinSerial::recv_callback(const char c) {
    // Need to figure out which serial port we are and react in consequence (Marlin does not have CONTAINER_OF macro)
    if (false) {}
    #if USING_HW_SERIAL1
      else if (this == &_MSerial1) emergency_parser.update(MSerial1.emergency_state, c);
    #endif
    #if USING_HW_SERIAL2
      else if (this == &_MSerial2) emergency_parser.update(MSerial2.emergency_state, c);
    #endif
    #if USING_HW_SERIAL3
      else if (this == &_MSerial3) emergency_parser.update(MSerial3.emergency_state, c);
    #endif
    #if USING_HW_SERIAL4
      else if (this == &_MSerial4) emergency_parser.update(MSerial4.emergency_state, c);
    #endif
    #if USING_HW_SERIAL5
      else if (this == &_MSerial5) emergency_parser.update(MSerial5.emergency_state, c);
    #endif
    #if USING_HW_SERIAL6
      else if (this == &_MSerial6) emergency_parser.update(MSerial6.emergency_state, c);
    #endif
    #if USING_HW_SERIAL7
      else if (this == &_MSerial7) emergency_parser.update(MSerial7.emergency_state, c);
    #endif
    return true;
  }

#endif

#endif // TARGET_LPC5528
