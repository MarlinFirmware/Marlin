#include "StorageManager.h"

#include <avr/eeprom.h>
#include "Configuration.h"
#include "Language.h"

namespace eeprom
{
	static uint8_t * const ADDR_ZOFFSET_ZPROBE = (uint8_t *) 232;
	static uint8_t * const ADDR_EMERGENCY_STOP = (uint8_t *) 499;
	static uint8_t * const ADDR_FIRST_POWER_ON = (uint8_t *) 500;
	static uint8_t * const ADDR_AUTOLEVEL      = (uint8_t *) 501;
	static uint8_t * const ADDR_LIGHT          = (uint8_t *) 502;
	static uint8_t * const ADDR_SERIAL         = (uint8_t *) 503;
	static uint8_t * const ADDR_LANGUAGE       = (uint8_t *) 504;

	StorageManager::StorageManager()
	{ }

	void StorageManager::setEmergency()
	{
		StorageManager::single::instance().writeByte(ADDR_EMERGENCY_STOP, EMERGENCY_STOP_ACTIVE);
	}

	void StorageManager::clearEmergency()
	{
		StorageManager::single::instance().writeByte(ADDR_EMERGENCY_STOP, EMERGENCY_STOP_INACTIVE);
	}

	const uint8_t StorageManager::getEmergency()
	{
		if(StorageManager::single::instance().readByte(ADDR_EMERGENCY_STOP) == EMERGENCY_STOP_ACTIVE)
		{
			return EMERGENCY_STOP_ACTIVE;
		}
		return EMERGENCY_STOP_INACTIVE;
	}

	void StorageManager::setOffset(float value)
	{
		float offset = value;
		StorageManager::single::instance().writeData(ADDR_ZOFFSET_ZPROBE, (uint8_t*)&offset, sizeof(offset));
	}

	float StorageManager::getOffset()
	{
		float offset = 0;
		StorageManager::single::instance().readData(ADDR_ZOFFSET_ZPROBE, (uint8_t*)&offset, sizeof(offset));

		if(offset < 0.0f || offset > 10.0f)
		{
			offset = Z_PROBE_OFFSET_FROM_EXTRUDER;
		}

		return offset;
	}

	void StorageManager::setInitialized()
	{
		StorageManager::single::instance().writeByte(ADDR_FIRST_POWER_ON, INITIALIZED);
	}

	bool StorageManager::getInitialized()
	{
		if(StorageManager::single::instance().readByte(ADDR_FIRST_POWER_ON) == INITIALIZED)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setLanguage(uint8_t language)
	{
		StorageManager::single::instance().writeByte(ADDR_LANGUAGE, language);
	}

	uint8_t StorageManager::getLanguage()
	{
		return StorageManager::single::instance().readByte(ADDR_LANGUAGE);
	}

	void StorageManager::setLight(bool state)
	{
		StorageManager::single::instance().writeByte(ADDR_LIGHT, state);
	}

	bool StorageManager::getLight()
	{
		if(StorageManager::single::instance().readByte(ADDR_LIGHT) == LIGHT_ON)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setAutoLevel(bool state)
	{
		StorageManager::single::instance().writeByte(ADDR_AUTOLEVEL, state);
	}

	bool StorageManager::getAutoLevel()
	{
		if(StorageManager::single::instance().readByte(ADDR_AUTOLEVEL) == AUTOLEVEL_ON)
		{
			return true;
		}
		return false;
	}

	void StorageManager::setSerialScreen(bool state)
	{
		StorageManager::single::instance().writeByte(ADDR_SERIAL, state);
	}

	bool StorageManager::getSerialScreen()
	{
		if(StorageManager::single::instance().readByte(ADDR_SERIAL) == SERIAL_SCREEN_ON)
		{
			return true;
		}
		return false;
	}

	void StorageManager::eraseEEPROM()
	{
		uint8_t * address = (uint8_t *) 0;
		uint8_t * max_address = (uint8_t *) 4080;

		while (address < max_address)
		{
			StorageManager::single::instance().writeByte(address, 0xFF);
			address++;
		}
	}

	uint8_t StorageManager::readByte(uint8_t * address)
	{
		while ( !eeprom_is_ready() ) {}
		return eeprom_read_byte(address);
	}

	void StorageManager::writeByte(uint8_t * address, uint8_t data)
	{
		while ( !eeprom_is_ready() ) {}
		eeprom_write_byte(address, data);
	}

	void StorageManager::readData(uint8_t * address, uint8_t * value, uint8_t size)
	{
		while(size > 0)
		{
			*value = readByte(address);
			address++;
			value++;
			size--;
		}
	}

	void StorageManager::writeData(uint8_t * address, uint8_t * value, uint8_t size)
	{
		while(size > 0)
		{
			writeByte(address, *value);
			address++;
			value++;
			size--;
		}
	}
}
