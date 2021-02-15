#pragma once

#include <Arduino.h>
#include "TMC_platforms.h"

class SW_SPIClass {
	public:
		SW_SPIClass(uint16_t sw_mosi_pin, uint16_t sw_miso_pin, uint16_t sw_sck_pin);
		void init();
		void begin() {};
		uint8_t transfer(uint8_t ulVal);
		uint16_t transfer16(uint16_t data);
		void endTransaction() {};
	private:
		const uint16_t	mosi_pin,
						miso_pin,
						sck_pin;

		#if defined(ARDUINO_ARCH_AVR)
			fastio_bm mosi_bm,
					miso_bm,
					sck_bm;
			fastio_reg mosi_register,
							 miso_register,
							 sck_register;
		#endif
};
