/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PASSWORD_FEATURE)

#include "password.h"
#include "../../gcode/gcode.h"
#include "../../core/serial.h"

Password password;

// public:
bool     Password::is_set, Password::is_locked;
uint32_t Password::value, Password::value_entry;

//
// Authenticate user with password.
// Called from Setup, after SD Prinitng Stops/Aborts, and M510
//
void Password::lock_machine() {
  is_locked = true;
  TERN_(HAS_LCD_MENU, authenticate_user(ui.status_screen, screen_password_entry));
}

//
// Authentication check
//
void Password::authentication_check() {
  if (value_entry == value)
    is_locked = false;
  else
    SERIAL_ECHOLNPGM(STR_WRONG_PASSWORD);

  TERN_(HAS_LCD_MENU, authentication_done());
}

#endif // PASSWORD_FEATURE
