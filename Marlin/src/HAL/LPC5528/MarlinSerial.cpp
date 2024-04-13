/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//#include "../platforms.h"

#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

#if USING_HW_SERIAL0
    MarlinSerial _MSerial0(USART0);
    MSerialT MSerial0(true, _MSerial0);
#endif

#if USING_HW_SERIAL1
    MarlinSerial _MSerial1(USART1);
    MSerialT MSerial1(true, _MSerial1);
#endif

#if USING_HW_SERIAL2
    MarlinSerial _MSerial2(USART2);
    MSerialT MSerial2(true, _MSerial2);
#endif

#if USING_HW_SERIAL3
    MarlinSerial _MSerial3(USART3);
    MSerialT MSerial3(true, _MSerial3);

    extern "C" void FLEXCOMM3_IRQHandler() {
        _MSerial3.IRQHandler();
    }

 #endif

void MarlinSerial::begin(uint32_t baud) {
    HardwareSerial::begin(baud);
}

#endif
