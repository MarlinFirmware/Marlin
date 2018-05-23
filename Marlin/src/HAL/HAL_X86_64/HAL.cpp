/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#ifdef __PLAT_X86_64__

#include "../../inc/MarlinConfig.h"
#include "../Delay.h"

HalSerial usb_serial;

// U8glib required functions
extern "C" void u8g_xMicroDelay(uint16_t val) {
  DELAY_US(val);
}
extern "C" void u8g_MicroDelay(void) {
  u8g_xMicroDelay(1);
}
extern "C" void u8g_10MicroDelay(void) {
  u8g_xMicroDelay(10);
}
extern "C" void u8g_Delay(uint16_t val) {
  delay(val);
}
//************************//

// return free heap space
int freeMemory() {
  return 0;
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_init(void) {

}

// externals need to make the call to KILL compile
#include "../../core/language.h"


void HAL_adc_enable_channel(int ch) {

}

uint8_t active_ch = 0;
void HAL_adc_start_conversion(const uint8_t ch) {
  active_ch = ch;
}

bool HAL_adc_finished(void) {
  return true;
}

uint16_t HAL_adc_get_result(void) {
  pin_t pin = analogInputToDigitalPin(active_ch);
  if(!VALID_PIN(pin)) return 0;
  uint16_t data = ((Gpio::get(pin) >> 2) & 0x3FF);
 // SERIAL_PRINTF("adc: %d\n", data);
  return data;    // return 10bit value as Marlin expects
}


void HAL_pwm_init(void) {

}

#endif // __PLAT_X86_64__
