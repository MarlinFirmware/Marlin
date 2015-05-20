#ifndef ELEMENT_H
#define ELEMENT_H

#include <new.h>

namespace screen
{
	struct Size
	{
		Size(uint8_t width, uint8_t height)
			: width(width)
			  , height(height)
		{ };

		uint8_t width;
		uint8_t height;
	};

	class Element
	{
		public:
			virtual void draw() = 0;
	};
}
#endif //ELEMENT_H
