/*************************
 * command_processor.cpp *
 *************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018                                        *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "ftdi_extended.h"

#if ENABLED(FTDI_EXTENDED)

CommandProcessor::btn_style_func_t  *CommandProcessor::_btn_style_callback = CommandProcessor::default_button_style_func;
bool CommandProcessor::is_tracking = false;

uint32_t CommandProcessor::memcrc(uint32_t ptr, uint32_t num) {
  const uint16_t x = CLCD::mem_read_16(CLCD::REG::CMD_WRITE);
  memcrc(ptr, num, 0);
  wait();
  return CLCD::mem_read_32(CLCD::MAP::RAM_CMD + x + 12);
}

bool CommandProcessor::wait() {
  while (is_processing() && !has_fault()) { /* nada */ }
  return !has_fault();
}

#endif // FTDI_EXTENDED
