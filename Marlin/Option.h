#ifndef OPTION
#define OPTION

#include <stdint.h>

namespace screen
{
	namespace option
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
	}

	class Option
	{
		public:
			Option(option::Size const & size, const char * text = 0);
			virtual ~Option();

			uint8_t const & width() const;
			uint8_t const & height() const;
			virtual const char * text() const;
			virtual void draw() = 0;

			virtual void press();
		
		protected:
			option::Size m_size;
			const char * m_text;
	};
}

#endif //OPTION