#ifndef _HAL_ESP_H
#define _HAL_ESP_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#undef DISABLED
#include <Arduino.h>
#undef DISABLED
#define DISABLED(b) (!_CAT(SWITCH_ENABLED_, b))

#include "fastio_ESP.h"
#include "watchdog_ESP.h"

#include "HAL_timers_ESP.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

void digitalWrite(uint8_t pin, uint8_t val);

// Workaround issue: https://github.com/esp8266/Arduino/issues/2078
//
// #ifdef ESP32
// #undef PROGMEM
// #define PROGMEM
// #undef PSTR
// #define PSTR(s) (s)
// #undef pgm_read_byte
// #define pgm_read_byte(addr) (*reinterpret_cast<const uint8_t*>(addr))
// #undef pgm_read_word
// #define pgm_read_word(addr) (*reinterpret_cast<const uint16_t*>(addr))
// #endif
#define MYSERIAL Serial

//
// #define _BV(bit) 	(1 << (bit))
//
// #ifndef analogInputToDigitalPin
//   #define analogInputToDigitalPin(p) ((p < 12u) ? (p) + 54u : -1)
// #endif
//
// #define CRITICAL_SECTION_START	uint32_t volatile register ilevel = XTOS_DISABLE_ALL_INTERRUPTS;
// #define CRITICAL_SECTION_END    XTOS_RESTORE_INTLEVEL(ilevel);
#define CRITICAL_SECTION_START NOOP //cli()  // TODO: check if i2c gpioex is used
#define CRITICAL_SECTION_END NOOP //sei()    // TODO: check if i2c gpioex is used 

//
// // On AVR this is in math.h?
// #define square(x) ((x)*(x))
//
// #ifndef strncpy_P
//   #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
// #endif
//
// // Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))
//
// #define RST_POWER_ON   1
// #define RST_EXTERNAL   2
// #define RST_BROWN_OUT  4
// #define RST_WATCHDOG   8
// #define RST_JTAG       16
// #define RST_SOFTWARE   32
// #define RST_BACKUP     64

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

/** result of last ADC conversion */
extern int HAL_adc_result;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// Disable interrupts
// void cli();

// Enable interrupts
// void sei();
//
// /** clear reset reason */
void HAL_clear_reset_source (void);
//
// /** reset reason */
uint8_t HAL_get_reset_source (void);
//
void _delay_ms(int delay);
//
int freeMemory(void);
//
// // SPI: Extended functions which take a channel number (hardware SPI only)
// /** Write single byte to specified SPI channel */
// void spiSend(uint32_t chan, byte b);
// /** Write buffer to specified SPI channel */
// void spiSend(uint32_t chan, const uint8_t* buf, size_t n);
// /** Read single byte from specified SPI channel */
// uint8_t spiRec(uint32_t chan);
//
//
// // EEPROM
void eeprom_write_byte(unsigned char *pos, unsigned char value);
unsigned char eeprom_read_byte(unsigned char *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);
//
//
// // ADC
//
#define HAL_ANALOG_SELECT(pin)
//
inline void HAL_adc_init(void) {}
//
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC        HAL_adc_result
//
//
void HAL_adc_start_conversion (uint8_t adc_pin);
//
// uint16_t HAL_adc_get_result(void);
//
// //
// uint16_t HAL_getAdcReading(uint8_t chan);
//
// void HAL_startAdcConversion(uint8_t chan);
// uint8_t HAL_pinToAdcChannel(int pin);
//
// uint16_t HAL_getAdcFreerun(uint8_t chan, bool wait_for_conversion = false);
// //uint16_t HAL_getAdcSuperSample(uint8_t chan);
//
// void HAL_enable_AdcFreerun(void);
// //void HAL_disable_AdcFreerun(uint8_t chan);
//
//
// // --------------------------------------------------------------------------
// //
// // --------------------------------------------------------------------------

#endif
