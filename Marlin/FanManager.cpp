///////////////////////////////////////////////////////////////////////////////
/// \file FanManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the ambient controller fan.
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

#include "FanManager.h"

#include "StorageManager.h"

FanManager::FanManager()
	: Subject<bool>()
	, m_state(false)
{
	state(eeprom::StorageManager::single::instance().getBoxFan());
}

void FanManager::toogleState()
{
	//Switch current status
	FanManager::single::instance().state(!FanManager::single::instance().state());
}

void FanManager::state(bool state)
{
	eeprom::StorageManager::single::instance().setBoxFan(state);
	m_state = state;
	notify();
}

bool FanManager::state()
{
	return m_state;
}

void FanManager::notify()
{
	if(this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}