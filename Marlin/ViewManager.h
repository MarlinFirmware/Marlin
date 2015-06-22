#ifndef VIEW_MANAGER_H
#define	VIEW_MANAGER_H

#include "Screen.h"
#include "GuiImpl_witbox_2.h"

namespace screen
{
	class ViewManager
	{
		public:
			static ViewManager & getInstance();

			void buildView();
			void activeView(Screen* state);
			Screen * activeView();

		protected:
			ViewManager();
			~ViewManager();

		private:
			ViewManager(ViewManager const & orig);
			ViewManager & operator=(ViewManager & orig);

		private:
			Screen * m_active_view;
	};
}


#endif //VIEW_MANAGER_H
