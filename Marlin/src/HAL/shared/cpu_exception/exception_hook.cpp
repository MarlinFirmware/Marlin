/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "exception_hook.h"

void * __attribute__((weak)) hook_get_hardfault_vector_address(unsigned) { return 0; }
void * __attribute__((weak)) hook_get_memfault_vector_address(unsigned) { return 0; }
void * __attribute__((weak)) hook_get_busfault_vector_address(unsigned) { return 0; }
void * __attribute__((weak)) hook_get_usagefault_vector_address(unsigned) { return 0; }
void __attribute__((weak)) hook_last_resort_func() {}
