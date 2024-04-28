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

#if ENABLED(BUFFER_MONITORING)
  #include "queue.h"
#endif

#include "../module/settings.h"
#include "../module/temperature.h"
#include "../libs/hex_print.h"
#include "../HAL/shared/eeprom_if.h"
#include "../HAL/shared/Delay.h"
#include "../sd/cardreader.h"
#include "../MarlinCore.h" // For kill

void dump_delay_accuracy_check();

/**
 * @brief Dn: G-code for development and testing
 *
 * Usage:
 *   D[<int>]
 *
 * Parameters:
 * @param  D-1   : Endless Loop
 * @param  D0    : Reset
 * @param  D10   : Kill Test. [P] to disable steppers
 * @param  D1    : Clear EEPROM and RESET
 * @param  D2    : Read/Write RAM
 * @param  D3    : Read/Write EEPROM
 * @param  D4    : Read/Write PIN
 * @param  D5    : Read/Write FLASH
 * @param  D6    : Check delay loop accuracy
 * @param  D7    : Dump the current serial port type (hence configuration)
 * @param  D100  : Disable heaters and attempt a hard hang (Watchdog Test)
 * @param  D101  : Test SD Write
 * @param  D102  : Test SD Read
 * @param  D451  : Trigger all kind of faults to test exception catcher
 * @param  D576  : Return buffer stats or set the auto-report interval
 *
 * @details See https://reprap.org/wiki/G-code#D:_Debug_codes
 *
 * NOTE: Put whatever else you need here to test ongoing development
 */
