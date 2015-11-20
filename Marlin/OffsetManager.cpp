///////////////////////////////////////////////////////////////////////////////
/// \file OffsetManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the value of the extruder's offset.
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
