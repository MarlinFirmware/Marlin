#ifndef ICON_STATUS_H
#define ICON_STATUS_H

#include "Icon.h"
#include "Observer.h"

#include "GuiPainter.h"

namespace screen
{
	template <typename T>
		class IconStatus : public Icon, public Observer<T>
	{
		public:
			IconStatus(Size const & size, const unsigned char * bitmap, const unsigned char * focused_bitmap, const unsigned char * alt_bitmap, const unsigned char * alt_focused_bitmap, const char * text, const char * alt_text, Subject<T> * model);
			virtual ~IconStatus();

			void update(T value);

			const char * text() const;
			void draw(uint8_t x, uint8_t y, bool focused = false);
			virtual void show();

		private:
			T m_value;
			const unsigned char * m_alt_bitmap;
			const unsigned char * m_alt_focused_bitmap;
			const char * m_alt_text;
	};

	template <typename T>
	IconStatus<T>::IconStatus(Size const & size, const unsigned char * bitmap, const unsigned char * focused_bitmap, const unsigned char * alt_bitmap, const unsigned char * alt_focused_bitmap, const char * text, const char * alt_text, Subject<T> * model)
		: Icon(size, bitmap, focused_bitmap, text)
		, m_alt_bitmap(alt_bitmap)
		, m_alt_focused_bitmap(alt_focused_bitmap)
		, m_alt_text(alt_text)
		, Observer<T>(model)
	{ }

	template <typename T>
	IconStatus<T>::~IconStatus()
	{ 
		this->m_model->dettach();
	}

	template <typename T>
	void IconStatus<T>::update(T value)
	{
		m_value = value;
	}

	template <typename T>
	const char * IconStatus<T>::text() const
	{
		if (m_value)
		{
			return m_alt_text;
		}
		else
		{
			return m_text;
		}
	}

	template <typename T>
	void IconStatus<T>::draw(uint8_t x, uint8_t y, bool focused)
	{
		painter.setColorIndex(1);

    	if (m_value)
    	{
			painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_alt_focused_bitmap : m_alt_bitmap);
		}
		else
		{
			painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);
		}
	}

	template <typename T>
	void IconStatus<T>::show()
	{
		this->m_model->attach(this);
		this->connect();
	}
}

#endif //ICON_STATUS_H
