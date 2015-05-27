#ifndef SCREEN_STATUS_H
#define SCREEN_STATUS_H


#include <stdint.h>
#include "Screen.h"
#include "Observer.h"

namespace screen
{
	template <typename T>
		class ScreenStatus : public Screen, public Observer<T>
	{
		public:
			ScreenStatus(const char * title = 0, FuncPtr_t function = do_nothing, Subject<T> * model = 0);
			virtual ~ScreenStatus();

			void icon(Icon & component);
			Icon & icon();

			void update(T value);

		private:
			Icon * m_icon_alternate;
			uint8_t m_icon_index;
	};

template <typename T>
	ScreenStatus<T>::ScreenStatus(const char * title, FuncPtr_t function, Subject<T> * model)
            : Screen(title, STATUS, function)
            , Observer<T>(model)
            , m_icon_index(0)
         {};

template <typename T>
	ScreenStatus<T>::~ScreenStatus()
	{ }

template <typename T>
   void ScreenStatus<T>::icon(Icon & component)
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

template <typename T>
   Icon & ScreenStatus<T>::icon()
   {
      return * m_icon;
   }

template <typename T>
        void ScreenStatus<T>::update(T value) 
{ }
}

#endif //SCREEN_STATUS_H
