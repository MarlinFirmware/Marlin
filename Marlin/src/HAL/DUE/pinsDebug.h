/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Support routines for Due
 */

/**
 * Translation of routines & variables used by pinsDebug.h
 */

#include "../shared/Marduino.h"

/**
 * Due/Marlin quirks
 *
 * a) determining the state of a pin
 *     The Due/Arduino status definitions for the g_pinStatus[pin] array are:
 *       #define PIN_STATUS_DIGITAL_INPUT_PULLUP  (0x01)
 *       #define PIN_STATUS_DIGITAL_INPUT         (0x02)
 *       #define PIN_STATUS_DIGITAL_OUTPUT        (0x03)
 *       #define PIN_STATUS_ANALOG                (0x04)
 *       #define PIN_STATUS_PWM                   (0x05)
 *       #define PIN_STATUS_TIMER                 (0x06)
 *
 *     These are only valid if the following Due/Arduino provided functions are used:
 *       analogRead
 *       analogWrite
 *       digitalWrite
 *       pinMode
 *
 *     The FASTIO routines do not touch the g_pinStatus[pin] array.
 *
 *     The net result is that both the g_pinStatus[pin] array and the PIO_OSR register
 *     needs to be looked at when determining if a pin is an input or an output.
 *
 * b) Due has only pins 6, 7, 8 & 9 enabled for PWMs.  FYI - they run at 1KHz
 *
 * c) NUM_DIGITAL_PINS does not include the analog pins
 *
 * d) Pins 0-78 are defined for Due but 78 has a comment of "unconnected!".  78 is
 *    included just in case.
 */

#define NUMBER_PINS_TOTAL PINS_COUNT

#define digitalRead_mod(p) extDigitalRead(p)  // AVR digitalRead disabled PWM before it read the pin
#define PRINT_PORT(p)
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN(p) do{ sprintf_P(buffer, PSTR("%02d"), p); SERIAL_ECHO(buffer); }while(0)
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define GET_ARRAY_IS_DIGITAL(p) pin_array[p].is_digital
#define VALID_PIN(pin) (pin >= 0 && pin < (int8_t)NUMBER_PINS_TOTAL ? 1 : 0)
#define DIGITAL_PIN_TO_ANALOG_PIN(p) int(p - analogInputToDigitalPin(0))
#define IS_ANALOG(P) WITHIN(P, char(analogInputToDigitalPin(0)), char(analogInputToDigitalPin(NUM_ANALOG_INPUTS - 1)))
#define pwm_status(pin) (((g_pinStatus[pin] & 0xF) == PIN_STATUS_PWM) && \
                        ((g_APinDescription[pin].ulPinAttribute & PIN_ATTR_PWM) == PIN_ATTR_PWM))
#define MULTI_NAME_PAD 14 // space needed to be pretty if not first name assigned to a pin

bool GET_PINMODE(int8_t pin) {  // 1: output, 0: input
  volatile Pio* port = g_APinDescription[pin].pPort;
  uint32_t mask = g_APinDescription[pin].ulPin;
  uint8_t pin_status = g_pinStatus[pin] & 0xF;
  return (  (pin_status == 0 && (port->PIO_OSR & mask))
          || pin_status == PIN_STATUS_DIGITAL_OUTPUT
          || pwm_status(pin));
}


void pwm_details(int32_t pin) {
  if (pwm_status(pin)) {
    uint32_t chan = g_APinDescription[pin].ulPWMChannel;
    SERIAL_ECHOPAIR("PWM = ", PWM_INTERFACE->PWM_CH_NUM[chan].PWM_CDTY);
  }
}

/**
 * DUE Board pin   |  PORT  | Label
 * ----------------+--------+-------
 *   0             |  PA8   | "RX0"
 *   1             |  PA9   | "TX0"
 *   2       TIOA0 |  PB25  |
 *   3       TIOA7 |  PC28  |
 *   4       NPCS1 |  PA29  |
 *           TIOB6 |  PC26  |
 *   5       TIOA6 |  PC25  |
 *   6       PWML7 |  PC24  |
 *   7       PWML6 |  PC23  |
 *   8       PWML5 |  PC22  |
 *   9       PWML4 |  PC21  |
 *  10       NPCS0 |  PA28  |
 *           TIOB7 |  PC29  |
 *  11       TIOA8 |  PD7   |
 *  12       TIOB8 |  PD8   |
 *  13       TIOB0 |  PB27  | LED AMBER "L"
 *  14       TXD3  |  PD4   | "TX3"
 *  15       RXD3  |  PD5   | "RX3"
 *  16       TXD1  |  PA13  | "TX2"
 *  17       RXD1  |  PA12  | "RX2"
 *  18       TXD0  |  PA11  | "TX1"
 *  19       RXD0  |  PA10  | "RX1"
 *  20             |  PB12  | "SDA"
 *  21             |  PB13  | "SCL"
 *  22             |  PB26  |
 *  23             |  PA14  |
 *  24             |  PA15  |
 *  25             |  PD0   |
 *  26             |  PD1   |
 *  27             |  PD2   |
 *  28             |  PD3   |
 *  29             |  PD6   |
 *  30             |  PD9   |
 *  31             |  PA7   |
 *  32             |  PD10  |
 *  33             |  PC1   |
 *  34             |  PC2   |
 *  35             |  PC3   |
 *  36             |  PC4   |
 *  37             |  PC5   |
 *  38             |  PC6   |
 *  39             |  PC7   |
 *  40             |  PC8   |
 *  41             |  PC9   |
 *  42             |  PA19  |
 *  43             |  PA20  |
 *  44             |  PC19  |
 *  45             |  PC18  |
 *  46             |  PC17  |
 *  47             |  PC16  |
 *  48             |  PC15  |
 *  49             |  PC14  |
 *  50             |  PC13  |
 *  51             |  PC12  |
 *  52       NPCS2 |  PB21  |
 *  53             |  PB14  |
 *  54             |  PA16  | "A0"
 *  55             |  PA24  | "A1"
 *  56             |  PA23  | "A2"
 *  57             |  PA22  | "A3"
 *  58       TIOB2 |  PA6   | "A4"
 *  69             |  PA4   | "A5"
 *  60       TIOB1 |  PA3   | "A6"
 *  61       TIOA1 |  PA2   | "A7"
 *  62             |  PB17  | "A8"
 *  63             |  PB18  | "A9"
 *  64             |  PB19  | "A10"
 *  65             |  PB20  | "A11"
 *  66             |  PB15  | "DAC0"
 *  67             |  PB16  | "DAC1"
 *  68             |  PA1   | "CANRX"
 *  69             |  PA0   | "CANTX"
 *  70             |  PA17  | "SDA1"
 *  71             |  PA18  | "SCL1"
 *  72             |  PC30  | LED AMBER "RX"
 *  73             |  PA21  | LED AMBER "TX"
 *  74       MISO  |  PA25  |
 *  75       MOSI  |  PA26  |
 *  76       SCLK  |  PA27  |
 *  77       NPCS0 |  PA28  |
 *  78       NPCS3 |  PB23  | unconnected!
 *
 * USB pin         |  PORT
 * ----------------+--------
 *  ID             |  PB11
 *  VBOF           |  PB10
 *
 */
