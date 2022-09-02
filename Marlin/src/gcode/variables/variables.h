/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2022 Carlon LaMont
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

/**
 * e_parser.cpp - Intercept special commands directly in the serial stream
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(Variable_Support)

#include "../gcode.h"
#include "../queue.h"
#include "../parser.h"

static int8_t get_var_from_command(const int8_t dval=0);
static uint8_t StoredVar;
static void input_var();
static int to_input_var(int8_t c) { return (int)c; };


static uint8_t L100();
static uint8_t L101();
static uint8_t L102();
static uint8_t L103();
static uint8_t L104();
static uint8_t L105();
static uint8_t L106();
static uint8_t L107();
static uint8_t L108();
static uint8_t L109();
static uint8_t L110();
static uint8_t L111();
static uint8_t L112();
static uint8_t L113();
static uint8_t L114();
static uint8_t L115();

#endif