///////////////////////////////////////////////////////////////////////////////
/// \file GuiPainter.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Painting interface class to U8glib
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or 
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef GUI_PAINTER_H
#define GUI_PAINTER_H

#include <stdint.h>

// Painting library
#include "Singleton.h"
#include "ultralcd_st7920_u8glib_rrd.h"

// Specify the printer UI implementation
#include "GuiImpl_witbox_2.h"

namespace ui
{
	struct Area
	{
		Area()
			: x_init(0)
			, y_init(0)
			, x_end(screen_width - 1)
			, y_end(screen_height - 1)
		{ }

		Area(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
			: x_init(x0)
			, y_init(y0)
			, x_end(x1)
			, y_end(y1)
		{ }

		uint8_t width() { return x_end - x_init + 1; }
		uint8_t height() { return y_end - y_init + 1; }

		uint8_t x_init;
		uint8_t y_init;
		uint8_t x_end;
		uint8_t y_end;
	};

	typedef enum
	{
		NONE = 0,
		LEFT,
		RIGHT,
		BOTH,
		NUM_SCREEN_TYPES,
	} ArrowType_t;

	typedef enum
	{
		BODY_FONT,
	} FontType_t;

	class GuiPainter
	{
		public:
			typedef Singleton<ui::GuiPainter> singleton;

		public:
			GuiPainter();
			~GuiPainter();

			void begin();
			void firstPage();
			bool nextPage();

			void clearWorkingArea();
			void setWorkingArea(Area a);
			Area getWorkingArea();

			void title(const char * title);
			void box(const char* text, ArrowType_t arrow = NONE);
			void printingStatus(const uint8_t percentage, bool progress_bar);

			void text(const char * msg);
			void text_P(const char * msg);
			void multiText(const char * msg);
			void multiText_P(const char * msg);

			void setFont(FontType_t font);
			void setColorIndex(uint8_t color);
			void setPrintPos(uint8_t x, uint8_t y);

			void print(const char * text);
			void print_P(const char * text);

			void animate(const char * text, uint8_t window, uint32_t delay_ms);
			void animationReset(uint32_t timeout);

			void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
			void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
			void drawBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const unsigned char* bitmap);

			char * itostr2(const int &xx);

			char * itostr3left(const int &xx);

			void coordinateXInit(uint8_t coordinate);

			uint8_t coordinateXInit ();

			void coordinateYInit(uint8_t coordinate);

			uint8_t coordinateYInit ();

			void coordinateXEnd(uint8_t coordinate);

			uint8_t coordinateXEnd ();

			void coordinateYEnd(uint8_t coordinate);

			uint8_t coordinateYEnd ();

		private:
			U8GLIB_ST7920_128X64_RRD m_impl;
			uint8_t m_x_init;
			uint8_t m_y_init;
			uint8_t m_x_end;
			uint8_t m_y_end;

			Area m_working_area;

			char m_animation_index;
			bool m_animation_dir;
			char m_animation_text[64];
			bool m_animation_loop;
			uint32_t m_current_update_time;
			uint32_t m_previous_update_time;
	};
}
#define painter ui::GuiPainter::singleton::instance()

#endif // GUI_PAINTER_H
