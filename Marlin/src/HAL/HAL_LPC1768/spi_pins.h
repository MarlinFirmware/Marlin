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

#ifndef SPI_PINS_LPC1768_H
#define SPI_PINS_LPC1768_H

#define LPC_SOFTWARE_SPI  // Re-ARM board needs a software SPI because using the
                          // standard LCD adapter results in the LCD and the
                          // SD card sharing a single SPI when the RepRap Full
                          // Graphic Smart Controller is selected

/** onboard SD card */
//#define SCK_PIN           P0_07
//#define MISO_PIN          P0_08
//#define MOSI_PIN          P0_09
//#define SS_PIN            P0_06
/** external */
#ifndef SCK_PIN
  #define SCK_PIN           P0_15
#endif
#ifndef MISO_PIN
  #define MISO_PIN          P0_17
#endif
#ifndef MOSI_PIN
  #define MOSI_PIN          P0_18
#endif
#ifndef SS_PIN
  #define SS_PIN            P1_23
#endif
#ifndef SDSS
  #define SDSS              SS_PIN
#endif

#endif // SPI_PINS_LPC1768_H
