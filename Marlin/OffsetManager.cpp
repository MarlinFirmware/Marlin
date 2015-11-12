#include "OffsetManager.h"

#include "StorageManager.h"
#include "Marlin.h"

OffsetManager::OffsetManager()
	: Subject<float>()
	, m_offset(Z_PROBE_OFFSET_FROM_EXTRUDER)
{
#ifdef DOGLCD
	m_offset = eeprom::StorageManager::single::instance().getOffset();
#else
	m_offset = zprobe_zoffset;
#endif
}

void OffsetManager::saveOffset()
{
	eeprom::StorageManager::single::instance().setOffset(m_offset);
}

float OffsetManager::getOffset()
{
	m_offset = eeprom::StorageManager::single::instance().getOffset();
	return m_offset;
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
