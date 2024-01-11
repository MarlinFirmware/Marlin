/**
 * Menu functions for ProUI
 * Author: Miguel A. Risco-Castillo
 * Version: 3.2.1
 * Date: 2023/09/30
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "dwinui.h"

#define MENU_CHAR_LIMIT  24

#ifndef MENU_MAX_ITEMS
  #define MENU_MAX_ITEMS 64
#endif

// Create and draw menu
#define SET_MENU(I,L) setMenu(I, GET_TEXT_F(L))
#define SET_MENU_F(I,L) setMenu(I, F(L))

// Create and add a MenuItem object to the menu array
#define BACK_ITEM(OC) menuItemAdd(ICON_Back, GET_TEXT_F(MSG_BUTTON_BACK), onDrawMenuItem, OC, nullptr)
#define MENU_ITEM(I,L,OD,OC) menuItemAdd(I, GET_TEXT_F(L), OD, OC, nullptr)
#define EDIT_ITEM(I,L,OD,OC,V) menuItemAdd(I, GET_TEXT_F(L), OD, OC, V)
#define MENU_ITEM_F(I,L,OD,OC) menuItemAdd(I, F(L), OD, OC, nullptr)
#define EDIT_ITEM_F(I,L,OD,OC,V) menuItemAdd(I, F(L), OD, OC, V)

typedef struct {
  int32_t maxValue     = 0;        // Auxiliar max integer/scaled float value
  int32_t minValue     = 0;        // Auxiliar min integer/scaled float value
  int8_t dp            = 0;        // Auxiliar decimal places
  int32_t value        = 0;        // Auxiliar integer / scaled float value
  int16_t *intPtr      = nullptr;  // Auxiliar pointer to 16 bit integer variable
  float *floatPtr      = nullptr;  // Auxiliar pointer to float variable
  void (*apply)()      = nullptr;  // Auxiliar apply function
  void (*liveUpdate)() = nullptr;  // Auxiliar live update function
} MenuData_t;
extern MenuData_t menuData;

struct MenuItem_t;
typedef void (*OnDrawItem)(int8_t pos, int8_t line);
typedef void (*OnClickItem)();
struct MenuItem_t {
  uint8_t icon = 0;
  FSTR_P caption = nullptr;
  OnDrawItem onDraw = nullptr;
  OnClickItem onClick = nullptr;
  void* value;
};
extern int8_t menuItemCount;

constexpr uint8_t menu_max_items = MENU_MAX_ITEMS;

class Menu {
public:
  int8_t topline = 0;
  int8_t selected = 0;
  char caption[32];
  Menu();
  virtual ~Menu(){};
  inline int8_t line() { return selected - topline; };
  inline int8_t line(int8_t pos) {return pos - topline; };
  virtual void draw();
  virtual void onScroll(bool dir);
  virtual void setCaption(const char * const titleStr);
  inline void setCaption(FSTR_P fTitle) { setCaption((char *)fTitle); }
  void onClick();
  uint8_t count;
  MenuItem_t selectedItem();
};
extern Menu *currentMenu;
extern Menu *previousMenu;

// Menuitem Drawing functions =================================================

void drawMenuCursor(const int8_t line);
void eraseMenuCursor(const int8_t line);
void eraseMenuText(const int8_t line);
void drawMenuItem(const int8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false, bool selected=false);
inline void drawMenuItem(const int8_t line, const uint8_t icon=0, FSTR_P label=nullptr, bool more=false, bool selected=false) {
  drawMenuItem(line, icon, FTOP(label), more, selected);
}
void drawCheckboxLine(const int8_t line, const bool checked);
void showCheckboxLine(const bool checked);
void toggleCheckboxLine(bool &checked);
void drawMenuIntValue(uint16_t bcolor, const int8_t line, uint8_t iNum, const int32_t value=0);
void drawItem(int8_t pos, int8_t line);

void onDrawMenuItem(int8_t pos, int8_t line);
void onDrawSubMenu(int8_t pos, int8_t line);
void onDrawIntMenu(int8_t pos, int8_t line, int32_t value);
void onDrawPIntMenu(int8_t pos, int8_t line);
void onDrawPInt8Menu(int8_t pos, int8_t line);
void onDrawPInt32Menu(int8_t pos, int8_t line);
void onDrawFloatMenu(int8_t pos, int8_t line, uint8_t dp, const float value);
void onDrawPFloatMenu(int8_t pos, int8_t line, uint8_t dp);
inline void onDrawPFloatMenu(int8_t pos, int8_t line) { onDrawPFloatMenu(pos, line, UNITFDIGITS); };
inline void onDrawPFloat2Menu(int8_t pos, int8_t line) { onDrawPFloatMenu(pos, line, 2); };
inline void onDrawPFloat3Menu(int8_t pos, int8_t line) { onDrawPFloatMenu(pos, line, 3); };
inline void onDrawPFloat4Menu(int8_t pos, int8_t line) { onDrawPFloatMenu(pos, line, 4); };
void onDrawChkbMenu(int8_t pos, int8_t line);

// On click functions =========================================================

// Generic onclick event without draw
//  process: process id HMI destiny
//  lo: low limit
//  hi: high limit
//  dp: decimal places, 0 for integers
//  val: value / scaled value
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setOnClick(uint8_t process, const int32_t lo, const int32_t hi, uint8_t dp, const int32_t val, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for integer values
//  process: process id HMI destiny
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setValueOnClick(uint8_t process, const int32_t lo, const int32_t hi, const int32_t val, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for float values
//  process: process id HMI destiny
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setValueOnClick(uint8_t process, const float lo, const float hi, uint8_t dp, const float val, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for integer values
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setIntOnClick(const int32_t lo, const int32_t hi, const int32_t val, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for set pointer to 16 bit integer values
//  lo: low limit
//  hi: high limit
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setPIntOnClick(const int32_t lo, const int32_t hi, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for float values
//  process: process id HMI destiny
//  lo: low limit
//  hi: high limit
//  dp: decimal places
//  val: value
void setFloatOnClick(const float lo, const float hi, uint8_t dp, const float val, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// Generic onclick event for set pointer to float values
//  lo: low limit
//  hi: high limit
//  liveUpdate: live update function when the encoder changes
//  apply: update function when the encoder is pressed
void setPFloatOnClick(const float lo, const float hi, uint8_t dp, void (*apply)()=nullptr, void (*liveUpdate)()=nullptr);

// HMI user control functions =================================================

// Generic menu control using the encoder
void hmiMenu();

// Set and draw a value using the encoder
void hmiSetDraw();

// Set an value without drawing
void hmiSetNoDraw();

// Set an integer pointer variable using the encoder
void hmiSetPInt();

// Set a scaled float pointer variable using the encoder
void hmiSetPFloat();

// Menu auxiliary functions ===================================================

// Initialize menu
void initMenu();

// Create a new menu (pass menu pointer as reference)
bool setMenu(Menu* &menu, FSTR_P fTitle);
inline bool setMenu(Menu* &menu) { return setMenu(menu, nullptr); };

// Reset top line and selected item
void resetMenu(Menu* &menu);

// Invalidate currentMenu to prepare for full menu drawing
void invalidateMenu();

//Redraw the current Menu if it is valid
void redrawMenu(bool force = false);

//Redraw menu item at pos in the array
void redrawItem(uint8_t pos, bool erase=false);

//Redraw selected menu item
void redrawItem();

// Clear menuItems array and free menuItems elements
void menuItemsClear();

// Is not the current menu
bool notCurrentMenu(Menu* menu);

// Is the screen a menu and that menu argument is current?
inline bool isMenu(Menu* menu) {
  return ((checkkey == ID_Menu) && !!currentMenu && currentMenu == menu);
}

// Add elements to the menuItems array
int8_t menuItemAdd(uint8_t cicon, FSTR_P text=nullptr, OnDrawItem ondraw=nullptr, OnClickItem onclick=nullptr, void* value=nullptr);
inline int8_t menuItemAdd(OnDrawItem ondraw=nullptr, OnClickItem onclick=nullptr) { return menuItemAdd(0, nullptr, ondraw, onclick); };

// Get an element from the menuItems array
MenuItem_t * getMenuItem(const int8_t pos);
