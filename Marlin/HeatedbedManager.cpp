///////////////////////////////////////////////////////////////////////////////
/// \file HeatedbedManager.cpp
///
/// \author Koldo Imanol de Miguel
///
/// \brief Management class for the state of the heated bed.
///
/// Copyright (c) 2016 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#include "HeatedbedManager.h"

#include "TemperatureManager.h" 
#include "StorageManager.h"
#include "Configuration.h"

HeatedbedManager::HeatedbedManager()
	: Subject<uint8_t>()
	, m_mode(0)
{
#if (HEATER_BED_PIN > -1) && (HBP_HEATER_AUX > -1)
		pinMode(HBP_HEATER_AUX, INPUT_PULLUP);
		m_bed_detected = !digitalRead(HBP_HEATER_AUX);
		
		if(m_bed_detected)
		{
			m_mode = eeprom::StorageManager::single::instance().getHeatedbedMode();
		}
		else
		{
			m_mode = eeprom::HEATEDBED_OFF;
		}
		notify();
#else
	m_bed_detected = false;
	m_mode = eeprom::HEATEDBED_OFF;
#endif
}

void HeatedbedManager::toggleMode()
{
	HeatedbedManager::single::instance().mode();
}

uint8_t HeatedbedManager::getMode()
{
	#if HBP_HEATER_AUX > -1
		if(detected())
		{
			return m_mode;
		}
	#endif
	
	return eeprom::HEATEDBED_OFF;
}

bool HeatedbedManager::detected()
{
	return m_bed_detected;
}

void HeatedbedManager::disableHeatedbed()
{
	m_bed_detected = false;
	m_mode = eeprom::HEATEDBED_OFF;
	temp::TemperatureManager::single::instance().configureSetup();
	notify();
}

void HeatedbedManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_mode);
	}
}

void HeatedbedManager::mode(uint8_t mode)
{
	if(m_bed_detected)
	{
		m_mode = mode;
		eeprom::StorageManager::single::instance().setHeatedbedMode(m_mode);
		notify();
	}
}

uint8_t HeatedbedManager::mode()
{
	if(m_bed_detected)
	{
		uint8_t const num_modes = 3;
		
		mode((m_mode+1) % num_modes);
	}
	return m_mode;
}
