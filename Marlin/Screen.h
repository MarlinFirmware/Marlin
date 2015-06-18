#ifndef SCREEN_H
#define SCREEN_H

#include <new.h>
#include <stdint.h>

#include "Icon.h"
#include "GuiPainter.h"
#include "ViewManager.h"

namespace screen
{
	class Screen
	{
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
			Screen(const char * title = 0, ScreenType_t const & type = SIMPLE);
			virtual ~Screen();

			const char * title() const;
			ScreenType_t const & type() const;

			virtual void left() {};
			virtual void right() {};
			virtual void draw() {};
			virtual void init() {};

			virtual void icon(Icon & component);
			virtual Icon & icon();
			virtual void press();
			virtual void add(Screen & component) {};

		protected:
			ScreenType_t m_type;
			const char * m_title;
			Icon * m_icon;
	};
}
#endif //SCREEN_H
