/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/* This file is included after Configuration.h and Configuration_adv.h 
 * Keep the Configuration.h as is and undef or define customizations 
 * here.
 * Include this file by setting the CUSTOM_CONFIG_FILE define in 
 * platformio_user.ini
 * 
 * If you have multiple boards or configurations, like one for testing
 * that disables from safety features and another one for production,
 * you can have a separate Configuration_***.h file for each, and use
 * platformio_user.ini to define multiple envs / build configurations
 *
 * NOTE: it would be nice if this file was included before Configuration.h
 * and Configuration_adv.h, but then we should make sure that any #define
 * in those files is surrounded by #ifndef/#endif
 */

#undef  STRING_CONFIG_H_AUTHOR
#define STRING_CONFIG_H_AUTHOR "(none, --user config)"

#error This is an example