/*

  dogmsysgr.c

  (c) 2011 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 Arduino library.

  The dogm128 Arduino library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 Arduino library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.


*/

#include "dogm128.h"

/*
  control the voltage RMS separation between the two gray shade levels
  val = 0: 24%
  val = 1: 29%
  val = 2: 36%
  val = 3: 40%
*/


void dog_SetUC1610GrayShade(uint8_t val)
{
#if defined(DOGXL160_HW_BW) || defined(DOGXL160_HW_GR)
  val &= 3;
  dog_spi_enable_client();  
  dog_cmd_mode();
  dog_spi_out(0x0d0 | val);		/* set RMS gray level separation value */
  dog_data_mode();
  dog_spi_disable_client();
#endif
}


