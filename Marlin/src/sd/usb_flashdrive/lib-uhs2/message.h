/**
 * Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact information
 * -------------------
 *
 * Circuits At Home, LTD
 * Web      :  https://www.circuitsathome.com
 * e-mail   :  support@circuitsathome.com
 */
#pragma once

#ifndef _usb_h_
  #error "Never include message.h directly; include Usb.h instead"
#endif

extern int UsbDEBUGlvl;

void E_Notify(char const * msg, int lvl);
void E_Notify(uint8_t b, int lvl);
void E_NotifyStr(char const * msg, int lvl);
void E_Notifyc(char c, int lvl);

#ifdef DEBUG_USB_HOST
  #define Notify E_Notify
  #define NotifyStr E_NotifyStr
  #define Notifyc E_Notifyc
  void NotifyFailGetDevDescr(uint8_t reason);
  void NotifyFailSetDevTblEntry(uint8_t reason);
  void NotifyFailGetConfDescr(uint8_t reason);
  void NotifyFailSetConfDescr(uint8_t reason);
  void NotifyFailGetDevDescr();
  void NotifyFailSetDevTblEntry();
  void NotifyFailGetConfDescr();
  void NotifyFailSetConfDescr();
  void NotifyFailUnknownDevice(uint16_t VID, uint16_t PID);
  void NotifyFail(uint8_t rcode);
#else
  #define Notify(...) ((void)0)
  #define NotifyStr(...) ((void)0)
  #define Notifyc(...) ((void)0)
  #define NotifyFailGetDevDescr(...) ((void)0)
  #define NotifyFailSetDevTblEntry(...) ((void)0)
  #define NotifyFailGetConfDescr(...) ((void)0)
  #define NotifyFailGetDevDescr(...) ((void)0)
  #define NotifyFailSetDevTblEntry(...) ((void)0)
  #define NotifyFailGetConfDescr(...) ((void)0)
  #define NotifyFailSetConfDescr(...) ((void)0)
  #define NotifyFailUnknownDevice(...) ((void)0)
  #define NotifyFail(...) ((void)0)
#endif

template <class ERROR_TYPE>
void ErrorMessage(uint8_t level, char const * msg, ERROR_TYPE rcode = 0) {
  #ifdef DEBUG_USB_HOST
    Notify(msg, level);
    Notify(PSTR(": "), level);
    D_PrintHex<ERROR_TYPE > (rcode, level);
    Notify(PSTR("\r\n"), level);
  #endif
}

template <class ERROR_TYPE>
void ErrorMessage(char const * msg __attribute__((unused)), ERROR_TYPE rcode __attribute__((unused)) = 0) {
  #ifdef DEBUG_USB_HOST
    Notify(msg, 0x80);
    Notify(PSTR(": "), 0x80);
    D_PrintHex<ERROR_TYPE > (rcode, 0x80);
    Notify(PSTR("\r\n"), 0x80);
  #endif
}
