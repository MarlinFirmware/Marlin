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
 * Web      :  http://www.circuitsathome.com
 * e-mail   :  support@circuitsathome.com
 */
#pragma once

#ifndef _usb_h_
  #error "Never include printhex.h directly; include Usb.h instead"
#endif

void E_Notifyc(char c, int lvl);

template <class T>
void PrintHex(T val, int lvl) {
  int num_nibbles = sizeof (T) * 2;
  do {
    char v = 48 + (((val >> (num_nibbles - 1) * 4)) & 0x0f);
    if (v > 57) v += 7;
    E_Notifyc(v, lvl);
  } while (--num_nibbles);
}

template <class T>
void PrintBin(T val, int lvl) {
  for (T mask = (((T)1) << ((sizeof (T) << 3) - 1)); mask; mask >>= 1)
    E_Notifyc(val & mask ? '1' : '0', lvl);
}

template <class T>
void SerialPrintHex(T val) {
  int num_nibbles = sizeof (T) * 2;
  do {
    char v = 48 + (((val >> (num_nibbles - 1) * 4)) & 0x0f);
    if (v > 57) v += 7;
    USB_HOST_SERIAL.print(v);
  } while (--num_nibbles);
}

template <class T>
void PrintHex2(Print *prn, T val) {
  T mask = (((T)1) << (((sizeof (T) << 1) - 1) << 2));
  while (mask > 1) {
    if (val < mask) prn->print("0");
    mask >>= 4;
  }
  prn->print((T)val, HEX);
}

template <class T> void D_PrintHex(T val __attribute__((unused)), int lvl __attribute__((unused))) {
  #ifdef DEBUG_USB_HOST
    PrintHex<T > (val, lvl);
  #endif
}

template <class T>
void D_PrintBin(T val, int lvl) {
  #ifdef DEBUG_USB_HOST
    PrintBin<T > (val, lvl);
  #endif
}
