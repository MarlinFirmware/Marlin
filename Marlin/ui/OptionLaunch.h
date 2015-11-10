///////////////////////////////////////////////////////////////////////////////
/// \file OptionLaunch.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface for launcher Option objects.
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

#ifndef OPTION_LAUNCH_H
#define OPTION_LAUNCH_H

#include "Option.h"
#include "Screen.h"

namespace ui
{
	class OptionLaunch : public Option
	{
		public:
			OptionLaunch(option::Size const & size, const char * text);
			virtual ~OptionLaunch();

			const char * text() const;
			virtual void draw();
			virtual void press();

			void add(ScreenIndex_t const & view);

		private:
			ScreenIndex_t m_next_screen;
	};
	
}

#endif //OPTION_LAUNCH_H