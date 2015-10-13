///////////////////////////////////////////////////////////////////////////////
/// \file OptionToggle.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface for toggle Option objects.
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

#ifndef OPTION_TOGGLE_H
#define OPTION_TOGGLE_H

#include "Option.h"
#include "Observer.h"
#include "Functor.h"

namespace screen
{
	typedef enum
	{
		ON,
		OFF,
		NUM_STATES
	} ToggleState_t;

	class OptionToggle : public Option , public Functor<void>, public Observer<bool>
	{
		public:
			OptionToggle(option::Size const & size, const char * text, Functor<void>::FuncPtr fptr = do_nothing, Subject<bool> * model = 0);
			virtual ~OptionToggle();

			const char * text() const;
			virtual void draw();
			virtual void press();

			void update(bool value);

		private:
			ToggleState_t m_state;
	};
	
}

#endif //OPTION_TOGGLE_H