void GcodeSuite::D(const int16_t dcode) {
  switch (dcode) {

    /**
     * @brief D-1: Endless Loop
     */
    case -1:
      for (;;) { /* loop forever (watchdog reset) */ }

    /**
     * @brief D0: Reset
     */
    case 0: // Reset
      hal.reboot();
      break;

    /**
     * @brief D10: Kill Test. [P] to disable steppers
     */
    case 10:
      kill(F("D10"), F("KILL TEST"), parser.seen_test('P'));
      break;

    /**
     * @brief D1: Clear EEPROM and RESET
     */
    case 1: {
      // Zero or pattern-fill the EEPROM data
      #if ENABLED(EEPROM_SETTINGS)
        persistentStore.access_start();
        size_t total = persistentStore.capacity();
        int pos = 0;
        const uint8_t value = 0x0;
        while (total--) persistentStore.write_data(pos, &value, 1);
        persistentStore.access_finish();
      #else
        settings.reset();
        settings.save();
      #endif
      hal.reboot();
    } break;

    /**
     * @brief D2: Read/Write RAM
     *            Without any additional parameters will read the entire RAM
     *
     * Usage:
     *   D2 [A<address>] [C<Count>] [X<Data>]
     *
     * Parameters:
     * @param  Annnn  : Address (x0000-x1fff)
     * @param  Cnnnn  : Count (1-8192)
     * @param  Xnnnn  : Data (hex)
     */
    case 2: {
      #define SRAM_SIZE 8192
      uint8_t *pointer = parser.hex_adr_val('A');
      uint16_t len = parser.ushortval('C', 1);
      uintptr_t addr = (uintptr_t)pointer;
      NOMORE(addr, size_t(SRAM_SIZE - 1));
      NOMORE(len, SRAM_SIZE - addr);
      if (parser.seenval('X')) {
        // Write the hex bytes after the X
        uint16_t val = parser.hex_val('X');
        while (len--) {
          *pointer = val;
          pointer++;
        }
      }
      else {
        while (len--) print_hex_byte(*(pointer++));
        SERIAL_EOL();
      }
    } break;

    #if ENABLED(EEPROM_SETTINGS)
      /**
       * @brief D3: Read/Write EEPROM
       *            Without any additional parameters will read the entire EEPROM
       *
       * Usage:
       *   D3 [A<address>] [C<Count>] [X<Data>]
       *
       * Parameters:
       * @param  Annnn  : Address (x0000-x0fff)
       * @param  Cnnnn  : Count (1-4096)
       * @param  Xnnnn  : Data (hex)
       */
      case 3: {
        uint8_t *pointer = parser.hex_adr_val('A');
        uint16_t len = parser.ushortval('C', 1);
        uintptr_t addr = (uintptr_t)pointer;
        NOMORE(addr, size_t(persistentStore.capacity() - 1));
        NOMORE(len, persistentStore.capacity() - addr);
        if (parser.seenval('X')) {
          uint16_t val = parser.hex_val('X');
          #if ENABLED(EEPROM_SETTINGS)
            persistentStore.access_start();
            while (len--) {
              int pos = 0;
              persistentStore.write_data(pos, (uint8_t *)&val, sizeof(val));
            }
            SERIAL_EOL();
            persistentStore.access_finish();
          #else
            SERIAL_ECHOLNPGM("NO EEPROM");
          #endif
        }
        else {
          // Read bytes from EEPROM
          #if ENABLED(EEPROM_SETTINGS)
            persistentStore.access_start();
            int pos = 0;
            uint8_t val;
            while (len--) if (!persistentStore.read_data(pos, &val, 1)) print_hex_byte(val);
            SERIAL_EOL();
            persistentStore.access_finish();
          #else
            SERIAL_ECHOLNPGM("NO EEPROM");
            len = 0;
          #endif
          SERIAL_EOL();
        }
      } break;
    #endif

    /**
     * @brief D4: D4 Read / Write PIN
     */
    case 4: {
      //const bool is_out = parser.boolval('F');
      //const uint8_t pin = parser.byteval('P'),
      //              val = parser.byteval('V', LOW);
      if (parser.seenval('X')) {
        // TODO: Write the hex bytes after the X
        //while (len--) {
        //}
      }
      else {
        //while (len--) {
        //// TODO: Read bytes from EEPROM
        //  print_hex_byte(eeprom_read_byte(adr++));
        //}
        SERIAL_EOL();
      }
    } break;

    /**
     * @brief D5: Read/Write FLASH
     *            Without any additional parameters will read the 1kb FLASH
     *
     * Usage:
     *   D5 [A<address>] [C<Count>] [X<Data>]
     *
     * Parameters:
     * @param  Annnn  : Address (x00000-x3ffff)
     * @param  Cnnnn  : Count (1-8192)
     * @param  Xnnnn  : Data (hex)
     *
     * WARNING: This will overwrite program and data, so don't use it!
     */
    case 5: {
      #define ONBOARD_FLASH_SIZE 1024 // 0x400
      uint8_t *pointer = parser.hex_adr_val('A');
      uint16_t len = parser.ushortval('C', 1);
      uintptr_t addr = (uintptr_t)pointer;
      NOMORE(addr, size_t(ONBOARD_FLASH_SIZE - 1));
      NOMORE(len, ONBOARD_FLASH_SIZE - addr);
      if (parser.seenval('X')) {
        // TODO: Write the hex bytes after the X
        //while (len--) {}
      }
      else {
        //while (len--) {
        //// TODO: Read bytes from FLASH
        //  print_hex_byte(flash_read_byte(adr++));
        //}
        SERIAL_EOL();
      }
    } break;

    /**
     * @brief D6: Check delay loop accuracy
     */
    case 6:
      dump_delay_accuracy_check();
      break;

    /**
     * @brief D7: Dump the current serial port type (hence configuration)
     */
    case 7:
      SERIAL_ECHOLNPGM("Current serial configuration RX_BS:", RX_BUFFER_SIZE, ", TX_BS:", TX_BUFFER_SIZE);
      SERIAL_ECHOLN(gtn(&SERIAL_IMPL));
      break;

    /**
     * @brief D100: Disable heaters and attempt a hard hang (Watchdog Test)
     */
    case 100: {
      SERIAL_ECHOLNPGM("Disabling heaters and attempting to trigger Watchdog");
      SERIAL_ECHOLNPGM("(USE_WATCHDOG " TERN(USE_WATCHDOG, "ENABLED", "DISABLED") ")");
      thermalManager.disable_all_heaters();
      delay(1000); // Allow time to print
      hal.isr_off();
      // Use a low-level delay that does not rely on interrupts to function
      // Do not spin forever, to avoid thermal risks if heaters are enabled and
      // watchdog does not work
      for (int i = 10000; i--;) DELAY_US(1000UL);
      hal.isr_on();
      SERIAL_ECHOLNPGM("FAILURE: Watchdog did not trigger board reset.");
    } break;

    #if HAS_MEDIA

      /**
       * @brief D101: Test SD Write
       */
      case 101: {
        card.openFileWrite("test.gco");
        if (!card.isFileOpen()) {
          SERIAL_ECHOLNPGM("Failed to open test.gco to write.");
          return;
        }
        __attribute__((aligned(sizeof(size_t)))) uint8_t buf[512];

        uint16_t c;
        for (c = 0; c < COUNT(buf); c++)
          buf[c] = 'A' + (c % ('Z' - 'A'));

        c = 1024 * 4;
        while (c--) {
          hal.watchdog_refresh();
          card.write(buf, COUNT(buf));
        }
        SERIAL_ECHOLNPGM(" done");
        card.closefile();
      } break;

      /**
       * @brief D102: Test SD Read
       */
      case 102: {
        char testfile[] = "test.gco";
        card.openFileRead(testfile);
        if (!card.isFileOpen()) {
          SERIAL_ECHOLNPGM("Failed to open test.gco to read.");
          return;
        }
        __attribute__((aligned(sizeof(size_t)))) uint8_t buf[512];
        uint16_t c = 1024 * 4;
        while (c--) {
          hal.watchdog_refresh();
          card.read(buf, COUNT(buf));
          bool error = false;
          for (uint16_t i = 0; i < COUNT(buf); i++) {
            if (buf[i] != ('A' + (i % ('Z' - 'A')))) {
              error = true;
              break;
            }
          }
          if (error) {
            SERIAL_ECHOLNPGM(" Read error!");
            break;
          }
        }
        SERIAL_ECHOLNPGM(" done");
        card.closefile();
      } break;

    #endif // HAS_MEDIA

    #if ENABLED(POSTMORTEM_DEBUGGING)

      /**
       * @brief D451: Trigger all kind of faults to test exception catcher
       *
       * Usage:
       *   D451 [T<int>]
       *
       * Parameters:
       * @param  T1  : Write at bad address
       * @param  T2  : Divide by zero (some CPUs accept this, like ARM)
       * @param  T3  : Unaligned access (some CPUs accept this)
       * @param  T4  : Invalid instruction
       */
      case 451: {
        SERIAL_ECHOLNPGM("Disabling heaters");
        thermalManager.disable_all_heaters();
        delay(1000); // Allow time to print
        volatile uint8_t type[5] = { parser.byteval('T', 1) };

        // The code below is obviously wrong and it's full of quirks to fool the compiler from optimizing away the code
        switch (type[0]) {
          case 1: default: *(int*)0 = 451; break;
          case 2: { volatile int a = 0; volatile int b = 452 / a; *(int*)&a = b; } break;
          case 3: { *(uint32_t*)&type[1] = 453; volatile int a = *(int*)&type[1]; type[0] = a / 255; } break;
          case 4: { volatile void (*func)() = (volatile void (*)()) 0xE0000000; func(); } break;
        }
        break;
      }

    #endif

    #if ENABLED(BUFFER_MONITORING)

      /**
       * @brief D576: Return buffer stats or set the auto-report interval
       *
       * Usage:
       *   D576 [S<seconds>]
       *
       * Parameters:
       * @param  S  : Set auto report interval in seconds
       *
       * With no parameters emits the following output:
       * "D576 P<nn> B<nn> PU<nn> PD<nn> BU<nn> BD<nn>"
       * Where:
       *   P   : Planner buffers free
       *   B   : Command buffers free
       *   PU  : Planner buffer underruns (since the last report)
       *   PD  : Longest duration (ms) the planner buffer was empty (since the last report)
       *   BU  : Command buffer underruns (since the last report)
       *   BD  : Longest duration (ms) command buffer was empty (since the last report)
       */
      case 576: {
        if (parser.seenval('S'))
          queue.set_auto_report_interval((uint8_t)parser.value_byte());
        else
          queue.report_buffer_statistics();
        break;
      }

    #endif // BUFFER_MONITORING
  }
}

#endif // MARLIN_DEV_MODE
