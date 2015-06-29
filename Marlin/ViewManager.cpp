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
		delete m_active_view;
		m_active_view = GuiBuild(index);
		m_active_view->init();
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
