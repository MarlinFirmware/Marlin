///////////////////////////////////////////////////////////////////////////////
/// \file StatsManager.h
///
/// \author Koldo Imanol de Miguel
///
/// \brief Management class for management of printing statistics
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
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

#include "StatsManager.h"

#include "Configuration.h"
#include "StorageManager.h"

StatsManager::StatsManager()
	: m_ptfe_maintenance_flag(false)
	, m_hours(0)
	, m_minutes(0)
	, m_total_prints(0)
	, m_succeded(0)
{ }

void StatsManager::loadStats()
{ 
	m_hours = eeprom::StorageManager::single::instance().getStatHours();
	m_minutes = eeprom::StorageManager::single::instance().getStatMinutes();
	m_total_prints = eeprom::StorageManager::single::instance().getStatTotalPrints();
	m_succeded = eeprom::StorageManager::single::instance().getStatSucceded();
}

void StatsManager::updateTotalTime(Time_t printTime)
{ 
	uint16_t new_hours = m_hours + printTime.hours;

	m_minutes += printTime.minutes;	
	if(m_minutes >= 60)
	{
		m_minutes -= 60;
		++new_hours;
	}
	
	// check for time overflow
	if(new_hours < m_hours)
	{
		resetStats();
		
		//show only the data for the new print after overflow
		m_hours = printTime.hours;
		m_minutes = printTime.minutes;
		increaseTotalPrints();
	}
	else
	{
		// Check for PTFE tube maintenance
		if( (m_hours / PTFE_CHANGE_TIME) < (new_hours / PTFE_CHANGE_TIME) )
		{
			m_ptfe_maintenance_flag = true;
		}
		
		m_hours = new_hours;
	}
	
	//update time in memory
	eeprom::StorageManager::single::instance().setStatHours(m_hours);
	eeprom::StorageManager::single::instance().setStatMinutes(m_minutes);

}

void StatsManager::increaseTotalPrints()
{ 	
	++m_total_prints;
	
	if(m_total_prints == 0xFFFF)
	{
		resetStats();
	}
	else
	{
		//update total prints in memory
		eeprom::StorageManager::single::instance().setStatTotalPrints(m_total_prints);
	}
}

void StatsManager::increaseSuccededPrints()
{ 
	++m_succeded;
	
	if(m_succeded == 0xFFFF)
	{
		resetStats();
	}
	else
	{
		//update succeded prints in memory
		eeprom::StorageManager::single::instance().setStatSucceded(m_succeded);
	}
}

void StatsManager::resetStats()
{
	m_hours = 0;
	m_minutes = 0;
	m_total_prints = 0;
	m_succeded = 0;
	
	//update stats in memory
	eeprom::StorageManager::single::instance().setStatHours(m_hours);
	eeprom::StorageManager::single::instance().setStatMinutes(m_minutes);
	eeprom::StorageManager::single::instance().setStatSucceded(m_succeded);
	eeprom::StorageManager::single::instance().setStatTotalPrints(m_total_prints);
}
