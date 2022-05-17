/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Menu functions for ProUI
 * Author: Miguel A. Risco-Castillo
 * Version: 1.4.1
 * Date: 2022/04/14
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../common/encoder.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"
#include "menus.h"

int8_t MenuItemTotal = 0;
int8_t MenuItemCount = 0;
MenuItemClass** MenuItems = nullptr;
MenuClass *CurrentMenu = nullptr;
MenuClass *PreviousMenu = nullptr;
void (*onMenuDraw)(MenuClass* menu) = nullptr;
void (*onCursorErase)(const int8_t line) = nullptr;
void (*onCursorDraw)(const int8_t line) = nullptr;
MenuData_t MenuData;

// Menuitem Drawing functions =================================================

void Draw_Title(TitleClass* title) {
  DWIN_Draw_Rectangle(1, HMI_data.TitleBg_color, 0, 0, DWIN_WIDTH - 1, TITLE_HEIGHT - 1);
  if (title->frameid)
    DWIN_Frame_AreaCopy(title->frameid, title->frame.left, title->frame.top, title->frame.right, title->frame.bottom, 14, (TITLE_HEIGHT - (title->frame.bottom - title->frame.top)) / 2 - 1);
  else
    DWIN_Draw_String(false, DWIN_FONT_HEAD, HMI_data.TitleTxt_color, HMI_data.TitleBg_color, 14, (TITLE_HEIGHT - DWINUI::fontHeight(DWIN_FONT_HEAD)) / 2 - 1, title->caption);
}

void Draw_Menu(MenuClass* menu) {
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  DWIN_Draw_Rectangle(1, DWINUI::backcolor, 0, TITLE_HEIGHT, DWIN_WIDTH - 1, STATUS_Y - 1);
}

void Draw_Menu_Cursor(const int8_t line) {
  const uint16_t ypos = MYPOS(line);
  DWINUI::Draw_Box(1, HMI_data.Cursor_color, {0, ypos, 15, MLINE - 1});
}

void Erase_Menu_Cursor(const int8_t line) {
  const uint16_t ypos = MYPOS(line);
  DWINUI::Draw_Box(1, HMI_data.Background_Color, {0, ypos, 15, MLINE - 1});
}

