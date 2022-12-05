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

#include "variant.h"

/*
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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pins descriptions
 */
extern const PinDescription g_APinDescription[]=
{
  // 0 .. 53 - Digital pins
  // ----------------------
  // 0/1 - UART (Serial)
  { PIOA, PIO_PA8A_URXD,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD
  { PIOA, PIO_PA9A_UTXD,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD

  // 2
  { PIOB, PIO_PB25B_TIOA0,   ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC0_CHA0     }, // TIOA0
  { PIOC, PIO_PC28B_TIOA7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHA7     }, // TIOA7
  { PIOC, PIO_PC26B_TIOB6,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHB6     }, // TIOB6

  // 5
  { PIOC, PIO_PC25B_TIOA6,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHA6     }, // TIOA6
  { PIOC, PIO_PC24B_PWML7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH7,     NOT_ON_TIMER }, // PWML7
  { PIOC, PIO_PC23B_PWML6,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH6,     NOT_ON_TIMER }, // PWML6
  { PIOC, PIO_PC22B_PWML5,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH5,     NOT_ON_TIMER }, // PWML5
  { PIOC, PIO_PC21B_PWML4,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH4,     NOT_ON_TIMER }, // PWML4
  // 10
  { PIOC, PIO_PC29B_TIOB7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHB7     }, // TIOB7
  { PIOD, PIO_PD7B_TIOA8,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHA8     }, // TIOA8
  { PIOD, PIO_PD8B_TIOB8,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHB8     }, // TIOB8

  // 13 - AMBER LED
  { PIOB, PIO_PB27B_TIOB0,   ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC0_CHB0     }, // TIOB0

  // 14/15 - USART3 (Serial3)
  { PIOD, PIO_PD4B_TXD3,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD3
  { PIOD, PIO_PD5B_RXD3,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD3

  // 16/17 - USART1 (Serial2)
  { PIOA, PIO_PA13A_TXD1,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD1
  { PIOA, PIO_PA12A_RXD1,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD1

  // 18/19 - USART0 (Serial1)
  { PIOA, PIO_PA11A_TXD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD0
  { PIOA, PIO_PA10A_RXD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD0

  // 20/21 - TWI1
  { PIOB, PIO_PB12A_TWD1,    ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD1 - SDA0
  { PIOB, PIO_PB13A_TWCK1,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK1 - SCL0

  // 22
  { PIOB, PIO_PB26,          ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 22
  { PIOA, PIO_PA14,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 23
  { PIOA, PIO_PA15,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 24
  { PIOD, PIO_PD0,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 25

  // 26
  { PIOD, PIO_PD1,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 26
  { PIOD, PIO_PD2,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 27
  { PIOD, PIO_PD3,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 28
  { PIOD, PIO_PD6,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 29

  // 30
  { PIOD, PIO_PD9,           ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 30
  { PIOA, PIO_PA7,           ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 31
  { PIOD, PIO_PD10,          ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 32
  { PIOC, PIO_PC1,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 33

  // 34
  { PIOC, PIO_PC2,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 34
  { PIOC, PIO_PC3,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 35
//{ PIOC, PIO_PC3B_PWMH0,    ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH0,     NOT_ON_TIMER }, // PWMH0 PIN 35
  { PIOC, PIO_PC4,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 36
  { PIOC, PIO_PC5,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 37
//{ PIOC, PIO_PC5B_PWMH1,    ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH1,     NOT_ON_TIMER }, // PWMH1 PIN 37

  // 38
  { PIOC, PIO_PC6,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 38
  { PIOC, PIO_PC7,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 39
//{ PIOC, PIO_PC7B_PWMH2,    ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH2,     NOT_ON_TIMER }, // PWMH2 PIN 39
  { PIOC, PIO_PC8,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 40
//{ PIOC, PIO_PC8B_PWML3,    ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH3,     NOT_ON_TIMER }, // PWML3 PIN 40
  { PIOC, PIO_PC9,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 41

  // 42
  { PIOA, PIO_PA19,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 42
  { PIOA, PIO_PA20,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 43
  { PIOC, PIO_PC19,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 44
//{ PIOC, PIO_PC19B_PWMH5,    ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH5,     NOT_ON_TIMER }, // PWMH5 PIN 44
  { PIOC, PIO_PC18,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 45

  // 46
  { PIOC, PIO_PC17,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 46
  { PIOC, PIO_PC16,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 47
  { PIOC, PIO_PC15,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 48
  { PIOC, PIO_PC14,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 49

  // 50
  { PIOC, PIO_PC13,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 50
  { PIOC, PIO_PC12,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 51
  { PIOB, PIO_PB21,          ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 52
  { PIOB, PIO_PB14,          ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 53


  // 54 .. 65 - Analog pins
  // ----------------------
  { PIOA, PIO_PA16X1_AD7,    ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC0,   ADC7,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD0
  { PIOA, PIO_PA24X1_AD6,    ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC1,   ADC6,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD1
  { PIOA, PIO_PA23X1_AD5,    ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC2,   ADC5,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD2
  { PIOA, PIO_PA22X1_AD4,    ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC3,   ADC4,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD3
  // 58
//{ PIOA, PIO_PA6X1_AD3,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC4,   ADC3,   NOT_ON_PWM,  TC0_CHB2     }, // AD4
  { PIOA, PIO_PA6A_TIOB2,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_TIMER|PIN_ATTR_DIGITAL), ADC4,   ADC3,   NOT_ON_PWM,  TC0_CHB2     }, // TIOB2
  { PIOA, PIO_PA4X1_AD2,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC5,   ADC2,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD5
  { PIOA, PIO_PA3X1_AD1,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC6,   ADC1,   NOT_ON_PWM,  TC0_CHB1     }, // AD6
  { PIOA, PIO_PA2X1_AD0,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC7,   ADC0,   NOT_ON_PWM,  TC0_CHA1     }, // AD7
  // 62
  { PIOB, PIO_PB17X1_AD10,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC8,   ADC10,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD8
  { PIOB, PIO_PB18X1_AD11,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC9,   ADC11,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD9
  { PIOB, PIO_PB19X1_AD12,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC10,  ADC12,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD10
  { PIOB, PIO_PB20X1_AD13,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC11,  ADC13,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD11

  // 66/67 - DAC0/DAC1
  { PIOB, PIO_PB15X1_DAC0,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC12,  DA0,    NOT_ON_PWM,  NOT_ON_TIMER }, // DAC0
  { PIOB, PIO_PB16X1_DAC1,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC13,  DA1,    NOT_ON_PWM,  NOT_ON_TIMER }, // DAC1

  // 68/69 - CANRX0/CANTX0
  { PIOA, PIO_PA1A_CANRX0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  ADC14,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANRX
  { PIOA, PIO_PA0A_CANTX0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  ADC15,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANTX

  // 70/71 - TWI0
  { PIOA, PIO_PA17A_TWD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD0 - SDA1
  { PIOA, PIO_PA18A_TWCK0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK0 - SCL1

  // 72/73 - LEDs
  { PIOC, PIO_PC30,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LED AMBER RXL
  { PIOA, PIO_PA21,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LED AMBER TXL

  // 74/75/76 - SPI
  { PIOA, PIO_PA25A_SPI0_MISO,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MISO
  { PIOA, PIO_PA26A_SPI0_MOSI,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MOSI
  { PIOA, PIO_PA27A_SPI0_SPCK,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPCK

  // 77 - SPI CS0
  { PIOA, PIO_PA28A_SPI0_NPCS0,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS0

  // 78 - SPI CS3 (unconnected)
  { PIOB, PIO_PB23B_SPI0_NPCS3,ID_PIOB,PIO_PERIPH_B,PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS3

  // 79 .. 84 - "All pins" masks

  // 79 - TWI0 all pins
  { PIOA, PIO_PA17A_TWD0|PIO_PA18A_TWCK0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 80 - TWI1 all pins
  { PIOB, PIO_PB12A_TWD1|PIO_PB13A_TWCK1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 81 - UART (Serial) all pins
  { PIOA, PIO_PA8A_URXD|PIO_PA9A_UTXD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 82 - USART0 (Serial1) all pins
  { PIOA, PIO_PA11A_TXD0|PIO_PA10A_RXD0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 83 - USART1 (Serial2) all pins
  { PIOA, PIO_PA13A_TXD1|PIO_PA12A_RXD1, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 84 - USART3 (Serial3) all pins
  { PIOD, PIO_PD4B_TXD3|PIO_PD5B_RXD3, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },

  // 85 - USB
//{ PIOB, PIO_PB11A_UOTGID|PIO_PB10A_UOTGVBOF, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ID - VBOF
  // 85 - PB11 only
  { PIOB, PIO_PB11A_UOTGID, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ID - VBOF

  // 86 - SPI CS2
  { PIOB, PIO_PB21B_SPI0_NPCS2, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS2

  // 87 - SPI CS1
  { PIOA, PIO_PA29A_SPI0_NPCS1, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS1

  // 88/89 - CANRX1/CANTX1 (same physical pin for 66/53)
  { PIOB, PIO_PB15A_CANRX1,     ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANRX1
  { PIOB, PIO_PB14A_CANTX1,     ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANTX1

  // 90 .. 91 - "All CAN pins" masks
  // 90 - CAN0 all pins
  { PIOA, PIO_PA1A_CANRX0|PIO_PA0A_CANTX0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  // 91 - CAN1 all pins
  { PIOB, PIO_PB15A_CANRX1|PIO_PB14A_CANTX1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 92 - Extra AMBIT Pins
  { PIOC, PIO_PC11,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 92
  { PIOB, PIO_PB2,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 93
  { PIOB, PIO_PB1,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 94
  { PIOB, PIO_PB0,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 95
  { PIOC, PIO_PC10,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 96
  { PIOB, PIO_PB24,           ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 97
  { PIOB, PIO_PB7,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 98
  { PIOB, PIO_PB6,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 99
  { PIOB, PIO_PB8,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 100
  { PIOB, PIO_PB5,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 101
  { PIOB, PIO_PB4,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 102
  { PIOB, PIO_PB3,            ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 103
  { PIOC, PIO_PC20,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 104
  { PIOB, PIO_PB22,           ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 105
  { PIOC, PIO_PC27,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 106
  { PIOB, PIO_PB10B_A18,      ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 107
  { PIOB, PIO_PB9,            ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 108
  { PIOA, PIO_PA5,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // DIO PIN 109

  // END
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
} ;


uint8_t g_pinStatus[PINS_COUNT] = {0};

#ifdef __cplusplus
}
#endif

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;

UARTClass Serial(UART, UART_IRQn, ID_UART, &rx_buffer1, &tx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

// IT handlers
void UART_Handler(void)
{
  Serial.IrqHandler();
}

// ----------------------------------------------------------------------------
/*
 * USART objects
 */
RingBuffer rx_buffer2;
RingBuffer rx_buffer3;
RingBuffer rx_buffer4;
RingBuffer tx_buffer2;
RingBuffer tx_buffer3;
RingBuffer tx_buffer4;

USARTClass Serial1(USART0, USART0_IRQn, ID_USART0, &rx_buffer2, &tx_buffer2);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }
USARTClass Serial2(USART1, USART1_IRQn, ID_USART1, &rx_buffer3, &tx_buffer3);
void serialEvent2() __attribute__((weak));
void serialEvent2() { }
USARTClass Serial3(USART3, USART3_IRQn, ID_USART3, &rx_buffer4, &tx_buffer4);
void serialEvent3() __attribute__((weak));
void serialEvent3() { }

// IT handlers
void USART0_Handler(void)
{
  Serial1.IrqHandler();
}

void USART1_Handler(void)
{
  Serial2.IrqHandler();
}

void USART3_Handler(void)
{
  Serial3.IrqHandler();
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
  if (Serial2.available()) serialEvent2();
  if (Serial3.available()) serialEvent3();
}

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void __libc_init_array(void);

void init( void )
{
  SystemInit();

  // Set Systick to 1ms interval, common to all SAM3 variants
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    // Capture error
    while (true);
  }

  // Initialize C library
  __libc_init_array();

  // Disable pull-up on every pin
  for (unsigned i = 0; i < PINS_COUNT; i++)
    digitalWrite(i, LOW);

  // Enable parallel access on PIO output data registers
  PIOA->PIO_OWER = 0xFFFFFFFF;
  PIOB->PIO_OWER = 0xFFFFFFFF;
  PIOC->PIO_OWER = 0xFFFFFFFF;
  PIOD->PIO_OWER = 0xFFFFFFFF;

  // Set USB Full Speed for Archim
  udd_low_speed_disable();
  udd_high_speed_disable();

  // Initialize Serial port U(S)ART pins
  PIO_Configure(
    g_APinDescription[PINS_UART].pPort,
    g_APinDescription[PINS_UART].ulPinType,
    g_APinDescription[PINS_UART].ulPin,
    g_APinDescription[PINS_UART].ulPinConfiguration);
  digitalWrite(0, HIGH); // Enable pullup for RX0
  PIO_Configure(
    g_APinDescription[PINS_USART0].pPort,
    g_APinDescription[PINS_USART0].ulPinType,
    g_APinDescription[PINS_USART0].ulPin,
    g_APinDescription[PINS_USART0].ulPinConfiguration);
  PIO_Configure(
    g_APinDescription[PINS_USART1].pPort,
    g_APinDescription[PINS_USART1].ulPinType,
    g_APinDescription[PINS_USART1].ulPin,
    g_APinDescription[PINS_USART1].ulPinConfiguration);
  PIO_Configure(
    g_APinDescription[PINS_USART3].pPort,
    g_APinDescription[PINS_USART3].ulPinType,
    g_APinDescription[PINS_USART3].ulPin,
    g_APinDescription[PINS_USART3].ulPinConfiguration);

  // Initialize USB pins
  PIO_Configure(
    g_APinDescription[PINS_USB].pPort,
    g_APinDescription[PINS_USB].ulPinType,
    g_APinDescription[PINS_USB].ulPin,
    g_APinDescription[PINS_USB].ulPinConfiguration);

  // Initialize CAN pins
  PIO_Configure(
    g_APinDescription[PINS_CAN0].pPort,
    g_APinDescription[PINS_CAN0].ulPinType,
    g_APinDescription[PINS_CAN0].ulPin,
    g_APinDescription[PINS_CAN0].ulPinConfiguration);
  PIO_Configure(
    g_APinDescription[PINS_CAN1].pPort,
    g_APinDescription[PINS_CAN1].ulPinType,
    g_APinDescription[PINS_CAN1].ulPin,
    g_APinDescription[PINS_CAN1].ulPinConfiguration);

  // Initialize Analog Controller
  pmc_enable_periph_clk(ID_ADC);
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
  adc_configure_trigger(ADC, ADC_TRIG_SW, 0); // Disable hardware trigger.
  adc_disable_interrupt(ADC, 0xFFFFFFFF); // Disable all ADC interrupts.
  adc_disable_all_channel(ADC);

  // Initialize analogOutput module
  analogOutputInit();
}

#ifdef __cplusplus
}
#endif

