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

#ifndef HAL_SERIAL_H_
#define HAL_SERIAL_H_

class HalSerial {
public:
	Serial serial_port;
	HalSerial(PinName tx, PinName rx): serial_port(tx, rx) {
	}

	void begin(int32_t baud) {
	  serial_port.baud(baud);
	}

	char read() {
    return serial_port.getc();
	}

	void write(char c) {
		serial_port.putc(c);
	}

  operator bool() {
    return true;
  }

	uint16_t available() {
    return serial_port.readable();
	}

	void flush() {
	}

	void print(const char value[]) {
		serial_port.printf("%s" , value);
	}
	void print(char value, int = 0) {
		serial_port.printf("%c" , value);
	}
	void print(unsigned char value, int = 0) {
		serial_port.printf("%u" , value);
	}
	void print(int value, int = 0) {
		serial_port.printf("%d" , value);
	}
	void print(unsigned int value, int = 0) {
		serial_port.printf("%u" , value);
	}
	void print(long value, int = 0) {
		serial_port.printf("%ld" , value);
	}
	void print(unsigned long value, int = 0) {
		serial_port.printf("%lu" , value);
	}

	void print(float value, int round = 6) {
		serial_port.printf("%f" , value);
	}
	void print(double value, int round = 6) {
    serial_port.printf("%f" , value );
	}

	void println(const char value[]) {
		serial_port.printf("%s\n" , value);
	}
	void println(char value, int = 0) {
		serial_port.printf("%c\n" , value);
	}
	void println(unsigned char value, int = 0) {
		serial_port.printf("%u\r\n" , value);
	}
	void println(int value, int = 0) {
		serial_port.printf("%d\n" , value);
	}
	void println(unsigned int value, int = 0) {
		serial_port.printf("%u\n" , value);
	}
	void println(long value, int = 0) {
		serial_port.printf("%ld\n" , value);
	}
	void println(unsigned long value, int = 0) {
		serial_port.printf("%lu\n" , value);
	}
	void println(float value, int round = 6) {
    serial_port.printf("%f\n" , value );
	}
	void println(double value, int round = 6) {
    serial_port.printf("%f\n" , value );
	}
	void println(void) {
		print('\n');
	}

};


#endif /* MARLIN_SRC_HAL_HAL_SERIAL_H_ */
