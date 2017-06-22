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

HalSerial usb_serial(USBTX, USBRX);
Timer system_counter;

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
  u8g_xMicroDelay(val*1000);
}
//************************//

int availableMemory(int resolution = 16, int maximum = 0x4000, bool disableInterrupts = false) {
    if (resolution < 1) resolution = 1;
    if (maximum < 0) maximum = 0;

    int low = 0;
    int high = maximum + 1;

    if (disableInterrupts) __disable_irq();
    while (high - low > resolution) {
        int mid = (low + high) / 2;
        void* p = malloc(mid);
        if (p == NULL) {
            high = mid;
        } else {
            free(p);
            low = mid;
        }
    }
    if (disableInterrupts) __enable_irq();
    return low;
}

// return free memory between end of heap (or end bss) and whatever is current
int freeMemory(){
	return availableMemory();
}


// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

#define ADC_DONE		  0x80000000
#define ADC_OVERRUN		0x40000000

void HAL_adc_init(void) {
  LPC_SC->PCONP |= (1 << 12);   // Enable CLOCK for internal ADC controller
  LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
  LPC_SC->PCLKSEL0 |= (0x1 << 24);
  LPC_ADC->ADCR = (0 << 0)      // SEL: 0 = no channels selected
                | (8 << 8)      // CLKDIV: PCLK max ~= 25MHz, /25 to give safe 1MHz at fastest
                | (0 << 16)     // BURST: 0 = software control
                | (0 << 17)     // CLKS: not applicable
                | (1 << 21)     // PDN: 1 = operational
                | (0 << 24)     // START: 0 = no start
                | (0 << 27);    // EDGE: not applicable
}

void HAL_adc_enable_channel(int pin) {
  LPC_PINCON->PINSEL1 &= ~0x000FC000;
  LPC_PINCON->PINSEL1 |= 0x00054000;
  //LPC_PINCON->PINSEL3 |= 0xC0000000;
}

void HAL_adc_start_conversion(uint8_t adc_pin) {
  if(adc_pin_map[adc_pin].port == 0xFF) {
    MYSERIAL.serial_port.printf("HAL: HAL_adc_start_conversion: no pinmap for %d\r\n",adc_pin);
    return;
  }
  LPC_ADC->ADCR &= ~0xFF;                                // Reset
  LPC_ADC->ADCR |= ( 0x01 << adc_pin_map[adc_pin].adc ); // Select Channel
  LPC_ADC->ADCR |= ( 0x01 << 24 );                       // start conversion
}

uint16_t HAL_adc_get_result(void) {
  uint32_t data = LPC_ADC->ADGDR;
  LPC_ADC->ADCR &= ~(1 << 24); //stop conversion
  if ( data & ADC_OVERRUN ) return 0;
  return ((data >> 6) & 0x3ff); //10bit
}

// ---------------------
// Userspace entry point
// ---------------------
extern void setup();
extern void loop();

void pinMode(int pin, int mode);
void digitalWrite(int pin, int pin_status);
bool digitalRead(int pin);

extern "C" void SysTick_Handler(void) {
  _millis++;
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
  LPC_PINCON->PINSEL4 = (1<<PWM_5) | (1<<PWM_6);

  LPC_PWM1->TCR = (1<<SBIT_CNTEN) | (1<<SBIT_PWMEN);
  LPC_PWM1->PR  =  0x0;               // No prescalar
  LPC_PWM1->MCR = (1<<SBIT_PWMMR0R);  // Reset on PWMMR0, reset TC if it matches MR0
  LPC_PWM1->MR0 = 255;                /* set PWM cycle(Ton+Toff)=255) */
  LPC_PWM1->MR5 = 0;                  /* Set 50% Duty Cycle for the channels */
  LPC_PWM1->MR6 = 0;

  // Trigger the latch Enable Bits to load the new Match Values MR0, MR5, MR6
  LPC_PWM1->LER = (1<<0) | (1<<5) | (1<<6);
  // Enable the PWM output pins for PWM_5-PWM_6(P2_4 - P2_5)
  LPC_PWM1->PCR = (1<<13) | (1<<14);
}

extern "C" int main() {
  SysTick_Config(SystemCoreClock / 1000);
  // Enable GPIO clock
  LPC_SC->PCONP |=  (1 << 15);

  HAL_timer_init();
  //HAL_pwm_init();

  system_counter.start(); //todo: rethink micros?
  setup();

  while (true) {
    loop();
  }
}

#endif // TARGET_LPC1768
