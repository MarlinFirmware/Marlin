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

#include "StorageManager.h"
//#include "Marlin.h"

StatsManager::StatsManager()
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
	bool carry = 0;
	
	m_minutes += printTime.minutes;
	if(m_minutes >= 60)
	{
		m_minutes -= 60;
		carry = 1;
	}
	
	m_hours = m_hours + printTime.hours + carry;
	
	//update time in memory
	eeprom::StorageManager::single::instance().setStatHours(m_hours);
	eeprom::StorageManager::single::instance().setStatMinutes(m_minutes);
}

void StatsManager::increaseTotalPrints()
{ 
	++m_total_prints;
	
	//update total prints in memory
	eeprom::StorageManager::single::instance().setStatTotalPrints(m_total_prints);
}

void StatsManager::increaseSuccededPrints()
{ 
	++m_succeded;
	
	//update succeded prints in memory
	eeprom::StorageManager::single::instance().setStatSucceded(m_succeded);
}
