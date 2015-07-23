#include "StorageManager.h"

#include <avr/eeprom.h>

namespace eeprom
{
	static uint8_t * const EMERGENCY_STOP_FLAG = (uint8_t *) 499;
	static uint8_t * const SCREEN_SERIAL_FLAG = (uint8_t *) 509;

	StorageManager::StorageManager()
	{ }

	void StorageManager::setEmergencyFlag()
	{
		StorageManager::single::instance().writeByte(EMERGENCY_STOP_FLAG, EMERGENCY_STOP_ACTIVE);
	}

	void StorageManager::clearEmergencyFlag()
	{
		StorageManager::single::instance().writeByte(EMERGENCY_STOP_FLAG, EMERGENCY_STOP_INACTIVE);
	}

	uint8_t StorageManager::getEmergencyFlag()
	{
		return StorageManager::single::instance().readByte(EMERGENCY_STOP_FLAG);
	}

	void StorageManager::enableScreenSerial()
	{
		StorageManager::single::instance().writeByte(SCREEN_SERIAL_FLAG, SCREEN_SERIAL_ACTIVE);
	}

	void StorageManager::disableScreenSerial()
	{
		StorageManager::single::instance().writeByte(SCREEN_SERIAL_FLAG, SCREEN_SERIAL_INACTIVE);
	}

	uint8_t StorageManager::getScreenSerialState()
	{
		return StorageManager::single::instance().readByte(SCREEN_SERIAL_FLAG);
	}

	uint8_t StorageManager::readByte(uint8_t* address)
	{
		while ( !eeprom_is_ready() ) {}
		return eeprom_read_byte(address);
	}

	void StorageManager::writeByte(uint8_t* address, uint8_t data)
	{
		while ( !eeprom_is_ready() ) {}
		eeprom_write_byte(address, data);
	}
}