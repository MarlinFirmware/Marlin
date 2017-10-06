/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * The U8glib-ARM has multiple hooks in it to facilitate adding ARM/HAL specific
 * routines to the library.  This file selects the appropriate definition file based
 * on the platform being compiled.
 *
 * The U8glib-ARM system usually selects the appropriate software via a long chain of
 * "if this platform then .." terminated by a "if not defined yet then use the default/dummy
 * option".  As long as the ARM/HAL definition is in place before the start of the chain
 * then it's what'll be used.  It can't be done after the chain is completed.
 *
 * The hooks the U8glib-ARM library are all prefaced with "defined(__arm__)".  They are:
 *  u8g.h - enables the U8G_WITH_PINLIST routines
 *        - just after the data structures are defined and before the chains start, this
 *          file gets pulled in.  Here we define the pointers to the code needed by supporting
 *          files and here is were we define any custom communication and/or device drivers
 *          and where we define what drivers will be used.
 *  u8g_com_io.c - pulls in the "HAL_LCD_pin_manipulation" file which contains the low level
 *                 pin setup and pin write routines.
 *  u8g_delay.c - pulls in the "HAL_LCD_delay" file which provides the delay funtions.
 *
 * You will also find "defined(__arm__)" is used by Teensy to pull in their custom code.  These
 * are always buried inside other qualifiers so the ARM/HAL and the Teensy items are independent
 * of each other.
*/


#ifndef HAL_LCD_DEFINES_H_
#define HAL_LCD_DEFINES_H_

#ifdef ARDUINO_ARCH_SAM
//  #include "HAL_DUE/HAL_DUE_LCD_defines.h"

#elif defined(IS_32BIT_TEENSY)
//  #include "HAL_TEENSY35_36/HAL_TEENSY_LCD_defines.h"

#elif defined(ARDUINO_ARCH_AVR)
//  #include "HAL_AVR/HAL_ARDUINO_LCD_defines.h"

#elif defined(TARGET_LPC1768)
  #include "HAL_LPC1768/HAL_LPC1768_LCD_defines.h"

#else
  #error "Unsupported Platform!"
#endif

#endif // HAL_LCD_DEFINES_H_
