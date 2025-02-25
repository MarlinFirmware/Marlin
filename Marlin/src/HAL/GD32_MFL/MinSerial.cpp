/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(POSTMORTEM_DEBUGGING)
#include "../shared/MinSerial.h"

#include "MinSerialHelper.h"

// isb
#define isb() __asm__ __volatile__ ("isb" : : : "memory")

// dsb
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")

#if WITHIN(SERIAL_PORT, 0, 4)
  rcu::RCU_PCLK pclk = clockRegs[SERIAL_PORT];
#endif

// Initializes the MinSerial interface.
// This function sets up the USART interface for serial communication.
// If the selected serial port is not a hardware port, it disables the severe error reporting feature.
static void MinSerialBegin() {
  #if !WITHIN(SERIAL_PORT, 0, 4)
    #warning "Using POSTMORTEM_DEBUGGING requires a physical U(S)ART hardware in case of severe error."
    #warning "Disabling the severe error reporting feature currently because the used serial port is not a HW port."
  #else
    const int nvicUART[] = { 37, 38, 39, 52, 53 };
    int nvicIndex = nvicUART[SERIAL_PORT];

    struct NVICMin {
      volatile uint32_t ISER[32];
      volatile uint32_t ICER[32];
    };

    NVICMin *nvicBase = (NVICMin*)0xE000E100;
    SBI32(nvicBase->ICER[nvicIndex >> 5], nvicIndex & 0x1F);

    // We require memory barriers to properly disable interrupts
    // (https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the)
    dsb();
    isb();

    // Disable then enable usart peripheral clocks
    rcu::RCU_DEVICE.set_pclk_enable(pclk, false);
    rcu::RCU_DEVICE.set_pclk_enable(pclk, true);

    uint32_t baudrate = register_read<uint32_t>(USART_Min_Regs::BAUD);
    register_write(USART_Min_Regs::CTL0, 0);  // reset the USART
    register_write(USART_Min_Regs::CTL1, 0);  // 1 stop bit

    // Recompute isn't needed since we don't touch the baudrate
    register_write(USART_Min_Regs::BAUD, baudrate);
    bit_write(USART_Min_Regs::CTL0, static_cast<uint32_t>(CTL0_bits::TEN), 1);
    bit_write(USART_Min_Regs::CTL0, static_cast<uint32_t>(CTL0_bits::UEN), 1);  // 8 bits, no parity, 1 stop bit (TEN | UEN)
  #endif
}

// Software memory barrier to prevent GCC overoptimization
#define sw_barrier() __asm__ volatile("": : :"memory");

// Writes a single character to the serial port.
static void MinSerialWrite(char c) {
  #if WITHIN(SERIAL_PORT, 0, 4)
    while (!(bit_read(USART_Min_Regs::STAT0, static_cast<uint32_t>(STAT0_bits::TBE)))) {
      hal.watchdog_refresh();
      sw_barrier();
    }
    register_write(USART_Min_Regs::DATA, c);
  #endif
}

// Installs the minimum serial interface.
// Sets the HAL_min_serial_init and HAL_min_serial_out function pointers to MinSerialBegin and MinSerialWrite respectively.
void install_min_serial() {
  HAL_min_serial_init = &MinSerialBegin;
  HAL_min_serial_out = &MinSerialWrite;
}

extern "C" {
  // A low-level assembly-based jump handler.
  // Unconditionally branches to the CommonHandler_ASM function.
  __attribute__((naked, aligned(4))) void JumpHandler_ASM() {
    __asm__ __volatile__ ("b CommonHandler_ASM\n");
  }
  void __attribute__((naked, alias("JumpHandler_ASM"), nothrow)) HardFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"), nothrow)) BusFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"), nothrow)) UsageFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"), nothrow)) MemManage_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"), nothrow)) NMI_Handler();
}

#endif // POSTMORTEM_DEBUGGING
#endif // ARDUINO_ARCH_MFL
