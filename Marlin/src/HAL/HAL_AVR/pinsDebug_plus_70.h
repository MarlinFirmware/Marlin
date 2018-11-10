/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * Structures for 2560 family boards that use more than 70 pins
 */

#undef NUM_DIGITAL_PINS
#if MB(BQ_ZUM_MEGA_3D)
  #define NUM_DIGITAL_PINS            85
#elif MB(MIGHTYBOARD_REVE)
  #define NUM_DIGITAL_PINS            80
#elif MB(MINIRAMBO)
  #define NUM_DIGITAL_PINS            85
#elif MB(SCOOVO_X9H)
  #define NUM_DIGITAL_PINS            85
#endif

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

const uint8_t PROGMEM digital_pin_to_port_PGM_plus_70[] = {
  // PORTLIST
  // -------------------------------------------
  PE  , // PE 0 ** 0 ** USART0_RX
  PE  , // PE 1 ** 1 ** USART0_TX
  PE  , // PE 4 ** 2 ** PWM2
  PE  , // PE 5 ** 3 ** PWM3
  PG  , // PG 5 ** 4 ** PWM4
  PE  , // PE 3 ** 5 ** PWM5
  PH  , // PH 3 ** 6 ** PWM6
  PH  , // PH 4 ** 7 ** PWM7
  PH  , // PH 5 ** 8 ** PWM8
  PH  , // PH 6 ** 9 ** PWM9
  PB  , // PB 4 ** 10 ** PWM10
  PB  , // PB 5 ** 11 ** PWM11
  PB  , // PB 6 ** 12 ** PWM12
  PB  , // PB 7 ** 13 ** PWM13
  PJ  , // PJ 1 ** 14 ** USART3_TX
  PJ  , // PJ 0 ** 15 ** USART3_RX
  PH  , // PH 1 ** 16 ** USART2_TX
  PH  , // PH 0 ** 17 ** USART2_RX
  PD  , // PD 3 ** 18 ** USART1_TX
  PD  , // PD 2 ** 19 ** USART1_RX
  PD  , // PD 1 ** 20 ** I2C_SDA
  PD  , // PD 0 ** 21 ** I2C_SCL
  PA  , // PA 0 ** 22 ** D22
  PA  , // PA 1 ** 23 ** D23
  PA  , // PA 2 ** 24 ** D24
  PA  , // PA 3 ** 25 ** D25
  PA  , // PA 4 ** 26 ** D26
  PA  , // PA 5 ** 27 ** D27
  PA  , // PA 6 ** 28 ** D28
  PA  , // PA 7 ** 29 ** D29
  PC  , // PC 7 ** 30 ** D30
  PC  , // PC 6 ** 31 ** D31
  PC  , // PC 5 ** 32 ** D32
  PC  , // PC 4 ** 33 ** D33
  PC  , // PC 3 ** 34 ** D34
  PC  , // PC 2 ** 35 ** D35
  PC  , // PC 1 ** 36 ** D36
  PC  , // PC 0 ** 37 ** D37
  PD  , // PD 7 ** 38 ** D38
  PG  , // PG 2 ** 39 ** D39
  PG  , // PG 1 ** 40 ** D40
  PG  , // PG 0 ** 41 ** D41
  PL  , // PL 7 ** 42 ** D42
  PL  , // PL 6 ** 43 ** D43
  PL  , // PL 5 ** 44 ** D44
  PL  , // PL 4 ** 45 ** D45
  PL  , // PL 3 ** 46 ** D46
  PL  , // PL 2 ** 47 ** D47
  PL  , // PL 1 ** 48 ** D48
  PL  , // PL 0 ** 49 ** D49
  PB  , // PB 3 ** 50 ** SPI_MISO
  PB  , // PB 2 ** 51 ** SPI_MOSI
  PB  , // PB 1 ** 52 ** SPI_SCK
  PB  , // PB 0 ** 53 ** SPI_SS
  PF  , // PF 0 ** 54 ** A0
  PF  , // PF 1 ** 55 ** A1
  PF  , // PF 2 ** 56 ** A2
  PF  , // PF 3 ** 57 ** A3
  PF  , // PF 4 ** 58 ** A4
  PF  , // PF 5 ** 59 ** A5
  PF  , // PF 6 ** 60 ** A6
  PF  , // PF 7 ** 61 ** A7
  PK  , // PK 0 ** 62 ** A8
  PK  , // PK 1 ** 63 ** A9
  PK  , // PK 2 ** 64 ** A10
  PK  , // PK 3 ** 65 ** A11
  PK  , // PK 4 ** 66 ** A12
  PK  , // PK 5 ** 67 ** A13
  PK  , // PK 6 ** 68 ** A14
  PK  , // PK 7 ** 69 ** A15
  PG  , // PG 4 ** 70 **
  PG  , // PG 3 ** 71 **
  PJ  , // PJ 2 ** 72 **
  PJ  , // PJ 3 ** 73 **
  PJ  , // PJ 7 ** 74 **
  PJ  , // PJ 4 ** 75 **
  PJ  , // PJ 5 ** 76 **
  PJ  , // PJ 6 ** 77 **
  PE  , // PE 2 ** 78 **
  PE  , // PE 6 ** 79 **
  PE  , // PE 7 ** 80 **
  PD  , // PD 4 ** 81 **
  PD  , // PD 5 ** 82 **
  PD  , // PD 6 ** 83 **
  PH  , // PH 2 ** 84 **
  PH  , // PH 7 ** 85 **
};

