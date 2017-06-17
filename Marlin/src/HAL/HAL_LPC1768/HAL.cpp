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


/**
 *
 * For TARGET_LPC1768
 */

#ifdef TARGET_LPC1768

#include "../HAL.h"

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
int freeMemory(){
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
#include "../../../language.h"

extern void kill(const char*);
extern const char errormagic[];

void HAL_adc_enable_channel(int pin) {
  if (pin < 0 || pin >= NUM_ANALOG_INPUTS)  {
    usb_serial.printf("%sINVALID ANALOG PORT:%d\n", errormagic, pin);
    kill(MSG_KILLED);
  }

  int8_t pin_port = adc_pin_map[pin].port;
  int8_t pin_port_pin = adc_pin_map[pin].pin;
  int8_t pinsel_start_bit = pin_port_pin > 15 ? 2 * (pin_port_pin - 16) : 2 * pin_port_pin;
  uint8_t pin_sel_register = (pin_port == 0 && pin_port_pin <= 15) ? 0 :
                             (pin_port == 0)                       ? 1 :
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

void HAL_adc_start_conversion(uint8_t adc_pin) {
  if(  (adc_pin >= NUM_ANALOG_INPUTS) || (adc_pin_map[adc_pin].port == 0xFF) ) {
    usb_serial.printf("HAL: HAL_adc_start_conversion: no pinmap for %d\n",adc_pin);
    return;
  }
  LPC_ADC->ADCR &= ~0xFF;                                // Reset
  LPC_ADC->ADCR |= ( 0x01 << adc_pin_map[adc_pin].adc ); // Select Channel
  LPC_ADC->ADCR |= ( 0x01 << 24 );                       // start conversion
}

bool HAL_adc_finished(void) {
  uint32_t data = LPC_ADC->ADGDR;
  return LPC_ADC->ADGDR & ADC_DONE;
}

uint16_t HAL_adc_get_result(void) {
  uint32_t data = LPC_ADC->ADGDR;
  LPC_ADC->ADCR &= ~(1 << 24); //stop conversion
  if ( data & ADC_OVERRUN ) return 0;
  return ((data >> 6) & 0x3ff); //10bit
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
