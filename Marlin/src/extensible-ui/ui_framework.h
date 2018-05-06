/********************
 * ui_framework.cpp *
 ********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#ifndef _UI_FRAMEWORK_H_
#define _UI_FRAMEWORK_H_

typedef enum {
  BACKGROUND  = 1,
  FOREGROUND  = 2,
  BOTH        = 3
} draw_mode_t;

 /********************** VIRTUAL DISPATCH DATA TYPE  ******************************/

// True virtual classes are extremely expensive on the Arduino
// as the compiler stores the virtual function tables in RAM.
// We invent a data type called ScreenRef that gives us
// polymorphism by mapping an ID to virtual methods on various
// classes. This works by keeping a table in PROGMEM of pointers
// to static methods.

#define DECL_SCREEN(className) { \
  className::onStartup, \
  className::onEntry, \
  className::onExit, \
  className::onIdle, \
  className::onRefresh, \
  className::onRedraw, \
  className::onTouchStart, \
  className::onTouchHeld, \
  className::onTouchEnd \
}

#define GET_METHOD(type, method) reinterpret_cast<method##_func_t*>( \
  sizeof(functionTable[type].method##_ptr) == 2 ? \
    pgm_read_word_near(&functionTable[type].method##_ptr) : \
    pgm_read_dword_near(&functionTable[type].method##_ptr) \
  )
#define SCREEN_TABLE             PROGMEM const ScreenRef::table_t ScreenRef::functionTable[] =
#define SCREEN_TABLE_POST        const uint8_t ScreenRef::functionTableSize = sizeof(ScreenRef::functionTable)/sizeof(ScreenRef::functionTable[0]);

class ScreenRef {
  protected:
    typedef void onStartup_func_t(void);
    typedef void onEntry_func_t(void);
    typedef void onExit_func_t(void);
    typedef void onIdle_func_t(void);
    typedef void onRefresh_func_t(void);
    typedef void onRedraw_func_t(draw_mode_t);
    typedef bool onTouchStart_func_t(uint8_t);
    typedef bool onTouchHeld_func_t(uint8_t);
    typedef bool onTouchEnd_func_t(uint8_t);

  private:
    typedef struct {
      onStartup_func_t     *onStartup_ptr;
      onEntry_func_t       *onEntry_ptr;
      onExit_func_t        *onExit_ptr;
      onIdle_func_t        *onIdle_ptr;
      onRefresh_func_t     *onRefresh_ptr;
      onRedraw_func_t      *onRedraw_ptr;
      onTouchStart_func_t  *onTouchStart_ptr;
      onTouchHeld_func_t   *onTouchHeld_ptr;
      onTouchEnd_func_t    *onTouchEnd_ptr;
    } table_t;

    uint8_t type = 0;
    static PROGMEM const table_t functionTable[];
    static const uint8_t functionTableSize;

  public:
    uint8_t getType() {return type;}

    void setType(uint8_t t) {
      type = t;
    }

    uint8_t lookupScreen(onRedraw_func_t onRedraw_ptr);

    void setScreen(onRedraw_func_t onRedraw_ptr);

    void onStartup()               {GET_METHOD(type, onStartup)();}
    void onEntry()                 {GET_METHOD(type, onEntry)();}
    void onExit()                  {GET_METHOD(type, onExit)();}
    void onIdle()                  {GET_METHOD(type, onIdle)();}
    void onRefresh()               {GET_METHOD(type, onRefresh)();}
    void onRedraw(draw_mode_t dm)  {GET_METHOD(type, onRedraw)(dm);}
    bool onTouchStart(uint8_t tag) {return GET_METHOD(type, onTouchStart)(tag);}
    bool onTouchHeld(uint8_t tag)  {return GET_METHOD(type, onTouchHeld)(tag);}
    bool onTouchEnd(uint8_t tag)   {return GET_METHOD(type, onTouchEnd)(tag);}

    void initializeAll();
};

/********************** SCREEN STACK  ******************************/

// To conserve dynamic memory, the screen stack is hard-coded to
// have four values, allowing a menu of up to four levels.

class ScreenStack : public ScreenRef {
  private:
    uint8_t stack[4];

  public:
    void start();
    void push();
    void pop();
    void forget();
    void goTo(onRedraw_func_t s);
    void goBack();

    uint8_t getScreen() {
      return getType();
    }
};

extern ScreenStack current_screen;

/********************** BASE SCREEN CLASSS ******************************/

/* UIScreen is the base class for all user interface screens.
 */
class UIScreen {
  public:
    static void onStartup()            {}
    static void onEntry()              {current_screen.onRefresh();}
    static void onExit()               {}
    static void onIdle()               {}
    static bool onTouchStart(uint8_t)  {return false;}
    static bool onTouchHeld(uint8_t)   {return false;}
    static bool onTouchEnd(uint8_t)    {return false;}
};


#define GOTO_SCREEN(screen) current_screen.goTo(screen::onRedraw);
#define GOTO_PREVIOUS()     current_screen.goBack();

#endif // _UI_FRAMEWORK_H_
