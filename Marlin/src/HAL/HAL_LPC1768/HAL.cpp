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
    SERIAL_PRINTF("%sINVALID ANALOG PORT:%d\n", errormagic, ch);
    kill(MSG_KILLED);
  }

  int8_t pin_port = LPC1768_PIN_PORT(pin),
         pin_port_pin = LPC1768_PIN_PIN(pin),
         pinsel_start_bit = pin_port_pin > 15 ? 2 * (pin_port_pin - 16) : 2 * pin_port_pin;
  uint8_t pin_sel_register = (pin_port == 0 && pin_port_pin <= 15) ? 0 :
                              pin_port == 0                        ? 1 :
                              pin_port == 1                        ? 3 : 10;

  switch (pin_sel_register) {
    case 1:
      LPC_PINCON->PINSEL1 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL1 |=  (0x1 << pinsel_start_bit);
      break;
    case 3:
      LPC_PINCON->PINSEL3 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL3 |=  (0x3 << pinsel_start_bit);
      break;
    case 0:
      LPC_PINCON->PINSEL0 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL0 |=  (0x2 << pinsel_start_bit);
      break;
  };
}

void HAL_adc_start_conversion(const uint8_t ch) {
  if (analogInputToDigitalPin(ch) == -1) {
    SERIAL_PRINTF("HAL: HAL_adc_start_conversion: invalid channel %d\n", ch);
    return;
  }

  LPC_ADC->ADCR &= ~0xFF; // Reset
  SBI(LPC_ADC->ADCR, ch); // Select Channel
  SBI(LPC_ADC->ADCR, 24); // Start conversion
}

bool HAL_adc_finished(void) {
  return LPC_ADC->ADGDR & ADC_DONE;
}

// possible config options if something similar is extended to more platforms.
#define ADC_USE_MEDIAN_FILTER          // Filter out erroneous readings
#define ADC_MEDIAN_FILTER_SIZE    (23) // Higher values increase step delay (phase shift),
                                       // (ADC_MEDIAN_FILTER_SIZE + 1) / 2 sample step delay (12 samples @ 500Hz: 24ms phase shift)
                                       // Memory usage per ADC channel (bytes): (6 * ADC_MEDIAN_FILTER_SIZE) + 16
                                       // 8 * ((6 * 23) + 16 ) = 1232 Bytes for 8 channels

#define ADC_USE_LOWPASS_FILTER         // Filter out high frequency noise
#define ADC_LOWPASS_K_VALUE       (6)  // Higher values increase rise time
                                       // Rise time sample delays for 100% signal convergence on full range step
                                       // (1 : 13, 2 : 32, 3 : 67, 4 : 139, 5 : 281, 6 : 565, 7 : 1135, 8 : 2273)
                                       // K = 6, 565 samples, 500Hz sample rate, 1.13s convergence on full range step
                                       // Memory usage per ADC channel (bytes): 4 (32 Bytes for 8 channels)


// Sourced from https://embeddedgurus.com/stack-overflow/tag/median-filter/
struct MedianFilter {
  #define STOPPER 0                // Smaller than any datum
  struct Pair {
    Pair   *point;                 // Pointers forming list linked in sorted order
    uint16_t  value;               // Values to sort
  };

  Pair buffer[ADC_MEDIAN_FILTER_SIZE] = {}; // Buffer of nwidth pairs
  Pair *datpoint = buffer;                  // Pointer into circular buffer of data
  Pair small = {NULL, STOPPER};             // Chain stopper
  Pair big = {&small, 0};                   // Pointer to head (largest) of linked list.

