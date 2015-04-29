#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <new.h>
#include "Marlin.h"
#include "Icon.h"

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
			Screen(const char * label, ScreenType_t const & type);
			virtual ~Screen();

			const char * label() const;
			ScreenType_t const & type() const;
			void icon(Icon & component);

			virtual void left() {};
			virtual void right() {};
			virtual Screen & press() {};
			virtual void draw() {};
			virtual void add(Screen & component) {};
			
		protected:
			const char * m_label;
			uint16_t m_num_icons;
			Icon * m_icons[max_icons];
			ScreenType_t m_type;
	};
}
#endif //SCREEN_H
