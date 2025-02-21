/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
#define PIN_TO_AIN(P)       (ANAPIN_TO_ADCAIN(P) & 0xFF)

// Private defines
#define PIN_TO_SAMD_PIN(P)    DIO##P##_PIN

#define _GET_SAMD_PORT(P)     ((P) >> 5)
#define _GET_SAMD_PIN(P)      ((P) & 0x1F)

// Get external interrupt line
#define _SAMDPORTPIN_TO_EILINE(P,B)   ((P == 0 && WITHIN(B, 0, 31) && B != 8 && B != 26 && B != 28 && B != 29) ? (B) & 0xF    \
                                       : (P == 1 && (WITHIN(B, 0, 25) || WITHIN(B, 30, 31))) ? (B) & 0xF                      \
                                       : (P == 1 && WITHIN(B, 26, 29)) ? 12 + (B) - 26                                        \
                                       : (P == 2 && (WITHIN(B, 0, 6) || WITHIN(B, 10, 31)) && B != 29) ? (B) & 0xF            \
                                       : (P == 2 && B == 7) ? 9                                                               \
                                       : (P == 3 && WITHIN(B, 0, 1)) ? (B)                                                    \
                                       : (P == 3 && WITHIN(B, 8, 12)) ? 3 + (B) - 8                                           \
                                       : (P == 3 && WITHIN(B, 20, 21)) ? 10 + (B) - 20                                        \
                                       : -1)

// Get adc/ain
#define ANAPIN_TO_ADCAIN(P)     _PIN_TO_ADCAIN(ANAPIN_TO_SAMDPIN(P))
#define _PIN_TO_ADCAIN(P)       _SAMDPORTPIN_TO_ADCAIN(_GET_SAMD_PORT(P), _GET_SAMD_PIN(P))

#define _SAMDPORTPIN_TO_ADCAIN(P,B)       ((P == 0 && WITHIN(B, 2, 3)) ? 0x000 + (B) - 2            \
                                           : (P == 0 && WITHIN(B, 4, 7)) ? 0x000 + (B)              \
                                           : (P == 0 && WITHIN(B, 8, 9)) ? 0x100 + 2 + (B) - 8      \
                                           : (P == 0 && WITHIN(B, 10, 11)) ? 0x000 + (B)            \
                                           : (P == 1 && WITHIN(B, 0, 3)) ? 0x000 + 12 + (B)         \
                                           : (P == 1 && WITHIN(B, 4, 7)) ? 0x100 + 6 + (B) - 4      \
                                           : (P == 1 && WITHIN(B, 8, 9)) ? 0x100 + (B) - 8          \
                                           : (P == 2 && WITHIN(B, 0, 1)) ? 0x100 + 10 + (B)         \
                                           : (P == 2 && WITHIN(B, 2, 3)) ? 0x100 + 4 + (B) - 2      \
                                           : (P == 2 && WITHIN(B, 30, 31)) ? 0x100 + 12 + (B) - 30  \
                                           : (P == 3 && WITHIN(B, 0, 1)) ? 0x100 + 14 + (B)         \
                                           : -1)