  uint16_t update(uint16_t datum) {
    Pair *successor;                        // Pointer to successor of replaced data item
    Pair *scan;                             // Pointer used to scan down the sorted list
    Pair *scanold;                          // Previous value of scan
    Pair *median;                           // Pointer to median
    uint16_t i;

    if (datum == STOPPER) {
      datum = STOPPER + 1;                  // No stoppers allowed.
    }

    if ( (++datpoint - buffer) >= ADC_MEDIAN_FILTER_SIZE) {
      datpoint = buffer;                    // Increment and wrap data in pointer.
    }

    datpoint->value = datum;                // Copy in new datum
    successor = datpoint->point;            // Save pointer to old value's successor
    median = &big;                          // Median initially to first in chain
    scanold = NULL;                         // Scanold initially null.
    scan = &big;                            // Points to pointer to first (largest) datum in chain

    // Handle chain-out of first item in chain as special case
    if (scan->point == datpoint) {
      scan->point = successor;
    }
    scanold = scan;                         // Save this pointer and
    scan = scan->point ;                    // step down chain

    // Loop through the chain, normal loop exit via break.
    for (i = 0 ; i < ADC_MEDIAN_FILTER_SIZE; ++i) {
      // Handle odd-numbered item in chain
      if (scan->point == datpoint) {
        scan->point = successor;            // Chain out the old datum
      }

      if (scan->value < datum) {            // If datum is larger than scanned value
        datpoint->point = scanold->point;   // Chain it in here
        scanold->point = datpoint;          // Mark it chained in
        datum = STOPPER;
      }

      // Step median pointer down chain after doing odd-numbered element
      median = median->point;               // Step median pointer
      if (scan == &small) {
        break;                              // Break at end of chain
      }
      scanold = scan;                       // Save this pointer and
      scan = scan->point;                   // step down chain

      // Handle even-numbered item in chain.
      if (scan->point == datpoint) {
        scan->point = successor;
      }

      if (scan->value < datum) {
        datpoint->point = scanold->point;
        scanold->point = datpoint;
        datum = STOPPER;
      }

      if (scan == &small) {
        break;
      }

      scanold = scan;
      scan = scan->point;
    }
    return median->value;
  }
};

struct LowpassFilter {
  uint32_t data_delay = 0;
  uint16_t update(uint16_t value) {
    data_delay = data_delay - (data_delay >> ADC_LOWPASS_K_VALUE) + value;
    return (uint16_t)(data_delay >> ADC_LOWPASS_K_VALUE);
  }
};

uint16_t HAL_adc_get_result(void) {
  uint32_t adgdr = LPC_ADC->ADGDR;
  CBI(LPC_ADC->ADCR, 24);                    // Stop conversion

  if (adgdr & ADC_OVERRUN) return 0;
  uint16_t data = (adgdr >> 4) & 0xFFF;      // copy the 12bit data value
  uint8_t adc_channel = (adgdr >> 24) & 0x7; // copy the  3bit used channel

  #ifdef ADC_USE_MEDIAN_FILTER
    static MedianFilter median_filter[NUM_ANALOG_INPUTS];
    data = median_filter[adc_channel].update(data);
  #endif

  #ifdef ADC_USE_LOWPASS_FILTER
    static LowpassFilter lowpass_filter[NUM_ANALOG_INPUTS];
    data = lowpass_filter[adc_channel].update(data);
  #endif

  return ((data >> 2) & 0x3FF);    // return 10bit value as Marlin expects
}

#define SBIT_CNTEN     0
#define SBIT_PWMEN     2
#define SBIT_PWMMR0R   1

#define PWM_1          0  //P2_00 (0-1 Bits of PINSEL4)
#define PWM_2          2  //P2_01 (2-3 Bits of PINSEL4)
#define PWM_3          4  //P2_02 (4-5 Bits of PINSEL4)
#define PWM_4          6  //P2_03 (6-7 Bits of PINSEL4)
#define PWM_5          8  //P2_04 (8-9 Bits of PINSEL4)
#define PWM_6          10 //P2_05 (10-11 Bits of PINSEL4)

void HAL_pwm_init(void) {
  LPC_PINCON->PINSEL4 = _BV(PWM_5) | _BV(PWM_6);

  LPC_PWM1->TCR = _BV(SBIT_CNTEN) | _BV(SBIT_PWMEN);
  LPC_PWM1->PR  =  0x0;               // No prescalar
  LPC_PWM1->MCR = _BV(SBIT_PWMMR0R);  // Reset on PWMMR0, reset TC if it matches MR0
  LPC_PWM1->MR0 = 255;                // set PWM cycle(Ton+Toff)=255)
  LPC_PWM1->MR5 = 0;                  // Set 50% Duty Cycle for the channels
  LPC_PWM1->MR6 = 0;

  // Trigger the latch Enable Bits to load the new Match Values MR0, MR5, MR6
  LPC_PWM1->LER = _BV(0) | _BV(5) | _BV(6);
  // Enable the PWM output pins for PWM_5-PWM_6(P2_04 - P2_05)
  LPC_PWM1->PCR = _BV(13) | _BV(14);
}

#endif // TARGET_LPC1768
