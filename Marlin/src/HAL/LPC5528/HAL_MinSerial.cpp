/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../platforms.h"

#include "../../inc/MarlinConfigPre.h"

#ifdef TARGET_LPC5528

#if ENABLED(POSTMORTEM_DEBUGGING)

  #include "../shared/HAL_MinSerial.h"
  #include "watchdog.h"

  /* Instruction Synchronization Barrier */
  #define isb() __asm__ __volatile__ ("isb" : : : "memory")

  /* Data Synchronization Barrier */
  #define dsb() __asm__ __volatile__ ("dsb" : : : "memory")

  void install_min_serial() {
    HAL_min_serial_init = &TXBegin;
    HAL_min_serial_out = &TX;
  }

  extern "C" {
    __attribute__((naked)) void JumpHandler_ASM() {
      __asm__ __volatile__ (
        "b CommonHandler_ASM\n"
      );
    }
    void __attribute__((naked, alias("JumpHandler_ASM"))) HardFault_Handler();
    void __attribute__((naked, alias("JumpHandler_ASM"))) BusFault_Handler();
    void __attribute__((naked, alias("JumpHandler_ASM"))) UsageFault_Handler();
    void __attribute__((naked, alias("JumpHandler_ASM"))) MemManage_Handler();
    void __attribute__((naked, alias("JumpHandler_ASM"))) NMI_Handler();
  }
#endif  // POSTMORTEM_DEBUGGING
#endif  // TARGET_LPC5528
