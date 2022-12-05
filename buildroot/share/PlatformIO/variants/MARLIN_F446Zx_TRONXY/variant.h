/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern unsigned long myvar[];
void myshow(int fre, int times);
void debugStr(const char *str);

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

#define PA0  0x00
#define PA1  0x01
#define PA2  0x02
#define PA3  0x03
#define PA4  0x04
#define PA5  0x05
#define PA6  0x06
#define PA7  0x07
#define PA8  0x08
#define PA9  0x09
#define PA10 0x0A
#define PA11 0x0B
#define PA12 0x0C
#define PA13 0x0D
#define PA14 0x0E
#define PA15 0x0F

#define PB0  0x10
#define PB1  0x11
#define PB2  0x12
#define PB3  0x13
#define PB4  0x14
#define PB5  0x15
#define PB6  0x16
#define PB7  0x17 // 36 pins (F103T)
#define PB8  0x18
#define PB9  0x19
#define PB10 0x1A
#define PB11 0x1B
#define PB12 0x1C
#define PB13 0x1D
#define PB14 0x1E
#define PB15 0x1F

#define PC0  0x20
#define PC1  0x21
#define PC2  0x22
#define PC3  0x23
#define PC4  0x24
#define PC5  0x25
#define PC6  0x26
#define PC7  0x27
#define PC8  0x28
#define PC9  0x29
#define PC10 0x2A
#define PC11 0x2B
#define PC12 0x2C
#define PC13 0x2D
#define PC14 0x2E
#define PC15 0x2F

#define PD0  0x30
#define PD1  0x31
#define PD2  0x32 // 64 pins (F103R)
#define PD3  0x33
#define PD4  0x34
#define PD5  0x35
#define PD6  0x36
#define PD7  0x37
#define PD8  0x38
#define PD9  0x39
#define PD10 0x3A
#define PD11 0x3B
#define PD12 0x3C
#define PD13 0x3D
#define PD14 0x3E
#define PD15 0x3F

#define PE0  0x40
#define PE1  0x41
#define PE2  0x42
#define PE3  0x43
#define PE4  0x44
#define PE5  0x45
#define PE6  0x46
#define PE7  0x47
#define PE8  0x48
#define PE9  0x49
#define PE10 0x4A
#define PE11 0x4B
#define PE12 0x4C
#define PE13 0x4D
#define PE14 0x4E
#define PE15 0x4F // 100 pins (F446V)

#define PF0  0x50
#define PF1  0x51
#define PF2  0x52
#define PF3  0x53
#define PF4  0x54
#define PF5  0x55
#define PF6  0x56
#define PF7  0x57
#define PF8  0x58
#define PF9  0x59
#define PF10 0x5A
#define PF11 0x5B
#define PF12 0x5C
#define PF13 0x5D
#define PF14 0x5E
#define PF15 0x5F

#define PG0  0x60
#define PG1  0x61
#define PG2  0x62
#define PG3  0x63
#define PG4  0x64
#define PG5  0x65
#define PG6  0x66
#define PG7  0x67
#define PG8  0x68
#define PG9  0x69
#define PG10 0x6A
#define PG11 0x6B
#define PG12 0x6C
#define PG13 0x6D
#define PG14 0x6E
#define PG15 0x6F

#define PH0  0x70
#define PH1  0x71
#define PH2  0x72
#define PH3  0x73
#define PH4  0x74
#define PH5  0x75
#define PH6  0x76
#define PH7  0x77
#define PH8  0x78
#define PH9  0x79
#define PH10 0x7A
#define PH11 0x7B
#define PH12 0x7C
#define PH13 0x7D
#define PH14 0x7E
#define PH15 0x7F // 144 pins (F446Z)

// This must be a literal with the same value as PEND
#define NUM_DIGITAL_PINS        0x80
// This must be a literal with a value less than or equal to to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       3
#define NUM_ANALOG_FIRST        128

// PWM resolution
// #define PWM_RESOLUTION          12
#define PWM_FREQUENCY           20000 // >= 20 Khz => inaudible noise for fans
#define PWM_MAX_DUTY_CYCLE      255

// SPI Definitions
// #define PIN_SPI_SS              PG15
// #define PIN_SPI_MOSI            PB5
// #define PIN_SPI_MISO            PB4
// #define PIN_SPI_SCK             PB3

// I2C Definitions
#define PIN_WIRE_SDA            PB9
#define PIN_WIRE_SCL            PB8
#define PIN_I2C_WP              PB7
#define EEPROM_DEVICE_ADDRESS   0x50

// Timer Definitions
// Do not use timer used by PWM pin. See PinMap_PWM.
#define TIMER_TONE              TIM8
#define TIMER_SERVO             TIM5
#define TIMER_SERIAL            TIM7

// UART Definitions
//#define SERIAL_UART_INSTANCE    1 // Connected to EXP3 header
/* Enable Serial 3 */
#define HAVE_HWSERIAL1
// #define HAVE_HWSERIAL3

// Default pin used for 'Serial' instance (ex: ST-Link)
// Mandatory for Firmata
#define PIN_SERIAL_RX           PA10
#define PIN_SERIAL_TX           PA9

/* HAL configuration */
#define HSE_VALUE               8000000U

#define FLASH_PAGE_SIZE         (4U * 1024U)

#ifdef __cplusplus
} // extern "C"
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR     Serial
#define SERIAL_PORT_HARDWARE_OPEN  Serial
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
