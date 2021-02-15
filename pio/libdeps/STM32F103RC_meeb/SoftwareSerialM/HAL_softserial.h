/**
 * FYSETC
 *
 * Copyright (c) 2019 SoftwareSerialM [https://github.com/FYSETC/SoftwareSerialM]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_STM32F1
  #include "HAL_softserial_STM32F1.h"
#elif HAL_SS_PLATFORM == HAL_PLATFORM_STM32
  #include "HAL_softserial_STM32.h"
#elif HAL_SS_PLATFORM == HAL_PLATFORM_SAMD51
  #include "HAL_softserial_SAMD51.h"
#else
  #error "Unsupported Platform!"
#endif

void HAL_softSerial_init();
void HAL_softserial_setSpeed(uint32_t speed);
