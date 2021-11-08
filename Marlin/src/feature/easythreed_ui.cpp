/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(EASYTHREED_UI)

#include "easythreed_ui.h"
#include "pause.h"
#include "../module/temperature.h"
#include "../module/printcounter.h"
#include "../sd/cardreader.h"
#include "../gcode/queue.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../MarlinCore.h"

EasythreedUI easythreed_ui;

void EasythreedUI::init() {
  SET_INPUT_PULLUP(PRINT_HOME_PIN);
  SET_OUTPUT(HOME_GND_PIN);
  SET_INPUT_PULLUP(FEED_PIN);
  SET_INPUT_PULLUP(RETRACT_PIN);
  SET_OUTPUT(FEED_GND_PIN);
  SET_OUTPUT(RETRACT_GND_PIN);
  SET_INPUT_PULLUP(PRINTER_PIN);
  SET_OUTPUT(PRINT_LED_PIN);
}

void EasythreedUI::UICheck(void) {
  NanoBlinkLED();
  NanoLoadFilament();
  NanoPrintOneKey();
}

//
// EasyThreeD ET4000+ Mainboard for Nano / K7
//

// Status LED on Start button
uint16_t blink_time = 0;
enum LEDStatus : uint16_t {
  LED_OFF      =    0,
  LED_ON       = 4000,
  LED_BLINK_0  = 2500,
  LED_BLINK_1  = 1500,
  LED_BLINK_2  = 1000,
  LED_BLINK_3  =  800,
  LED_BLINK_4  =  500,
  LED_BLINK_5  =  300,
  LED_BLINK_6  =  150,
  LED_BLINK_7  =   50
};

void EasythreedUI::NanoBlinkLED(void) {
  static millis_t blink_previous_ms = 0, blink_start_ms = 0;

  if (blink_time == LED_OFF) { WRITE(PRINT_LED_PIN, HIGH); return; }  // OFF
  if (blink_time > 3000)     { WRITE(PRINT_LED_PIN,  LOW); return; }  // ON

  const millis_t ms = millis();
  if (blink_previous_ms != blink_time) {
    blink_previous_ms = blink_time;
    blink_start_ms = ms;
  }
  if (PENDING(ms, blink_start_ms + blink_time))
    WRITE(PRINT_LED_PIN, LOW);
  else if (PENDING(ms, blink_start_ms + 2 * blink_time))
    WRITE(PRINT_LED_PIN, HIGH);
  else
    blink_start_ms = ms;
}

// Filament Load/Unload Switch
void EasythreedUI::NanoLoadFilament(void) {
  if (printingIsActive()) return;

  static millis_t filament_time = 0;
  static uint8_t filament_status = 0;
  switch (filament_status) {
    case 0:
      if (READ(RETRACT_PIN) == LOW || READ(FEED_PIN) == LOW) {
        filament_status++;
        filament_time = millis();
      }
      break;

    case 1:
      if (ELAPSED(millis(), filament_time + 20)) {
        if (READ(RETRACT_PIN) == LOW || READ(FEED_PIN) == LOW) {
          thermalManager.setTargetHotend(210, 0);
          blink_time = LED_BLINK_7;
          filament_status++;
        }
        else
          filament_status = 0;
      }
      break;

    case 2:
      if (thermalManager.degHotend(0) >= 180.0f) {
        filament_status++;
        blink_time = LED_BLINK_5;
      }
      if (READ(RETRACT_PIN) == HIGH && READ(FEED_PIN) == HIGH) {
        blink_time = LED_ON;
        filament_status = 0;
        thermalManager.disable_all_heaters();
      }
      break;

    case 3: {
      static bool flag = false;
      if (!flag && READ(RETRACT_PIN) == LOW) {
        flag = true;
        queue.inject(F("G91\nG0 E10 F180\nG0 E-120 F180\nM104 S0"));
        blink_time = LED_BLINK_5;
      }
      if (!flag && READ(FEED_PIN) == LOW) {
        flag = true;
        queue.inject(F("G91\nG0 E100 F120\nM104 S0"));
        blink_time = LED_BLINK_5;
      }
      if (READ(RETRACT_PIN) == HIGH && READ(FEED_PIN) == HIGH) {
        flag = false;
        filament_status = 0;
        quickstop_stepper();
        //planner.cleaning_buffer_counter = 2;
        thermalManager.disable_all_heaters();
      }
    } break;

    default: filament_status = 0;
  }

}

#if HAS_STEPPER_RESET
  void disableStepperDrivers();
#endif

// Start Button
void EasythreedUI::NanoPrintOneKey(void) {
  static uint8_t key_status = 0;
  static millis_t key_time = 0;
  static bool print_flag = false, print_key_flag = false;

  const millis_t ms = millis();

  switch (key_status) {
    case 0:
      if (READ(PRINTER_PIN) == LOW) {
        key_time = ms;
        key_status = 1;
      }
      if (print_flag && !printingIsActive()) {
        print_key_flag = print_flag = false;
        blink_time = LED_ON;
      }
      if (print_flag) blink_time = LED_BLINK_5;
      break;

    case 1:
      if (ELAPSED(ms, key_time + 30)) {
        if (READ(PRINTER_PIN) == LOW) {
          key_time = ms;
          key_status = 2;
        }
        else
          key_status = 0;
      }
      break;

    case 2:
      if (READ(PRINTER_PIN) == HIGH) {
        if (PENDING(ms, key_time + 1200)) { // short press print
          blink_time = LED_BLINK_5;
          if (!print_key_flag) {
            if (!printingIsActive()) {
              print_flag = true;
              card.mount();
              if (!card.isMounted) {
                blink_time = LED_OFF;
                key_status = 0;
                key_time = 0;
                print_flag = false;
                return;
              }
              print_key_flag = true;
              card.ls();
              uint16_t filecnt = card.countFilesInWorkDir();
              if (filecnt == 0) return;
              card.selectFileByIndex(filecnt);
              card.openAndPrintFile(card.filename);
            }
          }
          else
            print_key_flag = false;
        }
        else {
          blink_time = LED_BLINK_5;
          if (!print_key_flag)  // long press Z up 10mm
            queue.inject(F("G91\nG0 Z10 F600\nG90"));
          else {
            wait_for_heatup = false;
            quickstop_stepper();
            //planner.cleaning_buffer_counter = 2;
            thermalManager.disable_all_heaters();
            print_flag = false;
            blink_time = LED_ON;
          }
          planner.synchronize();
          TERN_(HAS_STEPPER_RESET, disableStepperDrivers());
          print_key_flag = false;
        }
        key_status = 0;
        key_time = 0;
      }
      break;

    default:
      key_status = 0;
      key_time = 0;
      break;
  }

}

#endif // EASYTHREED_UI
