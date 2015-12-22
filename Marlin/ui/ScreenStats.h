///////////////////////////////////////////////////////////////////////////////
/// \file ScreenStats.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///			Koldo Imanol de Miguel
///
/// \brief Definition of Print Complete screen.
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

#ifndef SCREEN_STATS_H
#define SCREEN_STATS_H

#include "ScreenDialog.h"

namespace ui
{
	class ScreenStats : public Screen
	{
		public:
			ScreenStats(const char * title, const char * box);
			virtual ~ScreenStats();

			void draw();
			void press();
			void init(uint16_t index = 0);

		private:
			const char * m_box;
			
			uint16_t m_hours;
			uint16_t m_failed_prints;
			uint16_t m_succeded_prints;
	};
}

#endif // SCREEN_STATS_H
