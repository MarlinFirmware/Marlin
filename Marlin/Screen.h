#ifndef SCREEN_H
#define SCREEN_H

#include <new.h>

#include <stdint.h>
#include "Marlin.h"
#include "Icon.h"
#include "GuiPainter.h"

static void do_nothing(){};

namespace screen
{
	class Screen
	{
		protected:
			typedef void (* FuncPtr_t)();

		public:
			typedef enum
			{
				SIMPLE = 0, 
				ACTION,
				STATUS,
				MENU,
				PRINT,
				DIALOG,
				LIST,
				SELECTOR,
				NUM_SCREEN_TYPES,
			} ScreenType_t;

		public:
			Screen(const char * title, ScreenType_t const & type = SIMPLE, FuncPtr_t function = do_nothing);
			virtual ~Screen();

			const char * title() const;
			ScreenType_t const & type() const;

			virtual void left() {};
			virtual void right() {};
			virtual void draw() {};
			virtual void icon(Icon & component);
			virtual Icon & icon();
			virtual Screen & press(Screen * parent_view);
			virtual void add(Screen & component);
			virtual void action();

		protected:
			ScreenType_t m_type;
			const char * m_title;
         Screen * m_next_screen;
			Icon * m_icon;
			FuncPtr_t m_function;
	};
}
#endif //SCREEN_H
