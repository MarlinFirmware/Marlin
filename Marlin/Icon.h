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
			Icon(Size const & size, const unsigned char * bitmap, const unsigned char * focused_bitmap = 0, const char * text = 0);
			~Icon();

			uint8_t const & width() const;
			uint8_t const & height() const;
			const char * text() const;
			virtual void draw(uint8_t x, uint8_t y, bool focused = false);

		protected:
			Size m_size;
			const unsigned char * m_bitmap;
			const unsigned char * m_focused_bitmap;
			const char * m_text;
	};
}
#endif
