///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSplash.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of Splash screen.
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

#ifndef SCREEN_SPLASH_H
#define SCREEN_SPLASH_H

#include "Screen.h"

namespace ui
{
	class ScreenSplash : public Screen
	{
		public:
			ScreenSplash(uint32_t timeout);
			~ScreenSplash();

			void init(uint16_t index = 0);
			void draw();
			void add(ScreenIndex_t const & component);

		private:
			uint32_t m_destroy_time;

			ScreenIndex_t m_alt_screen;
			ScreenIndex_t m_block_screen;
			uint8_t m_num_item_added;
	};
}
#endif // SCREEN_SPLASH_H
