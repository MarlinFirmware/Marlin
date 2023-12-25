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
#ifdef ARDUINO_ARCH_HC32

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"
#include <drivers/usart/Usart.h>

/**
 * Not every MarlinSerial instance should handle emergency parsing, as
 * it would not make sense to parse GCode from TMC responses
 */
constexpr bool serial_handles_emergency(int port) {
  return false
    #ifdef SERIAL_PORT
         || (SERIAL_PORT) == port
    #endif
    #ifdef SERIAL_PORT_2
         || (SERIAL_PORT_2) == port
    #endif
    #ifdef LCD_SERIAL_PORT
         || (LCD_SERIAL_PORT) == port
    #endif
      ;
}

//
// Define serial ports
//
#define DEFINE_HWSERIAL_MARLIN(name, n)      \
  MSerialT name(serial_handles_emergency(n), \
                &USART##n##_config,          \
                BOARD_USART##n##_TX_PIN,     \
                BOARD_USART##n##_RX_PIN);

DEFINE_HWSERIAL_MARLIN(MSerial1, 1);
DEFINE_HWSERIAL_MARLIN(MSerial2, 2);

//
// Serial port assertions
//

// Check the type of each serial port by passing it to a template function.
// HardwareSerial is known to sometimes hang the controller when an error occurs,
// so this case will fail the static assert. All other classes are assumed to be ok.
template <typename T>
constexpr bool IsSerialClassAllowed(const T &) { return true; }
constexpr bool IsSerialClassAllowed(const HardwareSerial &) { return false; }
constexpr bool IsSerialClassAllowed(const Usart &) { return false; }

// If you encounter this error, replace SerialX with MSerialX, for example MSerial3.
#define CHECK_CFG_SERIAL(A) static_assert(IsSerialClassAllowed(A), STRINGIFY(A) " is defined incorrectly");
#define CHECK_AXIS_SERIAL(A) static_assert(IsSerialClassAllowed(A##_HARDWARE_SERIAL), STRINGIFY(A) "_HARDWARE_SERIAL must be defined in the form MSerial1, rather than Serial1");

// Non-TMC ports were already validated in HAL.h, so do not require verbose error messages.
#ifdef MYSERIAL1
  CHECK_CFG_SERIAL(MYSERIAL1);
#endif
#ifdef MYSERIAL2
  CHECK_CFG_SERIAL(MYSERIAL2);
#endif
#ifdef LCD_SERIAL
  CHECK_CFG_SERIAL(LCD_SERIAL);
#endif
#if AXIS_HAS_HW_SERIAL(X)
  CHECK_AXIS_SERIAL(X);
#endif
#if AXIS_HAS_HW_SERIAL(X2)
  CHECK_AXIS_SERIAL(X2);
#endif
#if AXIS_HAS_HW_SERIAL(Y)
  CHECK_AXIS_SERIAL(Y);
#endif
#if AXIS_HAS_HW_SERIAL(Y2)
  CHECK_AXIS_SERIAL(Y2);
#endif
#if AXIS_HAS_HW_SERIAL(Z)
  CHECK_AXIS_SERIAL(Z);
#endif
#if AXIS_HAS_HW_SERIAL(Z2)
  CHECK_AXIS_SERIAL(Z2);
#endif
#if AXIS_HAS_HW_SERIAL(Z3)
  CHECK_AXIS_SERIAL(Z3);
#endif
#if AXIS_HAS_HW_SERIAL(Z4)
  CHECK_AXIS_SERIAL(Z4);
#endif
#if AXIS_HAS_HW_SERIAL(I)
  CHECK_AXIS_SERIAL(I);
#endif
#if AXIS_HAS_HW_SERIAL(J)
  CHECK_AXIS_SERIAL(J);
#endif
#if AXIS_HAS_HW_SERIAL(K)
  CHECK_AXIS_SERIAL(K);
#endif
#if AXIS_HAS_HW_SERIAL(E0)
  CHECK_AXIS_SERIAL(E0);
#endif
#if AXIS_HAS_HW_SERIAL(E1)
  CHECK_AXIS_SERIAL(E1);
#endif
#if AXIS_HAS_HW_SERIAL(E2)
  CHECK_AXIS_SERIAL(E2);
#endif
#if AXIS_HAS_HW_SERIAL(E3)
  CHECK_AXIS_SERIAL(E3);
#endif
#if AXIS_HAS_HW_SERIAL(E4)
  CHECK_AXIS_SERIAL(E4);
#endif
#if AXIS_HAS_HW_SERIAL(E5)
  CHECK_AXIS_SERIAL(E5);
#endif
#if AXIS_HAS_HW_SERIAL(E6)
  CHECK_AXIS_SERIAL(E6);
#endif
#if AXIS_HAS_HW_SERIAL(E7)
  CHECK_AXIS_SERIAL(E7);
#endif

#endif // ARDUINO_ARCH_HC32