void Draw_Menu_Line(const uint8_t line, const uint8_t icon /*=0*/, const char * const label /*=nullptr*/, bool more /*=false*/) {
  if (icon)  DWINUI::Draw_Icon(icon, ICOX, MBASE(line) - 3);
  if (label) DWINUI::Draw_String(LBLX, MBASE(line) - 1, (char*)label);
  if (more)  DWINUI::Draw_Icon(ICON_More, VALX + 16, MBASE(line) - 3);
  DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

void Draw_Chkb_Line(const uint8_t line, const bool checked) {
  DWINUI::Draw_Checkbox(HMI_data.Text_Color, HMI_data.Background_Color, VALX + 3 * DWINUI::fontWidth(), MBASE(line) - 1, checked);
}

void Draw_Menu_IntValue(uint16_t bcolor, const uint8_t line, uint8_t iNum, const int32_t value /*=0*/) {
  DWINUI::Draw_Signed_Int(HMI_data.Text_Color, bcolor, iNum , VALX, MBASE(line) - 1, value);
}

void onDrawMenuItem(MenuItemClass* menuitem, int8_t line) {
  if (menuitem->icon) DWINUI::Draw_Icon(menuitem->icon, ICOX, MBASE(line) - 3);
  if (menuitem->frameid)
    DWIN_Frame_AreaCopy(menuitem->frameid, menuitem->frame.left, menuitem->frame.top, menuitem->frame.right, menuitem->frame.bottom, LBLX, MBASE(line));
  else if (menuitem->caption)
    DWINUI::Draw_String(LBLX, MBASE(line) - 1, menuitem->caption);
  DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

void onDrawSubMenu(MenuItemClass* menuitem, int8_t line) {
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_Icon(ICON_More, VALX + 16, MBASE(line) - 3);
}

void onDrawIntMenu(MenuItemClass* menuitem, int8_t line, int32_t value) {
  onDrawMenuItem(menuitem, line);
  Draw_Menu_IntValue(HMI_data.Background_Color, line, 4, value);
}

void onDrawPIntMenu(MenuItemClass* menuitem, int8_t line) {
  const int16_t value = *(int16_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawIntMenu(menuitem, line, value);
}

void onDrawPInt8Menu(MenuItemClass* menuitem, int8_t line) {
  const uint8_t value = *(uint8_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawIntMenu(menuitem, line, value);
}

void onDrawPInt32Menu(MenuItemClass* menuitem, int8_t line) {
  const uint32_t value = *(uint32_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawIntMenu(menuitem, line, value);
}

void onDrawFloatMenu(MenuItemClass* menuitem, int8_t line, uint8_t dp, const float value) {
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, dp, VALX - dp * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(line), value);
}

void onDrawPFloatMenu(MenuItemClass* menuitem, int8_t line) {
  const float value = *(float*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  const int8_t dp = UNITFDIGITS;
  onDrawFloatMenu(menuitem, line, dp, value);
}

void onDrawPFloat2Menu(MenuItemClass* menuitem, int8_t line) {
  const float value = *(float*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawFloatMenu(menuitem, line, 2, value);
}

void onDrawPFloat3Menu(MenuItemClass* menuitem, int8_t line) {
  const float value = *(float*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawFloatMenu(menuitem, line, 3, value);
}

void onDrawChkbMenu(MenuItemClass* menuitem, int8_t line, bool checked) {
  onDrawMenuItem(menuitem, line);
  Draw_Chkb_Line(line, checked);
}

void onDrawChkbMenu(MenuItemClass* menuitem, int8_t line) {
  const bool val = *(bool*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  onDrawChkbMenu(menuitem, line, val);
}

//-----------------------------------------------------------------------------
// On click functions
//-----------------------------------------------------------------------------

// Generic onclick event without draw
//  process: process id HMI destiny
//  lo: low limit
//  hi: high limit
//  dp: decimal places, 0 for integers
//  val: value / scaled value
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetOnClick(uint8_t process, const int32_t lo, const int32_t hi, uint8_t dp, const int32_t val, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  checkkey = process;
  MenuData.MinValue = lo;
  MenuData.MaxValue = hi;
  MenuData.dp = dp;
  MenuData.Apply = Apply;
  MenuData.LiveUpdate = LiveUpdate;
  MenuData.Value = constrain(val, lo, hi);
  EncoderRate.enabled = true;
}

// Generic onclick event for integer values
//  process: process id HMI destiny
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetValueOnClick(uint8_t process, const int32_t lo, const int32_t hi, const int32_t val, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  SetOnClick(process, lo, hi, 0, val, Apply, LiveUpdate);
  Draw_Menu_IntValue(HMI_data.Selected_Color, CurrentMenu->line(), 4, MenuData.Value);
}

// Generic onclick event for float values
//  process: process id HMI destiny
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetValueOnClick(uint8_t process, const float lo, const float hi, uint8_t dp, const float val, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  const int32_t value =  round(val * POW(10, dp));
  SetOnClick(process, lo * POW(10, dp), hi * POW(10, dp), dp, value, Apply, LiveUpdate);
  DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, dp, VALX - dp * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), val);
}

// Generic onclick event for integer values
//  lo: scaled low limit
//  hi: scaled high limit
//  val: value
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetIntOnClick(const int32_t lo, const int32_t hi, const int32_t val, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  SetValueOnClick(SetInt, lo, hi, val, Apply, LiveUpdate);
}

// Generic onclick event for set pointer to 16 bit uinteger values
//  lo: low limit
//  hi: high limit
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetPIntOnClick(const int32_t lo, const int32_t hi, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  MenuData.P_Int = (int16_t*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
  const int32_t value = *MenuData.P_Int;
  SetValueOnClick(SetPInt, lo, hi, value, Apply, LiveUpdate);
}

// Generic onclick event for float values
//  process: process id HMI destiny
//  lo: low limit
//  hi: high limit
//  dp: decimal places
//  val: value
void SetFloatOnClick(const float lo, const float hi, uint8_t dp, const float val, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  SetValueOnClick(SetFloat, lo, hi, dp, val, Apply, LiveUpdate);
}

// Generic onclick event for set pointer to float values
//  lo: low limit
//  hi: high limit
//  LiveUpdate: live update function when the encoder changes
//  Apply: update function when the encoder is pressed
void SetPFloatOnClick(const float lo, const float hi, uint8_t dp, void (*Apply)() /*= nullptr*/, void (*LiveUpdate)() /*= nullptr*/) {
  MenuData.P_Float = (float*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
  SetValueOnClick(SetPFloat, lo, hi, dp, *MenuData.P_Float, Apply, LiveUpdate);
}

// HMI Control functions ======================================================

// Generic menu control using the encoder
void HMI_Menu() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (CurrentMenu) {
    if (encoder_diffState == ENCODER_DIFF_ENTER)
      CurrentMenu->onClick();
    else
      CurrentMenu->onScroll(encoder_diffState == ENCODER_DIFF_CW);
  }
}

// Get an integer value using the encoder without draw anything
//  lo: low limit
//  hi: high limit
// Return value:
//  0 : no change
//  1 : live change
//  2 : apply change
int8_t HMI_GetIntNoDraw(const int32_t lo, const int32_t hi) {
  const int32_t cval = MenuData.Value;
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, MenuData.Value)) {
      EncoderRate.enabled = false;
      checkkey = Menu;
      return 2;
    }
    LIMIT(MenuData.Value, lo, hi);
  }
  return int8_t(cval != MenuData.Value);
}

// Get an integer value using the encoder
//  lo: low limit
//  hi: high limit
// Return value:
//  0 : no change
//  1 : live change
//  2 : apply change
int8_t HMI_GetInt(const int32_t lo, const int32_t hi) {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, MenuData.Value)) {
      EncoderRate.enabled = false;
      DWINUI::Draw_Signed_Int(HMI_data.Text_Color, HMI_data.Background_Color, 4 , VALX, MBASE(CurrentMenu->line()) - 1, MenuData.Value);
      checkkey = Menu;
      return 2;
    }
    LIMIT(MenuData.Value, lo, hi);
    DWINUI::Draw_Signed_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 4 , VALX, MBASE(CurrentMenu->line()) - 1, MenuData.Value);
    return 1;
  }
  return 0;
}

