#include "OffsetManager.h"

static const int ZOFFSET_ZPROBE_EEPROM_POS = 232;
static const int FIRST_POWER_ON = 500;

OffsetManager::OffsetManager()
	: Subject<float>()
	, m_offset(0.0f)
{
	m_offset = ReadFromEEPROM();
}

void OffsetManager::saveOffset()
{
	WriteToEEPROM(m_offset);
}

void OffsetManager::offset(float value)
{
	m_offset = value;
	notify();
}

float OffsetManager::offset()
{
	return m_offset;
}

bool OffsetManager::isOffsetOnEEPROM()
{
	int i = FIRST_POWER_ON;
	unsigned char value;
	_EEPROM_readData(i, (uint8_t*)&value, sizeof(value));

	if (value == 0xFF)
	{
		return false;
	}
	return true;
}

void OffsetManager::offsetOnEEPROM()
{
	int i = FIRST_POWER_ON;
	unsigned char value = 0;
	_EEPROM_writeData(i, (uint8_t*)&value, sizeof(value));
}

float OffsetManager::ReadFromEEPROM()
{
	int i = ZOFFSET_ZPROBE_EEPROM_POS;
	float value = 0.0f;
	_EEPROM_readData(i, (uint8_t*)&value, sizeof(value));

	return value;
}

void OffsetManager::WriteToEEPROM(float value)
{
	int i = ZOFFSET_ZPROBE_EEPROM_POS;
	_EEPROM_writeData(i, (uint8_t*)&value, sizeof(value));
}

void OffsetManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_offset);
	}
}
