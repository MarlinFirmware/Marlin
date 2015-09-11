#include "StorageManager.h"

#include <avr/eeprom.h>

extern void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size);
extern void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size);

namespace eeprom
{
	static uint8_t * const DIR_ZOFFSET_ZPROBE = (uint8_t *)232;

	static uint8_t * const DIR_EMERGENCY_STOP = (uint8_t *) 499;
	static uint8_t * const DIR_FIRST_POWER_ON = (uint8_t *) 500;
	static uint8_t * const DIR_AUTOLEVEL = (uint8_t *) 501;
	static uint8_t * const DIR_LIGHT = (uint8_t *) 505;
	static uint8_t * const DIR_SERIAL = (uint8_t *) 509;
	static uint8_t * const DIR_LANGUAGE = (uint8_t *) 510;

	StorageManager::StorageManager()
	{ }

	void StorageManager::setEmergency()
	{
		StorageManager::single::instance().writeByte(DIR_EMERGENCY_STOP, EMERGENCY_STOP_ACTIVE);
	}

	void StorageManager::clearEmergency()
	{
		StorageManager::single::instance().writeByte(DIR_EMERGENCY_STOP, EMERGENCY_STOP_INACTIVE);
	}

	uint8_t StorageManager::getEmergency()
	{
		return StorageManager::single::instance().readByte(DIR_EMERGENCY_STOP);
	}

	void StorageManager::setOffset(float value)
	{
		int dir = *DIR_ZOFFSET_ZPROBE;
		_EEPROM_writeData(dir, (uint8_t*)&value, sizeof(value));
	}

	float StorageManager::getOffset()
	{
		int dir = *DIR_ZOFFSET_ZPROBE;
		float dummy;
		_EEPROM_readData(dir, (uint8_t*)&dummy, sizeof(dummy));
		return dummy;
	}

	void StorageManager::setInitialized()
	{
		StorageManager::single::instance().writeByte(DIR_FIRST_POWER_ON, INITIALIZED);
	}

	bool StorageManager::getInitialized()
	{
		if(StorageManager::single::instance().readByte(DIR_FIRST_POWER_ON) == INITIALIZED)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setLanguage(uint8_t language)
	{
		StorageManager::single::instance().writeByte(DIR_LANGUAGE, language);
	}

	uint8_t StorageManager::getLanguage()
	{
		return StorageManager::single::instance().readByte(DIR_LANGUAGE);
	}

	void StorageManager::setLight(bool state)
	{
		StorageManager::single::instance().writeByte(DIR_LIGHT, state);
	}

	bool StorageManager::getLight()
	{
		if(StorageManager::single::instance().readByte(DIR_LIGHT) == LIGHT_ON)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setAutoLevel(bool state)
	{
		StorageManager::single::instance().writeByte(DIR_AUTOLEVEL, state);
	}

	bool StorageManager::getAutoLevel()
	{
		if(StorageManager::single::instance().readByte(DIR_AUTOLEVEL) == AUTOLEVEL_ON)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setSerialScreen(bool state)
	{
		StorageManager::single::instance().writeByte(DIR_SERIAL, state);
	}

	bool StorageManager::getSerialScreen()
	{
		if(StorageManager::single::instance().readByte(DIR_SERIAL) == SERIAL_SCREEN_ON)
		{
			return true;
		}
		return false;
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