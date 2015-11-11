///////////////////////////////////////////////////////////////////////////////
/// \file ScreenTransition.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of transition-type screens.
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

#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include "Screen.h"
#include "Functor.h"
#include "PrintManager.h"

namespace ui
{
	class ScreenTransition : public Screen , public Functor<void>, public Observer<PrinterState_t>
	{
		public:
			ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr = do_nothing,  Subject<PrinterState_t> * model = 0);
			virtual ~ScreenTransition();

			void init(uint16_t index = 0);
			void draw();
			void update(PrinterState_t value);

		private:
			char m_message[64];
			const char * m_box;
			PrinterState_t m_printing_status;
	};
}
#endif //SCREEN_TRANSITION_H
