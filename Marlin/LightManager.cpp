///////////////////////////////////////////////////////////////////////////////
/// \file LightManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the printer's light.
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

#include "LightManager.h"

#include "StorageManager.h"

#include <Arduino.h>
#include "Configuration.h"

LightManager::LightManager()
	: Subject<uint8_t>()
	, m_state(false)
	, m_mode(0)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	pinMode(LIGHT_PIN, OUTPUT);
#endif

	if(mode() == eeprom::LIGHT_OFF)
	{
		state(false);
	}
	else
	{
		state(true);
	}
	notify();
}

void LightManager::setMode()
{
	uint8_t mode = 0;
	uint8_t m_num_modes = 3;

	mode = LightManager::single::instance().mode();
	mode = (mode + 1) % m_num_modes;

	LightManager::single::instance().mode(mode);

	if(mode == eeprom::LIGHT_OFF)
	{
		LightManager::single::instance().state(false);
	}
	else
	{
		LightManager::single::instance().state(true);
	}

	LightManager::single::instance().notify();
}

uint8_t LightManager::getMode()
{
	return LightManager::single::instance().mode();
}

void LightManager::state(bool state)
{
#ifdef LIGHT_ENABLED && LIGHT_PIN
	digitalWrite(LIGHT_PIN, state);
#endif
	m_state = state;
}

bool LightManager::state()
{
	return m_state;
}

void LightManager::mode(uint8_t mode)
{
	eeprom::StorageManager::single::instance().setLight(mode);
	m_mode = mode;
}

uint8_t LightManager::mode()
{
	m_mode = eeprom::StorageManager::single::instance().getLight();
	return m_mode;
}

void LightManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_mode);
	}
}
