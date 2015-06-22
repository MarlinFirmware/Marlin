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

	ViewManager::ViewManager()
		: m_active_view(NULL)
	{ }

	ViewManager::~ViewManager()
	{ }
}
