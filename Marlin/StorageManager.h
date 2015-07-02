#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"

class StorageManager
{
	public:
		typedef Singleton<StorageManager> single;

	public:
		StorageManager();

		static void setEmergencyFlag();
		static void clearEmergencyFlag();
		static uint8_t getEmergencyFlag();

	private:
		uint8_t readByte(uint8_t* address);
		void writeByte(uint8_t* address, uint8_t data);
};

#endif // STORAGE_MANAGER_H