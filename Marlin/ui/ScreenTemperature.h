///////////////////////////////////////////////////////////////////////////////
/// \file ScreenTemperature.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of screen temperature.
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

#ifndef SCREEN_TEMPERATURE_H
#define SCREEN_TEMPERATURE_H

#include <stdint.h>

#include "ScreenSelector.h"

namespace ui
{
	class ScreenTemperature : public ScreenSelector<void, uint16_t>
	{
		public:
			ScreenTemperature(const char * title, const char * box, uint16_t min, uint16_t max, uint16_t scale, uint16_t dflt, Functor<void, uint16_t>::FuncPtr fptr = 0);
			virtual ~ScreenTemperature();

			void draw();
			void press();
	};
}

#endif //SCREEN_TEMPERATURE_H
