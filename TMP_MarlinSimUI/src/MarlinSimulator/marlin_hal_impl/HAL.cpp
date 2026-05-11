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

#include <src/inc/MarlinConfig.h>
#include <src/HAL/shared/Delay.h>

MSerialT serial_stream_0(false);
MSerialT serial_stream_1(false);
MSerialT serial_stream_2(false);
MSerialT serial_stream_3(false);

// U8glib required functions
extern "C" {
  void u8g_xMicroDelay(uint16_t val) { DELAY_US(val); }
  void u8g_MicroDelay()              { u8g_xMicroDelay(1); }
  void u8g_10MicroDelay()            { u8g_xMicroDelay(10); }
  void u8g_Delay(uint16_t val)       { delay(val); }
}

//************************//

// return free heap space
int freeMemory() {
  return 0;
}

// ------------------------
// ADC
// ------------------------

void MarlinHAL::adc_init() {

}

void MarlinHAL::adc_enable(const uint8_t ch) {

}

uint8_t MarlinHAL::active_ch = 0;
void MarlinHAL::adc_start(const uint8_t ch) {
  active_ch = ch;
}

bool MarlinHAL::adc_ready() {
  return true;
}

uint16_t MarlinHAL::adc_value() {
  pin_t pin = analogInputToDigitalPin(active_ch);
  if (!isValidPin(pin)) return 0;
  uint16_t data = ((Gpio::get(pin) >> 2) & 0x3FF);
  return data;    // return 10bit value as Marlin expects
}

void MarlinHAL::reboot() {
  Kernel::shutdown();
}

void MarlinHAL::idletask() {  Kernel::yield(); };

void MarlinHAL::watchdog_refresh() {
  Kernel::yield();
}

void MarlinHAL::watchdog_init() {}

// Maple Compatibility
volatile uint32_t systick_uptime_millis = 0;
systickCallback_t systick_user_callback;
void systick_attach_callback(systickCallback_t cb) { systick_user_callback = cb; }
void SYSTICK_IRQHandler() {
  systick_uptime_millis++;
  if (systick_user_callback) systick_user_callback();
}
