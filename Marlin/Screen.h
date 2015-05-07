#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <new.h>
#include "Marlin.h"
#include "Icon.h"
#include "GuiPainter.h"

namespace screen
{
	const static uint8_t max_items = 10;
	const static uint8_t max_list = 100;
	const static uint8_t max_icons = 4;

	typedef enum
	{
		MENU = 0,
		DIALOG,
		LIST,
		SELECTOR,
		NUM_SCREEN_TYPES,
	} ScreenType_t;

	class Screen
	{
		public:
			Screen(ScreenType_t const & type, const char * title);
			virtual ~Screen();

			const char * title() const;
			ScreenType_t const & type() const;
			void icon(Icon & component);
			Icon & icon(Icon::IconType_t const & type);

			virtual void left() {};
			virtual void right() {};
			virtual Screen & press() {};
			virtual void draw() {};
			virtual void add(Screen & component) {};

		protected:
			ScreenType_t m_type;
			uint16_t m_num_icons;
			const char * m_title;
			Icon * m_icons[max_icons];
	};
}
#endif //SCREEN_H
