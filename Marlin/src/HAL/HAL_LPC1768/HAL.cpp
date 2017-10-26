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

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

extern "C" {
  //#include <lpc17xx_adc.h>
  //#include <lpc17xx_pinsel.h>
}

HalSerial usb_serial;

//u8glib required fucntions
extern "C" void u8g_xMicroDelay(uint16_t val) {
  delayMicroseconds(val);
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
  char stack_end;
  void *heap_start = malloc(sizeof(uint32_t));
  if (heap_start == 0) return 0;

  uint32_t result = (uint32_t)&stack_end - (uint32_t)heap_start;
  free(heap_start);
  return result;
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

#define ADC_DONE      0x80000000
#define ADC_OVERRUN   0x40000000

void HAL_adc_init(void) {
  LPC_SC->PCONP |= (1 << 12);      // Enable CLOCK for internal ADC controller
  LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
  LPC_SC->PCLKSEL0 |= (0x1 << 24); // 0: 25MHz, 1: 100MHz, 2: 50MHz, 3: 12.5MHZ to ADC clock divider
  LPC_ADC->ADCR = (0 << 0)         // SEL: 0 = no channels selected
                | (0xFF << 8)      // select slowest clock for A/D conversion 150 - 190 uS for a complete conversion
                | (0 << 16)        // BURST: 0 = software control
                | (0 << 17)        // CLKS: not applicable
                | (1 << 21)        // PDN: 1 = operational
                | (0 << 24)        // START: 0 = no start
                | (0 << 27);       // EDGE: not applicable
}

// externals need to make the call to KILL compile
#include "../../core/language.h"

extern void kill(const char*);
extern const char errormagic[];

void HAL_adc_enable_channel(int ch) {
  pin_t pin = analogInputToDigitalPin(ch);

  if (pin == -1) {
    MYSERIAL.printf("%sINVALID ANALOG PORT:%d\n", errormagic, ch);
    kill(MSG_KILLED);
  }

  int8_t pin_port = LPC1768_PIN_PORT(pin),
         pin_port_pin = LPC1768_PIN_PIN(pin),
         pinsel_start_bit = pin_port_pin > 15 ? 2 * (pin_port_pin - 16) : 2 * pin_port_pin;
  uint8_t pin_sel_register = (pin_port == 0 && pin_port_pin <= 15) ? 0 :
                              pin_port == 0                        ? 1 :
                              pin_port == 1                        ? 3 : 10;

  switch (pin_sel_register) {
    case 1 :
      LPC_PINCON->PINSEL1 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL1 |=  (0x1 << pinsel_start_bit);
      break;
    case 3 :
      LPC_PINCON->PINSEL3 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL3 |=  (0x3 << pinsel_start_bit);
      break;
    case 0 :
      LPC_PINCON->PINSEL0 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL0 |=  (0x2 << pinsel_start_bit);
      break;
  };
}

uint8_t active_adc = 0;
void HAL_adc_start_conversion(const uint8_t ch) {
  if (analogInputToDigitalPin(ch) == -1) {
    MYSERIAL.printf("HAL: HAL_adc_start_conversion: invalid channel %d\n", ch);
    return;
  }

  LPC_ADC->ADCR &= ~0xFF; // Reset
  SBI(LPC_ADC->ADCR, ch); // Select Channel
  SBI(LPC_ADC->ADCR, 24); // Start conversion
  active_adc = ch;
}

bool HAL_adc_finished(void) {
  return LPC_ADC->ADGDR & ADC_DONE;
}

// possible config options if something similar is extended to more platforms.
#define ADC_USE_MEDIAN_FILTER      // filter out erroneous readings
#define ADC_USE_LOWPASS_FILTER     // filter out high frequency noise
#define ADC_LOWPASS_K_VALUE 4      // how much to smooth out noise (1:8)

struct MedianFilter {
  uint16_t values[3];
  uint8_t next_val;
  MedianFilter() {
    next_val = 0;
    values[0] = values[1] = values[2] = 0;
  }
  uint16_t update(uint16_t value) {
    values[next_val++] = value;
    next_val = next_val % 3;
    return max(min(values[0], values[1]), min(max(values[0], values[1]), values[2]));     //median
  }
};

uint16_t lowpass_filter(uint16_t value) {
  const uint8_t k_data_shift = ADC_LOWPASS_K_VALUE;
  static uint32_t data_delay[NUM_ANALOG_INPUTS] = { 0 };
  uint32_t &active_filter = data_delay[active_adc];
  active_filter = active_filter - (active_filter >> k_data_shift) + value;
  return (uint16_t)(active_filter >> k_data_shift);
}

uint16_t HAL_adc_get_result(void) {
  uint32_t data = LPC_ADC->ADGDR;
  CBI(LPC_ADC->ADCR, 24);    // Stop conversion
  if (data & ADC_OVERRUN) return 0;
  #ifdef ADC_USE_MEDIAN_FILTER
    static MedianFilter median_filter[NUM_ANALOG_INPUTS];
    data = median_filter[active_adc].update((uint16_t)data);
  #endif
  #ifdef ADC_USE_LOWPASS_FILTER
    data = lowpass_filter((uint16_t)data);
  #endif
  return ((data >> 6) & 0x3ff);    // 10bit
}

#define SBIT_CNTEN     0
#define SBIT_PWMEN     2
#define SBIT_PWMMR0R   1

#define PWM_1          0  //P2_0 (0-1 Bits of PINSEL4)
#define PWM_2          2  //P2_1 (2-3 Bits of PINSEL4)
#define PWM_3          4  //P2_2 (4-5 Bits of PINSEL4)
#define PWM_4          6  //P2_3 (6-7 Bits of PINSEL4)
#define PWM_5          8  //P2_4 (8-9 Bits of PINSEL4)
#define PWM_6          10 //P2_5 (10-11 Bits of PINSEL4)

void HAL_pwm_init(void) {
  LPC_PINCON->PINSEL4 = _BV(PWM_5) | _BV(PWM_6);

  LPC_PWM1->TCR = _BV(SBIT_CNTEN) | _BV(SBIT_PWMEN);
  LPC_PWM1->PR  =  0x0;               // No prescalar
  LPC_PWM1->MCR = _BV(SBIT_PWMMR0R);  // Reset on PWMMR0, reset TC if it matches MR0
  LPC_PWM1->MR0 = 255;                /* set PWM cycle(Ton+Toff)=255) */
  LPC_PWM1->MR5 = 0;                  /* Set 50% Duty Cycle for the channels */
  LPC_PWM1->MR6 = 0;

  // Trigger the latch Enable Bits to load the new Match Values MR0, MR5, MR6
  LPC_PWM1->LER = _BV(0) | _BV(5) | _BV(6);
  // Enable the PWM output pins for PWM_5-PWM_6(P2_4 - P2_5)
  LPC_PWM1->PCR = _BV(13) | _BV(14);
}

#endif // TARGET_LPC1768
