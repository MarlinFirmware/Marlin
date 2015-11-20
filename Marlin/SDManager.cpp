///////////////////////////////////////////////////////////////////////////////
/// \file SDManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the SD card reader.
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

#include "SDManager.h"

#include "cardreader.h"
#include "PrintManager.h"

SDManager::SDManager()
	: Subject<bool>()
	, m_is_inserted(false)
{ }

void SDManager::updateSDStatus()
{
	if (IS_SD_INSERTED)
	{
		SDManager::single::instance().setInserted(true);
	}
	else
	{
		SDManager::single::instance().setInserted(false);
	}
}

bool const & SDManager::isInserted() const
{
	return m_is_inserted;
}

void SDManager::setInserted(bool state)
{
	if (state != m_is_inserted)
	{
		m_is_inserted = state;
		if (m_is_inserted)
		{
			card.initsd();
		}
		notify();
			
		PrintManager::single::instance().resetInactivity();
	}
}

void SDManager::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_is_inserted);
	}
}
