///////////////////////////////////////////////////////////////////////////////
/// \file Icon.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Implementation of Icon objects.
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

#include "Icon.h"
#include "GuiPainter.h"

namespace ui
{
	Icon::Icon (Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const char * text)
		: m_size(size)
		, m_bitmap(bitmap)
		, m_focused_bitmap(focused_bitmap)
		, m_text(text)
	{ }

	Icon::~Icon()
	{ };

	uint8_t const & Icon::width() const
	{
		return m_size.width;
	}

	uint8_t const & Icon::height() const
	{
		return m_size.height;
	}

	const char * Icon::text() const
	{
		return m_text;
	}

	void Icon::draw(uint8_t x, uint8_t y, bool focused)
	{
		painter.setColorIndex(1);
		painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);
	}

	void Icon::show()
	{ }
}
