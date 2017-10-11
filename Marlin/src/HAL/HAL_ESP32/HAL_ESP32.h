#ifndef _HAL_ESP32_H
#define _HAL_ESP32_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#undef DISABLED
#include <Arduino.h>
#undef DISABLED
#define DISABLED(b) (!_CAT(SWITCH_ENABLED_, b))

#include "fastio_ESP32.h"
#include "watchdog_ESP32.h"

#include "HAL_timers_ESP32.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

// void digitalWrite(uint8_t pin, uint8_t val);

#define MYSERIAL Serial

#define CRITICAL_SECTION_START cli()
#define CRITICAL_SECTION_END sei()

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

/** result of last ADC conversion */
extern uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

/** clear reset reason */
void HAL_clear_reset_source (void);

/** reset reason */
uint8_t HAL_get_reset_source (void);

void _delay_ms(int delay);

int freeMemory(void);

// EEPROM
void eeprom_write_byte(unsigned char *pos, unsigned char value);
unsigned char eeprom_read_byte(unsigned char *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

// ADC
#define HAL_ANALOG_SELECT(pin)

void HAL_adc_init(void);

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC        HAL_adc_result

void HAL_adc_start_conversion (uint8_t adc_pin);

#endif
