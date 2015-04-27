#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

namespace screen
{
	const static uint8_t max_items = 10;

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
			~Screen();

			const char * label() const;
			ScreenType_t const & type() const;

			virtual void left() {};
			virtual void right() {};
			virtual Screen & press() {};
			virtual void draw() {};
			virtual void add(Screen & component) {};

		protected:
			const char * m_label;
			ScreenType_t m_type;
	};
}
#endif //SCREEN_H
