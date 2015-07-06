#ifndef GUI_IMPL_H
#define GUI_IMPL_H

#include <stdint.h>

// Painting library
#include "Singleton.h"
#include "ultralcd_st7920_u8glib_rrd.h"


// Specify the printer UI implementation
#include "GuiImpl_witbox_2.h"

namespace screen
{
	class GuiPainter
	{
		public:
			typedef Singleton<screen::GuiPainter> singleton;

			typedef enum
			{
				XINIT = 0,
				YINIT,
				XEND,
				YEND,
			} CoordinateType_t;

		public:
			GuiPainter();
			~GuiPainter();

			void title(const char * title);

			void text(const char * msg, uint8_t h_pad = 0, uint8_t v_pad = 0);
			void text_P(const char * msg, uint8_t h_pad = 0, uint8_t v_pad = 0);
			
			void multiText(const char * msg, bool align_top);

			void printingStatus(const uint8_t percentage, const uint8_t hour, const uint8_t minute);

			void selector(uint16_t number, uint16_t total);

			void box(const char* text);
			void arrowBox(const char* text);

			void setColorIndex(uint8_t color);

			void setFont(const u8g_fntpgm_uint8_t* font);

			void setPrintPos(uint8_t x, uint8_t y);

			void print(const char * text);
			void print_P(const char * text);

			void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
			void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

			void drawBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const unsigned char* bitmap);

			void begin();

			void firstPage();
			bool nextPage();

			void coordinateXInit(uint8_t coordinate);

			uint8_t coordinateXInit ();

			void coordinateYInit(uint8_t coordinate);

			uint8_t coordinateYInit ();

			void coordinateXEnd(uint8_t coordinate);

			uint8_t coordinateXEnd ();

			void coordinateYEnd(uint8_t coordinate);

			uint8_t coordinateYEnd ();

			char * itostr2(const int &xx);

			char * itostr3left(const int &xx);

		private:
			U8GLIB_ST7920_128X64_RRD m_impl;
			uint8_t m_x_init;
			uint8_t m_y_init;
			uint8_t m_x_end;
			uint8_t m_y_end;


	};
}
#define painter screen::GuiPainter::singleton::instance()

#endif //GUI_IMPL_H
