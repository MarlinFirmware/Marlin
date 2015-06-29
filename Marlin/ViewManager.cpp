#include "ViewManager.h"

namespace screen
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

	ViewManager::ViewManager()
		: m_active_view(NULL)
		, m_last_focus(0)
	{ }

	ViewManager::~ViewManager()
	{ }
}
