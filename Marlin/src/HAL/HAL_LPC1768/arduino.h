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

#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <stdint.h>

#define LOW          0x00
#define HIGH         0x01

#define INPUT        0x00
#define OUTPUT       0x01
#define INPUT_PULLUP 0x02

#define _BV(bit) 	(1 << (bit))

typedef uint8_t byte;
#define PROGMEM
#define PSTR(v) (v)
#define PGM_P const char *
#define max(v1, v2) std::max((int)v1,(int)v2)
#define min(v1, v2) std::min((int)v1,(int)v2)
#define abs(v) std::abs(v)
#define sq(v) ((v) * (v))
#define square(v) sq(v)
#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))

//Interrupts
void cli(void); // Disable
void sei(void); // Enable

// Program Memory
#define pgm_read_ptr(address_short) (*(address_short))
#define pgm_read_byte_near(address_short) (*address_short)
#define pgm_read_byte(address_short) pgm_read_byte_near(address_short)
#define pgm_read_float_near(address_short) (*address_short)
#define pgm_read_float(address_short) pgm_read_float_near(address_short)
#define pgm_read_word_near(address_short) (*address_short)
#define pgm_read_word(address_short) pgm_read_word_near(address_short)
#define pgm_read_dword_near(address_short) (*address_short)
#define pgm_read_dword(address_short) pgm_read_dword_near(address_short)

#define sprintf_P sprintf
#define strstr_P strstr
#define strncpy_P strncpy
#define vsnprintf_P vsnprintf
void serialprintPGM(const char *);

// Time functions
void delay(int milis);
void _delay_ms(int delay);
void delayMicroseconds(unsigned long);
uint32_t millis();
uint64_t micros();

//IO functions
void pinMode(int pin_number, int mode);
void digitalWrite(int pin_number, int pin_status);
bool digitalRead(int pin);
void analogWrite(int pin_number, int pin_status);
uint16_t analogRead(int adc_pin);

// EEPROM
void eeprom_write_byte(unsigned char *pos, unsigned char value);
unsigned char eeprom_read_byte(unsigned char *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

#endif // __ARDUINO_DEF_H__
