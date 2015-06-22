#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include "Screen.h"
#include "Functor.h"

namespace screen
{
	class ScreenTransition : public Screen , public Functor<void>
	{
		public:
			ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr = do_nothing);
			virtual ~ScreenTransition();

			void init();

			void draw();

		private:
			const char * m_message;
			const char * m_box;
	};
}

#endif //SCREEN_TRANSITION_H
