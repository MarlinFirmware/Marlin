#ifndef ICON_H
#define ICON_H

#include <stdint.h>

namespace screen
{
	class Icon 
	{	
		public:
			typedef enum
			{
				NORMAL = 0,
				SELECTED,
			} IconType_t;

		public:
			Icon(IconType_t const & type, uint8_t width, uint8_t height, const unsigned char* bitmap);
			~Icon();

			IconType_t const & type() const;
			uint8_t const & width() const;
			uint8_t const & height() const;

		protected:
			IconType_t m_type;
			uint8_t m_width;
			uint8_t m_height;
			const unsigned char* m_bitmap;
	};
}
#endif