// Set an integer using the encoder
void HMI_SetInt() {
  int8_t val = HMI_GetInt(MenuData.MinValue, MenuData.MaxValue);
  switch (val) {
    case 0: return; break;
    case 1: if (MenuData.LiveUpdate) MenuData.LiveUpdate(); break;
    case 2: if (MenuData.Apply) MenuData.Apply(); break;
  }
}

// Set an integer without drawing
void HMI_SetIntNoDraw() {
  int8_t val = HMI_GetIntNoDraw(MenuData.MinValue, MenuData.MaxValue);
  switch (val) {
    case 0: return; break;
    case 1: if (MenuData.LiveUpdate) MenuData.LiveUpdate(); break;
    case 2: if (MenuData.Apply) MenuData.Apply(); break;
  }
}

// Set an integer pointer variable using the encoder
void HMI_SetPInt() {
  int8_t val = HMI_GetInt(MenuData.MinValue, MenuData.MaxValue);
  switch (val) {
    case 0: return;
    case 1: if (MenuData.LiveUpdate) MenuData.LiveUpdate(); break;
    case 2: *MenuData.P_Int = MenuData.Value; if (MenuData.Apply) MenuData.Apply(); break;
  }
}

// Get a scaled float value using the encoder
//  dp: decimal places
//  lo: scaled low limit
//  hi: scaled high limit
// Return value:
//  0 : no change
//  1 : live change
//  2 : apply change
int8_t HMI_GetFloat(uint8_t dp, int32_t lo, int32_t hi) {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, MenuData.Value)) {
      EncoderRate.enabled = false;
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, dp, VALX - dp * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), MenuData.Value / POW(10, dp));
      checkkey = Menu;
      return 2;
    }
    LIMIT(MenuData.Value, lo, hi);
    DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, dp, VALX - dp * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), MenuData.Value / POW(10, dp));
    return 1;
  }
  return 0;
}

// Set a scaled float using the encoder
void HMI_SetFloat() {
  const int8_t val = HMI_GetFloat(MenuData.dp, MenuData.MinValue, MenuData.MaxValue);
  switch (val) {
    case 0: return;
    case 1: if (MenuData.LiveUpdate) MenuData.LiveUpdate(); break;
    case 2: if (MenuData.Apply) MenuData.Apply(); break;
  }
}

// Set a scaled float pointer variable using the encoder
void HMI_SetPFloat() {
  const int8_t val = HMI_GetFloat(MenuData.dp, MenuData.MinValue, MenuData.MaxValue);
  switch (val) {
    case 0: return;
    case 1: if (MenuData.LiveUpdate) MenuData.LiveUpdate(); break;
    case 2: *MenuData.P_Float = MenuData.Value / POW(10, MenuData.dp); if (MenuData.Apply) MenuData.Apply(); break;
  }
}

// Menu Classes ===============================================================

MenuClass::MenuClass() {
  selected = 0;
  topline = 0;
}

void MenuClass::draw() {
  MenuTitle.draw();
  if (onMenuDraw != nullptr) onMenuDraw(this);
  for (int8_t i = 0; i < MenuItemCount; i++)
    MenuItems[i]->draw(i - topline);
  if (onCursorDraw != nullptr) onCursorDraw(line());
  DWIN_UpdateLCD();
}

void MenuClass::onScroll(bool dir) {
  int8_t sel = selected;
  if (dir) sel++; else sel--;
  LIMIT(sel, 0, MenuItemCount - 1);
  if (sel != selected) {
    if (onCursorErase != nullptr) onCursorErase(line());
    DWIN_UpdateLCD();
    if ((sel - topline) == TROWS) {
      DWIN_Frame_AreaMove(1, DWIN_SCROLL_UP, MLINE, DWINUI::backcolor, 0, TITLE_HEIGHT + 1, DWIN_WIDTH, STATUS_Y - 1);
      topline++;
      MenuItems[sel]->draw(TROWS - 1);
    }
    if ((sel < topline)) {
      DWIN_Frame_AreaMove(1, DWIN_SCROLL_DOWN, MLINE, DWINUI::backcolor, 0, TITLE_HEIGHT + 1, DWIN_WIDTH, STATUS_Y - 1);
      topline--;
      MenuItems[sel]->draw(0);
    }
    selected = sel;
    if (onCursorDraw != nullptr) onCursorDraw(line());
    DWIN_UpdateLCD();
  }
}

