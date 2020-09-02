/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../inc/MarlinConfigPre.h"

#if ENABLED(MARLIN_DEV_MODE)

  #include "gcode.h"
  #include "../module/settings.h"
  #include "../libs/hex_print.h"
  #include "../HAL/shared/eeprom_if.h"

  /**
   * Dn: G-code for development and testing
   *
   * See https://reprap.org/wiki/G-code#D:_Debug_codes
   *
   * Put whatever else you need here to test ongoing development.
   */
  void GcodeSuite::D(const int16_t dcode) {
    switch (dcode) {

      case -1:
        for (;;); // forever

      case 0:
        HAL_reboot();
        break;

      case 1:
        // TODO: Zero or pattern-fill the EEPROM data
        //settings.reset();
        //settings.save();
        HAL_reboot();
        break;

      case 2: { // D2 Read / Write SRAM
        #define SRAM_SIZE 8192
        uint8_t *adr = parser.hex_adr_val('A');
        uint16_t len = parser.ushortval('C', 1);
        NOMORE(*adr, SRAM_SIZE - 1);
        NOMORE(len, SRAM_SIZE - (uint16_t)adr);
        if (parser.seenval('X')) {
          // TODO: Write the hex bytes after the X
          //while (len--) {
          //}
        }
        else {
          while (len--) print_hex_byte(*(adr++));
          SERIAL_EOL();
        }
      } break;

      case 3: { // D3 Read / Write EEPROM
        uint8_t *adr = parser.hex_adr_val('A');
        uint16_t len = parser.ushortval('C', 1);
        #ifndef MARLIN_EEPROM_SIZE
          #define MARLIN_EEPROM_SIZE size_t(E2END + 1)
        #endif
        NOMORE(*adr, MARLIN_EEPROM_SIZE - 1);
        NOMORE(len, MARLIN_EEPROM_SIZE - (uint16_t)adr);
        if (parser.seenval('X')) {
          // TODO: Write the hex bytes after the X
          //while (len--) {
          //}
        }
        else {
          while (len--) {
            // TODO: Read bytes from EEPROM
            // print_hex_byte(eeprom_read_byte(adr++));
          }
          SERIAL_EOL();
        }
      } break;

      case 4: { // D4 Read / Write PIN
        // const uint8_t pin = parser.byteval('P');
        // const bool is_out = parser.boolval('F'),
        //            val = parser.byteval('V', LOW);
        if (parser.seenval('X')) {
          // TODO: Write the hex bytes after the X
          //while (len--) {
          //}
        }
        else {
          // while (len--) {
            // TODO: Read bytes from EEPROM
            // print_hex_byte(eeprom_read_byte(*(adr++));
          // }
          SERIAL_EOL();
        }
      } break;

      case 5: { // D4 Read / Write onboard Flash
        #define FLASH_SIZE 1024
        uint8_t *adr = parser.hex_adr_val('A');
        uint16_t len = parser.ushortval('C', 1);
        NOMORE(*adr, FLASH_SIZE - 1);
        NOMORE(len, FLASH_SIZE - (uint16_t)adr);
        if (parser.seenval('X')) {
          // TODO: Write the hex bytes after the X
          //while (len--) {
          //}
        }
        else {
          // while (len--) {
            // TODO: Read bytes from EEPROM
            // print_hex_byte(eeprom_read_byte(adr++));
          // }
          SERIAL_EOL();
        }
      } break;
    }
  }

#endif
