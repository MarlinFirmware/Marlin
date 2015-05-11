#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <new.h>
#include "Marlin.h"
#include "Icon.h"
#include "GuiImpl.h"

namespace screen
{
	typedef enum
	{
		SIMPLE = 0, 
		MENU,
		DIALOG,
		LIST,
		SELECTOR,
		NUM_SCREEN_TYPES,
	} ScreenType_t;

	class Screen
	{
		public:
			Screen(const char * title, ScreenType_t const & type = SIMPLE);
			virtual ~Screen();

			const char * title() const;
			ScreenType_t const & type() const;
			void icon(Icon & component);
			Icon & icon(Icon::IconType_t const & type);

			virtual void left() {};
			virtual void right() {};
			virtual Screen & press(Screen & parent_view);
			virtual void draw() {};
			virtual void add(Screen & component) {};

		protected:
			virtual void drawTitle();

		protected:
			ScreenType_t m_type;
			uint16_t m_num_icons;
			const char * m_title;
			Icon * m_icons[max_icons];
	};
}
#endif //SCREEN_H
