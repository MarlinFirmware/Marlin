#include "StorageManager.h"

#include <avr/eeprom.h>

static uint8_t * const EMERGENCY_STOP_FLAG = (uint8_t *) 499;

StorageManager::StorageManager()
{ }


void StorageManager::setEmergencyFlag()
{
	StorageManager::single::instance().writeByte((uint8_t*) EMERGENCY_STOP_FLAG, 0x01);
}

void StorageManager::clearEmergencyFlag()
{
	StorageManager::single::instance().writeByte((uint8_t*) EMERGENCY_STOP_FLAG, 0x00);
}

uint8_t StorageManager::getEmergencyFlag()
{
	return StorageManager::single::instance().readByte((uint8_t*) EMERGENCY_STOP_FLAG);
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
