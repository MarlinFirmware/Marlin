#include "OffsetManager.h"
#include "StorageManager.h"
#include "Configuration.h"

OffsetManager::OffsetManager()
	: Subject<float>()
	, m_offset(-Z_PROBE_OFFSET_FROM_EXTRUDER)
{
#ifdef DOGLCD
	m_offset = eeprom::StorageManager::single::instance().getOffset();
#else
	m_offset = zprobe_zoffset;
#endif

	if(m_offset > 0.0f || m_offset < -10.0f)
	{
		SERIAL_ECHOLN("Prevented out of range offset!");
		m_offset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
	}
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
