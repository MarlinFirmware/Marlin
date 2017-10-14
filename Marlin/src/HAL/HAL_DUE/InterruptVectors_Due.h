/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * InterruptVectors_Due.h - This module relocates the Interrupt vector table to SRAM,
 *  allowing to register new interrupt handlers at runtime. Specially valuable and needed
 * because Arduino runtime allocates some interrupt handlers that we NEED to override to
 * properly support extended functionality, as for example, USB host or USB device (MSD, MTP)
 * and custom serial port handlers, and we don't actually want to modify and/or recompile the
 * Arduino runtime. We just want to run as much as possible on Stock Arduino
 *
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 */

#ifndef INTERRUPTVECTORS_DUE_H
  #define INTERRUPTVECTORS_DUE_H

  #include "../../inc/MarlinConfig.h"

  #ifdef ARDUINO_ARCH_SAM 

    /* The type of an ISR handler */
    typedef void (*pfnISR_Handler)(void);

    /**
     * Installs a new interrupt vector handler for the given irq, and returns the old one
     */
    pfnISR_Handler install_isr(IRQn_Type irq, pfnISR_Handler newHandler);

  #endif
#endif