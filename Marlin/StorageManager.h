///////////////////////////////////////////////////////////////////////////////
/// \file StorageManager.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the EEPROM memory.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"

namespace eeprom
{
	const uint8_t EMERGENCY_STOP_ACTIVE = 0x00;
	const uint8_t EMERGENCY_STOP_INACTIVE = 0xFF;
	const uint8_t INITIALIZED = 0x00;
	const uint8_t UNINITIALIZED = 0xFF;
	const uint8_t LIGHT_OFF = 0x00;
	const uint8_t LIGHT_ON = 0x01;
	const uint8_t LIGHT_AUTO = 0x02;
	const uint8_t BOX_FAN_ENABLED = 0x01;
	const uint8_t BOX_FAN_DISABLED = 0x00;
	const uint8_t AUTOLEVEL_ON = 0x01;
	const uint8_t SERIAL_SCREEN_ON = 0x01;
	const uint8_t EEPROM_ENABLED = 0x00;
	const uint8_t EEPROM_DISABLED = 0x01;
	const uint8_t HEATEDBED_OFF = 0x00;
	const uint8_t HEATEDBED_ON = 0x01;
	const uint8_t HEATEDBED_AUTO = 0x02;

	class StorageManager
	{
		public:
			typedef Singleton<StorageManager> single;

		public:
			StorageManager();

			static void setEmergency();
			static void clearEmergency();
			static const uint8_t getEmergency();

			static void setOffset(float value);
			static float getOffset();

			static void setInitialized();
			static void setUninitialized();
			static bool getInitialized();

			static void setLanguage(uint8_t language);
			static const uint8_t getLanguage();

			static void setLight(uint8_t state);
			static uint8_t getLight();

			static void setBoxFan(bool state);
			static bool getBoxFan();

			static void setAutoLevel(bool state);
			static bool getAutoLevel();

			static void setSerialScreen(bool state);
			static bool getSerialScreen();

			static void eraseEEPROM();
			static const uint8_t getEEPROMVersion();
			static const uint8_t checkEEPROMState();
			
			static const uint8_t getBoardType();
			static const char * getSerialNumber();
			
			static bool checkStatsInitialized();
			static void InitilializeStats();
			
			static const uint16_t getStatHours();
			static const uint8_t getStatMinutes();
			static const uint16_t getStatTotalPrints();
			static const uint16_t getStatSucceded();
			
			static void setStatHours(uint16_t hours);
			static void setStatMinutes(uint8_t minutes);
			static void setStatTotalPrints(uint16_t total_prints);
			static void setStatSucceded(uint16_t succedded_prints);
			
			static const uint8_t getHeatedbedMode();
			static void setHeatedbedMode(uint8_t heatedbed_mode);

		private:
			void updateEEPROMVersion();
			void setEEPROMState(uint8_t state);

			uint8_t readByte(uint8_t * address);
			void writeByte(uint8_t * address, uint8_t data);

			void readData(uint8_t * address, uint8_t * value, uint8_t size);
			void writeData(uint8_t * address, uint8_t * value, uint8_t size);
	};
}

#endif // STORAGE_MANAGER_H
