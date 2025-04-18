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

// Base addresses for USART peripherals
static constexpr uintptr_t USART_base[] = {
  0x40013800, // USART0
  0x40004400, // USART1
  0x40004800, // USART2
  0x40004C00, // UART3
  0x40005000  // UART4
};

// Register offsets
static constexpr uint32_t STAT0_OFFSET = 0x00U;
static constexpr uint32_t DATA_OFFSET = 0x04U;
static constexpr uint32_t BAUD_OFFSET = 0x08U;
static constexpr uint32_t CTL0_OFFSET = 0x0CU;
static constexpr uint32_t CTL1_OFFSET = 0x14U;

// Bit positions
static constexpr uint32_t TBE_BIT = 7;
static constexpr uint32_t TEN_BIT = 3;
static constexpr uint32_t UEN_BIT = 13;

// NVIC interrupt numbers for USART
static constexpr int nvicUART[] = { 37, 38, 39, 52, 53 };

// RCU PCLK values for USART
static constexpr rcu::RCU_PCLK clockRegs[] = {
  rcu::RCU_PCLK::PCLK_USART0,
  rcu::RCU_PCLK::PCLK_USART1,
  rcu::RCU_PCLK::PCLK_USART2,
  rcu::RCU_PCLK::PCLK_UART3,
  rcu::RCU_PCLK::PCLK_UART4
};

// Memory barrier instructions
#define isb() __asm__ __volatile__ ("isb" : : : "memory")
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#define sw_barrier() __asm__ volatile("" : : : "memory")

// Direct register access macros
#define USART_REG(offset) (*(volatile uint32_t*)(USART_base[SERIAL_PORT] + (offset)))
#define USART_STAT0 USART_REG(STAT0_OFFSET)
#define USART_DATA  USART_REG(DATA_OFFSET)
#define USART_BAUD  USART_REG(BAUD_OFFSET)
#define USART_CTL0  USART_REG(CTL0_OFFSET)
#define USART_CTL1  USART_REG(CTL1_OFFSET)

// Bit manipulation macros
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 1U)
#define SET_BIT(reg, bit) ((reg) |= (1U << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1U << (bit)))

// Initializes the MinSerial interface.
// This function sets up the USART interface for serial communication.
// If the selected serial port is not a hardware port, it disables the severe error reporting feature.
static void MinSerialBegin() {
  #if !WITHIN(SERIAL_PORT, 0, 4)
    #warning "Using POSTMORTEM_DEBUGGING requires a physical U(S)ART hardware in case of severe error."
    #warning "Disabling the severe error reporting feature currently because the used serial port is not a HW port."
  #else
    int nvicIndex = nvicUART[SERIAL_PORT];

    // NVIC base address for interrupt disable
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

    // Get the RCU PCLK for this USART
    rcu::RCU_PCLK pclk = clockRegs[SERIAL_PORT];

    // Disable then enable usart peripheral clocks
    rcu::RCU_DEVICE.set_pclk_enable(pclk, false);
    rcu::RCU_DEVICE.set_pclk_enable(pclk, true);

    // Save current baudrate
    uint32_t baudrate = USART_BAUD;

    // Reset USART control registers
    USART_CTL0 = 0;
    USART_CTL1 = 0; // 1 stop bit

    // Restore baudrate
    USART_BAUD = baudrate;

    // Enable transmitter and USART (8 bits, no parity, 1 stop bit)
    SET_BIT(USART_CTL0, TEN_BIT);
    SET_BIT(USART_CTL0, UEN_BIT);
  #endif
}

// Writes a single character to the serial port.
static void MinSerialWrite(char c) {
  #if WITHIN(SERIAL_PORT, 0, 4)
    // Wait until transmit buffer is empty
    while (!READ_BIT(USART_STAT0, TBE_BIT)) {
      hal.watchdog_refresh();
      sw_barrier();
    }
    // Write character to data register
    USART_DATA = c;
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
