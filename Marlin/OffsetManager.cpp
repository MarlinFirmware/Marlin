#include "OffsetManager.h"

#include "StorageManager.h"
#include "Marlin.h"

OffsetManager::OffsetManager()
	: Subject<float>()
	, m_offset(0.0f)
{
	m_offset = eeprom::StorageManager::single::instance().getOffset();
}

void OffsetManager::saveOffset()
{
	eeprom::StorageManager::single::instance().setOffset(m_offset);
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

void OffsetManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_offset);
	}
}
