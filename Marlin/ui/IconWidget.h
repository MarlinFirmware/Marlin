///////////////////////////////////////////////////////////////////////////////
/// \file IconWidget.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface for widget-like Icon objects.
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

#ifndef ICON_WIDGET_H
#define ICON_WIDGET_H

#include "Icon.h"
#include "Observer.h"
#include "GuiPainter.h"

namespace ui
{
	template <typename T>
		class IconWidget : public Icon, public Observer<T>
	{
		public:
			IconWidget(Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const char * text, Subject<T> * model);
			virtual ~IconWidget();

			void update(T value);

			void draw(uint8_t x, uint8_t y, bool focused = false);
			virtual void show();

		private:
			T m_value;
	};

	template <typename T>
	IconWidget<T>::IconWidget(Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const char * text, Subject<T> * model)
		: Icon(size, bitmap, focused_bitmap, text)
		, Observer<T>(model)
	{ }

	template <typename T>
	IconWidget<T>::~IconWidget()
	{ }

	template <typename T>
	void IconWidget<T>::update(T value)
	{
		m_value = value;
	}

	template <typename T>
	void IconWidget<T>::draw(uint8_t x, uint8_t y, bool focused)
	{
		painter.setColorIndex(1);
		painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);

		if(focused)
		{
			painter.setColorIndex(0);
		}
		uint8_t margin = 26;
		if(m_value > 99)
		{
			margin = 20;
		}
		else if(m_value < 10 && m_value >= 0)
		{
			margin = 32;
		}
		painter.setPrintPos(x + margin, y + (m_size.height / 2) - (max_font_height / 2));
		painter.print(painter.itostr3left((int)m_value));
		painter.print("\xb0");
	}

	template <typename T>
	void IconWidget<T>::show()
	{
		this->connect();
	}
}
#endif //ICON_WIDGET_H
