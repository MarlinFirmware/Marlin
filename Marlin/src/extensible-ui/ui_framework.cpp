/******************
 * ui_framework.h *
 ******************/

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

#include "../Marlin.h"
#include "ui_config.h"

#if defined(USE_UI_FRAMEWORK)

#include "ui_framework.h"

/********************** VIRTUAL DISPATCH DATA TYPE  ******************************/

uint8_t ScreenRef::lookupScreen(onRedraw_func_t onRedraw_ptr) {
  for(uint8_t type = 0; type < functionTableSize; type++) {
    if(GET_METHOD(type, onRedraw) == onRedraw_ptr) {
      return type;
    }
  }
  #if defined(UI_FRAMEWORK_DEBUG)
    #if defined(SERIAL_PROTOCOLLNPAIR)
    SERIAL_PROTOCOLLNPAIR("Screen not found: ", (uintptr_t) onRedraw_ptr);
    #else
    Serial.print("Screen not found: ");
    Serial.println((uintptr_t) onRedraw_ptr, HEX);
    #endif
  #endif
  return 0xFF;
}

void ScreenRef::setScreen(onRedraw_func_t onRedraw_ptr) {
  uint8_t type = lookupScreen(onRedraw_ptr);
  if(type != 0xFF) {
    setType(type);
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined(SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLLNPAIR("New screen: ",type);
      #else
      Serial.print("New screen: ");
      Serial.println(type);
      #endif
    #endif
    return;
  }
  #if defined(UI_FRAMEWORK_DEBUG)
    #if defined(SERIAL_PROTOCOLLNPAIR)
    SERIAL_PROTOCOLLNPAIR("Screen not found: ", (uintptr_t) onRedraw_ptr);
    #else
    Serial.print("Screen not found: ");
    Serial.println((uintptr_t) onRedraw_ptr, HEX);
    #endif
  #endif
}

void ScreenRef::initializeAll() {
  for(uint8_t type = 0; type < functionTableSize; type++) {
    GET_METHOD(type, onStartup)();
  }
}

/********************** SCREEN STACK  ******************************/

void ScreenStack::start() {
  initializeAll();
  onEntry();
}

void ScreenStack::push() {
  stack[3] = stack[2];
  stack[2] = stack[1];
  stack[1] = stack[0];
  stack[0] = getType();
}

void ScreenStack::pop() {
  setType(stack[0]);
  forget();
}

void ScreenStack::forget() {
  stack[0] = stack[1];
  stack[1] = stack[2];
  stack[2] = stack[3];
  stack[3] = 0;
}

void ScreenStack::goTo(onRedraw_func_t s) {
  push();
  onExit();
  setScreen(s);
  onEntry();
}

void ScreenStack::goBack() {
  pop();
  onEntry();
}

ScreenStack current_screen;

#endif // USE_UI_FRAMEWORK