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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_PRUSA_MMU3

#include "../../gcode.h"
#include "../../../module/settings.h"
#include "../../../feature/mmu3/mmu2.h"

// Shared by the G-codes below to save flash memory.
static void gcodes_M704_M705_M706(uint16_t gcode) {
  const int8_t mmuSlotIndex = parser.intval('P', -1);

  if (mmu2.enabled() && WITHIN(mmuSlotIndex, 0, EXTRUDERS - 1)) {
    switch (gcode) {
      case 704: mmu2.load_to_feeder(mmuSlotIndex); break;
      case 705: mmu2.eject_filament(mmuSlotIndex, false); break;
      case 706:
        #if ENABLED(MMU_HAS_CUTTER)
          if (mmu2.cutter_mode > 0) mmu2.cut_filament(mmuSlotIndex);
        #endif
        break;
      default: break;
    }
  }
}

/**
 * ### M704 - Preload to MMU <a href="https://reprap.org/wiki/G-code#M704:_Preload_to_MMU">M704: Preload to MMU</a>
 * #### Usage
 *
 *   M704 [ P ]
 *
 * #### Parameters
 * - `P` - n index of slot (zero based, so 0-4 like T0 and T4)
 */
void GcodeSuite::M704() {
  gcodes_M704_M705_M706(704);
}

/**
 * ### M705 - Eject filament <a href="https://reprap.org/wiki/G-code#M705:_Eject_filament">M705: Eject filament</a>
 * #### Usage
 *
 *   M705 [ P ]
 *
 * #### Parameters
 * - `P` - n index of slot (zero based, so 0-4 like T0 and T4)
 */
void GcodeSuite::M705() {
  gcodes_M704_M705_M706(705);
}

/*!
 * ### M706 - Cut filament <a href="https://reprap.org/wiki/G-code#M706:_Cut_filament">M706: Cut filament</a>
 * #### Usage
 *
 *   M706 [ P ]
 *
 * #### Parameters
 * - `P` - n index of slot (zero based, so 0-4 like T0 and T4)
 */
void GcodeSuite::M706() {
  gcodes_M704_M705_M706(706);
}

/**
 * ### M707 - Read from MMU register <a href="https://reprap.org/wiki/G-code#M707:_Read_from_MMU_register">M707: Read from MMU register</a>
 * #### Usage
 *
 *   M707 [ A ]
 *
 * #### Parameters
 * - `A` - Address of register in hexidecimal.
 *
 * #### Example
 *
 * M707 A0x1b - Read a 8bit integer from register 0x1b and prints the result onto the serial line.
 *
 * Does nothing if the A parameter is not present or if MMU is not enabled.
 *
 */
void GcodeSuite::M707() {
  if (mmu2.enabled() ) {
    if (parser.seenval('A') ) {
      char *address = parser.stringval('A');
      mmu2.readRegister(uint8_t(strtol(address, NULL, 16)));
    }
  }
}

/**
 * ### M708 - Write to MMU register <a href="https://reprap.org/wiki/G-code#M708:_Write_to_MMU_register">M707: Write to MMU register</a>
 * #### Usage
 *
 *   M708 [ A | X ]
 *
 * #### Parameters
 * - `A` - Address of register in hexidecimal.
 * - `X` - Data to write (16-bit integer). Default value 0.
 *
 * #### Example
 * M708 A0x1b X05 - Write to register 0x1b the value 05.
 *
 * Does nothing if A parameter is missing or if MMU is not enabled.
 */
void GcodeSuite::M708() {
  if (mmu2.enabled() ) {
    uint8_t addr = 0;
    if (parser.seenval('A') ) {
      char *address = parser.stringval('A');
      addr = uint8_t(strtol(address, NULL, 16));
    }
    uint16_t data = 0;
    if (parser.seenval('X') ) {
      data = parser.ushortval('X', 0);
    }
    if (addr) {
      mmu2.writeRegister(addr, data);
    }
  }
}

/**
 * ### M709 - MMU power & reset <a href="https://reprap.org/wiki/G-code#M709:_MMU_power_&_reset">M709: MMU power & reset</a>
 * The MK3S cannot not power off the MMU, but we can en- and disable the MMU.
 *
 * The new state of the MMU is stored in printer's EEPROM.
 * i.e., If you disable the MMU via M709, it will not be activated after the printer resets.
 * Usage
 *
 *   M709 [ S | X ]
 *
 * Parameters
 * - `X` - Reset MMU (0:soft reset | 1:hardware reset | 42: erase MMU eeprom)
 * - `S` - En-/disable the MMU (0:off | 1:on)
 *
 * Examples
 *
 *  M709 X0   ; issue an X0 command via communication into the MMU (soft reset)
 *  M709 X1   ; toggle the MMU's reset pin (hardware reset)
 *  M709 X42  ; erase MMU EEPROM
 *  M709 S1   ; enable MMU
 *  M709 S0   ; disable MMU
 *  M709      ; Serial message if en- or disabled
 */
void GcodeSuite::M709() {
  if (parser.seenval('S')) {
    switch (parser.byteval('S', -1)) {
      case 0: mmu2.stop(); break;
      case 1: mmu2.start(); break;
      default: break;
    }
  }
  if (mmu2.enabled() && parser.seenval('X')) {
    switch (parser.byteval('X', -1)) {
      case  0: mmu2.reset(MMU2::MMU2::Software); break;
      case  1: mmu2.reset(MMU2::MMU2::ResetPin); break;
      case 42: mmu2.reset(MMU2::MMU2::EraseEEPROM); break;
      default: break;
    }
  }
  mmu2.status();
}

#endif // HAS_PRUSA_MMU3
