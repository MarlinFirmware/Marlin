#ifndef SCREEN_STATUS_H
#define SCREEN_STATUS_H

#include <stdint.h>

#include "Screen.h"
#include "Observer.h"
#include "Functor.h"

namespace screen
{
	template <typename T, typename R, typename... Args>
		class ScreenStatus : public Screen, public Observer<T>, public Functor<R, Args...>
	{
		public:
			ScreenStatus(const char * title = 0, typename Functor<R, Args...>::FuncPtr fptr = do_nothing, Subject<T> * model = 0);
			virtual ~ScreenStatus();

			void add(Screen & component);
			void init();
			void icon(Icon & component);
			Icon & icon();
			void update(T value);

		private:
			Screen * m_next_screen;
			Icon * m_icon_alternate;
			uint8_t m_icon_index;
			T m_status;
	};

	template <typename T, typename R, typename... Args>
	ScreenStatus<T, R, Args...>::ScreenStatus(const char * title,  typename Functor<R, Args...>::FuncPtr fptr, Subject<T> * model)
		: Screen(title, STATUS)
		, Functor<R, Args...>(fptr)
		, Observer<T>(model)
		, m_icon_index(0)
		, m_status()
	{ 
		this->connect();
	}

	template <typename T, typename R, typename... Args>
	ScreenStatus<T, R, Args...>::~ScreenStatus()
	{ }

	template <typename T, typename R, typename... Args>
	void ScreenStatus<T, R, Args...>::add(Screen & component)
	{
		m_next_screen = &component;
	}

	template <typename T, typename R, typename... Args>
	void ScreenStatus<T, R, Args...>::init()
	{
		this->action();
		ViewManager::getInstance().activeView(m_next_screen);
	}

	template <typename T, typename R, typename... Args>
	void ScreenStatus<T, R, Args...>::icon(Icon & component)
	{
		if (m_icon_index < 1)
		{
			m_icon = &component;
		}
		else
		{
			m_icon_alternate = &component;
		}
		++m_icon_index;
	}

	template <typename T, typename R, typename... Args>
	Icon & ScreenStatus<T, R, Args...>::icon()
	{
		if (m_status)
		{
			return * m_icon_alternate;
		}
		return * m_icon;
	}

	template <typename T, typename R, typename... Args>
	void ScreenStatus<T, R, Args...>::update(T value) 
	{ 
		m_status = value;
	}
}

#endif //SCREEN_STATUS_H
