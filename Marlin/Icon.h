#ifndef ICON_H
#define ICON_H

#include <stdint.h>


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

	class Icon 
	{	
		public:
			typedef enum
			{
				NORMAL = 0,
				SELECTED,
			} IconType_t;

		public:
			Icon(IconType_t const & type, Size const & size, const unsigned char* bitmap);
			~Icon();

			IconType_t const & type() const;
			uint8_t const & width() const;
			uint8_t const & height() const;
			void draw(uint8_t x, uint8_t y);

		protected:
			IconType_t m_type;
			Size m_size;
			const unsigned char* m_bitmap;
	};
}
#endif
