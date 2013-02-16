/*

  dogmoi.c
  
  other information: value from  SPI Interface

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "dogm128.h"

uint8_t dog_GetSPIByte(void)
{
  uint8_t result;
  dog_spi_enable_client();
  dog_cmd_mode();
  result = dog_spi_out(0x0e3);	/* NOP command for UC1610 and ST7565 */
  dog_spi_disable_client();
  return result;
}

