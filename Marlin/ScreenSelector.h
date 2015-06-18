#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include <stdint.h>
#include "Screen.h"
#include "Functor.h"

#include "language.h"

namespace screen
{
	template <typename R, typename... Args>
		class ScreenSelector : public Screen , public Functor<R, Args...>
	{
		public:
			ScreenSelector(const char * title, uint16_t min, uint16_t max, uint16_t dflt, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenSelector();

			void init();

			void left();
			void right();
			void draw();
			void press();

		private:
         uint16_t m_select;
         uint16_t m_minimum_value;
         uint16_t m_maximum_value;
         uint16_t m_default;
	};

	template <typename R, typename... Args>
	ScreenSelector<R, Args...>::ScreenSelector(const char * title, uint16_t min, uint16_t max, uint16_t dflt, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, SELECTOR)
		, Functor<R, Args...>(fptr)
		, m_minimum_value(min)
		, m_maximum_value(max)
		, m_default(dflt)
		, m_select(dflt)
	{
	}

	template <typename R, typename... Args>
	ScreenSelector<R, Args...>::~ScreenSelector()
	{
	}

	template <typename R, typename... Args>
	void ScreenSelector<R, Args...>::init()
	{
		m_select = m_default;
	}

	template <typename R, typename... Args>
	void ScreenSelector<R, Args...>::left()
	{
		if (m_select == m_minimum_value)
		{
			m_select = m_minimum_value;
		}
		else
		{
			--m_select;
		}
	}

	template <typename R, typename... Args>
	void ScreenSelector<R, Args...>::right()
	{
		if (m_select == m_maximum_value)
		{
			m_select = m_maximum_value;
		}
		else
		{
			++m_select;
		}
	}

	template <typename R, typename... Args>
	void ScreenSelector<R, Args...>::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(MSG_SELECTOR_BOX); 

			uint8_t x_init = painter.coordinateXInit();
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t y_end = painter.coordinateYEnd();

			painter.setColorIndex(1);
			painter.setFont(u8g_font_6x9);

			char tmp_selected[4] = { 0 };
			snprintf(tmp_selected, 4, "%d", m_select);

			painter.setPrintPos((x_end + x_init)/2 - (strlen("<  >")*6)/2 - (strlen(tmp_selected)*6)/2, (y_end + y_init)/2 - 9/2);

			if (m_select != m_minimum_value)
			{
				painter.print("< ");
			}
			else
			{
				painter.print("  ");
			}

			painter.print(tmp_selected);

			if (m_select != m_maximum_value)
			{
				painter.print(" >");
			}
			else
			{
				painter.print("  ");
			}

		} while( painter.nextPage() );
	}

	template <typename R, typename... Args>
	void ScreenSelector<R, Args...>::press()
	{
		this->action(m_select);
		ViewManager::getInstance().activeView(m_next_screen);
	}
}

#endif //SCREEN_SELECTOR_H
