#include "menu_base.h"
//#include <LiquidCrystal.h>
//extern LiquidCrystal lcd;
extern "C" {
  void __cxa_pure_virtual()
  {
    // put error handling here
  }
}

MenuBase::MenuBase()
{
//lcd=_lcd;
  curPage=0;
  //curLine=0;
  maxPage=0; 
  for(int i=0;i<MAXPAGES;i++)
    pages[i]=0;
}
MenuBase::~MenuBase()
{
  
}
 
void MenuBase::addMenuPage(MenuPage *_newpage)
{
  if(maxPage<MAXPAGES-1)
  {
    
    pages[maxPage]=_newpage;
    if(maxPage==0)
		{
     pages[0]->activate();
		}
    maxPage++;
  }
}


MenuPage::MenuPage()
{
  line=0;
	firstline=1;
}

void MenuPage::lineUp()
{
	emptyline();
  if(line==0)
    line=items;
   else
    line--;
	fillline();
}

void MenuPage::lineDown()
{
  emptyline();
  if(line==items)
    line=0;
   else
    line++;
  fillline();
}

