///////////////////////////////////////////////////////////////////////////////
/// \file ScreenInactivity.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of inactivity screen.
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

#ifndef SCREEN_INACTIVITY_H
#define SCREEN_INACTIVITY_H

#include "ScreenAnimation.h"

#include "GuiManager.h"
#include "TemperatureManager.h"
#include "ViewManager.h"

namespace ui
{
	class ScreenInactivity : public ScreenAnimation<float>
	{
		public:
			ScreenInactivity(const char * title, const char * text, uint16_t target, Subject<float> * model = 0);
			virtual ~ScreenInactivity();

			void init(uint16_t index = 0);

			void draw();
			void press();

			void add(ScreenIndex_t const & component);
			void update(float value);

			void left();
			void right();

		private:
			bool isConditionMet();
	};
}

#endif //SCREEN_INACTIVITY_H
