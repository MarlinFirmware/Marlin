#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"

namespace eeprom
{
	const uint8_t EMERGENCY_STOP_ACTIVE = 0x00;
	const uint8_t EMERGENCY_STOP_INACTIVE = 0xFF;
	const uint8_t INITIALIZED = 0x00;

	class StorageManager
	{
		public:
			typedef Singleton<StorageManager> single;

		public:
			StorageManager();

			static void setEmergency();
			static void clearEmergency();
			static uint8_t getEmergency();

			static void setOffset(float value);
			static float getOffset();
			
			static void setInitialized();
			static bool getInitialized();

			static void setLanguage(uint8_t language);
			static uint8_t getLanguage();

		private:
			uint8_t readByte(uint8_t* address);
			void writeByte(uint8_t* address, uint8_t data);
			float readFloat(float* address);
			void writeFloat(float* address, float data);
	};
}

#endif // STORAGE_MANAGER_H