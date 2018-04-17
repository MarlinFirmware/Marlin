/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "MarlinConfig.h"

#if defined(__AVR__) && defined(USBCON)
  #include <HardwareSerial.h>
  #if ENABLED(BLUETOOTH)
    extern HardwareSerial bluetoothSerial;
    #define PRIM_SERIAL bluetoothSerial
  #else
    #define PRIM_SERIAL Serial
  #endif // BLUETOOTH
#else
  #include "MarlinSerial.h"
  #define PRIM_SERIAL SERIAL_REGNAME(customizedSerial,SERIAL_PORT,)
  #ifdef SEC_SERIAL_PORT
    #define SEC_SERIAL SERIAL_REGNAME(customizedSerial,SEC_SERIAL_PORT,)
  #endif
#endif
// myserial is the currently active serial port
extern Stream *myserial;

extern const char echomagic[] PROGMEM;
extern const char errormagic[] PROGMEM;

#define SERIAL_CHAR(x) ((void)myserial->write(x))
#define SERIAL_EOL() SERIAL_CHAR('\n')

#define SERIAL_PRINT(x,b)      myserial->print(x,b)
#define SERIAL_PRINTLN(x,b)    myserial->println(x,b)
#define SERIAL_PRINTF(args...) myserial->printf(args)

#define SERIAL_FLUSH()         myserial->flush()
#if TX_BUFFER_SIZE > 0
  #define SERIAL_FLUSHTX()     myserial->flushTX()
#endif

#define SERIAL_PROTOCOLCHAR(x)              SERIAL_CHAR(x)
#define SERIAL_PROTOCOL(x)                  myserial->print(x)
#define SERIAL_PROTOCOL_F(x,y)              myserial->print(x,y)
#define SERIAL_PROTOCOLPGM(x)               serialprintPGM(PSTR(x))
#define SERIAL_PROTOCOLLN(x)                do{ myserial->print(x); SERIAL_EOL(); }while(0)
#define SERIAL_PROTOCOLLNPGM(x)             serialprintPGM(PSTR(x "\n"))
#define SERIAL_PROTOCOLPAIR(name, value)    serial_echopair_PGM(PSTR(name),(value))
#define SERIAL_PROTOCOLLNPAIR(name, value)  do{ SERIAL_PROTOCOLPAIR(name, value); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()            serialprintPGM(echomagic)
#define SERIAL_ECHO(x)                 SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x)              SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x)               SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x)            SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_ECHOPAIR(pre,value)     SERIAL_PROTOCOLPAIR(pre, value)
#define SERIAL_ECHOLNPAIR(pre,value)   SERIAL_PROTOCOLLNPAIR(pre, value)
#define SERIAL_ECHO_F(x,y)             SERIAL_PROTOCOL_F(x,y)

#define SERIAL_ERROR_START()           serialprintPGM(errormagic)
#define SERIAL_ERROR(x)                SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x)             SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x)              SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x)           SERIAL_PROTOCOLLNPGM(x)

// These macros compensate for float imprecision
#define SERIAL_PROTOCOLPAIR_F(pre, value)    SERIAL_PROTOCOLPAIR(pre, FIXFLOAT(value))
#define SERIAL_PROTOCOLLNPAIR_F(pre, value)  SERIAL_PROTOCOLLNPAIR(pre, FIXFLOAT(value))
#define SERIAL_ECHOPAIR_F(pre,value)         SERIAL_ECHOPAIR(pre, FIXFLOAT(value))
#define SERIAL_ECHOLNPAIR_F(pre, value)      SERIAL_ECHOLNPAIR(pre, FIXFLOAT(value))

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
FORCE_INLINE void serialprintPGM(const char* str) {
  while (char ch = pgm_read_byte(str++)) SERIAL_CHAR(ch);
}

void serial_echopair_PGM(const char* s_P, const char *v);
void serial_echopair_PGM(const char* s_P, char v);
void serial_echopair_PGM(const char* s_P, int v);
void serial_echopair_PGM(const char* s_P, long v);
void serial_echopair_PGM(const char* s_P, float v);
void serial_echopair_PGM(const char* s_P, double v);
void serial_echopair_PGM(const char* s_P, unsigned int v);
void serial_echopair_PGM(const char* s_P, unsigned long v);
FORCE_INLINE void serial_echopair_PGM(const char* s_P, uint8_t v) { serial_echopair_PGM(s_P, (int)v); }
FORCE_INLINE void serial_echopair_PGM(const char* s_P, uint16_t v) { serial_echopair_PGM(s_P, (int)v); }
FORCE_INLINE void serial_echopair_PGM(const char* s_P, bool v) { serial_echopair_PGM(s_P, (int)v); }
FORCE_INLINE void serial_echopair_PGM(const char* s_P, void *v) { serial_echopair_PGM(s_P, (unsigned long)v); }

void serial_spaces(uint8_t count);
#define SERIAL_ECHO_SP(C)     serial_spaces(C)
#define SERIAL_ERROR_SP(C)    serial_spaces(C)
#define SERIAL_PROTOCOL_SP(C) serial_spaces(C)

#endif // __SERIAL_H__
