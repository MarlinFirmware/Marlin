/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * SAMD21 HAL developed by Bart Meijer (brupje)
 * Based on SAMD51 HAL by Giuliano Zaro (AKA GMagician)
 */

#define SYNC(sc)    while (sc) {  \
                      asm("");    \
                    }

// Get SAMD port/pin from specified arduino pin
#define GET_SAMD_PORT(P)    _GET_SAMD_PORT(PIN_TO_SAMD_PIN(P))
#define GET_SAMD_PIN(P)     _GET_SAMD_PIN(PIN_TO_SAMD_PIN(P))

// Get external interrupt line associated to specified arduino pin
#define PIN_TO_EILINE(P)    _SAMDPORTPIN_TO_EILINE(GET_SAMD_PORT(P), GET_SAMD_PIN(P))

// Get adc/ain associated to specified arduino pin
#define PIN_TO_ADC(P)       (ANAPIN_TO_ADCAIN(P) >> 8)

// Private defines
#define PIN_TO_SAMD_PIN(P)    DIO##P##_PIN

#define _GET_SAMD_PORT(P)     ((P) >> 5)
#define _GET_SAMD_PIN(P)      ((P) & 0x1F)

// Get external interrupt line
#define _SAMDPORTPIN_TO_EILINE(P,B)   ((P == 0 && WITHIN(B, 0, 31) && B != 26 && B != 28 && B != 29) ? (B) & 0xF    \
                                       : (P == 1 && (WITHIN(B, 0, 25) || WITHIN(B, 30, 31))) ? (B) & 0xF                      \
                                       : (P == 1 && WITHIN(B, 26, 29)) ? 12 + (B) - 26                                        \
                                       : (P == 2 && (WITHIN(B, 0, 6) || WITHIN(B, 10, 31)) && B != 29) ? (B) & 0xF            \
                                       : (P == 2 && B == 7) ? 9                                                               \
                                       : (P == 3 && WITHIN(B, 0, 1)) ? (B)                                                    \
                                       : (P == 3 && WITHIN(B, 8, 12)) ? 3 + (B) - 8                                           \
                                       : (P == 3 && WITHIN(B, 20, 21)) ? 10 + (B) - 20                                        \
                                       : -1)

#define A2_AIN 3
#define A3_AIN 4
#define A4_AIN 5
#define PIN_TO_AIN(P)    A##P##_AIN
#define AIN_TO_RESULT(P) ( (P - HAL_ADC_AIN_START == HAL_ADC_AIN_NUM_SENSORS-1) ? 0 :  (P - HAL_ADC_AIN_START + 1) )
