/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef TARGET_LPC1768

#include "HAL.h"

// Interrupts
void cli(void) { __disable_irq(); } // Disable
void sei(void) { __enable_irq(); }  // Enable

// Program Memory
void serialprintPGM(const char * str){
	usb_serial.print(str);
}

// Time functions
void _delay_ms(int delay_ms) {
	delay (delay_ms);
}

uint32_t _millis = 0;
uint32_t millis() {
	return _millis;
}

uint64_t micros() {
  return system_counter.read_us();
}

void delayMicroseconds(uint32_t us) {
  wait_us(us);
}

void delay(int milis) {
	wait_ms(milis);
}

// IO functions
// As defined by Arduino INPUT(0x0), OUPUT(0x1), INPUT_PULLUP(0x2)
void pinMode(int pin, int mode) {
  if(pin_map[pin].port == 0xFF) {
    MYSERIAL.serial_port.printf("HAL: pinMode: no pinmap for %d\r\n", pin);
    return;
  }

  switch(mode) {
  case INPUT:
    LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
    pin_mode(LPC_MBED_PIN(pin_map[pin].port, pin_map[pin].pin), PinMode::PullNone);
    break;
  case OUTPUT:
    LPC_GPIO(pin_map[pin].port)->FIODIR |=  LPC_PIN(pin_map[pin].pin);
    pin_mode(LPC_MBED_PIN(pin_map[pin].port, pin_map[pin].pin), PinMode::PullNone);
    break;
  case INPUT_PULLUP:
    LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
    pin_mode(LPC_MBED_PIN(pin_map[pin].port, pin_map[pin].pin), PinMode::PullUp);
    break;
  default:
    MYSERIAL.serial_port.printf("HAL: pinMode: Invalid Mode(%d)\r\n", mode);
  }
}

void digitalWrite(int pin, int pin_status) {
  if(pin_map[pin].port == 0xFF) {
    MYSERIAL.serial_port.printf("HAL: digitalWrite: no pinmap for %d\r\n", pin);
    return;
  }
  if(pin_status) {
    LPC_GPIO(pin_map[pin].port)->FIOSET = LPC_PIN(pin_map[pin].pin);
  } else {
    LPC_GPIO(pin_map[pin].port)->FIOCLR = LPC_PIN(pin_map[pin].pin);
  }
}

bool digitalRead(int pin) {
  if(pin_map[pin].port == 0xFF) {
    MYSERIAL.serial_port.printf("HAL: digitalRead: no pinmap for %d\r\n", pin);
    return false;
  }
  return LPC_GPIO(pin_map[pin].port)->FIOPIN & LPC_PIN(pin_map[pin].pin) ? 1 : 0;
}

void analogWrite(int pin, int pin_status) { //todo: Hardware PWM
/*
  if(pin == P2_4) {
    LPC_PWM1->MR5 = pin_status; //set value
    LPC_PWM1->LER = (1<<5); //set latch
  } else if(pin == P2_5) {
    LPC_PWM1->MR6 = pin_status;
    LPC_PWM1->LER = (1<<6);
  }*/
}

uint16_t analogRead(int adc_pin) {
    HAL_adc_start_conversion (adc_pin);
	return HAL_adc_get_result();
}

// **************************
// Persistent Config Storage
// **************************

void eeprom_write_byte(unsigned char *pos, char value) {

}

unsigned char eeprom_read_byte(uint8_t * pos) {
    return '\0';
}

void eeprom_read_block (void *__dst, const void *__src, size_t __n) {

}

void eeprom_update_block (const void *__src, void *__dst, size_t __n) {

}

#endif //TARGET_LPC1768
