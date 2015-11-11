///////////////////////////////////////////////////////////////////////////////
/// \file ViewManager.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief A manager class to keep track of active view.
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

#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <stdint.h>

#include "Screen.h"
#include "GuiImpl_witbox_2.h"

namespace ui
{
	class ViewManager
	{
		public:
			static ViewManager & getInstance();

			void activeView(ScreenIndex_t const & index);
			Screen * activeView();
			void setLastFocus(uint16_t last_focus);
			ScreenIndex_t const & getViewIndex() const;

			void displayRefresh();

		protected:
			ViewManager();
			~ViewManager();

		private:
			ViewManager(ViewManager const & orig) = delete;
			ViewManager & operator=(ViewManager & orig) = delete;

		private:
			Screen * m_active_view;
			uint16_t m_last_focus;
			ScreenIndex_t m_index;
	};
}
#endif //VIEW_MANAGER_H
