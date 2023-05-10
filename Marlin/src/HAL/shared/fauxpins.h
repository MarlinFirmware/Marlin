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
#pragma once

//
// Faux pins for Dependency Check
//

//
// STM32 Pin Names
//
#define PA0  0x10
#define PA1  0x11
#define PA2  0x12
#define PA3  0x13
#define PA4  0x14
#define PA5  0x15
#define PA6  0x16
#define PA7  0x17
#define PA8  0x18
#define PA9  0x19
#define PA10 0x1A
#define PA11 0x1B
#define PA12 0x1C
#define PA13 0x1D
#define PA14 0x1E
#define PA15 0x1F

#define PB0  0x20
#define PB1  0x21
#define PB2  0x22
#define PB3  0x23
#define PB4  0x24
#define PB5  0x25
#define PB6  0x26
#define PB7  0x27
#define PB8  0x28
#define PB9  0x29
#define PB10 0x2A
#define PB11 0x2B
#define PB12 0x2C
#define PB13 0x2D
#define PB14 0x2E
#define PB15 0x2F

#define PC0  0x30
#define PC1  0x31
#define PC2  0x32
#define PC3  0x33
#define PC4  0x34
#define PC5  0x35
#define PC6  0x36
#define PC7  0x37
#define PC8  0x38
#define PC9  0x39
#define PC10 0x3A
#define PC11 0x3B
#define PC12 0x3C
#define PC13 0x3D
#define PC14 0x3E
#define PC15 0x3F

#define PD0  0x40
#define PD1  0x41
#define PD2  0x42
#define PD3  0x43
#define PD4  0x44
#define PD5  0x45
#define PD6  0x46
#define PD7  0x47
#define PD8  0x48
#define PD9  0x49
#define PD10 0x4A
#define PD11 0x4B
#define PD12 0x4C
#define PD13 0x4D
#define PD14 0x4E
#define PD15 0x4F

#define PE0  0x50
#define PE1  0x51
#define PE2  0x52
#define PE3  0x53
#define PE4  0x54
#define PE5  0x55
#define PE6  0x56
#define PE7  0x57
#define PE8  0x58
#define PE9  0x59
#define PE10 0x5A
#define PE11 0x5B
#define PE12 0x5C
#define PE13 0x5D
#define PE14 0x5E
#define PE15 0x5F

#define PF0  0x60
#define PF1  0x61
#define PF2  0x62
#define PF3  0x63
#define PF4  0x64
#define PF5  0x65
#define PF6  0x66
#define PF7  0x67
#define PF8  0x68
#define PF9  0x69
#define PF10 0x6A
#define PF11 0x6B
#define PF12 0x6C
#define PF13 0x6D
#define PF14 0x6E
#define PF15 0x6F

#define PG0  0x70
#define PG1  0x71
#define PG2  0x72
#define PG3  0x73
#define PG4  0x74
#define PG5  0x75
#define PG6  0x76
#define PG7  0x77
#define PG8  0x78
#define PG9  0x79
#define PG10 0x7A
#define PG11 0x7B
#define PG12 0x7C
#define PG13 0x7D
#define PG14 0x7E
#define PG15 0x7F

#define PH0  0x80
#define PH1  0x81
#define PH2  0x82
#define PH3  0x83
#define PH4  0x84
#define PH5  0x85
#define PH6  0x86
#define PH7  0x87
#define PH8  0x88
#define PH9  0x89
#define PH10 0x8A
#define PH11 0x8B
#define PH12 0x8C
#define PH13 0x8D
#define PH14 0x8E
#define PH15 0x8F

#define PI0  0x90
#define PI1  0x91
#define PI2  0x92
#define PI3  0x93
#define PI4  0x94
#define PI5  0x95
#define PI6  0x96
#define PI7  0x97
#define PI8  0x98
#define PI9  0x99
#define PI10 0x9A
#define PI11 0x9B
#define PI12 0x9C
#define PI13 0x9D
#define PI14 0x9E
#define PI15 0x9F

