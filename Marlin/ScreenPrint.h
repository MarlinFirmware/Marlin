#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "Screen.h"
#include "Functor.h"
#include "cardreader.h"

namespace screen
{
   template <typename R, typename... Args>
		class ScreenPrint : public Screen, public Functor<R, Args...>
	{
		public:
			ScreenPrint(const char * title = 0, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenPrint();

			void left();
			void right();
			void draw();
			Screen & press(Screen * parent_view);
			void add(Screen & component);

		private:
			uint16_t m_index;
			//uint16_t m_i; //Temporal
			uint16_t m_num_items;
			Screen * m_items[max_items];
	};

	template <typename R, typename... Args>
	ScreenPrint<R, Args...>::ScreenPrint(const char * title, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, PRINT)
      , Functor<R, Args...>(fptr)
		//, m_i(0) //Temporal
		, m_index(0)
		, m_num_items(0)
	{ }

   template <typename R, typename... Args>
	ScreenPrint<R, Args...>::~ScreenPrint()
	{ }

   template <typename R, typename... Args>
	void ScreenPrint<R, Args...>::left()
	{
		//if (m_i == 0) //Temporal
		//{
		//	m_i = 0;
		//}
		//else
		//{
		//	--m_i;
		//}

		if (m_index == 0)
		{
			m_index = 0;
		}
		else
		{
			--m_index;
		}
	}

   template <typename R, typename... Args>
	void ScreenPrint<R, Args...>::right()
	{
		//if ( m_i == 100 )
		//{
		//	m_i = 100;
		//}
		//else
		//{
		//	++m_i;
		//}

		if ( m_index == (m_num_items -1) )
		{
			m_index = m_num_items -1;
		}
		else
		{
			++m_index;
		}
	}

   template <typename R, typename... Args>
	void ScreenPrint<R, Args...>::draw()
	{
		/*SERIAL_ECHO(m_title);
		SERIAL_ECHO(">>>");
		SERIAL_ECHO(" [");
		SERIAL_ECHO(m_items[m_index]->title());
		SERIAL_ECHOLN("] "); */	
		painter.firstPage();
		do 
		{
			painter.title("Printing screen");
			uint16_t actual_time = 0;
			if (starttime != 0)
			{
				actual_time = millis()/60000 - starttime/60000;
			}
			painter.printing_status(card.percentDone(), actual_time);
         painter.box((m_items[m_index]->icon()).text());
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_end = painter.coordinateYEnd();
			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				int col = i % 5;
				int row = i / 5;
				int row_t = (m_num_items-1) / 5;

				int x = (x_end + x_init)/2 - (m_num_items*(icon_width+2)/(1+row_t)-2)/2 +col*(icon_width+2);
				int y = (y_end+y_init)/(2*(1+row_t)) + row_t - (icon_height/2) + ((icon_height+3)*row);

				if (i == m_index)
				{
					(m_items[i]->icon()).draw(x,y, true);
				}
				else
				{
					(m_items[i]->icon()).draw(x,y);
				}
			}	
		} while( painter.nextPage() ); 
	}

   template <typename R, typename... Args>
	Screen & ScreenPrint<R, Args...>::press(Screen * parent_view)
	{
		m_items[m_index]->action();
		if (m_items[m_index]->type() == Screen::STATUS)
		{
			return * parent_view;
		}
		else if (m_items[m_index]->type() == Screen::SIMPLE)
		{
			return m_items[m_index]->press(parent_view);
		}
		return * m_items[m_index];
	}

   template <typename R, typename... Args>
	void ScreenPrint<R, Args...>::add(Screen & component)
	{
		if (m_num_items < max_items)
		{
			m_items[m_num_items] = &component;
			++m_num_items;
		}
	}

}

#endif //SCREEN_PRINT_H