#define digitalPinToPort_plus_70(P) ( pgm_read_byte( digital_pin_to_port_PGM_plus_70 + (P) ) )

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM_plus_70[] = {
  // PIN IN PORT
  // -------------------------------------------
  _BV( 0 )  , // PE 0 ** 0 ** USART0_RX
  _BV( 1 )  , // PE 1 ** 1 ** USART0_TX
  _BV( 4 )  , // PE 4 ** 2 ** PWM2
  _BV( 5 )  , // PE 5 ** 3 ** PWM3
  _BV( 5 )  , // PG 5 ** 4 ** PWM4
  _BV( 3 )  , // PE 3 ** 5 ** PWM5
  _BV( 3 )  , // PH 3 ** 6 ** PWM6
  _BV( 4 )  , // PH 4 ** 7 ** PWM7
  _BV( 5 )  , // PH 5 ** 8 ** PWM8
  _BV( 6 )  , // PH 6 ** 9 ** PWM9
  _BV( 4 )  , // PB 4 ** 10 ** PWM10
  _BV( 5 )  , // PB 5 ** 11 ** PWM11
  _BV( 6 )  , // PB 6 ** 12 ** PWM12
  _BV( 7 )  , // PB 7 ** 13 ** PWM13
  _BV( 1 )  , // PJ 1 ** 14 ** USART3_TX
  _BV( 0 )  , // PJ 0 ** 15 ** USART3_RX
  _BV( 1 )  , // PH 1 ** 16 ** USART2_TX
  _BV( 0 )  , // PH 0 ** 17 ** USART2_RX
  _BV( 3 )  , // PD 3 ** 18 ** USART1_TX
  _BV( 2 )  , // PD 2 ** 19 ** USART1_RX
  _BV( 1 )  , // PD 1 ** 20 ** I2C_SDA
  _BV( 0 )  , // PD 0 ** 21 ** I2C_SCL
  _BV( 0 )  , // PA 0 ** 22 ** D22
  _BV( 1 )  , // PA 1 ** 23 ** D23
  _BV( 2 )  , // PA 2 ** 24 ** D24
  _BV( 3 )  , // PA 3 ** 25 ** D25
  _BV( 4 )  , // PA 4 ** 26 ** D26
  _BV( 5 )  , // PA 5 ** 27 ** D27
  _BV( 6 )  , // PA 6 ** 28 ** D28
  _BV( 7 )  , // PA 7 ** 29 ** D29
  _BV( 7 )  , // PC 7 ** 30 ** D30
  _BV( 6 )  , // PC 6 ** 31 ** D31
  _BV( 5 )  , // PC 5 ** 32 ** D32
  _BV( 4 )  , // PC 4 ** 33 ** D33
  _BV( 3 )  , // PC 3 ** 34 ** D34
  _BV( 2 )  , // PC 2 ** 35 ** D35
  _BV( 1 )  , // PC 1 ** 36 ** D36
  _BV( 0 )  , // PC 0 ** 37 ** D37
  _BV( 7 )  , // PD 7 ** 38 ** D38
  _BV( 2 )  , // PG 2 ** 39 ** D39
  _BV( 1 )  , // PG 1 ** 40 ** D40
  _BV( 0 )  , // PG 0 ** 41 ** D41
  _BV( 7 )  , // PL 7 ** 42 ** D42
  _BV( 6 )  , // PL 6 ** 43 ** D43
  _BV( 5 )  , // PL 5 ** 44 ** D44
  _BV( 4 )  , // PL 4 ** 45 ** D45
  _BV( 3 )  , // PL 3 ** 46 ** D46
  _BV( 2 )  , // PL 2 ** 47 ** D47
  _BV( 1 )  , // PL 1 ** 48 ** D48
  _BV( 0 )  , // PL 0 ** 49 ** D49
  _BV( 3 )  , // PB 3 ** 50 ** SPI_MISO
  _BV( 2 )  , // PB 2 ** 51 ** SPI_MOSI
  _BV( 1 )  , // PB 1 ** 52 ** SPI_SCK
  _BV( 0 )  , // PB 0 ** 53 ** SPI_SS
  _BV( 0 )  , // PF 0 ** 54 ** A0
  _BV( 1 )  , // PF 1 ** 55 ** A1
  _BV( 2 )  , // PF 2 ** 56 ** A2
  _BV( 3 )  , // PF 3 ** 57 ** A3
  _BV( 4 )  , // PF 4 ** 58 ** A4
  _BV( 5 )  , // PF 5 ** 59 ** A5
  _BV( 6 )  , // PF 6 ** 60 ** A6
  _BV( 7 )  , // PF 7 ** 61 ** A7
  _BV( 0 )  , // PK 0 ** 62 ** A8
  _BV( 1 )  , // PK 1 ** 63 ** A9
  _BV( 2 )  , // PK 2 ** 64 ** A10
  _BV( 3 )  , // PK 3 ** 65 ** A11
  _BV( 4 )  , // PK 4 ** 66 ** A12
  _BV( 5 )  , // PK 5 ** 67 ** A13
  _BV( 6 )  , // PK 6 ** 68 ** A14
  _BV( 7 )  , // PK 7 ** 69 ** A15
  _BV( 4 )  , // PG 4 ** 70 **
  _BV( 3 )  , // PG 3 ** 71 **
  _BV( 2 )  , // PJ 2 ** 72 **
  _BV( 3 )  , // PJ 3 ** 73 **
  _BV( 7 )  , // PJ 7 ** 74 **
  _BV( 4 )  , // PJ 4 ** 75 **
  _BV( 5 )  , // PJ 5 ** 76 **
  _BV( 6 )  , // PJ 6 ** 77 **
  _BV( 2 )  , // PE 2 ** 78 **
  _BV( 6 )  , // PE 6 ** 79 **
  _BV( 7 )  , // PE 7 ** 80 **
  _BV( 4 )  , // PD 4 ** 81 **
  _BV( 5 )  , // PD 5 ** 82 **
  _BV( 6 )  , // PD 6 ** 83 **
  _BV( 2 )  , // PH 2 ** 84 **
  _BV( 7 )  , // PH 7 ** 85 **
};