void MenuClass::onClick() {
  if (MenuItems[selected]->onClick != nullptr) (*MenuItems[selected]->onClick)();
}

MenuItemClass *MenuClass::SelectedItem() {
  return MenuItems[selected];
}

MenuItemClass** MenuClass::Items() {
  return MenuItems;
}

int8_t MenuClass::count() {
  return MenuItemCount;
};

/* MenuItem Class ===========================================================*/

MenuItemClass::MenuItemClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)()) {
  icon = cicon;
  onClick = onclick;
  onDraw = ondraw;
  const uint8_t len = _MIN(sizeof(caption) - 1, strlen(text));
  memcpy(&caption[0], text, len);
  caption[len] = '\0';
}

MenuItemClass::MenuItemClass(uint8_t cicon, uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)()) {
  icon = cicon;
  onClick = onclick;
  onDraw = ondraw;
  caption[0] = '\0';
  frameid = id;
  frame = { x1, y1, x2, y2 };
}

void MenuItemClass::SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  caption[0] = '\0';
  frameid = id;
  frame = { x1, y1, x2, y2 };
}

void MenuItemClass::draw(int8_t line) {
  if (line < 0 || line >= TROWS) return;
  if (onDraw != nullptr) (*onDraw)(this, line);
};

void MenuItemClass::redraw() {
  draw(CurrentMenu->line(this->pos));
}

MenuItemPtrClass::MenuItemPtrClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val) : MenuItemClass(cicon, text, ondraw, onclick) {
  value = val;
};

// Menu auxiliary functions ===================================================

void MenuItemsClear() {
  if (MenuItems == nullptr) return;
  for (int8_t i = 0; i < MenuItemCount; i++) delete MenuItems[i];
  delete[] MenuItems;
  MenuItems = nullptr;
  MenuItemCount = 0;
  MenuItemTotal = 0;
}

void MenuItemsPrepare(int8_t totalitems) {
  MenuItemsClear();
  MenuItemTotal = totalitems;
  MenuItems = new MenuItemClass*[totalitems];
}

MenuItemClass* MenuItemsAdd(MenuItemClass* menuitem) {
  MenuItems[MenuItemCount] = menuitem;
  menuitem->pos = MenuItemCount++;
  return menuitem;
}

MenuItemClass* MenuItemsAdd(uint8_t cicon, const char * const text/*=nullptr*/, void (*ondraw)(MenuItemClass* menuitem, int8_t line)/*=nullptr*/, void (*onclick)()/*=nullptr*/) {
  if (MenuItemCount < MenuItemTotal) {
    MenuItemClass* menuitem = new MenuItemClass(cicon, text, ondraw, onclick);
    return MenuItemsAdd(menuitem);
  }
  else return nullptr;
}

MenuItemClass* MenuItemsAdd(uint8_t cicon, uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void (*ondraw)(MenuItemClass* menuitem, int8_t line)/*=nullptr*/, void (*onclick)()/*=nullptr*/) {
  if (MenuItemCount < MenuItemTotal) {
    MenuItemClass* menuitem = new MenuItemClass(cicon, id, x1, y1, x2, y2, ondraw, onclick);
    return MenuItemsAdd(menuitem);
  }
  else return nullptr;
}

MenuItemClass* MenuItemsAdd(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val) {
  if (MenuItemCount < MenuItemTotal) {
    MenuItemClass* menuitem = new MenuItemPtrClass(cicon, text, ondraw, onclick, val);
    return MenuItemsAdd(menuitem);
  }
  else return nullptr;
}

bool SetMenu(MenuClass* &menu, FSTR_P title, int8_t totalitems) {
  if (!menu) menu = new MenuClass();
  const bool NotCurrent = (CurrentMenu != menu);
  if (NotCurrent) {
    menu->MenuTitle.SetCaption(title);
    MenuItemsPrepare(totalitems);
  }
  return NotCurrent;
}

void UpdateMenu(MenuClass* &menu) {
  if (!menu) return;
  if (CurrentMenu != menu) {
    PreviousMenu = CurrentMenu;
    CurrentMenu = menu;
  }
  menu->draw();
}

void ReDrawMenu() { if (CurrentMenu && checkkey==Menu) CurrentMenu->draw(); }

#endif // DWIN_LCD_PROUI
