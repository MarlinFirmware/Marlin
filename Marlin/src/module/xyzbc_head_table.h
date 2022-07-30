/**
 * Marlin2ForPipetBot [https://github.com/DerAndere1/Marlin]
 * Copyright 2022 DerAndere
 * 
 * Based on:
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#pragma once

/**
 * @file xyzbc_head_table.h
 * @author DerAndere
 * @brief Kinematics for 5 axis CNC machine in head-table configuration.
 * 
 * This machine has a tilting head (B axis parallel to the Y axis) and a horizontal rotary
 * table (C axis parallel to the Z axis). The user can use axes U, V, W to move in the rotated coordinate system.
 *
 * Inspired by: 
 * LinuxCNC maxkins.c (https://github.com/LinuxCNC/linuxcnc/blob/master/src/emc/kinematics/maxkins.c)
 * Copyright (c) 2007 Chris Radek
 */


#include "../core/types.h"

// Machine rotary zero point Z offset is the distance between the center of rotation of the B axis and Z home.
constexpr float mrzp_z_offset;

/**
 * xyzbc inverse kinematics
 *
 * Calculate the joints positions for a given machine
 * position, storing the result in the global delta[] array.
 *
 * This is an expensive calculation.
 */
void inverse_kinematics(const xyz_pos_t &raw);
