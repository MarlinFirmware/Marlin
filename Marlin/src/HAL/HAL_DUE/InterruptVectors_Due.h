/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * InterruptVectors_Due.h
 *
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 *
 * This module relocates the Interrupt vector table to SRAM, allowing new
 * interrupt handlers to be added at runtime. This is required because the
 * Arduino runtime steals interrupt handlers that Marlin MUST use to support
 * extended functionality such as USB hosts and USB devices (MSD, MTP) and
 * custom serial port handlers. Rather than modifying and/or recompiling the
 * Arduino runtime, We just want to run as much as possible on Stock Arduino.
 *
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 */

#ifdef ARDUINO_ARCH_SAM

// ISR handler type
typedef void (*pfnISR_Handler)(void);

// Install a new interrupt vector handler for the given irq, returning the old one
pfnISR_Handler install_isr(IRQn_Type irq, pfnISR_Handler newHandler);

#endif // ARDUINO_ARCH_SAM