#define digitalPinToBitMask_plus_70(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM_plus_70 + (P) ) )


const uint8_t PROGMEM digital_pin_to_timer_PGM_plus_70[] = {
  // TIMERS
  // -------------------------------------------
  NOT_ON_TIMER  , // PE 0 ** 0 ** USART0_RX
  NOT_ON_TIMER  , // PE 1 ** 1 ** USART0_TX
  TIMER3B , // PE 4 ** 2 ** PWM2
  TIMER3C , // PE 5 ** 3 ** PWM3
  TIMER0B , // PG 5 ** 4 ** PWM4
  TIMER3A , // PE 3 ** 5 ** PWM5
  TIMER4A , // PH 3 ** 6 ** PWM6
  TIMER4B , // PH 4 ** 7 ** PWM7
  TIMER4C , // PH 5 ** 8 ** PWM8
  TIMER2B , // PH 6 ** 9 ** PWM9
  TIMER2A , // PB 4 ** 10 ** PWM10
  TIMER1A , // PB 5 ** 11 ** PWM11
  TIMER1B , // PB 6 ** 12 ** PWM12
  TIMER0A , // PB 7 ** 13 ** PWM13
  NOT_ON_TIMER  , // PJ 1 ** 14 ** USART3_TX
  NOT_ON_TIMER  , // PJ 0 ** 15 ** USART3_RX
  NOT_ON_TIMER  , // PH 1 ** 16 ** USART2_TX
  NOT_ON_TIMER  , // PH 0 ** 17 ** USART2_RX
  NOT_ON_TIMER  , // PD 3 ** 18 ** USART1_TX
  NOT_ON_TIMER  , // PD 2 ** 19 ** USART1_RX
  NOT_ON_TIMER  , // PD 1 ** 20 ** I2C_SDA
  NOT_ON_TIMER  , // PD 0 ** 21 ** I2C_SCL
  NOT_ON_TIMER  , // PA 0 ** 22 ** D22
  NOT_ON_TIMER  , // PA 1 ** 23 ** D23
  NOT_ON_TIMER  , // PA 2 ** 24 ** D24
  NOT_ON_TIMER  , // PA 3 ** 25 ** D25
  NOT_ON_TIMER  , // PA 4 ** 26 ** D26
  NOT_ON_TIMER  , // PA 5 ** 27 ** D27
  NOT_ON_TIMER  , // PA 6 ** 28 ** D28
  NOT_ON_TIMER  , // PA 7 ** 29 ** D29
  NOT_ON_TIMER  , // PC 7 ** 30 ** D30
  NOT_ON_TIMER  , // PC 6 ** 31 ** D31
  NOT_ON_TIMER  , // PC 5 ** 32 ** D32
  NOT_ON_TIMER  , // PC 4 ** 33 ** D33
  NOT_ON_TIMER  , // PC 3 ** 34 ** D34
  NOT_ON_TIMER  , // PC 2 ** 35 ** D35
  NOT_ON_TIMER  , // PC 1 ** 36 ** D36
  NOT_ON_TIMER  , // PC 0 ** 37 ** D37
  NOT_ON_TIMER  , // PD 7 ** 38 ** D38
  NOT_ON_TIMER  , // PG 2 ** 39 ** D39
  NOT_ON_TIMER  , // PG 1 ** 40 ** D40
  NOT_ON_TIMER  , // PG 0 ** 41 ** D41
  NOT_ON_TIMER  , // PL 7 ** 42 ** D42
  NOT_ON_TIMER  , // PL 6 ** 43 ** D43
  TIMER5C , // PL 5 ** 44 ** D44
  TIMER5B , // PL 4 ** 45 ** D45
  TIMER5A , // PL 3 ** 46 ** D46
  NOT_ON_TIMER  , // PL 2 ** 47 ** D47
  NOT_ON_TIMER  , // PL 1 ** 48 ** D48
  NOT_ON_TIMER  , // PL 0 ** 49 ** D49
  NOT_ON_TIMER  , // PB 3 ** 50 ** SPI_MISO
  NOT_ON_TIMER  , // PB 2 ** 51 ** SPI_MOSI
  NOT_ON_TIMER  , // PB 1 ** 52 ** SPI_SCK
  NOT_ON_TIMER  , // PB 0 ** 53 ** SPI_SS
  NOT_ON_TIMER  , // PF 0 ** 54 ** A0
  NOT_ON_TIMER  , // PF 1 ** 55 ** A1
  NOT_ON_TIMER  , // PF 2 ** 56 ** A2
  NOT_ON_TIMER  , // PF 3 ** 57 ** A3
  NOT_ON_TIMER  , // PF 4 ** 58 ** A4
  NOT_ON_TIMER  , // PF 5 ** 59 ** A5
  NOT_ON_TIMER  , // PF 6 ** 60 ** A6
  NOT_ON_TIMER  , // PF 7 ** 61 ** A7
  NOT_ON_TIMER  , // PK 0 ** 62 ** A8
  NOT_ON_TIMER  , // PK 1 ** 63 ** A9
  NOT_ON_TIMER  , // PK 2 ** 64 ** A10
  NOT_ON_TIMER  , // PK 3 ** 65 ** A11
  NOT_ON_TIMER  , // PK 4 ** 66 ** A12
  NOT_ON_TIMER  , // PK 5 ** 67 ** A13
  NOT_ON_TIMER  , // PK 6 ** 68 ** A14
  NOT_ON_TIMER  , // PK 7 ** 69 ** A15
  NOT_ON_TIMER  , // PG 4 ** 70 **
  NOT_ON_TIMER  , // PG 3 ** 71 **
  NOT_ON_TIMER  , // PJ 2 ** 72 **
  NOT_ON_TIMER  , // PJ 3 ** 73 **
  NOT_ON_TIMER  , // PJ 7 ** 74 **
  NOT_ON_TIMER  , // PJ 4 ** 75 **
  NOT_ON_TIMER  , // PJ 5 ** 76 **
  NOT_ON_TIMER  , // PJ 6 ** 77 **
  NOT_ON_TIMER  , // PE 2 ** 78 **
  NOT_ON_TIMER  , // PE 6 ** 79 **
};

#define digitalPinToTimer_plus_70(P) ( pgm_read_byte( digital_pin_to_timer_PGM_plus_70 + (P) ) )

/**
 *  Interrupts that are not implemented
 *
 *  INT6    E6 79
 *  INT7    E7 80
 *  PCINT11 J2 72
 *  PCINT12 J3 73
 *  PCINT13 J4 75
 *  PCINT14 J5 76
 *  PCINT15 J6 77
 */
