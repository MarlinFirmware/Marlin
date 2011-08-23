#ifndef __MENU_BASE
#define __MENU_BASE
#include "LiquidCrystal.h"
extern LiquidCrystal lcd;
class MenuPage
{
public:
  MenuPage();
  void lineUp();
  void lineDown();
  virtual void activate()=0;
	virtual void update()=0;

  int8_t line;
  int8_t items;
  int8_t xshift;
	int8_t firstline;

	inline void emptyline()
	{
		if(items>0)
			{
				lcd.setCursor((line/(4-firstline))*xshift,firstline+line%(4-firstline));
				lcd.print(" ");
			}
	};
	inline void fillline()
	{
		if(items>0)
			{
				lcd.setCursor((line/(4-firstline))*xshift,firstline+line%(4-firstline));
				lcd.print("~");
			}
	};
};

#define MAXPAGES 10


class MenuBase
{
public:
  MenuBase(); 
  ~MenuBase();


  void addMenuPage(MenuPage *_newpage);


  inline void pageUp() 
  { 
    //if(pages[curPage!=0])
    // pages[curPage]->deactivate();
    curPage++; 
    if(curPage>=maxPage)
    {
      curPage=0; 
    }
    if(pages[curPage]!=0)
      pages[curPage]->activate();

  };

  inline void pageDown() 
  { 
    //if(pages[curPage!=0])
    // pages[curPage]->deactivate();
    curPage--; 
    if(curPage<0)
    {
      curPage=maxPage-1; 
    }
    if(pages[curPage]!=0)
      pages[curPage]->activate();
  };

  inline void lineUp() 
  { 
    if(pages[curPage]!=0)
      pages[curPage]->lineUp();
  };

  inline void lineDown() 
  { 
    if(pages[curPage]!=0)
      pages[curPage]->lineDown();
  };
	
	  inline void update() 
  {
      pages[curPage]->update();
  };

public:
  //short int curLine;
  short int curPage;
  short int maxPage;
  MenuPage *pages[MAXPAGES];
};


#endif



