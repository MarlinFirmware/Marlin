#include "OffsetManager.h"

static const int ZOFFSET_ZPROBE_EEPROM_POS = 232;

OffsetManager::OffsetManager()
	: Subject<float>()
{
	m_offset = ReadFromEEPROM();
}

void OffsetManager::saveOffset()
{
	WriteToEEPROM(m_offset);
	notify();
}

void OffsetManager::offset(float value)
{
	m_offset = value;
	notify();
}

float OffsetManager::offset()
{
	m_offset = ReadFromEEPROM();
	return m_offset;
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
