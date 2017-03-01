///////////////////////////////////////////////////////////////////////////////
/// \file IconWidget.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///			Koldo Imanol de Miguel
///
/// \brief Interface for widget-like Icon objects.
///
/// Copyright (c) 2016 BQ - Mundo Reader S.L.
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

#ifndef ICON_WIDGET_HEATEDBED_H
#define ICON_WIDGET_HEATEDBED_H

#include "Icon.h"
#include "Observer.h"
#include "GuiPainter.h"
#include "TemperatureManager.h"

namespace ui
{
	template <typename T>
		class IconWidgetBed : public Icon, public Observer<T>
	{
		public:
			IconWidgetBed(Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const unsigned char* bitmap_2, const unsigned char * focused_bitmap_2, const char * text, Subject<T> * model);
			virtual ~IconWidgetBed();

			void update(T value);

			void draw(uint8_t x, uint8_t y, bool focused = false);
			virtual void show();

		private:
			T m_value;
			bool m_above_switch;
			
			const unsigned char * m_bitmap_2;
			const unsigned char * m_focused_bitmap_2;
	};

	template <typename T>
	IconWidgetBed<T>::IconWidgetBed(Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const unsigned char* bitmap_2, const unsigned char * focused_bitmap_2, const char * text, Subject<T> * model)
		: Icon(size, bitmap, focused_bitmap, text)
		, Observer<T>(model)
		, m_above_switch(false)
		, m_bitmap_2(bitmap_2)
		, m_focused_bitmap_2(focused_bitmap_2)
	{ }

	template <typename T>
	IconWidgetBed<T>::~IconWidgetBed()
	{ }

	template <typename T>
	void IconWidgetBed<T>::update(T value)
	{
		m_value = value;
	}

	template <typename T>
	void IconWidgetBed<T>::draw(uint8_t x, uint8_t y, bool focused)
	{
		painter.setColorIndex(1);
		
	#if defined(HBP_HEATER_AUX) && (HBP_HEATER_AUX > -1)
		int8_t bed_histeresis = 1;
		if(m_above_switch)
		{
			bed_histeresis *= -1;
		}
		
		if(temp::TemperatureManager::single::instance().getBedCurrentTemperature() < (BED_HOT_TEMP + bed_histeresis))
		{
			painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);
			m_above_switch = false;
		}
		else
		{
			painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap_2 : m_bitmap_2);
			m_above_switch = true;
		}
	#else
		painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);
	#endif
	
		if(focused)
		{
			painter.setColorIndex(0);
		}
		uint8_t margin = 29;
		if(m_value >= 100)
		{
			margin = 23;
		}
		else if(m_value < 10 && m_value >= 0)
		{
			margin = 35;
		}
		painter.setPrintPos(x + margin, y + (m_size.height / 2) - (max_font_height / 2));
		painter.print(painter.itostr3left((int)m_value));
		painter.print("\xb0");
	}

	template <typename T>
	void IconWidgetBed<T>::show()
	{
		this->connect();
	}
}
#endif //ICON_WIDGET_HEATEDBED_H
