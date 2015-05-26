#include "ScreenSelector.h"

namespace screen
{
	ScreenSelector::ScreenSelector(const char * title, uint16_t min, uint16_t max, uint16_t dflt)
		: Screen(title, SELECTOR)
		, m_minimum_value(min)
		, m_maximum_value(max)
		, m_select(dflt)
	{
	}

	ScreenSelector::~ScreenSelector()
	{
	}

	void ScreenSelector::left()
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

	void ScreenSelector::right()
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

	void ScreenSelector::draw()
	{
		painter.firstPage();
		do 
		{
			painter.title(m_title);
			painter.box(m_next_screen->title()); //TEMPORAL

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

	Screen & ScreenSelector::press(Screen * parent_view)
	{
		return * m_next_screen;
	}

   void ScreenSelector::add(Screen & component)
   {
      m_next_screen = &component;
   }
}
