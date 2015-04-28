#ifndef ICON_H
#define ICON_H

#include <stdint.h>

namespace screen
{
	class Icon 
	{
		public:
			Icon(uint8_t width, uint8_t height, const unsigned char* bitmap);

		protected:
			uint8_t m_width;
			uint8_t m_height;
			const unsigned char* m_bitmap;

	};
}
#endif
