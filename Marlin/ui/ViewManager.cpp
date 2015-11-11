///////////////////////////////////////////////////////////////////////////////
/// \file ViewManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief A manager class to keep track of active view.
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

#include "ViewManager.h"

namespace ui
{
	ViewManager & ViewManager::getInstance()
	{
		static ViewManager instance;
		return instance;
	}

	void ViewManager::activeView(ScreenIndex_t const & index)
	{
		// Check if last view requires to keep element focus
		bool keep_focus = false;
		if (m_active_view->type() == Screen::ACTION)
		{
			keep_focus = true;
		}

		// Delete last view
		delete m_active_view;
		m_index = index;

		// Build new active view
		m_active_view = GuiBuild(index);

		// Keep focus if required
		if ( (keep_focus) && (m_active_view->type() == Screen::MENU) )
		{
			m_active_view->init(m_last_focus);
		}
		else
		{
			m_active_view->init();
		}
	}

	Screen * ViewManager::activeView()
	{
		return m_active_view;
	}

	void ViewManager::setLastFocus(uint16_t last_focus)
	{
		m_last_focus = last_focus;
	}

	ScreenIndex_t const & ViewManager::getViewIndex() const
	{
		return m_index;
	}

	void ViewManager::displayRefresh()
	{
		m_active_view->m_needs_drawing = true;
	}

	ViewManager::ViewManager()
		: m_active_view(NULL)
		  , m_last_focus(0)
	{ }

	ViewManager::~ViewManager()
	{ }
}
