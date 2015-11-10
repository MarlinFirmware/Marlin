///////////////////////////////////////////////////////////////////////////////
/// \file Icon.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface definition of Icon objects.
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

#ifndef ICON_H
#define ICON_H

#include <stdint.h>

namespace ui
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
			virtual ~Icon();

			uint8_t const & width() const;
			uint8_t const & height() const;
			virtual const char * text() const;
			virtual void draw(uint8_t x, uint8_t y, bool focused = false);

			virtual void show();

		protected:
			Size m_size;
			const unsigned char * m_bitmap;
			const unsigned char * m_focused_bitmap;
			const char * m_text;
	};
}
#endif //ICON_H
