///////////////////////////////////////////////////////////////////////////////
/// \file OptionLaunch.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Implementation for launcher Option objects.
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

#include "OptionLaunch.h"

#include "GuiPainter.h"

namespace ui
{
	OptionLaunch::OptionLaunch(option::Size const & size, const char * text)
		: Option(size, text)
	{ }

	OptionLaunch::~OptionLaunch()
	{ }

	const char * OptionLaunch::text() const
	{
		return m_text;
	}

	void OptionLaunch::draw()
	{	
		Area draw_area = painter.getWorkingArea();
		painter.setPrintPos(0, draw_area.y_init);
		painter.print_P(m_text);
	}

	void OptionLaunch::press()
	{	
		ViewManager::getInstance().activeView(m_next_screen);
	}

	void OptionLaunch::add(ScreenIndex_t const & view)
	{
		m_next_screen = view;
	}
}