#define PJ0  0xA0
#define PJ1  0xA1
#define PJ2  0xA2
#define PJ3  0xA3
#define PJ4  0xA4
#define PJ5  0xA5
#define PJ6  0xA6
#define PJ7  0xA7
#define PJ8  0xA8
#define PJ9  0xA9
#define PJ10 0xAA
#define PJ11 0xAB
#define PJ12 0xAC
#define PJ13 0xAD
#define PJ14 0xAE
#define PJ15 0xAF

//
// LPC Pin Names
//
#define P0_00 100
#define P0_01 101
#define P0_02 102
#define P0_03 103
#define P0_04 104
#define P0_05 105
#define P0_06 106
#define P0_07 107
#define P0_08 108
#define P0_09 109
#define P0_10 110
#define P0_11 111
#define P0_12 112
#define P0_13 113
#define P0_14 114
#define P0_15 115
#define P0_16 116
#define P0_17 117
#define P0_18 118
#define P0_19 119
#define P0_20 120
#define P0_21 121
#define P0_22 122
#define P0_23 123
#define P0_24 124
#define P0_25 125
#define P0_26 126
#define P0_27 127
#define P0_28 128
#define P0_29 129
#define P0_30 130
#define P0_31 131

#define P1_00 200
#define P1_01 201
#define P1_02 202
#define P1_03 203
#define P1_04 204
#define P1_05 205
#define P1_06 206
#define P1_07 207
#define P1_08 208
#define P1_09 209
#define P1_10 210
#define P1_11 211
#define P1_12 212
#define P1_13 213
#define P1_14 214
#define P1_15 215
#define P1_16 216
#define P1_17 217
#define P1_18 218
#define P1_19 219
#define P1_20 220
#define P1_21 221
#define P1_22 222
#define P1_23 223
#define P1_24 224
#define P1_25 225
#define P1_26 226
#define P1_27 227
#define P1_28 228
#define P1_29 229
#define P1_30 230
#define P1_31 231

#define P2_00 300
#define P2_01 301
#define P2_02 302
#define P2_03 303
#define P2_04 304
#define P2_05 305
#define P2_06 306
#define P2_07 307
#define P2_08 308
#define P2_09 309
#define P2_10 310
#define P2_11 311
#define P2_12 312
#define P2_13 313
#define P2_14 314
#define P2_15 315
#define P2_16 316
#define P2_17 317
#define P2_18 318
#define P2_19 319
#define P2_20 320
#define P2_21 321
#define P2_22 322
#define P2_23 323
#define P2_24 324
#define P2_25 325
#define P2_26 326
#define P2_27 327
#define P2_28 328
#define P2_29 329
#define P2_30 330
#define P2_31 331

#define P3_00 400
#define P3_01 401
#define P3_02 402
#define P3_03 403
#define P3_04 404
#define P3_05 405
#define P3_06 406
#define P3_07 407
#define P3_08 408
#define P3_09 409
#define P3_10 410
#define P3_11 411
#define P3_12 412
#define P3_13 413
#define P3_14 414
#define P3_15 415
#define P3_16 416
#define P3_17 417
#define P3_18 418
#define P3_19 419
#define P3_20 420
#define P3_21 421
#define P3_22 422
#define P3_23 423
#define P3_24 424
#define P3_25 425
#define P3_26 426
#define P3_27 427
#define P3_28 428
#define P3_29 429
#define P3_30 430
#define P3_31 431

#define P4_00 500
#define P4_01 501
#define P4_02 502
#define P4_03 503
#define P4_04 504
#define P4_05 505
#define P4_06 506
#define P4_07 507
#define P4_08 508
#define P4_09 509
#define P4_10 510
#define P4_11 511
#define P4_12 512
#define P4_13 513
#define P4_14 514
#define P4_15 515
#define P4_16 516
#define P4_17 517
#define P4_18 518
#define P4_19 519
#define P4_20 520
#define P4_21 521
#define P4_22 522
#define P4_23 523
#define P4_24 524
#define P4_25 525
#define P4_26 526
#define P4_27 527
#define P4_28 528
#define P4_29 529
#define P4_30 530
#define P4_31 531
