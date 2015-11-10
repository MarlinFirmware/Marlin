///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSwitch.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of switch-type screens.
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

#include "ScreenSwitch.h"

#include "GuiManager.h"

namespace ui
{
	ScreenSwitch::ScreenSwitch(const char * title, Functor<bool>::FuncPtr fptr)
		: ScreenAction<bool>(title, fptr)
		, m_num_items(0)
	{
		m_type = SWITCH;
	}

	ScreenSwitch::~ScreenSwitch()
	{ }

	void ScreenSwitch::init(uint16_t index)
	{
		if( this->action() == true )
		{
			ViewManager::getInstance().activeView(m_items[0]);
		}
		else
		{
			ViewManager::getInstance().activeView(m_items[1]);
		}
		
	}

	void ScreenSwitch::add(ScreenIndex_t const & component)
	{
		if (m_num_items < m_max_items)
		{
			m_items[m_num_items] = component;
			++m_num_items;
		}
	}
}