#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"

namespace eeprom
{
	const uint8_t EMERGENCY_STOP_ACTIVE = 0x00;
	const uint8_t EMERGENCY_STOP_INACTIVE = 0xFF;
	const uint8_t SCREEN_SERIAL_ACTIVE = 0xFF;
	const uint8_t SCREEN_SERIAL_INACTIVE = 0x00;

	class StorageManager
	{
		public:
			typedef Singleton<StorageManager> single;

		public:
			StorageManager();

			static void setEmergencyFlag();
			static void clearEmergencyFlag();
			static uint8_t getEmergencyFlag();

			static void enableScreenSerial();
			static void disableScreenSerial();
			static uint8_t getScreenSerialState();

		private:
			uint8_t readByte(uint8_t* address);
			void writeByte(uint8_t* address, uint8_t data);
	};
}

#endif // STORAGE_MANAGER_H